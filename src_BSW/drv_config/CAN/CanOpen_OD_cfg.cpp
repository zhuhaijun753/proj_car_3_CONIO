/*
 * CanOpen.cpp
 *
 *  Created on: 01.05.2016
 *      Author: Fromm
 */

#include "CanOpen_OD_cfg.h"
#include "CANopen_Client.h"

#include "signal_bb.h"

#include "enginedecoder_type.h"
#include "enginestate_type.h"
#include "engine_type.h"

#include "lidarscan_type.h"

#include "det.h"
#include "SystemEvents.h"
#include "Task_C0_Communication.h"

extern "C" {




/**********************************************************************************************************************************************
/** Data dictionary objects
 **********************************************************************************************************************************************/

/**
 * Add all data dictionary objects into this section
 * Naming Convention:
 * 	Type: CODD_<your signal><_optionalDescription>_t
 * 	Name: CODD_<your signal><_optionalDescription>_data
 * 	The type is declared in the corresponding <signal>_type.h file
 * 	The raw data of the signal is a pointer to this data structure
 * 	All CanOpen Objects are sored in the section .com.canopen.datadictionary
 */


#pragma section ".com.canopen.datadictionary.safety"


//Engine NMT, state and speed objects
CODD_ENGINESTATE_t CODD_ENGINESTATE_data = ENGINESTATE_INIT_CODD_ENGINESTATE ;
//CODD_ENGINESTATE_nmtStateAllEngines_t CODD_ENGINESTATE_nmtStateAllEngines_data = ENGINESTATE_INIT_CODD_NMT;
CODD_ENGINE_t CODD_ENGINE_data = ENGINE_INIT_CODD;


//Lidar data object
uint32_t CODD_LIDAR_targetSpeed = 0;		//Requested driving speed
uint32_t CODD_LIDARCONFIG_data = 0;			//Current configuration

//Decoder data objects
//Moved to decoder signal
//CODD_ENGINEDECODER_t CODD_ENGINEDECODER_currentPosition_data = ENGINEDECODER_INIT_CODD;
//CODD_ENGINEDECODER_t CODD_ENGINEDECODER_previousPosition_data = ENGINEDECODER_INIT_CODD;

uint32_t CODD_SMARTPOWER_powerState = 0;													/**< \brief Bitpattern holding the state of the individual power domains */

#pragma section
#pragma section ".com.canopen.datadictionary.control"

//Lidar data
CODD_LIDARSCAN_t CODD_LIDARSCAN_data[LIDARCONFIG_MAX_RESOLUTION];

#pragma section


/**********************************************************************************************************************************************
/** Local Functions
 **********************************************************************************************************************************************/


STATIC RC_t CANOPEN_SDO_callback(volatile CANopen_SDO_dispatcher_element_t const * const SDO_info);

/**********************************************************************************************************************************************
/** Generic Functions
 **********************************************************************************************************************************************/

RC_t CANOPEN_process_PDO(CANopen_CAN_MSG_t* CAN_in)
{
	if (NODE_ID_LIDAR == (CAN_in->COB_ID & CANOPEN_NODE_ID_MSK))
	{
		PxTask_t taskId = Task_C0_Communication_getTaskId();
		PxTaskSignalEvents_Hnd(taskId, ev_Task_C0_Communication_4_Can_Rx_PDO);

		return RC_SUCCESS;
	}

	return RC_ERROR;
}


/**********************************************************************************************************************************************
/** Engine (Faulhaber) Functions
 **********************************************************************************************************************************************/


RC_t CANOPEN_write_engineSpeedToFaulhaber(const ENGINE_raw_t * const canbuffer,
		const SIG_IDX_TYPE index) {

	CANopen_SDO_transfer_RET_t status_FL;
	CANopen_SDO_transfer_RET_t status_FR;
	CANopen_SDO_transfer_RET_t status_RR;
	CANopen_SDO_transfer_RET_t status_RL;

	RC_t result = RC_SUCCESS;


	//Engine front left
	status_FL = CANopen_setSDO(0, &((*canbuffer)->wheelspeedFL), sizeof(sint32_t), NODE_ID_FL, INDEX_ENGINE_SETSPEED, 0, TIMEOUT_ENGINESDO, 0);
	if (status_FL != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_FL_SDO_SETSPEED,status_FL);
	}

	//Engine front right
	status_FR = CANopen_setSDO(0, &((*canbuffer)->wheelspeedFR), sizeof(sint32_t), NODE_ID_FR, INDEX_ENGINE_SETSPEED, 0, TIMEOUT_ENGINESDO, 0);
	if (status_FR != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_FR_SDO_SETSPEED,status_FR);
	}

	//Engine rear right
	status_RR = CANopen_setSDO(0, &((*canbuffer)->wheelspeedRR), sizeof(sint32_t), NODE_ID_RR, INDEX_ENGINE_SETSPEED, 0, TIMEOUT_ENGINESDO, 0);
	if (status_RR != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_RR_SDO_SETSPEED,status_RR);
	}

	//Engine rear left
	status_RL = CANopen_setSDO(0, &((*canbuffer)->wheelspeedRL), sizeof(sint32_t), NODE_ID_RL, INDEX_ENGINE_SETSPEED, 0, TIMEOUT_ENGINESDO, 0);
	if (status_RL != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_RL_SDO_SETSPEED,status_RL);
	}


	//Return logical AND result of all four engines
	return result;

}



