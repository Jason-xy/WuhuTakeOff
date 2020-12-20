/**
  ******************************************************************************
  * 文件名程: i2c.h 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-9-18
  * 功    能: I2C模块驱动
  ******************************************************************************
  * 说明：
  * 无。
  * 
	* 功能：
	* 1.I2C初始化。
  ******************************************************************************
  */

#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */
