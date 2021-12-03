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
  ******************************************************************************
  */

#ifndef __GY_86_H__
#define __GY_86_H__

#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "math.h"

#define MPU_I2C     (hi2c2)                   //i2c句柄

#define M_PI 				3.1415926
#define MagnetcDeclination          1.16     //成都市地磁倾角

//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define MPU_ADDR				0X68

//因为MPU6050的AD0接GND,所以则读写地址分别为0XD1和0XD0
//            (如果AD0接VCC,则读写地址分别为0XD3和0XD2)  
#define MPU_READ    0XD1//实际上就是MPU_ADDR左移低位补0还是补1(1101000x)
#define MPU_WRITE   0XD0

#define MPU6050_RA_XG_OFFS_TC       0x00 
#define MPU6050_RA_YG_OFFS_TC       0x01 
#define MPU6050_RA_ZG_OFFS_TC       0x02 
#define MPU6050_RA_X_FINE_GAIN      0x03 
#define MPU6050_RA_Y_FINE_GAIN      0x04 
#define MPU6050_RA_Z_FINE_GAIN      0x05 
#define MPU6050_RA_XA_OFFS_H        0x06 
#define MPU6050_RA_XA_OFFS_L_TC     0x07
#define MPU6050_RA_YA_OFFS_H        0x08 
#define MPU6050_RA_YA_OFFS_L_TC     0x09
#define MPU6050_RA_ZA_OFFS_H        0x0A        
#define MPU6050_RA_ZA_OFFS_L_TC     0x0B
#define MPU6050_RA_XG_OFFS_USRH     0x13 
#define MPU6050_RA_XG_OFFS_USRL     0x14
#define MPU6050_RA_YG_OFFS_USRH     0x15 
#define MPU6050_RA_YG_OFFS_USRL     0x16
#define MPU6050_RA_ZG_OFFS_USRH     0x17 
#define MPU6050_RA_ZG_OFFS_USRL     0x18
#define MPU6050_RA_SMPLRT_DIV       0x19        //采样频率分频器
#define MPU6050_RA_CONFIG           0x1A        //配置寄存器
#define MPU6050_RA_GYRO_CONFIG      0x1B        //陀螺仪配置寄存器
#define MPU6050_RA_ACCEL_CONFIG     0x1C        //加速度计配置寄存器
#define MPU6050_RA_FF_THR           0x1D 
#define MPU6050_RA_FF_DUR           0x1E
#define MPU6050_RA_MOT_THR          0x1F        //运动检测阀值设置寄存器
#define MPU6050_RA_MOT_DUR          0x20
#define MPU6050_RA_ZRMOT_THR        0x21
#define MPU6050_RA_ZRMOT_DUR        0x22
#define MPU6050_RA_FIFO_EN          0x23        //FIFO使能寄存器
#define MPU6050_RA_I2C_MST_CTRL     0x24        //IIC主机控制寄存器
#define MPU6050_RA_I2C_SLV0_ADDR    0x25        //IIC从机0器件地址寄存器
#define MPU6050_RA_I2C_SLV0_REG     0x26        //IIC从机0数据地址寄存器
#define MPU6050_RA_I2C_SLV0_CTRL    0x27        //IIC从机0控制寄存器
#define MPU6050_RA_I2C_SLV1_ADDR    0x28        //IIC从机1器件地址寄存器
#define MPU6050_RA_I2C_SLV1_REG     0x29        //IIC从机1数据地址寄存器
#define MPU6050_RA_I2C_SLV1_CTRL    0x2A        //IIC从机1控制寄存器
#define MPU6050_RA_I2C_SLV2_ADDR    0x2B        //IIC从机2器件地址寄存器
#define MPU6050_RA_I2C_SLV2_REG     0x2C        //IIC从机2数据地址寄存器
#define MPU6050_RA_I2C_SLV2_CTRL    0x2D        //IIC从机2控制寄存器
#define MPU6050_RA_I2C_SLV3_ADDR    0x2E        //IIC从机3器件地址寄存器
#define MPU6050_RA_I2C_SLV3_REG     0x2F        //IIC从机3数据地址寄存器
#define MPU6050_RA_I2C_SLV3_CTRL    0x30        //IIC从机3控制寄存器
#define MPU6050_RA_I2C_SLV4_ADDR    0x31        //IIC从机4器件地址寄存器
#define MPU6050_RA_I2C_SLV4_REG     0x32        //IIC从机4数据地址寄存器
#define MPU6050_RA_I2C_SLV4_DO      0x33        //IIC从机4写数据寄存器
#define MPU6050_RA_I2C_SLV4_CTRL    0x34        //IIC从机4控制寄存器
#define MPU6050_RA_I2C_SLV4_DI      0x35        //IIC从机4读数据寄存器
#define MPU6050_RA_I2C_MST_STATUS   0x36        //IIC主机状态寄存器
#define MPU6050_RA_INT_PIN_CFG      0x37        //中断/旁路设置寄存器
#define MPU6050_RA_INT_ENABLE       0x38        //中断使能寄存器
#define MPU6050_RA_DMP_INT_STATUS   0x39        
#define MPU6050_RA_INT_STATUS       0x3A        //中断状态寄存器
#define MPU6050_RA_ACCEL_XOUT_H     0x3B        //加速度值,X轴高8位寄存器
#define MPU6050_RA_ACCEL_XOUT_L     0x3C        //加速度值,X轴低8位寄存器
#define MPU6050_RA_ACCEL_YOUT_H     0x3D        //加速度值,Y轴高8位寄存器
#define MPU6050_RA_ACCEL_YOUT_L     0x3E        //加速度值,Y轴低8位寄存器
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F        //加速度值,Z轴高8位寄存器
#define MPU6050_RA_ACCEL_ZOUT_L     0x40        //加速度值,Z轴低8位寄存器
#define MPU6050_RA_TEMP_OUT_H       0x41        //温度值高八位寄存器
#define MPU6050_RA_TEMP_OUT_L       0x42        //温度值低8位寄存器
#define MPU6050_RA_GYRO_XOUT_H      0x43        //陀螺仪值,X轴高8位寄存器
#define MPU6050_RA_GYRO_XOUT_L      0x44        //陀螺仪值,X轴低8位寄存器
#define MPU6050_RA_GYRO_YOUT_H      0x45        //陀螺仪值,Y轴高8位寄存器
#define MPU6050_RA_GYRO_YOUT_L      0x46        //陀螺仪值,Y轴低8位寄存器
#define MPU6050_RA_GYRO_ZOUT_H      0x47        //陀螺仪值,Z轴高8位寄存器
#define MPU6050_RA_GYRO_ZOUT_L      0x48        //陀螺仪值,Z轴低8位寄存器
#define MPU6050_RA_EXT_SENS_DATA_00 0x49
#define MPU6050_RA_EXT_SENS_DATA_01 0x4A
#define MPU6050_RA_EXT_SENS_DATA_02 0x4B
#define MPU6050_RA_EXT_SENS_DATA_03 0x4C
#define MPU6050_RA_EXT_SENS_DATA_04 0x4D
#define MPU6050_RA_EXT_SENS_DATA_05 0x4E
#define MPU6050_RA_EXT_SENS_DATA_06 0x4F
#define MPU6050_RA_EXT_SENS_DATA_07 0x50
#define MPU6050_RA_EXT_SENS_DATA_08 0x51
#define MPU6050_RA_EXT_SENS_DATA_09 0x52
#define MPU6050_RA_EXT_SENS_DATA_10 0x53
#define MPU6050_RA_EXT_SENS_DATA_11 0x54
#define MPU6050_RA_EXT_SENS_DATA_12 0x55
#define MPU6050_RA_EXT_SENS_DATA_13 0x56
#define MPU6050_RA_EXT_SENS_DATA_14 0x57
#define MPU6050_RA_EXT_SENS_DATA_15 0x58
#define MPU6050_RA_EXT_SENS_DATA_16 0x59
#define MPU6050_RA_EXT_SENS_DATA_17 0x5A
#define MPU6050_RA_EXT_SENS_DATA_18 0x5B
#define MPU6050_RA_EXT_SENS_DATA_19 0x5C
#define MPU6050_RA_EXT_SENS_DATA_20 0x5D
#define MPU6050_RA_EXT_SENS_DATA_21 0x5E
#define MPU6050_RA_EXT_SENS_DATA_22 0x5F
#define MPU6050_RA_EXT_SENS_DATA_23 0x60
#define MPU6050_RA_MOT_DETECT_STATUS    0x61
#define MPU6050_RA_I2C_SLV0_DO      0x63        //IIC从机0数据寄存器
#define MPU6050_RA_I2C_SLV1_DO      0x64        //IIC从机1数据寄存器
#define MPU6050_RA_I2C_SLV2_DO      0x65        //IIC从机2数据寄存器
#define MPU6050_RA_I2C_SLV3_DO      0x66        //IIC从机3数据寄存器
#define MPU6050_RA_I2C_MST_DELAY_CTRL   0x67    //IIC主机延时管理寄存器
#define MPU6050_RA_SIGNAL_PATH_RESET    0x68    //信号通道复位寄存器
#define MPU6050_RA_MOT_DETECT_CTRL      0x69    //运动检测控制寄存器
#define MPU6050_RA_USER_CTRL        0x6A        //用户控制寄存器
#define MPU6050_RA_PWR_MGMT_1       0x6B        //电源管理寄存器1
#define MPU6050_RA_PWR_MGMT_2       0x6C        //电源管理寄存器2 
#define MPU6050_RA_BANK_SEL         0x6D
#define MPU6050_RA_MEM_START_ADDR   0x6E
#define MPU6050_RA_MEM_R_W          0x6F
#define MPU6050_RA_DMP_CFG_1        0x70
#define MPU6050_RA_DMP_CFG_2        0x71
#define MPU6050_RA_FIFO_COUNTH      0x72        //FIFO计数寄存器高八位
#define MPU6050_RA_FIFO_COUNTL      0x73        //FIFO计数寄存器低八位
#define MPU6050_RA_FIFO_R_W         0x74        //FIFO读写寄存器
#define MPU6050_RA_WHO_AM_I         0x75        //器件ID寄存器,who am i寄存器

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

