/**
  ******************************************************************************
  * �ļ�����: i2c.h 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-9-18
  * ��    ��: I2Cģ������
  ******************************************************************************
  * ˵����
  * �ޡ�
  * 
	* ���ܣ�
	* 1.I2C��ʼ����
  ******************************************************************************
  */

#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */
