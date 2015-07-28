/*
 * sensor.h
 *
 * Created: 04/01/2014 05:32:48
 *  Author: Xevel
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_
    #include "common.h"

    typedef struct {
        uint16_t    nb_val;
        double      tmp;
		double		tmp_instant;
        double      hum;
		double		hum_instant;
        uint32_t    voc;
        double      pm;
		double		pm_instant;
        uint32_t    co2; //Used also for vz27
		uint32_t	ohm;
		uint16_t	raw2; //VOC for vz87, sensor1
		uint32_t	raw3; //RAW for vz87, sensor2
    } sensors_acc_t;

    extern double           sensor_pm;
    extern sensors_acc_t    sensors_acc;
    extern double           allpolu;
	extern uint16_t			sensor_voc;

    void sensor_update();
    void sensors_reset_acc();


#endif /* SENSOR_H_ */