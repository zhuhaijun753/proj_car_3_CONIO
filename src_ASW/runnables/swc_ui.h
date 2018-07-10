/**
 * \file	ui_car_state.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief car state ui header
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
#ifndef SRC_ASW_UI_UI_CAR_STATE_H_
#define SRC_ASW_UI_UI_CAR_STATE_H_

//-------------------------------------------------------------------- [Includes]

#include <BASIC/conio_page.h>

#include "signal_bb.h"
#include "det.h"

//-------------------------------------------------------------------- [Types]

typedef struct
{
	boolean_t mbLidarIsNew;
	boolean_t mbEnRLIsNew;
	boolean_t mbEnRRIsNew;
	boolean_t mbEnFLIsNew;
	boolean_t mbEnFRIsNew;
	NMT_data_t mNMTData;
}UI_NMT_data_t;

/** \brief UI car state data type*/
typedef struct
{
	boolean_t	mbIsNew;
	CARSTATE_data_t mData;

}UI_carState_data_t;

/** \brief UI car state data type*/
typedef struct
{
	boolean_t		mbIsNew;
	char_t			*mData;

}UI_carSafetyRunnable_data_t;

/** \brief UI car target speed data type*/
typedef struct
{
	boolean_t				mbIsNew;
	TARGETSPEED_data_t		mData;
	TARGETSPEED_time_t		mAge;

}UI_targetSpeed_data_t;

/** \brief UI car target speed data type*/
typedef struct
{
	boolean_t			mbIsNew;
	JOYSTICK_data_t		mData;
	JOYSTICK_time_t		mAge;

}UI_joystick_data_t;

/** \brief UI engine data type*/
typedef struct
{
	boolean_t		mbIsNew;
	ENGINE_data_t 	mData;

}UI_engine_data_t;

/** \brief UI DET data type*/
typedef struct
{
	DET__dumpinfoarray_t mData;
}UI_DET_data_t;

//-------------------------------------------------------------------- [Global prototypes]
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief RC_t UI_CARSTATE__init_normalState(const void *apConfig)
 *
 * Initialize normal state field.
 *
 * \param	apConfig : configuration
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__init_normalState(const void *apConfig);

/**
 * \brief RC_t UI_CARSTATE__init_safetyState(const void *apConfig)
 *
 * Initialize safety state field.
 *
 * \param	apConfig : configuration
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__init_safetyState(const void *apConfig);

/**
 * \brief RC_t UI_CARSTATE__init_safetyState(const void *apConfig)
 *
 * Initialize safety state field.
 *
 * \param	apConfig : configuration
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__init_safetyRunnable(const void *apConfig);

/**
 * \brief RC_t UI_CARSTATE__init_joystick(const void *apConfig)
 *
 * Initialize joystick field.
 *
 * \param	apConfig : configuration
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__init_joystick(const void *apConfig);

/**
 * \brief RC_t UI_CARSTATE__init_targetSpeed(const void *apConfig)
 *
 * Initialize target speed field.
 *
 * \param	apConfig : configuration
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__init_targetSpeed(const void *apConfig);

/**
 * \brief RC_t UI_CARSTATE__init_engine(const void *apConfig)
 *
 * Initialize engine field.
 *
 * \param	apConfig : configuration
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__init_engine(const void *apConfig);

/**
 * \brief RC_t UI_CARSTATE__init_DET(const void *apConfig)
 *
 * Initialize engine field.
 *
 * \param	apConfig : configuration
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__init_DET(const void *apConfig);

/**
 * \brief RC_t UI_CARSTATE__read_normalState(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
 *
 * read normal state field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_normalState(void *apData, CONIO_PAGE_elementUpdateState_t updateState);

/**
 * \brief RC_t UI_CARSTATE__read_safetyState(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
 *
 * read safety state field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_safetyState(void *apData, CONIO_PAGE_elementUpdateState_t updateState);

/**
 * \brief RC_t UI_CARSTATE__read_safetyState(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
 *
 * read safety state field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_safetyRunnable(void *apData, CONIO_PAGE_elementUpdateState_t updateState);

/**
 * \brief RC_t UI_CARSTATE__read_joystick(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
 *
 * read joystick field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_joystick(void *apData, CONIO_PAGE_elementUpdateState_t updateState);

/**
 * \brief RC_t UI_CARSTATE__read_targetSpeed(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
 *
 * read target speed field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_targetSpeed(void *apData, CONIO_PAGE_elementUpdateState_t updateState);

/**
 * \brief RC_t UI_CARSTATE__read_engine(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
 *
 * read engine field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_engine(void *apData, CONIO_PAGE_elementUpdateState_t updateState);

/**
 * \brief RC_t UI_CARSTATE__read_DET(void *apData)
 *
 * read DET field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_DET(void *apData, CONIO_PAGE_elementUpdateState_t updateState);

/**
 * \brief RC_t UI_CARSTATE__update_normalState(const void *apConfig, const void *apData)
 *
 * update normal state field.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__update_normalState(const void *apConfig, void *apData);

/**
 * \brief RC_t UI_CARSTATE__update_safetyState(const void *apConfig, const void *apData)
 *
 * update safety state field.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__update_safetyState(const void *apConfig, void *apData);

/**
 * \brief RC_t UI_CARSTATE__update_safetyState(const void *apConfig, const void *apData)
 *
 * update safety state field.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__update_safetyRunnable(const void *apConfig, void *apData);

/**
 * \brief RC_t UI_CARSTATE__update_joystick(const void *apConfig, const void *apData)
 *
 * update joystick field.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__update_joystick(const void *apConfig, void *apData);

/**
 * \brief RC_t UI_CARSTATE__update_targetSpeed(const void *apConfig, const void *apData)
 *
 * update target speed field.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__update_targetSpeed(const void *apConfig, void *apData);

/**
 * \brief RC_t UI_CARSTATE__update_engine(const void *apConfig, const void *apData)
 *
 * update engine field.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__update_engine(const void *apConfig, void *apData);

/**
 * \brief RC_t UI_CARSTATE__update_DET(const void *apConfig, const void *apData)
 *
 * update DET window.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__update_DET(const void *apConfig, void *apData);

/**
 * \brief RC_t UI_CARSTATE__update_DET_field(const void *apConfig, const void *apData)
 *
 * update DET field.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__update_DET_field(const void* apConfig, void* apData);

/**
 * \brief RC_t UI_CARSTATE__reset_DET()
 *
 * reset DET field.
 *
 * \param	void
 * \return #RC_SUCCESS
 *
 */
RC_t UI_CARSTATE__reset_DET(const void* apConfig);

/**
 * \brief RC_t UI_CARSTATE__event_DET_scrollDOWN()
 *
 *  DET field scroll DOWN.
 *
 * \param	void
 * \return error-codes on errors
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__event_DET_scrollDOWN(const void* apConfig);

/**
 * \brief RC_t UI_CARSTATE__event_DET_scrollUP()
 *
 *  DET field scroll UP.
 *
 * \param	void
 * \return error-codes on errors
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__event_DET_scrollUP(const void* apConfig);


RC_t UI_WORK__init_nmtState(const void *apConfig);
RC_t UI_WORK__read_normalState(void *apData, CONIO_PAGE_elementUpdateState_t updateState);
RC_t UI_CARWORK__update_NMTState(const void *apConfig, void *apData);
RC_t UI_WORK__init_nmtDisplay(const void *apConfig);
#ifdef __cplusplus
}
#endif


#endif /* SRC_ASW_UI_UI_CAR_STATE_H_ */
