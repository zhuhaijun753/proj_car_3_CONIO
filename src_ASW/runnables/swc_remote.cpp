/**
 * \file swc_remote.h
 * \author Fromm, Team Remote
 * \date 04.06.2016
 *
 * \brief swc_remote
 *
 * \copyright Copyright ©2016
 * Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
 * (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
 * this paragraph and the following paragraph appear in all copies, modifications, and distributions.
 * Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
 *
 * \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
 * The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
 * This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
 */

#include "pxdef.h"
#include "system.h"
#include "global.h"

#include "swc_remote.h"

#include "cardataprotocol_type.h"
#include "cardataprotocol_signal.h"
#include "signal_global.h"
#include "signal_bb.h"
#include "joystick_type.h"
#include "joystick_signal.h"
#include "event_type.h"
#include "event_signal.h"

#include "statemachine_config.h"
#include "swc_control.h"

#include "SystemEvents.h"
#include "Task_C0_State.h"
#include "Task_C0_Communication.h"

/*****************************************************************************************
 * Local static functions
 *****************************************************************************************/

/**
 * This function will perform a system reset in case a reset protocol is received
 */
static void REMOTE__reactOnReset();

/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

void REMOTE_init()
{
	CARDATAPROTOCOL_data_t remote_sendData = REMOTE_INIT_DATA;

	remote_sendData.msgID = REMOTE_MID_CONNECT;
	remote_sendData.featureID = REMOTE_FID_RESET;
	remote_sendData.length = 2; //Feature Id + CRC

	//REMOTEMSG_write(&so_out_remoteMsgFromState,remote_sendData);
	CARDATAPROTOCOL_write(&so_out_remote, remote_sendData);

}

void REMOTE_deinit()
{

}

/*****************************************************************************************
 * Safety Function Implementations
 *****************************************************************************************/


//Timeout values
#define SAFETY_JOYSTICK_TIMEOUT_WARNING	1000		//1000ms = 1s
#define SAFETY_JOYSTICK_TIMEOUT_ERROR	3000		//3000ms = 3s
/**
 * @brief Safety function to verify the values of the joystick
 * This function checks the age of the joystick signal
 * @return requested safety state
 */
RT_safety_t REMOTE_run_checkJoystickAge()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	//Increment the age with every cyclic call
	//JOYSTICK_inc_age(&so_joystick); //This is done centrally through timertick function call

	if (carState.state_normal == IS_RUNNING)
	{
		if (JOYSTICK_get_age(&so_joystick) >= SAFETY_JOYSTICK_TIMEOUT_ERROR)
		{
			//Stop the car
			return SAFEERROR2;
		}
		else if (JOYSTICK_get_age(&so_joystick) >= SAFETY_JOYSTICK_TIMEOUT_WARNING)
		{
			//Slow the car down
			return SAFEERROR1;
		}

	}

	//Set state invalid in case of not running or prerun state to avoid wrong
	//usage of old zero-data e.g. for check joystick zero position
	if (carState.state_normal != IS_RUNNING && carState.state_normal != IS_PRERUN)
	{
		JOYSTICK_set_status_invalid(&so_joystick);
	}

	return SAFEOK;

}

/**
 * @brief Safety function to verify the values of the joystick
 * This function checks values of the joystick signal
 * @return requested safety state
 */
RT_safety_t REMOTE_run_checkJoystickData()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	sig_sts_t joystick_sig_stat;
	JOYSTICK_data_t joystick_data;

	//Read Joystick signal
	joystick_sig_stat = JOYSTICK_get_with_status(&so_joystick, &joystick_data);

	if (carState.state_normal == IS_RUNNING)
	{

		/*
		//Status check will not be performed by safety logic, but only by application
		if (SIG_STS_VALID != joystick_sig_stat)
		{
			//Stop the car
			return SAFEERROR2;
		}
		*/
		//Range check
		if (((-100 > joystick_data.x_in) || (joystick_data.x_in > 100) || (-100 > joystick_data.y_in) || (joystick_data.y_in > 100)
				|| (-100 > joystick_data.Rz_in) || (joystick_data.Rz_in > 100)))
		{
			{
				//Slow down the car
				JOYSTICK_set_status_invalid(&so_joystick);
				return SAFEERROR1;
			}

		}
	}

	return SAFEOK;

}

/**
 * @brief Safety function to verify joystick is in zero position before the car starts to drive
 * Before leaving the safe 2 (stopped) state, the joystick has to be moved into zero position
 * @return requested safety state
 */
RT_safety_t REMOTE_run_checkJoystickZero()
{
	JOYSTICK_data_t joystick = JOYSTICK_get(&so_joystick);

	//Check if joystick is in 0 position
	//and signal is valid (i.e. it has been updated at least once
	//and if it is not too old (to avoid a positive check if stop was pressed and last value before stop was 0)
	if (joystick.x_in == 0 && joystick.y_in == 0 && joystick.Rz_in == 0
			&& JOYSTICK_get_status(&so_joystick) == SIG_STS_VALID /*&& JOYSTICK_get_age(&so_joystick) < 3*/)
	{
		asm("nop");
		return SAFEOK;
	}
	else
	{
		//else error case, stop the car
		asm("nop");
		return SAFEERROR2;
	}

}



