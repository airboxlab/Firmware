/*
 * state.c
 *
 * Created: 29/01/2014 21:16:03
 *  Author: Xevel
 */ 
 #include "state.h"



// can be used in place of functions that are not needed.
void state_default_func(){
}

void state_set_next(state_machine_t* fsm, state_t* new_state){
    fsm->next_state = new_state;
}

void state_change(state_machine_t* fsm){
    if (fsm->next_state == NULL){
        return;
    }
    if (fsm->current_state) fsm->current_state->end();
    fsm->next_state->init();
    fsm->current_state = fsm->next_state;
    fsm->next_state = NULL;
}



