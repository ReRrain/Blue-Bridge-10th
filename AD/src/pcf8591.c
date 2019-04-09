#include "config.h"
#include "i2C.h"

#define VCC 48

u8 GetADCValue(u8 chn)
{
	u8 val;
	
	I2CStart();
	if(!I2CWrite(0x48<<1))
	{
		I2CStop();
		return 0;
	}
	I2CWrite(0x40 | chn);
	I2CStart();
	I2CWrite((0x48<<1) | 0x01);
	I2CReadACK();
	val = I2CReadNAK();
	I2CStop();
	
	val = (val*VCC) / 255;
	return val;
}

void SetDACOut(u8 val)
{
	val = (val*255) / VCC;
	
	I2CStart();
	if(!I2CWrite(0x48<<1))
	{
		I2CStop();
		return ;//�趨�¶�û�з���ֵ�����Է��ؿ�
	}
	I2CWrite(0x40);
	I2CWrite(val);
	I2CStop();
}