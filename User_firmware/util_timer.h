/*
 * util_timer.h
 *
 * Created: 07/02/2014 03:28:41
 *  Author: Xevel
 */ 


#ifndef UTIL_TIMER_H_
#define UTIL_TIMER_H_
    #include "common.h"

    extern volatile uint8_t upsidedown_detected; // tells if an upsidedown gesture has been detected and not used yet.
	extern volatile uint8_t double_tap_detected;

    // The 'util timer' is a timer that takes care of LEDs and the accelerometer in cases where they have to be updated regularly.
    // it is intended to be enabled only when posting normally or in config mode. Any other case, it should be off and LEDs should be managed directly.


    void util_timer_init();
    void util_timer_enable_interrupt();
    void util_timer_disable_interrupt();

    void util_timer_set_accel(uint8_t b);
    void util_timer_set_config(uint8_t b);
    void util_timer_set_calib(uint8_t b);
	
    uint8_t util_timer_is_upsidedown();
    void util_timer_isr();

#endif /* UTIL_TIMER.H_H_ */