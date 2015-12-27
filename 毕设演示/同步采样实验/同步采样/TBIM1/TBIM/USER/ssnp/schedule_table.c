#include "schedule_table.h"
#include "stdio.h"

#define SCHEDULE_TABLE_SIZE (10 + 6)

int schedule_size = SCHEDULE_TABLE_SIZE;

u8 sendScheduleArray[SCHEDULE_TABLE_SIZE * SCHEDULE_SLOT_SIZE];

ScheduleSlot ScheduleTable[SCHEDULE_TABLE_SIZE];

int tbim_schedule_size = SCHEDULE_TABLE_SIZE;

tbimScheduleSlot tbimScheduleTable[SCHEDULE_TABLE_SIZE];

tbimScheduleOp opArray[SCHEDULE_TABLE_SIZE];

tbimScheduleOp * tbimOpTable[SCHEDULE_TABLE_SIZE];

void generateTbimOpTable()
{
	  int i = 0;
		for (i = 0; i < SCHEDULE_TABLE_SIZE; i ++)
		{
			  opArray[i].op = ScheduleTable[i].op;
				opArray[i].tdcn_num = ScheduleTable[i].tdcn_num;
				tbimOpTable[i] = &(opArray[i]);
    }
}

void sendScheduleTable(void)
{
	  int i = 0;
		u8 * src = (u8 *)ScheduleTable;
		u8 * dst = sendScheduleArray;
		for (i = 0; i < SCHEDULE_TABLE_SIZE * SCHEDULE_SLOT_SIZE; i ++, src ++, dst ++)
		    *dst = *src;
				
	 // call send function to broadcast this schedule table
}

void localScheduleTest(void)
{
	  generateTestScheduleTable();
		generateTbimOpTable();
		generateTBIMScheduleTable();
}

void generateTestScheduleTable(void)
{
	  u8 i = 0;
		for (i = 0; i < 10; i ++)
		{
			  switch (i % 6)
				{
					  case 0:
							    ScheduleTable[i].alias = 1;
					        ScheduleTable[i].tdcn_num = 1;
					        ScheduleTable[i].syn = 1;
					        ScheduleTable[i].op = SAMPLING;
							    break;
					  case 1:
							    ScheduleTable[i].alias = 1;
					        ScheduleTable[i].tdcn_num = 2;
					        ScheduleTable[i].syn = 1;
					        ScheduleTable[i].op = SAMPLING;
							    break;
						case 2:
							    ScheduleTable[i].alias = 1;
					        ScheduleTable[i].tdcn_num = 3;
					        ScheduleTable[i].syn = 1;
					        ScheduleTable[i].op = SAMPLING;
							    break;
						case 3:
							    ScheduleTable[i].alias = 2;
					        ScheduleTable[i].tdcn_num = 1;
					        ScheduleTable[i].syn = 1;
					        ScheduleTable[i].op = SAMPLING;
							    break;
						case 4:
							    ScheduleTable[i].alias = 2;
					        ScheduleTable[i].tdcn_num = 2;
					        ScheduleTable[i].syn = 1;
					        ScheduleTable[i].op = SAMPLING;
							    break;
						case 5:
							    ScheduleTable[i].alias = 2;
					        ScheduleTable[i].tdcn_num = 3;
					        ScheduleTable[i].syn = 1;
					        ScheduleTable[i].op = SAMPLING;
							    break;
						default:
							break;
					  
		    }

		}
		
}

void generateTBIMScheduleTable(void)
{
	  int i = 0;
		tbimScheduleOp *p = NULL;
		for (i = 0; i < tbim_schedule_size - 6; i ++)
		{
			 //  tbimScheduleTable[i].size = 1;
				 p = (tbimScheduleOp*) malloc(sizeof(tbimScheduleOp));
				 p->next = NULL;
				 p->tdcn_num = i % 3 + 1;
				 p->op = SAMPLING;
				 tbimScheduleTable[i].slot = p;
				//tbimScheduleTable[i].slot = tbimOpTable[i];
    }
		for (i = SCHEDULE_TABLE_SIZE - 6; i < SCHEDULE_TABLE_SIZE; i ++)
		{
		     //tbimScheduleTable[i].size = 0;
		     tbimScheduleTable[i].slot = NULL;
		}
		 p = tbimScheduleTable[1].slot;
		 tbimScheduleTable[1].slot = NULL;
		 free(p);
		 p = NULL;
		 p = tbimScheduleTable[8].slot;
		 tbimScheduleTable[8].slot = NULL;
		 free(p);
		 p = NULL;
		    
}

void addTBIMScheduleOp(int index, u8 tdcn_num, OPERATION op)
{
	 		tbimScheduleOp *p = NULL;
      p = (tbimScheduleOp*) malloc(sizeof(tbimScheduleOp));
		  p->next = NULL;
		  p->tdcn_num = tdcn_num;
		  p->op = op;
		  tbimScheduleTable[index].slot = p;			
}

tbimScheduleOp *deleteTBIMScheduleOp(int index, tbimScheduleOp *p)
{
	  tbimScheduleOp *q = NULL;
		tbimScheduleOp *r = NULL;
		tbimScheduleOp *ret = NULL;
	  if (p == NULL)
			  return NULL;
				
		q = tbimScheduleTable[index].slot;
		if (q == p)
		{
			  r = p;
				tbimScheduleTable[index].slot = p->next;
    }
		else
		{
			  while (q != NULL && q->next != p)
					  q = q->next;
				if (q == NULL)
					  return NULL;
				q->next = p->next;
				r = p;
    }
		ret = r->next;
		free(r);
		return ret;
		
}
