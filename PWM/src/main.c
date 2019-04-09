#include <stc15.h>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

sbit PWMOUT= P0^0;

u8 T0RH;
u8 T0RL;
u8 HIGHRH;
u8 HIGHRL;
u8 LOWRH;
u8 LOWRL;
u32 tmp2 = 0;

void ConfigTimer0(u16 ms);
void ConfigTimer1(u32 fr, u8 dc);
void CloseOther();

void main()
{
	EA =1;
	CloseOther();
	P2 = (P2 & 0x1F) | 0x80; 
	P0 = 0xFF;
	ConfigTimer0(50);
	ConfigTimer1(100,1);
	
	while(1)
	{
		
	}
}

void ConfigTimer0(u16 ms)
{
	u32 tmp1;
	
	tmp1 = 11059200 / 12;
	tmp1 = (tmp1 * ms) / 1000;
	tmp1 = 65536 - tmp1;
	T0RH = (u8)(tmp1 >> 8);
	T0RL = (u8)tmp1;
	TMOD &= 0xF0;
	TMOD |= 0x01; 
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}

void AdjustPWM(u8 dc)
{
	u8 high,low;
	
	high = (tmp2 * dc) / 100;
	low = tmp2 - high;
	high = 65536 - high;
	low = 65536 - low;
	HIGHRH = (u8)(high >> 8);
	HIGHRL = (u8)high;
	LOWRH = (u8)(low >> 8);
	LOWRL = (u8)low;
}

void InterruptTimer0()interrupt 1
{
	u8 code table[] = {10,15,20,25,30,35,40,45,50,55,60,65,70,75,80};
	static u8 dir = 0;
	static u8 index = 0;
	
	AdjustPWM(table[index]);
	
	if(dir == 0)
	{
		index++;
		if(index == 15)
		{
			dir = 1;
		}
	}
	else
	{
		index--;
		if(index == 0)
		{
			dir = 0;
		}
	}
	
}
/*PWM��ָ�ߵ�ƽռ�İٷֱ�*/
void ConfigTimer1(u32 fr, u8 dc)//fr����Ƶ�ʣ�ʱ��= 1/fr��,dc��ʾ�ߵ�ƽ��ռ�ķݶ�
{
	u8 high,low;
	
	tmp2 = 11059200/12/fr;//�����ӦƵ���µļ�������
	high = (tmp2 * dc) / 100;//�ߵ�ƽ�ļ�������
	low = tmp2 - high;//�͵�ƽ�ļ�������
	high = 65536 - high;//�ߵ�ƽ�ĳ�ֵ
	low = 65536 - low;//�͵�ƽ�ĳ�ֵ
	HIGHRH = (u8)(high >> 8);//���߰�λ��ֵ
	HIGHRL = (u8)high;//���Ͱ�λ��ֵ
	LOWRH = (u8)(low >> 8);
	LOWRL = (u8)low;
	
	TMOD &= 0x0F;//ѡ����ʱ��1����ʱ��0Ҳ���ԣ��������õ��Ƕ�ʱ��1
	TMOD |= 0x10;
	TH1 = HIGHRH;//���Ĵ����߰�λ��ֵ
	TL1 = HIGHRL;//���Ĵ����Ͱ�λ��ֵ
	ET1 = 1;//�����ж�1
	TR1 = 1;//����������
	PWMOUT = 1;//�����������
}

void InterruptTimer1() interrupt 3
{
	if(PWMOUT)
	{
		TH1 = LOWRH;
		TL1 = LOWRL;
		PWMOUT = 0;
	}
	else
	{
		TH1 = HIGHRH;
		TL1 = HIGHRL;
		PWMOUT = 1;
	}
}

void CloseOther()
{
	P2 = (P2 & 0x1F) | 0xA0;
	P0 = 0xAF;
	P2 &= 0x1F;
}