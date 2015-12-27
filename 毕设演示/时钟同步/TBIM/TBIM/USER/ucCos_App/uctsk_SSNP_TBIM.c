#include "ssnp/config.h"
#if NODE == TBIM
#include <includes.h>
#include "ssnp/ssnp_for_app.h"
#include "ssnp/ssnp.h"
#include "ssnp/TBIM.h"

#include "ssnp/schedule_table.h"

#include "ssnp/debug.h"
#include "ssnp/mem.h"
#include "uctsk_SSNP_TBIM.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
#define TBC_ADDRESS          		  255
#define RECV_SEND_TIME        		200 //#未完成#：这里假设接收和回复命令需要使用200us
#define TIME_SLOT_LEN	        		100 //这里设定时间槽为100us
	
#define DELAY                  		50  //从TBC开始发送到TBIM应用层接收到的时延                   #测量#
#define TRIGGER_RECV_EXE_TIME  		100 //把这个时间定义为TBIM足够接收并执行一个trigger的时间     #测量#
#define CMD_RECV_EXE_TIME      		200 //把这个时间定义为TBIM足够接收名执行一个命令的时间        #测量#
#define DELAY_TIME_MEASURE_NUM    100
#define EPOCH_PEROID              1000      // epoch 周期
#define EPOCH_LEN                 1000   //每个epoch长度
#define SAMPLE_PEROID              100     //采样周期

#define SAMPLE_TIME 1
int sam_time = 0;

sys_sem_t sem;
#define SIGNAL sys_sem_signal(sem)//释放信号量

enum msg_type       
{
	DP,
	TP
};

typedef enum
{
	FREE=0,
	SYN,
	ASYN,
	WORKING
}state;



struct TBIM_msg
{
	enum msg_type type;
	struct netconn* conn;
	struct netbuf* buf;//只有dp数据才需要参数，tp不需要参数
	sys_sem_t op_completed;
};


//#define TBIM_MSG_NUM  10
//static OS_MEM* tbim_msg;
//static u8t tbim_msg_pool[TBIM_MSG_NUM * sizeof(struct TBIM_msg)];

static sys_mbox_t tbim_mbox;


extern  struct TBIM_ tbim;
/*
------------------------------------------------------------------------------------------------------------
	一定要注意这里的计时范围一次最多是65536us,也就是大概65ms,注意两个时间段不要超过65ms否则没有办法计时
------------------------------------------------------------------------------------------------------------
*/
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
u8t interrupt5 = 0;
static void TIM5_Init_Slot()
{	
	TIM_TimeBaseInitTypeDef Tim5; 
	
	//interrupt5 = 1;
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  
  	Tim5.TIM_Period= TIME_PEROID;
//	printf("Tim5.TIM_Period = %d\r\n", Tim5.TIM_Period);
	Tim5.TIM_Prescaler=71;
	Tim5.TIM_ClockDivision=0;
	Tim5.TIM_CounterMode=TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM5,&Tim5);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
} 

static void TIM4_Init()
{
	TIM_TimeBaseInitTypeDef Tim4;  
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
  	Tim4.TIM_Period= 99;
	Tim4.TIM_Prescaler= 7199;
	Tim4.TIM_ClockDivision=0;
	Tim4.TIM_CounterMode=TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM4,&Tim4);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
} 

