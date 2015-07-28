/*
 * post_fsm.c
 *
 * Created: 31/01/2014 03:28:53
 *  Author: Xevel
 */ 
#include "post_fsm.h"
#include "states.h"
#include <Hardware_lib/time.h>
#include "../wifi_config.h"
#include "../sensor.h"
#include "../error.h"
#include "../post.h"


uint32_t backend_ip;

typedef enum {
    PFSM_IDLE,
    PFSM_POWER_UP,
    PFSM_CONNECTION,
#if SET_PROFILE_AND_RETRY
    PFSM_SET_PROFILE,
#endif
    PFSM_GET_BACKEND_IP,
    PFSM_GET_BACKEND_IP_2,
    PFSM_START_TCP,
    PFSM_POST,
    PFSM_WAIT_FOR_REPLY,
    PFSM_POST_GET_LED,
    PFSM_WAIT_FOR_LED_REPLY,
    PFSM_OK,
    PFSM_ERROR
} post_fsm_state_t;

post_fsm_state_t post_fsm_state, prev_post_fsm_state, error_post_fsm_state; // current state, last state, state from which we got the error when we have an error
uint8_t timeout_count, tcp_retries;
static uint32_t timeout_end;
void* sock;

char ret_string[HTTP_RESPONSE_EXPECTED_LEN+1];
uint8_t ret_string_pos;
uint8_t returned_led_state;

uint8_t wifi_set_profile( char* ssid, char* password, uint8_t encrypt_mode){
    uint8_t pairwise_cipher_or_key_len, group_cipher, key_management, key_len;
    uint8_t* key;
    
    switch(encrypt_mode){
        case WLAN_SEC_UNSEC://None
            pairwise_cipher_or_key_len = 0;
            group_cipher = 0;
            key_management = 0;
            key = NULL;
            key_len = 0;
            break;
        case WLAN_SEC_WEP://WEP
            pairwise_cipher_or_key_len = strlen(password);
            group_cipher = 0;
            key_management = 1; // TODO the various posts about it diverge on this value. Some say it should be 1 (http://e2e.ti.com/support/low_power_rf/f/851/t/311630.aspx), driver code uses 0...
            key = (uint8_t*)password;
            key_len = 0;
            break;
        case WLAN_SEC_WPA://WPA
        case WLAN_SEC_WPA2://WPA2
            pairwise_cipher_or_key_len = 0x18;
            group_cipher = 0x1e;
            key_management = 2;
            key = (uint8_t*)password;
            key_len = strlen(password);
            break;
        default:
            DWIFI_ERR_PRINTF("ERR encrypt mode=%u\r\n",encrypt_mode);
            return 2;
            break;
    }

    // delete all profiles
    CC3000_WIFI_CHECK_SUCCESS( wlan_ioctl_del_profile(255), "ERR del prof.", 1);
    DWIFI_INFO_PRINTLN("del prof");

    // write profile
    CC3000_WIFI_CHECK_SUCCESS(
        wlan_add_profile(
            encrypt_mode,                   // security type
            (uint8_t*)ssid,                 // SSID
            strlen(ssid),                   // SSID length
            NULL, 							// BSSID (currently unsupported it seems)
            1,								// Priority
            pairwise_cipher_or_key_len,		// PairwiseCipher
            group_cipher, 					// GroupCipher
            key_management,					// KEY management
            key,				            // KEY
            key_len),                       // KEY length
        "ERR prof",
        2);
    DWIFI_INFO_PRINTLN("prof added");
    
    //set policy to use profiles and fast reconnect
    CC3000_WIFI_CHECK_SUCCESS( wlan_ioctl_set_connection_policy(0, 1, 1), "ERR connect policy", 3);
    DWIFI_INFO_PRINTLN("connect policy 011");
    return 0;
}



void reset_fsm(){
    post_fsm_state = PFSM_POWER_UP;
    prev_post_fsm_state = PFSM_POWER_UP;
    error_post_fsm_state = PFSM_POWER_UP;
    timeout_count = 0;
    tcp_retries = 0;
}

void end_fsm(){
    if (sock){
        cc3000_cli_close(sock);
        sock = NULL;
    }
}

uint8_t post_fsm_error(){
    return post_fsm_state == PFSM_ERROR;
}

