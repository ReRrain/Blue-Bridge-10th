#include "config.h"

u8 LedChar[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6};
u8 LedBuff[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void LedScan()
{
	static u8 index = 0;
	
	P2 = (P2 & 0x1F) | 0xE0;
	P0 |= 0xFF;
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
	u8 buff[8];
	char i;
	
	for(i = 0; i<7; i++)
	{
		buff[i] = dat % 10;
		dat /= 10;
	}
	for(i = 7; i>0; i--)
	{
		if(buff[i] == 0)
			LedBuff[i] = 0xFF; 
		else
			break;
	}
	for(; i>=0; i--)
	{
		LedBuff[i] = LedChar[buff[i]];
	}
}