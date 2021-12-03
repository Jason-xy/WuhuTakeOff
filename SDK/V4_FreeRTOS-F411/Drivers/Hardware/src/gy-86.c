/**
 ******************************************************************************
 * 文件名程: gy-86.h
 * 作    者: Jason_xy
 * 个人博客：https://jason-xy.cn
 * 版    本: V1.2.2
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
 * 2021-11-27
 * 1.添加去极值的滑动窗口滤波算法
 * 2.添加数字二阶低通滤波
 ******************************************************************************
 */
#include "gy-86.h"

// GY-86
GY_86 *GY86;
Vector3f_t Accel_raw[6], Gyro_raw[6], Mag_raw[6];

// window filter
float Accel_x_buffer[WINDOW_SIZE];
int Accel_x_head;
float Accel_y_buffer[WINDOW_SIZE];
int Accel_y_head;
float Accel_z_buffer[WINDOW_SIZE];
int Accel_z_head;
float Gyro_x_buffer[WINDOW_SIZE];
int Gyro_x_head;
float Gyro_y_buffer[WINDOW_SIZE];
int Gyro_y_head;
float Gyro_z_buffer[WINDOW_SIZE];
int Gyro_z_head;

// PT2 filter
pt2Filter_t Accel_x_PT2, Accel_y_PT2, Accel_z_PT2, Gyro_x_PT2, Gyro_y_PT2, Gyro_z_PT2;

// GY-86初始化配置
void GY86_Init(void)
{
  MPU6050_Init();
  MPU_Write_Byte(MPU_ADDR, MPU_CFG, 0x02);  //将MPU的CFG寄存器的第二位设置为1，其他位在使用MPU时配置
  MPU_Write_Byte(MPU_ADDR, MPU_CTRL, 0x00); //将MPU的CTRL寄存器的第六位设置为0，与上面一步共同开启bypass模式
  HAL_Delay(200);
  HMC_Init(); // HMC初始化
  GY86_OOPinit(GY86);
  GY86_Offset();
  pt2init();
}

