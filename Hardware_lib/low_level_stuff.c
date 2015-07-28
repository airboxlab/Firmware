/*
 * low_level_stuff.c
 *
 * Created: 05/02/2014 00:06:30
 *  Author: Xevel
 */ 
#include "low_level_stuff.h"
#include <LUFA/Drivers/Peripheral/TWI.h>

void lls_timer4_init(){
    // phase and frequency correct PWM mode, top 0xFF
    OCR4C = 0xFF; // top
    TCCR4D = ( 0 << WGM41 ) | (1 << WGM40 );

    // no clock pre-scaler (synchronous)
    TCCR4B = (0 << CS43) | (0 << CS42) | (0 << CS41) | (1 << CS40);

}

void lls_twi_init(){
    // Initialize the TWI driver at 100KHz
    TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 100000));
    
}


uint8_t lls_twi_read_regs(uint8_t i2c_addr, uint8_t addr, uint8_t* data, uint8_t len){
    uint8_t err = TWI_StartTransmission( i2c_addr | TWI_ADDRESS_WRITE, 2);
    if ( err == TWI_ERROR_NoError){
        TWI_SendByte(addr);
        TWI_StopTransmission();
        err = TWI_StartTransmission(i2c_addr | TWI_ADDRESS_READ, 2);
        if ( err == TWI_ERROR_NoError){
            uint8_t i = 0;
            for (; i<len-1; i++){
                TWI_ReceiveByte(data+i, false);
            }
            TWI_ReceiveByte(data+i, true);
            TWI_StopTransmission();
        }
    }
    return err;
}


