#include "config.h"
#include "led.h"
#include "key.h"
#include "ds18b02.h"
u8 T0RH;
u8 T0RL;
u8 flag200ms = 0;
u8 flagstart = 0;

void ConfigTimer0(u8 ms);
void CloseOther();
void OpenOther();
void RefreshTemp();

void main()
{
	CloseOther();
	EA = 1;
	ConfigTimer0(2);
	Start18b02();
	
	while(1)
	{
		KeyDriver();
//	if(flagstart == 1)
//		{
			if(flag200ms == 1)
			{
				flag200ms = 0;
				RefreshTemp();
			} 
//		}
//		else
//		{
//			LedBuff[2] = 0xFF;
//			LedBuff[1] = 0xFF;
//			LedBuff[0] = 0xFF;
//		}
	}
}

void ConfigTimer0(u8 ms)
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

void InterruptTimer0() interrupt 1
{
	static u8 tmr200ms = 0;
	
	TH0 = T0RH;
	TL0 = T0RL;
	tmr200ms++;
	
	KeyScan();
	LedScan();
	if(tmr200ms >= 100)
	{
		flag200ms = 1;
		tmr200ms = 0;
	}
}

void KeyAction(u8 keycode)
{
	if(keycode == '1')
		flagstart ^= 1; 
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

//void OpenOther()
//{
//	P2 = (P2 & 0x1F) | 0x80;
//	P0 = 0x00;
//	P2 &= 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xA0;
//	P0 = 0x5F;
//	P2 &= 0x1F;
//}

void RefreshTemp()
{
	int temp;
	u8 i;
	
	for(i = 7; i>=3; i--)
	{
		LedBuff[i] = 0xFF;
	}
	Get18b02Temp(&temp);
	Start18b02();
	
	
	temp >>= 4;
	
	LedBuff[2] = LedChar[temp / 10];
	LedBuff[1] = LedChar[temp % 10];
	LedBuff[0] = LedChar[12];
}
	
	
