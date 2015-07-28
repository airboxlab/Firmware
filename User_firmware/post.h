/*
 * post.h
 *
 * Created: 02/01/2014 18:20:57
 *  Author: Xevel
 */ 


#ifndef POST_H_
#define POST_H_
    #include "common.h"
    
    void post_init(); // requires CC3000 to be ON
    uint8_t post_do(void* sock);
 
    char* get_mac_str();

    //internal stuff
    void post_update(); // updates the string that will be sent with the sensor values
    uint8_t post_send(void* sock); 
    uint8_t post_get_leds(void* sock);


#endif /* POST_H_ */