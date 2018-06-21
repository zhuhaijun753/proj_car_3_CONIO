/*
 * swc_engine.cpp
 *
 *  Created on: 04.06.2016
 *      Author: Fromm
 */

#include "swc_engine.h"
#include "math.h"
#include "signal_bb.h"
#include "det.h"

#include "engine_signal.h"
#include "engine_type.h"

#include "enginestate_type.h"
#include "enginestate_signal.h"

#include "carstate_signal.h"
#include "carstate_type.h"

#include "carconfig.h"


/*************************************************************************************************************
 * Static local functions
 *************************************************************************************************************/



static bool ENGINE__setAllEngineNMTState(CANopen_NMT_commands_t targetNMT)
{


	RC_t writeStatus;

	ENGINESTATE_data_t engineStateWriteVal = ENGINESTATE_get(&so_out_engineState);
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	//command: NMT
	(engineStateWriteVal.engineFL_appState).command = CMD_GetSetNmt;
	(engineStateWriteVal.engineFR_appState).command = CMD_GetSetNmt;
	(engineStateWriteVal.engineRR_appState).command = CMD_GetSetNmt;
	(engineStateWriteVal.engineRL_appState).command = CMD_GetSetNmt;

	//set all engines NMT to the desired value
	(engineStateWriteVal.engineFL_appState).applicationNmtState = targetNMT;
	(engineStateWriteVal.engineFR_appState).applicationNmtState = targetNMT;
	(engineStateWriteVal.engineRR_appState).applicationNmtState = targetNMT;
	(engineStateWriteVal.engineRL_appState).applicationNmtState = targetNMT;

	//Write if CAN is available
	if (carState.state_normal != IS_NMT_BOOTING)
	{
		writeStatus = ENGINESTATE_write(&so_out_engineState, engineStateWriteVal);

		if (writeStatus == RC_SUCCESS)
		{
			return true;
		}
	}

	return false;
}


static bool ENGINE__setAllEngineDriveState(ENGINESTATE_DriveState_t targetDriveState)
{

	ENGINESTATE_data_t engineStateReadVal = ENGINESTATE_INIT_DATA;
	ENGINESTATE_data_t engineStateWriteVal = ENGINESTATE_get(&so_out_engineState);

	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	RC_t writeStatus;
	RC_t readStatus;

	//command: NMT
	(engineStateWriteVal.engineFL_appState).command = CMD_GetSetEngineState;
	(engineStateWriteVal.engineFR_appState).command = CMD_GetSetEngineState;
	(engineStateWriteVal.engineRR_appState).command = CMD_GetSetEngineState;
	(engineStateWriteVal.engineRL_appState).command = CMD_GetSetEngineState;

	//set all engines Drive Management to the desired value
	(engineStateWriteVal.engineFL_appState).applicationDriveState = targetDriveState;
	(engineStateWriteVal.engineFR_appState).applicationDriveState = targetDriveState;
	(engineStateWriteVal.engineRR_appState).applicationDriveState = targetDriveState;
	(engineStateWriteVal.engineRL_appState).applicationDriveState = targetDriveState;


	//Only execute this if drivers are initialised
	if (carState.state_normal != IS_NMT_BOOTING)
	{
		writeStatus = ENGINESTATE_write(&so_out_engineState, engineStateWriteVal);

		//if writeStatus != RC_Success
		//Retry option here

		//Set command to read engine state
		//@PF Todo: read NMT to be added
		engineStateReadVal.engineFL_appState.command = CMD_GetSetEngineState;
		engineStateReadVal.engineFR_appState.command = CMD_GetSetEngineState;
		engineStateReadVal.engineRL_appState.command = CMD_GetSetEngineState;
		engineStateReadVal.engineRR_appState.command = CMD_GetSetEngineState;

		//Set the commandword to the rawdata
		readStatus = ENGINESTATE_set(&so_in_engineState,engineStateReadVal);

		//Get the enginestate from the Motion Controller
		//wait for some time - currently blocking mode, so no problem
		readStatus = ENGINESTATE_read(&so_in_engineState,&engineStateReadVal);

		//Check if all states have been set accordingly
		if (	engineStateReadVal.engineFL_appState.applicationDriveState == targetDriveState &&
				engineStateReadVal.engineFR_appState.applicationDriveState == targetDriveState &&
				engineStateReadVal.engineRR_appState.applicationDriveState == targetDriveState &&
				engineStateReadVal.engineRL_appState.applicationDriveState == targetDriveState	)
		{
			asm("nop");
			return true;
		}

	}

	return false;
}

