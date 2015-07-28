/*
AirBoxLab v2

*/

#include "cc3000_tester.h"
#include <stdbool.h>
#include <util/delay.h>

#include "Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <Hardware_lib/led.h>

#include <Hardware_lib/reset.h>
#include <Hardware_lib/time.h>
#include "wifi_config.h"
#include "wifi.h"
#include "post_config.h"


#define DEBUG_WIFI_ERR      (1)
#define DEBUG_WIFI_INFO     (1)
#define DEBUG_POST_ERR      (1)
#define DEBUG_POST_INFO     (1)


uint8_t post_id;

USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = 0,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};


/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
    *  used like any regular character stream in the C APIs
    */
static FILE USBSerialStream;



void usb_serial_init(){
    USB_Init();

    /* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
    CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
}

int16_t usb_serial_get_nb_received(){
    return CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);
}

uint8_t usb_serial_get_byte(){
    return CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
}

void usb_serial_task(){
    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    USB_USBTask();
}

void println(const char* str){
    fprintf(&USBSerialStream, "%s\r\n", str);
}

void print(const char* str){
    fprintf(&USBSerialStream, "%s", str);
}

void usb_serial_send(const char c){
    CDC_Device_SendByte(&VirtualSerial_CDC_Interface, c);
    CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '\n');
}



#if DEBUG_WIFI_ERR
#define DWIFI_ERR_PRINT(str)  {print(str);usb_serial_task();}
#define DWIFI_ERR_PRINTLN(str)  {println(str);usb_serial_task();}
#define DWIFI_ERR_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
#define DWIFI_ERR_LED(r,g,b)  {led_set(r,g,b);}
#else
#define DWIFI_ERR_PRINT(str)
#define DWIFI_ERR_PRINTLN(str)
#define DWIFI_ERR_PRINTF(str,...)
#define DWIFI_ERR_LED(r,g,b)
#endif

#if DEBUG_WIFI_INFO
#define DWIFI_INFO_PRINT(str)  {print(str);usb_serial_task();}
#define DWIFI_INFO_PRINTLN(str)  {println(str);usb_serial_task();}
#define DWIFI_INFO_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
#define DWIFI_INFO_LED(r,g,b)  {led_set(r,g,b);}
#else
#define DWIFI_INFO_PRINT(str)
#define DWIFI_INFO_PRINTLN(str)
#define DWIFI_INFO_PRINTF(str,...)
#define DWIFI_INFO_LED(r,g,b)
#endif

#if DEBUG_POST_ERR
#define DPOST_ERR_PRINT(str)  {print(str);usb_serial_task();}
#define DPOST_ERR_PRINTLN(str)  {println(str);usb_serial_task();}
#define DPOST_ERR_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
#define DPOST_ERR_LED(r,g,b)  {led_set(r,g,b);}
#else
#define DPOST_ERR_PRINT(str)
#define DPOST_ERR_PRINTLN(str)
#define DPOST_ERR_PRINTF(str,...)
#define DPOST_ERR_LED(r,g,b)
#endif

#if DEBUG_POST_INFO
#define DPOST_INFO_PRINT(str)  {print(str);usb_serial_task();}
#define DPOST_INFO_PRINTLN(str)  {println(str);usb_serial_task();}
#define DPOST_INFO_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
#define DPOST_INFO_LED(r,g,b)  {led_set(r,g,b);}
#else
#define DPOST_INFO_PRINT(str)
#define DPOST_INFO_PRINTLN(str)
#define DPOST_INFO_PRINTF(str,...)
#define DPOST_INFO_LED(r,g,b)
#endif

uint8_t post_do(void* sock){
    uint16_t res;
    
    char str_id[6];
    char str_len[4];
    
    memset(str_id, 0, sizeof(str_id));
    memset(str_len, 0, sizeof(str_len));
    sprintf(str_id, "%u\n", post_id);
    sprintf(str_len, "%u", 33+strlen(str_id));
    
    DPOST_INFO_PRINTF("Posting ID %u\r\n", post_id);
    
    res  = cc3000_cli_fastrprint(sock, "POST /v1/airboxlab/ HTTP/1.1\n");
    res += cc3000_cli_fastrprint(sock, USER_AGENT);
    res += cc3000_cli_fastrprint(sock, "Host: api.airboxlab.com\n");
    res += cc3000_cli_fastrprint(sock, "Accept: */*\n");
    res += cc3000_cli_fastrprint(sock, "Content-Length: ");
    res += cc3000_cli_fastrprint(sock, str_len);
    res += cc3000_cli_fastrprint(sock, "\n\n");
    res += cc3000_cli_fastrprint(sock, "5040f3bd20c8,20.7,53.9,1255,11.1,");
    res += cc3000_cli_fastrprint(sock, str_id);
    
    return res;
}



