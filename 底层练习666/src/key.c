#include "config.h"

u8 KeySta[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};
u8 KeyCodeMap[4][4] = {{'1', '2', '3', '0'}, {'4', '5', '6', '0'}, {'7', '8', '9', '0'}, {'0', '0', '0', '0'}};
	
extern void KeyAction(u8 keycode);
	
void KeyScan()
{
	static u8 keybuff[4][4] = {{0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF, 0xFF}};
	static u8 keyout = 0;
    u8 i;
	
	keybuff[keyout][0] = (keybuff[keyout][0] << 1) | KEY_IN_1;
	keybuff[keyout][1] = (keybuff[keyout][1] << 1) | KEY_IN_2;
	keybuff[keyout][2] = (keybuff[keyout][2] << 1) | KEY_IN_3;
	keybuff[keyout][3] = (keybuff[keyout][3] << 1) | KEY_IN_4;

	for(i = 0; i<4; i++)
	{
		if(keybuff[keyout][i] == 0xFF)
		{
			KeySta[keyout][i] = 1;
		}
		else if(keybuff[keyout][i] == 0x00)
		{
			KeySta[keyout][i] = 0;
		}
	}
	keyout++;
	keyout %= 4;
	
	switch(keyout)
	{
		case 0: KEY_OUT_1 = 0; KEY_OUT_4 = 1;break;
		case 1: KEY_OUT_2 = 0; KEY_OUT_1 = 1;break;
		case 2: KEY_OUT_3 = 0; KEY_OUT_2 = 1;break;
		case 3: KEY_OUT_4 = 0; KEY_OUT_3 = 1;break;
		default: break;
	}
}

void KeyDriver()
{
	static u8 backup[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};
	u8 i, j;
		
	for(i = 0; i<4; i++)
	{
		for(j = 0; j<4; j++)
		{
			if(KeySta[i][j] != backup[i][j])
			{
				if(backup[i][j] == 1)
				{
					KeyAction(KeyCodeMap[i][j]);
				}
				backup[i][j] = KeySta[i][j];
			}
		}
	}
}