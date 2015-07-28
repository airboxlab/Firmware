/*
 * regs.h
 *
 * Created: 04/09/2013 06:23:50
 *  Author: Xevel
 */ 


#ifndef REGS_H_
#define REGS_H_



/*
AirBoxLab  v2.0 Sensor Board

Registers for I2C communication

Pins : none.

STATUS : needs to be tested
*/
#include "stdint.h"

#define REG_LEN             16  // number of bytes in memory

// registers
#define REG_IDENTIFIER      0x00 // 1 byte  (R) identifier used to test that the communication is OK. Only 100% fixed value across revisions and runs (see common.h).
#define REG_VERSION         0x01 // 1 byte  (R) code version, in case we need to update one day...
#define REG_OSCCAL          0x02 // 1 byte (RW) value of OSCCAL register
#define REG_TEMP            0x03 // 2 bytes (R) ADC value (10 bits) of internal temp sensor. See MCU datasheet for interpretation. [DOES NOT WORK NOW]
#define REG_MQ135           0x05 // 2 bytes (R) voltage value of the MQ-135 in mV, assuming power is 5.00V
#define REG_HEATER_ON       0x07 // 1 byte (RW) state of the MQ-135 heater. 0 is OFF, any other value is ON. Default: 0
#define REG_NB_SAMPLES      0x08 // 1 byte (RW) number of samples the MQ135 value is computed on.
#define REG_UPDATE          0x09 // 1 byte (RW) write anything non-zero to trigger an update of the MQ135 and temp values (cleared when completed).


// Bytes are transmitted in ascending order, and for two-byte values it's MSB first 
// For example, reading 5 bytes from REG_TEMP will return [TEMP_H, TEMP_L, MQ135_H, MQ135_L]

void regs_init(void);

uint8_t  regs_read_1(uint8_t place);
uint16_t regs_read_2(uint8_t place);
void     regs_read_n(uint8_t place, uint8_t* destination, uint8_t num);

void regs_write_1(uint8_t place, uint8_t  val);
void regs_write_2(uint8_t place, uint16_t val);
void regs_write_n(uint8_t place, uint8_t* origin, uint8_t num);



#endif /* REGS_H_ */