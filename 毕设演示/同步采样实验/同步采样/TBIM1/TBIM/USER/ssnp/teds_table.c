
#include "teds_table.h"

/*
  类型： 共用体
 	功能： 用于在teds entry 和数组之间进行转化，通过数组向EEPROM中写入
 */
typedef union TedsEntry
{	  u8t array[TEDS_ENTRY_SIZE];
    struct teds_entry entry;
} TedsEntry; 

/*
  功能： 向EEPROM指定地址中写入一个teds entry表项
	参数: @entry 要写入的teds entry 结构体
	参数：@addr 要写入的地址
 */
void writeTedsEntry(struct teds_entry * entry, u16 addr)
{
	u8 i = 0;
    TedsEntry uEntry;
	uEntry.entry = *entry;
    printf("writing teds entry\r\n");
	for (i = 0; i < 4; i ++)
	{	   
		printf(" addr = %d uEntry[%d] = %d\r\n", addr, i, uEntry.array[i]);
	}
	printf("\r\n");
  /* */
  // for (i = 0; i < 4; i ++)
    //   printf("entry: %d %d %d %d", uEntry.array[0], uEntry.array[1], uEntry.array[2], uEntry.array[3]);
	I2C_BufferWrite(uEntry.array, addr, TEDS_ENTRY_SIZE);

}

/*
  功能：向EEPROM中写入所有teds entry表项
	参数: NULL
 */
void writeTedsTable()
{
   u8 i = 0;
   u8 array[40];
   u8 k = 0;
   for (i = 0; i < TEDS_TABLE_SIZE; i ++)
   {
      writeTedsEntry(&tedsTable[i], TEDS_TABLE_ADDR + i * TEDS_ENTRY_SIZE);		
   }   
   I2C_FRAM_BufferRead(array, TEDS_TABLE_ADDR, 40);
   for (i = 0; i < 40; i ++)
       printf("%d ", array[i]);
   printf("\r\n");
}
/*
   定义TEDS查询表
*/
teds_entry tedsTable[TEDS_TABLE_SIZE];


/*
  功能： 用来初始化TEDS TABLE 表， 初始化应从EEPROM中读取TEDS TABLE， 本例做测试 要实现从EEPROM中读取来进行初始化的功能
  参数： NULL
 */
void initTedsTable()
{
    u8 i = 0;
  	//PHY TEDS entry
    tedsTable[0].type = 1;
    tedsTable[0].valid = 0; 
   	tedsTable[0].addr = (u16)PHY_TEDS_ADDR;
  	tedsTable[0].len = (u16)PHY_TEDS_SIZE;
    //Meta TEDS entry	
	  tedsTable[1].type = 2;
    tedsTable[1].valid = 0; 
	  tedsTable[1].addr = (u16)META_TEDS_ADDR;
	  tedsTable[1].len = (u16)META_TEDS_SIZE;
		//ts teds， 因为目前开发板上面接了三个灯 所以前三个通道teds有效
	//	printf("PHY_TEDS_ADDR = %d PHY_TEDS_SIZE = %d\r\n", PHY_TEDS_ADDR, PHY_TEDS_SIZE);
	//	printf("META_TEDS_ADDR = %d META_TEDS_SIZE = %d\r\n", META_TEDS_ADDR, META_TEDS_SIZE);
	  for (i = 0; i < TEDS_TABLE_SIZE - 2; i ++)
	  {
		  tedsTable[i + 2].type = 3;
		  if (i < 3)
      	    tedsTable[i + 2].valid = 0; 
		  else
		  	tedsTable[i + 2].valid = 0;
						
	  	tedsTable[i + 2].addr = (u16)(TS_TEDS_ADDR + TS_TEDS_SIZE * i);
		  tedsTable[i + 2].len = (u16)TS_TEDS_SIZE;
	  }   

}
/*
   功能： 用户读取TEDS table 表， 目前用于验证写入的TEDS table是否正确， 修改之后可用于初始化时从EEPROM中读取数据初始化TEDS TABEL
   参数： @test 用于做测试用的teds table entry
 */
