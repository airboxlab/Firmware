/*
 * state_error.c
 *
 * Created: 29/01/2014 21:10:25
 *  Author: Xevel
 */ 
#include "state_error.h"
#include <Hardware_lib/led.h>
#include <Hardware_lib/motor.h>
#include <Hardware_lib/sensor_board.h>
#include "states.h"
#include "../error.h"


void state_error_init(){
    led_set(255, 5, 5); // pink
    motor_set(0);
    
    if (0){ // TODO if sensor board has been init
        sensor_board_heater_on(false);
    }
    // flush input buffer
    while (usb_serial_get_nb_received()){
        usb_serial_get_byte();
    }
}

void state_error_play(){
    if(usb_serial_get_nb_received()){
        uint8_t c = usb_serial_get_byte();
        
        fprintf( &USBSerialStream, "ERR int:%u, ERR post:%u\r\n", error_internal(), error_post() );

        // These commands are there to make troubleshooting easier, but are not advertised anywhere to the user.
        switch (c){
            case 'i': // clear internal error
                error_set_internal(INTERNAL_ERROR_NO_ERROR);
                break;
            case 'p': // clear post error
                error_set_post(POST_ERROR_NO_ERROR);
                break;
            case 'r': // Return to normal state
                state_set_next(&main_fsm, &state_normal);
                break;
            default:
                break;
        }

    }
}


void state_error_end(){
    //Nobody ever gets out ! (at least not normally)
}



//***********************************************************
//                       Helpers
//***********************************************************



