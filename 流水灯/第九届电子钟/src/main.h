#ifndef _MAIN_H
#define _MAIN_H

void ConfigTimer0(u8 ms);
void CloseOther();
void RefreshTemp();
void LedBlink();
void RefreshSetTime(struct sTime *time);

extern u8 flagenter;
extern u8 numBlinkSta;
extern char index;

#endif