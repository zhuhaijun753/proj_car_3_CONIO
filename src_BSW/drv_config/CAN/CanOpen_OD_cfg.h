/*
 * CanOpen.h
 *
 *  Created on: 01.05.2016
 *      Author: Fromm
 */

#ifndef CANOPEN_H_
#define CANOPEN_H_

#include "global.h"
#include "signal_global.h"

#include "engine_type.h"
#include "lidarscan_type.h"
#include "enginedecoder_type.h"
#include "enginestate_type.h"
#include "lidarconfig_type.h"
#include "targetspeed_type.h"
#include "nmt_type.h"

#include "CANopen_Client.h"
#ifdef  __cplusplus
extern "C"
{
#endif


/**********************************************************************************************************************************************
/** Node Id's,....
 **********************************************************************************************************************************************/



/*
#define NODE_ID_FL 					1
#define NODE_ID_FR 					2
#define NODE_ID_RR 					3
#define NODE_ID_RL 					4
*/


//Modified identifiers due to car kinematics
//Node ID for Aurix is set in CANopen_config.h
#define NODE_ID_FL 					3				/**< \brief Node ID for front left engine */
#define NODE_ID_FR 					4				/**< \brief Node ID for front right engine */
#define NODE_ID_RR 					1				/**< \brief Node ID for rear right engine */
#define NODE_ID_RL 					2				/**< \brief Node ID for rear left engine */
#define NODE_ID_LIDAR 				5				/**< \brief Node ID for Lidar Board */
#define NODE_ID_SMARTPOWER 			7				/**< \brief Node ID for SmartPower Board */


//Engine
#define INDEX_ENGINE_SETSPEED		0x60FF
#define INDEX_ENGINE_GETDECODER		0x6064
#define INDEX_ENGINE_GETSTATE		0x6041
#define INDEX_ENGINE_SETSTATE		0x6040

//Lidar

#define INDEX_LIDAR_SETCONFIG 		0x133D						 /**< \brief CANopen OD Index for setting lidar configuration (rotation speed,... ) */
#define INDEX_LIDAR_SETTARGETSPEED 	0x133C					     /**< \brief CANopen OD Index for transfering current target speed and direction (required for anticollission) */
#define INDEX_LIDAR_GETFULLSCAN		0x133B						 /**< \brief CANopen OD Index for getting lidar fullscan */

//Smart Power
#define INDEX_SMARTPOWER_POWSTATE        0x1100                  /**< \brief CANopen OD Index for converter enable PIN*/
#define INDEX_SMARTPOWER_CW_IDX          0x1101                  /**< \brief CANopen OD Index for converter warnings*/
#define INDEX_SMARTPOWER_CF_IDX          0x1102                  /**< \brief CANopen OD Index for converter faults*/
#define INDEX_SMARTPOWER_CV_IDX          0x1103                  /**< \brief CANopen OD Index for the converter voltage, converter addressing via s-index*/
#define INDEX_SMARTPOWER_CC_IDX          0x1104                  /**< \brief CANopen OD Index for the converter current, converter addressing via s-index*/
#define INDEX_SMARTPOWER_SC_IDX          0x1105                  /**< \brief CANopen OD Index for the converter state, converter addressing via s-index*/


#define INDEX_SMARTPOWER_OVWT_IDX        0x1110                  /**< \brief CANopen OD Index for converter OverVoltage warning threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_UVWT_IDX        0x1111                  /**< \brief CANopen OD Index for converter UnderVoltage warning threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_OCWT_IDX        0x1112                  /**< \brief CANopen OD Index for converter OverCurrent warning threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_OVFT_IDX        0x1113                  /**< \brief CANopen OD Index for converter OverVoltage fault threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_UVFT_IDX        0x1114                  /**< \brief CANopen OD Index for converter UnderVoltage fault threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_OCFT_IDX        0x1115                  /**< \brief CANopen OD Index for converter OverCurrent fault threshold, converter addressing via s-index*/

#define INDEX_SMARTPOWER_NODE_IDX        0x1120                  /**< \brief CANopen OD Index for Node-ID*/

#define INDEX_SMARTPOWER_EMSWD_IDX       0x1200                  /**< \brief CANopen OD Index for Watchdog EMS control. Requires password*/
#define INDEX_SMARTPOWER_EMS2_IDX        0x1201                  /**< \brief CANopen OD Index for EMS 2 control. Requires password*/
#define INDEX_SMARTPOWER_EMS2T_IDX       0x1202                  /**< \brief CANopen OD Index for EMS 2 reboot delay*/

#define INDEX_SMARTPOWER_PMCMD_IDX       0x1300                  /**< \brief CANopen OD Index for Power Management Commands*/

#define INDEX_SMARTPOWER_NMTSHM_IDX      0x1400                  /**< \brief CANopen OD Index for System Health monitoring NMT state, Subindex NodeId, State is defined as enum in CANOpen.h */


//Timeout for blocking mode
#define TIMEOUT_LIDARSCANSDO 		2000
#define TIMEOUT_ENGINESDO 			50
#define TIMEOUT_LIDARSDO 			1000
#define TIMEOUT_SMARTPOWER_SDO		50


/**********************************************************************************
/** Generic Functions
 **********************************************************************************/

RC_t CANOPEN_process_PDO(CANopen_CAN_MSG_t* CAN_in);

/**********************************************************************************
/** Engine (Faulhaber) Functions
 **********************************************************************************/
extern CODD_ENGINE_t CODD_ENGINE_data; //required for rawdata signal initialisation
//extern CODD_ENGINEDECODER_t CODD_ENGINEDECODER_currentPosition_data;

RC_t CANOPEN_write_engineSpeedToFaulhaber( const ENGINE_raw_t * const canbuffer, const SIG_IDX_TYPE index);

RC_t CANOPEN_read_engineStateFromFaulhaber( ENGINESTATE_raw_t * const canbuffer, const SIG_IDX_TYPE index);
RC_t CANOPEN_write_engineStateToFaulhaber( const ENGINESTATE_raw_t * const canbuffer, const SIG_IDX_TYPE index);

RC_t CANOPEN_read_DecoderFromFaulhaber( ENGINEDECODER_raw_t * const  canbuffer, const SIG_IDX_TYPE index);


/**********************************************************************************
/** Lidar Functions
 **********************************************************************************/
extern uint32_t CODD_LIDAR_targetSpeed;		//Requested driving speed, required for rawdata signal initialisation
extern uint32_t CODD_LIDARCONFIG_data;		//Current configuration, required for rawdata signal initialisation

extern CODD_LIDARSCAN_t CODD_LIDARSCAN_data[LIDARCONFIG_MAX_RESOLUTION];

RC_t CANOPEN_write_ConfigToLidar( const LIDARCONFIG_raw_t * const canbuffer, const SIG_IDX_TYPE index);
RC_t CANOPEN_read_ConfigFromLidar( LIDARCONFIG_raw_t * const canbuffer, const SIG_IDX_TYPE index);


RC_t CANOPEN_write_targetSpeedToLidar( const TARGETSPEED_raw_t * const canbuffer, const SIG_IDX_TYPE index);

RC_t CANOPEN_read_ScanFromLidar( LIDARSCAN_raw_t * const canbuffer, const SIG_IDX_TYPE index);

/**********************************************************************************
/** SmartPower Functions
 **********************************************************************************/
extern uint32_t CODD_SMARTPOWER_powerState;

/**
 * Request the power domain state from SmartPower Board
 */
RC_t CANOPEN_read_PowerStateFromSmartPower(uint32_t * const canbuffer, const SIG_IDX_TYPE index);

/**
 * Request the NMT state from SmartPower Board
 */
RC_t CANOPEN_read_NMTfromSmartpower(NMT_raw_t * const canbuffer, const SIG_IDX_TYPE index);



#ifdef  __cplusplus
}
#endif

#endif /* CANOPEN_H_ */

