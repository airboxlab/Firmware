/*
AirBoxLab v2

*/

//****************   interrrupters  **********************
#define WIFI                // when commented, disables wifi support to save space for debugging
#define DEV                 // when commented, disables developer mode. This must be commented for releases.
//********************************************************

#include "PatchProgrammer_firmware.h"

#include <Hardware_lib/common.h>
#include <Hardware_lib/reset.h>
#include <Hardware_lib/led.h>


#include <CC3000/CC3000.h>

#include <CC3000/Adafruit_CC3000_Library_20131020/ccspi.h> // HACK to get access to low level SPI
#include <CC3000/Adafruit_CC3000_Library_20131020/utility/nvmem.h> // HACK to get access to nvmem functions
#include <CC3000/Adafruit_CC3000_Library_20131020/utility/wlan.h> // HACK to get access to low level wlan stuff
#include <avr/eeprom.h>
#include "data.h"


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

// Some helper macros for quick printing
#define println(str)  fputs(str, &USBSerialStream);fputs("\r\n", &USBSerialStream)
#define print(str)    fputs(str, &USBSerialStream)

#define MAC_SIZE    6        // how many bytes in a MAC address
uint8_t macBuffer[MAC_SIZE];    // the buffer for the MAC addr
#define MAC_ADDRESS_EEPROM_LOCATION    ((uint8_t*)113)    // where in EEPROM to write the MAC address for safekeeping


#define RM_SIZE        128        // how many bytes in the CC3000 RM file
uint8_t rmBuffer[RM_SIZE];        // our RM buffer
#define RM_ADDRESS_EEPROM_LOCATION ((uint8_t*)(MAC_ADDRESS_EEPROM_LOCATION + MAC_SIZE))        // where in EEPROM to write the RM file for safekeeping


#define BIT0    0x01
#define BIT1    0x02
#define BIT2    0x04
#define BIT3    0x08
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7    0x80


void DisplayMenu();

void do_usb(){
    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    USB_USBTask();
}

int main(void)
{
    SetupHardware();
    GlobalInterruptEnable();
    
    /* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
    CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

    println("CC3000 Patcher");
    
    for (;;)
    {
        process_bytes();
        do_usb();
    }
}




/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    GlobalInterruptEnable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    /* Hardware Initialization */
    USB_Init();
    //led_init();
}


uint8_t wifi_start_driverless(){
    uint8_t res = cc3000_init2(2,false);
    return res;
}

void wifi_stop(){
    //wlan_stop(); // BIG HACK
}

void wifi_print_RM_Record(void) {
   for (int i=0; i<8; i++) {
        for (int j=0; j<16; j++) {
            fprintf(&USBSerialStream , " %02X ", rmBuffer[i*16+j]);
        }
        print("\r\n");
    }
}

void wifi_print_MAC_buff(){
    fprintf(&USBSerialStream, "MAC: %02X %02X %02X %02X %02X %02X\r\n", macBuffer[0], macBuffer[1], macBuffer[2], macBuffer[3], macBuffer[4], macBuffer[5]);
}
uint8_t wifi_print_MAC(){
    if (cc3000_getMacAddress(macBuffer)){
        wifi_print_MAC_buff();
        return true;
    } else {
        println("MAC ERR");
        return false;
    }
}


void ShowEEPROM(void) {
    println("MAC & radio file info from MCU EEPROM");

    for (int i=0; i<MAC_SIZE; i++) {
        macBuffer[i] = eeprom_read_byte(MAC_ADDRESS_EEPROM_LOCATION+i);
    }
    for (int i=0; i<RM_SIZE; i++) {
        rmBuffer[i] = eeprom_read_byte(RM_ADDRESS_EEPROM_LOCATION+i);
    }
    
    wifi_print_MAC();
    wifi_print_RM_Record();
}


uint8_t wifi_print_version(){
    uint8_t major, minor;
    if (cc3000_getFirmwareVersion(&major, &minor)){
        fprintf(&USBSerialStream, "V%d.%d\r\n", major,minor);
        return true;
    } else {
        println("Version ERROR");
        return false;
    }
}

void BasicCC30000Test(void) {
    println("Testing...");
    //led_set(0,0,100);
    
    print("init...");
    if (!cc3000_init()) {
        //led_set(100,0,0);
        println("ERR");
        do_usb();
        for (;;);
    }
    println("OK");
    //led_set(0,100,0);
            
    wifi_print_MAC();
    wifi_print_version();
    
    fprintf(&USBSerialStream, "RX buffer: %u\r\n",  CC3000_RX_BUFFER_SIZE);
    fprintf(&USBSerialStream, "TX buffer: %u\r\n",  CC3000_TX_BUFFER_SIZE);
    
    wifi_stop();
}

