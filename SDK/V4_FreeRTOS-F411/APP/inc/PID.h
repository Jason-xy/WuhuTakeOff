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

#define PID_Err_MAX			180		
#define PID_AccuErr_MAX		1200    

typedef float Target_Type;
typedef float Feedback_Type;
typedef float Error_Type;
typedef float Gain_Type;
typedef float Output_Type;

typedef struct PID{
	Target_Type Target;
	Feedback_Type Feedback;
	Feedback_Type LastFeedback;
	Feedback_Type PrevFeedback;
	Error_Type Err;				
	Error_Type LastErr;        
	Error_Type PrevErr;       
	Error_Type ErrDiff;        
	Error_Type ErrAccu;         
	Error_Type Err_Max;			
	Error_Type Accu_Err_Max;   
	Gain_Type Kp;
	Gain_Type Ki;
	Gain_Type Kd;
	Output_Type Delta;			
	Output_Type Output;			
	Output_Type Output_Max;		
	void (*Set_PID_Arg_Handler)(struct PID*, Gain_Type*);
	void (*Update_Target_Handler)(struct PID*);
	void (*Update_Feedback_Handler)(struct PID*);
	void (*Update_Err_Handler)(struct PID*);
	void (*Calculate_Output_Handler)(struct PID*);
}PID_TYPE, *p_PID_TYPE;


void PID_Init(p_PID_TYPE PID);
void Set_PID_Arg(p_PID_TYPE PID, Gain_Type* K_pid);

void PID_Cycle(p_PID_TYPE PID);

void Calculate_Position_PID_Output(p_PID_TYPE PID);
void Calculate_Delta_PID_Output(p_PID_TYPE PID);

void Update_Target(p_PID_TYPE PID);
void Update_Feedback(p_PID_TYPE PID);
void Update_Err(p_PID_TYPE PID);

void PID_DEBUG_ANO_Send(Target_Type target, Feedback_Type* real);


void Gesture_PID_Init(void);

// 内环PID, 以外环PID的输出值作为目标
void Update_Roll_w_Target(p_PID_TYPE PID);
void Update_Pitch_w_Target(p_PID_TYPE PID);
void Update_Yaw_w_Target(p_PID_TYPE PID);

// 内环PID, 角速度计的值作为反馈值
void Update_Roll_w_Feedback(p_PID_TYPE PID);
void Update_Pitch_w_Feedback(p_PID_TYPE PID);
void Update_Yaw_w_Feedback(p_PID_TYPE PID);

// 外环PID, 以上位机或遥控器设定的值为目标
void Update_Roll_Target(p_PID_TYPE PID);
void Update_Pitch_Target(p_PID_TYPE PID);
void Update_Yaw_Target(p_PID_TYPE PID);

// 外环PID, 四元数转欧拉角后的欧拉角作为反馈值
void Update_Roll_Feedback(p_PID_TYPE PID);
void Update_Pitch_Feedback(p_PID_TYPE PID);
void Update_Yaw_Feedback(p_PID_TYPE PID);

// 内环执行任务
void Quadcopter_Imple_Task(void);
// 外环遥控任务
void Quadcopter_Control_Task(void);
