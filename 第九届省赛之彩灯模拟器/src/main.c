#include "config.h"
#include "eeprom.h"
#include "key.h"
#include "led.h"
#include "i2.h"
#include "pcf8591.h"

u8 T0RH;
u8 T0RL;
u8 flag200ms = 0;
u8 hightime = 0;//�洢�ߵ�ƽ������ʱ��
u16 PWMSTA = 0xFF;//����ע��Ҫ��PWMSTA����ֵΪ0xFF��
//��������PWMSTA�洢P0�ڵ�״̬�ģ��ϵ��ʱ��ȷ�����ǹر�״̬

u8 flagled1 = 0;
u8 flagled2 = 0;
u8 flagled3 = 0;
u8 flagled4 = 0;
u8 flagstart = 0;//0-ֹͣLED��ת��1-��ʼLED��ת
u8 flagset = 0;//1-����ģʽ��0-������ģʽ
u8 runmode = 1;//����ģʽ���
u16 runtime[4] = {400, 400, 400, 400};//�ĸ�ģʽ�µ���˸���ʱ�䣨���ʹ���
u8 brightness = 0;//���ȵȼ�
u8 numBlinkSta = 1;//0-�������1-�������
u8 SetMode = 0;//0-��˸LED��ģʽ��1-��˸��ת���

void CloseOther();
void ConfigTimer0(u16 ms);//��ʱ
void ConfigTimer1();//��ʱ
void LedMode1();
void LedMode2();
void LedMode3();
void LedMode4();
void SetTime();
void AddTime();
void SubTime();
u8 AdjustDC();//�жϸߵ�ƽ��ʱ��

void main()
{
	u8 i;
	u8 runtimebuff[4];
	
	E2Read(runtimebuff, 0x00, 4);//һ���ֽ���1��u8��E2Prom�ĵ�ַ��Χ��0x00��0xFF����������־��ǵ�ַ������Ҫ��ȡ��ַ����
	for(i = 0;i <4; i++)
	{
	    runtime[i] = runtimebuff[i] * 10;
	}
	 
	EA = 1;
	CloseOther();
	ConfigTimer0(2);
	ConfigTimer1();
	
	while(1)
	{
		KeyDriver();
		if(flag200ms)
		{
			flag200ms = 0;
			hightime = AdjustDC();
		}
		if(flagstart)
		{
			P2 = (P2 & 0x1F) | 0x80;
			if(flagled1)
			{
				flagled1 = 0;
				LedMode1();
			}
			if(flagled2)
			{
				flagled2 = 0;
				LedMode2();
			}
			if(flagled3)
			{
				flagled3 = 0;
				LedMode3();
			}
			if(flagled4)
			{
				flagled4 = 0;
				LedMode4();
			}
			P2 &= 0x1F;
		}
		else if(flagstart == 0)
		{
			P2 = (P2 & 0x1F) | 0x80;
			P0 = 0xFF;
			P2 &= 0x1F;
		}
		if(flagset)
		{
			SetTime();
			flagstart = 0;
		}
		else
		{
			for(i = 0; i<8; i++)
			{
				LedBuff[i] = 0xFF;
			}
		}
    }
}

void LedMode1()
{
	static u8 shift1 = 0x01;//L1-L8
	P0 = ~shift1;
	PWMSTA = ~shift1;//��Ҫֱ�Ӳ���P0�ڣ�������
	
	shift1 <<= 1;
	
	if(shift1 == 0x00)
	{
		runmode = 2;
		shift1 = 0x01;
	}
}

void LedMode2()
{
	static u8 shift2 = 0x80;//L1-L8
	P0 = ~shift2;
	PWMSTA = ~shift2;
	
	shift2 >>= 1;
	P2 &= 0x1F;
	if(shift2 == 0x00)
	{
		runmode = 3;
		shift2 = 0x80;
	}
}

void LedMode3()
{
	static u8 index = 1;
	if(index == 1)
	{
		P0 = 0x7E;
		PWMSTA = 0x7E;
		index+=1;
	}
	else if(index == 2)
	{
		P0 = 0xBD;
		PWMSTA = 0xBD;
		index += 1;
	}
	else if(index == 3)
	{
		P0 = 0xDB;
		PWMSTA = 0xDB;
		index += 1;

	}
	else if(index == 4)
	{
		P0 = 0xE7;
		PWMSTA = 0xE7;
		index = 1;
		runmode = 4;
	}
}

