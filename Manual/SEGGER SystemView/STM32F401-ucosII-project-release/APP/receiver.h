#ifndef __RECEIVER_H
#define __RECEIVER_H

#include "stm32f4xx.h"
#include "tim.h"


void Receiver_Config(void);
void Receiver_IRQ_Handler(void);

#endif
