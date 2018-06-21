/*************************************
 * Task_C0_Init.h
 * Author: 			Fromm
 * Description: 	Initialisation Task of Core 0
 * Task creation: 	
 * This file is auto-generated by PxROS Generator (c) Hochschule Darmstadt, Linde
 */

/*******************************************************************************************************
 * Includes
 *******************************************************************************************************/

// System
#include "core.h"
#include "pxdef.h"

//Debugging aid
#include "det.h"
#include "sync.h"

//System Tasks
#include "Task_ReleaseServer.h"
#include "Task_Time.h"
#include "Task_Services.h"

//Global adress symbols
#include "symbols.h"
#include "SystemEvents.h"

//Own header
#include "Task_Init.h"
#include "Task_C0_Init.h"

//Application Tasks
#include "Task_C0_Communication.h"
#include "Task_C0_Safety_Input.h"
#include "Task_C0_Safety_Logic.h"
#include "Task_C0_State.h"
#include "Task_C0_Safety_System.h"

//Trap Handler and Interrupts
#include "trap_handler_runtime.h"
#include "Interrupts.h"
#include "Interrupts_cfg.h"

//RTE Timing Analysis
#include "rte_time.h"
#include "det_time.h"
#include "Gpt_timer.h"
#include "UART.h"


/*******************************************************************************************************
 * Pragma sections
 *******************************************************************************************************/

 //To allow some core specific handling later one...
#define CORE 0


//Task local memory (stack)
#pragma section ".Task_C0_Init.stack"
struct
{
	PxStackAligned_t stack[TASK_C0_INIT_STACKSIZE_CORE0];
	PxStackAligned_t intstack[TASK_C0_INIT_INTSTACKSIZE_CORE0];
	PxChar_t	     stack_protectionPad[6] __attribute__ ((aligned(8)));
} Task_C0_Init_obj;
#pragma section

#pragma section ".Task_Init_Lock.data" awB 8
//Data to be added
#pragma section

/*******************************************************************************************************
 * Protection regions Task_C0_Init
 *******************************************************************************************************/