void readTedsTable(void)
{
    u8 i = 0;
		teds_entry test[TEDS_TABLE_SIZE]; 
	  for (i = 0 ; i < TEDS_TABLE_SIZE; i ++)
	  {
	      I2C_FRAM_BufferRead((u8 *)&tedsTable[i], TEDS_TABLE_ADDR + i * TEDS_ENTRY_SIZE, TEDS_ENTRY_SIZE);
		  printf("reading addr = %d\r\n", TEDS_TABLE_ADDR + i * TEDS_ENTRY_SIZE);
		    test[i] = tedsTable[i];
   	}
	  printf("Reading teds table\r\n");
	  for (i = 0; i < TEDS_TABLE_SIZE; i ++)
	      printf("teds entry %d :   type = %d  valid = %d addr = %d  len = %d\r\n", i, test[i].type, test[i].valid, test[i].addr, test[i].len);
}

/*
    功能： 写入整个TEDS
		参数：  @array, 要写入的数据
 */
void writeTeds(u8 * array, u16 num)
{	  u8 i = 0;
    u8 arr[187];
//	for (i = 0; i < 187; i ++)
//	    arr[i] = i + 4;
	printf("recv_teds\r\n");
	for (i = 0; i < num; i ++)
	    printf("%d ", array[i]);
	printf("TEDS_START_ADDR = %d  TEDS_SIZE = %d  \r\n", TEDS_START_ADDR, num);
    I2C_BufferWrite(array, /*TEDS_START_ADDR*/TEDS_START_ADDR, num);   //write TEDS
	printf("\r\n teds read 1 start\r\n");
	for (i = 0; i < num; i ++)
	    arr[i] = 0;
	I2C_FRAM_BufferRead(arr, TEDS_START_ADDR, num);
	for (i = 0; i < num; i ++)
	    printf("%d ", arr[i]);
	printf("\r\n teds read 1 finished\r\n");
/**/
}

META_TEDS metaTeds;    // 定义元TEDS 
Phy_TEDS phyTeds;      // 定义物理TEDS
Transducerchannel_TEDS tsTeds[TS_TEDS_NUM];    // 定义通道TEDS

/*
  类型： 共用体
	功能： 将元TEDS转化成数组array， 通过数组将其写入EEPROM中
*/
typedef union uMeta
{
    u8 array[META_TEDS_SIZE];
	  META_TEDS meta;
}uMeta;

/*
  类型： 共用体
	功能： 将物理TEDS转化成数组array， 通过数组将其写入EEPROM中
*/
typedef union uPhy
{
    u8 array[PHY_TEDS_SIZE];
	  Phy_TEDS phy;
}uPhy;

/*
  类型： 共用体
	功能： 将通道TEDS转化成数组array， 通过数组将其写入EEPROM中
*/
typedef union uTs
{
      u8 array[TS_TEDS_SIZE];
	  Transducerchannel_TEDS ts;
}uTs; 

/*
    功能： 向EEPROM指定地址写入物理TEDS
		参数： @phy 要写入的物理teds结构体
		参数:  @addr 要写入的地址
 */
void writePhyTeds(Phy_TEDS *phy, u16 addr)
{
	 uPhy u;
   u.phy = *phy;
   I2C_BufferWrite(u.array, addr, PHY_TEDS_SIZE);
}

/*
    功能： 向EEPROM指定地址写入元TEDS
		参数： @meta 要写入的元teds结构体
		参数:  @addr 要写入的地址
 */
void writeMetaTeds(META_TEDS *meta, u16 addr)
{
	  uMeta u;
		u.meta = *meta;
		I2C_BufferWrite(u.array, addr, META_TEDS_SIZE);
}
/*
    功能： 向EEPROM指定地址写入的通道TEDS
		参数： @ts 要写入的通道teds结构体
		参数:  @addr 要写入的地址
 */
void writeTsTeds(Transducerchannel_TEDS *ts, u16 addr)
{
	  uTs u;
		u.ts = *ts;
		I2C_BufferWrite(u.array, addr, TS_TEDS_SIZE);
}

/*
    功能： 向EEPROM指定地址读取元TEDS
		参数： @meta 要读取的元teds结构体
		参数:  @addr 要读取的地址
 */
