/**
  ******************************************************************************
  * �ļ�����: mpu6050.c 
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
  ******************************************************************************
  */

#include "mpu6050.h" 
#include "stdio.h"

//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//      ����,�������
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data) 				 
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data=0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, MPU_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  HAL_Delay(100);
  
  return 0;
}

//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
uint8_t MPU_Read_Byte(uint8_t reg)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char R_Data=1;
  
  HAL_I2C_Mem_Read(&MPU_I2C, MPU_READ, reg, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xfff);
  HAL_Delay(100);

  return R_Data;
}

//IIC����д
//addr:������ַ
//reg:Ҫд�ļĴ�����ַ
//len:Ҫд�ĳ���
//buf:д���ݵ����ݴ洢��
//����ֵ:0,����
//      ����,�������
uint8_t MPU_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, MPU_WRITE, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  HAL_Delay(100);
  
  return 0;
}

//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//      ����,�������
uint8_t MPU_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, MPU_READ, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  HAL_Delay(100);
  
  return 0;	
}

//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU6050_Init(void)
{ 
  uint8_t res;
  extern I2C_HandleTypeDef MPU_I2C;
  MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X80);	//��λMPU6050
  MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X00);	//����MPU6050 
  MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
  MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
  MPU_Set_Rate(50);						//���ò�����50Hz
  MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0X00);	//�ر������ж�
  MPU_Write_Byte(MPU6050_RA_USER_CTRL,0X00);	//I2C��ģʽ�ر�
  MPU_Write_Byte(MPU6050_RA_FIFO_EN,0X00);	//�ر�FIFO
  MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X80);	//INT���ŵ͵�ƽ��Ч
  res=MPU_Read_Byte(MPU6050_RA_WHO_AM_I);
  if(res==MPU_ADDR)//����ID��ȷ
  {
    MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
    MPU_Write_Byte(MPU6050_RA_PWR_MGMT_2,0X00);	//���ٶ��������Ƕ�����
    MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
  }else
  {
		return 1;
	}
  return 0;
}

//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6050_RA_GYRO_CONFIG, fsr<<3);//���������������̷�Χ  
}

//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6050_RA_ACCEL_CONFIG, fsr<<3);//���ü��ٶȴ����������̷�Χ  
}

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU6050_RA_CONFIG, data);//�������ֵ�ͨ�˲���  
}

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU6050_RA_SMPLRT_DIV, data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
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

//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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

//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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
