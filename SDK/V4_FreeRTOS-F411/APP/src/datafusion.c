#include "datafusion.h"

//变量声明
extern GY_86 *GY86;
Angle_t angle; //姿态解算-角度值

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
// 快速计算开根号的倒数
float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

//互补滤波数据融合
float Kp = 4.0f; // 比例常数
float Ki = 0.005f; // 积分常数
float halfT = 0.005f; //采样周期的一半，实际halfT由定时器求出
float T = 0.01f; // 采样周期为10ms
float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // 四元数
float exInt = 0, eyInt = 0, ezInt = 0; // 误差积分累计值

// 四元数初始化
void Quat_Init(void)
{
    int16_t initMx, initMy, initMz;
    float initYaw, initPitch, initRoll;

    uint8_t dataBuf[14];
    if (!HMC_Read_Len(HMC_DATA_XMSB, 6, dataBuf)) {
        initMx = (dataBuf[0] << 8) | dataBuf[1];
        initMy = (dataBuf[4] << 8) | dataBuf[5];
        initMz = (dataBuf[2] << 8) | dataBuf[3];

        // Complement processing and unit conversion
        if (initMx > 0x7fff)
            initMx -= 0xffff;
        if (initMy > 0x7fff)
            initMy -= 0xffff;
        if (initMz > 0x7fff)
            initMz -= 0xffff;
    }

    //求出初始欧拉角，初始状态水平，所以roll、pitch为0
    initRoll = 0.0f;
    initPitch = 0.0f;
    initYaw = atan2(initMx * cos(initRoll) + initMy * sin(initRoll) * sin(initPitch) + initMz * sin(initRoll) * cos(initPitch),
        initMy * cos(initPitch) - initMz * sin(initPitch));

    // 四元数计算
    q0 = cos(0.5f * initRoll) * cos(0.5f * initPitch) * cos(0.5f * initYaw) + sin(0.5f * initRoll) * sin(0.5f * initPitch) * sin(0.5f * initYaw); //w
    q1 = cos(0.5f * initRoll) * sin(0.5f * initPitch) * cos(0.5f * initYaw) - sin(0.5f * initRoll) * cos(0.5f * initPitch) * sin(0.5f * initYaw); //x Pitch
    q2 = sin(0.5f * initRoll) * cos(0.5f * initPitch) * cos(0.5f * initYaw) + cos(0.5f * initRoll) * sin(0.5f * initPitch) * sin(0.5f * initYaw); //y Roll
    q3 = cos(0.5f * initRoll) * cos(0.5f * initPitch) * sin(0.5f * initYaw) - sin(0.5f * initRoll) * sin(0.5f * initPitch) * cos(0.5f * initYaw); //z Yaw
}

// ==================================================================================
// 函数原型：void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
// 功        能：互补滤波进行姿态解算
// 输        入：陀螺仪数据及加速度计数据及磁力计数据
// ==================================================================================
void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
    float norm;
    float hx, hy, hz, bz, by;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez;
    float q0_last, q1_last, q2_last, q3_last;

    //空间换时间，提高效率
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    //加速度计归一化，这就是为什么之前只要陀螺仪数据进行单位转换
    norm = invSqrt(ax * ax + ay * ay + az * az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    //计算上一时刻机体坐标系下加速度坐标
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    //磁力计数据归一化
    norm = invSqrt(mx * mx + my * my + mz * mz);
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;
    //计算地理坐标系下磁力坐标（地理南北极）
    hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
    hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
    hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);
    //（地磁南北极）因为地磁南北极与地理南北极有偏差
    //bx=0;
    by = sqrtf((hx * hx) + (hy * hy));
    bz = hz;
    //磁力转换回机体坐标系坐标
    wx = 2 * by * (q1q2 + q0q3) + 2 * bz * (q1q3 - q0q2);
    wy = 2 * by * (0.5f - q1q1 - q3q3) + 2 * bz * (q0q1 + q2q3);
    wz = 2 * by * (q2q3 - q0q1) + 2 * bz * (0.5f - q1q1 - q2q2);

    //误差计算（加速度和磁场强度一起）
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

    //由定时器获取采样周期的一半
    halfT = 0.005;

    //pi运算
    if (ex != 0.0f && ey != 0.0f && ez != 0.0f) {
        // 误差积分
        exInt += ex * Ki * halfT;
        eyInt += ey * Ki * halfT;
        ezInt += ez * Ki * halfT;

        // 角速度补偿
        gx = gx + Kp * ex + exInt;
        gy = gy + Kp * ey + eyInt;
        gz = gz + Kp * ez + ezInt;
    }

    // 保存四元数
    q0_last = q0;
    q1_last = q1;
    q2_last = q2;
    q3_last = q3;
    // 积分增量运算
    q0 = q0_last + (-q1_last * gx - q2_last * gy - q3_last * gz) * halfT;
    q1 = q1_last + (q0_last * gx + q2_last * gz - q3_last * gy) * halfT;
    q2 = q2_last + (q0_last * gy - q1_last * gz + q3_last * gx) * halfT;
    q3 = q3_last + (q0_last * gz + q1_last * gy - q2_last * gx) * halfT;

    // 归一化四元数
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 * norm; //w
    q1 = q1 * norm; //x
    q2 = q2 * norm; //y
    q3 = q3 * norm; //z

    //四元数转欧拉角
    angle.roll = asin(2.0f * (q0 * q2 - q1 * q3)) * RAD_TO_ANGLE;
    angle.pitch = -atan2(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * RAD_TO_ANGLE;
    angle.yaw = atan2(2.0f * (q0 * q3 + q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * RAD_TO_ANGLE;
}
