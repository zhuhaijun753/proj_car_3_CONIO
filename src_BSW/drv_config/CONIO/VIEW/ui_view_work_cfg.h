/*
 * ui_view_work_cfg.h
 *
 *  Created on: Jun 23, 2018
 *      Author: dinhq
 */

#ifndef SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_WORK_CFG_H_
#define SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_WORK_CFG_H_

//-------------------------------------------------------------------- [Includes]

#include "conio.h"
#include "BASIC/conio_text.h"
#include "BASIC/conio_shape.h"

//-------------------------------------------------------------------- [Defines]

//-------------------------------------------------------------------- [Types]


typedef struct
{
	CONIO_TEXT_t	mKey;
	const char_t	*mpKey;
	CONIO_TEXT_t	mVal;

}UI_NMTState_cfg_t;



#endif /* SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_WORK_CFG_H_ */
