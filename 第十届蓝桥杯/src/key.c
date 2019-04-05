#include "config.h"

u8 KeySta[4] = {1, 1, 1, 1};
u8 KeyCodeMap[4] = {'1', '2', '3', '4'};

extern void KeyAction(u8 keycode);

void KeyScan()
{
	static u8 keybuff[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	u8 i;
	
	keybuff[0] = (keybuff[0] << 1) | KEY_IN_1;
	keybuff[1] = (keybuff[1] << 1) | KEY_IN_2;
	keybuff[2] = (keybuff[2] << 1) | KEY_IN_3;
	keybuff[3] = (keybuff[3] << 1) | KEY_IN_4;
	
	for(i = 0; i < 4; i++)
	{
		if(keybuff[i] == 0xFF)
		{
			KeySta[i] = 1;
		}
		else if(keybuff[i] == 0x00)
		{
			KeySta[i] = 0;
		}
	}
}

void KeyDriver()
{
	u8 i;
	static u8 backup[4] = {1, 1, 1, 1};
	
	for(i = 0; i < 4;i++)
	{
		if(KeySta[i] != backup[i])
		{
			if(backup[i] == 1)
			{
				KeyAction(KeyCodeMap[i]);
			}
			backup[i] = KeySta[i];
		}
	}
}