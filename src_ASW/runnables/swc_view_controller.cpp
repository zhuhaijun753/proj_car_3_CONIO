/**
 * \file	CViewController.cpp
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief View Controller implementation
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

#include <swc_view_controller.h>
#include "CView.h"
#include "VIEW/ui_view_cfg.h"
#include "tft_reference.h"
#include "BASIC/conio_text.h"

namespace ui {

//-------------------------------------------------------------------- [Methods]

/**
 * \brief CViewController()
 *
 * View controller constructor.
 *
 * \param	void
 * \return NA
 *
 */
CViewController::CViewController()
{
	mnViewCnt		= 0;
	mnCurViewPos 	= 0;
}

/**
 * \brief RC_t init()
 *
 * initialize view controller
 *
 * \param	void
 * \return #RC_ERROR if no views are configured,
 * \return #RC_SUCCESS if successful
 * \return error-codes otherwise
 *
 */
RC_t CViewController::init()
{
	uint16_t i = 0;

	TFT_init();

#if (CONIO_MODE == CONIO_REV_LANDSCAPE)
	TFT_set_orientation(TFT_orientation_revLandscape);
	mOrientation = TFT_orientation_revLandscape;
#elif (CONIO_MODE == CONIO_LANDSCAPE)
	TFT_set_orientation(TFT_orientation_landscape);
	mOrientation = TFT_orientation_landscape;
#elif (CONIO_MODE == CONIO_REV_PORTRAIT)
	TFT_set_orientation(TFT_orientation_revPortrait);
	mOrientation = TFT_orientation_revPortrait;
#elif (CONIO_MODE == CONIO_PORTRAIT)
	TFT_set_orientation(TFT_orientation_portrait);
	mOrientation = TFT_orientation_portrait;
#endif


	CONIO_TEXT_init();

	if (NULL == gpUIViews)
		return RC_ERROR;

	while (TRUE)
	{
		if (NULL == gpUIViews[i])
			break;

		i++;
	}

	mnViewCnt = i;

	mnCurViewPos = 0;
	if (NULL != gpUIViews[mnCurViewPos])
	{
		mCurView.set(gpUIViews[mnCurViewPos]);
		return renderCurView();
	}

	return RC_ERROR;
}

/**
 * \brief RC_t init()
 *
 * view controller run-time
 *
 * \param	void
 * \return #RC_SUCCESS if successful
 * \return error-codes otherwise
 *
 */
RC_t CViewController::run()
{
	return mCurView.model();
}

/**
 * \brief void notify(CTouchNotification& notification)
 *
 * Touch notification interface to the view controller
 *
 * \param	notification : Touch notification
 * \return void
 *
 */
void CViewController::notify(CTouchNotification& aNotification)
{
	CTouchNotification notification;
	CONIO_PIXEL_t pixel = {0, 0};

	if (TFT_orientation_revLandscape == mOrientation)
	{
		pixel = aNotification.get();
		notification.set(pixel);
	}
	else if	(TFT_orientation_landscape == mOrientation)
	{
		pixel.mnX = TFT_XSIZE - aNotification.get().mnX;
		pixel.mnY = TFT_YSIZE - aNotification.get().mnY;
		notification.set(pixel);
	}
	else if (TFT_orientation_revPortrait == mOrientation)
	{
		pixel.mnX = aNotification.get().mnY;
		pixel.mnY = TFT_XSIZE - aNotification.get().mnX;
		notification.set(pixel);
	}
	else if	 (TFT_orientation_portrait == mOrientation)
	{
		pixel.mnX = TFT_YSIZE - aNotification.get().mnY;
		pixel.mnY = aNotification.get().mnX;
		notification.set(pixel);
	}

	// every view is a touch observer, for now
	ITouchObserver *pObserver = dynamic_cast<ITouchObserver *>(&mCurView);
	if (NULL != pObserver)
		pObserver->notify(notification);

}

/**
 * \brief void notify(CViewChangeNotification& notification)
 *
 * View change notification interface to the view controller
 *
 * \param	notification : View change notification
 * \return void
 *
 */
void CViewController::notify(CViewChangeNotification& notification)
{
	switch(notification.get())
	{
	case VIEWCHANGE_NEXT:
	{
		if (mnCurViewPos != (mnViewCnt - 1))
		{
			mCurView.set(gpUIViews[++mnCurViewPos]);
			mCurView.clearViewChangeObserver();
			renderCurView();
		}
	}
	break;

	case VIEWCHANGE_PREV:
	{
		if (mnCurViewPos != 0)
		{
			mCurView.set(gpUIViews[--mnCurViewPos]);
			mCurView.clearViewChangeObserver();
			renderCurView();
		}
	}
	break;

	case VIEWCHANGE_SCROLL_DOWN:
	case VIEWCHANGE_SCROLL_UP:
	default:
		//unhandled
		break;

	}

}

/**
 * \brief RC_t renderCurView()
 *
 * View change notification interface to the view controller
 *
 * \param	void
 * \return  #RC_SUCCESS on success,
 * 			error-codes otherwise
 *
 */
RC_t CViewController::renderCurView()
{
	RC_t retCode = RC_SUCCESS;

	if ((TFT_orientation_portrait == mOrientation) ||
		(TFT_orientation_revPortrait == mOrientation))
	{
		TFT_set_orientation(TFT_orientation_landscape);
		TFT_paint_frame(RGB565_WHITE);
		TFT_set_orientation((TFT_orientation_t)mOrientation);
	}

	mCurView.registerViewChangeObserver(this);

	TFT_display_CASET(0, CONIO_PAGE_MAX_WIDTH);
	TFT_display_PASET(0, CONIO_PAGE_MAX_HEIGHT);
	TFT_set_verticalScrollArea(gpUIViews[mnCurViewPos]->mnBFA, gpUIViews[mnCurViewPos]->mnTFA);

	retCode = mCurView.render();
	mnScrollPos = 0;

	return retCode;
}

/**
 * \brief ~CViewController()
 *
 * View Controller destructor.
 *
 * \param	NA
 * \return NA
 *
 */
CViewController::~CViewController()
{
	// TODO Auto-generated destructor stub
}



} /* namespace ui */
