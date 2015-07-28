/*
 * state_power_saving.c
 *
 * Created: 24/04/2014 10:17:24
 *  Author: Tony
 */ 
#include "state_power_saving.h"
#include <Hardware_lib/sensor_board.h>
#include <Hardware_lib/led.h>
#include <Hardware_lib/motor.h>
#include "states.h"

//Disable some component
void state_power_saving_init()
{
		
		sensor_board_heater_on(false);
		motor_set(0);
		led_set(0,0,0);	
		//Disable TWI, SPI,Timer2 Timer1 ADC
		PRR0=(1 <<PRTWI) | (1 << PRTIM2) | (1 << PRTIM0) | (1 << PRTIM1) | (1 << PRSPI) | (1 << PRADC) ;
		//Disable Timer3, USART1
		PRR1=(1 << PRTIM3) | (1 << PRUSART1);
		//Disable Analog Comparator Disable
		ACSR|= (1<<ACD);
		//Enter in sleep mode
		SMCR=(1 << SM1) | (1 << SE);
		Delay_MS(200);
		sleep();
}

//When wake up, go to normal mode
void state_power_saving_play(){
	state_set_next(&main_fsm, &state_normal);
}

//We enable everything we disabled
void state_power_saving_end(){
		//Re enable everything
		PRR0=(0 <<PRTWI) | (0 << PRTIM2) | (0 << PRTIM0) | (0 << PRTIM1) | (0 << PRSPI) | (0 << PRADC) ;
		PRR1=(0 << PRTIM3) | (0 << PRUSART1);
		ACSR|= (0<<ACD);
		SMCR |= (0 << SE);
		//We switch on the mq135
		sensor_board_heater_on(true);
}