RT_safety_t REMOTE_run_readAndCheckProtocol()
{
	RC_t remote_readResult = RC_ERROR;
	CARDATAPROTOCOL_data_t remote_recievedData;

	sig_sts_t remote_sig_status;

	RC_t joystick_writeResult;
	JOYSTICK_data_t joystick_value;

	//Get the state machine task
	EVENT_data_t event = EVENT_INIT_DATA;
	event.receiverTask = Task_C0_State_getTaskId();

	//Read in the signal from the signallayer
	remote_sig_status = CARDATAPROTOCOL_get_with_status(&so_in_remote, &remote_recievedData);
	if (SIG_STS_VALID == remote_sig_status)
	{
		//We have valid data, now let's process it depending on the type of protocol
		switch (remote_recievedData.msgID)
		{
		case REMOTE_MID_CONNECT: // 'C'
			//Connection related protocol, will be processed by the central state machine
			switch (remote_recievedData.featureID)
			{
			case REMOTE_FID_CONNECT: // 'C' Connect
				event.eventMask = ev_Task_C0_State_3_Connect;
				EVENT_write(&so_out_remoteEvent, event);
				//PxTaskSignalEvents(receiverTask, ev_Task_C0_State_1_Connect);
				break;
			case REMOTE_FID_DISCONNECT: // 'D' Disconnect
				//Set remoteId to disconnect
				SINT32_IR_set(&so_remoteId, REMOTE_DISCONNECT_ID);

				event.eventMask = ev_Task_C0_State_8_Disconnect;
				EVENT_write(&so_out_remoteEvent, event);

				//PxTaskSignalEvents(receiverTask, ev_Task_C0_State_5_Disconnect);
				break;
			case REMOTE_FID_PRESENT: // 'P' Present (Check for available cars)

				event.eventMask = ev_Task_C0_State_2_Present;
				EVENT_write(&so_out_remoteEvent, event);
				//PxTaskSignalEvents(receiverTask, ev_Task_C0_State_0_Present);
				break;
			case REMOTE_FID_STOP: // 'S' Stop Feature

				event.eventMask = ev_Task_C0_State_7_Stop;
				EVENT_write(&so_out_remoteEvent, event);
				//PxTaskSignalEvents(receiverTask, ev_Task_C0_State_4_Stop);
				break;
			case REMOTE_FID_RESET: // 'R' Reset Feature
				REMOTE__reactOnReset();
				break;

			default:
				break;
			}
			break; //Processing connect commands

		case REMOTE_MID_FEATURE: // 'F'
			//Feature request command, will also be handled by the central state machine
			switch (remote_recievedData.featureID)
			{
			case REMOTE_FID_REQUEST_FEATURES: // 0xff Feature list request

				event.eventMask = ev_Task_C0_State_4_FeatureRequest;
				EVENT_write(&so_out_remoteEvent, event);
				//PxTaskSignalEvents(receiverTask, ev_Task_C0_State_2_FeatureRequest);
				break;
			default: //Feature activation
				if (remote_recievedData.featureID < STATE_driveTable_Size)
				{
					//Requested feature is valid
					SINT32_IR_set(&so_driveMode, remote_recievedData.featureID);

					event.eventMask = ev_Task_C0_State_5_FeatureActivate;
					EVENT_write(&so_out_remoteEvent, event);
					//PxTaskSignalEvents(receiverTask, ev_Task_C0_State_3_FeatureActivate);
				}
				else
				{
					//Todo: some error handling
				}
				break;

			}
			break;

		case REMOTE_MID_JOYSTICK: // 'J'
			//Joystick data, will be processed in this runnable
			joystick_value.x_in = remote_recievedData.featureData[0];
			joystick_value.y_in = remote_recievedData.featureData[1];
			joystick_value.Rz_in = remote_recievedData.featureData[2];

			joystick_value.emergency_stop = (boolean_t) ((((remote_recievedData.featureData[3]) & 0x02) == 0x02) ? true : false);
			joystick_value.gradual_stop = (boolean_t) ((((remote_recievedData.featureData[4]) & 0x02) == 0x02) ? true : false);

			joystick_writeResult = JOYSTICK_set(&so_joystick, joystick_value);

			if (joystick_value.emergency_stop)
			{
				//Initiate Emergency Stop through normal state machine
				event.eventMask = ev_Task_C0_State_9_EmergencyBrake;
				EVENT_write(&so_out_remoteEvent, event);
				//PxTaskSignalEvents(receiverTask, ev_Task_C0_State_8_EmergencyBrake);

				//Secure emergency stop through transition to stop state
				event.eventMask = ev_Task_C0_State_12_ErrorRequestError2;
				EVENT_write(&so_out_remoteEvent, event);
				//PxTaskSignalEvents(receiverTask, ev_Task_C0_State_11_ErrorRequestError2);
			}

			break;
		default: ;
			//Some protocol are invalid, slowing the car down. At the moment, error handling is only done through the age
			//return SAFEERROR1;
		}

	}
	else
	{

		//Some protocol are invalid, slowing the car down. At the moment, error handling is only done through the age
		//return SAFEERROR1;
	}
	return SAFEOK;
}



static void REMOTE__reactOnReset()
{
	SYSTEM_ResetFromPxros();
}


