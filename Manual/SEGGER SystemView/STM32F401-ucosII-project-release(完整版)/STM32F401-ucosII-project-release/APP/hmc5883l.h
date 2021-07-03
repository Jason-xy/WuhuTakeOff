#ifndef __HMC5883L_H
#define __HMC5883L_H

#include "stm32f4xx.h"

#define MPU_CFG 0x37  //该寄存器与下面的寄存器都是用来开启MPU旁通模式
#define MPU_CTRL 0x6A
#define HMC_CONFIGA 0x00    //配置寄存器A
#define HMC_CONFIGB 0x01    //配置寄存器B
#define HMC_MODE 0x02       //模式寄存器
#define HMC_DATA_XMSB 0x03  //数据寄存器X高位
#define HMC_DATA_XLSB 0x04  //数据寄存器X低位
#define HMC_DATA_ZMSB 0x05  //数据寄存器Z高位
#define HMC_DATA_ZLSB 0x06  //数据寄存器Z低位
#define HMC_DATA_YMSB 0x07  //数据寄存器Y高位
#define HMC_DATA_YLSB 0x08  //数据寄存器Y低位
#define HMC_STATUS 0x09     //状态寄存器
#define HMC_RECOGA 0x0A     //识别寄存器A
#define HMC_RECOGB 0x0B     //识别寄存器B
#define HMC_RECOGC 0x0C     //识别寄存器C
#define MPU_ADD 0xD0        // MPU的i2c地址
#define HMC_WRITE 0x3C      // HMC的i2c写地址
#define HMC_READ 0x3D       // HMC的i2c读地址
#define HMC_ADD 0x3C

void HMC_Init(void);
void GY86_Init(void);
// void READ_HMCALL(u16 *n);
void HMC5884LReadMe(short *MeData);
#endif
