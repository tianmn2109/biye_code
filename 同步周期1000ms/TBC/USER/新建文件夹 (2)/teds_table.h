

#ifndef TEDS_TABLE_H
#define TEDS_TABLE_H

#include <includes.h>

#include "ssnp_for_app.h"
 #include "i2c_fram.h"

#define PHY_TEDS_SIZE 19
#define META_TEDS_SIZE 35
#define TS_TEDS_SIZE 10

#pragma pack(1)

typedef struct Meta_TEDS {
    unsigned int length;         // TEDS ���ȣ����ݿ��У��ͣ�
    unsigned char identifier;    // TEDS ��ʶ������־��ΪMeta-TEDS
    unsigned char family;        // 1451Э���壬ֵ�̶�Ϊ3����ʾ1451.3
    unsigned char version;       // TEDS�汾�� 0,2-255 ����  1 ��һ���ٷ��汾

    //ʱ����صĲ���
    float teds_hold_off_time;        // �����������ֵ������Ϊ��λ����һ���ǣ����յ�READ TEDS BLOCK COMMAND �����TEDS���ݵĵ�һ���ֽڷ����������֮�������ӳ�
                                     // �ڶ����ǣ����յ�WRITE TEDS BLOCK COMMAND �����WRITE TEDS BLOCK��REPLY COMMAND�����������֮�������ӳ�
                                     // COMMAND ������ Query��Read��Write��Update
    float operation_hold_off_time;   // ����Ϊ��λ���κβ��Ƕ�TEDS����������Ը������REPLY�ĵ�һ���ֽڱ��ŵ����������

    float sustained_load_current;    // �ӱ��������ߵõ������������ص������԰���Ϊ��λ�����κθ�����Դ��õĵ�������������

    unsigned char multirange_capability; // �����̵������� �����־TBIM�Ƿ������������ڲ�ͬ�����̷�Χ��
                                        // 0 ��������������  1 TBIM�е�һ�����߶��������ͨ���߱������ڶ�����̷�Χ�ڵ����� 2-255 ����
                                        // TEDS������Ҫ����ѡ������ķ�Χ���Ҷ��������ͨ�������������

    //���������
    //��������һЩ������ͨ���ļ��ϣ�����һ������������ͨ�����������Ǵӱ�����ͨ�����ӱ�����ͨ���ṩ������������ͨ���Ķ�����Ϣ����������������������ͨ����ĳЩ����
    //���磺һ���������������������ģ���¼���������ر�����ͨ����һ�����������¼���������ģ����������һ������������ʱ�䴫������ֵ��ִ������һ�������������¼��������ͺ�hysteresis����ִ����
    unsigned char number_of_controlgroup;  //�����������
    unsigned char controlgroup_type;       // ����������ͣ����ֶΰ�������ض������������ͨ��֮��Ĺ�ϵ
                                           // 0���� 1-8 ʹ��   9-17 ����δ����չ  128-255 �������̿���
    unsigned char num_of_ts_in_contrlgroup;   // �������б�����ͨ��������
    unsigned char controlgroup_list;          //??????????????????

    //���������
    //����������һ�����ж�ͨ����TBIM�и�ͨ��֮��Ĺ�ϵ����Щͨ������������ʾ��������Щͨ������֮��Ĺ�ϵ
    //��������������ͨ������������ά������֮��Ĺ�ϵ�������ǵĽ����Ϊ����������ʾ���߽�����ѧ���㡣
    unsigned char number_of_vectorgroups;       // ����������
    unsigned char vectorgroup_type;          // ���������ͣ����ֶΰ�������ض������������ͨ��֮��Ĺ�ϵ
    unsigned char num_of_ts_in_vectorgroup;  // �������б�����ͨ��������
    unsigned char vectorgroup_list;          //??????????????????

    //������ͨ���������
    //������ͨ��������һ������Ľṹ���ɶ��������ͨ����������߶��������ͨ����������ɣ�������ͨ�������п���������д�ı�����ͨ���ŵ���û�б�����ͨ���������ص�
    //������ͨ��ֻ֧������ݵı��������Ȳ����Ǵ�������ִ���������
    //������ϱ�����ͨ���������ݼ��ķ�����block method���Բ�ͬ���ȣ�interleave method ������ͬ����
    unsigned char num_of_ts_proxy;           // ������ͨ�����������
    unsigned char ts_num_of_ts_proxy;        // ���̶����������Ϊ����������ı�����ͨ��������
    unsigned char ts_proxy_data_set_organization;  // �������������ݼ��ķ��� 1 ��״ 2 ������1   3 ������2   3-255 ����
    unsigned char ts_num_of_proxy;                 // ��ǰTBIMӵ�еı�����ͨ�����������
    unsigned char ts__proxy_teds;                //ÿ�ִ������ͱ�����ͨ��������
    unsigned short checksum;
}Meta_TEDS;


#pragma pack()

#pragma pack(1)
typedef struct Phy_TEDS {
    unsigned int length;           // TEDS length
    unsigned char identifier;      // TEDS ����
    unsigned char asynchronous_only_flag;     // ��0: ֻ�ܹ������첽ģʽ  �� ֧��ͬ������
    unsigned char highest_supported_payload_encoding;    // ������յ���֡���б���ֵ�е�payload encoding set���򽫴�֡��������bad frame received ֡״̬λ��һ
    float start_delay;          // ����ʱ��ۿ�ʼ�����������ӳ�(the begining of the priority delay for transmission)��ʼ֮������ʱ����
                                // ��sΪ��λ��ͬ��ģʽ����С��0.000025s�����ڲ�֧��ͬ��ģʽ��TBIM����field��ʹ�ã�����ΪNaN
    float reflect_delay;        // reflect message ���һλ���� �� reply to reply message ���һλ���� ֮���ʱ����

    float reflect_delay_uncertainty;    // reflect delay ��ȷ����  ��sΪ��λ
}Phy_TEDS;

#pragma pack()

#pragma pack(1)
// TransducerChannel
// ����ģ�ʹ�ܱ�����ͨ��Ѱַ
typedef struct Transducerchannel_TEDS {
    unsigned int length;    // TEDS����
    unsigned char identifier;   //  TEDS ������

  
    unsigned char ts_type_key;        // ������ͨ������ 0 ������  1 ִ����   2 �¼�ִ����  3-255 ����

    float ts_sampling_period;              // ������ͨ����������С���ڣ������Ƕ�д�������ڴ�������ִ������������A/D��D/A��ת��������TBIM�Ĵ����ٶȣ�
    
}Transducerchannel_TEDS;
#pragma pack()

#define TEDS_TABLE_ADDR 0x00
#define TEDS_ENTRY_SIZE 4
#pragma pack(1)
typedef struct teds_entry 				 //�洢TEDS�ı���ṹ�嶨��
{
    u8 type;
	u8 valid;
	u8 addr;
	u8 len;
} teds_entry;
#pragma pack()
extern struct Meta_TEDS metaTeds;
extern struct Phy_TEDS phyTeds;
extern struct Transducerchannel_TEDS tsTeds[8];
#define TEDS_TABLE_SIZE 10				  // ���Դ洢TEDS������ ��10�� 1��phy teds	һ��meta teds  ��8�� transducer channel teds
extern teds_entry tedsTable[];   //�洢	teds �ı�������

void writeTedsEntry(struct teds_entry * entry, u8 addr);

void writeTedsTable(void); 

void initTedsTable(void);

void readTedsTable(void);

void readTeds(void);

void writeTeds(u8 * array);

void startWork(void);


#endif // TEDS_TABLE_H
