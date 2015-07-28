/*
 * usb_serial.c
 *
 * Created: 04/01/2014 04:25:58
 *  Author: Xevel
 */ 

#include "common.h" // this include is before its own .h to avoid an include loop 
#include "usb_serial.h"

#include <Hardware_lib/reset.h>


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
 FILE USBSerialStream;

volatile uint8_t is_open;


void usb_serial_init(){
    USB_Init();

    /* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
    CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
}

uint8_t usb_serial_is_open(){
    return is_open;
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


/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
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

    is_open = CurrentDTRState;

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

