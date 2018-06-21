/**
 * \file	conio_text.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief conio shapes header
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

#ifndef SRC_BSW_DRV_CONIO_BASIC_CONIO_SHAPE_H_
#define SRC_BSW_DRV_CONIO_BASIC_CONIO_SHAPE_H_

//-------------------------------------------------------------------- [Includes]

#include "global.h"
#include "conio.h"

//-------------------------------------------------------------------- [Types]

/** \brief Rectangular fill mode type */
typedef enum
{
	RECT_FILL_FROM_BL = 0,		/**< Fill from Bottom Left  **/
	RECT_FILL_FROM_TR			/**< Fill from Top Right **/
}CONIO_RECT_fill_t;

/** \brief Rectangular shape type */
typedef struct
{
	CONIO_PIXEL_t 			mPos;			/**< Begin Pixel **/
	uint16_t				mnW;			/**< Width **/
	uint16_t				mnH;			/**< Height **/
	CONIO_COLOR_rgb565_t	mColor;			/**< Border Color **/
}CONIO_RECT_t;

//-------------------------------------------------------------------- [Global prototypes]

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief RC_t CONIO_RECT_draw(const CONIO_RECT_t *apRect)
 *
 * Draw a rectangle.
 *
 * \param	apRect 	: rectangle handle
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_RECT_draw(const CONIO_RECT_t *apRect);

/**
 * \brief CONIO_RECT_fill(const CONIO_RECT_t *apRect, CONIO_COLOR_rgb565_t aColor, uint16_t anPercent)
 *
 * Fill a rectangle
 *
 * \param	apRect 		: rectangle handle
 * \param	aFillMode	: fill start position
 * \param	aColor 		: fill color
 * \param	anPercent 	: fill percent
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_ERROR_BAD_DATA if INVALID percent ranges,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_RECT_fill(const CONIO_RECT_t *apRect, CONIO_RECT_fill_t aFillMode, CONIO_COLOR_rgb565_t aColor, sint16_t anPercent);

#ifdef __cplusplus
}
#endif

#endif /* SRC_BSW_DRV_CONIO_CONIO_SHAPE_H_ */
