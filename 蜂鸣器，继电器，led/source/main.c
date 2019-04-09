#include<stc15.h>
sbit buzz = P0^6;
sbit relay = P0^4;
sbit led = P0^0;
void main()
{
	P2 = ((P2 & 0x1F) | 0xA0);
	buzz = 0;
	P2 = P2 & 0x1F;
	
	P2 = ((P2 & 0x1F) | 0xA0);
	relay = 1;
	P2 = P2 & 0x1F;
	
	P2 = ((P2 & 0x1F) | 0x80);
	led = 1;
	P2 = P2 & 0x1F;
	
	while(1);
}