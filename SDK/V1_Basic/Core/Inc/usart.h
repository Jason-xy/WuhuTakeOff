/**
  ******************************************************************************
  * 文件名程: usart.h
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-9-18
  * 功    能: USART模块驱动
  ******************************************************************************
  * 说明：
  * 无。
  * 
	* 功能：
	* 1.USART1异步传输初始化。
  ******************************************************************************
  */

#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */
