/*
 * iaqengine.c
 *
 * Created: 04/01/2014 06:38:11
 *  Author: Xevel
 */ 
#include "iaqengine.h"
#include "low_level_stuff.h"



void iaqengine_init(){
    lls_twi_init();
}

//Return the resistance & voc value
iaqengine_result iaqengine_get_value(){
	iaqengine_result res;
	uint8_t Byte1,Byte2,Byte4,Byte5,Byte6;
    uint8_t err = TWI_StartTransmission(IAQENGINE_I2C_ADDR | TWI_ADDRESS_READ, 10);
    if ( err == TWI_ERROR_NoError){
	    // Read 7 bytes, acknowledge after the seventh byte is received
		// ppm value
	    TWI_ReceiveByte(&Byte1, false);
		TWI_ReceiveByte(&Byte2, false);
		//No need (flag status and 0)
		TWI_ReceiveByte(&Byte4, false);
		TWI_ReceiveByte(&Byte4, false);
		//Resistance value start here
		TWI_ReceiveByte(&Byte4, false);
		TWI_ReceiveByte(&Byte5, false);
	    TWI_ReceiveByte(&Byte6, true);
	    TWI_StopTransmission();
    }//TO DO : handle error
	res.iaqengine_ohm =((uint32_t) Byte4)<<16 | ((uint16_t) Byte5) << 8 | Byte6;
	res.iaqengine_voc = ((uint16_t)Byte1)<<8 | Byte2;
    return res;
}
	

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        