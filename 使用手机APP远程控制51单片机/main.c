/**************************************************************************************
接线：
ESP826601S: TX--P30 RX--P31  VCC--3.3V  GND--GND
DS18B20: IO--P37  

使用APP：TCP连接
WIFI模块的IP地址：可以在手机连接热点后查看热点属性得知
 
由于51单片机的波特率是9600，而WiFi模块的默认波特率是115200，故需要修改WIFI模块的波特率
使用USB转TTL的模块发送AT指令修改：AT+UART=9600,8,1,0,0

注意，ESP8266 作为server 有超时机制，如果连接建立后，一段时间内无
数据来往，server 会将client 踢掉。请在 PC 工具连上 ESP8266 后建立一个
2s 的循环数据发送，用于保持连接。																			  
***************************************************************************************/
#include "reg52.h"    //此文件中定义了单片机的一些特殊功能寄存器
#include "usart.h"
#include "delay.h"
#include "stdio.h"
#include "18b20.h"
sbit led=P2^0;	   //将单片机的P0.0端口定义为led
unsigned char xdata dataBuf[16]= {0};//定义显示区域临时存储数组

void main()
{
	int temp1;
	float temperature;
	Init_DS18B20();
	
	UART_init();//串口初始化
	DelayMs(1000);
	
	UART_SendStr("AT\r\n",4); //测试 AT 启动
	DelayMs(1000);
	
	DelayMs(1000);
	UART_SendStr("AT+CWMODE=2\r\n",13);//选择 WIFI 应用模式:AP 模式
	
	DelayMs(1000);
	UART_SendStr("AT+CWSAP=\"X_CHUN\",\"12345678\",6,4\r\n",34);	// 设置 AP 模式下的参数:接入点名称,密码，通道号

	DelayMs(1000);
	
	UART_SendStr("AT+CIPMUX=1\r\n",13);		 //启动多连接
	
	DelayMs(1000);
	UART_SendStr("AT+CIPSERVER=1,8080\r\n",21);	  	//配置为服务器,1:开启 server 模式,8080端口号
	
	while(1)
	{
		temp1=ReadTemperature();		//读取温度
		temperature=(float)temp1*0.0625; //温度值转换
		
		UART_SendStr("AT+CIPSEND=0,16\r\n",17);		//发送数据
		sprintf(dataBuf,"Temp: %4.1fC",temperature);  //将温度数据放入dataBuf数组中
		UART_SendStr(dataBuf,16);		//发送16位数据
		
		DelayMs(1000);
	}		
}
