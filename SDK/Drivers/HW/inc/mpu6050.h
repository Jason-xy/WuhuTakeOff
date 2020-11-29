/**
  ******************************************************************************
  * �ļ�����: mpu6050.h 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-10-2
  * ��    ��: MPU6050��ʼ��
  ******************************************************************************
  * ˵����
  * 1.ʹ����GY-86ģ�顣
  * 2.��Ҫʵ�����ú�I2C������
  * 
  * ���ܣ�
  * 1.MPU6050��ʼ����
  * 2.I2C������д��
  * 3.���������ݻ�ȡ��
  * 4.���ٶȼ����ݻ�ȡ��
  * 5.�¶ȼ����ݻ�ȡ�ͽ�����
  * 6.�궨������Ĵ�����ַ��
  ******************************************************************************
  */

#ifndef __MPU6050_H
#define __MPU6050_H	

#include "stm32f4xx_hal.h"
#include "i2c.h"

#define MPU6050_ADD	0xD0	                   //������ַ��AD0���ջ�͵�ƽʱ��ַ��0xD0��Ϊ�ߵ�ƽʱΪ0xD2��7λ��ַ��1101 000x��
#define MPU_I2C     (hi2c2)                      //i2c���

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
#define MPU6050_RA_SMPLRT_DIV       0x19        //����Ƶ�ʷ�Ƶ��
#define MPU6050_RA_CONFIG           0x1A        //���üĴ���
#define MPU6050_RA_GYRO_CONFIG      0x1B        //���������üĴ���
#define MPU6050_RA_ACCEL_CONFIG     0x1C        //���ٶȼ����üĴ���
#define MPU6050_RA_FF_THR           0x1D 
#define MPU6050_RA_FF_DUR           0x1E
#define MPU6050_RA_MOT_THR          0x1F        //�˶���ֵⷧ���üĴ���
#define MPU6050_RA_MOT_DUR          0x20
#define MPU6050_RA_ZRMOT_THR        0x21
#define MPU6050_RA_ZRMOT_DUR        0x22
#define MPU6050_RA_FIFO_EN          0x23        //FIFOʹ�ܼĴ���
#define MPU6050_RA_I2C_MST_CTRL     0x24        //IIC�������ƼĴ���
#define MPU6050_RA_I2C_SLV0_ADDR    0x25        //IIC�ӻ�0������ַ�Ĵ���
#define MPU6050_RA_I2C_SLV0_REG     0x26        //IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU6050_RA_I2C_SLV0_CTRL    0x27        //IIC�ӻ�0���ƼĴ���
#define MPU6050_RA_I2C_SLV1_ADDR    0x28        //IIC�ӻ�1������ַ�Ĵ���
#define MPU6050_RA_I2C_SLV1_REG     0x29        //IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU6050_RA_I2C_SLV1_CTRL    0x2A        //IIC�ӻ�1���ƼĴ���
#define MPU6050_RA_I2C_SLV2_ADDR    0x2B        //IIC�ӻ�2������ַ�Ĵ���
#define MPU6050_RA_I2C_SLV2_REG     0x2C        //IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU6050_RA_I2C_SLV2_CTRL    0x2D        //IIC�ӻ�2���ƼĴ���
#define MPU6050_RA_I2C_SLV3_ADDR    0x2E        //IIC�ӻ�3������ַ�Ĵ���
#define MPU6050_RA_I2C_SLV3_REG     0x2F        //IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU6050_RA_I2C_SLV3_CTRL    0x30        //IIC�ӻ�3���ƼĴ���
#define MPU6050_RA_I2C_SLV4_ADDR    0x31        //IIC�ӻ�4������ַ�Ĵ���
#define MPU6050_RA_I2C_SLV4_REG     0x32        //IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU6050_RA_I2C_SLV4_DO      0x33        //IIC�ӻ�4д���ݼĴ���
#define MPU6050_RA_I2C_SLV4_CTRL    0x34        //IIC�ӻ�4���ƼĴ���
#define MPU6050_RA_I2C_SLV4_DI      0x35        //IIC�ӻ�4�����ݼĴ���
#define MPU6050_RA_I2C_MST_STATUS   0x36        //IIC����״̬�Ĵ���
#define MPU6050_RA_INT_PIN_CFG      0x37        //�ж�/��·���üĴ���
#define MPU6050_RA_INT_ENABLE       0x38        //�ж�ʹ�ܼĴ���
#define MPU6050_RA_DMP_INT_STATUS   0x39        
#define MPU6050_RA_INT_STATUS       0x3A        //�ж�״̬�Ĵ���
#define MPU6050_RA_ACCEL_XOUT_H     0x3B        //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU6050_RA_ACCEL_XOUT_L     0x3C        //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU6050_RA_ACCEL_YOUT_H     0x3D        //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU6050_RA_ACCEL_YOUT_L     0x3E        //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F        //���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU6050_RA_ACCEL_ZOUT_L     0x40        //���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU6050_RA_TEMP_OUT_H       0x41        //�¶�ֵ�߰�λ�Ĵ���
#define MPU6050_RA_TEMP_OUT_L       0x42        //�¶�ֵ��8λ�Ĵ���
#define MPU6050_RA_GYRO_XOUT_H      0x43        //������ֵ,X���8λ�Ĵ���
#define MPU6050_RA_GYRO_XOUT_L      0x44        //������ֵ,X���8λ�Ĵ���
#define MPU6050_RA_GYRO_YOUT_H      0x45        //������ֵ,Y���8λ�Ĵ���
#define MPU6050_RA_GYRO_YOUT_L      0x46        //������ֵ,Y���8λ�Ĵ���
#define MPU6050_RA_GYRO_ZOUT_H      0x47        //������ֵ,Z���8λ�Ĵ���
#define MPU6050_RA_GYRO_ZOUT_L      0x48        //������ֵ,Z���8λ�Ĵ���
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
#define MPU6050_RA_I2C_SLV0_DO      0x63        //IIC�ӻ�0���ݼĴ���
#define MPU6050_RA_I2C_SLV1_DO      0x64        //IIC�ӻ�1���ݼĴ���
#define MPU6050_RA_I2C_SLV2_DO      0x65        //IIC�ӻ�2���ݼĴ���
#define MPU6050_RA_I2C_SLV3_DO      0x66        //IIC�ӻ�3���ݼĴ���
#define MPU6050_RA_I2C_MST_DELAY_CTRL   0x67    //IIC������ʱ����Ĵ���
#define MPU6050_RA_SIGNAL_PATH_RESET    0x68    //�ź�ͨ����λ�Ĵ���
#define MPU6050_RA_MOT_DETECT_CTRL      0x69    //�˶������ƼĴ���
#define MPU6050_RA_USER_CTRL        0x6A        //�û����ƼĴ���
#define MPU6050_RA_PWR_MGMT_1       0x6B        //��Դ����Ĵ���1
#define MPU6050_RA_PWR_MGMT_2       0x6C        //��Դ����Ĵ���2 
#define MPU6050_RA_BANK_SEL         0x6D
#define MPU6050_RA_MEM_START_ADDR   0x6E
#define MPU6050_RA_MEM_R_W          0x6F
#define MPU6050_RA_DMP_CFG_1        0x70
#define MPU6050_RA_DMP_CFG_2        0x71
#define MPU6050_RA_FIFO_COUNTH      0x72        //FIFO�����Ĵ����߰�λ
#define MPU6050_RA_FIFO_COUNTL      0x73        //FIFO�����Ĵ����Ͱ�λ
#define MPU6050_RA_FIFO_R_W         0x74        //FIFO��д�Ĵ���
#define MPU6050_RA_WHO_AM_I         0x75        //����ID�Ĵ���,who am i�Ĵ���

//���AD0��(9��)�ӵ�,IIC��ַΪ0X68(���������λ).
//�����V3.3,��IIC��ַΪ0X69(���������λ).
#define MPU_ADDR				0X68

//��ΪMPU6050��AD0��GND,�������д��ַ�ֱ�Ϊ0XD1��0XD0
//            (���AD0��VCC,���д��ַ�ֱ�Ϊ0XD3��0XD2)  
#define MPU_READ    0XD1
#define MPU_WRITE   0XD0

uint8_t MPU_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf);    //IIC����д
uint8_t MPU_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf);     //IIC������ 
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data);				//IICдһ���ֽ�
uint8_t MPU_Read_Byte(uint8_t reg);					            //IIC��һ���ֽ�

uint8_t MPU6050_Init(void);
uint8_t MPU_Get_Gyroscope(uint16_t *gx,uint16_t *gy,uint16_t *gz);
uint8_t MPU_Get_Accelerometer(uint16_t *ax,uint16_t *ay,uint16_t *az);
float MPU_Get_Temperature(void);
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr);
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr);
uint8_t MPU_Set_Rate(uint16_t rate);

//void MPU6050_READ(u16* n);   

#endif /* __MPU6050_H */



