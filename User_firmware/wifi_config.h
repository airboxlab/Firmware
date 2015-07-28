/*
 * wifi_config.h
 *
 * Created: 26/08/2013 20:12:37
 *  Author: Xevel
 */ 


#ifndef WIFI_CONFIG_H_
#define WIFI_CONFIG_H_
    #include "common.h"
	#include "eeprom_adress.h"
    #include <CC3000/Adafruit_CC3000_Library_20131020/utility/wlan.h> // HACK to get access to encryption constants
    #include <avr/eeprom.h>


    #define WIFI_SSID_LEN            32  // max length of the SSID, including terminating null char // cf Adafruit_CC3000.cpp line 47
    #define WIFI_PASSWORD_LEN        32  // max length of the password, including terminating null char // cf Adafruit_CC3000.cpp line 48
    char    wifi_ap_ssid [WIFI_SSID_LEN];
    char    wifi_ap_password[WIFI_PASSWORD_LEN];
    uint8_t wifi_ap_encrypt_mode; // encryption mode can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
    uint8_t wifi_configured; 




    uint8_t wifi_config_is_set();
    uint8_t wifi_config_set(const char* ssid, const char* password, uint8_t encrypt_mode);
    uint8_t wifi_config_save();
    uint8_t wifi_config_load();

    void wifi_config_mem_clear();

#ifdef DEV
    uint8_t wifi_config_set_dev();
#endif


#endif /* WIFI_CONFIG_H_ */