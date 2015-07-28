/*
 * error.c
 *
 * Created: 29/01/2014 17:28:35
 *  Author: Xevel
 */ 
#include "error.h"

// main errors flags.
uint8_t _internal_error; // any kind of internal error
uint8_t _post_error; // any kind of wifi error


void error_set_internal(uint8_t err_code){
    _internal_error = err_code;
}

uint8_t error_internal(){
    return _internal_error;
}

void error_set_post(uint8_t err_code){
    _post_error = err_code;
}

uint8_t error_post(){
    return _post_error;
}