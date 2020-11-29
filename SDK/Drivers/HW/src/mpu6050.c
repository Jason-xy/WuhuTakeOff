/**
  ******************************************************************************
  * 文件名程: mpu6050.c 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-10-2
  * 功    能: MPU6050初始化
  ******************************************************************************
  * 说明：
  * 1.使用在GY-86模块。
  * 2.需要实现配置好I2C驱动。
  * 
  * 功能：
  * 1.MPU6050初始化。
  * 2.I2C基本读写。
  * 3.陀螺仪数据获取。
  * 4.加速度计数据获取。
  * 5.温度计数据获取和解析。
  ******************************************************************************
  */

#include "mpu6050.h" 
#include "stdio.h"

//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//      其他,错误代码
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data) 				 
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data=0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, MPU_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  HAL_Delay(100);
  
  return 0;
}

//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t reg)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char R_Data=1;
  
  HAL_I2C_Mem_Read(&MPU_I2C, MPU_READ, reg, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xfff);
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
uint8_t MPU_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, MPU_WRITE, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
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
uint8_t MPU_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, MPU_READ, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  HAL_Delay(100);
  
  return 0;	
}

//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU6050_Init(void)
{ 
  uint8_t res;
  extern I2C_HandleTypeDef MPU_I2C;
  MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X80);	//复位MPU6050
  MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X00);	//唤醒MPU6050 
  MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
  MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
  MPU_Set_Rate(50);						//设置采样率50Hz
  MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0X00);	//关闭所有中断
  MPU_Write_Byte(MPU6050_RA_USER_CTRL,0X00);	//I2C主模式关闭
  MPU_Write_Byte(MPU6050_RA_FIFO_EN,0X00);	//关闭FIFO
  MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X80);	//INT引脚低电平有效
  res=MPU_Read_Byte(MPU6050_RA_WHO_AM_I);
  if(res==MPU_ADDR)//器件ID正确
  {
    MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X01);	//设置CLKSEL,PLL X轴为参考
    MPU_Write_Byte(MPU6050_RA_PWR_MGMT_2,0X00);	//加速度与陀螺仪都工作
    MPU_Set_Rate(50);						//设置采样率为50Hz
  }else
  {
		return 1;
	}
  return 0;
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6050_RA_GYRO_CONFIG, fsr<<3);//设置陀螺仪满量程范围  
}

//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6050_RA_ACCEL_CONFIG, fsr<<3);//设置加速度传感器满量程范围  
}

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU6050_RA_CONFIG, data);//设置数字低通滤波器  
}

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU6050_RA_SMPLRT_DIV, data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
float MPU_Get_Temperature(void)
{
  unsigned char  buf[2]; 
  uint8_t raw;
  float temp;
  
  MPU_Read_Len(MPU6050_RA_TEMP_OUT_H, 2, buf); 
  raw=(buf[0]<<8)| buf[1];  
  temp=(36.53+((double)raw)/340)*100;  
  return temp/100.0f;
}

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(uint16_t *gx,uint16_t *gy,uint16_t *gz)
{
    uint8_t buf[6],res; 
	
	res=MPU_Read_Len(MPU6050_RA_GYRO_XOUT_H, 6, buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];  
		*gy=((uint16_t)buf[2]<<8)|buf[3];  
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(uint16_t *ax,uint16_t *ay,uint16_t *az)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU6050_RA_ACCEL_XOUT_H, 6, buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
