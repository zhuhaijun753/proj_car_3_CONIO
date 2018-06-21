/*************************************
 * Task_C1_Control.h
 * Author: 			Fromm
 * Description: 	Control Logic Task
 * Task creation: 	29.04.2016 18:22:21
 */

/*****************************************************************************************************
 * Includes
 *****************************************************************************************************/
//System
#include "pxdef.h"
#include "shared.h"
#include "global.h"
#include "symbols.h"
#include "SystemEvents.h"
#include "signal_bb.h"

//Communication Stacks
#include "can.h"
#include "CANopen.h"

//Application
#include "swc_control.h"
#include "runnable_table.h"
#include "runnable_table_config.h"

//Own header
#include "Task_C1_Control.h"

//Drivers
#include "Gpt_timer.h"

//Messaging
#include "msg_dispatcher.h"
#include "msg_config.h"



/*****************************************************************************************************
 * Local defines, memory sizes etc
 *****************************************************************************************************/


#define TASK_C1_CONTROL_STACKSIZE           400
#define TASK_C1_CONTROL_INTR_STACKSIZE      50
#define TASK_C1_CONTROL_CycleTime 			10


/*****************************************************************************************************
 * Task class declaration
 *****************************************************************************************************/

class CTask_C1_Control {
    PxStackAligned_t InterruptStack[TASK_C1_CONTROL_INTR_STACKSIZE];
    PxStackAligned_t Stack[TASK_C1_CONTROL_STACKSIZE];

	// add 6 dummy bytes to avoid protection disruption by using "st.d" at the end of the data area
	// normaly an access to this pad bytes will lead to an protection fault
	// this 6 byte have to be always the last elements in the data area of a task
	// don't at any members behind this protection pad
    PxChar_t	__protectionPad[6] __attribute__ ((aligned(8)));

public:
	// The task function with the application code
    void Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);

	//The task create function (C-style)
	friend PxTask_t Task_C1_Control_Create(PxUChar_t prio, PxEvents_t actevents);

} __attribute__ ((aligned(64)));

/*****************************************************************************************************
 * Task object(s)
 *****************************************************************************************************/

#pragma section ".Task_C1_Control"
CTask_C1_Control Task_C1_Control_obj;
#pragma section

/*****************************************************************************************************
 * Local storage of task id. Faster and more reliable than nameserver
 *****************************************************************************************************/

#pragma section ".Task_Init_Lock.data" awB 8
static PxTask_t Task_C1_Control_id={0};
#pragma section

 
PxTask_t Task_C1_Control_getTaskId()
{
	return Task_C1_Control_id;
}


/*****************************************************************************************************
 * Create function
 *****************************************************************************************************/

		
