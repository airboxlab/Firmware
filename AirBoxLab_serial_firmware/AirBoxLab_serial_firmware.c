/*
AirBoxLab v2

This code was used for the v1, and modifications are not complete yet for V2.


*/



/*
             LUFA Library
     Copyright (C) Dean Camera, 2013.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "AirBoxLab_serial_firmware.h"

#include <Hardware_lib/common.h>
#include <Hardware_lib/led.h>
#include <Hardware_lib/sht21.h>
#include <Hardware_lib/sensor_board.h>
#include <Hardware_lib/motor.h>
#include <Hardware_lib/power.h>
#include <Hardware_lib/gp2y.h>
#include <Hardware_lib/iaqengine.h>
#include <Hardware_lib/cc2dxx.h>
#include <Hardware_lib/mma8453q.h>
#include <CC3000/CC3000.h>
#include "wifi_config.h"
#include <Hardware_lib/reset.h>



/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
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

uint8_t val = 1;
uint8_t addd = 1;
bool do_charge_test = false;
uint8_t MCUCR_old;

//
//void charging_test(){
	//if (val == 200 ){
		//addd = -1;
		//} else if (val == 0){
		//addd=1;
	//}
	//val += addd;
	//if (power_charging()){
		//led_set(val, 0, 0);
	//} else {
		//led_set(0, val, 0);
	//}
	//Delay_MS(5);
	//
//}
//
/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
    
    bitSet(DDRC,7); // HACK DEBUG
    
	SetupHardware();

	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	GlobalInterruptEnable();

	for (;;)
	{
		process_bytes();
		
		/*if(do_charge_test){
			charging_test();
		}*/
		
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}




/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
    sei();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	USB_Init();
    
    led_init();
    //motor_init();
    //gp2y_init();
    //cc2dxx_init();
    sensor_board_init();
    //mma8435q_init();
    //led_set(0,0,100);
    //if (cc3000_init()){
    //    led_set(0,100,0);
    //} else {
    //    led_set(100,0,0);
    //}
    
	/*sensor_board_init();
	power_init();
	mq135_init();
	iaqengine_init();
	wifi_init();
	
	sensor_board_on(true);
    */
}

void println(const char* str){
	fputs(str, &USBSerialStream);
	fputs("\r\n", &USBSerialStream);
}
void print(const char* str){
	fputs(str, &USBSerialStream);
}


