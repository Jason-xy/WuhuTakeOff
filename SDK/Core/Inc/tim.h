/**
  ******************************************************************************
  * 文件名程: tim.h
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-9-18
  * 功    能: TIM模块驱动
  ******************************************************************************
  * 说明：
  * 无。
  * 
	* 功能：
	* 1.TIM3 PWM输出初始化。
  * 2.TIM2\4 输入捕获初始化。
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
