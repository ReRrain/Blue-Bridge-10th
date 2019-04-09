///*��������*/
//#include<stc15.h>

//typedef unsigned char u8;
//typedef unsigned int u16;
//typedef unsigned long u32;

//u8 LedChar[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
//u8 LedBuff[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//u8 Keysta[] = {1,1,1,1};//���浱ǰֵ
//u8 KeycodeMap[] = {'1','2','3','4'};//�䵱�����ַ�

//u8 T0RH;
//u8 T0RL;

//sbit key_in_1 = P3^3;
//sbit key_in_2 = P3^2;
//sbit key_in_3 = P3^1;
//sbit key_in_4 = P3^0;

//void CloseOther();
//void LedScan();
//void ShowNumber(u32 dat);
//void ConfigTimer0(u16 ms);
//void KeyScan();//����ɨ�裬���жϺ����ڲ�ִ�У����ö�ʱ���ж�����
//void KeyDriver();//�жϰ����Ƿ��£������������ʾ��ֵ
//void KeyAction(u8 keycode);//�ж����ĸ��������£���Ӧ���������������ʾ��ֵ

//void main()
//{
//	CloseOther();
//	EA = 1;
//	ConfigTimer0(2);
//	ShowNumber(0);
//	
//	while(1)
//	{
//		KeyDriver();
//	}
//}

//void CloseOther()
//{
//	P2 = (P2 & 0x1F) | 0x80;
//	P0 = 0xFF;
//	P2 &= 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xA0;
//	P0 = 0xAF;
//	P2 &= 0x1F;
//}

//void KeyAction(u8 keycode)//�ж����ĸ��������£���Ӧ���������������ʾ��ֵ
//{
//	static u8 cnt = 0;//��Ҫʵ������һ�εĻ����ϼӷ������Զ����static����������������
//	if(keycode == '1')
//	{
//		cnt += 1;
//		ShowNumber(cnt);
//	}
//	if(keycode == '2')
//	{
//		cnt += 2;
//		ShowNumber(cnt);
//	}
//	if(keycode == '3')
//	{
//		cnt += 3;
//		ShowNumber(cnt);
//	}
//	if(keycode == '4')
//	{
//		cnt += 4;
//		ShowNumber(cnt);
//	}
//}

//void KeyDriver()
//{
//	u8 i;
//	static u8 backup[4] = {1,1,1,1};//ע������Ҫ�þ�̬�����������������ǰ��ֵ��ÿ����һ��ѭ����Ҫ����һ����ֵ
//	
//	for(i = 0;i<4;i++)
//	{
//		if(Keysta[i] != backup[i])//��ǰֵ��ǰ��ֵ����ȣ�˵����ʱ����״̬�����仯
//		{
//			if(backup[i] == 1)//˵����ʱ��������
//			{
//				KeyAction(KeycodeMap[i]);
//			}
//			backup[i] = Keysta[i];//�õ�ǰֵ����ǰ��ֵ
//		}
//	}
//}

//void KeyScan()                              
//{
//	u8 i;
//	static u8 keybuff[4] = {0xFF,0xFF,0xFF,0xFF};
//	keybuff[0] = (keybuff[0] << 1) | key_in_1;
//	keybuff[1] = (keybuff[1] << 1) | key_in_2;
//	keybuff[2] = (keybuff[2] << 1) | key_in_3;
//	keybuff[3] = (keybuff[3] << 1) | key_in_4;
//	
//	for(i = 0;i<4;i++)
//	{
//		if(keybuff[i] == 0xFF)
//			Keysta[i] = 1;
//		else if(keybuff[i] == 0x00)//��������д��i��������������������������
//			Keysta[i] = 0;
//		else
//		{}
//	}
//}

//void configTimer0(u16 ms)
//{
//	u32 tmp;//tmpҪ����Ϊ32λ!!!!!!!
//	tmp = 11059200 / 12;
//	tmp = (tmp * ms) / 1000;
//	tmp = 65536 - tmp;//���ﲻҪ����!!!!!��65536������������
//	T0RH = (u8)(tmp>>8);//(u8)ǿ��ת����8λ��ֻȡ�ӵ�λ���𹻰�λ��ֹͣ����tmp�ĸ߰�λ��T0RH������Ҫ���ư�λ
//	T0RL = (u8)tmp;
//	TMOD &= 0xF0;
//	TMOD |= 0x01;
//	TH0 = T0RH;
//	TL0 = T0RL;
//	TR0 = 1;
//	ET0 = 1;
//}

