/**
 * \file 	det_time.cpp
* \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date 	16-Aug-2017
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

#include "det_time.h"

/** \brief DET Timer Precision */
#define DET_TIME_PRECISION 	(0.00064f) 	//640ns

/** \brief Helper Macros */
#define TIME_MSECS_PER_SEC 	(1000)
#define TIME_SECS_PER_MIN 	(60)
#define TIME_MINS_PER_HOUR 	(60)

/** \brief DET Timer States */
typedef enum
{
	DET_TIME_UnInitialized = 0,
	DET_TIME_Initialized,
	DET_TIME_Running,
	DET_TIME_Stopped

}DET_TIME_State_t;


#pragma section ".DET.data"

/** \brief DET Timer Ticks */
volatile uint32_t gDetTimeMSB = 0;

/** \brief DET Timer State */
DET_TIME_State_t gDetTimeState = DET_TIME_UnInitialized;

volatile static uint32_t DET_TIME__stopTime = 0;

volatile static float32_t DET_TIME__delayTimeInMs = 0;

//volatile static uint32_t DET_TIME__delayTimeInTicks = 0;

#pragma section


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief STATIC void DET_TIME__tick()
 *
 *  handler Timer tick
 *
 * \param	void
 * \return  void
 *
 */
STATIC void DET_TIME__tick();

#ifdef __cplusplus
}
#endif

/*
 * DET_TIME_ISR.
 * @param user_data : unused
 * @return void
 */
void DET_TIME_ISR(int user_data)
{
	DET_TIME__tick();
	GPT12_Reload(GPT12_T2);
}


/**
 * \brief RC_t DET_TIME_init()
 *
 *  initialize DET Timer.
 *
 * \param	void
 * \return error-codes on errors
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t DET_TIME_init()
{
	GPT12_Init(GPT12_T2);
	GPT12_StartStop(GPT12_T2,Stop);
	gDetTimeState = DET_TIME_Initialized;

	return RC_SUCCESS;
}


/**
 * \brief RC_t DET_TIME_ctrl(GPT12_OnOff_t ctrl)
 *
 *  control DET Timer.
 *
 * \param	ctrl 	: ctrl command
 * \return error-codes on errors
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t DET_TIME_ctrl(GPT12_OnOff_t ctrl)
{
	if (Start == ctrl)
	{
		if ((DET_TIME_Stopped == gDetTimeState) || (DET_TIME_Initialized == gDetTimeState))
		{
			GPT12_StartStop(GPT12_T2, Start);
			gDetTimeState = DET_TIME_Running;
			return RC_SUCCESS;
		}
		else if (DET_TIME_Running == gDetTimeState)
		{
			return RC_SUCCESS;
		}
	}
	else if (Stop == ctrl)
	{
		if (DET_TIME_Running == gDetTimeState)
		{
			GPT12_StartStop(GPT12_T2, Stop);
			gDetTimeState = DET_TIME_Stopped;
			return RC_SUCCESS;
		}
	}

	return RC_ERROR;
}


/**
 * \brief uint32_t DET_TIME_get()
 *
 *  get DET Timer value.
 *
 * \param	void
 * \return  timer value
 *
 */
uint32_t DET_TIME_get()
{
	if (DET_TIME_Running == gDetTimeState)
		return ((GPT12_TimerRead(GPT12_T2) & 0x0000FFFF) | (uint32_t)(gDetTimeMSB << 16));
	else
		return 0;
}

/**
 * \brief TIME_t DET_TIME_get2()
 *
 *  get time in hours, mins, secs and msec
 *
 * \param	void
 * \return time value
 *
 */
TIME_t DET_TIME_get2()
{
	TIME_t time = {0, 0, 0, 0};

	if (DET_TIME_Running == gDetTimeState)
	{
		uint32_t 	whole_ms 	=   (uint32_t)((float32_t)DET_TIME_get() * DET_TIME_PRECISION);

		time.msecs 	= (whole_ms % TIME_MSECS_PER_SEC);
		time.secs 	= (whole_ms / TIME_MSECS_PER_SEC) % TIME_SECS_PER_MIN;
		time.mins 	= (whole_ms / (TIME_MSECS_PER_SEC * TIME_SECS_PER_MIN)) % TIME_MINS_PER_HOUR;
		time.hour 	= (whole_ms / (TIME_MSECS_PER_SEC * TIME_SECS_PER_MIN * TIME_MINS_PER_HOUR));
	}

	return time;
}


/**
 * \brief STATIC void DET_TIME__tick()
 *
 *  handler Timer tick
 *
 * \param	void
 * \return  void
 *
 */
STATIC void DET_TIME__tick()
{
	gDetTimeMSB++;
}

/**
 * Sets the offset time to the current det time
 */
void DET_TIME_resetStopwatch()
{
	DET_TIME__stopTime = DET_TIME_get();
}

/**
 * Calculates the delay time since stopwatch reset and stores it
 * Can be checked in a debugger watch window
 */
void DET_TIME_stopStopwatch()
{
	uint32_t deltaTime = DET_TIME_get() - DET_TIME__stopTime;

	DET_TIME__delayTimeInMs = (float32_t)deltaTime * DET_TIME_PRECISION;
}

