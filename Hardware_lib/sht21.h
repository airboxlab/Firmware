/*
 * sht21.h
 *
 * Created: 23/08/2013 21:36:04
 *  Author: Xevel
 */ 


#ifndef SHT21_H_
#define SHT21_H_
#include "common.h"
#include <LUFA/Drivers/Peripheral/TWI.h>

/*
AirBoxLab  v1.0

SHT21
Pins :
PD1 (SDA)
PD0 (SCL)

STATUS : deprecated, might need rework
*/

void sht21_init();
double sht21_get_humidity();
double sht21_get_temperature();

//internal
int16_t sht21_get(uint8_t addr);


#endif /* SHT21_H_ */