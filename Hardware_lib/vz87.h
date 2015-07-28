/*
 * vz87.h
 *
 * Created: 6/2/2014 6:42:17 PM
 *  Author: Tony
 */ 


#ifndef VZ87_H_
#define VZ87_H_
#include "common.h"
#include <LUFA/Drivers/Peripheral/TWI.h>
typedef struct {
	int8_t vz87_voc_short;
	int8_t vz87_voc_long;
	int8_t vz87_co2;
	int32_t vz87_ohm;
} vz87_result;

void vz87_init();
//int16_t iaqengine_get_ppm();
vz87_result vz87_get_value();

#define VZ87_I2C_ADDR   (0x70 << 1)


#endif /* VZ87_H_ */