#ifndef _LED_H
#define _LED_H
#include "sys.h"


//LED�˿ڶ���
#define LED0(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET))
#define LED0_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11)) //LED0�����ƽ��ת
#define LED1(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET))
#define LED1_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10)) //LED1�����ƽ��ת

void LED_Init(void); //LED��ʼ������
#endif