//void ShowNumber(u32 dat)
//{
//	char i;//!!!!!!!!!!!!
//	u8 buff[8];
//	
//	for(i = 0;i<8;i++)
//	{
//		buff[i] = dat % 10;
//		dat = dat / 10;
//	}
//	for(i = 7;i>0;i--)
//	{
//		if(buff[i] == 0)
//			LedBuff[i] = 0xFF;
//		else
//			break;
//	}
//	for(;i>=0;i--)//��������i�Ǹ������������i���붨��Ϊchar����!!!!!!!!!1
//	{
//		LedBuff[i] = LedChar[buff[i]];
//	}
//}

//void LedScan()
//{
//	static u8 index = 0;
//	
//	P2 = (P2 & 0x1F) | 0xE0;
//	P0  = 0xFF;//��������ܣ��͵�ƽ����ܵ���
//	P2 &= 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xC0;
//	P0 = 0x80>>index;
//	P2 &= 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xE0;
//	P0 = LedBuff[index];
//	P2 &= 0x1F;
//	
//	index++;
//	if(index >= 8)
//		index = 0;
//}

//void InterruptTimer0() interrupt 1
//{
//	TH0 = T0RH;
//	TL0 = T0RL;
//	
//	LedScan();
//	KeyScan();
//}
//���󰴼�
//#include<stc15.h>

//typedef unsigned char u8;
//typedef unsigned int u16;
//typedef unsigned long u32;

//u8 LedChar[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
//u8 LedBuff[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//u8 Keysta[4][4] = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
//u8 KeyCodeMap[4][4]= {{'7','8','9','F'},{'4','5','6','F'},{'1','2','3','F'},{'0','F','F','F'}};

//u8 T0RH;
//u8 T0RL;

//sbit key_out_1 = P3^0;
//sbit key_out_2 = P3^1;
//sbit key_out_3 = P3^2;
//sbit key_out_4 = P3^3;
//sbit key_in_1 = P4^4;
//sbit key_in_2 = P4^2;
//sbit key_in_3 = P3^5;
//sbit key_in_4 = P3^4;

//void CloseOther();
//void ConfigTimer0(u16 ms);
//void KeyDriver();
//void ShowNumber(u32 dat);

//void main()
//{
//	CloseOther();
//	EA = 1;
//	ConfigTimer0(1);
//	ShowNumber(0);
//	
//	while(1)
//	{
//		KeyDriver();
//	}
//}

//void CloseOther()
//{
//	P2 = (P2 & 0x1F) | 0x80;
//	P0 = 0xFF;
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xA0;
//	P0 = 0xAF;
//	P2 &= 0x1F;
//}

//void KeyAction(u8 keycode)
//{
//	if(keycode >= '0' && keycode <= '9')
//		ShowNumber(keycode-'0');
//	else
//		LedBuff[0] = LedChar[15];
//}

//void KeyScan()
//{
//	u8 i;
//	static u8 keyout = 0;
//	static u8 keybuff[4][4] = {
//	{0xFF,0xFF,0xFF,0xFF},{0xFF,0xFF,0xFF,0xFF},{0xFF,0xFF,0xFf,0xFF},{0xFF,0xFF,0xFF,0xFF}
//	};

//	keybuff[keyout][0] = (keybuff[keyout][0] << 1) | key_in_1;
//	keybuff[keyout][1] = (keybuff[keyout][1] << 1) | key_in_2;
//	keybuff[keyout][2] = (keybuff[keyout][2] << 1) | key_in_3;
//	keybuff[keyout][3] = (keybuff[keyout][3] << 1) | key_in_4;
//	
//	for(i = 0;i<4;i++)
//	{
//		if((keybuff[keyout][i] & 0x0F) == 0x0F)
//			Keysta[keyout][i] = 1;
//		else if ((keybuff[keyout][i] & 0x0F) == 0x00)
//			Keysta[keyout][i] = 0;
//		else
//		{}
//	}
//	keyout++;
//	if(keyout>3)
//		keyout = 0;	
//	
//	switch(keyout)
//	{
//		case 0: key_out_1 = 0;key_out_4 = 1;break;
//		case 1: key_out_2 = 0;key_out_1 = 1;break;
//		case 2: key_out_3 = 0;key_out_2 = 1;break;
//		case 3: key_out_4 = 0;key_out_3 = 1;break;
//		default: break;
//	}	
//}

