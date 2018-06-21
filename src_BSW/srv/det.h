/**
 * \file 	det.h
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	18.7.2017
 *
 * \brief Development Error Tracer
 *
 * This file contains functions to react on runtime errors during the development
 *
 * \note <notes>
 * \todo <todos>
 * \warning <warnings, e.g. dependencies, order of execution etc.>
 *
 *  Changelog:\n
 *  - 1.0; 2016; Fromm
 *            - Initial Creation
 *  - 2.0; 7/2017; Aaron, Rahul
 *            - Extension of report mode
 *
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
#ifndef DET_H_
#define DET_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "global.h"
#include "det_time.h"


/********************************************[ #define ]*************************************************/

/**
 * Central error messages of the DET module
 */
#define DET_ARRAYSIZE 30
#define AUTOCORE 0xffff
#define TIMEOUTSYNC 0xffffffff

//Macros to create enums and strings based on the same literal
#define DET_DEF_ENUM(x,y) 		x,
#define DET_DEF_ENUM_MODE(x,y)	y,
#define DET_DEF_ENUM_STR(x,y) 	#x,


/********************************************[ Type definitions ]*****************************************/

/**
 * Controls the behavior on an error
 * REPORT: 	DET_stop dumps the message on the display
 * BLOCKED: DET_STOP will terminate
 */
typedef enum
{
	REPORT,
	BLOCKED

}DET_mode_t;



//The list of error messages
//======================Fault Name________________________ FAULT MODE==========================//
#define DET_MSG_DEF(DEF)	\
		DEF(			EMPTY,								REPORT)\
		DEF(			SETUPTIMER,							REPORT)\
		DEF(			CREATENAMESERVER,					REPORT)\
		DEF(			CREATERELEASESERVER,				REPORT)\
		DEF(			CHECKSERVICE,						REPORT)\
		DEF(			REGISTERNAME,						REPORT)\
		DEF(			CREATETASK,							REPORT)\
		DEF(			INTERRUPTVALIDATION,				REPORT)\
		DEF(			SIGNALEVENT,						REPORT)\
		DEF(			PXBOOTSYNC0,						REPORT)\
		DEF(			PXBOOTSYNC1,						REPORT)\
		DEF(			PXBOOTSYNC2,						REPORT)\
		DEF(			PXBOOTSYNC3,						REPORT)\
		DEF(			PXBOOTSYNC4,						REPORT)\
		DEF(			RTL_WRONGCORE,						REPORT)\
		DEF(			RTL_NOMESSAGE,						REPORT)\
		DEF(			RTL_NOMAILBOX,						REPORT)\
		DEF(			RUN_TIMEOUT1,						REPORT)\
		DEF(			RUN_TIMEOUT2,						REPORT)\
		DEF(			QUERYNAME,							REPORT)\
		DEF(			RTE_SCALERISNULL,					REPORT)\
		DEF(			APPL_BUFFEROVERFLOW,				REPORT)\
		DEF(			APPL_WRONGPROTOCOLSIZE,				REPORT)\
		DEF(			STATE_WRONGSTATE,					REPORT)\
		DEF(			CAN_ERROR,							REPORT)\
		DEF(			MSG_ERROR,							REPORT)\
		DEF(			MSG_TX,								REPORT)\
		DEF(			MSG_RX_PAYLOAD,						REPORT)\
		DEF(			MSG_RX_RETURN,						REPORT)\
		DEF(			MSG_RX_BUFFER,						REPORT)\
		DEF(			MSG_RX_WRONGDATA,					REPORT)\
		DEF(			POINTER_ERROR,						REPORT)\
		DEF(			ENGINE_FL_SDO_SETSPEED,				REPORT)\
		DEF( 			ENGINE_FR_SDO_SETSPEED,				REPORT)\
		DEF(			ENGINE_RL_SDO_SETSPEED,				REPORT)\
		DEF(			ENGINE_RR_SDO_SETSPEED,				REPORT)\
		DEF(			ENGINE_FL_SDO_GETSPEED,				REPORT)\
		DEF(			ENGINE_FR_SDO_GETSPEED,				REPORT)\
		DEF(			ENGINE_RL_SDO_GETSPEED,				REPORT)\
		DEF(			ENGINE_RR_SDO_GETSPEED,				REPORT)\
		DEF(			ENGINE_FL_SDO_SETSTATE,				REPORT)\
		DEF(			ENGINE_FR_SDO_SETSTATE,				REPORT)\
		DEF(			ENGINE_RL_SDO_SETSTATE,				REPORT)\
		DEF(			ENGINE_RR_SDO_SETSTATE,				REPORT)\
		DEF(			ENGINE_FL_SDO_GETSTATE,				REPORT)\
		DEF(			ENGINE_FR_SDO_GETSTATE,				REPORT)\
		DEF(			ENGINE_RL_SDO_GETSTATE,				REPORT)\
		DEF(			ENGINE_RR_SDO_GETSTATE,				REPORT)\
		DEF(			LIDAR_SDO_SETCONFIG,				REPORT)\
		DEF(			LIDAR_SDO_SETTARGETSPEED,			REPORT)\
		DEF(			LIDAR_SDO_GETFULLSCAN,				REPORT)\
		DEF(			CAN_INIT,							REPORT)\
		DEF(			SYSTEM_INIT,						REPORT)\
		DEF(			CFG_CHECK,							REPORT)\
		DEF(			QSPI_MODULE_INIT,					REPORT)\
		DEF(            SMARTPOWER_SDO_GETPOWERSTATE,       REPORT)\
		DEF(            SMARTPOWER_SDO_GETNMTSTATE,         REPORT)\
		DEF(            SMARTPOWER_POWERDOMAIN_FAIL,        REPORT)

