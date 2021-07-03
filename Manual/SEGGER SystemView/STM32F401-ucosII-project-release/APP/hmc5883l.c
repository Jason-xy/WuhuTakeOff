#include "hmc5883l.h"
//#include "i2c.h"
#include "mpu6050.h"
#include "stdio.h"
#include "sysTick.h"
#include "i2c.h"

void HMC5883L_WriteReg(u8 reg_add, u8 reg_dat) {
	I2C_WriteByte(I2C1,HMC_WRITE,reg_add,reg_dat);
 //   I2C_SAND_BYTE(HMC_WRITE, reg_add, reg_dat);
}

void HMC5883L_ReadData(u8 reg_add, unsigned char* Read, u8 num) {
	I2C_ReadData(I2C1,HMC_READ,reg_add,Read,6);
//   I2C_READ_BUFFER(HMC_READ, reg_add, Read, 6);
}

// GY-86初始化配置
void GY86_Init(void) {
    MPU6050_Config();
    MPU6050_Init();
    MPU6050_WriteReg(
        MPU_CFG,
        0x02);  //将MPU的CFG寄存器的第二位设置为1，其他位在使用MPU时配置
    MPU6050_WriteReg(
        MPU_CTRL,
        0x00);  //将MPU的CTRL寄存器的第六位设置为0，与上面一步共同开启bypass模式
    Delay_ms(200);
    HMC_Init();  // HMC初始化
}

// HMC初始化配置
void HMC_Init(void) {
    HMC5883L_WriteReg(HMC_CONFIGA,
                      0x50);  // 01010000/采样平均数4，输出速率15，正常测量配置
    HMC5883L_WriteReg(HMC_CONFIGB, 0xE0);  // 11100000/将增益调至最小
    HMC5883L_WriteReg(HMC_MODE, 0x00);     // 00000000/设置为连续模式
}

void HMC5884LReadMe(short* MeData) {
    u8 buf[6];
    // HMC5883L_ReadData(HMC_DATA_XMSB,buf,6);
    HMC5883L_ReadData(HMC_DATA_XMSB, buf, 6);
    MeData[0] = (buf[0] << 8) | buf[1];
    MeData[1] = (buf[2] << 8) | buf[3];
    MeData[2] = (buf[4] << 8) | buf[5];
}
