/*
 * LIDARSCAN_type.h
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated by H-DA RTE Generator
 */



#ifndef LIDARSCAN_TYPE_H_
#define LIDARSCAN_TYPE_H_

#include "global.h"
#include "lidarconfig_type.h"

/*****************************************************************************************
 *************** Type Definition for LIDARSCAN  signal class **************************
 *****************************************************************************************/

typedef struct
{
	uint16_t		r;		// In cm, resolution is 2.5cm
	uint16_t	theta;		// In degree
}LIDARSCAN_point_t;

typedef struct
{
	LIDARSCAN_point_t point[LIDARCONFIG_CUR_RESOLUTION];
}LIDARSCAN_data_t;

typedef uint8_t CODD_LIDARSCAN_t;

typedef CODD_LIDARSCAN_t* LIDARSCAN_raw_t;	//pointer to the structure of raw data of the signal

#define LIDARSCAN_INIT_DATA ((LIDARSCAN_data_t)(LIDARSCAN_data_t{{0}}))
#define LIDARSCAN_INIT_RAW  ((LIDARSCAN_raw_t)(&CODD_LIDARSCAN_data[0]))

/*****************************************************************************************
 *************** Type specific scaler functions ******************************************
 *****************************************************************************************/
#ifdef  __cplusplus
extern "C"
{
#endif

/**
 * Scaler to translate bytestream protocol into ENGINE application data type
 * @param const LIDARSCAN_raw_t * const praw  ==> INPUT: Pointer to the signal raw data object
 * @param const LIDARSCAN_data_t * const pvalue ==> OUTPUT: Pointer to the signal application data object
 */
RC_t LIDARSCAN_scaleR2D( const LIDARSCAN_raw_t * const praw, LIDARSCAN_data_t * const pvalue);

/**
 * Scaler to translate engine control speed into CanOpen protocol
 * @param const LIDARSCAN_data_t const pvalue ==> INPUT: Pointer to the signal application data object
 * @param LIDARSCAN_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t LIDARSCAN_scaleD2R( const LIDARSCAN_data_t * const pvalue, LIDARSCAN_raw_t * const praw);

#ifdef  __cplusplus
}
#endif

#endif /* SWC_ENGINE_TYPE_H_ */

