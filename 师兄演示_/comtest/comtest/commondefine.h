
#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__
#pragma pack(1)		
struct tdcn_teds  //������ͨ��TEDS
{
   unsigned char type;//��������
   unsigned char sensortrantucer;//����������������
   unsigned char tbim;//tbim���
   unsigned char num;//������ͨ����
   unsigned char datatype;//�������ͣ����¶ȣ���ѹ�����ص�
};
struct tdcn_value  //������ͨ������
{
   unsigned char type;//��������
   unsigned char tbim;//tbim���
   unsigned char num;//������ͨ����
   unsigned char data[4];//����
};
struct tbc_cmd  //��λ����tbc���͵�����
{
   unsigned char tbim;//tbim���
   unsigned char num;//������ͨ����
   unsigned char cmdclass;//�������
   unsigned char cmdfunc;//�����
};
CString codeToinfo(int a);
#pragma pack()
#endif 

