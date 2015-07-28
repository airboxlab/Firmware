/*
 * sensor_calib.h
 *
 * Created: 02/05/2014 17:09:00
 *  Author: Tony
 */ 


#ifndef SENSOR_CALIB_H_
#define SENSOR_CALIB_H_
#include <avr/eeprom.h>
#include "common.h"

#ifdef CALIBRATION
void sensor_calib_set_k(uint8_t d_k, int32_t d_b,int significative_digit,int number_of_calib);

void sensor_calib_save_k(int type_of_calib);
#endif
void sensor_calib_load_k();

//Erase only the magic key
#ifdef CALIBRATION
void sensor_calib_erase_calibration_k();
#endif
uint8_t sensor_calib_PM_is_done();
uint8_t sensor_calib_HUM_is_done();
uint8_t sensor_calib_TEMP_is_done();

extern double k_pm;
extern double b_pm;
extern double k_hum;
extern double b_hum;
extern double k_temp;
extern double b_temp;
#endif /* SENSOR_CALIB_H_ */