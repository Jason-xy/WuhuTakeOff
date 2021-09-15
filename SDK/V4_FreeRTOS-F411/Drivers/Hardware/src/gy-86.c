/**
  ******************************************************************************
  * 文件名程: gy-86.h 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.2.1
  * 编写日期: 2020-10-2
  * 功    能: GY-86初始化
  ******************************************************************************
  * 说明：
  * 1.使用在GY-86模块。
  * 2.需要实现配置好I2C驱动。
  * 3.需要事先配置好MPU6050。
  * 4.需要事先配置好HMC5883L。
  * 5.陀螺仪校准数据计算公式 Gx=Gyro_x-Gyro_xFix。
  * 6.地磁仪校准数据计算公式 Mx=MagScaleX*Mag_x-offsetMagX。
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
  * 10.陀螺仪零偏矫正。
  * 11.磁力计校准。
  * 12.陀螺仪显式数据输出。
  * 13.磁力计显式数据输出。
  * 
  * 更新：
  * 2020-12-15
  * 1.数据获取修改uint16_t为short。
  * 2.添加陀螺仪零偏矫正。
  * 2020-12-16
  * 1.温度传感器数据Bug修复。
  * 2020-12-17
  * 1.添加磁力计校准，8字校准方式。
  * 2.磁力计显式数据读出。
  * 3.陀螺仪显式数据读出。
  * 2020-12-20
  * 1.修改I2C读写入口参数，便于提供DMP接口。
  * 2021-9-12
  * 1.采用面向对象的方法重构代码
  * 2.添加高斯牛顿迭代校准算法法
  ******************************************************************************
  */
#include "gy-86.h"

//GY-86
GY_86 *GY86;   
Vector3f_t Accel_raw[6], Gyro_raw[6], Mag_raw[6];

//GY-86初始化配置
void GY86_Init(void)
{
  MPU6050_Init();
  MPU_Write_Byte(MPU_ADDR,MPU_CFG, 0x02);   //将MPU的CFG寄存器的第二位设置为1，其他位在使用MPU时配置
  MPU_Write_Byte(MPU_ADDR,MPU_CTRL, 0x00);  //将MPU的CTRL寄存器的第六位设置为0，与上面一步共同开启bypass模式
	HAL_Delay(200);
	HMC_Init();				        //HMC初始化
  GY86_OOPinit(GY86);
}

//对象实例化
void GY86_OOPinit(GY_86 *GY86){
  GY86->Accel = (Accelerometer *)malloc(sizeof(Accelerometer));
  GY86->Gyro = (Gyroscope *)malloc(sizeof(Gyroscope));
  GY86->Mag = (Magnetic *)malloc(sizeof(Magnetic));
  //dataMem
  GY86->Accel->data = (AccelData *)malloc(sizeof(AccelData));
  GY86->Gyro->data = (GyroData *)malloc(sizeof(GyroData));
  GY86->Mag->data = (MagData *)malloc(sizeof(MagData));
  //data init
  GY86->Accel->data->Accel_offset.x = 0;
  GY86->Accel->data->Accel_offset.x = 0;
  GY86->Accel->data->Accel_offset.x = 0;
  GY86->Accel->data->Accel_scale.x = 1;
  GY86->Accel->data->Accel_scale.y = 1;
  GY86->Accel->data->Accel_scale.z = 1;
  GY86->Accel->data->Accel_raw.x = 0;
  GY86->Accel->data->Accel_raw.y = 0;
  GY86->Accel->data->Accel_raw.z = 0;
  GY86->Accel->data->Accel_ms2.x = 0;
  GY86->Accel->data->Accel_ms2.y = 0;
  GY86->Accel->data->Accel_ms2.z = 0;
  GY86->Accel->getAccelerometer_raw = MPU_Get_Accelerometer;

  GY86->Gyro->data->Gyro_offset.x = 0;
  GY86->Gyro->data->Gyro_offset.y = 0;
  GY86->Gyro->data->Gyro_offset.z = 0;
  GY86->Gyro->data->Gyro_scale.x = 1;
  GY86->Gyro->data->Gyro_scale.y = 1;
  GY86->Gyro->data->Gyro_scale.z = 1;
  GY86->Gyro->data->Gyro_raw.x = 0;
  GY86->Gyro->data->Gyro_raw.y = 0;
  GY86->Gyro->data->Gyro_raw.z = 0;
  GY86->Gyro->data->Gyro_ds.x = 0;
  GY86->Gyro->data->Gyro_ds.y = 0;
  GY86->Gyro->data->Gyro_ds.z = 0;
  GY86->Gyro->getGyroscope_raw = MPU_Get_Gyroscope;

  GY86->Mag->data->Mag_offset.x = 0;
  GY86->Mag->data->Mag_offset.y = 0;
  GY86->Mag->data->Mag_offset.z = 0;
  GY86->Mag->data->Mag_scale.x = 1;
  GY86->Mag->data->Mag_scale.y = 1;
  GY86->Mag->data->Mag_scale.z = 1;
  GY86->Mag->data->Mag_raw.x = 0;
  GY86->Mag->data->Mag_raw.y = 0;
  GY86->Mag->data->Mag_raw.z = 0;
  GY86->Mag->data->Mag_d.x = 0;
  GY86->Mag->data->Mag_d.y = 0;
  GY86->Mag->data->Mag_d.z = 0;
}

