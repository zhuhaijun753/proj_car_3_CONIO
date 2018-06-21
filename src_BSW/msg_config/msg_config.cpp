/**
 * \file 	msg_config.cpp
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	06.12.2017
 *
 * \brief DEFAULT SOURCE FILE TEMPLATE
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

#include "msg_type.h"
#include "msg_config.h"

//Header files for callbacks
#include "msg_engine.h"
#include "msg_uart.h"
#include "msg_canopen.h"

/**********************************************************************************************************************************************
/** Dispatcher table
 **********************************************************************************************************************************************/

//================== C0_Communication

const MSG_port_callback_t MSG_portList_C0_Communication[] = {
		//Port					Payload Handler					Return config handler
		{ PP_CANOPENSDO, 		MSG_rx_getSetSdoBlocked, 		MSG_rxconf_getSetSdoBlocked },
		{ PP_XBEETX, 	    	MSG_rx_xbeeTx, 					0 }

};

const uint32_t MSG_portList_C0_Communication_size = sizeof(MSG_portList_C0_Communication) / sizeof (MSG_port_callback_t);


//================== C0_SafetyLogic

const MSG_port_callback_t MSG_portList_C0_SafetyLogic[] = {
		//Port						Payload Handler							Return config handler
		{ RP_ENGINEGETDECODER, 		MSG_rx_decoderFromCanopen, 				0},
};

const uint32_t MSG_portList_C0_SafetyLogic_size = sizeof(MSG_portList_C0_SafetyLogic) / sizeof (MSG_port_callback_t);

//================== C1_Control

const MSG_port_callback_t MSG_portList_C1_Control[] = {
		//Port						Payload Handler							Return config handler
		{ RP_ENGINEGETDECODER, 		MSG_rx_decoderFromCanopen, 				0},
		{ PP_ONUPDATEDECODER,		MSG_rx_decoderDeadreckoning,			0},

};

const uint32_t MSG_portList_C1_Control_size = sizeof(MSG_portList_C1_Control) / sizeof (MSG_port_callback_t);




