/**
* \file Task_C0_Communication.cpp
* \author P. Fromm
* \date 17.08.2016
*
* \brief Task_C0_Communication
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

/*****************************************************************************************************
 * Includes
 *****************************************************************************************************/
//System
#include "pxdef.h"
#include "shared.h"
#include "global.h"
#include "symbols.h"
#include "SystemEvents.h"
#include "logging.h"
#include "Interrupts.h"
#include "Interrupts_cfg.h"
#include "carconfig.h"
#include "det.h"

//#include "UART_TP.h"
#include "CRingBuffer.h"

//Signallayer
#include "signal_bb.h"
#include "cardataprotocol_signal.h"
#include "uint8_signal.h"

//Communication Stacks
#include "can.h"
#include "CAN_config.h"
#include "CANopen.h"
#include "CanOpen_OD_cfg.h"

#include "UART.h"
#include "UART_config.h"
#include "UARTprot.h"

#include "rte_time.h"
#include "Gpt_timer.h"

//Own header
#include "Task_C0_Communication.h"

//Other tasks
#include "Task_Init.h"
#include "Task_C0_State.h"

//Includes required for the simulation of the state control
#include "sint32_ir_signal.h"
#include "nmt_signal.h"

//Runnables
#include "runnable_table.h"
#include "runnable_table_type.h"
#include "runnable_table_config.h"

//Messaging
#include "msg_dispatcher.h"
#include "msg_config.h"

/*****************************************************************************************************
 * Local defines, memory sizes etc
 *****************************************************************************************************/
#define TASK_C0_COMMUNICATION_STACKSIZE           800
#define TASK_C0_COMMUNICATION_INTR_STACKSIZE      800
#define TASK_C0_Communication_CanOpen_CycleTime   1

#define	DRV_UARTUSBBEGIN	((unsigned int)&MODULE_ASCLIN0)
#define	DRV_UARTUSBEND		((unsigned int)&MODULE_ASCLIN0 + sizeof(Ifx_ASCLIN))

/*****************************************************************************************************
 * References
 *****************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*****************************************************************************************************
 * Task class declaration
 *****************************************************************************************************/

class CTask_C0_Communication {
    PxStackAligned_t InterruptStack[TASK_C0_COMMUNICATION_INTR_STACKSIZE];
    PxStackAligned_t Stack[TASK_C0_COMMUNICATION_STACKSIZE];


	// add 6 dummy bytes to avoid protection disruption by using "st.d" at the end of the data area
	// normaly an access to this pad bytes will lead to an protection fault
	// this 6 byte have to be always the last elements in the data area of a task
	// don't at any members behind this protection pad
    PxChar_t	__protectionPad[6] __attribute__ ((aligned(8)));

public:
    // Task function
	void Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);

	//Task create function (C-style)
	friend PxTask_t Task_C0_Communication_Create(PxUChar_t prio, PxEvents_t actevents);

} __attribute__ ((aligned(64)));

/*****************************************************************************************************
 * Explicit template class instantiations - required due to compiler weakness
 *****************************************************************************************************/

//template class CRingBuffer<uint8_t, 20>;

/*****************************************************************************************************
 * Task object(s)
 *****************************************************************************************************/

#pragma section ".Task_C0_Communication"
	CTask_C0_Communication Task_C0_Communication_obj;
#pragma section




/*****************************************************************************************************
 * Local storage of task id. Faster and more reliable than nameserver
 *****************************************************************************************************/

#pragma section ".Task_Init_Lock.data" awB 8
	static PxTask_t Task_C0_Communication_id={0};
#pragma section

PxTask_t Task_C0_Communication_getTaskId()
{
	return Task_C0_Communication_id;
}

/*****************************************************************************************************
 * Create function
 *****************************************************************************************************/

		
