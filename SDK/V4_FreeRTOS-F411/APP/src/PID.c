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

#include "PID.h"

PID_TYPE Roll_w_PID = {0}, Pitch_w_PID = {0}, Yaw_w_PID = {0};
PID_TYPE Roll_PID = {0}, Pitch_PID = {0}, Yaw_PID = {0};

extern volatile uint16_t Base_CCR; // 遥控器提供的基准油门
extern uint16_t Fly_Thre_CCR; // 无人机起飞的最低油门
extern uint16_t Margin_CCR; // 做姿态需要的油门余量, 防止掉下来

void Gesture_PID_Init(void){
    Gain_Type K_pid[3];

    PID_Init(&Roll_w_PID);
    // 注册PID类型函数
    Roll_w_PID.Calculate_Output_Handler = Calculate_Position_PID_Output;
    // 注册更新反馈值函数
    Roll_w_PID.Update_Feedback_Handler = Update_Roll_w_Feedback;
    // 注册更新目标函数
    Roll_w_PID.Update_Target_Handler = Update_Roll_w_Target;
    // 注册增益设置函数
    Roll_w_PID.Set_PID_Arg_Handler = Set_PID_Arg;
    // 设置误差限幅
    Roll_w_PID.Err_Max = ;
    // 设置输出限幅 
    Roll_w_PID.Output_Max = ;
    // 设置积分限幅 
    Roll_w_PID.Accu_Err_Max = ;
    // 设置增益Kp, Ki, Kd
    K_pid[0] = ; K_pid[1] = ; K_pid[2] = ;
    Roll_w_PID.Set_PID_Arg_Handler(&Roll_w_PID, K_pid);

    PID_Init(&Pitch_w_PID);
    // 注册PID类型函数
    Pitch_w_PID.Calculate_Output_Handler = Calculate_Position_PID_Output;
    // 注册更新反馈值函数
    Pitch_w_PID.Update_Feedback_Handler = Update_Pitch_w_Feedback;
    // 注册更新目标函数
    Pitch_w_PID.Update_Target_Handler = Update_Pitch_w_Target;
    // 注册增益设置函数
    Pitch_w_PID.Set_PID_Arg_Handler = Set_PID_Arg;
    // 设置误差限幅
    Pitch_w_PID.Err_Max = ;
    // 设置输出限幅 
    Pitch_w_PID.Output_Max = ;
    // 设置积分限幅
    Pitch_w_PID.Accu_Err_Max = ;
    // 设置增益Kp, Ki, Kd
    K_pid[0] = ; K_pid[1] = ; K_pid[2] = ;
    Pitch_w_PID.Set_PID_Arg_Handler(&Pitch_w_PID, K_pid);

    PID_Init(&Yaw_w_PID);
    // 注册PID类型函数
    Yaw_w_PID.Calculate_Output_Handler = Calculate_Position_PID_Output;
    // 注册更新反馈值函数
    Yaw_w_PID.Update_Feedback_Handler = Update_Yaw_w_Feedback;
    // 注册更新目标函数
    Yaw_w_PID.Update_Target_Handler = Update_Yaw_w_Target;
    // 注册增益设置函数
    Yaw_w_PID.Set_PID_Arg_Handler = Set_PID_Arg;
    // 设置误差限幅
    Yaw_w_PID.Err_Max = ;
    // 设置输出限幅 
    Yaw_w_PID.Output_Max = ;
    // 设置积分限幅
    Yaw_w_PID.Accu_Err_Max = ;
    // 设置增益Kp, Ki, Kd
    K_pid[0] = ; K_pid[1] = ; K_pid[2] = ;
    Yaw_w_PID.Set_PID_Arg_Handler(&Yaw_w_PID, K_pid);

    PID_Init(&Roll_PID);
    // 注册PID类型函数
    Roll_PID.Calculate_Output_Handler = Calculate_Position_PID_Output;
    // 注册更新反馈值函数
    Roll_PID.Update_Feedback_Handler = Update_Roll_Feedback;
    // 注册更新目标函数
    Roll_PID.Update_Target_Handler = Update_Roll_Target;
    // 注册增益设置函数
    Roll_PID.Set_PID_Arg_Handler = Set_PID_Arg;
    // 设置误差限幅
    Roll_PID.Err_Max = ;
    // 设置输出限幅  
    Roll_PID.Output_Max = ;
    // 设置积分限幅
    Roll_PID.Accu_Err_Max = ;
    // 设置增益Kp, Ki, Kd
    K_pid[0] = ; K_pid[1] = ; K_pid[2] = ;
    Roll_PID.Set_PID_Arg_Handler(&Roll_PID, K_pid);

    PID_Init(&Pitch_PID);
    // 注册PID类型函数
    Pitch_PID.Calculate_Output_Handler = Calculate_Position_PID_Output;
    // 注册更新反馈值函数
    Pitch_PID.Update_Feedback_Handler = Update_Pitch_Feedback;
    // 注册更新目标函数
    Pitch_PID.Update_Target_Handler = Update_Pitch_Target;
    // 注册增益设置函数
    Pitch_PID.Set_PID_Arg_Handler = Set_PID_Arg;
    // 设置误差限幅
    Pitch_PID.Err_Max = ;
    // 设置输出限幅
    Pitch_PID.Output_Max = ;
    // 设置积分限幅
    Pitch_PID.Accu_Err_Max = ;
    // 设置增益Kp, Ki, Kd
    K_pid[0] = ; K_pid[1] = ; K_pid[2] = ;
    Pitch_PID.Set_PID_Arg_Handler(&Pitch_PID, K_pid);

    PID_Init(&Yaw_PID);
    // 注册PID类型函数
    Yaw_PID.Calculate_Output_Handler = Calculate_Position_PID_Output;
    // 注册更新反馈值函数
    Yaw_PID.Update_Feedback_Handler = Update_Yaw_Feedback;
    // 注册更新目标函数
    Yaw_PID.Update_Target_Handler = Update_Yaw_Target;
    // 注册增益设置函数
    Yaw_PID.Set_PID_Arg_Handler = Set_PID_Arg;
    // 设置误差限幅
    Yaw_PID.Err_Max = ;
    // 设置输出限幅
    Yaw_PID.Output_Max = ;
    // 设置积分限幅
    Yaw_PID.Accu_Err_Max = ;
    // 设置增益Kp, Ki, Kd
    K_pid[0] = ; K_pid[1] = ; K_pid[2] = ;
    Yaw_PID.Set_PID_Arg_Handler(&Yaw_PID, K_pid);

}

