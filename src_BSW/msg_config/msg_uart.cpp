/**
 * \file 	UART_messageWrapper.cpp
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	02.01.2018
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

#include "msg_uart.h"

#include "signal_bb.h"
#include "CARDATAPROTOCOL_type.h"
#include "CARDATAPROTOCOL_signal.h"

#include "det.h"

extern "C"
{
/**
 * This function will be called by the central message dispatcher and will forward the data to the CanOpen getSetSdo function
 * \param void * const pInPayload									IN: Pointer to the payload of the incoming message object
 * \param uint32_t inPayloadSize									IN: Size of the incoming messagy payload
 * \param void * const pOutPayload									IN/OUT: Buffer for the return message payload
 * \param uint32_t outPayloadSize									IN: Size of the return message payload
 */
RC_t MSG_rx_xbeeTx(void * const pInPayload, uint32_t inPayloadSize, void * const pOutPayload, uint32_t outPayloadSize)
{
	RC_t result = RC_SUCCESS;

	//Check for correct size of payload
	if (inPayloadSize != sizeof(CARDATAPROTOCOL_data_t))
	{
		result = RC_ERROR_BAD_PARAM;
		DET_stop(AUTOCORE, MSG_RX_PAYLOAD, inPayloadSize);
	}


	//Deep copy to be able to release the message memory
	CARDATAPROTOCOL_data_t* remoteptr = (CARDATAPROTOCOL_data_t*)pInPayload;
	CARDATAPROTOCOL_data_t remotedata = (*remoteptr);

	//Write data to the physical port
	CARDATAPROTOCOL_write(&so_out_remote, remotedata);

	return result;
}



} //extern C
