/**
  ******************************************************************************
  * 文件名程: hmc5883l.h 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-10-7
  * 功    能: 地磁仪初始化
  ******************************************************************************
  * 说明：
  * 1.使用在GY-86模块。
  * 2.需要事先配置好I2C驱动。
  * 3.需要事先配置好MPU6050。
  * 
  * 功能：
  * 1.hmc5883l地磁仪初始化。
  * 2.GY-86初始化。
  * 3.I2C基本读写。
  * 4.传感器数据获取。
  * 5.宏定义所需寄存器地址
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "mpu6050.h"

#define MPU_CFG 0x37    //该寄存器与下面的寄存器都是用来开启MPU旁通模式
#define MPU_CTRL 0x6A
#define HMC_CONFIGA 0x00  //配置寄存器A
#define HMC_CONFIGB 0x01   //配置寄存器B
#define HMC_MODE 0x02      //模式寄存器
#define HMC_DATA_XMSB 0x03  //数据寄存器X高位
#define HMC_DATA_XLSB 0x04  //数据寄存器X低位
#define HMC_DATA_ZMSB 0x05  //数据寄存器Z高位
#define HMC_DATA_ZLSB 0x06  //数据寄存器Z低位
#define HMC_DATA_YMSB 0x07  //数据寄存器Y高位
#define HMC_DATA_YLSB 0x08  //数据寄存器Y低位
#define HMC_STATUS 0x09  //状态寄存器
#define HMC_RECOGA 0x0A  //识别寄存器A
#define HMC_RECOGB 0x0B  //识别寄存器B
#define HMC_RECOGC 0x0C  //识别寄存器C
#define MPU_ADD 0xD0  //MPU的i2c地址
#define HMC_WRITE 0x3C  //HMC的i2c写地址
#define HMC_READ  0x3D	//HMC的i2c读地址

void HMC_Init(void);
void GY86_Init(void);
uint8_t HMC_Write_Byte(uint8_t reg,uint8_t data);
uint8_t HMC_Read_Byte(uint8_t reg);
uint8_t HMC_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t HMC_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf);

uint8_t READ_HMCALL(uint16_t* x,uint16_t* y, uint16_t* z);
