/*
 * motor.c
 *
 * Created: 04/01/2014 06:32:26
 *  Author: Xevel
 */ 
#include "motor.h"
#include "low_level_stuff.h"

void motor_init(){
    
    // Timer4 is shared with LEDs, make sure it is initialized
    lls_timer4_init();

    // Enable PWM modes, non inverting
    TCCR4A |= (1 << COM4A1) | (0 << COM4A0) | ( 1 << PWM4A);

    OCR4A = 0; // motor OFF
    
    // set as output
    bitSet(DDRC, 7);
}

void motor_set(uint8_t pwm){
    OCR4A = pwm;
}

uint8_t motor_get(){
    return OCR4A;
}
