#include "config.h"
#include "eeprom.h"
#include "key.h"
#include "led.h"
#include "i2.h"
#include "pcf8591.h"

u8 T0RH;
u8 T0RL;
u8 flag200ms = 0;
u8 hightime = 0;//存储高电平持续的时间
u16 PWMSTA = 0xFF;//这里注意要给PWMSTA赋初值为0xFF，
//我们是用PWMSTA存储P0口的状态的，上电的时候确保灯是关闭状态

u8 flagled1 = 0;
u8 flagled2 = 0;
u8 flagled3 = 0;
u8 flagled4 = 0;
u8 flagstart = 0;//0-停止LED流转，1-开始LED流转
u8 flagset = 0;//1-设置模式，0-非设置模式
u8 runmode = 1;//运行模式编号
u16 runtime[4] = {400, 400, 400, 400};//四个模式下的闪烁间隔时间（类型错误）
u8 brightness = 0;//亮度等级
u8 numBlinkSta = 1;//0-数码管灭，1-数码管亮
u8 SetMode = 0;//0-闪烁LED的模式，1-闪烁流转间隔

void CloseOther();
void ConfigTimer0(u16 ms);//定时
void ConfigTimer1();//定时
void LedMode1();
void LedMode2();
void LedMode3();
void LedMode4();
void SetTime();
void AddTime();
void SubTime();
u8 AdjustDC();//判断高电平的时间

void main()
{
	u8 i;
	u8 runtimebuff[4];
	
	E2Read(runtimebuff, 0x00, 4);//一个字节是1个u8，E2Prom的地址范围是0x00到0xFF（数组的名字就是地址，不需要再取地址符）
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
	PWMSTA = ~shift1;//不要直接操作P0口！！！！
	
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

void ConfigTimer1()//软件生成的10us定时器（每10us进一次中断）
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0xF7;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;//注意这里软件生成的函数里是没有这条语句的，要自己加上！！！！
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
	static u16 PWMCnt = 0;//存储进入中断的次数
	
	TL1 = 0xF7;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	PWMCnt++;
	PWMCnt %= 101;//666的操作，实现满100清零，设置总时间为1ms，即PWM波的频率为1k（在1ms的时间里让小灯灭hightime的时间，其余时间是亮的状态）
	              //这里就体现了之前的PWM波的思想了，只不过之前的那个是更高级的操作，就是频率可调
	              //这里我们是直接限定了频率	
	P2 = (P2 & 0x1F) | 0x80;
	if(PWMCnt >= hightime)//如果进入中断扫描的时间大于等于高电平持续的时间
	{
		P0 = PWMSTA;//点亮对应状态下的led
	}
	else
	{
		P0 = 0xFF;//否则关闭所有的小灯
	}

	P2 &= 0x1F;
}
//说白了，小灯亮度是怎么调的呢？就是在很短的时间内，不断地让灯开和关，这样我们在视觉上看起来，小灯的亮度就是发生变化的
u8 AdjustDC()//判断高电平的时间，实现调节rb2，灯的亮度发生改变
{
	u16 val;

	val = GetADCValue(3);
	if(val<60)
	{
		hightime = 90;
		brightness = 1;
	}
	else if(val <130)//这里必须大于等于130（否则不会显示这个等级的亮度）
	{
		hightime = 70;
		brightness = 2;
	}
	else if(val <195)//这里必须大于190,emm这个数据是后期调的时候发现的，不太懂为啥
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
	u8 buff[4];//定义一个数组用来存储缩小10倍后的流转间隔
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
		E2Write(buff, 0x00, 4);//将数组中的值写入E2PROM
		//（注意这个写法），写入地址从0x00开始，因为是4个字节的数据，所以字节数写4
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
	LedBuff[4] = 0xFF;//关闭第5个数码管
	if(SetMode == 0)//设置LED的模式
	{
		if(numBlinkSta)
		{
			LedBuff[7] = 0xBF;//第8个数码管显示"-"
			LedBuff[5] = 0xBF;//第6个数码管显示"-"
			LedBuff[6] = LedChar[runmode];//第7个数码管显示运行模式
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
	else if(SetMode == 1)//设置流转间隔
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

//void SetTime()//这个方法会出错
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
