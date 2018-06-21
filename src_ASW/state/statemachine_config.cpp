/**
 * \file statemachine_config.h
 * \author Fromm
 * \date 16.06.2016
 *
 * \brief statemachine_config
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

#include "global.h"
#include "statemachine_type.h"
#include "statemachine_config.h"

#include "remotemsg_type.h"
#include "remotemsg_signal.h"

#include "carstate_type.h"
#include "carstate_signal.h"

#include "sint32_ir_signal.h"

#include "signal_bb.h"

//Add any new control algorithm file here and add the function to the drive table
#include "swc_engine.h"
#include "swc_control.h"
#include "swc_control_0_manual.h"

#include "runnable_table_type.h"
#include "runnable_table.h"
#include "runnable_table_config.h"

#include "det.h"

#include "pxdef.h"
#include "SystemEvents.h"
#include "Task_C0_Safety_Logic.h"

#include <string.h>	//strlen

/***************************************************************************************************************************
 * Definitions for normal state machine
 ***************************************************************************************************************************/

/**
 * Prototypes for the action expressions
 */

/**
 * Acknowledge presence to GUI
 */
static void STATE__normal_sendPresentAck();

/**
 * Acknowledge connection to GUI
 */
static void STATE__normal_sendConnectAck();

/**
 * Acknowledge disconnect to GUI
 */
static void STATE__normal_sendDisconnectAck();

/**
 * Send availöable featires to GUI
 */
static void STATE__normal_sendFeatureList();

/**
 * Activate requested feature
 */
static void STATE__normal_activateFeature();

/**
 * Deactivate selected feature
 */
static void STATE__normal_deactivateFeature();

/**
 * Stop the car
 */
static void STATE__normal_StopCar();

/**
 * Request an emergency brake of the car
 */
static void STATE_normal_emergencyBrake();

/**
 * State String Identifiers
 */
const char_t* const STATE_normal_strTable[] =
{
		NORMAL_STATE_DEF(STATE_DEF_ENUM_STR)
};

/**
 * Transition Table
 */

