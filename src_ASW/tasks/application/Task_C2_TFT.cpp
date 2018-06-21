/*************************************
 * Task_C2_TFT.h
 * Author: 			P. Fromm et al
 * Description: 	Task for TFT Display and Touchscreen
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
#include "Interrupts.h"
#include "system.h"


//Application
#include "PORT.h"
#include "QSPI.h"

//Own header
#include "Task_C2_TFT.h"

//Runnables
#include "runnable_table.h"
#include "runnable_table_type.h"
#include "runnable_table_config.h"

//Signals
#include "signal_bb.h"

//Drivers
#include "Gpt_timer.h"



/*****************************************************************************************************
 * Local defines, memory sizes etc
 *****************************************************************************************************/


#define TASK_C2_TFT_STACKSIZE           500
#define TASK_C2_TFT_INTR_STACKSIZE      100
#define TASK_C2_TFT_CycleTime 			100

/*****************************************************************************************************
 * Task class declaration
 *****************************************************************************************************/

class CTask_C2_TFT {
    PxStackAligned_t InterruptStack[TASK_C2_TFT_INTR_STACKSIZE];
    PxStackAligned_t Stack[TASK_C2_TFT_STACKSIZE];


	// add 6 dummy bytes to avoid protection disruption by using "st.d" at the end of the data area
	// normaly an access to this pad bytes will lead to an protection fault
	// this 6 byte have to be always the last elements in the data area of a task
	// don't at any members behind this protection pad
    PxChar_t	__protectionPad[6] __attribute__ ((aligned(8)));

public:
    // Task function
	void Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);

	//Task create function (C-style)
	friend PxTask_t Task_C2_TFT_Create(PxUChar_t prio, PxEvents_t actevents);

} __attribute__ ((aligned(64)));

/*****************************************************************************************************
 * Task object(s)
 *****************************************************************************************************/

#pragma section ".Task_C2_TFT.stack" 
CTask_C2_TFT Task_C2_TFT_obj;
#pragma section

/*****************************************************************************************************
 * Local storage of task id. Faster and more reliable than nameserver
 *****************************************************************************************************/

#pragma section ".Task_Init_Lock.data" awB 8
	static PxTask_t Task_C2_TFT_id={0};
#pragma section

 
PxTask_t Task_C2_TFT_getTaskId()
{
	return Task_C2_TFT_id;
}


/*****************************************************************************************************
 * Create function
 *****************************************************************************************************/

		
