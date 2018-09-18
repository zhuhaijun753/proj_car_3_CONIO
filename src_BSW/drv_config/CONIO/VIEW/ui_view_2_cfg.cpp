/**
 * \file	ui_view_1_cfg.cpp
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief UI view1 configuration
 *
 * <detailed description what the file (header and/or source it belongs to) does>
 *
 * \note <notes>
 * \todo <todos>
 * \warning <warnings, e.g. dependencies, order of execution etc.>
 *
 *  Changelog:\n
 *  - <version; data of change; author>
 *            - <description of the change>
 *
 * \copyright Copyright ©2016
 * Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
 * (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
 * this paragraph and the following paragraph appear in all copies, modifications, and distributions.
 * Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
 *
 * \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
 * The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
 * This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
 */

//-------------------------------------------------------------------- [Includes]

#include "ui_view_2_cfg.h"

#include "VIEW/ui_view_cfg.h"

#include "swc_ui.h"

#include "BASIC/conio_button.h"

#include <cstring>
#include <cstdlib>

//-------------------------------------------------------------------- [Defines]

#define UI_DET_VAL_TIMESTAMP_WIDTH		(12)
#define UI_DET_VAL_CORE_WIDTH			(12)
#define UI_DET_VAL_STR_WIDTH			(24)

#define UI_PAGE2_SCROLLBUTTON_WIDTH		(24)
#define UI_PAGE2_SCROLLBUTTON_HEIGHT	(CONIO_TEXT_Y_INC)

#define UI_PAGE2_PREVBUTTON_WIDTH		(72)
#define UI_PAGE2_PREVBUTTON_HEIGHT		(CONIO_TEXT_Y_INC)

#define UI_PAGE2_NEXTBUTTON_WIDTH		(72)
#define UI_PAGE2_NEXTBUTTON_HEIGHT		(CONIO_TEXT_Y_INC)

#define UI_PAGE2_TFA					(CONIO_TEXT_Y_INC)
#define UI_PAGE2_BFA					(2 * CONIO_TEXT_Y_INC)

//-------------------------------------------------------------------- [Global data]

#pragma section ".data.ui"

UI_DET_data_t gUIDETData;

#pragma section

//-------------------------------------------------------------------- [Global Configuration]

#pragma section ".rodata"

#if ((CONIO_MODE == CONIO_REV_LANDSCAPE) || (CONIO_MODE == CONIO_LANDSCAPE))

/** \brief DET UI config */
const UI_DET_cfg_t gUIDetCfg[UI_CAR_STATE_DET_ENTRIES] =
	{
			{
				{{0, 10}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 10}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 9}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			{
				{{0, 8}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 8}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 7}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			{
				{{0, 6}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 6}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 5}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			{
				{{0, 4}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 4}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 3}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI2PrevPageButton =
	{
		{
			{0, 0},
			UI_PAGE2_PREVBUTTON_WIDTH,
			UI_PAGE2_PREVBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_GREEN
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI2NextPageButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGE2_NEXTBUTTON_WIDTH), 0},
			UI_PAGE2_NEXTBUTTON_WIDTH,
			UI_PAGE2_NEXTBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_GREEN
	};


/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI2ScrollUpButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGE2_SCROLLBUTTON_WIDTH), (CONIO_PAGE_MAX_HEIGHT - UI_PAGE2_TFA)},
			UI_PAGE2_SCROLLBUTTON_WIDTH,
			UI_PAGE2_SCROLLBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_ORANGE
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI2ScrollDownButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGE2_SCROLLBUTTON_WIDTH), (UI_PAGE2_BFA - UI_PAGE2_SCROLLBUTTON_HEIGHT)},
			UI_PAGE2_SCROLLBUTTON_WIDTH,
			UI_PAGE2_SCROLLBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_ORANGE
	};

#elif ((CONIO_MODE == CONIO_PORTRAIT) || (CONIO_MODE == CONIO_REV_PORTRAIT))

