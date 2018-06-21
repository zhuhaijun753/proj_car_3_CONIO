/*
 * Gpt_timer.h
 *
 *  Created on: Jun 9, 2016
 *      Author: Shriki
 */

#ifndef SRC_BSW_DRV_GPT_GPT_TIMER_H_
#define SRC_BSW_DRV_GPT_GPT_TIMER_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "system.h"
#include "global.h"
#include "Gpt_timer_config.h"
#include "Gpt_timer_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define	DRV_GPT_BEGIN	((PxUInt_t)&MODULE_GPT120)
#define	DRV_GPT_END		((PxUInt_t)&MODULE_GPT120 + sizeof(Ifx_GPT12))
/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/*
 * Configure Gpt Timer module
 * @param 	*pStrGptConfig  pointer to Gpt module control register configuration
 * @return	GptError
 * */
void GPT12_Config(const GPT12_Config_t* pStrGPTConfig);

/*
 * Configure Gpt Timer module
 * @param 	*pStrGptConfig  pointer to Gpt module control register configuration
 * @return	GptError
 * */
void GPT12_Init(GPT12_Module_t eGptModule);

/*
 * Configure Gpt Timer module
 * @param 	*pStrGptConfig  pointer to Gpt module control register configuration
 * @return	GptError
 * */
void GPT12_Reload(GPT12_Module_t eGptModule);

/*
 * Start or stop GPT timer module
 * @param 	eGptModule Module name
 * @param 	startStop  enum start or stop
 * @return 	void
 * */
void GPT12_StartStop(GPT12_Module_t eGptModule, GPT12_OnOff_t startStop);

/*
 * Reads the timer register and returns the read value.
 * @param    eGptModule - Module name
 * @return   Timer read value
 */
extern uint32_t GPT12_TimerRead(GPT12_Module_t eGptModule);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SRC_BSW_DRV_GPT_GPT_TIMER_H_ */
