#include "TBIM.h"
#include "TEDS.h"
#include "api.h"
#include "uctsk_SSNP_TBIM.h"
#include "TBIMDriver.h"
#include "mem.h"
#include "debug.h"
#include "sdp.h"
#include "../i2c_fram.h"
#include "teds_table.h"
#include "schedule_table.h"

#define TIME_SLOT_NUM 64	//ʱ��Ƭ��ʱ��۵�����
#define TBC_ADDRESS 0xff
#define UUIDLEN 10

#define TDCN_NUM   16
#define MAX_TBIM_ADDR 16

extern sys_sem_t sem;
#define WAIT sys_arch_sem_wait(sem,0)//�ȴ��ź���

/*********************************************************************************
 *                                 TBIM��ʼ����غ���
 *********************************************************************************/
typedef enum 
{
	HALTED,
	OPERATING,
	SLEEP
} STATE;
struct channelRegistItem
{
	u8t nChnNum;//ͨ���ţ�ͨ����TBIM�ڵı�ţ�Ҳ����ע����е��±꣬����ʵ������û�б�Ҫ�ģ���Ϊֱ��ʹ���±�Ϳ�����
	u8t nAddr;//��TBIM�ڵĵ�ַ
	u8t nChnSn;//�˵�ַ��ƫ�Ƶ�ַ
	void* tedsBuf;
	union
	{
		int d;
		u8t databuf[4];
	}data;
	u16t nPeriod;//��������
	u8t state;//״̬
};
struct schedultable_item
{
	u8t tdcn;
	u16t num;
	u16t begin_time_slot;
};
typedef struct TBIM_
{
	u8t alias;
	
  INIT_STATE init_st;//TBIM�����ĳ�ʼ��״̬
  STATE current_state;//TBIM�����Ĺ���״̬
	
	struct netconn* conn_dp;
	struct netconn* conn_sdp;
	struct netconn* conn_tp;
	
	u8t Chn_num;
	struct channelRegistItem chnReg[TDCN_NUM];//ע���,������һ���������Ӧ����Ӧ�ò�ʹ�����ע��������ע���Ӧ�ó�����û�б�Ҫ֪����


	struct schedultable_item schedultable[TIME_SLOT_NUM];
	u16t asy_iso_num;//0 time slot and iso+asy timeslot;
	u16t time_slot_seq;//global time slot sequence;
} TBIM_;
 struct TBIM_ tbim;


