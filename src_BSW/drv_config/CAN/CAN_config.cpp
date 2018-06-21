/**
 * \file 	CAN_config.cpp
 * \author 	Thomas Barth	-	thomas.barth@h-da.de
 * \date 	06.12.2016
 * \version	0.1
 *
 * \brief AURIX CAN configuration
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


#include "CAN_config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "CANopen_Client.h"
#define CANOPEN_EOR_ID          uint8_t_MAX 		                       	/**< \brief Indicates the end of a list (e.g. configuration table)*/


/**
 * \brief CAN node configuration table
 */
const CAN_N_cfg_t CANNodeConfig[DERIVATE_NUM_CAN_N] ={
		//Node 0: Controller -RX P207.7 Tx P20.8		AppKit X202 CANL - 3 CANH - 4
		{CAN_1Mbps,RxSel_b},
		//Node 1
		{CAN_inactive,RxSel_a},
		//Node 2
		{CAN_inactive,RxSel_a},
		//Node 3
		{CAN_inactive,RxSel_a},
};


//Stub
RC_t    _CANopenDefaultHdl 			(CANOPEN_RX_CALLBACK_PARAMS) {
	(void)CAN_in;
	return RC_SUCCESS;};

/**
 * \brief RX Mailbox Configuration, see #CANopen_Rx_Mailbox_t for entries.
 */
const CANopen_Rx_Mailbox_t CANopen_Rx_Mailboxes[]={
//  Node	Start ADR                               Callback-Function               scope    					RTR,	buffer depth
    {0,		CANopen_default_CANID_TSDO,          	CANopen_SDO_resp_hdl,       	CANopen_range_fct,			FALSE,	1},
	{0,		CANopen_default_CANID_TPDO_1,           CANopen_PDO_hdl,       			CANopen_range_fct,			FALSE,  1},
	{0,		CANopen_default_CANID_NODEGUARD,       	CANopen_NG_Hdl,                 CANopen_range_node,     	TRUE,	1},

//Do not remove this line!
    {CANOPEN_EOR_ID, 0 ,0,CANopen_range_fct,0}
};

/**
 * \brief TX Mailbox Configuration, see #CANopen_Tx_Mailbox_t for entries.
 */
const CANopen_Tx_Mailbox_t CANopen_Tx_Mailboxes[]={
//  Node	buffer depth
    {0,		0},

//Do not remove this line!
    {CANOPEN_EOR_ID,0}
};



#ifdef __cplusplus
}
#endif /* __cplusplus */
