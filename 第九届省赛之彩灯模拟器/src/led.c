#include "config.h"

u8 LedChar[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80 ,0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
u8 LedBuff[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void LedScan()
{
	static u8 index = 0;
	
	P2 = (P2 & 0x1F) | 0xE0;
	P0 = 0xFF;
	P2 &= 0x1F;
	
	P2 = (P2 & 0x1F) | 0xC0;
	P0 = 0x80 >> index;
	P2 &= 0x1F;
	
	P2 = (P2 & 0x1F) | 0xE0;
	P0 = LedBuff[index];
	P2 &= 0x1F;
	
	index++;
	index &= 7;
}

void ShowNumber(u32 dat)
{
	char i;
	u8 buff[8];
	
	for(i = 0;i<8;i++)
	{
		buff[i] = dat % 10;
		dat = dat / 10;
	}
	for(i = 7;i>0; i--)
	{
		if (buff[i] == 0)
			LedBuff[i] = 0xFF;//注意是用Ledbuff这个数组存储的显示数字
        else
			break;
	}
	for(;i>=0;i--)
	{
		LedBuff[i] = LedChar[buff[i]];
	}
}

