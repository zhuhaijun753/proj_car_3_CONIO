/*
 * rte_time.h
 *
 *  Created on: May 4, 2016
 *      Author: Shriki
 */

#ifndef RTE_TIME_H_
#define RTE_TIME_H_

#include "register.h"
#include "global.h"
#include "system.h"
#include "pxdef.h"
#include "rte_time_type.h"
#include "rte_time_config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RTE_TIME_FULL_PROTOCOL_SIZE	(5)

/**
 * @\def	: typedef enum RTE_TIME_taskRingBufferIndex_t
 * @\brief	: Used by application tasks.
 * @\detail	: Tasks calls the runnable execution function with this parameter,
 * 			  to access its own ring buffer from an tasks Ring buffer table
 */
typedef enum {
	eRTE_TIME_taskC0CommunicationTimings_idx = 0, eRTE_TIME_taskC0SafetyInputTimings_idx = 1,
	eRTE_TIME_taskC0SafetyLogicTimings_idx   = 2, eRTE_TIME_taskC1ControlTimings_idx     = 3,
	eRTE_TIME_taskC2TFTTimings_idx = 4
} RTE_TIME_taskRingBufferIndex_t;

/**
 * @\var	: RTE_TIME_appTaskInitDetails
 * @\type	: RTE_TIME_initData_t
 * @\brief	: 2D Array to store the tasks name and its ID.
 */
extern RTE_TIME_initData_t RTE_TIME_appTaskInitDetails[RTE_MAX_CORES][RTE_MAX_TASKS_PER_CORE];

/**
 * @\var	: RTE_TIME_noOfAppTasksInCore0
 * @\type	: uint8_t
 * @\brief	: Has the no.of application tasks created in core0 init task.
 */
extern uint8_t RTE_TIME_noOfAppTasksInCore0;

/**
 * @\var	: RTE_TIME_noOfAppTasksInCore1
 * @\type	: uint8_t
 * @\brief	: Has the no.of application tasks created in core1 init task.
 */
extern uint8_t RTE_TIME_noOfAppTasksInCore1;

/**
 * @\var	: RTE_TIME_noOfAppTasksInCore2
 * @\type	: uint8_t
 * @\brief	: Has the no.of application tasks created in core2 init task.
 */
extern uint8_t RTE_TIME_noOfAppTasksInCore2;

/**
 * @\var	: RTE_TIME_comCommand
 * @\type	: uint8_t
 * @\brief	: Has the no.of application tasks created in core2 init task.
 */
extern uint8_t RTE_TIME_comCommand ;

/**
 * @\var	: RTE_TIME_fillDataFlag
 * @\type	: uint8_t
 * @\brief	: This Flag is used to start storing the time measurement data.
 */
extern boolean_t RTE_TIME_fillDataFlag;

/**
 * @\func	:	uint32_t RTE_GetTime(void)
 * @\brief	: 	Read the Current timer value and return with MSB ticks
 * @\params	:	void
 * @\return	:	uint32_t
 */
uint32_t RTE_TIME_GetTime(void);

/**
 * @\func	: void RTE_TIME_MsbTicksIncr(void);
 * @\brief	: Callback function for RTE TIME measurement. GPT T3 ISR callback.
 */
void RTE_TIME_MsbTicksIncr(void);

/**
 * @\func	: void RTE_TIME_TimerInit(void);
 * @\brief	: Initializes the GPT T3 as a base timer for RTE TIME measurements.
 */
void RTE_TIME_TimerInit(void);

/**
 * @\func	: void RTE_TIME_SendOutRteData(void);
 * @\brief	: Callback function for RTE TIME measurement. GPT T3 ISR callback.
 */
void RTE_TIME_SendOutRteData(void);

/**
 * @\func	: void RTE_TIME_InitDataTx(void);
 * @\brief	: Sends out Tasks Details on UARTUSB port. Called by the GUI command as part of boot up function.
 */
void RTE_TIME_InitDataTx(void);

/**
 * @\func	: void RTE_TIME_fillTaskStartInfo(...);
 * @\brief	: Stores the Task start time with its ID, Core Id etc.
 */
void RTE_TIME_fillTaskStartInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index);

/**
 * @\func	: void RTE_TIME_fillTaskEndInfo(...);
 * @\brief	: Stores the Task end time with its ID, Core Id etc.
 */
void RTE_TIME_fillTaskEndInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index);

/**
 * @\func	: void RTE_TIME_fillRunnableStartInfo(...);
 * @\brief	: Stores the Runnable start time with its ID, Core Id etc.
 */
void RTE_TIME_fillRunnableStartInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index);

/**
 * @\func	: void RTE_TIME_fillRunnableEndInfo(...);
 * @\brief	: Stores the Runnable end time with its ID, Core Id etc.
 */
void RTE_TIME_fillRunnableEndInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index);

/**
 * @\func	: void RTE_TIME_checkAndSetComCommand(uint8_t *pProtocolBuffer);
 * @\brief	: validates the received data and sets the command.
 */
void RTE_TIME_checkAndSetComCommand(uint8_t *pProtocolBuffer);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RUNNABLES_CYCLIC_RUNNABLE_TABLE_H_ */
