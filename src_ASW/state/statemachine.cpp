/*
 * statemachine.cpp
 *
 *  Created on: 16.06.2016
 *      Author: Fromm
 */

#include "carstate_signal.h"
#include "carstate_type.h"

#include "signal_bb.h"

#include "statemachine_type.h"
#include "statemachine_config.h"
#include "statemachine.h"

#include "Ctrace.h"

#include "det.h"

#include "SystemEvents.h"


//#define STATETRACE

#ifdef STATETRACE

#define STATETRACESIZE 20

typedef struct {
	uint8_t command;
	uint8_t state;
	uint16_t eventmask;
} statetrace_t;

#pragma section ".trace.data"
statetrace_t debugTraceState[STATETRACESIZE];
uint16_t debugTraceIndex = 0;
#pragma section

#endif

#ifdef DoesNotWork

#pragma section ".trace.data"
CTrace<statetrace_t,20> myTraceObject;
#pragma section

#endif




/***********************************************************************************
 * Static function declarations
 **********************************************************************************/

enum STATE__statetype_t{STATE_NORMAL, STATE_SAFETY};

/**
 * @brief Generic state handler for processing events
 *
 * Configuration of the state machine is passed as table parameters.
 * The tabels are implemented in the file statemachine_config.cpp
 *
 * @param stateTransitionTable		Pointer to the normal state machine transition table
 * @param transitionTableSize		Size of the state machine transition table (use the predefined const value)
 * @param stateActionTable			Pointer to the normal state machine state action table
 * @param actionTableSize			Size of the state machine state action table (use the predefined const values)
 * @param event						The eventmask which will be processed
 * @param storageState				Identifier for the carstate signal, defining which state variable will be updated
 */
static void STATE__processEvent(const STATE_stateTransitionTable_t stateTransitionTable, const uint16_t transitionTableSize,
		                        const STATE_stateActionTable_t stateActionTable, const uint16_t actionTableSize,
		                        PxEvents_t event, STATE__statetype_t storageState);


/***********************************************************************************
 * Function Implementations
 **********************************************************************************/

void STATE_init()
{
#ifdef STATETRACE
	debugTraceIndex = 0;
#endif
}

/**
 * Calls the normal state machine
 * @param eventMask events which will be processed
 */
void STATE_normal_processEvent(PxEvents_t eventMask)
{
	STATE__processEvent(STATE_normal_transitionTable, STATE_normal_transitionTable_Size,
						STATE_normal_actionTable, STATE_normal_actionTable_Size,
						eventMask, STATE_NORMAL);
}

/**
 * Calls the safety state machine
 * @param eventMask events which will be processed
 */
void STATE_safety_processEvent(PxEvents_t eventMask)
{
	//We have to scale the events to right aligned bits
	//Otherwise the offset calculation in the state machine will fail
	eventMask = eventMask / ev_Task_C0_State_10_ErrorAllOk;

	STATE__processEvent(STATE_safety_transitionTable, STATE_safety_transitionTable_Size,
						STATE_safety_actionTable, STATE_safety_actionTable_Size,
						eventMask, STATE_SAFETY);
}



