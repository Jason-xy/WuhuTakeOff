#include "datafusion.h"

//��������
extern GY_86 *GY86;
Angle_t angle; //��̬����-�Ƕ�ֵ

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
// ���ټ��㿪���ŵĵ���
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

//�����˲������ں�
float Kp = 4.0f; // ��������
float Ki = 0.005f; // ���ֳ���
float halfT = 0.005f; //�������ڵ�һ�룬ʵ��halfT�ɶ�ʱ�����
float T = 0.01f; // ��������Ϊ10ms
float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // ��Ԫ��
float exInt = 0, eyInt = 0, ezInt = 0; // �������ۼ�ֵ

// ��Ԫ����ʼ��
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

    //�����ʼŷ���ǣ���ʼ״̬ˮƽ������roll��pitchΪ0
    initRoll = 0.0f;
    initPitch = 0.0f;
    initYaw = atan2(initMx * cos(initRoll) + initMy * sin(initRoll) * sin(initPitch) + initMz * sin(initRoll) * cos(initPitch),
        initMy * cos(initPitch) - initMz * sin(initPitch));

    // ��Ԫ������
    q0 = cos(0.5f * initRoll) * cos(0.5f * initPitch) * cos(0.5f * initYaw) + sin(0.5f * initRoll) * sin(0.5f * initPitch) * sin(0.5f * initYaw); //w
    q1 = cos(0.5f * initRoll) * sin(0.5f * initPitch) * cos(0.5f * initYaw) - sin(0.5f * initRoll) * cos(0.5f * initPitch) * sin(0.5f * initYaw); //x Pitch
    q2 = sin(0.5f * initRoll) * cos(0.5f * initPitch) * cos(0.5f * initYaw) + cos(0.5f * initRoll) * sin(0.5f * initPitch) * sin(0.5f * initYaw); //y Roll
    q3 = cos(0.5f * initRoll) * cos(0.5f * initPitch) * sin(0.5f * initYaw) - sin(0.5f * initRoll) * sin(0.5f * initPitch) * cos(0.5f * initYaw); //z Yaw
}

// ==================================================================================
// ����ԭ�ͣ�void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
// ��        �ܣ������˲�������̬����
// ��        �룺���������ݼ����ٶȼ����ݼ�����������
// ==================================================================================
void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
    float norm;
    float hx, hy, hz, bz, by;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez;
    float q0_last, q1_last, q2_last, q3_last;

    //�ռ任ʱ�䣬���Ч��
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

    //���ٶȼƹ�һ���������Ϊʲô֮ǰֻҪ���������ݽ��е�λת��
    norm = invSqrt(ax * ax + ay * ay + az * az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    //������һʱ�̻�������ϵ�¼��ٶ�����
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    //���������ݹ�һ��
    norm = invSqrt(mx * mx + my * my + mz * mz);
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;
    //�����������ϵ�´������꣨�����ϱ�����
    hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
    hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
    hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);
    //���ش��ϱ�������Ϊ�ش��ϱ���������ϱ�����ƫ��
    //bx=0;
    by = sqrtf((hx * hx) + (hy * hy));
    bz = hz;
    //����ת���ػ�������ϵ����
    wx = 2 * by * (q1q2 + q0q3) + 2 * bz * (q1q3 - q0q2);
    wy = 2 * by * (0.5f - q1q1 - q3q3) + 2 * bz * (q0q1 + q2q3);
    wz = 2 * by * (q2q3 - q0q1) + 2 * bz * (0.5f - q1q1 - q2q2);

    //�����㣨���ٶȺʹų�ǿ��һ��
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

    //�ɶ�ʱ����ȡ�������ڵ�һ��
    halfT = 0.005;

    //pi����
    if (ex != 0.0f && ey != 0.0f && ez != 0.0f) {
        // ������
        exInt += ex * Ki * halfT;
        eyInt += ey * Ki * halfT;
        ezInt += ez * Ki * halfT;

        // ���ٶȲ���
        gx = gx + Kp * ex + exInt;
        gy = gy + Kp * ey + eyInt;
        gz = gz + Kp * ez + ezInt;
    }

    // ������Ԫ��
    q0_last = q0;
    q1_last = q1;
    q2_last = q2;
    q3_last = q3;
    // ������������
    q0 = q0_last + (-q1_last * gx - q2_last * gy - q3_last * gz) * halfT;
    q1 = q1_last + (q0_last * gx + q2_last * gz - q3_last * gy) * halfT;
    q2 = q2_last + (q0_last * gy - q1_last * gz + q3_last * gx) * halfT;
    q3 = q3_last + (q0_last * gz + q1_last * gy - q2_last * gx) * halfT;

    // ��һ����Ԫ��
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 * norm; //w
    q1 = q1 * norm; //x
    q2 = q2 * norm; //y
    q3 = q3 * norm; //z

    //��Ԫ��תŷ����
    angle.roll = asin(2.0f * (q0 * q2 - q1 * q3)) * RAD_TO_ANGLE;
    angle.pitch = -atan2(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * RAD_TO_ANGLE;
    angle.yaw = atan2(2.0f * (q0 * q3 + q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * RAD_TO_ANGLE;
}