static void insertChannel(u8t index,u8t addr,u8t nChnSn,struct TransducerChannel_TEDS* teds_buf)
{
	tbim.chnReg[index].nAddr=addr;
	tbim.chnReg[index].nChnNum=nChnSn;
	tbim.chnReg[index].tedsBuf=teds_buf;

	//#δ���#:�������teds�õ�����
}
void TBIM_init(u8t alias)
{
	u8t addr;
	u8t nChnCnt;
	u8t nChnSn;
	u8t index=0;
	struct TransducerChannel_TEDS* teds_buf; 
	

	u16t i;

	tbim.schedultable[0].tdcn=0;
	tbim.schedultable[0].begin_time_slot=0xffff;
	tbim.schedultable[0].num=0;
  /*
	for(i=1;i<TIME_SLOT_NUM;i++)
	{
		tbim.schedultable[i].tdcn=0;
		tbim.schedultable[i].num=0;
		tbim.schedultable[i].begin_time_slot=0;
	}
	tbim.asy_iso_num=0;
	tbim.time_slot_seq=0;


	for(addr=0;addr<MAX_TBIM_ADDR;addr++)
	{
		nChnCnt=findTdcn(addr);
		if(nChnCnt != (u8t)-1)
		{
			for(nChnSn=0;nChnSn<nChnCnt;nChnSn++)
			{
				getTedsFromTdcn(teds_buf,addr,nChnSn);
				insertChannel(index++,addr,nChnSn,teds_buf);
			}
		}
		else
			break;
	}
*/		printf("1\r\n");
	tbim.alias=alias;
	tbim.Chn_num=index;
	
	tbim.conn_dp=netconn_new(NETCONN_DP);
	if(tbim.conn_dp == NULL)
		printf("TBIM_init():ERROR,conn_dp is NULL.\r\n");
	netconn_bind(tbim.conn_dp,alias,0);
	netconn_connect(tbim.conn_dp,TBC_ADDRESS,0);//TBC�ı�������255 
	printf("2\r\n");
	
	tbim.conn_sdp=netconn_new(NETCONN_SDP);
	if(tbim.conn_sdp == NULL)
		printf("TBIM_init():ERROR,conn_sdp is NULL.\r\n");
	netconn_bind(tbim.conn_sdp,alias,0);
	netconn_connect(tbim.conn_sdp,TBC_ADDRESS,0);//TBC�ı�������255
	printf("3\r\n");
		
	tbim.conn_tp=netconn_new(NETCONN_TP);
	if(tbim.conn_tp == NULL)
		printf("TBIM_init():ERROR,conn_tp is NULL.\r\n");
	netconn_bind(tbim.conn_tp,alias,0);
	netconn_connect(tbim.conn_tp,TBC_ADDRESS,0);//TBC�ı�������255
	printf("4\r\n");
				
	tbim.init_st=READ_TBIM_STRUCT;
	tbim.current_state=HALTED;
}
void set_assign_time_slot(u8t tdcn,u16t timeslot_seq,u16t num)
{
	tbim.schedultable[timeslot_seq].tdcn=tdcn;
	tbim.schedultable[timeslot_seq].num=num;
	tbim.schedultable[timeslot_seq].begin_time_slot=timeslot_seq;
}
void set_schedul_table_table(struct schedultable_item* item)
{
	set_assign_time_slot(item->tdcn,item->begin_time_slot,item->num);	
}
void test_schedul_table()
{
	u8t i;
	u8t tdcn_num;
	u8t start_timeslot_seq;

	start_timeslot_seq=0;//��ʼʱ���
	tdcn_num=16;		 //������ͨ��������

	for(i=1;i<=tdcn_num;i++)
		set_assign_time_slot(i,i+start_timeslot_seq,1);
	set_assign_time_slot(1,0,0);   //���������ã����û�б������������ô���
}
struct TBIM_* get_tbim(void)
{
	return &tbim;
}
void send_sampling_data(void)
{
	u32t data;
	while(1)
	{
		WAIT; 
		data=sensor_manage(tbim.schedultable[tbim.time_slot_seq].tdcn);
		TBIM_send_streaming_data(tbim.schedultable[tbim.time_slot_seq].tdcn,&data);
	}
}
void send_sampling_data_test(void)
{
	u32t data;
	data=sensor_manage(tbim.schedultable[tbim.time_slot_seq].tdcn);
	TBIM_send_streaming_data(tbim.schedultable[tbim.time_slot_seq].tdcn,&data);
}
void set_asy_iso_num(u16t num)
{
	tbim.asy_iso_num=num;	
}
u8t can_send_streaming_data(void)
{
	tbim.time_slot_seq=(tbim.time_slot_seq+1)%tbim.asy_iso_num;
	if(tbim.schedultable[tbim.time_slot_seq].begin_time_slot==tbim.time_slot_seq)//the first available time slot
		return 1;
	return 0;	
}
static err_t TBIM_send_cmd_reply_msg(u8t tbim_tdcn_num,void* data,u16t data_len)
{
	struct netbuf buf;
	err_t err;
	
	buf.alias=TBC_ADDRESS;
	buf.tdcn_num=0;
	buf.tbim_tdcn_num=tbim_tdcn_num;
	buf.p=NULL;
	
	netbuf_ref(&buf,(void*)data,data_len);
	err=netconn_sendto(tbim.conn_dp,&buf,buf.alias,buf.tdcn_num);
	netbuf_free(&buf);

	
	return err;
}
INIT_STATE get_TBIM_init_state(void)
{
	return tbim.init_st;
}

u16t get_sensor_period(u8t tdcn_num)
{
	return tbim.chnReg[tdcn_num-1].nPeriod;
}
u8t get_TBIM_Chn_num(void)
{
	return tbim.Chn_num;
}