void LedMode4()
{
	static u8 index = 1;
	if(index == 1)
	{
		P0 = 0xE7;
		PWMSTA = 0xE7;
		index+=1;
	}
	else if(index == 2)
	{
		P0 = 0xDB;
		PWMSTA = 0xDB;
		index += 1;
	}
	else if(index == 3)
	{
		P0 = 0xBD;
		PWMSTA = 0xBD;
		index += 1;
	}
	else if(index == 4)
	{
		P0 = 0x7E;
		PWMSTA = 0x7E;
		index = 1;
		runmode = 1;
	}
}

void ConfigTimer0(u16 ms)
{
	u32 tmp;
	
	tmp = 11059200 / 12;
	tmp = (tmp *ms) / 1000;
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

void ConfigTimer1()//������ɵ�10us��ʱ����ÿ10us��һ���жϣ�
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x10;		//���ö�ʱ��ģʽ
	TL1 = 0xF7;		//���ö�ʱ��ֵ
	TH1 = 0xFF;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;//ע������������ɵĺ�������û���������ģ�Ҫ�Լ����ϣ�������
}

void InterruptTimer0()interrupt 1
{
	static u16 tmr1 = 0;
	static u16 tmr2 = 0;
	static u16 tmr3 = 0;
	static u16 tmr4 = 0;
	static u16 tmr800ms = 0;
	static u8 tmr200ms = 0;
	
	TH0 = T0RH;
	TL0 = T0RL;
	tmr200ms++;
	KeyScan();
	LedScan();
	
	if(runmode == 1)
	{
		tmr1++;
		if(tmr1 >= (runtime[0] / 2))
		{
			tmr1 = 0;
			flagled1 = 1;
		}
	}
	
	if(runmode == 2)
	{
		tmr2++;
		if(tmr2 >= (runtime[1] / 2))
		{
			tmr2 = 0;
			flagled2 = 1;
		}
	}
	
	if(runmode == 3)
	{
		tmr3++;
		if(tmr3 >= (runtime[2] / 2))
		{
			tmr3 = 0;
			flagled3 = 1;
		}
	}
	
	if(runmode == 4)
	{
		tmr4++;
		if(tmr4 >= (runtime[3] / 2))
		{
			tmr4 = 0;
			flagled4 = 1;
		}
	}
	
	if(flagset)
	{
		tmr800ms++;
		if(tmr800ms >= 400)
		{
			tmr800ms = 0;
			numBlinkSta ^= 1;
		}
	}
	if(tmr200ms >= 100)
	{
		tmr200ms = 0;
		flag200ms = 1;
	}
}

