/*
 * runnabletableconfig.h
 *
 *  Created on: 03.06.2016
 *      Author: Fromm
 */

#ifndef RUNNABLETABLECONFIG_H_
#define RUNNABLETABLECONFIG_H_

#include "global.h"
#include "runnable_table.h"


/*========================== Core 0, Task Communication =============================*/
extern const RT_eventSafetyTable_t RT_C0_communication_eventRunTable[];
extern const uint16_t RT_C0_communication_eventRunTable_size;


extern const RT_initTable_t RT_C0_communication_initRunTable[];
extern const uint16_t RT_C0_communication_initRunTable_size;

/*========================== Core 0, Task SafetyInput =============================*/

// calls all state independant runnables
extern const RT_cyclicSafetyTable_t RT_C0_safetyInput_cyclicRunTable[];
extern const uint16_t RT_C0_safetyInput_cyclicRunTable_size;

//calls all runnables for prerun state
extern const RT_cyclicSafetyTable_t RT_C0_safetyInput_cyclicPreRunTable[];
extern const uint16_t RT_C0_safetyInput_cyclicPreRunTable_size;


/*========================== Core 0, Task SafetyLogic =============================*/

extern const RT_initTable_t RT_C0_safetyLogic_initRunTable[];
extern const uint16_t RT_C0_safetyLogic_initRunTable_size;

extern const RT_eventSafetyTable_t RT_C0_safetyLogic_eventRunTable[];
extern const uint16_t RT_C0_safetyLogic_eventRunTable_size;

/*========================== Core 1, Task Control =============================*/

extern const RT_initTable_t RT_C1_control_initRunTable[];
extern const uint16_t RT_C1_control_initRunTable_size;

extern const RT_cyclicTable_t RT_C1_control_cyclicRunTable[];
extern const uint16_t RT_C1_control_cyclicRunTable_size;

extern const RT_eventTable_t RT_C1_control_eventRunTable[];
extern const uint16_t RT_C1_control_eventRunTable_size;

/*========================== Core 2, Task TFT =============================*/

extern const RT_initTable_t RT_C2_tft_initRunTable[];
extern const uint16_t RT_C2_tft_initRunTable_size;

extern const RT_eventTable_t RT_C2_tft_eventRunTable[];
extern const uint16_t RT_C2_tft_eventRunTable_size;

#endif /* RUNNABLETABLECONFIG_H_ */
