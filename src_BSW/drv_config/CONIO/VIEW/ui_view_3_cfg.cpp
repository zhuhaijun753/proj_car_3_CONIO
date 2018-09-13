/*
 * ui_view_work_cfg.cpp
 *
 *  Created on: Jun 23, 2018
 *      Author: dinhq
 */
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

#define UI_PAGEWORK_NEXTBUTTON_WIDTH		(72)
#define UI_PAGEWORK_NEXTBUTTON_HEIGHT		(CONIO_TEXT_Y_INC)

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
/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI1NextPageButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGEWORK_NEXTBUTTON_WIDTH), 0},
			UI_PAGEWORK_NEXTBUTTON_WIDTH,
			UI_PAGEWORK_NEXTBUTTON_HEIGHT,
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
	{TRUE, &gUI1NextPageButton, &CONIO_RECTBUTTON_init, &CONIO_RECTBUTTON_isPressed, PAGE_ACTION_SWITCH_NEXT, NULL},
	/* Mandatory Empty Entry */
	{FALSE, NULL, NULL, NULL, PAGE_ACTION_NONE, NULL}
};

/** \brief View 1 config */
const CONIO_PAGE_cfg_t gUIView3 =
{
		UI_PAGEWORK_TFA, UI_PAGEWORK_BFA, &gUIView3OutCfg[0], &gUIView3InCfg[0], RGB565_WHITE
};

#pragma section
