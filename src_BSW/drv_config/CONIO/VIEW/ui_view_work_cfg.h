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

#endif /* SRC_BSW_DRV_CONFIG_CONIO_VIEW_UI_VIEW_WORK_CFG_H_ */