void* get_sensor_data(u8t tdcn_num)
{
	void* data;
	
	data=NULL;
	if(getDataFromChn(&data,tbim.chnReg[tdcn_num].nAddr,tbim.chnReg[tdcn_num].nChnSn) != -1)
	{
		return data;
	}
	else
	{
		return NULL;
	}
}
u16t max_period(void)
{
	u16 max;
	u8t i;
	
	max=0;
	for(i=0;i<tbim.Chn_num;i++)
		if(tbim.chnReg[i].nPeriod > max)
			max=tbim.chnReg[i].nPeriod;
	
	return max;
}
err_t TBIM_send_streaming_data(u8t tbim_tdcn_num,void* data)
{
	struct netbuf buf;
	err_t err;
//	u8 array[] = {1,2,3,4,5,6,7,8,9,10,11};
	
	buf.alias=TBC_ADDRESS;
	buf.tdcn_num=0;
	buf.tbim_tdcn_num=tbim_tdcn_num;//��TBC���������ݣ���ôTBC�ı�����ͨ����һ��Ϊ0
	
	buf.p=NULL;	//����һ��Ҫע�⽫p����Ϊ��
	netbuf_ref(&buf,data,4);//���ݳ���һ����4�ֽ�
//	err=netconn_sendto(tbim.conn_sdp,&buf,buf.alias,buf.tdcn_num);//ע������ʹ��sdp���������ݵķ���
	tbim.conn_sdp->pcb.sdp->local_tdcn_num=tbim_tdcn_num;
	
	if((err=sdp_sendto(tbim.conn_sdp->pcb.sdp,buf.p,buf.alias,buf.tdcn_num)) != ERR_OK)
		printf("TBIM_send_streaming_data():send data failed.\r\n");
//	printf("TBIM_send_streaming_data():tbim send data to %d.\r\n",buf.alias);
	netbuf_free(&buf);//����һ��Ҫע���ͷ��ڴ棬�����ڴ�й¶
	
	return err;
}
//ע�⣺��������к�����ǰ�������е������trigger������TBC���͵ģ���˻ظ�Ҳ����TBC����
err_t TBIM_recv_cmd_and_execute(void)
{
	struct netbuf* buf;
	void* data;
	u16t data_len;
	u8t* dataptr;
	u8t cmdclass;
	u8t cmdfunc;
	void* argument;
	err_t err;
	
	buf=NULL;
	err=netconn_recv_unblock(tbim.conn_dp,&buf);
	
	if(err != ERR_OK)
		return err;
	
	netbuf_data(buf,&data,&data_len);
	
	dataptr=(u8t*)data;
	cmdclass=*dataptr;
	cmdfunc=*(++dataptr);
	argument=(void*)(++dataptr);
	printf("TBIM_recv_cmd_and_execute():executing cmdclass=%d,cmdfunc=%d.\r\n",cmdclass,cmdfunc);
    printf("TBIM_recv_cmd_and_execute");
	execute(cmdclass,cmdfunc,buf->tbim_tdcn_num, argument);
	
	netbuf_delete(buf);//�ײ㷢������������һ��Ҫdelete��
	
	return ERR_OK;
}

err_t TBIM_recv_trigger_and_execute(void)
{
	err_t err;
	u8t alias;
	u8t tdcn_num;
	
	err=netconn_recvtrigger_unblock(tbim.conn_tp,&alias,&tdcn_num);
	
	if(err == ERR_OK)
		trigger_do();
	
	return err;
}


/*********************************************************************************
 *                                 uuid��غ���
 *********************************************************************************/
static struct uuid uid={0,0,0,0,0,0,0,0,0,2}; 
struct uuid* get_uuid()
{
	return &uid;
}
u8t is_uuid_eq(struct uuid* id)
{
	u8t i;
	for(i=0;i<UUIDLEN;i++)
	{
		if(id->uid[i] != uid.uid[i])
			return 0;
	}
	return 1;
}


/**********************************************************************************
 *    ����û�б�Ҫ��Ҫ�ײ�Э��֪���ϲ�TEDS�Ĵ��ڣ��²�Э��ֻ��Ҫ��TBIM�����Ϳ�����
 **********************************************************************************/
u8t get_asy_flag() 
{
	return phy_teds_asy_flag();
}
u8t get_payload_encoding()
{
	return phy_teds_payload_encoding();
}
f32 get_start_delay()
{
	return phy_teds_start_delay();
}
f32 get_reflect_delay()
{
	return phy_teds_reflect_delay();
}
f32 get_reflect_delay_uncertainty()
{
	return phy_teds_reflect_delay_uncertainty();
}

/**********************************************************************************
 *
 *
 *
 *                               ����ִ����غ���
 *
 *
 *
 *
 **********************************************************************************/
#define INITCOMMNUM 17
#define OPCOMMNUM   14
#define OPMODENUM   6

#define ACCODE_METATEDS 2
#define ACCODE_TCTEDS   3

//�ڲ����еı���������������صĲ�����ͨ����������������
static void send_cmd_reply(void* data,void*argument)
{
	
}
static void sent_trigger_reply(void* argument)
{
	
}

typedef void (*function)(u8t tdcn_num,void *argument);
typedef enum
{
    INITIALIZATION=1,
    OPERATIONAL,
    QUERYTEDS,
    READTEDSBLCOK,
    WRITETEDSBLCOK,
    UPDATETEDS,
    SETOPERATINGMODE,
    READOPERATINGMODE,
    RUNDIAGONSITICS,
		CHANGESCHEDULETABLE,
} STANDARD_COMMAND;

