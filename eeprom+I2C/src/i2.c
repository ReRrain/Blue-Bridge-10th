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
//	I2C_SCL = 0;//ֹͣ��scl��������0
}

bit I2CWrite(u8 dat)//ע�����ﺯ���з���ֵ���ʲ��ܶ���Ϊvoid��ʽ
{
	bit ack;
	u8 mask;
	
	for(mask = 0x80; mask != 0x00; mask >>= 1)
	{
		if((mask & dat) == 0)//��������ȼ������⣡��������
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
	
	I2C_SDA = 1;//ֻ����sdaΪ�ߵ�ƽʱ�������ܽ��ж�����
	I2CDelay();//���ﲻҪ���Ǽ���ʱ����
	
	for(mask = 0x80; mask != 0x00; mask >>= 1)
	{
		I2C_SCL = 1;//scl�ڸߵ�ƽ�ڼ���ܶ�
		I2CDelay();
		
		if(I2C_SDA == 0)
			dat &= ~mask;
		else
			dat |= mask;
	    I2CDelay();
		I2C_SCL = 0;//����scl��ʹ�ӻ����ͳ���һλ
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
	I2CDelay();//���ﲻҪ���Ǽ���ʱ����
	
	for(mask = 0x80; mask != 0x00; mask >>= 1)
	{
		I2C_SCL = 1;//scl�ڸߵ�ƽ�ڼ���ܶ�
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
	

	

	