//void KeyDriver()
//{
//	u8 i,j;
//	static u8 backup[4][4] = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
//	for(i = 0;i<4;i++)
//	{
//		for(j = 0;j<4;j++)
//		{
//			if(Keysta[i][j] != backup[i][j])
//			{
//				if(backup[i][j] == 0)
//				{
//					KeyAction(KeyCodeMap[i][j]);
//				}
//                backup[i][j] = Keysta[i][j];
//			}
//		}
//	}	  
//}

//void ConfigTimer0(u16 ms)
//{
//	u32 tmp;
//	tmp = 11059200 / 12;
//	tmp = (tmp * ms) / 1000;
//	tmp = 65536 - tmp;
//	T0RH = (u8)(tmp>>8);
//	T0RL = (u8)tmp;
//	TMOD &= 0xF0;
//	TMOD |= 0x01;
//	TH0 = T0RH;
//	TL0 = T0RL;
//	ET0 = 1;
//	TR0 = 1;
//}

//void LedScan()
//{
//	static u8 index = 0;
//	
//	P2 = (P2 & 0x1F) | 0xE0;
//	P0 = 0xFF;
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xC0;
//	P0 = 0x80 >> index;
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xE0;
//	P0 = LedBuff[index];
//	P2 &= 0x1F;
//	
//	index++;
//	if(index>7)
//		index = 0;
//}

//void ShowNumber(u32 dat)
//{
//	char i;
//	u8 buff[8];
//	
//	for(i = 0;i<7;i++)
//	{
//		buff[i] = dat % 10;
//	    dat /= 10;
//	}
//	for(i = 7;i>0;i--)
//	{
//		if(buff[i] == 0)
//			LedBuff[i] = 0xFF;
//		else
//			break;
//	}
//	for(;i>=0;i--)
//	{
//		LedBuff[i] = LedChar[buff[i]];
//	}
//}

//void InterruptTimer0() interrupt 1
//{
//	TH0 = T0RH;
//	TL0 = T0RL;
//	
//	LedScan();
//	KeyScan();
//}
//������
#include<stc15.h>

typedef unsigned char u8;//0-255
typedef unsigned int u16;//0-65535
typedef unsigned long u32;

u8 LedChar[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};//�洢�������ʾֵ��0-F��
u8 LedBuff[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//���������洢�����ʵ����ʾ������
u8 Keysta[4] = {1,1,1,1};//�洢������ǰ״̬��������ǰ״̬����һ�ε�״̬����ȣ���˵������״̬�����仯��
u8 KeyCodeMap[4] = {'1','2','3','4'};
u16 KeyDownTime[4] = {0,0,0,0};//�������µ�ʱ�䣬ע������ҲҪ��u16!!!!!!!!!!!!

u8 T0RH;
u8 T0RL;

sbit key_in_1 = P3^3;
sbit key_in_2 = P3^2;
sbit key_in_3 = P3^1;
sbit key_in_4 = P3^0;

void CloseOther();
void KeyDriver();
void ConfigTimer0(u16 ms);
void ShowNumber(u32 dat);

void main()
{
	CloseOther();
	EA = 1;
	ConfigTimer0(2);
	ShowNumber(0);
	
	while(1)
	{
		KeyDriver();
	}
}

void CloseOther()//�ر��޹����裬��LED�����������̵�����
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
	static cnt = 0;
	if(keycode == '1')
	{
		cnt += 1;
		ShowNumber(cnt);
	}
	if(keycode == '2')
	{
		cnt += 2;
		ShowNumber(cnt);
	}
	if(keycode == '3')
	{
		cnt += 3;
		ShowNumber(cnt);
	}
	if(keycode == '4')
	{
		cnt += 4;
		ShowNumber(cnt);
	}
}

