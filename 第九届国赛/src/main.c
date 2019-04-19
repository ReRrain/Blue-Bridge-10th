#include "config.h"
#include "led.h"
#include "key.h"
#include "e2prom.h"
#include "pcf8591.h"
#include "ds18b20.h"

u8 T1RH;
u8 T1RL;
u8 val = 0;
u8 setval = 1;
int temp = 0;
u8 tempH;//存储温度数据的前两位数据
u8 tempL;//存储温度数据的后两位数据
u8 cntH;//存储频率数据的高三位数据
u8 cntL;//存储频率数据的后两位数据
u8 flag200ms = 0;
u8 flag1s = 0;
u32 cnt = 0;//记录脉冲个数
u8 flagShow = 0;//0-温度，1-电压，2-频率
u8 flagShowSave = 0;//0-非回显，1-回显
u8 flagset = 0;//0-数据显示，1-设置
u8 flagLedBlink = 0;//0-灭，1-亮

void CloseOther();
void CloseLED();
void ConfigTimer1(u16 ms);
void ConfigTimer0();
void RefreshVal();
void RefreshTemp();
void RefreshFr();
void RefreshSetVal();
void RefreshSaveData();
void SaveData();
void ReadData();
void LedBlink();

void main()
{
	ReadData();
	EA = 1;
	CloseOther();
	CloseLED();
	ConfigTimer1(1);
	ConfigTimer0();
	Start18B20();
	
	while(1)
	{
		KeyDriver();
		if(flag200ms)
		{
			flag200ms = 0;
			if(flagset == 0)
			{
				if(flagShow == 0)
				{
					RefreshTemp();
				}
				else if(flagShow == 1)
				{
					RefreshVal();
					if(val > setval)
					{
						LedBlink();
					}
				}
				else
				{
					if(flag1s)
					{
						flag1s = 0;
						RefreshFr();
						cnt = 0;
					}
				}
			}
			else
			{
				RefreshSetVal();
			}
		}
	}
}

void CloseOther()
{
	P2 = (P2 & 0x1F) | 0xA0;
	P0 = 0xAF;
	P2 &= 0x1F;
}

void CloseLED()
{
	P2 = (P2 & 0x1F) | 0x80;
	P0 = 0xFF;
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

void InterruptTimer0()interrupt 1
{
	cnt++;
}

void ConfigTimer1(u16 ms)
{
	u32 tmp;
	tmp = 11059200 / 12;
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

void InterruptTimer1() interrupt 3
{
	static u8 tmr200ms = 0;
	static u32 tmr1s = 0;
	TH1 = T1RH;
	TL1 = T1RL;
	tmr200ms++;
	tmr1s++;
	LedScan();
	KeyScan();
	if(tmr200ms >= 200)
	{
		tmr200ms = 0;
		flag200ms = 1;
		flagLedBlink ^= 1;
	}
	if(tmr1s >= 1000)
	{
		tmr1s = 0;
		flag1s = 1;	
	}
}

void KeyAction(u8 keycode)
{
	if(keycode == '4')
	{
		if(flagShow < 2)
		{
			flagShow += 1;
		}
		else if(flagShow >= 2)
		{
			flagShow = 0;
		}
	}
	else if(keycode == '3')
	{
		SaveData();
	}
	else if(keycode == '2')
	{
		if(flagset == 0)
		{
			ReadData();
			flagShowSave = 1;
		}
		else
		{
			setval++;
			E2Write(&setval,0x05,1);
		}
	}
	else if(keycode == '1')
	{
		if(flagset == 0)
		{
			flagset = 1;
		}
		else if(flagset == 1)
		{
			flagset = 0;
			flagShowSave = 0;
		}
	}
}

void RefreshVal()
{
	u8 i;
	
	for(i = 7; i>1;i--)
	{
		LedBuff[i] = 0xFF;
	}
	if(flagShowSave == 0)
	{
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFB;
		P2 &= 0x1F;
		val = GetADCValue(3);
		LedBuff[6] = 0xFF;
		LedBuff[7] &= 0xC1;
	}
	else
	{
		CloseLED();
		LedBuff[6] &= 0xC1;
		LedBuff[7] &= 0x89;
	}
	LedBuff[0] = LedChar[val % 10];
	LedBuff[1] = LedChar[(val / 10) % 10];
	LedBuff[1] &= 0x7F;
	
}

void RefreshTemp()
{
	u8 i;

	tempL = temp % 100;
	tempH = temp / 100;
	Start18B20();
	for(i = 7; i>3; i--)
	{
		LedBuff[i] = 0xFF;
	}
	if(flagShowSave == 0)
	{
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFE;
		P2 &= 0x1F;
		Get18B20Temp(&temp);
		LedBuff[7] &= LedChar[12];
		LedBuff[6] = 0xFF;
	}
	else
	{
		CloseLED();
		LedBuff[7] &= 0x89;
		LedBuff[6] &= LedChar[12];
	}
	LedBuff[0] = LedChar[tempL % 10];
	LedBuff[1] = LedChar[(tempL / 10) % 10];
	LedBuff[2] = LedChar[tempH % 10];
	LedBuff[3] = LedChar[(tempH / 10) % 10];
	
	LedBuff[2] &= 0x7F;
}

void RefreshFr()
{
	cntL = cnt % 100;
	cntH = cnt / 100;
	if(flagShowSave == 0)
	{
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFD;
		P2 &= 0x1F;
		ShowNumber(cnt);
		LedBuff[7] = LedChar[15];
	}
	else
	{
		CloseLED();
		E2Read(&cntL,0x03,1);
		E2Read(&cntH,0x04,1);
		cnt = cntL + (cntH*100);
		ShowNumber(cnt);
		LedBuff[6] &= LedChar[15];
		LedBuff[7] &= 0x89;
	}
}

void RefreshSetVal()
{
	u8 i;
	
	for(i = 7; i>1; i--)
	{
		LedBuff[i] = 0xFF;
	}
	if(setval > 50)
	{
		setval = 1;
	}
	LedBuff[0] = LedChar[(setval % 10)];
	LedBuff[1] = LedChar[(setval / 10) % 10];
	LedBuff[1] &= 0x7F;
	LedBuff[7] &= 0x8C;
}

void SaveData()
{
	E2Write(&val,0x00,1);
	E2Write(&tempL, 0x01, 1);
	E2Write(&tempH, 0x02, 1);
	E2Write(&cntL,0x03,1);
	E2Write(&cntH,0x04,1);
}

void ReadData()
{
	E2Read(&val,0x00,1);
	E2Read(&tempL, 0x01, 1);
	E2Read(&tempH, 0x02, 1);
	E2Read(&cntL,0x03,1);
	E2Read(&cntH,0x04,1);
	E2Read(&setval,0x05,1);
}

void LedBlink()
{
	if(flagShowSave == 0)
	{
		P2 = (P2 & 0x1F) | 0x80;
		if(flagLedBlink == 0)
		{
			P0 = 0xFB;
		}
		else
		{
			P2 = (P2 & 0x1F) | 0x80;
			P0 = 0x7B;
			P2 &= 0x1F;
		}
		P2 &= 0xFF; 
	}
	else
	{
		CloseLED();
	}
}