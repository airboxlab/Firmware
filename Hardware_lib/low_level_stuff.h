/*
 * low_level_stuff.h
 *
 * Created: 05/02/2014 00:06:17
 *  Author: Xevel
 */ 


#ifndef LOW_LEVEL_STUFF_H_
#define LOW_LEVEL_STUFF_H_
    #include "common.h"

    void lls_timer4_init();
    void lls_twi_init();
    

    uint8_t lls_twi_write_1(uint8_t i2c_addr, uint8_t data);
    uint8_t lls_twi_write_2(uint8_t i2c_addr, uint8_t data1, uint8_t data2);
    uint8_t lls_twi_write_n(uint8_t i2c_addr, uint8_t* data, uint8_t len);
    uint8_t lls_twi_read_regs(uint8_t i2c_addr, uint8_t addr, uint8_t* data, uint8_t len);
    int16_t lls_twi_get_int16(uint8_t i2c_addr);
    uint8_t lls_twi_read_1(uint8_t i2c_addr, uint8_t* data);
    uint8_t lls_twi_read_2(uint8_t i2c_addr, uint8_t* data1, uint8_t* data2);
    uint8_t lls_twi_read_n(uint8_t i2c_addr, uint8_t* data, uint8_t len);

#endif /* LOW_LEVEL_STUFF_H_ */