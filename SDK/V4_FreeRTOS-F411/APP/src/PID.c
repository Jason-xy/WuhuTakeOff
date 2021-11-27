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

PID_t rollCore, rollShell, pitchCore, pitchShell, yawCore, thrShell; //六个环的pid结构体
uint32_t pidT = 0; //采样周期
float expRoll, expPitch, expYaw, expMode, expHeight; //期望值
float motor1, motor2, motor3, motor4; //四个电机速度
extern float Duty[6]; //遥控器推杆百分比
FlyMode_t flyMode; //飞行模式
//extern Float_t fGyro; //角速度数据（rad）
extern GY_86 *GY86;
extern Angle_t angle; //姿态解算-角度值
float height, velocity; //高度（cm）,速度short(cm/s)
float pidRoll, pidPitch, pidYaw, pidThr; //pid输出

float rollShellKp = 0.0f; //外环Kp 1.5
float rollCoreKp = 0.5f; //内环Kp
float rollCoreTi = 5000.0f; //内环Ti
float rollCoreTd = 1.0f; //内环Td

float pitchShellKp = 0.0f;
float pitchCoreKp = 0.5f;
float pitchCoreTi = 5000.0f;
float pitchCoreTd = 1.0f;

float yawCoreKp = 0.0f;//0.25f
float yawCoreTd = 2.0f;

float thrShellKp = 0.05f;
float thrShellTd = 0.0f;

/******************************************************************************
函数原型：	void PID_Init(void)
功    能：	PID初始化
*******************************************************************************/
void PID_Init(void)
{
    //roll
    rollShell.Kp = rollShellKp;
    rollCore.Kp = rollCoreKp;
    rollCore.Ti = rollCoreTi;
    rollCore.Td = rollCoreTd;

    //pitch
    pitchShell.Kp = pitchShellKp;
    pitchCore.Kp = pitchCoreKp;
    pitchCore.Ti = pitchCoreTi;
    pitchCore.Td = pitchCoreTd;

    //yaw
    yawCore.Kp = yawCoreKp;
    yawCore.Td = yawCoreTd;

    //thr
    thrShell.Kp = thrShellKp;
    thrShell.Td = thrShellTd;

    //flyMode
    flyMode = STOP;

    //expHeight
    expHeight = 0;
}

/******************************************************************************
函数原型：	float PID_Calc(float shellErr, float coreStatus, PID_t *shell, PID_t *core)
功    能：	PID计算
输    入：  shellErr，角度偏差
            coreStatus，对应轴角速度
            shell,外环
            core，内环
返    回：  内环输出
*******************************************************************************/
float PID_Calc(float shellErr, float coreStatus, PID_t* shell, PID_t* core)
{
    float shellKd, coreKi, coreKd;

    //ROLL,PITCH--串级PID
    if (shell && core) {
//        coreKi = pidT / core->Ti;
//        coreKd = core->Td / pidT;
				coreKi = 0;
        coreKd = 0;

        shell->eK = shellErr;
        shell->output = shell->Kp * shell->eK;
        shell->eK_1 = shell->eK;

        core->eK = shell->output - coreStatus; //外环输出，作为内环输入 用陀螺仪当前的角速度作为实际值

        //TODO:内环积分浮点数比大小出问题
        //内环积分限幅
        if (core->eSum > CORE_INT_MAX) {
            if (core->eK < -0.0f)
                core->eSum += core->eK;
        } else if (core->eSum < -CORE_INT_MAX) {
            if (core->eK > 0.0f)
                core->eSum += core->eK;
        } else {
            core->eSum += core->eK;
        }

        core->output = core->Kp * core->eK + core->eSum * coreKi + (core->eK - core->eK_1) * coreKd; //内环输出
        core->output = Limit(core->output, -PID_OUT_MAX, PID_OUT_MAX);
        core->eK_1 = core->eK;

        return core->output;
    }

    //HEIGHT--外环PID
    if (shell && !core) {
        shellKd = shell->Td / pidT;
        shell->eK = shellErr;
        shell->output = shell->Kp * shell->eK + (shell->eK - shell->eK_1) * shellKd;
        shell->output = Limit(shell->output, -PID_OUT_MAX, PID_OUT_MAX);
        shell->eK_1 = shell->eK;
        return shell->output;
    }

    //YAW--内环PID
    if (!shell && core) {
        coreKd = core->Td / pidT;
        core->eK = expYaw - coreStatus;
        core->output = core->Kp * core->eK + (core->eK - core->eK_1) * coreKd;
        core->output = Limit(core->output, -PID_OUT_MAX, PID_OUT_MAX);
        core->eK_1 = core->eK;
        return core->output;
    }

    return 0;
}