//Task data access (variable)
static const PxProtectRegion_T InitTaskRegions_CORE0[] = {
		
#ifdef RTE_TIMING_ANALYSIS
	// The while(1) loop of the inittask transmits the rte timing data to the PC program
	// The following regions define access to the ringbuffers filled by the application tasks
	{ (PxUInt_t) __CPU0_DATAPOOL_BEGIN, (PxUInt_t) __CPU0_DATAPOOL_END, (PxProtectType_t)WRProtection},
	{ (PxUInt_t) __CPU1_DATAPOOL_BEGIN, (PxUInt_t) __CPU1_DATAPOOL_END, (PxProtectType_t)WRProtection},
	{ (PxUInt_t) __CPU2_DATAPOOL_BEGIN, (PxUInt_t) __CPU2_DATAPOOL_END, (PxProtectType_t)WRProtection},
#endif

	//Complete peripheral section
	{ (PxUInt_t) PERIPHERAL_START, (PxUInt_t) PERIPHERAL_END, (PxProtectType_t)WRProtection},


	//Global task memory
	{ (PxUInt_t) __GLOBAL_TASK_BEGIN, (PxUInt_t) __GLOBAL_TASK_END, (PxProtectType_t)WRProtection},

	//Stores data which is created by the Inittask, e.g. task and message Id's
	{ (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN, (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_END, (PxProtectType_t)WRProtection},

	//Indicating end of the list....
	{0,0,0}
};

//Task data access (fixed) - do not change
static const PxTaskContext_T InitTaskContext_CORE0 = {
	.protection[0] =
	{
	    .lowerBound = (PxUInt_t)&PxTricSystemRodataLowerBound,
        .upperBound = (PxUInt_t)&PxTricSystemRodataUpperBound,
        .prot 		= ReadProtection
	}
	,
	.protection[1] =
	{
	    .lowerBound = (PxUInt_t)&Task_C0_Init_obj.stack,
        .upperBound = (PxUInt_t)&Task_C0_Init_obj.stack_protectionPad[0],
        .prot 		= WRProtection
	}

};

/*******************************************************************************************************
 * Init specification Task_C0_Init
 *******************************************************************************************************/

const PxTaskSpec_T InitTaskSpec_CORE0 =
{
		.ts_name = (const PxChar_t *)"Task_C0_Init",
		.ts_fun = Task_C0_Init_Func,
		.ts_mc = PXMcTaskdefault,
		.ts_opool = PXOpoolSystemdefault,
		.ts_privileges = PXUser1Privilege,
		.ts_accessrights = 0| PXACCESS_HANDLERS| PXACCESS_INSTALL_HANDLERS| PXACCESS_INSTALL_SERVICES| PXACCESS_REGISTERS| PXACCESS_SYSTEMDEFAULT| PXACCESS_RESOURCES| PXACCESS_NEW_RESOURCES| PXACCESS_SYSTEM_CONTROL| PXACCESS_MODEBITS| PXACCESS_OVERRIDE_ABORT_EVENTS | PXACCESS_TASK_CREATE | PXACCESS_TASK_CREATE_HIGHER_PRIO | PXACCESS_TASK_SET_HIGHER_PRIO | PXACCESS_CHANGE_PRIO | PXACCESS_TASK_RESTORE_ACCESS_RIGHTS | PXACCESS_TASK_CREATE_HIGHER_ACCESS ,
		.ts_context = &InitTaskContext_CORE0,
		.ts_protect_region = InitTaskRegions_CORE0,
		.ts_taskstack =
		{
			.stk_type = PXStackFall,
			.stk_size = PXStackDontCheck,
			.stk_src.stk = &Task_C0_Init_obj.stack[TASK_C0_INIT_STACKSIZE_CORE0]
		},
		.ts_inttaskstack =
		{
			.stk_type = PXStackFall,
			.stk_size = PXStackDontCheck,
			.stk_src.stk = &Task_C0_Init_obj.intstack[TASK_C0_INIT_INTSTACKSIZE_CORE0]
		},
		.ts_abortstacksize = 0

};


/*******************************************************************************************************
 * ISR functions running in the context of Task_C0_Init
 *******************************************************************************************************/


/**
 * ISR for RTE Timing Timer Tick
 * Will be called upon timer reload to increment software timer, approx. every 100ms
 */
void RTE_TIMER_T3_ISR (PxArg_t arg);

/**
 * ISR for DET Timing Timer Tick
 * Will be called upon timer reload to increment software timer, approx. every 100ms
 */
void DET_TIMER_T2_ISR (PxArg_t arg);

/*******************************************************************************************************
 * Task function Task_C0_Init
 *******************************************************************************************************/


void Task_C0_Init_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{

	// Turn off warning
	(void) myID;
	(void) myMailbox;
	(void) myActivationEvents;

	//Local Variable to check return status of PxRos functions
	PxError_t result;


/*******************************************************************************************************
 * Initialise System Services
 *******************************************************************************************************/
    //Set up Trap Handlers
    PxTrapInstallHandler(0x0,Px_trap_hdl_0,0);
    PxTrapInstallHandler(0x1,Px_trap_hdl_1,0);
    PxTrapInstallHandler(0x2,Px_trap_hdl_2,0);
    PxTrapInstallHandler(0x3,Px_trap_hdl_3,0);
    PxTrapInstallHandler(0x4,Px_trap_hdl_4,0);
    PxTrapInstallHandler(0x5,Px_trap_hdl_5,0);
    PxTrapInstallHandler(0x6,Px_trap_hdl_6,0);
    PxTrapInstallHandler(0x7,Px_trap_hdl_7,0);


	// Start PXROS time base with HZ (defined in clock.h) ticks per second.
	//Needs to be done on every core
	SYSTICK_TicksInit(1000);

	PxUInt_t id = PxGetCoreId();

/*******************************************************************************************************
 * Initialise Diagnostic Timers
 *******************************************************************************************************/

	//DET Timer used for getting the time stamp for DET messages
/*
	if (RC_SUCCESS != INT_osInstall(DET_TIMER_T2_SRN, DET_TIMER_T2_ISR, DET_TIMER_T2_PRIO, INT_FASTCONTEXTHANDLER, (PxArg_t) myID.id))
	{
		DET_stop(AUTOCORE, INTERRUPTVALIDATION, 1);
	}
*/
	DET_TIME_ctrl(Start);


#ifdef RTE_TIMING_ANALYSIS
/*
	if(FALSE == ISR_Install_C(RTE_TIMER_T3_ISR, (PxArg_t) myID.id))
	{
		DET_stop(AUTOCORE, SETUPTIMER, 2);
	}
*/
#endif //RTE_TIMING_ANALYSIS

/*******************************************************************************************************
 * Initialise Name and Releaseserver
 *******************************************************************************************************/

#if CORE == 0
	// Initialize the name server only on core 0
	PxTask_t NameSrv;
	NameSrv = PxNamesrvInit(NAMESRV_PRIO_CORE0, (PxUInt_t)PxNameServerSpaceBase, (PxUInt_t)PxNameServerSpaceEnd-(PxUInt_t)PxNameServerSpaceBase);
	if ( (result = PxTaskIdError(NameSrv)) != PXERR_NOERROR)
	{
		DET_stop(AUTOCORE,CREATENAMESERVER, result);
	}
#endif

	// Initialise the release server
	//Task_ReleaseService_Id must run for PxGetGlobalNameServerMbx
	PxTask_t Task_ReleaseServer_Id;
	Task_ReleaseServer_Id = Task_ReleaseServerCreate(id,TASK_RELEASESERVER_PRIO, TASK_RELEASESERVER_ACTIVATION_EVENTS);
	if ((result = PxTaskIdError(Task_ReleaseServer_Id)) != PXERR_NOERROR)
	{
		DET_stop(AUTOCORE,CREATERELEASESERVER, result);
	}

	//Set the local priority to the highest value, so that no application task can start, before all of them are created
	PxTaskSetPrio(PxGetId(), 15);

	//PXROS Service Tasks on core 0 finished
	SYNC_SetC0(BASE_SYSTEM_INIT);

/*******************************************************************************************************
 * Check if the nameserver is already up and running
 *******************************************************************************************************/


    //Check if the a mailbox can be requested from the name server
    result = WaitForService (PXCORE_0, _PxNameSrvReqMbxId, 10);

    if ( result != PXERR_NOERROR)
    {
    	DET_stop(AUTOCORE,CHECKSERVICE, result);
    }

/*******************************************************************************************************
 * Create application tasks here (Tasks and Mailboxes) and register them eiterh with the name server
 * or in a specific memory location
 *******************************************************************************************************/	

		
/****************  Task_C0_Safety_Input ********************/

    //Create a local info block which will contain the task data
    taskinfo_t Task_C0_Safety_Input_info;

    //Call the task create function
    PxTask_t Task_C0_Safety_Input_Id;
    Task_C0_Safety_Input_Id = Task_C0_Safety_Input_Create(TASK_C0_SAFETY_INPUT_PRIO, ev_Task_C0_Safety_Input_0_Activate);

    result = PxTaskIdError(Task_C0_Safety_Input_Id);
    if (result != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,CREATETASK, result);
    }

    // Get the handler to the tasks message box
    PxMbx_t Task_C0_Safety_Input_Mbx = PxTaskGetMbx(Task_C0_Safety_Input_Id);

    // Register information about Task and Mbx Id ==> the structure of the data depends on the application requirements
    Task_C0_Safety_Input_info.taskId = Task_C0_Safety_Input_Id;
    Task_C0_Safety_Input_info.mbxId = Task_C0_Safety_Input_Mbx;

    // Register information about Task and Mbx Id
    // This step depends on the architecture, e.g. when using the RTL it is not required
    if ((result = PxNameRegister(TASK_C0_SAFETY_INPUT_NAMESERVERID,sizeof(taskinfo_t),&Task_C0_Safety_Input_info)) != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,REGISTERNAME, result);
    }

    //PxTaskGetName(Task_C0_Safety_Input_Id, name, 19);
	
/****************  Task_C0_Safety_Logic ********************/

    //Create a local info block which will be stored on the nameserver
    taskinfo_t Task_C0_Safety_Logic_info;

    //Call the task create function
    PxTask_t Task_C0_Safety_Logic_Id;
    Task_C0_Safety_Logic_Id = Task_C0_Safety_Logic_Create(TASK_C0_SAFETY_LOGIC_PRIO, ev_Task_C0_Safety_Logic_0_Activate);

    result = PxTaskIdError(Task_C0_Safety_Logic_Id);
    if (result != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,CREATETASK, result);
    }

    // Get the handler to the tasks message box
    PxMbx_t Task_C0_Safety_Logic_Mbx = PxTaskGetMbx(Task_C0_Safety_Logic_Id);

    // Register information about Task and Mbx Id ==> the structure of the data depends on the application requirements
    Task_C0_Safety_Logic_info.taskId = Task_C0_Safety_Logic_Id;
    Task_C0_Safety_Logic_info.mbxId = Task_C0_Safety_Logic_Mbx;

    // Register information about Task and Mbx Id
    // This step depends on the architecture, e.g. when using the RTL it is not required
    if ((result = PxNameRegister(TASK_C0_SAFETY_LOGIC_NAMESERVERID,sizeof(taskinfo_t),&Task_C0_Safety_Logic_info)) != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,REGISTERNAME, result);
    }

	
