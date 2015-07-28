/*
 * state_calibration.c
 *
 * Created: 28/04/2014 16:33:25
 *  Author: Tony
 */ 
#include "state_calibration.h"
#include "states.h"
#include "../sensor.h"
#include <Hardware_lib/time.h>


#define DELAY_BETWEEN_READING 600000 //5min
static uint32_t waiting_end; 
static uint32_t delay_between_readings;
uint32_t Sensor_Ref_0;
uint32_t Sensor_Ref_5;
uint32_t Sensor_Ref_10;

uint32_t Sensor_t[3];

char Value_T[22];

uint8_t fillpos;

coef_calib coef;
coef_calib coef1;
coef_calib coef2;

uint8_t type_of_calib;
uint8_t number_of_reading=1;


typedef enum {
	CALFSM_STATE_CHOICE_DELAY,
	CALFSM_STATE_READING_DELAY,
	CALFSM_STATE_INITIALIZING,
	CALFSM_STATE_WAITING,
	CALFSM_STATE_READING_SENSOR,
	CALFSM_STATE_READING_VALUE_T,
	CALFSM_STATE_CHOICE_CALIB,
	CALFSM_STATE_CALCULATE_K,
	CALFSM_STATE_CHOICE_SAVE,
	CALFSM_STATE_SAVE,
	CALFSM_STATE_END
} calibration_fsm_states_t;

uint32_t now;
static calibration_fsm_states_t calibration_fsm_state;

//ref(measured)=a*measured+b
coef_calib calculate_k(uint32_t ref1, uint32_t measured1,uint32_t ref2, uint32_t measured2)
{
	coef_calib res;
	double num=(double) ((int16_t) (ref1-ref2));
	res.a=((uint16_t) round(100*(num)/((int16_t)(measured1-measured2))));
	double resa=((double) (res.a))/100;
	res.b=(int32_t) (100.0*(( ref2-resa * measured2)));
	return res;
}

void state_calibration_init(){
	util_timer_enable_interrupt();
	util_timer_set_calib(1);
	fillpos=0;
	calibration_fsm_state = CALFSM_STATE_CHOICE_DELAY;
}

