/**
 * \file	ui_car_state.cpp
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief car state ui API implementation
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


//-------------------------------------------------------------------- [header]

#include "swc_ui.h"
#include "BASIC/conio_page.h"
#include "VIEW/ui_view_1_cfg.h"
#include "VIEW/ui_view_2_cfg.h"
#include "VIEW/ui_view_work_cfg.h"

//-------------------------------------------------------------------- [global functions]



typedef struct
{
	boolean_t 	mbIsFirstRead;
	sint16_t	mnFirstIdx;
	sint16_t	mnLastIdx;
}UI_DET_driver_t;

#pragma section ".data.ui"
UI_DET_driver_t gUIDET = {FALSE};
#pragma section

RC_t UI_WORK__init_nmtState(const void *apConfig)
{
	RC_t ret = RC_SUCCESS;

	const UI_NMTState_cfg_t *pStateCfg = (const UI_NMTState_cfg_t *)apConfig;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_TEXT_write(&pStateCfg->mKey, pStateCfg->mpKey);
	if (RC_SUCCESS != ret)
		return ret;

	return RC_SUCCESS;
}


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
RC_t UI_CARSTATE__init_normalState(const void *apConfig)
{
	RC_t ret = RC_SUCCESS;

	const UI_normalState_cfg_t *pStateCfg = (const UI_normalState_cfg_t *)apConfig;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_TEXT_write(&pStateCfg->mKey, pStateCfg->mpKey);
	if (RC_SUCCESS != ret)
		return ret;

	return RC_SUCCESS;
}

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
RC_t UI_CARSTATE__init_safetyState(const void *apConfig)
{
	RC_t ret = RC_SUCCESS;

	const UI_safetyState_cfg_t *pSafetyStateCfg = (const UI_safetyState_cfg_t *)apConfig;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_TEXT_write(&pSafetyStateCfg->mKey, pSafetyStateCfg->mpKey);
		if (RC_SUCCESS != ret)
			return ret;

	return RC_SUCCESS;
}

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
RC_t UI_CARSTATE__init_safetyRunnable(const void *apConfig)
{
	RC_t ret = RC_SUCCESS;

	const UI_safety_runnable_cfg_t *pSafetyRunnableCfg = (const UI_safety_runnable_cfg_t *)apConfig;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_TEXT_write(&pSafetyRunnableCfg->mKey, pSafetyRunnableCfg->mpKey);
		if (RC_SUCCESS != ret)
			return ret;

	return RC_SUCCESS;
}

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
RC_t UI_CARSTATE__init_joystick(const void *apConfig)
{
	RC_t ret = RC_SUCCESS;

	const UI_joystick_cfg_t *pJoystickCfg = (const UI_joystick_cfg_t *)apConfig;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_TEXT_write(&pJoystickCfg->mKey, pJoystickCfg->mpKey);
		if (RC_SUCCESS != ret)
			return ret;

	return RC_SUCCESS;
}

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
RC_t UI_CARSTATE__init_targetSpeed(const void *apConfig)
{
	RC_t ret = RC_SUCCESS;

	const UI_targetSpeed_cfg_t *pTargetSpeedCfg = (const UI_targetSpeed_cfg_t *)apConfig;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_TEXT_write(&pTargetSpeedCfg->mKey, pTargetSpeedCfg->mpKey);
		if (RC_SUCCESS != ret)
			return ret;

	return RC_SUCCESS;
}

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
RC_t UI_CARSTATE__init_engine(const void *apConfig)
{
	RC_t ret = RC_SUCCESS;

	const UI_engine_cfg_t *pEngineCfg = (const UI_engine_cfg_t *)apConfig;

	uint16_t i = 0,
			 j = 0;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_TEXT_write(&pEngineCfg->mKey, pEngineCfg->mpKey);
	if (RC_SUCCESS != ret)
		return ret;

	for (i = 0;  i < UI_CAR_STATE_ENGINES; i++)
	{
		for (j = 0;  j < UI_CAR_STATE_ENGINE_DIRECTION; j++)
		{
			ret = CONIO_RECT_draw(&pEngineCfg->mVal[i][j]);
			if (RC_SUCCESS != ret)
				return ret;

			CONIO_RECT_fill(&pEngineCfg->mVal[i][j], RECT_FILL_FROM_BL, pEngineCfg->mnClrColor, 100);
		}
	}


	return RC_SUCCESS;
}

/**
 * \brief RC_t UI_CARSTATE__init_DET(const void *apConfig)
 *
 * Initialize DET fields.
 *
 * \param	apConfig : configuration
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__init_DET(const void* apConfig)
{
	gUIDET.mbIsFirstRead = TRUE;

	return RC_SUCCESS;
}

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
RC_t UI_CARSTATE__read_normalState(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
{
	UI_carState_data_t	*pCarState = (UI_carState_data_t *)apData;
	CARSTATE_data_t state = CARSTATE_get(&so_carstate);

	if (NULL == apData)
		return RC_ERROR_BAD_PARAM;

	if (ELEMENT_UPDATE_FORCED == updateState)
	{
		pCarState->mData = state;
		pCarState->mbIsNew = TRUE;
	}
	else
	{
		// checking for modifications
		if (pCarState->mData.state_normal != state.state_normal)
		{
			pCarState->mData = state;
			pCarState->mbIsNew = TRUE;
		}
		else
		{
			pCarState->mbIsNew = FALSE;
		}
	}

	return RC_SUCCESS;
}

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
RC_t UI_CARSTATE__read_safetyState(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
{
	UI_carState_data_t	*pCarState = (UI_carState_data_t *)apData;
	CARSTATE_data_t state = CARSTATE_get(&so_carstate);

	if (NULL == apData)
		return RC_ERROR_BAD_PARAM;

	if (ELEMENT_UPDATE_FORCED == updateState)
	{
		pCarState->mData = state;
		pCarState->mbIsNew = TRUE;
	}
	else
	{
		// checking for modifications
		if (pCarState->mData.state_safety != state.state_safety)
		{
			pCarState->mData = state;
			pCarState->mbIsNew = TRUE;
		}
		else
		{
			pCarState->mbIsNew = FALSE;
		}
	}

	return RC_SUCCESS;
}

const static char UI_allOkString[] = "-";

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
RC_t UI_CARSTATE__read_safetyRunnable(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
{
	UI_carSafetyRunnable_data_t	*pCarSafetyRunnable = (UI_carSafetyRunnable_data_t *)apData;
	char_t* data = (char_t*)DET_getFunctionName();
	if (NULL == data)
	{
		data = (char_t*)UI_allOkString;
	}

	if (NULL == apData)
		return RC_ERROR_BAD_PARAM;

	if (ELEMENT_UPDATE_FORCED == updateState)
	{
		//Initial display of page
		pCarSafetyRunnable->mData = data;
		pCarSafetyRunnable->mbIsNew = TRUE;
	}
	else
	{
		//checking for modifications. Only show content if it has been changed

		//Check error state
		CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

		//Check if data has been updated
		if (data == pCarSafetyRunnable->mData)
		{
			pCarSafetyRunnable->mbIsNew = FALSE;
		}
		else
		{
			pCarSafetyRunnable->mbIsNew = TRUE;
		}

		//Only show function name in case safetState is not ok
		if (IS_SAFE_ALLOK == carState.state_safety)
		{
			//Set string to ALL OK
			pCarSafetyRunnable->mData = (char_t*)UI_allOkString;
		}
		else
		{
			pCarSafetyRunnable->mData = data;
		}


	}
	return RC_SUCCESS;
}

/**
 * \brief RC_t UI_CARSTATE__read_joystick(void *apData)
 *
 * read joystick field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_joystick(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
{
	UI_joystick_data_t	*pJoystick = (UI_joystick_data_t *)apData;
	JOYSTICK_data_t joystick = JOYSTICK_get(&so_joystick);

	if (NULL == apData)
		return RC_ERROR_BAD_PARAM;

	pJoystick->mAge 	= JOYSTICK_get_age(&so_joystick);

	if (ELEMENT_UPDATE_FORCED == updateState)
	{
		pJoystick->mData = joystick;
		pJoystick->mbIsNew = TRUE;
	}
	else
	{
		// checking for modifications
		if ((pJoystick->mData.x_in != joystick.x_in) ||
			(pJoystick->mData.y_in != joystick.y_in) ||
			(pJoystick->mData.Rz_in != joystick.Rz_in))
		{
			pJoystick->mData 	= joystick;
			pJoystick->mbIsNew 	= TRUE;
		}
		else
		{
			pJoystick->mbIsNew = FALSE;
		}
	}

	return RC_SUCCESS;
}



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
RC_t UI_CARSTATE__read_targetSpeed(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
{
	UI_targetSpeed_data_t	*pTargetSpeed = (UI_targetSpeed_data_t *)apData;
	TARGETSPEED_data_t targetSpeed = TARGETSPEED_get(&so_targetspeed);

	if (NULL == apData)
		return RC_ERROR_BAD_PARAM;

	pTargetSpeed->mAge 		= TARGETSPEED_get_age(&so_targetspeed);

	if (ELEMENT_UPDATE_FORCED == updateState)
	{
		pTargetSpeed->mData = targetSpeed;
		pTargetSpeed->mbIsNew = TRUE;
	}
	else
	{
		// checking for modifications
		if ((pTargetSpeed->mData.vx != targetSpeed.vx) ||
			(pTargetSpeed->mData.vy != targetSpeed.vy) ||
			(pTargetSpeed->mData.vphi != targetSpeed.vphi))
		{
			pTargetSpeed->mData 	= targetSpeed;
			pTargetSpeed->mbIsNew 	= TRUE;
		}
		else
		{
			pTargetSpeed->mbIsNew = FALSE;
		}
	}

	return RC_SUCCESS;
}



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
RC_t UI_CARSTATE__read_engine(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
{
	UI_engine_data_t	*pEngine = (UI_engine_data_t *)apData;
	ENGINE_data_t		engine = ENGINE_get(&so_out_engine);

	if (NULL == apData)
		return RC_ERROR_BAD_PARAM;

	//Scale Engine from RPM to Percent
	engine.speed_frontLeft /= ENGINE_PERCENT2RPM;
	engine.speed_frontRight /= ENGINE_PERCENT2RPM;
	engine.speed_rearLeft /= ENGINE_PERCENT2RPM;
	engine.speed_rearRight /= ENGINE_PERCENT2RPM;


	if (ELEMENT_UPDATE_FORCED == updateState)
	{
		pEngine->mData 		= engine;
		pEngine->mbIsNew 	= TRUE;
	}
	else
	{
		// checking for modifications
		if ((pEngine->mData.speed_frontLeft != engine.speed_frontLeft) ||
			(pEngine->mData.speed_frontRight != engine.speed_frontRight) ||
			(pEngine->mData.speed_rearLeft != engine.speed_rearLeft) ||
			(pEngine->mData.speed_rearRight != engine.speed_rearRight))
		{
			pEngine->mData 		= engine;
			pEngine->mbIsNew 	= TRUE;
		}
		else
		{
			pEngine->mbIsNew 	= FALSE;
		}
	}

	return RC_SUCCESS;
}

/**
 * \brief RC_t UI_CARSTATE__read_DET(void *apData, CONIO_PAGE_elementUpdateState_t updateState)
 *
 * read DET field.
 *
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid data handle
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__read_DET(void* apData, CONIO_PAGE_elementUpdateState_t updateState)
{
	RC_t retCode;
	UI_DET_data_t	*pDET = (UI_DET_data_t *)apData;

	if (TRUE == gUIDET.mbIsFirstRead)
	{
		retCode = DET_get_first(&pDET->mData);
		if (RC_SUCCESS == retCode)
			gUIDET.mbIsFirstRead = FALSE;

		gUIDET.mnLastIdx = 0;
	}
	else
	{
		retCode = DET_get_next(&pDET->mData, &gUIDET.mnLastIdx);
 	}

	if (RC_SUCCESS == retCode)
		gUIDET.mnFirstIdx = (gUIDET.mnLastIdx < UI_CAR_STATE_DET_ENTRIES) ? 0 : (gUIDET.mnLastIdx - UI_CAR_STATE_DET_ENTRIES + 1);

	return retCode;
}

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
RC_t UI_CARSTATE__update_normalState(const void *apConfig, void *apData)
{
	RC_t ret = RC_SUCCESS;

	const UI_carState_data_t	*pCarState = (const UI_carState_data_t *)apData;
	UI_normalState_cfg_t *pStateCfg = (UI_normalState_cfg_t *)apConfig;
	STATE_normalState_t nState;

	if ((NULL == apData) || (NULL == apConfig))
		return RC_ERROR_BAD_PARAM;

	if (TRUE == pCarState->mbIsNew)
	{
		nState = (STATE_normalState_t)pCarState->mData.state_normal;

		if (NORMAL_STATE_MAX <= nState)
			return RC_ERROR_BAD_DATA;

		ret = CONIO_TEXT_write(&pStateCfg->mVal, "%s", STATE_normal_strTable[nState]);
	}

	return ret;
}

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
RC_t UI_CARSTATE__update_safetyState(const void *apConfig, void *apData)
{
	RC_t ret = RC_SUCCESS;

	const UI_carState_data_t	*pCarState = (const UI_carState_data_t *)apData;
	UI_safetyState_cfg_t *pSafetyStateCfg = (UI_safetyState_cfg_t *)apConfig;
	STATE_safetyState_t nState;

	if ((NULL == apData) || (NULL == apConfig))
		return RC_ERROR_BAD_PARAM;

	if (TRUE == pCarState->mbIsNew)
	{
		nState = (STATE_safetyState_t)pCarState->mData.state_safety;

		if (SAFETY_STATE_MAX <= nState)
			return RC_ERROR_BAD_DATA;

		ret = CONIO_TEXT_write(&pSafetyStateCfg->mVal, "%s", STATE_safety_strTable[nState]);
	}

	return ret;
}

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
RC_t UI_CARSTATE__update_safetyRunnable(const void *apConfig, void *apData)
{
	RC_t ret = RC_SUCCESS;

	UI_carSafetyRunnable_data_t	*pCarSafetyRunnable = (UI_carSafetyRunnable_data_t *)apData;
	const UI_safety_runnable_cfg_t *pSafetyRunnableCfg = (const UI_safety_runnable_cfg_t *)apConfig;

	if ((NULL == apData) || (NULL == apConfig))
		return RC_ERROR_BAD_PARAM;

	if (TRUE == pCarSafetyRunnable->mbIsNew)
	{
		ret = CONIO_TEXT_write(&pSafetyRunnableCfg->mVal, "%s", pCarSafetyRunnable->mData);
	}

	return ret;
}

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
RC_t UI_CARSTATE__update_joystick(const void *apConfig, void *apData)
{
	RC_t ret = RC_SUCCESS;

	const UI_joystick_cfg_t *pJoystickCfg = (const UI_joystick_cfg_t *)apConfig;
	UI_joystick_data_t *pJoystick = (UI_joystick_data_t *)apData;

	if ((NULL == apData) || (NULL == apConfig))
		return RC_ERROR_BAD_PARAM;

	if (TRUE == pJoystick->mbIsNew)
	{
		//char_t cBoolVal;

		ret = CONIO_TEXT_write(&pJoystickCfg->mVal[0], "%d", pJoystick->mData.x_in);
		if (RC_SUCCESS != ret)
			return ret;

		ret = CONIO_TEXT_write(&pJoystickCfg->mVal[1], "%d", pJoystick->mData.y_in);
		if (RC_SUCCESS != ret)
			return ret;

		ret = CONIO_TEXT_write(&pJoystickCfg->mVal[2], "%d", pJoystick->mData.Rz_in);
		if (RC_SUCCESS != ret)
			return ret;

		/* @PF

		cBoolVal = (TRUE == pJoystick->mData.emergency_stop) ? 'Y' : 'N';
		ret = CONIO_TEXT_write(&pJoystickCfg->mVal[3], "%c", cBoolVal);
		if (RC_SUCCESS != ret)
			return ret;

		cBoolVal = (TRUE == pJoystick->mData.gradual_stop) ? 'Y' : 'N';
		ret = CONIO_TEXT_write(&pJoystickCfg->mVal[4], "%c", cBoolVal);
		if (RC_SUCCESS != ret)
			return ret;

	    */
	}

	ret = CONIO_TEXT_write(&pJoystickCfg->mVal[3], "%d", pJoystick->mAge);
	if (RC_SUCCESS != ret)
		return ret;

	return ret;
}

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
RC_t UI_CARSTATE__update_targetSpeed(const void *apConfig, void *apData)
{
	RC_t ret = RC_SUCCESS;

	const UI_targetSpeed_cfg_t *pTargetSpeedCfg = (const UI_targetSpeed_cfg_t *)apConfig;
	UI_targetSpeed_data_t *pTargetSpeed = (UI_targetSpeed_data_t *)apData;

	if ((NULL == apData) || (NULL == apConfig))
		return RC_ERROR_BAD_PARAM;

	if (TRUE == pTargetSpeed->mbIsNew)
	{
		ret = CONIO_TEXT_write(&pTargetSpeedCfg->mVal[0], "%d", pTargetSpeed->mData.vx);
		if (RC_SUCCESS != ret)
			return ret;

		ret = CONIO_TEXT_write(&pTargetSpeedCfg->mVal[1], "%d", pTargetSpeed->mData.vy);
		if (RC_SUCCESS != ret)
			return ret;

		ret = CONIO_TEXT_write(&pTargetSpeedCfg->mVal[2], "%d", pTargetSpeed->mData.vphi);
		if (RC_SUCCESS != ret)
			return ret;
	}

	ret = CONIO_TEXT_write(&pTargetSpeedCfg->mVal[3], "%d", pTargetSpeed->mAge);
	if (RC_SUCCESS != ret)
		return ret;

	return ret;
}

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
RC_t UI_CARSTATE__update_engine(const void *apConfig, void *apData)
{
	RC_t ret = RC_SUCCESS;

	uint16_t i = 0;

	const UI_engine_cfg_t *pEngineCfg = (const UI_engine_cfg_t *)apConfig;
	UI_engine_data_t *pEngine = (UI_engine_data_t *)apData;

	if ((NULL == apData) || (NULL == apConfig))
		return RC_ERROR_BAD_PARAM;

	if (TRUE == pEngine->mbIsNew)
	{
		sint32_t nSpeeds[UI_CAR_STATE_ENGINES] = {0};

		// Left side engines are reversed
		nSpeeds[0] = -(sint32_t)pEngine->mData.speed_frontLeft;
		nSpeeds[1] = (sint32_t)pEngine->mData.speed_frontRight;
		nSpeeds[2] = -(sint32_t)pEngine->mData.speed_rearLeft;
		nSpeeds[3] = (sint32_t)pEngine->mData.speed_rearRight;

		for (i = 0;  i < UI_CAR_STATE_ENGINES; i++)
		{
			CONIO_RECT_fill(&pEngineCfg->mVal[i][0], RECT_FILL_FROM_BL, pEngineCfg->mnClrColor, 100);
			CONIO_RECT_fill(&pEngineCfg->mVal[i][1], RECT_FILL_FROM_BL, pEngineCfg->mnClrColor, 100);


			if (0 > nSpeeds[i])
				CONIO_RECT_fill(&pEngineCfg->mVal[i][0], RECT_FILL_FROM_TR, pEngineCfg->mnSetColor, (_abs(nSpeeds[i]) % 101));
			else if (0 < nSpeeds[i])
				CONIO_RECT_fill(&pEngineCfg->mVal[i][1], RECT_FILL_FROM_BL, pEngineCfg->mnSetColor, (_abs(nSpeeds[i]) % 101));
		}
	}

	return ret;
}


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
RC_t UI_CARSTATE__update_DET(const void* apConfig, void* apData)
{
	RC_t ret = RC_SUCCESS;
	uint16_t i = 0;

	uint32_t nEntries = DET_getFillLevel();

	const UI_DET_cfg_t *pDETCfg = (const UI_DET_cfg_t *)apConfig;
	UI_DET_data_t *pDET = (UI_DET_data_t *)apData;

	if (FALSE == DET_isNew())
		return RC_SUCCESS;

	gUIDET.mnFirstIdx = (nEntries < UI_CAR_STATE_DET_ENTRIES) ? 0 : (nEntries - UI_CAR_STATE_DET_ENTRIES);

	for (i = 0; i < UI_CAR_STATE_DET_ENTRIES; i++)
	{
		if (RC_SUCCESS == DET_readByIndex(&(pDET->mData), (gUIDET.mnFirstIdx + i)))
		{
			gUIDET.mnLastIdx = (gUIDET.mnFirstIdx + i);
			ret = UI_CARSTATE__update_DET_field(&pDETCfg[i], apData);
			if (RC_SUCCESS != ret)
				break;
		}
		else
		{
			break;
		}
	}

	return ret;
}


