
#include "teds_table.h"

typedef union tedsEntry
{	u8t array[TEDS_ENTRY_SIZE];
    struct teds_entry entry;
} TedsEntry;

void writeTedsEntry(struct teds_entry * entry, u8 addr)
{
	u8 i = 0;
    TedsEntry uEntry;
	uEntry.entry = *entry;
 /*   printf("writing teds entry\r\n");
	for (i = 0; i < 4; i ++)
	{	   
		printf(" uEntry[%d] = %d\r\n",i, uEntry.array[i]);
	}
	printf("\r\n");
   */
	I2C_BufferWrite(uEntry.array, addr, TEDS_ENTRY_SIZE);

}



void writeTedsTable()
{
   u8 i = 0;
   for (i = 0; i < TEDS_TABLE_SIZE; i ++)
       writeTedsEntry(&tedsTable[i], TEDS_TABLE_ADDR + i * 4);	
}

teds_entry tedsTable[TEDS_TABLE_SIZE];

void initTedsTable()
{
    u8 i = 0;
	//PHY TEDS entry
    tedsTable[0].type = 1;
    tedsTable[0].valid = 1; 
	tedsTable[0].addr = 40;
	tedsTable[0].len = 19;
    //Meta TEDS entry	
	tedsTable[1].type = 2;
    tedsTable[1].valid = 1; 
	tedsTable[1].addr = 59;
	tedsTable[1].len = 35;
	for (i = 0; i < TEDS_TABLE_SIZE - 2; i ++)
	{
		tedsTable[i + 2].type = 3;
		if (i < 3)
    	    tedsTable[i + 2].valid = 1; 
		else
			tedsTable[i + 2].valid = 0;
		tedsTable[i + 2].addr = 94 + 10 * i;
		tedsTable[i + 2].len = 10;
	}   

}

void readTedsTable()
{
    u8 i = 0;
    teds_entry test[TEDS_TABLE_SIZE];
	for (i = 0 ; i < TEDS_TABLE_SIZE; i ++)
	{
	    I2C_FRAM_BufferRead((u8 *)&tedsTable[i], TEDS_TABLE_ADDR + i * 4, TEDS_ENTRY_SIZE);
		test[i] = tedsTable[i];
	}
//	printf("Reading teds table\r\n");
//	for (i = 0; i < TEDS_TABLE_SIZE; i ++)
//	    printf("teds entry %d :   type = %d  valid = %d addr = %d  len = %d\r\n", i, test[i].type, test[i].valid, test[i].addr, test[i].len);
}
#define TEDS_START_ADDR (TEDS_TABLE_ADDR + TEDS_TABLE_SIZE * 4)
#define TEDS_SIZE 84
void writeTeds(u8 * array)
{	u8 i = 0;
    u8 arr[187];
//	for (i = 0; i < 187; i ++)
//	    arr[i] = i + 4;
	printf("recv_teds\r\n");
	for (i = 0; i < 187; i ++)
	    printf("%d ", array[i]);
	//printf("TEDS_START_ADDR = %d  TEDS_SIZE = %d  \r\n", TEDS_START_ADDR, TEDS_SIZE);
    I2C_BufferWrite(array, /*TEDS_START_ADDR*/40, 187);   //write TEDS
	printf("\r\n teds read 1 start\r\n");
	for (i = 0; i < 187; i ++)
	    arr[i] = 0;
	I2C_FRAM_BufferRead(arr, 40, 187);
	for (i = 0; i < 187; i ++)
	    printf("%d ", arr[i]);
	printf("\r\n teds read 1 finished\r\n");
/**/
}

struct Meta_TEDS metaTeds;
struct Phy_TEDS phyTeds;
struct Transducerchannel_TEDS tsTeds[8];

typedef union uMeta
{
    u8 array[META_TEDS_SIZE];
	Meta_TEDS meta;
}uMeta;

typedef union uPhy
{
    u8 array[PHY_TEDS_SIZE];
	Phy_TEDS phy;
}uPhy;

