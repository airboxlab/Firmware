/*
 * power.h
 *
 * Created: 24/08/2013 17:52:23
 *  Author: Xevel
 */ 

/*
AirBoxLab  v1.0

Power management. Charger status and battery voltage.
Pins:
Battery voltage (VRAW) : PF0 (ADC1)
 
STATUS : NEEDS TO BE CHECKED, MIGHT BE FOR THE OLD BOARD
*/


#ifndef POWER_H_
#define POWER_H_
#include "common.h"
#include <LUFA/Drivers/Peripheral/ADC.h>

void power_init();
uint16_t power_battery_get_raw();
uint16_t power_battery_get_voltage(); //voltage in mV
uint8_t power_battery_no_power(); //Check if the voltage is under 3.5V

#endif /* POWER_H_ */