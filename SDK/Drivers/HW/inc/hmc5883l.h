/**
  ******************************************************************************
  * �ļ�����: hmc5883l.h 
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
  * 5.�궨������Ĵ�����ַ
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "mpu6050.h"

#define MPU_CFG 0x37    //�üĴ���������ļĴ���������������MPU��ͨģʽ
#define MPU_CTRL 0x6A
#define HMC_CONFIGA 0x00  //���üĴ���A
#define HMC_CONFIGB 0x01   //���üĴ���B
#define HMC_MODE 0x02      //ģʽ�Ĵ���
#define HMC_DATA_XMSB 0x03  //���ݼĴ���X��λ
#define HMC_DATA_XLSB 0x04  //���ݼĴ���X��λ
#define HMC_DATA_ZMSB 0x05  //���ݼĴ���Z��λ
#define HMC_DATA_ZLSB 0x06  //���ݼĴ���Z��λ
#define HMC_DATA_YMSB 0x07  //���ݼĴ���Y��λ
#define HMC_DATA_YLSB 0x08  //���ݼĴ���Y��λ
#define HMC_STATUS 0x09  //״̬�Ĵ���
#define HMC_RECOGA 0x0A  //ʶ��Ĵ���A
#define HMC_RECOGB 0x0B  //ʶ��Ĵ���B
#define HMC_RECOGC 0x0C  //ʶ��Ĵ���C
#define MPU_ADD 0xD0  //MPU��i2c��ַ
#define HMC_WRITE 0x3C  //HMC��i2cд��ַ
#define HMC_READ  0x3D	//HMC��i2c����ַ

void HMC_Init(void);
void GY86_Init(void);
uint8_t HMC_Write_Byte(uint8_t reg,uint8_t data);
uint8_t HMC_Read_Byte(uint8_t reg);
uint8_t HMC_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t HMC_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf);

uint8_t READ_HMCALL(uint16_t* x,uint16_t* y, uint16_t* z);
