#ifndef _TIME_H
#define _TIME_H

void SetClockTimer();
void SetAlarmTimer();
void AddTime(struct sTime *time);
void SubTime(struct sTime *time);

extern struct sTime setClockTime;
extern struct sTime setAlarmTime;


#endif