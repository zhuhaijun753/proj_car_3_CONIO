/*
 * Task_Init.h
 *
 *  Aggregates the 3 individual task init files
 *
 *  Created on: Dec 18, 2014
 *      Author: fromm
 */

#ifndef TASK_INIT_H_
#define TASK_INIT_H_

#include "Task_C0_Init.h"
#include "Task_C1_Init.h"
#include "Task_C2_Init.h"

/*******************************************************************************************************
 * Task specific types
 *******************************************************************************************************/

typedef struct
{
	PxTask_t taskId;	//Task Id, required for sending event
	PxMbx_t  mbxId;		//Mailbox Id, required for sending messages
} taskinfo_t;

/*******************************************************************************************************
 * System wide defines
 *******************************************************************************************************/

#define NAMESRV_PRIO_CORE0 20			//Priority of the name server task

/*******************************************************************************************************
 * Init Specification
 *******************************************************************************************************/
extern const PxInitSpec_T InitSpec[3];
extern const PxInitSpecsArray_t InitSpecsArray;

/*******************************************************************************************************
 * Global external symbols
 *******************************************************************************************************/
extern PxUInt_t	  GLOBAL_SYSTEM_RAM_BEGIN[];
extern PxUInt_t	  GLOBAL_SYSTEM_RAM_END[];


#endif /* TASK_INIT_H_ */
