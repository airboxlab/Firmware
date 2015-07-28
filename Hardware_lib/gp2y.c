/*
 * gp2y.c
 *
 * Created: 04/01/2014 06:38:59
 *  Author: Xevel
 */ 
#include "gp2y.h"

#include <util/delay.h>
#include <LUFA/Drivers/Peripheral/ADC.h>
#include "motor.h"

#define GP2Y_CHANNEL_NO 9
#define GP2Y_CHANNEL ADC_CHANNEL9

void gp2y_init(){
	// LED : set as output
	bitSet(PORTD,4); // LED OFF
	bitSet(DDRD,4);
	
	// GP2Y_output
	// Initialize the ADC driver before first use
	ADC_Init(ADC_SINGLE_CONVERSION | ADC_PRESCALE_64);
	// do a conversion to initialize the hardware
	ADC_SetupChannel(GP2Y_CHANNEL_NO);
	uint16_t dummy = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_RIGHT_ADJUSTED | GP2Y_CHANNEL);
    dummy++;  // to remove warnings about unused values
}

// TODO find out how about the dynamics of the sensor, how many does it take to settle...

uint16_t gp2y_read_val(){
    bitClear(PORTD,4); // LED ON
    _delay_us(216); // use the fact that when we init the ADC it takes 12 more adc clocks than normal
    ADC_SetupChannel(GP2Y_CHANNEL_NO);
    ADC_Init(ADC_SINGLE_CONVERSION | ADC_PRESCALE_64);
    //bitSet(PORTD,4); // TMP
    //bitClear(PORTD,4); // TMP
    uint16_t current_read = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_RIGHT_ADJUSTED | GP2Y_CHANNEL);
    bitSet(PORTD,4); // LED OFF
    ADC_Disable();
    return current_read;
}

uint16_t gp2y_get_avg(uint8_t nbr_readings, uint8_t nb_start){
	uint32_t total_reads = 0;
    
    //perform nb_start reads that will be discarded. The sensor takes some time to stabilized
    for(uint8_t i = 0; i < nb_start; i++){
        gp2y_read_val();
        _delay_us(9680); // DELAY BEFORE ANOTHER CYCLE
    }
	for(uint8_t i = 0; i < nbr_readings; i++){
		total_reads += gp2y_read_val();
        _delay_us(9680); // DELAY BEFORE ANOTHER CYCLE
    }
	return (uint16_t)(total_reads/nbr_readings);
}


// DEPRECATED
uint16_t gp2y_get_voltage(){
	uint32_t res = gp2y_get_avg(100,25);
	return (uint16_t)(res*5000/1023);
}

// DEPRECATED
double gp2y_get_pm(){
	/*motor_set(200);
	Delay_MS(2500);
	motor_set(0);
	Delay_MS(500);*/
	uint32_t res = gp2y_get_avg(100,25);
	return (double)(((res*5000/1023)*0.017)-10);
}
