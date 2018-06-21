/*************************************
 * Task_C0_Safety_Logic.h
 * Author: 			Fromm
 * Description: 	Safety Logic Monitor
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

//Runtime Environment
#include "runnable_table.h"
#include "runnable_table_config.h"

#include "msg_config.h"
#include "msg_dispatcher.h"

//Own header
#include "Task_C0_Safety_Logic.h"
#include "Task_C0_State.h"

//Drivers
#include "CAN.h"
#include "Gpt_timer.h"

/*****************************************************************************************************
 * Local defines, memory sizes etc
 *****************************************************************************************************/


#define TASK_C0_SAFETY_LOGIC_STACKSIZE           800
#define TASK_C0_SAFETY_LOGIC_INTR_STACKSIZE      50
#define TASK_C0_SAFETY_LOGIC_CycleTime 			 100


/*****************************************************************************************************
 * Task class declaration
 *****************************************************************************************************/

class CTask_C0_Safety_Logic {
    PxStackAligned_t InterruptStack[TASK_C0_SAFETY_LOGIC_INTR_STACKSIZE];
    PxStackAligned_t Stack[TASK_C0_SAFETY_LOGIC_STACKSIZE];

	// add 6 dummy bytes to avoid protection disruption by using "st.d" at the end of the data area
	// normaly an access to this pad bytes will lead to an protection fault
	// this 6 byte have to be always the last elements in the data area of a task
	// don't at any members behind this protection pad
    PxChar_t	__protectionPad[6] __attribute__ ((aligned(8)));

public:
	// The task function with the application code
    void Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);

	//The task create function (C-style)
	friend PxTask_t Task_C0_Safety_Logic_Create(PxUChar_t prio, PxEvents_t actevents);

} __attribute__ ((aligned(64)));

/*****************************************************************************************************
 * Task object(s)
 *****************************************************************************************************/

#pragma section ".Task_C0_Safety_Logic"
CTask_C0_Safety_Logic Task_C0_Safety_Logic_obj;
#pragma section

/*****************************************************************************************************
 * Local storage of task id. Faster and more reliable than nameserver
 *****************************************************************************************************/

#pragma section ".Task_Init_Lock.data" awB 8
static PxTask_t Task_C0_Safety_Logic_id={0};
#pragma section

 
PxTask_t Task_C0_Safety_Logic_getTaskId()
{
	return Task_C0_Safety_Logic_id;
}


/*****************************************************************************************************
 * Create function
 *****************************************************************************************************/


