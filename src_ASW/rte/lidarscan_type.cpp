/*
 * LIDARSCAN_type.cpp
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated
 */



#include "LIDARSCAN_type.h"
#include "lidarconfig_type.h"
//#include <cmath>

extern "C"
{

RC_t LIDARSCAN_scaleR2D( const LIDARSCAN_raw_t * const praw, LIDARSCAN_data_t * const pvalue)
{
	LIDARSCAN_point_t *points = &(pvalue->point[0]);

	// converting points to polar co-ordinates
	for (uint16_t i = 0; i < LIDARCONFIG_CUR_RESOLUTION; i++)
	{
		points[i].r = (*praw)[i] * 2.5; //Distance in cm
		points[i].theta = (uint16_t)(LIDARCONFIG_ANGLE_PER_SCANENTRY * i);
	}

	return RC_SUCCESS;
}

RC_t LIDARSCAN_scaleD2R( const LIDARSCAN_data_t * const pvalue, LIDARSCAN_raw_t * const praw)
{

	return RC_ERROR;
}

} //extern  "C"
