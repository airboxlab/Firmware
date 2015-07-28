/*
 * common.h
 *
 * Created: 23/08/2013 18:39:33
 *  Author: Xevel
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <avr/sfr_defs.h>

#define bitSet(value, bit)       ((value) |= (1UL << (bit)))
#define bitClear(value, bit)     ((value) &= ~(1UL << (bit)))
#define min(x,y)                 ( ((x)<(y)) ? (x) : (y) )

// Inline assembly: The nop = do nothing for one clock cycle.
#define nop()  __asm__ __volatile__("nop")


#endif /* COMMON_H_ */