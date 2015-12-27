#include "ssnp/config.h"
#if NODE == TBC
#include <includes.h>
#include <math.h>
#include "ssnp/ssnp_for_app.h"

#include "ssnp/ssnp.h"
#include "ssnp/debug.h"
#include "ssnp/TBC.h"
#include "upcomputer.h"
#include "ssnp/teds.h"
#include "ssnp/ethbcp.h"

#include "i2c_fram.h"
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/*******************************************************************************
 *
 *   注意初始化顺序    
 *
 *
 *******************************************************************************/
static sys_mbox_t streaming_data_recv_mbox;//TBC需要处理的消息队列，这一队列用于流数据存储
static sys_mbox_t cmd_reply_recv_mbox;
sys_mbox_t upcomputer_cmd_recv_mbox;//TBC需要处理的消息队列，这一队列用于流数据存储

extern struct TBC_ tbc;
#define SEND_DISMSG_NUM         	6
#define SEND_DISMSG_INTERVSL    	500
#define RCV_MSG_TIME            	100+6// 需要测量的值：接收一次数据并且完全处理完成所需要的时间,比如推送需要的时间6us
#define SEND_MSG_TIME           	100
#define SYN_INTERVAL            	51400//同步时间长度
#define ASYN_INTERVAL           	48600//异步时间长度,异步时间长度这样计算是可以的，因为TBC不需要严格的按照时间片执行，只需要分清同步异步界限就可以了
#define DELAY_TIME_MEASURE_NUM    100
#define SYNC_PEROID               20000000 // 同步周期120s
#define EPOCH_PEROID              10000000      // epoch 周期
#define EPOCH_LEN                 1000   //每个epoch长度
#define SAMPLE_PEROID              100     //采样周期
#define START_EPOCH                50000   //开始发送epoch消息
#define START_EPOCH_AUX            	16
extern unsigned int delayTime[];
typedef enum
{
	FREE=0,
	SYN,
	ASYN
}state;