static void NVIC_Configuration_TIM5(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the ETH Interrupt 在这里使能以太网中断*/
	  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void NVIC_Configuration_TIM4(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the ETH Interrupt 在这里使能以太网中断*/
	  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void TIM5_delay_us(u16t us)//使用定时器不用自己判断了
{
	TIM_SetCounter(TIM5,us);
	while(us > 1)
		us=TIM_GetCounter(TIM5);
}

void begin_epoch_callback(u16t syn_timeslot,u16t asyn_timeslot)
{
	///time_f=TIM_GetCounter(TIM5);//计时开始
}
static void app_init()
{
//	u8t err;
//	tbim_msg=OSMemCreate((void*)tbim_msg_pool,TBIM_MSG_NUM,sizeof(struct TBIM_msg),&err);
	
//	tbim_mbox=sys_mbox_new(MAX_QUEUE_ENTRIES);
	sem=sys_sem_new(0);
	TIM5_Init();
//	NVIC_Configuration_TIM5();
}

/***********************************************************************************************
 * 应用程序各个线程栈大小
 ***********************************************************************************************/

static OS_STK AppSSNPTaskStk[APP_TASK_SSNP_STK_SIZE];//ssnp应用程序主线程栈大小

static OS_STK AppSSNPdpTaskStk[APP_TASK_DP_STK_SIZE];//ssnp应用dp线程栈大小
static OS_STK AppSSNPtpTaskStk[APP_TASK_TP_STK_SIZE];//ssnp应用tp线程栈大小
static OS_STK AppSSNPsdpTaskStk[APP_TASK_SDP_STK_SIZE];//ssnp应用sdp线程栈大小


static void uctsk_SSNP_dp_recv(void* pdata)
{
	struct netconn* conn;
	struct netbuf* buf;
	struct TBIM_msg msg;
	u8t* alias;

	
	alias=pdata;
	msg.op_completed=sys_sem_new(0);
	
	conn=netconn_new(NETCONN_DP);
	netconn_bind(conn,*alias,0);//先将别名绑定到0
	netconn_connect(conn,TBC_ADDRESS,0);//TBC的别名总是1
	printf("uctsk_SSNP_dp_recv(): tbim alias is %d, tbc alias is %d.\r\n",*alias,TBC_ADDRESS);
	
	for(;;)
	{
		netconn_recv(conn,&buf);
		SSNP_DEBUG_PRINT("uctsk_SSNP_dp_recv(): app get dp data.\r\n");
		msg.type=DP;
		msg.buf=buf;
		msg.conn=conn;
		sys_mbox_post(tbim_mbox,&msg);
		SSNP_DEBUG_PRINT("uctsk_SSNP_dp_recv(): waiting for TBIM exe cmd.\r\n");
		sys_arch_sem_wait(msg.op_completed,0);
		SSNP_DEBUG_PRINT("uctsk_SSNP_dp_recv(): TBIM exe cmd done.\r\n");
		netbuf_delete(buf);

		OSTimeDly(1);
	}
}
static void uctsk_SSNP_tp_recv(void* pdata)
{
	struct netconn* conn;
	struct netbuf* buf;
	struct TBIM_msg msg;
	u8t* alias;
	
	alias=pdata;
	buf=NULL;
	msg.op_completed=sys_sem_new(0);
	
	conn=netconn_new(NETCONN_TP);
	netconn_bind(conn,*alias,0);
	netconn_connect(conn,TBC_ADDRESS,0);
	printf("uctsk_SSNP_tp_recv(): tbim alias is %d, tbc alias is %d.\r\n",*alias,TBC_ADDRESS);
	for(;;)
	{
		netconn_recvtrigger_buf(conn,&buf);
		msg.type=TP;
		msg.buf=buf;
		msg.conn=conn;
		sys_mbox_post(tbim_mbox,&msg);
		SSNP_DEBUG_PRINT("uctsk_SSNP_tp_recv(): waiting for TBIM exe trigger.\r\n");
		sys_arch_sem_wait(msg.op_completed,0);
		SSNP_DEBUG_PRINT("uctsk_SSNP_dp_recv(): TBIM exe done trigger.\r\n");
		netbuf_delete(buf);
		
		OSTimeDly(1);
	}
}
static void uctsk_SSNP_sdp(void* pdata)
{
//	send_sampling_data();
}
static void App_SSNP_DP_TaskCreate(u8t* alias)
{
	CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */
	
	os_err=OSTaskCreate((void (*)(void *)) uctsk_SSNP_dp_recv,	
		                   (void*          ) alias,
											 (OS_STK*        ) &AppSSNPdpTaskStk[APP_TASK_DP_STK_SIZE -1],
											 (INT8U          ) APP_TASK_SSNP_DP_PRIO);
}
static void App_SSNP_SDP_TaskCreate(u8t* alias)
{
	CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */
	
	os_err=OSTaskCreate((void (*)(void *)) uctsk_SSNP_sdp,	
		                   (void*          ) alias,
											 (OS_STK*        ) &AppSSNPsdpTaskStk[APP_TASK_SDP_STK_SIZE -1],
											 (INT8U          ) APP_TASK_SSNP_SDP_PRIO);
}
static void App_SSNP_TP_TaskCreate(u8t* alias)
{
	CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */
	
	os_err=OSTaskCreate((void (*)(void *)) uctsk_SSNP_tp_recv,	
		                   (void*          ) alias,
											 (OS_STK*        ) &AppSSNPtpTaskStk[APP_TASK_TP_STK_SIZE -1],
											 (INT8U          ) APP_TASK_SSNP_TP_PRIO);
}

static void uctsk_SSNP(void* pdata); 
static void uctsk_SSNP_TBIM(void* pdata); 
static void uctsk_SSNP_TBIM_new(void* pdata);
static void uctsk_SSNP_TBIM_new_new(void* pdata);
void  App_SSNPTaskCreate (void)//ssnp应用线程
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_SSNP_TBIM_new,				
                          (void          * ) 0,							
                          (OS_STK        * )&AppSSNPTaskStk[APP_TASK_SSNP_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_SSNP_PRIO  );							
}






u16t syn_time;//同步等待时间 (从同步开始到此TBIM第一个时间槽的时间长度)
u16t first_syn_delay_time;//同步等待时间去掉传输延迟(同步时间槽去掉延迟的时间长度)
u16t asyn_time;//异步时间间隔(从异步时间开始到结束)
u16t wait_for_asyn_time;//(当发送完所有的变送器数据后等待进入异步区间的时间，当然最后一个TBIM是不需要这个时间段的)
u16t time_h;//head
u16t time_t;//tail
u16t time;//时间间隔
u8t tdcn_sequence;//当前需要发送流数据的传感器序列号
u8t isfirst=1;
typedef void (*func)(void);
#define DEBUGING 0
static void first_syn_delay(void)
{
#if DEBUGING == 0
	if(isfirst)
	{
		TIM5_delay_us(first_syn_delay_time);
		isfirst=0;
	}
	else
		TIM5_delay_us(syn_time);
#endif
}
static void syn_delay(void)
{
#if DEBUGING == 0
	TIM5_delay_us(syn_time);
	//printf("syn_time():syn_time is %d.\r\n",syn_time);
#endif
}
static void time_slot_delay(void)
{
#if DEBUGING == 0
	TIM5_delay_us(TIME_SLOT_LEN);
#endif
}
static void wait_for_asyn(void)
{
#if DEBUGING == 0
	TIM5_delay_us(wait_for_asyn_time);
	//printf("wait_for_asyn():wait_for_asyn_time is %d.\r\n",wait_for_asyn_time);
#endif
}
u32t number=0;
u16t a;
u16t b;
u16t t;
static void send_streaming_date(void)
{
	u8t data[2];
	time_h=TIM_GetCounter(TIM5);
	 printf("send.\r\n");
	++number;	
	if(data != NULL)
	{
		TBIM_send_streaming_data(tdcn_sequence,&number);
	
		time_t=TIM_GetCounter(TIM5);
		time = time_h >= time_t ? time_h - time_t : 0xffff - time_t + time_h;
		TIM5_delay_us(TIME_SLOT_LEN - time);//发送完数据后在自己的时间槽中等待
	}
	else
	{
		time_t=TIM_GetCounter(TIM5);
		time = time_h >= time_t ? time_h - time_t : 0xffff - time_t + time_h;
		TIM5_delay_us(TIME_SLOT_LEN - time);							
	}

}

static void recv_exe_cmd(void)
{
#if DEBUGING == 0
//	u16t time_e;

	time_h=TIM_GetCounter(TIM5);
//	printf("recv_exe_cmd():time_h is %d.\r\n",time_h);
	while(1)
	{
		TBIM_recv_cmd_and_execute();	
		time_t=TIM_GetCounter(TIM5);
		time =time_h >= time_t ? time_h - time_t : 0xffff - time_t + time_h;
		if(time + CMD_RECV_EXE_TIME >= asyn_time)//如果剩余的时间不足以接收并执行一个trigger
		{
			TIM5_delay_us(asyn_time - time);//注意这里跳过了第0号时间槽
			break;
		}
		
		TBIM_recv_trigger_and_execute();	
		time_t=TIM_GetCounter(TIM5);
		time = time_h >= time_t ? time_h - time_t : 0xffff - time_t + time_h;
			
		if(time + TRIGGER_RECV_EXE_TIME >= asyn_time)//如果剩余的时间不足以接收并执行一个命令
		{
			TIM5_delay_us(asyn_time- time);//注意这里跳过了第0号时间槽
			break;
		}							
	}
//	time_e=TIM_GetCounter(TIM5);
//	printf("recv_exe_cmd():time_e is %d.\r\n",TIM_GetCounter(TIM5));
//  printf("recv_exe_cmd():the time tt %d,total is %d and is %d.\r\n",time,asyn_time- time,time_e >= time_h ? time_e - time_h : 0xffff - time_h + time_e);
#endif
}	
u8t* table;
int send_number=0; 
u8t isOk_now=0;
int be_gin;
int timeCurrent = 0;
extern int timeOffset;
int timeEpoch = 0;
extern int epochPrev;
int epochCur = 0;
int epochBegin = 0;
int auxEpoch = 0;
int epochDis = 0;
u16 sampData[5000];
//int timerValue[1000];
//u16 sampData1[1000];

u16 indexSamp = 0;
static int syncTime()
{
	return 	((TIM_GetCounter(TIM5) + timeOffset) % 0xffff);
}
static void uctsk_SSNP_TBIM_new(void* pdata)
{
	u8t alias;
	void* data;
	u16t syn_itv;
	u16t asyn_itv;
	u16t begin_timeslot;
	u8t k;
	int i;
	u8t j;
	state st;
	u8t Chn_num;
	u8t tdcn_sn;
	func** f_table;

	u8t func_table_row;
	u8t func_table_col;
	u8t factor;
    volatile u16 readData;
	u16 samp;
    float f = 1234.111111;

	
	

	hardware_init();
	protocol_init(); 
	printf("f = %0.6f\r\n", f);
	app_init();			   

	
	
	SSNP_DEBUG_PRINT("\r\nTBIM init ok.\r\n");
	data=NULL;
	st=FREE;
	
	SSNP_DEBUG_PRINT("waitting for alias.\r\n");	
	alias=get_alias();
	TBIM_init(alias);
	Chn_num=get_TBIM_Chn_num();
	printf("uctsk_SSNP():get alias %d.\r\n",alias);

	
	TBIM_recv_cmd_and_execute();

	//时间测量函数	
	   /*  set_reflect_ok_unfinished_TEST();
					SSNP_DEBUG_PRINT("waiting for reflect msg.\r\n");
				    //
					
					for(k=0;k<DELAY_TIME_MEASURE_NUM;k++)//这里只是用来测量时延
					{
						while(!is_reflect_ok());
						
						tbim_send_reflect_reply_msg();
						
						set_reflect_ok_unfinished_TEST();
						
					}
					SSNP_DEBUG_PRINT("send reflect reply msg done.\r\n");
		*/			TIM_Cmd(TIM5,DISABLE);
				//	NVIC_Configuration_TIM5();
				  //  TIM5_Init_Slot();
				//	while(!is_streaming_ok());
				//	printf("begin sampling\r\n");
		 
 // localScheduleTest();	

 //  TIM5_Init();
	
              //  TIM_Cmd(TIM5,ENABLE);
				interrupt5 = 1;
			//	printf("interrupt5 = %d tbimDelayTime %d\r\n", interrupt5, tbimDelayTime);
			//	TIM_SetCounter(TIM5, 9999 - tbimDelayTime);
				NVIC_Configuration_TIM5();
				TIM5_Init_Slot();
			    // NVIC_Configuration_TIM5();
				
				//	   epochPrev = TIM_GetCounter(TIM5);
				/**/ 
	
					while(1)
				   {    //printf("**************\r\n");
				       epochCur = syncTime();
			       	   epochDis = (epochCur < epochPrev) ? epochPrev - epochCur : 0xffff - epochCur + epochPrev;
					   //printf("%d %d %d\r\n",epochCur, epochPrev, epochDis);
			    	   if (flagBeginEpoch && epochDis >= EPOCH_LEN)
			           { //   printf("***%d %d %d\r\n",epochCur, epochPrev, epochDis);
					       while (epochDis >= 2 * EPOCH_LEN)
						       epochDis -= EPOCH_LEN;
				           auxEpoch = epochDis - EPOCH_LEN;
				           epochPrev = (epochCur + auxEpoch) % 0xffff;	//downflow
					//	   epochPrev = epochCur - auxEpoch;
					//	   timerValue[indexSamp] = syncTime();
						
						//   if (timerValue[indexSamp] > 0xffff) timerValue[indexSamp] -= 0xffff;
						   sam_time = (sam_time + 1) % 10;
						   if (sam_time == SAMPLE_TIME) {
						  			   
				           readData = GPIO_ReadInputData(GPIOE);
	                       samp = (readData & 0x0f00) >> 8;
					       sampData[indexSamp++] = samp;
						   //printf("indexSamp = %d samp = %d\r\n", indexSamp, samp);
						   sam_time = 0;
				           if (indexSamp == 5000)
				           {   
						   	   indexSamp = 0;
							   flagBeginEpoch = 0;
				               for (i = 0; i < 5000; i ++)
							 	  printf("i = %d samp = %d \r\n", i, sampData[i]);
			                //   printf("i = %d samp = %d  timer value = %d\r\n", i, sampData[i], timerValue[i]);
							  
				           }
						   }
			           }
			    		
				/*	*/   TBIM_recv_cmd_and_execute();
		    		}

	
}  
static void uctsk_SSNP_TBIM_new_new(void* pdata)
{
hardware_init();
protocol_init();
	printf("init.\r\n");
  	TIM5_Init();
	NVIC_Configuration_TIM5();
	printf("TIM5 init.\r\n");
	TBIM_init(1);
	OSTimeDly(2000);	
	isOk_now=1;
	
	
	
	while(1)
	{
		OSTimeDly(1);
	}
}
static err_t send_streaming_date_new(void)
{
	++tdcn_sequence;
	if(tdcn_sequence!=1)
	{
		number=sensor_manage(tdcn_sequence);
		if(tdcn_sequence==17)
			tdcn_sequence=0;
		return TBIM_send_streaming_data(tdcn_sequence,&number);
	}
}

volatile int timeSlot = 0;
int interrupt = 0;
u8 sync[1500];
u8 sync1[1500];
//u8 sync2[1500];
//u8 sync3[1500];
u16 slot[1500];
u16 slot1[1500];
//u16 slot2[1500];
//u16 slot3[1500];

int k = 0;
int store = 0;
void TIM5_IRQHandler(void)
{


	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET )
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);


	}

}

static err_t send_sdp_data(void)
{
//	u32t data;
//	while(1)
//	{
//		WAIT; 
//		data=sensor_manage(tbim.schedultable[tbim.time_slot_seq].tdcn);
//		return TBIM_send_streaming_data(tbim.schedultable[tbim.time_slot_seq].tdcn,&data);
//	}	
}
int epoch_num=0;
int send_num=0;
/*
	在等待第一个时间槽过后，开中断，这时还要等待100us才可以开启第一个中断
*/
void TIM5_IRQHandler_test1(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET )
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		if(isOk_now)
		{ 
			++send_number;
			if(be_gin*16+1<=send_number && send_number <= (be_gin+1)*16)
			{
				send_streaming_date_new();
			}
			if(send_number%64==0)
			{
				send_number=0;
			}	
		}
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
