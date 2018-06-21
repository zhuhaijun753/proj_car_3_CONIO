/*
 * rte_time_type.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Shriki
 */

#ifndef SRC_BSW_DRV_CONFIG_SRV_RTE_TIME_TYPE_H_
#define SRC_BSW_DRV_CONFIG_SRV_RTE_TIME_TYPE_H_

#include "runnable_table_type.h"
#include "pxdef.h"

/*
 * For Runnables Time measurements (Entry and Exit timings)
 */
typedef struct
{
	RT_runnablePtr_t runnableAddr;
	uint32_t time;
	uint16_t taskId;
	uint8_t cpuCoreId;
	boolean_t  taskDataFlag;
	boolean_t  startTimeFlag;
	boolean_t endTimeFlag;
	uint16_t dummy_pack;
}RTE_TIME_timings_t;


typedef struct
{
	PxTask_t taskID;
	char taskName[20];
}RTE_TIME_initData_t;


//typedef struct
//{
//	uint32_t taskStartTime;
//	uint32_t taskEndTime;
//	uint16_t taskId;
//	uint8_t  cpuCoreId;
//	boolean_t  taskStartFlag;
//	boolean_t  taskEndFlag;
//}RTE_TIME_taskInfo_t;

#endif /* SRC_BSW_DRV_CONFIG_SRV_RTE_TIME_TYPE_H_ */