/*
void UseDefaults(void) {
    println("Copy default Radio File to Arduino EEPROM");

    
//    println("    Generating random MAC...");
//    for (int i=0; i<MAC_SIZE; i++) {
//        EEPROM.write(MAC_ADDRESS_EEPROM_LOCATION+i, random(256));
//    }
  

    println("    Copying default RM file data...");
    for (int i=0; i<RM_SIZE; i++) {
        uint8_t zaz = pgm_read_byte(cRMdefaultParams + i);
        eeprom_write_byte(RM_ADDRESS_EEPROM_LOCATION+i, zaz);
    }
    println("        Verifying...");
    for (int i=0; i<RM_SIZE; i++) {
        uint8_t zaz = pgm_read_byte(cRMdefaultParams + i);
        if (eeprom_read_byte(RM_ADDRESS_EEPROM_LOCATION+i)  != zaz) {
            println("            Verify failed. Aborting.");
            for (;;);
        }
    }
    println("        Verify OK");
    
    println("    Done!");
}
*/


//*****************************************************************************
//
//! fat_read_content
//!
//! \param[out] is_allocated  array of is_allocated in FAT table:\n
//!                         an allocated entry implies the address and length of the file are valid.
//!                           0: not allocated; 1: allocated.
//! \param[out] is_valid  array of is_valid in FAT table:\n
//!                         a valid entry implies the content of the file is relevant.
//!                           0: not valid; 1: valid.
//! \param[out] write_protected  array of write_protected in FAT table:\n
//!                         a write protected entry implies it is not possible to write into this entry.
//!                           0: not protected; 1: protected.
//! \param[out] file_address  array of file address in FAT table:\n
//!                         this is the absolute address of the file in the EEPROM.
//! \param[out] file_length  array of file length in FAT table:\n
//!                         this is the upper limit of the file size in the EEPROM.
//!
//! \return on succes 0, error otherwise
//!
//! \brief  parse the FAT table from eeprom 
//
//*****************************************************************************
unsigned char fat_read_content(unsigned char *is_allocated,
                               unsigned char *is_valid,
                               unsigned char *write_protected,
                               unsigned short *file_address,
                               unsigned short *file_length)
{
    unsigned short  index;
    unsigned char   ucStatus;
    unsigned char   fatTable[48];
    unsigned char*  fatTablePtr = fatTable;
    
    // read in 6 parts to work with tiny driver
    for (index = 0; index < 6; index++)
    {
        ucStatus = nvmem_read(16, 8, 4 + 8*index, fatTablePtr); 
        fatTablePtr += 8;
    }
    
    fatTablePtr = fatTable;
    
    for (index = 0; index <= NVMEM_RM_FILEID; index++)
    {
        *is_allocated++ = (*fatTablePtr) & BIT0;
        *is_valid++ = ((*fatTablePtr) & BIT1) >> 1;
        *write_protected++ = ((*fatTablePtr) & BIT2) >> 2;
        *file_address++ = ((*(fatTablePtr+1)<<8) | (*fatTablePtr)) & (BIT4|BIT5|BIT6|BIT7);
        *file_length++ = ((*(fatTablePtr+3)<<8) | (*(fatTablePtr+2))) & (BIT4|BIT5|BIT6|BIT7);
        
        fatTablePtr += 4;  // move to next file ID
    }
    
    return ucStatus;
}


#define NUM_FAT_RECORDS    (NVMEM_RM_FILEID + 1)

void fat_print_helper(uint8_t x){
    if (x--){
        if (x){
            print("?");
        } else {
            print("Y");
        }
    } else {
        print("N");
    }
    print("    ");
}

void ShowFAT(void) {

    unsigned char isAllocated[NUM_FAT_RECORDS],
        isValid[NUM_FAT_RECORDS],
        writeProtected[NUM_FAT_RECORDS];
    unsigned short fileAddress[NUM_FAT_RECORDS], fileLength[NUM_FAT_RECORDS];
    
    println("File Table");    

    wifi_start_driverless();    
    
    uint8_t status = fat_read_content(isAllocated, isValid, writeProtected, fileAddress, fileLength);
    if (status==0) {
        println("Active, Valid, Write_protected, Start_addr, Length");
        for (int i=0; i<NUM_FAT_RECORDS; i++) {
            fprintf(&USBSerialStream, " %u ", i);
            
            fat_print_helper(isAllocated[i]);
            fat_print_helper(isValid[i]);
            fat_print_helper(writeProtected[i]);
            fprintf(&USBSerialStream, "%X  %X \r\n", fileAddress[i], fileLength[i]);
            }

        }
    else {
        println("Unable to read FAT");
    }
        
    wifi_stop();
}




