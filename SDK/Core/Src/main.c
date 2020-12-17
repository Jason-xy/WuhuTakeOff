/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-9-18
  * 功    能: 模块集成测试
  ******************************************************************************
  * 说明：
  * 1.要完整添加各模块驱动文件。
  * 2.修改次数太多，记不住小版本号了。
  * 
	* 功能：
	* 1.GY-86传感器数据读取。
	* 2.电机PWM控制。
	* 3.接收机PWM捕获。
	*	4.上位机json格式数据TCP协议通信。
	* 5.飞控OLED实时数据显示。
  ******************************************************************************
  */
	
//基本外设驱动层
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

//硬件模块驱动层
#include "oled.h"
//#include "mpu6050.h"
//#include "hmc5883l.h"
#include "gy-86.h"
#include "motor.h"
#include "controller.h"
#include "esp8266.h"

int main(void)
{
  HAL_Init(); //HAL库初始化

  SystemClock_Config(); //时钟配置

  //基本外设初始化
  MX_GPIO_Init(); //GPIO初始化
  MX_I2C1_Init(); //OLED I2C初始化
  MX_I2C2_Init(); //GY-86 I2C初始化
  MX_TIM2_Init(); //电机4通道PWM输入捕获初始化
  MX_TIM3_Init(); //电机4通道PWM输出初始化
  MX_TIM4_Init(); //接收机5、6通道电机解锁、锁定PWM输入捕获初始化
  MX_USART1_UART_Init();  //ESP8266串口初始化

  //硬件模块初始化
  //OLED初始化
  OLED_Init();  
	OLED_Clear();
	
  //ESP8266初始化
	esp8266_init(); 
  //输出调试信息
  OLED_Clear();
	OLED_ShowString(8,3,(uint8_t*)"ESP8266_Init OK",16);
	esp8266_cipsend("ESP8266_Init OK\r\n");
	HAL_Delay(1000);
	
  //电机初始化
	Motor_Init(); 
  //输出调试信息
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"Motor_Init OK",16);
	esp8266_cipsend("Motor_Init OK\r\n");
	HAL_Delay(1000);

  //GY-86初始化
	GY86_Init();
  //输出调试信息
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"GY86_Init OK",16);
	esp8266_cipsend("GY86_Init OK\r\n");
	HAL_Delay(1000);
	
  //输出调试信息
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"Motor_Unlock",16);
	esp8266_cipsend("Motor_Unlock\r\n");
	HAL_Delay(1000);
	//电机自动解锁
	Motor_Unlock();
	//输出调试信息
	OLED_Clear();
	OLED_ShowString(6,3,(uint8_t*)"Motor_Unlock OK",16);
	esp8266_cipsend("Motor_Unlock OK\r\n");
	HAL_Delay(1000);

  //接收机初始化
	Input_Capture_Init();
	//输出调试信息
	OLED_Clear();
	OLED_ShowString(0,3,(uint8_t*)"Input_Capture_Init",16);
	esp8266_cipsend("Input_Capture_Init OK\r\n");
	HAL_Delay(3000);

  //OLED图形界面绘制
	OLED_Clear();
	OLED_Draw_interface();
  
  short Gy[3], Ac[3], Me[3]; //GY-86传感器原始数据 Gy：陀螺仪 Ac：加速度计 Me:磁力计
	unsigned int Cap[6];  //接收机各通道实时行程 0~100%
	extern float Duty[6]; //接收机各通道PWM占空比
	char data[100]; //调试数据缓存
	
  while (1)
  {
    read_Gyroscope_DPS(Gy, Gy+1, Gy+2);
    MPU_Get_Accelerometer(Ac, Ac+1, Ac+2);
    read_hmc_degree(Me, Me+1, Me+2);
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

void SystemClock_Config(void)//输出100MHz时钟频率
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();//开启时钟
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); //电压缩放

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
  //错误信息输出
}
