/*
 * cc2dxx.h
 *
 * Created: 15/10/2013 18:58:14
 *  Author: Xevel
 */ 


#ifndef CC2DXX_H_
#define CC2DXX_H_
#include "common.h"

/*
AirBoxLab  v2.0

CC2D25 or CC2D23 temperature and humidity sensor.

Pins:
PD1 (SDA)
PD0 (SCL)

STATUS : OK
*/


#include <LUFA/Drivers/Peripheral/TWI.h>

#define CC2DXX_ADDR     ( 0x28 << 1 ) // default addr


void cc2dxx_init();
double cc2dxx_get_humidity();// humidity in %RH, with auto update
double cc2dxx_get_temperature();// temp in °C, with auto update
double cc2dxx_get_humidity_no_update();// humidity in %RH, from last update
double cc2dxx_get_temperature_no_update();// temp in °C, from last update

//internal
extern uint16_t cc2dxx_humidity;
extern uint16_t cc2dxx_temperature;
extern uint8_t cc2dxx_status;
uint8_t cc2dxx_get();


#endif /* CC2DXX_H_ */