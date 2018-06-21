/**
* \file swc_engine.h
* \author Fromm
* \date 04.06.2016
*
* \brief Driver for the Faulhaber motion controller
*
*
* \todo
*
* \warning
*
*  Changelog:\n
*  - 1.1; 10.5.17; Fromm
*            - Rearranged functions
*
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

#ifndef SWC_ENGINE_H_
#define SWC_ENGINE_H_


#include "runnable_table_type.h"


/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

/**
 * @brief Init function for engines
 * This function sets the NMT and operational mode of the engines to driving mode
 * @return void
 */
void ENGINE_init();

/**
 * @brief De-Init function for engines
 * Currently empty
 * @return void
 */
void ENGINE_deinit();


/*****************************************************************************************
 * Safety Function Implementations - Engine Control
 *****************************************************************************************/

/**
 * @brief Normal driving function for engine
 * This function translates the x/y/phi values of the target speed signal into motion commands of the individual engines and transmits the data through CanOpen
 * @return successful calculation and transmission of the data
 */
RT_safety_t ENGINE_run_setSpeed();

/**
 * @brief Emergency Stop function
 * This function stops the engines using a brute force NMT command
 * @return successful calculation and transmission of the data
 */
RT_safety_t ENGINE_run_emergencyStop();


/**
 * @brief Normal Stop function
 * This function stops the engines setting speed to 0. Will be called by the safety runnables
 * @return successful calculation and transmission of the data
 */
RT_safety_t ENGINE_run_stopCar();


/**
 * @brief Emergency Shut Off function
 * This function turns off the power in case of a critical DET_stop or other fatal error
 * @return void
 */
void ENGINE_run_powerOff();


/*****************************************************************************************
 * Safety Function Implementations - Engine Signal
 *****************************************************************************************/

/**
 * @brief Safety function to verify the targetspeed values for the engine
 * This function checks if the target speed values are in a valid range
 * @return requested safety state
 */
RT_safety_t ENGINE_run_checkEngineTargetSpeed();


/**
 * @brief Checks if the target speed is updated in regular intervalls
 * Checks if the target speed is updated in regular intervalls
 * @return requested safety state
 */
RT_safety_t ENGINE_run_checkTargetSpeedUpdate();


/**
 * @brief Set engine speed to 0 in case the car is not in the running state
 * Set engine speed to 0 in case the car is not in the running state
 * @return requested safety state
 */
RT_safety_t ENGINE_run_setEngineSpeedCarState();


#endif /* SWC_ENGINE_H_ */
