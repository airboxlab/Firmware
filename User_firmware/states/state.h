/*
 * state.h
 *
 * Created: 29/01/2014 19:56:40
 *  Author: Xevel
 */ 
// State machine definitions and functions to use them

#ifndef STATE_H_
#define STATE_H_
    #include "../common.h"

    // function pointers can be set to state_default_func if they are not needed.
    typedef struct {
        uint8_t id;
        void (*init)(void);
        void (*play)(void);
        void (*end)(void);
    } state_t;

    typedef struct {
        uint8_t id;
        state_t* current_state;
        state_t* next_state;
    } state_machine_t;

    // can be used forfunctions that are not needed. 
    void state_default_func();
    void state_set_next(state_machine_t* fsm, state_t* new_state);
    void state_change(state_machine_t* fsm);




#endif /* STATE_H_ */