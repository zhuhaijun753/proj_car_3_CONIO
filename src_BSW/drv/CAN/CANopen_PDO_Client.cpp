/*
 * CANopen_PDO_Client.cpp
 *
 *  Created on: 18-Aug-2017
 *      Author: Rahul
 */


//-------------------------------------------------------------------- [Header]
#include "CANopen.h"
#include "CAN_config.h"
#include "CanOpen_OD_cfg.h"


RC_t CANopen_PDO_hdl(CANOPEN_RX_CALLBACK_PARAMS)
{
	return CANOPEN_process_PDO(CAN_in);
}
