/*
 * wifi.c
 *
 * Created: 04/01/2014 06:05:18
 *  Author: Xevel
 */ 
#include "wifi.h"


// checks if the CC3000 has been updated to v1.24
uint8_t wifi_check_version(){
    uint8_t major, minor;
    return cc3000_getFirmwareVersion(&major, &minor) && major == 1 && minor == 24;
}


uint8_t wifi_connect_to_ap(const char* ssid, const char* password, uint8_t encrypt_mode){
    if ( !cc3000_connectToAP(ssid, password, encrypt_mode)){
    //    println("W No Cnct");
        return 0;
    } else {
    //    println("W Cncted");
    }
    
    int nb_dhcp_tries = 0;
    uint32_t retip, netmask, gateway, dhcpserv, dnsserv;
    while(!cc3000_checkDHCP() || !cc3000_getIPAddress(&retip, &netmask, &gateway, &dhcpserv, &dnsserv)){
    //    print(".");
        //usb_serial_task();
        
        nb_dhcp_tries++;
        if (nb_dhcp_tries > 150){
            return 0;
        }
        _delay_ms(100);
    }
    //print("DHCP OK\r\n");
    return 1;
}

void print_ip_helper(const char* name, uint32_t ip){
    //printf()
    //fprintf(&USBSerialStream, "%s: %d.%d.%d.%d\r\n", name, (uint8_t)(ip>>24), (uint8_t)(ip>>16), (uint8_t)(ip>>8), (uint8_t)(ip));
}

void wifi_print_ip(){
    uint32_t retip, netmask, gateway, dhcpserv, dnsserv;
    if(cc3000_getIPAddress(&retip, &netmask, &gateway, &dhcpserv, &dnsserv)){
        print_ip_helper("IP", retip);
        print_ip_helper("Netmask", netmask);
        print_ip_helper("Gateway", gateway);
        print_ip_helper("DHCPServ", dhcpserv);
        print_ip_helper("DNSServ", dnsserv);
        if (cc3000_checkDHCP()){
      //      println("Got IP from DHCP");
            } else {
       //     println("Couldn't get DHCP lease");
            }
        } else {
       // println("Could not get IP address info.");
    }
}

uint8_t wifi_ping(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
    if (cc3000_ping(cc3000_IP2U32(a,b,c,d), 3, 500, 32)){
       // fprintf(&USBSerialStream, "pinged %u.%u.%u.%u successfully\r\n", a, b, c, d);
        return true;
        } else {
      //  fprintf(&USBSerialStream, "%u.%u.%u.%u ping timeout\r\n", a, b, c, d);
        return false;
    }
}

uint8_t wifi_print_version(){
    uint8_t major, minor;
    if (cc3000_getFirmwareVersion(&major, &minor)){
    //    fprintf(&USBSerialStream, "V%d.%d\r\n", major,minor);
        return true;
        } else {
     //   println("Could not get version");
        return false;
    }
}

uint8_t wifi_print_MAC(){
    uint8_t mac[6];
    if (cc3000_getMacAddress(mac)){
        // Note: the full version of the printf lib is needed, otherwise the 0 in the format (that forces 2 digits) will be ignored.
        // clean the full solution between changes in the linkage, otherwise it won't be taken into account
      //  fprintf(&USBSerialStream, "MAC:%X %X %X %X %X %X\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return true;
        } else {
      //  println("Could not get MAC");
        return false;
    }
}

uint32_t wifi_resolve_backend_ip(){
    uint32_t backend_ip = 0;
    while ( backend_ip == 0) {
        if (! cc3000_getHostByName("api.airboxlab.com", &backend_ip)){
            //println("Could not resolve backend DNS");
        }
        _delay_ms(1000); // did not work with 500ms
    }
    //fprintf(&USBSerialStream, "backend IP %lu\r\n", backend_ip);
    return backend_ip;
}

