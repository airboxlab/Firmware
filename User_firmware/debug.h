/*
 * debug.h
 *
 * Created: 02/02/2014 03:54:42
 *  Author: Xevel
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_
    
    #include "usb_serial.h"
    #define DEBUG_WIFI_ERR          (0)
    #define DEBUG_WIFI_INFO         (0)

    #define DEBUG_POST_ERR          (0)
    #define DEBUG_POST_INFO         (0)
    
    #define DEBUG_CONFIG_INFO       (0)
    #define DEBUG_CONFIG_ERR        (0)

    #define DEBUG_SENSOR_INFO       (1)
    #define DEBUG_SENSOR_ERR        (0)
	
	#define DEBUG_CALIB				(0)


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


	#if DEBUG_CALIB
	#define DCALIB_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
	#define DCALIB_PRINT(str)  {print(str);usb_serial_task();}
	#else 
	#define DCALIB_PRINTF(str,...)
	#define DCALIB_PRINT(str)
	#endif
	
    #define CC3000_WIFI_CHECK_SUCCESS(func,err_message,error_return_value)  {if ((func) != CC3000_SUCCESS) { DWIFI_ERR_PRINTLN(err_message); return error_return_value;}}



    #if DEBUG_CONFIG_INFO
    #define DCONFIG_INFO_PRINT(str)  {print(str);usb_serial_task();}
    #define DCONFIG_INFO_PRINTLN(str)  {println(str);usb_serial_task();}
    #define DCONFIG_INFO_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
    #define DCONFIG_INFO_LED(r,g,b)  {led_set(r,g,b);}
    #else
    #define DCONFIG_INFO_PRINT(str)
    #define DCONFIG_INFO_PRINTLN(str)
    #define DCONFIG_INFO_PRINTF(str,...)
    #define DCONFIG_INFO_LED(r,g,b)
    #endif

    #if DEBUG_CONFIG_ERR
    #define DCONFIG_ERR_PRINT(str)  {print(str);usb_serial_task();}
    #define DCONFIG_ERR_PRINTLN(str)  {println(str);usb_serial_task();}
    #define DCONFIG_ERR_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
    #define DCONFIG_ERR_LED(r,g,b)  {led_set(r,g,b);}
    #else
    #define DCONFIG_ERR_PRINT(str)
    #define DCONFIG_ERR_PRINTLN(str)
    #define DCONFIG_ERR_PRINTF(str,...)
    #define DCONFIG_ERR_LED(r,g,b)
    #endif


    #define CC3000_TEST_SUCCESS(func,err_message)  {if ((func) != CC3000_SUCCESS) { DCONFIG_ERR_PRINTLN(err_message);}}


    #if DEBUG_SENSOR_INFO
    #define DSENSOR_INFO_PRINT(str)  {print(str);usb_serial_task();}
    #define DSENSOR_INFO_PRINTLN(str)  {println(str);usb_serial_task();}
    #define DSENSOR_INFO_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
    #define DSENSOR_INFO_LED(r,g,b)  {led_set(r,g,b);}
    #else
    #define DSENSOR_INFO_PRINT(str)
    #define DSENSOR_INFO_PRINTLN(str)
    #define DSENSOR_INFO_PRINTF(str,...)
    #define DSENSOR_INFO_LED(r,g,b)
    #endif

    #if DEBUG_SENSOR_ERR
    #define DSENSOR_ERR_PRINT(str)  {print(str);usb_serial_task();}
    #define DSENSOR_ERR_PRINTLN(str)  {println(str);usb_serial_task();}
    #define DSENSOR_ERR_PRINTF(str,...)  {fprintf(&USBSerialStream, str, __VA_ARGS__);usb_serial_task();}
    #define DSENSOR_ERR_LED(r,g,b)  {led_set(r,g,b);}
    #else
    #define DSENSOR_ERR_PRINT(str)
    #define DSENSOR_ERR_PRINTLN(str)
    #define DSENSOR_ERR_PRINTF(str,...)
    #define DSENSOR_ERR_LED(r,g,b)
    #endif



#endif /* DEBUG_H_ */