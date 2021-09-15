#ifndef __GAUSS_NEWTON_H__
#define __GAUSS_NEWTON_H__
#include <stdio.h>
#include <stdlib.h>
#include "gy-86.h"

//struct Vector3f_t{
//    float x;
//    float y;
//    float z;
//};
typedef struct Vector3f_t Vector3f_t;
typedef struct GY_86 GY_86;
extern GY_86 *GY86;   
extern Vector3f_t Accel_raw[6], Gyro_raw[6], Mag_raw[6];

static void ResetMatrices(float JtR[6], float JtJ[6][6]);
static void UpdateMatrices(float JtR[6], float JtJ[6][6], float beta[6], Vector3f_t inputData[6]);
static int GaussEliminateSolveDelta(float JtR[6], float JtJ[6][6], float delta[6]);
void GaussNewton(Vector3f_t inputData[6], Vector3f_t* offset, Vector3f_t* scale, float length);
void GaussNewtonOutput(void);
void inputDataUpdate(void);

#endif // __GAUSS_NEWTON_H__