void InterruptTimer1()interrupt 3
{
	static u16 PWMCnt = 0;//�洢�����жϵĴ���
	
	TL1 = 0xF7;		//���ö�ʱ��ֵ
	TH1 = 0xFF;		//���ö�ʱ��ֵ
	PWMCnt++;
	PWMCnt %= 101;//666�Ĳ�����ʵ����100���㣬������ʱ��Ϊ1ms����PWM����Ƶ��Ϊ1k����1ms��ʱ������С����hightime��ʱ�䣬����ʱ��������״̬��
	              //�����������֮ǰ��PWM����˼���ˣ�ֻ����֮ǰ���Ǹ��Ǹ��߼��Ĳ���������Ƶ�ʿɵ�
	              //����������ֱ���޶���Ƶ��	
	P2 = (P2 & 0x1F) | 0x80;
	if(PWMCnt >= hightime)//��������ж�ɨ���ʱ����ڵ��ڸߵ�ƽ������ʱ��
	{
		P0 = PWMSTA;//������Ӧ״̬�µ�led
	}
	else
	{
		P0 = 0xFF;//����ر����е�С��
	}

	P2 &= 0x1F;
}
//˵���ˣ�С����������ô�����أ������ں̵ܶ�ʱ���ڣ����ϵ��õƿ��͹أ������������Ӿ��Ͽ�������С�Ƶ����Ⱦ��Ƿ����仯��
u8 AdjustDC()//�жϸߵ�ƽ��ʱ�䣬ʵ�ֵ���rb2���Ƶ����ȷ����ı�
{
	u16 val;

	val = GetADCValue(3);
	if(val<60)
	{
		hightime = 90;
		brightness = 1;
	}
	else if(val <130)//���������ڵ���130�����򲻻���ʾ����ȼ������ȣ�
	{
		hightime = 70;
		brightness = 2;
	}
	else if(val <195)//����������190,emm��������Ǻ��ڵ���ʱ���ֵģ���̫��Ϊɶ
	{
		hightime = 30;
		brightness = 3;
	}
	else 
	{
		hightime = 5;
		brightness = 4;
	}
	return hightime;
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

void KeyAction(u8 keycode)
{
	u8 buff[4];//����һ�����������洢��С10�������ת���
	u8 i;
	if(keycode == '1')
	{
		if(flagstart == 0)
		{
			flagstart = 1;
		}
		else if(flagstart == 1)
		{
			flagstart = 0;
		}
	}
	else if(keycode  == '2')
	{
		if(flagset == 0)
		{
			flagset = 1;
			SetMode = 0;
		}
		else if(flagset == 1)
		{
			SetMode += 1;
			if(SetMode > 1)
			{
				flagset = 0;
				runmode = 1;
			}
		}
	}
	else if(keycode  == '3')
	{
		AddTime();
		for(i = 0; i<4; i++)
		{
			buff[i] = runtime[i] / 10;
		}
		E2Write(buff, 0x00, 4);//�������е�ֵд��E2PROM
		//��ע�����д������д���ַ��0x00��ʼ����Ϊ��4���ֽڵ����ݣ������ֽ���д4
	}
	else if(keycode  == '4')
	{
		SubTime();
		for(i = 0; i<4; i++)
		{
			buff[i] = runtime[i] / 10;
		}
		E2Write(buff, 0x00, 4);
	}
}

void SetTime()
{
	LedBuff[4] = 0xFF;//�رյ�5�������
	if(SetMode == 0)//����LED��ģʽ
	{
		if(numBlinkSta)
		{
			LedBuff[7] = 0xBF;//��8���������ʾ"-"
			LedBuff[5] = 0xBF;//��6���������ʾ"-"
			LedBuff[6] = LedChar[runmode];//��7���������ʾ����ģʽ
		}
		else
		{
			LedBuff[7] = 0xFF;
			LedBuff[5] = 0xFF;
			LedBuff[6] = 0xFF;
		}
		LedBuff[0] = LedChar[runtime[runmode-1] % 10];
		LedBuff[1] = LedChar[(runtime[runmode-1]/10) % 10];
		LedBuff[2] = LedChar[(runtime[runmode-1]/100) % 10];
		if(runtime[runmode-1] < 1000)
		{
			LedBuff[3] = 0xFF;
		}
		else
		{
			LedBuff[3] = LedChar[(runtime[runmode-1]/1000) % 10];
		}
	}
	else if(SetMode == 1)//������ת���
	{
		if(numBlinkSta)
		{
			LedBuff[0] = LedChar[runtime[runmode-1] % 10];
			LedBuff[1] = LedChar[(runtime[runmode-1]/10) % 10];
			LedBuff[2] = LedChar[(runtime[runmode-1]/100) % 10];
			if(runtime[runmode-1] < 1000)
			{
				LedBuff[3] = 0xFF;
			}
			else
			{
				LedBuff[3] = LedChar[(runtime[runmode-1]/1000) % 10];
			}
		}
		else
		{
			LedBuff[0] = 0xFF;
			LedBuff[1] = 0xFF;
			LedBuff[2] = 0xFF;
			LedBuff[3] = 0xFF;
		}
		LedBuff[7] = 0xBF;
		LedBuff[5] = 0xBF;
		LedBuff[6] = LedChar[runmode];
	}
}

//void SetTime()//������������
//{
//	u8 i;
//	if(SetMode == 0)
//	{
//		ShowNumber(runtime[runmode-1]);
//		if(numBlinkSta)
//		{
//			LedBuff[7] = 0xBF;
//			LedBuff[5] = 0xBF;
//			LedBuff[6] = LedChar[runmode];
//		}
//		else
//		{
//			LedBuff[7] = 0xFF;
//			LedBuff[6] = 0xFF;
//			LedBuff[5] = 0xFF;
//		}
//	}
//	else if(SetMode == 1)
//	{
//		LedBuff[7] = 0xBF;
//		LedBuff[5] = 0xBF;
//		LedBuff[6] = LedChar[runmode];
//		if(numBlinkSta)
//		{
//			ShowNumber(runtime[runmode-1]);
//		}
//		else
//		{
//			for(i = 0; i< 5;i++)
//			{
//				LedBuff[i] = 0xFF;
//			}
//		}
//	}
//}	

void AddTime()
{
	u8 i;
	if(SetMode == 0)
	{
		runmode+=1;
		if(runmode >4)
		{
			runmode = 1;
		}
	}
	else 
	{
		for(i = 0; i<4; i++)
		{
			if(runmode == i+1)
			{
				runtime[i] += 100;
				if(runtime[i] > 1200)
				{
					runtime[i] = 400;
				}
			}
		}
	}
}

void SubTime()
{
	u8 i;
	if(SetMode == 0)
	{
		runmode-=1;
		if(runmode < 1)
		{
			runmode = 4;
		}
	}
	else 
	{
		for(i = 0; i<4; i++)
		{
			if(runmode == i+1)
			{
				runtime[i] -= 100;
				if(runtime[i] < 400)
				{
					runtime[i] = 1200;
				}
			}
		}
	}
}