/**
 * \brief RC_t UI_CARSTATE__update_DET_field(const void* apConfig, void* apData)
 *
 * update DET field.
 *
 * \param	apConfig 	: configuration
 * \param 	apData		: data to update
 * \return #RC_ERROR_BAD_PARAM if invalid configuration
 * \return #RC_SUCCESS otherwise
 *
 */

RC_t UI_CARSTATE__update_DET_field(const void* apConfig, void* apData)
{
	RC_t ret = RC_SUCCESS;

	const UI_DET_cfg_t *pDETCfg = (const UI_DET_cfg_t *)apConfig;
	UI_DET_data_t *pDET = (UI_DET_data_t *)apData;

	if ((NULL == apData) || (NULL == apConfig))
		return RC_ERROR_BAD_PARAM;

	ret = CONIO_TEXT_write(&pDETCfg->mValTS, "%d::%d::%d", pDET->mData.m_timestamp.mins, pDET->mData.m_timestamp.secs, pDET->mData.m_timestamp.msecs);
	if (RC_SUCCESS != ret)
		return ret;

	ret = CONIO_TEXT_write(&pDETCfg->mValCore, "%d(%d)", pDET->mData.m_uservalue, pDET->mData.core);
	if (RC_SUCCESS != ret)
		return ret;

	ret = CONIO_TEXT_write(&pDETCfg->mValStr, "%s", pDET->mData.mp_message);
	if (RC_SUCCESS != ret)
		return ret;

	return RC_SUCCESS;
}


