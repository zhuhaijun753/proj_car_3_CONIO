/**
 * \file	ui_view_3_cfg.cpp
 * \author	Huy Dinh 	Hochschule Darmstadt - huy.dinh@stud.h-da.de
 * \date	23.06.2018
 * \version	0.1
 *
 * \brief UI view3 configuration
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
 * \copyright Copyright �2016
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
#include "ui_view_3_cfg.h"
#include "VIEW/ui_view_cfg.h"

#include "swc_ui.h"
#include "BASIC/conio_button.h"

#include <cstring>
#include <cstdlib>
//-------------------------------------------------------------------- [Defines]

#define UI_NMT_LABEL_HEADER_STR			"NMT"
#define UI_NMT_LABEL_HEADER_LINE		(11)
#define UI_NMT_LABEL_LIDR_STR			"LIDR:"
#define UI_NMT_LABEL_LIDR_LINE			(10)
#define UI_NMT_LABEL_ENRL_STR			"EnRL:"
#define UI_NMT_LABEL_ENRL_LINE			(9)
#define UI_NMT_LABEL_ENRR_STR			"EnRR:"
#define UI_NMT_LABEL_ENRR_LINE			(8)
#define UI_NMT_LABEL_ENFL_STR			"EnFL:"
#define UI_NMT_LABEL_ENFL_LINE			(7)
#define UI_NMT_LABEL_ENFR_STR			"EnFR:"
#define UI_NMT_LABEL_ENFR_LINE			(6)

#define UI_NMT_STATE_VAL_WIDTH			(4) // For example "OPRT"
#define UI_NMT_STATE_MARGIN				(strlen(UI_NMT_LABEL_LIDR_STR) + 1) // For example "LIDR: "
#define UI_NMT_STATE_OFFSET				(UI_NMT_STATE_VAL_WIDTH + 1)

#define UI_PAGE3_PREVBUTTON_WIDTH		(72)
#define UI_PAGE3_PREVBUTTON_HEIGHT		(CONIO_TEXT_Y_INC)

#define UI_PAGEWORK_TFA					(0)
#define UI_PAGEWORK_BFA					(24)

//-------------------------------------------------------------------- [Global data]

#pragma section ".data.ui"

/** \brief NMT state UI data */
UI_NMT_data_t			gUINMTStateWork;


#pragma section

//-------------------------------------------------------------------- [Global Configuration]

#pragma section ".rodata"

#if ((CONIO_MODE == CONIO_REV_LANDSCAPE) || (CONIO_MODE == CONIO_LANDSCAPE))

/** \brief NMT state display UI config */
const UI_NMTStateDisplay_cfg_t gUINMTDisplayCfg =
	{
		// Header
		{{UI_NMT_STATE_MARGIN, UI_NMT_LABEL_HEADER_LINE}, strlen(UI_NMT_LABEL_HEADER_STR), RGB565_BLACK, RGB565_WHITE},
		UI_NMT_LABEL_HEADER_STR,
		// Lidar
		{
				{{0, UI_NMT_LABEL_LIDR_LINE}, strlen(UI_NMT_LABEL_LIDR_STR), RGB565_BLACK, RGB565_WHITE},
				UI_NMT_LABEL_LIDR_STR,
				{{UI_NMT_STATE_MARGIN, UI_NMT_LABEL_LIDR_LINE}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
		},
		// EnRL
		{
				{{0, UI_NMT_LABEL_ENRL_LINE}, strlen(UI_NMT_LABEL_ENRL_STR), RGB565_BLACK, RGB565_WHITE},
				UI_NMT_LABEL_ENRL_STR,
				{{UI_NMT_STATE_MARGIN, UI_NMT_LABEL_ENRL_LINE}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
		},
		// EnRR
		{
				{{0, UI_NMT_LABEL_ENRR_LINE}, strlen(UI_NMT_LABEL_ENRR_STR), RGB565_BLACK, RGB565_WHITE},
				UI_NMT_LABEL_ENRR_STR,
				{{UI_NMT_STATE_MARGIN, UI_NMT_LABEL_ENRR_LINE}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
		},
		// EnFL
		{
				{{0, UI_NMT_LABEL_ENFL_LINE}, strlen(UI_NMT_LABEL_ENFL_STR), RGB565_BLACK, RGB565_WHITE},
				UI_NMT_LABEL_ENFL_STR,
				{{UI_NMT_STATE_MARGIN, UI_NMT_LABEL_ENFL_LINE}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
		},
		// EnFR
		{
				{{0, UI_NMT_LABEL_ENFR_LINE}, strlen(UI_NMT_LABEL_ENFR_STR), RGB565_BLACK, RGB565_WHITE},
				UI_NMT_LABEL_ENFR_STR,
				{{UI_NMT_STATE_MARGIN, UI_NMT_LABEL_ENFR_LINE}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
		}
	};
/** \brief UI prev button */
const CONIO_RECTBUTTON_cfg_t gUI3PrevPageButton =
	{
		{
			{0, 0},
			UI_PAGE3_PREVBUTTON_WIDTH,
			UI_PAGE3_PREVBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_GREEN
	};


#endif


/** \brief View 3 output config */
const CONIO_PAGE_outputElement_t gUIView3OutCfg[] =
{
	/* NMT Display Entry */
	{
		TRUE, &gUINMTDisplayCfg, &gUINMTStateWork,
		&UI_NMT__init_nmtDisplay, UI_NMT__read_nmtState, UI_NMT__update_nmtState
	},

	/* Mandatory Empty Entry */
	{
		FALSE, NULL, NULL,
		NULL, NULL, NULL
	}
};

/** \brief View 3 input config */
const CONIO_PAGE_inputElement_t gUIView3InCfg[] =
{
	/* Next Page Button */
	{TRUE, &gUI3PrevPageButton, &CONIO_RECTBUTTON_init, &CONIO_RECTBUTTON_isPressed, PAGE_ACTION_SWITCH_PREV, NULL},
	/* Mandatory Empty Entry */
	{FALSE, NULL, NULL, NULL, PAGE_ACTION_NONE, NULL}
};

/** \brief View 1 config */
const CONIO_PAGE_cfg_t gUIView3 =
{
		UI_PAGEWORK_TFA, UI_PAGEWORK_BFA, &gUIView3OutCfg[0], &gUIView3InCfg[0], RGB565_WHITE
};

#pragma section
