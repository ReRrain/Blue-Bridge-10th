#include "config.h"
#include "led.h"
#include "key.h"
#include "pcf8591.h"

u8 T0RH;
u8 T0RL;
u8 flag200ms = 0;
u8 flagstart = 0;

void ConfigTimer0(u8 ms);
void KeyAction(u8 keycode);
void CloseOther();
void RefreshVal();

void main()
{
	CloseOther();
	EA = 1;
	ConfigTimer0(2);
//	ShowNumber(0);
	
	while(1)
	{
		KeyDriver();
		if(flagstart == 1)
		{
			if(flag200ms == 1)
			{
				flag200ms = 0;
				//ShowNumber(GetADCValue(0));
				RefreshVal();
			}
		}
	}
}

void CloseOther()
{
	P2 = (P2 & 0x1F) | 0x80;
	P0 = 0xFF;
	P2 &= 0x1F;
	
	P2 = (P2 & 0x1F) | 0xA0;
	P0 = 0xAF;//这里不能是或！！！！
	P2 &= 0x1F;
}

	
void KeyAction(u8 keycode)
{
	if(keycode  == '1')
	{
		flagstart = 1;
	}
//		ShowNumber(3);
}

void ConfigTimer0(u8 ms)
{
	u32 tmp;
	tmp = 11059200 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;
	T0RH = (u8)(tmp>>8);
	T0RL = (u8)tmp;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
} 

void InterruptTimer0() interrupt 1
{
	static u32 tmr200ms = 0;
	TH0 = T0RH;
	TL0 = T0RL;
	tmr200ms++;
	
	LedScan();
	KeyScan();
	
	if(tmr200ms >= 100)
	{
		tmr200ms = 0;
		flag200ms = 1;
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
	
	for(i = 0;i<2; i++)
	{
		LedBuff[i] = LedChar[val % 10];
		val /= 10;
	}
	
	LedBuff[1] &= 0x7F;
}