//得到GY-86值(原始值)
void GY86_RawDataUpdate(void){
  MPU_Get_Gyroscope(GY86->Gyro->data);
  MPU_Get_Accelerometer(GY86->Accel->data);
}

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(GyroData *data)
{
  uint8_t buf[6],res; 
	
	res = MPU_Read_Len(MPU_ADDR, MPU6050_RA_GYRO_XOUT_H, 6, buf);
	if(res == 0)
	{
		data->Gyro_raw.x = (int)(((uint16_t)buf[0] << 8) | buf[1]) / GYRO_250DPS;  
		data->Gyro_raw.y = (int)(((uint16_t)buf[2] << 8) | buf[3]) / GYRO_250DPS;  
		data->Gyro_raw.z = (int)(((uint16_t)buf[4] << 8) | buf[5]) / GYRO_250DPS;
	} 	
    return res;
}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功逻辑分析仪出现framing error
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(AccelData *data)
{
    uint8_t buf[6],res;  
	res = MPU_Read_Len(MPU_ADDR,MPU6050_RA_ACCEL_XOUT_H, 6, buf);
	if(res == 0)
	{
		data->Accel_raw.x=(int)(((uint16_t)buf[0] << 8) | buf[1]) / ACCEL_2G;  
		data->Accel_raw.y=(int)(((uint16_t)buf[2] << 8) | buf[3]) / ACCEL_2G;  
		data->Accel_raw.z=(int)(((uint16_t)buf[4] << 8) | buf[5]) / ACCEL_2G;
	} 	
    return res;
}

//HMC初始化配置
void HMC_Init(void)
{
	HMC_Write_Byte(HMC_CONFIGA, 0x58);   //01011000/采样平均数4，输出速率75Hz，正常测量配置
	HMC_Write_Byte(HMC_CONFIGB, 0xE0);   //11100000/将增益调至最小
	HMC_Write_Byte(HMC_MODE, 0x00);      //00000000/设置为连续模式
}

//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU6050_Init(void)
{ 
  uint8_t res;
  extern I2C_HandleTypeDef MPU_I2C;
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_PWR_MGMT_1,0X80);	//复位MPU6050
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_PWR_MGMT_1,0X00);	//唤醒MPU6050 
  MPU_Set_Gyro_Fsr(0);					//陀螺仪传感器,±250dps
  MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
  MPU_Set_Rate(200);						//设置采样率200Hz
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_INT_ENABLE,0X00);	//关闭所有中断
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_USER_CTRL,0X00);	//I2C主模式关闭
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_FIFO_EN,0X00);	//关闭FIFO
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_INT_PIN_CFG,0X80);	//INT引脚低电平有效
	HAL_Delay(100);
  MPU_Read_Byte(MPU_ADDR,MPU6050_RA_WHO_AM_I,&res);
  if(res==MPU_ADDR)//器件ID正确
  {
    MPU_Write_Byte(MPU_ADDR,MPU6050_RA_PWR_MGMT_1,0X01);	//设置CLKSEL,PLL X轴为参考
    MPU_Write_Byte(MPU_ADDR,MPU6050_RA_PWR_MGMT_2,0X00);	//加速度与陀螺仪都工作
    MPU_Set_Rate(200);						//设置采样率为50Hz
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
	return MPU_Write_Byte(MPU_ADDR,MPU6050_RA_GYRO_CONFIG, fsr<<3);//设置陀螺仪满量程范围  
}

//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ADDR,MPU6050_RA_ACCEL_CONFIG, fsr<<3);//设置加速度传感器满量程范围  
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
	return MPU_Write_Byte(MPU_ADDR,MPU6050_RA_CONFIG, data);//设置数字低通滤波器  
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
	data=MPU_Write_Byte(MPU_ADDR,MPU6050_RA_SMPLRT_DIV, data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//      其他,错误代码
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data) 				 
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data=0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, (addr<<1), reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  return 0;
}

//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg,uint8_t *data)
{
  extern I2C_HandleTypeDef MPU_I2C;
  
  HAL_I2C_Mem_Read(&MPU_I2C, (addr<<1), reg, I2C_MEMADD_SIZE_8BIT, data, 1, 0xfff);
  return 0;
}

//IIC连续写
//addr:器件地址
//reg:要写的寄存器地址
//len:要写的长度
//buf:写数据的数据存储区
//返回值:0,正常
//      其他,错误代码
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, (addr<<1), reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//      其他,错误代码
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, (addr<<1), reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;	
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
