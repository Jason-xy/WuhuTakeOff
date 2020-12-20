/**
  ******************************************************************************
  * 文件名程: controller.h
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-11-15
  * 功    能: 接收机模块驱动
  ******************************************************************************
  * 说明：
  * 1.需要先完成TIM的初始化。
  * 2.需提前设置时钟频率100MHz（根据自己的板子设定，比我我的Psc = 99）捕获通道每一个节拍1us。
  * 
  * 功能：
  * 1.接收机数据采集。
  * 2.根据采集数据改变马达转速。
  ******************************************************************************
  */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "motor.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void Input_Capture_Init(void);

#endif /* __CONTROLLER_H__ */
