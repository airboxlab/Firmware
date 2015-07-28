/*
 * states.c
 *
 * Created: 29/01/2014 21:04:52
 *  Author: Xevel
 */ 
#include "states.h"

#include "state_normal.h"
#include "state_config.h"
#include "state_error.h"
#include "state_power_saving.h"
#ifdef CALIBRATION
#include "state_calibration.h"
#endif

state_machine_t main_fsm;


state_t state_normal =			{1, state_normal_init, state_normal_play, state_normal_end};
state_t state_config =			{2, state_config_init, state_config_play, state_config_end};
state_t state_error =			{3, state_error_init, state_error_play, state_error_end};
state_t state_power_saving =	{4, state_power_saving_init, state_power_saving_play, state_power_saving_end};
#ifdef CALIBRATION
state_t state_calibration =		{5, state_calibration_init, state_calibration_play, state_calibration_end};
#endif CALIBRATION