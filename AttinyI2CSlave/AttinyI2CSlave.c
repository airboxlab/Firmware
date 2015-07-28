/*
 * AttinyI2CSlave.c
 *
 * Created: 04/09/2013 06:22:20
 *  Author: Xevel
 */ 

/*
Firmware for the ATTiny24 on the Sensor board of AirBoxLab v2.
This board holds the MQ135 and iAQengine sensors.
Both the ATtiny and the iAQengine are I2C slaves, and the Tiny is connected to:
- MQ135 heater (ON/OFF)
- MQ135 sensor (ADC)

In addition, the temperature variations that may affect the
internal 8Mhz oscillator should be corrected based on the internal temperature
sensor output.  <TODO verify if it actually is a problem... it might not be!>

I2C can be used to read or write from the "registers", and these registers
are used as input or output of the free-running driver functions. <TODO>

*/

#include "common.h"
#include <avr/io.h>
#include "regs.h"
#include "mq135.h"
#include "usiTwiSlave.h"
#include "temperature.h"

#include "util/delay.h"
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define SLAVE_ADDR 42

volatile uint8_t reg_position = 0;

// Master asks for a byte. Only one byte should be returned each time.
void i2c_onRequest(void){
    if (reg_position >= REG_LEN){
        //reading outside reg table... virtually extend with zeros
        usiTwiTransmitByte(0);
    } else {
        // return reg at desired position and increment position
        usiTwiTransmitByte( regs_read_1(reg_position) );
        reg_position++;
    }
}

// Master writes bytes
// WARNING this is only executed after a request is made! So the easiest way to fix that is to make a dummy request after each write.
void i2c_onReceive(uint8_t nb_written){
    if ( nb_written < 1 ){
        return; // should never happen
    }
    //db_t;
    reg_position = usiTwiReceiveByte();
    nb_written--;
    // if nb_written == 1, we only position the cursor for an incoming read operation

    while (nb_written){
        if (reg_position >= REG_LEN){
            usiTwiReceiveByte(); //throw away data that falls outside the reg table.
        } else {
            regs_write_1(reg_position++, usiTwiReceiveByte()); // write data to the regs
        }
        //db_t;
        nb_written--;
    }
        
}


int main(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();
    sei();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);
    
    regs_init();
    mq135_init();
    mq135_heater(0);
    
    //bitSet(DDRB, 0);
    
    usiTwiSlaveInit(SLAVE_ADDR);
    //db_t;
    
    usi_onRequestPtr = i2c_onRequest;
    usi_onReceiverPtr = i2c_onReceive;
    
    while(1){
        // everything is happening in I2C interrupts!
        _delay_us(20);
        
        if (regs_read_1(REG_UPDATE)){
            // read MQ135 value
            regs_write_2(REG_MQ135, mq135_get_voltage(regs_read_1(REG_NB_SAMPLES)));

            // read internal temp value
            regs_write_2(REG_TEMP, temperature_get());

            regs_write_1(REG_UPDATE, 0); // clear the register to say that the update is done.
        }

        // update heater state
        mq135_heater(regs_read_1(REG_HEATER_ON));
            
        // update OSCCAL
        OSCCAL = regs_read_1(REG_OSCCAL);
    }
}