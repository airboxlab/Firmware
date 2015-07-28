/*
 * state_config.c
 *
 * Created: 29/01/2014 21:10:37
 *  Author: Xevel
 */ 
#include "state_config.h"
#include "../error.h"
#include "../wifi_config.h"
#include "states.h"
#include "post_fsm.h"
#include <Hardware_lib/time.h>
#include <Hardware_lib/led.h>
#include "../util_timer.h"

// TODO update the post_error and internal_error

typedef enum {
    CFSM_STATE_READ_SSID,
    CFSM_STATE_READ_PASSWORD,
    CFSM_STATE_READ_ENCRYPT_MODE,
    CFSM_STATE_VALIDATE,
    CFSM_STATE_VALIDATE_CONNECT_AP,
    CFSM_STATE_VALIDATE_DHCP,
    CFSM_STATE_VALIDATE_DNS,
    CFSM_STATE_VALIDATE_PING,
    CFSM_STATE_VALIDATE_FAILED,
    CFSM_STATE_WAIT_SAVE,
    CFSM_STATE_SAVE,
    CFSM_STATE_END
} config_fsm_states_t;

static config_fsm_states_t config_fsm_state;    // state of the internal state machine
static uint8_t fill_pos;                        // next position to be filled in the buffer
static uint32_t config_timeout_end;             // timeout end for all read operations
uint8_t password_len;

// helpers
void config_send_error_value(uint8_t val);
void config_reset_read_timeout();
void config_check_read_timeout();
uint8_t config_read_char(uint8_t* c);


void state_config_init(){
    memset(wifi_ap_ssid, 0, WIFI_SSID_LEN);
    memset(wifi_ap_password, 0, WIFI_PASSWORD_LEN);
    wifi_ap_encrypt_mode = 0;
    fill_pos = 0;
    // flush input buffer
    while (usb_serial_get_nb_received()){
        usb_serial_get_byte();
    }
    config_send_error_value(CONFIG_ERROR_NO_ERROR);
    config_reset_read_timeout();
    
    util_timer_enable_interrupt();
    util_timer_set_config(1);

    DCONFIG_INFO_PRINTLN("SSID?");

    config_fsm_state = CFSM_STATE_READ_SSID;
}


