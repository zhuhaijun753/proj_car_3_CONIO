/**
 * \file 	CTouchController.cpp
 * \author 	Rahul
 * \date 	10-Jun-2017
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

#include <swc_touch_controller.h>
#include <ITouchObserver.h>

#include "touch_reference.h"
#include "ui.h"
#include "signal_bb.h"

namespace ui
{

/**
 * \brief CTouchController()
 *
 * Touch controller constructor.
 *
 * \param	void
 * \return NA
 *
 */
CTouchController::CTouchController()
{
	mnObservers = 0;
}

/**
 * \brief RC_t init()
 *
 * initialize touch controller
 *
 * \param	void
 * \return #RC_ERROR if no views are configured,
 * \return #RC_SUCCESS if successful
 * \return error-codes otherwise
 *
 */
RC_t CTouchController::init()
{
	return TOUCHDRIVER_init();
}

/**
 * \brief RC_t registerObserver(ITouchObserver *apObserver)
 *
 * register touch observer
 *
 * \param	apObserver : observer
 * \return #RC_SUCCESS if successful
 * \return error-codes otherwise
 *
 */
RC_t CTouchController::registerObserver(ITouchObserver* apObserver)
{
	if (NULL == apObserver)
		return RC_ERROR_BAD_PARAM;

	if (MAX_OBSERVERS_PER_TOUCH_CONTROLLER <= mnObservers)
		return RC_ERROR_OVERRUN;

	mpObserver[mnObservers++] = apObserver;

	return RC_SUCCESS;
}

/**
 * \brief RC_t run()
 *
 * touch controller run-time
 *
 * \param	void
 * \return #RC_SUCCESS if successful
 * \return error-codes otherwise
 *
 */
RC_t CTouchController::run()
{
	CONIO_PIXEL_t			touchPos;

	if (RC_SUCCESS == TOUCHDRIVER_read_poll(&touchPos.mnX, &touchPos.mnY))
	{
		/*
		uint32_t i = 0;

		for (i = 0; i < mnObservers; i++)
		mpObserver[i++]->notify(touchEvent.set(touchPos));
		*/

		return TOUCH_set(&so_in_touch, (const TOUCH_data_t)touchPos);
	}

	return RC_ERROR_TIME_OUT;
}

/**
 * \brief ~CTouchController()
 *
 * Touch controller destructor.
 *
 * \param	NA
 * \return  NA
 *
 */
CTouchController::~CTouchController()
{
}


} /* namespace ui */

