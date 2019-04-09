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
}

bit I2CWrite(u8 dat)
{
	u8 mask;
	bit ack;
	
	for(mask = 0x80; mask != 0; mask >>=1)
	{
		if((dat & mask) == 0)
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
	I2C_SCL = 0;
	I2CDelay();
	
	return ~ack;
}

u8 I2CReadACK()
{
	u8 mask;
	u8 dat;
	
	I2C_SDA = 1;//SDA为1，主机才能读！！！！1
	I2CDelay();
	for(mask = 0x80; mask != 0; mask >>= 1)
	{
		I2C_SCL = 1;//SCL为1，才能读数据！！！！
		I2CDelay();
		if(I2C_SDA == 1)
			dat |= mask;
		else
			dat &= ~mask;
		I2C_SCL = 0;//这里只要拉低SCl就可以了！！
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
	
u8 I2CReadNAK()
{
	u8 mask;
	u8 dat;
	
	I2C_SDA = 1;//SDA为1，主机才能读！！！！1
	I2CDelay();
	for(mask = 0x80; mask != 0; mask >>= 1)
	{
		I2C_SCL = 1;//SCL为1，才能读数据！！！！
		I2CDelay();
		if(I2C_SDA == 1)
			dat |= mask;
		else
			dat &= ~mask;
		I2C_SCL = 0;//这里只要拉低SCl就可以了！！
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
	

