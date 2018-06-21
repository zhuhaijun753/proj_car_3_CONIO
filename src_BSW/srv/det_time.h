/**
 * \file 	det_time.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date 	16-Aug-2017
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
#ifndef SRC_BSW_SRV_DET_TIME_H_
#define SRC_BSW_SRV_DET_TIME_H_

#include "global.h"
#include "Gpt_timer.h"

/** \brief TIME structure */
typedef struct
{
	uint8_t		hour;		/**< time hours */
	uint8_t		mins;		/**< time minutes */
	uint8_t 	secs;		/**< time seconds */
	uint16_t	msecs;		/**< time milliseconds */
}TIME_t;

#ifdef __cplusplus
extern "C"
{
#endif


/*
 * DET_TIME_ISR.
 * @param user_data : unused
 * @return void
 */
void DET_TIME_ISR(int user_data);

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
RC_t DET_TIME_init();

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
RC_t DET_TIME_ctrl(GPT12_OnOff_t ctrl);

/**
 * \brief uint32_t DET_TIME_get()
 *
 *  get DET Timer value.
 *
 * \param	void
 * \return  timer value
 *
 */
uint32_t DET_TIME_get();

/**
 * \brief TIME_t DET_TIME_get2()
 *
 *  get time in hours, mins, secs and msec
 *
 * \param	void
 * \return time value
 *
 */
TIME_t DET_TIME_get2();

/**
 * Sets the offset time to the current det time
 */
void DET_TIME_resetStopwatch();

/**
 * Calculates the delay time since stopwatch reset and stores it
 * Can be checked in a debugger watch window
 */
void DET_TIME_stopStopwatch();



#ifdef __cplusplus
}
#endif

#endif /* SRC_BSW_SRV_DET_TIME_H_ */
