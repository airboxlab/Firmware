/*
 * cc2dxx.c
 *
 * Created: 04/01/2014 06:18:50
 *  Author: Xevel
 */ 
#include "cc2dxx.h"

#define CC2DXX_STATUS_VALID 0x00
#define CC2DXX_STATUS_STALE 0x01
#define CC2DXX_STATUS_COMMAND_MODE 0x02

uint16_t cc2dxx_humidity;
uint16_t cc2dxx_temperature;
uint8_t cc2dxx_status;

void cc2dxx_init(){
    // Initialize the TWI driver at 100KHz
    TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 100000));
}

uint8_t cc2dxx_get(){
    uint8_t bytes[4];
    
    uint8_t err = TWI_StartTransmission(CC2DXX_ADDR | TWI_ADDRESS_WRITE, 10);
    
    // Start a read session to device at address 0xA0, internal address 0xDC with a 10ms timeout
    if (err == TWI_ERROR_NoError)
    {
        TWI_StopTransmission();
        Delay_MS(170); // wait for the sensor to finish
        
        err = TWI_StartTransmission(CC2DXX_ADDR | TWI_ADDRESS_READ, 10);
        if (err == TWI_ERROR_NoError)
        {
            TWI_ReceiveByte(bytes+0, false);
            TWI_ReceiveByte(bytes+1, false);
            TWI_ReceiveByte(bytes+2, false);
            TWI_ReceiveByte(bytes+3, true);
            TWI_StopTransmission();
        } else return err;
    } else return err;
    cc2dxx_status = bytes[0] >> 6;
    cc2dxx_humidity = (((uint16_t)bytes[0] & 0x3F)<<8) | bytes[1];
    cc2dxx_temperature = (((uint16_t)bytes[2])<<6) | (bytes[3]>>2);
    return TWI_ERROR_NoError;
}


// humidity in %RH
double cc2dxx_get_humidity_no_update(){
    return (double)cc2dxx_humidity / 163.84 ; // same as /16384.0*100.0 but smaller. 
}

// temp in °C
double cc2dxx_get_temperature_no_update(){
    return (double)cc2dxx_temperature * 0.01007 - 40.0; // same as /16384.0*165.0 but smaller
}

// humidity in %RH
double cc2dxx_get_humidity(){
    cc2dxx_get();
    return cc2dxx_get_humidity_no_update();
}

// temp in °C
double cc2dxx_get_temperature(){
    cc2dxx_get();
    return cc2dxx_get_temperature_no_update();
}


