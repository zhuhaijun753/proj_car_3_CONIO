/*
 * statemachine_type.h
 *
 * Contains general typedefinition for the state machine
 *  Created on: 16.06.2016
 *      Author: Fromm
 */

#ifndef STATEMACHINE_TYPE_H_
#define STATEMACHINE_TYPE_H_

#include "global.h"
#include "cardataprotocol_type.h"

/******************************************************************************/
/*------------------------- Type Definitions ---------------------------------*/
/******************************************************************************/

#define FEATURE_NAME_MAX_LENGTH 			REMOTE_MAX_DATASIZE  // Max length of features incl \0

/**
 * function pointer used for actions
 */
typedef void (*STATE_ActionPtr_t)();

/**
 * function pointer used for guards
 */
typedef bool (*STATE_GuardPtr_t)();



/**
 * State Transition
 */
typedef struct
{
	STATE_GuardPtr_t guard;
	STATE_ActionPtr_t actionTransition;
	uint32_t toState;
} STATE_stateTransition_t;

typedef STATE_stateTransition_t  STATE_stateTransitionTable_t[];

/**
 * State Action
 */
typedef struct
{
	STATE_ActionPtr_t actionEntry;
	// STATE_ActionPtr_t actionDo; //Not used, as state machine is called time triggered
	STATE_ActionPtr_t actionExit;
} STATE_stateAction_t;

typedef STATE_stateAction_t  STATE_stateActionTable_t[];

/**
 * Drive mode
 */
typedef struct
{
	STATE_ActionPtr_t init;
	STATE_ActionPtr_t drive;
	STATE_ActionPtr_t deInit;
	char featureName[FEATURE_NAME_MAX_LENGTH];
} STATE_driveAction_t;

typedef STATE_driveAction_t STATE_driveActionTable_t[];


#endif /* STATEMACHINE_TRANSITION_TYPE_H_ */