/****************  Task_C0_Communication ********************/

    //Create a local info block which will be stored on the nameserver
    taskinfo_t Task_C0_Communication_info;

    //Call the task create function
    PxTask_t Task_C0_Communication_Id;
    Task_C0_Communication_Id = Task_C0_Communication_Create(TASK_C0_COMMUNICATION_PRIO, ev_Task_C0_Communication_0_Activate);

    result = PxTaskIdError(Task_C0_Communication_Id);
    if (result != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,CREATETASK, result);
    }

    // Get the handler to the tasks message box
    PxMbx_t Task_C0_Communication_Mbx = PxTaskGetMbx(Task_C0_Communication_Id);

    // Register information about Task and Mbx Id ==> the structure of the data depends on the application requirements
    Task_C0_Communication_info.taskId = Task_C0_Communication_Id;
    Task_C0_Communication_info.mbxId = Task_C0_Communication_Mbx;

    // Register information about Task and Mbx Id
    // This step depends on the architecture, e.g. when using the RTL it is not required
    if ((result = PxNameRegister(TASK_C0_COMMUNICATION_NAMESERVERID,sizeof(taskinfo_t),&Task_C0_Communication_info)) != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,REGISTERNAME, result);
    }

	
/****************  Task_C0_State ********************/

    //Create a local info block which will be stored on the nameserver
    taskinfo_t Task_C0_State_info;

    //Call the task create function
    PxTask_t Task_C0_State_Id;
    Task_C0_State_Id = Task_C0_State_Create(TASK_C0_STATE_PRIO, ev_Task_C0_State_0_Activate);

    result = PxTaskIdError(Task_C0_State_Id);
    if (result != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,CREATETASK, result);
    }

    // Get the handler to the tasks message box
    PxMbx_t Task_C0_State_Mbx = PxTaskGetMbx(Task_C0_State_Id);

    // Register information about Task and Mbx Id ==> the structure of the data depends on the application requirements
    Task_C0_State_info.taskId = Task_C0_State_Id;
    Task_C0_State_info.mbxId = Task_C0_State_Mbx;

    Task_C0_State_info.taskId.id = Task_C0_State_Id.id;

    // Register information about Task and Mbx Id
    // This step depends on the architecture, e.g. when using the RTL it is not required
    if ((result = PxNameRegister(TASK_C0_STATE_NAMESERVERID,sizeof(taskinfo_t),&Task_C0_State_info)) != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,REGISTERNAME, result);
    }

	