void setup_hardware(void)
{
    // Disable watchdog if enabled by bootloader/fuses
    MCUSR &= ~(1 << WDRF);
    wdt_disable();
    
    clock_prescale_set(clock_div_1);	// Disable clock division
    sei();

    // Hardware Initialization
    usb_serial_init();
    led_init();

    millis_init();
}


int getFreeRam(void)
{
    extern int  __bss_end;
    extern int  *__brkval;
    int free_memory;
    if((int)__brkval == 0) {
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
    }
    else {
        free_memory = ((int)&free_memory) - ((int)__brkval);
    }

    return free_memory;
}

#define CC3000_SUCCESS (0)
#define CC3000_CHECK_SUCCESS(func,err_message,error_return_value)  {if ((func) != CC3000_SUCCESS) { DWIFI_ERR_PRINTLN(err_message); return error_return_value;}}



typedef enum { PFSM_POWER_UP,
    PFSM_CONNECTION,
    PFSM_SET_PROFILE,
    PFSM_GET_BACKEND_IP,
    PFSM_GET_BACKEND_IP_2,
    PFSM_START_TCP,
    PFSM_POST,
    PFSM_WAIT_FOR_REPLY,
    PFSM_OK,
    PFSM_ERROR
} post_fsm_state_t;

post_fsm_state_t post_fsm_state, prev_post_fsm_state, error_post_fsm_state; // current state, last state, state from which we got the error when we have an error 
uint8_t timeout_count, tcp_retries;
uint32_t timeout_end;
uint32_t backend_ip;
#define BACKEND_PORT                        80
#define BACKEND_NAME                        "api.airboxlab.com"
#define CONNECT_TIMEOUT_MS                  (15000)
#define GET_BACKEND_IP_TIMEOUT_MS           (3000)
#define TCP_CONNECT_RETRIES                 (3)
void* sock;
#define HTTP_REPLY_TIMEOUT_MS               (3000)
#define HTTP_RESPONSE_EXPECTED              "HTTP/1.1 200"
#define HTTP_RESPONSE_EXPECTED_LEN          12
char ret_string[HTTP_RESPONSE_EXPECTED_LEN+1];
uint8_t ret_string_pos;


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
            key_management = 0; // TODO the various posts about it diverge on this value. Some say it should be 1 (http://e2e.ti.com/support/low_power_rf/f/851/t/311630.aspx), driver code uses 0...
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
    CC3000_CHECK_SUCCESS( wlan_ioctl_del_profile(255), "ERR del prof.", 1);
    DWIFI_INFO_PRINTLN("del prof");

    // write profile
    CC3000_CHECK_SUCCESS( wlan_add_profile( encrypt_mode,   // security type
                            (uint8_t*)ssid,                 // SSID
                            strlen(ssid),                   // SSID length
                            NULL, 							// BSSID
                            1,								// Priority
                            pairwise_cipher_or_key_len,				// PairwiseCipher
                            group_cipher, 					// GroupCipher
                            key_management,					// KEY management
                            key,				            // KEY
                            key_len),                       // KEY length
                        "ERR prof", 2);
    DWIFI_INFO_PRINTLN("prof added");
    
    //set policy to use profiles and fast reconnect
    CC3000_CHECK_SUCCESS( wlan_ioctl_set_connection_policy(0, 1, 1), "ERR connect policy", 3);
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


