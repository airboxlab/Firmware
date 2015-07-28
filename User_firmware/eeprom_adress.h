/*
 * eeprom_adress.h
 *
 * Created: 02/05/2014 17:09:29
 *  Author: Tony
 */ 


#ifndef EEPROM_ADRESS_H_
#define EEPROM_ADRESS_H_

//For WiFi
#define WIFI_SSID_LEN            32  // max length of the SSID, including terminating null char // cf Adafruit_CC3000.cpp line 47
#define WIFI_PASSWORD_LEN        32  // max length of the password, including terminating null char // cf Adafruit_CC3000.cpp line 48
#define wifi_eeprom_magic_key_addr  ((uint32_t*)0x00000000)
#define wifi_eeprom_encrypt_addr    ((uint8_t*)(wifi_eeprom_magic_key_addr+sizeof(uint32_t)))
#define wifi_eeprom_ssid_addr       ((void*)(wifi_eeprom_encrypt_addr+sizeof(uint8_t)))
#define wifi_eeprom_password_addr   ((void*)(wifi_eeprom_ssid_addr+WIFI_SSID_LEN))
#define MAGIC_KEY_VAL				0xBADBEEF



//For calibration
#define calib_eeprom_pm_magic_key   ((uint32_t*)(wifi_eeprom_password_addr+WIFI_PASSWORD_LEN))
#define calib_eeprom_k_pm_addr		((uint8_t*)(calib_eeprom_pm_magic_key+sizeof(uint32_t)))
#define calib_eeprom_b_pm_addr		((float*)(calib_eeprom_k_pm_addr+sizeof(uint8_t)))

#define calib_eeprom_temp_magic_key ((uint32_t*)(calib_eeprom_b_pm_addr+sizeof(float)))
#define calib_eeprom_k_temp_addr	((uint8_t*)(calib_eeprom_temp_magic_key+sizeof(uint32_t)))
#define calib_eeprom_b_temp_addr	((float*)(calib_eeprom_k_temp_addr+sizeof(uint8_t)))

#define calib_eeprom_hum_magic_key  ((uint32_t*)(calib_eeprom_b_temp_addr+sizeof(float)))
#define calib_eeprom_k_hum_addr		((uint8_t*)(calib_eeprom_hum_magic_key+sizeof(uint32_t)))
#define calib_eeprom_b_hum_addr		((float*)(calib_eeprom_k_hum_addr+sizeof(uint8_t)))

#define CALIB_MAGIC_KEY				0xBADBEEF //TO DO change it


#endif /* EEPROM_ADRESS_H_ */