enum INITIALIZATION_COMMAND
{
	SLEEP_=1,
	WAKEUP,
	SET_TDCN_DATA_REPETITION_COUNT,
	READ_TDCN_DATA_REPETITION_COUNT,
	SET_TDCN_PRETIRGGER_COUNT,
	READ_TDCN_PRETRIGGER_COUNT,
	CALIBRATE_TDCN,
	ZERO_TDCN,
	ENABLE_CORRECTIONS,
	DISABLE_CORRECTIONS,
	ENABLE_TDCN,
	ADDRESSGROUP_DEFINITION,
	READ_ADDRESSGROUP_ASSIGNMENT,
	ERASE_OPERATIONAL_SETUP,
	STORE_OPERATIONAL_SETUP,
	READ_TBIM_STRUCTURE
};
enum OPERATIONAL_COMMAND
{
	ENABLE_TDCN_TRIGGER=1,
	DISABLE_TDCN_TRIGGER,
	WRITE_SERVICE_REQUEST_MASK,
	READ_SERVICE_REQUEST_MASK,
	READ_STATUS,
	QUERY_DATA_BLOCK,
	READ_TDCN_DATA,
	READ_TDCN_DATA_BLOCK,
	WRITE_TDCN_DATA,
	WRITE_TDCN_DATA_BLOCK,
	READ_TBIM_VERSION,
	RESETS,
	HALT
};
enum TEDS_ACCESS_CODE
{
	MODULE_META_TEDS=1,
	META_ID_TEDS,
	TDCN_TEDS,
	TDCN_ID_TEDS,
	CALIBRATION_TEDS,
	CALIBRATION_ID_TEDS,
	END_USER_APP_SEPCIFIC_TEDS,
	FREQUENCY_RESPONSE_TEDS,
	TRANSFER_FUNCTION_TEDS,
	CMD_TEDS,
	LOCATION_AND_TITLE_TEDS,
	COMMISSINOING_TEDS,
	PHY_TEDS
};
static void sleep(u8t tdcn_num,void *argument)
{
	printf("transducer channel %d sleep.\r\n",tdcn_num);
//	actor_manage(tdcn_num);
/*
    �˴�ʵ�ֵĹ����ǣ�
		���մ������TBIM���߱�����ͨ������low-powerģʽ�������ڴ�ģʽ��ֻ��Ӧwake-up����
		�Ӳ���Sleep�������ȷ��
*/
}
static void wakeup(u8t tdcn_num,void *argument)
{
	/*
	   ������Ҫʵ�ֵĹ����ǣ�
		 ǿ�ƽ�TBIM���߱�����ͨ������halted״̬
		 �����Ҫ������������ȷ�ϵĻ���Ӧ����Ϣ��TBIM��������Ӧ�ð���32λ��״̬��
	 */
/*	struct TBIM_arg* arg;
	struct netbuf buf;
	u8t data[12]="TBIM wake up";
	
	printf("TBIM(): TBIM wake up.\r\n");
	
	arg=(struct TBIM_arg*)argument;


	buf.tbim_tdcn_num=arg->tbim_tdcn_num;
	buf.alias=arg->alias;
	buf.tdcn_num=arg->tdcn_num;
	buf.p=NULL;	//����һ��Ҫע�⽫p����Ϊ��
	netbuf_ref(&buf,(void*)data,12);
	printf("TBIM(): sending msg.\r\n");
	
	netconn_sendto(arg->conn,&buf,buf.alias,buf.tdcn_num);
	
	printf("TBIM(): sent msg done ok .\r\n");
	netbuf_free(&buf);//����һ��Ҫע���ͷ��ڴ棬�����ڴ�й¶
	*/
}
static void setTCdataRepCount(u8t tdcn_num,void *argument)
{
}
static void readTCdataRepCount(u8t tdcn_num,void *argument)
{
}
static void setTCpretriggerCount(u8t tdcn_num,void *argument)
{
}
static void readTCpretriggerCount(u8t tdcn_num,void *argument)
{
}
static void calibrateTC(u8t tdcn_num,void *argument)
{
}
static void zeroTC(u8t tdcn_num,void *argument)
{
}
static void enableCorrections(u8t tdcn_num,void *argument)
{
}
static void disableCorrections(u8t tdcn_num,void *argument)
{
}
static void enableTC(u8t tdcn_num,void *argument)
{
}
static void addressGroupDef(u8t tdcn_num,void *argument)
{
}
static void readAddressGroupAssignment(u8t tdcn_num,void *argument)
{
}
static void eraseOperationSetup(u8t tdcn_num,void *argument)
{
}
static void storeOperationSetup(u8t tdcn_num,void *argument)
{
}
static void readTBIMstruct(u8t tdcn_num,void *argument)
{
	u8t chnNum;
	u8t* reply_msg;
	u8t* iset;
	u8t i;
	u8t data_len;
	err_t err;
//	SSNP_DEBUG_PRINT("readTBIMstruct():sending TBIM struct.\r\n");
	reply_msg=NULL;
	
	/* �˴���Ҫ����ʵ�� ****************************/
	chnNum=tbim.Chn_num;
	chnNum=1;//TEST:����ֻ��������֤
	/* �˴���Ҫ����ʵ�� ****************************/
	
	
	data_len=3+chnNum+1;//3�ֽڵ�ͷ����Ϣ��Э������
	reply_msg=(u8t*)mem_alloc(data_len);
	
	if(reply_msg == NULL)
		return ;
	iset=reply_msg;
	
	*iset=INITIALIZATION;
	*(++iset)=READ_TBIM_STRUCTURE;
	*(++iset)=0;//#δ���#�����ｫstatus octet������Ϊ0
	*(++iset)=chnNum;
	for(i=1;i<=chnNum;i++)//ע�������ͨ������С��1��ʼ��0��TBIM
		*(++iset)=i;
	
	err=TBIM_send_cmd_reply_msg(0,reply_msg,data_len);
	mem_free((void*)reply_msg);
	tbim.init_st=READ_META_TEDS;//�ȴ�TBC��ȡmeta_teds
	SSNP_DEBUG_PRINT("readTBIMstruct():send TBIM struct OK.\r\n");
}

