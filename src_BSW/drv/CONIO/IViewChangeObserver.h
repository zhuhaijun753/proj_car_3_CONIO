/**
 * \file	IViewChangeObserver.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief View change Observer interface definition
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
 * \copyright Copyright �2016
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

#ifndef SRC_ASW_UI_IVIEWCHANGEOBSERVER_H_
#define SRC_ASW_UI_IVIEWCHANGEOBSERVER_H_

//-------------------------------------------------------------------- [Includes]

#include "IObserver.h"

namespace ui {

//-------------------------------------------------------------------- [Types]


/** \brief View Change event type*/
typedef enum
{
	VIEWCHANGE_NEXT = 0,	/**< Move to next page */
	VIEWCHANGE_PREV,		/**< Move to previous page */
	VIEWCHANGE_LEVEL_UP,	/**< Go a level up */
	VIEWCHANGE_LEVEL_DOWN,	/**< Go a level down */
	VIEWCHANGE_SCROLL_UP,	/**< Scroll up */
	VIEWCHANGE_SCROLL_DOWN,	/**< Scroll down */
	VIEWCHANGE_ROOT,		/**< Jump to Root Page */
}VIEWCHANGE_mode_t;

/** \brief View Change Notification interface type*/
typedef INotification<VIEWCHANGE_mode_t> CViewChangeNotification;

/** \brief View Change Observer type*/
class IViewChangeObserver: public IObserver<VIEWCHANGE_mode_t> {
public:
	/**
	 * \brief void notify(CTouchNotification& notification)
	 *
	 * view change notification interface to view change observer
	 *
	 * \param	notification : view change notification
	 * \return void
	 *
	 */
	virtual void notify(CViewChangeNotification& notification) = 0;
};

} /* namespace ui */

#endif /* SRC_ASW_UI_IVIEWCHANGEOBSERVER_H_ */
