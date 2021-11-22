/**
  ******************************************************************************
  * 文件名程: ANO.c
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0.0
  * 编写日期: 2021-11-17
  * 功    能: 匿名上位机通讯
  ******************************************************************************
  * 说明：
  * 1.需要基本的串口驱动支持。
  * 
  * 功能：
  * 1.姿态帧。
  * 2.PID帧。
  * 
  * 更新：
  * 
  ******************************************************************************
  */

#include "ANO.h"
	
//需要使用的全局变量
extern Angle_t angle;

//数据缓冲区
uint8_t CycleSendData[64];		//循环发送数据临时缓冲
uint8_t send_buffer[64];	//非循环发送数据临时缓冲

//DataToSend
void ANO_Angle_Transform(int roll, int pitch, int yaw){
    uint8_t _cnt = 0;

    send_buffer[_cnt++] = 0xAA;
	  send_buffer[_cnt++] = 0xff;
	  send_buffer[_cnt++] = 0x03;
	  send_buffer[_cnt++] = 0;

    //roll
    send_buffer[_cnt++] = BYTE0(roll);
    send_buffer[_cnt++] = BYTE1(roll);
    //pitch
    send_buffer[_cnt++] = BYTE0(pitch);
    send_buffer[_cnt++] = BYTE1(pitch);
    //yaw
    send_buffer[_cnt++] = BYTE0(yaw);
    send_buffer[_cnt++] = BYTE1(yaw);
    //state
    send_buffer[_cnt++] = 0x01;
    //length
    send_buffer[3] = _cnt - 4;
    //data check
    uint8_t check_sum1 = 0, check_sum2 = 0;
	for (uint8_t i = 0; i < _cnt; i++)
	{
		check_sum1 += send_buffer[i];
		check_sum2 += check_sum1;
	}
	send_buffer[_cnt++] = check_sum1;
	send_buffer[_cnt++] = check_sum2;

    HAL_UART_Transmit(&huart1, send_buffer, _cnt, 0xfff);
}

void ANO_RC_Transform(int ch_1, int ch_2, int ch_3, int ch_4, int ch_5, int ch_6)
{
  
}
//DataToRecive
