

#ifndef TEDS_TABLE_H
#define TEDS_TABLE_H

#include <includes.h>

#include "ssnp_for_app.h"
 #include "i2c_fram.h"

#pragma pack(1)

typedef struct META_TEDS {
    unsigned int length;         // TEDS 长度（数据块和校验和）
    unsigned char identifier;    // TEDS 标识符，标志其为Meta-TEDS
    unsigned char family;        // 1451协议族，值固定为3，表示1451.3
    unsigned char version;       // TEDS版本号 0,2-255 保留  1 第一个官方版本

    //时间相关的参数
    float teds_hold_off_time;        // 包括两个最大值，以秒为单位，第一个是，接收到READ TEDS BLOCK COMMAND 命令到将TEDS内容的第一个字节放入物理介质之间的最大延迟
                                     // 第二个是，接收到WRITE TEDS BLOCK COMMAND 命令到将WRITE TEDS BLOCK的REPLY COMMAND放入物理介质之间的最大延迟
                                     // COMMAND 可以是 Query，Read，Write，Update
    float operation_hold_off_time;   // 以秒为单位，任何不是对TEDS操作的命令到对该命令的REPLY的第一个字节被放到物理介质上

    float sustained_load_current;    // 从变送器总线得到的最大持续负载电流，以安培为单位，从任何辅助电源获得的电流不包括在内

    unsigned char multirange_capability; // 多量程的能力， 此项标志TBIM是否有能力工作在不同的量程范围内
                                        // 0 不存在这种能力  1 TBIM中的一个或者多个变送器通道具备操作在多个量程范围内的能力 2-255 保留
                                        // TEDS命令需要定义选择操作的范围并且定义变送器通道具有这个能力

    //控制组相关
    //控制组是一些变送器通道的集合，其中一个是主变送器通道，其他的是从变送器通道，从变送器通道提供关于主变送器通道的额外信息，或者用来控制主变送器通道的某些方面
    //例如：一个控制组有三个额外的与模拟事件传感器相关变送器通道，一个用来测量事件传感器的模拟输入量，一个是用来设置时间传感器阈值的执行器，一个是用来设置事件传感器滞后（hysteresis）的执行器
    unsigned char number_of_controlgroup;  //控制组的数量
    unsigned char controlgroup_type;       // 控制组的类型，此字段包含组成特定控制组变送器通道之间的关系
                                           // 0保留 1-8 使用   9-17 保留未来扩展  128-255 对制造商开放
    unsigned char num_of_ts_in_contrlgroup;   // 控制组中变送器通道的数量
    unsigned char controlgroup_list;          //??????????????????

    //向量组相关
    //向量组描述一个含有多通道的TBIM中各通道之间的关系，这些通道可以用来显示，或者这些通道算数之间的关系
    //比如三个变送器通道可以描述三维坐标轴之间的关系，将它们的结果作为向量进行显示或者进行数学计算。
    unsigned char number_of_vectorgroups;       // 向量组数量
    unsigned char vectorgroup_type;          // 向量组类型，此字段包含组成特定向量组变送器通道之间的关系
    unsigned char num_of_ts_in_vectorgroup;  // 向量组中变送器通道的数量
    unsigned char vectorgroup_list;          //??????????????????

    //变送器通道代理相关
    //变送器通道代理是一个虚拟的结构，由多个变送器通道的输出或者多个变送器通道的输入组成，变送器通道代理有可以用来读写的变送器通道号但是没有变送器通道的其他特点
    //变送器通道只支持相兼容的变送器，既不能是传感器和执行器的组合
    //两种组合变送器通道代理数据集的方法：block method可以不同长度，interleave method 必须相同长度
    unsigned char num_of_ts_proxy;           // 变送器通道代理的数量
    unsigned char ts_num_of_ts_proxy;        // 厂商定义的用来作为变送器代理的变送器通道的数量
    unsigned char ts_proxy_data_set_organization;  // 定义代理组合数据集的方法 1 块状 2 交错方法1   3 交错方法2   3-255 保留
    unsigned char ts_num_of_proxy;                 // 当前TBIM拥有的变送器通道代理的数量
    unsigned char ts__proxy_teds;                //每种代理类型变送器通道的数量
    unsigned short checksum;
}META_TEDS;


#pragma pack()

#pragma pack(1)
typedef struct Phy_TEDS {
    unsigned int length;           // TEDS length
    unsigned char identifier;      // TEDS 类型
    unsigned char asynchronous_only_flag;     // 非0: 只能工作在异步模式  ； 支持同步操作
    unsigned char highest_supported_payload_encoding;    // 如果接收到的帧含有保留值中的payload encoding set，则将此帧丢弃并将bad frame received 帧状态位置一
    float start_delay;          // 分配时间槽开始到传输优先延迟(the begining of the priority delay for transmission)开始之间的最大时间间隔
                                // 以s为单位，同步模式必须小于0.000025s，对于不支持同步模式的TBIM，此field不使用，设置为NaN
    float reflect_delay;        // reflect message 最后一位到达 到 reply to reply message 最后一位传输 之间的时间间隔

    float reflect_delay_uncertainty;    // reflect delay 不确定性  以s为单位
	unsigned short checksum;
}Phy_TEDS;

