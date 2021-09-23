#ifndef __DATAFUSION_H__
#define __DATAFUSION_H__

#include "math.h"
#include "GY-86.h"

#define PI 3.1415927f
//�Ƕȵ�λ���ת��
#define RAD_TO_ANGLE 57.2957795f //����ת�Ƕ�
#define RAW_TO_ANGLE 0.0610351f //ԭʼ����ת�Ƕȣ���Ӧ��2000��/s
#define RAW_TO_RAD 0.0010653f //ԭʼ����ת���ȣ���Ӧ��2000��/s

/* ��̬����--�Ƕ�ֵ */
typedef struct
{
    float yaw;
    float roll;
    float pitch;
} Angle_t;

float invSqrt(float x);
void Quat_Init(void);
void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);

#endif /*__DATAFUSION_H__ */
