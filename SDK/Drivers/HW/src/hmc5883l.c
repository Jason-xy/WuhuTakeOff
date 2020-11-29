/**
  ******************************************************************************
  * �ļ�����: hmc5883l.c 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-10-7
  * ��    ��: �ش��ǳ�ʼ��
  ******************************************************************************
  * ˵����
  * 1.ʹ����GY-86ģ�顣
  * 2.��Ҫ�������ú�I2C������
  * 3.��Ҫ�������ú�MPU6050��
  * 
  * ���ܣ�
  * 1.hmc5883l�ش��ǳ�ʼ����
  * 2.GY-86��ʼ����
  * 3.I2C������д��
  * 4.���������ݻ�ȡ��
  ******************************************************************************
  */

#include "hmc5883l.h"

//GY-86��ʼ������
void GY86_Init(void)
{
  MPU6050_Init();
	MPU_Write_Byte(MPU_CFG, 0x02);   //��MPU��CFG�Ĵ����ĵڶ�λ����Ϊ1������λ��ʹ��MPUʱ����
	MPU_Write_Byte(MPU_CTRL, 0x00);  //��MPU��CTRL�Ĵ����ĵ���λ����Ϊ0��������һ����ͬ����bypassģʽ
	HAL_Delay(200);
	HMC_Init();				        //HMC��ʼ��
}

//HMC��ʼ������
void HMC_Init(void)
{

	HMC_Write_Byte(HMC_CONFIGA, 0x50);   //01010000/����ƽ����4���������15��������������
	HMC_Write_Byte(HMC_CONFIGB, 0xE0);   //11100000/�����������С
	HMC_Write_Byte(HMC_MODE, 0x00);      //00000000/����Ϊ����ģʽ
}

//HMC���ݶ�ȡ
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

//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//      ����,�������
uint8_t HMC_Write_Byte(uint8_t reg,uint8_t data) 				 
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data=0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, HMC_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  HAL_Delay(100);
  
  return 0;
}

//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
uint8_t HMC_Read_Byte(uint8_t reg)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char R_Data=1;
  
  HAL_I2C_Mem_Read(&MPU_I2C, HMC_READ, reg, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xfff);
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
uint8_t HMC_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, HMC_WRITE, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
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
uint8_t HMC_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, HMC_READ, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  HAL_Delay(100);
  
  return 0;	
}