//Will chedck if the engine controllers are in a specific state
static bool ENGINE__checkAllEngineDriveState(ENGINESTATE_DriveState_t targetDriveState)
{
	ENGINESTATE_data_t engineStateReadVal = ENGINESTATE_INIT_DATA;

	RC_t readStatus;

	engineStateReadVal.engineFL_appState.command = CMD_GetSetEngineState;
	engineStateReadVal.engineFR_appState.command = CMD_GetSetEngineState;
	engineStateReadVal.engineRL_appState.command = CMD_GetSetEngineState;
	engineStateReadVal.engineRR_appState.command = CMD_GetSetEngineState;

	//Set the commandword to the rawdata
	readStatus = ENGINESTATE_set(&so_in_engineState,engineStateReadVal);

	//Get the enginestate from the Motion Controller
	//wait for some time - currently blocking mode, so no problem
	readStatus = ENGINESTATE_read(&so_in_engineState,&engineStateReadVal);

	if (	engineStateReadVal.engineFL_appState.applicationDriveState == targetDriveState &&
			engineStateReadVal.engineFR_appState.applicationDriveState == targetDriveState &&
			engineStateReadVal.engineRR_appState.applicationDriveState == targetDriveState &&
			engineStateReadVal.engineRL_appState.applicationDriveState == targetDriveState	)
	{
		asm("nop");
		return true;
	}

	return false;
}


/*************************************************************************************************************
 * Init Function Implementations
 *************************************************************************************************************/

void ENGINE_init()
{

//#ifndef CONFIG_SMARTPOWER
			//Engines NMT needs only to be initialzed if this is not already done by the smartpower board
	//Set network management to operational
	if (!ENGINE__setAllEngineNMTState(CANopen_NMT_CMD_START_NODE))
	{
		DET_stop(AUTOCORE,CAN_ERROR,0);
	}
//#endif


	//Iterate through the state machine to bring the system into Operation Enable State

	if (!ENGINE__setAllEngineDriveState(DriveState_ReadyToSwitchOn))
	{
		DET_stop(AUTOCORE,CAN_ERROR,1);
	}

	if (!ENGINE__setAllEngineDriveState(DriveState_SwitchedOn))
	{
		DET_stop(AUTOCORE,CAN_ERROR,2);
	}

	if (!ENGINE__setAllEngineDriveState(DriveState_OperationEnable))
	{
		DET_stop(AUTOCORE,CAN_ERROR,3);
	}

}


void ENGINE_deinit() {

}

/*************************************************************************************************************
 * Safety Function Implementations
 *************************************************************************************************************/


#define ENGINE_K_X 		ENGINE_PERCENT2RPM
#define ENGINE_K_Y 		ENGINE_PERCENT2RPM
#define ENGINE_K_PHI 	ENGINE_PERCENT2RPM
#define ENGINE_MAXTARGETSPEEDAGE 20	/* As the engine will be set directly through event, the age should be close to 0 */