#define GYRO_250DPS 131.072f
#define ACCEL_2G 1673.469f

//校准参数
typedef struct Vector3f_t{
  float x;
  float y;
  float z;
}Vector3f_t;

typedef struct AccelData{
  Vector3f_t Accel_offset;
  Vector3f_t Accel_scale;
  Vector3f_t Accel_raw;
  Vector3f_t Accel_ms2;
}AccelData;

typedef struct GyroData{
  Vector3f_t Gyro_offset;
  Vector3f_t Gyro_scale;
  Vector3f_t Gyro_raw;
  Vector3f_t Gyro_ds;
}GyroData;

typedef struct MagData{
  Vector3f_t Mag_offset;
  Vector3f_t Mag_scale;
  Vector3f_t Mag_raw;
  Vector3f_t Mag_d;
}MagData;

typedef struct Accelerometer{
  AccelData *data;
  uint8_t (*Accelerometer_gauss_newton)(AccelData *data);
  uint8_t (*getAccelerometer_raw)(AccelData *data);
  uint8_t (*getAccelerometer_ms2)(AccelData *data);
}Accelerometer;

typedef struct Gyroscope{
  GyroData *data;
  uint8_t (*Gyroscope_gauss_newton)(GyroData *data);
  uint8_t (*getGyroscope_raw)(GyroData *data);
  uint8_t (*getGyroscope_ds)(GyroData *data);
}Gyroscope;

