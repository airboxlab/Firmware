/*
 * post_config.h
 *
 * Created: 21/01/2014 11:26:20
 *  Author: Xevel
 */ 


#ifndef POST_CONFIG_H_
#define POST_CONFIG_H_
    #include "common.h"
    
    #ifdef DEV
        #define USER_AGENT "User-Agent: test10\n"
    #else
        #define USER_AGENT "User-Agent: abl118\n"
    #endif

#endif /* POST_CONFIG_H_ */