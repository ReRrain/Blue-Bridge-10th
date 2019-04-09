#include "config.h"
#include "led.h"
#include "ds1302.h"

u8 flag200ms = 0;
u8 T0RH;
u8 T0RL;

struct sTime buffTime;

void ConfigTimer0(u8 ms);
void CloseOther();
void RefreshTime();

void main()
{
	CloseOther();
	EA = 1;
	ConfigTimer0(2);
	InitDS1302();
	
	while(1)
	{
		if(flag200ms == 1)
		{
			flag200ms = 0;
			RefreshTime();
		}
	}
}

void CloseOther()
{
	P2 = (P2 & 0x1F) | 0x80;
	P0 = 0xFF;
	P2 &= 0x1F;
	
	P2 = (P2 & 0x1F) | 0xA0;
	P0 = 0xAF;
	P2 &= 0x1F;
}

void ConfigTimer0(u8 ms)
{
	u32 tmp;
	 
	tmp = 11059200 / 12;
	tmp = (tmp * ms) /1000;
	tmp = 65536 - tmp;
	T0RH = (u8)(tmp>>8);//这里是右移！！！
	T0RL = (u8)tmp;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}



void Interrupttimer0() interrupt 1
{
	static u16 tmr200ms = 0;
	TH0 = T0RH;
	TL0 = T0RL;
	tmr200ms++;
	LedScan();
	
	if(tmr200ms >= 100)
	{
		flag200ms = 1;
		tmr200ms = 0;
	}
}

void ShowLedNumber(u8 index, u8 num)
{
	if(num == 0xBF)//第2.5数码管的特殊处理
		LedBuff[index] = 0xBF;
	else
		LedBuff[index] = LedChar[num];
}


void RefreshTime()
{
	GetRealTime(&buffTime);
	ShowLedNumber(7, buffTime.hour>>4);
	ShowLedNumber(6, buffTime.hour&0x0F);
	ShowLedNumber(5, 0xBF);
	ShowLedNumber(4, buffTime.min>>4);
	ShowLedNumber(3, buffTime.min&0x0F);
	ShowLedNumber(2, 0xBF);
	ShowLedNumber(1, buffTime.sec>>4);
	ShowLedNumber(0, buffTime.sec&0x0F);
}


