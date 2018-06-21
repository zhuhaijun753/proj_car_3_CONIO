/**
 * \file 	msg_config.h
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	06.12.2017
 *
 * \brief DEFAULT HEADER TEMPLATE
 *
 * <detailed description what the file (header and/or source it belongs to) does>
 *
 * \note <notes>
 * \todo <todos>
 * \warning <warnings, e.g. dependencies, order of execution etc.>
 *
 *  Changelog:\n
 *  - <version; data of change; author>
 *            - <description of the change>
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
#ifndef MSG_CONFIG_H_
#define MSG_CONFIG_H_

#include "msg_type.h"

/**************************************************************************************
 * Type Definiotion
 **************************************************************************************/


/**
 * Identifiers for the dispatcher tables
 */
typedef enum {
	PORT_NONE,					/** < Empty port */



	//Signal services
	PORT_ONUPDATEDATA,			/** < Default Sender port for signals - trial */
	PP_ONUPDATEDECODER,			/** < Port lisening on decoder update */

	//Request Service ports: Engine (Various Tasks)

	RP_ENGINEGETDECODER, 		/** < Request Port : Get decoder data */
	RP_ENGINESETSPEED,			/** < Request Port : Set engine speed */

	//Request Service ports: Remote (Various Tasks)

	RP_REMOTE,					/** < Request Port : Send out remote command */

	//Provide service ports: Communication (Communication Task)
	PP_CANOPENSDO,				/** < Provide Port: Perform SDO request */
	PP_XBEETX,					/** < Provide Port : Perform XBEE Transfer */

	} MSG_port_t;


	//,

/**
 * Internal structure of the dispatcher table
 */
typedef struct
	{
		MSG_port_t 									port;
		MSG_receiver_processPayload_hdl_t 			payload_hdl;
		MSG_receiver_getReturnMessageConfig_hdl_t	returnconfig_hdl;
	} MSG_port_callback_t;

/**************************************************************************************
 * Portlists defined in the cpp file
 **************************************************************************************/


	//================== C0_Communication

	extern const MSG_port_callback_t MSG_portList_C0_Communication[] ;
	extern const uint32_t MSG_portList_C0_Communication_size ;

	//================== C0_SafetyLogic

	extern const MSG_port_callback_t MSG_portList_C0_SafetyLogic[];
	extern const uint32_t MSG_portList_C0_SafetyLogic_size;

	//================== C1_Control

	extern const MSG_port_callback_t MSG_portList_C1_Control[] ;
	extern const uint32_t MSG_portList_C1_Control_size;

	/**************************************************************************************
	 * Functions
	 **************************************************************************************/


#endif /* MSG_CONFIG_H_ */
