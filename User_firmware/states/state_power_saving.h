/*
 * state_power_saving.h
 *
 * Created: 24/04/2014 10:19:19
 *  Author: Tony
 */ 


#ifndef STATE_POWER_SAVING_H_
#define STATE_POWER_SAVING_H_
#include "../common.h"

//This is the state when the battery is low (<3.5v) 
void state_power_saving_init();
void state_power_saving_play();
void state_power_saving_end();

#endif /* STATE_POWER_SAVING_H_ */