// 内环PID目标值来自外环输出, 单环调试时来自上位机
void Update_Roll_w_Target(p_PID_TYPE PID){
    PID->Target = Roll_PID.Output;
}

void Update_Pitch_w_Target(p_PID_TYPE PID){
    PID->Target = Pitch_PID.Output;
}

void Update_Yaw_w_Target(p_PID_TYPE PID){
    PID->Target = Yaw_PID.Output;
}

// Roll_T, Pitch_T, Yaw_T是外环目标值, 来自遥控器
void Update_Roll_Target(p_PID_TYPE PID){
    PID->Target = Roll_T;
}

void Update_Pitch_Target(p_PID_TYPE PID){
    PID->Target = Pitch_T;
}

void Update_Yaw_Target(p_PID_TYPE PID){
    PID->Target = Yaw_T;
}

// 角速度计的值, 来自GY86的数据
void Update_Roll_w_Feedback(p_PID_TYPE PID){
    PID->Feedback = Gyro.wx;
}

void Update_Pitch_w_Feedback(p_PID_TYPE PID){
    PID->Feedback = Gyro.wy;
}

void Update_Yaw_w_Feedback(p_PID_TYPE PID){
    PID->Feedback = Gyro.wz;
}

// Roll, Pitch, Yaw角来自姿态解算四元数转欧拉角后的欧拉角
void Update_Roll_Feedback(p_PID_TYPE PID){
    PID->Feedback = Roll;
}

void Update_Pitch_Feedback(p_PID_TYPE PID){
    PID->Feedback = Pitch;
}

void Update_Yaw_Feedback(p_PID_TYPE PID){
    PID->Feedback = Yaw;
}

