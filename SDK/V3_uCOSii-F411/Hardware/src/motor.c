/**
  ******************************************************************************
  * �ļ�����: motor.c 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-10-12
  * ��    ��: ���ģ������
  ******************************************************************************
  * ˵����
  * 1.��Ҫ�����TIM�ĳ�ʼ����
  * 
  * ���ܣ�
  * 1.�����ʼ����
  * 2.�ı���ת�١�
  * 3.������ԡ�
  * 4.���������
  ******************************************************************************
  */

#include "motor.h"

//�����ʼ��
void Motor_Init(void)
{
  HAL_TIM_PWM_Start(&MOTOR_TIM,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&MOTOR_TIM,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&MOTOR_TIM,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&MOTOR_TIM,TIM_CHANNEL_4);
}

//�������
void Motor_Unlock(void)
{
    __HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_1, 1000);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_2, 1000);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_3, 1000);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_4, 1000);
    HAL_Delay(2000);
    __HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_1, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_2, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_3, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_4, 500);
    HAL_Delay(2000);
}

//�������
void Motor_Test(void)
{
		int a=500;
    __HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_1, a);
	  __HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_2, a);
	  __HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_3, a);
	  __HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_4, a);
		OLED_Show_2num((a-500)/5,(a-500)/5,5);
		OLED_Show_2num((a-500)/5,(a-500)/5,6);
    HAL_Delay(2);
    a=a+5;
    if(a >= 1000)
			a = 500;
}

//x�ŵ���ٶ��趨
void Motor_Set(float percentage, uint8_t Channel)
{
	__HAL_TIM_SetCompare(&MOTOR_TIM, Channel, (int)(500 + 500*(percentage-0.05)/0.05)-200);
}

//�������
void Motor_Lock(void)
{
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_1, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_2, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_3, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_4, 500);
}
