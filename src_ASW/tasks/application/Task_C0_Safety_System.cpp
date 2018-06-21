/*************************************
 * Task_C0_Safety_System.h
 * Author: 			Fromm
 * Description: 	Safety Input Monitor
 * Task creation: 	02.01.2017 22:19:10
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

//Application
#include "swc_engine.h"

//Communication Stacks
#include "CANopen_Client.h"


//Own header
#include "Task_C0_Safety_System.h"

//Drivers
#include "Gpt_timer.h"


/*****************************************************************************************************
 * Local defines, memory sizes etc
 *****************************************************************************************************/


#define TASK_C0_SAFETY_SYSTEM_STACKSIZE           400
#define TASK_C0_SAFETY_SYSTEM_INTR_STACKSIZE      50
#define TASK_C0_SAFETY_SYSTEM_CycleTime 			1


/*****************************************************************************************************
 * Task class declaration
 *****************************************************************************************************/

class CTask_C0_Safety_System {
    PxStackAligned_t InterruptStack[TASK_C0_SAFETY_SYSTEM_INTR_STACKSIZE];
    PxStackAligned_t Stack[TASK_C0_SAFETY_SYSTEM_STACKSIZE];

	// add 6 dummy bytes to avoid protection disruption by using "st.d" at the end of the data area
	// normaly an access to this pad bytes will lead to an protection fault
	// this 6 byte have to be always the last elements in the data area of a task
	// don't at any members behind this protection pad
    PxChar_t	__protectionPad[6] __attribute__ ((aligned(8)));

public:
	// The task function with the application code
    void Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);

	//The task create function (C-style)
	friend PxTask_t Task_C0_Safety_System_Create(PxUChar_t prio, PxEvents_t actevents);

} __attribute__ ((aligned(64)));

/*****************************************************************************************************
 * Task object(s)
 *****************************************************************************************************/

#pragma section ".Task_C0_Safety_System"
	CTask_C0_Safety_System Task_C0_Safety_System_obj;
#pragma section

/*****************************************************************************************************
 * Local storage of task id. Faster and more reliable than nameserver
 *****************************************************************************************************/

#pragma section ".Task_Init_Lock.data" awB 8
	static PxTask_t Task_C0_Safety_System_id={0};
#pragma section

 
PxTask_t Task_C0_Safety_System_getTaskId()
{
	return Task_C0_Safety_System_id;
}


/*****************************************************************************************************
 * Create function
 *****************************************************************************************************/
 
		
