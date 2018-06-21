/**
 * \file 	conio_button.cpp
 * \author 	Rahul
 * \date 	12-Jun-2017
 *
 * \brief DEFAULT SOURCE FILE TEMPLATE
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

#include "conio_button.h"

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
RC_t CONIO_RECTBUTTON_init(const void *apConfig)
{
	RC_t ret = RC_SUCCESS;

	const CONIO_RECTBUTTON_cfg_t *pRectButtonCfg = (const CONIO_RECTBUTTON_cfg_t *)apConfig;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_RECT_draw(&pRectButtonCfg->mShape);
	if (RC_SUCCESS != ret)
		return ret;

	ret = CONIO_RECT_fill(&pRectButtonCfg->mShape, RECT_FILL_FROM_BL, pRectButtonCfg->mnFillColor, 100);
	if (RC_SUCCESS != ret)
		return ret;

#if 0
	ret = conio_text_write(&rect_button->mLabel, rect_button->mpLabel);
	if (RC_SUCCESS != ret)
		return ret;
#endif

	return RC_SUCCESS;
}

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
RC_t CONIO_RECTBUTTON_isPressed(const void *apConfig, const void *apData)
{
	const CONIO_PIXEL_t	*touchPixel= (const CONIO_PIXEL_t *)apData;

	if ((NULL == apConfig) || (NULL == apData))
		return RC_ERROR_BAD_PARAM;

	const CONIO_RECT_t *pRectCfg = &(((const CONIO_RECTBUTTON_cfg_t *)apConfig)->mShape);

	if ((pRectCfg->mPos.mnX <= touchPixel->mnX) &&
	    ((pRectCfg->mPos.mnX + pRectCfg->mnW) >= touchPixel->mnX))
	{
		if ((pRectCfg->mPos.mnY <= touchPixel->mnY) &&
			((pRectCfg->mPos.mnY + pRectCfg->mnH) >= touchPixel->mnY))
		{
			return RC_SUCCESS;
		}
	}

	return RC_ERROR_NOT_MATCH;
}