static const PxProtectRegion_T Task_C2_TFT_Regions[] = {

#ifdef RTE_TIMING_ANALYSIS
	//GPT timer peripheral register read access
	{ (PxUInt_t) DRV_GPT_BEGIN, (PxUInt_t) DRV_GPT_END, (PxProtectType_t)ReadProtection},

#endif //RTE_TIMING_ANALYSIS



	//Global data of the TFT drivers
	{ (PxUInt_t) __CPU2_TFT_BEGIN, (PxUInt_t) __CPU2_TFT_END, (PxProtectType_t)WRProtection},

	//Stores global data for timing analysis
	{ (PxUInt_t) __CPU2_DATAPOOL_BEGIN, (PxUInt_t) __CPU2_DATAPOOL_END, (PxProtectType_t)WRProtection},

	//Stores data which is created by the Inittask, e.g. task and message Id's
	{ (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN, (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_END, (PxProtectType_t)ReadProtection},

	//QSPI0 Port Access
	{ (PxUInt_t) PORT_SPI_0_BEGIN, (PxUInt_t) PORT_SPI_0_END, (PxProtectType_t)WRProtection},

	//QSPI0 Channel-0 Line Access
	{ (PxUInt_t) PORT_20_BEGIN, (PxUInt_t) PORT_20_END, (PxProtectType_t)WRProtection},

	//Required for DET, TRAP,...
	{ (PxUInt_t) __GLOBAL_TASK_BEGIN, (PxUInt_t) __GLOBAL_TASK_END, (PxProtectType_t)WRProtection},

	//Signal Layer Control Signals
	{ (PxUInt_t) __CPU1_SL_CONTROL_BEGIN, (PxUInt_t) __CPU1_SL_CONTROL_END, (PxProtectType_t)ReadProtection},

	//Signal Layer Safety Signals
	{ (PxUInt_t) __CPU0_SL_SAFETY_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_END, (PxProtectType_t)ReadProtection},

	//Signal Layer Safety State Signals
	{ (PxUInt_t) __CPU0_SL_SAFETY_STATE_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_STATE_END, (PxProtectType_t)ReadProtection},

	//Signal Layer Miscellaneous Signals like TFT signals
	{ (PxUInt_t) __CPU2_SL_MISC_BEGIN, (PxUInt_t) __CPU2_SL_MISC_END, (PxProtectType_t)WRProtection},

	//Library functions
	{ (PxUInt_t) LIBDNK_DATA_BASE, (PxUInt_t) LIBDNK_DATA_END, (PxProtectType_t)WRProtection},

	//End of list
	{0,0,(PxProtectType_t)0}
};



void Task_C2_TFT_Entry(PxTask_t myId, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
        Task_C2_TFT_obj.Task_Func(myId,myMailbox,myActivationEvents);
}


extern "C" {
PxTask_t Task_C2_TFT_Create(PxUChar_t prio,PxEvents_t events)
{

	PxTaskContext_T task_Context;
	PxTaskSpec_T task_Spec;
	PxBzero((PxUChar_t *)&task_Spec,sizeof(task_Spec));

	//Protection [0] = 0 --> Inherited from the Owner.
	task_Context.protection[0].lowerBound 	= 0,
	task_Context.protection[0].upperBound 	= 0,
	task_Context.protection[0].prot 		= NoAccessProtection;

	//Task stack memory
	task_Context.protection[1].lowerBound 	= (PxUInt_t)&Task_C2_TFT_obj,
	task_Context.protection[1].upperBound 	= (PxUInt_t)&Task_C2_TFT_obj.__protectionPad[0],
	task_Context.protection[1].prot 		= WRProtection;

	task_Spec.ts_name						= (const PxChar_t*)"Task_C2_TFT",
	task_Spec.ts_fun						= Task_C2_TFT_Entry,
	task_Spec.ts_mc							= PXMcTaskdefault,
	task_Spec.ts_opool						= PXOpoolTaskdefault,
	task_Spec.ts_privileges					= PXUser1Privilege,
	task_Spec.ts_accessrights				= 0| PXACCESS_HANDLERS| PXACCESS_INSTALL_HANDLERS| PXACCESS_INSTALL_SERVICES| PXACCESS_REGISTERS| PXACCESS_SYSTEMDEFAULT| PXACCESS_RESOURCES| PXACCESS_NEW_RESOURCES| PXACCESS_SYSTEM_CONTROL| PXACCESS_MODEBITS| PXACCESS_OVERRIDE_ABORT_EVENTS,
	task_Spec.ts_context					= &task_Context,
	task_Spec.ts_protect_region  			= Task_C2_TFT_Regions,
	task_Spec.ts_taskstack.stk_type 		= PXStackFall,
	task_Spec.ts_taskstack.stk_size 		= PXStackDontCheck,
	
	task_Spec.ts_taskstack.stk_src.stk = &Task_C2_TFT_obj.Stack[TASK_C2_TFT_STACKSIZE],
	task_Spec.ts_inttaskstack.stk_type 		= PXStackFall;
	task_Spec.ts_inttaskstack.stk_size 		= PXStackDontCheck;
	task_Spec.ts_inttaskstack.stk_src.stk 	= &Task_C2_TFT_obj.InterruptStack[TASK_C2_TFT_INTR_STACKSIZE];
	task_Spec.ts_abortstacksize				= 1;

	Task_C2_TFT_id = PxTaskCreate(PXOpoolTaskdefault,&task_Spec,prio,events);
    return Task_C2_TFT_id;
	
}
} //extern C

/*****************************************************************************************************
 * Task Function
 *****************************************************************************************************/

//ISR must be compiled with C-linkage
extern "C" {


// QSPI TX function
void QSPI0_txISR(PxArg_t arg)
{
	_QSPI_TX_ISR(0);

	return;
}

// QSPI RX function
void QSPI0_ptISR(PxArg_t arg)
{
	_QSPI_PHASE_TRANSITION_ISR(0);

	return;
}

// QSPI Error function
void QSPI0_errorISR(PxArg_t arg)
{
	_QSPI_ERROR_ISR(0);

	return;

}
} //extern C

/*****************************************************************************************************
 * Task Function
 *****************************************************************************************************/

void CTask_C2_TFT::Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	// Turn off warning
	(void) myID;
	(void) myMailbox;
	(void) myActivationEvents;
	RC_t err;

	

	//Initialise the TFT SIgnals
	SL_MISC_init();

	// This function will initialise all interrupts for this task as defined in interrupt_cfg.cpp
	if ((err = INT_osInstallIsrTable(INT_TID_C2_TFT, (PxArg_t) myID.id)) != RC_SUCCESS)
	{
		DET_stop(AUTOCORE, INTERRUPTVALIDATION, INT_TID_C2_TFT);
	}

	//Setup the interrupts
	//InterruptRouter_InstallISR(QSPI0_txISR, (PxArg_t) myID.id);
	//InterruptRouter_InstallISR(QSPI0_ptISR, (PxArg_t) myID.id);
	//InterruptRouter_InstallISR(QSPI0_errorISR, (PxArg_t) myID.id);

	PxEvents_t event = {0};

	PxPe_t pe = PxPeRequest(PXOpoolTaskdefault, TASK_C2_TFT_CycleTime, ev_Task_C2_TFT_1_100ms);
	PxPeStart(pe);

	//Call all device runnable which require the communication system to be up and running
	RT_initRunExecute(RT_C2_tft_initRunTable, RT_C2_tft_initRunTable_size);

	while (1)
	{
		//Wait for next event to come
		event = PxAwaitEvents(ev_Task_C2_TFT_0_Touch | ev_Task_C2_TFT_1_100ms);

		RT_eventRunExecute(RT_C2_tft_eventRunTable, RT_C2_tft_eventRunTable_size,
				           (uint32_t)event, myID,
						   eRTE_TIME_taskC2TFTTimings_idx);

	}
}



