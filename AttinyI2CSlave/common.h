/*
 * common.h
 *
 * Created: 04/09/2013 09:19:10
 *  Author: Xevel
 */ 


#ifndef COMMON_H_
#define COMMON_H_
#include <avr/sfr_defs.h>

#define F_CPU 8000000UL

#define bitSet(value, bit)       ((value) |= (1UL << (bit)))
#define bitClear(value, bit)     ((value) &= ~(1UL << (bit)))
#define min(x,y)                 ( ((x)<(y)) ? (x) : (y) )

// Inline assembly: The nop = do nothing for one clock cycle.
#define nop()  __asm__ __volatile__("nop")

#define db_up bitSet(PORTB,0)
#define db_down bitClear(PORTB,0)
#define db_t {db_up;db_down;}

#define DEVICE_IDENTIFIER 0x77

#endif /* COMMON_H_ */