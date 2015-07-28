/*
 * sensor_board.h
 *
 * Created: 23/08/2013 21:17:54
 *  Author: Xevel
 */ 


#ifndef SENSOR_BOARD_H_
#define SENSOR_BOARD_H_
#include "common.h"
#include <LUFA/Drivers/Peripheral/TWI.h>

/*
AirBoxLab  v2.0

The sensor board has an ATtiny MCU that mostly controls the MQ135 sensor.
It can also give some info on the temperature of the board (Not Implemented Yet).

Pins:
I2C

STATUS : OK
*/

void sensor_board_init();

// all these functions return a TWI_ERROR_xxx value, 0 if OK.
uint8_t sensor_board_heater_on(uint8_t turn_on);
uint8_t sensor_board_heater_is_on(uint8_t* res);
uint8_t sensor_board_get_mq135(uint16_t* val); // in mV, assuming power is 5.00V
uint8_t sensor_board_get_sensor1(uint16_t* val);
uint8_t sensor_board_get_sensor2(uint16_t* val);

// private
uint8_t sensor_board_update(); // update MQ135 and temp (NIY) values
uint8_t sensor_board_write_reg(const uint8_t addr, const uint8_t data);

#endif /* SENSOR_BOARD_H_ */