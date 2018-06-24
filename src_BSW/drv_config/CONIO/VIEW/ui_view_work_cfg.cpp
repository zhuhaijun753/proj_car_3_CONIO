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

#define UI_NMT_LABEL_KEY_STR			"NMT: LIDR EnRL EnRR EnFL EnFR"

#define UI_NMT_STATE_VAL_WIDTH			(4) //7 + 2'()' + 2 + 1
#define UI_NMT_STATE_MARGIN				(5) // "NMT: "
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

const UI_NMTLabel_cfg_t gUINMTLabelCfg =
	{
		{{0, 11}, strlen(UI_NMT_LABEL_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_NMT_LABEL_KEY_STR
	};

/** \brief NMT state UI config */
const UI_NMTState_cfg_t gUINMTStateCfg =
	{
		{{UI_NMT_STATE_MARGIN, 10}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE},
		{{UI_NMT_STATE_MARGIN + UI_NMT_STATE_OFFSET, 10}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE},
		{{UI_NMT_STATE_MARGIN + UI_NMT_STATE_OFFSET*2, 10}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE},
		{{UI_NMT_STATE_MARGIN + UI_NMT_STATE_OFFSET*3, 10}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE},
		{{UI_NMT_STATE_MARGIN + UI_NMT_STATE_OFFSET*4, 10}, UI_NMT_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE},
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


/** \brief View 1 output config */
const CONIO_PAGE_outputElement_t gUIViewWorkOutCfg[] =
{
	/* NMT Label Entry */
	{
		TRUE, &gUINMTLabelCfg, &gUINMTStateWork,
		&UI_WORK__init_nmtLabel, NULL, NULL
	},
	/* NMT State Entry */
	{
		TRUE, &gUINMTStateCfg, &gUINMTStateWork,
		&UI_WORK__init_nmtState, UI_WORK__read_normalState, UI_CARWORK__update_NMTState
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
		UI_PAGEWORK_TFA, UI_PAGEWORK_BFA, &gUIViewWorkOutCfg[0], &gUIViewWorkInCfg[0], RGB565_WHITE
};

#pragma section
