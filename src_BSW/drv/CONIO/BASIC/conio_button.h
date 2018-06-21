/**
 * \file 	conio_button.h
 * \author 	Rahul
 * \date 	12-Jun-2017
 *
 * \brief DEFAULT HEADER TEMPLATE
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
#ifndef SRC_BSW_DRV_CONIO_BASIC_CONIO_BUTTON_H_
#define SRC_BSW_DRV_CONIO_BASIC_CONIO_BUTTON_H_

#include "conio.h"
#include "conio_text.h"
#include "conio_shape.h"

typedef struct
{
	CONIO_RECT_t			mShape;			/**< Button Shape */
	CONIO_TEXT_t			mLabel;			/**< Button Label holder */
	const 	char_t			*mpLabel;		/**< Button Label */
	CONIO_COLOR_rgb565_t	mnFillColor;	/**< Button fill color */

}CONIO_RECTBUTTON_cfg_t;


/**
 * \brief RC_t CONIO_RECTBUTTON_init(const void *apConfig)
 *
 * Initialize a rectangle button
 *
 * \param	apConfig 	: button configuration
 * \return #RC_ERROR_BAD_PARAM if NULL param is passed,
 * \return error-codes if other initialization errors,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_RECTBUTTON_init(const void *apConfig);

/**
 * \brief boolean_t CONIO_RECTBUTTON_isPressed(const void *apConfig, const void *apData);
 *
 * Check if a rectangle button is pressed
 *
 * \param	apConfig 	: button configuration
 * \param	apData 		: pixel data
 * \return #RC_ERROR_BAD_PARAM if NULL param is passed,
 * \return #RC_ERROR_NOT_MATCH if button is not pressed,
 * \return #RC_SUCCESS if button is pressed
 *
 */
RC_t CONIO_RECTBUTTON_isPressed(const void *apConfig, const void *apData);

#endif /* SRC_BSW_DRV_CONIO_BASIC_CONIO_BUTTON_H_ */
