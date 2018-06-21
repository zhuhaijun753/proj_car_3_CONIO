/*
 * runnable_table_type.h
 *
 *  Created on: 04.06.2016
 *      Author: Fromm
 */

#ifndef RUNNABLE_TABLE_TYPE_H_
#define RUNNABLE_TABLE_TYPE_H_

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/*------------------------- Type Definitions ---------------------------------*/
/******************************************************************************/

//Must be aligned with size in ui_view_1_cfg.
#define RT_TFTMAXNAMESIZE 20	//Max number of char's for string
								//Todo: must be verified

typedef enum {SAFEOK, SAFEERROR1, SAFEERROR2, SAFENOCHECK} RT_safety_t;


/**
 * function pointer used in cyclic runnable table
 */
typedef void (*RT_runnablePtr_t)();

/**
 * function pointer used in cyclic runnable table
 */
typedef RT_safety_t (*RT_runnableSafetyPtr_t)();


/**
 * Table for the cyclic runnables
 */
typedef struct
{
	RT_runnablePtr_t	runnable;
	uint16_t 			cyclicTime;
	uint16_t 			offsetTime;

}RT_cyclicTable_t;

/**
 * Table for the event runnables
 */
typedef struct
{
	RT_runnablePtr_t	runnable;
	uint32_t 			evMask;

}RT_eventTable_t;

/**
 * Table for the cyclic safety runnables
 */
typedef struct
{
	RT_runnableSafetyPtr_t	runnable;
	uint16_t 				cyclicTime;
	uint16_t 				offsetTime;
	char					name[RT_TFTMAXNAMESIZE];

}RT_cyclicSafetyTable_t;

/**
 * Table for the event driven safety runnables
 */
typedef struct
{
	RT_runnableSafetyPtr_t	runnable;
	uint32_t 				evMask;
	char					name[RT_TFTMAXNAMESIZE];

}RT_eventSafetyTable_t;

/**
 * Table for the init runnables
 */

typedef struct
{
	RT_runnablePtr_t	initRunnable;
}RT_initTable_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RUNNABLE_TABLE_TYPE_H_ */
