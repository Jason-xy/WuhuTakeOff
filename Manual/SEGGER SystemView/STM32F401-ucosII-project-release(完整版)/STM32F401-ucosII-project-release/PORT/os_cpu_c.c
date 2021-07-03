#include "includes.h"

OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt)
{
	OS_STK *p_stk;	
	(void)opt;		   /* opt为保留选项，通常不使用。进行类型转化避免报错*/
	p_stk      = ptos + 1u;        /*传值调用，避免改变原指针*/
	
//	p_stk      = (OS_STK *)((OS_STK)(p_stk) & 0xFFFFFFF8u);
                                                       /* Registers stacked as if auto-saved on exception      */
                                                       /*设定通用寄存器的初始值为寄存器号，便于调试时观察出栈时对应值是否匹配*/
    *(--p_stk) = (OS_STK)0x01000000uL;                          /* xPSR                                                 */
    *(--p_stk) = (OS_STK)task;                                  /* Entry Point                                          */
    *(--p_stk) = (OS_STK)OS_TaskReturn;                         /* R14 (LR)                                             */
    *(--p_stk) = (OS_STK)0x12121212uL;                          /* R12                                                  */
    *(--p_stk) = (OS_STK)0x03030303uL;                          /* R3                                                   */
    *(--p_stk) = (OS_STK)0x02020202uL;                          /* R2                                                   */
    *(--p_stk) = (OS_STK)0x01010101uL;                          /* R1                                                   */
    *(--p_stk) = (OS_STK)p_arg;                                 /* R0 : argument                                        */

                                                                /* Remaining registers saved on process stack           */
    *(--p_stk) = (OS_STK)0x11111111uL;                          /* R11                                                  */
    *(--p_stk) = (OS_STK)0x10101010uL;                          /* R10                                                  */
    *(--p_stk) = (OS_STK)0x09090909uL;                          /* R9                                                   */
    *(--p_stk) = (OS_STK)0x08080808uL;                          /* R8                                                   */
    *(--p_stk) = (OS_STK)0x07070707uL;                          /* R7                                                   */
    *(--p_stk) = (OS_STK)0x06060606uL;                          /* R6                                                   */
    *(--p_stk) = (OS_STK)0x05050505uL;                          /* R5                                                   */
    *(--p_stk) = (OS_STK)0x04040404uL;                          /* R4                                                   */

	return (p_stk);
}

void  OSInitHookBegin (void)
{

}

void  OSInitHookEnd (void)
{

}

void  OSTaskCreateHook (OS_TCB *ptcb)
{

}

void  OSTaskDelHook (OS_TCB *ptcb)
{

}

void  OSTaskIdleHook (void)
{

}

void  OSTaskStatHook (void)
{

}

void  OSTaskSwHook (void)
{

}

void  OSTCBInitHook (OS_TCB *ptcb)
{

}

void  OSTimeTickHook (void)
{

}

void OSTaskReturnHook(OS_TCB *ptcb)
{

}
