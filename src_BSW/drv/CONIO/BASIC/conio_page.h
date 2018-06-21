/**
 * \file	conio_text.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief conio page header
 *
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

#ifndef SRC_BSW_DRV_CONIO_BASIC_CONIO_PAGE_H_
#define SRC_BSW_DRV_CONIO_BASIC_CONIO_PAGE_H_

//-------------------------------------------------------------------- [Includes]

#include "conio.h"

//-------------------------------------------------------------------- [Defines]

/** \brief Page Dimensions */
#if ((CONIO_MODE == CONIO_REV_LANDSCAPE) || (CONIO_MODE == CONIO_LANDSCAPE))
#define CONIO_PAGE_MAX_WIDTH	(320)
#define CONIO_PAGE_MAX_HEIGHT	(240)
#elif ((CONIO_MODE == CONIO_REV_PORTRAIT) || (CONIO_MODE == CONIO_PORTRAIT))
#define CONIO_PAGE_MAX_WIDTH	(240)
#define CONIO_PAGE_MAX_HEIGHT	(320)
#endif

//-------------------------------------------------------------------- [Types]


/** \brief Page States */
typedef enum
{
	PAGE_IS_INITIALIZING = 0,		/**< Page initialization state  **/
	PAGE_IS_ACTIVE					/**< Page running state  **/

}CONIO_PAGE_state_t;

/** \brief Page Element Update States */
typedef enum
{
	ELEMENT_UPDATE_FORCED = 0,				/**< Page element force update  **/
	ELEMENT_UPDATE_ON_CHANGE				/**< Page element update only on change state  **/
}CONIO_PAGE_elementUpdateState_t;

/** \brief Page Action Event type */
typedef enum
{
	PAGE_ACTION_NONE = 0,		/**< No action  **/
	PAGE_ACTION_SWITCH_NEXT,	/**< jump to next page  **/
	PAGE_ACTION_SWITCH_PREV,	/**< jump to previous page  **/
	PAGE_ACTION_SCROLL_UP,		/**< scroll upwards  **/
	PAGE_ACTION_SCROLL_DOWN		/**< scroll downwards  **/

}CONIO_PAGE_inputAction_t;

/** \brief Page Element Init Function Pointer */
typedef RC_t (*fCONIO_PAGE_elementInit)(const void *apConfig);

/** \brief Page Element data read Function Pointer */
typedef RC_t (*fCONIO_PAGE_elementSrc)(void *apData, CONIO_PAGE_elementUpdateState_t aUpdateState);

/** \brief Page Element Update Function Pointer */
typedef RC_t (*fCONIO_PAGE_elementSink)(const void *apConfig, void *apData);

/** \brief Function pointer to check input is selected */
typedef RC_t (*fCONIO_PAGE_elementIsTriggered)(const void *apConfig, const void *apData);

/** \brief Function pointer for custom input handling */
typedef RC_t (*fCONIO_PAGE_inputActionHandler)(const void* apConfig);

/** \brief Page Model Function Pointer */
typedef RC_t (*fCONIO_PAGE_model)(CONIO_PAGE_state_t aState);

/** \brief Page Output element data structure */
typedef struct
{
	boolean_t					mbEnabled;	/**< Is the element enabled   **/
	//boolean_t					mbCyclic;	/**< Is the element cyclically updated   **/
	const void 					*mpConfig;	/**< Output Configuration  **/
	void						*mpData;	/**< element data object **/
	fCONIO_PAGE_elementInit		mfInit;		/**< element initialization function  **/
	fCONIO_PAGE_elementSrc		mfSrc;		/**< element data source function  **/
	fCONIO_PAGE_elementSink		mfSink;		/**< element data sink function  **/

}CONIO_PAGE_outputElement_t;

/** \brief Page Input element data structure */
typedef struct
{
	boolean_t						mbEnabled;		/**< Is the element enabled   **/
	const void 						*mpConfig;		/**< Input Configuration  **/
	fCONIO_PAGE_elementInit			mfInit;			/**< element initialization function  **/
	fCONIO_PAGE_elementIsTriggered	mfIsTriggered;	/**< element is selected check function  **/
	CONIO_PAGE_inputAction_t		mAction;		/**< on element selection action  **/
	fCONIO_PAGE_inputActionHandler	mfHandler;

}CONIO_PAGE_inputElement_t;

/** \brief Page data structure */
typedef struct
{
	uint16_t							mnTFA;		/**< Top Fixed Area in number of rows **/
	uint16_t							mnBFA;		/**< Bottom Fixed Area in number of rows **/
	const CONIO_PAGE_outputElement_t	*mpOut;		/**< Output element list **/
	const CONIO_PAGE_inputElement_t		*mpIn;		/**< Input element list **/
	CONIO_COLOR_rgb565_t				mBGColor;		/**< page background color **/

}CONIO_PAGE_cfg_t;

//-------------------------------------------------------------------- [Global prototypes]

#ifdef __cplusplus
extern "C"
{
#endif

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
RC_t CONIO_PAGE_clear(const CONIO_PAGE_cfg_t *apPage);

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
RC_t CONIO_PAGE_init(const CONIO_PAGE_cfg_t *apPage);


/**
 * \brief RC_t CONIO_PAGE_outputModel(const CONIO_PAGE_t *apPage)
 *
 * page output model.
 *
 * \param	apPage 	: page handle
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_PAGE_outputModel(const CONIO_PAGE_cfg_t *apPage, CONIO_PAGE_state_t aState);

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
RC_t CONIO_PAGE_inputModel(const CONIO_PAGE_cfg_t *apPage, const CONIO_PIXEL_t *apPixel, CONIO_PAGE_inputAction_t *apAction);

/**
 * \brief RC_t CONIO_PAGE_onInput(const CONIO_PAGE_cfg_t *apPage, CONIO_PAGE_inputAction_t aAction)
 *
 * page input model.
 *
 * \param	apPage 	: page handle
 * \param 	aAction:  input action
 * \return #RC_ERROR_BAD_PARAM if INVALID handle,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_PAGE_onInput(const CONIO_PAGE_cfg_t *apPage, CONIO_PAGE_inputAction_t aAction);

#ifdef __cplusplus
}
#endif

#endif /* SRC_BSW_DRV_CONIO_BASIC_CONIO_PAGE_H_ */
