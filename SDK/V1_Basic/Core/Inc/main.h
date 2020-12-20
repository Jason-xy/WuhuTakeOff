/**
  ******************************************************************************
  * 文件名程: main.h 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-9-18
  * 功    能: 模块集成测试
  ******************************************************************************
  * 说明：
  * 1.要完整添加各模块驱动文件。
  * 2.修改次数太多，记不住小版本号了。
  * 
	* 功能：
	* 1.GY-86传感器数据读取。
	* 2.电机PWM控制。
	* 3.接收机PWM捕获。
	*	4.上位机json格式数据TCP协议通信。
	* 5.飞控OLED实时数据显示。
  ******************************************************************************
  */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

void Error_Handler(void);
void SystemClock_Config(void);	//时钟配置

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