// 2 dim array to store address and length of new FAT
const unsigned short aFATEntries[2][NVMEM_RM_FILEID + 1] = 
/*  address     */  {{0x50,     0x1f0,     0x390,     0x1390,     0x2390,     0x4390,     0x6390,     0x63a0,     0x63b0,     0x63f0,     0x6430,     0x6830},
/*  length    */    {0x1a0,     0x1a0,     0x1000,     0x1000,     0x2000,     0x2000,     0x10,     0x10,     0x40,     0x40,     0x400,     0x200}};
/* 0. NVS */
/* 1. NVS Shadow */
/* 2. Wireless Conf */
/* 3. Wireless Conf Shadow */
/* 4. BT (WLAN driver) Patches */
/* 5. WiLink (Firmware) Patches */
/* 6. MAC addr */
/* 7. Frontend Vars */
/* 8. IP config */
/* 9. IP config Shadow */
/* 10. Bootloader Patches */
/* 11. Radio Module params */
/* 12. AES128 for smart config */
/* 13. user file */
/* 14. user file */
/* 15. user file */



//*****************************************************************************
//
//! fat_write_content
//!
//! \param[in] file_address  array of file address in FAT table:\n
//!                         this is the absolute address of the file in the EEPROM.
//! \param[in] file_length  array of file length in FAT table:\n
//!                         this is the upper limit of the file size in the EEPROM.
//!
//! \return on succes 0, error otherwise
//!
//! \brief  parse the FAT table from eeprom 
//
//*****************************************************************************
unsigned char fat_write_content(
                                unsigned short const *file_address,
                                unsigned short const *file_length)
{
    unsigned short  index = 0;
    unsigned char   ucStatus;
    unsigned char   fatTable[48];
    unsigned char*  fatTablePtr = fatTable;
    
    // first, write the magic number
    ucStatus = nvmem_write(16, 2, 0, (unsigned char *)"LS");
    
    for (; index <= NVMEM_RM_FILEID; index++)
    {
        // write address low char and mark as allocated
        *fatTablePtr++ = (unsigned char)(file_address[index] & 0xff) | BIT0;
        
        // write address high char
        *fatTablePtr++ = (unsigned char)((file_address[index]>>8) & 0xff);
        
        // write length low char
        *fatTablePtr++ = (unsigned char)(file_length[index] & 0xff);
        
        // write length high char
        *fatTablePtr++ = (unsigned char)((file_length[index]>>8) & 0xff);        
    }
    
    // second, write the FAT
    // write in two parts to work with tiny driver
    ucStatus = nvmem_write(16, 24, 4, fatTable); 
    ucStatus = nvmem_write(16, 24, 24+4, &fatTable[24]); 
    
    // third, we want to erase any user files
    memset(fatTable, 0, sizeof(fatTable));
    ucStatus = nvmem_write(16, 16, 52, fatTable); 
    
    return ucStatus;
}



void WriteFAT(void) {
    print("Writing FAT..."); //file table to CC3000
    wifi_start_driverless();

    uint8_t rval = fat_write_content(aFATEntries[0], aFATEntries[1]);
    if (rval==0) {
        println("OK");
    } else {
        fprintf(&USBSerialStream, "ERR %u", rval);
        do_usb();
        for(;;);
    }
    
    wifi_stop();
}


void CopyInfoToNVRAM(void) {
    wifi_start_driverless();
        
    for (int i=0; i<MAC_SIZE; i++) {
        macBuffer[i] = eeprom_read_byte(MAC_ADDRESS_EEPROM_LOCATION+i);
    }
    print("Writing MAC...");
    uint8_t rval = nvmem_set_mac_address(macBuffer);
    if (rval==0) {
        println("OK");
    } else {
        println("ERR");
        do_usb();
        for (;;);
    }
    
    for (int i=0; i<RM_SIZE; i++) {
        rmBuffer[i] = eeprom_read_byte(RM_ADDRESS_EEPROM_LOCATION+i);
    }
    print("Writing RM...");
    // TI writes the RM record back as 4 chunks of 32 bytes, so that's what we'll do
    for (int i=0; i<4; i++) {
        rval = nvmem_write(NVMEM_RM_FILEID, 32, 32*i, (unsigned char *)&rmBuffer[i*32]);
        if (rval!=0) {
            fprintf(&USBSerialStream, "ERR chunk %i", i);
            do_usb();
            for(;;);
        }
    }
    println("OK");

    wifi_stop();
}



void WritePatch(bool writeDriver) {
    if (writeDriver) {
        println("Writing Driver...");
    } else {
        println("Write Firmware...");
    }
        
    wifi_start_driverless();
    
    uint8_t rval;
    
    if (writeDriver) {
        rval = nvmem_write_patch(NVMEM_WLAN_DRIVER_SP_FILEID, drv_length, wlan_drv_patch);
    } else {
        rval = nvmem_write_patch(NVMEM_WLAN_FW_SP_FILEID, fw_length, fw_patch);
    }
        
    if (rval==0) {
        println("OK");
    } else {
        println("ERR");
        do_usb();
        for(;;);
    }
    
    wifi_stop();
}