//对象实例化
void GY86_OOPinit(GY_86 *GY86)
{
  GY86->Accel = (Accelerometer *)malloc(sizeof(Accelerometer));
  GY86->Gyro = (Gyroscope *)malloc(sizeof(Gyroscope));
  GY86->Mag = (Magnetic *)malloc(sizeof(Magnetic));
  GY86->MS = (MS561101BA *)malloc(sizeof(MS561101BA));
  // dataMem
  GY86->Accel->data = (AccelData *)malloc(sizeof(AccelData));
  GY86->Gyro->data = (GyroData *)malloc(sizeof(GyroData));
  GY86->Mag->data = (MagData *)malloc(sizeof(MagData));
  // data init
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
void GY86_RawDataUpdate(void)
{
  //更新陀螺仪数据
  MPU_Get_Gyroscope(GY86->Gyro->data);
#ifdef PT2_FILTER
  pt2update();
  GY86->Gyro->data->Gyro_ds.x = pt2FilterApply(&Gyro_x_PT2, (GY86->Gyro->data->Gyro_raw.x - GY86->Gyro->data->Gyro_offset.x) * GY86->Gyro->data->Gyro_scale.x / GYRO_250DPS);
  GY86->Gyro->data->Gyro_ds.y = pt2FilterApply(&Gyro_y_PT2, (GY86->Gyro->data->Gyro_raw.y - GY86->Gyro->data->Gyro_offset.y) * GY86->Gyro->data->Gyro_scale.y / GYRO_250DPS);
  GY86->Gyro->data->Gyro_ds.z = pt2FilterApply(&Gyro_z_PT2, (GY86->Gyro->data->Gyro_raw.z - GY86->Gyro->data->Gyro_offset.z) * GY86->Gyro->data->Gyro_scale.z / GYRO_250DPS);
#endif
#ifdef WINDOW_FILTER
  GY86->Gyro->data->Gyro_ds.x = window_filter((GY86->Gyro->data->Gyro_raw.x - GY86->Gyro->data->Gyro_offset.x) * GY86->Gyro->data->Gyro_scale.x / GYRO_250DPS, &Gyro_x_head, Gyro_x_buffer);
  GY86->Gyro->data->Gyro_ds.y = window_filter((GY86->Gyro->data->Gyro_raw.y - GY86->Gyro->data->Gyro_offset.y) * GY86->Gyro->data->Gyro_scale.y / GYRO_250DPS, &Gyro_y_head, Gyro_y_buffer);
  GY86->Gyro->data->Gyro_ds.z = window_filter((GY86->Gyro->data->Gyro_raw.z - GY86->Gyro->data->Gyro_offset.z) * GY86->Gyro->data->Gyro_scale.z / GYRO_250DPS, &Gyro_z_head, Gyro_z_buffer);
#endif
#ifdef NONE_FILTER
  GY86->Gyro->data->Gyro_ds.x = (GY86->Gyro->data->Gyro_raw.x - GY86->Gyro->data->Gyro_offset.x) * GY86->Gyro->data->Gyro_scale.x / GYRO_250DPS;
  GY86->Gyro->data->Gyro_ds.y = (GY86->Gyro->data->Gyro_raw.y - GY86->Gyro->data->Gyro_offset.y) * GY86->Gyro->data->Gyro_scale.y / GYRO_250DPS;
  GY86->Gyro->data->Gyro_ds.z = (GY86->Gyro->data->Gyro_raw.z - GY86->Gyro->data->Gyro_offset.z) * GY86->Gyro->data->Gyro_scale.z / GYRO_250DPS;
#endif
  //更新加速度计数据
  MPU_Get_Accelerometer(GY86->Accel->data);
#ifdef PT2_FILTER
  GY86->Accel->data->Accel_ms2.x = pt2FilterApply(&Accel_x_PT2, (GY86->Accel->data->Accel_raw.x - GY86->Accel->data->Accel_offset.x) * GY86->Accel->data->Accel_scale.x / ACCEL_2G);
  GY86->Accel->data->Accel_ms2.y = pt2FilterApply(&Accel_y_PT2, (GY86->Accel->data->Accel_raw.y - GY86->Accel->data->Accel_offset.y) * GY86->Accel->data->Accel_scale.y / ACCEL_2G);
  GY86->Accel->data->Accel_ms2.z = pt2FilterApply(&Accel_z_PT2, (GY86->Accel->data->Accel_raw.z - GY86->Accel->data->Accel_offset.z) * GY86->Accel->data->Accel_scale.z / ACCEL_2G);
#endif
#ifdef WINDOW_FILTER
  GY86->Accel->data->Accel_ms2.x = window_filter((GY86->Accel->data->Accel_raw.x - GY86->Accel->data->Accel_offset.x) * GY86->Accel->data->Accel_scale.x / ACCEL_2G, &Accel_x_head, Accel_x_buffer);
  GY86->Accel->data->Accel_ms2.y = window_filter((GY86->Accel->data->Accel_raw.y - GY86->Accel->data->Accel_offset.y) * GY86->Accel->data->Accel_scale.y / ACCEL_2G, &Accel_y_head, Accel_y_buffer);
  GY86->Accel->data->Accel_ms2.z = window_filter((GY86->Accel->data->Accel_raw.z - GY86->Accel->data->Accel_offset.z) * GY86->Accel->data->Accel_scale.z / ACCEL_2G, &Accel_z_head, Accel_z_buffer);
#endif
#ifdef NONE_FILTER
  GY86->Accel->data->Accel_ms2.x = (GY86->Accel->data->Accel_raw.x - GY86->Accel->data->Accel_offset.x) * GY86->Accel->data->Accel_scale.x / ACCEL_2G;
  GY86->Accel->data->Accel_ms2.y = (GY86->Accel->data->Accel_raw.y - GY86->Accel->data->Accel_offset.y) * GY86->Accel->data->Accel_scale.y / ACCEL_2G;
  GY86->Accel->data->Accel_ms2.z = (GY86->Accel->data->Accel_raw.z - GY86->Accel->data->Accel_offset.z) * GY86->Accel->data->Accel_scale.z / ACCEL_2G;
#endif
  //更新磁力计数据
  READ_HMCALL(GY86->Mag->data);
  //更新温度计数据
  MPU_Get_Temperature(GY86->MS);
}

//得到陀螺仪值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(GyroData *data)
{
  uint8_t buf[6], res;

  res = MPU_Read_Len(MPU_ADDR, MPU6050_RA_GYRO_XOUT_H, 6, buf);
  if (res == 0)
  {
    data->Gyro_raw.x = (short)(((int16_t)buf[0] << 8) | buf[1]);
    data->Gyro_raw.y = (short)(((int16_t)buf[2] << 8) | buf[3]);
    data->Gyro_raw.z = (short)(((int16_t)buf[4] << 8) | buf[5]);
  }
  return res;
}

//得到加速度值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(AccelData *data)
{
  uint8_t buf[6], res;
  res = MPU_Read_Len(MPU_ADDR, MPU6050_RA_ACCEL_XOUT_H, 6, buf);
  if (res == 0)
  {
    data->Accel_raw.x = (short)(((int16_t)buf[0] << 8) | buf[1]);
    data->Accel_raw.y = (short)(((int16_t)buf[2] << 8) | buf[3]);
    data->Accel_raw.z = (short)(((int16_t)buf[4] << 8) | buf[5]);
  }
  return res;
}

//得到磁力计值(原始值)
// mx,my,mz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t READ_HMCALL(MagData *data)
{
  uint8_t buf[6], res;
  res = HMC_Read_Len(HMC_DATA_XMSB, 6, buf);
  if (res == 0)
  {
    data->Mag_raw.x = (short)(((int16_t)buf[0] << 8) | buf[1]);
    data->Mag_raw.y = (short)(((int16_t)buf[4] << 8) | buf[5]);
    data->Mag_raw.z = (short)(((int16_t)buf[2] << 8) | buf[3]);
    // 读取的原数据为补码形式，这里完成转换
    if (data->Mag_raw.x > 0x7fff)
      data->Mag_raw.x -= 0xffff;
    if (data->Mag_raw.y > 0x7fff)
      data->Mag_raw.y -= 0xffff;
    if (data->Mag_raw.z > 0x7fff)
      data->Mag_raw.z -= 0xffff;
  }
  return res;
}

//得到温度值
//返回值:温度值(扩大了100倍)
uint8_t MPU_Get_Temperature(MS561101BA *MS)
{
  unsigned char buf[2];
  int16_t raw, res;

  res = MPU_Read_Len(MPU_ADDR, MPU6050_RA_TEMP_OUT_H, 2, buf);
  if (res == 0)
  {
    raw = (buf[0] << 8) | buf[1];
    MS->Temperature = (36.53f + ((double)raw) / 340.0f);
  }
  return res;
}

// GY-86零偏校正
void GY86_Offset(void)
{
  static float ACC_X = 0, ACC_Y = 0, ACC_Z = 0;
  static float GYRO_X = 0, GYRO_Y = 0, GYRO_Z = 0;
  int i = 0;
  for (i = 0; i < 100; i++)
  {
    GY86_RawDataUpdate();
    ACC_X += GY86->Accel->data->Accel_raw.x;
    ACC_Y += GY86->Accel->data->Accel_raw.y;
    ACC_Z += GY86->Accel->data->Accel_raw.z;

    GYRO_X += GY86->Gyro->data->Gyro_raw.x;
    GYRO_Y += GY86->Gyro->data->Gyro_raw.y;
    GYRO_Z += GY86->Gyro->data->Gyro_raw.z;
  }
  GY86->Accel->data->Accel_offset.x = ACC_X / 100;
  GY86->Accel->data->Accel_offset.y = ACC_Y / 100;
  GY86->Accel->data->Accel_offset.z = ACC_Z / 100 - 16384; //+-2g量程

  GY86->Gyro->data->Gyro_offset.x = GYRO_X / 100;
  GY86->Gyro->data->Gyro_offset.y = GYRO_Y / 100;
  GY86->Gyro->data->Gyro_offset.z = GYRO_Z / 100;
}

// HMC初始化配置
void HMC_Init(void)
{
  HMC_Write_Byte(HMC_CONFIGA, 0x58); // 01011000/采样平均数4，输出速率75Hz，正常测量配置
  HMC_Write_Byte(HMC_CONFIGB, 0xE0); // 11100000/将增益调至最小
  HMC_Write_Byte(HMC_MODE, 0x00);    // 00000000/设置为连续模式
}

//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU6050_Init(void)
{
  uint8_t res;
  extern I2C_HandleTypeDef MPU_I2C;
  MPU_Write_Byte(MPU_ADDR, MPU6050_RA_PWR_MGMT_1, 0X00); //唤醒MPU6050
  HAL_Delay(100);
  MPU_Set_Gyro_Fsr(0); //陀螺仪传感器,±250dps
  HAL_Delay(100);
  MPU_Set_Accel_Fsr(0); //加速度传感器,±2g
  HAL_Delay(100);
  MPU_Set_Rate(1000); //设置采样率1000Hz
  HAL_Delay(100);
  MPU_Write_Byte(MPU_ADDR, MPU6050_RA_USER_CTRL, 0X00); // I2C主模式关闭
  HAL_Delay(100);
  MPU_Write_Byte(MPU_ADDR, MPU6050_RA_FIFO_EN, 0X00); //关闭FIFO
  HAL_Delay(100);
  MPU_Read_Byte(MPU_ADDR, MPU6050_RA_WHO_AM_I, &res);
  if (res == MPU_ADDR) //器件ID正确
  {
    MPU_Write_Byte(MPU_ADDR, MPU6050_RA_PWR_MGMT_1, 0X01); //设置CLKSEL,PLL X轴为参考
    MPU_Write_Byte(MPU_ADDR, MPU6050_RA_PWR_MGMT_2, 0X00); //加速度与陀螺仪都工作
  }
  else
    return 1;
  return 0;
}

//设置MPU6050陀螺仪传感器满量程范围
// fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
  return MPU_Write_Byte(MPU_ADDR, MPU6050_RA_GYRO_CONFIG, fsr << 3); //设置陀螺仪满量程范围
}