// returns 0 as long as it hasn't finished
// if we get to the error state, error_post_fsm_state contains the state in which the error occurred.
uint8_t post_fsm(){
#if DEBUG_POST_ERR
    uint32_t retip, netmask, gateway, dhcpserv, dnsserv;
#endif
    prev_post_fsm_state = post_fsm_state;
    
    switch(post_fsm_state){
        case PFSM_POWER_UP:
            DPOST_INFO_PRINTF("power up :%lu\r\n", millis());
            cc3000_resume();
            DPOST_INFO_PRINTF("connec... :%lu\r\n", millis());
            timeout_end = millis() + CONNECT_TIMEOUT_MS;
            post_fsm_state = PFSM_CONNECTION;
            break;
        case PFSM_CONNECTION:
            // check that the CC3000 has connected
            DPOST_INFO_PRINTF( "cntd:%u dhcp:%u status:%u, get_ip:%u, %lu\r\n", cc3000_checkConnected(), cc3000_checkDHCP(), cc3000_getStatus(), cc3000_getIPAddress(&retip, &netmask, &gateway, &dhcpserv, &dnsserv), millis() );
            
            if ( cc3000_checkDHCP() ){
                // OK we're connected
                post_fsm_state = PFSM_GET_BACKEND_IP;
            } else {
                //DPOST_INFO_PRINTF("no dchp %lu\r\n", millis());
                if ( millis() > timeout_end ){
                    if (timeout_count == 0){ // first timeout
                        // try reseting the profile
                        timeout_count++;
                        post_fsm_state = PFSM_SET_PROFILE;
                    } else {
                        // this is serious, we could not connect :(
                        DPOST_ERR_PRINTLN("ERR connect/dhcp timeout");
                        post_fsm_state = PFSM_ERROR;
                    }
                } else {
                    _delay_ms(100);
                }
            }
            break;
        case PFSM_SET_PROFILE: // restore profile from the MCU's eeprom
            wifi_config_set_dev(); // TODO check that we have something saved instead... and go in PFSM_ERROR if there isn't.
            wifi_set_profile(wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode); // TODO is it too long? maybe split it? what if it fails?
            _delay_ms(10);
            cc3000_stop();
            _delay_ms(50);
            post_fsm_state = PFSM_POWER_UP;
            break;
        case PFSM_GET_BACKEND_IP:
            DPOST_INFO_PRINTLN("check backend ip");
            // this takes around a 1 second
            // TODO clean it up every hour maybe? or just put it all the time ?
            // backend_ip = 0;// HACK TEMP
            timeout_end = millis() + GET_BACKEND_IP_TIMEOUT_MS;
            timeout_count = 0;
            post_fsm_state = PFSM_GET_BACKEND_IP_2;
            break;
        case PFSM_GET_BACKEND_IP_2:
            //backend_ip = cc3000_IP2U32(192,168,1,1);
            if (backend_ip){ // we have a backend IP
                timeout_count = 0;
                post_fsm_state = PFSM_START_TCP;
            } else {
                DPOST_INFO_PRINTF("get backend ip :%lu\r\n", millis());
                if (!cc3000_getHostByName(BACKEND_NAME, &backend_ip)){
                    if (millis() > timeout_end){
                        DPOST_ERR_PRINTLN("ERR IP resolve timeout");
                        post_fsm_state = PFSM_ERROR;
                    }
                }
            }
            break;
        case PFSM_START_TCP:
            DPOST_INFO_PRINTF("connect TCP...:%lu\r\n", millis());
            sock = cc3000_connectTCP(backend_ip, BACKEND_PORT);  
            //********************
            // HUGE UGLY HACK : this has been modified to return an unconnected socket if it takes too long, and the app has to reboot the CC3000 when it fails.
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
            timeout_end = millis() + HTTP_REPLY_TIMEOUT_MS;
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
#if !DEBUG_POST_INFO // we could stop as soon as we have enough characters if we don't print debug info
                    && ret_string_pos < HTTP_RESPONSE_EXPECTED_LEN
#endif
                    );
                DPOST_INFO_PRINT("<EOF>\r\n");
                if ( !strncmp(ret_string, HTTP_RESPONSE_EXPECTED, HTTP_RESPONSE_EXPECTED_LEN) ){
                    DPOST_INFO_PRINTLN("HTTP resp OK");
                    post_fsm_state = PFSM_OK;
                } else {
                    DPOST_ERR_PRINTLN("ERR bad HTTP response");
                    post_fsm_state = PFSM_ERROR;
                }
            } else if (millis() > timeout_end){
                DPOST_ERR_PRINTLN("ERR HTTP response timeout");
                post_fsm_state = PFSM_ERROR;
            }
            break;

            
        case PFSM_OK:
            // end state
            DPOST_INFO_PRINTLN("OK!");
            end_fsm();
            _delay_ms(8); // it seems to be important to wait a little after closing the port and before shutting down the CC3000...
            cc3000_stop();
            break;
        case PFSM_ERROR:
            // end state
            end_fsm();
            _delay_ms(8); // it seems to be important to wait a little after closing the port and before shutting down the CC3000...
            cc3000_stop();
            DPOST_INFO_PRINTLN("ERROR!");
            break;
    }
    
    if (prev_post_fsm_state != post_fsm_state && post_fsm_state == PFSM_ERROR){
        error_post_fsm_state = prev_post_fsm_state;
    }
    
    return (prev_post_fsm_state == PFSM_OK || prev_post_fsm_state == PFSM_ERROR);
}

