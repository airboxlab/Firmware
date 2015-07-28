/*
 * ADC.h
 *
 * Created: 09/10/2013 23:36:41
 *  Author: Xevel
 */ 


#ifndef ADC_H_
#define ADC_H_


#include "common.h"

void init_adc(){
    // set prescaler to 64
    bitSet(ADCSRA, ADPS2);
    bitSet(ADCSRA, ADPS1);
    // ref is VCC
    // enable ADC
    bitSet(ADCSRA, ADEN);
    
    //init ADC hardware by running a conversion
    bitSet(ADCSRA, ADSC);
    while ( bit_is_clear(ADCSRA, ADIF));
    bitSet(ADCSRA, ADIF);
}


#endif /* ADC_H_ */