//设置MPU6050加速度传感器满量程范围
// fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
  return MPU_Write_Byte(MPU_ADDR, MPU6050_RA_ACCEL_CONFIG, fsr << 3); //设置加速度传感器满量程范围
}

//设置MPU6050的数字低通滤波器
// lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_LPF(uint16_t lpf)
{
  uint8_t data = 0;
  if (lpf >= 188)
    data = 1;
  else if (lpf >= 98)
    data = 2;
  else if (lpf >= 42)
    data = 3;
  else if (lpf >= 20)
    data = 4;
  else if (lpf >= 10)
    data = 5;
  else
    data = 6;
  return MPU_Write_Byte(MPU_ADDR, MPU6050_RA_CONFIG, data); //设置数字低通滤波器
}

//设置MPU6050的采样率(假定Fs=1KHz)
// rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Rate(uint16_t rate)
{
  uint8_t data;
  if (rate > 1000)
    rate = 1000;
  if (rate < 4)
    rate = 4;
  data = 1000 / rate - 1;
  data = MPU_Write_Byte(MPU_ADDR, MPU6050_RA_SMPLRT_DIV, data); //设置数字低通滤波器
  return MPU_Set_LPF(rate / 2);                                 //自动设置LPF为采样率的一半
}

// IIC写一个字节
// reg:寄存器地址
// data:数据
//返回值:0,正常
//       其他,错误代码
uint8_t MPU_Write_Byte(uint8_t addr, uint8_t reg, uint8_t data)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data = 0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  return 0;
}

