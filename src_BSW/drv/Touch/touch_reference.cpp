/**
 * \file 	touch_reference.cpp
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

#include "touch_reference.h"
#include "tft_reference.h"

#include "QSPI.h"
#include "PORT.h"

#include <cstring>
#include <cstdlib>

#define ON_TOUCH_PIN Port20_Pin0

#define XMAX_TOUCH   3700.0f		//maybe a option is needed to trim the values
#define XMIN_TOUCH   240.0f
#define YMIN_TOUCH   300.0f
#define YMAX_TOUCH   3740.0f

typedef struct
{
	QSPI_channel_t 		mChannel;
	uint8_t				mTxBuffer[5];
	uint8_t				mRxBuffer[5];
	RC_t				mError;
}TouchDriver_t;

#pragma section ".data.tft"

TouchDriver_t gTouch;

#pragma section



/*
 * QSPI Application callback
 * @param aResult 		- QSPI transaction result
 * @param apUserData 	- user data
 * @return void
 */
static void touch_app_cb(QSPI_exchangeResult_t aResult, void *apUserData);

/*
 * release Resource
 * @param apRes		- resource
 * @return void
 */
static void touch_app_release(volatile boolean_t *apRes);

/*
 * wait for Resource
 * @param apRes		- resource
 * @return void
 */
static void touch_app_wait(volatile boolean_t *apRes);


/*
 * Initialize Touch QSPI channel.
 * @param NIL
 * @return #RC_SUCCESS on success,
 * 		   error-codes on data transfer errors
 */
RC_t TOUCHDRIVER_init()
{
	RC_t retCode = RC_ERROR;

	memset(&gTouch, 0, sizeof(TouchDriver_t));

	retCode = QSPI_channel_init(QSPI0,QSPI_CH9,&gTouch.mChannel);
	if (RC_SUCCESS != retCode)
	{
		return retCode;
	}

	/** Initializing TX buffer with touch read command **/
	gTouch.mTxBuffer[0] = 0x90;
	gTouch.mTxBuffer[1] = 0x00;
	gTouch.mTxBuffer[2] = 0xD0;
	gTouch.mTxBuffer[3] = 0x00;
	gTouch.mTxBuffer[4] = 0x00;

	return RC_SUCCESS;
}

/*
 * Read Touch by polling.
 * @param px - x pixel
 * @param py - y pixel
 * @return #RC_SUCCESS on touch,
 * 		   #RC_ERROR on no touch
 * 		   error-codes on data transfer errors
 */
RC_t TOUCHDRIVER_read_poll(uint16_t* px, uint16_t* py)
{
	RC_t retCode = RC_ERROR;
	PinData_t  pinVal = 0;

	PORTPIN_get(ON_TOUCH_PIN, &pinVal);

	if (pinVal == 0)
	{
		sint16_t read_x;
		sint16_t read_y;

		sint32_t calc_x = 0,
				 calc_y = 0;

		QSPI_exchange_param_t	touch_exchange_param;
		volatile boolean_t bResource  = FALSE;

		touch_exchange_param.mfOnExchange 	= &touch_app_cb;
		touch_exchange_param.mpUserData 	= (void *)&bResource;
		touch_exchange_param.mnDataWidth 	= 8;
		touch_exchange_param.mpReadPtr 		= &gTouch.mRxBuffer[0];
		touch_exchange_param.mpWritePtr		= &gTouch.mTxBuffer[0];
		touch_exchange_param.mnLength 		= 5;

		retCode = QSPI_channel_exchange(&gTouch.mChannel, QSPI_exchange_readwrite, &touch_exchange_param);
		if (RC_SUCCESS != retCode)
		{
			return retCode;
		}

		touch_app_wait(&bResource);

		if (RC_SUCCESS == gTouch.mError)
		{
			read_x = ((gTouch.mRxBuffer[1]<<8) | (gTouch.mRxBuffer[2])) >> 3;
			read_y = ((gTouch.mRxBuffer[3]<<8) | (gTouch.mRxBuffer[4])) >> 3;


			calc_x = ((float)read_x - XMIN_TOUCH) / (XMAX_TOUCH - XMIN_TOUCH) * (float)TFT_XSIZE;
			if (0 > calc_x)
				calc_x = 0;
			if (TFT_XSIZE <= calc_x)
				calc_x = TFT_XSIZE - 1;

			calc_y = ((float)read_y - YMIN_TOUCH) / (YMAX_TOUCH - YMIN_TOUCH) * (float)TFT_YSIZE;
			if (0 > calc_y)
				calc_y = 0;
			if (TFT_YSIZE <= calc_y)
				calc_y = TFT_YSIZE - 1;

			*px = (uint16_t)calc_x;
			*py = (uint16_t)calc_y;
		}

		return gTouch.mError;
	}

	return RC_ERROR;

}

/*
 * release Resource
 * @param apRes		- resource
 * @return void
 */
static void touch_app_release(volatile boolean_t *apRes)
{
	// Update Value
	*((volatile boolean_t *)apRes) = TRUE;
}

/*
 * wait for Resource
 * @param apRes		- resource
 * @return void
 */
static void touch_app_wait(volatile boolean_t *apRes)
{
	// Busy Wait
	while (TRUE != *apRes);
}

/*
 * QSPI Application callback
 * @param aResult 		- QSPI transaction result
 * @param apUserData 	- user data
 * @return void
 */
static void touch_app_cb(QSPI_exchangeResult_t aResult, void *apUserData)
{
	if (QSPI_exchangeResult_SUCCESS == aResult)
	{
		if (NULL != apUserData)
			touch_app_release((volatile boolean_t *)apUserData);

		gTouch.mError = RC_SUCCESS;
	}
	else
	{
		gTouch.mError = RC_ERROR;
	}
}



