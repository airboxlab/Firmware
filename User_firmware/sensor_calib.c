/*
 * sensor_calib.c
 *
 * Created: 02/05/2014 17:08:39
 *  Author: Tony
 */ 

#include "sensor_calib.h"
#include "eeprom_adress.h"

double k_pm;
double k_temp;
double k_hum;

double b_temp;
double b_hum;
double b_pm;

#ifdef CALIBRATION
void sensor_calib_set_k(uint8_t d_k, int32_t d_b,int significative_digit,int number_of_calib)
{
	switch(number_of_calib)
	{
		//PM
		case 1:
			k_pm=((double) d_k)/100;
			b_pm=((double) d_b)/10000;
			break; 
		//Temperature
		case 2:
			k_temp=((double) d_k)/100;
			b_temp=((double) d_b)/10000;
			break;
		//Humidity
		case 3:
			k_hum=((double) d_k)/100;
			b_hum=((double) d_b)/10000;
			break;
	}
}



void sensor_calib_save_k(int type_of_calib)
{
	switch (type_of_calib)
	{
		case 1:
			eeprom_write_byte(calib_eeprom_k_pm_addr,((uint8_t) round(k_pm*100)));
			eeprom_write_float(calib_eeprom_b_pm_addr,((float) b_pm));
			eeprom_write_dword(calib_eeprom_pm_magic_key,CALIB_MAGIC_KEY);
			break;
		case 2:
			eeprom_write_byte(calib_eeprom_k_temp_addr,((uint8_t) round(k_temp*100)));
			eeprom_write_float(calib_eeprom_b_temp_addr,((float) b_temp));
			eeprom_write_dword(calib_eeprom_temp_magic_key,CALIB_MAGIC_KEY);
			break;
		case 3:
			eeprom_write_byte(calib_eeprom_k_hum_addr,((uint8_t) round(k_hum*100)));
			eeprom_write_float(calib_eeprom_b_hum_addr,((float) b_hum));
			eeprom_write_dword(calib_eeprom_hum_magic_key,CALIB_MAGIC_KEY);
			break;
		default :
			break;
	}
}
#endif

void sensor_calib_load_k(int signicative_digit)
{

	if(sensor_calib_PM_is_done())
	{
		k_pm=((double)eeprom_read_byte(calib_eeprom_k_pm_addr))/((double)signicative_digit);
		b_pm=((double)eeprom_read_float(calib_eeprom_b_pm_addr));
	}
	else
	{
		k_pm=1.0;
		b_pm=0.0;
	}
	if (sensor_calib_TEMP_is_done())
	{
		k_temp=((double)eeprom_read_byte(calib_eeprom_k_temp_addr))/((double)signicative_digit);
		b_temp=((double)eeprom_read_float(calib_eeprom_b_temp_addr));
	}
	else
	{
		k_temp=1.0;
		b_temp=0.0;
	}
	if (sensor_calib_HUM_is_done())
	{
		k_hum=((double)eeprom_read_byte(calib_eeprom_k_hum_addr))/((double)signicative_digit);
		b_hum=((double)eeprom_read_float(calib_eeprom_b_hum_addr));
	}
	else
	{
		k_hum=1.0;
		b_hum=0.0;
	}
}

#ifdef CALIBRATION
void sensor_calib_erase_calibration_k(){
	eeprom_write_dword(calib_eeprom_temp_magic_key,0);
	eeprom_write_dword(calib_eeprom_pm_magic_key,0);
	eeprom_write_dword(calib_eeprom_hum_magic_key,0);
	sensor_calib_load_k(100);
}
#endif

uint8_t sensor_calib_PM_is_done(){
	return (eeprom_read_dword(calib_eeprom_pm_magic_key)==CALIB_MAGIC_KEY);
}

uint8_t sensor_calib_HUM_is_done(){
	return (eeprom_read_dword(calib_eeprom_hum_magic_key)==CALIB_MAGIC_KEY);
}

uint8_t sensor_calib_TEMP_is_done(){
	return (eeprom_read_dword(calib_eeprom_temp_magic_key)==CALIB_MAGIC_KEY);
}