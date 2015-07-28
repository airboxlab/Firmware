/*
* util_timer.c
*
* Created: 07/02/2014 03:28:09
*  Author: Xevel
*/
#include "util_timer.h"

#include <Hardware_lib/led.h>
#include <Hardware_lib/mma8453q.h>
#include "error.h"
#include "sensor.h"
#include "states/post_fsm.h"
#ifdef UPSIDE_DOWN
volatile uint8_t upsidedown_detected; // flag that tells the rest of the world if an upsidedown gesture has been detected and not used yet.
#endif
volatile uint8_t double_tap_detected;

#ifdef UPSIDE_DOWN
uint8_t util_timer_check_accel;
#endif
uint8_t util_timer_in_config;
uint8_t util_timer_in_calib;


// When we have detected that the device has been turned upside down, led blinking is
// controlled with a counter that is decremented. We look at a specific bit to know if we
// want to light up the led or not, which gives a 50% pulse width ratio for cheap.
// The counter is tested to be non null to know if we are in a case where this led pattern is needed.
// To blink once, we must go from MASK*2-1 to 0. The doubling is needed to have a full period.
// Decrementing after the non-null test takes care of the "-1", therefore to blink once the counter should be
// initialized to MASK*2, and its last time should be before it becomes null.
#ifdef UPSIDE_DOWN
uint8_t accel_blink_count;
#define ACCEL_BLINK_MASK        0x08 //  bit 3 switches every 8 decrements, so the blink time is 8 x timer_period up, then the same time down.
#define ACCEL_BLINK_NB_BLINK    3
#define ACCEL_BLINK_START_VAL   (ACCEL_BLINK_NB_BLINK * 2 * ACCEL_BLINK_MASK)
#endif


#ifdef DOUBLE_TAP_ENABLED
uint8_t tap_blink_count;
#define TAP_BLINK_MASK			0x08
#define TAP_BLINK_NB_LINK		1
#define TAP_BLINK_START_VAL		(TAP_BLINK_NB_LINK * 2 * TAP_BLINK_MASK)
#endif
// To avoid false detections caused by shaking, we need to filter the output.
// For that, check that we have had at least X consecutive values that fell on the same side of the test.
// LED feedback is instantaneous, but the change is signaled to the back-end at the next post.
// We are only interested is the transition Upward > Downward, that's the gesture we detect.
#ifdef UPSIDE_DOWN
uint8_t accel_state_current;
uint8_t accel_upsidedown_prev; // no need to init
uint8_t accel_nb_idem;
#define ACCEL_NB_CONSECUTIVE_NEEDED  20
#endif
// To make the leds glow, we use a variable that will oscillate between a top and bottom value.
// At WOOWOO_TOP, the leds have their full assigned value. To bring back the leds into 0-255,
// we divide them by the TOP value, which is made fast by defining it as a power of 2.
// The difference between top and bottom gives how many timer periods it takes to go from min to max,
// so the whole glow period is twice that.
#define LED_WOOWOO_TOP          32
#define LED_WOOWOO_BOTTOM       3
uint8_t led_woowoo_factor=LED_WOOWOO_BOTTOM; //
uint8_t led_woowoo_dec=1; // does not need to be initialized :) It starts at 0 which makes the factor (which starts at 0 too) go up, after that it's self-regulating
// we could init led_woowoo stuff only to make sure that the blinking will start at a specified power level, but we don't really care.

#ifdef FADE_LED
	uint8_t r_prev=0;
	uint8_t g_prev=0;
	uint8_t b_prev=0;

	uint8_t r_next;
	uint8_t g_next;
	uint8_t b_next;
	
	uint8_t booting=1;
#endif

	uint8_t fading_out=0;
	uint8_t fading_in=0;

#define LED_WOOWOO_BOTTOM_FADE 8

void util_timer_init(){
	// CTC, top = OCR3A, clock/256 pre-scaler => 1 timer tick every 16us
	TCCR3A = (0 << WGM31) | (0 << WGM30);
	TCCR3B =  (0 << WGM33) | (1 << WGM32) | (1 << CS12) | (0 << CS11) | (0 << CS10);
	OCR3A = 3125;  // ISR every 50ms
}

void util_timer_enable_interrupt(){
	TIMSK3 = (1 << OCIE3A);  // enable Timer Compare A interrupt
}

void util_timer_disable_interrupt(){
	TIMSK3 = 0;
}
#ifdef UPSIDE_DOWN
void util_timer_set_accel(uint8_t b){
	util_timer_check_accel = b;
	accel_nb_idem = 0;
	accel_state_current = 1; // init as if it's upside down, so there is no risk of immediate upright => upsidedown transition.
}
#endif
void util_timer_set_config(uint8_t b){
	util_timer_in_config = b;
}
#ifdef CALIBRATION
void util_timer_set_calib(uint8_t b){
	util_timer_in_calib = b;
}
#endif




