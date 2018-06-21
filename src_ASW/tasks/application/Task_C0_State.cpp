/*************************************
 * Task_C0_State.h
 * Author: 			Fromm
 * Description: 	Central State Machine Task
 * Task creation: 	
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
#include "Symbols.h"
#include "signal_bb.h"

#include "UART.h"
#include "UART_config.h"
#include "UARTprot.h"

#include "CAN.h"

//Application
#include "statemachine.h"

//Own header
#include "Task_C0_State.h"


/*****************************************************************************************************
 * Local defines, memory sizes etc
 *****************************************************************************************************/


#define TASK_C0_STATE_STACKSIZE           500
#define TASK_C0_STATE_INTR_STACKSIZE      50


/*****************************************************************************************************
 * Task class declaration
 *****************************************************************************************************/

class CTask_C0_State {
    PxStackAligned_t InterruptStack[TASK_C0_STATE_INTR_STACKSIZE];
    PxStackAligned_t Stack[TASK_C0_STATE_STACKSIZE];


	// add 6 dummy bytes to avoid protection disruption by using "st.d" at the end of the data area
	// normaly an access to this pad bytes will lead to an protection fault
	// this 6 byte have to be always the last elements in the data area of a task
	// don't at any members behind this protection pad
    PxChar_t	__protectionPad[6] __attribute__ ((aligned(8)));

public:
    // Task function
	void Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);

	//Task create function (C-style)
	friend PxTask_t Task_C0_State_Create(PxUChar_t prio, PxEvents_t actevents);

} __attribute__ ((aligned(64)));

/*****************************************************************************************************
 * Task object(s)
 *****************************************************************************************************/

#pragma section ".Task_C0_State"
CTask_C0_State Task_C0_State_obj;
#pragma section

/*****************************************************************************************************
 * Local storage of task id. Faster and more reliable than nameserver
 *****************************************************************************************************/

#pragma section ".Task_Init_Lock.data" awB 8
static PxTask_t Task_C0_State_id={0};
#pragma section

 
PxTask_t Task_C0_State_getTaskId()
{
	return Task_C0_State_id;
}


/*****************************************************************************************************
 * Create function
 *****************************************************************************************************/

		