void KeyDriver()
{
	u8 i;
	static u8 backup[4] = {1,1,1,1};//�洢�������һ�ε�״̬��ע��Ҫ����Ϊ��̬��������Ϊ״ֵ̬ÿһ�ΰ������¶���Ҫ����
	static u16 TimeMax[4] = {1000,1000,1000,1000};//ע������Ҫ����Ϊu16����Ȼ�������!!!!!!
	for(i = 0;i<4;i++)
	{
		if(Keysta[i] != backup[i])//��������״̬�����ı�
		{
			if(backup[i] == 1)//�����һ�ε�״̬Ϊ1��˵����ǰ״̬Ϊ0��Ϊ0�ͱ�ʾ���������£�
			{
				KeyAction(KeyCodeMap[i]);//ִ����Ӧ�����Ĺ���
			}
			backup[i] = Keysta[i];//���°���״̬
		}
		if(KeyDownTime[i]>0)//��������ʱ�����2ms
		{
			if(KeyDownTime[i] > TimeMax[i])//��������ʱ�������󰴼�ʱ�䣬˵�����ڳ���״̬
			{
				KeyAction(KeyCodeMap[i]);//ִ����Ӧ�����Ĺ���
				TimeMax[i] += 200;//���ӳ������ж�ʱ��
			}
		}
		else//Ҫ����ifѭ����,���ǵ�KeyDownTime[i]<=0������δ���£������
		{
			TimeMax[i] = 1000;
		}
	}
}

void KeyScan()//����
{
	u8 i;
	static u8 keybuff[4] = {0xFF,0xFF,0xFF,0xFF};
	
	keybuff[0] = (keybuff[0] << 1) | key_in_1;
	keybuff[1] = (keybuff[1] << 1) | key_in_2;
	keybuff[2] = (keybuff[2] << 1) | key_in_3;
	keybuff[3] = (keybuff[3] << 1) | key_in_4;
	
	for(i = 0;i<4;i++)
	{
		if(keybuff[i] == 0xFF)//ɨ��8�Σ�8λ��Ϊ1
		{
			Keysta[i] = 1;//˵����ǰ������û�б�����
			KeyDownTime[i] = 0;//����ע��ҲҪ���ǵ�����������������û�а���ʱ��keydowntime[i]��Ϊ0��
		}
		else if(keybuff[i] == 0x00)//ɨ��8�Σ�8λ��Ϊ0
		{
			Keysta[i] = 0;//˵������������
			KeyDownTime[i] += 4;//��������ʱ��ÿ����2ms��KeyDownTime��+4
		}
		else
		{}
	}
}

void ConfigTimer0(u16 ms)
{
	u32 tmp;
	
	tmp = 11059200 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;
	T0RH = (u8)(tmp>>8);
	T0RL = (u8)tmp;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}

void LedScan()//�����ɨ�裬��ɨ��߸�ֵ
{
	static u8 index = 0;
	
	P2 = (P2 & 0x1F) | 0xE0;//��������ܶ�ѡ
	P0 = 0xFF;//�ر����������
	P2 = P2 & 0x1F;
	
	P2 =(P2 & 0x1F) | 0xC0;//����λѡ
	P0 = 0x80 >> index;//��ѡ��COM1������ÿɨ��һ�Σ�����һλ��ֱ��ѡ��8��COM�ڣ���8������ܣ�
	P2 = P2 & 0x1F;
	
	P2 = (P2 & 0x1F) | 0xE0;//������ѡ
	P0 = LedBuff[index];//���θ�ÿһ������ܸ�ֵ
	P2 = P2 & 0x1F;
	
	index++;
	if(index > 7)
		index = 0;
}

void ShowNumber(u32 dat)
{
	char i;
	u8 buff[8];
	
	for(i = 0;i<7;i++)
	{
		buff[i] = dat % 10;
		dat /= 10;
	}
	for(i = 7;i>0;i--)//
	{
		if(buff[i] == 0)
			LedBuff[i] = 0xFF;
		else
			break;
	} 
	for(;i>=0;i--)
	{
		LedBuff[i] = LedChar[buff[i]];
	}
}

void InterruptTimer0() interrupt 1
{
	TH0 = T0RH;
	TL0 = T0RL;
	
	LedScan();
	KeyScan();
}