static const PxProtectRegion_T Task_C1_Control_Regions[] = {
		
#ifdef RTE_TIMING_ANALYSIS
	//GPT timer peripheral register read access
	{ (PxUInt_t) DRV_GPT_BEGIN, (PxUInt_t) DRV_GPT_END, (PxProtectType_t)ReadProtection},

#endif //RTE_TIMING_ANALYSIS

	//Required for DET, TRAP,...
	{ (PxUInt_t) __GLOBAL_TASK_BEGIN, (PxUInt_t) __GLOBAL_TASK_END, (PxProtectType_t)WRProtection},

	//The RTE ring buffer for the tasks belongs to Core 1 is in this region, Canopen data dictionary etc.
	{ (PxUInt_t) __CPU1_DATAPOOL_BEGIN, (PxUInt_t) __CPU1_DATAPOOL_END, (PxProtectType_t)WRProtection},

	//Signal Layer Control Signals
	{ (PxUInt_t) __CPU1_SL_CONTROL_BEGIN, (PxUInt_t) __CPU1_SL_CONTROL_END, (PxProtectType_t)WRProtection},

	//Signal Layer Safety Signals
	//TODO Temporary Write access to allow messaging test
	{ (PxUInt_t) __CPU0_SL_SAFETY_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_END, (PxProtectType_t)WRProtection},

	//Signal Layer Safety State Signals
	{ (PxUInt_t) __CPU0_SL_SAFETY_STATE_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_STATE_END, (PxProtectType_t)ReadProtection},

	//Stores data which is created by the Inittask, e.g. task and message Id's
	{ (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN, (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_END, (PxProtectType_t)ReadProtection},

	//Library functions for math
	{ (PxUInt_t) LIBDNK_DATA_BASE, (PxUInt_t) LIBDNK_DATA_END, (PxProtectType_t)WRProtection},

	//End of List
		{0,0,(PxProtectType_t)0}



};


void Task_C1_Control_Entry(PxTask_t myId, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
        Task_C1_Control_obj.Task_Func(myId,myMailbox,myActivationEvents);
}


extern "C" {
PxTask_t Task_C1_Control_Create(PxUChar_t prio,PxEvents_t events)
{

	PxTaskContext_T task_Context;
	PxTaskSpec_T task_Spec;
	PxBzero((PxUChar_t *)&task_Spec,sizeof(task_Spec));

	//Protection [0] = 0 --> Inherited from the Owner.
	task_Context.protection[0].lowerBound 	= 0,
	task_Context.protection[0].upperBound 	= 0,
	task_Context.protection[0].prot 		= NoAccessProtection;

	//Task stack memory
	task_Context.protection[1].lowerBound 	= (PxUInt_t)&Task_C1_Control_obj,
	task_Context.protection[1].upperBound 	= (PxUInt_t)&Task_C1_Control_obj.__protectionPad[0],
	task_Context.protection[1].prot 		= WRProtection;

	task_Spec.ts_name						= (const PxChar_t*)"Task_C1_Control",
	task_Spec.ts_fun						= Task_C1_Control_Entry,
	task_Spec.ts_mc							= PXMcTaskdefault,
	task_Spec.ts_opool						= PXOpoolTaskdefault,
#ifdef RTE_TIMING_ANALYSIS
	task_Spec.ts_privileges					= PXUser1Privilege,
#else
	task_Spec.ts_privileges					= PXUser0Privilege,
#endif //RTE_TIMING_ANALYSIS
	task_Spec.ts_accessrights				= 0| PXACCESS_HANDLERS| PXACCESS_INSTALL_HANDLERS| PXACCESS_INSTALL_SERVICES| PXACCESS_REGISTERS| PXACCESS_SYSTEMDEFAULT| PXACCESS_RESOURCES| PXACCESS_NEW_RESOURCES| PXACCESS_SYSTEM_CONTROL| PXACCESS_MODEBITS| PXACCESS_OVERRIDE_ABORT_EVENTS,
	task_Spec.ts_context					= &task_Context,
	task_Spec.ts_protect_region  			= Task_C1_Control_Regions,
	task_Spec.ts_taskstack.stk_type 		= PXStackFall,
	task_Spec.ts_taskstack.stk_size 		= PXStackDontCheck,
	
	task_Spec.ts_taskstack.stk_src.stk 		= &Task_C1_Control_obj.Stack[TASK_C1_CONTROL_STACKSIZE],
	task_Spec.ts_inttaskstack.stk_type 		= PXStackFall;
	task_Spec.ts_inttaskstack.stk_size 		= PXStackDontCheck;
	task_Spec.ts_inttaskstack.stk_src.stk 	= &Task_C1_Control_obj.InterruptStack[TASK_C1_CONTROL_INTR_STACKSIZE];
	task_Spec.ts_abortstacksize				= 0;

	Task_C1_Control_id = PxTaskCreate(PXOpoolTaskdefault,&task_Spec,prio,events);
    return Task_C1_Control_id;
	
}
} //extern C

/*****************************************************************************************************
 * Task Function
 *****************************************************************************************************/


 
void CTask_C1_Control::Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	// Turn off warning in case parameters are not used
	(void) myID;
	(void) myMailbox;
	(void) myActivationEvents;

	PxMsgEvent_t EveMsg = {0};
	volatile uint32_t count = 0;

	//Initialise the control signal layer signals
	SL_CONTROL_init();


	//Generate a periodic event handler
	PxPe_t pe = PxPeRequest(PXOpoolTaskdefault, TASK_C1_CONTROL_CycleTime, ev_Task_C1_Control_0_1ms);
	PxPeStart(pe);

	//Call all init function
	RT_initRunExecute(RT_C1_control_initRunTable, RT_C1_control_initRunTable_size);

	while (1)
	{

		EveMsg = PxMsgReceive_EvWait(myMailbox,
				ev_Task_C1_Control_0_1ms);


		//============================================= Call message handler
		if (PxMsgIdIsValid(EveMsg.msg))
		{
				RC_t result = MSG_receive(&EveMsg.msg, MSG_portList_C1_Control, MSG_portList_C1_Control_size);
		}

		//============================================= Call cyclic runnables
		RT_cyclicRunExecute(RT_C1_control_cyclicRunTable, RT_C1_control_cyclicRunTable_size,
								count, TASK_C1_CONTROL_CycleTime,
								myID,eRTE_TIME_taskC1ControlTimings_idx);
		//Increment time base
		count++;
	}
}