void state_config_play(){
    uint8_t c;
        
    switch(config_fsm_state){
        case CFSM_STATE_READ_SSID:
            if ( config_read_char(&c) ) {
                config_reset_read_timeout();
                if (c == '\r' && fill_pos != 0){  // SSID shouldn't be empty
                    fill_pos = 0;
                    DCONFIG_INFO_PRINTLN("PWD?");
                    config_fsm_state = CFSM_STATE_READ_PASSWORD;
                } else if( fill_pos < WIFI_SSID_LEN - 1){
                    wifi_ap_ssid[fill_pos++] = c;
                } else {
                    config_send_error_value(CONFIG_ERROR_SSID_LEN);
                    config_fsm_state = CFSM_STATE_END;
                }
            }
            config_check_read_timeout();
            break;

        case CFSM_STATE_READ_PASSWORD:
            if ( config_read_char(&c) ) {
                config_reset_read_timeout();
                if (c == '\r' ){ // Password can be empty when the security is NONE 
                    password_len = fill_pos;
                    DCONFIG_INFO_PRINTLN("EncMode?");
                    config_fsm_state = CFSM_STATE_READ_ENCRYPT_MODE;
                } else if( fill_pos < WIFI_PASSWORD_LEN - 1){
                    wifi_ap_password[fill_pos++] = c;
                } else {
                    config_send_error_value(CONFIG_ERROR_PASSWORD_LEN);
                    config_fsm_state = CFSM_STATE_END;
                }
            }
            config_check_read_timeout();
            break;

        case CFSM_STATE_READ_ENCRYPT_MODE:
            if ( config_read_char(&c) ) {
                if( (c >= '0') && (c <= '3') ){
                    wifi_ap_encrypt_mode = c - '0';
                    DCONFIG_INFO_PRINTLN("Validate...");
                    config_fsm_state = CFSM_STATE_VALIDATE;
                } else {
                    config_send_error_value(CONFIG_ERROR_BAD_ENCRYPT_MODE);
                    config_fsm_state = CFSM_STATE_END;
                }
            }
            config_check_read_timeout();
            break;

        case CFSM_STATE_VALIDATE: // setup validation
            config_send_error_value(CONFIG_ERROR_NO_ERROR); // start of the validation process

            if (wifi_ap_encrypt_mode == WLAN_SEC_WEP && password_len != 5 && password_len != 13 ){
                config_send_error_value(CONFIG_ERROR_PASSWORD_LEN);
                config_fsm_state = CFSM_STATE_END;
            }

            cc3000_resume();
            if (wifi_set_profile(wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode) == CC3000_SUCCESS){
                _delay_ms(50);
                cc3000_stop();
                _delay_ms(100);
                cc3000_resume();
                DCONFIG_INFO_PRINTLN("WaitConnect");
                config_timeout_end = set_timeout(millis() + CONFIG_CONNECT_TIMEOUT_MS);
                config_fsm_state = CFSM_STATE_VALIDATE_CONNECT_AP;
            } else {
                config_send_error_value(CONFIG_ERROR_COULD_NOT_SET_PROFILE);
                config_fsm_state = CFSM_STATE_VALIDATE_FAILED;
            }
            break;

        case CFSM_STATE_VALIDATE_CONNECT_AP:
            if ( cc3000_checkConnected() ){
                DCONFIG_INFO_PRINTLN("WaitDHCP");
                config_timeout_end = set_timeout(millis() + CONFIG_DHCP_TIMEOUT_MS);
                config_fsm_state = CFSM_STATE_VALIDATE_DHCP;                    
            } else if (millis() > config_timeout_end) {
                config_send_error_value(CONFIG_ERROR_COULD_NOT_CONNECT_TIMEOUT);
                config_fsm_state = CFSM_STATE_VALIDATE_FAILED;
            }
            break;

        case CFSM_STATE_VALIDATE_DHCP:
            if ( cc3000_checkDHCP() ){
                DCONFIG_INFO_PRINTLN("WaitDNS");
                config_timeout_end = set_timeout(millis() + CONFIG_DNS_TIMEOUT_MS);
                config_fsm_state = CFSM_STATE_VALIDATE_DNS;
            } else if ( millis() > config_timeout_end ){
                config_send_error_value(CONFIG_ERROR_NO_DHCP);
                config_fsm_state = CFSM_STATE_VALIDATE_FAILED;
            }
            break;
            
        case CFSM_STATE_VALIDATE_DNS:
            cc3000_getHostByName(BACKEND_NAME, &backend_ip);
            if (!backend_ip){
                if (millis() > config_timeout_end){
                    config_send_error_value(CONFIG_ERROR_NO_DNS);
                    config_fsm_state = CFSM_STATE_VALIDATE_FAILED;
                }
            } else {
                DCONFIG_INFO_PRINTLN("Ping");
                config_fsm_state = CFSM_STATE_VALIDATE_PING;
            }
            break;

        case CFSM_STATE_VALIDATE_PING:  // validate Internet connectivity 
            // we use ping instead of connecting a TCP socket because if the socket fails, it's a mess to try again.
            if (cc3000_ping(backend_ip, 4, 1000, 32)){
                cc3000_stop();
                config_send_error_value(CONFIG_ERROR_NO_ERROR);
                config_fsm_state = CFSM_STATE_SAVE;
            } else {
                config_send_error_value(CONFIG_ERROR_NO_SERVER_CONNECTIVITY);
                config_fsm_state = CFSM_STATE_VALIDATE_FAILED;
            }
            break;

        case CFSM_STATE_VALIDATE_FAILED:
            cc3000_stop();
            config_timeout_end = set_timeout(millis() + CONFIG_WAIT_SAVE_TIMEOUT_MS);
            config_fsm_state = CFSM_STATE_WAIT_SAVE;
            break;

        case CFSM_STATE_WAIT_SAVE: // wait for confirmation if the new config should be saved
            if ( config_read_char(&c) ){
                if( c == 's' ){
                    config_fsm_state = CFSM_STATE_SAVE;
                } else {
                    config_fsm_state = CFSM_STATE_END;
                }
            }
            config_check_read_timeout();
            break;

        case CFSM_STATE_SAVE: // save new config
            wifi_configured = 1;
            wifi_config_save();
            DCONFIG_INFO_PRINTLN("saved");
            config_send_error_value(CONFIG_ERROR_NO_ERROR);
            config_fsm_state = CFSM_STATE_END;
            break;

        case CFSM_STATE_END: // return to normal operation
            DCONFIG_INFO_PRINTLN("end");
            state_set_next(&main_fsm, &state_normal);
            break;
    }
}


void state_config_end(){
    error_set_post(POST_ERROR_NO_ERROR);
    if (wifi_config_load()){
        config_send_error_value(CONFIG_ERROR_NO_ERROR);
    } else {
        error_set_post(POST_ERROR_WIFI_NO_CONFIG);
        config_send_error_value(CONFIG_ERROR_NO_CONFIG);
    }
    // flush input buffer
    while (usb_serial_get_nb_received()){
        usb_serial_get_byte();
    }

    util_timer_disable_interrupt();
    util_timer_set_config(0);
}



//***********************************************************
//                       Helpers
//***********************************************************


// returns the error value as part of the config workflow. 
void config_send_error_value(uint8_t val){
    fprintf(&USBSerialStream, "#%c\r\n", val);
    usb_serial_task();
}

// restart the read timeout
void config_reset_read_timeout(){
    config_timeout_end = set_timeout(millis() + CONFIG_READ_TIMEOUT_MS);
}

void config_check_read_timeout(){
    if(millis() > config_timeout_end){
        config_send_error_value(CONFIG_ERROR_TIMEOUT);
        config_fsm_state = CFSM_STATE_END;
    }
}

uint8_t config_read_char(uint8_t* c){
    if(usb_serial_get_nb_received()){
        *c = usb_serial_get_byte();
        return 1;
    }
    return 0;
}