static OS_STK AppSSNPTaskStk[APP_TASK_SSNP_STK_SIZE];//ssnp应用程序主线程栈大小
static void uctsk_SSNP_TBC(void* pdata); 
static void uctsk_SSNP_TBC_new(void* pdata);
void  App_SSNPTaskCreate (void)//ssnp应用线程
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_SSNP_TBC_new,				
                          (void          * ) 0,							
                          (OS_STK        * )&AppSSNPTaskStk[APP_TASK_SSNP_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_SSNP_PRIO  );							
}
static void Delay(u16t time)
{ 
	OSTimeDly(time/(1000/OS_TICKS_PER_SEC));
}
static void TIM5_Init()
{
	TIM_TimeBaseInitTypeDef Tim5;  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  
  Tim5.TIM_Period=0xffff;
	Tim5.TIM_Prescaler=71;
	Tim5.TIM_ClockDivision=0;
 
	Tim5.TIM_CounterMode=TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM5,&Tim5);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
}  
static void TIM5_Init_Slot()
{	
	TIM_TimeBaseInitTypeDef Tim5; 
	
//	interrupt5 = 1;
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  
  	Tim5.TIM_Period= TIME_PEROID;
	Tim5.TIM_Prescaler=71;
	Tim5.TIM_ClockDivision=0;
	Tim5.TIM_CounterMode=TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM5,&Tim5);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
}
static void TIM5_delay_us(u16t us)
{
	TIM_SetCounter(TIM5,us);
	while(us > 1)
		us=TIM_GetCounter(TIM5);
}
static void NVIC_Configuration_TIM5(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the ETH Interrupt 在这里使能以太网中断*/
   	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void app_init()
{	
	streaming_data_recv_mbox=sys_mbox_new(MAX_QUEUE_ENTRIES);//流数据接收
	if(streaming_data_recv_mbox == SYS_MBOX_NULL)
		printf("app_init():error,streaming_data_recv_mbox is NULL.\r\n");
	cmd_reply_recv_mbox=sys_mbox_new(MAX_QUEUE_ENTRIES);
	if(cmd_reply_recv_mbox == SYS_MBOX_NULL)
		printf("app_init():error,cmd_reply_recv_mbox is NULL.\r\n");
	upcomputer_cmd_recv_mbox=sys_mbox_new(MAX_QUEUE_ENTRIES);
	if(upcomputer_cmd_recv_mbox == SYS_MBOX_NULL)
		printf("app_init():error,upcomputer_cmd_recv_mbox is NULL.\r\n");
	TIM5_Init();
	TBC_init(streaming_data_recv_mbox,cmd_reply_recv_mbox);
	init_queue();
}




/*
----------------------------------------------------------------------------------
	测试使用的变量。
----------------------------------------------------------------------------------
*/
u32t recv_num=0;//接收到的数据量
u32t syn_num=0;	//同步周期的个数
u32t asyn_num=0;//异步周期的个数
 //-----------测试用变量。--------------------


void send_cmd(u8t alias,u8t tdcn)
{
	TBC_send_noArgucmd(alias,tdcn,INITIALIZATION,SLEEP);
	printf("ok\r\n");
}
int get_up_cmd;
u8t get_up_cmd_alias;
u8t interrupt;
u8t get_up_cmd_tdcn;
extern  u16 buf_tail;
typedef union BL
{
    char ch[4];
	int i;
} BL;
BL b;
extern u8t tbimAlias;
void delayMeasure()
{	int i,k;
	for(i=0;i<MAX_TBIM_NUM;i++)
	{ 
		if(tbc.tbim_table[i].alias != 0)
		{
			//计时	 
			set_reflect_reply_unfinished();
			for(k=0;k<DELAY_TIME_MEASURE_NUM;k++)  //只是用来测量时延
			{
			//	printf("uctsk_SSNP():TBC send reflect msg to tbim %d.\r\n",tbc.tbim_table[i].alias);
			//	time_f=TIM_GetCounter(TIM5);
			//	printf("time_f = %d\r\n", time_f);
				
				tbc_send_reflect_msg(tbc.tbim_table[i].alias);
			//	printf("******\r\n");
			    tbimAlias = tbc.tbim_table[i].alias;
				while(!is_reflect_reply_ok());
				
			//	time_n=TIM_GetCounter(TIM5);
			//	time = time_f > time_n ? time_f - time_n : 0xffff - time_n + time_f;
			//	delayTime[k]=time;	
			//	printf("uctsk_SSNP():time_f=%d,time_n=%d,the delay time is %d.\r\n",time_f,time_n,time);
		 	
				set_reflect_reply_unfinished();
			}
			//#未完成#：这里应该将时间差发送回去
		//	averageTime=average_time(delay_time,DELAY_TIME_MEASURE_NUM);
			//printf("uctsk_SSNP():the average time is %d.\r\n",averageTime);
		//	printf("delayTime  = %d\r\n", delayTime[2]);
		//	dTime = avertime_new(tbc.tbim_table[i].uuid,delayTime,DELAY_TIME_MEASURE_NUM);
		//printf("uctsk_SSNP():get delay to TBIM%d done. averange time = %d\r\n",tbc.tbim_table[i].alias, dTime);
	//	printf("uctsk_SSNP():tbimDelayTime =%d  \r\n", tbimDelayTime);
		tbc_send_reflect_delay_to_tbim(tbc.tbim_table[i].alias, tbimDelayTime);
		}
	}
}


u8 timeArr[] = {23, 32};
u8 flag = 1;
volatile int timeSlot  = 0;
extern unsigned int tbimDelayTime;
u16 timePrev = 0;
u16 timeCur = 0;
u16 distance = 0;
unsigned int timeSync = 0;
int timeEpoch = 0;
int epochPrev = 0;
int epochCur = 0;
int epochBegin = 0;
int auxEpoch = 0;
int epochDis = 0;
u16 sampData[1000];
//int timerValue[50];
u16 indexSamp = 0;

extern u8t flagBeginEpoch;
u16 timeValue[100];
u16 sampValue[100];
static void uctsk_SSNP_TBC_new(void* pdata)
{
	struct tbc_msg *msg;
	u8t n_send_dismsg;
	void* tbim_list;
	state st;
	int i;
	int j;
	int k;
	

	u16t time_f;
	u16t time_n;
	u32t time;

	u16t aaaa;
	u16t bbbb;
	u32t syn_total_time;

	u16t index;
	int startFactor = 0;
	u8t m;
	u8t ret;
	u16 startTime = 0;
	u16 startEpochDis = 0;
	struct cmd_item item;
  //-----------测试用变量。--------------------

   //---------------用来测试EEPROM------------


	//----------------------------------------
   u8t test_alias;
   u16 readData;
   u16 samp;
   
   u16t test_time_index;
   u16t t_i; 
   u8t is_test;
   u8t cmdnum;
   u16* upcomputer_cmd;
 //  u8 temp_buf[] = {4,5,10,0,6,0,13,1,2,3,4,5};
   u8 temp_len = 12;
  //-------------------------------
	float f = 1234.5678;
	u32t averageTime;
	u32t* data;
	struct netbuf* buf;
		unsigned int dTime;
			u8 flagBeginEpoch = 1;

//	char strbuf[256];
	syn_total_time=0; 	
	tbim_list=NULL;
	st=FREE;
	msg=NULL;
	aaaa=0;
	index=0;
	upcomputer_cmd=NULL;




	
  //-----------测试用变量。--------------------
   test_time_index=0;
   is_test=0;
   cmdnum=0;

      
   
  
   
  //-------------------------------	
	hardware_init();
	
	protocol_init();
	//	   sprintf(strbuf, "f = %0.4f\r\n", 0.5);
	printf("\r\n\r\n\r\nTBC init ok.\r\n");

	app_init();

    printf("f = %f\r\n", f);
	interrupt ++;

	printf("\r\nTest for writing and reading EEPROM\r\n");

	

#if	 1  

//	startWork();
 //   while (1){}
/*	while (1) 
	{
	      if(buf_tail > 5)
		      printf("hello world\r\n");
	      for (i = 0; i < 65500; i ++)
		      for (j = 0; j < 650; j ++);
			  //    for (k = 0; k < 6788; k ++);
	  printf("buf_tail = %d\r\n",buf_tail);
	}
*/
/*	ret = 0;
    ret = I2C_FRAM_BufferRead(array, 40, 40);
	if (ret)
	    printf("\r\nread success\r\n");
	for (i = 0; i < 40; i ++)
	{
	    printf("array[%d] = %d\r\n",i,array[i]);
	}
  */
/*	for (i = 0; i < 300; i ++)
	{
	    array[i] = i; 
		printf("array[%d] = %d\r\n",i,array[i]);
	}
	printf("\r\narray[0] = %d\r\n",array[0]);
	printf("\r\narray[99] = %d\r\n",array[99]);

	ret = 0;
    ret = I2C_FRAM_BufferWrite(array,0x0000, 300);
//	I2C_BufferWrite(array,0x0000, 300);
	if (ret) 
	    printf("\r\nwrite sucess\r\n");
	printf("\r\nwrite doner\n");
	for (i = 0; i < 300; i ++)
	{
	    array[i] = 0;
	}

    printf("\r\narray[0] = %d\r\n",array[0]);
	printf("\r\narray[19] = %d\r\n",array[99]);

	ret = 0;
    ret = I2C_FRAM_BufferRead(array, 0x0000, 300);
	if (ret)
	    printf("\r\nread success\r\n");
	for (i = 0; i < 300; i ++)
	{
	    printf("array[%d] = %d\r\n",i,array[i]);
	}
*/
//	writeTedsTableTest();
//    write_test();
	for(n_send_dismsg=0;n_send_dismsg<SEND_DISMSG_NUM;n_send_dismsg++)
	{   printf("send discovery msg\r\n");
		send_discovery_msg();//这里能直接调用吗？因为内核和这里都使用了netif，会不会出错？但是这里应该是线程安全的，因为对于netif都是只读操作
		Delay(SEND_DISMSG_INTERVSL);
	}
	//识别TBIM
	SSNP_DEBUG_PRINT("uctsk_SSNP():get alias list.\r\n");
	tbim_list=get_tbim();//#未完成#：这里的get_tbim()函数需要修改一下
	if(tbim_list)
	{
		SSNP_DEBUG_PRINT("uctsk_SSNP():recongizing TBIM.\r\n");
		printf("------------------------------------------\r\n");
		TBIM_recongnition(tbim_list);
		printf("------------------------------------------\r\n");
	}
	SSNP_DEBUG_PRINT("uctsk_SSNP():recongize TBIM done.\r\n");
	for(i=0;i<MAX_TBIM_NUM;i++)
	{ 
		if(tbc.tbim_table[i].alias != 0)
		{
			//计时	 
			set_reflect_reply_unfinished();
			for(k=0;k<DELAY_TIME_MEASURE_NUM;k++)  //只是用来测量时延
			{
			//	printf("uctsk_SSNP():TBC send reflect msg to tbim %d.\r\n",tbc.tbim_table[i].alias);
			//	time_f=TIM_GetCounter(TIM5);
			//	printf("time_f = %d\r\n", time_f);
				
				tbc_send_reflect_msg(tbc.tbim_table[i].alias);
			//	printf("******\r\n");
				while(!is_reflect_reply_ok());
				
			//	time_n=TIM_GetCounter(TIM5);
			//	time = time_f > time_n ? time_f - time_n : 0xffff - time_n + time_f;
			//	delayTime[k]=time;	
			//	printf("uctsk_SSNP():time_f=%d,time_n=%d,the delay time is %d.\r\n",time_f,time_n,time);
		 	
				set_reflect_reply_unfinished();
			}
			//#未完成#：这里应该将时间差发送回去
		//	averageTime=average_time(delay_time,DELAY_TIME_MEASURE_NUM);
			//printf("uctsk_SSNP():the average time is %d.\r\n",averageTime);
		//	printf("delayTime  = %d\r\n", delayTime[2]);
		//	dTime = avertime_new(tbc.tbim_table[i].uuid,delayTime,DELAY_TIME_MEASURE_NUM);
		//printf("uctsk_SSNP():get delay to TBIM%d done. averange time = %d\r\n",tbc.tbim_table[i].alias, dTime);
	//	printf("uctsk_SSNP():tbimDelayTime =%d  \r\n", tbimDelayTime);
		tbc_send_reflect_delay_to_tbim(tbc.tbim_table[i].alias, tbimDelayTime);
		}
	}
/**/
/*//	printf("uctsk_SSNP():send time slot assign msg.\r\n");
	set_syn_asyn_num(64,1);
	get_epoch();
	//tbc_send_define_epoch_msg(SYN_TIMESLOT_NUM + 2,tbc.min_period*10 - SYN_TIMESLOT_NUM -2);//同步时间预留出两个时间槽
	tbc_send_define_epoch_msg(SYN_TIMESLOT_NUM + 2,100*10 - SYN_TIMESLOT_NUM -2);//同步时间预留出两个时间槽
	printf("uctsk_SSNP():send define epoch msg done.\r\n");
	timeslot_alloc();//分配时间槽,并将时间槽分配信息发送出去
	printf("uctsk_SSNP():send time slot alloc msg done. **\r\n");

	time_f=TIM_GetCounter(TIM5);//计时开始
	st=SYN;
	NVIC_Configuration_TIM5();
	
 */	
 #endif
	TIM_Cmd(TIM5,DISABLE);
	NVIC_Configuration_TIM5();
   // tbc_send_begin_of_epoch_msg();//发送开始标识
				TIM5_Init_Slot();
				tbc_send_sync_time_msg();
				startTime = TIM_GetCounter(TIM5);
			    tbc_send_follow_up_msg(startTime);
			//	tbc_send_begin_of_epoch_msg();//发送开始标识
	//	printf("TIM5 time = %d\r\n", TIM_GetCounter(TIM5));
			//	printf("TIM5 time = %d\r\n", TIM_GetCounter(TIM5));
			//	printf("TIM5 time = %d\r\n", TIM_GetCounter(TIM5));
			//	printf("TIM5 time = %d\r\n", TIM_GetCounter(TIM5));
 
	//	   NVIC_Configuration_TIM5();
		 //  interrupt ++;
		  // printf("hello \r\n");\
		  

		  // printf("hello \r\n");
		   
		  // tbc_send_begin_of_epoch_msg();
		   timePrev = startTime;
		  // epochBegin = timePrev;
		  // epochPrev = timePrev;	 
		  tbc_send_begin_of_epoch_msg();
		   flagBeginEpoch = 1;
		    while(1)
			 {
		#if 1
				timeCur = TIM_GetCounter(TIM5);
				distance = (timeCur < timePrev) ? timePrev - timeCur : 0xffff - timeCur + timePrev;
				startEpochDis = (timeCur < startTime) ? startTime - timeCur : 0xffff - timeCur + startTime;
				//printf("startEpochDis = %d \r\n", startEpochDis);
				if (startEpochDis >= START_EPOCH && flagBeginEpoch == 1)
				 {//	printf("&&&&&&&&\r\n");
				 /**/   startFactor ++;
					startTime = timeCur;
					if (startFactor == START_EPOCH_AUX)	 
					{
						tbc_send_sync_time_msg();
			        	startTime = TIM_GetCounter(TIM5);
			            tbc_send_follow_up_msg(startTime);
			 	   	//	tbc_send_begin_of_epoch_msg();
		  				epochBegin = TIM_GetCounter(TIM5);
		 				epochPrev = epochBegin;
						flagBeginEpoch = 1;
						startFactor = 0;
						printf("send sync message\r\n");
					}
				}
			    
				timeSync += distance;
				timeEpoch += distance;
				timePrev = timeCur;
				epochCur = TIM_GetCounter(TIM5);
				epochDis = (epochCur < epochPrev) ? epochPrev - epochCur : 0xffff - epochCur + epochPrev;
				if (epochDis >= EPOCH_LEN)
				{	while (epochDis >= 2 * EPOCH_LEN )
						epochDis -= EPOCH_LEN;
					auxEpoch = epochDis - EPOCH_LEN;
				    epochPrev = (epochCur + auxEpoch) % 0xffff;	//downflow
				//	epochPrev = epochCur - auxEpoch;
				//	timerValue[indexSamp] = TIM_GetCounter(TIM5);
			/*		readData = GPIO_ReadInputData(GPIOE);
	            	samp = (readData & 0x0f00) >> 8;
					timeValue[indexSamp] = 	 TIM_GetCounter(TIM5);
					sampData[indexSamp++] = samp;
				//	printf("samp = %d  TIM_GetCounter(TIM5) = %d\r\n", samp, TIM_GetCounter(TIM5));
					
					if (indexSamp == 100)
					{
					   for (i = 0; i < 1000; i ++)
					   printf("i = %d samp = %d timeValue = %d\r\n", i, sampData[i], timeValue[i]);
				
					}
			*/	}
			//	printf("timeCur = %d timeSync = %d timePrev = %d distance = %d \r\n", timeCur, timeSync, timePrev, distance);
				if (timeSync >= SYNC_PEROID)  //120ms 发送同步报文
				{	// printf("Send sync msg\r\n");
			/*		 tbc_send_sync_time_msg();
					 tbc_send_follow_up_msg(TIM_GetCounter(TIM5));
					 timeSync = 0;
			  */  }
				if (timeEpoch >= EPOCH_PEROID) 
				{
					 timeEpoch = 0;
			    }
	#endif
				if (flagMeasure)
				{
					flagMeasure = 0;
					delayMeasure();
				}
				buf=(struct netbuf*)sys_arch_mbox_fetch_unblock(cmd_reply_recv_mbox);
				//sys_arch_mbox_fetch(streaming_data_recv_mbox,(void**)&buf,0);					
				if(buf != NULL)
				{
					send_sdp_upcomputer(buf);
					netbuf_delete(buf);
					buf=NULL;
				}
				if(!isQempty())
				{
				   // printf("queue is not empty\r\n");
					item=deQueue(&ret);
				//	TBC_send_upcomputer_cmd(item);
				}
	
			}
		
	
}


void TIM5_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET )
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	}

}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
#endif
