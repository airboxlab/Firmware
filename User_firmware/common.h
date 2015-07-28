/*
 * common.h
 *
 * Created: 04/01/2014 05:40:10
 *  Author: Xevel
 */ 


#ifndef USER_COMMON_H_
#define USER_COMMON_H_

    // THIS FILE SHOULD BE INCLUDED AT THE TOP OF EVERY HEADER OF THIS PROJECT
    // It takes care of a few includes that every code might use (like standard integers...) and

    //****************   interrrupters  **********************
    #define WIFI			    // when commented, disables wifi support to save space for debugging
	#define SENSORBOARD         // when commented, disables sensor board if not connected
    #define ONBOARD_STUFF       // when commented, disables on-board sensors and motor
	#define DOUBLE_TAP_ENABLED // when commented, disables send when double tap
	#define FADE_LED			// when commented, disables led fade in/out when color change 
	//#define CALIBRATION  			// when commented, disables calibration state
	//#define COLOR_NEW_GEN		//when commented, use RGB, else Orange/Blue
	//#define DEV               // when commented, disables developer mode. This must be commented for releases.
	//#define BATTERY	//when commented, disable battery management
	//#define UPSIDE_DOWN			//when commented, disable Upside Down detection
    //********************************************************

	#define BitIsSet(adr,bit) (adr & (1<<bit))

    #include <Hardware_lib/common.h>
	#include <CC3000/CC3000.h>
    #include "usb_serial.h"
    #include "debug.h"

#endif /* COMMON_H_ */