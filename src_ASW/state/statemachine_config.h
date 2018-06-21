/**
* \file statemachine_config.h
* \author Fromm
* \date 16.06.2016
*
* \brief statemachine_config
*
* Configuration for statemachine object. At the moment, we have two central state machine,
* one for the normal system state (connection state, feature selection) and one for emergency states and central
* error handling.
*
* Checklist for extending the logic of the statemachine
* =====================================================
*
* UC: New Transition
* - Simply add the transition to the table of the statemachine_config.cpp file
*
* UC: New State
* - Add the state to the enum declaration in this file
* - Extend the entry / exit table in statemachine_config.cpp
* - Extend the transition table in statemachine_config.cpp
*
* UC: New Event
* - Add the event to the list of events in SystemEvents.h
* - Make sure, that all events for one statemachine are defined using bitpatterns without holes
*   0001, 0010, 0100, 1000 ==> ok
*   0001, 0100, 1000 ==> nok (0010 missing)
* - Update the #define for the number of events in this file
* - Check the event mask scaling in STATE_safety_processEvent in statemachine.cpp.
* - Update the masks 'normalEvents' and 'safetyEvents' in Task_Co_State.cpp
* - Extend the table in statemachine_config.cpp
*
*
* \todo <todos>
* \warning works only with statemachine.cpp
*
*  Changelog:\n
*  - 1.0; 16.06.2016; Fromm
*            - Initial release
*  - 1.1; 16.08.2016; Fromm
*            - Integration with SWC_remote
*            - Added action expressions for 'normal' state machine
*  - 1.2; 14.06.2017; Rahul Raj
*  			 - Macro based STATE enums and strings defined
*  - 1.3; 26.10.2017; Fromm
*            - Added boot states and events to normal state machine
*            - Added Safe3 state and events to error state machine
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

#ifndef STATEMACHINE_CONFIG_H_
#define STATEMACHINE_CONFIG_H_

#include "statemachine_type.h"


/*****************************************************************************************************************
 * Definitions for normal state machine
 *****************************************************************************************************************/

/**
 * Number of events, required for calculating the offset in the table
 */
#define STATE_NORMAL_NOEVENTS 10	    // Check SystemEvents.h


#define STATE_DEF_ENUM(x) 		x,
#define STATE_DEF_ENUM_STR(x) 	#x,

/**
 * List of state for the normal state machine
 */
#define NORMAL_STATE_DEF(DEF)	\
DEF(IS_NMT_BOOTING)  			\
DEF(IS_DEVICES_BOOTING) 		\
DEF(IS_DISCONNECTED)			\
DEF(IS_PRESENT)					\
DEF(IS_CONNECTED)				\
DEF(IS_PRERUN)					\
DEF(IS_RUNNING)

typedef enum {
	NORMAL_STATE_DEF(STATE_DEF_ENUM)
	NORMAL_STATE_MAX
} STATE_normalState_t;

/**
 * String identifier Table of state for the normal state machine
 */
extern const char_t* const STATE_normal_strTable[];

/**
 * Table containing all transition of the normal state machine
 */
extern const STATE_stateTransitionTable_t STATE_normal_transitionTable;

/**
 * Number of entries in the transition table
 */
extern const uint16_t STATE_normal_transitionTable_Size;

/**
 * Table containing the state actions
 */
extern const STATE_stateActionTable_t STATE_normal_actionTable;

/**
 * Number of entries in the action table
 */
extern const uint16_t STATE_normal_actionTable_Size;

/**
 * Table with the drive modes
 */
extern const STATE_driveActionTable_t STATE_driveTable;

/**
 * Number of entries in the drive table
 */
extern const uint16_t STATE_driveTable_Size;

/****************************************************************************************************************
 * Definitions for safety state machine
 ****************************************************************************************************************/

/**
 * Number of events, required for calculating the offset in the table
 */
#define STATE_SAFETY_NOEVENTS 4	    // Check SystemEvents.h

/**
 * List of state for the normal state machine
 */
#define SAFETY_STATE_DEF(DEF)	\
DEF(IS_SAFE_ERROR2) 			\
DEF(IS_SAFE_ERROR1)				\
DEF(IS_SAFE_ALLOK)				\
DEF(IS_SAFE_ERROR3)				\

typedef enum
{
	SAFETY_STATE_DEF(STATE_DEF_ENUM)
	SAFETY_STATE_MAX
} STATE_safetyState_t;

/**
 * String identifier Table of Safety state for the normal state machine
 */
extern const char_t* const STATE_safety_strTable[];

/**
 * Table containing all transition of the normal state machine
 */
extern const STATE_stateTransitionTable_t STATE_safety_transitionTable;

/**
 * Number of entries in the transition table
 */
extern const uint16_t STATE_safety_transitionTable_Size;

/**
 * Table containing the state actions
 */
extern const STATE_stateActionTable_t STATE_safety_actionTable;

/**
 * Number of entries in the action table
 */
extern const uint16_t STATE_safety_actionTable_Size;


















#endif /* STATEMACHINE_CONFIG_H_ */
