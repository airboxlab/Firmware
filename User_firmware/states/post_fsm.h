/*
 * post_fsm.h
 *
 * Created: 31/01/2014 03:28:34
 *  Author: Xevel
 */ 


#ifndef POST_FSM_H_
#define POST_FSM_H_
    #include "../common.h"

    #define BACKEND_PORT                        (80)
	#ifdef DEV
		#define BACKEND_NAME                        "api-test.airboxlab.com"
	#else
		#define BACKEND_NAME                        "api.airboxlab.com"
	#endif
    #define CONNECT_TIMEOUT_MS                  (15000)
    #define GET_BACKEND_IP_TIMEOUT_MS           (3000)
    #define TCP_CONNECT_RETRIES                 (3)

    #define HTTP_REPLY_TIMEOUT_MS               (3000)
    #define HTTP_RESPONSE_EXPECTED              "HTTP/1.1 200"
    #define HTTP_RESPONSE_EXPECTED_LEN          (12)
    
    #define SET_PROFILE_AND_RETRY               (0) // set to 1 to do a profile setting and retry when the CC3000 could not connect to the AP or get an IP.

    
    extern uint32_t backend_ip;
    extern uint8_t returned_led_state;

    uint8_t wifi_set_profile( char* ssid, char* password, uint8_t encrypt_mode);
    uint8_t post_fsm();
    void reset_fsm();
    void end_fsm();
    uint8_t post_fsm_error();


#endif /* POST_FSM_H_ */