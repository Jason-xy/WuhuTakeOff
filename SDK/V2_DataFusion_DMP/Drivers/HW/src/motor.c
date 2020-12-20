/**
  ******************************************************************************
  * 文件名程: motor.c 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-10-12
  * 功    能: 电机模块驱动
  ******************************************************************************
  * 说明：
  * 1.需要先完成TIM的初始化。
  * 
  * 功能：
  * 1.电机初始化。
  * 2.改变电机转速。
  * 3.电机测试。
  * 4.电机锁定。
  ******************************************************************************
  */

#include "motor.h"

//电机初始化
void Motor_Init(void)
{
  HAL_TIM_PWM_Start(&MOTOR_TIM,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&MOTOR_TIM,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&MOTOR_TIM,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&MOTOR_TIM,TIM_CHANNEL_4);
}

//电机解锁
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

//电机测试
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

//x号电机速度设定
void Motor_Set(float percentage, uint8_t Channel)
{
	__HAL_TIM_SetCompare(&MOTOR_TIM, Channel, (int)(500 + 500*(percentage-0.05)/0.05)-200);
}

//电机锁定
void Motor_Lock(void)
{
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_1, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_2, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_3, 500);
	__HAL_TIM_SetCompare(&MOTOR_TIM, TIM_CHANNEL_4, 500);
}
