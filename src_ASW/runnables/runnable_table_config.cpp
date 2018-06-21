/*
 * runnabletableconfig.cpp
 *
 *  Created on: 03.06.2016
 *      Author: Fromm
 */

#include "runnable_table_config.h"
#include "runnable_table.h"

#include "SystemEvents.h"

#include "signal_bb.h"

#include "swc_remote.h"
#include "swc_dead_reackoning.h"
#include "swc_lidar.h"
#include "swc_control.h"
#include "swc_engine.h"
#include "swc_gfx_diagnostic.h"

#include "carconfig.h"



/******************************************************************************/
/*------------------------- Table definitions---------------------------------*/
/******************************************************************************/

/*========================== Core 0, Task Communication =============================*/


/*
 * Init Functions of all the application runnables in a table
 * Add all runnables which require the communication system to be initialised
 */
const RT_initTable_t RT_C0_communication_initRunTable[] = {
		{(RT_runnablePtr_t)(REMOTE_init)},
		{(RT_runnablePtr_t)(ENGINE_init)},
		{(RT_runnablePtr_t)(LIDAR_init_antiCollission)},

};

const uint16_t RT_C0_communication_initRunTable_size = sizeof(RT_C0_communication_initRunTable) / sizeof(RT_initTable_t);


/*
 * Event driven Runnables Table
 * Entry to the table shall be made as
 * runnable ptr,		eventMask
 */
const RT_eventSafetyTable_t RT_C0_communication_eventRunTable[] = {

		//runnable ptr,																    eventMask													name
		{(RT_runnableSafetyPtr_t)(REMOTE_run_readAndCheckProtocol),						ev_Task_C0_Communication_0_Uart_Remote_Rx_completeProtocol, "REMOTE_readProtocol"},
		{(RT_runnableSafetyPtr_t)(LIDAR_run_requestFullScan), 						    ev_Task_C0_Communication_4_Can_Rx_PDO,                      "LIDAR_reqFullScan"},
		//Add Lidar Full scan here

};

const uint16_t RT_C0_communication_eventRunTable_size = sizeof(RT_C0_communication_eventRunTable) / sizeof(RT_eventSafetyTable_t);




/*========================== Core 0, Task SafetyInput =============================*/

/*
 * Cyclic Runnables Table for all states
 * Entry to the table shall be made as
 * runnable ptr,		cyclic Time, 		relative Offset Time
 */
const RT_cyclicSafetyTable_t RT_C0_safetyInput_cyclicRunTable[] = {

		//runnable ptr,																cyclic Time, 		Offset		Name
		{(RT_runnableSafetyPtr_t)(REMOTE_run_checkJoystickAge), 					100, 				0,			"REMOTE_checkJoyAge"},
		{(RT_runnableSafetyPtr_t)(REMOTE_run_checkJoystickData), 					100, 				0,			"REMOTE_checkJoyData"},
		{(RT_runnableSafetyPtr_t)(LIDAR_run_checkCollissionSensor), 	 			100, 				0,			"LIDAR_checkColl"},


};

const uint16_t RT_C0_safetyInput_cyclicRunTable_size = sizeof(RT_C0_safetyInput_cyclicRunTable) / sizeof(RT_cyclicSafetyTable_t);

/*
 * Cyclic Runnables Table for prerun state
 * Entry to the table shall be made as
 * runnable ptr,		cyclic Time, 		relative Offset Time
 */
const RT_cyclicSafetyTable_t RT_C0_safetyInput_cyclicPreRunTable[] = {

		//runnable ptr,																cyclic Time, 		Offset		Name
		{(RT_runnableSafetyPtr_t)(REMOTE_run_checkJoystickZero), 					100, 				0,			"REMOTE_checkJoyZero"},


};

const uint16_t RT_C0_safetyInput_cyclicPreRunTable_size = sizeof(RT_C0_safetyInput_cyclicPreRunTable) / sizeof(RT_cyclicSafetyTable_t);




/*========================== Core 0, Task SafetyLogic =============================*/

/*
 * Init Functions of all the application runnables in a table
 */
const RT_initTable_t RT_C0_safetyLogic_initRunTable[] = {

		{(RT_runnablePtr_t)(DR_init)},

};

const uint16_t RT_C0_safetyLogic_initRunTable_size = sizeof(RT_C0_safetyLogic_initRunTable) / sizeof(RT_initTable_t);



/*
 * Cyclic Runnables Table
 * Entry to the table shall be made as
 * runnable ptr,		cyclic Time, 		relative Offset Time
 */
