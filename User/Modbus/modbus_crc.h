/* 
 * Modbus CRC
 */

#ifndef _MODBUSB_CRC_H
#define _MODBUSB_CRC_H

#include "main.h"

uint8_t  Modbus_Check_Crc16(uint8_t * data, uint16_t length);
uint16_t Modbus_Create_Crc16( uint8_t * data, uint16_t length);


#endif