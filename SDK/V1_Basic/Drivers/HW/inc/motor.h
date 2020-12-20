/**
  ******************************************************************************
  * �ļ�����: motor.h 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-10-12
  * ��    ��: ���ģ������
  ******************************************************************************
  * ˵����
  * 1.��Ҫ�����TIM�ĳ�ʼ����
  * 
  * ���ܣ�
  * 1.�����ʼ����
  * 2.�ı���ת�١�
  * 3.������ԡ�
  * 4.���������
  ******************************************************************************
  */

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "oled.h"

#define MOTOR_TIM   (htim3)

void Motor_Init(void);              //�����ʼ��
void Motor_Unlock(void);            //�������
void Motor_Lock(void);              //�������
void Motor_Set(float percentage, uint8_t Channel);   //x�ŵ���ٶ��趨

void Motor_Test(void);

#endif /*__MOTOR_H__*/
