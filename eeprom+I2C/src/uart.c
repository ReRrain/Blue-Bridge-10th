#include "config.h"
#include <string.h>
#include <stdio.h>

bit flagTxd = 0;

bit flagRxdFrame = 0;
u8 cntRxd = 0;
u8 bufRxd[100];

extern void UartAction();//ע�������Ҫ��main.c����д��

/*������ת��Ϊ�ַ���*/
u8 LongToStr(long dat, u8* str)
{
    char i = 0;
    u8 len = 0;
    u8 buf[11];     // // ���������ֵ4294967295��תASCII���ռ��10+1=11�ֽ�(��'\0'�ַ�)

    if(dat < 0)
    {
        dat = -dat;
        *str++ = '-';
        len++;
    }
    do{
        buf[i++] = dat%10 + '0';
        dat /= 10;
    }while(dat > 0);
    len += i;
    while(i-- > 0)
    {
        *str++ = buf[i]; // �����Ѿ�����+0x30�Ĵ����ʴ˴����ٴ���
    }
    *str = '\0';
    return len;
}

/*�ַ���ת��Ϊ������*/
u32 StrToLong(char* str)
{
    u32 result = 0;
    u32 fact = 1;
    u8 len = strlen(str); // �������ַ�'\0'
    u8 i;

    for(i=len-1; i>=0; i--)
    {
        result += ((str[i] - '0') * fact);
        fact *= 10;
    }
    return result;
}

/*���ڳ�ʼ��*/
void InitUart(u8 sta)
{
	if(sta == 0) // �����ʣ�9600 /11.0592MHZ
	{
		PCON &= 0x7F;       //�����ʲ�����
		SCON = 0x50;        //8λ����,�ɱ䲨����
		AUXR &= 0xBF;       //��ʱ��1ʱ��ΪFosc/12,��12T
		AUXR &= 0xFE;       //����1ѡ��ʱ��1Ϊ�����ʷ�����
		TMOD &= 0x0F;       //�����ʱ��1ģʽλ
		TMOD |= 0x20;       //�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
		TL1 = 0xFD;         //�趨��ʱ��ֵ
		TH1 = 0xFD;         //�趨��ʱ����װֵ
		ET1 = 0;            //��ֹ��ʱ��1�ж�
		TR1 = 1;            //������ʱ��1
		
	}
	else if(sta == 1)// �����ʣ�9600 /22.1184MHZ
	{	  
		AUXR = 0x00;  		// ��ʱ��1��Ϊ�����ʷ�����       
		SCON = 0x50; 	
		TMOD |= 0x20;		// 0010 0000 ��ʱ��1�����ڷ�ʽ2��8λ�Զ���װ��ʽ��
		TH1  = 0xFA;	
		TL1  = 0xFA;
		TR1  = 1;
		       
	}
	else if(sta == 2)// �����ʣ�115200 /11.0592MHZ
	{
		PCON &= 0x7F;		//�����ʲ�����
		SCON = 0x50;		//8λ����,�ɱ䲨����
		AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
		AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
		TMOD &= 0x0F;		//�����ʱ��1ģʽλ
		TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
		TL1 = 0xFD;			//�趨��ʱ��ֵ
		TH1 = 0xFD;			//�趨��ʱ����װֵ
		ET1 = 0;			//��ֹ��ʱ��1�ж�
		TR1 = 1;			//������ʱ��1
	}
	
	ES = 1;
}

/*���͵��ֽ�*/
void UartSendByte(u8 dat)
{
    flagTxd = 0;
    SBUF = dat;
    while(!flagTxd);
}

/*���ͻس�����*/
void UartSendEnter()
{
    UartSendByte(0x0D);
    UartSendByte(0x0A);
}

/*�����ַ����������β��\n��������Զ�����س�����*/
void UartSendStr(char *str)
{
    u16 i;
    u16 len = strlen(str); 

    for(i=0; i<len-1; i++) // ���һ���ַ���������ʵ�ּ���س�����
    {
        UartSendByte(str[i]);
    }

    if(str[i] == '\n')
    {
        UartSendEnter();
    }
    else
    {
        UartSendByte(str[i]);
    }
}

