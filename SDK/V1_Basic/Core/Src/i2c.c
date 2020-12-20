/**
  ******************************************************************************
  * �ļ�����: i2c.c 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-9-18
  * ��    ��: I2Cģ������
  ******************************************************************************
  * ˵����
  * �ޡ�
  * 
	* ���ܣ�
	* 1.I2C��ʼ����
  ******************************************************************************
  */

#include "i2c.h"

//I2C����ṹ�嶨��
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* OLED I2C1 ��ʼ�� */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;  //I2C�Ĵ�������ַ
  hi2c1.Init.ClockSpeed = 100000; //ʱ��Ƶ��
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2; //ռ�ձ�
  hi2c1.Init.OwnAddress1 = 0; //ָ����һ��������ַ
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;  //��ַģʽѡ�� 7bit����10bit
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; //�ر�˫��ַģʽ
  hi2c1.Init.OwnAddress2 = 0; //ָ���ڶ���������ַ
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; //�رչ㲥����ģʽ
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE; //�رշ��ӳ�ģʽ
  if (HAL_I2C_Init(&hi2c1) != HAL_OK) //��ʼ��
  {
    Error_Handler();
  }

}

/* GY-86 I2C2 ��ʼ�� */
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
     /**I2C1 GPIO ��������
     PB6     ------> I2C1_SCL
     PB7     ------> I2C1_SDA
     */
     GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;  //����ѡ��
     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; //���ÿ�©���
     GPIO_InitStruct.Pull = GPIO_PULLUP; //����
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  //����ģʽ
     GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;  //���Ÿ���
     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

     /* I2C1 ʱ��ʹ�� */
     __HAL_RCC_I2C1_CLK_ENABLE();
   }
   else if(i2cHandle->Instance==I2C2)
   {
     __HAL_RCC_GPIOB_CLK_ENABLE();
     /**I2C2 GPIO ��������
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

     /* I2C2 ʱ��ʹ�� */
     __HAL_RCC_I2C2_CLK_ENABLE();
   }
 }
