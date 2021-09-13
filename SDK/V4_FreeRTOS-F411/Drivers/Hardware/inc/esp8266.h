/**
  ******************************************************************************
  * 文件名程: esp8266.h
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-11-28
  * 功    能: WiFi(ESP8266)应用层驱动实现
  ******************************************************************************
  * 说明：
  * 1.需要事先配置好串口收发，并重定向printf至串口。
  * 2.未进行应答检测。
  * 
  * 功能：
  * 1.AP模式下的TCP_Server透传初始化。
  ******************************************************************************
  */


#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_usart.h"
#include <stdio.h>

void esp8266_cmd(char* cmd);    //发送AT指令
void esp8266_init(void);    //8266的初始化
void esp8266_ap_cipsend_init(void); //AP模式下的TCP_Server透传初始化
void esp8266_cipsend(char* data);   //TCP_Server透传数据发送

#endif /* __ESP8266_H__ */