static const PxProtectRegion_T Task_C0_Safety_Logic_Regions[] = {

#ifdef RTE_TIMING_ANALYSIS
	//GPT timer peripheral register read access
	{ (PxUInt_t) DRV_GPT_BEGIN, (PxUInt_t) DRV_GPT_END, (PxProtectType_t)ReadProtection},

#endif //RTE_TIMING_ANALYSIS

	//Task global region, contains data accessed by all tasks. Required for DET, TRAP,...
	{ (PxUInt_t) __GLOBAL_TASK_BEGIN, (PxUInt_t) __GLOBAL_TASK_END, (PxProtectType_t)WRProtection},

	//Stores global data from UART and CAN
	{ (PxUInt_t) __CPU0_DATAPOOL_BEGIN, (PxUInt_t) __CPU0_DATAPOOL_END, (PxProtectType_t)WRProtection},

	//Signal Layer Safety Signals
	{ (PxUInt_t) __CPU0_SL_SAFETY_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_END, (PxProtectType_t)WRProtection},

	//Signal Layer Safety State Signals
	{ (PxUInt_t) __CPU0_SL_SAFETY_STATE_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_STATE_END, (PxProtectType_t)ReadProtection},

	//Signal Layer Control Signals
	{ (PxUInt_t) __CPU1_SL_CONTROL_BEGIN, (PxUInt_t) __CPU1_SL_CONTROL_END, (PxProtectType_t)ReadProtection},

	//Stores data which is created by the Inittask, e.g. task and message Id's
	{ (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN, (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_END, (PxProtectType_t)ReadProtection},

	//CAN Module for CanOpen
	{ (PxUInt_t) MP_CAN_M_BEGIN, (PxUInt_t) MP_CAN_M_END, (PxProtectType_t)WRProtection},



	//End of list
	{0,0,(PxProtectType_t)0}
};


void Task_C0_Safety_Logic_Entry(PxTask_t myId, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
        Task_C0_Safety_Logic_obj.Task_Func(myId,myMailbox,myActivationEvents);
}


extern "C" {
PxTask_t Task_C0_Safety_Logic_Create(PxUChar_t prio,PxEvents_t events)
{

	PxTaskContext_T task_Context;
	PxTaskSpec_T task_Spec;
	PxBzero((PxUChar_t *)&task_Spec,sizeof(task_Spec));

	//Protection [0] = 0 --> Inherited from the Owner.
	task_Context.protection[0].lowerBound 	= 0,
	task_Context.protection[0].upperBound 	= 0,
	task_Context.protection[0].prot 		= NoAccessProtection;

	//Task stack memory
	task_Context.protection[1].lowerBound 	= (PxUInt_t)&Task_C0_Safety_Logic_obj,
	task_Context.protection[1].upperBound 	= (PxUInt_t)&Task_C0_Safety_Logic_obj.__protectionPad[0],
	task_Context.protection[1].prot 		= WRProtection;

	task_Spec.ts_name						= (const PxChar_t*)"Task_C0_Safety_Logic",
	task_Spec.ts_fun						= Task_C0_Safety_Logic_Entry,
	task_Spec.ts_mc							= PXMcTaskdefault,
	task_Spec.ts_opool						= PXOpoolTaskdefault,
	task_Spec.ts_privileges					= PXUser1Privilege,
	task_Spec.ts_accessrights				= 0| PXACCESS_HANDLERS| PXACCESS_INSTALL_HANDLERS| PXACCESS_INSTALL_SERVICES| PXACCESS_REGISTERS| PXACCESS_SYSTEMDEFAULT| PXACCESS_RESOURCES| PXACCESS_NEW_RESOURCES| PXACCESS_SYSTEM_CONTROL| PXACCESS_MODEBITS| PXACCESS_OVERRIDE_ABORT_EVENTS,
	task_Spec.ts_context					= &task_Context,
	task_Spec.ts_protect_region  			= Task_C0_Safety_Logic_Regions,
	task_Spec.ts_taskstack.stk_type 		= PXStackFall,
	task_Spec.ts_taskstack.stk_size 		= PXStackDontCheck,
	
	task_Spec.ts_taskstack.stk_src.stk 		= &Task_C0_Safety_Logic_obj.Stack[TASK_C0_SAFETY_LOGIC_STACKSIZE],
	task_Spec.ts_inttaskstack.stk_type 		= PXStackFall;
	task_Spec.ts_inttaskstack.stk_size 		= PXStackDontCheck;
	task_Spec.ts_inttaskstack.stk_src.stk 	= &Task_C0_Safety_Logic_obj.InterruptStack[TASK_C0_SAFETY_LOGIC_INTR_STACKSIZE];
	task_Spec.ts_abortstacksize				= 0;

	Task_C0_Safety_Logic_id = PxTaskCreate(PXOpoolTaskdefault,&task_Spec,prio,events);
    return Task_C0_Safety_Logic_id;
	
}
} //extern C

/*****************************************************************************************************
 * Task Function
 *****************************************************************************************************/


 
void CTask_C0_Safety_Logic::Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	// Turn off warning in case parameters are not used
	(void) myID;
	(void) myMailbox;
	(void) myActivationEvents;



	//=============================================== Data Initialisations ==================================================

	PxMsgEvent_t EveMsg = {0};
	//PxEvents_t eventMask = 0;
	RT_safety_t result = SAFENOCHECK;

	//This task will receive error messages in case one of the runnable returns an error
	PxTask_t stateTask = Task_C0_State_getTaskId();

	//Generate a periodic event handler
	PxPe_t pe = PxPeRequest(PXOpoolTaskdefault, TASK_C0_SAFETY_LOGIC_CycleTime, ev_Task_C0_Safety_Logic_0_100ms);
	PxPeStart(pe);


	//=============================================== Start normal task operation =============================================

	while (1)
	{

		EveMsg = PxMsgReceive_EvWait(myMailbox,
									ev_Task_C0_Safety_Logic_0_100ms |
									ev_Task_C0_Safety_Logic_1_newTargetSpeed |
									ev_Task_C0_Safety_Logic_2_requestEmergencyStop |
									ev_Task_C0_Safety_Logic_3_requestStop);


		//============================================= Call message handler
		if (PxMsgIdIsValid(EveMsg.msg))
		{
				RC_t result = MSG_receive(&EveMsg.msg, MSG_portList_C0_SafetyLogic, MSG_portList_C0_SafetyLogic_size);
		}

		//=============================================== Call of time / event triggered runnables in any state ===================

		if (0 != EveMsg.events)
		{
			RT_safety_t result = RT_eventRunSafetyExecute(RT_C0_safetyLogic_eventRunTable, RT_C0_safetyLogic_eventRunTable_size, (uint32_t)EveMsg.events, stateTask, myID, eRTE_TIME_taskC0SafetyLogicTimings_idx);
		}

	}
}

