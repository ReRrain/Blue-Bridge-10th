#include "config.h"
#include "led.h"
#include "ds1302.h"
#include "key.h"
#include "ds18b02.h"
#include "time.h"
#include "main.h"

u8 flag18b20 = 0;//0-ʱ����ʾ��1-�¶���ʾ
u8 flag200ms = 0;
u8 T0RH;
u8 T0RL;
u8 flagstop = 1;//0-led��ʼ��˸��1-led����˸
u8 flagmode = 0;//0-����ģʽ��1-ʱ������ģʽ��2-��������ģʽ
char index = 7;
u8 flagenter = 0;//0-�õ�ʵʱʱ�䣬1-��ˢ��ʱ��
u8 numBlinkSta = 0;//0-���������1-�������
u8 sta = 0;//1-L1����0-L1��

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
	if(num == 0xBF)//��2.5����ܵ����⴦��
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
	T0RH = (u8)(tmp>>8);//���������ƣ�����
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
	if(keycode == '1')//����S7����
	{
		if(flagmode == 0)//�����ǰΪ����ģʽ
		{
			if(flagstop == 0)
			{
				flagstop = 1;
			}
			else
			{
				flagmode = 1;//��ô��������ģʽ
				flagenter = 0;
				index = 7;
			}
		}
		else if(flagmode == 1)//������°�����ǰΪ����ʱ��ģʽ
		{
			index -= 3;
			if(index < 0)
			{
				index = 7;
				flagmode = 0;//��ô��������ģʽ
			}
			RefreshSetTime(&setClockTime);//�������°�����ʱ���֣��룬��������һ����˸�����
			
		}
	}
	else if(keycode == '2')
	{
		if(flagmode == 0)//�����ǰΪ����ģʽ
		{
			if(flagstop == 0)
			{
				flagstop = 1;
			}
			else
			{
				flagmode = 2;//��ô��������ģʽ
				flagenter = 0;
				index = 7;
			}
		}
		else if(flagmode == 2)//������°�����ǰΪ��������ģʽ
		{
			index -= 3;
			if(index < 0)
			{
				index = 7;
				flagmode = 0;//��ô��������ģʽ
			}
			RefreshSetTime(&setAlarmTime);//�������°�����ʱ���֣��룬��������һ����˸�����
		}
	}
	else if(keycode == '3')
	{
		if(flagmode == 0)//�������������ģʽ
		{
			if(flagstop == 0)//L1������˸״̬
			{
				flagstop = 1;//��ô����L1ֹͣ��˸
			}
		}
		if(flagmode == 1)//�����ʱ������ģʽ
		{
				AddTime(&setClockTime);
				SetRealTime(&setClockTime);//���ı���ʱ��ʱ��д��DS1302
			    RefreshSetTime(&setClockTime);//Ϊ������������ʱ��ʱ��������ؿ������ú������
		}
		if(flagmode == 2)//�������������ģʽ
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
		if(flagmode == 2)//�������������ģʽ
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
	else//һ��С��ֹͣ��˸����ֹͣ5s��ʱ
	{
		tmr5s = 0;
	}
	if(tmr1s >= 500)
	{
		tmr1s = 0;
		numBlinkSta ^= 1;
	}
}




