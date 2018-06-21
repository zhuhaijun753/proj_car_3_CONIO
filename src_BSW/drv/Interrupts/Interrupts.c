/**
* \file Interrupts.c
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

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "Interrupts.h"

#include "system.h"
#include "global.h"
#include "register.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

#define INT_MAXTABLE 		256	/* Max number of interrupt priorities, offset value will be substracted  */
#define INT_PREOSOFFSET 	1	/* First valid priority number is 1 */
#define INT_OSOFFSET 		2   /* First valid priority number is 2, as 1 is reserved by PXROS */

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */


#pragma section ".drv.isr.data"
  /**
   *  INT__preOsTable points to the currently valid preOs isr configuration table
   */
  static INT_preOsIsrEntry_t* INT__preOsTable = 0;

  /**
   *  INT__preOsTableSize contains the size of the table without trailing 0-Vector
   */
  static uint32_t INT__preOsTableSize = 0;


  /**
   *  INT__OsTable points to the currently valid Os isr configuration table
   */
  static INT_osIsrEntry_t* INT__osTable = 0;

#pragma section

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

//static void INT__setConfigBit(uint8_t priority);
//static void INT__clearConfigBit(uint8_t priority);
//static uint8_t INT__configBitIsSet(uint8_t priority);

/**
 *\brief Default handler doing nothing
 */
static void INT__DefaultHandler(PxArg_t arg);

/**
 * \brief Helper function to set SRN in supervisor mode
 */
static int INT__configSRN(uint32_t serviceRequestNode, uint32_t prio);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/


//####################### PreOs / Bare Metal Interrupt Configuration

/**
 * This function sets the interrupt table to be used
 * \param INT_isrEntry_t* table - Pointer to the preOS or bare metal table
 * \return error code
 */
RC_t INT_preOsSetConfig(INT_preOsIsrEntry_t* pTable)
{
	//Only set table if interrupts have been disabled before
	if (0 != INT__preOsTable) return RC_ERROR_INVALID_STATE;

	//Set pointer and size to internal configuration
	INT__preOsTable = pTable;

	uint32_t i = 0;
	do
	{
		if (0 == INT__preOsTable[i].SRC)
		{
			INT__preOsTableSize = i;
			break;
		}
		i++;

	}while (i < INT_MAXTABLE - INT_PREOSOFFSET);

	return RC_SUCCESS;
}

/**
 * This function configures the service request nodes and interrupt vector table based on the given configuration parameter
 * \param <Format: copy of the parameter type and name - description>
 * \return error code
 */
RC_t INT_preOsInstallIsrTable()
{
	uint8_t i = 0;

	if (0 == INT__preOsTable) return RC_ERROR_NULL;

	SYSTEM_DisableInterrupts();
	//Iterate through the table

	do
	{
		if (0 == INT__preOsTable[i].SRC)
		{
			//End of table found
			//Enable global interrupts
			SYSTEM_EnableInterrupts();
			return RC_SUCCESS;
		}
		else
		{
			//Configure the SRN
			INT__preOsTable[i].SRC->B.SRPN = i + INT_PREOSOFFSET; 		//Set up the priority
			INT__preOsTable[i].SRC->B.TOS = INT__preOsTable[i].Core; 	//Select the core
			INT__preOsTable[i].SRC->B.CLRR = 1; 						//Clear pending request
			INT__preOsTable[i].SRC->B.SRE = 1; 							//Enable the SRN
		}

		i++;

	}while (i < INT_MAXTABLE - INT_PREOSOFFSET);

	//Opps, end of table not found or table too big
	return RC_ERROR_OVERRUN;

}

/**
 * This function clears the service request nodes and interrupt vector table
 * \note This function has to be called before reconfiguring the interrupt system or before starting the OS
 * \return error code
 */