void state_calibration_play(){
	uint8_t c;
	char t[6];
	uint32_t now;
	value_sensor value_sensor_d;
	switch(calibration_fsm_state)
	{
			case CALFSM_STATE_CHOICE_DELAY:
				DCALIB_PRINT("\r\nPlease enter the delay between readings (in ms) : \r\n");	
				calibration_fsm_state=CALFSM_STATE_READING_DELAY;
				break;
			case CALFSM_STATE_READING_DELAY:
				c=usb_serial_get_byte();
				if (c>=48 && c<=57)
				{
					fprintf(&USBSerialStream,"%c",c);
					Value_T[fillpos++]=c;
				}
				else if (c=='\r')
				{
					Value_T[fillpos++]=c;
					delay_between_readings=char_to_int(Value_T,0,fillpos-1);
					fillpos=0;
					calibration_fsm_state=CALFSM_STATE_INITIALIZING;
				}
				break;
			case CALFSM_STATE_INITIALIZING:
				DCALIB_PRINT("\r\nPlease chose the calibration type (1 for PM, 2 for temperature, 3 for Humidity) \r\n");
				calibration_fsm_state=CALFSM_STATE_CHOICE_CALIB;
				break;
			case CALFSM_STATE_CHOICE_CALIB:
				c=usb_serial_get_byte();
				if (c>48 && c<=51)
				{
					type_of_calib=c-'0';
					calibration_fsm_state=CALFSM_STATE_READING_SENSOR;
				}				
				break;
			case CALFSM_STATE_READING_SENSOR:
				sensor_update();
				switch(type_of_calib)
				{
					case 1:
						Sensor_t[number_of_reading-1] = (int32_t) (sensors_acc.pm_instant*100);
						break;
					case 2:
						Sensor_t[number_of_reading-1]	=	(uint32_t) (sensors_acc.tmp_instant*100.0);
						break;
					case 3:
						Sensor_t[number_of_reading-1]	=	(uint32_t) (sensors_acc.hum_instant*100.0);
						break;
				}
				if (number_of_reading==1 || number_of_reading ==2)
				{
					number_of_reading++;
					now=millis();
					waiting_end=set_timeout(now+delay_between_readings);
					calibration_fsm_state=CALFSM_STATE_WAITING;
				}
				else 
				{
					DCALIB_PRINTF("T_0 : %lu, T_5 : %lu, T_10 : %lu \r\n", Sensor_t[0], Sensor_t[1], Sensor_t[2]);
					DCALIB_PRINT("Enter the reference values (ref_0;ref_5;ref_10) : \r\n");
					number_of_reading=1;
					calibration_fsm_state=CALFSM_STATE_READING_VALUE_T;
				}
				break;
			case CALFSM_STATE_WAITING:
				//DCALIB_PRINTF("\r\n %li", (int32_t) (millis()-waiting_end));
				if (millis()>waiting_end)
				{
						calibration_fsm_state=CALFSM_STATE_READING_SENSOR;
				}
				break;
			case CALFSM_STATE_READING_VALUE_T:
				c=usb_serial_get_byte();
				if (c>=48 && c<=57 || c==';')
				{
					fprintf(&USBSerialStream,"%c",c);
					Value_T[fillpos++]=c;
				}
				else if (c=='\r')
				{
					Value_T[fillpos++]=c;
					value_sensor_d=parse_ref_value(Value_T,fillpos);
					Sensor_Ref_0=char_to_int(Value_T,0,value_sensor_d.length_S_0);
					Sensor_Ref_5=char_to_int(Value_T,value_sensor_d.length_S_0+1,value_sensor_d.length_S_5);
					Sensor_Ref_10=char_to_int(Value_T,value_sensor_d.length_S_5+1,value_sensor_d.length_S_10);
					//DCALIB_PRINTF("T_0 : %lu, T_5 : %lu, T_10 %lu\r\n",Sensor_Ref_0, Sensor_Ref_5 ,Sensor_Ref_10);
					fillpos=0;
					calibration_fsm_state=CALFSM_STATE_CALCULATE_K;	
				}
				break;
			case CALFSM_STATE_CALCULATE_K:
				coef=calculate_k(Sensor_Ref_0, Sensor_t[0], Sensor_Ref_10, Sensor_t[2]);
				coef1=calculate_k(Sensor_Ref_0, Sensor_t[0], Sensor_Ref_5, Sensor_t[1]);
				coef2=calculate_k(Sensor_Ref_5, Sensor_t[1], Sensor_Ref_10, Sensor_t[2]);
				fprintf(&USBSerialStream,"\n\r 0-10	a	: %u ,	b : %li	, Calibrated_t_5	 : %lu ",coef.a, coef.b, (uint32_t) round(((double)(Sensor_t[1]*coef.a+coef.b)/100)));
				fprintf(&USBSerialStream,"\n\r 0-5	a	: %u ,	b : %li	, Calibrated_t_10	 : %lu  ",coef1.a, coef1.b, (uint32_t) round(((double)(Sensor_t[2]*coef1.a+coef1.b)/100)));
				fprintf(&USBSerialStream,"\n\r 5-10	a	: %u ,	b : %li	, Calibrated_t_0	 : %lu  \r\n",coef2.a, coef2.b, (uint32_t) round(((double)(Sensor_t[0]*coef2.a+coef2.b)/100)) );
				DCALIB_PRINT("Press 1, 2, 3 to save the corresponding coefficient. Press 0 to skip saving.\r\n");
				calibration_fsm_state=CALFSM_STATE_CHOICE_SAVE;
				break;	
			case CALFSM_STATE_CHOICE_SAVE:
				c=usb_serial_get_byte();
				switch (c)
				{
					case '0':
						calibration_fsm_state=CALFSM_STATE_END;
						break;
					case '1':
							sensor_calib_set_k(coef.a,coef.b,SIGNIFICATIVE_DIGITS_K,type_of_calib);
						calibration_fsm_state=CALFSM_STATE_SAVE;
						break;
					case '2':
						sensor_calib_set_k(coef1.a,coef1.b,SIGNIFICATIVE_DIGITS_K,type_of_calib);
						calibration_fsm_state=CALFSM_STATE_SAVE;
						break;
					case '3':
						sensor_calib_set_k(coef2.a,coef2.b,SIGNIFICATIVE_DIGITS_K,type_of_calib);
						calibration_fsm_state=CALFSM_STATE_SAVE;
						break;
					case '-':
						DCALIB_PRINT("Enter the reference values (ref_0;ref_5;ref_10) : \r\n");
						calibration_fsm_state=CALFSM_STATE_READING_VALUE_T;
						break;
					default :
						calibration_fsm_state=CALFSM_STATE_CHOICE_SAVE;
						break;
				}
				break;				
			case CALFSM_STATE_SAVE:	
				sensor_calib_save_k(type_of_calib);
				DCALIB_PRINT("Coefficients saved\r\n");
				calibration_fsm_state=CALFSM_STATE_END;
				break;
			case CALFSM_STATE_END:
				state_set_next(&main_fsm, &state_normal);
				break;
			default:
				state_set_next(&main_fsm, &state_normal);
				break;		
		}
}
	

void state_calibration_end(){
		util_timer_disable_interrupt();
		util_timer_set_calib(0);
}

value_sensor parse_ref_value(char* t, int len)
{
	int i;
	i=0;
	int occurence_sep=0;
	value_sensor val;
	while (occurence_sep != 3)
	{
		if(occurence_sep == 0 && *t==';')
		{
			val.length_S_0=i;
			occurence_sep =1;
		}
		else if(occurence_sep == 1 && *t==';')
		{
			val.length_S_5=i;
			occurence_sep = 2;
		}
		else if(*t=='\r')
		{
			val.length_S_10=i;
			occurence_sep = 3;
		}
		t++;		
		i++;
	}
	return val;
}

uint32_t ten_pow(uint8_t pow)
{
	if (pow==0)
	{
		return 1;
	}
	else if (pow==1)
	{
		return 10;
	}
	else if (pow==2)
	{
		return 100;
	}
	else if (pow==3)
	{
		return 1000;
	}
	else if (pow==4)
	{
		return 10000;
	}
	else if (pow==5)
	{
		return 100000;
	}
	else if (pow==6)
	{
		return 1000000;
	}
	else if (pow==7)
	{
		return 10000000;
	}
}

uint32_t char_to_int(char* t,int start,int len)
{
	uint32_t res=0;
	for (int i=start;i<len;i++)
	{
		res+=ten_pow(len-1-i)*(t[i]-'0');
	}
	return res;
}
