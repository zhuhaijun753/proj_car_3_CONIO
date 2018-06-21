/**
 * \file 	conio_page.cpp
 * \author 	Rahul
 * \date 	21-May-2017
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

//-------------------------------------------------------------------- [header]

#include <BASIC/conio_page.h>
#include "tft_reference.h"

//-------------------------------------------------------------------- [global functions]

/**
 * \brief RC_t CONIO_PAGE_clear(const CONIO_PAGE_t *apPage)
 *
 * Clear a page.
 *
 * \param	apPage 	: page handle
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_PAGE_clear(const CONIO_PAGE_cfg_t *apPage)
{
	if (NULL == apPage)
		return RC_ERROR_BAD_PARAM;

	return TFT_paint_frame(apPage->mBGColor);
}

/**
 * \brief RC_t CONIO_PAGE_init(const CONIO_PAGE_t *apPage)
 *
 * Initialize a page.
 *
 * \param	apPage 	: page handle
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_PAGE_init(const CONIO_PAGE_cfg_t *apPage)
{
	RC_t ret = RC_SUCCESS;

	uint16_t i = 0;

	if (NULL == apPage)
		return RC_ERROR_BAD_PARAM;

	CONIO_PAGE_clear(apPage);

	for (i = 0; (NULL != apPage->mpOut); i++)
	{
		if (NULL == apPage->mpOut[i].mpConfig)
			break;

		if (NULL != apPage->mpOut[i].mfInit)
		{
			ret = apPage->mpOut[i].mfInit(apPage->mpOut[i].mpConfig);
			if (RC_SUCCESS != ret)
				return ret;
		}
	}

	for (i = 0; (NULL != apPage->mpIn); i++)
	{
		if (NULL == apPage->mpIn[i].mpConfig)
			break;

		if (NULL != apPage->mpIn[i].mfInit)
		{
			ret = apPage->mpIn[i].mfInit(apPage->mpIn[i].mpConfig);
			if (RC_SUCCESS != ret)
				return ret;
		}
	}

	return RC_SUCCESS;
}

/**
 * \brief RC_t CONIO_PAGE_outputModel(const CONIO_PAGE_t *apPage,  CONIO_PAGE_state_t aState)
 *
 * Initialize a page.
 *
 * \param	apPage 	: page handle
 * \param	aState 	: page rendering state
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_PAGE_outputModel(const CONIO_PAGE_cfg_t *apPage, CONIO_PAGE_state_t aState)
{
	RC_t ret = RC_SUCCESS;

	uint16_t i = 0;
	const CONIO_PAGE_outputElement_t *pCur = NULL;

	CONIO_PAGE_elementUpdateState_t updateState = (aState == PAGE_IS_INITIALIZING) ?
												  ELEMENT_UPDATE_FORCED : ELEMENT_UPDATE_ON_CHANGE;

	if (NULL == apPage)
		return RC_ERROR_BAD_PARAM;

	for (i = 0; (NULL != apPage->mpOut); i++)
	{
		pCur = &apPage->mpOut[i];

		if (NULL == pCur->mpConfig)
			break;

		if (FALSE == pCur->mbEnabled)
			continue;

		if (NULL != pCur->mfSrc)
		{
			ret = pCur->mfSrc(pCur->mpData, updateState);
			if (RC_SUCCESS != ret)
				break;
		}

		if (NULL != pCur->mfSink)
		{
			ret = pCur->mfSink(pCur->mpConfig, pCur->mpData);
			if (RC_SUCCESS != ret)
				break;
		}
	}

	return ret;
}

/**
 * \brief RC_t CONIO_PAGE_inputModel(const CONIO_PAGE_cfg_t *apPage, const CONIO_PIXEL_t *apPixel, CONIO_PAGE_inputAction_t *apAction)
 *
 * page input model.
 *
 * \param	apPage 	: page handle
 * \param 	apPixel : pixel input
 * \param 	apAction: pointer to input action
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_PAGE_inputModel(const CONIO_PAGE_cfg_t *apPage, const CONIO_PIXEL_t *apPixel, CONIO_PAGE_inputAction_t *apAction)
{
	RC_t ret = RC_SUCCESS;

	uint16_t i = 0;
	const CONIO_PAGE_inputElement_t *pCur = NULL;

	if ((NULL == apPage) || (NULL == apPixel) || (NULL == apAction))
		return RC_ERROR_BAD_PARAM;

	*apAction = PAGE_ACTION_NONE;

	for (i = 0; (NULL != apPage->mpIn); i++)
	{
		pCur = &apPage->mpIn[i];

		if (NULL == pCur->mpConfig)
			break;

		if (NULL != pCur->mfIsTriggered)
		{
			if (RC_SUCCESS == pCur->mfIsTriggered(pCur->mpConfig, apPixel))
			{
				*apAction = pCur->mAction;
				break;
			}
		}
	}

	return ret;
}


/**
 * \brief RC_t CONIO_PAGE_onInput(const CONIO_PAGE_cfg_t *apPage, CONIO_PAGE_inputAction_t aAction)
 *
 * page input model.
 *
 * \param	apPage 	: page handle
 * \param 	aAction:  input action
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_ERROR_NOT_MATCH if not matching inputhandler was found
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_PAGE_onInput(const CONIO_PAGE_cfg_t *apPage, CONIO_PAGE_inputAction_t aAction)
{
	RC_t ret = RC_SUCCESS;

	uint16_t i = 0;
	const CONIO_PAGE_inputElement_t *pCur = NULL;

	if (NULL == apPage)
		return RC_ERROR_BAD_PARAM;

	for (i = 0; (NULL != apPage->mpIn); i++)
	{
		pCur = &apPage->mpIn[i];

		if (NULL == pCur->mpConfig)
			break;

		if (aAction == pCur->mAction)
		{
			if (NULL != pCur->mfHandler)
				return pCur->mfHandler(apPage);
		}
	}

	return RC_ERROR_NOT_MATCH;
}
