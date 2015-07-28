/*
 * iaqengine.h
 *
 * Created: 23/08/2013 21:57:21
 *  Author: Xevel
 */ 


#ifndef IAQENGINE_H_
#define IAQENGINE_H_
#include "common.h"
#include <LUFA/Drivers/Peripheral/TWI.h>

/*
AirBoxLab  v2.0

iAQengine

Pins :
PD1 (SDA)
PD0 (SCL)

STATUS : need testing, but should still be OK
*/

typedef struct {
	int16_t iaqengine_voc;
	int32_t iaqengine_ohm;
	} iaqengine_result;
	
void iaqengine_init();
//int16_t iaqengine_get_ppm();
iaqengine_result iaqengine_get_value();

#define IAQENGINE_I2C_ADDR   0xB5

#endif /* IAQENGINE_H_ */