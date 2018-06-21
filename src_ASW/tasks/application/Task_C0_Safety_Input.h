/*************************************
 * Task_C0_Safety_Input.h
 * Author: 			Fromm
 * Description: 	Safety Input Monitor
 * Task creation: 	
 */


#ifndef _TASK_C0_SAFETY_INPUT_H_
#define _TASK_C0_SAFETY_INPUT_H_

#include "pxdef.h"

/**
 * The C API of the task.....
 */

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */


/*****************************************************************************************************
 * Global defines, NameserverId and Events
 *****************************************************************************************************/

#define TASK_C0_SAFETY_INPUT_NAMESERVERID        ((PxNameId_t){2,0,1,0})	/**< Name identifier */
#define TASK_C0_SAFETY_INPUT_PRIO                 25						/**< Task Priority (31 lowest) */




/*****************************************************************************************************
 * API function for creating the task
 *****************************************************************************************************/

/**
 * Get the PXROS task id. This id is required for all task services.
 * \return				task id
 */
extern PxTask_t Task_C0_Safety_Input_getTaskId();

/**
 * The task creation and initialisation function. Once the task has been created, it can be started by firing the configured activation event
 * \param prio 			task priority (31 lowest)
 * \param actEvents		activation event mask
 * \return				task id
 */
extern PxTask_t Task_C0_Safety_Input_Create(PxUChar_t prio, PxEvents_t actevents);


#ifdef __cplusplus
  }
#endif /* __cplusplus */


#endif // _TASK_C0_SAFETY_INPUT_H_
