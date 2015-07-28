/*
 * CC3000.cpp
 *
 * Created: 20/10/2013 06:38:43
 *  Author: Xevel
 */ 

#include <avr/io.h>


#include <Adafruit_CC3000.h>
#include <ccspi.h>

Adafruit_CC3000 cc3000 = Adafruit_CC3000();


#if defined(__cplusplus)
extern "C" {
#endif

uint8_t cc3000_init(){
    return cc3000.begin();
}
uint8_t cc3000_init2(uint8_t patchReq, bool useSmartConfigData){
    return cc3000.begin(patchReq, useSmartConfigData);
}

void     cc3000_reboot(uint8_t patchReq){
    return cc3000.reboot(patchReq);
}
    
void     cc3000_stop(void){
    cc3000.stop();
}

void     cc3000_resume(void){
    cc3000.resume();
}

bool     cc3000_disconnect(void){
    return cc3000.disconnect();
}

bool     cc3000_deleteProfiles(void){
    return cc3000.deleteProfiles();
}

//void     cc3000_printHex(const uint8_t * data, const uint32_t numBytes){
    //cc3000.printHex(data, numBytes);
//}
//
//void     cc3000_printHexChar(const uint8_t * data, const uint32_t numBytes){
    //cc3000.printHexChar(data, numBytes);
//}
//
//void     cc3000_printIPdots(uint32_t ip){
    //cc3000.printIPdots(ip);
//}
//
//void     cc3000_printIPdotsRev(uint32_t ip){
    //cc3000.printIPdotsRev(ip);
//}

uint32_t cc3000_IP2U32(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
    return cc3000.IP2U32(a,b,c,d);
}


bool     cc3000_getMacAddress(uint8_t address[6]){
    return cc3000.getMacAddress(address);
}
bool     cc3000_setMacAddress(uint8_t address[6]){
    return cc3000.setMacAddress(address);
}


bool     cc3000_connectToAP(const char *ssid, const char *key, uint8_t secmode){
    return cc3000.connectToAP(ssid, key, secmode);
}

bool     cc3000_connectSecure(const char *ssid, const char *key, int32_t secMode){
    return cc3000.connectSecure(ssid, key, secMode);
}

bool     cc3000_connectOpen(const char *ssid){
    return cc3000.connectOpen(ssid);
}

bool     cc3000_checkConnected(void){
    return cc3000.checkConnected();
}

bool     cc3000_checkDHCP(void){
    return cc3000.checkDHCP();
}

bool     cc3000_getIPAddress(uint32_t *retip, uint32_t *netmask, uint32_t *gateway, uint32_t *dhcpserv, uint32_t *dnsserv){
    return cc3000.getIPAddress(retip, netmask, gateway, dhcpserv, dnsserv);
}

bool     cc3000_checkSmartConfigFinished(void){
    return cc3000.checkSmartConfigFinished();
}

/* TODO HACK THIS NEEDS TO BE CHANGED 
Adafruit_CC3000_Client cc3000_connectTCP(uint32_t destIP, uint16_t destPort){
    
}
Adafruit_CC3000_Client cc3000_connectUDP(uint32_t destIP, uint16_t destPort){
    
}
*/

//returns client pointer, used for all the cc3000_cli_ functions
void*  cc3000_connectTCP(uint32_t destIP, uint16_t destPort){
    Adafruit_CC3000_Client* client = cc3000.connectTCP(destIP, destPort);
    return (void*)client; //return handle
}
void*    cc3000_connectUDP(uint32_t destIP, uint16_t destPort){
    Adafruit_CC3000_Client* client = cc3000.connectUDP(destIP, destPort);
    return (void*)client; //return handle
}

bool     cc3000_cli_connected(void* client){
    return ((Adafruit_CC3000_Client*)client)->connected();
}

size_t   cc3000_cli_fastrprint(void* client, const char *str){
    return ((Adafruit_CC3000_Client*)client)->fastrprint(str);
}
size_t   cc3000_cli_fastrprintln(void* client, const char *str){
    return ((Adafruit_CC3000_Client*)client)->fastrprintln(str);
}
//size_t   cc3000_cli_fastrprint_fsh(void* client, const __FlashStringHelper *ifsh){
    //return ((Adafruit_CC3000_Client*)client)->fastrprint(ifsh);
//}
//size_t   cc3000_cli_fastrprintln_fsh(void* client, const __FlashStringHelper *ifsh){
    //return ((Adafruit_CC3000_Client*)client)->fastrprintln(ifsh);
//}

size_t   cc3000_cli_write(void* client, uint8_t c){
    return ((Adafruit_CC3000_Client*)client)->write(c);
}
int16_t  cc3000_cli_write_n(void* client, const void *buf, uint16_t len, uint32_t flags){
    return ((Adafruit_CC3000_Client*)client)->write(buf, len, flags);
}
uint8_t  cc3000_cli_read(void* client){
    return ((Adafruit_CC3000_Client*)client)->read();
}
int16_t  cc3000_cli_read_n(void* client, void *buf, uint16_t len, uint32_t flags){
    return ((Adafruit_CC3000_Client*)client)->read(buf, len, flags);
}
int32_t  cc3000_cli_close(void* client){
    uint32_t res = ((Adafruit_CC3000_Client*)client)->close();
    delete ((Adafruit_CC3000_Client*)client);
    return res;
}
uint8_t  cc3000_cli_available(void* client){
    return ((Adafruit_CC3000_Client*)client)->available();
}
// returns pointer to *the first element of* _rx_buf of the client
uint8_t* cc3000_cli_get_rx_buf(void* client){
    return ((Adafruit_CC3000_Client*)client)->_rx_buf;
}
// returns pointer to _rx_buf_idx of the client
uint8_t* cc3000_cli_get_rx_buf_idx(void* client){
    return &(((Adafruit_CC3000_Client*)client)->_rx_buf_idx);
}
// returns pointer to *the first element of* _tx_buf of the client 
char*    cc3000_cli_get_tx_buf(void* client){
    return ((Adafruit_CC3000_Client*)client)->_tx_buf;
}
// returns pointer to bufsiz of the client 
int16_t* cc3000_cli_get_bufsiz(void* client){
    return &(((Adafruit_CC3000_Client*)client)->bufsiz);
}




/* Functions that aren't available with the tiny driver */
#ifndef CC3000_TINY_DRIVER
bool     cc3000_getFirmwareVersion(uint8_t *major, uint8_t *minor){
    return cc3000.getFirmwareVersion(major,minor);
}

uint8_t cc3000_getStatus(void){
    return cc3000.getStatus();
}

uint16_t cc3000_startSSIDscan(void){
    return cc3000.startSSIDscan();
}

void     cc3000_stopSSIDscan(){
    cc3000.stopSSIDscan();
}

uint8_t  cc3000_getNextSSID(uint8_t *rssi, uint8_t *secMode, char *ssidname){
    return cc3000.getNextSSID(rssi, secMode, ssidname);
}


bool     cc3000_listSSIDResults(void){
    return cc3000.listSSIDResults();
}

bool     cc3000_startSmartConfig(bool enableAES){
    return cc3000.startSmartConfig(enableAES);
}    


bool     cc3000_getIPConfig(tNetappIpconfigRetArgs *ipConfig){
    return cc3000.getIPConfig(ipConfig);
}

uint16_t cc3000_ping(uint32_t ip, uint8_t attempts,  uint16_t timeout, uint8_t size){
    return cc3000.ping(ip, attempts, timeout, size);
}

uint16_t cc3000_getHostByName(char *hostname, uint32_t *ip){
    return cc3000.getHostByName(hostname, ip);
}

bool     cc3000_scanSSIDs(uint32_t time){
    return cc3000.scanSSIDs(time);
}
#endif

#if defined(__cplusplus)
}
#endif
