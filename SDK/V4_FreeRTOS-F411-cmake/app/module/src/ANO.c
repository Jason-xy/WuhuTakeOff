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
uint8_t CycleSendData[64]; //循环发送数据临时缓冲
uint8_t send_buffer[64];   //非循环发送数据临时缓冲

//DataToSend
//欧拉角
void ANO_Angle_Transform(int roll, int pitch, int yaw)
{
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
//遥控器信号
void ANO_RC_Transform(int ch_1, int ch_2, int ch_3, int ch_4, int ch_5, int ch_6)
{
  uint8_t _cnt = 0;

  int roll = (ch_1 - 0.5) * 20 * 100;  //D
  int pitch = (ch_2 - 0.5) * 20 * 100; //D
  int thr = ch_3;
  int yaw = (ch_4 - 0.5) * 10 * 100; //DPS

  send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0xf1;
  send_buffer[_cnt++] = 12;

  //roll
  send_buffer[_cnt++] = BYTE0(roll);
  send_buffer[_cnt++] = BYTE1(roll);
  //pitch
  send_buffer[_cnt++] = BYTE0(pitch);
  send_buffer[_cnt++] = BYTE1(pitch);
  //thr
  send_buffer[_cnt++] = BYTE0(thr);
  send_buffer[_cnt++] = BYTE1(thr);
  //yaw
  send_buffer[_cnt++] = BYTE0(yaw);
  send_buffer[_cnt++] = BYTE1(yaw);
  //ch5
  send_buffer[_cnt++] = BYTE0(ch_5);
  send_buffer[_cnt++] = BYTE1(ch_5);
  //ch6
  send_buffer[_cnt++] = BYTE0(ch_6);
  send_buffer[_cnt++] = BYTE1(ch_6);

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

//电机输出
void ANO_MotorOut(int ch_1, int ch_2, int ch_3, int ch_4)
{
  uint8_t _cnt = 0;

  send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0xf2;
  send_buffer[_cnt++] = 8;

  //roll
  send_buffer[_cnt++] = BYTE0(ch_1);
  send_buffer[_cnt++] = BYTE1(ch_1);
  //pitch
  send_buffer[_cnt++] = BYTE0(ch_2);
  send_buffer[_cnt++] = BYTE1(ch_2);
  //thr
  send_buffer[_cnt++] = BYTE0(ch_3);
  send_buffer[_cnt++] = BYTE1(ch_3);
  //yaw
  send_buffer[_cnt++] = BYTE0(ch_4);
  send_buffer[_cnt++] = BYTE1(ch_4);

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

//GY86原始数据
void ANO_GY86_RAW(int Accel_x, int Accel_y, int Accel_z, int Gyro_x, int Gyro_y, int Gyro_z)
{
  uint8_t _cnt = 0;

  send_buffer[_cnt++] = 0xAA;
  send_buffer[_cnt++] = 0xff;
  send_buffer[_cnt++] = 0x01;
  send_buffer[_cnt++] = 13;

  //Accel_x
  send_buffer[_cnt++] = BYTE0(Accel_x);
  send_buffer[_cnt++] = BYTE1(Accel_x);
  //Accel_y
  send_buffer[_cnt++] = BYTE0(Accel_y);
  send_buffer[_cnt++] = BYTE1(Accel_y);
  //Accel_z
  send_buffer[_cnt++] = BYTE0(Accel_z);
  send_buffer[_cnt++] = BYTE1(Accel_z);
  //Gyro_x
  send_buffer[_cnt++] = BYTE0(Gyro_x);
  send_buffer[_cnt++] = BYTE1(Gyro_x);
  //Gyro_y
  send_buffer[_cnt++] = BYTE0(Gyro_y);
  send_buffer[_cnt++] = BYTE1(Gyro_y);
  //Gyro_z
  send_buffer[_cnt++] = BYTE0(Gyro_z);
  send_buffer[_cnt++] = BYTE1(Gyro_z);
  //SHOCK
  send_buffer[_cnt++] = 0;

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

//DataToRecive
uint8_t _datatemp[UART1_Rx_MAX];
uint8_t UART1_temp[REC_LENGTH];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  static u8 _data_len = 0, _data_cnt = 0;
  static u8 rxstate = 0;
  if (huart->Instance == USART1)
  {
    if (rxstate == 0 && UART1_temp[0] == 0xAA)
    {
      rxstate = 1;
      _datatemp[0] = UART1_temp[0];
    }
    else if (rxstate == 1 && (UART1_temp[0] == HW_TYPE || UART1_temp[0] == HW_ALL))
    {
      rxstate = 2;
      _datatemp[1] = UART1_temp[0];
    }
    else if (rxstate == 2)
    {
      rxstate = 3;
      _datatemp[2] = UART1_temp[0];
    }
    else if (rxstate == 3 && UART1_temp[0] < 250)
    {
      rxstate = 4;
      _datatemp[3] = UART1_temp[0];
      _data_len = UART1_temp[0];
      _data_cnt = 0;
    }
    else if (rxstate == 4 && _data_len > 0)
    {
      _data_len--;
      _datatemp[4 + _data_cnt++] = UART1_temp[0];
      if (_data_len == 0)
        rxstate = 5;
    }
    else if (rxstate == 5)
    {
      rxstate = 6;
      _datatemp[4 + _data_cnt++] = UART1_temp[0];
    }
    else if (rxstate == 6)
    {
      rxstate = 0;
      _datatemp[4 + _data_cnt] = UART1_temp[0];
      //		DT_data_cnt = _data_cnt+5;
      //
      Ano_DataAnl(_datatemp, _data_cnt + 5); //
    }
    else
    {
      rxstate = 0;
    }
  }
  HAL_UART_Receive_IT(&huart1,(uint8_t *)UART1_temp, 1);
}

void Ano_DataAnl(uint8_t *data, int length)
{
  ;
}
