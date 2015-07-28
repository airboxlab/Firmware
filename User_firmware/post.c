/*
 * post.c
 *
 * Created: 04/01/2014 05:26:27
 *  Author: Xevel
 */ 

#include "post.h"
#include "sensor.h"
#include "usb_serial.h"
#include "post_config.h"
#include "board.h"
#include "util_timer.h"

#define POST_STR_LEN 64 // max length of the data part of the string sent for the POST
char send_str[POST_STR_LEN]; // csv data string
char send_len[3]; // string representation of the length of the data string
char mac_str[13]; // mac with terminating null char

// format is <MAC>,<tmp>,<hum>,<voc>,<pm>,<co2>
// <MAC> 12 hexa char, all in maj, no spaces, with padding. ex: 0800F3BD20C8 (already there)
    
// <tmp> fixed point, xx.x . ex: 20.7   // TODO min/max
// <hum> fixed point, xx.x . ex: 53.9   // TODO min/max
// <voc> int. ex: 1255                  // TODO min/max
// <pm>  fixed point, xx.x . ex: 11.1   // TODO min/max
// <co2>  byte. ex: 78                   // TODO min/max


// return the HEX character corresponding to the value (in 0-F)
uint8_t oct2hex(uint8_t val){
    val &= 0xF;
    if ( val < 0xA){
        return val + '0';
    }
    return val - 0xA + 'A';
    
}

void sprintf_02X(char* dst, uint8_t data){
    dst[0] = oct2hex(data>>4);
    dst[1] = oct2hex(data);
}



void post_init(){ // requires the CC3000 to be ON
#ifdef DEV
	#ifdef ABL1

	#elif defined(ABL2)

	#elif defined(ABL3)
		strcpy(mac_str, "08002859C93A");
		
	#elif defined(ABL4)

	#elif defined(ABL5)
		strcpy(mac_str, "08002859CC10");
	#elif defined(ABL6)
		
	#elif defined(ABL7)
		strcpy(mac_str, "08002859C92E");
	#elif defined(ABL8)

	#elif defined(ABL9)

	#elif defined(ABL10)
	
	#elif defined(ABLNAKED)
		strcpy(mac_str, "0800285753D3");
	#else
		// In dev mode we use always the same MAC to post
		strcpy(mac_str, "080028575399");
	#endif
#else
    uint8_t mac[6];
    if (cc3000_getMacAddress(mac)){
        for (uint8_t i = 0; i<6; i++){
            sprintf_02X(mac_str+(i*2), mac[i]);
        }
    }
#endif
    send_str[0] = ',';
}

char* get_mac_str(){
    return mac_str;
}

uint8_t post_do(void* sock){
    post_update();
    return post_send(sock);
}


int16_t sprintf_double_fixed( char* __s , double val, uint8_t precision ){ 
    int32_t ipart = (uint32_t) val;
    int nb_written = sprintf(__s, "%lu.", ipart);

    val -= ipart;
    for (uint8_t i = 0; i < precision; i++){
        val *= 10;
        uint8_t digit_value = (uint8_t)val;
        val -= digit_value;

        __s[nb_written++] = digit_value + '0';
    }
    __s[nb_written] = '\0'; // append null char, but do not count it, just like sprintf does
    
    return nb_written;
}


// updates the string that will be sent with the sensor values
void post_update(){
    uint16_t nb_val = sensors_acc.nb_val;
    
    //sprintf(send_str+1, "%.1f,%.1f,%u,%.1f,%u\n", sensors_acc.tmp/nb_val, sensors_acc.hum/nb_val, (uint16_t)(sensors_acc.voc/nb_val), sensor_pm, (uint16_t)(sensors_acc.co2/nb_val) );
    
    uint8_t pos =  1;
    pos += sprintf_double_fixed(send_str + pos, sensors_acc.tmp/nb_val, 1);
    send_str[pos] = ',';
    pos += 1;
    pos += sprintf_double_fixed(send_str + pos, sensors_acc.hum/nb_val, 1);
	
    pos += sprintf(send_str + pos, ",%u,", (uint16_t)(sensors_acc.voc/nb_val) );
	//Send the max value of pm during the period
    pos += sprintf_double_fixed(send_str + pos, sensors_acc.pm, 1);
    pos += sprintf(send_str + pos, ",%u", sensors_acc.co2/nb_val );
    pos += sprintf(send_str + pos,",%lu", sensors_acc.ohm/nb_val);
	pos += sprintf(send_str + pos,",%u", sensors_acc.raw2/nb_val);
	pos += sprintf(send_str + pos, ",%lu\r\n", sensors_acc.raw3/nb_val);
    sensors_reset_acc();
    snprintf(send_len, 3, "%u", strlen(send_str)-1+12 );
}


uint8_t post_send(void* sock){
    uint16_t res;
if (!double_tap_detected)
{
		res  = cc3000_cli_fastrprint(sock, "POST /v1/airboxlab/ HTTP/1.1\n");
}
#ifdef DOUBLE_TAP_ENABLED
else 
{	
	res  = cc3000_cli_fastrprint(sock, "POST /v1/airboxlab/doubletap HTTP/1.1\n");
	double_tap_detected=0;
}  
#endif
	res += cc3000_cli_fastrprint(sock, USER_AGENT);
#ifdef DEV
    res += cc3000_cli_fastrprint(sock, "Host: api-test.airboxlab.com\n");
#else 
	res += cc3000_cli_fastrprint(sock, "Host: api.airboxlab.com\n");
#endif
    res += cc3000_cli_fastrprint(sock, "Accept: */*\n");
    //res += cc3000_cli_fastrprint(sock, "Content-Type: application/json\n"); // Removed since it causes the back-end to take the post for a java object
    res += cc3000_cli_fastrprint(sock, "Content-Length: ");
    res += cc3000_cli_fastrprint(sock, send_len);
    res += cc3000_cli_fastrprint(sock, "\n\n");
    res += cc3000_cli_fastrprint(sock, mac_str);
    res += cc3000_cli_fastrprint(sock, send_str);
    
    
    //fprintf(&USBSerialStream, "written: %u / %u\r\n", res, 171);
    //CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    //USB_USBTask();
    return res;
}



uint8_t post_get_leds(void* sock){
    uint16_t res;
    res  = cc3000_cli_fastrprint(sock, "GET /v1/leds/");
    res += cc3000_cli_fastrprint(sock, mac_str);
    res += cc3000_cli_fastrprint(sock, " HTTP/1.1\n");
    res += cc3000_cli_fastrprint(sock, "Host: api.airboxlab.com\n");
    res += cc3000_cli_fastrprint(sock, "Content-Length: 0\n\n");
    return res;
}


#if 0 // simplified post that does not require sensors

uint8_t post_id;

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
    
    post_id++;

    return res;
}
#endif
