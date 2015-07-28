/*
AirBoxLab v2

User firmware - this firmware is intended to be used for the units sent in the wild.

(c) 2013-2014 Airboxlab
    Nicolas SAUNGIER
    Inouk BOURGON

*/

#include "User_firmware.h"
#include "usb_serial.h"
#include <Hardware_lib/led.h>
#include <Hardware_lib/time.h>
#include "error.h"
#include "sensor.h"
#include "states/states.h"
#include "states/post_fsm.h"
#include "util_timer.h"
#include "states/state_normal.h"
#include "sensor_calib.h"

// flags to enable or disable various parts of the firmware are in common.h
#ifdef ONBOARD_STUFF
    #include <Hardware_lib/motor.h>
    #include <Hardware_lib/gp2y.h>
    #include <Hardware_lib/cc2dxx.h>
    #include <Hardware_lib/mma8453q.h>
    #include <Hardware_lib/power.h>
//    #include <Hardware_lib/charger.h>
#endif
#ifdef SENSORBOARD
#ifdef IAQENGINE
    #include <Hardware_lib/iaqengine.h>
#endif
#ifdef VZ87
	#include <Hardware_lib/vz87.h>
#endif
    #include <Hardware_lib/sensor_board.h>
#endif
#ifdef WIFI
    #include "wifi_config.h"
    #include "post.h"
#endif

// checks if the CC3000 has been updated to v1.24
uint8_t wifi_check_version(){
    uint8_t major, minor;
    return cc3000_getFirmwareVersion(&major, &minor) && major == 1 && (minor == 24 || minor == 28) ;
}



void setup_hardware(void){
    // Disable watchdog if enabled by bootloader/fuses
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    // Disable clock division
    clock_prescale_set(clock_div_1);

    // enable global interrupts
    sei();
    // Hardware Initialization
    usb_serial_init();
    led_init();
    led_set(0,4,3);
	motor_init();
	motor_set(230);
    util_timer_init();
	

    #ifdef WIFI
    if (!cc3000_init()){
        error_set_internal(INTERNAL_ERROR_CC3000_NO_INIT);
        return;
    }
    if (!wifi_check_version()){
        error_set_internal(INTERNAL_ERROR_CC3000_VERSION);
        return;
    }
    if ( wifi_config_load() ){
        if (wifi_set_profile(wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode) != CC3000_SUCCESS){
            error_set_internal(INTERNAL_ERROR_CC3000_PROFILE);
            return;
        }
    } else {
        error_set_post(POST_ERROR_WIFI_NO_CONFIG);
    }
    post_init();
    cc3000_stop();
    #endif

    // TODO check that they work and set the internal error if they don't!!!
    #ifdef ONBOARD_STUFF
    gp2y_init(); // TODO check that the value is not absurd
    cc2dxx_init(); // TODO check that it talks
    mma8435q_init(); // TODO check that it talks
    #endif
    
    #ifdef SENSORBOARD
	vz87_init();
    iaqengine_init(); // TODO check that it talks
    sensor_board_init(); // TODO check that it talks
    sensor_board_heater_on(true); // TODO re-read to check that it has been applied as needed
    #endif
    
	sensor_calib_load_k(100);
	
    millis_init();
}


int main(void){
	setup_hardware();
    state_set_next(&main_fsm, &state_normal);
    for (;;){
#ifdef BATTERY
		//UDINT - SUSPI : Interruption when already connected
		//USBSTA - VBUS : When started not connected
		if((bitIsSet(UDINT,SUSPI) || !bitIsSet(USBSTA,VBUS)) && power_battery_no_power()){
				state_set_next(&main_fsm,&state_power_saving);
		}
#endif
        if (error_internal()){
            state_set_next(&main_fsm, &state_error);
        }
        state_change(&main_fsm);

        main_fsm.current_state->play();

        usb_serial_task();
        Delay_MS(10);
    }
}