/****************  Task_C0_Safety_System ********************/

    //Create a local info block which will be stored on the nameserver
    taskinfo_t Task_C0_Safety_System_info;

    //Call the task create function
    PxTask_t Task_C0_Safety_System_Id;
    Task_C0_Safety_System_Id = Task_C0_Safety_System_Create(TASK_C0_SAFETY_SYSTEM_PRIO, ev_Task_C0_Safety_System_0_Activate);

    result = PxTaskIdError(Task_C0_Safety_System_Id);
    if (result != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,CREATETASK, result);
    }

    // Get the handler to the tasks message box
    PxMbx_t Task_C0_Safety_System_Mbx = PxTaskGetMbx(Task_C0_Safety_System_Id);

    // Register information about Task and Mbx Id ==> the structure of the data depends on the application requirements
    Task_C0_Safety_System_info.taskId = Task_C0_Safety_System_Id;
    Task_C0_Safety_System_info.mbxId = Task_C0_Safety_System_Mbx;

    // Register information about Task and Mbx Id
    // This step depends on the architecture, e.g. when using the RTL it is not required
    if ((result = PxNameRegister(TASK_C0_SAFETY_SYSTEM_NAMESERVERID,sizeof(taskinfo_t),&Task_C0_Safety_System_info)) != PXERR_NOERROR)
    {
	    DET_stop(AUTOCORE,REGISTERNAME, result);
    }


	
/*******************************************************************************************************
 * Initialise the runtime environment
 *******************************************************************************************************/
	SYNC_SetC0(TASKS_CREATED);

    //Before we initilaise the Signallayer objects, the message and event receiving tasks on core 1 and core 2
    //must be there and must have been registered with the nameserver in order to store the receiver task id in the signal objects

    //SYNC_WaitForState(TASKS_READY, TIMEOUTSYNC, PXBOOTSYNC2);

    //Initialise the safety signallayer objects


