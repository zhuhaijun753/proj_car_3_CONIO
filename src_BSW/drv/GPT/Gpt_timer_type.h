/*
 * Gpt_timer_type.h
 *
 *  Created on: 29.06.2016
 *      Author: Fromm
 */

#ifndef GPT_TIMER_TYPE_H_
#define GPT_TIMER_TYPE_H_


#include "global.h"

/******************************************************************************/
/*---------------------------------- MACROS ----------------------------------*/
/******************************************************************************/


#define GPT_CLEAR					(0)
#define GPT_SET						(1)


/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
/**/
typedef void(*GPT12_ISR)(void);


/*Defines different Timers in GPT Module*/
typedef enum GPT12_Module
{
	GPT12_NONE = 0,
	GPT12_T2,
	GPT12_T3,
	GPT12_T4,
	GPT12_T5,
	GPT12_T6
}GPT12_Module_t;

/*Types of Common prescalar for module clock*/
typedef enum GPT12_BPS1
{
	FgptDevideBy8_00  = 0,
	FgptDevideBy4_01  = 1,
	FgptDevideBy16_10 = 2,
	FgptDevideBy32_11 = 3,
	FgptDevide_MAX
}GPT12_BPS1_t;

/*Types of individual prescalar for Tx*/
typedef enum GPT12_TxI
{
	IndividualPrescale_000 = 0,
	IndividualPrescale_001 = 1,
	IndividualPrescale_010 = 2,
	IndividualPrescale_011 = 3,
	IndividualPrescale_100 = 4,
	IndividualPrescale_101 = 5,
	IndividualPrescale_110 = 6,
	IndividualPrescale_111 = 7,
	IndividualPrescale_MAX
}GPT12_TxI_t;

/*Type of mode the timer is configured*/
typedef enum GPT12_TxM
{
	TimerMode_000 			= 0,
	CounterMode_001 		= 1,
	GatedTimerModeGAL_010 	= 2,
	GatedTimerModeGAH_011 	= 3,
	IncInterfaceModeRD_110 	= 6,
	IncInterfaceModeED_111 	= 7,
	Mode_MAX
}GPT12_TxM_t;

/*Timer count direction up/down*/
typedef enum GPT12_CountDir
{
	CountDirUP   = 0,
	CountDirDOWN = 1
}GPT12_CountDir_t;

/*Defines timer Start/Stop type*/
typedef enum GPT12_OnOff
{
	Stop  = 0,
	Start = 1
}GPT12_OnOff_t;

/*Type for external control of count direction*/
typedef enum GPT12_ExtControl
{
	InputTxUDE_Disconnect = 0,
	InputTxUDE_Connect    = 1
}GPT12_ExtControl_t;

/*Defines Auxiliary timers control status*/
typedef enum GPT12_AuxControl
{
	AuxControlBySoftware = 0,
	AuxControlByCoreGpt  = 1
}GPT12_AuxControl_t;

/*Types for Auxiliary timer interrupt configuration*/
typedef enum GPT12_AuxInterrupt
{
	AuxIntEnable = 0,
	AuxIntDisable = 1
}GPT12_AuxINT_t;

/*Timer overflow/underflow output control*/
typedef enum GPT12_CoreOverFlow
{
	CoreGptOverFlowDisable	= 0,
	CoreGptOverFlowEnable	= 1
}GPT12_CoreOverFlow_t;

/*Timer T3 overflow toggle latch/Can be set or cleared by software*/
typedef enum GPT12_OutToggle
{
	CoreGptToggleDisable	= 0,
	CoreGptToggleEnable		= 1
}GPT12_OutToggle_t;

/*Type for Timer configuration error data*/
typedef enum Gpt12_ConfigDataError
{
	 Success		= 0,
	 InvalidModule	= 2,
	 WrongGptBSP1	= 4,
	 WrongGptTxI	= 8,
	 WrongGptTxM	= 16,
	 WrongDivider   = 32
}GptError_t;

/*Type for module control*/
typedef enum GPT12_ModuleDisableRequest
{
	ModuleControlEnable  = 0,
	ModuleControlDisable = 1

}GPT12_ModuleDisableRequest_t;

/*Type for module sleep mode control*/
typedef enum GPT12_SleepModeControl
{
	SleepModeRequestEnable  = 0,
	SleepModeRequestDisable = 1

}GPT12_SleepModeControl_t;

/*Local structure for reload value configuration*/
typedef struct GPT12_ReloadValueConfig
{
	uint16_t T2ReloadVal;
	uint16_t T3ReloadVal;
	uint16_t T4ReloadVal;
	uint16_t T5ReloadVal;
	uint16_t T6ReloadVal;
}GPT12_ReloadValueConfig_t;

/*Local Configuration structure for timer module*/
typedef struct GPT12_Config
{
	//Added after
	//boolean_t						GptEnablebit;					//timer enable bit
	GPT12_Module_t					GptModule;						//GPT Timer module
	GPT12_BPS1_t					CommonPreScalar;				//Timer common prescalar
	GPT12_TxI_t						IndividualPrescalar;			//Individual prescalar factor
	GPT12_TxM_t						GptMode;						//Different timer modes for each module
	GPT12_CountDir_t 				GptCountDir;					//Timer count direction
	GPT12_ExtControl_t 				GptCountCtrl;					//Count direction external control
	GPT12_AuxControl_t 				GptAuxCtrl;						//Auxiliary timer control
	GPT12_AuxINT_t					GptAuxINT;						//Auxiliary timer interrupt configuration
	GPT12_CoreOverFlow_t			GptCoreOverFlowEnDis;			//Timer overflow output control
	GPT12_OutToggle_t				GptCoreOutBitToggleEnDis;		//Core timer output toggle Enable/Disable
	uint32_t						GptReloadValue;					//Timer reload value
	GPT12_ModuleDisableRequest_t 	GptModuleControl;				//CLC module control request
	GPT12_SleepModeControl_t 		GptSleepMode;					//CLC sleep mode control
	CpuId_t							CPUCOREID;						//CPU core ID
}GPT12_Config_t;

/*User configuration structure for timer module*/
typedef struct GPT12_TimerModeConfig
{
	GPT12_Module_t			GptModule;								//GPT timer module(T2,T3,T4,T5,T6)
	float					FrequncyInHz;							//User input frequency
	GPT12_ISR				GptIrqHandler;							//Interrupt request handler
}GPT12_TMconfig_t;



#endif /* GPT_TIMER_TYPE_H_ */
