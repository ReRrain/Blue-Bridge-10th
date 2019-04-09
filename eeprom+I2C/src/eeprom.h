#ifndef _EEPROM_H
#define _EEPROM_H

void E2Read(u8 *buf, u8 addr, u8 len);
void E2Write(u8 *buf, u8 addr, u8 len);

#endif