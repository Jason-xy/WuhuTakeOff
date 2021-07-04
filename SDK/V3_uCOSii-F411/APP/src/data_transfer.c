#include "data_transfer.h"
#include "gy-86.h"
#include "esp8266.h"
#include <stdio.h>

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

dt_flag_t f;            //需要发送数据的标志
u8 data_to_send[50];    //发送数据缓存 //// bug 没有上锁。。。

//微妙粗略延时
void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
}

//Send_Data函数是协议中所有发送数据功能使用到的发送函数
//移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
void ANO_DT_Send_Data(u8 *dataToSend , u8 length)
{
		//printf("AT+CIPSEND=0,%d\r\n",length);
		//delay_us(200);
    HAL_UART_Transmit(&huart1,dataToSend,length,0xfff);
}

void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
    u8 _cnt=0;
    vs16 _temp;
    vs32 _temp2 = alt;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x05;
		data_to_send[_cnt++]=0xAF;
    data_to_send[_cnt++]=0x01;
    data_to_send[_cnt++]=0x0C;

    _temp = (int)(angle_rol*100);
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = (int)(angle_pit*100);
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = (int)(angle_yaw*100);
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    data_to_send[_cnt++]=BYTE3(_temp2);
    data_to_send[_cnt++]=BYTE2(_temp2);
    data_to_send[_cnt++]=BYTE1(_temp2);
    data_to_send[_cnt++]=BYTE0(_temp2);

    data_to_send[_cnt++] = fly_model;

    data_to_send[_cnt++] = armed;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
    u8 _cnt=0;
    vs16 _temp;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x05;
		data_to_send[_cnt++]=0xAF;
    data_to_send[_cnt++]=0x02;
    data_to_send[_cnt++]=0x12;

    _temp = a_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = a_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = a_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    _temp = g_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = g_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = g_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    _temp = m_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = m_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = m_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++] = sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
    u8 _cnt=0;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x05;
		data_to_send[_cnt++]=0xAF;
    data_to_send[_cnt++]=0x03;
    data_to_send[_cnt++]=0x14;
	
    data_to_send[_cnt++]=BYTE1(thr);
    data_to_send[_cnt++]=BYTE0(thr);
    data_to_send[_cnt++]=BYTE1(yaw);
    data_to_send[_cnt++]=BYTE0(yaw);
    data_to_send[_cnt++]=BYTE1(rol);
    data_to_send[_cnt++]=BYTE0(rol);
    data_to_send[_cnt++]=BYTE1(pit);
    data_to_send[_cnt++]=BYTE0(pit);
    data_to_send[_cnt++]=BYTE1(aux1);
    data_to_send[_cnt++]=BYTE0(aux1);
    data_to_send[_cnt++]=BYTE1(aux2);
    data_to_send[_cnt++]=BYTE0(aux2);
    data_to_send[_cnt++]=BYTE1(aux3);
    data_to_send[_cnt++]=BYTE0(aux3);
    data_to_send[_cnt++]=BYTE1(aux4);
    data_to_send[_cnt++]=BYTE0(aux4);
    data_to_send[_cnt++]=BYTE1(aux5);
    data_to_send[_cnt++]=BYTE0(aux5);
    data_to_send[_cnt++]=BYTE1(aux6);
    data_to_send[_cnt++]=BYTE0(aux6);

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
    u8 _cnt=0;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x05;
		data_to_send[_cnt++]=0xAF;
    data_to_send[_cnt++]=0x02;
    data_to_send[_cnt++]=0x14;

    data_to_send[_cnt++]=BYTE1(m_1);
    data_to_send[_cnt++]=BYTE0(m_1);
    data_to_send[_cnt++]=BYTE1(m_2);
    data_to_send[_cnt++]=BYTE0(m_2);
    data_to_send[_cnt++]=BYTE1(m_3);
    data_to_send[_cnt++]=BYTE0(m_3);
    data_to_send[_cnt++]=BYTE1(m_4);
    data_to_send[_cnt++]=BYTE0(m_4);
    data_to_send[_cnt++]=BYTE1(m_5);
    data_to_send[_cnt++]=BYTE0(m_5);
    data_to_send[_cnt++]=BYTE1(m_6);
    data_to_send[_cnt++]=BYTE0(m_6);
    data_to_send[_cnt++]=BYTE1(m_7);
    data_to_send[_cnt++]=BYTE0(m_7);
    data_to_send[_cnt++]=BYTE1(m_8);
    data_to_send[_cnt++]=BYTE0(m_8);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_Fx_9float(u8 data_to_send[], u8 Fx,
                    float _f1, float _f2, float _f3,
                    float _f4, float _f5, float _f6,
                    float _f7, float _f8, float _f9)
{
    u8 _cnt=0;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=Fx;
    data_to_send[_cnt++]=0;

    data_to_send[_cnt++]=BYTE3(_f1);
    data_to_send[_cnt++]=BYTE2(_f1);
    data_to_send[_cnt++]=BYTE1(_f1);
    data_to_send[_cnt++]=BYTE0(_f1);
    data_to_send[_cnt++]=BYTE3(_f2);
    data_to_send[_cnt++]=BYTE2(_f2);
    data_to_send[_cnt++]=BYTE1(_f2);
    data_to_send[_cnt++]=BYTE0(_f2);
    data_to_send[_cnt++]=BYTE3(_f3);
    data_to_send[_cnt++]=BYTE2(_f3);
    data_to_send[_cnt++]=BYTE1(_f3);
    data_to_send[_cnt++]=BYTE0(_f3);

    data_to_send[_cnt++]=BYTE3(_f4);
    data_to_send[_cnt++]=BYTE2(_f4);
    data_to_send[_cnt++]=BYTE1(_f4);
    data_to_send[_cnt++]=BYTE0(_f4);
    data_to_send[_cnt++]=BYTE3(_f5);
    data_to_send[_cnt++]=BYTE2(_f5);
    data_to_send[_cnt++]=BYTE1(_f5);
    data_to_send[_cnt++]=BYTE0(_f5);
    data_to_send[_cnt++]=BYTE3(_f6);
    data_to_send[_cnt++]=BYTE2(_f6);
    data_to_send[_cnt++]=BYTE1(_f6);
    data_to_send[_cnt++]=BYTE0(_f6);

    data_to_send[_cnt++]=BYTE3(_f7);
    data_to_send[_cnt++]=BYTE2(_f7);
    data_to_send[_cnt++]=BYTE1(_f7);
    data_to_send[_cnt++]=BYTE0(_f7);
    data_to_send[_cnt++]=BYTE3(_f8);
    data_to_send[_cnt++]=BYTE2(_f8);
    data_to_send[_cnt++]=BYTE1(_f8);
    data_to_send[_cnt++]=BYTE0(_f8);
    data_to_send[_cnt++]=BYTE3(_f9);
    data_to_send[_cnt++]=BYTE2(_f9);
    data_to_send[_cnt++]=BYTE1(_f9);
    data_to_send[_cnt++]=BYTE0(_f9);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
