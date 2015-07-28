/*
 * wifi_config.c
 *
 * Created: 30/01/2014 02:00:08
 *  Author: Xevel
 */ 
#include "wifi_config.h"


#ifdef DEV
// default for DEV only


/*
#define WIFI_AP_SSID "linksys"
#define WIFI_AP_PASS ""
#define WIFI_AP_MODE WLAN_SEC_UNSEC
*/


#define WIFI_AP_SSID "alima"
#define WIFI_AP_PASS "123soleilc4"
#define WIFI_AP_MODE WLAN_SEC_WPA2

#endif




uint8_t wifi_config_is_set(){
    return wifi_configured;
}

uint8_t wifi_config_set(const char* ssid, const char* password, uint8_t encrypt_mode){
    if( strlen(ssid) < WIFI_SSID_LEN && strlen(password) < WIFI_PASSWORD_LEN && encrypt_mode <= 3 ){
        strcpy(wifi_ap_ssid, ssid);
        strcpy(wifi_ap_password, password);
        wifi_ap_encrypt_mode = encrypt_mode;
        
        wifi_configured = true;
        return true;
    }
    return false;
}

uint8_t wifi_config_save(){
    if (wifi_configured){
        eeprom_write_byte(wifi_eeprom_encrypt_addr, wifi_ap_encrypt_mode) ;
        eeprom_write_block(wifi_ap_ssid, wifi_eeprom_ssid_addr, WIFI_SSID_LEN);
        eeprom_write_block(wifi_ap_password, wifi_eeprom_password_addr, WIFI_PASSWORD_LEN);
        
        eeprom_write_dword(wifi_eeprom_magic_key_addr, MAGIC_KEY_VAL); // write magic key to prove that eeprom has been written and is not just some garbage
        return true;
    }
    return false;
}

uint8_t wifi_config_load(){
    // check if EEPROM contains a valid setup, which is indicated by the magic key having been written.
    if (eeprom_read_dword(wifi_eeprom_magic_key_addr) == MAGIC_KEY_VAL){
        wifi_ap_encrypt_mode = eeprom_read_byte(wifi_eeprom_encrypt_addr);
        eeprom_read_block(wifi_ap_ssid, wifi_eeprom_ssid_addr, WIFI_SSID_LEN);
        eeprom_read_block(wifi_ap_password, wifi_eeprom_password_addr, WIFI_PASSWORD_LEN);

        wifi_configured = true;
        return true;
    }
    return false;
}

void wifi_config_mem_clear(){
    eeprom_write_dword(wifi_eeprom_magic_key_addr, 0); // just clear the magic value
}

#ifdef DEV
uint8_t wifi_config_set_dev(){
    return wifi_config_set(WIFI_AP_SSID, WIFI_AP_PASS, WIFI_AP_MODE);
}
#endif

