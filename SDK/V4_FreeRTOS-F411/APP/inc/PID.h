/**
  ******************************************************************************
  * 文件名程: PID.h
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0.0
  * 编写日期: 2021-11-10
  * 功    能: PID姿态控制
  ******************************************************************************
  * 说明：
  * 1.需要姿态解算最终数据。
  * 
  * 功能：
  * 1.PID姿态控制。
  * 
  * 更新：
  * 
  ******************************************************************************
  */
#ifndef __PID_H
#define __PID_H

#include "pid.h"
#include "gy-86.h"

#define CORE_INT_SEP_MAX 300.0f //内环积分分离幅值
#define CORE_INT_MAX 5.0f //内环积分幅值
#define PID_OUT_MAX 50.0f //PID输出幅值
#define MOTOR_OUT_MAX 100.0f //PWM输出幅值最大值
#define MOTOR_OUT_MIN 0.0f //PWM输出幅值最小值

typedef struct {
    float eK; //本次误差
    float eK_1; //上次误差
    float eSum; //误差和
    float Kp; //比例系数
    float Ti; //积分时间
    float Td; //微分时间
    float output; //pid输出
} PID_t;

typedef enum {
    STOP = 0, //停止
    DOWN, //下降
    HOVER, //悬停
    UP //上升
} FlyMode_t;

void PID_Init(void);
void Judge_FlyMode(float expMode);
float PID_Calc(float shellErr, float coreErr, PID_t* shell, PID_t* core);
void Motor_Calc(void);
void Motor_Exp_Calc(void);
float Limit(float pwm, float min, float max);

#endif
