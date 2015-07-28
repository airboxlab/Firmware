/*
 * state_normal.h
 *
 * Created: 29/01/2014 17:55:57
 *  Author: Xevel
 */ 


#ifndef STATE_NORMAL_H_
#define STATE_NORMAL_H_
    #include "../common.h"

    // This is the normal state of operation.
    // It updates the sensor values, manages the fan and posts every few minutes.
    // It also checks the accelerometer to know if the device has been put upside down (this gesture indicates changing room).
    // For the LEDs, it asks the server if they should be ON or not. This affects only the normal operation, not the gesture or other modes.

    #ifdef DEV
        #define POST_PERIOD_MS          (60000) // time between two posts, in ms
        #define SENSOR_UPDATE_PERIOD_MS (3000) // time between two sensor updates
    #else
        #define POST_PERIOD_MS          (300000) // time between two posts, in ms // 5min = 300s
        #define SENSOR_UPDATE_PERIOD_MS (5000)
    #endif


    void state_normal_init();
    void state_normal_play();
    void state_normal_end();

#endif /* STATE_NORMAL_H_ */