/**************************************************************************************
���ߣ�
ESP826601S: TX--P30 RX--P31  VCC--3.3V  GND--GND
DS18B20: IO--P37  

ʹ��APP��TCP����
WIFIģ���IP��ַ���������ֻ������ȵ��鿴�ȵ����Ե�֪
 
����51��Ƭ���Ĳ�������9600����WiFiģ���Ĭ�ϲ�������115200������Ҫ�޸�WIFIģ��Ĳ�����
ʹ��USBתTTL��ģ�鷢��ATָ���޸ģ�AT+UART=9600,8,1,0,0

ע�⣬ESP8266 ��Ϊserver �г�ʱ���ƣ�������ӽ�����һ��ʱ������
����������server �Ὣclient �ߵ������� PC �������� ESP8266 ����һ��
2s ��ѭ�����ݷ��ͣ����ڱ������ӡ�																			  
***************************************************************************************/
#include "reg52.h"    //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "usart.h"
#include "delay.h"
#include "stdio.h"
#include "18b20.h"
sbit led=P2^0;	   //����Ƭ����P0.0�˿ڶ���Ϊled
unsigned char xdata dataBuf[16]= {0};//������ʾ������ʱ�洢����

void main()
{
	int temp1;
	float temperature;
	Init_DS18B20();
	
	UART_init();//���ڳ�ʼ��
	DelayMs(1000);
	
	UART_SendStr("AT\r\n",4); //���� AT ����
	DelayMs(1000);
	
	DelayMs(1000);
	UART_SendStr("AT+CWMODE=2\r\n",13);//ѡ�� WIFI Ӧ��ģʽ:AP ģʽ
	
	DelayMs(1000);
	UART_SendStr("AT+CWSAP=\"X_CHUN\",\"12345678\",6,4\r\n",34);	// ���� AP ģʽ�µĲ���:���������,���룬ͨ����

	DelayMs(1000);
	
	UART_SendStr("AT+CIPMUX=1\r\n",13);		 //����������
	
	DelayMs(1000);
	UART_SendStr("AT+CIPSERVER=1,8080\r\n",21);	  	//����Ϊ������,1:���� server ģʽ,8080�˿ں�
	
	while(1)
	{
		temp1=ReadTemperature();		//��ȡ�¶�
		temperature=(float)temp1*0.0625; //�¶�ֵת��
		
		UART_SendStr("AT+CIPSEND=0,16\r\n",17);		//��������
		sprintf(dataBuf,"Temp: %4.1fC",temperature);  //���¶����ݷ���dataBuf������
		UART_SendStr(dataBuf,16);		//����16λ����
		
		DelayMs(1000);
	}		
}
