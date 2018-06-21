/**
 * \file	conio_text.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief conio shapes implemenation
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

//-------------------------------------------------------------------- [header]

#include "conio_shape.h"

//-------------------------------------------------------------------- [Defines]

/** \brief SHAPE no-fill percent*/
#define CONIO_SHAPE_FILL_PERCENT_NONE (0)
/** \brief SHAPE full-fill percent*/
#define CONIO_SHAPE_FILL_PERCENT_FULL (100)

//-------------------------------------------------------------------- [global functions]

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
RC_t CONIO_RECT_draw(const CONIO_RECT_t *apRect)
{
	if (NULL == apRect)
		return RC_ERROR_BAD_PARAM;

	gGFXHndl.drawRect(apRect->mPos.mnX, apRect->mPos.mnY, apRect->mnW, apRect->mnH, apRect->mColor);

	return RC_SUCCESS;
}

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
RC_t CONIO_RECT_fill(const CONIO_RECT_t *apRect, CONIO_RECT_fill_t aFillMode, CONIO_COLOR_rgb565_t aColor, sint16_t anPercent)
{
	sint16_t nFillWidth = 0,
			 nFillHeight = 0;

	if (NULL == apRect)
		return RC_ERROR_BAD_PARAM;

	if ((CONIO_SHAPE_FILL_PERCENT_NONE >= anPercent) || (CONIO_SHAPE_FILL_PERCENT_FULL < anPercent))
		return RC_ERROR_BAD_DATA;


	switch (aFillMode)
	{
	case RECT_FILL_FROM_BL:
	{
		/**
		 * the offset-ed fill start position, decreased fill width and height
		 * are to leave the borders unaffected by fill!
		 */

		nFillWidth = (uint16_t)((float32_t)apRect->mnW * ((float32_t)anPercent / (float32_t)CONIO_SHAPE_FILL_PERCENT_FULL)) - 2;
		nFillHeight = (apRect->mnH - 2);

		nFillWidth = (nFillWidth < 0) ? 0 : nFillWidth;
		nFillHeight = (nFillHeight < 0) ? 0 : nFillHeight;

		gGFXHndl.fillRect((apRect->mPos.mnX + 1), (apRect->mPos.mnY + 1),
						  nFillWidth, nFillHeight, aColor);
	}
	break;

	case RECT_FILL_FROM_TR:
	{
		/**
		 * the offset-ed fill start position, decreased fill width and height
		 * are to leave the borders unaffected by fill!
		 */

		nFillWidth =  (uint16_t)((float32_t)apRect->mnW * ((float32_t)anPercent / (float32_t)CONIO_SHAPE_FILL_PERCENT_FULL)) - 2;
		nFillHeight = (apRect->mnH - 2);

		nFillWidth = (nFillWidth < 0) ? 0 : nFillWidth;
		nFillHeight = (nFillHeight < 0) ? 0 : nFillHeight;

		gGFXHndl.fillRect((apRect->mPos.mnX + apRect->mnW - 1 - nFillWidth), (apRect->mPos.mnY + 1),
						  nFillWidth, nFillHeight, aColor);

	}
	break;

	}

	return RC_SUCCESS;
}
