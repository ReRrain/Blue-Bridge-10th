#include "config.h"

u8 Keysta[4] = {1, 1, 1, 1};
u8 KeyCodeMap[4] = {'1', '2', '3', '4'};
u16 KeyDownTime[4] = {0, 0, 0, 0};//�������µ�ʱ�䣬ע������ҲҪ��u16!!!!!!!!!!!!
extern void KeyAction(u8 keycode);
extern u8 flagset;

void KeyScan()//����
{
	u8 i;
	static u8 keybuff[4] = {0xFF,0xFF,0xFF,0xFF};
	
	keybuff[0] = (keybuff[0] << 1) | KEY_IN_1;
	keybuff[1] = (keybuff[1] << 1) | KEY_IN_2;
	keybuff[2] = (keybuff[2] << 1) | KEY_IN_3;
	keybuff[3] = (keybuff[3] << 1) | KEY_IN_4;
	
	for(i = 0;i<4;i++)
	{
		if(keybuff[i] == 0xFF)//ɨ��8�Σ�8λ��Ϊ1
		{
			Keysta[i] = 1;//˵����ǰ������û�б�����
			KeyDownTime[i] = 0;//����ע��ҲҪ���ǵ�����������������û�а���ʱ��keydowntime[i]��Ϊ0��
		}
		else if(keybuff[i] == 0x00)//ɨ��8�Σ�8λ��Ϊ0
		{
			Keysta[i] = 0;//˵������������
			if(flagset)
			{
				if(i == 1)
				{
					KeyDownTime[i] += 1;//��������ʱ��ÿ����1ms��KeyDownTime��+1
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
	static u8 backup[4] = {1,1,1,1};//�洢�������һ�ε�״̬��ע��Ҫ����Ϊ��̬��������Ϊ״ֵ̬ÿһ�ΰ������¶���Ҫ����
	static u16 TimeMax[4] = {800, 800, 800, 800};//ע������Ҫ����Ϊu16����Ȼ�������!!!!!!
	for(i = 0;i<4;i++)
	{
		if(Keysta[i] != backup[i])//��������״̬�����ı�
		{
			if(backup[i] == 1)//�����һ�ε�״̬Ϊ1��˵����ǰ״̬Ϊ0��Ϊ0�ͱ�ʾ���������£�
			{
				KeyAction(KeyCodeMap[i]);//ִ����Ӧ�����Ĺ���
			}
			backup[i] = Keysta[i];//���°���״̬
		}
		if(KeyDownTime[i] > 0)//��������ʱ�����2ms
		{
			if(KeyDownTime[i] > TimeMax[i])//��������ʱ�������󰴼�ʱ��(800ms)��˵�����ڳ���״̬
			{
				KeyAction(KeyCodeMap[i]);//ִ����Ӧ�����Ĺ���
				TimeMax[i] += 100;//���ӳ������ж�ʱ��
			}
		}
		else//Ҫ����ifѭ����,���ǵ�KeyDownTime[i]<=0������δ���£������
		{
			TimeMax[i] = 800;
		}
	}
}
