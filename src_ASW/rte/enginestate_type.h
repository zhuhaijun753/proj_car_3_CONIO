/*
 * ENGINESTATE_type.h
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm, 2016
 */



#ifndef ENGINESTATE_TYPE_H_
#define ENGINESTATE_TYPE_H_

#include "global.h"
#include "CANopen.h"

/*****************************************************************************************
 *************** Type Definition for ENGINESTATE  signal class **************************
 *****************************************************************************************/

// State of the engine and network management

typedef enum
{
	CMD_none,						/**< \brief command which will be ignored */
	CMD_GetSetNmt,					/**< \brief command for reading/writing the current NMT state to the engine node */
	CMD_GetSetEngineState			/**< \brief command for reading/writing the current engine state to the engine node */
} ENGINESTATE_Command_t;

typedef enum
{
	DriveState_NotReadyToSwitchON,
	DriveState_SwitchOnDisabled,
	DriveState_ReadyToSwitchOn,
	DriveState_SwitchedOn,
	DriveState_OperationEnable,
	DriveState_QuickStop,
	DriveState_Unknown
}ENGINESTATE_DriveState_t;


typedef struct
{
	ENGINESTATE_Command_t			command;					/**< \brief Depending on the command, a specific part of the data will be written, e.g. engine state or NMT */
	ENGINESTATE_DriveState_t 		applicationDriveState;
	CANopen_NMT_commands_t 			applicationNmtState;
	uint8_t 						heartbeatErrorCount;
}ENGINESTATE_DataPerEngine_t;

typedef struct
{
	ENGINESTATE_DataPerEngine_t engineFL_appState;
	ENGINESTATE_DataPerEngine_t engineFR_appState;
	ENGINESTATE_DataPerEngine_t engineRR_appState;
	ENGINESTATE_DataPerEngine_t engineRL_appState;
} ENGINESTATE_data_t;


// Type definitions for the CanOpen protocol (Object Directory)

typedef struct
{

	uint16_t 						rawDriveState;
	CANopen_NMT_commands_t 			rawNmtState;
	ENGINESTATE_Command_t			command;
}CODD__rawStatePerEngine_t;

typedef struct
{
	CODD__rawStatePerEngine_t enfineFL_rawState;
	CODD__rawStatePerEngine_t enfineFR_rawState;
	CODD__rawStatePerEngine_t enfineRR_rawState;
	CODD__rawStatePerEngine_t enfineRL_rawState;
}CODD_ENGINESTATE_t;


//Data returned from the engine??????
/*
typedef struct{
	uint8_t engineFL_NMT;
	uint8_t engineFR_NMT;
	uint8_t engineRR_NMT;
	uint8_t engineRL_NMT;
}CODD_ENGINESTATE_nmtStateAllEngines_t;
*/

typedef CODD_ENGINESTATE_t* ENGINESTATE_raw_t;


extern "C" {
extern CODD_ENGINESTATE_t CODD_ENGINESTATE_data;
}

//defining init values for Raw and Application data

#define ENGINESTATE_INIT_DATA_PER_ENGINE (ENGINESTATE_DataPerEngine_t){CMD_none, DriveState_SwitchOnDisabled, CANopen_NMT_CMD_INIT_DONE,0}
#define ENGINESTATE_INIT_DATA ((ENGINESTATE_data_t){ENGINESTATE_INIT_DATA_PER_ENGINE, ENGINESTATE_INIT_DATA_PER_ENGINE, ENGINESTATE_INIT_DATA_PER_ENGINE, ENGINESTATE_INIT_DATA_PER_ENGINE})

#define ENGINESTATE_INIT_RAW  ((ENGINESTATE_raw_t)&CODD_ENGINESTATE_data)

//@PF This one needs to be checked
/*
#define ENGINESTATE_INIT_CODD_NMT ((CODD_ENGINESTATE_nmtStateAllEngines_t){0x7f, 0x7f, 0x7f, 0x7f})
*/

#define  ENGINESTATE_INIT_CODD_ENGINESTATE_PER_ENGINE ( (CODD__rawStatePerEngine_t){  DriveState_SwitchOnDisabled, CANopen_NMT_CMD_INIT_DONE , CMD_none})
#define ENGINESTATE_INIT_CODD_ENGINESTATE ((CODD_ENGINESTATE_t){ENGINESTATE_INIT_CODD_ENGINESTATE_PER_ENGINE, ENGINESTATE_INIT_CODD_ENGINESTATE_PER_ENGINE, ENGINESTATE_INIT_CODD_ENGINESTATE_PER_ENGINE, ENGINESTATE_INIT_CODD_ENGINESTATE_PER_ENGINE })

/*****************************************************************************************
 *************** Type specific scaler functions ******************************************
 *****************************************************************************************/
#ifdef  __cplusplus
extern "C"
{
#endif

/**
 * Scaler to translate bytestream protocol into ENGINE application data type
 * @param const ENGINE_raw_t * const praw  ==> INPUT: Pointer to the signal raw data object
 * @param const ENGINE_data_t * const pvalue ==> OUTPUT: Pointer to the signal application data object
 */
RC_t ENGINESTATE_scaleR2D( const ENGINESTATE_raw_t * const praw, ENGINESTATE_data_t * const pvalue);

/**
 * Scaler to translate engine control speed into CanOpen protocol
 * @param const ENGINE_data_t * const pvalue ==> INPUT: Pointer to the signal application data object
 * @param ENGINESTATE_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t ENGINESTATE_scaleD2R( const ENGINESTATE_data_t * const pvalue, ENGINESTATE_raw_t * const praw);

#ifdef  __cplusplus
}
#endif

#endif /* SWC_ENGINE_TYPE_H_ */

