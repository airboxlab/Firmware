/*
 * regs.c
 *
 * Created: 04/09/2013 06:26:22
 *  Author: Xevel
 */ 
#include "regs.h"
#include "version.h"
#include <string.h>
#include <avr/interrupt.h>
#include "common.h"


uint8_t regs[REG_LEN];



void regs_init(){
    memset( (void*)regs, 0, REG_LEN);

    //set internal state
    regs_write_1(REG_IDENTIFIER, DEVICE_IDENTIFIER);
    regs_write_1(REG_VERSION, FIRMWARE_VERSION);
    regs_write_1(REG_OSCCAL, OSCCAL);
    regs_write_1(REG_NB_SAMPLES, 5);
    
    //The rest defaults to zero.
}

uint8_t regs_read_1(uint8_t place){
    return regs[place];
}
uint16_t regs_read_2(uint8_t place){
    uint8_t oldSREG = SREG;
    cli();
    uint8_t val_0 = regs[place];
    uint8_t val_1 = regs[place+1];
    SREG = oldSREG;
    return (((uint16_t)val_0) <<8) | val_1;
}
void regs_read_n(uint8_t place, uint8_t* destination, uint8_t num){
    uint8_t oldSREG = SREG;
    cli();
    memcpy(destination, regs+place, num);
    SREG = oldSREG;
}


void regs_write_1(uint8_t place, uint8_t val){
    regs[place] = val;
}
void regs_write_2(uint8_t place, uint16_t val){
    uint8_t val_0 = (val>>8)&0xFF;
    uint8_t val_1 = val&0xFF;
    
    uint8_t oldSREG = SREG;
    cli();
    regs[place] = val_0;
    regs[place+1] = val_1;
    SREG = oldSREG;
}
void regs_write_n(uint8_t place, uint8_t* origin, uint8_t num){
    uint8_t oldSREG = SREG;
    cli();
    memcpy( regs+place, origin, num);
    SREG = oldSREG;
}
