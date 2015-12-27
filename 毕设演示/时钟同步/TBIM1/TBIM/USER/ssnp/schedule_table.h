#ifndef SCHEDULE_TABLE
#define SCHEDULE_TABLE
#include "opt.h"

// 变送器应该执行的操作
typedef enum operation
{
	  SAMPLING,           //采样
		ACTION,             //执行以下 不用等待结果  或者 不用关心执行结束
		ACTION_START,       //执行开始
		ACTION_END,        //执行结束
} OPERATION;

typedef struct scheduleslot
{
	  u8 alias;            // 别名
		u8 tdcn_num;         // 通道号
		u8 syn;              // 此时间槽是同不还是异步时间槽
		OPERATION op;        // 在此时间槽执行的操作
		
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
