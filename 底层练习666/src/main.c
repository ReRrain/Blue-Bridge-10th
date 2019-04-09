#include "config.h"
#include "led.h"
#include "key.h"
#include "e2prom.h"
#include "pcf8591.h"
#include "ds18b20.h"
#include "ds1302.h"

u8 T0RH;
u8 T0RL;
u8 test = 0;
u8 flag200ms = 0;

struct sTime buffTime;

void CloseOther();
void ConfigTimer0(u16 ms);
void RefreshVal();
void RefreshTemp();
void RefreshTime();

void main()
{
    E2Read(&test, 0x00, 1);
	EA = 1;
	CloseOther();
	ConfigTimer0(1);
	Start18B20();
	InitDS1302Time();
	
	while(1)
	{
		KeyDriver();
		if(flag200ms)
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

void ConfigTimer0(u16 ms)
{
	u32 tmp;
	tmp = 11059200 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;
	T0RH = (u8)(tmp >> 8);
	T0RL = (u8)tmp;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}

void InterruptTimer0()interrupt 1
{
	static tmr200ms = 0;
	TH0 = T0RH;
	TL0 = T0RL;
	LedScan();
	KeyScan();
	
	tmr200ms++;
	if(tmr200ms >= 200)
	{
		tmr200ms = 0;
		flag200ms = 1;
	}
}

void KeyAction(u8 keycode)
{
	if(keycode == '1')
	{
		test++;
		E2Write(&test, 0x00, 1);
	}
}

void RefreshVal()
{
	u8 i;
	u8 val;
	val = GetADCValue(3);
	
	for(i = 7; i>1; i--)
	{
		LedBuff[i] = 0xFF;
	}
	LedBuff[0] = LedChar[val % 10];
	LedBuff[1] = LedChar[(val / 10) % 10];
	LedBuff[1] &= 0x7F;
}

void RefreshTemp()
{
	u8 i;
	int temp;
	
	Start18B20();
	Get18B20Temp(&temp);
	temp >>= 4;
	Start18B20();
	for(i = 7; i>2; i--)
	{
		LedBuff[i] = 0xFF;
	}
	LedBuff[0] = LedChar[12];
	LedBuff[1] = LedChar[temp % 10];
	LedBuff[2] = LedChar[(temp / 10) % 10];
}

void ShowLedNumber(u8 index, u8 num)
{
	if(num == 0xBF)
	{
		LedBuff[index] = 0xBF;
	}
	else
	{
		LedBuff[index] = LedChar[num];
	}
}

void RefreshTime()
{
	GetRealTime(&buffTime);
	ShowLedNumber(7, (buffTime.hour) >> 4);
	ShowLedNumber(6, (buffTime.hour) & 0x0F);
	ShowLedNumber(5, 0xBF);
	ShowLedNumber(4, (buffTime.min) >> 4);
	ShowLedNumber(3, (buffTime.min) & 0x0F);
	ShowLedNumber(2, 0xBF);
	ShowLedNumber(1, (buffTime.sec) >> 4);
	ShowLedNumber(0, (buffTime.sec) & 0x0F);
}
