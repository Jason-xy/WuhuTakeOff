/**
  ******************************************************************************
  * 文件名程: gy-86.c 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.1
  * 编写日期: 2020-10-2
  * 功    能: GY-86初始化
  ******************************************************************************
  * 说明：
  * 1.使用在GY-86模块。
  * 2.需要实现配置好I2C驱动。
  * 3.需要事先配置好MPU6050。
  * 4.需要事先配置好HMC5883L。
  * 
  * 功能：
  * 1.MPU6050初始化。
  * 2.hmc5883l地磁仪初始化。
  * 3.GY-86初始化。
  * 4.I2C基本读写。
  * 5.陀螺仪数据获取。
  * 6.加速度计数据获取。
  * 7.温度计数据获取和解析。
  * 8.地磁仪数据获取。
  * 9.宏定义所需寄存器地址。
  * 10.陀螺仪零飘矫正。
  * 
  * 更新：
  * 2020-12-15
  * 1.数据获取修改uint16_t为short。
  * 2.添加陀螺仪零飘矫正。
  ******************************************************************************
  */
#include "gy-86.h" 
//#include "stdio.h"

//校准参数
short Gyro_xFix=0,Gyro_yFix=0,Gyro_zFix=0;

//数据变量
short Gyro_x=0,Gyro_y=0,Gyro_z=0;

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
  MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0X00);	//关闭所有中断
  MPU_Write_Byte(MPU6050_RA_USER_CTRL,0X00);	//I2C主模式关闭
  MPU_Write_Byte(MPU6050_RA_FIFO_EN,0X00);	//关闭FIFO
  MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X80);	//INT引脚低电平有效
  MPU_Set_Rate(500);						//设置采样率为50Hz
  MPU_Set_LPF(1000);            //数字低通滤波器1000Hz
  res=MPU_Read_Byte(MPU6050_RA_WHO_AM_I);
  if(res==MPU_ADDR)//器件ID正确
  {
    MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X01);	//设置CLKSEL,PLL X轴为参考
    MPU_Write_Byte(MPU6050_RA_PWR_MGMT_2,0X00);	//加速度与陀螺仪都工作
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
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    unsigned char buf[6],res; 
	
	res=MPU_Read_Len(MPU6050_RA_GYRO_XOUT_H, 6, buf);
	if(res==0)
	{
		*gx=((buf[0]<<8)|buf[1])-Gyro_xFix;  
		*gy=((buf[2]<<8)|buf[3])-Gyro_yFix;  
		*gz=((buf[4]<<8)|buf[5])-Gyro_zFix;
	} 	
    return res;
}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功逻辑分析仪出现framing error
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    unsigned char buf[6],res;  
	res=MPU_Read_Len(MPU6050_RA_ACCEL_XOUT_H, 6, buf);
	if(res==0)
	{
		*ax=(buf[0]<<8)|buf[1];  
		*ay=(buf[2]<<8)|buf[3];  
		*az=(buf[4]<<8)|buf[5];
	} 	
    return res;;
}

//GY-86初始化配置
void GY86_Init(void)
{
  MPU6050_Init();
	MPU_Write_Byte(MPU_CFG, 0x02);   //将MPU的CFG寄存器的第二位设置为1，其他位在使用MPU时配置
	MPU_Write_Byte(MPU_CTRL, 0x00);  //将MPU的CTRL寄存器的第六位设置为0，与上面一步共同开启bypass模式
	HAL_Delay(200);
	HMC_Init();				        //HMC初始化
  GY86_SelfTest();
}

//HMC初始化配置
void HMC_Init(void)
{

	HMC_Write_Byte(HMC_CONFIGA, 0x50);   //01010000/采样平均数4，输出速率15，正常测量配置
	HMC_Write_Byte(HMC_CONFIGB, 0xE0);   //11100000/将增益调至最小
	HMC_Write_Byte(HMC_MODE, 0x00);      //00000000/设置为连续模式
}

//HMC数据读取
uint8_t READ_HMCALL(short* x,short* y, short* z)
{
    uint8_t res;
    unsigned char t[6];
    res=HMC_Read_Len(HMC_DATA_XMSB, 6, t);
    if(res == 0)
		{			
			*x = ((t[0]<<8)+t[1]);
    	*y = ((t[2]<<8)+t[3]);
    	*z = ((t[4]<<8)+t[5]);
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

//GY-86启动自检校准
void GY86_SelfTest(void)
{
  Gyro_Test();
}

//陀螺仪校准
void Gyro_Test(void)
{
  short sum_x=0,sum_y=0,sum_z=0;
  Gyro_x=0,Gyro_y=0,Gyro_z=0;
  int times = 50;
  for(int i=0; i<times; i++)
  {
    MPU_Get_Gyroscope(&Gyro_x,&Gyro_y,&Gyro_z);
    sum_x+=Gyro_x;
    sum_y+=Gyro_y;
    sum_z+=Gyro_z;
  }
  Gyro_xFix=sum_x/times;
  Gyro_yFix=sum_y/times;
  Gyro_zFix=sum_z/times;
}
