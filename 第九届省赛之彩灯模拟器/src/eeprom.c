#include "config.h"
#include "i2.h"

void E2Read(u8 *buf, u8 addr, u8 len)//多字节读
{
	do{
		I2CStart();//开启I2C
		if(I2CWrite(0x50 << 1))//寻址器件，后续写，I2Cwrite返回值为~ack故若为1，则表示应答，跳出循环，执行read操作
		{
			break;
		}	
		I2CStop();
	}while(1);
	I2CWrite(addr);//写入起始地址
	I2CStart();//重复启动，因为接下来要执行读操作了
	I2CWrite((0x50 << 1) | 0x01);//写入寻址器件，后续读
	
	while(len > 1)
	{
		*buf++ = I2CReadACK();
		len--;
	}
	*buf = I2CReadNACK();
	I2CStop();	
}

void E2Write(u8 *buf, u8 addr, u8 len)//页写入
{
	while(len > 0)
	{
		do{
			I2CStart();
			if(I2CWrite(0x50 << 1))//判断E2prom是否处于响应状态，若ack = 0，表示处于响应状态
			{
				break;
			}
			I2CStop();
		}while(1);
		I2CWrite(addr);
		while(len > 0)
		{
			I2CWrite(*buf++); //buf为源数据指针
			addr++;
			len--;
			if((addr & 0x70) == 0)//判断地址是否达到页边界，24C02每页8字节
			{
				break;
			}
		}
		I2CStop();
	}
}