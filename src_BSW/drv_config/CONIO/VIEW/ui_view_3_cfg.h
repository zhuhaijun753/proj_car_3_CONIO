/**
 * \file	ui_view_3_cfg.h
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

#ifndef SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_3_CFG_H_
#define SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_3_CFG_H_

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

#endif /* SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_3_CFG_H_ */
