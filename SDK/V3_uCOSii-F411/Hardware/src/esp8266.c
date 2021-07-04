/**
  ******************************************************************************
  * �ļ�����: esp8266.c 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-11-28
  * ��    ��: WiFi(ESP8266)Ӧ�ò�����ʵ��
  ******************************************************************************
  * ˵����
  * 1.��Ҫ�������úô����շ������ض���printf�����ڡ�
  * 2.δ����Ӧ���⡣
  * 
  * ���ܣ�
  * 1.APģʽ�µ�TCP_Server͸����ʼ����
  ******************************************************************************
  */

#include "esp8266.h"

//8266ָ���
void esp8266_cmd(char* cmd)
{
    printf("%s",cmd);
}

//8266��ʼ��
void esp8266_init(void)
{
    esp8266_ap_cipsend_init();
    
}

//8266��TCP_Server͸��ģʽ����
void esp8266_ap_cipsend_init(void)
{
//    esp8266_cmd("AT+RST\r\n");  //��λģ��
//    HAL_Delay(500);
//    esp8266_cmd("AT+CWMODE=2\r\n"); //����ΪAPģʽ
//    HAL_Delay(500);
//    esp8266_cmd("AT+CWSAP=\"WuhuTakeOff\",\"uestc404\",11,4\r\n");   //WiFi��������
//    HAL_Delay(500);
//    esp8266_cmd("AT+CIPAP=\"192.168.4.1\",\"192.168.4.1\",\"255.255.255.0\"\r\n");   //·������
//    HAL_Delay(500);
//    esp8266_cmd("AT+CIPMUX=1\r\n"); //����·ģʽ
//    HAL_Delay(500);
//    esp8266_cmd("AT+CIPSERVER=1,8080\r\n"); //TCP_Server����
//    HAL_Delay(500);
	
		esp8266_cmd("AT+RST\r\n");  //��λģ��
    HAL_Delay(500);
    esp8266_cmd("AT+CWMODE=1\r\n"); //����ΪAPģʽ
    HAL_Delay(500);
    esp8266_cmd("AT+CWJAP=\"WuhuTakeOff\",\"uestc404\"\r\n");   //WiFi��������
    HAL_Delay(15000);
    esp8266_cmd("AT+CIPMUX=0\r\n");   //����·����
    HAL_Delay(500);
    esp8266_cmd("AT+CIPSTART=\"TCP\",\"192.168.4.1\",8080\r\n"); //TCP������
    HAL_Delay(500);
    esp8266_cmd("AT+CIPMODE=1\r\n"); //͸��
    HAL_Delay(500);
		esp8266_cmd("AT+CIPSEND\r\n"); //͸����ʼ
    HAL_Delay(500);
}

//8266��TCP_Server͸��ģʽ���ݷ���
void esp8266_cipsend(char* data)
{
    //printf("AT+CIPSEND=0,%d\r\n",strlen(data));
    //HAL_Delay(1);
    printf("%s\r\n",data);
}
