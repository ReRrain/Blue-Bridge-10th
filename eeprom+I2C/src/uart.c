#include "config.h"
#include <string.h>
#include <stdio.h>

bit flagTxd = 0;

bit flagRxdFrame = 0;
u8 cntRxd = 0;
u8 bufRxd[100];

extern void UartAction();//注意这个需要在main.c中书写！

/*长整型转化为字符串*/
u8 LongToStr(long dat, u8* str)
{
    char i = 0;
    u8 len = 0;
    u8 buf[11];     // // 长整数最大值4294967295，转ASCII码后占用10+1=11字节(加'\0'字符)

    if(dat < 0)
    {
        dat = -dat;
        *str++ = '-';
        len++;
    }
    do{
        buf[i++] = dat%10 + '0';
        dat /= 10;
    }while(dat > 0);
    len += i;
    while(i-- > 0)
    {
        *str++ = buf[i]; // 上面已经进行+0x30的处理，故此处不再处理
    }
    *str = '\0';
    return len;
}

/*字符串转化为长整型*/
u32 StrToLong(char* str)
{
    u32 result = 0;
    u32 fact = 1;
    u8 len = strlen(str); // 不包括字符'\0'
    u8 i;

    for(i=len-1; i>=0; i--)
    {
        result += ((str[i] - '0') * fact);
        fact *= 10;
    }
    return result;
}

/*串口初始化*/
void InitUart(u8 sta)
{
	if(sta == 0) // 波特率：9600 /11.0592MHZ
	{
		PCON &= 0x7F;       //波特率不倍速
		SCON = 0x50;        //8位数据,可变波特率
		AUXR &= 0xBF;       //定时器1时钟为Fosc/12,即12T
		AUXR &= 0xFE;       //串口1选择定时器1为波特率发生器
		TMOD &= 0x0F;       //清除定时器1模式位
		TMOD |= 0x20;       //设定定时器1为8位自动重装方式
		TL1 = 0xFD;         //设定定时初值
		TH1 = 0xFD;         //设定定时器重装值
		ET1 = 0;            //禁止定时器1中断
		TR1 = 1;            //启动定时器1
		
	}
	else if(sta == 1)// 波特率：9600 /22.1184MHZ
	{	  
		AUXR = 0x00;  		// 定时器1作为波特率发生器       
		SCON = 0x50; 	
		TMOD |= 0x20;		// 0010 0000 定时器1工作于方式2（8位自动重装方式）
		TH1  = 0xFA;	
		TL1  = 0xFA;
		TR1  = 1;
		       
	}
	else if(sta == 2)// 波特率：115200 /11.0592MHZ
	{
		PCON &= 0x7F;		//波特率不倍速
		SCON = 0x50;		//8位数据,可变波特率
		AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
		AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
		TMOD &= 0x0F;		//清除定时器1模式位
		TMOD |= 0x20;		//设定定时器1为8位自动重装方式
		TL1 = 0xFD;			//设定定时初值
		TH1 = 0xFD;			//设定定时器重装值
		ET1 = 0;			//禁止定时器1中断
		TR1 = 1;			//启动定时器1
	}
	
	ES = 1;
}

/*发送单字节*/
void UartSendByte(u8 dat)
{
    flagTxd = 0;
    SBUF = dat;
    while(!flagTxd);
}

/*发送回车换行*/
void UartSendEnter()
{
    UartSendByte(0x0D);
    UartSendByte(0x0A);
}

/*发送字符串，如果结尾有\n，则后面自动加入回车换行*/
void UartSendStr(char *str)
{
    u16 i;
    u16 len = strlen(str); 

    for(i=0; i<len-1; i++) // 最后一个字符单独处理，实现加入回车换行
    {
        UartSendByte(str[i]);
    }

    if(str[i] == '\n')
    {
        UartSendEnter();
    }
    else
    {
        UartSendByte(str[i]);
    }
}

void UartSendFloat(float dat, u8* str)
{
	sprintf(str, "%.2f", dat);
	
	UartSendStr(str);
}

/*发送数值，实则转化为字符串发送，结尾自动加入回车换行*/
void UartSendNum(u32 dat, u8 sta)
{
    u8 buf[11];

    LongToStr(dat, buf);
    UartSendStr(buf);
	if(sta)
	{
		UartSendEnter();
	}
}

/*发送字符串+数值*/
void UartSendStrNum(char* buf, u32 dat, u8 sta)
{
    UartSendStr(buf);
    UartSendNum(dat, sta);
}

/*十进制转十六进制*/
void HexToASCII(u16 hex, char* str)
{
    u8 temp = 0;

    temp = ((hex & 0xF000) >> 12);
    str[0] = (temp >= 10) ? (temp-10+'A') : (temp+'0');
    
    temp = ((hex & 0x0F00) >> 8);
    str[1] = (temp >= 10) ? (temp-10+'A') : (temp+'0');
                                                 
    temp = ((hex & 0x00F0) >> 4);                 
    str[2] = (temp >= 10) ? (temp-10+'A') : (temp+'0');
                                                 
    temp = ((hex & 0x000F) >> 0);                 
    str[3] = (temp >= 10) ? (temp-10+'A') : (temp+'0');

    str[4] = '\0';
}

/*发送十六进制字符*/
void UartSendHex(u16 hex)
{
    u8 temp[11];

    HexToASCII(hex, temp);
    UartSendStr(temp);
    UartSendEnter();
}

/*发送二进制字符*/
void UartSendBinary(u8 dat)
{
    u8 i;
    u8 temp[17];

    for(i=0; i<8; i++)
    {
        temp[i] = ((dat<<i) & 0x80) ? '1':'0';
    }
    temp[i] = '\0';

    for(i=0; i<strlen(temp); i++)
    {
        UartSendByte(temp[i]);
        UartSendByte(' ');
    }
    UartSendEnter();
}

/*串口数据读取函数，len为指定的数据长度，返回为实际的数据长度*/
u8 UartRead(u8* buf, u8 len)
{
	u8 i;
	
	if(len > cntRxd)
	{
		len = cntRxd;
	}
	for(i=0; i<len; i++)
	{
		*buf++ = bufRxd[i];
	}
	*buf = '\0';
	cntRxd = 0;
	
	return len;
}

/*串口发送数据函数，len为指定的长度，较UartSendStr可以指定发送字符串长度*/
void UartWrite(u8* buf, u8 len)
{
	while(len--)
	{
		UartSendByte(*buf++);
	}
}

/*串口接收监控, ms-为定时间隔*/
void UartMonitor(u8 ms)
{
	static u8 cntbkp = 0;
	static u8 idletmr = 0;
	
	if(cntRxd > 0)
	{
		if(cntbkp != cntRxd)
		{
			cntbkp = cntRxd;
			idletmr = 0;
		}
		else
		{
			if(idletmr < 30)
			{
				idletmr += ms;
				if(idletmr >= 30)
				{
					flagRxdFrame = 1;
				}
			}
		}
	}
	else
	{
		cntbkp = 0;
	}
}

/*串口驱动函数，在主函数中调用*/
void UartDriver()
{
	if(flagRxdFrame)
	{
		flagRxdFrame = 0;
		UartAction();
	}
}

/*串口中断*/
void UartInterrupt() interrupt 4
{
	if(RI)
	{
		RI = 0;
		if(cntRxd < sizeof(bufRxd))
		{
			bufRxd[cntRxd++] = SBUF;
		}
	}
	
	if(TI)
	{
		TI = 0;
		flagTxd = 1;
	}

}
