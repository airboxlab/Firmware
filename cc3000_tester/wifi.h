/*
 * wifi.h
 *
 * Created: 02/01/2014 03:11:44
 *  Author: Xevel
 */ 


#ifndef WIFI_H_
#define WIFI_H_

#include "common.h"

#include <CC3000/CC3000.h>

uint8_t wifi_check_version(); // checks if the CC3000 has been updated to v1.24

uint8_t wifi_connect_to_ap(const char* ssid, const char* password, uint8_t encrypt_mode);
void print_ip_helper(const char* name, uint32_t ip);
void wifi_print_ip();
uint8_t wifi_ping(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
uint8_t wifi_print_version();
uint8_t wifi_print_MAC();
uint32_t wifi_resolve_backend_ip();


#endif /* WIFI_H_ */