/**
 * \brief RC_t UI_CARSTATE__reset_DET()
 *
 * reset DET field.
 *
 * \param	apConfig 	: configuration
 * \return #RC_SUCCESS
 *
 */
RC_t UI_CARSTATE__reset_DET(const void* apConfig)
{
	DET_clr_age();
	gUIDET.mbIsFirstRead = TRUE;

	return RC_SUCCESS;
}

/**
 * \brief RC_t UI_CARSTATE__event_DET_scrollDOWN()
 *
 *  DET field scroll DOWN.
 *
 * \param	apConfig 	: configuration
 * \return error-codes on errors
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__event_DET_scrollDOWN(const void* apConfig)
{
	//I do not like bring in page related concepts in here
	//but I am helpless
	const CONIO_PAGE_cfg_t* pPage =  (const CONIO_PAGE_cfg_t*)apConfig;;
	UI_DET_data_t *pDET = NULL;

	RC_t ret = RC_SUCCESS;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	const UI_DET_cfg_t *pDETCfg = (const UI_DET_cfg_t *)pPage->mpOut->mpConfig;

	uint32_t nEntries = DET_getFillLevel();

	pDET = (UI_DET_data_t *)pPage->mpOut->mpData;

	if (gUIDET.mnLastIdx >= (nEntries - 1))
		return RC_SUCCESS;

	gUIDET.mnLastIdx++;
	gUIDET.mnFirstIdx = (nEntries < UI_CAR_STATE_DET_ENTRIES) ? 0 : (gUIDET.mnLastIdx - UI_CAR_STATE_DET_ENTRIES + 1);

	for (uint16_t i = 0; i < UI_CAR_STATE_DET_ENTRIES; i++)
	{
		if (RC_SUCCESS == DET_readByIndex(&(pDET->mData), (gUIDET.mnFirstIdx + i)))
		{
			gUIDET.mnLastIdx = (gUIDET.mnFirstIdx + i);
			ret = UI_CARSTATE__update_DET_field(&pDETCfg[i], &(pDET->mData));
			if (RC_SUCCESS != ret)
				break;
		}
		else
		{
			break;
		}
	}

	return ret;
}

/**
 * \brief RC_t UI_CARSTATE__event_DET_scrollUP()
 *
 *  DET field scroll UP.
 *
 * \param	apConfig 	: configuration
 * \return error-codes on errors
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t UI_CARSTATE__event_DET_scrollUP(const void* apConfig)
{

	//I do not like bring in page related concepts in here
	//but I am helpless
	const CONIO_PAGE_cfg_t* pPage =  (const CONIO_PAGE_cfg_t*)apConfig;;
	UI_DET_data_t *pDET = NULL;

	RC_t ret = RC_SUCCESS;

	if (NULL == apConfig)
		return RC_ERROR_BAD_PARAM;

	const UI_DET_cfg_t *pDETCfg = (const UI_DET_cfg_t *)pPage->mpOut->mpConfig;

	uint32_t nEntries = DET_getFillLevel();

	pDET = (UI_DET_data_t *)pPage->mpOut->mpData;

	if (gUIDET.mnFirstIdx <= 0)
		return RC_SUCCESS;

	gUIDET.mnFirstIdx--;

	for (uint16_t i = 0; i < UI_CAR_STATE_DET_ENTRIES; i++)
	{
		if (RC_SUCCESS == DET_readByIndex(&(pDET->mData), (gUIDET.mnFirstIdx + i)))
		{
			gUIDET.mnLastIdx = (gUIDET.mnFirstIdx + i);
			ret = UI_CARSTATE__update_DET_field(&pDETCfg[i], &(pDET->mData));
			if (RC_SUCCESS != ret)
				break;
		}
		else
		{
			break;
		}
	}

	return ret;
}