static const PxProtectRegion_T Task_C0_Safety_System_Regions[] = {

#ifdef RTE_TIMING_ANALYSIS
	//GPT timer peripheral register read access
	{ (PxUInt_t) DRV_GPT_BEGIN, (PxUInt_t) DRV_GPT_END, (PxProtectType_t)ReadProtection},

#endif //RTE_TIMING_ANALYSIS


	//Todo: Temporary access to CAN driver, as emergency stop through SmartPower is not integrated yet.
	//CAN Module for CanOpen
	{ (PxUInt_t) MP_CAN_M_BEGIN, (PxUInt_t) MP_CAN_M_END, (PxProtectType_t)WRProtection},

	//Signal Layer Safety Signals, Write access required to increment the age
	{ (PxUInt_t) __CPU0_SL_SAFETY_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_END, (PxProtectType_t)WRProtection},

	//Signal Layer Safety State Signals, Write access required to increment the age
	{ (PxUInt_t) __CPU0_SL_SAFETY_STATE_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_STATE_END, (PxProtectType_t)WRProtection},

	//Signal Layer Control Signals, Write access required to increment the age
	{ (PxUInt_t) __CPU1_SL_CONTROL_BEGIN, (PxUInt_t) __CPU1_SL_CONTROL_END, (PxProtectType_t)WRProtection},

	//Required for DET, TRAP,...
	{ (PxUInt_t) __GLOBAL_TASK_BEGIN, (PxUInt_t) __GLOBAL_TASK_END, (PxProtectType_t)WRProtection},

	//Stores global data from UART and CAN
	{ (PxUInt_t) __CPU0_DATAPOOL_BEGIN, (PxUInt_t) __CPU0_DATAPOOL_END, (PxProtectType_t)WRProtection},


	//Stores data which is created by the Inittask, e.g. task and message Id's
	{ (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN, (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_END, (PxProtectType_t)ReadProtection},

	//End of List
	{0,0,(PxProtectType_t)0}
};


void Task_C0_Safety_System_Entry(PxTask_t myId, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
        Task_C0_Safety_System_obj.Task_Func(myId,myMailbox,myActivationEvents);
}


extern "C" {
PxTask_t Task_C0_Safety_System_Create(PxUChar_t prio,PxEvents_t events)
{

	PxTaskContext_T task_Context;
	PxTaskSpec_T task_Spec;
	PxBzero((PxUChar_t *)&task_Spec,sizeof(task_Spec));

	//Protection [0] = 0 --> Inherited from the Owner.
	task_Context.protection[0].lowerBound 	= 0,
	task_Context.protection[0].upperBound 	= 0,
	task_Context.protection[0].prot 		= NoAccessProtection;

	//Task stack memory
	task_Context.protection[1].lowerBound 	= (PxUInt_t)&Task_C0_Safety_System_obj,
	task_Context.protection[1].upperBound 	= (PxUInt_t)&Task_C0_Safety_System_obj.__protectionPad[0],
	task_Context.protection[1].prot 		= WRProtection;

	task_Spec.ts_name						= (const PxChar_t*)"Task_C0_Safety_System",
	task_Spec.ts_fun						= Task_C0_Safety_System_Entry,
	task_Spec.ts_mc							= PXMcTaskdefault,
	task_Spec.ts_opool						= PXOpoolTaskdefault,
	task_Spec.ts_privileges					= PXUser1Privilege,
	task_Spec.ts_accessrights				= 0| PXACCESS_HANDLERS| PXACCESS_INSTALL_HANDLERS| PXACCESS_INSTALL_SERVICES| PXACCESS_REGISTERS| PXACCESS_SYSTEMDEFAULT| PXACCESS_RESOURCES| PXACCESS_NEW_RESOURCES| PXACCESS_SYSTEM_CONTROL| PXACCESS_MODEBITS| PXACCESS_OVERRIDE_ABORT_EVENTS,
	task_Spec.ts_context					= &task_Context,
	task_Spec.ts_protect_region  			= Task_C0_Safety_System_Regions,
	task_Spec.ts_taskstack.stk_type 		= PXStackFall,
	task_Spec.ts_taskstack.stk_size 		= PXStackDontCheck,
	
	task_Spec.ts_taskstack.stk_src.stk 		= &Task_C0_Safety_System_obj.Stack[TASK_C0_SAFETY_SYSTEM_STACKSIZE],
	task_Spec.ts_inttaskstack.stk_type 		= PXStackFall;
	task_Spec.ts_inttaskstack.stk_size 		= PXStackDontCheck;
	task_Spec.ts_inttaskstack.stk_src.stk 	= &Task_C0_Safety_System_obj.InterruptStack[TASK_C0_SAFETY_SYSTEM_INTR_STACKSIZE];
	task_Spec.ts_abortstacksize				= 0;

	Task_C0_Safety_System_id = PxTaskCreate(PXOpoolTaskdefault,&task_Spec,prio,events);
    return Task_C0_Safety_System_id;
	
}
} //extern C

/*****************************************************************************************************
 * Task Function
 *****************************************************************************************************/

void CTask_C0_Safety_System::Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	// Turn off warning in case parameters are not used
	(void) myID;
	(void) myMailbox;
	(void) myActivationEvents;

	PxEvents_t eventMask = 0;

	PxUInt32_t count = 0;

	//Generate a periodic event handler
	PxPe_t pe = PxPeRequest(PXOpoolTaskdefault, TASK_C0_SAFETY_SYSTEM_CycleTime, ev_Task_C0_Safety_System_0_1ms);
	PxPeStart(pe);

	while (1)
	{
		eventMask = PxAwaitEvents(ev_Task_C0_Safety_System_0_1ms);

		count++;

		//============================================ Turn of engine in case of fatal error
		if  (DET_ShutDownEngines() == TRUE)
		{
			ENGINE_run_powerOff();
		}

		//============================================ Every 10ms perform timertick for the signal age
		if (count == 10)
		{
			count = 0;
			SL_SAFETY_timertick();
			SL_SAFETY_STATE_timertick();
			SL_CONTROL_timertick();
		}

		//=========================================== Hardcoded, ScheduleTable would be overkill
		if(eventMask & ev_Task_C0_Safety_System_0_1ms == ev_Task_C0_Safety_System_0_1ms)
		{
			CANopen_Client_Tick();
		}
		//Wait for next timer event to come

	}
}

