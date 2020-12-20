/**
  ******************************************************************************
  * 文件名程: motor.h 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-10-12
  * 功    能: 电机模块驱动
  ******************************************************************************
  * 说明：
  * 1.需要先完成TIM的初始化。
  * 
  * 功能：
  * 1.电机初始化。
  * 2.改变电机转速。
  * 3.电机测试。
  * 4.电机锁定。
  ******************************************************************************
  */

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "oled.h"

#define MOTOR_TIM   (htim3)

void Motor_Init(void);              //电机初始化
void Motor_Unlock(void);            //电机解锁
void Motor_Lock(void);              //电机锁定
void Motor_Set(float percentage, uint8_t Channel);   //x号电机速度设定

void Motor_Test(void);

#endif /*__MOTOR_H__*/
