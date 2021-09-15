//* ResetMatrices，用于初始化矩阵变量
#include "gauss-newton.h"
// typedef short int16_t;
// typedef unsigned int uint32_t;
// typedef unsigned char uint8_t;

static void ResetMatrices(double JtR[6], double JtJ[6][6])
{
    int16_t j,k;
    for(j=0; j<6; j++) 
    {
        JtR[j] = 0.0f;
        for(k=0; k<6; k++) 
        {
            JtJ[j][k] = 0.0f;
        }
    }
}

//* UpdateMatrices
static void UpdateMatrices(double JtR[6], double JtJ[6][6], double beta[6], Vector3f_t inputData[6])
{
    short i, j, k;
    double dx, b;
    double residual[6] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    double jacobian[6][6];
    double data[6][3];

    for(i=0; i<6; i++){
        data[i][0] = inputData[i].x;
        data[i][1] = inputData[i].y;
        data[i][2] = inputData[i].z;
    }
    
    for(i=0; i<6; i++){
        for(j=0; j<3; j++) 
        { 
            b = beta[3+j];
            dx = (double)data[i][j] - beta[j];
            //计算残差 (传感器误差方程的误差)
            residual[i] -= b*b*dx*dx;
            
            //计算雅可比矩阵
            jacobian[i][j] = 2.0f*b*b*dx;
            jacobian[i][3+j] = -2.0f*b*dx*dx;
        }
    }
    for(i=0; i<6; i++){   
        for(j=0; j<6; j++) 
        {
            //计算函数梯度
            JtR[i] += jacobian[j][i]*residual[j];
            
            for(k=0; k<6; k++) 
            {
                //计算Hessian矩阵（简化形式，省略二阶偏导），即雅可比矩阵与其转置的乘积
                JtJ[i][j] += jacobian[k][i]*jacobian[k][j];
            }
        }
    }

}
//* GaussEliminateSolveDelta，使用高斯消元法求解△ \bigtriangleup△
static void GaussEliminateSolveDelta(double JtR[6], double JtJ[6][6], double delta[6])
{
    int16_t i, j, k;
    double mu;
    
    
    //逐次消元，将线性方程组转换为上三角方程组
    for (j = 0; j < 6; j++)
    {
        for (i = j + 1; i < 6; i++)
        {
            mu = JtJ[i][j] / JtJ[j][j];
            if (mu != 0.0f)
            {
                for (k = j; k < 6; k++)
                {
                    JtJ[i][k] -= mu * JtJ[j][k];
                }
                JtR[i] -= mu * JtR[j];
            }
        }
    }

    //回代得到方程组的解
    for (j = 5; j >= 0; j--)
    {
        JtR[j] /= JtJ[j][j];
        JtJ[j][j] = 1.0f;

        for (i = 0; i < j; i++)
        {
            mu = JtJ[i][j];
            JtR[i] -= mu * JtR[j];
            JtJ[i][j] = 0.0f;
        }
    }

    for (i = 0; i < 6; i++)
    {
        delta[i] = JtR[i];
        //printf("delta : %f\n",delta[i]);
    }
}


/**********************************************************************************************************
*函 数 名: GaussNewton
*功能说明: 高斯牛顿法求解传感器误差方程，得到校准参数
*形    参: 传感器采样数据（6组） 零偏误差指针 比例误差指针 数据向量长度
*返 回 值: 无
**********************************************************************************************************/
void GaussNewton(Vector3f_t inputData[6], Vector3f_t* offset, Vector3f_t* scale, float length)
{
    uint32_t cnt    = 0;   //迭代次数
    double   eps    = 0.000001;
    double   change = 100.0;
    double    beta[6] = {39, 39, 9.8, 1, 1, 1.05};      //方程解
    double    delta[6] = {0};     //迭代步长
    double    JtR[6] = {0};       //梯度矩阵
    double    JtJ[6][6];    //Hessian矩阵
   
    //设定方程解初值
//    beta[0] = beta[1] = beta[2] = 0; 
//    beta[3] = beta[4] = beta[5] = 1 / length;

    //开始迭代，当迭代步长小于eps时结束计算，得到方程近似最优解
    while(change > eps) 
    {
        //矩阵初始化
        ResetMatrices(JtR, JtJ);

        //计算误差方程函数的梯度JtR和Hessian矩阵JtJ
        UpdateMatrices(JtR, JtJ, beta, inputData);

        //高斯消元法求解方程：JtJ * delta = JtR，得到delta
        GaussEliminateSolveDelta(JtR, JtJ, delta);

        //计算迭代步长
        change = delta[0]*delta[0] +
                 delta[1]*delta[1] +
                 delta[2]*delta[2] +
                 delta[3]*delta[3] +
                 delta[4]*delta[4] +
                 delta[5]*delta[5];
        //更新方程解
        for(uint8_t i=0; i<6; i++) 
        {
            beta[i] -= delta[i];
        }
            
        //限制迭代次数
        if(cnt++ > 10000)
            break;
    }

    //更新校准参数
    scale->x  = beta[3] * length;
    scale->y  = beta[4] * length;
    scale->z  = beta[5] * length;
    offset->x = beta[0];
    offset->y = beta[1];
    offset->z = beta[2];
}

int count = 0;
void inputDataUpdate(void){
    if(count < 6){
        Accel_raw[count] = GY86->Accel->data->Accel_raw;
        Gyro_raw[count] = GY86->Gyro->data->Gyro_raw; 
        Mag_raw[count] = GY86->Mag->data->Mag_raw;
				count++;
    }else 
        count = 0;
}

void GaussNewtonOutput(void){
    float length;
    length = 1.0f;
    GaussNewton(Accel_raw, &(GY86->Accel->data->Accel_offset), &(GY86->Accel->data->Accel_scale), length);
    //GaussNewton(Gyro_raw, &(GY86->Gyro->data->Gyro_offset), &(GY86->Gyro->data->Gyro_scale), length);
}
