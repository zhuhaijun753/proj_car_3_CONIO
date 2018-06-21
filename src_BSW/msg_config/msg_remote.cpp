/**
 * \file 	message_driver.cpp
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	18.10.2016
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


#include "msg_remote.h"

#include "global.h"
#include "pxdef.h"
#include "det.h"

#include "signal_bb.h"

#include "msg_type.h"
#include "msg_config.h"
#include "msg_dispatcher.h"

#include "det.h"
#include "det_time.h"

#include "Task_C0_Communication.h"



RC_t MSG_tx_remoteToXbee(const CARDATAPROTOCOL_data_t * const msg, const SIG_IDX_TYPE index)
{

	RC_t result;

	result = MSG_send((void*)msg, sizeof(CARDATAPROTOCOL_data_t), RP_REMOTE, PP_XBEETX, Task_C0_Communication_getTaskId);

	return result;

}