typedef union uTs
{
    u8 array[TS_TEDS_SIZE];
	Transducerchannel_TEDS ts;
}uTs; 

void readTeds()
{	uMeta meta;
    uPhy phy;
	uTs ts[TEDS_TABLE_SIZE - 2];
	u8 i = 0;
	u8 j = 0;
	u8 array[187];
	printf("\r\n teds read start\r\n");
	I2C_FRAM_BufferRead(array, 0x00, 187);
	for (i = 0; i < 187; i ++)
	    printf("%d ", array[i]);
	printf("\r\n teds read finished\r\n");
/*	for (i = 40; i < 59; i ++)
	{
	    phy.array[i - 40] = array[i];
		
		phyTeds = phy.phy;
	}
	for (i = 59; i < 94; i ++)
	{
	    meta.array[i -59] = array[i];
		printf("meta[%d]= %d  array[%d] = %d\r\n", i-59, meta.array[i - 59], i, array[i]);
		metaTeds = meta.meta;
	}
	for (i = 93; i < 187; i ++)
	{
	    ts[0].array[i - 94] = array[i];
		tsTeds[0] = ts[0].ts;
	}
  */ if (tedsTable[0].valid == 1)
	{
	    I2C_FRAM_BufferRead(phy.array, tedsTable[0].addr, tedsTable[0].len);
	//	printf("\r\nphy teds read finished \r\n");
	//	for (i = 0; i < tedsTable[0].len; i ++)
	//	    printf("%d ", phy.array[i]);
		
		phyTeds = phy.phy;
	}
	if (tedsTable[1].valid == 1)
	{
	    I2C_FRAM_BufferRead(meta.array, tedsTable[1].addr, tedsTable[1].len);
		metaTeds = meta.meta;
		printf("\r\nmeta teds read finished \r\n");
		printf("addr = %d len = %d ",tedsTable[1].addr, tedsTable[1].len);
		for (i = 0; i < tedsTable[1].len; i ++)
		    printf("%d ", meta.array[i]);
	}

	for (i = 0; i < TEDS_TABLE_SIZE - 2; i ++)
	{
	    if (tedsTable[i+2].valid == 1)
		{
		    I2C_FRAM_BufferRead(ts[i].array, tedsTable[i + 2].addr, tedsTable[i + 2].len);
			
			tsTeds[i] =  ts[i].ts;
			printf("\r\nts teds read finished \r\n");
			printf("addr = %d len = %d ",tedsTable[i + 2].addr, tedsTable[i + 2].len);
	     	for (j = 0; j < tedsTable[i + 2].len; j ++)
		         printf("%d ", ts[i].array[j]);	
		 //   printf();
		}
	}
 /* */
	// test
	printf("\r\n Teds Reading Finished \r\n");
/*	for (i = 0; i < PHY_TEDS_SIZE; i ++)
	    printf("%d ",phy.array[i]);
	printf("\r\n");
  for (i = 0; i < META_TEDS_SIZE; i ++)
	    printf("%d ", meta.array[i]);
	printf("\r\n");
	for (i = 0; i < TS_TEDS_SIZE; i ++)
	    printf("%d ", ts[0].array[i]);
	printf("\r\n");
*/	printf("phy lengh = %d\r\n", phyTeds.length);
	printf("phy type = %d \r\n", phyTeds.identifier);
	printf("phy reflect delay uncertainty = %f \r\n", phy.phy.reflect_delay_uncertainty);

	//printf("phy lengh = %d\r\n", metaTeds.length);
	//printf("phy type = %d \r\n", metaTeds.identifier);
//	printf("phy reflect delay uncertainty = %f \r\n", phy.phy.reflect_delay_uncertainty);
  printf("ts length = %d \r\n", tsTeds[0].length);
	printf("ts indetifier = %d \r\n", tsTeds[0].identifier);
//	printf("ts direction_angles = %f \r\n", tsTeds[0].direction_angles);
	printf("ts ts_sampling_period = %f \r\n", tsTeds[0].ts_sampling_period);
	
	printf("ts length = %d \r\n", tsTeds[1].length);
	printf("ts indetifier = %d \r\n", tsTeds[1].identifier);
//	printf("ts direction_angles = %f \r\n", tsTeds[1].direction_angles);
	printf("ts ts_sampling_period = %f \r\n", tsTeds[1].ts_sampling_period);
	
	printf("ts length = %d \r\n", tsTeds[2].length);
	printf("ts indetifier = %d \r\n", tsTeds[2].identifier);
//	printf("ts direction_angles = %f \r\n", tsTeds[2].direction_angles);
	printf("ts ts_sampling_period = %f \r\n", tsTeds[2].ts_sampling_period);
}

