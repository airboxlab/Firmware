/*
AirBoxLab v2

Test Firmware. This firmware is used to test the production units in conjunction
with the Automated Test app.

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


#include "Test_firmware.h"

#include <Hardware_lib/common.h>
#include <Hardware_lib/led.h>
//#include <Hardware_lib/sht21.h>
#include <Hardware_lib/sensor_board.h>
#include <Hardware_lib/motor.h>
#include <Hardware_lib/power.h>
//#include <Hardware_lib/gp2y.h>
#include <Hardware_lib/iaqengine.h>
#include <Hardware_lib/cc2dxx.h>
#include <Hardware_lib/mma8453q.h>
#include <CC3000/CC3000.h>
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



int main(void)
{
    
	SetupHardware();

	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	GlobalInterruptEnable();

	for (;;)
	{
		process_bytes();
		
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}



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
    motor_init();
    //gp2y_init(); // we don't test it :/
    cc2dxx_init();
    mma8435q_init();
    
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
            case 'a':{ // test all, automatic
                // Not tested: Grove, GP2Y
                
                // Test that accel talks and gives reasonably good results (norm should give ~1g)
            	int16_t acc_x = 0, acc_y = 0, acc_z = 0;
                mma8435q_getAccXYZ(&acc_x, &acc_y, &acc_z, true);
                double norm = sqrt((double)acc_x*(double)acc_x+ (double)acc_y*(double)acc_y + (double)acc_z*(double)acc_z)/255.0; 
                if (norm > 1.2 || norm < 0.8 || ((acc_x == acc_y) && (acc_y == acc_z))){
                    println("MMA8435 error");
                } else {
                    println("MMA8435 OK");
                }
                CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                USB_USBTask();
                
                //Test cc2dxx
                float rh = cc2dxx_get_humidity();
                float temp = cc2dxx_get_temperature();
                if (  rh < 0.01 ||  temp < -39.9 ){
                    println("CC2D23 error");
                } else {
                    println("CC2D23 OK");
                }
                CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                USB_USBTask();
                
                // test ballpark : VBAT, VRAW... ?
                
                // Test continuously: LEDS, motor
                for (uint8_t x = 0; x < 255; x++){
                    led_set(x,0,0);
                    Delay_MS(2);
                }
                for (uint8_t x = 0; x < 255; x++){
                    led_set(0,x,0);
                    Delay_MS(2);
                }
                for (uint8_t x = 0; x < 255; x++){
                    led_set(0,0,x);
                    Delay_MS(2);
                }
                Delay_MS(200);
                led_set(75,50,40);
                motor_set(250);
                
                // test WIFI last since it can block
                uint8_t major, minor;
                if (cc3000_init() && cc3000_getFirmwareVersion(&major, &minor)){
                    println("CC3000 OK");
                } else {
                    println("CC3000 error");
                }
                CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                USB_USBTask();
                
                for (uint8_t x = 0; x < 255; x++){
                    led_set(x,0,0);
                    Delay_MS(2);
                }
                for (uint8_t x = 0; x < 255; x++){
                    led_set(0,x,0);
                    Delay_MS(2);
                }
                for (uint8_t x = 0; x < 255; x++){
                    led_set(0,0,x);
                    Delay_MS(2);
                }
                led_set(75,50,40);
                
                }break;
            default:
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
