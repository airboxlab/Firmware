/*
 * time.c
 *
 * Created: 28/01/2014 15:28:58
 *  Author: Xevel
 */ 
#include "time.h"
volatile uint32_t timer_millis; // should overflow in ~50 days.

void millis_init(){
    // Start the global timer with Timer0
    TCCR0A = (1 << WGM01); // CTC mode (Clear Timer on Compare match)
    TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00); // clock/64 pre-scaler => 1 clock tick every 4us
    OCR0A = 0xFA;  // ticks every 1ms
    TIMSK0 |= (1 << OCIE0A);  // enable Timer Compare A interrupt
}

uint32_t millis(){
    cli();
    uint32_t res = timer_millis;
    sei();
    return res;
}

uint32_t millis_reset(){
    cli();
    uint32_t res = timer_millis;
    timer_millis = 0;
    TCNT0 = 0;
    sei();
    return res;
}

long set_timeout(long timeout)
{
	if (timeout>UINT32_MAX)
	{
		return timeout-UINT32_MAX;
	}
	else return timeout;
}

// global timer
ISR(TIMER0_COMPA_vect, ISR_BLOCK){
    timer_millis++;
}