// IIC读一个字节
// reg:寄存器地址
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t addr, uint8_t reg, uint8_t *data)
{
  extern I2C_HandleTypeDef MPU_I2C;

  HAL_I2C_Mem_Read(&MPU_I2C, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, data, 1, 0xfff);
  return 0;
}

// IIC连续写
// addr:器件地址
// reg:要写的寄存器地址
// len:要写的长度
// buf:写数据的数据存储区
//返回值:0,正常
//       其他,错误代码
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

// IIC连续读
// addr:器件地址
// reg:要读取的寄存器地址
// len:要读取的长度
// buf:读取到的数据存储区
//返回值:0,正常
//       其他,错误代码
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

// IIC写一个字节
// reg:寄存器地址
// data:数据
//返回值:0,正常
//       其他,错误代码
uint8_t HMC_Write_Byte(uint8_t reg, uint8_t data)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data = 0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, HMC_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  return 0;
}

// IIC读一个字节
// reg:寄存器地址
//返回值:读到的数据
uint8_t HMC_Read_Byte(uint8_t reg)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char R_Data = 1;

  HAL_I2C_Mem_Read(&MPU_I2C, HMC_READ, reg, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xfff);
  return R_Data;
}

// IIC连续写
// addr:器件地址
// reg:要写的寄存器地址
// len:要写的长度
// buf:写数据的数据存储区
//返回值:0,正常
//       其他,错误代码
uint8_t HMC_Write_Len(uint8_t reg, uint8_t len, uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, HMC_WRITE, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

// IIC连续读
// addr:器件地址
// reg:要读取的寄存器地址
// len:要读取的长度
// buf:读取到的数据存储区
//返回值:0,正常
//       其他,错误代码
uint8_t HMC_Read_Len(uint8_t reg, uint8_t len, uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, HMC_READ, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

// window filter
float window_filter(float data, int *phead, float *window_buffer)
{
  int idx;
  float sum = 0;
  //装载新数据，覆盖旧数据
  window_buffer[*phead] = data;
  *phead = (*phead + 1) % WINDOW_SIZE;
  //就地排序
  qsort(window_buffer, 0, WINDOW_SIZE - 1);
  //去极值求和
  for (idx = 1; idx < WINDOW_SIZE - 1; idx++)
  {
    sum += window_buffer[idx];
  }

  return sum / (WINDOW_SIZE - 2);
}

void qsort(float *a, int left, int right)
{
  int i, j, temp;
  i = left;
  j = right;
  temp = a[left];
  if (left > right)
    return;
  while (i != j) /*i不等于j时，循环进行*/
  {
    while (a[j] >= temp && j > i)
      j--;
    if (j > i)
      a[i++] = a[j];
    while (a[i] <= temp && j > i)
      i++;
    if (j > i)
      a[j--] = a[i];
  }
  a[i] = temp;
  qsort(a, left, i - 1);  /*对小于基准元素的部分进行快速排序*/
  qsort(a, i + 1, right); /*对大于基准元素的部分进行快速排序*/
}

// PT2 Low Pass filter
float PT2Dt;
float pt2FilterGain(float f_cut, float dT)
{
  const float order = 2.0f;
  const float orderCutoffCorrection = 1 / sqrtf(powf(2, 1.0f / order) - 1);
  float RC = 1 / (2 * orderCutoffCorrection * M_PIf * f_cut);
  // float RC = 1 / (2 * 1.553773974f * M_PIf * f_cut);
  // where 1.553773974 = 1 / sqrt( (2^(1 / order) - 1) ) and order is 2
  return dT / (RC + dT);
}

void pt2FilterInit(pt2Filter_t *filter, float k)
{
  filter->state = 0.0f;
  filter->state1 = 0.0f;
  filter->k = k;
}

void pt2FilterUpdateCutoff(pt2Filter_t *filter, float k)
{
  filter->k = k;
}

float pt2FilterApply(pt2Filter_t *filter, float input)
{
  filter->state1 = filter->state1 + filter->k * (input - filter->state1);
  filter->state = filter->state + filter->k * (filter->state1 - filter->state);
  return filter->state;
}

void pt2init(void)
{
  pt2FilterInit(&Accel_x_PT2, 0);
  pt2FilterInit(&Accel_y_PT2, 0);
  pt2FilterInit(&Accel_z_PT2, 0);
  pt2FilterInit(&Gyro_x_PT2, 0);
  pt2FilterInit(&Gyro_y_PT2, 0);
  pt2FilterInit(&Gyro_z_PT2, 0);
}

void pt2update(void)
{
  pt2FilterUpdateCutoff(&Accel_x_PT2, pt2FilterGain(ACCEL_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Accel_y_PT2, pt2FilterGain(ACCEL_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Accel_z_PT2, pt2FilterGain(ACCEL_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Gyro_x_PT2, pt2FilterGain(GYRO_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Gyro_y_PT2, pt2FilterGain(GYRO_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Gyro_z_PT2, pt2FilterGain(GYRO_LPF_CUTOFF_FREQ, PT2Dt));
}