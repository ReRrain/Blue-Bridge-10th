#include "config.h"
#include "ds1302.h"
#include "led.h"
#include "main.h"

struct sTime setClockTime;
struct sTime setAlarmTime={0,0,0,0,0,0,0};//闹钟时间结构体，初始化为0

void SetClockTimer()
{
	if(flagenter == 0)//允许得到实时时间
	{
		GetRealTime(&setClockTime);//得到实时时间
		flagenter = 1;//马上置1，表示不再获取实时时间了，确保设置模式下，数码管不再刷新时间
	}
	if(numBlinkSta == 1)
	{
		LedBuff[index] = 0xFF;
		LedBuff[index - 1] = 0xFF; 
	}
	else
	{
		RefreshSetTime(&setClockTime);//显示时间
	}	
}

void SetAlarmTimer()
{
	if(numBlinkSta == 1)
	{
		LedBuff[index] = 0xFF;
		LedBuff[index - 1] = 0xFF; 
	}
	else
	{
		RefreshSetTime(&setAlarmTime);
	}
}

void AddTime(struct sTime *time)
{
	if(index == 7)
	{
		(time->hour) += 0x01;
		if(((time->hour) & 0x0F) == 0x0A)//如果低四位是A的情况
		{
			(time->hour) = ((time->hour) & 0xF0) + 0x10; //显示正常值
		}
		if((time->hour) > 0x23)
		{
			(time->hour) = 0x00;
		}
	}
	else if(index == 4)
	{
		time->min += 0x01;
		if(((time->min) & 0x0F) == 0x0A)
		{
			time->min = ((time->min) & 0xF0) + 0x10;
		}
		if((time->min) > 0x59)
		{
			time->min = 0x00;
		}
	}
	else
	{
		(time->sec) += 0x01;
		if(((time->sec) & 0x0F) == 0x0A)
		{
			time->sec = ((time->sec) & 0xF0) + 0x10;
		}
		if((time->sec) > 0x59)
		{
			time->sec = 0x00;
		}
	}
}

void SubTime(struct sTime *time)
{
	if(index == 7)
	{
		(time->hour) -= 0x01;
		if(((time->hour) & 0x0F) == 0x0F)//如果低四位是A的情况
		{
			(time->hour) = ((time->hour) & 0xF0) + 0x09; //显示正常值
		}
		if((time->hour) == 0xF9)
		{
			(time->hour) = 0x23;
		}
	}
	else if(index == 4)
	{
		time->min -= 0x01;
		if(((time->min) & 0x0F) == 0x0F)
		{
			time->min = ((time->min) & 0xF0) + 0x09;
		}
		if((time->min) == 0xF9)
		{
			time->min = 0x59;
		}
	}
	else
	{
		(time->sec) -= 0x01;
		if(((time->sec) & 0x0F) == 0x0F)
		{
			time->sec = ((time->sec) & 0xF0) + 0x09;
		}
		if((time->sec) == 0xF9)
		{
			time->sec = 0x59;
		}
	}
}