/*
 * error.h
 *
 * Created: 29/01/2014 17:28:05
 *  Author: Xevel
 */ 


#ifndef ERROR_H_
#define ERROR_H_
    #include "common.h"


    void error_set_internal(uint8_t err_code);
    uint8_t error_internal();
    void error_set_post(uint8_t err_code);
    uint8_t error_post();

    // internal error codes. These are for problems that we cannot recover from and any non-zero value will put the device in global error state.
    #define INTERNAL_ERROR_NO_ERROR         0   // OK
    #define INTERNAL_ERROR_CC3000_VERSION   1   // version mismatch, should be 1.24
    #define INTERNAL_ERROR_CC3000_NO_INIT   2   // could not init the CC3000
    #define INTERNAL_ERROR_CC3000_PROFILE   3   // something went wrong while setting the AP profile
    // TODO

    // post error codes. These are for error that affect only the current post and could be recovered from at the next post.
    #define POST_ERROR_NO_ERROR             0   // OK
    #define POST_ERROR_WIFI_NO_CONNECT      1   // could not connect
    #define POST_ERROR_WIFI_NO_CONFIG       2   // no AP configured
    #define POST_ERROR_WIFI_NO_POST         3   // could not post in normal mode
    
    // TODO






#endif /* ERROR_H_ */