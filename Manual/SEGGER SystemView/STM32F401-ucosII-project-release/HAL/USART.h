#ifndef __USART_H
#define __USART_H

#include <stdio.h>

#include "stm32f4xx.h"

static void NVIC_Configuration(void);
void USART6_Config(void);
static void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t ch);
unsigned char USARTGetByte(USART_TypeDef* pUSARTx, unsigned char* GetData);
void Usart_SendString(USART_TypeDef* pUSARTx, char* str);
void USARTTest(USART_TypeDef* pUSARTx);

#endif
