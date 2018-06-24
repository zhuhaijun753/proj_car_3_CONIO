/*
 * ui_view_work_cfg.cpp
 *
 *  Created on: Jun 23, 2018
 *      Author: dinhq
 */
#include "ui_view_work_cfg.h"
#include "VIEW/ui_view_cfg.h"

#include "swc_ui.h"
#include "BASIC/conio_button.h"

#include <cstring>
#include <cstdlib>
//-------------------------------------------------------------------- [Defines]

#define UI_NMT_STATE_KEY_STR			"NMT :"
#define UI_NORMAL_STATE_KEY_STR 		"(.)(.):"
#define UI_SAFETY_STATE_KEY_STR 		"SS :"
#define UI_SAFETY_RUNNABLE_STR			"RUN:"
#define UI_JOYSTICK_KEY_STR 			"JS :"
#define UI_TARGET_SPEED_KEY_STR 		"TS :"
#define UI_ENGINE_KEY_STR 				"ENG:"

#define UI_NMT_STATE_VAL_WIDTH			(16) //7 + 2'()' + 2 + 1
#define UI_NORMAL_STATE_VAL_WIDTH		(16) //7 + 2'()' + 2 + 1

#define UI_SAFETY_STATE_VAL_WIDTH		(16) //7 + 2'()' + 2 + 1

//Must be aligned with string size in rte
#define UI_SAFETY_RUNNABLE_VAL_WIDTH	(20) //7 + 2'()' + 2 + 1


#define UI_JOYSTICK_LINE				(6)		/** Line position on the display */
#define UI_JOYSTICK_X_WIDTH				(4)		/** -127 to 128 **/
#define UI_JOYSTICK_Y_WIDTH				(4)		/** -127 to 128 **/
#define UI_JOYSTICK_RZ_WIDTH			(4)		/** -127 to 128 **/
#define UI_JOYSTICK_AGE_WIDTH			(5)		/** 0 to 65535 **/
#define UI_JOYSTICK_ESTOP_WIDTH			(4)		/** Y/N - only 1 required,  4 for beauty**/
#define UI_JOYSTICK_GSTOP_WIDTH			(4)		/** Y/N - only 1 required,  4 for beauty**/

#define UI_TARGET_SPEED_LINE			(5)		/** Line position on the display */
#define UI_TARGET_SPEED_X_WIDTH			(4)		/** -127 to 128 **/
#define UI_TARGET_SPEED_Y_WIDTH			(4)		/** -127 to 128 **/
#define UI_TARGET_SPEED_PHI_WIDTH		(4)		/** -127 to 128 **/
#define UI_TARGET_SPEED_AGE_WIDTH		(5)		/** 0 to 65535 **/

#define UI_ENGINE_VAL_WIDTH				(36)
#define UI_ENGINE_VAL_HEIGHT			(CONIO_TEXT_Y_INC)

#define UI_PAGE1_NEXTBUTTON_WIDTH		(72)
#define UI_PAGE1_NEXTBUTTON_HEIGHT		(CONIO_TEXT_Y_INC)

#define UI_PAGE1_TFA					(0)
#define UI_PAGE1_BFA					(24)

//-------------------------------------------------------------------- [Global data]

#pragma section ".data.ui"

/** \brief NMT state UI data */
UI_NMT_data_t			gUINMTStateWork;


#pragma section

//-------------------------------------------------------------------- [Global Configuration]

#pragma section ".rodata"

#if ((CONIO_MODE == CONIO_REV_LANDSCAPE) || (CONIO_MODE == CONIO_LANDSCAPE))

/** \brief NMT state UI config */
const UI_NMTState_cfg_t gUINMTStateCfg =
	{
		{{0, 11}, strlen(UI_NMT_STATE_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_NMT_STATE_KEY_STR,
		{{(strlen(UI_NMT_STATE_KEY_STR) + 1), 10}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI1NextPageButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGE1_NEXTBUTTON_WIDTH), 0},
			UI_PAGE1_NEXTBUTTON_WIDTH,
			UI_PAGE1_NEXTBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_GREEN
	};

#endif


/** \brief View 1 output config */
const CONIO_PAGE_outputElement_t gUIViewWorkOutCfg[] =
{
	/* NMT State Entry */
	{
		TRUE, &gUINMTStateCfg, &gUINMTStateWork,
		&UI_WORK__init_nmtState, NULL, NULL
	},

	/* Mandatory Empty Entry */
	{
		FALSE, NULL, NULL,
		NULL, NULL, NULL
	}
};

/** \brief View 1 input config */
const CONIO_PAGE_inputElement_t gUIViewWorkInCfg[] =
{
	/* Next Page Button */
	{TRUE, &gUI1NextPageButton, &CONIO_RECTBUTTON_init, &CONIO_RECTBUTTON_isPressed, PAGE_ACTION_SWITCH_NEXT, NULL},
	/* Mandatory Empty Entry */
	{FALSE, NULL, NULL, NULL, PAGE_ACTION_NONE, NULL}
};

/** \brief View 1 config */
const CONIO_PAGE_cfg_t gUIViewWork =
{
		UI_PAGE1_TFA, UI_PAGE1_BFA, &gUIViewWorkOutCfg[0], &gUIViewWorkInCfg[0], RGB565_WHITE
};

#pragma section
