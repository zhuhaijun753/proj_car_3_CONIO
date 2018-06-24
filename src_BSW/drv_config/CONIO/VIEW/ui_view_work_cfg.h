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
#define UI_NMT_STATE_INIT_STR			"INIT"
#define UI_NMT_STATE_STOPPED_STR		"STOP"
#define UI_NMT_STATE_OPERATIONAL_STR	"OPRT"
#define UI_NMT_STATE_PREOP_STR			"PROP"

//-------------------------------------------------------------------- [Types]

typedef struct
{
	CONIO_TEXT_t 	mKey;
	const char_t	*mpKey;

}UI_NMTLabel_cfg_t;

typedef struct
{
	CONIO_TEXT_t	mLidarNMT;
	CONIO_TEXT_t	mEngineRLNMT;
	CONIO_TEXT_t	mEngineRRNMT;
	CONIO_TEXT_t	mEngineFLNMT;
	CONIO_TEXT_t	mEngineFRNMT;

}UI_NMTState_cfg_t;



#endif /* SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_WORK_CFG_H_ */
