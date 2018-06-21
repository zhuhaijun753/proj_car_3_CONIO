/**
* \file swc_lidar.h
* \author Fromm
* \date 04.06.2016
*
* \brief Drivers for the LIDAR sensor board
*
* Software component which is processing the incoming remote data command.
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

#ifndef SWCLIDAR_H_
#define SWCLIDAR_H_

#include "runnable_table_type.h"

/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

/**
 * @brief Initfunction which will initialise the anti collission function of the LIDAR
 * This function will initialize the anticollssion functionality of the Lidar sensor
 * This includes the complete hardware initialisation, which may not be changed by the normal application code
 * @return void
 */
void LIDAR_init_antiCollission();

/**
 * @brief Initfunction which will initialise the fullscan function of the LIDAR
 * Initfunction which will initialise the fullscan function of the LIDAR
 * @return void
 */
void LIDAR_init_fullScan();

/**
 * @brief De-Initfunction for the LIDAR
 * Currently empty
 * @return void
 */
void LIDAR_deinit();

/*****************************************************************************************
 * Safety Function Implementations
 *****************************************************************************************/

/**
 * @brief Safety function which will read in the collission sensor data based on the previously transmitted target speed
 * This function checks for a possible collission and limits the car speed accordingly
 * @return requested safety state
 */
RT_safety_t LIDAR_run_checkCollissionSensor();


/**
 * @brief Send the requested targetspeed to the LIDAR sensor
 * Send the requested targetspeed to the LIDAR sensor
 * @return requested safety state
 */
RT_safety_t LIDAR_run_setLidarTargetspeed();

/**
 * @brief Request a full scan from the LIDAR
 * Request a full scan from the LIDAR
 * @return requested safety state
 */
RT_safety_t LIDAR_run_requestFullScan();

/*****************************************************************************************
 * Non-Safety Function Implementations
 *****************************************************************************************/

/**
 * @brief Request a full scan from the LIDAR
 * Request a full scan from the LIDAR
 * @return requested safety state
 */
void LIDAR_run_checkFullScan();



#endif /* SWCLIDAR_H_ */