#pragma pack()

#pragma pack(1)
// TransducerChannel
// 必须的，使能变送器通道寻址
typedef struct Transducerchannel_TEDS {
    unsigned int length;    // TEDS长度
    unsigned char identifier;   //  TEDS 的类型

  
    unsigned char ts_type_key;        // 变送器通道类型 0 传感器  1 执行器   2 事件执行器  3-255 保留

    float ts_sampling_period;              // 变送器通道采样的最小周期（不考虑读写），对于传感器和执行器，受限于A/D和D/A的转换次数，TBIM的处理速度，
    
}Transducerchannel_TEDS;
#pragma pack()

#pragma pack(1)
typedef struct teds_entry 				 //存储TEDS的表项结构体定义
{
  u8 type;
	u8 valid;
	u8 addr;
	u8 len;
} teds_entry;
#pragma pack()

#define UUID_ADDR 0x00
#define TEDS_TABLE_ADDR 0x10
#define TEDS_ENTRY_SIZE sizeof(struct teds_entry)

#define PHY_TEDS_SIZE sizeof(struct Phy_TEDS)
#define META_TEDS_SIZE sizeof(struct META_TEDS)
#define TS_TEDS_SIZE sizeof(struct Transducerchannel_TEDS)


// TBIM 支持存放的变送器通道TEDS，META TEDS， PHY TEDS的个数
#define TS_TEDS_NUM 8    // 这个值以后要变大
#define META_TEDS_NUM 1
#define PHY_TEDS_NUM 1
// 所存储的各种类型的TEDS的总的个数
#define TOTAL_TEDS_NUM (TS_TEDS_NUM + META_TEDS_NUM + PHY_TEDS_NUM)
// teds table 中表项的个数等于总的TEDS的个数
#define TEDS_TABLE_SIZE TOTAL_TEDS_NUM

// 物理TEDS的起始地址， 紧挨在teds table之后
#define PHY_TEDS_ADDR (TEDS_TABLE_ADDR + TEDS_TABLE_SIZE * TEDS_ENTRY_SIZE)
// 元Teds地址，紧挨在物理teds之后
#define META_TEDS_ADDR (PHY_TEDS_ADDR + PHY_TEDS_SIZE)
//通道teds地址，紧挨在元teds之后
#define TS_TEDS_ADDR (META_TEDS_ADDR + META_TEDS_SIZE)

#define TEDS_START_ADDR  PHY_TEDS_ADDR    // TEDS在EEPROM中的开始地址
#define TEDS_SIZE (PHY_TEDS_SIZE + META_TEDS_SIZE + TS_TEDS_SIZE * 3)  // 整个TEDS的大小 目前仅写入三个TS_TEDS_SIZE


extern META_TEDS metaTeds;
extern Phy_TEDS phyTeds;
extern Transducerchannel_TEDS tsTeds[TS_TEDS_NUM];

extern teds_entry tedsTable[TEDS_TABLE_SIZE];   //存储	teds 的表项数组

void writeTedsEntry(struct teds_entry * entry, u16 addr);//写一个teds entry表项

void writeTedsTable(void); //写整个teds table 表

void initTedsTable(void);// 初始化Teds table 表

void readTedsTable(void);//读TEDS table 表

void readTeds(void);//读TEDS

void writeTeds(u8 * array, u16 num);//写TEDS

void writePhyTeds(Phy_TEDS *phy, u16 addr);  //向EEPROM指定地址写入物理TEDS

void writeMetaTeds(META_TEDS *meta, u16 addr);//向EEPROM指定地址写入元TEDS

void writeTsTeds(Transducerchannel_TEDS *ts, u16 addr);//向EEPROM指定地址写入通道TEDS

void readPhyTeds(u16 addr, Phy_TEDS *phy);  //向EEPROM指定地址读取物理TEDS

void readMetaTeds(u16 addr, META_TEDS *meta);//向EEPROM指定地址读取元TEDS

void readTsTeds(u16 addr, Transducerchannel_TEDS *ts);//向EEPROM指定地址读取通道TEDS

void startWork(void); // 测试用

u8 getValidTsNum(void); // 获取有效的通道TEDS的个数

void getValidTsID(u8 *array, u8 numOfValidIDPlusOne); // 获取每个有效的通道TEDS的编号， 数组第一个为个数 第二个为最小的通道号，以此类推





#endif // TEDS_TABLE_H
