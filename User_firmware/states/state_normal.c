/*
 * state_normal.c
 *
 * Created: 29/01/2014 18:02:40
 *  Author: Xevel
 */ 
#include "state_normal.h"
#include "states.h"
#include "post_fsm.h"
#include <Hardware_lib/time.h>
#include <Hardware_lib/led.h>
#include <Hardware_lib/power.h>
#include <Hardware_lib/sensor_board.h>
#include "../wifi_config.h"
#include "../sensor.h"
#include "../error.h"
#include <Hardware_lib/motor.h>
#include "../post.h"
#include "../util_timer.h"
#include "../sensor_calib.h"


int getFreeRam(void)
{
    extern int  __bss_end;
    extern int  *__brkval;
    int free_memory;
    if((int)__brkval == 0) {
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
    }
    else {
        free_memory = ((int)&free_memory) - ((int)__brkval);
    }

    return free_memory;
}

// TODO see what problems the timer overflow will cause and find a strategy to manage it. It happens every 49.7 days so it's not _uber_ critical, but still.



uint8_t post_res, prev_post_res;
uint32_t post_timeout_end, update_timeout_end;
uint8_t needs_to_post_room_changed; // tells if the room has changed and the post must include this information
#define POSTING  0 // states to compare post_res to.
#define NOT_POSTING  1

void state_normal_init(){
    reset_fsm();
    sensor_update();
    post_timeout_end = set_timeout(millis() + POST_PERIOD_MS);
    post_res = NOT_POSTING;
    needs_to_post_room_changed = 0;
    returned_led_state = 1;
	#ifdef UPSIDE_DOWN
    util_timer_set_accel(1);
	#endif
}


void state_normal_play(){
   // HACK do not allow all of these during posting until a clean way of aborting the FSM is coded.
    if (post_res == NOT_POSTING && usb_serial_get_nb_received() > 0){
		uint8_t res;
		char t[32];
        uint8_t c = usb_serial_get_byte();
        switch(c){
            case 'w':
#ifdef WIFI
                state_set_next(&main_fsm, &state_config);
#endif
                return;
                break;
            case 'e':
				DCALIB_PRINTF("HUM calib : %u\r\n", sensor_calib_HUM_is_done());
				DCALIB_PRINTF("TEMP calib : %u\r\n", sensor_calib_TEMP_is_done());
				DCALIB_PRINTF("PM calib : %u\r\n", sensor_calib_PM_is_done());
				DCALIB_PRINTF("k_HUM	: %u\r\n", eeprom_read_byte(calib_eeprom_k_hum_addr));
				DCALIB_PRINTF("b_HUM	: %li\r\n", (uint32_t)  round(eeprom_read_float(calib_eeprom_b_hum_addr)*100));
				DCALIB_PRINTF("k_TEMP : %u\r\n", eeprom_read_byte(calib_eeprom_k_temp_addr));
				DCALIB_PRINTF("b_TEMP : %li\r\n", (int32_t) round(eeprom_read_float(calib_eeprom_b_temp_addr)*100));
				DCALIB_PRINTF("k_PM	: %u\r\n", eeprom_read_byte(calib_eeprom_k_pm_addr));
				DCALIB_PRINTF("b_PM	: %li\r\n", (int32_t) round(eeprom_read_float(calib_eeprom_b_pm_addr)*100));
                break;
            case 'm':
                fprintf(&USBSerialStream, "MAC: %s\r\n", get_mac_str());
                break;
			case 'g':
#ifdef CALIBRATION
				sensor_calib_erase_calibration_k();
#endif
				break;
			case 'c':
				#ifdef CALIBRATION
				state_set_next(&main_fsm,&state_calibration);
				#endif
				break;
            case '-':
                println("Mem clear");
                wifi_config_mem_clear();
                error_set_post(POST_ERROR_WIFI_NO_CONFIG);
            default:
                break;
        }
    }


    if (error_post() != POST_ERROR_WIFI_NO_CONFIG){
        util_timer_enable_interrupt();
        

        // TODO update motor
        motor_set(180);// TEMP
        prev_post_res = post_res;
        // TODO integrate needs_to_post_room_changed to the flow to tell the backend that the room has changed...
        post_res = post_fsm(); // this runs the fsm until it stops then doesn't do anything until it's reset.
        
        uint32_t now = millis();

        if (prev_post_res == POSTING && post_res == NOT_POSTING){ // just finished posting
            if (post_fsm_error()){
                error_set_post(POST_ERROR_WIFI_NO_POST);
            } else {
                error_set_post(POST_ERROR_NO_ERROR);
            }
            DSENSOR_INFO_PRINTF("%lu, VOC, VOC_RAW, CO2, VZ87_VOC, VZ87_CO2, VZ87_RAW, SENSOR_1, SENSOR_2, PM*100, PM_Cal*100, TMP*100,PMP_Cal*100, HUM*100, HUM_Cal*100\r\n", now);
            sensor_update();
            update_timeout_end = set_timeout(now + SENSOR_UPDATE_PERIOD_MS);

            needs_to_post_room_changed = 0;
        }
        
        // when a change of room is detected, the timer before next post is put to at least
        // 1 min to avoid posting old data to the new room.
#ifdef UPSIDE_DOWN
        if ((post_res == NOT_POSTING) && upsidedown_detected ){ // posting must not be happening right now and an upside down condition must have been detected
            upsidedown_detected = 0;
            post_timeout_end = MAX(post_timeout_end, set_timeout(now + 60000));
            needs_to_post_room_changed = 1;
        }
#endif
#ifdef DOUBLE_TAP_ENABLED
		if ((post_res == NOT_POSTING) && double_tap_detected){
			reset_fsm();
		}
#endif
        if ( now > post_timeout_end && (post_res == NOT_POSTING)){ 
            reset_fsm();
            post_timeout_end = set_timeout(now + POST_PERIOD_MS);
        } else if (now > update_timeout_end && (post_res == NOT_POSTING)){
            sensor_update();
            update_timeout_end = set_timeout(now + SENSOR_UPDATE_PERIOD_MS);
        }
		

    } else {
        // The wifi has not been configured. This case is special since it makes it impossible to do anything interesting,
        // aside from configuring it.
        
        util_timer_disable_interrupt();

        led_set(0, 0, 100);
    }

}

void state_normal_end(){
    // TODO cleanup stuff, stop what is running, etc
    
    #ifdef UPSIDE_DOWN
    util_timer_set_accel(0);
	#endif
    util_timer_disable_interrupt();

    // TODO find a clean way to abort the FSM
}

//***********************************************************
//                       Helpers
//***********************************************************