const STATE_stateTransitionTable_t STATE_normal_transitionTable =
{

//Event 		//fromState			         //Guard							//Action											//toState
//=========================================================================================================================================================

		{/* evNMTBooted, IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0 /* Action will be called in Communication Task */ , (uint32_t) IS_DEVICES_BOOTING },
		{/* evDeviceBoot,IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },
		{/* evPresent,   IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },
		{/* evConnect,   IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },
		{/* evFeatReq,   IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },
		{/* evFeatAct,   IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },
		{/* evPreRunChk, IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },
		{/* evStop, 	 IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },
		{/* evDisconn,   IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },
		{/* evEmBrake,   IS_NMT_BOOTING    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_NMT_BOOTING },


		{/* evNMTBooted, IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },
		{/* evDeviceBoot,IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0 /* Action will be called in Communication Task */, (uint32_t) IS_DISCONNECTED },
		{/* evPresent,   IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },
		{/* evConnect,   IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },
		{/* evFeatReq,   IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },
		{/* evFeatAct,   IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },
		{/* evPreRunChk, IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },
		{/* evStop, 	 IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },
		{/* evDisconn,   IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },
		{/* evEmBrake,   IS_DEVICES_BOOTING */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DEVICES_BOOTING },


		{/* evNMTBooted, IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },
		{/* evDeviceBoot,IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },
		{/* evPresent,   IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_sendPresentAck,	(uint32_t) IS_PRESENT },
		{/* evConnect,   IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },
		{/* evFeatReq,   IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },
		{/* evFeatAct,   IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },
		{/* evPreRunChk, IS_DISCONEECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },
		{/* evStop, 	 IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },
		{/* evDisconn,   IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },
		{/* evEmBrake,   IS_DISCONNECTED */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_DISCONNECTED },

		{/* evNMTBooted, IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRESENT },
		{/* evDeviceBoot,IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRESENT },
		{/* evPresent,   IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRESENT },
		{/* evConnect,   IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_sendConnectAck,	(uint32_t) IS_CONNECTED },
		{/* evFeatReq,   IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRESENT },
		{/* evFeatAct,   IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRESENT },
		{/* evPreRunChk, IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRESENT },
		{/* evStop, 	 IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRESENT },
		{/* evDisconn,   IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_sendDisconnectAck, (uint32_t) IS_DISCONNECTED },
		{/* evEmBrake,   IS_PRESENT      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRESENT },

		{/* evNMTBooted, IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_CONNECTED },
		{/* evDeviceBoot,IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_CONNECTED },
		{/* evPresent,   IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_CONNECTED },
		{/* evConnect,   IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_CONNECTED },
		{/* evFeatReq,   IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_sendFeatureList,	(uint32_t) IS_CONNECTED },
		{/* evFeatAct,   IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0,	(uint32_t) IS_PRERUN },
		{/* evPreRunChk, IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_CONNECTED },
		{/* evStop, 	 IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_CONNECTED },
		{/* evDisconn,   IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_sendDisconnectAck,(uint32_t) IS_DISCONNECTED },
		{/* evEmBrake,   IS_CONNECTED    */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_CONNECTED },

		{/* evNMTBooted, IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRERUN },
		{/* evDeviceBoot,IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRERUN },
		{/* evPresent,   IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRERUN },
		{/* evConnect,   IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRERUN },
		{/* evFeatReq,   IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0,	(uint32_t) IS_PRERUN },
		{/* evFeatAct,   IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0,	(uint32_t) IS_PRERUN },
		{/* evPreRunChk, IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_activateFeature, (uint32_t) IS_RUNNING },
		{/* evStop, 	 IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_CONNECTED },
		{/* evDisconn,   IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_sendDisconnectAck,(uint32_t) IS_DISCONNECTED },
		{/* evEmBrake,   IS_PRERUN       */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_PRERUN },

		{/* evNMTBooted, IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_RUNNING },
		{/* evDeviceBoot,IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_RUNNING },
		{/* evPresent,   IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_RUNNING },
		{/* evConnect,   IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_RUNNING },
		{/* evFeatReq,   IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_RUNNING },
		{/* evFeatAct,   IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_RUNNING },
		{/* evPreRunChk, IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_RUNNING },
		{/* evStop, 	 IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_deactivateFeature,	(uint32_t) IS_CONNECTED },
		{/* evDisconn,   IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_sendDisconnectAck,	(uint32_t) IS_DISCONNECTED },
		{/* evEmBrake,   IS_RUNNING      */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE_normal_emergencyBrake, (uint32_t) IS_PRERUN },

};
const uint16_t STATE_normal_transitionTable_Size = sizeof(STATE_normal_transitionTable)
		/ sizeof(STATE_stateTransition_t);

extern const STATE_stateActionTable_t STATE_normal_actionTable =
{

//State						      //Entry										//Exit
//======================================================================================
		{/* IS_NMT_BOOTING */     (STATE_ActionPtr_t) 0, (STATE_ActionPtr_t) 0 },
		{/* IS_DEVICES_BOOTING */ (STATE_ActionPtr_t) 0, (STATE_ActionPtr_t) 0 },
		{/* IS_DISCONNECTED */    (STATE_ActionPtr_t) STATE__normal_StopCar, (STATE_ActionPtr_t) 0 },
		{/* IS_PRESENT      */    (STATE_ActionPtr_t) 0, (STATE_ActionPtr_t) 0 },
		{/* IS_CONNECTED    */    (STATE_ActionPtr_t) 0, (STATE_ActionPtr_t) 0 },
		{/* IS_PRERUN       */    (STATE_ActionPtr_t) 0, (STATE_ActionPtr_t) 0 },
		{/* IS_RUNNING      */    (STATE_ActionPtr_t) 0, (STATE_ActionPtr_t) STATE__normal_StopCar }

};
const uint16_t STATE_normal_actionTable_Size = sizeof(STATE_normal_actionTable) / sizeof(STATE_stateAction_t);