function initialization[INITCOMMNUM]={NULL,sleep,wakeup,setTCdataRepCount,readTCdataRepCount,setTCpretriggerCount,
                            readTCpretriggerCount,calibrateTC,zeroTC,enableCorrections,disableCorrections,enableTC,
                            addressGroupDef,readAddressGroupAssignment,eraseOperationSetup,storeOperationSetup,readTBIMstruct};


static void enableTCtrigger(u8t tdcn_num,void *argument)
{
}
static void disableTCtrigger(u8t tdcn_num,void *argument)
{
}
static void writeServiceReqMask(u8t tdcn_num,void *argument)
{
}
static void readServiceReqMask(u8t tdcn_num,void *argument)
{
}
static void readStatus(u8t tdcn_num,void *argument)
{
}
static void queryDataBlock(u8t tdcn_num,void *argument)
{
}
static void readTCdata(u8t tdcn_num,void *argument)
{
}
static void readTCdataBlock(u8t tdcn_num,void *argument)
{
}
static void writeTCdata(u8t tdcn_num,void *argument)
{
}
static void writeTCdataBlock(u8t tdcn_num,void *argument)
{
}
static void readTBIMversion(u8t tdcn_num,void *argument)
{
}
static void reset(u8t tdcn_num,void *argument)
{
}
static void halt(u8t tdcn_num,void *argument)
{
}
function op[OPCOMMNUM]={NULL,enableTCtrigger,disableTCtrigger,writeServiceReqMask,readServiceReqMask,readStatus,queryDataBlock,
                       readTCdata,readTCdataBlock,writeTCdata,writeTCdataBlock,readTBIMversion,reset,halt};


