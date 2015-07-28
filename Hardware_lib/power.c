/*
 * power.c
 *
 * Created: 04/01/2014 06:30:17
 *  Author: Xevel
 */ 
#include "power.h"
#define POWER_CHANNEL_NO 1
#define POWER_CHANNEL ADC_CHANNEL1
#define POWER_LIMIT 3500


void power_init(){
    // Initialize the ADC driver before first use
    ADC_Init(ADC_SINGLE_CONVERSION | ADC_PRESCALE_64);
    // do a conversion to initialize the hardware
    ADC_SetupChannel(0);
    uint16_t dummy = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_RIGHT_ADJUSTED | POWER_CHANNEL);
    dummy++; // to remove warnings about unused values
}



uint16_t power_battery_get_raw(){
    // Must setup the ADC channel to read beforehand
    ADC_SetupChannel(0);
    // Perform a single conversion
    return ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_RIGHT_ADJUSTED | POWER_CHANNEL);
}

//voltage in mV
uint16_t power_battery_get_voltage(){
    uint32_t res = power_battery_get_raw();
    return (uint16_t)(res*5100/1023);
}

//Check the voltage of the battery when not charging
uint8_t power_battery_no_power(){
		power_init();
		return (power_battery_get_voltage()<POWER_LIMIT);
}
