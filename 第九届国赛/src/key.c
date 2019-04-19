#include "config.h"

u8 Keysta[4] = {1, 1, 1, 1};
u8 KeyCodeMap[4] = {'1', '2', '3', '4'};
u16 KeyDownTime[4] = {0, 0, 0, 0};//按键按下的时间，注意这里也要用u16!!!!!!!!!!!!
extern void KeyAction(u8 keycode);
extern u8 flagset;

void KeyScan()//消抖
{
	u8 i;
	static u8 keybuff[4] = {0xFF,0xFF,0xFF,0xFF};
	
	keybuff[0] = (keybuff[0] << 1) | KEY_IN_1;
	keybuff[1] = (keybuff[1] << 1) | KEY_IN_2;
	keybuff[2] = (keybuff[2] << 1) | KEY_IN_3;
	keybuff[3] = (keybuff[3] << 1) | KEY_IN_4;
	
	for(i = 0;i<4;i++)
	{
		if(keybuff[i] == 0xFF)//扫描8次，8位都为1
		{
			Keysta[i] = 1;//说明当前按键并没有被按下
			KeyDownTime[i] = 0;//这里注意也要考虑到！！！！！当按键没有按下时，keydowntime[i]是为0的
		}
		else if(keybuff[i] == 0x00)//扫描8次，8位都为0
		{
			Keysta[i] = 0;//说明按键被按下
			if(flagset)
			{
				if(i == 1)
				{
					KeyDownTime[i] += 1;//按键按下时间每增加1ms，KeyDownTime就+1
				}
			}
		}
		else
		{}
	}
}
void KeyDriver()
{
	u8 i;
	static u8 backup[4] = {1,1,1,1};//存储数码管上一次的状态，注意要定义为静态变量，因为状态值每一次按键按下都需要更新
	static u16 TimeMax[4] = {800, 800, 800, 800};//注意这里要定义为u16，不然就溢出了!!!!!!
	for(i = 0;i<4;i++)
	{
		if(Keysta[i] != backup[i])//表明按键状态发生改变
		{
			if(backup[i] == 1)//如果上一次的状态为1，说明当前状态为0（为0就表示按键被按下）
			{
				KeyAction(KeyCodeMap[i]);//执行相应按键的功能
			}
			backup[i] = Keysta[i];//更新按键状态
		}
		if(KeyDownTime[i] > 0)//按键按下时间大于2ms
		{
			if(KeyDownTime[i] > TimeMax[i])//按键按下时间大于最大按键时间(800ms)，说明处于长按状态
			{
				KeyAction(KeyCodeMap[i]);//执行相应按键的功能
				TimeMax[i] += 100;//增加长按键判断时间
			}
		}
		else//要加在if循环外,考虑到KeyDownTime[i]<=0（按键未按下）的情况
		{
			TimeMax[i] = 800;
		}
	}
}
