/*
 * swc_remote.cpp
 *
 *  Created on: 29.04.2016
 *      Author: Fromm
 */


#include "swc_control.h"

#include "global.h"
#include <string.h>	//strlen
#include "pxdef.h"

#include "SystemEvents.h"
#include "Task_C0_Safety_Logic.h"

#include "remotemsg_signal.h"
#include "remotemsg_type.h"

#include "signal_bb.h"

/*****************************************************************************************
 * Non-Safety Function Implementations
 *****************************************************************************************/


void CONTROL_run_selectedFeature()
{
	//Read carstate from signal layer
	CARSTATE_data_t carState=CARSTATE_get(&so_carstate);


	//Get the safetyLogic task
	PxTask_t receiverTask = Task_C0_Safety_Logic_getTaskId();

	if (carState.run_drive != 0)
	{
		carState.run_drive();

		//Notify SafetyLogicTask that new control data is available
		PxTaskSignalEvents(receiverTask, ev_Task_C0_Safety_Logic_1_newTargetSpeed);
	}
}


void CONTROL_showMessage(char* message)
{
	CARDATAPROTOCOL_data_t remote = REMOTE_INIT_DATA;

	uint8_t slen = strlen(message);

	//Limit length  to maxsize of protocol
	slen = _minf(slen, REMOTE_MAX_DATASIZE);

	remote.msgID = REMOTE_MID_CONNECT;
	remote.featureID = REMOTE_FID_INFO;
	remote.length = slen + REMOTE_ACK_LENGTH;

	for (int i = 0; i < slen; i++)
	{
		remote.featureData[i] = message[i];
	}

	REMOTEMSG_write(&so_out_remoteMsgFromControl, remote);
}



