/*
 * Gpt_timer_config.h
 *
 *  Created on: Jun 9, 2016
 *      Author: Shriki
 */

#ifndef SRC_BSW_DRV_CONFIG_GPT_GPT_TIMER_CONFIG_H_
#define SRC_BSW_DRV_CONFIG_GPT_GPT_TIMER_CONFIG_H_

#include "global.h"
#include "Gpt_timer_type.h"

/******************************************************************************/
/*---------------------------------- MACROS ----------------------------------*/
/******************************************************************************/
#define GPT12_MAX_REG_VALUE		(0xFFFF)
#define GPT_MODULE_FREQUENCY	(100000000U)	//Todo: To be moved to derivate? To be checked with Th. Barth


#define MAX_TxI			(8)

#define DEFAULTRELOADVALUE (0)

/******************************************************************************/
/*----------------------------- Global data ----------------------------------*/
/******************************************************************************/

EXTERN GPT12_Config_t const GPT_timerConfig[];
extern const uint16_t GPT_configTableSize;

#endif /* SRC_BSW_DRV_CONFIG_GPT_GPT_TIMER_CONFIG_H_ */
