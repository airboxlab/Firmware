/*
 * led.c
 *
 * Created: 04/01/2014 06:34:52
 *  Author: Xevel
 */ 
#include "led.h"
#include "low_level_stuff.h"




void led_init(){
	
	// initialize timers for PWM 8bits
	
	//**** green
    // Timer4 is shared with the motor, make sure it is initialized
	lls_timer4_init();
	
	// Enable PWM modes, inverting
	TCCR4C |= (1 << COM4D1) | (1 << COM4D0) | ( 1 << PWM4D);

	OCR4D = 0; // start OFF
	
	//**** red and blue
	// phase correct PWM mode, inverting, top 0xFF, no clock pre-scaler
    TCCR1B = ( 0 << WGM13 ) | ( 0 << WGM12 ) | ( 0 << CS12 ) | ( 0 << CS11 ) | ( 1 << CS10 ) ;
    TCCR1A |=    ( 1 << COM1A1 ) | ( 1 << COM1A0 ) | ( 1 << COM1B1 ) | ( 1 << COM1B0 )
            | ( 0 << WGM11 ) | ( 1 << WGM10 );

	OCR1A = 0;  // start OFF
	OCR1B = 0;  // start OFF
	
	// set as outputs
	bitSet(DDRD,7);
	bitSet(DDRB,5);
	bitSet(DDRB,6);
}

void led_set(uint8_t red, uint8_t green, uint8_t blue){
	OCR1B = red;
	OCR1A = blue;
	OCR4D = green;
}

void led_clear(){
	led_set(0,0,0);
}

