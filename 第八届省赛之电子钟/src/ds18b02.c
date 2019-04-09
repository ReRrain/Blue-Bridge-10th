#include "config.h"
#include <intrins.h>

void Delayus(u8 us)
{
	do{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}while(--us);
}

bit Get18b02ACK()
{
	bit ack;
	
	EA = 0;//记住使用18b02时首先要关闭总中断
	IO_18B02 = 0;
	Delayus(250);
	Delayus(250);
	IO_18B02 = 1;
	Delayus(60);
	ack = IO_18B02;
	while(!IO_18B02);
	EA = 1;
	
    return ~ack;	
}

void Write18b02(u8 dat)
{
	u8 mask;
	
	EA = 0;
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		IO_18B02 = 0;
		Delayus(2);
		if((dat & mask) == 0)
			IO_18B02 = 0;
		else
			IO_18B02 = 1;
		Delayus(60);
		IO_18B02 = 1;
	}
	EA = 1;
}

u8 Read18b02()
{
	u8 mask;
	u8 dat = 0;
	
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		IO_18B02 = 0;
		Delayus(2);
		IO_18B02 = 1;
		Delayus(2);
		
		if(IO_18B02 == 1)
			dat |= mask;
		else
			dat &= ~mask;
		Delayus(60);
	}
	EA = 1;
	
	return dat;
}

bit Start18b02()
{
	bit ack;
	
	ack = Get18b02ACK();
	if(ack == 1)
	{
		Write18b02(0xCC);
		Write18b02(0x44);
	}
	return ~ack;
}

bit Get18b02Temp(int *temp)
{
	bit ack;
	u8 LSB,MSB;
	
	ack = Get18b02ACK();
	if(ack == 1)
	{
		Write18b02(0xCC);
		Write18b02(0xBE);
		LSB = Read18b02();
		MSB = Read18b02();
		*temp = ((int)MSB << 8) + LSB;
	}
	return ~ack;		
}
	
	