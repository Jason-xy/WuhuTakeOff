#ifndef  OS_CPU_H
#define  OS_CPU_H
#include "lib_def.h"

#ifdef   OS_CPU_GLOBALS
#define  OS_CPU_EXT
#else
#define  OS_CPU_EXT  extern
#endif

#ifndef  OS_CPU_EXCEPT_STK_SIZE
#define  OS_CPU_EXCEPT_STK_SIZE    128u          /* Default exception stack size is 128 OS_STK entries */
#endif

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#ifndef  __TARGET_FPU_SOFTVFP
#define  OS_CPU_ARM_FP_EN                              DEF_ENABLED
#else
#define  OS_CPU_ARM_FP_EN                              DEF_DISABLED
#endif

#define  OS_CPU_ARM_FP_REG_NBR                           32u

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;			/* Unsigned  8 bit quantity       */
typedef signed   char  INT8S;			/* Signed    8 bit quantity       */
typedef unsigned short INT16U;			/* Unsigned 16 bit quantity       */
typedef signed   short INT16S;			/* Signed   16 bit quantity       */
typedef unsigned int   INT32U;			/* Unsigned 32 bit quantity       */
typedef signed   int   INT32S;			/* Signed   32 bit quantity       */
typedef float          FP32;			/* Single precision floating point*/
typedef double         FP64;			/* Double precision floating point*/

//STM32是32位位宽的,这里OS_STK和OS_CPU_SR都应该为32位数据类型
typedef unsigned int   OS_STK;			/* Each stack entry is 32-bit wide*/
typedef unsigned int   OS_CPU_SR;		/* Define size of CPU status register*/

//OS_CRITICAL_METHOD = 1 :直接使用处理器的开关中断指令来实现宏 
//OS_CRITICAL_METHOD = 2 :利用堆栈保存和恢复CPU的状态 
//OS_CRITICAL_METHOD = 3 :利用编译器扩展功能获得程序状态字，保存在局部变量cpu_sr

#define  OS_CRITICAL_METHOD   3	 	//进入临界段的方法

#if OS_CRITICAL_METHOD == 3

#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}

#endif

/*
*********************************************************************************************************
*                                        Cortex-M4 Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1u                  /* Stack grows from HIGH to LOW memory on ARM        */

#define  OS_TASK_SW()         OSCtxSw()

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

OS_CPU_EXT  OS_STK   OS_CPU_ExceptStk[OS_CPU_EXCEPT_STK_SIZE];
OS_CPU_EXT  OS_STK  *OS_CPU_ExceptStkBase;

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
#if OS_CRITICAL_METHOD == 3u                      /* See OS_CPU_A.ASM                                  */
OS_CPU_SR  OS_CPU_SR_Save    (void);
void       OS_CPU_SR_Restore (OS_CPU_SR cpu_sr);
#endif


void  OSCtxSw                (void);
void  OSIntCtxSw             (void);
void  OSStartHighRdy         (void);

void  OS_CPU_PendSVHandler   (void);


#if (OS_CPU_ARM_FP_EN > 0u)
void  OS_CPU_FP_Reg_Push     (OS_STK   *stkPtr);
void  OS_CPU_FP_Reg_Pop      (OS_STK   *stkPtr);
#endif

#endif
