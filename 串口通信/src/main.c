#include "config.h"
#include "uart.h"

void main()
{
    unsigned char a=0x55;
    unsigned int b=0xAB98;
    unsigned long c=1234567890; 
    unsigned char Buf[]="欢迎使用STC15单片机!\n";   //字符串在内存结尾必然有一个附加字符：\0   

	EA = 1;
	
    InitUart(0);                      				// 9600bps@11.0592MHz
    UartSendStr("串口设置完毕：123ABC\n");  		// 发送字符串
    UartSendStr(Buf);		
    UartSendNum(b, 1);               				// 发送数值
    UartSendStrNum("发送=:",c, 1);   				// 发送字符串+数值
    UartSendHex(b) ;                 				// 发送16进制
    UartSendBinary(a);               				// 发送2进制

		
    while(1)
    {
		
	}
}

void UartAction()
{
//以下仅仅作为演示

}
