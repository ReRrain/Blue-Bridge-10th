#include "config.h"
#include "i2c.h"

#define VCC 500

u16 GetADCValue(u8 chn)
{
	u16 val;
	
	I2CStart();
	if(!I2CWrite(0x48 << 1))
	{
		I2CStop();
		return 0;
	}
	I2CWrite((0x40 << 1) | chn);
	I2CStart();
	I2CWrite((0x48 << 1) | 0x01);
	I2CReadACK();
	val = I2CReadNAK();
	
	I2CStop();
	
	val = ((u32)val * 500) / 255;
	return val;
}

void  SetDACValue(u16 val)
{
	val = ((u32)val * 255) / 500;
	
	I2CStart();
	if(!I2CWrite(0x48 << 1))
	{
		I2CStop();
		return ;
	}
	I2CWrite(0x40);
	I2CWrite(val);
	I2CStop();
}