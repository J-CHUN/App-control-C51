#include "usart.h"
//#define FOSC 22118400L      //System frequency
#define FOSC   11059200L      //System frequency
unsigned  int baud=9600;       //UART波特率
sbit led=P2^0;	   //将单片机的P0.0端口定义为led
/************************************************************************
函 数 名： 串口初始化
功能描述： 51单片机串口初始化函数
返回函数： none
其他说明： none
**************************************************************************/
void UART_init(void)
{
    SCON = 0x50;             // SCON: 模式 1, 8-bit UART, 使能接收 
    TMOD = 0x20;            // TMOD: timer 1, mode 2, 8-bit 重装
    TH1 = TL1 = -(FOSC/12/32/baud); //TH1:  重装值 9600 波特率 晶振 11.0592MHz
    TR1 = 1;                //TR1:  timer 1 打开  
    ES = 1;                 //打开串口中断
    EA = 1;                 //打开总中断
}

/************************************************************************
功能描述： 	串口发送一字节数据
入口参数：	DAT:带发送的数据
返 回 值： 	none
其他说明：	none
**************************************************************************/
void UARTSendByte(unsigned char DAT)
{
	ES  =  0;
	TI=0;
	SBUF = DAT;
	while(TI==0);
	TI=0;
	ES = 1;
}


/************************************************************************
功能描述： 串口发送字符串数据
入口参数： 	*DAT：字符串指针
返 回 值： none
其他说明： API 供外部使用，直观！
**************************************************************************/
void PrintCom(unsigned char *DAT)
{
	while(*DAT)
	{
	 	UARTSendByte(*DAT++);
	}	
}


void UART_SendByte(unsigned char dat)
{
	unsigned char time_out;
	time_out=0x00;
	SBUF = dat;			  //将数据放入SBUF中
	while((!TI)&&(time_out<100))  //检测是否发送出去
	{time_out++;DelayUs2x(10);}	//未发送出去 进行短暂延时
	TI = 0;						//清除ti标志
}

void UART_SendStr(unsigned char *s,unsigned char length)
{
	unsigned char NUM;
	NUM=0x00;
	while(NUM<length)	//发送长度对比
	{
		UART_SendByte(*s);  //放松单字节数据
		s++;		  //指针++
		NUM++;		  //下一个++
  	 }
}
static  char  uart_buf[32]={0};  //用于保存串口数据
static  unsigned char uart_cnt=0;   //用于定位串口数据的位置
void UART_SER (void) interrupt 4 	//串行中断服务程序
{
	if(RI)                        //判断是接收中断产生
	{
		RI=0;                      //标志位清零
		//接收串口数据
		uart_buf[uart_cnt] =SBUF;  
		uart_cnt++;
		//判断是否是下发的指令
		if(uart_buf[uart_cnt-4]=='L'&&uart_buf[uart_cnt-3]=='E'&&uart_buf[uart_cnt-2]=='D'&&uart_buf[uart_cnt-1] == '0')  
		{
			led = 0;       //开灯
			uart_cnt = 0;  //清零，重新计数
		}
		else if(uart_buf[uart_cnt-4]=='L'&&uart_buf[uart_cnt-3]=='E'&&uart_buf[uart_cnt-2]=='D'&&uart_buf[uart_cnt-1] == '1')  
		{
			led = 1;       //关灯
			uart_cnt = 0;
		}
		if(uart_cnt>28)   //防止数组越界
		{
			uart_cnt = 0;
		}
	}
	if(TI)  //如果是发送标志位，清零
	TI=0;
} 





