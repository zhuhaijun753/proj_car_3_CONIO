/**
* \file Interrupts_cfg.cpp
* \author Dinh / Fromm
* \date 20.5.2018
*
* \brief Interrupt configuration for preOS and OS (PCROS) operations
*
* This file provides the tables and taskId's for the preOs and OS interrupt configuration
*
* *
* \note\n
* 		PreOS Configuration
* 			- Modify / Extend the tables in the CPP File
*
* 		OS COnfiguration
* 			- Modify / Extend the tables in the CPP File
* 			- Add Task Identifiers to the enum as required
*
*  Changelog:\n
*  - 1.0; 20.8.2018; Dinh
*            - Initial version. preOs part tested and working
*  - 1.1; 26.5.2018; Fromm
*  			 - Added PxHandler for OS configuration
*  			 - Modified OS to table call
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

#include "Interrupts_cfg.h"
//#include "Task_C0_Communication.h"
#include "register.h"
#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

  /*
   * SRC registers
   * ================ UART & ASCLIN SRC register===========
   * ===================RX - REGISTERS====================
   * SRC_ASCLIN0RX		- UARTusb RX
   * SRC_ASCLIN1RX		- UART1RX or UART2RX or UART3RX
   * SRC_ASCLIN2RX		- UART4RX or UART5Rx
   * SRC_ASCLIN3RX		- UART6RX
   * ===================TX- REGISTERS=====================
   * SRC_ASCLIN0TX		- UARTusb TX
   * SRC_ASCLIN1TX		- UART1TX or UART2TX or UART3TX
   * SRC_ASCLIN2TX		- UART4TX or UART5Tx
   * SRC_ASCLIN3TX		- UART6TX
   * ===================ERROR REGISTERS====================
   * SRC_ASCLINERR		- UARTusb ERR
   * SRC_ASCLIN1ERR		- UART1ERR or UART2ERR or UART3ERR
   * SRC_ASCLIN2ERR		- UART4ERR or UART5ERR
   * SRC_ASCLIN3ERR		- UART6TERR
   * =======================================================
   * ===================== CAN SRC REGISTER=================
   * NOTE: CAN MODULE HAS 16 INTERRUPT REGISTERS AND 4 CAN NODES
   * 		FOR EACH NODE 4 INT REGISTERS IS ALLOCATED
   * ===================CAN NODE 0 SRC REGISTER===========================
   * SRC_CANINT0		CAN0 - TX
   * SRC_CANINT1		CANO - RX
   * SRC_CANINT2		CAN0 - ERR
   * SRC_CANINT3		CANO - RESERVED
   * ===================CAN NODE 1 SRC REGISTER===========================
   * SRC_CANINT4		CAN1 - TX
   * SRC_CANINT5		CAN1 - RX
   * SRC_CANINT6		CAN1 - ERR
   * SRC_CANINT7		CAN1 - RESERVED
   * ===================CAN NODE 2 SRC REGISTER===========================
   * SRC_CANINT8		CAN2 - TX
   * SRC_CANINT9		CAN2 - RX
   * SRC_CANINT10		CAN2 - ERR
   * SRC_CANINT11		CAN2 - RESERVED
  * ===================CAN NODE 3 SRC REGISTER===========================
   * SRC_CANINT12		CAN3 - TX
   * SRC_CANINT13		CAN3 - RX
   * SRC_CANINT14		CAN3 - ERR
   * SRC_CANINT15		CAN3 - RESERVED
   */
  /*
   * =========================RULES TO BE FOLLOWED======================
   * 1.The priority is given by the position in the array - high position == high priority
   * 2. CONFIGURE THE TABLE IN ASCENDING ORDER TO AVOID CONFUSIONS
   * 3. CONFIGURE THE TABLE IN ORDER OF CORES TO AVOID CONFUSION
   * 4. All ISR handlers must be declared with "extern "C" linkage"
   */
extern void Sample_Timer_Isr(PxArg_t  inputArg);
extern void TX_UART_RX_Isr(PxArg_t  inputArg);
extern void TX_UART_TX_Isr(PxArg_t  inputArg);
extern void RX_UART_RX_Isr(PxArg_t  inputArg);
extern void RX_UART_TX_Isr(PxArg_t  inputArg);
extern void UART_Err_Isr(PxArg_t  inputArg);


