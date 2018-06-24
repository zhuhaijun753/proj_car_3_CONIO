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
/** \brief CAR state UI data */
UI_carState_data_t		gUICarStateWork;
/** \brief Joystick UI data */
UI_joystick_data_t 		gUIJoystickWork;
/** \brief Target Speed UI data */
UI_targetSpeed_data_t	gUITargetSpdWork;
/** \brief Engine UI data */
UI_engine_data_t		gUIEngineWork;
/** \brief Safety Runnable Name data */
UI_carSafetyRunnable_data_t gUISafetyRunnableWork;


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

/** \brief Normal state UI config */
const UI_normalState_cfg_dep_t gUINormStateCfg =
	{
		{{0, 10}, strlen(UI_NORMAL_STATE_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_NORMAL_STATE_KEY_STR,
		{{(strlen(UI_NORMAL_STATE_KEY_STR) + 1), 10}, UI_NORMAL_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief Safety state UI config */
const UI_safetyState_cfg_dep_t gUISafeStateCfg =
	{
		{{0, 9}, strlen(UI_SAFETY_STATE_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_SAFETY_STATE_KEY_STR,
		{{(strlen(UI_SAFETY_STATE_KEY_STR) + 1), 9}, UI_SAFETY_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief Safety runnable UI config */
const UI_safety_runnable_cfg_dep_t gUISafetyRunnableWorkCfg =
	{
		{{0, 8}, strlen(UI_SAFETY_RUNNABLE_STR), RGB565_BLACK, RGB565_WHITE},
		UI_SAFETY_RUNNABLE_STR,
		{{(strlen(UI_SAFETY_RUNNABLE_STR) + 1), 8}, UI_SAFETY_RUNNABLE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief Joystick UI config */
const UI_joystick_cfg_dep_t gUIJoystickWorkCfg =
	{
		{{0, UI_JOYSTICK_LINE}, strlen(UI_JOYSTICK_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_JOYSTICK_KEY_STR,
//		 @PF {
//			{{(strlen(UI_JOYSTICK_KEY_STR) + 1), 8}, UI_JOYSTICK_X_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* x */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + 2), 8}, UI_JOYSTICK_Y_WIDTH, RGB565_BLACK, RGB565_WHITE},									/* y */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + UI_JOYSTICK_Y_WIDTH + 3), 8}, UI_JOYSTICK_RZ_WIDTH, RGB565_BLACK, RGB565_WHITE},				/* rz */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + 1), 7}, UI_JOYSTICK_ESTOP_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* estop */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_ESTOP_WIDTH + 2), 7}, UI_JOYSTICK_GSTOP_WIDTH, RGB565_BLACK, RGB565_WHITE},							/* gstop */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_ESTOP_WIDTH + UI_JOYSTICK_GSTOP_WIDTH + 3), 7}, UI_JOYSTICK_AGE_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* age */
//		}
		{
			{{(strlen(UI_JOYSTICK_KEY_STR) + 1), UI_JOYSTICK_LINE}, UI_JOYSTICK_X_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* x */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + 2), UI_JOYSTICK_LINE}, UI_JOYSTICK_Y_WIDTH, RGB565_BLACK, RGB565_WHITE},									/* y */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + UI_JOYSTICK_Y_WIDTH + 3), UI_JOYSTICK_LINE}, UI_JOYSTICK_RZ_WIDTH, RGB565_BLACK, RGB565_WHITE},				/* rz */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + UI_JOYSTICK_Y_WIDTH + UI_JOYSTICK_RZ_WIDTH + 4), UI_JOYSTICK_LINE}, UI_JOYSTICK_AGE_WIDTH, RGB565_BLUE, RGB565_WHITE},													/* age */
		}


	};


/** \brief target speed UI config */
const UI_targetSpeed_cfg_dep_t gUITargetSpdWorkCfg =
	{
		{{0, UI_TARGET_SPEED_LINE}, strlen(UI_TARGET_SPEED_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_TARGET_SPEED_KEY_STR,
		{
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + 1), UI_TARGET_SPEED_LINE}, UI_TARGET_SPEED_X_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* x */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + UI_TARGET_SPEED_X_WIDTH + 2), UI_TARGET_SPEED_LINE}, UI_TARGET_SPEED_Y_WIDTH, RGB565_BLACK, RGB565_WHITE},							/* y */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + UI_TARGET_SPEED_X_WIDTH + UI_TARGET_SPEED_Y_WIDTH + 3), UI_TARGET_SPEED_LINE}, UI_TARGET_SPEED_PHI_WIDTH, RGB565_BLACK, RGB565_WHITE},	/* phi */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + UI_TARGET_SPEED_X_WIDTH + UI_TARGET_SPEED_Y_WIDTH + UI_TARGET_SPEED_PHI_WIDTH + 4), UI_TARGET_SPEED_LINE}, UI_TARGET_SPEED_AGE_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* age */
		}
	};

/** \brief engine UI config */
const UI_engine_cfg_dep_t gUIEngineWorkCfg =
	{
		{{0, 4}, strlen(UI_ENGINE_KEY_STR), RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		UI_ENGINE_KEY_STR,						   							/** mpKey **/
		RGB565_WHITE,														/** ClrColor - WHITE **/
		RGB565_GREEN,														/** SetColor - GREEN **/
		{
			{
				/** mVal[0][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (0 * UI_ENGINE_VAL_WIDTH)), (3 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[0][1] **/
				{
					/** mVal[0][1] **/
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (1 * UI_ENGINE_VAL_WIDTH)), (3 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[1][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1)  * CONIO_TEXT_X_INC) + (3 * UI_ENGINE_VAL_WIDTH)), (3 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[1][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (4 * UI_ENGINE_VAL_WIDTH)), (3 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[2][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (0 * UI_ENGINE_VAL_WIDTH)), (1 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[2][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (1 * UI_ENGINE_VAL_WIDTH)), (1 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[3][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (3 * UI_ENGINE_VAL_WIDTH)), (1 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[3][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (4 * UI_ENGINE_VAL_WIDTH)), (1 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

			},
		}
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
	/* Normal State Entry */
	{
		TRUE, &gUINormStateCfg, &gUICarStateWork,
		&UI_CARSTATE__init_normalState, &UI_CARSTATE__read_normalState, &UI_CARSTATE__update_normalState
	},

	/* Safety State Entry */
	{
		TRUE, &gUISafeStateCfg, &gUICarStateWork,
		&UI_CARSTATE__init_safetyState, &UI_CARSTATE__read_safetyState, &UI_CARSTATE__update_safetyState
	},

	/* Safety Runnable Entry */
	{
		TRUE, &gUISafetyRunnableWorkCfg, &gUISafetyRunnableWork,
		&UI_CARSTATE__init_safetyRunnable, &UI_CARSTATE__read_safetyRunnable, &UI_CARSTATE__update_safetyRunnable
	},

	/* Joystick Entry */
	{
		TRUE, &gUIJoystickWorkCfg, &gUIJoystickWork,
		&UI_CARSTATE__init_joystick, &UI_CARSTATE__read_joystick, &UI_CARSTATE__update_joystick
	},

	/* target speed Entry */
	{
		TRUE, &gUITargetSpdWorkCfg, &gUITargetSpdWork,
		&UI_CARSTATE__init_targetSpeed, &UI_CARSTATE__read_targetSpeed, &UI_CARSTATE__update_targetSpeed
	},

	/* Engine Entry */
	{
		TRUE, &gUIEngineWorkCfg, &gUIEngineWork,
		&UI_CARSTATE__init_engine, &UI_CARSTATE__read_engine, &UI_CARSTATE__update_engine
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
