/*
 * led.h
 *
 * Created: 23/08/2013 19:43:32
 *  Author: Xevel
 */ 

#ifndef LED_H_
#define LED_H_
#include "common.h"

/*
AirBoxLab  v2.0

LEDs

Pins:
R: PB6 (OC1B)
G: PD7 (OC4D)
B: PB5 (OC1A)

STATUS : OK
*/

void led_init();
void led_set(uint8_t red, uint8_t green, uint8_t blue);
void led_clear();


#endif /* LED_H_ */