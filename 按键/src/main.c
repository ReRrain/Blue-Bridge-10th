///*独立按键*/
//#include<stc15.h>

//typedef unsigned char u8;
//typedef unsigned int u16;
//typedef unsigned long u32;

//u8 LedChar[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
//u8 LedBuff[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//u8 Keysta[] = {1,1,1,1};//保存当前值
//u8 KeycodeMap[] = {'1','2','3','4'};//充当索引字符

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
//void KeyScan();//按键扫描，在中断函数内部执行，利用定时器中断消抖
//void KeyDriver();//判断按键是否按下，如果按下则显示数值
//void KeyAction(u8 keycode);//判断是哪个按键按下，对应按键在数码管上显示数值

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

//void KeyAction(u8 keycode)//判断是哪个按键按下，对应按键在数码管上显示数值
//{
//	static u8 cnt = 0;//需要实现在上一次的基础上加法，所以定义成static！！！！！！！！
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
//	static u8 backup[4] = {1,1,1,1};//注意这里要用静态变量，数组用来存放前次值，每进入一次循环需要更改一次数值
//	
//	for(i = 0;i<4;i++)
//	{
//		if(Keysta[i] != backup[i])//当前值与前次值不相等，说明此时按键状态发生变化
//		{
//			if(backup[i] == 1)//说明此时按键按下
//			{
//				KeyAction(KeycodeMap[i]);
//			}
//			backup[i] = Keysta[i];//用当前值更新前次值
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
//		else if(keybuff[i] == 0x00)//这里忘记写【i】了啦啦啦啦啦。。。。。。
//			Keysta[i] = 0;
//		else
//		{}
//	}
//}

//void configTimer0(u16 ms)
//{
//	u32 tmp;//tmp要定义为32位!!!!!!!
//	tmp = 11059200 / 12;
//	tmp = (tmp * ms) / 1000;
//	tmp = 65536 - tmp;//这里不要忘记!!!!!用65536减！！！！！
//	T0RH = (u8)(tmp>>8);//(u8)强制转换成8位，只取从低位数起够八位即停止，把tmp的高八位给T0RH，故需要右移八位
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
//	for(;i>=0;i--)//这里会出现i是负数的情况，故i必须定义为char类型!!!!!!!!!1
//	{
//		LedBuff[i] = LedChar[buff[i]];
//	}
//}

//void LedScan()
//{
//	static u8 index = 0;
//	
//	P2 = (P2 & 0x1F) | 0xE0;
//	P0  = 0xFF;//共阳数码管，低电平数码管点亮
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
//矩阵按键
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
//长按键
#include<stc15.h>

typedef unsigned char u8;//0-255
typedef unsigned int u16;//0-65535
typedef unsigned long u32;

u8 LedChar[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};//存储数码管显示值”0-F“
u8 LedBuff[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//缓冲区，存储数码管实际显示的数据
u8 Keysta[4] = {1,1,1,1};//存储按键当前状态，（若当前状态与上一次的状态不相等，则说明按键状态发生变化）
u8 KeyCodeMap[4] = {'1','2','3','4'};
u16 KeyDownTime[4] = {0,0,0,0};//按键按下的时间，注意这里也要用u16!!!!!!!!!!!!

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

void CloseOther()//关闭无关外设，（LED，蜂鸣器，继电器）
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
	static u8 backup[4] = {1,1,1,1};//存储数码管上一次的状态，注意要定义为静态变量，因为状态值每一次按键按下都需要更新
	static u16 TimeMax[4] = {1000,1000,1000,1000};//注意这里要定义为u16，不然就溢出了!!!!!!
	for(i = 0;i<4;i++)
	{
		if(Keysta[i] != backup[i])//表明按键状态发生改变
		{
			if(backup[i] == 1)//如果上一次的状态为1，说明当前状态为0（为0就表示按键被按下）
			{
				KeyAction(KeyCodeMap[i]);//执行相应按键的功能
			}
			backup[i] = Keysta[i];//更新按键状态
		}
		if(KeyDownTime[i]>0)//按键按下时间大于2ms
		{
			if(KeyDownTime[i] > TimeMax[i])//按键按下时间大于最大按键时间，说明处于长按状态
			{
				KeyAction(KeyCodeMap[i]);//执行相应按键的功能
				TimeMax[i] += 200;//增加长按键判断时间
			}
		}
		else//要加在if循环外,考虑到KeyDownTime[i]<=0（按键未按下）的情况
		{
			TimeMax[i] = 1000;
		}
	}
}

void KeyScan()//消抖
{
	u8 i;
	static u8 keybuff[4] = {0xFF,0xFF,0xFF,0xFF};
	
	keybuff[0] = (keybuff[0] << 1) | key_in_1;
	keybuff[1] = (keybuff[1] << 1) | key_in_2;
	keybuff[2] = (keybuff[2] << 1) | key_in_3;
	keybuff[3] = (keybuff[3] << 1) | key_in_4;
	
	for(i = 0;i<4;i++)
	{
		if(keybuff[i] == 0xFF)//扫描8次，8位都为1
		{
			Keysta[i] = 1;//说明当前按键并没有被按下
			KeyDownTime[i] = 0;//这里注意也要考虑到！！！！！当按键没有按下时，keydowntime[i]是为0的
		}
		else if(keybuff[i] == 0x00)//扫描8次，8位都为0
		{
			Keysta[i] = 0;//说明按键被按下
			KeyDownTime[i] += 4;//按键按下时间每增加2ms，KeyDownTime就+4
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

void LedScan()//数码管扫描，边扫描边赋值
{
	static u8 index = 0;
	
	P2 = (P2 & 0x1F) | 0xE0;//开启数码管段选
	P0 = 0xFF;//关闭所有数码管
	P2 = P2 & 0x1F;
	
	P2 =(P2 & 0x1F) | 0xC0;//开启位选
	P0 = 0x80 >> index;//先选中COM1，而后每扫描一次，右移一位，直到选完8个COM口（即8个数码管）
	P2 = P2 & 0x1F;
	
	P2 = (P2 & 0x1F) | 0xE0;//开启段选
	P0 = LedBuff[index];//依次给每一个数码管赋值
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