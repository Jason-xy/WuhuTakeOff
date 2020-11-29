/**
  ******************************************************************************
  * 文件名程: i2c.c 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-9-18
  * 功    能: I2C模块驱动
  ******************************************************************************
  * 说明：
  * 无。
  * 
	* 功能：
	* 1.I2C初始化。
  ******************************************************************************
  */

#include "i2c.h"

//I2C处理结构体定义
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* OLED I2C1 初始化 */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;  //I2C寄存器基地址
  hi2c1.Init.ClockSpeed = 100000; //时钟频率
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2; //占空比
  hi2c1.Init.OwnAddress1 = 0; //指定第一个器件地址
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;  //地址模式选择 7bit或者10bit
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; //关闭双地址模式
  hi2c1.Init.OwnAddress2 = 0; //指定第二个器件地址
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; //关闭广播呼叫模式
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE; //关闭非延长模式
  if (HAL_I2C_Init(&hi2c1) != HAL_OK) //初始化
  {
    Error_Handler();
  }

}

/* GY-86 I2C2 初始化 */
void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

}

 void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
 {

   GPIO_InitTypeDef GPIO_InitStruct = {0};
   if(i2cHandle->Instance==I2C1)
   {
     __HAL_RCC_GPIOB_CLK_ENABLE();
     /**I2C1 GPIO 引脚配置
     PB6     ------> I2C1_SCL
     PB7     ------> I2C1_SDA
     */
     GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;  //引脚选择
     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; //复用开漏输出
     GPIO_InitStruct.Pull = GPIO_PULLUP; //上拉
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  //高速模式
     GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;  //引脚复用
     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

     /* I2C1 时钟使能 */
     __HAL_RCC_I2C1_CLK_ENABLE();
   }
   else if(i2cHandle->Instance==I2C2)
   {
     __HAL_RCC_GPIOB_CLK_ENABLE();
     /**I2C2 GPIO 引脚设置
     PB10     ------> I2C2_SCL
     PB3     ------> I2C2_SDA
     */
     GPIO_InitStruct.Pin = GPIO_PIN_10;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
     GPIO_InitStruct.Pull = GPIO_PULLUP;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
     GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

     GPIO_InitStruct.Pin = GPIO_PIN_3;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
     GPIO_InitStruct.Pull = GPIO_PULLUP;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
     GPIO_InitStruct.Alternate = GPIO_AF9_I2C2;
     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

     /* I2C2 时钟使能 */
     __HAL_RCC_I2C2_CLK_ENABLE();
   }
 }
