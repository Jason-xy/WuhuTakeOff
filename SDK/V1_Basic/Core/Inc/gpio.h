/**
  ******************************************************************************
  * 文件名程: gpio.h 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-9-18
  * 功    能: GPIO模块驱动
  ******************************************************************************
  * 说明：
  * 无。
  * 
	* 功能：
	* 1.GPIO初始化。
  ******************************************************************************
  */

#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

void MX_GPIO_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */
