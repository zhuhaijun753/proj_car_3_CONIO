/**
* \file Interrupts_cfg.h
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

#ifndef INTERRUPTS_CFG_H_
#define INTERRUPTS_CFG_H_

#include "pxdef.h"
#include "global.h"
#include "register.h"

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

//####################### PreOs / Bare Metal Interrupt Configuration

/**
* \brief ISR function pointer
**/
typedef void (*INT_isr_t)(PxArg_t);


/**
* \brief structure definition of preOs Interrupt configuration table
**/
typedef struct{
	volatile Ifx_SRC_SRCR* 	SRC;			// SRC register
	INT_isr_t				pIsr;			// ISR function pointer
	CpuId_t					Core;			// Type Of service
}INT_preOsIsrEntry_t;

extern const INT_preOsIsrEntry_t INT_preOsTable[];


//####################### Os Interrupt Configuration

typedef enum  {
	INT_TID_C0_INIT,				/**< \brief Interrupt Identifier for C0 init task (GPT det timer, GPT timing analysis) */
	INT_TID_C0_COMMUNICATION,		/**< \brief Interrupt Identifier for C0 communication task (CAN, UART) */
	INT_TID_C2_COMMUNICATION,		/**< \brief Interrupt Identifier for C2 communication task (UART) */
	INT_TID_C2_TFT,					/**< \brief Interrupt Identifier for C0 communication task (QSPI) */
} INT_osIsrTaskId_t ;

/**
* \brief INT_handler_t describes the type of the OS installation function to be used for the ISR
*
* Depending on the type of the handler, different contexts and latencies do apply.
*/
typedef enum {
	INT_FASTCONTEXTHANDLER,         /**< \brief Fast interrupt handler with task context
										Should be used in most cases. A safe handler will set the task context when the ISR is executed. Instead of #INT_CONTEXTHANDLER, the handler is called immediately if the corresponding interrupt has occurred.
										\note 		The installing task requires #PXACCESS_INSTALL_HANDLERS */

	INT_CONTEXTHANDLER,         	/**< \brief Interrupt handler with task context
										A safe handler will set the task context when the ISR is executed. It is executed after the device returns from interrupt to system level.
										\note 		The installing task requires #PXACCESS_INSTALL_HANDLERS
										\note 		A local Handler is not immediately called if the corresponding interrupt has occurred; instead it is entered into a list, which is processed after returning from interrupt level to system level*/

	INT_FASTHANDLER,                /**< 	Fast Handlers are the fastest and highest prioritised interrupt Handlers. They run with system privileges(!!!) in interrupt mode and are activated as soon as an interrupt occurs.
										\note 		the installing task requires #PXACCESS_HANDLERS
										\warning 	Should only be used if you really know what you are doing. */

} INT_handler_t;

/**
* \brief structure definition Os Interrupt configuration table
**/
typedef struct{
	volatile Ifx_SRC_SRCR* 	SRC;			// SRC register
	INT_isr_t				pIsr;			// ISR function pointer
	INT_handler_t			HandlerType;	// Handler Type
	INT_osIsrTaskId_t		Task;			// Task

}INT_osIsrEntry_t;


extern const INT_osIsrEntry_t INT_osTable[];



#ifdef __cplusplus
  }
#endif /* __cplusplus */
#endif /* SRC_BSW_DRV_CONFIG_INTERRUPTROUTER_INTERRUPTROUTER_CFG_H_ */