int main(void){
    setup_hardware();
    
    // HACK just some time to connect to the serial...
    led_set(100,100,100);
    _delay_ms(2000);
    led_clear();
    
    
    if (cc3000_init() && wifi_check_version()){
        DPOST_INFO_PRINTF("ref %lu\r\n", millis());
        cc3000_stop();
        led_set(0,255,0);
        _delay_ms(500);
        led_clear();
    } else {
        while(1){
            led_set(255,0,0);
            Delay_MS(500);
            led_clear();
            Delay_MS(500);
        }
    }
    millis_reset();
    while (1){
            
        if (millis() > 1000 ){ // main loop
            reset_fsm();
            while (!post_fsm()){
                // todo check accel here, and other things that needs to be done all the time...
                Delay_MS(10);
            }
            
            DPOST_INFO_PRINTF("Free RAM :%u\r\n", getFreeRam());
            
            post_id++;
            millis_reset(); // reset it here to make sure there is no wrapping, but nowhere else!
        }
    }
    
}




/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    //LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    //LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

// *********************  Soft reset/bootloader functionality *******************************************
// The bootloader can be ran by opening the serial port at 1200bps. Upon closing it, the reset process will be initiated
// and two seconds later, the board will re-enumerate as a DFU bootloader.

uint8_t needs_bootload = false;

void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo){
    if (CDCInterfaceInfo->State.LineEncoding.BaudRateBPS == 1200){
        needs_bootload = true;
    }
}

// adapted from http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=1008792#1008792
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo){
    static uint8_t PreviousDTRState = false;
    uint8_t        CurrentDTRState  = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR);

    //is_open = CurrentDTRState;

    /* Check how the DTR line has been asserted */
    if (PreviousDTRState && !(CurrentDTRState) ){
        // PreviousDTRState == True AND CurrentDTRState == False
        // Host application has Disconnected from the COM port
        
        if (needs_bootload){
            Jump_To_Reset(true);
        }
    }
    PreviousDTRState = CurrentDTRState;
}
// ******************************************************************************************************


