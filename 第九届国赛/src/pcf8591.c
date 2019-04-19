#include "config.h"
#include "i2c.h"

#define VCC 50

u8 GetADCValue(u8 chn)
{
	u8 val;
	I2CStart();
	if(!I2CWrite(0x48<<1))
	{
		I2CStop();
		return 0;
	}
	I2CWrite(0x40 | chn);//先写入控制字节
	I2CStart();
	I2CWrite((0x48<<1) | 0x01);
	I2CReadACK();
	val = I2CReadNAK();
	I2CStop();//这里不要忘记啦！！！
	
	val = (val * VCC) / 255;
	return val;
}

void SetDACValue(u8 val)
{
	I2CStart();
	if(!I2CWrite(0x48<<1))
	{
		I2CStop();
		return ;
	}
	I2CWrite(0x40);
	I2CWrite(val);
	I2CStop();
}