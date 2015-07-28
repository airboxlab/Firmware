/*
 * states.h
 *
 * Created: 29/01/2014 21:03:41
 *  Author: Xevel
 */ 
// actual implementation of the state machine

#ifndef STATES_H_
#define STATES_H_
    #include "../common.h"
    #include "state.h"
    
    extern state_machine_t main_fsm;

    // states of the main state machine main_fsm
    extern state_t state_normal;
    extern state_t state_config;
    extern state_t state_error;
	extern state_t state_power_saving;
	extern state_t state_calibration;
	

#endif /* STATES_H_ */