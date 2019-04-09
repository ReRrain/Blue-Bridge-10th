#ifndef _CONFIG_H
#define _CONFIG_H
#include <stc15.h>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

sbit DS1302_CE = P1^3;
sbit DS1302_CK = P1^7;
sbit DS1302_IO = P2^3;

#endif