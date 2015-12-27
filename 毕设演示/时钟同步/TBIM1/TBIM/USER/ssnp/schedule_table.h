#ifndef SCHEDULE_TABLE
#define SCHEDULE_TABLE
#include "opt.h"

// ������Ӧ��ִ�еĲ���
typedef enum operation
{
	  SAMPLING,           //����
		ACTION,             //ִ������ ���õȴ����  ���� ���ù���ִ�н���
		ACTION_START,       //ִ�п�ʼ
		ACTION_END,        //ִ�н���
} OPERATION;

typedef struct scheduleslot
{
	  u8 alias;            // ����
		u8 tdcn_num;         // ͨ����
		u8 syn;              // ��ʱ�����ͬ�������첽ʱ���
		OPERATION op;        // �ڴ�ʱ���ִ�еĲ���
		
} ScheduleSlot;

typedef struct tbimscheduleop
{
	u8 tdcn_num;
	OPERATION op;
	struct tbimscheduleop *next;
} tbimScheduleOp;

typedef struct tbimscheduleslot
{
	u8 size;
	tbimScheduleOp * slot;
} tbimScheduleSlot;

typedef union uScheduleSlot
{
	ScheduleSlot s;
	int array[sizeof(ScheduleSlot)];
} uScheduleSlot;

#define SCHEDULE_SLOT_SIZE  sizeof(ScheduleSlot)

extern int tbim_schedule_size;

extern tbimScheduleSlot tbimScheduleTable[];

extern tbimScheduleOp * tbimOpTable[];

extern int schedule_size;


extern ScheduleSlot scheduleTable[];

void sendScheduleTable(void);

void generateTestScheduleTable(void);

void generateTBIMScheduleTable(void);

void localScheduleTest(void);

void addTBIMScheduleOp(int index, u8 tdcn_num, OPERATION op);

tbimScheduleOp * deleteTBIMScheduleOp(int index, tbimScheduleOp * p);


#endif 
