#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h"
#include "sysTick.h"
#include "tim.h"

void Motor_Config(void);
void Motor_Unlock(void);
void Motor_Test(void);
void Motor_Set(double percentage, uint8_t channel);

#endif