static const PxProtectRegion_T Task_C0_State_Regions[] = {
		
		//Signal Layer Safety Signals
		{ (PxUInt_t) __CPU0_SL_SAFETY_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_END, (PxProtectType_t)ReadProtection},

		//Signal Layer Control Signals - will be replaced with message later on
		{ (PxUInt_t) __CPU1_SL_CONTROL_BEGIN, (PxUInt_t) __CPU1_SL_CONTROL_END, (PxProtectType_t)ReadProtection},

		//Signal Layer Safety State Signals
		{ (PxUInt_t) __CPU0_SL_SAFETY_STATE_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_STATE_END, (PxProtectType_t)WRProtection},

		//Required for DET, TRAP,...
		{ (PxUInt_t) __GLOBAL_TASK_BEGIN, (PxUInt_t) __GLOBAL_TASK_END, (PxProtectType_t)WRProtection},

		//Stores global data from UART and CAN
		{ (PxUInt_t) __CPU0_DATAPOOL_BEGIN, (PxUInt_t) __CPU0_DATAPOOL_END, (PxProtectType_t)WRProtection},

		//Stores data which is created by the Inittask, e.g. task and message Id's
		{ (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN, (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_END, (PxProtectType_t)ReadProtection},

		//End of list
		{0,0,(PxProtectType_t)0}

};



void Task_C0_State_Entry(PxTask_t myId, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
        Task_C0_State_obj.Task_Func(myId,myMailbox,myActivationEvents);
}


extern "C" {
PxTask_t Task_C0_State_Create(PxUChar_t prio,PxEvents_t events)
{

	PxTaskContext_T task_Context;
	PxTaskSpec_T task_Spec;
	PxBzero((PxUChar_t *)&task_Spec,sizeof(task_Spec));

	//Protection [0] = 0 --> Inherited from the Owner.
	task_Context.protection[0].lowerBound 	= 0,
	task_Context.protection[0].upperBound 	= 0,
	task_Context.protection[0].prot 		= NoAccessProtection;

	//Task stack memory
	task_Context.protection[1].lowerBound 	= (PxUInt_t)&Task_C0_State_obj,
	task_Context.protection[1].upperBound 	= (PxUInt_t)&Task_C0_State_obj.__protectionPad[0],
	task_Context.protection[1].prot 		= WRProtection;

	task_Spec.ts_name						= (const PxChar_t*)"Task_C0_State",
	task_Spec.ts_fun						= Task_C0_State_Entry,
	task_Spec.ts_mc							= PXMcTaskdefault,
	task_Spec.ts_opool						= PXOpoolTaskdefault,
	task_Spec.ts_privileges					= PXUser1Privilege,
	task_Spec.ts_accessrights				= 0| PXACCESS_HANDLERS| PXACCESS_INSTALL_HANDLERS| PXACCESS_INSTALL_SERVICES| PXACCESS_REGISTERS| PXACCESS_SYSTEMDEFAULT| PXACCESS_RESOURCES| PXACCESS_NEW_RESOURCES| PXACCESS_SYSTEM_CONTROL| PXACCESS_MODEBITS| PXACCESS_OVERRIDE_ABORT_EVENTS,
	task_Spec.ts_context					= &task_Context,
	task_Spec.ts_protect_region  			= Task_C0_State_Regions,
	task_Spec.ts_taskstack.stk_type 		= PXStackFall,
	task_Spec.ts_taskstack.stk_size 		= PXStackDontCheck,
	
	task_Spec.ts_taskstack.stk_src.stk = &Task_C0_State_obj.Stack[TASK_C0_STATE_STACKSIZE],
	task_Spec.ts_inttaskstack.stk_type 		= PXStackFall;
	task_Spec.ts_inttaskstack.stk_size 		= PXStackDontCheck;
	task_Spec.ts_inttaskstack.stk_src.stk 	= &Task_C0_State_obj.InterruptStack[TASK_C0_STATE_INTR_STACKSIZE];
	task_Spec.ts_abortstacksize				= 0;

	Task_C0_State_id = PxTaskCreate(PXOpoolTaskdefault,&task_Spec,prio,events);
    return Task_C0_State_id;
	
}
} //extern C

/*****************************************************************************************************
 * Task Function
 *****************************************************************************************************/

void CTask_C0_State::Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	// Turn off warning
	(void) myID;
	(void) myMailbox;
	(void) myActivationEvents;
	
	PxEvents_t eventMask = 0;

	//Masks to separate between safety and normal events
	PxEvents_t normalEvents = ev_Task_C0_State_0_NMTHasBooted | ev_Task_C0_State_1_DevicesHaveBooted | ev_Task_C0_State_2_Present | ev_Task_C0_State_3_Connect | ev_Task_C0_State_4_FeatureRequest | ev_Task_C0_State_5_FeatureActivate |
							  ev_Task_C0_State_6_PreRunCheckOk | ev_Task_C0_State_7_Stop | ev_Task_C0_State_8_Disconnect | ev_Task_C0_State_9_EmergencyBrake ;
	PxEvents_t safetyEvents = ev_Task_C0_State_10_ErrorAllOk | ev_Task_C0_State_11_ErrorRequestError1 | ev_Task_C0_State_12_ErrorRequestError2 | ev_Task_C0_State_13_ErrorRequestError3;


	//Initialise the state machine signals
	SL_SAFETY_STATE_init();

	//Initialise  state machine
	STATE_init();

	volatile int count = 0;

	while (1)
	{
		//============================================ Wait for next event to come
		eventMask = PxAwaitEvents(normalEvents | safetyEvents);
		//PxAwaitEvents(ev_Task_C0_State_0_Present);
		count++;

		//============================================ Process the safety events
		if ((eventMask & safetyEvents) != 0)
		{
			STATE_safety_processEvent(eventMask & safetyEvents);
		}


		//=========================================== Process the normal events
		if ((eventMask & normalEvents) != 0)
		{
			STATE_normal_processEvent(eventMask & normalEvents);
		}

		//Just to see a change in the debugger
		eventMask = 0;
	}
}

