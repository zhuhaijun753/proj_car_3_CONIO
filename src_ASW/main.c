/**
 * \author	Peter Fromm 	Hochschule Darmstadt
 * \date 	12.12.2016
 * \version	0.1
 *
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

#include "global.h"

#include "system.h"
#include "PORT.h"
#include "UART.h"
#include "CANopen.h"
#include "CFG_check.h"
#include "QSPI.h"

#include "interrupts_cfg.h"

#include "det.h"
#include "sync.h"
#include "task_init.h"

#include "rte_time.h"
#include "det_time.h"

int main(){

	// Storage for the result of PxInit
	PxError_t 	PxInit_ret = PXERR_NOERROR;

	//storage for the current CPU-ID
	CpuId_t 	CoreID = SYSTEM_GetCoreId();

	// Core 0 performs driver initialization
	if(CoreID == cpu0)
	{

		PORT_Init();

		//Start PreOs Interrupts
		//INT_preOsStart(INT_preOsTable, INT_preOsTableSize);

		//RTE_TIME_TimerInit();
		//DET_TIME_init();

		//check integrity of system configuration
		if(CFG_check()!=RC_SUCCESS)
			DET_stop(AUTOCORE,CFG_CHECK, 0);

		//system Init (PLL setup, DTS init and TLF shutdown)
		if(SYSTEM_Init()!=RC_SUCCESS)
			DET_stop(AUTOCORE,SYSTEM_INIT, 0);

		//Driver Init
		UART_init();

		//SPI Init for TFT
		if (RC_SUCCESS != QSPI_module_init(QSPI0, NULL))
			DET_stop(AUTOCORE, QSPI_MODULE_INIT, 0);

		//CANopen_Init which will initialize CAN
		if(CANopen_Init()!=RC_SUCCESS)
			DET_stop(AUTOCORE,CAN_INIT, 0);

		//Initialize core synchronization
		SYNC_Init();
		_nop();

		//Stop PreOs Interrupts
		//INT_preOsStop();

		//Setup OS Interrupt nodes
		INT_osSetConfig(INT_osTable);
	}

	//Initialize PxROS
	switch(CoreID)
	{
		case cpu0:
			//Initialize PXROS on core0
			PxInit_ret = PxInit(InitSpecsArray,3);
			break;

		case cpu1:
			//Initialize PXROS on core1
			PxInit_ret = PxInit(InitSpecsArray,3);
			break;

		case cpu2:
			//Initialize PXROS on core2
			PxInit_ret = PxInit(InitSpecsArray,3);
			break;
	}

	//in case PxROS was not initialized, the return value will be processed by DET
	if (PxInit_ret != PXERR_NOERROR)
		DET_stop(CoreID,PXBOOTSYNC1,PxInit_ret);

	//this line should be unreachable, but you never know...
	while(1)
		asm("NOP");

	//as the return type of main is int, we have to return something. The result is not evaluated.
	return 1;

}
