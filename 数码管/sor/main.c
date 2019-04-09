/*静态显示*/
//#include<stc15.h>

//sbit buzz = P0^6;
//sbit relay = P0^4;

//void main()
//{
//	P2 = (P2 & 0x1F) | 0xA0;//关闭蜂鸣器和继电器
//	buzz = 0;
//	relay = 0;
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xC0;//位选
//	P0 = (P0 & 0x1F) | 0x01;//选中com1
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xE0;//段选
//	P0 = 0xA4;//显示数字1
//	P2 = P2 & 0x1F; 	
//	
//	while(1);
//}
///*动态显示*/
//#include<stc15.h>
// 
//typedef unsigned char u8;
//typedef unsigned int u16;

//sbit buzz = P0^6;
//sbit relay = P0^4;

//u8 code led[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
//void main()
//{
//	u16 i;//变量必须定义在函数开始的位置
//	u8 count = 0;//。。。。。。。。。。
//	
//	P2 = (P2 & 0x1F) | 0xA0;//关掉蜂鸣器和继电器
//	P0 = 0xAF;
//	P2 = P2 & 0x1F;
//	
//	P2 = (P2 & 0x1F) | 0xC0;//位选
//	P0 = 0x01;
//	P2 = P2 & 0x1F;
//	
//	while(1)
//	{
//		
//		P2 = (P2 & 0x1F) | 0xE0;//段选
//		P0 = led[count++];
//		if(count > 0x0F)//注意计数值是大于15
//			count = 0x00;
//		for(i = 0;i<60000;i++);
//		P2 = P2 & 0x1F;
//	}
//}
/*高位为0则不显示，从0~9999999利用定时器进行1s计数*/
#include<stc15.h>
 
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

u8 TORH;
u8 TORL;
u16 flag1s = 0;

u8 Ledchar[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
u8 LedBuff[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//数码管缓冲区，确保打开时全部不显示

void CloseOther();
void LedScan();
void ShowNumber(u32 dat);
void ConfigTimer0 (u16 ms);

void main()
{
	u32 sec = 0;//sec表示经过了多长时间（秒）
	CloseOther();
	EA = 1;//打开总开关
	ConfigTimer0(1);
	ShowNumber(0);
	
	while(1)//此循环每过一秒执行一次
	{
		if(flag1s)//如果flag1s为1，则进入循环（即计数1秒）
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
	P2 = (P2 & 0x1F) | 0x80;//关闭八个led
	P0 = 0xFF;
	P2 = P2 & 0x1F;
	
	P2 = (P2 & 0x1F) | 0xA0;//关闭蜂鸣器和继电器
	P0 = 0xAF;
	P2 = P2 & 0x1F;
}
 
void ConfigTimer0(u16 ms)//计时时间为1ms
{
    u32 tmp;
	tmp = 11059200 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;
	TORH = (u8)(tmp>>8);//将tmp数值的低八位给TORH
	TORL  = (u8)tmp;//将tmp数值的低八位给TORL
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = TORH;
	TL0 = TORL;
	ET0 = 1;
	TR0 = 1;
}

void interruptTimer0()interrupt 1//定时器0对应的中断号为1
{
	static u16 tmr1s = 0;
	TH0 = TORH;
	TL0 = TORL;
	LedScan();
	tmr1s++; 
	if(tmr1s >=1000)//计时1000次（即计时1秒）
	{
		flag1s = 1;
		tmr1s = 0;
	}
}

void ShowNumber(u32 dat)
{
	char i;//u8是无符号数，执行i--操作时i需要为有符号数
	u8 buff[8];
	for(i = 0;i<8;i++)
	{
	    buff[i] = dat % 10;
		dat = dat / 10;
	}
	for(i = 7;i>0;i--)//最低位的数码管必须有数，所以不能为0
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
	P2 = (P2 & 0x1F) | 0xE0;//消隐，确保每个数码管段选打开时均不显示
	P0 = 0xFF;
	P2 &= 0x1F;
	
	P2 = (P2 & 0x1F) | 0xC0;//位选
	P0 = 0x80 >> index;//选定com8
	P2 = P2 & 0x1F;
	
	P2 = (P2 & 0x1F) | 0xE0;
	P0 = LedBuff[index];
	P2 = P2 & 0x1F;
	
	index++;
	if(index>=7)
		index = 0;
}