typedef struct Magnetic{
  MagData *data;
  uint8_t (*Magnetic_gauss_newton)(MagData const *data);
  uint8_t (*getMagnetic_raw)(MagData const *data);
  uint8_t (*getMagnetic_d)(MagData const *data);
}Magnetic;

typedef struct MS561101BA{
	float Temperature;
	float	pressure;
}MS561101BA;

typedef struct GY_86{
  Accelerometer *Accel;
  Gyroscope *Gyro;
  Magnetic *Mag;
	MS561101BA *MS;
}GY_86;

//filter
typedef struct pt2Filter_s {
    float state;
    float state1;
    float k;
} pt2Filter_t;

extern GY_86 *GY86;   
extern Vector3f_t Accel_raw[6], Gyro_raw[6], Mag_raw[6];

uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data);    //IIC写一个字节
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg,uint8_t *data);		//IIC读一个字节
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);		//IIC连续写
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);     //IIC连续读 
uint8_t HMC_Write_Byte(uint8_t reg,uint8_t data);
uint8_t HMC_Read_Byte(uint8_t reg);
uint8_t HMC_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t HMC_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf);

void GY86_Init(void);
void GY86_OOPinit(GY_86 *GY86);
void HMC_Init(void);
uint8_t MPU6050_Init(void);

uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr);
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr);
uint8_t MPU_Set_Rate(uint16_t rate);
uint8_t MPU_Set_LPF(uint16_t lpf);

uint8_t MPU_Get_Gyroscope(GyroData *data);
uint8_t MPU_Get_Accelerometer(AccelData *data);
uint8_t READ_HMCALL(MagData *data);
uint8_t MPU_Get_Temperature(MS561101BA* Temperature);

void GY86_Offset(void);
void GY86_RawDataUpdate(void);

//window filter
#define WINDOW_SIZE 6
float window_filter(float data, int *phead, float* window_buffer);
void qsort(float *a, int left, int right);

//PT2
#define PT2_FILTER
#define M_PIf       3.14159265358979323846f
#define ACCEL_LPF_CUTOFF_FREQ 	15.0f
#define GYRO_LPF_CUTOFF_FREQ  	40.0f
float pt2FilterGain(float f_cut, float dT);
void pt2FilterInit(pt2Filter_t *filter, float k);
void pt2FilterUpdateCutoff(pt2Filter_t *filter, float k);
float pt2FilterApply(pt2Filter_t *filter, float input);
void pt2init(void);
void pt2update(void);

#endif /* __GY_86_H__ */