const STATE_driveActionTable_t STATE_driveTable =
{
// Init									Drive									DeInit										    Name[max 14 characters]
//===============================================================================================================================12345678901234|=======
		{ CONTROL0_init_manual,         CONTROL0_run_manual, 					CONTROL0_deinit_manual, 						"Manual Drive" },

};
const uint16_t STATE_driveTable_Size = sizeof(STATE_driveTable) / sizeof(STATE_driveAction_t);

/**
 * Implementation for the action expressions
 */



static void STATE__normal_sendPresentAck()
{
	REMOTEMSG_data_t remote_transmitData = REMOTE_INIT_DATA;

	remote_transmitData.msgID = REMOTE_MID_CONNECT;
	remote_transmitData.length = REMOTE_ACK_LENGTH;
	remote_transmitData.featureID = REMOTE_FID_ACK_PRESENT;

	REMOTEMSG_set(&so_out_remoteMsgFromState, remote_transmitData);
	REMOTEMSG_flush(&so_out_remoteMsgFromState);
}

static void STATE__normal_sendConnectAck()
{
	REMOTEMSG_data_t remote_transmitData = REMOTE_INIT_DATA;

	remote_transmitData.msgID = REMOTE_MID_CONNECT;
	remote_transmitData.length = REMOTE_ACK_LENGTH;
	remote_transmitData.featureID = REMOTE_FID_ACK_CONNECT;

	//Set remoteId to sender id. This is done in the remote R2D function, as the sender is not stored in the application data of the remote type

	REMOTEMSG_set(&so_out_remoteMsgFromState, remote_transmitData);
	REMOTEMSG_flush(&so_out_remoteMsgFromState);
}

static void STATE__normal_sendDisconnectAck()
{
	REMOTEMSG_data_t remote_transmitData = REMOTE_INIT_DATA;

	remote_transmitData.msgID = REMOTE_MID_CONNECT;
	remote_transmitData.length = REMOTE_ACK_LENGTH;
	remote_transmitData.featureID = REMOTE_FID_ACK_DISCONNECT;

	//Set remoteId to disconnect
	//SINT32_IR_set(&so_remoteId, REMOTE_DISCONNECT_ID);

	REMOTEMSG_set(&so_out_remoteMsgFromState, remote_transmitData);
	REMOTEMSG_flush(&so_out_remoteMsgFromState);
}

static void STATE__normal_sendFeatureList()
{
	RC_t result;

	REMOTEMSG_data_t remote_transmitData = REMOTE_INIT_DATA;
	remote_transmitData.msgID = 'F';

	for (int l_featureCount = 0; l_featureCount < STATE_driveTable_Size; l_featureCount++)
	{
		remote_transmitData.featureID = l_featureCount;

		for (int i = 0; i < REMOTE_MAX_DATASIZE; i++)
		{
			remote_transmitData.featureData[i] = STATE_driveTable[l_featureCount].featureName[i];
		}
		remote_transmitData.length = strlen(STATE_driveTable[l_featureCount].featureName) + REMOTE_ACK_LENGTH;

		result = REMOTEMSG_set(&so_out_remoteMsgFromState,  remote_transmitData);

		if (result == RC_SUCCESS)
		{
			REMOTEMSG_flush(&so_out_remoteMsgFromState);
		}
		else
		{
			//Todo: Hardcore error handling
			DET_stop(AUTOCORE, APPL_BUFFEROVERFLOW, 0);
		}


	}
}

static void STATE__normal_activateFeature()
{

	//Read selected mode from the signallayer
	sint32_t targetMode = SINT32_IR_get(&so_driveMode);

	//Read carstate from signal layer
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);


	//Set control loop for current drive state
	if (targetMode >= 0 && targetMode < STATE_driveTable_Size)
	{

		//Set current drive mode to new mode
		carState.run_drive = STATE_driveTable[targetMode].drive;

		//Check for null pointer
		if (carState.run_drive == (STATE_ActionPtr_t) 0)
		{
			//Empty driving mode - let's stop the car and change to manual driving
			STATE__normal_StopCar();
		}

		//Call Init of new drive state
		if (STATE_driveTable[targetMode].init != (STATE_ActionPtr_t) 0)
		{
			STATE_driveTable[targetMode].init();
		}

		//Write signal so signallayer
		CARSTATE_set(&so_carstate, carState);

	}
	else
	{
		//No valid data, change to manual driving mode
		STATE__normal_StopCar();
	}

}