/*******************************************************************************************************
 * All cores are ready, tasks can be activated
 *******************************************************************************************************/

	
    if ((result = PxTaskSignalEvents(Task_C0_Safety_Input_Id, ev_Task_C0_Safety_Input_0_Activate)) != PXERR_NOERROR)
	{
		DET_stop(AUTOCORE,SIGNALEVENT, result);
	}
    if ((result = PxTaskSignalEvents(Task_C0_Safety_Logic_Id, ev_Task_C0_Safety_Logic_0_Activate)) != PXERR_NOERROR)
	{
		DET_stop(AUTOCORE,SIGNALEVENT, result);
	}

    if ((result = PxTaskSignalEvents(Task_C0_Communication_Id, ev_Task_C0_Communication_0_Activate)) != PXERR_NOERROR)
	{
		DET_stop(AUTOCORE,SIGNALEVENT, result);
	}
    if ((result = PxTaskSignalEvents(Task_C0_State_Id, ev_Task_C0_State_0_Activate)) != PXERR_NOERROR)
	{
		DET_stop(AUTOCORE,SIGNALEVENT, result);
	}
    if ((result = PxTaskSignalEvents(Task_C0_Safety_System_Id, ev_Task_C0_Safety_System_0_Activate)) != PXERR_NOERROR)
	{
		DET_stop(AUTOCORE,SIGNALEVENT, result);
	}

/*******************************************************************************************************
 * Start endless background loop with lowest priority
 *******************************************************************************************************/

	SYNC_SetC0(TASKS_READY);
	
	//At this point of time, we have to make sure that the safety critical OS instance on core 0 is up and running
    //Depending on the architecture, this may be ommitted
#if CORE != 0
    SYNC_C12WaitForC0State(TASKS_READY, TIMEOUTSYNC, PXBOOTSYNC2);
#endif


	//Lowest prio
	PxTaskSetPrio(PxGetId(), MINPRIO_CORE0);

/*******************************************************************************************************
 * Timing analysis configuration
 *******************************************************************************************************/

#ifdef RTE_TIMING_ANALYSIS
	//Get the task names. They will be transmitted to the RTE Timing viewer on the PC during initialisation

	RTE_TIME_appTaskInitDetails[CORE][RTE_TIME_noOfAppTasksInCore0].taskID = Task_C0_Safety_Input_Id;
	PxTaskGetName(Task_C0_Safety_Input_Id, &(RTE_TIME_appTaskInitDetails[0][RTE_TIME_noOfAppTasksInCore0++].taskName[0]), 20);

	RTE_TIME_appTaskInitDetails[CORE][RTE_TIME_noOfAppTasksInCore0].taskID = Task_C0_Safety_Logic_Id;
	PxTaskGetName(Task_C0_Safety_Logic_Id, &(RTE_TIME_appTaskInitDetails[0][RTE_TIME_noOfAppTasksInCore0++].taskName[0]), 20);

	RTE_TIME_appTaskInitDetails[CORE][RTE_TIME_noOfAppTasksInCore0].taskID = Task_C0_Communication_Id;
	PxTaskGetName(Task_C0_Communication_Id, &(RTE_TIME_appTaskInitDetails[0][RTE_TIME_noOfAppTasksInCore0++].taskName[0]), 20);

	RTE_TIME_appTaskInitDetails[CORE][RTE_TIME_noOfAppTasksInCore0].taskID = Task_C0_State_Id;
	PxTaskGetName(Task_C0_State_Id, &(RTE_TIME_appTaskInitDetails[0][RTE_TIME_noOfAppTasksInCore0++].taskName[0]), 20);
#endif //RTE_TIMING_ANALYSIS

	// Endless loop for background task
	while (1) {

#ifdef RTE_TIMING_ANALYSIS
		RTE_TIME_SendOutRteData();
#endif //RTE_TIMING_ANALYSIS

		asm("wait");
	}


}

/*******************************************************************************************************
 * Interrupt Service Functions
 *******************************************************************************************************/


/**
 * ISR for RTE Timing Timer Tick
 * Will be called every 100ms
 */
void RTE_TIMER_T3_ISR (PxArg_t arg)
{
	RTE_TIME_MsbTicksIncr();
	GPT12_Reload(GPT12_T3);
}

/**
 * ISR for DET Timing Timer Tick
 * Will be called every 100ms
 */
void DET_TIMER_T2_ISR (PxArg_t arg)
{
	DET_TIME_ISR(0);
}
