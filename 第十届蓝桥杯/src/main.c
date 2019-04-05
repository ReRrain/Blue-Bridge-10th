#include "config.h"
#include "led.h"
#include "key.h"
#include "pcf8591.h"
#include "test.h"

u8 T1RH;
u8 T1RL;
u8 flag1s = 0;
u16 val = 0;
u32 cnt = 0;//记录脉冲个数
u32 cntBuf = 0;//记录脉冲个数
u8 flag200ms = 0;
u8 flagDAC = 0;//0-初始状态，1-RB2
u8 flagmode = 0;//0-电压测量，1-频率测量
u8 flagLED = 0;//0-LED模式，1-关闭
u8 flagShow = 0;//0-数码管显示，1-不显示
u8 flag200ms1 = 0;//200ms刷新一次灯的状态

void CloseOther();
void ConfigTimer1(u16 ms);
void ConfigTimer0();
void RefreshVal();
void SetDAC();

void main()
{
	u8 i;
	CloseOther();
	ConfigTimer1(1);
	ConfigTimer0();
	
	EA = 1;
	while(1)
	{
		KeyDriver();
		
		if(flagShow == 0)
		{
			if(flagmode == 0)
			{
				if(flag200ms)
				{
					flag200ms = 0;
					
					ValTest();
					LedBuff[7] = 0xFF;
					
					RefreshVal();
					SetDAC();
				}
			}
			else if(flagmode == 1)
			{
				if(flag200ms1)
				{
					flag200ms1 = 0;
					FrTest();
				}
				if(flag1s)
				{
					flag1s = 0;
					cntBuf = cnt;
					ShowNumber(cntBuf);
					LedBuff[7] &= LedChar[15];
					cnt = 0;
				}
			}
		}
		else
		{
			for(i = 0; i< 8; i++)
			{
				LedBuff[i] = 0xFF;
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
	P0 = 0xAF;
	P2 &= 0x1F;
}

void ConfigTimer0()
{
	TH0 = 0xFF;
	TL0 = 0xFF;
	TMOD &= 0xF0;
	TMOD |= 0x04;
    ET0 = 1;
	TR0 = 1;
}

void ConfigTimer1(u16 ms)
{
	u32 tmp;
	tmp = 12000000 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;
	T1RH = (u8)(tmp >> 8);
	T1RL = (u8)tmp;
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TH1 = T1RH;
	TL1 = T1RL;
	ET1 = 1;
	TR1 = 1;
}

void InterruptTimer1()interrupt 3
{
	static u8 tmr200ms = 0;
	static u8 tmr200ms1 = 0;
	static u16 tmr1s = 0;
	TH1 = T1RH;
	TL1 = T1RL;
	LedScan();
	KeyScan();
	tmr1s++;
	tmr200ms1++;
	if(tmr200ms1 >= 200)
	{
		tmr200ms1 = 0;
		flag200ms1 = 1;
	}
	if(tmr1s >= 1000)
	{
		tmr1s = 0;
		flag1s = 1;
	}
	
	if(flagmode == 0)
	{
		tmr200ms++;
		if(tmr200ms >= 200)
		{
			tmr200ms = 0;
			flag200ms = 1;
		}
	}
}

void InterruptTimer0()interrupt 1
{
	cnt++;
}

void KeyAction(u8 keycode)
{
	if(keycode == '4')
	{
		flagmode ^= 1;
	}
	else if(keycode == '3')
	{
		flagDAC ^= 1;
	}
	else if(keycode == '2')
	{
		flagLED ^= 1;
	}
	else if(keycode == '1')
	{
		flagShow ^= 1;
	}
}

void RefreshVal()
{
	u8 i;
	val = GetADCValue(3);
	
	for(i = 6; i>2; i--)
	{
		LedBuff[i] = 0xFF;
	}
	
	LedBuff[7] &= 0xC1;
	LedBuff[0] = LedChar[val % 10];
	LedBuff[1] = LedChar[(val / 10) % 10];
	LedBuff[2] = LedChar[(val / 100) % 10];
	LedBuff[2] &= 0x7F;
}

void SetDAC()
{
	u16 InitDAC = 200;//初始化DAC输出电压2v
	
	if(flagDAC == 0)//DAC输出是初始状态
	{
		SetDACValue(InitDAC);
	}
	else//DAC输出随RB2改变
	{
		SetDACValue(val);
	}
}
