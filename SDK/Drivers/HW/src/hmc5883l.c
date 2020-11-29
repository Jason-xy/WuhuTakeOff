/**
  ******************************************************************************
  * 文件名程: hmc5883l.c 
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
  ******************************************************************************
  */

#include "hmc5883l.h"

//GY-86初始化配置
void GY86_Init(void)
{
  MPU6050_Init();
	MPU_Write_Byte(MPU_CFG, 0x02);   //将MPU的CFG寄存器的第二位设置为1，其他位在使用MPU时配置
	MPU_Write_Byte(MPU_CTRL, 0x00);  //将MPU的CTRL寄存器的第六位设置为0，与上面一步共同开启bypass模式
	HAL_Delay(200);
	HMC_Init();				        //HMC初始化
}

//HMC初始化配置
void HMC_Init(void)
{

	HMC_Write_Byte(HMC_CONFIGA, 0x50);   //01010000/采样平均数4，输出速率15，正常测量配置
	HMC_Write_Byte(HMC_CONFIGB, 0xE0);   //11100000/将增益调至最小
	HMC_Write_Byte(HMC_MODE, 0x00);      //00000000/设置为连续模式
}

//HMC数据读取
uint8_t READ_HMCALL(uint16_t* x,uint16_t* y, uint16_t* z)
{
    uint8_t res;
    uint8_t t[6];
    res=HMC_Read_Len(HMC_DATA_XMSB, 6, t);
    if(res == 0)
		{			
			*x = (uint16_t)((t[0]<<8)+t[1]);
    	*y = (uint16_t)((t[2]<<8)+t[3]);
    	*z = (uint16_t)((t[4]<<8)+t[5]);
		}
    return res;
}

//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//      其他,错误代码
uint8_t HMC_Write_Byte(uint8_t reg,uint8_t data) 				 
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data=0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, HMC_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  HAL_Delay(100);
  
  return 0;
}

//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
uint8_t HMC_Read_Byte(uint8_t reg)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char R_Data=1;
  
  HAL_I2C_Mem_Read(&MPU_I2C, HMC_READ, reg, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xfff);
  HAL_Delay(100);

  return R_Data;
}

//IIC连续写
//addr:器件地址
//reg:要写的寄存器地址
//len:要写的长度
//buf:写数据的数据存储区
//返回值:0,正常
//      其他,错误代码
uint8_t HMC_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, HMC_WRITE, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  HAL_Delay(100);
  
  return 0;
}

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//      其他,错误代码
uint8_t HMC_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, HMC_READ, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  HAL_Delay(100);
  
  return 0;	
}

