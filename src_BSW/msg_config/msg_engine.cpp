/**
 * \file 	cdd_engine.cpp
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

#include "msg_engine.h"

#include "msg_type.h"
#include "msg_config.h"
#include "msg_dispatcher.h"

#include "enginedecoder_asyncbuffer_type.h"

#include "det.h"
#include "det_time.h"

#include "CanOpen_OD_cfg.h"
#include "msg_canopen.h"
#include "Task_C0_Communication.h"

#include "signal_bb.h"

#include "swc_dead_reackoning.h"


#define TIMEOUT_CANOPENREQuest 20

/**
 * The function creates the SDO requests for the decoders and sends them through the message handler to the communication task
 * @param ENGINEDECODER_data_t * const decoderdata 	: The function works non blocking, i.e. upon termination the "decoderdata" parameter is not updated, but set to 0 values and the signal itself is set to INVALID
 * @param const SIG_IDX_TYPE index 						: The index parameter is the index of the dispatcher table used for the communication
 */
RC_t MSG_tx_decoderToCanopen(ENGINEDECODER_raw_t * const decoderdata, const SIG_IDX_TYPE index)
{
	RC_t result;
	CANOPEN_sdo_record_t data;

	//Clear the buffer engine flag. All other data must remain
	ENGINEDECODER_ASYNCBUFFER_data_t decoder = ENGINEDECODER_ASYNCBUFFER_get(&so_in_enginedecoderBuffer);
	decoder.status.allEngine = 0;
	ENGINEDECODER_ASYNCBUFFER_set(&so_in_enginedecoderBuffer, decoder);

	//Common configuration per decoder
	data.index = INDEX_ENGINE_GETDECODER;
	data.subindex = 0;
	data.timeout = TIMEOUT_CANOPENREQuest;
	data.direction = CANOPEN_SDO_DIR_UP;
	data.data = 0;


	/*
	DET_TIME_resetStopwatch();
	 */

	//Last parameter will be used to identify the return message
	//Send decoder front right (FR) request
	data.nodeId = NODE_ID_FR;
	result = MSG_send((void*)&data, sizeof(CANOPEN_sdo_record_t), RP_ENGINEGETDECODER, PP_CANOPENSDO, Task_C0_Communication_getTaskId);

	//Send decoder front left (FL) request
	data.nodeId = NODE_ID_FL;
	result = MSG_send((void*)&data, sizeof(CANOPEN_sdo_record_t), RP_ENGINEGETDECODER, PP_CANOPENSDO, Task_C0_Communication_getTaskId);

	//Send decoder rear right (RR) request
	data.nodeId = NODE_ID_RR;
	result = MSG_send((void*)&data, sizeof(CANOPEN_sdo_record_t), RP_ENGINEGETDECODER, PP_CANOPENSDO, Task_C0_Communication_getTaskId);

	//Send decoder rear left (RL) request
	data.nodeId = NODE_ID_RL;
	result = MSG_send((void*)&data, sizeof(CANOPEN_sdo_record_t), RP_ENGINEGETDECODER, PP_CANOPENSDO, Task_C0_Communication_getTaskId);



}

/**
 * This function will be called by the central message dispatcher and will assemble the decoder signal out out the individual SDO reply messages
 * \param void * const pInPayload									IN: Pointer to the payload of the incoming message object
 * \param uint32_t inPayloadSize									IN: Size of the incoming messagy payload
 * \param void * const pOutPayload									IN/OUT: Buffer for the return message payload
 * \param uint32_t outPayloadSize									IN: Size of the return message payload
 */
RC_t MSG_rx_decoderFromCanopen( void * const pInPayload, uint32_t inPayloadSize, void * const pOutPayload, uint32_t outPayloadSize )
{
	RC_t result = RC_SUCCESS;

	//TODO get rid of magic number
	if (5 != inPayloadSize)
	{
		//Wrong size
		result = RC_ERROR_BAD_PARAM;
	}
	else
	{
		//Good size
		uint8_t nodeId = ((uint8_t*)pInPayload)[inPayloadSize-1];
		//We want to read 4 byte, therefore we have to cast the pointer to a uint32_t pointer
		sint32_t data = (sint32_t)(*(uint32_t*)(pInPayload));


		//We add the data to the buffer signal
		ENGINEDECODER_ASYNCBUFFER_data_t decoderBuffer = ENGINEDECODER_ASYNCBUFFER_get(&so_in_enginedecoderBuffer);


		switch (nodeId)
		{
		case NODE_ID_FL :
			decoderBuffer.engineFL_Pos = data;
			decoderBuffer.status.engine.fl = 1;
			break;
		case NODE_ID_FR :
			decoderBuffer.engineFR_Pos = data;
			decoderBuffer.status.engine.fr = 1;
			break;
		case NODE_ID_RL :
			decoderBuffer.engineRL_Pos = data;
			decoderBuffer.status.engine.rl = 1;
			break;
		case NODE_ID_RR :
			decoderBuffer.engineRR_Pos = data;
			decoderBuffer.status.engine.rr = 1;
			break;
		default:
			DET_stop(AUTOCORE, MSG_RX_WRONGDATA, nodeId );
			break;

		}

		//We store the data in the buffer
		ENGINEDECODER_ASYNCBUFFER_set(&so_in_enginedecoderBuffer, decoderBuffer);


		//Once all engines are updated, we transfer the data to the "real" decoder signal
		if (0x0F == decoderBuffer.status.allEngine)
		{
			ENGINEDECODER_raw_t decoderReal = ENGINEDECODER_get_raw(&so_in_enginedecoder);

			decoderReal.engineFL_Pos = decoderBuffer.engineFL_Pos;
			decoderReal.engineFR_Pos = decoderBuffer.engineFR_Pos;
			decoderReal.engineRL_Pos = decoderBuffer.engineRL_Pos;
			decoderReal.engineRR_Pos = decoderBuffer.engineRR_Pos;

			result = ENGINEDECODER_set_raw(&so_in_enginedecoder, decoderReal);
		}

		/*
		//Debugging
		if (decoderBuffer.status.allEngine == 15)
		{
			DET_TIME_stopStopwatch();
		}
		*/

	}



	return result;
}


/**
 * The function uses the updated decoder values for the calculation of the current position using a dead reckoning approach
 * \param void * const pInPayload								IN: Pointer to the payload of the incoming message object
 * \param uint32_t inPayloadSize									IN: Size of the incoming messagy payload
 * \param void * const pOutPayload									IN/OUT: Buffer for the return message payload
 * \param uint32_t outPayloadSize									IN: Size of the return message payload
 */
RC_t MSG_rx_decoderDeadreckoning( void * const pInPayload, uint32_t inPayloadSize, void * const pOutPayload, uint32_t outPayloadSize )
{
	RC_t result = RC_SUCCESS;

	//TODO get rid of magic number
	if (sizeof(ENGINEDECODER_data_t) != inPayloadSize)
	{
		//Wrong size
		result = RC_ERROR_BAD_PARAM;
	}
	else
	{
		//Process the payload if needed
		//Can be ignored for the time being as every task has read access to the signals.
		//TODO Lateron, we will copy the data into an own signal to ensure data consistency

		//Call the corresponding runnable
		DR_run_calculatePosition();

	}

	return result;
}
