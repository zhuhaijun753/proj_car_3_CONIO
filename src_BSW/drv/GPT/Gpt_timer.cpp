/*
 * Gpt_timer.cpp
 *
 *  Created on: Jun 9, 2016
 *      Author: Shriki
 */

#include "Gpt_timer.h"
#include "register.h"
//#include "InterruptRouter.h"

/******************************************************************************/
/*---------------------------------- Local Variables--------------------------*/
/******************************************************************************/

/*!
 *  bit positions of uint8_t GptError
 *  ___ ___ ___ ___ ___ ___ ___ ___
 * |_7_|_6_|_5_|_4_|_3_|_2_|_1_|_0_|
 *   |   |   |   |   |   |   |   |_ _success/fail_bit
 *   |   |   |   |   |   |   |_ _ _ _wrongModule_bit
 *   |	 |	 |	 |	 |	 |_ _ _ _ _ _wrongBPS1_bit
 *   |	 |	 |	 |	 |_ _ _ _ _ _ _ _wrongTxI_bit
 *   |	 |	 |	 |_ _ _ _ _ _ _ _ _ _wrongTxM_bit
 *   |_ _|_ _|_ _ _ _ _ _ _ _ _ _ _ _Unused_bits
 * */
uint8_t GptError = GPT_CLEAR;

/******************************************************************************/
/*-------------------------Local Function-------------------------------------*/
/******************************************************************************/

static void GPT12_enableModule(const GPT12_Config_t* pStrGPTConfig)
{
	SYSTEM_UnlockENDINIT();             		/* sets the endinit protection back on*/
	MODULE_GPT120.CLC.U      = 0;			/*set Clock control register bit field*/
	MODULE_GPT120.CLC.B.DISR = pStrGPTConfig->GptModuleControl;  /* module disable is not requested*/
	MODULE_GPT120.CLC.B.EDIS = pStrGPTConfig->GptSleepMode;  /*sleep mode request is disregarded*/
	SYSTEM_LockENDINIT();             		/* sets the endinit protection back on*/
}

INLINE uint8_t GPT12_ReadBPS(void)
{
	 return (MODULE_GPT120.T3CON.B.BPS1);
}
/**
 * mehod	:	uint8_t GPT12_Init(GPT12_Module_t eGptModule)
 * @brief	: 	This function checks which timer is enabled and thereby
 * 				calls the function to configure the corresponding timer
 * @details	:	User needs to fill the param struct with
 * 				1)Gpt timer module
 *
 * @params	:	GPT12_Module_t eGptModule
 * @return	:	void
 */

void GPT12_Init(GPT12_Module_t eGptModule)
{
	uint16_t index;
	for (index = 0; index < GPT_configTableSize; index++)
	{
		if( eGptModule != GPT12_NONE)
		{
			GPT12_Config(&GPT_timerConfig[index]);
		}
	}
}


/******************************************************************************/
/*--------------------------Global Functions----------------------------------*/
/******************************************************************************/

/**
 * mehod	:	void Gpt12_Config(GPT12_Config_t *pStrGptConfig)
 * @brief	: 	Cofigures the GPT timer module
 * @params	:	GPT12_Config_t *pStrGptConfig
 * @return	:	void
 */