static void STATE__normal_deactivateFeature()
{

	//Read carstate from signal layer
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	//Call DeInit of previous drive state
	if (STATE_driveTable[carState.state_normal].deInit != (STATE_ActionPtr_t) 0)
	{
		STATE_driveTable[carState.state_normal].deInit();
	}

	carState.run_drive = 0;

	//Write signal so signallayer
	CARSTATE_set(&so_carstate, carState);
}

static void STATE__normal_StopCar()
{
	//Fire the event for emergency brake to the safety logic task
	PxTask_t receiverTask = Task_C0_Safety_Logic_getTaskId();
	PxTaskSignalEvents(receiverTask, ev_Task_C0_Safety_Logic_3_requestStop);
}

static void STATE_normal_emergencyBrake()
{
	//Fire the event for emergency brake to the safety logic task
	PxTask_t receiverTask = Task_C0_Safety_Logic_getTaskId();
	PxTaskSignalEvents(receiverTask, ev_Task_C0_Safety_Logic_2_requestEmergencyStop);

}



/***************************************************************************************************************************
 * Definitions for safety state machine
 ***************************************************************************************************************************/

/***************************************************************************************************************************
 * Definitions for safety state machine
 ***************************************************************************************************************************/

/**
 * Prototypes for the action expressions
 */

/**
 * Allow car to run with max speed
 */
static void STATE__safety_allowMaxSpeed();

/**
 * Allow car to run with reduced speed
 */
static void STATE__safety_allowReducedSpeed();

/**
 * Stop car, after sufficient AllOk or Safe1 requests the car will continue driving
 */
static void STATE__safety_stopCar();

/**
 * Permanent stop car
 */
static void STATE__safety_terminate();

/**
 * Reset the hysteresis counter for the starting the car again
 */
static void STATE__safety_resetSafetyCount();


/**
 * Prototypes for the guard expressions
 */

static bool STATE__safety_countSafeOk_1();

static bool STATE__safety_countSafeOk_2();

static bool STATE__safety_countSafeOkOrSafe1_2();

/**
 * Safety State String Identifiers
 */
const char_t* const STATE_safety_strTable[] =
{
	SAFETY_STATE_DEF(STATE_DEF_ENUM_STR)
};

/**
 * Transition Table
 */

const STATE_stateTransitionTable_t STATE_safety_transitionTable =
{

//Event 		//fromState			         //Guard							//Action									//toState
//=========================================================================================================================================================

		//Temporal Stop
		{/* evAllOk,     IS_SAFE_ERROR2  */(STATE_GuardPtr_t) STATE__safety_countSafeOk_2, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ALLOK },
		{/* evReqErr1,   IS_SAFE_ERROR2  */(STATE_GuardPtr_t) STATE__safety_countSafeOkOrSafe1_2, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ERROR1 },
		{/* evReqErr2,   IS_SAFE_ERROR2  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__safety_resetSafetyCount, (uint32_t) IS_SAFE_ERROR2 },
		{/* evReqErr3,   IS_SAFE_ERROR2  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ERROR3 },

		//Reduced speed
		{/* evAllOk,     IS_SAFE_ERROR1  */(STATE_GuardPtr_t) STATE__safety_countSafeOk_1, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ALLOK },
		{/* evReqErr1,   IS_SAFE_ERROR1  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__safety_resetSafetyCount, (uint32_t) IS_SAFE_ERROR1 },
		{/* evReqErr2,   IS_SAFE_ERROR1  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__safety_resetSafetyCount, (uint32_t) IS_SAFE_ERROR2 },
		{/* evReqErr3,   IS_SAFE_ERROR1  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ERROR3 },

		//Full speed
		{/* evAllOk,     IS_SAFE_ALLOK   */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ALLOK },
		{/* evReqErr1,   IS_SAFE_ALLOK   */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__safety_resetSafetyCount, (uint32_t) IS_SAFE_ERROR1 },
		{/* evReqErr2,   IS_SAFE_ALLOK   */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) STATE__safety_resetSafetyCount, (uint32_t) IS_SAFE_ERROR2 },
		{/* evReqErr3,   IS_SAFE_ALLOK   */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ERROR3 },

		//Final Error, no more actions, ENDSTATE
		{/* evAllOk,     IS_SAFE_ERROR3  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ERROR3 },
		{/* evReqErr1,   IS_SAFE_ERROR3  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ERROR3 },
		{/* evReqErr2,   IS_SAFE_ERROR3  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ERROR3 },
		{/* evReqErr3,   IS_SAFE_ERROR3  */(STATE_GuardPtr_t) 0, (STATE_ActionPtr_t) 0, (uint32_t) IS_SAFE_ERROR3 },

};
const uint16_t STATE_safety_transitionTable_Size = sizeof(STATE_safety_transitionTable)
		/ sizeof(STATE_stateTransition_t);

