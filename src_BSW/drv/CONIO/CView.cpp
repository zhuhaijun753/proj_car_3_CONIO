/**
 * \file	CView.cpp
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief View class implementation
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

#include "CView.h"
#include "tft_reference.h"

namespace ui {

//-------------------------------------------------------------------- [Methods]

/**
 * \brief CView()
 *
 * View constructor.
 *
 * \param	void
 * \return NA
 *
 */
CView::CView()
{

}

/**
 * \brief void set(const CONIO_PAGE_cfg_t *apPageCfg)
 *
 * set a page config to the view
 *
 * \param	apPageCfg 	: page configuration
 * \return void
 *
 */
void CView::set(const CONIO_PAGE_cfg_t *apPageCfg)
{
	mpConfig 	= apPageCfg;
}

/**
 * \brief RC_t registerViewChangeObserver(IViewChangeObserver *observer)
 *
 * register a view change observer
 *
 * \param	observer 	: observer handle
 * \return #RC_ERROR_BAD_PARAM if invalid arguments,
 * \return #RC_ERROR_OVERRUN if max observers are already added
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CView::registerViewChangeObserver(IViewChangeObserver* observer)
{
	if (NULL == observer)
		return RC_ERROR_BAD_PARAM;

	if (MAX_OBSERVERS_PER_VIEW > mnViewChangeObservers)
	{
		mpViewChangeObservers[mnViewChangeObservers++] = observer;
		return RC_SUCCESS;
	}

	return RC_ERROR_OVERRUN;

}

/**
 * \brief RC_t render()
 *
 * Initializes a page.
 *
 * \param	void
 * \return #RC_SUCCESS
 *
 */
RC_t CView::render()
{
	if (NULL !=  mpConfig)
	{
		CONIO_PAGE_init(mpConfig);
		CONIO_PAGE_outputModel(mpConfig, PAGE_IS_INITIALIZING);
	}

	return RC_SUCCESS;
}

/**
 * \brief void notify(CTouchNotification& notification)
 *
 * Touch notification interface to the view
 *
 * \param	notification : Touch notification
 * \return 	void
 *
 */
void CView::notify(CTouchNotification& notification)
{
	const CONIO_PAGE_cfg_t *pConfig = mpConfig;
	CONIO_PIXEL_t pixel = notification.get();
	CONIO_PAGE_inputAction_t action = PAGE_ACTION_NONE;

	if (RC_SUCCESS == CONIO_PAGE_inputModel(pConfig, &pixel, &action))
	{
		if (PAGE_ACTION_NONE != action)
			inputActionHandler(action);
	}
}

/**
 * \brief RC_t render()
 *
 * Models a page active time.
 *
 * \param	void
 * \return #RC_SUCCESS
 *
 */
RC_t CView::model()
{
	CONIO_PAGE_outputModel(mpConfig, PAGE_IS_ACTIVE);

	return RC_SUCCESS;
}

/**
 * \brief void inputActionHandler(CONIO_PAGE_input_t mAction)
 *
 * handles an input
 *
 * \param	mAction : input action
 * \return void
 *
 */
void CView::inputActionHandler(CONIO_PAGE_inputAction_t mAction)
{
	switch(mAction)
	{
	case PAGE_ACTION_SWITCH_NEXT:
	{
		CViewChangeNotification notification;
		VIEWCHANGE_mode_t mode = VIEWCHANGE_NEXT;

		// To be done before notifying as the notifier changes this object
		CONIO_PAGE_onInput(mpConfig, PAGE_ACTION_SWITCH_NEXT);

		notification.set(mode);


		for (uint16_t i = 0; i < mnViewChangeObservers; i++)
		{
			mpViewChangeObservers[i]->notify(notification);
		}


	}
	break;

	case PAGE_ACTION_SWITCH_PREV:
	{
		CViewChangeNotification notification;

		VIEWCHANGE_mode_t mode = VIEWCHANGE_PREV;

		// To be done before notifying as the notifier changes this object
		CONIO_PAGE_onInput(mpConfig, PAGE_ACTION_SWITCH_PREV);

		notification.set(mode);

		for (uint16_t i = 0; i < mnViewChangeObservers; i++)
		{
			mpViewChangeObservers[i]->notify(notification);
		}
	}
	break;

	case PAGE_ACTION_SCROLL_UP:
	{
		CViewChangeNotification notification;

		VIEWCHANGE_mode_t mode = VIEWCHANGE_SCROLL_UP;

		notification.set(mode);


		for (uint16_t i = 0; i < mnViewChangeObservers; i++)
		{
			mpViewChangeObservers[i]->notify(notification);
		}

		// To be done after notifying as the notifier updates the screen position
		CONIO_PAGE_onInput(mpConfig, PAGE_ACTION_SCROLL_UP);
	}
	break;

	case PAGE_ACTION_SCROLL_DOWN:
	{
		CViewChangeNotification notification;
		VIEWCHANGE_mode_t mode = VIEWCHANGE_SCROLL_DOWN;

		notification.set(mode);

		for (uint16_t i = 0; i < mnViewChangeObservers; i++)
		{
			mpViewChangeObservers[i]->notify(notification);
		}

		// To be done after notifying as the notifier updates the screen position
		CONIO_PAGE_onInput(mpConfig, PAGE_ACTION_SCROLL_DOWN);

	}
	break;

	default:
		//unhandled
		break;

	}
}

/**
 * \brief ~CView()
 *
 * View destructor.
 *
 * \param	NA
 * \return NA
 *
 */
CView::~CView()
{
}

} /* namespace ui */
