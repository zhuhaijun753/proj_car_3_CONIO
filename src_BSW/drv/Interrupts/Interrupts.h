/**
* \file Interrupts.h
* \author Dinh / Fromm
* \date 20.5.2018
*
* \brief Intererupt driver for preOS and OS (PCROS) operations
*
* This file provides API functions to configure the ISR system based on tables provided in the
* corresponding configuration file.
*
* \note <notes>
* \todo <todos>
* \warning <warnings, e.g. dependencies, order of execution etc.>
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


#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "Interrupts_cfg.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros and type declarations                 */
/*****************************************************************************/

//####################### Defines/Macros


#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

//####################### Enumerations


//####################### Structures

// Wrapper to allow representing the file in Together as class
#ifdef TOGETHER

class INT
{
public:
#endif /* Together */

/*****************************************************************************/
/* Extern global variables                                                   */
/*****************************************************************************/



/*****************************************************************************/
/* API functions                                                             */
/*****************************************************************************/

//####################### PreOs / Bare Metal Interrupt Configuration

/**
 * This function sets the interrupt table to be used
 * \param INT_preOsIsrEntry_t* pTable - Pointer to the preOS or bare metal table
 * \return error code
 *               RC_SUCCESS everything ok
 *               RC_ERROR_INVALID_STATE if tables already has been set
 */
RC_t INT_preOsSetConfig(INT_preOsIsrEntry_t* pTable);

/**
 * This function configures the service request nodes and interrupt vector table based on the previously set table
 * \param void
 * \return error code
 *               RC_SUCCESS everything ok
 *               RC_ERROR_NULL if no tables has been set
 *               RC_ERROR_OVERRUN No null termination found or table too big
 */
RC_t INT_preOsInstallIsrTable();

/**
 * This function clears the service request nodes and interrupt vector table
 * \note This function has to be called before reconfiguring the interrupt system or before starting the OS
 * \return error code
 *               RC_SUCCESS everything ok
 *               RC_ERROR_OVERRUN No null termination found or table too big
 */
RC_t INT_preOsStop(void);

//####################### OS Configuration

/**
 * This function sets the interrupt table to be used. This function must be called before any interrupt is configured
 * \param INT_osIsrEntry_t* pTable - Pointer to the OS table
 * \return error code
 *               RC_SUCCESS everything ok
 *               RC_ERROR_INVALID_STATE if tables already has been set
 */
RC_t INT_osSetConfig(INT_osIsrEntry_t* pTable);

/**
 * This function configures the service request nodes and interrupt vector table based on the previously set table
 * \param INT_osIsrTaskId_t taskId Task Identifier from the INT_configuration. Based on this identifier, only those ISR's will
 *                                 be installed whcih belong to the task.
 * \return error code
 *               RC_SUCCESS everything ok
 *               RC_ERROR_NULL if no tables has been set
 *               RC_ERROR_OVERRUN No null termination found or table too big
 */
RC_t INT_osInstallIsrTable(INT_osIsrTaskId_t taskId, PxArg_t argument);


/**
 * This function configures the service request node and interrupt vector table in a task context
 * The function will be called by INT_osInstallIsrTable and usually is not called from user side,
 * unless a ISR should be reconfigured during runtime.
 * \param Ifx_SRC_SRCR* serviceRequestNode the service request node which needs to be configured
 * \param INT_isr_t pIsr point to the ISR function
 * \param INT_handler_t handlerType type of hanlder to be installed. Check the enum description for details
 * \return error code
 *               RC_SUCCESS everything ok
 *               RC_ERROR_NULL if no tables has been set
 *               RC_ERROR in case PXROS handler installation return error code
 *               RC_ERROR_BAD_PARAM if SRC has not been found in table
 */
RC_t INT_osInstall(volatile Ifx_SRC_SRCR* serviceRequestNode, INT_isr_t pIsr, INT_handler_t handlerType, PxArg_t argument);

RC_t INT_osUnInstall();
#ifdef __cplusplus
  }
#endif /* __cplusplus */


#endif /* FILE_H */
