/*************************************
 * Task_C2_Init.h
 * Author: 			Fromm
 * Description: 	Initialisation Task of Core 2
 * Task creation: 	
 */

#ifndef TASK_C2_INIT_CREATE_2_H_
#define TASK_C2_INIT_CREATE_2_H_


#include "pxdef.h"

/*******************************************************************************************************
 * Core 2 specific defines
 *******************************************************************************************************/

#define NUM_OF_PXOBJS_CORE2  			50			//Number of objects
//#define PXROS_NAMESIZE_CORE2			20			//Not needed anymore


#define SYSMEMSIZE_CORE2				12000		//System memory for TCB's, 400 Byte per Task
#define TASKMEMSIZE_CORE2				2000		//Task memory for message objects

#define TASK_C2_INIT_STACKSIZE_CORE2	200			//Stack size of init task
#define TASK_C2_INIT_INTSTACKSIZE_CORE2	32			//Intterupt stack size of init task

#define MINPRIO_CORE2        			31			//Task priority after start of application tasks

/*******************************************************************************************************
 * Events
 ******************************************************************************************************/




/*******************************************************************************************************
 * Core 2 InitTask specification record
 *******************************************************************************************************/
extern const PxTaskSpec_T InitTaskSpec_CORE2;


/*******************************************************************************************************
 * Core 2 init task
 *******************************************************************************************************/
extern void Task_C2_Init_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);


#endif /* TASK_C2_INIT_CREATE_2_H_ */
