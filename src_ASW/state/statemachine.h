/*
 * statemachine.h
 *
 *  Created on: 16.06.2016
 *      Author: Fromm
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "global.h"
#include "statemachine_config.h"
#include "pxdef.h"

/**
 * @brief initialisation of state machine data
 */
void STATE_init();

/**
 * @brief Calls the normal state machine
 * @param eventMask 	eventmask which will be processed
 */
void STATE_normal_processEvent(PxEvents_t eventMask);


/**
 * Calls the safety state machine
 * @param eventMask: events which will be processed
 */
void STATE_safety_processEvent(PxEvents_t eventMask);

#endif /* STATEMACHINE_H_ */