RC_t INT_preOsStop(void)
{
	uint8_t i = 0;
	//Disable global interrupts
	SYSTEM_DisableInterrupts();

	//Reset all SRN configurations
	do
	{
		if (0 == INT__preOsTable[i].SRC)
		{
			//End of table found
			INT__preOsTable = 0;
			return RC_SUCCESS;
		}
		else
		{
			//Configure the SRN
			INT__preOsTable[i].SRC->U = 0;	//Reset all registers
		}

		i++;

	}while (i < INT_MAXTABLE - INT_PREOSOFFSET);

	//Opps, end of table not found or table too big
	return RC_ERROR_OVERRUN;

}

//####################### OS Configuration


/**
 * This function configures the service request nodes for the OS operation.
 * The function has to be called before the OS starts as the SRN configuration needs to be done in supervisor mode
 * \param <Format: copy of the parameter type and name - description>
 * \return error code
 */
RC_t INT_osSetConfig(INT_osIsrEntry_t* pTable)
{
	//Only set table if interrupts have been disabled before
	if (0 != INT__osTable) return RC_ERROR_INVALID_STATE;

	//Set pointer and size to internal configuration
	INT__osTable = pTable;

	return RC_SUCCESS;
}


/**
 * This function configures the service request nodes and interrupt vector table based on the previously set table
 * \param INT_osIsrTaskId_t taskId Task Identifier from the INT_configuration. Based on this identifier, only those ISR's will
 *                                 be installed whcih belong to the task.
 * \return error code
 *               RC_SUCCESS everything ok
 *               RC_ERROR_NULL if no tables has been set
 *               RC_ERROR_OVERRUN No null termination found or table too big
 */
RC_t INT_osInstallIsrTable(INT_osIsrTaskId_t taskId, PxArg_t argument)
{
	uint8_t i = 0;
	RC_t ret = RC_ERROR;

	if (0 == INT__osTable) return RC_ERROR_NULL;

	//SYSTEM_DisableInterrupts();
	//TODO This requires supervisor priviliges and probably does also not really make sense here, as it would affect the other tasks running...

	//Iterate through the table

	do
	{
		if (0 == INT__osTable[i].SRC)
		{
			//End of table found
			//Enable global interrupts
			//SYSTEM_EnableInterrupts();
			//TODO This requires supervisor priviliges and probably does also not really make sense here, as it would affect the other tasks running...

			return RC_SUCCESS;
		}
		else
		{
			if (INT__osTable[i].Task == taskId)
			{
				//Handler shall be installed for this task
				ret = INT_osInstall(INT__osTable[i].SRC, INT__osTable[i].pIsr, INT__osTable[i].HandlerType, argument);
				if (RC_SUCCESS != ret)
				{
					//Abort installation
					return ret;
				}
			}
		}

		i++;

	}while (i < INT_MAXTABLE);

	//Opps, end of table not found or table too big
	return RC_ERROR_OVERRUN;

}


static int INT__configSRN(uint32_t serviceRequestNode, uint32_t prio)
{
	volatile Ifx_SRC_SRCR* srn = (volatile Ifx_SRC_SRCR*)serviceRequestNode;

	srn->B.SRPN = (uint8_t)prio; //Set up the priority
	srn->B.TOS = SYSTEM_GetCoreId(); //Select the core
	srn->B.SRE = 1; //Enable
	srn->B.CLRR = 1; //Clear pending request

	return 0;

}

/**
 * This function sets the interrupt handler in the task context
 * \param Ifx_SRC_SRCR* serviceRequestNode the service request node which needs to be configured
 * \param INT_isr_t pIsr point to the ISR function
 * \param uint8_t priority, every priority may only be used once.
 * \param INT_handler_t handlerType type of hanlder to be installed. Check the enum description for details
 * \return error code
 */
