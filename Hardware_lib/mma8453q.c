/*
 * mma8435q.c
 *
 * Created: 16/10/2013 16:31:21
 *  Author: Xevel
 */ 


#include "mma8453q.h"
#include <LUFA/Drivers/Peripheral/TWI.h>
#include "low_level_stuff.h"

void mma8435q_init(){
    lls_twi_init();
	uint8_t double_tap_y=0x48;
	uint8_t threshold=0x0C; //0.5g, step of 0.063g, 8 counts;
	uint8_t time_limit_detection=0x0C; // ODR 800Hz, 20 ms time limit 20/1.25=16 counts 
	uint8_t time_limit_double_tap=0x100; //100 ms max, 80 count;
	uint8_t pulse_latency=0x20;
	mma8435q_standbyMode(); //register settings must be made in standby mode
    mma8435q_regWrite(REG_XYZ_DATA_CFG, FULL_SCALE_RANGE_2g);
	//Enable tap detection
	mma8435q_regWrite(REG_PULSE_CFG,double_tap_y);
	//Set the acceleration lvl in each direction to trigger the event
	//2g in y direction
	mma8435q_regWrite(REG_PULSE_THSY,threshold);
	//100 ms max of acceleration (ODR 800Hz)
	mma8435q_regWrite(REG_PULSE_TMLT,time_limit_detection);
	//300 ms max for double tap
	mma8435q_regWrite(REG_PULSE_WIND,time_limit_double_tap);
	mma8435q_regWrite(REG_PULSE_LTCY,pulse_latency);
    mma8435q_hiResMode(); //this is the default setting and can be omitted.
    mma8435q_activeMode();
    mma8435q_check_comm();

	
}


// tests if we are successfully talking to the accelerometer
uint8_t mma8435q_check_comm(){
    uint8_t b = 0;
    lls_twi_read_regs(MMA8435Q_I2C_ADDR, REG_WHO_AM_I, &b,1);
    return b == 0x3A;
}


uint8_t mma8435q_regWrite(uint8_t reg, uint8_t val){
    uint8_t err = TWI_StartTransmission(MMA8435Q_I2C_ADDR | TWI_ADDRESS_WRITE, 1);
    if (err == TWI_ERROR_NoError){
        TWI_SendByte(reg);
        TWI_SendByte(val);
        TWI_StopTransmission();
    } else return err;
    return TWI_ERROR_NoError;
}

/*
  Put MMA8453Q into standby mode
*/
void mma8435q_standbyMode(){
    uint8_t reg;
    uint8_t activeMask = 0x01;
    
    lls_twi_read_regs(MMA8435Q_I2C_ADDR, REG_CTRL_REG1, &reg, 1);
    mma8435q_regWrite(REG_CTRL_REG1, reg & ~activeMask);
}

/*
  Put MMA8453Q into active mode
*/
void mma8435q_activeMode(){
    uint8_t reg;
	//ODR 800Hz
    uint8_t activeMask = 0x01;
    
    lls_twi_read_regs(MMA8435Q_I2C_ADDR, REG_CTRL_REG1, &reg, 1);
    mma8435q_regWrite(REG_CTRL_REG1, reg | activeMask);
}

/*
  Use fast mode (low resolution mode)
  The acceleration readings will be
  limited to 8 bits in this mode.
*/
void mma8435q_lowResMode(){
    uint8_t reg;
    uint8_t fastModeMask = 0x02;
    
    lls_twi_read_regs(MMA8435Q_I2C_ADDR, REG_CTRL_REG1, &reg, 1);
    mma8435q_regWrite(REG_CTRL_REG1, reg | fastModeMask);
}

/*
  Use default mode (high resolution mode)
  The acceleration readings will be
  10 bits in this mode.
*/
void mma8435q_hiResMode(){
    uint8_t reg;
    uint8_t fastModeMask = 0x02;
    
    lls_twi_read_regs(MMA8435Q_I2C_ADDR, REG_CTRL_REG1, &reg, 1);
    mma8435q_regWrite(REG_CTRL_REG1,  reg & ~fastModeMask);
}

/*
  Get accelerometer readings (x, y, z)
  by default, standard 10 bits mode is used.
 
  This function also converts 2's complement number to
  signed integer result.
 
  If accelerometer is initialized to use low res mode,
  isHighRes must be passed in as false.
*/
void mma8435q_getAccXYZ(int16_t *x, int16_t *y, int16_t *z, uint8_t isHighRes){
    uint8_t buf[6];
    
    if (isHighRes) {
        lls_twi_read_regs(MMA8435Q_I2C_ADDR, REG_OUT_X_MSB, buf, 6);
        *x = ((int16_t)buf[0] << 2) | ((buf[1] >> 6) & 0x3);
        *y = ((int16_t)buf[2] << 2) | ((buf[3] >> 6) & 0x3);
        *z = ((int16_t)buf[4] << 2) | ((buf[5] >> 6) & 0x3);
    }
    else {
        lls_twi_read_regs(MMA8435Q_I2C_ADDR, REG_OUT_X_MSB, buf, 3);
        *x = buf[0] << 2;
        *y = buf[1] << 2;
        *z = buf[2] << 2;
    }
    
    if (*x > 511) *x = *x - 1024;
    if (*y > 511) *y = *y - 1024 ;
    if (*z > 511) *z = *z - 1024;
}

int16_t mma8435q_getAccY(){
    uint8_t buf;
    
    if (lls_twi_read_regs(MMA8435Q_I2C_ADDR, REG_OUT_Y_MSB, &buf, 1) == TWI_ERROR_NoError){
        int16_t y = ((int16_t)buf << 2);
        
        if (y > 511){
            y = y - 1024 ;
        }
        return y;
    }
    return 0;
}


uint8_t mma8435q_check_doubletap(){
	uint8_t buf;
	lls_twi_read_regs(MMA8435Q_I2C_ADDR,REG_PULSE_SRC,&buf,1);
	return (buf);
}