static const PxProtectRegion_T Task_C0_Communication_Regions[] = {

#ifdef RTE_TIMING_ANALYSIS
	//GPT timer peripheral register read access
	{ (PxUInt_t) DRV_GPT_BEGIN, (PxUInt_t) DRV_GPT_END, (PxProtectType_t)ReadProtection},

	//UART 0 for RTE Timing data
	{ (PxUInt_t) PORT_UART_0_BEGIN, (PxUInt_t) PORT_UART_0_END, (PxProtectType_t)WRProtection},

#endif //RTE_TIMING_ANALYSIS


	//Shared data between the tasks
	{ (PxUInt_t) __GLOBAL_TASK_BEGIN, (PxUInt_t) __GLOBAL_TASK_END, (PxProtectType_t)WRProtection},

	//Stores global data from UART and CAN for this task
	{ (PxUInt_t) __CPU0_DATAPOOL_BEGIN, (PxUInt_t) __CPU0_DATAPOOL_END, (PxProtectType_t)WRProtection},

	//Stores data which is created by the Inittask, e.g. task and message Id's
	{ (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN, (PxUInt_t) __GLOBAL_RW_INIT_RO_AFTERWARDS_END, (PxProtectType_t)ReadProtection},


	//Signal Layer Safety Signals
	//HACK: temporarily required to access lidar scan signal. PF
	//{ (PxUInt_t) __CPU1_SL_CONTROL_BEGIN, (PxUInt_t) __CPU1_SL_CONTROL_END, (PxProtectType_t)WRProtection},

	//Signal Layer Safety Signals
	{ (PxUInt_t) __CPU0_SL_SAFETY_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_END, (PxProtectType_t)WRProtection},

	//Signal Layer Safety State Signals
	{ (PxUInt_t) __CPU0_SL_SAFETY_STATE_BEGIN, (PxUInt_t) __CPU0_SL_SAFETY_STATE_END, (PxProtectType_t)ReadProtection},



	//UART 1 for Zigbee/Remote data PC Control
	{ (PxUInt_t) PORT_UART_123_BEGIN, (PxUInt_t) PORT_UART_123_END, (PxProtectType_t)WRProtection},

	//UART 4 for Lidar Collission Signal
	{ (PxUInt_t) PORT_UART_45_BEGIN, (PxUInt_t) PORT_UART_45_END, (PxProtectType_t)WRProtection},

	//CAN Module for CanOpen
	{ (PxUInt_t) MP_CAN_M_BEGIN, (PxUInt_t) MP_CAN_M_END, (PxProtectType_t)WRProtection},

	//The RTE ring bufer for the tasks belongs to Core 1 is in this region, Canopen data dictionary for control tasketc.
	//Todo: to be replaced by messages later on
	{ (PxUInt_t) __CPU1_DATAPOOL_BEGIN, (PxUInt_t) __CPU1_DATAPOOL_END, (PxProtectType_t)WRProtection},

	//End of list
	{0,0,(PxProtectType_t)0}
};



void Task_C0_Communication_Entry(PxTask_t myId, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{

        Task_C0_Communication_obj.Task_Func(myId,myMailbox,myActivationEvents);
}


extern "C" {
PxTask_t Task_C0_Communication_Create(PxUChar_t prio,PxEvents_t events)
{

	PxTaskContext_T task_Context;
	PxTaskSpec_T task_Spec;
	PxBzero((PxUChar_t *)&task_Spec,sizeof(task_Spec));

	//Protection [0] = 0 --> Inherited from the Owner.
	task_Context.protection[0].lowerBound 	= 0,
	task_Context.protection[0].upperBound 	= 0,
	task_Context.protection[0].prot 		= NoAccessProtection;

	//Task stack memory
	task_Context.protection[1].lowerBound 	= (PxUInt_t)&Task_C0_Communication_obj,
	task_Context.protection[1].upperBound 	= (PxUInt_t)&Task_C0_Communication_obj.__protectionPad[0],
	task_Context.protection[1].prot 		= WRProtection;

	task_Spec.ts_name						= (const PxChar_t*)"Task_C0_Communication",
	task_Spec.ts_fun						= Task_C0_Communication_Entry,
	task_Spec.ts_mc							= PXMcTaskdefault,
	task_Spec.ts_opool						= PXOpoolTaskdefault,
	task_Spec.ts_privileges					= PXUser1Privilege,
	task_Spec.ts_accessrights				= 0| PXACCESS_HANDLERS| PXACCESS_INSTALL_HANDLERS| PXACCESS_INSTALL_SERVICES| PXACCESS_REGISTERS| PXACCESS_SYSTEMDEFAULT| PXACCESS_RESOURCES| PXACCESS_NEW_RESOURCES| PXACCESS_SYSTEM_CONTROL| PXACCESS_MODEBITS| PXACCESS_OVERRIDE_ABORT_EVENTS,
	task_Spec.ts_context					= &task_Context,
	task_Spec.ts_protect_region  			= Task_C0_Communication_Regions,
	task_Spec.ts_taskstack.stk_type 		= PXStackFall,
	task_Spec.ts_taskstack.stk_size 		= PXStackDontCheck,
	
	task_Spec.ts_taskstack.stk_src.stk = &Task_C0_Communication_obj.Stack[TASK_C0_COMMUNICATION_STACKSIZE],
	task_Spec.ts_inttaskstack.stk_type 		= PXStackFall;
	task_Spec.ts_inttaskstack.stk_size 		= PXStackDontCheck;
	task_Spec.ts_inttaskstack.stk_src.stk 	= &Task_C0_Communication_obj.InterruptStack[TASK_C0_COMMUNICATION_INTR_STACKSIZE];
	task_Spec.ts_abortstacksize				= 0;

	Task_C0_Communication_id = PxTaskCreate(PXOpoolTaskdefault,&task_Spec,prio,events);
    return Task_C0_Communication_id;
}
} //extern C


/*****************************************************************************************************
 * Task Function
 *****************************************************************************************************/

extern "C" {


/******************************* ISR for RTE Timing *************************************************/
//UART 0 USB
void UART_RTETIME_ASCLIN0_RxISR(PxArg_t arg)
{

	//Get the handle to the task receiving the event
	PxTask_t taskId = PxTaskIdSet(arg);

	//Store data and check for EOP

	bool fullProtocol = false;
	bool result = UARTPROT_rte_receive_isr(&UARTPROT_rteRxBuffer, fullProtocol) ;
	if (result && fullProtocol)
	{
		//ev_Task_C0_State_6_FinishedComInit
		PxTaskSignalEvents_Hnd(taskId,ev_Task_C0_Communication_1_Uart_RteTiming_Rx_completeProtocol );
	}

	if (!result)
	{
		//Something went wrong, let's clear the ringbuffer
		UARTPROT_rteRxBuffer.init(0x0d,0x0e);
		//Todo: better solution required - job for 2020
	}

}

/******************************* ISR for remote / zigbee *********************************************/
// ASCLIN 2
// TX  - P2.0
// RX  - P2.1
// CTS - P10.7

void UART_REMOTE_ASCLIN2_RxISR(PxArg_t arg)
{

	//Get the handle to the task receiving the event
	PxTask_t taskId = PxTaskIdSet(arg);


	//Store data and check for EOP
	bool fullProtocol = false;
	bool result = UARTPROT_remote_receive_isr(&UARTPROT_remoteRxBuffer, fullProtocol);

	if (result && fullProtocol)
	{
		PxTaskSignalEvents_Hnd(taskId, ev_Task_C0_Communication_0_Uart_Remote_Rx_completeProtocol);
	}

	if (!result)
	{
		//Something went wrong, let's clear the ringbuffer
		UARTPROT_remoteRxBuffer.init(0x0d,0x0e);
		//Todo: better solution required - job for 2020
	}



}

void UART_REMOTE_ASCLIN2_TxISR(PxArg_t arg)
{

	UARTPROT_remote_transmit_isr(&UARTPROT_remoteTxBuffer);
}

/******************************* ISR for Lidar Collission / Heartbeat  *************************************************/
// ASCLIN 1
// TX  - P2.2
// RX  - P2.3

void UART_LIDAR_ASCLIN1_RxISR(PxArg_t arg)
{
	//Get the handle to the task receiving the event
	PxTask_t taskId = PxTaskIdSet(arg);

	//Only a 1 byte protocol is received, therefore the value will be stored directly in the corresponding signal
	uint8_t data = 0;

	RC_t result = UART_ReadData(uart2, &data);
	UINT8_set_raw(&so_in_lidarCollision, data);

	//Event is required to trigger the collission runnable
	PxTaskSignalEvents_Hnd(taskId, ev_Task_C0_Communication_3_Uart_Rx_Lidar);
}


/******************************* ISR for CanOpen *********************************************************************/

void CAN_RxISR(PxArg_t arg){

	(void)arg;

	//RX Soft-Mailbox ID
	uint8_t __RXMbx_i=0;

	//Buffer for RX frame in AURIX format
	CAN_RX_t __rx_buf;

	//get RX frame
	if(CAN_Frame_Read(&__rx_buf,&__RXMbx_i)!=RC_SUCCESS){
		LOGGING_SEND(LOGGING_LEVEL_ERROR,"CAN_RX_ISR", "CAN RX Frame could not be read");
		return;
	}

	//call mailbox handler
	if(CANopen_Rx_Mailboxes[__RXMbx_i].callback!=NULL)
		CANopen_Rx_Mailboxes[__RXMbx_i].callback((CANopen_CAN_MSG_t*)&__rx_buf.frame);

	//done
	return;

//
//	//get RX frame
//	if(CAN_Frame_Read(&__rx_buf)!=RC_SUCCESS){
//		LOGGING_SEND(LOGGING_LEVEL_ERROR,"CAN_RX_ISR", "CAN RX Frame could not be read");
//		return;
//	}
//
//	//find matching handler (because we have a common ISR for all mailboxes)
//	while(CANopen_Rx_Mailboxes[__RXMbx_i].Node!=CANOPEN_EOR_ID){
//		switch(CANopen_Rx_Mailboxes[__RXMbx_i].range){
//			case CANopen_range_fct:
//				if((__rx_buf.frame.MsgID&CANOPEN_FCT_MSK)==CANopen_Rx_Mailboxes[__RXMbx_i].fctCODE){
//					CANopen_Rx_Mailboxes[__RXMbx_i].callback((CANopen_CAN_MSG_t*)&__rx_buf.frame);
//					return;
//				}
//				break;
//
//			case CANopen_range_node:
//			case CANopen_range_explicit:
//				if(__rx_buf.frame.MsgID==CANopen_Rx_Mailboxes[__RXMbx_i].fctCODE){
//					CANopen_Rx_Mailboxes[__RXMbx_i].callback((CANopen_CAN_MSG_t*)&__rx_buf.frame);
//					return;
//				}
//				break;
//
//			//invalid configuration in #CANopen_Rx_Mailboxes
//			default:
//				return;
//		}
//		//iterate to next entry
//		__RXMbx_i++;
//	}
//
//	LOGGING_SEND(LOGGING_LEVEL_WARNING,"CAN_RX_ISR", "No matching handler could be found for message to 0x%04x",__CO_rx.COBid);
}

void CAN_TxISR(PxArg_t arg){

	CAN_Frame_t next_frame;
	//try to get next Frame from the Ringbuffer
	if(CAN_TX_BUF.readData(next_frame)==TRUE)
		CAN__frame_write_HW(&next_frame);

}


} //extern C

/******************************* Task Function *********************************************************************/


void CTask_C0_Communication::Task_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	// Turn off warning
	(void) myID;
	(void) myActivationEvents;
	RC_t err;

//=============================================== Data Initialisations ==================================================

	//Get the state machine task
	PxTask_t receiverTask = Task_C0_State_getTaskId();

	//Initialisation of communication buffer
	//Needs to be done here as the C++ object cannot be set in main.c
	UARTPROT_remote_init();
	UARTPROT_rte_init();

	//Explicit initialisation of the safety signals - should be done here because signals are first filled by communication services
	SL_SAFETY_init();

//=============================================== ISR Installations ======================================================

	// This function will initialise all interrupts for this task as defined in interrupt_cfg.cpp
	if ((err = INT_osInstallIsrTable(INT_TID_C0_COMMUNICATION, (PxArg_t) myID.id)) != RC_SUCCESS)
	{
		DET_stop(AUTOCORE, INTERRUPTVALIDATION, INT_TID_C0_COMMUNICATION);
	}

	//RTE Time through USB
	//InterruptRouter_InstallISR(UART_ASCLIN0_RxISR, (PxArg_t) myID.id);

	//Remote through UART1
	//InterruptRouter_InstallISR(UART_ASCLIN1_RxISR, (PxArg_t) myID.id);
	//InterruptRouter_InstallISR(UART_ASCLIN1_TxISR, (PxArg_t) myID.id);

	//Lidar collission / heartbeat through UART4
	//InterruptRouter_InstallISR(UART_REMOTE_ASCLIN2_RxISR, (PxArg_t) myID.id);




	//InterruptRouter_InstallISR(CAN_RxISR, (PxArg_t) myID.id);
	//InterruptRouter_InstallISR(CAN_TxISR, (PxArg_t) myID.id);

	//Event variable set in the ISR's
	PxMsgEvent_t EveMsg = {0};


//=============================================== Check NMT status (other boards) ========================================

#ifdef CONFIG_SMARTPOWER
    //TODO: Hack to wait until all power domains are up and running
    int count = 0;
    do
    {
    	RC_t result = CANOPEN_read_PowerStateFromSmartPower(&CODD_SMARTPOWER_powerState,0);
    	count++;
    }while (CODD_SMARTPOWER_powerState != 0x3C && count < 10);
    if (count >= 10)
    {
    	//Not all power domains reported ok state
    	DET_stop(AUTOCORE, SMARTPOWER_POWERDOMAIN_FAIL, CODD_SMARTPOWER_powerState);
    }

    //Let's check the NMT state
    NMT_refresh(&so_in_NMT);

#endif

    //Signal initialisation has finished
    PxTaskSignalEvents(receiverTask, ev_Task_C0_State_0_NMTHasBooted);


//=============================================== Initialise the other devices (other boards) =============================

	//Call all device runnable which require the communication system to be up and running
	RT_initRunExecute(RT_C0_communication_initRunTable, RT_C0_communication_initRunTable_size);

	//Signal initialisation has finished
    PxTaskSignalEvents(receiverTask, ev_Task_C0_State_1_DevicesHaveBooted);


//=============================================== Disconnect signals in case of simulation =============================================
#ifdef CONFIG_DISCONNECT_CAN
    so_out_engine.discon_raw_value = TRUE;
    so_in_engineState.discon_raw_value = TRUE;
    so_out_engineState.discon_raw_value = TRUE;
    so_inout_lidarConfig.discon_raw_value = TRUE;
    so_targetspeedToLidar.discon_raw_value = TRUE;
    so_in_NMT.discon_raw_value = TRUE;
    so_in_lidarscan.discon_raw_value = TRUE;
#endif

//=============================================== Start normal task operation =============================================

	while (1)
	{


		EveMsg = PxMsgReceive_EvWait(myMailbox,
							   ev_Task_C0_Communication_0_Uart_Remote_Rx_completeProtocol |
				               ev_Task_C0_Communication_1_Uart_RteTiming_Rx_completeProtocol |
				               ev_Task_C0_Communication_2_Can_Rx_SDO |
				               ev_Task_C0_Communication_3_Uart_Rx_Lidar |
							   ev_Task_C0_Communication_4_Can_Rx_PDO 
							   
				               );

//=============================================== Check for messages ======================================================
		if (PxMsgIdIsValid(EveMsg.msg))
		{
			//Message detected, let's call the generic message handler for this task
			RC_t result = MSG_receive(&EveMsg.msg, MSG_portList_C0_Communication, MSG_portList_C0_Communication_size);
		}

//=============================================== Check for events fired from the ISR =====================================
#ifdef RTE_TIMING_ANALYSIS
		//RTE timimg protocol received on UART USB
		if((EveMsg.events & ev_Task_C0_Communication_1_Uart_RteTiming_Rx_completeProtocol) == ev_Task_C0_Communication_1_Uart_RteTiming_Rx_completeProtocol)
		{
			uint8_t rteTimeData[RTE_TIME_FULL_PROTOCOL_SIZE] = {0};
			uint8_t idx = 0;
			uint8_t size = UARTPROT_rteRxBuffer.getFilledCount();
			for(idx = 0; idx < size; idx++)
			{
				UARTPROT_rteRxBuffer.readData(rteTimeData[idx]);
			}
			RTE_TIME_checkAndSetComCommand(rteTimeData);
		}
#endif

		//Remote protocol received on UART 1
		if((EveMsg.events & ev_Task_C0_Communication_0_Uart_Remote_Rx_completeProtocol) == ev_Task_C0_Communication_0_Uart_Remote_Rx_completeProtocol)
		{
			asm("nop");
			CARDATAPROTOCOL_set_raw(&so_in_remote, &UARTPROT_remoteRxBuffer);
		}

		//Lidar collision
		//As only one byte is received, the storage is directly done in the ISR

//=============================================== Call of event triggered runnables =========================================

		RT_safety_t result = RT_eventRunSafetyExecute(RT_C0_communication_eventRunTable, RT_C0_communication_eventRunTable_size, (uint32_t)EveMsg.events,receiverTask, myID, eRTE_TIME_taskC0CommunicationTimings_idx);

	}
}