static void queryTEDS(u8t cmdFunc, u8t tdcn_num,void *argument)
{
	u8t *iset;
	err_t err;
	u32t* attribute;
	u16t* maximum_block_size;
	u8t query_TEDS_response[3+1+4+4+2];
	u8 resArray[11] = {4, 5, 6, 7, 8, 9,
	                   10, 11, 12, 13, 14};
/*	u32 *p;
	u16*q;
	resArray[0] = 1;
	p = (u32 *)(resArray + 1);
	*p = 35;
	p ++;
	*p = 35;
	p ++;
	q = (u16 *)p;
	*q = 35;
*/  struct netbuf buf;

	
/*	buf.alias=TBC_ADDRESS;
	buf.tdcn_num=0;
	buf.tbim_tdcn_num=1;
	buf.p=NULL;
	
	netbuf_ref(&buf,(void*)resArray,11);
	err=netconn_sendto(tbim.conn_sdp,&buf,buf.alias,buf.tdcn_num);
	netbuf_free(&buf);
*/	switch(cmdFunc)
	{
		case MODULE_META_TEDS:
		{
		//	SSNP_DEBUG_PRINT("queryTEDS():querying meta teds.\r\n");
			iset=(u8t*)query_TEDS_response;
			*iset=QUERYTEDS;
			*(++iset)=MODULE_META_TEDS;
			*(++iset)=0;//status bit
			*(++iset)=0;//TEDS attribure;
			attribute=(u32t*)iset;
			*attribute=35;//meta_teds��С
			*(++attribute)=35;
			maximum_block_size=(u16t*)(iset+8);
			*maximum_block_size=35;
		// Ӧ��Э�鷢�� Ŀǰ���Բ���ȷ  ������������Э�����	
			err = TBIM_send_cmd_reply_msg(0,resArray,11);

	//	TBIM_send_cmd_reply_msg(0,resArray, 11);	
		//	err = TBIM_send_streaming_data(1, resArray);
			printf("err = %d\r\n", err);
			SSNP_DEBUG_PRINT("queryTEDS():querying meta teds OK.\r\n");
			
			break;
		}
		case TDCN_TEDS:
		{
			//#δ���#������Ӧ�ø��ݱ�����ͨ��������ȡ��teds����
		//	printf("queryTEDS():querying tdcn %d teds.\r\n",tdcn_num);
			iset=(u8t*)query_TEDS_response;
			*iset=QUERYTEDS;
			*(++iset)=TDCN_TEDS;
			*(++iset)=0;//status bit
			*(++iset)=0;//TEDS attribure;
			attribute=(u32t*)iset;
			*attribute=118;//tdcn_teds��С
			*(++attribute)=118;
			maximum_block_size=(u16t*)(iset+8);
			*maximum_block_size=118;
			
			err=TBIM_send_cmd_reply_msg(0,query_TEDS_response,14);
			SSNP_DEBUG_PRINT("queryTEDS():querying tdcn teds OK.\r\n");
			break;			
		}
		default:break;
	}
}

 struct tdcn_info
{
	char sORt;
	char type;
}info[17]={{0,0},
           {0,1},{1,1},{1,6},{1,3},
           {1,4},{1,5},{1,6},{1,7},
		   {1,8},{1,9},{1,8},{1,7},
		   {1,6},{1,5},{1,4},{1,9}};
  
 /*
struct tdcn_info
{
	char sORt;
	char type;
}info[17]={{0,0},
           {0,2},{1,2},{1,6},{1,3},
           {1,4},{1,5},{1,6},{1,7},
		   {1,8},{1,9},{1,8},{1,7},
		   {1,6},{1,5},{1,4},{1,3}};
   */
/*		
 struct tdcn_info
{
	char sORt;
	char type;
}info[17]={{0,0},
           {0,1},{1,2},{1,6},{1,3},
           {1,4},{1,5},{1,6},{1,7},
		   {1,8},{1,9},{1,8},{1,7},
		   {1,6},{1,5},{1,4},{1,3}};
  */
/*
 struct tdcn_info
{
	char sORt;
	char type;
}info[17]={{0,0},
           {0,1},{1,3},{1,6},{1,3},
           {1,4},{1,5},{1,6},{1,7},
		   {1,8},{1,9},{1,8},{1,7},
		   {1,6},{1,5},{1,4},{1,3}};
 */

