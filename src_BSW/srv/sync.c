/*
 * sync.c
 *
 *  Created on: May 28, 2014
 *      Author: fromm
 */

#include "register.h"
#include <machine/intrinsics.h>

#include "sync.h"
#include "det.h"



#pragma section ".SYNC.Task_Sync.data" 8

static volatile SYNC_state_t SYNC__Core0_Initialized = NOTHING_INIT;
static volatile SYNC_state_t SYNC__Core1_Initialized = NOTHING_INIT;
static volatile SYNC_state_t SYNC__Core2_Initialized = NOTHING_INIT;

volatile unsigned int SYNC___protection3;
volatile unsigned int SYNC___protection4;

#pragma section

#pragma section ".sync.text"

inline void SYNC_Init()
{
	//Needed in case of missing initialisation through copy table
	SYNC__Core0_Initialized = NOTHING_INIT;
	SYNC__Core1_Initialized = NOTHING_INIT;
	SYNC__Core2_Initialized = NOTHING_INIT;

}

inline void SYNC_SetC0(SYNC_state_t state)
{
	SYNC__Core0_Initialized = state;
}

inline void SYNC_SetC1(SYNC_state_t state)
{
	SYNC__Core1_Initialized = state;
}

inline void SYNC_SetC2(SYNC_state_t state)
{
	SYNC__Core2_Initialized = state;
}


inline void SYNC_C0WaitForC1C2State(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode)
{
    volatile unsigned int counter = 0;

    SYNC__Core0_Initialized = checkstate;

    while ( (SYNC__Core1_Initialized < checkstate) || (SYNC__Core2_Initialized < checkstate))
	{
		if (counter++ > timeout) DET_stop(__MFCR(CPU_CORE_ID), errorcode, timeout);
	}
}


inline void SYNC_C1WaitForC0C2State(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode)
{
    volatile unsigned int counter = 0;

    SYNC__Core1_Initialized = checkstate;

    while ( (SYNC__Core0_Initialized < checkstate) || (SYNC__Core2_Initialized < checkstate))
	{
		if (counter++ > timeout) DET_stop(__MFCR(CPU_CORE_ID), errorcode, timeout);
	}

}

inline void SYNC_C2WaitForC0C1State(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode)
{
    volatile unsigned int counter = 0;

    SYNC__Core2_Initialized = checkstate;

    while ( (SYNC__Core0_Initialized < checkstate) || (SYNC__Core1_Initialized < checkstate))
	{
		if (counter++ > timeout) DET_stop(__MFCR(CPU_CORE_ID), errorcode, timeout);
	}

}

//================================================

inline void SYNC_C12WaitForC0State(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode)
{
    volatile unsigned int counter = 0;

    while (SYNC__Core0_Initialized < checkstate)
	{
		if (counter++ > timeout) DET_stop(__MFCR(CPU_CORE_ID), errorcode, checkstate);
	}

}


//================================================

inline void SYNC_WaitForState(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode)
{
    volatile unsigned int counter = 0;

    while ((SYNC__Core0_Initialized < checkstate) ||
    	   (SYNC__Core1_Initialized < checkstate) ||
    	   (SYNC__Core2_Initialized < checkstate))
	{
		if (counter++ > timeout) DET_stop(__MFCR(CPU_CORE_ID), errorcode, checkstate);

	}

}

#pragma section
