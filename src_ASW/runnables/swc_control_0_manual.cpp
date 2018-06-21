/*
 * swc_control_0_manual.cpp
 *
 *  Created on: 19.08.2016
 *      Author: Fromm
 */

#include "global.h"

#include "joystick_type.h"
#include "joystick_signal.h"

#include "targetspeed_type.h"
#include "targetspeed_signal.h"

#include "signal_bb.h"

#include "swc_control_0_manual.h"

/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

void CONTROL0_init_manual()
{
}

void CONTROL0_deinit_manual()
{
}

/*****************************************************************************************
 * Non-Safety Function Implementations
 *****************************************************************************************/

#define JOYSTICK_NULL 0x7F

void CONTROL0_run_manual()
{

	sint8_t loc_x = JOYSTICK_NULL;
	sint8_t loc_y = JOYSTICK_NULL;
	sint8_t loc_Rz = JOYSTICK_NULL;

	sig_sts_t joystick_sig_stat;
	JOYSTICK_data_t joystick_data;
	TARGETSPEED_data_t target_speed;

	//Read Joystick signal
	joystick_sig_stat = JOYSTICK_get_with_status(&so_joystick, &joystick_data);

	if (SIG_STS_VALID == joystick_sig_stat)
	{
		loc_x = joystick_data.x_in;
		loc_y = joystick_data.y_in;
		loc_Rz = joystick_data.Rz_in;

	}

	//The status will always be valid, because speed 0 is considered to be safe
	TARGETSPEED_set_status(&so_targetspeed, SIG_STS_VALID);

	target_speed.vx = loc_x;
	target_speed.vy = loc_y;
	target_speed.vphi = loc_Rz;

	//Set target speed
	TARGETSPEED_set(&so_targetspeed, target_speed);
}


