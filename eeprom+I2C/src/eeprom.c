#include "config.h"
#include "i2.h"

void E2Read(u8 *buf, u8 addr, u8 len)//���ֽڶ�
{
	do{
		I2CStart();//����I2C
		if(I2CWrite(0x50 << 1))//Ѱַ����������д��I2Cwrite����ֵΪ~ack����Ϊ1�����ʾӦ������ѭ����ִ��read����
		{
			break;
		}	
		I2CStop();
	}while(1);
	I2CWrite(addr);//д����ʼ��ַ
	I2CStart();//�ظ���������Ϊ������Ҫִ�ж�������
	I2CWrite((0x50 << 1) | 0x01);//д��Ѱַ������������
	
	while(len > 1)
	{
		*buf++ = I2CReadACK();
		len--;
	}
	*buf = I2CReadNACK();
	I2CStop();	
}

void E2Write(u8 *buf, u8 addr, u8 len)//ҳд��
{
	while(len > 0)
	{
		do{
			I2CStart();
			if(I2CWrite(0x50 << 1))//�ж�E2prom�Ƿ�����Ӧ״̬����ack = 0����ʾ������Ӧ״̬
			{
				break;
			}
			I2CStop();
		}while(1);
		I2CWrite(addr);
		while(len > 0)
		{
			I2CWrite(*buf++); //bufΪԴ����ָ��
			addr++;
			len--;
			if((addr & 0x70) == 0)//�жϵ�ַ�Ƿ�ﵽҳ�߽磬24C02ÿҳ8�ֽ�
			{
				break;
			}
		}
		I2CStop();
	}
}