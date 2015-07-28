/*
 * sensor_board.c
 *
 * Created: 04/01/2014 06:26:30
 *  Author: Xevel
 */ 
#include "sensor_board.h"
#include "low_level_stuff.h"

// See the code of ATtinyI2CSlave for implementation details (there are some gotchas!!!)
// Writing data to the registers of the sensor boards MUST be done in one transmission, the first char being the address.
// Reading data can be done in any number of transmission of any number of bytes, they are treated individually anyway.


#define SENSOR_BOARD_ATTINY_ADDR    (42 << 1) // addr is shifted left to make room for read/write bit
#define SENSOR_BOARD_IDENTIFIER     0x77 // expected value in REG_IDENTIFIER

// registers
#define SENSOR_BOARD_REG_IDENTIFIER      0x00 // 1 byte  (R) identifier used to test that the communication is OK. Only 100% fixed value across revisions and runs (see common.h).
#define SENSOR_BOARD_REG_VERSION         0x01 // 1 byte  (R) code version, in case we need to update one day...
#define SENSOR_BOARD_REG_OSCCAL          0x02 // 1 byte (RW) value of OSCCAL register
#define SENSOR_BOARD_REG_TEMP            0x03 // 2 bytes (R) ADC value (10 bits) of internal temp sensor. See MCU datasheet for interpretation. [DOES NOT WORK NOW]
#define SENSOR_BOARD_REG_MQ135           0x05 // 2 bytes (R) voltage value of the MQ-135 in mV, assuming power is 5.00V
#define SENSOR_BOARD_REG_HEATER_ON       0x07 // 1 byte (RW) state of the MQ-135 heater. 0 is OFF, any other value is ON. Default: 0
#define SENSOR_BOARD_REG_NB_SAMPLES      0x08 // 1 byte (RW) number of samples the MQ135 value is computed on.
#define SENSOR_BOARD_REG_UPDATE          0x09 // 1 byte (RW) write anything non-zero to trigger an update of the MQ135 and temp values (cleared when completed).

#define SENSOR_BOARD_REG_SENSOR_1		 0x05 // 2 byte (R) voltage value of sensor 1 in mV, assuming power is 5.00V
#define SENSOR_BOARD_REG_SENSOR_2		 0x0A // 2 byte (R) voltage value of sensor 2 in mV, assuming power is 5.00V


void sensor_board_init(){
    lls_twi_init();
}


uint8_t sensor_board_write_reg(const uint8_t addr, const uint8_t data){
    uint8_t err = TWI_StartTransmission( SENSOR_BOARD_ATTINY_ADDR | TWI_ADDRESS_WRITE, 10);
    if ( err == TWI_ERROR_NoError){
        TWI_SendByte(addr);
        TWI_SendByte(data);
        TWI_StopTransmission();
    
        // a dummy request is needed to validate the write...
        uint8_t dummy;
        err = TWI_StartTransmission(SENSOR_BOARD_ATTINY_ADDR | TWI_ADDRESS_READ, 10);
        if ( err == TWI_ERROR_NoError){
            TWI_ReceiveByte(&dummy, true);
            TWI_StopTransmission();
        }
    }
    return err;
}

uint8_t sensor_board_update(){
    return sensor_board_write_reg(SENSOR_BOARD_REG_UPDATE, 1);
}

uint8_t sensor_board_heater_on(uint8_t turn_on){
    return sensor_board_write_reg(SENSOR_BOARD_REG_HEATER_ON, turn_on);
}

uint8_t sensor_board_heater_is_on(uint8_t *res){
    sensor_board_update();
    return lls_twi_read_regs(SENSOR_BOARD_ATTINY_ADDR, SENSOR_BOARD_REG_HEATER_ON, res, 1);
}

uint8_t sensor_board_get_mq135(uint16_t* val){
    uint8_t data[2];
    sensor_board_update();
    uint8_t err = lls_twi_read_regs(SENSOR_BOARD_ATTINY_ADDR, SENSOR_BOARD_REG_MQ135, data, 2);
    if (err == TWI_ERROR_NoError){
        *val = ((uint16_t)(data[0]))<<8 | data[1];
    }
    return err;
}

uint8_t sensor_board_get_sensor1(uint16_t* val)
{
	uint8_t data[2];
	sensor_board_update();
	uint8_t err = lls_twi_read_regs(SENSOR_BOARD_ATTINY_ADDR, SENSOR_BOARD_REG_SENSOR_1, data, 2);
	if (err == TWI_ERROR_NoError){
        *val = ((uint16_t)(data[0]))<<8 | data[1];
    }
    return err;
}

uint8_t sensor_board_get_sensor2(uint16_t* val)
{
	uint8_t data[2];
	uint8_t err = lls_twi_read_regs(SENSOR_BOARD_ATTINY_ADDR, SENSOR_BOARD_REG_SENSOR_2, data, 2);
	if (err == TWI_ERROR_NoError){
        *val = ((uint16_t)(data[0]))<<8 | data[1];
    }
    return err;
}