const RT_eventSafetyTable_t RT_C0_safetyLogic_eventRunTable[] = {

		//runnable ptr,																	eventMask											Name
		{(RT_runnableSafetyPtr_t)(ENGINE_run_checkTargetSpeedUpdate), 					ev_Task_C0_Safety_Logic_0_100ms,					"ENG_checkTargetUpd"},
		{(RT_runnableSafetyPtr_t)(ENGINE_run_setEngineSpeedCarState), 					ev_Task_C0_Safety_Logic_0_100ms,					"ENG_setSpdCarState"},
		{(RT_runnableSafetyPtr_t)(DR_run_getDecoder),					 				ev_Task_C0_Safety_Logic_0_100ms,					"DR_getDecoder"},		//can be moved to output check later
		{(RT_runnableSafetyPtr_t)(ENGINE_run_emergencyStop), 							ev_Task_C0_Safety_Logic_2_requestEmergencyStop,		"ENG_emergencyStop"},
		{(RT_runnableSafetyPtr_t)(ENGINE_run_stopCar), 									ev_Task_C0_Safety_Logic_3_requestStop,				"ENG_stopCar"},			//not really required, but an extra safety line in case the setSpeed is not called
		{(RT_runnableSafetyPtr_t)(LIDAR_run_setLidarTargetspeed),						ev_Task_C0_Safety_Logic_1_newTargetSpeed,			"LIDAR_setTargetSpd"},
		{(RT_runnableSafetyPtr_t)(ENGINE_run_setSpeed), 								ev_Task_C0_Safety_Logic_1_newTargetSpeed,			"ENG_setSpeed"}		//This runnable has to be the last one!

};


const uint16_t RT_C0_safetyLogic_eventRunTable_size = sizeof(RT_C0_safetyLogic_eventRunTable) / sizeof(RT_eventSafetyTable_t);







/*========================== Core 1, Task Control =============================*/

/*
 * Init Functions of all the application runnables in a table
 */
const RT_initTable_t RT_C1_control_initRunTable[] = {
		{(RT_runnablePtr_t)(LIDAR_init_fullScan)}

};

const uint16_t RT_C1_control_initRunTable_size = sizeof(RT_C1_control_initRunTable) / sizeof(RT_initTable_t);


/*
 * Cyclic Runnables Table
 * Entry to the table shall be made as
 * runnable ptr,		cyclic Time, 		relative offset time
 */
const RT_cyclicTable_t RT_C1_control_cyclicRunTable[] = {

		//runnable ptr,											cyclic Time, 		Offset
		{(RT_runnablePtr_t)(CONTROL_run_selectedFeature), 		100, 				0},
		//{(RT_runnablePtr_t)(DR_run_calculatePosition), 			100, 				0},	//Will be called by message onUpdate
		{(RT_runnablePtr_t)(DR_run_reportPosition),		 		1000, 				0},

		//{(RT_runnablePtr_t)(DR_run_getDecoder_V2), 				100, 				0},			//================================= Just for testing purposes

};

const uint16_t RT_C1_control_cyclicRunTable_size = sizeof(RT_C1_control_cyclicRunTable) / sizeof(RT_cyclicTable_t);

/*
 * Event driven Runnables Table
 * Entry to the table shall be made as
 * runnable ptr,		eventMask
 */
const RT_eventTable_t RT_C1_control_eventRunTable[] = {

		//runnable ptr,																    eventMask
		//{(RT_runnablePtr_t)(SLAMALGORITHM_run_extractCorners),				ev_Task_C1_Control_1_lidarScanReady},


};

const uint16_t RT_C1_control_eventRunTable_size = sizeof(RT_C1_control_eventRunTable) / sizeof(RT_eventTable_t);

/*========================== Core 2, Task TFT =============================*/
/*
 * Init Functions of all the application runnables in a table
 * Add all runnables which require the communication system to be initialised
 */
const RT_initTable_t RT_C2_tft_initRunTable[] = {
		{(RT_runnablePtr_t)(GFXDIAGNOSTIC_display_init)},
		{(RT_runnablePtr_t)(GFXDIAGNOSTIC_touch_init)},

};

const uint16_t RT_C2_tft_initRunTable_size = sizeof(RT_C2_tft_initRunTable) / sizeof(RT_initTable_t);

/*
 * Cyclic Runnables Table
 * Entry to the table shall be made as
 * runnable ptr,		cyclic Time, 		relative offset time
 */
const RT_cyclicTable_t RT_C2_tft_cyclicRunTable[] = {

		//runnable ptr,											cyclic Time, 		Offset
		{(RT_runnablePtr_t)(GFXDIAGNOSTIC_display_run), 		100, 				0},
		{(RT_runnablePtr_t)(GFXDIAGNOSTIC_touch_run), 			100, 				0},

};

const uint16_t RT_C2_tft_cyclicRunTable_size = sizeof(RT_C2_tft_cyclicRunTable) / sizeof(RT_cyclicTable_t);

/*
 * Event driven Runnables Table
 * Entry to the table shall be made as
 * runnable ptr,		eventMask
 */
const RT_eventTable_t RT_C2_tft_eventRunTable[] = {

		//runnable ptr,																    eventMask
		{(RT_runnablePtr_t)(GFXDIAGNOSTIC_display_run), 						ev_Task_C2_TFT_1_100ms},
		{(RT_runnablePtr_t)(GFXDIAGNOSTIC_touch_run),							ev_Task_C2_TFT_1_100ms},
		{(RT_runnablePtr_t)(GFXDIAGNOSTIC_display_onTouch),						ev_Task_C2_TFT_0_Touch},
		//Add Lidar Full scan here

};

const uint16_t RT_C2_tft_eventRunTable_size = sizeof(RT_C2_tft_eventRunTable) / sizeof(RT_eventTable_t);
