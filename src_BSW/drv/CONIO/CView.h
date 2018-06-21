/**
 * \file	CView.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief View class definition
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

#ifndef SRC_ASW_UI_CVIEW_H_
#define SRC_ASW_UI_CVIEW_H_

//-------------------------------------------------------------------- [Includes]

#include "ITouchObserver.h"
#include "IViewChangeObserver.h"
#include "IView.h"
#include "BASIC/conio_page.h"


namespace ui {

//-------------------------------------------------------------------- [Defines]

const uint8_t		MAX_OBSERVERS_PER_VIEW  = 4;

//-------------------------------------------------------------------- [Types]

class CView: public IView, public ITouchObserver {

protected:
	const CONIO_PAGE_cfg_t  	*mpConfig;											/**< Page config associated with the View */
	IViewChangeObserver			*mpViewChangeObservers[MAX_OBSERVERS_PER_VIEW];		/**< View change observer list */
	uint8_t						mnViewChangeObservers;								/**< View change observer count */

protected:
	/**
	 * \brief void inputActionHandler(CONIO_PAGE_input_t mAction)
	 *
	 * handles an input
	 *
	 * \param	mAction : input action
	 * \return void
	 *
	 */
	void inputActionHandler(CONIO_PAGE_inputAction_t mAction);

public:
	/**
	 * \brief CView()
	 *
	 * View constructor.
	 *
	 * \param	void
	 * \return NA
	 *
	 */
	CView();

	/**
	 * \brief void set(const CONIO_PAGE_cfg_t *apPageCfg)
	 *
	 * set a page config to the view
	 *
	 * \param	apPageCfg 	: page configuration
	 * \return void
	 *
	 */
	void set(const CONIO_PAGE_cfg_t *apPageCfg);

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
	RC_t registerViewChangeObserver(IViewChangeObserver *observer);

	/**
	 * \brief RC_t render()
	 *
	 * Initializes a page.
	 *
	 * \param	void
	 * \return #RC_SUCCESS
	 *
	 */
	RC_t render();

	/**
	 * \brief RC_t render()
	 *
	 * Models a page active time.
	 *
	 * \param	void
	 * \return #RC_SUCCESS
	 *
	 */
	RC_t model();

	/**
	 * \brief void notify(CTouchNotification& notification)
	 *
	 * Touch notification interface to the view
	 *
	 * \param	notification : Touch notification
	 * \return 	void
	 *
	 */
	void notify(CTouchNotification& notification);

	/**
	 * \brief ~CView()
	 *
	 * View destructor.
	 *
	 * \param	NA
	 * \return NA
	 *
	 */
	virtual ~CView();
};

} /* namespace ui */

#endif /* SRC_ASW_UI_CVIEW_H_ */