//####################### PreOs / Bare Metal Interrupt Configuration


 /*
  * 			SRC register
  * 				|				ISR function
  * 				|					|			   Core
  * 				|					|				|
  * 				|					|				|
  * 				|					|				|
  */
const INT_preOsIsrEntry_t INT_preOsTable[] = {
		//Low priority
		//{&SRC_GPT120T2, 		&Sample_Timer_Isr,	cpu0 }, // Timer priority 1
		//{&SRC_ASCLIN2RX,		&TX_UART_RX_Isr,	cpu0 },	// TX UART's RX priority 2
		//{&SRC_ASCLIN2TX,		&TX_UART_TX_Isr,	cpu0 },	// TX UART's TX riority 3
		//{&SRC_ASCLIN3RX,		&RX_UART_RX_Isr,	cpu0 },	// RX UART's RX riority 4
		//{&SRC_ASCLIN3TX,		&RX_UART_TX_Isr, 	cpu0 },	// TX UART's TX riority 5
		//High Priority
		{0,0,(CpuId_t)0}												// end of table
};


//####################### Os Interrupt Configuration


extern void CAN_RxISR(PxArg_t arg);
extern void CAN_TxISR(PxArg_t arg);

extern void QSPI0_txISR(PxArg_t arg);
extern void QSPI0_ptISR(PxArg_t arg);
extern void QSPI0_errorISR(PxArg_t arg);

extern void UART_REMOTE_ASCLIN2_RxISR(PxArg_t arg);
extern void UART_REMOTE_ASCLIN2_TxISR(PxArg_t arg);

void UART_LIDAR_ASCLIN1_RxISR(PxArg_t arg);

/**
 * The tables must be called in every task requiring interrupts
 */

/*
 * 			SRC register
 * 				|				ISR function
 * 				|					|			   			Contexthandler
 * 				|					|						 |                                Task
 * 				|					|						 |                                |
 * 				|					|						 |                                |
 */
const INT_osIsrEntry_t INT_osTable[] = {
		//Low Priority
		{&SRC_ASCLIN2RX,		UART_REMOTE_ASCLIN2_RxISR,	INT_FASTCONTEXTHANDLER,			INT_TID_C0_COMMUNICATION },			//Joystick Zigbee (XBee Module 1, Uart 4, ASCLIN 2 and some pins
		{&SRC_ASCLIN2TX,		UART_REMOTE_ASCLIN2_TxISR,	INT_FASTCONTEXTHANDLER,			INT_TID_C0_COMMUNICATION },

		{&SRC_ASCLIN1RX,		UART_LIDAR_ASCLIN1_RxISR,	INT_FASTCONTEXTHANDLER,			INT_TID_C0_COMMUNICATION }, 		//Lidar AntiCollission Signal

		//{&SRC_ASCLIN3RX,		0,					cpu0 },	// RX UART's RX riority 4
		//{&SRC_ASCLIN3TX,		0,					cpu0 },	// TX UART's TX riority 5
		//{&SRC_GPT120T2, 		0,					cpu0 }, // Timer priority 1
		{&SRC_CANINT0,			CAN_TxISR,					INT_FASTCONTEXTHANDLER, 		INT_TID_C0_COMMUNICATION}, 			//CAN TX
		{&SRC_CANINT1,			CAN_RxISR,					INT_FASTCONTEXTHANDLER,			INT_TID_C0_COMMUNICATION }, 		//CAN RX

		{&SRC_QSPI0ERR,         &QSPI0_errorISR,    		INT_FASTCONTEXTHANDLER, 		INT_TID_C2_TFT},					//QSPI for TFT display
		{&SRC_QSPI0TX,          &QSPI0_txISR,       		INT_FASTCONTEXTHANDLER, 		INT_TID_C2_TFT},
		{&SRC_QSPI0PT,          &QSPI0_ptISR,       		INT_FASTCONTEXTHANDLER, 		INT_TID_C2_TFT},

		//High Priority
		{0,0,(INT_handler_t)0,(INT_osIsrTaskId_t)0}												// end of table
};




//Note: Systick currently runs on prio 33 ==> see system configuration.c

#ifdef __cplusplus
  }
#endif /* __cplusplus */