void process_bytes(){
	uint16_t nb_received = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);
	uint8_t c;
	
    
	for( uint16_t i = 0; i < nb_received ; i++ ){
		c = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
		
		switch (c){
			//case 'h':
				//println("AirBoxLab serial firmware - for tests only");
				//println("Commands:");
				//println("h : this help");
				//println("l : run LED test cycle");
				//println("m : toggle motor");
				//println("k : print humidity and temperature");
                //println("t : print tilt sensor");
                //
                //println("w : test CC3000...");
                //
				//println("c : toggle charge test (disabled JTAG temporarily)");
				//println("b : print battery voltage");
				//println("g : print MQ-135 voltage (sensor board needs to be ON)");
				//println("d : print GP2Y voltage");
				//println("J : WARNING toggles JTAG interface (automatically disabled for CHARGING, not for WIFI_CONNECT)");
				//println("i : print iAQengine (sensor board needs to be ON)");
				//println("k : print humidity and temperature (sensor board needs to be ON)");
				//break;
            case '7':{
                uint8_t is_on;
                sensor_board_heater_is_on(&is_on);
                fprintf(&USBSerialStream, "Sensor board ON : %u\r\n", is_on);
                sensor_board_heater_on(1);
                sensor_board_heater_is_on(&is_on);
                fprintf(&USBSerialStream, "Sensor board ON : %u\r\n", is_on);
                }break;
            case '8':{
                uint16_t val;
                sensor_board_get_mq135(&val);
                fprintf(&USBSerialStream, "MQ135 : %u\r\n", val);
            }break;/*
			case 't':{
                int16_t acc_x, acc_y, acc_z;
                mma8435q_getAccXYZ(&acc_x, &acc_y, &acc_z, 1);
                double norm = sqrt((double)acc_x*(double)acc_x+ (double)acc_y*(double)acc_y + (double)acc_z*(double)acc_z)/255.0; 
                fprintf(&USBSerialStream, "Accel: %d, %d, %d (%f)\r\n", acc_x, acc_y, acc_z, norm );
				
                }break;
			case 'y':
                if (cc3000_connectToAP("linksys", "", 0)){
                    wlan_ioctl_set_connection_policy(0, 0, 0);
                    // TODO add wlan_ioctl_set_connection_policy(0, 0, 0); and reset the CC3000
                    println("Connected to wifi");
                } else {
                    println("Couldn't connect to wifi");
                }
                
                while(!cc3000_checkDHCP()){
                    Delay_MS(100);
                    print(".");
                }
                
                uint32_t retip, netmask, gateway, dhcpserv, dnsserv;
                cc3000_getIPAddress(&retip, &netmask, &gateway, &dhcpserv, &dnsserv);
                fprintf(&USBSerialStream, "IP: %d.%d.%d.%d\r\n", (uint8_t)(retip>>24), (uint8_t)(retip>>16), (uint8_t)(retip>>8), (uint8_t)(retip));
                fprintf(&USBSerialStream, "Netmask: %d.%d.%d.%d\r\n", (uint8_t)(netmask>>24), (uint8_t)(netmask>>16), (uint8_t)(netmask>>8), (uint8_t)(netmask));
                fprintf(&USBSerialStream, "Gateway: %d.%d.%d.%d\r\n", (uint8_t)(gateway>>24), (uint8_t)(gateway>>16), (uint8_t)(gateway>>8), (uint8_t)(gateway));
                fprintf(&USBSerialStream, "DHCPServ: %d.%d.%d.%d\r\n", (uint8_t)(dhcpserv>>24), (uint8_t)(dhcpserv>>16), (uint8_t)(dhcpserv>>8), (uint8_t)(dhcpserv));
                fprintf(&USBSerialStream, "DNSServ: %d.%d.%d.%d\r\n", (uint8_t)(dnsserv>>24), (uint8_t)(dnsserv>>16), (uint8_t)(dnsserv>>8), (uint8_t)(dnsserv));
                if (cc3000_checkDHCP()){
                    println("Got IP from DHCP");
                } else {
                    println("Couldn't get DHCP lease");
                }
                
                if (cc3000_ping(cc3000_IP2U32(192,168,1,1), 3, 500, 32)){
                    println("pinged 192.168.1.1 successfully");
                } else {
                    println("192.168.1.1 not pinged");
                }
                
                break;
			//case 'l':
				//// LED test : cycle each led up and down then all 3 at the same time
				//println("starting LED test cycle");
				//println("Red (0 to 255 in 2.55s)...");
				//CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
				//USB_USBTask();
				//for (uint8_t x = 0; x < 255; x++){
					//led_set(x,0,0);
					//Delay_MS(10);
				//}
				//println("Green (0 to 255 in 2.55s)...");
				//CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
				//USB_USBTask();
				//for (uint8_t x = 0; x < 255; x++){
					//led_set(0,x,0);
					//Delay_MS(10);
				//}
				//println("Blue (0 to 255 in 2.55s)...");
				//CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
				//USB_USBTask();
				//for (uint8_t x = 0; x < 255; x++){
					//led_set(0,0,x);
					//Delay_MS(10);
				//}
				//println("All at once (0 to 255 in 2.55s)...");
				//CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
				//USB_USBTask();
				//for (uint8_t x = 0; x < 255; x++){
					//led_set(x,x,x);
					//Delay_MS(10);
				//}
				//println("All at 255 blinking 3 times...");
				//CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
				//USB_USBTask();
				//led_clear();
				//Delay_MS(200);
				//led_set(255, 255, 255);
				//Delay_MS(200);
				//led_clear();
				//Delay_MS(200);
				//led_set(255, 255, 255);
				//Delay_MS(200);
				//led_clear();
				//Delay_MS(200);
				//led_set(255, 255, 255);
				//Delay_MS(200);
				//led_clear();
				//println("LED test finished");
				//break;
			//case 'm':
				//switch (motor_get()){
					//case 0:
						//println("Motor ON (PWM @255)");
						//motor_set(255);
						//break;
					//case 255:
						//println("Motor ON (PWM @100)");
						//motor_set(100);
						//break;
					//case 100:
						//println("Motor OFF");
						//motor_set(0);
						//break;
				//}
				//break;
			//case 'c':
				//do_charge_test = !do_charge_test;
				//if (do_charge_test){
					////disable JTAG
					//MCUCR = (1<<JTD);
					//MCUCR = (1<<JTD);
					//println("Start charge test: RED pulse = charging, GREEN pulse = not charging...");
				//} else {
					////enable JTAG
					//MCUCR = MCUCR_old;
					//println("End of charge test");
					//led_clear();
				//}
				//break;
			//case 'b':
				//fprintf(&USBSerialStream, "Battery voltage: %d mV\r\n", power_battery_get_voltage() );
				//break;
			//case 'g':
				////fprintf(&USBSerialStream, "MQ-135 voltage: %d mV\r\n", mq135_get_voltage() );
				//break;
			//case 'd':
				//fprintf(&USBSerialStream, "GP2Y voltage: %d mV\r\n", gp2y_get_voltage() );
				//break;
				//
			//case 'J':
				//if (bit_is_clear(MCUCR, JTD)){
					//MCUCR = (1<<JTD);
					//MCUCR = (1<<JTD);
					//println("JTAG interface DISABLED");
				//} else {
					//MCUCR = 0;
					//MCUCR = 0;
					//println("JTAG interface ENABLED");
				//}
				//break;
			//case 'i':
				//fprintf(&USBSerialStream, "iAQengine: %d\r\n", iaqengine_get_ppm());
				//break;
			case 'k':
				fprintf(&USBSerialStream, "humidity: %f %%\r\n", cc2dxx_get_humidity());
				Delay_MS(100);
				fprintf(&USBSerialStream, "temp: %f deg C\r\n", cc2dxx_get_temperature());
				break;
			case 'w':{
                printf("Testing CC3000...");
                CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                USB_USBTask();
                Delay_MS(500);
                
				uint8_t major, minor;
                if (cc3000_getFirmwareVersion(&major, &minor)){
                    fprintf(&USBSerialStream, "V%d.%d\r\n", major,minor);
                } else {
                    println("Could not get version");
                }
                uint8_t mac[6];
                if (cc3000_getMacAddress(mac)){
                    fprintf(&USBSerialStream, "MAC:%02X%02X%02X%02X%02X%02X\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                } else {
                    println("Could not get MAC");
                }
                
				}break;
				*/
			default:
				println("press h for help");
				break;
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

	//LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
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
    static bool PreviousDTRState = false;
    bool        CurrentDTRState  = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR);

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