// returns 0 as long as it hasn't finished
// if we get to the error state, error_post_fsm_state contains the state in which the error occurred.
uint8_t post_fsm(){
#if DEBUG_POST_INFO
    uint32_t retip, netmask, gateway, dhcpserv, dnsserv;
#endif
    prev_post_fsm_state = post_fsm_state;
    
    switch(post_fsm_state){
        case PFSM_IDLE:
            break;
        case PFSM_POWER_UP:
            DPOST_INFO_PRINTF("power up :%lu\r\n", millis());
            cc3000_resume();
            DPOST_INFO_PRINTF("connec... :%lu\r\n", millis());
            timeout_end = set_timeout(millis() + CONNECT_TIMEOUT_MS);
            post_fsm_state = PFSM_CONNECTION;
            break;
        case PFSM_CONNECTION:
            // check that the CC3000 has connected
            DPOST_INFO_PRINTF( "cntd:%u dhcp:%u status:%u, get_ip:%u, %lu\r\n", cc3000_checkConnected(), cc3000_checkDHCP(), cc3000_getStatus(), cc3000_getIPAddress(&retip, &netmask, &gateway, &dhcpserv, &dnsserv), millis() );
            
            if ( cc3000_checkDHCP() ){
                // OK we're connected
                post_fsm_state = PFSM_GET_BACKEND_IP;
            } else {
                //DPOST_INFO_PRINTF("no DHCP %lu\r\n", millis());
                if ( millis() > timeout_end ){
#if SET_PROFILE_AND_RETRY
                    if (timeout_count == 0){ // first timeout
                        // try reseting the profile
                        timeout_count++;
                        post_fsm_state = PFSM_SET_PROFILE;
                    } else {
#endif
                        // this is serious, we could not connect even after reseting the profile :(
                        DPOST_ERR_PRINTLN("ERR connect/dhcp timeout");
                        post_fsm_state = PFSM_ERROR;
#if SET_PROFILE_AND_RETRY
                    }
#endif

                } else {
                    _delay_ms(100);
                }
            }
            break;
#if SET_PROFILE_AND_RETRY
        case PFSM_SET_PROFILE: // restore profile from the MCU's EEPROM
            if (!wifi_config_load()){
                error_set_post(POST_ERROR_WIFI_NO_CONFIG);
                post_fsm_state = PFSM_ERROR;
                break;
            }
            wifi_set_profile(wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode); // TODO what if it fails?
            _delay_ms(10);
            cc3000_stop();
            _delay_ms(50);
            post_fsm_state = PFSM_POWER_UP;
            break;
#endif
        case PFSM_GET_BACKEND_IP:
            DPOST_INFO_PRINTLN("check backend ip");
            // this takes around a 1 second
            // TODO clean it up every hour maybe? or just put it all the time ?
            // backend_ip = 0; // HACK to test recovering the IP every time
            timeout_end = set_timeout(millis() + GET_BACKEND_IP_TIMEOUT_MS);
            post_fsm_state = PFSM_GET_BACKEND_IP_2;
            break;
        case PFSM_GET_BACKEND_IP_2:
            //backend_ip = cc3000_IP2U32(192,168,1,1);
            if (backend_ip){ // we have a back-end IP
                post_fsm_state = PFSM_START_TCP;
            } else {
                if (millis() > timeout_end){
                    DPOST_ERR_PRINTLN("ERR IP resolve timeout");
                    post_fsm_state = PFSM_ERROR;
                } else {
                    DPOST_INFO_PRINTF("get backend ip :%lu\r\n", millis());
                    cc3000_getHostByName(BACKEND_NAME, &backend_ip);
                }
            }
            break;
        case PFSM_START_TCP:
            DPOST_INFO_PRINTF("connect TCP...:%lu\r\n", millis());
            sock = cc3000_connectTCP(backend_ip, BACKEND_PORT);  
			DPOST_INFO_PRINTF("Sock : %s",sock);
            //********************
            // HUGE UGLY HACK : this has been modified to return an unconnected socket if it takes too long, and the device has to reboot the CC3000 when it fails.
            // cf lib CC3000, even_handler.cpp, hci_event_handler() to set the timeout... :/ 
            //********************
            DPOST_INFO_PRINTF("connect TCP ended:%lu\r\n", millis());
            
            if (cc3000_cli_connected(sock)){
                post_fsm_state = PFSM_POST;
            } else {
                cc3000_cli_close(sock); // cleanup is very important to avoid memory leaks!
                sock = NULL;
                tcp_retries++;
                DPOST_INFO_PRINTF("TCP pb: %u\r\n", tcp_retries);
                if (tcp_retries > TCP_CONNECT_RETRIES){
                    DPOST_ERR_PRINTLN("ERR no TCP connect");
                    post_fsm_state = PFSM_ERROR;
                } else {
                    _delay_ms(10);
                    cc3000_stop();
                    _delay_ms(50);
                    post_fsm_state = PFSM_POWER_UP;
                }
            }
            //Note: we don't ping anymore, since it's redundant, and the API is slow and inadequate for what we want.
            break;
        case PFSM_POST:
            // do the actual posting
            post_do(sock);
            DPOST_INFO_PRINTF("post done :%lu\r\n", millis());
            timeout_end = set_timeout(millis() + HTTP_REPLY_TIMEOUT_MS);
            memset(ret_string, 0, sizeof(ret_string));
            ret_string_pos = 0;
            post_fsm_state = PFSM_WAIT_FOR_REPLY;
            break;
        case PFSM_WAIT_FOR_REPLY:
            if ( cc3000_cli_available(sock) ){
                DPOST_INFO_PRINTF("HTTP resp :%lu\r\n", millis());
                
                do {
                    uint8_t c = cc3000_cli_read(sock);
                    if (ret_string_pos < HTTP_RESPONSE_EXPECTED_LEN){
                        ret_string[ret_string_pos++] = c;
                    }
                    DPOST_INFO_PRINTF("%c", c);
                } while (
                    cc3000_cli_available(sock)
                    && !(millis() > timeout_end)
                    );
                DPOST_INFO_PRINT("<EOF>\r\n");
                if ( !strncmp(ret_string, HTTP_RESPONSE_EXPECTED, HTTP_RESPONSE_EXPECTED_LEN) ){
                    DPOST_INFO_PRINTLN("HTTP resp OK");
                    post_fsm_state = PFSM_POST_GET_LED;
                } else {
                    DPOST_ERR_PRINTLN("ERR bad HTTP response");
                    post_fsm_state = PFSM_ERROR;
                }
            } else if (millis() > timeout_end){
                DPOST_ERR_PRINTLN("ERR HTTP response timeout");
                post_fsm_state = PFSM_ERROR;
            }
            break;

        case PFSM_POST_GET_LED:
            post_get_leds(sock);
            timeout_end = set_timeout(millis() + HTTP_REPLY_TIMEOUT_MS*2);
            memset(ret_string, 0, sizeof(ret_string));
            ret_string_pos = 0;
            post_fsm_state = PFSM_WAIT_FOR_LED_REPLY;
            break;
        case PFSM_WAIT_FOR_LED_REPLY:
            if ( cc3000_cli_available(sock) ){
                DPOST_INFO_PRINTF("GET resp :%lu\r\n", millis());
                
                do {
                    uint8_t c = cc3000_cli_read(sock);
                    if (ret_string_pos < HTTP_RESPONSE_EXPECTED_LEN){
                        ret_string[ret_string_pos] = c;
                    }
                    if (ret_string_pos == 160){ // the 160th char is the LED state, and is '0' or '1'
                        returned_led_state = c - '0';
                    }
                    ret_string_pos++;
                    // TODO save the useful byte, the one that says if the LEDs should be ON or not!!
                    DPOST_INFO_PRINTF("%c", c);
                } while (
                    cc3000_cli_available(sock)
                    && !(millis() > timeout_end)
                    );
                DPOST_INFO_PRINT("<EOF>\r\n");
                if ( !strncmp(ret_string, HTTP_RESPONSE_EXPECTED, HTTP_RESPONSE_EXPECTED_LEN) ){
                    DPOST_INFO_PRINTF("GET resp OK, %u\r\n", returned_led_state);
                    post_fsm_state = PFSM_OK;
                } else {
                    DPOST_ERR_PRINTLN("ERR bad GET response");
                    post_fsm_state = PFSM_OK; // don't fail the whole process, it's not that important
                }
            } else if (millis() > timeout_end){
                DPOST_ERR_PRINTLN("ERR GET response timeout");
                post_fsm_state = PFSM_OK; // don't fail the whole process, it's not that important
            }

            break;
            
        case PFSM_OK:
            // end state
            //DPOST_INFO_PRINTLN("OK!");
            end_fsm();
            _delay_ms(8); // it seems to be important to wait a little after closing the port and before shutting down the CC3000...
            cc3000_stop();
            break;
        case PFSM_ERROR:
            // end state
            end_fsm();
            _delay_ms(8); // it seems to be important to wait a little after closing the port and before shutting down the CC3000...
            cc3000_stop();
            //DPOST_INFO_PRINTLN("ERROR!");
            break;
    }
    
    if (prev_post_fsm_state != post_fsm_state && post_fsm_state == PFSM_ERROR){
        error_post_fsm_state = prev_post_fsm_state;
    }
    
    return (prev_post_fsm_state == PFSM_OK || prev_post_fsm_state == PFSM_ERROR);
}


