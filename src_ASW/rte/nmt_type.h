/*
 * NMT_type.h
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm, 2016
 */



#ifndef NMT_TYPE_H_
#define NMT_TYPE_H_

#include "global.h"
#include "canopen.h"

/*****************************************************************************************
 *************** Type Definition for NMT  signal class **************************
 *****************************************************************************************/


typedef struct
{
	uint32_t dd_lidar;
	uint32_t dd_engineRL;
	uint32_t dd_engineRR;
	uint32_t dd_engineFL;
	uint32_t dd_engineFR;
} NMT_raw_t;

typedef struct
{
	CANopen_NMT_states_t NMT_lidar;
	CANopen_NMT_states_t NMT_engineRL;
	CANopen_NMT_states_t NMT_engineRR;
	CANopen_NMT_states_t NMT_engineFL;
	CANopen_NMT_states_t NMT_engineFR;

} NMT_data_t;


#define NMT_INIT_DATA ((NMT_data_t){CANopen_NMT_STATE_Initialization})
#define NMT_INIT_RAW  ((NMT_raw_t){0})

/*****************************************************************************************
 *************** Type specific scaler functions ******************************************
 *****************************************************************************************/
#ifdef  __cplusplus
extern "C"
{
#endif

/**
 * Scaler to translate NMT request into NMT state
 * @param const NMT_raw_t * const praw  ==> INPUT: Pointer to the signal raw data object
 * @param const NMT_data_t * const pvalue ==> OUTPUT: Pointer to the signal application data object
 */
RC_t NMT_scaleR2D( const NMT_raw_t * const praw, NMT_data_t * const pvalue);

/**
 * Scaler to translate  NMT request into NMT state
 * @param const NMT_data_t const pvalue ==> INPUT: Pointer to the signal application data object
 * @param NMT_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t NMT_scaleD2R( const NMT_data_t * const pvalue, NMT_raw_t * const praw);

#ifdef  __cplusplus
}
#endif

#endif /* SWC_ENGINE_TYPE_H_ */

