/**
  ******************************************************************************
  * �ļ�����: usart.h
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-9-18
  * ��    ��: USARTģ������
  ******************************************************************************
  * ˵����
  * �ޡ�
  * 
	* ���ܣ�
	* 1.USART1�첽�����ʼ����
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