struct tdcn_TEDS				    
{
	char cmdclass;
	char cmdfunc;
	char pad;
	struct tdcn_info info;
};
char ch[] = {'a','b','c','d'};
static void readTEDS(u8t cmdFunc,u8t tdcn_num, void *argument)
{
	u8t i;
	u8t* iset;
	err_t err;
	u32t* offset;
	u16t* max_block_size;
	u16 off = 0;
	u16 num = 0;
	u16 *ptr;
	u8 *array = NULL;
	struct tdcn_TEDS tdcnTEDS;
	u8 arrayMetaTeds[META_TEDS_SIZE];
	u8 arrayTsTeds[TS_TEDS_SIZE];

	
	u8t meta_teds[3+35];
	u8t tdcn_teds[3+118];
	switch(cmdFunc)
	{
		case MODULE_META_TEDS:
		{
		//	SSNP_DEBUG_PRINT("readTEDS():reading meta teds.\r\n");
		/*	iset=(u8t*)argument;
			offset=(u32t*)argument;//�õ�����read teds block�������
			max_block_size=(u16t*)(iset+4);//�õ�����read teds block�������
			
			iset=(u8t*)meta_teds;
			*iset=READTEDSBLCOK;
			*(++iset)=MODULE_META_TEDS;
			*(++iset)=0;//status bit
			for(i=0;i<35;i++)//meta_teds��û�����ݣ�ȫ������
				iset[i]=0;
		*/	I2C_FRAM_BufferRead(arrayMetaTeds, tedsTable[0].addr, META_TEDS_SIZE);
		//	err=TBIM_send_cmd_reply_msg(0,arrayMetaTeds, META_TEDS_SIZE);
		//    TBIM_send_streaming_data(1, meta_teds);
			tbim.init_st=READ_TDCN_TEDS;
			SSNP_DEBUG_PRINT("readTEDS():reading meta teds OK.\r\n");
			break;
		}
		case TDCN_TEDS:
		{
		/*		iset=(u8t*)argument;
		offset=(u32t*)argument;//�õ�����read teds block�������
			max_block_size=(u16t*)(iset+4);//�õ�����read teds block�������
		//	printf("readTEDS():reading tdcn %d teds.\r\n",tdcn_num);
			iset=(u8t*)tdcn_teds;
			info[0].sORt=READTEDSBLCOK;
			info[0].type=TDCN_TEDS;
//			*iset=READTEDSBLCOK;
//			*(++iset)=TDCN_TEDS;
//			*(++iset)=0;//status bit
//			for(i=0;i<118;i++)//tdcn_teds��û�����ݣ�ȫ������
//				iset[i]=0;		
	  */    ptr = (u16*)argument;
	        off = *ptr;
			ptr ++;
			num = *ptr;
			ptr ++;
			array = (u8*)ptr;
		    printf("offset = %d num = %d tdcn_num = %d\r\n",off, num, tdcn_num);
			err=TBIM_send_cmd_reply_msg(0,array,2+32);
	    //	TBIM_send_streaming_data(1, meta_teds);
			tbim.init_st=WAITTING_FOR_TIME;
			SSNP_DEBUG_PRINT("readTEDS():reading tdcn teds OK.\r\n");
			break;			
		}
		case CALIBRATION_TEDS:
		{   TBIM_send_streaming_data(1, ch);
		    break;
		}
		default:break;
	}
}
static void writeTEDS(u8t cmdFunc, u8t tdcn_num,void *argument)
{
	    u32 *ptr = (u32*) argument;
		u8 *array = NULL;
	    u32 offset = *ptr;
		Phy_TEDS *phy = (Phy_TEDS *) malloc(sizeof(Phy_TEDS));
		META_TEDS *meta = (META_TEDS *) malloc(sizeof(META_TEDS));
		Transducerchannel_TEDS *ts = (Transducerchannel_TEDS *) malloc(sizeof(Transducerchannel_TEDS));
		u8 arr[200];
		u32 num = 0;
		u16 i = 0;
		u8 type = 0;
		float f;
		ptr ++;
		num = *ptr;
		type = *(ptr + 1);
		array = (u8*) ptr;
		printf("TBIM write TEDS Online\r\n");
		printf("func = %d offset = %d num = %d \r\n",cmdFunc, offset, num);
		for (i = 0; i < 35; i ++)
			printf("%d ", array[i]);
		printf("\r\n type = %d\r\n", type);
	   // initTedsTable();
	//	writeTedsTable();
	//	readTedsTable();
	    
		switch (type)
		{
		    case 1:
				  I2C_BufferWrite(array, tedsTable[0].addr, num);
				  readPhyTeds(tedsTable[0].addr, phy);
				  printf("len = %d\r\n", phy->length);
				  printf("identifier = %d\r\n", phy->identifier);
				  printf("highest_supported_payload_encoding = %d\r\n", phy->highest_supported_payload_encoding);
				  printf("start_delay;  = %f \r\n", phy->start_delay );
			      printf("reflect_delay = %f \r\n", phy->reflect_delay);
				  printf("reflect_delay_uncertainty = %f \r\n", phy->reflect_delay_uncertainty);
				  printf("checksum = %d\r\n", phy->checksum);
				  for (i = 0; i < num; i ++)
	                  arr[i] = 0;
	              I2C_FRAM_BufferRead(arr, tedsTable[0].addr, num);
                  for (i = 0; i < num; i ++)
	                  printf("%d ", arr[i]);
				  break;
		    case 2:
			      I2C_BufferWrite(array, tedsTable[1].addr, num);
				  readMetaTeds(tedsTable[1].addr, meta);
				  printf("len = %d\r\n", meta->length);
				  printf("identifier = %d\r\n", meta->identifier);
				  printf("sustained_load_current = %f\r\n", meta->sustained_load_current);
				  printf("checksum = %d\r\n", meta->checksum);
				  for (i = 0; i < num; i ++)
	                  arr[i] = 0;
	              I2C_FRAM_BufferRead(arr, tedsTable[1].addr, num);
				  for (i = 0; i < num; i ++)
	                  printf("%d ", arr[i]);
			      break;
			case 3:
			      printf("case 3 tdcn_num = %d\r\n", tdcn_num);
			      I2C_BufferWrite(array, tedsTable[2 + tdcn_num - 1].addr, num);
				  printf("read finished \r\n");
				  readTsTeds(tedsTable[2 + tdcn_num - 1].addr, ts);
				  printf("read finished \r\n");
				  printf("len = %d\r\n", ts->length);
				  printf("identifier = %d\r\n", ts->identifier);
				  f = 1234.5678;
				  printf("f = %f \r\n", f);
				  f =  ts->ts_sampling_period;
				  printf("f = %f \r\n", f);
				  printf("ts_sampling_period = %f\r\n", ts->ts_sampling_period);
				  for (i = 0; i < num; i ++)
	                  arr[i] = 0;
	              I2C_FRAM_BufferRead(arr, tedsTable[2 + tdcn_num - 1].addr, num);
				  for (i = 0; i < num; i ++)
	                  printf("%d ", arr[i]);
			      break;
			default:
			      break;
		}
	//	writeTeds(array, num);
	//	readTeds();
	 //   I2C_BufferWrite(array, TEDS_START_ADDR + offset, num); 
		
}
static void updateTEDS(u8t cmdFunc,u8t tdcn_num, void *argument)
{
}
static void setOPmode(u8t cmdFunc,u8t tdcn_num, void *argument)
{
}
static void readOPmode()
{
}
static void rundiagnostics(u8t cmdFunc)
{
}
static void invalidCommand()
{
}

static void changeScheduleTable(u8t cmdFunc,u8t tdcn_num, void *argument)
{
	  int slot = 0;
		OPERATION op;
		int len = 0;
	  u8 *p = (u8 *) argument;
		slot = *p;
		p ++;
		op = *p;
		//p ++;
		len = *p;
	/*	tbimScheduleTable[slot].size ++;
		tbimScheduleTable[slot].slot[0].tdcn_num = tdcn_num;
		tbimScheduleTable[slot].slot[0].op = ACTION_START;
		tbimScheduleTable[slot].size ++;
		tbimScheduleTable[(slot + len) % tbim_schedule_size].slot[0].tdcn_num = tdcn_num;
		tbimScheduleTable[(slot + len) % tbim_schedule_size].slot[0].op = ACTION_END;
  */
	  addTBIMScheduleOp(slot, tdcn_num, op);
		addTBIMScheduleOp(slot + len, tdcn_num, op);
}

void execute(u8t cmdClass, u8t cmdFunc,u8t tdcn_num,void *argument)
{
	  printf("excute function\r\n");
    switch(cmdClass)
		{
    case INITIALIZATION:
	  {
        if(cmdFunc >= 1 && cmdFunc < INITCOMMNUM)
            initialization[cmdFunc](tdcn_num, argument);
        else
            invalidCommand();
        break;
    }
    case OPERATIONAL:
		{
        if(cmdFunc >= 1&& cmdFunc < OPCOMMNUM)
            op[cmdFunc](tdcn_num, argument);
        else
            invalidCommand();
        break;
    }
    case QUERYTEDS:
		{
        if(cmdFunc >= MODULE_META_TEDS && cmdFunc <= PHY_TEDS)
            queryTEDS(cmdFunc, tdcn_num, argument); 
        else
            invalidCommand();
        break;
    }
    case READTEDSBLCOK:
		{
        if(cmdFunc >= MODULE_META_TEDS && cmdFunc<=PHY_TEDS)
            readTEDS(cmdFunc,tdcn_num,argument);
        else
            invalidCommand();
        break;
    }
    case WRITETEDSBLCOK:
		{
        if(cmdFunc >= MODULE_META_TEDS && cmdFunc<=PHY_TEDS)
            writeTEDS(cmdFunc, tdcn_num, argument);
        else
            invalidCommand();
        break;
    }
    case UPDATETEDS:
		{
        if(cmdFunc >= MODULE_META_TEDS && cmdFunc <= PHY_TEDS)
            updateTEDS(cmdFunc, tdcn_num, argument);
        else
            invalidCommand();
        break;
    }
    case SETOPERATINGMODE:
		{
        if(cmdFunc >= 1 && cmdFunc <= OPMODENUM)
            setOPmode(cmdFunc, tdcn_num, argument);
        else
            invalidCommand();
        break;
    }
    case READOPERATINGMODE:
		{
        readOPmode();
        break;
    }
    case RUNDIAGONSITICS:
		{
        rundiagnostics(cmdFunc);
        break;
    }
		case CHANGESCHEDULETABLE:
		{
			  changeScheduleTable(cmdFunc, tdcn_num, argument);
		}
    default:
		{
        invalidCommand();
        break;
    }
   }
}
void trigger(void* argument)
{
	struct TBIM_arg* arg;
	
	printf("TBIM(): TBIM get trigger.\r\n");
	
	arg=(struct TBIM_arg*)argument;
	netconn_sendtrigger(arg->conn,arg->tbim_tdcn_num,arg->alias,arg->tdcn_num);	
}
void trigger_do()
{
	
}
