/*
 * vz87.c
 *
 * Created: 6/2/2014 6:42:02 PM
 *  Author: Tony
 */ 

#include "vz87.h"

#include "low_level_stuff.h"



void vz87_init(){
	lls_twi_init();
}

//Return the resistance & voc value
vz87_result vz87_get_value(){
	vz87_result res;
	uint8_t Byte1,Byte2,Byte3,Byte4,Byte5,Byte6;
	uint8_t err = TWI_StartTransmission(VZ87_I2C_ADDR | TWI_ADDRESS_WRITE,10);
	if (err == TWI_ERROR_NoError){
		TWI_SendByte(0x09);
		TWI_SendByte(0x00);
		TWI_SendByte(0x00);
		TWI_StopTransmission();
		Delay_MS(200);
		err = TWI_StartTransmission(VZ87_I2C_ADDR | TWI_ADDRESS_READ, 10);
		if ( err == TWI_ERROR_NoError){
			TWI_ReceiveByte(&Byte1, false);
			TWI_ReceiveByte(&Byte2, false);
			TWI_ReceiveByte(&Byte3, false);
			TWI_ReceiveByte(&Byte4, false);
			TWI_ReceiveByte(&Byte5, false);
			TWI_ReceiveByte(&Byte6, true);
			TWI_StopTransmission();
		}
	}
	res.vz87_ohm =((uint32_t) Byte6<<16 | (uint16_t) Byte5<<8 | Byte4);
	res.vz87_voc_short = Byte2;
	res.vz87_voc_long  = Byte3;
	res.vz87_co2 = Byte1;
	return res;
}