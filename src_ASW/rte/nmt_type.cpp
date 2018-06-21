/*
 * NMT_type.cpp
 *
 *  Created on: 01.05.2016
 *      Author: P. Fromm
 */



#include "NMT_type.h"

extern "C"
{

RC_t NMT_scaleR2D( const NMT_raw_t * const praw, NMT_data_t * const pvalue)
{

	pvalue->NMT_lidar = (CANopen_NMT_states_t)praw->dd_lidar;
	pvalue->NMT_engineFL = (CANopen_NMT_states_t)praw->dd_engineFL;
	pvalue->NMT_engineFR = (CANopen_NMT_states_t)praw->dd_engineFR;
	pvalue->NMT_engineRL = (CANopen_NMT_states_t)praw->dd_engineRL;
	pvalue->NMT_engineRR = (CANopen_NMT_states_t)praw->dd_engineRR;
	return RC_SUCCESS;
}

RC_t NMT_scaleD2R( const NMT_data_t * const pvalue, NMT_raw_t * const praw)
{
	//Not needed
	return RC_ERROR;
}

} //extern  "C"
