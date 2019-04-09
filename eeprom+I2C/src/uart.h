#ifndef _UART_H
#define _UART_H

void InitUart(u8 sta);
void UartSendStr(char* str);
void UartSendNum(u32 dat, u8 sta);
void UartSendStrNum(char* buf, u32 dat, u8 sta);
void UartSendHex(u16 hex);
void UartSendBinary(u8 dat);

void UartDriver();//注意这个需要在主函数中while(1)调用
void UartMonitor(u8 ms);
u8 UartRead(u8* buf, u8 len);
void UartWrite(u8* buf, u8 len);
void UartSendFloat(float dat, u8* str);

#endif
