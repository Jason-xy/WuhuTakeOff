#ifndef __I2C_H__
#define __I2C_H__

#include <stdio.h>

#include "stm32f4xx.h"
#include "sysTick.h"
// OLED
#define OLED_ADDRESS 0x78  //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

/* STM32 I2C 快速模式 */
#define I2C_Speed 400000

/*I2C接口*/
#define OLED_I2C I2C3
#define OLED_I2C_CLK RCC_APB1Periph_I2C3
#define OLED_I2C_CLK_INIT RCC_APB1PeriphClockCmd

#define OLED_I2C_SCL_PIN GPIO_Pin_8
#define OLED_I2C_SCL_GPIO_PORT GPIOA
#define OLED_I2C_SCL_GPIO_CLK RCC_AHB1Periph_GPIOA
#define OLED_I2C_SCL_SOURCE GPIO_PinSource8
#define OLED_I2C_SCL_AF GPIO_AF_I2C3

#define OLED_I2C_SDA_PIN GPIO_Pin_9
#define OLED_I2C_SDA_GPIO_PORT GPIOC
#define OLED_I2C_SDA_GPIO_CLK RCC_AHB1Periph_GPIOC
#define OLED_I2C_SDA_SOURCE GPIO_PinSource9
#define OLED_I2C_SDA_AF GPIO_AF_I2C3

// GY-86
#define I2C_SPEED 400000
#define I2C_OWN_ADDRESS 0xC0

//毫秒级延时(需要定时器支持)，或者重写Delay宏
#define Delay Delay_ms

/*引脚定义*/

#define SENSORS_I2C_SCL_GPIO_PORT GPIOB
#define SENSORS_I2C_SCL_GPIO_CLK RCC_AHB1Periph_GPIOB
#define SENSORS_I2C_SCL_GPIO_PIN GPIO_Pin_8
#define SENSORS_I2C_SCL_GPIO_PINSOURCE GPIO_PinSource8

#define SENSORS_I2C_SDA_GPIO_PORT GPIOB
#define SENSORS_I2C_SDA_GPIO_CLK RCC_AHB1Periph_GPIOB
#define SENSORS_I2C_SDA_GPIO_PIN GPIO_Pin_9
#define SENSORS_I2C_SDA_GPIO_PINSOURCE GPIO_PinSource9

#define SENSORS_I2C_AF GPIO_AF_I2C1

#define SENSORS_I2C I2C1
#define SENSORS_I2C_RCC_CLK RCC_APB1Periph_I2C1


/*函数定义*/
void I2C3_Configuration(void);
//#define I2C_Config() I2C1_Init();
void I2C1_Init(void);

void I2C_WriteByte(I2C_TypeDef * I2Cx,uint8_t slave_addr,uint8_t reg_addr, uint8_t data);
void I2C_ReadData(I2C_TypeDef * I2Cx,uint8_t slave_addr, uint8_t reg_addr, uint8_t *pBuffer,uint16_t NumByteToRead);

#endif  // __I2C_H__
