#ifndef _I2C_H
#define _I2C_H

void I2CStart();
void I2CStop();
bit I2CWrite(u8 dat);
u8 I2CReadACK();
u8 I2CReadNAK();

#endif