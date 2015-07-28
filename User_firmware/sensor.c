/*
 * sensor.c
 *
 * Created: 04/01/2014 05:33:29
 *  Author: Xevel
 */ 

#include "sensor.h"
#include "board.h"

#include <Hardware_lib/motor.h>
#include <Hardware_lib/gp2y.h>
#include <Hardware_lib/cc2dxx.h>
#ifdef IAQ
	#include <Hardware_lib/iaqengine.h>
#endif
#ifdef VZ87
	#include <Hardware_lib/vz87.h>
#endif
#include <Hardware_lib/sensor_board.h>
#include "board.h"
#include "util_timer.h"
#include <Hardware_lib/time.h>
#include "sensor_calib.h"

double      sensor_pm;
double      allpolu;
uint16_t	sensor_voc;
sensors_acc_t   sensors_acc;

void sensors_reset_acc(){
    memset(&sensors_acc, 0, sizeof(sensors_acc));
}


// The GP2Y is 

#define GP2Y_PERIOD 10
// WARNING the sum of NB_START and NB_READING must be less than 255, or we would need to change the type of the loop variable.
#define NB_START    50      // number of samples to discard at the begining
#define NB_READINGS 100     // number of samples to collect

#define GP2Y_FACTOR     (100.0) // At 1 it's too low and returns 0.0...
#define GP2Y_OFFSET     (-0.07) // TODO calibrate (default -0.1 according to http://www.howmuchsnow.com/arduino/airquality)

void update_gp2y(){
    uint32_t gp2y_get_timeout = set_timeout(millis() + GP2Y_PERIOD);
    double total_reads = 0;

    for ( uint8_t i = 0; i < NB_READINGS + NB_START; i++ ){
        while ( millis() < gp2y_get_timeout );
        util_timer_disable_interrupt();
        uint16_t val = gp2y_read_val();
        util_timer_enable_interrupt();
        gp2y_get_timeout += GP2Y_PERIOD;
        if (i >= NB_START) {
            total_reads += val;
        }
    }
    
    sensor_pm = total_reads * (  1.0 / NB_READINGS * (5.0 / 1023.0) * 0.172 * GP2Y_FACTOR) + GP2Y_OFFSET* GP2Y_FACTOR ;
}

double map100(double val, double istart, double istop) {
    return (val - istart) * 100.0 / (istop - istart);
}

void sensor_update(){

	
    // TODO manage errors : if we have a read error, we can't just continue, it would affect the accumulator.
	uint16_t	sensor_co2;
	uint32_t	sensor_ohm;
	uint16_t	sensor1=0;
	uint16_t	sensor2=0;
	uint8_t		vz87_voc=0;
	uint8_t		vz87_co2=0;
	uint16_t	vz87_raw=0;

	#ifdef IAQ
		util_timer_disable_interrupt();
		iaqengine_result iaqresult=iaqengine_get_value();
		sensor_voc = iaqresult.iaqengine_voc;
		sensor_ohm = iaqresult.iaqengine_ohm;
		util_timer_enable_interrupt();
	#endif
	#ifdef VZ87	
		util_timer_disable_interrupt();
		vz87_result vzresult=vz87_get_value();
		vz87_voc=vzresult.vz87_voc;
		vz87_co2=vzresult.vz87_co2;
		vz87_raw=(uint32_t) vzresult.vz87_ohm;
		util_timer_enable_interrupt();
	#endif
	#ifdef SENSOR_1
		util_timer_disable_interrupt();
		sensor_board_update();
		sensor_board_get_sensor1(&sensor1);
		util_timer_enable_interrupt();
	#endif
	#ifdef SENSOR_2
		util_timer_disable_interrupt();
		sensor_board_update();
		sensor_board_get_sensor2(&sensor2);
		util_timer_enable_interrupt();
	#endif
	#ifdef MQ135
		util_timer_disable_interrupt();
		sensor_board_get_mq135(&sensor_co2);
		util_timer_enable_interrupt();
	#endif
    util_timer_disable_interrupt();
    cc2dxx_get();
    util_timer_enable_interrupt();
    sensors_acc.tmp_instant = cc2dxx_get_temperature_no_update() - 1.5;	//crappy calibration hack
    sensors_acc.hum_instant = cc2dxx_get_humidity_no_update() + 12;	//crappy calibration hack
	double sensor_tmp_calib = sensors_acc.tmp_instant*k_temp+b_temp;
	double sensor_hum_calib = sensors_acc.hum_instant*k_hum+b_hum;
    DSENSOR_INFO_PRINTF("%lu, %d, %lu, %u ,%u, %u, %u, %u, %u, ", millis(), sensor_voc, sensor_ohm, sensor_co2, vz87_voc, vz87_co2, vz87_raw, sensor1, sensor2);
    update_gp2y();
	
	double sensor_pm_calib = sensor_pm*k_pm+b_pm;
    DSENSOR_INFO_PRINTF( "%u, %u, %u, %u, %u ,%u\r\n", (uint16_t)(sensor_pm*100), (uint16_t) (sensor_pm_calib*100), (uint16_t)(sensors_acc.tmp_instant*100), (uint16_t)(sensor_tmp_calib*100), (uint16_t)(sensors_acc.hum_instant*100),(uint16_t)(sensor_hum_calib*100) ); // TODO replace with fixed point calls
    

    sensors_acc.nb_val++;
#ifdef IAQ
    sensors_acc.voc += sensor_voc;
	//sensors_acc.voc_calibrated += k_voc*sensor_voc;
	sensors_acc.ohm += sensor_ohm;
#endif
	//We send only the maximum value
    if (sensors_acc.pm<sensor_pm)
	{
		sensors_acc.pm = sensor_pm;
	}
	sensors_acc.pm_instant=sensor_pm;
#ifdef MQ135
	sensors_acc.co2 += sensor_co2;
#endif
    sensors_acc.tmp += sensors_acc.tmp_instant;
    sensors_acc.hum += sensors_acc.hum_instant;
#ifdef VZ87
	sensors_acc.co2	 += vz87_co2;
	sensors_acc.raw2 += vz87_voc;
    sensors_acc.raw3 += vz87_raw;
#endif
#ifdef SENSOR_1
	sensors_acc.raw2 += sensor1;
#endif
#ifdef SENSOR_2
	sensors_acc.raw3 += sensor2;
#endif
	double voc100 = map100(sensor_voc, 450, 3000);
    double pm100 = map100(sensor_pm, 0, 75);
    double gaspolu = ( (voc100*9) + (map100(sensor_co2, 30, 150)) )/10;
	allpolu = (gaspolu+pm100)/2;

    // DEBUG test values
    //sensor_tmp = 20.7;
    //sensor_hum = 53.9;
    //sensor_voc = 1255;
    //sensor_pm = 11.1;
    //sensor_co2 = 78;
	
}
