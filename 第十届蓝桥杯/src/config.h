#ifndef _CONFIG_H
#define _CONFIG_H

#include <stc15.h>
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

sbit KEY_IN_1 = P3^0;
sbit KEY_IN_2 = P3^1;
sbit KEY_IN_3 = P3^2;
sbit KEY_IN_4 = P3^3;

sbit I2C_SCL = P2^0;
sbit I2C_SDA = P2^1;

sbit IO_18B20 = P1^4;

sbit DS1302_CE = P1^3;
sbit DS1302_IO = P2^3;
sbit DS1302_CK = P1^7;

#endif