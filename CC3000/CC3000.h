/*
 * CC3000.h
 *
 * Created: 20/10/2013 06:44:25
 *  Author: Xevel
 */ 

#ifndef CC3000_H_
#define CC3000_H_


#include <stdbool.h>

/*
AirBoxLab  v2.0

CC3000 wifi module

Pins
SPI_CS     : PD3
SPI_DOUT   : PB3 (MISO)
SPI_IRQ    : PD2 (INT2)
SPI_DIN    : PB2 (MOSI)
SPI_CLK    : PB1 (SCLK)
VBAT_SW_EN : PB0

STATUS: basic functionalities, no sockets yet
*/


#include "Adafruit_CC3000_Library_20131020/utility/netapp.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CC3000_SUCCESS (0)


uint8_t  cc3000_init();
uint8_t  cc3000_init2(uint8_t patchReq, bool useSmartConfigData);
void     cc3000_reboot(uint8_t patchReq);
void     cc3000_stop(void);
void     cc3000_resume(void);
bool     cc3000_disconnect(void);
bool     cc3000_deleteProfiles(void);
//void     cc3000_printHex(const uint8_t * data, const uint32_t numBytes);
//void     cc3000_printHexChar(const uint8_t * data, const uint32_t numBytes);
//void     cc3000_printIPdots(uint32_t ip);
//void     cc3000_printIPdotsRev(uint32_t ip);
uint32_t cc3000_IP2U32(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
bool     cc3000_getMacAddress(uint8_t address[6]);
bool     cc3000_setMacAddress(uint8_t address[6]);

bool     cc3000_connectToAP(const char *ssid, const char *key, uint8_t secmode);
bool     cc3000_connectSecure(const char *ssid, const char *key, int32_t secMode);
bool     cc3000_connectOpen(const char *ssid);
bool     cc3000_checkConnected(void);
bool     cc3000_checkDHCP(void);
bool     cc3000_getIPAddress(uint32_t *retip, uint32_t *netmask, uint32_t *gateway, uint32_t *dhcpserv, uint32_t *dnsserv);

bool     cc3000_checkSmartConfigFinished(void);

//Adafruit_CC3000_Client cc3000_connectTCP(uint32_t destIP, uint16_t destPort); // API for that has changed, see below!!!
//Adafruit_CC3000_Client cc3000_connectUDP(uint32_t destIP, uint16_t destPort);
void*    cc3000_connectTCP(uint32_t destIP, uint16_t destPort); //returns client pointer, used for all the cc3000_cli_ functions
void*    cc3000_connectUDP(uint32_t destIP, uint16_t destPort); //returns client pointer, used for all the cc3000_cli_ functions

bool     cc3000_cli_connected(void* client);

size_t   cc3000_cli_fastrprint(void* client, const char *str);
size_t   cc3000_cli_fastrprintln(void* client, const char *str);
//size_t   cc3000_cli_fastrprint_fsh(void* client, const __FlashStringHelper *ifsh);
//size_t   cc3000_cli_fastrprintln_fsh(void* client, const __FlashStringHelper *ifsh);

size_t   cc3000_cli_write(void* client, uint8_t c);
uint8_t  cc3000_cli_read(void* client);
int16_t  cc3000_cli_write_n(void* client, const void *buf, uint16_t len, uint32_t flags);
int16_t  cc3000_cli_read_n(void* client, void *buf, uint16_t len, uint32_t flags);
int32_t  cc3000_cli_close(void* client);
uint8_t  cc3000_cli_available(void* client);

uint8_t* cc3000_cli_get_rx_buf(void* client); // returns pointer to *the first element of* _rx_buf of the client
uint8_t* cc3000_cli_get_rx_buf_idx(void* client); // returns pointer to _rx_buf_idx of the client
char*    cc3000_cli_get_tx_buf(void* client); // returns pointer to _tx_buf of the client 
int16_t* cc3000_cli_get_bufsiz(void* client); // returns pointer to bufsiz of the client

/* Functions that aren't available with the tiny driver */
#ifndef CC3000_TINY_DRIVER
bool     cc3000_getFirmwareVersion(uint8_t *major, uint8_t *minor);
uint8_t  cc3000_getStatus(void);
uint16_t cc3000_startSSIDscan(void);
void     cc3000_stopSSIDscan();
uint8_t  cc3000_getNextSSID(uint8_t *rssi, uint8_t *secMode, char *ssidname);

bool     cc3000_listSSIDResults(void);
bool     cc3000_startSmartConfig(bool enableAES);

bool     cc3000_getIPConfig(tNetappIpconfigRetArgs *ipConfig);

// defaults: attempts = 3, timeout = 500, size = 32
uint16_t cc3000_ping(uint32_t ip, uint8_t attempts,  uint16_t timeout, uint8_t size);
uint16_t cc3000_getHostByName(char *hostname, uint32_t *ip);

bool     cc3000_scanSSIDs(uint32_t time);
#endif

//void cc3000_setPrinter(Print*); // HACK removed until a solution for printing has been found

#if defined(__cplusplus)
}
#endif


#endif /* CC3000_H_ */