//飞行器模式状态机转换
void Judge_FlyMode(float expMode)
{
    switch (flyMode) {
    case STOP:
        if (expMode > 30) {
            flyMode = HOVER;
            expHeight = 100;
        }
        break;
    case HOVER:
        if (expMode > 50) {
            flyMode = UP;
        }
        if (expMode < 20) {
            flyMode = DOWN;
        }
        break;
    case DOWN:
        if (expMode > 30) {
            flyMode = HOVER;
            expHeight = height;
        }
        if (expMode < 10) {
            flyMode = STOP;
        }
        break;
    case UP:
        if (expMode < 40) {
            flyMode = HOVER;
            expHeight = height;
        }
        break;
    default:
        flyMode = STOP;
    }
}

/******************************************************************************
函数原型：	void Motor_Calc(void)
功    能：	计算输出速度（PWM）给四个电机
*******************************************************************************/
void Motor_Calc(void)
{
    //float pidRoll = 0, pidPitch = 0, pidYaw = 0, pidThr = 0; //pid输出
	
    //计算采样周期,已在Task中完成计算。
    //pidT = 0.020;
	
		//角速度低通滤波
		static float alpha = 0.4;
		static int Gyro_now_x, Gyro_now_y, Gyro_now_z, Gyro_past_x, Gyro_past_y, Gyro_past_z;
		Gyro_now_x = alpha * GY86->Gyro->data->Gyro_ds.x + (1 - alpha) * Gyro_past_x; 
		Gyro_now_y = alpha * GY86->Gyro->data->Gyro_ds.y + (1 - alpha) * Gyro_past_y; 
		Gyro_now_z = alpha * GY86->Gyro->data->Gyro_ds.z + (1 - alpha) * Gyro_past_z;	
		Gyro_past_x = Gyro_now_x;
		Gyro_past_y = Gyro_now_y;
		Gyro_past_z = Gyro_now_z;


    //计算姿态PID
    //注意正负
    pidRoll = PID_Calc(expRoll - angle.roll, Gyro_now_y, &rollShell, &rollCore);
    pidPitch = PID_Calc(expPitch - angle.pitch, Gyro_now_x, &pitchShell, &pitchCore);
    //yaw 与pitch、roll的pid计算不一样
    pidYaw = PID_Calc(0,  Gyro_past_z, 0, &yawCore);

    //PWM限幅
    motor1 = Limit(expMode + pidPitch - pidRoll - pidYaw, MOTOR_OUT_MIN, MOTOR_OUT_MAX);
    motor2 = Limit(expMode + pidPitch + pidRoll + pidYaw, MOTOR_OUT_MIN, MOTOR_OUT_MAX);
    motor3 = Limit(expMode - pidPitch + pidRoll + pidYaw, MOTOR_OUT_MIN, MOTOR_OUT_MAX);
    motor4 = Limit(expMode - pidPitch - pidRoll - pidYaw, MOTOR_OUT_MIN, MOTOR_OUT_MAX);

    //如果机体处于停止模式或倾斜角大于45度，则停止飞行
    if (expMode <= 10 || angle.pitch >= 45 || angle.pitch <= -45 || angle.roll >= 45 || angle.roll <= -45) {
        motor1 = MOTOR_OUT_MIN;
        motor2 = MOTOR_OUT_MIN;
        motor3 = MOTOR_OUT_MIN;
        motor4 = MOTOR_OUT_MIN;
    }
}

/******************************************************************************
函数原型：	void Motor_Exp_Calc(void)
功    能：	计算遥控器的期望值
*******************************************************************************/
void Motor_Exp_Calc(void)
{
    int16_t PWMInCh1, PWMInCh2, PWMInCh3, PWMInCh4;
    //限幅
    PWMInCh1 = Limit(Duty[0] * 100, 0, 100);
    PWMInCh2 = Limit(Duty[1] * 100, 0, 100);
    PWMInCh3 = Limit(Duty[2] * 100, 0, 100);
    PWMInCh4 = Limit(Duty[3] * 100, 0, 100);

    //调节角速度环
    //    expRoll = (PWMInCh4 - 1500)*0.1f;

    //转化为期望值
    expRoll = (float)((PWMInCh1 - 50) * 0.4f); //最大20度
    expPitch = -(float)((PWMInCh2 - 50) * 0.4f); //最大20度
    //TODO:yaw与roll、pitch不一样
    expYaw = (float)((PWMInCh4 - 50) * 0.1f); //最大10度每秒
    expMode = PWMInCh3; //模式
}

/******************************************************************************
函数原型：	float inline Limit(float pwm, float min, float max)
功    能：	PWM限幅
输    入：  pwm，输入pwm值
            min，最小值
            max，最大值
返    回：  限幅后的pwm
*******************************************************************************/
float Limit(float pwm, float min, float max)
{
    return pwm < min ? min : (pwm > max ? max : pwm);
}

