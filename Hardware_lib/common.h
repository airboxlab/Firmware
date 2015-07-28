/*
 * common.h
 *
 * Created: 23/08/2013 18:39:33
 *  Author: Xevel
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#define F_CPU 16000000UL

#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

#define bitSet(value, bit)       ((value) |= (1UL << (bit)))
#define bitClear(value, bit)     ((value) &= ~(1UL << (bit)))
#define bitIsSet(value, bit)     ((value) & (1<<bit))

// Inline assembly: The nop = do nothing for one clock cycle.
#define nop()  __asm__ __volatile__("nop")
#define sleep() __asm__ __volatile__ ("sleep")

#endif /* COMMON_H_ */