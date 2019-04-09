//
#ifndef DS1302_H
#define DS1302_H

struct sTime{
	u16 year;
	u8 mon;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
	u8 week;
};

void InitDS1302();
u8 DS1302SingleRead(u8 reg);
void DS1302SingleWrite(u8 reg, u8 dat);
void DS1302BurstRead (u8 *dat);
void DS1302BurstWrite(u8 *dat);
void GetRealTime(struct sTime *time);
void SetRealTime(struct sTime *time);


#endif