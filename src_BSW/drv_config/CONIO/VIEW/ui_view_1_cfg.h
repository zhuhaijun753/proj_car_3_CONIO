/**
 * \file	ui_car_state_cfg.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief car state ui configuration header
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
#ifndef SRC_ASW_UI_VIEW_1_CFG_H_
#define SRC_ASW_UI_VIEW_1_CFG_H_

//-------------------------------------------------------------------- [Includes]

#include "conio.h"
#include "BASIC/conio_text.h"
#include "BASIC/conio_shape.h"

//-------------------------------------------------------------------- [Defines]

#define UI_CAR_STATE_ENGINES			 	(4)		/**< front (left, right), rear (left, right) **/
#define UI_CAR_STATE_TARGET_SPEED_FIELDS 	(4) 	/**< x, y, phi, age **/
#define UI_CAR_STATE_JOYSTICK_FIELDS 		(4) 	/**< x, y, rz, age, **/
#define UI_CAR_STATE_ENGINE_DIRECTION		(2)		/**< fwd, rev **/

//-------------------------------------------------------------------- [Types]



typedef struct
{
	CONIO_TEXT_t			mKey;
	const char_t			*mpKey;
	CONIO_COLOR_rgb565_t	mnClrColor;
	CONIO_COLOR_rgb565_t	mnSetColor;
	CONIO_RECT_t			mVal[UI_CAR_STATE_ENGINES][UI_CAR_STATE_ENGINE_DIRECTION];

}UI_engine_cfg_t;

typedef struct
{
	CONIO_TEXT_t 	mKey;
	const char_t	*mpKey;
	CONIO_TEXT_t	mVal;

}UI_normalState_cfg_t;

typedef struct
{
	CONIO_TEXT_t 	mKey;
	const char_t	*mpKey;
	CONIO_TEXT_t	mVal;

}UI_safetyState_cfg_t;

typedef struct
{
	CONIO_TEXT_t 	mKey;
	const char_t	*mpKey;
	CONIO_TEXT_t	mVal;

}UI_safety_runnable_cfg_t;

typedef struct
{
	CONIO_TEXT_t	mKey;
	const char_t	*mpKey;
	CONIO_TEXT_t	mVal[UI_CAR_STATE_TARGET_SPEED_FIELDS];
}UI_targetSpeed_cfg_t;

typedef struct
{
	CONIO_TEXT_t	mKey;		//Headline position
	const char_t	*mpKey;		//Headline content
	CONIO_TEXT_t	mVal[UI_CAR_STATE_JOYSTICK_FIELDS];		//Data element positions
}UI_joystick_cfg_t;



#endif /* SRC_ASW_UI_VIEW_1_CFG_H_ */
