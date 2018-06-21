/*
 * sync.h
 *
 *  Created on: May 28, 2014
 *      Author: fromm
 */

#ifndef SYNC_H_
#define SYNC_H_

#include "det.h"

/**
 * Synchronisation state of the system
 */
typedef enum {NOTHING_INIT, BASE_SYSTEM_INIT, TASKS_CREATED, TASKS_READY} SYNC_state_t;

/*
 * Set all states to NOTHING_INIT
 */
inline void SYNC_Init();

/*
 * Initialise Core n to a specific state
 * @param state Initialisation state for the core
 */
inline void SYNC_SetC0(SYNC_state_t state);

/*
 * Initialise Core n to a specific state
 * @param state Initialisation state for the core
 */
inline void SYNC_SetC1(SYNC_state_t state);

/*
 * Initialise Core n to a specific state
 * @param state Initialisation state for the core
 */
inline void SYNC_SetC2(SYNC_state_t state);




/**
 * Function to syncronise core 0 with core 1 and 2
 * Core0 will wait until core 1 and core 2 reach the given state (checkstate)
 * After reching this state, core 0 will also change to the given state
 * Error conditions are checked by providing a timeout counter
 * @callcore Core0
 * @param checkstate The state which is checked on the other core's
 * @param timeout The number of retries
 * @param errorcode User error code which will be stored in error memory
 *
 */
inline void SYNC_C0WaitForC1C2State(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode);



/**
 * Function to syncronise core 1 with core 0 and 2
 * Core1 will wait until core 0 and core 2 reach the given state (checkstate)
 * After reching this state, core 1 will also change to the given state
 * Error conditions are checked by providing a timeout counter
 * @callcore Core1
 * @param checkstate The state which is checked on the other core's
 * @param timeout The number of retries
 * @param errorcode User error code which will be stored in error memory
 *
 */
inline void SYNC_C1WaitForC0C2State(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode);



/**
 * Function to syncronise core 2 with core 0 and 1
 * Core 2 will wait until core 0 and core 1 reach the given state (checkstate)
 * After reaching this state, core 2 will also change to the given state
 * Error conditions are checked by providing a timeout counter
 * @callcore Core2
 * @param checkstate The state which is checked on the other core's
 * @param timeout The number of retries
 * @param errorcode User error code which will be stored in error memory
 *
 */
inline void SYNC_C2WaitForC0C1State(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode);



/**
 * Function to syncronise core 1 and 2 with core 0
 * Core 1 and 2 will wait until core 0 reach the given state (checkstate)
 * After reaching this state, core 1 and 2 will also change to the given state
 * Error conditions are checked by providing a timeout counter
 * @callcore Core 1 and 2
 * @param checkstate The state which is checked on the other core's
 * @param timeout The number of retries
 * @param errorcode User error code which will be stored in error memory
 *
 */
inline void SYNC_C12WaitForC0State(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode);


/**
 * Function to syncronise all core.
 * Checkstate of own core has to be set before this call
 * Error conditions are checked by providing a timeout counter
 * @callcore all
 * @param checkstate The state which is checked on the other core's
 * @param timeout The number of retries
 * @param errorcode User error code which will be stored in error memory
 *
 */
inline void SYNC_WaitForState(SYNC_state_t checkstate, unsigned int timeout, DET_message_t errorcode);

#endif /* SYNC_H_ */
