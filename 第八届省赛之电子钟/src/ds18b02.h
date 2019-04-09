#ifndef _DS18B02_H
#define _DS18B02_H

//void Delayus(u8 us);
//bit Get18b02ACK();
//void Write18b02(u8 dat);
//u8 Read18b02();
bit Start18b02();
bit Get18b02Temp(int *temp);

#endif