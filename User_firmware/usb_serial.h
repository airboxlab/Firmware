/*
 * USBserial.h
 *
 * Created: 04/01/2014 04:25:28
 *  Author: Xevel
 */
#ifndef USBSERIAL_H_
#define USBSERIAL_H_

    #include "Descriptors.h"
    #include <LUFA/Drivers/USB/USB.h>

    extern FILE USBSerialStream;

    void usb_serial_init(); // call once at the very beginning
    void usb_serial_task(); // call regularly to process characters
    
    uint8_t usb_serial_is_open();

    int16_t usb_serial_get_nb_received();
    uint8_t usb_serial_get_byte();

    // Some helpers for easy printing
    void println(const char* str);
    void print(const char* str);
    
    void usb_serial_send(const char c);
    
    void EVENT_USB_Device_Connect(void);
    void EVENT_USB_Device_Disconnect(void);
    void EVENT_USB_Device_ConfigurationChanged(void);
    void EVENT_USB_Device_ControlRequest(void);
    void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo);
    void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo);


#endif /* USBSERIAL_H_ */