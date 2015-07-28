/*
 * Temperature.h
 *
 * Created: 09/10/2013 23:06:32
 *  Author: Xevel
 */ 


#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "adc.h"

/*
AirBoxLab  v2.0 Sensor Board

Internal temperature

Pins :
none

STATUS : TODO
*/
#include "common.h"
#include "util/delay.h"

void temperature_init(){
    init_adc();
}

uint16_t temperature_get(){
   return 0;
}


#endif /* TEMPERATURE_H_ */