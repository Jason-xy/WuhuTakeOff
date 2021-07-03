#ifndef __TIM_H
#define __TIM_H

#include "stm32f4xx.h"

// extern u32 temp ;
// extern float set ;
// extern u8 TIM3CH1_CAPTURE_STA ; //输入捕获中断
// extern u16 TIM3CH1_CAPTURE_VAL; //输入捕获值

#define GENERAL_OCPWM_PIN_1 GPIO_Pin_0
#define GENERAL_OCPWM_PIN_2 GPIO_Pin_1
#define GENERAL_OCPWM_PIN_3 GPIO_Pin_2
#define GENERAL_OCPWM_PIN_4 GPIO_Pin_3
#define GENERAL_OCPWM_GPIO_PORT GPIOA
#define GENERAL_OCPWM_GPIO_CLK RCC_AHB1Periph_GPIOA
#define GENERAL_OCPWM_PINSOURCE_1 GPIO_PinSource0
#define GENERAL_OCPWM_PINSOURCE_2 GPIO_PinSource1
#define GENERAL_OCPWM_PINSOURCE_3 GPIO_PinSource2
#define GENERAL_OCPWM_PINSOURCE_4 GPIO_PinSource3
#define GENERAL_OCPWM_AF GPIO_AF_TIM2

#define GENERAL_TIM TIM2
#define GENERAL_TIM_CLK RCC_APB1Periph_TIM2

#define GENERAL_TIM_IRQn TIM2_IRQn
#define GENERAL_TIM_IRQHandler TIM2_IRQHandler

void TIM2_PWMOUTPUT_Config(void);

void TIM3_Cap_Init(u16 arr, u16 psc);

#endif
