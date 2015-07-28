/*
 * state_config.h
 *
 * Created: 29/01/2014 21:09:13
 *  Author: Xevel
 */ 


#ifndef STATE_CONFIG_H_
#define STATE_CONFIG_H_
    #include "../common.h"

    // This is the state used to configure the WiFi.
    // It only serves as a way to allow an app to input info to connect to an AP from the USB serial port. 

    // PROCESS:
    // We arrive in this state from normal state by sending 'w'. 
    // The device signals that it has entered the config mode by returning a CONFIG_ERROR_NO_ERROR error code.
    //
    // Error codes are formatted as follows:
    //  <#> <CONFIG_ERROR_VALUE> <\r> <\n>
    // where CONFIG_ERROR_VALUE is a single ASCII char (example: "#0\r\n" is the code returned when there isn't any error).
    // 
    // The host is then expected to input the AP info according to this format (with a short timeout between two consecutive bytes):
    //  <SSID> <\r> <PASSWORD> <\r> <encrypt_mode>
    // where SSID and PASSWORD are ASCII strings of maximum 31 char and encrypt_mode is a numerical character in '0'-'3'.
    // No need for a null character terminating the strings, but it should not cause problems if there is one as long as
    // the number of received bytes is lower than the maximum length.
    // 
    // Once these elements are received, the device returns an error code.
    // If the error code is CONFIG_ERROR_NO_ERROR, the device checks that it can connect to the AP. This can take a long time.
    // Upon reaching the conclusion of the connectivity tests (connect to AP, DHCP, DNS, ping backend), an error code is returned.
    // If the error code is CONFIG_ERROR_NO_ERROR, the device automatically saves the new AP and returns a last error code before
    // returning to normal operation. If the connectivity fails however, the option is left to the host to either force
    // saving this configuration by sending 's', or to discard the configuration by sending 'n'. If no answer is received before
    // the timeout, the new configuration is automatically discarded.


    // errors that can be returned during the process
    #define CONFIG_ERROR_NO_ERROR                   '0'     // OK!
    #define CONFIG_ERROR_SSID_LEN                   '1'     // SSID len is either null or too long
    #define CONFIG_ERROR_PASSWORD_LEN               '2'     // Password length is too long, or in WEP it's not 5 or 13 as it should be.
    #define CONFIG_ERROR_BAD_ENCRYPT_MODE           '3'     // Encrypt mode is not a valid value
    #define CONFIG_ERROR_COULD_NOT_CONNECT          '4'     // Could not connect to the new AP
    #define CONFIG_ERROR_COULD_NOT_SAVE             '5'     // Error occurred while trying to save the new config
    #define CONFIG_ERROR_TIMEOUT                    '6'     // a read operation timeout occurred
    #define CONFIG_ERROR_NO_CONFIG                  '7'     // no config saved to restore
    #define CONFIG_ERROR_NO_DHCP                    '8'     // could not get an IP
    #define CONFIG_ERROR_NO_DNS                     '9'     // could not get the address of the back-end
    #define CONFIG_ERROR_NO_SERVER_CONNECTIVITY     'a'     // could not ping the back-end
    #define CONFIG_ERROR_COULD_NOT_CONNECT_TIMEOUT  'b'     // timeout while trying to connect to new AP
    #define CONFIG_ERROR_COULD_NOT_SET_PROFILE      'c'     // setting the profile into the CC3000 failed

    
    #define CONFIG_READ_TIMEOUT_MS           (1000)     // timeout between two char, in ms
    #define CONFIG_WAIT_SAVE_TIMEOUT_MS     (60000)     // timeout for the Wait Save state, in ms
    #define CONFIG_CONNECT_TIMEOUT_MS       (15000)     // timeout for AP connection
    #define CONFIG_DHCP_TIMEOUT_MS          (10000)     // timeout for IP address acquisition by DHCP
    #define CONFIG_DNS_TIMEOUT_MS            (5000)     // timeout for DNS resolution of the back-end IP


    void state_config_init();
    void state_config_play();
    void state_config_end();



#endif /* STATE_CONFIG_H_ */