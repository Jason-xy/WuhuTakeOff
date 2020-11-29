#include "led.h"

//��ʼ��PB10,PB11Ϊ���.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();					//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;			//PB10��11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//�������
    GPIO_Initure.Pull=GPIO_PULLUP;         			//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     		//��ʼ��GPIOB.10��GPIOB.11
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);	//PB10��0
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);	//PB11��1 
}