extern const STATE_stateActionTable_t STATE_safety_actionTable =
{

//State						//Entry												//Exit
//======================================================================================
		{/* IS_SAFE_ERROR2 */(STATE_ActionPtr_t) STATE__safety_stopCar, (STATE_ActionPtr_t) 0 },
		{/* IS_SAFE_ERROR1 */(STATE_ActionPtr_t) STATE__safety_allowReducedSpeed, (STATE_ActionPtr_t) 0 },
		{/* IS_SAFE_ALLOK  */(STATE_ActionPtr_t) STATE__safety_allowMaxSpeed, (STATE_ActionPtr_t)0 },
		{/* IS_SAFE_ERROR3  */(STATE_ActionPtr_t) STATE__safety_terminate, (STATE_ActionPtr_t)0 }
};
const uint16_t STATE_safety_actionTable_Size = sizeof(STATE_safety_actionTable) / sizeof(STATE_stateAction_t);

/**
 * Implementation for the action expressions
 */

static void STATE__safety_allowMaxSpeed()
{

	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	carState.speedLimit = 100;
	DET_setFunctionName(0);		//Reset error string for display

	CARSTATE_set(&so_carstate, carState);
}

static void STATE__safety_allowReducedSpeed()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	carState.speedLimit = 50;

	CARSTATE_set(&so_carstate, carState);
}

static void STATE__safety_stopCar()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	carState.speedLimit = 0;

	CARSTATE_set(&so_carstate, carState);
}

static void STATE__safety_terminate()
{
	ENGINE_run_powerOff();

}

#define STATE__HYSTERESIS_SAFE_1 50		/* 0.5s */
#define STATE__HYSTERESIS_SAFE_2 100    /* 1s */


static bool STATE__safety_countSafeOk_1()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	carState.countSafeOk++;
	carState.countSafeOkOrSafe1++;

	CARSTATE_set(&so_carstate, carState);

	return (carState.countSafeOk >= STATE__HYSTERESIS_SAFE_1);
}

static bool STATE__safety_countSafeOk_2()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	carState.countSafeOk++;
	carState.countSafeOkOrSafe1++;

	CARSTATE_set(&so_carstate, carState);

	return (carState.countSafeOk >= STATE__HYSTERESIS_SAFE_2);
}

static bool STATE__safety_countSafeOkOrSafe1_2()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	carState.countSafeOkOrSafe1++;

	CARSTATE_set(&so_carstate, carState);

	return (carState.countSafeOkOrSafe1 >= STATE__HYSTERESIS_SAFE_2);
}

static void STATE__safety_resetSafetyCount()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	carState.countSafeOk = 0;
	carState.countSafeOkOrSafe1 = 0;

	CARSTATE_set(&so_carstate, carState);
}

