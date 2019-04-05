#include "config.h"
#include "main.h"
#include "pcf8591.h"

extern u16 val;

void ValTest()//电压提示
{
	if(flagLED == 0)
	{
		if(val < 150)
		{
			if(flagDAC == 0)
			{
				P2 = (P2 & 0x1F) | 0x80;
				P0 = 0xFE;
				P2 &= 0x1F;
			}
			else
			{
				P2 = (P2 & 0x1F) | 0x80;
				P0 = 0xEE;
				P2 &= 0x1F;
			}
		}
		else if(val < 250)
		{
			if(flagDAC == 0)
			{
				P2 = (P2 & 0x1F) | 0x80;
				P0 = 0xFA;
				P2 &= 0x1F;
			}
			else
			{
				P2 = (P2 & 0x1F) | 0x80;
				P0 = 0xEA;
				P2 &= 0x1F;
			}
		}
		else if(val < 350)
		{
			if(flagDAC == 0)
			{
				P2 = (P2 & 0x1F) | 0x80;
				P0 = 0xFE;
				P2 &= 0x1F;
			}
			else
			{
				P2 = (P2 & 0x1F) | 0x80;
				P0 = 0xEE;
				P2 &= 0x1F;
			}
		}
		else
		{
			if(flagDAC == 0)
			{
				P2 = (P2 & 0x1F) | 0x80;
				P0 = 0xFA;
				P2 &= 0x1F;
			}
			else
			{
				P2 = (P2 & 0x1F) | 0x80;
				P0 = 0xEA;
				P2 &= 0x1F;
			}
		}
	}
	else
	{
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFF;
		P2 &= 0x1F;
	}
}

void FrTest()//频率提示
{
	if(flagLED == 0)
	{
		if(cntBuf < 1000)
		{
			P2 = (P2 & 0x1F) | 0x80;
			P0 = 0xFD;
			P2 &= 0x1F;
		}
		else if(cntBuf < 5000)
		{
			P2 = (P2 & 0x1F) | 0x80;
			P0 = 0xF5;
			P2 &= 0x1F;
		}
		else if(cntBuf < 10000)
		{
			P2 = (P2 & 0x1F) | 0x80;
			P0 = 0xFD;
			P2 &= 0x1F;
		}
		else 
		{
			P2 = (P2 & 0x1F) | 0x80;
			P0 = 0xF5;
			P2 &= 0x1F;
		}
	}
	else
	{
		P2 = (P2 & 0x1F) | 0x80;
		P0 = 0xFF;
		P2 &= 0x1F;
	}
}