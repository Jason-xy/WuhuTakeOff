/**
  ******************************************************************************
  * �ļ�����: tim.h
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-9-18
  * ��    ��: TIMģ������
  ******************************************************************************
  * ˵����
  * �ޡ�
  * 
	* ���ܣ�
	* 1.TIM3 PWM�����ʼ����
  * 2.TIM2\4 ���벶���ʼ����
  ******************************************************************************
  */

#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */
