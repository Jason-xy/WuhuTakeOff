#ifndef __HM10_H
#define __HM10_H

#include "USART.h"
#include "stm32f4xx.h"


void HM10_Config(void);
void HM10_Test(void);
void HM10_IRQ_IRQHandler(void);
int fputc(int ch, FILE *f);

#endif
