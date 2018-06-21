/**
 * \file	CViewController.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief View Controller definition
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

#ifndef SRC_ASW_UI_CVIEWCONTROLLER_H_
#define SRC_ASW_UI_CVIEWCONTROLLER_H_

//-------------------------------------------------------------------- [Includes]

#include "IController.h"
#include "ITouchObserver.h"
#include "IViewChangeObserver.h"
#include "CView.h"

namespace ui {

//-------------------------------------------------------------------- [Types]

class CViewController: public IController, public ITouchObserver, public IViewChangeObserver
{
protected:

	uint16_t				mnViewCnt;				/**< Total View count */
	CView					mCurView;				/**< Current View */
	uint16_t				mnCurViewPos;			/**< Current View Position in the total views list */
	uint8_t					mOrientation;
	sint32_t				mnScrollPos;

protected:

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
	RC_t renderCurView();

public:
	/**
	 * \brief CViewController()
	 *
	 * View controller constructor.
	 *
	 * \param	void
	 * \return NA
	 *
	 */
	CViewController();

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
	RC_t init();

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
	RC_t run();

	/**
	 * \brief void notify(CTouchNotification& notification)
	 *
	 * Touch notification interface to the view controller
	 *
	 * \param	notification : Touch notification
	 * \return void
	 *
	 */
	void notify(CTouchNotification& notification);

	/**
	 * \brief void notify(CViewChangeNotification& notification)
	 *
	 * View change notification interface to the view controller
	 *
	 * \param	notification : View change notification
	 * \return void
	 *
	 */
	void notify(CViewChangeNotification& notification);

	/**
	 * \brief ~CViewController()
	 *
	 * View Controller destructor.
	 *
	 * \param	NA
	 * \return NA
	 *
	 */
	virtual ~CViewController();
};

} /* namespace ui */

#endif /* SRC_ASW_UI_CVIEWCONTROLLER_H_ */