RT_safety_t ENGINE_run_setSpeed() {

	RC_t engineSetSpeedStatus = RC_ERROR;
	RT_safety_t returnCode = SAFEOK;

	ENGINE_data_t engine_data = ENGINE_get(&so_out_engine);
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);


	float fr = 0;
	float fl = 0;
	float rr = 0;
	float rl = 0;


	//Targetspeed should always be updated:
	// - In any other state than running state ==> stop
	// - Invalid signal ==>  stop
	// - System error state is reflected through carState.speedLimit

	//Read currect speed if available and ok, in all other cases use initial 0 value
	if(!TARGETSPEED_is_status_invalid(&so_targetspeed)	&& carState.state_normal == IS_RUNNING )
	{

		//Only calculate engine speed in case everything is ok
		TARGETSPEED_data_t target_speed = TARGETSPEED_get(&so_targetspeed);

/*
		//Impact of x component
		fr = target_speed.vx * ENGINE_K_X;
		fl = target_speed.vx * ENGINE_K_X;
		rr = -target_speed.vx * ENGINE_K_X;
		rl = -target_speed.vx * ENGINE_K_X;

		//Impact of y component
		fr += -target_speed.vy * ENGINE_K_Y;
		fl += +target_speed.vy * ENGINE_K_Y;
		rr += -target_speed.vy * ENGINE_K_Y;
		rl += +target_speed.vy * ENGINE_K_Y;

		//Impact of PHI component
		fr += target_speed.vphi * ENGINE_K_PHI;
		fl += target_speed.vphi * ENGINE_K_PHI;
		rr += target_speed.vphi * ENGINE_K_PHI;
		rl += target_speed.vphi * ENGINE_K_PHI;

*/


		//Impact of x component
		fr = -target_speed.vx * ENGINE_K_X;
		fl = -target_speed.vx * ENGINE_K_X;
		rr = +target_speed.vx * ENGINE_K_X;
		rl = +target_speed.vx * ENGINE_K_X;

		//Impact of y component
		fr += +target_speed.vy * ENGINE_K_Y;
		fl += -target_speed.vy * ENGINE_K_Y;
		rr += +target_speed.vy * ENGINE_K_Y;
		rl += -target_speed.vy * ENGINE_K_Y;

		//Impact of PHI component
		fr += target_speed.vphi * ENGINE_K_PHI;
		fl += target_speed.vphi * ENGINE_K_PHI;
		rr += target_speed.vphi * ENGINE_K_PHI;
		rl += target_speed.vphi * ENGINE_K_PHI;

		//Scale to max speed limit
		float maxSpeedParamater = _abs(fr);
		if (_abs(fl) > maxSpeedParamater) maxSpeedParamater = _abs(fl);
		if (_abs(rr) > maxSpeedParamater) maxSpeedParamater = _abs(rr);
		if (_abs(rl) > maxSpeedParamater) maxSpeedParamater = _abs(rl);

		//speed limit == 100% ==> simple scaling of speed value to 120 rpm
		//speed limit == 50% ==> scaling of speed value and limitation to 50% = 60 rpm
		float speedLimit = (float)carState.speedLimit * ENGINE_PERCENT2RPM;
		if (maxSpeedParamater > speedLimit)
		{
			fr *= speedLimit / maxSpeedParamater;
			fl *= speedLimit / maxSpeedParamater;
			rl *= speedLimit / maxSpeedParamater;
			rr *= speedLimit / maxSpeedParamater;
		}

	}

	//The engine speed and Lidar speed information will be written in any case
	//If the engine speed has not been updated, it will be set to 0 (initial value of fl, fr....)

	engine_data.speed_frontLeft = (sint8_t)fl;
	engine_data.speed_frontRight = (sint8_t)fr;
	engine_data.speed_rearLeft = (sint8_t)rl;
	engine_data.speed_rearRight = (sint8_t)rr;

	//Set the signal and send the data to the CAN
	//Only, if the engine controllers are in the correct drive state
	//@PF Hack...

	engineSetSpeedStatus = ENGINE_write(&so_out_engine,engine_data);

	/* Possible overrun on CAN / ENgine controller if we request state directly before setting the speed?
	if (ENGINE__checkAllEngineDriveState(DriveState_OperationEnable))
	{
		asm("nop");
		engineSetSpeedStatus = ENGINE_write(&so_out_engine,engine_data);
	}
	else
	{
		asm("nop");
		engineSetSpeedStatus = RC_ERROR;
	}
	 */

	//Now let's calculate the returncode
	//Error on driver level, let's stop
	if (engineSetSpeedStatus != RC_SUCCESS)
	{
		asm("nop");
		//Todo: As we cannot control the engine, we have to pull the last line of defence and turn of the power
		returnCode = SAFEERROR2;
	}

	return returnCode;

}

/**
 * Emergency Stop of the car
 */