/*
void DisplayMenu(void) {
    println("\n\nCommand options:");
    
    println("    0  - Test the CC3000");
    println("    1  - Show MAC address and radio file on CC3000");
    println("    2  - Show MAC address and radio file in Arduino EEPROM");
    println("    3  - Show current CC3000 file table");
    println("    4  - Copy MAC address and radio file from CC3000 to Arduino EEPROM");
    println("    5  - Copy default MAC and radio file to Arduino EEPROM");
    println("    6  - Write a new file table to the CC3000");
    println("    7  - Copy MAC address and radio file from Arduino EEPROM to CC3000");
    println("    8  - Write new driver to CC3000");
    println("    9  - Write new firmware to CC3000");
    
}
*/

void GetCC3000Info(bool copyToEEPROM) {
    wifi_start_driverless();
    do_usb();
    
    if (wifi_print_MAC() && copyToEEPROM) {
        print("Saving to MCU EEPROM...");
        for (int i=0; i<MAC_SIZE; i++) {
            eeprom_write_byte(MAC_ADDRESS_EEPROM_LOCATION+i, macBuffer[i]);
        }
        for (int i=0; i<MAC_SIZE; i++) {
            if (eeprom_read_byte(MAC_ADDRESS_EEPROM_LOCATION+i)  != macBuffer[i]) {
                println("ERR");
                do_usb();
                for (;;);
            }
        }
        println("OK");
    }
    do_usb();
    
    print("Reading Radio file...");
    // The TI PatchProgrammer reads this 128 uint8_t record as 16 chunks of 8 bytes, so that's what we'll do
    for (int i=0; i<16; i++) {
        uint8_t rval = nvmem_read(NVMEM_RM_FILEID, 8, 8*i, (unsigned char *)&rmBuffer[i*8]);
        if (rval!=0) {
            fprintf(&USBSerialStream, "ERR chunk %u", i);
            do_usb();
            for (;;);
        }
    }
    println("OK");
    do_usb();
    
    wifi_print_RM_Record();
    
    if (copyToEEPROM) {
        print("Saving to EEPROM...");
        for (int i=0; i<RM_SIZE; i++) {
            eeprom_write_byte(RM_ADDRESS_EEPROM_LOCATION+i, rmBuffer[i]);
        }
        for (int i=0; i<RM_SIZE; i++) {
            if (eeprom_read_byte(RM_ADDRESS_EEPROM_LOCATION+i)  != rmBuffer[i]) {
                println("ERR");
                do_usb();
                for (;;);
            }
        }
        println("OK");
    }

    wifi_stop();
}

uint8_t warning_issued;

void process_bytes(){
    uint16_t nb_received = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);
    uint8_t c;
    
    for( uint16_t i = 0; i < nb_received ; i++ ){
        c = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
        
        switch (c){
            case 't':
                // test CC3000
                BasicCC30000Test();
                ShowEEPROM();
                //fprintf(&USBSerialStream , "eep mk: %u\r\n", (uint8_t)wifi_eeprom_magic_key_addr);
                //fprintf(&USBSerialStream , "eep enc: %u\r\n", (uint8_t)wifi_eeprom_encrypt_addr);
                //fprintf(&USBSerialStream , "eep ssid: %u\r\n", (uint8_t)wifi_eeprom_ssid_addr);
                //fprintf(&USBSerialStream , "eep psw: %u\r\n", (uint8_t)wifi_eeprom_password_addr);
                //fprintf(&USBSerialStream , "eep mac: %u\r\n", (uint8_t)MAC_ADDRESS_EEPROM_LOCATION);
                //fprintf(&USBSerialStream , "eep rm: %u\r\n", (uint8_t)RM_ADDRESS_EEPROM_LOCATION);
                break;
            
            case 'p':
                // apply patch
                GetCC3000Info(true);
                do_usb();
                WriteFAT();
                do_usb();
                CopyInfoToNVRAM();
                do_usb();
                WritePatch(true);
                do_usb();
                WritePatch(false);
                break;
            case 'r':
                println("recovery from eeprom may KILL your CC3000 if eeprom is not initialized!");
                println("press 'y' to do it.");
                do_usb();
                warning_issued = 1;
                break;
            case 'y':
                if (warning_issued){     
                    WriteFAT();
                    do_usb();
                    CopyInfoToNVRAM();
                    do_usb();
                    WritePatch(true);
                    do_usb();
                    WritePatch(false);
                }
                break;
                
            default:
                println("t : test CC3000");
                println("p : upgrade to v1.24");
                println("r : upgrade from eeprom");
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


