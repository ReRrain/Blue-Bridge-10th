#include "config.h"
#include "ds1302.h"

void DS1302ByteWrite(u8 dat)
{
	u8 mask;
	
	DS1302_IO = 1;
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		if(dat & mask)
		{
			DS1302_IO = 1;
		}
		else
		{
			DS1302_IO = 0;
		}
		DS1302_CK = 1;
		DS1302_CK = 0;
	}
	DS1302_IO = 1;//ÊÍ·Å×ÜÏß
}

u8 DS1302ByteRead()
{
	u8 dat = 0;
	u8 mask;
	
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		if(DS1302_IO)
		{
			 dat |= mask;
		}
		DS1302_CK = 1;
		DS1302_CK = 0;
	}
	return dat;
}

void DS1302SingleWrite(u8 reg, u8 dat)
{
	DS1302_CE = 1;
	DS1302ByteWrite((reg << 1) | 0x80);
	DS1302ByteWrite(dat);
	DS1302_CE = 0;
}

u8 DS1302SingleRead(u8 reg)
{
	u8 dat;
	DS1302_CE = 1;
	DS1302ByteWrite((reg << 1) | 0x81);
	dat = DS1302ByteRead();
	DS1302_CE = 0;
	DS1302_IO = 0;
	return dat;
}

void DS1302BurstWrite(u8 *dat)
{
	u8 i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xBE);
	for(i = 0; i<7;i++)
	{
		DS1302ByteWrite(*dat++);
	}
	DS1302_CE = 0;
}

void DS1302BurstRead(u8 *dat)
{
	u8 i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xBF);
	for(i = 0; i<7;i++)
	{
		dat[i] = DS1302ByteRead();
	}
	DS1302_CE = 0;
	DS1302_IO = 0;
}

void SetRealTime(struct sTime *time)
{
	u8 buf[8];
	
	buf[7] = 0x00;
	buf[6] = time->year;
	buf[4] = time->mon;
	buf[3] = time->day;
	buf[2] = time->hour;
	buf[1] = time->min;
	buf[0] = time->sec;
	buf[5] = time->week;
	DS1302BurstWrite(buf);
}

void GetRealTime(struct sTime *time)
{
	u8 buf[8];
	
	DS1302BurstRead(buf);
	time->year = buf[6] + 0x2000;
	time->mon = buf[4];
	time->day = buf[3];
	time->hour = buf[2];
	time->min = buf[1];
	time->sec = buf[0];
	time->week = buf[5];
}

void InitDS1302Time()
{
	struct sTime InitTime[] = {0x00, 0x00, 0x00, 0x10, 0x20, 0x30, 0x07};
	DS1302_CE = 0;
	DS1302_CK = 0;
	DS1302SingleWrite(7, 0x00);
	SetRealTime(InitTime);
}