void UartSendFloat(float dat, u8* str)
{
	sprintf(str, "%.2f", dat);
	
	UartSendStr(str);
}

/*������ֵ��ʵ��ת��Ϊ�ַ������ͣ���β�Զ�����س�����*/
void UartSendNum(u32 dat, u8 sta)
{
    u8 buf[11];

    LongToStr(dat, buf);
    UartSendStr(buf);
	if(sta)
	{
		UartSendEnter();
	}
}

/*�����ַ���+��ֵ*/
void UartSendStrNum(char* buf, u32 dat, u8 sta)
{
    UartSendStr(buf);
    UartSendNum(dat, sta);
}

/*ʮ����תʮ������*/
void HexToASCII(u16 hex, char* str)
{
    u8 temp = 0;

    temp = ((hex & 0xF000) >> 12);
    str[0] = (temp >= 10) ? (temp-10+'A') : (temp+'0');
    
    temp = ((hex & 0x0F00) >> 8);
    str[1] = (temp >= 10) ? (temp-10+'A') : (temp+'0');
                                                 
    temp = ((hex & 0x00F0) >> 4);                 
    str[2] = (temp >= 10) ? (temp-10+'A') : (temp+'0');
                                                 
    temp = ((hex & 0x000F) >> 0);                 
    str[3] = (temp >= 10) ? (temp-10+'A') : (temp+'0');

    str[4] = '\0';
}

/*����ʮ�������ַ�*/
void UartSendHex(u16 hex)
{
    u8 temp[11];

    HexToASCII(hex, temp);
    UartSendStr(temp);
    UartSendEnter();
}

/*���Ͷ������ַ�*/
void UartSendBinary(u8 dat)
{
    u8 i;
    u8 temp[17];

    for(i=0; i<8; i++)
    {
        temp[i] = ((dat<<i) & 0x80) ? '1':'0';
    }
    temp[i] = '\0';

    for(i=0; i<strlen(temp); i++)
    {
        UartSendByte(temp[i]);
        UartSendByte(' ');
    }
    UartSendEnter();
}

/*�������ݶ�ȡ������lenΪָ�������ݳ��ȣ�����Ϊʵ�ʵ����ݳ���*/
u8 UartRead(u8* buf, u8 len)
{
	u8 i;
	
	if(len > cntRxd)
	{
		len = cntRxd;
	}
	for(i=0; i<len; i++)
	{
		*buf++ = bufRxd[i];
	}
	*buf = '\0';
	cntRxd = 0;
	
	return len;
}

/*���ڷ������ݺ�����lenΪָ���ĳ��ȣ���UartSendStr����ָ�������ַ�������*/
void UartWrite(u8* buf, u8 len)
{
	while(len--)
	{
		UartSendByte(*buf++);
	}
}

/*���ڽ��ռ��, ms-Ϊ��ʱ���*/
void UartMonitor(u8 ms)
{
	static u8 cntbkp = 0;
	static u8 idletmr = 0;
	
	if(cntRxd > 0)
	{
		if(cntbkp != cntRxd)
		{
			cntbkp = cntRxd;
			idletmr = 0;
		}
		else
		{
			if(idletmr < 30)
			{
				idletmr += ms;
				if(idletmr >= 30)
				{
					flagRxdFrame = 1;
				}
			}
		}
	}
	else
	{
		cntbkp = 0;
	}
}

/*�����������������������е���*/
void UartDriver()
{
	if(flagRxdFrame)
	{
		flagRxdFrame = 0;
		UartAction();
	}
}

/*�����ж�*/
void UartInterrupt() interrupt 4
{
	if(RI)
	{
		RI = 0;
		if(cntRxd < sizeof(bufRxd))
		{
			bufRxd[cntRxd++] = SBUF;
		}
	}
	
	if(TI)
	{
		TI = 0;
		flagTxd = 1;
	}

}
