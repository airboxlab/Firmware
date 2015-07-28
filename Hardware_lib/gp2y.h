/*
 * gp2y.h
 *
 * Created: 25/08/2013 01:58:52
 *  Author: Xevel
 */ 


#ifndef GP2Y_H_
#define GP2Y_H_
#include "common.h"

/*
AirBoxLab  v2.0

GP2Y... particle sensor.

Pins :
LED : PD4
GP2Y_output : PD6 (ADC9)

STATUS : OK
*/


void gp2y_init();

// TODO find out how about the dynamics of the sensor, how many does it take to settle...

uint16_t gp2y_read_val();
uint16_t gp2y_get_avg(uint8_t nbr_readings, uint8_t nb_start);
uint16_t gp2y_get_voltage();
double gp2y_get_pm();



#endif /* GP2Y_H_ */