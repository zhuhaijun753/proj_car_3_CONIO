/*
 * cyclic_runnable_table.h
 *
 *  Created on: May 4, 2016
 *      Author: Shriki
 */

#ifndef RUNNABLES_CYCLIC_RUNNABLE_TABLE_H_
#define RUNNABLES_CYCLIC_RUNNABLE_TABLE_H_

#include "register.h"
#include "pxdef.h"


#include "global.h"
#include "system.h"

#include "runnable_table_type.h"
#include "rte_time.h"
#include "rte_time_type.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************/
/*------------------------- Global defines -----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------- Global functions ---------------------------------*/
/******************************************************************************/
/**
 * @\func	: RT_initRunExecute()
 * @\brief	: Calls the Init runnables from the init table in sequence
 */
void RT_initRunExecute(const RT_initTable_t* pTableInit, uint16_t tableSize);

/**
 * This function is used for all event triggered runnables
 *
 */
/**
 * @\func	: RT_eventRunExecute()
 * @\brief	: Calls the runnables from the table in if the corresponding event has been fired
 */
RT_safety_t RT_eventRunSafetyExecute(const RT_eventSafetyTable_t*  pTableEvent, uint16_t tableSize, uint32_t eventMask, PxTask_t stateTaskId, PxTask_t senderId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex);

/**
 * @\func	: RT_cyclicRunExecute()
 * @\brief	: Calls the runnables from the table in sequence and also measures the timing parameters if enabled.
 */
void RT_cyclicRunExecute(const RT_cyclicTable_t*  pTableCyclic, uint16_t tableSize, uint32_t taskTime, uint32_t cycleTime,
		PxTask_t taskId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex);

/**
 * @\func	: RT_cyclicRunExecute()
 * @\brief	: Calls the runnables from the table in sequence if corresponding events are fired and also measures the timing parameters if enabled.
 */
void RT_eventRunExecute(const RT_eventTable_t*  pTableEvent, uint16_t tableSize, uint32_t eventMask, PxTask_t senderId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex);

/**
 * @\func	: RT_cyclicRunSafetyExecute()
 * @\brief	: Calls the runnables from the table in sequence and also measures the timing parameters if enabled.
 * @\detail	: this is specially made for safety runnables as the return values of runnables are different.
 */
RT_safety_t RT_cyclicRunSafetyExecute(const RT_cyclicSafetyTable_t*  pTableCyclic, uint16_t tableSize, uint32_t taskTime, uint32_t cycleTime,
		PxTask_t CallertaskId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex, PxTask_t stateTaskId);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RUNNABLES_CYCLIC_RUNNABLE_TABLE_H_ */
