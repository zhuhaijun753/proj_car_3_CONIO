/*************************************
 * Task_C1_Init.h
 * Author: 			Fromm
 * Description: 	Initialisation Task of Core 1
 * Task creation: 	
 */

#ifndef TASK_C1_INIT_CREATE_1_H_
#define TASK_C1_INIT_CREATE_1_H_


#include "pxdef.h"

/*******************************************************************************************************
 * Core 1 specific defines
 *******************************************************************************************************/

#define NUM_OF_PXOBJS_CORE1  			50				//Number of objects on core 1
//#define PXROS_NAMESIZE_CORE1			20				//Not needed anymore


#define SYSMEMSIZE_CORE1				12000			//System memory for the TCB's, 400 byte per Task
#define TASKMEMSIZE_CORE1				2000			//Task memory for the message objects (sender)

#define TASK_C1_INIT_STACKSIZE_CORE1	200				//Task stack size
#define TASK_C1_INIT_INTSTACKSIZE_CORE1	32				//Task interrupt stack size

#define MINPRIO_CORE1        			31				//Task priority after application task start

/*******************************************************************************************************
 * Events
 ******************************************************************************************************/




/*******************************************************************************************************
 * Core 1 InitTask specification record
 *******************************************************************************************************/
extern const PxTaskSpec_T InitTaskSpec_CORE1;


/*******************************************************************************************************
 * Core 1 init task
 *******************************************************************************************************/
extern void Task_C1_Init_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);


#endif /* TASK_C1_INIT_CREATE_1_H_ */
