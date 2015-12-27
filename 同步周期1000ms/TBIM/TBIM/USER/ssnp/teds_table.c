
#include "teds_table.h"

/*
  ���ͣ� ������
 	���ܣ� ������teds entry ������֮�����ת����ͨ��������EEPROM��д��
 */
typedef union TedsEntry
{	  u8t array[TEDS_ENTRY_SIZE];
    struct teds_entry entry;
} TedsEntry; 

/*
  ���ܣ� ��EEPROMָ����ַ��д��һ��teds entry����
	����: @entry Ҫд���teds entry �ṹ��
	������@addr Ҫд��ĵ�ַ
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
  ���ܣ���EEPROM��д������teds entry����
	����: NULL
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
   ����TEDS��ѯ��
*/
teds_entry tedsTable[TEDS_TABLE_SIZE];


/*
  ���ܣ� ������ʼ��TEDS TABLE �� ��ʼ��Ӧ��EEPROM�ж�ȡTEDS TABLE�� ���������� Ҫʵ�ִ�EEPROM�ж�ȡ�����г�ʼ���Ĺ���
  ������ NULL
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
		//ts teds�� ��ΪĿǰ������������������� ����ǰ����ͨ��teds��Ч
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
   ���ܣ� �û���ȡTEDS table �� Ŀǰ������֤д���TEDS table�Ƿ���ȷ�� �޸�֮������ڳ�ʼ��ʱ��EEPROM�ж�ȡ���ݳ�ʼ��TEDS TABEL
   ������ @test �����������õ�teds table entry
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
    ���ܣ� д������TEDS
		������  @array, Ҫд�������
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

META_TEDS metaTeds;    // ����ԪTEDS 
Phy_TEDS phyTeds;      // ��������TEDS
Transducerchannel_TEDS tsTeds[TS_TEDS_NUM];    // ����ͨ��TEDS

/*
  ���ͣ� ������
	���ܣ� ��ԪTEDSת��������array�� ͨ�����齫��д��EEPROM��
*/
typedef union uMeta
{
    u8 array[META_TEDS_SIZE];
	  META_TEDS meta;
}uMeta;

/*
  ���ͣ� ������
	���ܣ� ������TEDSת��������array�� ͨ�����齫��д��EEPROM��
*/
typedef union uPhy
{
    u8 array[PHY_TEDS_SIZE];
	  Phy_TEDS phy;
}uPhy;

/*
  ���ͣ� ������
	���ܣ� ��ͨ��TEDSת��������array�� ͨ�����齫��д��EEPROM��
*/
typedef union uTs
{
      u8 array[TS_TEDS_SIZE];
	  Transducerchannel_TEDS ts;
}uTs; 

/*
    ���ܣ� ��EEPROMָ����ַд������TEDS
		������ @phy Ҫд�������teds�ṹ��
		����:  @addr Ҫд��ĵ�ַ
 */
void writePhyTeds(Phy_TEDS *phy, u16 addr)
{
	 uPhy u;
   u.phy = *phy;
   I2C_BufferWrite(u.array, addr, PHY_TEDS_SIZE);
}

/*
    ���ܣ� ��EEPROMָ����ַд��ԪTEDS
		������ @meta Ҫд���Ԫteds�ṹ��
		����:  @addr Ҫд��ĵ�ַ
 */
void writeMetaTeds(META_TEDS *meta, u16 addr)
{
	  uMeta u;
		u.meta = *meta;
		I2C_BufferWrite(u.array, addr, META_TEDS_SIZE);
}
/*
    ���ܣ� ��EEPROMָ����ַд���ͨ��TEDS
		������ @ts Ҫд���ͨ��teds�ṹ��
		����:  @addr Ҫд��ĵ�ַ
 */
void writeTsTeds(Transducerchannel_TEDS *ts, u16 addr)
{
	  uTs u;
		u.ts = *ts;
		I2C_BufferWrite(u.array, addr, TS_TEDS_SIZE);
}

/*
    ���ܣ� ��EEPROMָ����ַ��ȡԪTEDS
		������ @meta Ҫ��ȡ��Ԫteds�ṹ��
		����:  @addr Ҫ��ȡ�ĵ�ַ
 */
void readPhyTeds(u16 addr, Phy_TEDS *phy)
{
	  uPhy u;
	  I2C_FRAM_BufferRead(u.array, addr, PHY_TEDS_SIZE);
	  *phy = u.phy;
}

/*
    ���ܣ� ��EEPROMָ����ȡд��Meta Teds
		������ @meta Ҫ��ȡ��Meta Teds�ṹ��
		����:  @addr Ҫ��ȡ�ĵ�ַ
 */
void readMetaTeds(u16 addr, META_TEDS *meta)
{
	  uMeta u;
		I2C_FRAM_BufferRead(u.array, addr, META_TEDS_SIZE);
		*meta = u.meta;
}

/*
    ���ܣ� ��EEPROMָ����ַ��ȡ��ͨ��TEDS
		������ @ts Ҫ��ȡ��ͨ��teds�ṹ��
		����:  @addr Ҫ��ȡ�ĵ�ַ
 */
void readTsTeds(u16 addr, Transducerchannel_TEDS *ts)
{
	    uTs u;
		I2C_FRAM_BufferRead(u.array, addr, TS_TEDS_SIZE);
		*ts = u.ts;
}

/*
    ���ܣ� ��ȡ��Ч��ͨ��TEDS�ĸ���
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
   ���ܣ� ��ȡÿ����Ч��ͨ��TEDS�ı�ţ� �����һ��Ϊ���� �ڶ���Ϊ��С��ͨ���ţ��Դ�����
	 ������ @array �洢��Чͨ���ĸ�����ͨ�����
	 ������ @numOfValidID ��Чͨ���ĸ���
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
    ���ܣ� ����Teds table�Ƿ���Ч ��ȡTEDS�ṹ��
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
