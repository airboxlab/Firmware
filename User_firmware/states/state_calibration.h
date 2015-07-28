/*
 * state_calibration.h
 *
 * Created: 28/04/2014 16:33:50
 *  Author: Tony
 */ 


#ifndef STATE_CALIBRATION_H_
#define STATE_CALIBRATION_H_
#include "../common.h"
#include "../sensor_calib.h"


#define SIGNIFICATIVE_DIGITS_K		100 //2 significative digits

typedef struct{
	int	length_S_0;
	int	length_S_5;
	int	length_S_10;
} value_sensor;



//r(x)=ax+b
typedef struct{
	uint8_t a;
	int32_t b;
} coef_calib;

//State used to calibrate the sensor with a k-factor
void state_calibration_init();
void state_calibration_play();
void state_calibration_end();

coef_calib calculate_k(uint32_t ref1, uint32_t measured1,uint32_t ref2, uint32_t measured2);
uint32_t char_to_int(char t[],int start, int len);
value_sensor parse_ref_value(char t[],int len);
uint32_t ten_pow(uint8_t pow);
#endif /* STATE_CALIBRATION_H_ */