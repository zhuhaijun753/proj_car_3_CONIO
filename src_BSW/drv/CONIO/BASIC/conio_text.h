/**
 * \file	conio_text.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief conio text header
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

#ifndef SRC_BSW_DRV_CONIO_BASIC_CONIO_TEXT_H_
#define SRC_BSW_DRV_CONIO_BASIC_CONIO_TEXT_H_

//-------------------------------------------------------------------- [Includes]

#include "conio.h"

//-------------------------------------------------------------------- [Defines]

#define CONIO_TEXT_X_INC (11)		/**< text x-direction pixel increment **/
#define CONIO_TEXT_Y_INC (20)		/**< text y-direction pixel increment  - has to FontH + 2 **/

//-------------------------------------------------------------------- [Types]

/** \brief Text Cursor type*/
typedef struct
{
	uint16_t 		mnX;			/**< Column in the text domain **/
	uint16_t 		mnY;			/**< Row in the text domain  **/

}CONIO_TEXT_cursor_t;

/** \brief Text Handle type */
typedef struct _CONIO_TEXT_
{
	CONIO_TEXT_cursor_t		mCursor;	/**< text cursor **/
	uint16_t				mnLen;		/**< length **/
	CONIO_COLOR_rgb565_t	mColor;		/**< text color **/
	CONIO_COLOR_rgb565_t	mBGColor;	/**< background color **/

}CONIO_TEXT_t;

//-------------------------------------------------------------------- [Global prototypes]
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief RC_t CONIO_TEXT_init()
 *
 * Initialize text environment.
 *
 * \param	void
 * \return #RC_SUCCESS
 *
 */
RC_t CONIO_TEXT_init();

/**
 * \brief RC_t CONIO_TEXT_write(const CONIO_TEXT_t *apText, const char_t *apFmt, ...)
 *
 * Write text.
 *
 * \param	apText 	: text handle
 * \param 	apFmt	: variadic argument format followed by arguments
 * \return #RC_ERROR_BAD_PARAM x_pos or y_pos is beyond display limits,
 * \return #RC_SUCCESS otherwise
 *
 */

RC_t CONIO_TEXT_write(const CONIO_TEXT_t *apText, const char_t *apFmt, ...);

/**
 * \brief RC_t CONIO_TEXT_deinit()
 *
 * De-initialize text environment.
 *
 * \param	void
 * \return #RC_SUCCESS
 *
 */
RC_t CONIO_TEXT_deinit();

#ifdef __cplusplus
}
#endif
#endif /* SRC_BSW_DRV_CONIO_CONIO_TEXT_H_ */
