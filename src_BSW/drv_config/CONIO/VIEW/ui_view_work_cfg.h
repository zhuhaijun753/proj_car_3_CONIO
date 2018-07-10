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
	CONIO_TEXT_t	mLabel;
	const char_t	*mpLabel;
	CONIO_TEXT_t	mState;
}UI_NMTStateEntry_cfg_t;

typedef struct
{
	CONIO_TEXT_t	mHeader;
	const char_t	*mpHeader;
	UI_NMTStateEntry_cfg_t mLidarNMT;
	UI_NMTStateEntry_cfg_t mEngineRLNMT;
	UI_NMTStateEntry_cfg_t mEngineRRNMT;
	UI_NMTStateEntry_cfg_t mEngineFLNMT;
	UI_NMTStateEntry_cfg_t mEngineFRNMT;
}UI_NMTStateDisplay_cfg_t;

#endif /* SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_WORK_CFG_H_ */
