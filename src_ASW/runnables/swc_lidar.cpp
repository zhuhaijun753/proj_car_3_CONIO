/*
 * swclidar.cpp
 *
 *  Created on: 30.05.2016
 *      Author: Fromm
 */

#include "swc_lidar.h"
#include "signal_bb.h"
#include "lidarscan_signal.h"
#include "lidarscan_type.h"
#include "lidarfeature_signal.h"
#include "lidarfeature_type.h"
#include "lidarconfig_signal.h"
#include "lidarconfig_type.h"




/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

void LIDAR_init_antiCollission()
{
	LIDARCONFIG_data_t lidarconfig;

	lidarconfig.speed = 60;
	lidarconfig.collissionAngle = 20;
	lidarconfig.resolution = 360;

	LIDARCONFIG_write(&so_inout_lidarConfig, lidarconfig);
}

void LIDAR_init_fullScan()
{

}

void LIDAR_deinit()
{

}

/*****************************************************************************************
 * Safety Function Implementations
 *****************************************************************************************/

//Values for the LIDAR colission signal
typedef enum
{
	ANTICOLLISSION_LIDAR_INACTIVE,
	ANTICOLLISSION_LIDAR_NO_OBSTACLE,
	ANTICOLLISSION_LIDAR_CLOSE_OBSTACLE,
	ANTICOLLISSION_LIDAR_COLLISSION,
	ANTICOLLISSION_LIDAR_ERROR
} ANTICOLLISSION__uartProtocol_t;

#define LIDAR_SLOWDOWN_AGE 	500
#define LIDAR_STOP_AGE 		1000

/**
 * @brief Safety function which will read in the collission sensor data
 * This function checks for a possible collission and limits the car speed accordingly
 * @return requested safety state
 */
RT_safety_t LIDAR_run_checkCollissionSensor()
{

	uint8_t data = 0;
	ANTICOLLISSION__uartProtocol_t collissionData = ANTICOLLISSION_LIDAR_INACTIVE;

	//Get the signalvalue
	sig_sts_t signalStatus = UINT8_get_with_status(&so_in_lidarCollision, &data);

	collissionData = (ANTICOLLISSION__uartProtocol_t) data;

	//Check how old it is
	UINT8_time_t signalAge = UINT8_get_age(&so_in_lidarCollision);

	if (signalStatus != SIG_STS_VALID)
		return SAFEERROR2;

	if (signalAge > LIDAR_STOP_AGE)
		return SAFEERROR2;
	if (signalAge > LIDAR_SLOWDOWN_AGE)
		return SAFEERROR1;

	//Increment the age, This value will be set to 0 once a valid signal is received through the UART
	//UINT8_inc_age(&so_in_lidarCollision); //this is done centrally through timertick function

	if (UINT8_get_age(&so_in_lidarCollision) < LIDAR_SLOWDOWN_AGE)
	{
		//Now check the protocol content
		if (collissionData == ANTICOLLISSION_LIDAR_NO_OBSTACLE)
			return SAFEOK;

		if (collissionData == ANTICOLLISSION_LIDAR_CLOSE_OBSTACLE)
			return SAFEERROR1;
		if (collissionData == ANTICOLLISSION_LIDAR_COLLISSION)
			return SAFEERROR2;

		if (collissionData == ANTICOLLISSION_LIDAR_ERROR)
			return SAFEERROR1;
		if (collissionData == ANTICOLLISSION_LIDAR_INACTIVE)
			return SAFEERROR1;

	}
	else if (UINT8_get_age(&so_in_lidarCollision) < LIDAR_STOP_AGE)
	{
		return SAFEERROR1;
	}

	//Stop
	return SAFEERROR2;
}

/**
 * Send the requested targetspeed to the LIDAR sensor

 */
RT_safety_t LIDAR_run_setLidarTargetspeed()
{
	//Get the tragetspeed from the application
	TARGETSPEED_data_t targetSpeed = TARGETSPEED_get(&so_targetspeed);

	// Write it through the safety signal
	RC_t result = TARGETSPEED_write(&so_targetspeedToLidar, targetSpeed);

	if (result == RC_SUCCESS)
	{
		return SAFEOK;
	}
	else
	{
		return SAFEERROR1;
	}
}

/*****************************************************************************************
 * Non-Safety Function Implementations
 *****************************************************************************************/

RT_safety_t LIDAR_run_requestFullScan()
{
	RC_t result;

	//Let's request some updated scan data
	result = LIDARSCAN_refresh(&so_in_lidarscan);

	/**
		LIDARSCAN_refresh shall request the LIDAR for a full-scan in a 
		non-blocking mode. The further signal flow is driven when the 
		asynchronous data transfer is complete.
	**/
	
	return SAFEOK;
}



