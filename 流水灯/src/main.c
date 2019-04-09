/*#include<stc15.h>

typedef unsigned char u8;
typedef unsigned int u16;

void Closeothers();
void Flowleds();

void main()
{
	Closeothers();
	
	while(1)
	{
		Flowleds();
	}
}

void Closeothers()
{
	P2 = (P2 & 0x1F) | 0xA0;
	P0 = P0 & 0xAF;
	P2 = P2 & 0x1F;
}

void Flowleds()
{
	u16 i;
	static u8 shift = 0x01;
	static bit flag = 1;
	P2 = (P2 & 0x1F) | 0x80;
	
	P0 = ~shift;
	if(flag == 1)
	{
		shift <<= 1;
		if(shift == 0x80)
			flag = 0;
	}
	else 
	{
		shift >>=1;
		if(shift == 0x01)
			flag = 1;
	}
	for(i = 0;i<50000;i++)
	P2 = P2 & 0x1F;
}*/
#include<stc15.h>
typedef unsigned char u8;
typedef unsigned int u16;

void closeothers();
void flowleds();

void main()
{
	closeothers();
	
	while(1)
	{
		flowleds();
	}
}

void closeothers()
{
	P2 = (P2 & 0x1F) | 0xA0;
	P0 = P0 & 0xAF;
	P2 = P2 & 0x1F;
}

void flowleds()
{
	u16 i;
	static bit flag = 1;
	static u8 shift = 0x01;
	P2 = (P2 & 0x1F) | 0x80;
	
	P0 = ~shift;
	if(flag == 1)
	{
		shift <<= 1;
		if(shift == 0x80)
			flag = 0;
	}
	else
	{
		shift >>=1;
		if(shift == 0x01)
			flag = 1;
	}
	
	for(i = 0 ; i<50000 ; i++)
	P2 = (P2 & 0x1F);
}