RC_t INT_osInstall(volatile Ifx_SRC_SRCR* serviceRequestNode, INT_isr_t pIsr, INT_handler_t handlerType, PxArg_t argument)
{
	PxError_t err;

	//Iterate through the selected table and check for the priority
	uint32_t priority = 0;

	if (0 == INT__osTable) return RC_ERROR_NULL;

	uint32_t i = 0;

	do
	{
		if (INT__osTable[i].SRC == serviceRequestNode)
		{
			//corresponding entry found
			priority = i + INT_OSOFFSET; //Todo HACK!!!!
			break;
		}
		i++;

	}while (i < INT_MAXTABLE - INT_OSOFFSET);

	if (0 == priority)
	{
		return RC_ERROR_BAD_PARAM;
	}

	//Set the IRN in supervisor mode
	_PxHndcall(INT__configSRN, PxGetId(), 2 * sizeof(uint32_t), (uint32_t)serviceRequestNode, (uint32_t)priority);


	//Configure the IRN
	//This is done in INT_osInstall
	//serviceRequestNode->B.SRPN = priority; //Set up the priority
	//serviceRequestNode->B.TOS = SYSTEM_GetCoreId(); //Select the core
	//serviceRequestNode->B.CLRR = 1; //Clear pending request
	//Depending on the handler type, call the corresponding PXROS function
	switch (handlerType)
	{
		case INT_FASTCONTEXTHANDLER:
			err = PxIntInstallFastContextHandler(priority, pIsr, argument);
			if (err != PXERR_NOERROR)
			{
				return RC_ERROR;
			}

			break;
		case INT_FASTHANDLER:
			err = PxIntInstallFastHandler(priority, pIsr, argument);
			if (err != PXERR_NOERROR)
			{
				return RC_ERROR;
			}

			break;
		case INT_CONTEXTHANDLER:
			//TODO add handler call
			return RC_ERROR_BAD_PARAM;
			break;
		default:
			return RC_ERROR_BAD_PARAM;
			break;
	}
	return RC_SUCCESS;
}

/********************/
/* Local functions  */
/********************/
/*
void INT__setConfigBit(uint8_t priority)
{
	uint8_t bitPosition = priority % 8;
	uint8_t bytePosition = priority / 8;
	INT__OSConfigBits[bytePosition] |= 1 << bitPosition;
}

void INT__clearConfigBit(uint8_t priority)
{
	uint8_t bitPosition = priority % 8;
	uint8_t bytePosition = priority / 8;
	INT__OSConfigBits[bytePosition] &= ~(1 << bitPosition);
}
uint8_t INT__configBitIsSet(uint8_t priority)
{
	uint8_t bitPosition = priority % 8;
	uint8_t bytePosition = priority / 8;
	if (INT__OSConfigBits[bytePosition] & (1 << bitPosition))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
*/
/********************/
/* Technical Stuffs */
/********************/

/*
 *\brief Default handler doing nothing
 */
static void INT__DefaultHandler(PxArg_t arg)
{
	asm("nop");
}


//This function is called whenever an Interrupt occurs
void INT_CommonDispatcher()
{
	Ifx_CPU_ICR IcrValue = {__MFCR(CPU_ICR)}; // Workaround for the C++ constructor problem
  	if ((IcrValue.B.CCPN <= INT__preOsTableSize) || (IcrValue.B.CCPN > 0))
  	{
  		if (INT__preOsTable[IcrValue.B.CCPN - 1].pIsr != NULL)
  		{
  			INT__preOsTable[IcrValue.B.CCPN - 1].pIsr(NULL);
  		}
  	}
}
// Interrupt Table Entry, the BIV points to this after startup
asm (".section .interrupttable_init_in, \"ax\", @progbits");	// Create an input section
asm("enable");													//enable interrupts
asm("svlcx");													//save lower context
asm("calla INT_CommonDispatcher");								//call common dispatcher function
asm("rslcx");													//restore lower context
asm("rfe");														//return from exception (leave ISR)
asm (".text");


#ifdef __cplusplus
}
#endif /* __cplusplus */

