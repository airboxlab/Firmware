/*
 * MQ135.h
 *
 * Created: 05/09/2013 08:01:40
 *  Author: Xevel
 */ 


#ifndef MQ135_H_
#define MQ135_H_
#include "common.h"
#include "adc.h"

/*
AirBoxLab  v2.0 Sensor Board

MQ135

Pins :
MQ135 output: PA0 (ADC0): 
Heater on/off: PB1

STATUS : OK
*/

#define MQ135_CHANNEL_NO 0
#define MQ135_CHANNEL ADC_CHANNEL1

void mq135_init(){
    // set ON/OFF as output, it's OFF at start.
    bitSet(DDRB, 1);
        
    bitClear(PORTA, 0); // set ADC pin as input
    bitSet(DIDR0, 0); // disable logic input on the ADC pin, it won't be needed and it saves some power...

    init_adc();
}

uint16_t mq135_get_raw(){
    
    // select ADC channel 0, with VCC as ref, keep it right-adjusted.
    ADMUX = 0;

    // start conversion
    bitSet(ADCSRA, ADSC);

    while ( bit_is_clear(ADCSRA, ADIF));     //wait for the end of the conversion
    bitSet(ADCSRA, ADIF); // clear interrupt flag before proceeding
    
    // read value and return it
    return ADC;
}


uint16_t mq135_get_voltage(uint8_t nb_samples){
    uint32_t res = 0;
    for( uint8_t i = 0; i<nb_samples; i++){
        uint32_t raw = mq135_get_raw();
        res += (uint16_t)(raw*5000/1023);
    }
    return res/nb_samples;
}

void mq135_heater(uint8_t on){
    if (on){
        bitSet(PORTB, 1);
    } else {
        bitClear(PORTB, 1);
    }
}


#endif /* MQ135_H_ */