RC_t CANOPEN_read_engineStateFromFaulhaber(ENGINESTATE_raw_t * const canbuffer,
		const SIG_IDX_TYPE index) {
	//read the NMT state from the global variable set from ISR for each engine
	//@PF guess this is not working at all
	//Hier muss das Heartbeat Protokoll eingesetzt werden




	CANopen_SDO_transfer_RET_t status_FL;
	CANopen_SDO_transfer_RET_t status_FR;
	CANopen_SDO_transfer_RET_t status_RR;
	CANopen_SDO_transfer_RET_t status_RL;

	RC_t result = RC_SUCCESS;


	//engine front left
	if (((*canbuffer)->enfineFL_rawState).command == CMD_GetSetEngineState)
	{
		status_FL = CANopen_getSDO(0, &((CODD_ENGINESTATE_data.enfineFL_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_FL, INDEX_ENGINE_GETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_FL != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_FL_SDO_GETSTATE,status_FL);
		}
	}


	//engine front right
	if (((*canbuffer)->enfineFR_rawState).command == CMD_GetSetEngineState)
	{
		status_FR = CANopen_getSDO(0, &((CODD_ENGINESTATE_data.enfineFR_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_FR, INDEX_ENGINE_GETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_FR != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_FR_SDO_GETSTATE,status_FR);
		}
	}


	//engine rear right
	if (((*canbuffer)->enfineRR_rawState).command == CMD_GetSetEngineState)
	{
		status_RR = CANopen_getSDO(0, &((CODD_ENGINESTATE_data.enfineRR_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_RR, INDEX_ENGINE_GETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_RR != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_RR_SDO_GETSTATE,status_RR);
		}
	}

	//engine rear left
	if (((*canbuffer)->enfineRL_rawState).command == CMD_GetSetEngineState)
	{
		status_RL = CANopen_getSDO(0, &((CODD_ENGINESTATE_data.enfineRL_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_RL, INDEX_ENGINE_GETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_RL != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_RL_SDO_GETSTATE,status_RL);
		}
	}

	//Return logical AND of all engine error codes
	return result;
}

RC_t CANOPEN_write_engineStateToFaulhaber(const ENGINESTATE_raw_t * const canbuffer, const SIG_IDX_TYPE index) {

	//set the NMT state for each engine
	if (((*canbuffer)->enfineFL_rawState).command == CMD_GetSetNmt)
	{
		CANopen_sendNMTcmd(((*canbuffer)->enfineFL_rawState).rawNmtState, NODE_ID_FL);
	}

	if (((*canbuffer)->enfineFR_rawState).command == CMD_GetSetNmt)
	{
		CANopen_sendNMTcmd(((*canbuffer)->enfineFR_rawState).rawNmtState, NODE_ID_FR);
	}

	if (((*canbuffer)->enfineRR_rawState).command == CMD_GetSetNmt)
	{
		CANopen_sendNMTcmd(((*canbuffer)->enfineRR_rawState).rawNmtState, NODE_ID_RR);
	}

	if (((*canbuffer)->enfineRL_rawState).command == CMD_GetSetNmt)
	{
		CANopen_sendNMTcmd(((*canbuffer)->enfineRL_rawState).rawNmtState, NODE_ID_RL);
	}



	/*
	if (((*canbuffer)->enfineFL_rawState).rawNmtState != 0x05
			|| ((*canbuffer)->enfineFR_rawState).rawNmtState != 0x05
			|| ((*canbuffer)->enfineRL_rawState).rawNmtState != 0x05
			|| ((*canbuffer)->enfineRR_rawState).rawNmtState != 0x05) {
		CANopen_sendNMTcmd(CANopen_NMT_CMD_START_NODE, 0x00);
	}
	*/


	CANopen_SDO_transfer_RET_t status_FL;
	CANopen_SDO_transfer_RET_t status_FR;
	CANopen_SDO_transfer_RET_t status_RR;
	CANopen_SDO_transfer_RET_t status_RL;

	RC_t result = RC_SUCCESS;

	//engine front left
	if (((*canbuffer)->enfineFL_rawState).command == CMD_GetSetEngineState)
	{
		status_FL = CANopen_setSDO(0, &(((*canbuffer)->enfineFL_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_FL, INDEX_ENGINE_SETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_FL != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_FL_SDO_SETSTATE, status_FL);
		}
	}

	//engine front right
	if (((*canbuffer)->enfineFR_rawState).command == CMD_GetSetEngineState)
	{
		status_FR = CANopen_setSDO(0, &(((*canbuffer)->enfineFR_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_FR, INDEX_ENGINE_SETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_FR != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_FR_SDO_SETSTATE, status_FR);
		}
	}

	//engine rear right
	if (((*canbuffer)->enfineRR_rawState).command == CMD_GetSetEngineState)
	{
		status_RR = CANopen_setSDO(0, &(((*canbuffer)->enfineRR_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_RR, INDEX_ENGINE_SETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_RR != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_RR_SDO_SETSTATE, status_RR);
		}
	}

	//engine rear left
	if (((*canbuffer)->enfineRL_rawState).command == CMD_GetSetEngineState)
	{
		status_RL = CANopen_setSDO(0, &(((*canbuffer)->enfineRL_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_RL, INDEX_ENGINE_SETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_RL != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_RL_SDO_SETSTATE, status_RL);
		}
	}

	//Return logical and of all error codes
	return result;
}

RC_t CANOPEN_read_DecoderFromFaulhaber(ENGINEDECODER_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{

	CANopen_SDO_transfer_RET_t status_FL;
	CANopen_SDO_transfer_RET_t status_FR;
	CANopen_SDO_transfer_RET_t status_RR;
	CANopen_SDO_transfer_RET_t status_RL;

	RC_t result = RC_SUCCESS;

/*
	//Engine front left
	status_FL = CANopen_getSDO(0, &(CODD_ENGINEDECODER_currentPosition_data.engineFL_Pos), sizeof(sint32_t), NODE_ID_FL, INDEX_ENGINE_GETDECODER, 0, TIMEOUT_ENGINESDO, 0);
	if (status_FL != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_FL_SDO_GETSPEED, status_FL);
	}


	//Engine front right
	status_FR = CANopen_getSDO(0, &(CODD_ENGINEDECODER_currentPosition_data.engineFR_Pos), sizeof(sint32_t), NODE_ID_FR, INDEX_ENGINE_GETDECODER, 0, TIMEOUT_ENGINESDO, 0);
	if (status_FR != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_FR_SDO_GETSPEED, status_FR);
	}

	//Engine rear right
	status_RR = CANopen_getSDO(0, &(CODD_ENGINEDECODER_currentPosition_data.engineRR_Pos), sizeof(sint32_t), NODE_ID_RR, INDEX_ENGINE_GETDECODER, 0, TIMEOUT_ENGINESDO, 0);
	if (status_RR != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_RR_SDO_GETSPEED, status_RR);
	}

	//Engine rear left
	status_RL = CANopen_getSDO(0, &(CODD_ENGINEDECODER_currentPosition_data.engineRL_Pos), sizeof(sint32_t), NODE_ID_RL, INDEX_ENGINE_GETDECODER, 0, TIMEOUT_ENGINESDO, 0);
	if (status_RL != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_RL_SDO_GETSPEED, status_RL);
	}
*/
	//Return logical and of all error codes
	return result;
}


/**********************************************************************************************************************************************
/** Lidar Functions
 **********************************************************************************************************************************************/


RC_t CANOPEN_write_ConfigToLidar( const LIDARCONFIG_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{

	CANopen_SDO_transfer_RET_t status = CANopen_setSDO(0, &(CODD_LIDARCONFIG_data), sizeof(uint32_t), NODE_ID_LIDAR, INDEX_LIDAR_SETCONFIG, 0, TIMEOUT_LIDARSDO, 0);

	if (status != CANOPEN_SDO_SUCCESS)
	{
		DET_stop(AUTOCORE, LIDAR_SDO_SETCONFIG, status);
		return RC_ERROR_WRITE_FAILS;
	}

	return RC_SUCCESS;
}

RC_t CANOPEN_read_ConfigFromLidar( LIDARCONFIG_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{
	return RC_SUCCESS;
}

RC_t CANOPEN_write_targetSpeedToLidar( const TARGETSPEED_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{

	CANopen_SDO_transfer_RET_t status = CANopen_setSDO(0, &(CODD_LIDAR_targetSpeed), sizeof(uint32_t), NODE_ID_LIDAR, INDEX_LIDAR_SETTARGETSPEED, 0, TIMEOUT_LIDARSDO, 0);

	if (status != CANOPEN_SDO_SUCCESS)
	{
		DET_stop(AUTOCORE, LIDAR_SDO_SETTARGETSPEED, status);
		return RC_ERROR_WRITE_FAILS;
	}

	return RC_SUCCESS;

}

RC_t CANOPEN_read_ScanFromLidar(LIDARSCAN_raw_t * const canbuffer, const SIG_IDX_TYPE index) {

	//Request a data update, in a first step in a blocking mode

	CANopen_SDO_transfer_RET_t status = CANopen_getSDO(&CANOPEN_SDO_callback,
													   &(CODD_LIDARSCAN_data[0]), 72,
													   NODE_ID_LIDAR, INDEX_LIDAR_GETFULLSCAN, 0,
													   TIMEOUT_LIDARSCANSDO, 0);
	if (status != CANOPEN_SDO_SUCCESS)
	{
		DET_stop(AUTOCORE, LIDAR_SDO_GETFULLSCAN, status);
		return RC_ERROR_WRITE_FAILS;
	}

	return RC_SUCCESS;
}

STATIC RC_t CANOPEN_SDO_callback(volatile CANopen_SDO_dispatcher_element_t const * const SDO_info)
{
	uint8_t error_flags = (CANOPEN_DISP_FLAGS_T | CANOPEN_DISP_FLAGS_A | CANOPEN_DISP_FLAGS_O | CANOPEN_DISP_FLAGS_E);

	if ((NODE_ID_LIDAR == SDO_info->node) && (INDEX_LIDAR_GETFULLSCAN == SDO_info->index))
	{
		if (0 == (SDO_info->flags & error_flags))
			LIDARSCAN_write_raw(&so_in_lidarscan, &(CODD_LIDARSCAN_data[0]));
	}

	return RC_SUCCESS;
}


/**********************************************************************************************************************************************
/** SmartPower Functions
 **********************************************************************************************************************************************/

RC_t CANOPEN_read_PowerStateFromSmartPower(uint32_t * const canbuffer, const SIG_IDX_TYPE index)
{
	RC_t result = RC_SUCCESS;


	CANopen_SDO_transfer_RET_t status = CANopen_getSDO(0, canbuffer, sizeof(uint32_t), NODE_ID_SMARTPOWER, INDEX_SMARTPOWER_POWSTATE, 0, TIMEOUT_SMARTPOWER_SDO, 0);
	if (status != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_READ_FAILS;
		DET_stop(AUTOCORE, SMARTPOWER_SDO_GETPOWERSTATE, status);
	}

	return result;
}


/**
 * Request the NMT state from SmartPower Board
 */
RC_t CANOPEN_read_NMTfromSmartpower(NMT_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{
	RC_t result = RC_SUCCESS;
	CANopen_SDO_transfer_RET_t status;

	status = CANopen_getSDO(0, &canbuffer->dd_lidar,  sizeof(uint32_t), NODE_ID_SMARTPOWER, INDEX_SMARTPOWER_NMTSHM_IDX, NODE_ID_LIDAR, TIMEOUT_SMARTPOWER_SDO, 0);
	if (status != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_READ_FAILS;
		DET_stop(AUTOCORE, SMARTPOWER_SDO_GETNMTSTATE, status);
	}


	status = CANopen_getSDO(0, &canbuffer->dd_engineFL,  sizeof(uint32_t), NODE_ID_SMARTPOWER, INDEX_SMARTPOWER_NMTSHM_IDX, NODE_ID_FL, TIMEOUT_SMARTPOWER_SDO, 0);
	if (status != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_READ_FAILS;
		DET_stop(AUTOCORE, SMARTPOWER_SDO_GETNMTSTATE, status);
	}

	status = CANopen_getSDO(0, &canbuffer->dd_engineFR,  sizeof(uint32_t), NODE_ID_SMARTPOWER, INDEX_SMARTPOWER_NMTSHM_IDX, NODE_ID_FR, TIMEOUT_SMARTPOWER_SDO, 0);
	if (status != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_READ_FAILS;
		DET_stop(AUTOCORE, SMARTPOWER_SDO_GETNMTSTATE, status);
	}

	status = CANopen_getSDO(0, &canbuffer->dd_engineRL,  sizeof(uint32_t), NODE_ID_SMARTPOWER, INDEX_SMARTPOWER_NMTSHM_IDX, NODE_ID_RL, TIMEOUT_SMARTPOWER_SDO, 0);
	if (status != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_READ_FAILS;
		DET_stop(AUTOCORE, SMARTPOWER_SDO_GETNMTSTATE, status);
	}

	status = CANopen_getSDO(0, &canbuffer->dd_engineRR,  sizeof(uint32_t), NODE_ID_SMARTPOWER, INDEX_SMARTPOWER_NMTSHM_IDX, NODE_ID_RR, TIMEOUT_SMARTPOWER_SDO, 0);
	if (status != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_READ_FAILS;
		DET_stop(AUTOCORE, SMARTPOWER_SDO_GETNMTSTATE, status);
	}


}



} //extern c

