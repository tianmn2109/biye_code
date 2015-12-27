
#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__
#pragma pack(1)		
struct tdcn_teds  //变送器通道TEDS
{
   unsigned char type;//数据类型
   unsigned char sensortrantucer;//变送器还是作动器
   unsigned char tbim;//tbim标号
   unsigned char num;//变送器通道号
   unsigned char datatype;//数据类型，如温度，电压，开关等
};
struct tdcn_value  //变送器通道数据
{
   unsigned char type;//数据类型
   unsigned char tbim;//tbim标号
   unsigned char num;//变送器通道号
   unsigned char data[4];//数据
};
struct tbc_cmd  //上位机向tbc发送的命令
{
   unsigned char tbim;//tbim标号
   unsigned char num;//变送器通道号
   unsigned char cmdclass;//命令类别
   unsigned char cmdfunc;//命令功能
};
CString codeToinfo(int a);
#pragma pack()
#endif 

