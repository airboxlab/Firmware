/*
 * Motor.h
 *
 * Created: 23/08/2013 18:31:56
 *  Author: Xevel
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_
#include "common.h"

/*
AirBoxLab  v2.0

Motor (for ventilation)

Pins :
PC7 (OC4A)

STATUS : OK
*/

void motor_init();
void motor_set(uint8_t pwm);
uint8_t motor_get();

#endif /* MOTOR_H_ */