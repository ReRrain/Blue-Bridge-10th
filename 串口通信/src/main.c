#include "config.h"
#include "uart.h"

void main()
{
    unsigned char a=0x55;
    unsigned int b=0xAB98;
    unsigned long c=1234567890; 
    unsigned char Buf[]="��ӭʹ��STC15��Ƭ��!\n";   //�ַ������ڴ��β��Ȼ��һ�������ַ���\0   

	EA = 1;
	
    InitUart(0);                      				// 9600bps@11.0592MHz
    UartSendStr("����������ϣ�123ABC\n");  		// �����ַ���
    UartSendStr(Buf);		
    UartSendNum(b, 1);               				// ������ֵ
    UartSendStrNum("����=:",c, 1);   				// �����ַ���+��ֵ
    UartSendHex(b) ;                 				// ����16����
    UartSendBinary(a);               				// ����2����

		
    while(1)
    {
		
	}
}

void UartAction()
{
//���½�����Ϊ��ʾ

}
