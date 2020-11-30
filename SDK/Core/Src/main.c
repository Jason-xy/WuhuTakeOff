/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-9-18
  * ��    ��: ģ�鼯�ɲ���
  ******************************************************************************
  * ˵����
  * 1.Ҫ������Ӹ�ģ�������ļ���
  * 2.�޸Ĵ���̫�࣬�ǲ�סС�汾���ˡ�
  * 
	* ���ܣ�
	* 1.GY-86���������ݶ�ȡ��
	* 2.���PWM���ơ�
	* 3.���ջ�PWM����
	*	4.��λ��json��ʽ����TCPЭ��ͨ�š�
	* 5.�ɿ�OLEDʵʱ������ʾ��
  ******************************************************************************
  */
	
//��������������
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

//Ӳ��ģ��������
#include "oled.h"
#include "mpu6050.h"
#include "hmc5883l.h"
#include "motor.h"
#include "controller.h"
#include "esp8266.h"

int main(void)
{
  HAL_Init(); //HAL���ʼ��

  SystemClock_Config(); //ʱ������

  //���������ʼ��
  MX_GPIO_Init(); //GPIO��ʼ��
  MX_I2C1_Init(); //OLED I2C��ʼ��
  MX_I2C2_Init(); //GY-86 I2C��ʼ��
  MX_TIM2_Init(); //���4ͨ��PWM���벶���ʼ��
  MX_TIM3_Init(); //���4ͨ��PWM�����ʼ��
  MX_TIM4_Init(); //���ջ�5��6ͨ���������������PWM���벶���ʼ��
  MX_USART1_UART_Init();  //ESP8266���ڳ�ʼ��

  //Ӳ��ģ���ʼ��
  //OLED��ʼ��
  OLED_Init();  
	OLED_Clear();
	
  //ESP8266��ʼ��
	esp8266_init(); 
  //���������Ϣ
  OLED_Clear();
	OLED_ShowString(8,3,(uint8_t*)"ESP8266_Init OK",16);
	esp8266_cipsend("ESP8266_Init OK\r\n");
	HAL_Delay(1000);
	
  //�����ʼ��
	Motor_Init(); 
  //���������Ϣ
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"Motor_Init OK",16);
	esp8266_cipsend("Motor_Init OK\r\n");
	HAL_Delay(1000);

  //GY-86��ʼ��
	GY86_Init();
  //���������Ϣ
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"GY86_Init OK",16);
	esp8266_cipsend("GY86_Init OK\r\n");
	HAL_Delay(1000);
	
  //���������Ϣ
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"Motor_Unlock",16);
	esp8266_cipsend("Motor_Unlock\r\n");
	HAL_Delay(1000);
	//����Զ�����
	Motor_Unlock();
	//���������Ϣ
	OLED_Clear();
	OLED_ShowString(6,3,(uint8_t*)"Motor_Unlock OK",16);
	esp8266_cipsend("Motor_Unlock OK\r\n");
	HAL_Delay(1000);

  //���ջ���ʼ��
	Input_Capture_Init();
	//���������Ϣ
	OLED_Clear();
	OLED_ShowString(0,3,(uint8_t*)"Input_Capture_Init",16);
	esp8266_cipsend("Input_Capture_Init OK\r\n");
	HAL_Delay(3000);

  //OLEDͼ�ν������
	OLED_Clear();
	OLED_Draw_interface();
  
  uint16_t Gy[3], Ac[3], Me[3]; //GY-86������ԭʼ���� Gy�������� Ac�����ٶȼ� Me:������
	unsigned int Cap[6];  //���ջ���ͨ��ʵʱ�г� 0~100%
	extern float Duty[6]; //���ջ���ͨ��PWMռ�ձ�
	char data[100]; //�������ݻ���
	
  while (1)
  {
    MPU_Get_Gyroscope(Gy, Gy+1, Gy+2);
    MPU_Get_Accelerometer(Ac, Ac+1, Ac+2);
    READ_HMCALL(Me, Me+1, Me+2);
		for(int i = 0; i < 6; i++)
		{
			Cap[i] =  (Duty[i] * 100 - 5) / 0.05;
		}
    OLED_Show_3num(Gy[0], Gy[1], Gy[2], 0);
    OLED_Show_3num(Ac[0], Ac[1], Ac[2], 1);
    OLED_Show_3num(Me[0], Me[1], Me[2], 2);
		OLED_Show_3num(Cap[0], Cap[1], Cap[2], 3);
    OLED_Show_3num(Cap[3], Cap[4], Cap[5], 4);
    OLED_ShowNum(24, 7, MPU_Get_Temperature(),2,12);
		sprintf(data,"{\"G\":\"%d %d %d\"\t\"A\":\"%d %d %d\"\t\"M\":\"%d %d %d\"\t\"C\":\"%d %d %d %d %d %d\"\r\n}",\
						Gy[0], Gy[1], Gy[2],\
						Ac[0], Ac[1], Ac[2],\
						Me[0], Me[1], Me[2],\
						Cap[0], Cap[1], Cap[2],Cap[3], Cap[4], Cap[5]);
		esp8266_cipsend(data);
  }
}

void SystemClock_Config(void)//���100MHzʱ��Ƶ��
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();//����ʱ��
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); //��ѹ����

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  //������Ϣ���
}
