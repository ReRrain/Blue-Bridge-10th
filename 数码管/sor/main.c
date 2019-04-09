/*��̬��ʾ*/
//#include<stc15.h>

//sbit buzz = P0^6;
//sbit relay = P0^4;

//void main()
//{
//	P2 = (P2 & 0x1F) | 0xA0;//�رշ������ͼ̵���
//	buzz = 0;
//	relay = 0;
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xC0;//λѡ
//	P0 = (P0 & 0x1F) | 0x01;//ѡ��com1
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xE0;//��ѡ
//	P0 = 0xA4;//��ʾ����1
//	P2 = P2 & 0x1F; 	
//	
//	while(1);
//}
///*��̬��ʾ*/
//#include<stc15.h>
// 
//typedef unsigned char u8;
//typedef unsigned int u16;

//sbit buzz = P0^6;
//sbit relay = P0^4;

//u8 code led[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
//void main()
//{
//	u16 i;//�������붨���ں�����ʼ��λ��
//	u8 count = 0;//��������������������
//	
//	P2 = (P2 & 0x1F) | 0xA0;//�ص��������ͼ̵���
//	P0 = 0xAF;
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xC0;//λѡ
//	P0 = 0x01;
//	P2 = P2 & 0x1F;
//	
//	while(1)
//	{
//		
//		P2 = (P2 & 0x1F) | 0xE0;//��ѡ
//		P0 = led[count++];
//		if(count > 0x0F)//ע�����ֵ�Ǵ���15
//			count = 0x00;
//		for(i = 0;i<60000;i++);
//		P2 = P2 & 0x1F;
//	}
//}
/*��λΪ0����ʾ����0~9999999���ö�ʱ������1s����*/
#include<stc15.h>
 
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

u8 TORH;
u8 TORL;
u16 flag1s = 0;

u8 Ledchar[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
u8 LedBuff[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//����ܻ�������ȷ����ʱȫ������ʾ

void CloseOther();
void LedScan();
void ShowNumber(u32 dat);
void ConfigTimer0 (u16 ms);

void main()
{
	u32 sec = 0;//sec��ʾ�����˶೤ʱ�䣨�룩
	CloseOther();
	EA = 1;//���ܿ���
	ConfigTimer0(1);
	ShowNumber(0);
	
	while(1)//��ѭ��ÿ��һ��ִ��һ��
	{
		if(flag1s)//���flag1sΪ1�������ѭ����������1�룩
		{
			flag1s = 0;
			sec++;
			if(sec > 99999999)
				sec = 0;
			else
				ShowNumber(sec); 
		}
	}
}

void CloseOther()
{
	P2 = (P2 & 0x1F) | 0x80;//�رհ˸�led
	P0 = 0xFF;
	P2 = P2 & 0x1F;
	
	P2 = (P2 & 0x1F) | 0xA0;//�رշ������ͼ̵���
	P0 = 0xAF;
	P2 = P2 & 0x1F;
}
 
void ConfigTimer0(u16 ms)//��ʱʱ��Ϊ1ms
{
    u32 tmp;
	tmp = 11059200 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;
	TORH = (u8)(tmp>>8);//��tmp��ֵ�ĵͰ�λ��TORH
	TORL  = (u8)tmp;//��tmp��ֵ�ĵͰ�λ��TORL
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = TORH;
	TL0 = TORL;
	ET0 = 1;
	TR0 = 1;
}

void interruptTimer0()interrupt 1//��ʱ��0��Ӧ���жϺ�Ϊ1
{
	static u16 tmr1s = 0;
	TH0 = TORH;
	TL0 = TORL;
	LedScan();
	tmr1s++; 
	if(tmr1s >=1000)//��ʱ1000�Σ�����ʱ1�룩
	{
		flag1s = 1;
		tmr1s = 0;
	}
}

void ShowNumber(u32 dat)
{
	char i;//u8���޷�������ִ��i--����ʱi��ҪΪ�з�����
	u8 buff[8];
	for(i = 0;i<8;i++)
	{
	    buff[i] = dat % 10;
		dat = dat / 10;
	}
	for(i = 7;i>0;i--)//���λ������ܱ������������Բ���Ϊ0
	{
		if(buff[i] == 0)
			LedBuff[i] = 0xFF;
		else
           break;
	}
	for(;i>=0;i--)
	{
		LedBuff[i] = Ledchar[buff[i]];
	}
}

void LedScan()
{
	static u8 index = 0;
	P2 = (P2 & 0x1F) | 0xE0;//������ȷ��ÿ������ܶ�ѡ��ʱ������ʾ
	P0 = 0xFF;
	P2 &= 0x1F;
	
	P2 = (P2 & 0x1F) | 0xC0;//λѡ
	P0 = 0x80 >> index;//ѡ��com8
	P2 = P2 & 0x1F;
	
	P2 = (P2 & 0x1F) | 0xE0;
	P0 = LedBuff[index];
	P2 = P2 & 0x1F;
	
	index++;
	if(index>=7)
		index = 0;
}