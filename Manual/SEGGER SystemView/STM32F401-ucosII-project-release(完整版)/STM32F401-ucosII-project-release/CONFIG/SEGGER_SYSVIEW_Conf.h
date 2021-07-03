/*********************************************************************
* (c) SEGGER Microcontroller GmbH & Co. KG *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------
File : SEGGER_SYSVIEW_Conf.h
Purpose : SEGGER SysView configuration for Cortex-M3 / Cortex-M4.
*/
#ifndef SEGGER_SYSVIEW_CONF_H
#define SEGGER_SYSVIEW_CONF_H
/*********************************************************************
*
* SysView timestamp configuration
*/
//  ?????
#define SEGGER_SYSVIEW_RTT_BUFFER_SIZE       4096u
#define SEGGER_SYSVIEW_GET_TIMESTAMP() ((*(U32 *)(0xE0001004)) >> 4)
#define SEGGER_SYSVIEW_TIMESTAMP_BITS 28
/*********************************************************************
*
* SysView Id configuration
*/
//  ??ID
// Default value for the lowest Id reported by the application.
// Can be overridden by the application via SEGGER_SYSVIEW_SetRAMBase().
#define SEGGER_SYSVIEW_ID_BASE 0x20000000
// Number of bits to shift the Id to save bandwidth.
// (i.e. 2 when all reported Ids (pointers) are 4 byte aligned)
#define SEGGER_SYSVIEW_ID_SHIFT 0
/*********************************************************************
*
* SysView interrupt configuration
*/
// Get the currently active interrupt Id. (read Cortex-M ICSR[8:0]= active vector)
#define SEGGER_SYSVIEW_GET_INTERRUPT_ID() ((*(U32 *)(0xE000ED04)) & 0x1FF)
/*********************************************************************
*
* SysView locking
*/
//   SystemView?????
// Lock SysView (nestable)
#define SEGGER_SYSVIEW_LOCK() SEGGER_RTT_LOCK()
// Unlock SysView (nestable)
#define SEGGER_SYSVIEW_UNLOCK() SEGGER_RTT_UNLOCK()
#endif
/*************************** End of file ****************************/