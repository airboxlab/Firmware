
/*
 * fat.h
 *
 * Created: 27/12/2013 19:38:56
 *  Author: Xevel
 */ 


#ifndef FAT_H_
#define FAT_H_

#define NUM_FAT_RECORDS    (NVMEM_RM_FILEID + 1)

#define BIT0    0x01
#define BIT1    0x02
#define BIT2    0x04
#define BIT3    0x08
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7    0x80


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





/*
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
    
    print("File Table\r\n");    

    wifi_start_driverless();    
    
    uint8_t status = fat_read_content(isAllocated, isValid, writeProtected, fileAddress, fileLength);
    if (status==0) {
        print("Active, Valid, Write_protected, Start_addr, Length\r\n");
        for (int i=0; i<NUM_FAT_RECORDS; i++) {
            fprintf(&USBSerialStream, " %u ", i);
            
            fat_print_helper(isAllocated[i]);
            fat_print_helper(isValid[i]);
            fat_print_helper(writeProtected[i]);
            fprintf(&USBSerialStream, "%X  %X \r\n", fileAddress[i], fileLength[i]);
            }

        }
    else {
        printlnERR();
    }
        
    wifi_stop();
}

*/

#endif /* FAT_H_ */