uint16_t Servo_PWM[4] = {0};
// 内环任务, 运行频率可以和姿态更新频率保持一致。
void Quadcopter_Imple_Task(void){
    PID_Cycle(&Roll_w_PID);
    PID_Cycle(&Pitch_w_PID);
    PID_Cycle(&Yaw_w_PID);

    /* 给电机分配占空比 */
    // 电机输出足够起飞且有一定余量做动作时, 设置PWM
    // TODO: 电机输出分配要根据飞行模式(十字或X), 正反桨位置来调整, 本代码适用飞行模式是X飞行模式, 仅供参考
    if(Base_CCR > (Fly_Thre_CCR + Margin_CCR)){
        Servo_PWM[0] = Base_CCR + Roll_w_PID.Output + Pitch_w_PID.Output - Yaw_w_PID.Output;
        Servo_PWM[1] = Base_CCR - Roll_w_PID.Output + Pitch_w_PID.Output + Yaw_w_PID.Output;
        Servo_PWM[2] = Base_CCR + Roll_w_PID.Output - Pitch_w_PID.Output + Yaw_w_PID.Output;
        Servo_PWM[3] = Base_CCR - Roll_w_PID.Output - Pitch_w_PID.Output - Yaw_w_PID.Output;
    }
    // 电机输出不够, 不允许起飞
    else{
        for(int i = 0; i < 4; i++)
            Servo_PWM[i] = 0;
    }

    // TODO: 将Servo_PWM中的值写入控制电机的TIM相应通道的CCR寄存器
    // 未实现, 仅作为逻辑示意
    Set_Servo(Servo_PWM);
}


// 外环任务, 运行频率要小于内环频率, 运行频率小于内环的一半计算才有意义, 本人设定 外环频率 : 内环频率 = 1 : 5
void Quadcopter_Control_Task(void){
    PID_Cycle(&Roll_PID);
    PID_Cycle(&Pitch_PID);
    PID_Cycle(&Yaw_PID);
}

void PID_Init(p_PID_TYPE PID){
	PID->Set_PID_Arg_Handler = Set_PID_Arg;
	PID->Update_Err_Handler = Update_Err;
}

void Set_PID_Arg(p_PID_TYPE PID, Gain_Type* K_pid){
	PID->Kp = K_pid[0];
	PID->Ki = K_pid[1];
	PID->Kd = K_pid[2];
}

// 计算位置式PID
void Calculate_Position_PID_Output(p_PID_TYPE PID){
	PID->Output = PID->Kp * PID->Err + PID->Ki * PID->ErrAccu + PID->Kd * PID->ErrDiff;
	//	printf("PID->Output %d\n", PID->Output);

	if(PID->Output > PID->Output_Max)
		PID->Output = PID->Output_Max;
	else if(PID->Output < (-1) * PID->Output_Max)
		PID->Output = (-1) * PID->Output_Max;
}

// 计算增量式PID
void Calculate_Delta_PID_Output(p_PID_TYPE PID){
	PID->Delta = PID->Kp * PID->ErrDiff + PID->Ki * PID->Err + PID->Kd * (PID->Err - 2*PID->LastErr + PID->PrevErr);
//	printf("PID->Delta %d\n", PID->Delta);

	PID->Output = PID->Output + PID->Delta;
//	printf("PID->Output %d\n", PID->Output);
}

void PID_Cycle(p_PID_TYPE PID){
	// 更新反馈值
	PID->Update_Feedback_Handler(PID);
//	printf("PID->Feedback %f\n", PID->Feedback);
	
	// 更新目标值
	PID->Update_Target_Handler(PID);
//	printf("PID->Target %f\n", PID->Target);
	
	// 更新误差值
	PID->Update_Err_Handler(PID);
//	printf("PID->Err %f\n", PID->Err);
	
	// 计算输出值
	PID->Calculate_Output_Handler(PID);
//	printf("PID->Output %d\n", PID->Output);
}
	
void Update_Err(p_PID_TYPE PID){
	
	PID->PrevErr = PID->LastErr;
//	printf("PID->PrevErr %f\n", PID->PrevErr);
	
	PID->LastErr = PID->Err;
//	printf("PID->LastErr %f\n", PID->LastErr);

	PID->Err = PID->Target - PID->Feedback;
//	printf("PID->Err %f\n", PID->Err);
	
	PID->ErrDiff = PID->Err - PID->LastErr;
//	printf("PID->ErrDiff %f\n", PID->ErrDiff);
	
	if(PID->Err > PID->Err_Max)
		PID->ErrAccu += PID->Err_Max;
	else if(PID->Err < -1 * PID->Err_Max)
		PID->ErrAccu += -1 * PID->Err_Max;
	else
		PID->ErrAccu += PID->Err;

	// 积分限幅
	if(PID->Err > PID->Accu_Err_Max)
		PID->ErrAccu = PID->Accu_Err_Max;
	else if(PID->Err < -1 * PID->Accu_Err_Max)
		PID->ErrAccu = -1 * PID->Accu_Err_Max;

//	printf("PID->ErrAccu %f\n", PID->ErrAccu);
}