//The enum list of the messages
typedef enum
{
	DET_MSG_DEF(DET_DEF_ENUM)
	DET_MESSAGE_MAX
} DET_message_t;

//Data structure for the error array entries (REPORT mode)
typedef struct
{

	uint8_t 		core;
	DET_message_t 	m_message;
	uint32_t 		m_uservalue;
	TIME_t 			m_timestamp;

} DET__infoarray_t;

//Data structure for the error array entries used by the display (REPORT mode)
typedef struct {

	uint8_t 		core;
	const char_t	*mp_message;
	uint32_t 		m_uservalue;
	TIME_t 			m_timestamp;

} DET__dumpinfoarray_t;

/********************************************[ API Functions - Report error and control car ]*****************************************/


/*
 * The DET_stop will fill the Message into a buffer
 * If the Mode corresponding to message is BLOCKED the data is filled to the buffer and the core halts means the error is critical
 * If the Mode corresponding to message is REPORT the data is only filled into the buffer
 * Once the buffer is full the core is halted.
 * Fill level is to indicate how much data is present buffer with respect to the DISPLAYSIZE as the DISPLACESIZE represents the 'Last N'
 * 			updated Errors as DISPLAYSIZE is limited to fit on a small screen
 *
 * @param core The core which is currently being executed
 * @param DET_message Error message of the DET module
 * @param uservalue Error message from the user
 */
extern void DET_stop(unsigned int core, DET_message_t, unsigned int uservalue);

/**
 * Check if the flag for turning off the engines has been set
 * This function is needed to send a final stop command in case the core halts
 */
extern boolean_t DET_ShutDownEngines();


/**
 * Will store the string representing the name of a function, typically a safety function
 * @param char* data string for function name
 */
extern void DET_setFunctionName(const char* data);

/**
 * Will return the string representing the name of a function, typically a safety function
 */
extern const char* DET_getFunctionName();



/********************************************[ API Functions - Report functions for the display ]*****************************************/

/**
 * Returns the fill level of the reporting buffer.
 */
extern uint32_t DET_getFillLevel();

/**
 * Returns if DET was updated after the last read operation
 */
extern boolean_t DET_isNew();

/**
 * Reset the age of the storage buffer
 */
extern RC_t DET_clr_age();

/**
 * get_first takes the first value from the updated series of data
 * Initialised read_index to startindex which points to the first value to be read
 * @param ArrayDump Used to return the the first entity of the list into a Temporary data structure
 */
extern RC_t DET_get_first(DET__dumpinfoarray_t* ArrayDump);

/*
 *
 * get the next value from the buffer
 * @param ArrayDump Used to return the next entity of the list into a Temporary data structure
 */
extern RC_t DET_get_next(DET__dumpinfoarray_t* ArrayDump, sint16_t *apIdx);

/*
 *
 * read one data structure from the set of latest updated value
 * IF the DISPLAYSIZE is not the same as ARRAYSIZE then startindex need to updated to write_index till write_index+fill_level <array size
 *  		to get the last updated N values.
 * 			This should be done only after on read cycle not to run into inconsistent behaviour
 * 	@param ArrayDump Used to return the  entity from the buffer into a Temporary data structure
 */
RC_t DET_read(DET__dumpinfoarray_t* ArrayDump, sint16_t *apIdx);

/*
 *
 * read one data entry by index
 *  @prerequisite - use DET_getFillLevel API for the numer of entries present
 * 	@param ArrayDump Used to return the  entity from the buffer into a Temporary data structure
 * 	@param anIdx specifies the index to be accessed
 */
extern RC_t DET_readByIndex(DET__dumpinfoarray_t* ArrayDump, sint16_t anIdx);

#ifdef __cplusplus
}
#endif

#endif /* DET_H_ */
