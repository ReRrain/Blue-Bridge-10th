#include "config.h"
#include "led.h"
#include "ds1302.h"
#include "key.h"
#include "ds18b02.h"
#include "time.h"
#include "main.h"

u8 flag18b20 = 0;//0-时钟显示，1-温度显示
u8 flag200ms = 0;
u8 T0RH;
u8 T0RL;
u8 flagstop = 1;//0-led开始闪烁，1-led不闪烁
u8 flagmode = 0;//0-运行模式，1-时钟设置模式，2-闹钟设置模式
char index = 7;
u8 flagenter = 0;//0-得到实时时间，1-不刷新时间
u8 numBlinkSta = 0;//0-数码管亮，1-数码管灭
u8 sta = 0;//1-L1亮，0-L1灭

void main()
{
	CloseOther();
	EA = 1;
	ConfigTimer0(2);
	InitDS1302();
	Start18b02();
	GetRealTime(&setClockTime);
	
	while(1)
	{
		KeyDriver();
		if(flag200ms == 1)
		{
			flag200ms = 0;
			if(flagmode == 0)
			{
				if(((setClockTime.hour) == (setAlarmTime.hour)) && ((setClockTime.min) == (setAlarmTime.min)) && ((setClockTime.sec) == (setAlarmTime.sec)))
				{
					flagstop = 0;
				}
				if(flag18b20 == 0)
				{
					GetRealTime(&setClockTime);
					RefreshSetTime(&setClockTime);
				}
				else
				{
					RefreshTemp();
				}
		
				if(flagstop == 0)
				{
					LedBlink();
				}
				else
				{
					P2 = (P2 & 0x1F) | 0x80;
					P0 = 0xFF;
					P2 &= 0x1F;
				}
			}
			else
			{
				if(flagmode == 1)
				{
					SetClockTimer();
				}
				else
				{
					SetAlarmTimer();
				}
			}
		}
	}
}
void ShowLedNumber(u8 index, u8 num)
{
	if(num == 0xBF)//第2.5数码管的特殊处理
		LedBuff[index] = 0xBF;
	else
		LedBuff[index] = LedChar[num];
}

void RefreshSetTime(struct sTime *time)
{
	ShowLedNumber(7, (time->hour)>>4);
	ShowLedNumber(6, (time->hour)&0x0F);
	ShowLedNumber(5, 0xBF);
	ShowLedNumber(4, (time->min)>>4);
	ShowLedNumber(3, (time->min)&0x0F);
	ShowLedNumber(2, 0xBF);
	ShowLedNumber(1, (time->sec)>>4);
	ShowLedNumber(0, (time->sec)&0x0F);
}

void LedBlink()
{
//	static u8 sta = 0;
	
	if(sta)
	{
//		sta = 0;
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFE;
		P2 &= 0x1F;
	}
	else
	{
//		sta = 1;
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFF;
		P2 &= 0x1F;
	}
}

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
void KeyAction(u8 keycode)
{
	if(keycode == '1')//按下S7按键
	{
		if(flagmode == 0)//如果当前为运行模式
		{
			if(flagstop == 0)
			{
				flagstop = 1;
			}
			else
			{
				flagmode = 1;//那么进入设置模式
				flagenter = 0;
				index = 7;
			}
		}
		else if(flagmode == 1)//如果按下按键当前为设置时钟模式
		{
			index -= 3;
			if(index < 0)
			{
				index = 7;
				flagmode = 0;//那么进入运行模式
			}
			RefreshSetTime(&setClockTime);//消除按下按键，时，分，秒，任意两个一起闪烁的情况
			
		}
	}
	else if(keycode == '2')
	{
		if(flagmode == 0)//如果当前为运行模式
		{
			if(flagstop == 0)
			{
				flagstop = 1;
			}
			else
			{
				flagmode = 2;//那么进入设置模式
				flagenter = 0;
				index = 7;
			}
		}
		else if(flagmode == 2)//如果按下按键当前为设置闹钟模式
		{
			index -= 3;
			if(index < 0)
			{
				index = 7;
				flagmode = 0;//那么进入运行模式
			}
			RefreshSetTime(&setAlarmTime);//消除按下按键，时，分，秒，任意两个一起闪烁的情况
		}
	}
	else if(keycode == '3')
	{
		if(flagmode == 0)//如果是正常运行模式
		{
			if(flagstop == 0)//L1处于闪烁状态
			{
				flagstop = 1;//那么就让L1停止闪烁
			}
		}
		if(flagmode == 1)//如果是时钟设置模式
		{
				AddTime(&setClockTime);
				SetRealTime(&setClockTime);//将改变后的时钟时间写入DS1302
			    RefreshSetTime(&setClockTime);//为了在我们设置时间时可以清楚地看到设置后的数字
		}
		if(flagmode == 2)//如果是闹钟设置模式
		{
				AddTime(&setAlarmTime);
				RefreshSetTime(&setAlarmTime);
		}
	}
	else if(keycode == '4')
	{
		if(flagmode == 0)
		{
			if(flagstop == 0)
			{
				flagstop = 1;
			}
		}
		if(flagmode == 1)
		{
			SubTime(&setClockTime);
			SetRealTime(&setClockTime);
			RefreshSetTime(&setClockTime);
		}
		if(flagmode == 2)//如果是闹钟设置模式
		{
				SubTime(&setAlarmTime);
			RefreshSetTime(&setAlarmTime);
		}
	}
}

void Interrupttimer0() interrupt 1
{
	static u16 tmr1s = 0;
	
	static u16 tmr200ms = 0;
	static u16 tmr5s = 0;
	TH0 = T0RH;
	TL0 = T0RL;
	tmr200ms++;

	tmr1s++;
	
	LedScan();
	KeyScan();
	
	if(tmr200ms >= 100)
	{
		tmr200ms = 0;
		flag200ms = 1;
		sta ^= 1;
	}
	if(flagstop == 0)
	{
		tmr5s++;
		if(tmr5s >= 2500)
		{
			tmr5s = 0;
			flagstop = 1;
		}
	}
	else//一旦小灯停止闪烁，就停止5s计时
	{
		tmr5s = 0;
	}
	if(tmr1s >= 500)
	{
		tmr1s = 0;
		numBlinkSta ^= 1;
	}
}




