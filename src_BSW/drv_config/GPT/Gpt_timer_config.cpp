/*
 * Gpt_timer_config.cpp
 *
 *  Created on: Jun 9, 2016
 *      Author: Shriki
 */

#include "Gpt_timer_type.h"
#include "Gpt_timer_Config.h"

static const uint32_t GPT__DefaultReloadValue = 0;

/*
 * const Table for timer configuration
 */
GPT12_Config_t const GPT_timerConfig []={

/*		GPT_Timer    Common      Individual prescalar for Tx
		 Module		prescalar
		 |				|			   |           		 Type of mode the timer
		 |				|			   |				  is configured
		 |				|			   |					|
		 |				|			   |					|
		 |				|			   |					|	count direction
		 |				|			   |					|	   up/down
		 |				|			   |					|			|
		 |				|			   |					|			|
		 |				|			   |					|			|
		 |				|			   |					|			|	external control
																			of count direction
		 |				|			   |					|			|			|
		 |				|			   |					|			|			|
		 |				|			   |					|			|		 	|
		 |				|			   |					|			|		 	|
		 |				|			   |					|			|		 	|		  		Auxiliary timers        Auxiliary timer
		 |				|			   |					|			|		 	|		  		control status			interrupt configuration
		 |				|			   |					|			|		 	|		  		 		|                       |      Timer overflow
		 |				|			   |					|			|		 	|		  		 		|                       |      underflow output control
		 |				|			   |					|			|		 	|		  		 		|                       |           |                    Timer T3 overflow
		 |				|			   |					|			|		 	|		  				|                       |           |                     toggle latch            GPTReload Value      Type for module                                                                      |
		 |				|			   |					|			|		 	|		  		 		|                       |           |                       |                       |                  control               Type for module
		 |				|			   |					|			|		 	|		 		 		|                       |           |                       |                       |                   |                    sleep mode control         CPU Core number
		 |				|			   |					|			|		 	|		  		 		|                       |           |                       |                       |                   |                       |                       |
		 |				|			   |					|			|		 	|		  				|                       |           |                       |                       |                   |                       |                       |
		 |				|			   |					|			|		 	|		  	  			|                       |           |                       |                       |                   |                       |                       |
		 |				|			   |					|			|		 	|		  		 		|                       |           |                       |                       |                   |                       |                       |
		 |				|			   |					|			|		 	|		  		 		|                       |           |                       |                       |                   |                       |                       |
		 |				|			   |					|			|		 	|		  		 		|                       |           |                       |                       |                   |                       |                       |
		 |				|			   |					|			|		 	|		  		 		|                       |           |                       |                       |                   |                       |                       |
		 */
{	GPT12_T2, FgptDevideBy8_00, IndividualPrescale_011, TimerMode_000, CountDirUP, InputTxUDE_Disconnect, AuxControlBySoftware, AuxIntEnable, CoreGptOverFlowDisable, CoreGptToggleDisable, 3036,					 ModuleControlEnable, SleepModeRequestDisable, 	cpu0},
{	GPT12_T3, FgptDevideBy8_00, IndividualPrescale_011, TimerMode_000, CountDirUP, InputTxUDE_Disconnect, AuxControlBySoftware, AuxIntEnable, CoreGptOverFlowDisable, CoreGptToggleDisable, 3036,					 ModuleControlEnable, SleepModeRequestDisable, 	cpu0},
{	GPT12_T4, FgptDevideBy8_00, IndividualPrescale_000, TimerMode_000, CountDirUP, InputTxUDE_Disconnect, AuxControlBySoftware, AuxIntEnable, CoreGptOverFlowDisable, CoreGptToggleDisable, GPT__DefaultReloadValue, ModuleControlEnable, SleepModeRequestDisable, 	cpu0},
};
//26472 - reload value for 100ms for T3
//Const variable to store the the timer configuration table size
const uint16_t GPT_configTableSize = sizeof(GPT_timerConfig)/sizeof(GPT12_Config_t);
