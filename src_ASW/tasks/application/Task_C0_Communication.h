/**
* \file Task_C0_Communication.h
* \author P. Fromm
* \date 16.08.2016
*
* \brief Task_C0_Communication
*
* This task receives all communication (CAN, UART) through ISR's and distributes the data to the corresponding signallayer objects
*
* \note <notes>
* \todo LIDAR communication to be added
* \todo CAN communication to be checked
* \warning <warnings, e.g. dependencies, order of execution etc.>
*
*  Changelog:\n
*  - 1.0; 16.08.2016; P. Fromm
*            - Initial release
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


#ifndef _TASK_C0_COMMUNICATION_H_
#define _TASK_C0_COMMUNICATION_H_

#include "pxdef.h"

/**
 * The C API of the task.....
 */

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */


/*****************************************************************************************************
 * Global defines, NameserverId and Priority
 *****************************************************************************************************/

#define TASK_C0_COMMUNICATION_NAMESERVERID        ((PxNameId_t){2,0,3,0})	/**< Name identifier */
#define TASK_C0_COMMUNICATION_PRIO                 21 						/**< Task Priority (31 lowest) */


/*****************************************************************************************************
 * API function for creating the task
 *****************************************************************************************************/

/**
 * Get the PXROS task id. This id is required for all task services.
 * \return				task id
 */
extern PxTask_t Task_C0_Communication_getTaskId();

/**
 * The task creation and initialisation function. Once the task has been created, it can be started by firing the configured activation event
 * \param prio 			task priority (31 lowest)
 * \param actEvents		activation event mask
 * \return				task id
 */
extern PxTask_t Task_C0_Communication_Create(PxUChar_t prio, PxEvents_t actevents);

#ifdef __cplusplus
  }
#endif /* __cplusplus */


#endif // _TASK_C0_COMMUNICATION_H_
