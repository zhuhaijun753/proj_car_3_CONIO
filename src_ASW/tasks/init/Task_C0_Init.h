/*************************************
 * Task_C0_Init.h
 * Author: 			Fromm
 * Description: 	Initialisation Task of Core 0
 * Task creation: 	
 */

#ifndef TASK_C0_INIT_CREATE_0_H_
#define TASK_C0_INIT_CREATE_0_H_


#include "pxdef.h"

/*******************************************************************************************************
 * Core 0 specific defines
 *******************************************************************************************************/

#define NUM_OF_PXOBJS_CORE0  			50			//Max number of objects
//#define PXROS_NAMESIZE_CORE0			20			//Not required anymore?


#define SYSMEMSIZE_CORE0				12000		//Memory for the TCB's, 400Byte per Task
#define TASKMEMSIZE_CORE0				2000		//Memory for the message objects (sender)

#define TASK_C0_INIT_STACKSIZE_CORE0	200			//Task stack size
#define TASK_C0_INIT_INTSTACKSIZE_CORE0	32			//Task interrupt stack size

#define MINPRIO_CORE0        			31			//Priority after all tasks have been initialized

/*******************************************************************************************************
 * Events
 ******************************************************************************************************/




/*******************************************************************************************************
 * Core 0 InitTask specification record
 *******************************************************************************************************/
extern const PxTaskSpec_T InitTaskSpec_CORE0;


/*******************************************************************************************************
 * Core 0 init task
 *******************************************************************************************************/
extern void Task_C0_Init_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);


#endif /* TASK_C0_INIT_CREATE_0_H_ */
