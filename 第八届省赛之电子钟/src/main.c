#include "config.h"
#include "led.h"
#include "key.h"
#include "ds18b02.h"
#include "ds1302.h"

struct sTime setClockTime;
struct sTime setAlarmTime = {0, 0, 0, 0, 0, 0, 0};

u8 T0RH;
u8 T0RL;
u8 flag200ms = 0;
u8 flag18b20 = 0;//0-时钟模式，1-温度显示
u8 flagmode = 0;//0-运行模式，1-时钟设置模式，2-闹钟设置模式
u8 numBlink = 1;//0-数码管灭，1-数码管亮
u8 ledSta = 0;//0-L1灭，1-L1亮
char index = 7;
u8 flaginter = 0;//0-得到实时时间，1-不得到实时时间
u8 flagstop =1;//0-Led闪烁，1-led停止闪烁

void ConfigTimer0(u8 ms);
void CloseOther();
void RefreshTemp();
void RefreshTime(struct sTime *time);
void SetClockTimer();
void SetAlarmTimer();
void AddTime(struct sTime *time);
void SubTime(struct sTime *time);
void LedBlink();


void main()
{
	CloseOther();
	EA = 1;
	ConfigTimer0(2);
	Start18b02();
	InitDS1302();//初始化DS1302
    GetRealTime(&setClockTime);//相当于给setClockTime这个变量赋一个初值，
	                          //否则在主函数中定义的变量，初始化相当于0，
	                           //在子函数中的变量是垃圾值
	while(1)
	{
		KeyDriver();
	    if(flag200ms == 1)
		{
			
			flag200ms = 0;
			
			if(flagmode == 0)
			{
				if(((setClockTime.hour) == (setAlarmTime.hour)) &&((setClockTime.min) == (setAlarmTime.min)) && ((setClockTime.sec) == (setAlarmTime.sec)))
				{
					flagstop = 0;
				}
				if(flag18b20 == 0)
				{
					GetRealTime(&setClockTime);
					RefreshTime(&setClockTime);

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
			else if(flagmode == 1)
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
	static u16 tmr1s = 0;
	static u16 tmr5s = 0;
	
	TH0 = T0RH;
	TL0 = T0RL;
	tmr200ms++;
	tmr1s++;
	
	KeyScan();
	LedScan();
	if(tmr200ms >= 100)
	{
		flag200ms = 1;
		tmr200ms = 0;
		ledSta ^= 1;
	}
	if(tmr1s >= 500)
	{
		tmr1s = 0;
		numBlink ^= 1;
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
	else
	{
		tmr5s = 0;
	}
}

void KeyAction(u8 keycode)
{
	if(keycode == '1')
	{
		if(flagmode == 0)
		{
			if(flagstop == 0)
			{
				flagstop = 1;
			}
			else
			{
				flagmode = 1;
				index = 7;
				flaginter = 0;
			}
		}
		else if(flagmode == 1)
		{
			index -= 3;
			if(index < 0)
			{
				index = 7;
				flagmode = 0;
			}
			RefreshTime(&setClockTime);
		}
	}
	else if(keycode == '2')
	{
		if(flagmode == 0)
		{
			if(flagstop == 0)
			{
				flagstop = 1;
			}
			else
			{
				flagmode = 2;
				index = 7;
			}
		}
		else if(flagmode == 2)
		{
			index -= 3;
			if(index < 0)
			{
				flagmode = 0;
				index = 7;
			}
			RefreshTime(&setAlarmTime);			
		}
	}
    else if(keycode == '3')
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
			AddTime(&setClockTime);
			RefreshTime(&setClockTime);
			SetRealTime(&setClockTime);
		}
		if(flagmode == 2)
		{
			AddTime(&setAlarmTime);
			RefreshTime(&setAlarmTime);
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
			else
			{	
				flag18b20 = 1;
			}
		}
		if(flagmode == 1)
		{
			SubTime(&setClockTime);
			RefreshTime(&setClockTime);
			SetRealTime(&setClockTime);
		}
		if(flagmode == 2)
		{
			SubTime(&setAlarmTime);
			RefreshTime(&setAlarmTime);
		}
	}
}

void SetClockTimer()
{
	if(flaginter == 0)
	{
		GetRealTime(&setClockTime);
		flaginter = 1;
	}
	if(numBlink == 0)
	{
		LedBuff[index] = 0xFF;
		LedBuff[index-1] = 0xFF;
	}
	else
	{
		 RefreshTime(&setClockTime);
	}
}

void SetAlarmTimer()
{
	if(numBlink == 0)
	{
		LedBuff[index] = 0xFF;
		LedBuff[index-1] = 0xFF;
	}
	else
	{
		RefreshTime(&setAlarmTime);
	}
}

void LedBlink()
{
	if(ledSta)
	{
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFE;
		P2 &= 0x1F;
	}
	else
	{
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFF;
		P2 &= 0x1F;
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
	
void ShowLedNumber(u8 index, u8 num)
{
	if(num == 0xBF)//第2.5数码管的特殊处理
		LedBuff[index] = 0xBF;
	else
		LedBuff[index] = LedChar[num];
}


void RefreshTime(struct sTime *time)
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

void AddTime(struct sTime *time)
{
	if(index == 7)
	{
		time->hour += 0x01;
		if(((time->hour) & 0x0F) == 0x0A)
		{
			(time->hour) = ((time->hour) & 0xF0) + 0x10;
		}
		if((time->hour) == 0x24)
		{
			time->hour = 0x00;
		}
	}
	else if(index == 4)
	{
		time->min += 0x01;
		if(((time->min) & 0x0F) == 0x0A)
		{
			(time->min) = ((time->min) & 0xF0) + 0x10;
		}
		if((time->min) == 0x60)
		{
			time->min = 0x00;
		}
	}
	else
	{
		time->sec += 0x01;
		if(((time->sec) & 0x0F) == 0x0A)
		{
			(time->sec) = ((time->sec) & 0xF0) + 0x10;
		}
		if((time->sec) == 0x60)
		{
			time->sec = 0x00;
		}
	}
}

void SubTime(struct sTime *time)
{
	if(index == 7)
	{
		time->hour -= 0x01;
		if(((time->hour) & 0x0F) == 0x0F)
		{
			time->hour = ((time->hour) & 0xF0) + 0x09;
		}
	    if(time->hour == 0xF9)
		{
			time->hour = 0x23;
		}
	}
	else if(index == 4)
	{
		time->min -= 0x01;
		if(((time->min) & 0x0F) == 0x0F)
		{
			time->min = ((time->min) & 0xF0) + 0x09;
		}
	    if(time->min == 0xF9)
		{
			time->min = 0x59;
		}
	}
	else
	{
		time->sec -= 0x01;
		if(((time->sec) & 0x0F) == 0x0F)
		{
			time->sec = ((time->sec) & 0xF0) + 0x09;
		}
		if(time->sec == 0xF9)
		{
			time->sec = 0x59;
		}
	}
}