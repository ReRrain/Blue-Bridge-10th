#ifndef _I2_H
#define _I2_H

void Delay5us();	
void I2CStart();
void I2CStop();
bit I2CWrite(u8 dat);
u8 I2CReadACK();
u8 I2CReadNACK();

#endif