static void STATE__processEvent(const STATE_stateTransitionTable_t stateTransitionTable, const uint16_t transitionTableSize,
								const STATE_stateActionTable_t stateActionTable, const uint16_t actionTableSize,
								PxEvents_t event, STATE__statetype_t storageState)
{
	//get the old carstate
	CARSTATE_data_t stateSignal = CARSTATE_get(&so_carstate);


	STATE_GuardPtr_t guard = (STATE_GuardPtr_t)0;
	STATE_ActionPtr_t actionTransition = (STATE_ActionPtr_t)0;
	STATE_ActionPtr_t stateEntry = (STATE_ActionPtr_t)0;
	STATE_ActionPtr_t stateDo = (STATE_ActionPtr_t)0;
	STATE_ActionPtr_t stateExit = (STATE_ActionPtr_t)0;
	uint32_t toState = -1;

	//Select either normal state or safety state
	uint32_t currentState = 0;
	uint32_t noEvents = 0;
	if (storageState == STATE_NORMAL)
	{
		currentState = stateSignal.state_normal;
		noEvents = STATE_NORMAL_NOEVENTS;
	}
	else if (storageState == STATE_SAFETY)
	{
		currentState = stateSignal.state_safety;
		noEvents = STATE_SAFETY_NOEVENTS;
	}
	else
	{
		//Todo: add error handling, e.g. DET_Stop
	}

#ifdef STATETRACE
	debugTraceState[debugTraceIndex].command = 1;
	debugTraceState[debugTraceIndex].eventmask = event;
	debugTraceState[debugTraceIndex].state = currentState;
	debugTraceIndex++;

#endif


	//Tranform bit from event to table position
	//In case several bits are set, the highest bit is considered
	int evCount = 0;
	PxEvents_t evLocal;

	while (event)
	{
		evCount = 0;
		evLocal = event;


		//Bits are processed right to left
		//Get the lowest bit
		while ((evLocal & 1) != 1)
		{
			evLocal=evLocal>>1;
			evCount++;
		}

		//Clear the bit
		evLocal = 1<<(evCount);
		event = event & ~evLocal;

#ifdef STATETRACE
	debugTraceState[debugTraceIndex].command = 2;
	debugTraceState[debugTraceIndex].eventmask = evLocal;
	debugTraceState[debugTraceIndex].state = currentState;
	debugTraceIndex++;

#endif


		//Check for valid position in table
		uint16_t transitionPos = currentState*noEvents+evCount;
		if (transitionPos >= transitionTableSize)
		{
			//Todo: some proper error handling
			DET_stop(AUTOCORE, STATE_WRONGSTATE,1);
			continue;
		}


		//Get the correct entry from the transition table
		guard				= stateTransitionTable[transitionPos].guard;
		actionTransition	= stateTransitionTable[transitionPos].actionTransition;
		toState				= stateTransitionTable[transitionPos].toState;

#ifdef STATETRACE
	debugTraceState[debugTraceIndex].command = 3;
	debugTraceState[debugTraceIndex].eventmask = transitionPos;
	debugTraceState[debugTraceIndex].state = toState;
	debugTraceIndex++;

#endif



		//Check for valid position in table
		if (toState >= actionTableSize || currentState >= actionTableSize)
		{
			//Todo: some proper error handling
			DET_stop(AUTOCORE, STATE_WRONGSTATE,2);
			continue;
		}

		stateEntry			= stateActionTable[toState].actionEntry;
		stateExit			= stateActionTable[currentState].actionExit;

		//Check for guard (guard exists and return false
		if((guard!=(STATE_GuardPtr_t)0) && (!guard()))
		{
			continue; //Transition will not be executed, let's go to the next event
		}

		//Perform Exit action
		if ((currentState != toState) && stateExit != (STATE_ActionPtr_t)0)
		{
			stateExit();
		}

		//Perform transition action
		if (actionTransition != (STATE_ActionPtr_t)0)
		{
			actionTransition();
		}

		//Perform Entry action
		if ((currentState != toState) && stateEntry != (STATE_ActionPtr_t)0)
		{
			stateEntry();
		}

		//set new state
		currentState=toState;
	} //while


	//In the meantime, other parts of the signal might have got updated by the actions, so read the state again
	stateSignal = CARSTATE_get(&so_carstate);

	//Store new state in signal
	if (storageState == STATE_NORMAL)
	{
		stateSignal.state_normal = currentState;
	}
	else if (storageState == STATE_SAFETY)
	{
		stateSignal.state_safety = currentState;
	}
	else
	{
		//Todo: add error handling, e.g. DET_Stop
	}

#ifdef STATETRACE
	debugTraceState[debugTraceIndex].command = 4;
	debugTraceState[debugTraceIndex].eventmask = 0;
	debugTraceState[debugTraceIndex].state = currentState;
	debugTraceIndex++;

#endif


	CARSTATE_set(&so_carstate, stateSignal);

}
