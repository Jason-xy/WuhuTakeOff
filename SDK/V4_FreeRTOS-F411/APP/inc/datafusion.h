/**
  ******************************************************************************
  * 文件名程: datafusion.h
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0.0
  * 编写日期: 2021-10-01
  * 功    能: 数据融合、姿态解算
  ******************************************************************************
  * 说明：
  * 1.需要姿态数据接口。
  * 
  * 功能：
  * 1.姿态解算，输出四元数和欧拉角。
  * 
  * 更新：
  * 
  ******************************************************************************
  */
#ifndef __DATAFUSION_H__
#define __DATAFUSION_H__

#include "math.h"
#include "GY-86.h"

#define PI 3.1415927f
//角度单位相关转换
#define RAD_TO_ANGLE 57.2957795f //弧度转角度
#define ANGLE_TO_RAD 0.01745329f //弧度转角度
#define RAW_TO_ANGLE 0.0610351f //原始数据转角度，对应±2000°/s
#define RAW_TO_RAD 0.0010653f //原始数据转弧度，对应±2000°/s

/* 姿态解算--角度值 */
typedef struct
{
    float yaw;
    float roll;
    float pitch;
} Angle_t;

extern Angle_t angle;   


float invSqrt(float x);
void Quat_Init(void);
void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);

#endif /*__DATAFUSION_H__ */
