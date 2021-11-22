/**
  ******************************************************************************
  * 文件名程: ANO.h 
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

#ifndef __ANO_H__
#define __ANO_H__
#include "usart.h"
#include "datafusion.h"

//需要使用的宏
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
	
void ANO_Angle_Transform(int roll, int pitch, int yaw);
void ANO_RC_Transform(int ch_1, int ch_2, int ch_3, int ch_4, int ch_5, int ch_6);

#endif // __ANO_H__
