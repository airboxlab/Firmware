/*
 * state_error.h
 *
 * Created: 29/01/2014 21:08:59
 *  Author: Xevel
 */ 


#ifndef STATE_ERROR_H_
#define STATE_ERROR_H_
    #include "../common.h"

    // Unrecoverable error state.
    // This is where the programs goes to die when something important failed (a sensor, or a call that should always work on a functional unit).


    void state_error_init();
    void state_error_play();
    void state_error_end();

#endif /* STATE_ERROR_H_ */