/* 
 * File:   eeprom.h
 * Author: andrew
 *
 * Created on March 13, 2018, 1:11 AM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h>

/**
 * Write data to eeprom
 * @param eepromAddress
 * @param eepromData
 */
void EEPROM_WriteByte(unsigned char, unsigned char);

/**
 * Read data from eeprom
 * @param eepromAddress
 * @return data
 */
unsigned char EEPROM_ReadByte(unsigned char);

#endif	/* EEPROM_H */

