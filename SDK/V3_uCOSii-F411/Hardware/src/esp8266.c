/**
  ******************************************************************************
  * 文件名程: esp8266.c 
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

#include "esp8266.h"

//8266指令发送
void esp8266_cmd(char* cmd)
{
    printf("%s",cmd);
}

//8266初始化
void esp8266_init(void)
{
    esp8266_ap_cipsend_init();
    
}

//8266的TCP_Server透传模式配置
void esp8266_ap_cipsend_init(void)
{
//    esp8266_cmd("AT+RST\r\n");  //复位模块
//    HAL_Delay(500);
//    esp8266_cmd("AT+CWMODE=2\r\n"); //设置为AP模式
//    HAL_Delay(500);
//    esp8266_cmd("AT+CWSAP=\"WuhuTakeOff\",\"uestc404\",11,4\r\n");   //WiFi基本设置
//    HAL_Delay(500);
//    esp8266_cmd("AT+CIPAP=\"192.168.4.1\",\"192.168.4.1\",\"255.255.255.0\"\r\n");   //路由设置
//    HAL_Delay(500);
//    esp8266_cmd("AT+CIPMUX=1\r\n"); //多链路模式
//    HAL_Delay(500);
//    esp8266_cmd("AT+CIPSERVER=1,8080\r\n"); //TCP_Server启动
//    HAL_Delay(500);
	
		esp8266_cmd("AT+RST\r\n");  //复位模块
    HAL_Delay(500);
    esp8266_cmd("AT+CWMODE=1\r\n"); //设置为AP模式
    HAL_Delay(500);
    esp8266_cmd("AT+CWJAP=\"WuhuTakeOff\",\"uestc404\"\r\n");   //WiFi基本设置
    HAL_Delay(15000);
    esp8266_cmd("AT+CIPMUX=0\r\n");   //单链路设置
    HAL_Delay(500);
    esp8266_cmd("AT+CIPSTART=\"TCP\",\"192.168.4.1\",8080\r\n"); //TCP服务器
    HAL_Delay(500);
    esp8266_cmd("AT+CIPMODE=1\r\n"); //透传
    HAL_Delay(500);
		esp8266_cmd("AT+CIPSEND\r\n"); //透传开始
    HAL_Delay(500);
}

//8266的TCP_Server透传模式数据发送
void esp8266_cipsend(char* data)
{
    //printf("AT+CIPSEND=0,%d\r\n",strlen(data));
    //HAL_Delay(1);
    printf("%s\r\n",data);
}
