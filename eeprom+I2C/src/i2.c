#include "config.h"
#include <intrins.h>

#define I2CDelay() Delay5us()

void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 11;
	while (--i);
}



void I2CStart()
{
	I2C_SCL = 1;
	I2C_SDA = 1;
	I2CDelay();
	I2C_SDA = 0;
	I2CDelay();
	I2C_SCL = 0;
	I2CDelay();
}

void I2CStop()
{
	I2C_SCL = 0;
	I2C_SDA = 0;
	I2CDelay();
	I2C_SCL = 1;
	I2CDelay();
	I2C_SDA = 1;
	I2CDelay();
//	I2C_SCL = 0;//停止后scl不必再置0
}

bit I2CWrite(u8 dat)//注意这里函数有返回值，故不能定义为void形式
{
	bit ack;
	u8 mask;
	
	for(mask = 0x80; mask != 0x00; mask >>= 1)
	{
		if((mask & dat) == 0)//运算符优先级的问题！！！！！
			I2C_SDA = 0;
		else
			I2C_SDA = 1;
		I2CDelay();
		I2C_SCL = 1;
		I2CDelay();
		I2C_SCL = 0;
		I2CDelay();
	}
	I2C_SDA = 1;
	I2CDelay();
	I2C_SCL = 1;
	ack = I2C_SDA;
	I2CDelay();
	I2C_SCL= 0;
	I2CDelay();
	
	return(~ack);
}

u8 I2CReadACK()
{
	u8 mask;
	u8 dat;
	
	I2C_SDA = 1;//只有在sda为高电平时主机才能进行读操作
	I2CDelay();//这里不要忘记加延时函数
	
	for(mask = 0x80; mask != 0x00; mask >>= 1)
	{
		I2C_SCL = 1;//scl在高电平期间才能读
		I2CDelay();
		
		if(I2C_SDA == 0)
			dat &= ~mask;
		else
			dat |= mask;
	    I2CDelay();
		I2C_SCL = 0;//拉低scl，使从机发送出下一位
		I2CDelay();
	}
	I2C_SDA = 0;                                                                               
	I2CDelay();
	I2C_SCL = 1;
	I2CDelay();
	I2C_SCL = 0;
	I2CDelay();
	
	return dat;
}

u8 I2CReadNACK()
{
	u8 mask;
	u8 dat;
	
	I2C_SDA = 1;
	I2CDelay();//这里不要忘记加延时函数
	
	for(mask = 0x80; mask != 0x00; mask >>= 1)
	{
		I2C_SCL = 1;//scl在高电平期间才能读
		I2CDelay();
		
		if(I2C_SDA == 0)
			dat &= ~mask;
		else
			dat |= mask;
	    I2CDelay();
		I2C_SCL = 0;
		I2CDelay();
	}
	I2C_SDA = 1;
	I2CDelay();
	I2C_SCL = 1;
	I2CDelay();
	I2C_SCL = 0;
	I2CDelay();
	
	return dat;
}
	

	

	