void readPhyTeds(u16 addr, Phy_TEDS *phy)
{
	  uPhy u;
	  I2C_FRAM_BufferRead(u.array, addr, PHY_TEDS_SIZE);
	  *phy = u.phy;
}

/*
    功能： 向EEPROM指定读取写入Meta Teds
		参数： @meta 要读取的Meta Teds结构体
		参数:  @addr 要读取的地址
 */
void readMetaTeds(u16 addr, META_TEDS *meta)
{
	  uMeta u;
		I2C_FRAM_BufferRead(u.array, addr, META_TEDS_SIZE);
		*meta = u.meta;
}

/*
    功能： 向EEPROM指定地址读取的通道TEDS
		参数： @ts 要读取的通道teds结构体
		参数:  @addr 要读取的地址
 */
void readTsTeds(u16 addr, Transducerchannel_TEDS *ts)
{
	    uTs u;
		I2C_FRAM_BufferRead(u.array, addr, TS_TEDS_SIZE);
		*ts = u.ts;
}

/*
    功能： 获取有效的通道TEDS的个数
*/
u8 getValidTsNUm(void)
{
	  u8 i = 0;
		u8 num = 0;
		for (i = 0; i < TEDS_TABLE_SIZE; i ++)
		    if (tedsTable[i].valid == 1)
				    num ++;
		return num;
}

/* 
   功能： 获取每个有效的通道TEDS的编号， 数组第一个为个数 第二个为最小的通道号，以此类推
	 参数： @array 存储有效通道的个数和通道编号
	 参数： @numOfValidID 有效通道的个数
 */
void getValidTsID(u8 *array, u8 numOfValidIDPlusOne)
{
	  u8 i = 0;
		u8 k = 0;
		array[k ++] = numOfValidIDPlusOne;
		for (i = 0; i < TEDS_TABLE_SIZE; i ++)
		    if (tedsTable[i].valid == 1)
					  array[k ++] = i;
}

/*
    功能： 根据Teds table是否有效 读取TEDS结构体
 */
void readTeds(void)
{
	  uMeta meta;
    uPhy phy;
	  uTs ts[TEDS_TABLE_SIZE - 2];
	  u8 i = 0;
	  u8 j = 0;
	  u8 array[187];
	  printf("\r\n teds read start\r\n");
	  I2C_FRAM_BufferRead(array, TEDS_START_ADDR, TEDS_SIZE);
	  for (i = 0; i < TEDS_SIZE; i ++)
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
		readPhyTeds(tedsTable[0].addr, &phyTeds);
	  //  I2C_FRAM_BufferRead(phy.array, tedsTable[0].addr, tedsTable[0].len);
	//	printf("\r\nphy teds read finished \r\n");
	//	for (i = 0; i < tedsTable[0].len; i ++)
	//	    printf("%d ", phy.array[i]);
		
		  phyTeds = phy.phy;
	}
	if (tedsTable[1].valid == 1)
	{
		readMetaTeds(tedsTable[1].addr, &metaTeds);
	/*    I2C_FRAM_BufferRead(meta.array, tedsTable[1].addr, tedsTable[1].len);
		  metaTeds = meta.meta;
		  printf("\r\nmeta teds read finished \r\n");
		  printf("addr = %d len = %d ",tedsTable[1].addr, tedsTable[1].len);
	  	for (i = 0; i < tedsTable[1].len; i ++)
		      printf("%d ", meta.array[i]);
	*/
	}

	for (i = 0; i < TEDS_TABLE_SIZE - 2; i ++)
	{
	    if (tedsTable[i+2].valid == 1)
		  {
				  readTsTeds(tedsTable[i + 2].addr, &(tsTeds[i]));
		   /*   I2C_FRAM_BufferRead(ts[i].array, tedsTable[i + 2].addr, tedsTable[i + 2].len);
			
		    	tsTeds[i] =  ts[i].ts;
		    	printf("\r\nts teds read finished \r\n");
		    	printf("addr = %d len = %d ",tedsTable[i + 2].addr, tedsTable[i + 2].len);
	       	for (j = 0; j < tedsTable[i + 2].len; j ++)
		           printf("%d ", ts[i].array[j]);	
		 *///   printf();
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
//		 u8 light = 1;
//		 u16 i = 0;
	   
		 
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