/** \brief DET UI config */
const UI_DET_cfg_t gUIDetCfg[UI_CAR_STATE_DET_ENTRIES] =
	{
			/*
			{
				{{0, 16},(UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 16}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 15}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			*/
			{
				{{0, 14}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 14}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 13}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			{
				{{0, 12}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 12}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 11}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			{
				{{0, 10}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 10}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 9}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			{
				{{0, 8}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 8}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 7}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			{
				{{0, 6}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 6}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 5}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
			{
				{{0, 4}, (UI_DET_VAL_TIMESTAMP_WIDTH + 1), RGB565_BLACK, RGB565_WHITE},
				{{(UI_DET_VAL_TIMESTAMP_WIDTH + 1), 4}, UI_DET_VAL_CORE_WIDTH, RGB565_BLACK, RGB565_WHITE},
				{{0, 3}, UI_DET_VAL_STR_WIDTH, RGB565_BLACK, RGB565_WHITE},
			},
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI2PrevPageButton =
	{
		{
			{0, 0},
			UI_PAGE2_PREVBUTTON_WIDTH,
			UI_PAGE2_PREVBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_GREEN
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI2ScrollUpButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGE2_SCROLLBUTTON_WIDTH), (CONIO_PAGE_MAX_HEIGHT - UI_PAGE2_TFA)},
			UI_PAGE2_SCROLLBUTTON_WIDTH,
			UI_PAGE2_SCROLLBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_ORANGE
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI2ScrollDownButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGE2_SCROLLBUTTON_WIDTH), (UI_PAGE2_BFA - UI_PAGE2_SCROLLBUTTON_HEIGHT)},
			UI_PAGE2_SCROLLBUTTON_WIDTH,
			UI_PAGE2_SCROLLBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_ORANGE
	};

#endif

/** \brief View 2 output config */
const CONIO_PAGE_outputElement_t gUIView2OutCfg[] =
{
	/* DET Entry 1*/
	{
		TRUE, &gUIDetCfg[0], &gUIDETData,
		&UI_CARSTATE__init_DET, NULL, &UI_CARSTATE__update_DET
	},

	/* Mandatory Empty Entry */
	{
		FALSE, NULL, NULL,
		NULL, NULL, NULL
	}
};

/** \brief View 2 input config */
const CONIO_PAGE_inputElement_t gUIView2InCfg[] =
{
	/* Prev Page Button */
	{
		TRUE, &gUI2PrevPageButton,
		&CONIO_RECTBUTTON_init, &CONIO_RECTBUTTON_isPressed,
		PAGE_ACTION_SWITCH_PREV, &UI_CARSTATE__reset_DET
	},
	/* Next Page Button */
	{
		TRUE, &gUI2NextPageButton,
		&CONIO_RECTBUTTON_init, &CONIO_RECTBUTTON_isPressed,
		PAGE_ACTION_SWITCH_NEXT, &UI_CARSTATE__reset_DET
	},
	/* Scroll Down Button */
	{
		TRUE, &gUI2ScrollUpButton,
		&CONIO_RECTBUTTON_init, &CONIO_RECTBUTTON_isPressed,
		PAGE_ACTION_SCROLL_UP, &UI_CARSTATE__event_DET_scrollUP
	},
	/* Scroll Down Button */
	{
		TRUE, &gUI2ScrollDownButton,
		&CONIO_RECTBUTTON_init, &CONIO_RECTBUTTON_isPressed,
		PAGE_ACTION_SCROLL_DOWN, &UI_CARSTATE__event_DET_scrollDOWN
	},
	/* Mandatory Empty Entry */
	{FALSE, NULL, NULL, NULL, PAGE_ACTION_NONE, NULL}
};

/** \brief View 2 config */
const CONIO_PAGE_cfg_t gUIView2 =
{
		UI_PAGE2_TFA, UI_PAGE2_BFA, &gUIView2OutCfg[0], &gUIView2InCfg[0], RGB565_WHITE
};

#pragma section
