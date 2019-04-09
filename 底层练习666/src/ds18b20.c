#include "config.h"

void DelayUs(u8 us)
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

bit Get18B20ACK()
{
	bit ack;
	EA = 0;
	IO_18B20 = 0;
	DelayUs(250);
	DelayUs(250);
	IO_18B20 = 1;
	DelayUs(60);
	ack = IO_18B20;
	while(!IO_18B20);
	EA = 1;
	
	return ~ack;
}

void Write18B20(u8 dat)
{
	u8 mask;
	
	EA = 0;
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		IO_18B20 = 0;
		DelayUs(2);
		if(dat & mask)
		{
			IO_18B20 = 1;
		}
		else
		{
			IO_18B20 = 0;
		}
		DelayUs(60);
		IO_18B20 = 1;//ÿдһλ���ݶ�Ҫ�ͷ�����
	}
	EA = 1;
}

u8 Read18B20()
{
	u8 mask;
	u8 dat = 0;
	
	EA = 0;
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		IO_18B20 = 0;
		DelayUs(2);
		IO_18B20 = 1;
		DelayUs(2);
		if(IO_18B20)
		{
			dat |= mask;
		}
		DelayUs(60);
	}
	EA = 1;
	return dat;
}

bit Start18B20()
{
	bit ack;
	ack = Get18B20ACK();
	
	if(ack)
	{
		Write18B20(0xCC);
		Write18B20(0x44);
	}
	return ~ack;
}

bit Get18B20Temp(int *temp)
{
	bit ack;
	u8 MSB,LSB;
	ack = Get18B20ACK();
	
	if(ack)
	{
		Write18B20(0xCC);
		Write18B20(0xBE);//��ȡת���¶���0xBE����������
		LSB = Read18B20();//�ȶ���8λ
		MSB = Read18B20();//�ٶ���8λ
		
		MSB &= 0x0F;//�������λ������λ��
		*temp = ((int)MSB << 8) | LSB;
	}
	return ~ack;
}