RT_safety_t ENGINE_run_emergencyStop()
{
	//For the time being....


/*
	if (!ENGINE__setAllEngineDriveState(DriveState_QuickStop))
	{
		DET_stop(AUTOCORE,CAN_ERROR,4);
	}

	if (!ENGINE__setAllEngineDriveState(DriveState_SwitchOnDisabled))
	{
		DET_stop(AUTOCORE,CAN_ERROR,5);
	}

	//Iterate through the state machine to bring the system into Operation Enable State

	if (!ENGINE__setAllEngineDriveState(DriveState_ReadyToSwitchOn))
	{
		DET_stop(AUTOCORE,CAN_ERROR,6);
	}

	if (!ENGINE__setAllEngineDriveState(DriveState_SwitchedOn))
	{
		DET_stop(AUTOCORE,CAN_ERROR,7);
	}

	if (!ENGINE__setAllEngineDriveState(DriveState_OperationEnable))
	{
		DET_stop(AUTOCORE,CAN_ERROR,8);
	}

*/
	//Turn off speed in addition
	ENGINE_run_stopCar();
	return SAFEOK;
}

/**
 * @brief Emergency Shut Off function
 * This function turns off the power in case of a critical DET_stop or other fatal error
 * @return void
 */
void ENGINE_run_powerOff()
{

	//Problem: This causes an infinite loop
	//Emergency stop call CAN
	//CAN returns error
	//Det call this function
	//Will be replaced with true power off function

#ifdef CONFIG_SMARTPOWER
	//Todo: Will be replaced with a real power off signal once the smart power board is available
	//ENGINE_run_emergencyStop();
#else
	//ENGINE_run_emergencyStop();
#endif

}


RT_safety_t ENGINE_run_stopCar()
{
	ENGINE_data_t engine_data = ENGINE_get(&so_out_engine);

	engine_data.speed_frontLeft = 0;
	engine_data.speed_frontRight = 0;
	engine_data.speed_rearLeft = 0;
	engine_data.speed_rearRight = 0;

	//TARGETSPEED_data_t target_data = TARGETSPEED_INIT_DATA;
	//TARGETSPEED_set(&so_targetspeed,target_data);

	//Only write if engine is in correct state
	if (ENGINE__checkAllEngineDriveState(DriveState_OperationEnable))
	{
		//Normal brake
		asm("nop");
		ENGINE_write(&so_out_engine, engine_data);
	}
	else
	{
		asm("nop");
		ENGINE_run_powerOff();
	}

	return SAFEOK;
}



#define SAFETY_TARGETSPEED_TIMEOUT_ERROR	3000		//3000ms = 3s
#define SAFETY_TARGETSPEED_TIMEOUT_WARNING	1000		//1000ms = 1s

/**
 * Checks if the target speed is updated in regular intervalls
 */
RT_safety_t ENGINE_run_checkTargetSpeedUpdate()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	//Increment targetspeed age with every increment
	//TARGETSPEED_inc_age(&so_targetspeed); //Not needed anymore due to timertick update

	if (carState.state_normal == IS_RUNNING)
	{
		if (TARGETSPEED_get_age(&so_targetspeed) >= SAFETY_TARGETSPEED_TIMEOUT_ERROR)
		{
			//Stop the car
			return SAFEERROR2;
		}
		else if (TARGETSPEED_get_age(&so_targetspeed) >= SAFETY_TARGETSPEED_TIMEOUT_WARNING)
		{
			//Slow the car down
			return SAFEERROR1;
		}

	}

	return SAFEOK;
}


/**
 * Set engine speed to 0 in case the car is not in the running state
 */
RT_safety_t ENGINE_run_setEngineSpeedCarState()
{
	CARSTATE_data_t carState = CARSTATE_get(&so_carstate);

	if (carState.state_normal == IS_NMT_BOOTING || carState.state_normal == IS_RUNNING || carState.state_normal == IS_DEVICES_BOOTING)
	{
		//No need to stop the car. In Preoperational (IS_NMT_BOOTING, IS_DEVICES_BOOTING) this will not work, in IS_RUNNING it is not intended
		return SAFEOK;
	}

	//To be on the safe side, let's stop the car
	ENGINE_run_stopCar();

	return SAFEERROR2;

}

