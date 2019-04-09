#ifndef _CONFIG_H
#define _CONFIG_H

#include <stc16.h>
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

sbit KEY_OUT_1 = P3^0;
sbit KEY_OUT_2 = P3^1;
sbit KEY_OUT_3 = P3^2;
sbit KEY_OUT_4 = P3^3;
sbit KEY_IN_4 = P3^4;
sbit KEY_IN_3 = P3^5;
sbit KEY_IN_2 = P4^2;
sbit KEY_IN_1 = P4^4;

sbit I2C_SCL = P2^0;
sbit I2C_SDA = P2^1;

sbit IO_18B20 = P1^4;

sbit DS1302_CE = P1^3;
sbit DS1302_CK = P1^7;
sbit DS1302_IO = P2^3;

#endif