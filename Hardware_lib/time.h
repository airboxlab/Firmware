/*
 * time.h
 *
 * Created: 28/01/2014 15:11:47
 *  Author: Xevel
 */ 


#ifndef TIME_H_
#define TIME_H_
#include "common.h"

void millis_init();
uint32_t millis();
uint32_t millis_reset();
long set_timeout(long timeout);
#endif /* TIME_H_ */