void startWork()
{
		 u8 light = 1;
		 u16 i = 0;
	   
		 
		 u16 period = 0; 
		 u16 ts0;
		 u16 ts1;
		 u16 ts2;
		 u16 ts0Count = 0;
		 u16 ts1Count = 0;
		 u16 ts2Count = 0;
		 u16 ts0Down = ts0;
		 u16 ts1Down = ts1;
		 u16 ts2Down = ts2;
		 float max = 0;
		 readTedsTable();
		 readTeds();
		 max = tsTeds[0].ts_sampling_period;
		 if (max > tsTeds[1].ts_sampling_period)
			   max = tsTeds[1].ts_sampling_period;
		 if (max > tsTeds[2].ts_sampling_period)
			   max = tsTeds[2].ts_sampling_period;
		 period = max / 100.0;;
		 ts0 = tsTeds[0].ts_sampling_period / 100.0;
		 ts1 = tsTeds[1].ts_sampling_period / 100.0;
		 ts2 = tsTeds[2].ts_sampling_period / 100.0;
		 printf("period = %d\r\n", period);
		 
		 printf("start work/r/n");
		 if (tedsTable[2].valid)
		 {	    printf("start work1/r/n");
			     if (tsTeds[0].ts_type_key == 2)
					 {	   printf("start work2/r/n");
						   while (1)
							 {	  //  printf("light on\r\n");
								 //   GPIO_SetBits(GPIOD , GPIO_Pin_14);	
								//    OSTimeDly(100);
								//	printf("light down\r\n"); 
								//	GPIO_ResetBits(GPIOD , GPIO_Pin_14);
							//		OSTimeDly(100);
                  // 
           //									OSTimeDly(tsTeds[0].ts_sampling_period);
		                       printf("start work2\r\n");
							   printf("ts0Count = %d ts1Count = %d ts2Count = %d\r\n",ts0Count, ts1Count, ts2Count);
							   printf("ts0 = %d ts1 = %d ts2 = %d\r\n",ts0, ts1, ts2);
					           if ((ts0Count % ts0) == 3) 
										{
											 GPIO_SetBits(GPIOD , GPIO_Pin_13);
											 ts0Down = 1;
										}
										
										if ((ts1Count % ts1) == 2) 
										{
											 GPIO_SetBits(GPIOD , GPIO_Pin_14);
											 ts1Down = 1;
										}
										
										if ((ts2Count % ts2) == 1) 
										{
											 GPIO_SetBits(GPIOD , GPIO_Pin_15);
											 ts2Down = 1;
										}
										OSTimeDly(100);
										
										if (ts0Down > 0)
											  ts0Down --;
										if (ts0Down == 0)
											  GPIO_ResetBits(GPIOD , GPIO_Pin_13);
											
										if (ts1Down > 0)
											  ts1Down --;
										if (ts1Down == 0)
											  GPIO_ResetBits(GPIOD , GPIO_Pin_14);
											
										if (ts2Down > 0)
										  	ts2Down --;
										if (ts2Down == 0)
											  GPIO_ResetBits(GPIOD , GPIO_Pin_15);
											
										ts0Count = (ts0Count + 1) % ts0;
										ts1Count = (ts1Count + 1) % ts1;
										ts2Count = (ts2Count + 1) % ts2;
					/*	*/				
					     } 
		       }
     }
}