void GPT12_Config(const GPT12_Config_t* pStrGPTConfig)
{
	GPT12_enableModule(pStrGPTConfig);

	(void) MODULE_GPT120.CLC.U;
	switch(pStrGPTConfig->GptModule)
	{
		case GPT12_T2:
		{
			MODULE_GPT120.T2CON.B.T2I 		= pStrGPTConfig->IndividualPrescalar;
			MODULE_GPT120.T2CON.B.T2M 		= pStrGPTConfig->GptMode;
			MODULE_GPT120.T2CON.B.T2R 		= GPT_CLEAR; //Stop the timer initially
			MODULE_GPT120.T2CON.B.T2UD 		= pStrGPTConfig->GptCountDir;
			MODULE_GPT120.T2CON.B.T2UDE 	= pStrGPTConfig->GptCountCtrl;
			MODULE_GPT120.T2CON.B.T2RC 		= pStrGPTConfig->GptAuxCtrl;
			MODULE_GPT120.T2CON.B.T2IRDIS 	= pStrGPTConfig->GptAuxINT;
			MODULE_GPT120.T2CON.B.T2EDGE 	= GPT_CLEAR;
			MODULE_GPT120.T2CON.B.T2RDIR 	= GPT_CLEAR;
			MODULE_GPT120.T2CON.B.T2CHDIR 	= GPT_CLEAR;
			MODULE_GPT120.T2.B.T2			= pStrGPTConfig->GptReloadValue;
			//InterruptRouter_CfgSRC(&SRC_GPT120T2);
			break;
		}

		case GPT12_T3:
		{
			MODULE_GPT120.T3CON.B.T3I 		= pStrGPTConfig->IndividualPrescalar;
			MODULE_GPT120.T3CON.B.T3M 		= pStrGPTConfig->GptMode;
			MODULE_GPT120.T3CON.B.T3R 		= GPT_CLEAR; //Stop the timer initially
			MODULE_GPT120.T3CON.B.BPS1      = pStrGPTConfig->CommonPreScalar;
			MODULE_GPT120.T3CON.B.T3UD 		= pStrGPTConfig->GptCountDir;
			MODULE_GPT120.T3CON.B.T3UDE 	= pStrGPTConfig->GptCountCtrl;
			MODULE_GPT120.T3CON.B.T3OE 		= pStrGPTConfig->GptCoreOverFlowEnDis;
			MODULE_GPT120.T3CON.B.T3OTL 	= pStrGPTConfig->GptCoreOutBitToggleEnDis;
			MODULE_GPT120.T3CON.B.T3EDGE 	= GPT_CLEAR;
			MODULE_GPT120.T3CON.B.T3RDIR 	= GPT_CLEAR;
			MODULE_GPT120.T3CON.B.T3CHDIR 	= GPT_CLEAR;
			MODULE_GPT120.T3.B.T3			= pStrGPTConfig->GptReloadValue;
			//InterruptRouter_CfgSRC(&SRC_GPT120T3);
			break;
		}

		case GPT12_T4:
		{
			MODULE_GPT120.T4CON.B.T4I 		= pStrGPTConfig->IndividualPrescalar;
			MODULE_GPT120.T4CON.B.T4M 		= pStrGPTConfig->GptMode;
			MODULE_GPT120.T4CON.B.T4R 		= GPT_CLEAR; //Stop the timer initially
			MODULE_GPT120.T4CON.B.T4UD 		= pStrGPTConfig->GptCountDir;
			MODULE_GPT120.T4CON.B.T4UDE 	= pStrGPTConfig->GptCountCtrl;
			MODULE_GPT120.T4CON.B.T4RC 		= pStrGPTConfig->GptAuxCtrl;
			MODULE_GPT120.T4CON.B.T4IRDIS 	= pStrGPTConfig->GptAuxINT;
			MODULE_GPT120.T4CON.B.T4EDGE 	= GPT_CLEAR;
			MODULE_GPT120.T4CON.B.T4RDIR 	= GPT_CLEAR;
			MODULE_GPT120.T4CON.B.T4CHDIR 	= GPT_CLEAR;
			MODULE_GPT120.T4.B.T4			= pStrGPTConfig->GptReloadValue;
			break;
		}
		default:
			break;
	}
}

/**
 * mehod	:	void Gpt12_StartStop(GPT12_Module_t eGptModule, GPT12_OnOff_t startStop)
 * @brief	: 	This Starts and Stops the GPT modules.
 * 				1. Start = 1,
 * 				2. Stop  = 0
 * @params	:	GPT12_Module_t eGptModule, GPT12_OnOff_t startStop
 * @return	:	void
 */
void GPT12_StartStop(GPT12_Module_t eGptModule, GPT12_OnOff_t startStop)
{
	/*Start or stop timer*/
	switch(eGptModule)
	{
		case GPT12_T2:
		{
			MODULE_GPT120.T2CON.B.T2R = startStop;
			break;
		}
		case GPT12_T3:
		{
			MODULE_GPT120.T3CON.B.T3R = startStop;
			break;
		}
		case GPT12_T4:
		{
			MODULE_GPT120.T4CON.B.T4R = startStop;
			break;
		}
		default:
			break;
	}
}

/**
 * mehod	:	uint32_t Gpt12_TimerRead(GPT12_Module_t eGptModule)
 * @brief	: 	Reads the Current counts from the given Timer module.
 * @params	:	GPT12_Module_t eGptModule
 * @return	:	uint32_t
 */
uint32_t GPT12_TimerRead(GPT12_Module_t eGptModule)
{
	if(eGptModule == GPT12_T2)
		return ((uint32_t)MODULE_GPT120.T2.B.T2);
	else if(eGptModule == GPT12_T3)
		return ((uint32_t)MODULE_GPT120.T3.B.T3);
	else if(eGptModule == GPT12_T4)
		return ((uint32_t)MODULE_GPT120.T4.B.T4);

	return (uint32_t)0;
}

/**
 * mehod	:	void GPT12_Reload(GPT12_Module_t eGptModule)
 * @brief	: 	Reload the Timer regiter upon overflow
 * @detail	: 	This function is called by the GPT ISR
 * @params	:	GPT12_Module_t eGptModule
 * @return	:	void
 */
void GPT12_Reload(GPT12_Module_t eGptModule)
{
	if(eGptModule == GPT12_T2)
		MODULE_GPT120.T2.B.T2 = GPT_timerConfig[0].GptReloadValue;
	else if(eGptModule == GPT12_T3)
		MODULE_GPT120.T3.B.T3 = GPT_timerConfig[1].GptReloadValue;
	else if(eGptModule == GPT12_T4)
		MODULE_GPT120.T4.B.T4 = GPT_timerConfig[2].GptReloadValue;
}



