#include "config.h"
#include "ds1302.h"

void DS1302ByteWrite(u8 dat)
{
	u8 mask;
	DS1302_IO = 1;
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		if((dat & mask) == 0)//
			DS1302_IO = 0;
		else
			DS1302_IO = 1;
		DS1302_CK = 1;//拉高时钟
		DS1302_CK = 0;//拉低时钟,完成一个位的操作
	}
	DS1302_IO = 1;//确保释放io引脚
}

u8 DS1302ByteRead()
{
	u8 mask;
	u8 dat = 0;
	
	for(mask = 0x01; mask != 0; mask <<=1)
	{
		if(DS1302_IO)
			dat |= mask;
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
	DS1302ByteWrite((reg << 1) | 0x81);//为什么左移还不太理解
	dat = DS1302ByteRead();
	DS1302_CE = 0;
	DS1302_IO = 0;//单字节读必须要加这个！！！
	
	return dat;
}


void DS1302BurstWrite(u8 *dat)
{
	u8 i;
	 
	DS1302_CE = 1;
	DS1302ByteWrite(0xBE);
	for(i = 0; i<7; i++)
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
	for(i = 0; i<7; i++)
	{
		dat[i] = DS1302ByteRead();
	}
	DS1302_CE = 0;
	DS1302_IO = 0;//突发读必须加！！！
	
}

void GetRealTime(struct sTime *time)//得到时间，读出
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

void SetRealTime(struct sTime *time)//设置时间日期，写入
{
	u8 buf[8];
	
	buf[7] = 0;
	buf[6] = time->year;
	buf[4] = time->mon;
	buf[3] = time->day;
	buf[2] = time->hour;
	buf[1] = time->min;
	buf[0] = time->sec;
	buf[5] = time->week;
	DS1302BurstWrite(buf);	
}

void InitDS1302()
{
	struct sTime InitTime[] = {
		0x18, 0x03, 0x01, 0x10, 0x40, 0x00, 0x04
	};
	DS1302_CE = 0;
	DS1302_CK = 0;
	DS1302SingleWrite(7, 0x00);//撤销写保护允许写入数据
	SetRealTime(&InitTime);
}

