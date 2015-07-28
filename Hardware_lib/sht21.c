/*
 * sht21.c
 *
 * Created: 04/01/2014 06:24:32
 *  Author: Xevel
 */ 
#include "sht21.h"


void sht21_init(){
    //bitSet(PORTD, 1); // pull up SDA // not needed after fix
    //bitSet(PORTD, 0); // pull up SCL // not needed after fix
    // Initialize the TWI driver at 100KHz
    TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 100000));
    
}

#define SHT21_ADDR				(0x40<<1)

#define SHT21_TEMP_HOLD			0xE3
#define SHT21_HUMIDITY_HOLD		0xE5
#define SHT21_TEMP_NO_HOLD		0xF3
#define SHT21_HUMIDITY_NO_HOLD	0xF5

// Using NO HOLD like that works but actually on the bus the last byte gives a NAK.

int16_t sht21_get(uint8_t addr){
    uint8_t Byte1, Byte2;
    uint8_t err = TWI_StartTransmission(SHT21_ADDR | TWI_ADDRESS_WRITE, 10);
    
    // Start a read session to device at address 0xA0, internal address 0xDC with a 10ms timeout
    if (err == TWI_ERROR_NoError)
    {
        TWI_SendByte(addr);
        TWI_StopTransmission();
        Delay_MS(100); // wait for the sensor to finish, BUT we could parallelize instead of waiting!!!
        
        err = TWI_StartTransmission(SHT21_ADDR | TWI_ADDRESS_READ, 10);
        if (err == TWI_ERROR_NoError)
        {
            TWI_ReceiveByte(&Byte1, false);
            TWI_ReceiveByte(&Byte2, true);
            TWI_StopTransmission();
        } else return -err;
    } else return -err;
    return ((uint16_t)Byte1)<<8 | Byte2;
}

double sht21_get_humidity(){
    return (-6.0 + 125.0 / 65536.0 * (double)(sht21_get(SHT21_HUMIDITY_NO_HOLD)));
}

double sht21_get_temperature(){
    return (-46.85 + 175.72 / 65536.0 * (double)(sht21_get(SHT21_TEMP_NO_HOLD)));
}
