/**
 * \file 	CTouchController.h
 * \author 	Rahul
 * \date 	10-Jun-2017
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
#ifndef SRC_ASW_UI_CTOUCHCONTROLLER_H_
#define SRC_ASW_UI_CTOUCHCONTROLLER_H_

#include <IController.h>
#include <ITouchObserver.h>

namespace ui
{

const uint16_t MAX_OBSERVERS_PER_TOUCH_CONTROLLER = 4;


class CTouchController: public IController
{
protected:
	ITouchObserver	*mpObserver[MAX_OBSERVERS_PER_TOUCH_CONTROLLER];
	uint16_t		mnObservers;

public:
	/**
	 * \brief CTouchController()
	 *
	 * Touch controller constructor.
	 *
	 * \param	void
	 * \return NA
	 *
	 */
	CTouchController();

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
	RC_t init();

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
	RC_t registerObserver(ITouchObserver *apObserver);

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
	RC_t run();

	/**
	 * \brief ~CTouchController()
	 *
	 * Touch controller destructor.
	 *
	 * \param	NA
	 * \return  NA
	 *
	 */
	virtual ~CTouchController();

};

} /* namespace ui */
#endif /* SRC_ASW_UI_CTOUCHCONTROLLER_H_ */