void util_timer_isr(){
	// we don't want auto interruptions, but we want the millis timer to continue ticking, so we disable ourselves then re-enable global interrupts
	uint8_t TIMSK3_old = TIMSK3;
	TIMSK3 = 0;
	TCNT3 = 0; // reset the counter so that next execution is not anticipated
	bitClear( TIFR3, OCF3A ); // clear the interrupt flag in case for whatever reason the timer has ticked again while we were doing the few previous lines

	sei();

	uint8_t r = 0, g = 0, b = 0;

#ifdef UPSIDE_DOWN
	if (util_timer_check_accel){
		uint8_t upsidedown = mma8435q_getAccY() < -200;  // 512 = + 2g, -512 = -2g
		if ( upsidedown == accel_upsidedown_prev ){
			if (accel_nb_idem < ACCEL_NB_CONSECUTIVE_NEEDED){
				accel_nb_idem++;
			}
			} else {
			accel_nb_idem = 0;
		}
		if ( accel_nb_idem == ACCEL_NB_CONSECUTIVE_NEEDED && upsidedown != accel_state_current ){
			// change!
			accel_state_current = upsidedown;
			if (upsidedown){ // we have detected that we really are upside down, and have been for long enough
				accel_blink_count = ACCEL_BLINK_START_VAL; // start led blinking
				upsidedown_detected = 1; // communicate the fact that it's upside down to the rest of the world. This flag has to be cleared by the consumer code
			}
		}
		accel_upsidedown_prev = upsidedown;
	}
#endif

#ifdef DOUBLE_TAP_ENABLED
	if (mma8435q_check_doubletap())
	{
		tap_blink_count=TAP_BLINK_START_VAL;
		double_tap_detected = 1;
	}
#endif


#ifdef UPSIDE_DOWN
	if (accel_blink_count){
		if (--+ & ACCEL_BLINK_MASK){
			b = 100;
		}
	}
	else
#endif
#ifdef DOUBLE_TAP_ENABLED
	if(tap_blink_count)
#else
	if (0)
#endif
	{
#ifdef DOUBLE_TAP_ENABLED
		if (--tap_blink_count & TAP_BLINK_MASK)
#else
		if (0)
#endif
		{
			r=255;
			b=140;
		}
	}
	else
	{
			// update leds
		if ((!util_timer_in_config && !double_tap_detected && !util_timer_in_calib) && returned_led_state == 0)
			{
				r=g=b=0;
				if (error_post())
				{
					b=2;
				}
			}
			
		else if (util_timer_in_config){
			b = 110;
		}
		else if(util_timer_in_calib){
			g= 200;
			b= 100;
		} else {
			if (!fading_in && !fading_out)
			{
				if (sensor_voc > 1000 || sensor_pm > 13) {					
					r = 255;
				
					if (sensor_voc > 1700 || sensor_pm > 14){
						#ifdef COLOR_NEW_GEN
							g = 30;
						#else
							g = 0;
						#endif
						} else if (sensor_voc > 1300 || sensor_pm > 16){
						#ifdef COLOR_NEW_GEN
							g = 30;
						#else
							g = 10;
						#endif
						} else {
						#ifdef COLOR_NEW_GEN
							g = 30;
						#else
							g = 20;// even less red
						#endif
					}
					} else {
					#ifdef COLOR_NEW_GEN
							g= 255;
							b= 255;
					#else
							g = 90;
					#endif
				}
				#ifdef FADE_LED
				if (!error_post() && (r_prev != r || g_prev !=g))
				{
					if (booting)
					{
						fading_in=1;
					}
					else 
					{
						fading_out=1;
						r_next=r; 
						g_next=g;
						b_next=b; 
					}
				}
				#endif				
			}
		}
	}
	if (util_timer_in_config || util_timer_in_calib || error_post() )
	{
		// glowing in r,g,b if error_post
		
		if (!led_woowoo_dec)
		{
			led_woowoo_factor++;
		} 
		else {
			led_woowoo_factor--;
		}
		if (led_woowoo_factor == LED_WOOWOO_TOP)
		{
			led_woowoo_dec = 1;
		} else if (led_woowoo_factor == LED_WOOWOO_BOTTOM)
		{
			led_woowoo_dec = 0;
		}
		r = ((uint16_t)r * led_woowoo_factor) / LED_WOOWOO_TOP;
		g = ((uint16_t)g * led_woowoo_factor) / LED_WOOWOO_TOP;
		b = ((uint16_t)b * led_woowoo_factor) / LED_WOOWOO_TOP;
	}

#ifdef FADE_LED
	else if (fading_out) 
	{
		led_woowoo_factor--;
		r=r_prev*led_woowoo_factor/LED_WOOWOO_TOP;
		g=g_prev*led_woowoo_factor/LED_WOOWOO_TOP;
		b=b_prev*led_woowoo_factor/LED_WOOWOO_TOP;
		if (led_woowoo_factor==LED_WOOWOO_BOTTOM_FADE)
		{
			fading_out=0;
			fading_in=1;
		}
	}
	else if (fading_in)
	{
		led_woowoo_factor++;
		r=r_next*led_woowoo_factor/LED_WOOWOO_TOP;
		g=g_next*led_woowoo_factor/LED_WOOWOO_TOP;
		b=b_next*led_woowoo_factor/LED_WOOWOO_TOP;
		if (led_woowoo_factor==LED_WOOWOO_TOP)
		{
			fading_in=0;
			r_prev=r;
			g_prev=g;
			b_prev=b;
		}
		if (booting)
		{
			booting=0;
		}
	}
#endif
		
	led_set(r,g,b);
	TIMSK3 = TIMSK3_old; // restore the state of the COMPA interruption enable flag
}



ISR(TIMER3_COMPA_vect, ISR_BLOCK){
	util_timer_isr();
}


