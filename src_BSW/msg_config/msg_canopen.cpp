/**
 * \file 	CANopen_messageWrapper.cpp
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	21.12.2017
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

#include "msg_canopen.h"
#include "CanOpen_OD_cfg.h"

#include "msg_type.h"

#include "pxdef.h"
#include "det.h"


extern "C"
{

/**********************************************************************************
 /** Typedef and constant data
 **********************************************************************************/

typedef struct
{
	uint32_t node; /**< node Id */
	uint32_t index; /**< Index value */
	uint32_t size; /**< Required payload size based on node id and index */
} MHCANOPEN__returnConfig_t;

#define CO_PREFIX 1	//1 byte required for node id

const static MHCANOPEN__returnConfig_t MHCANOPEN__returnConfig[] =
{

//Engine configuration
//2 byte required for node id
		{ NODE_ID_RR, INDEX_ENGINE_SETSPEED, 	CO_PREFIX+0 },
		{ NODE_ID_RR, INDEX_ENGINE_GETDECODER, 	CO_PREFIX+4 },
		{ NODE_ID_RR, INDEX_ENGINE_GETSTATE, 	CO_PREFIX+4 },
		{ NODE_ID_RR, INDEX_ENGINE_SETSTATE, 	CO_PREFIX+0 },

		{ NODE_ID_RL, INDEX_ENGINE_SETSPEED, 	CO_PREFIX+0 },
		{ NODE_ID_RL, INDEX_ENGINE_GETDECODER, 	CO_PREFIX+4 },
		{ NODE_ID_RL, INDEX_ENGINE_GETSTATE, 	CO_PREFIX+4 },
		{ NODE_ID_RL, INDEX_ENGINE_SETSTATE, 	CO_PREFIX+0 },

		{ NODE_ID_FR, INDEX_ENGINE_SETSPEED, 	CO_PREFIX+0 },
		{ NODE_ID_FR, INDEX_ENGINE_GETDECODER, 	CO_PREFIX+4 },
		{ NODE_ID_FR, INDEX_ENGINE_GETSTATE, 	CO_PREFIX+4 },
		{ NODE_ID_FR, INDEX_ENGINE_SETSTATE, 	CO_PREFIX+0 },

		{ NODE_ID_FL, INDEX_ENGINE_SETSPEED, 	CO_PREFIX+0 },
		{ NODE_ID_FL, INDEX_ENGINE_GETDECODER, 	CO_PREFIX+4 },
		{ NODE_ID_FL, INDEX_ENGINE_GETSTATE, 	CO_PREFIX+4 },
		{ NODE_ID_FL, INDEX_ENGINE_SETSTATE, 	CO_PREFIX+0 },

//TODO Additional entries to be added

		};

/**********************************************************************************
 /** Function Implementation
 **********************************************************************************/

/**
 * This function will be called by the central message dispatcher and will forward the data to the CanOpen getSetSdo function
 * \param void * const pInPayload								IN: Pointer to the payload of the incoming message object
 * \param uint32_t inPayloadSize									IN: Size of the incoming messagy payload
 * \param void * const pOutPayload									IN/OUT: Buffer for the return message payload
 * \param uint32_t outPayloadSize									IN: Size of the return message payload
 */
RC_t MSG_rx_getSetSdoBlocked(void * const pInPayload, uint32_t inPayloadSize, void * const pOutPayload, uint32_t outPayloadSize)
{
	RC_t result = RC_SUCCESS;
	CANopen_SDO_transfer_RET_t status = CANOPEN_SDO_SUCCESS;

	//Check for correct size of payload
	if (inPayloadSize != sizeof(CANOPEN_sdo_record_t) ||
			0 == pOutPayload ||
			0 == outPayloadSize)
	{
		result =  RC_ERROR_BAD_PARAM;
	}
	else
	{
		//Size seems ok, let's pass it on to the CANOPEN stack

		//Cast the void pointer to real object
		CANOPEN_sdo_record_t* r = (CANOPEN_sdo_record_t*) pInPayload;

		//Add the node id to the return message on the last position, to keep a proper alignment for the payload
		((uint8_t*)pOutPayload)[outPayloadSize-1] = (uint8_t)r->nodeId;

		//Call CanOpen Function
		if (CANOPEN_SDO_DIR_UP == r->direction)
		{
			//Important: Record has to be big enough to hold the data
			if (pOutPayload != 0 && outPayloadSize-CO_PREFIX > 0)
			{
				status = CANopen_getSDO(0, pOutPayload, outPayloadSize-CO_PREFIX, r->nodeId, r->index, r->subindex, r->timeout, 0);
			}
			else
			{
				result = RC_ERROR_MEMORY;
			}
		}
		else if (CANOPEN_SDO_DIR_DN == r->direction)
		{
			status = CANopen_setSDO(0, pInPayload, inPayloadSize, r->nodeId, r->index, r->subindex, r->timeout, 0);
		}

		if (CANOPEN_SDO_SUCCESS != status)
		{
			result = RC_ERROR;
		}
	}

	return result;

}

/**
 * This function will calculate the payload size of the return message.
 * \param void const * const pInPayload		IN: Payload of the message, required if the return message size depends on the message content
 * \param uint32_t inPayloadSize			IN: Size of the incomping payload
 * \param uint32_t* pOutPayloadSize			OUT: Payload size of the returned answer
 * \param boolean_t* pSendReturnMessage		OUT: Will an answer be send (TRUE) or not (FALSE)
 */
RC_t MSG_rxconf_getSetSdoBlocked(void const * const pInPayload, uint32_t inPayloadSize, uint32_t* pOutPayloadSize, boolean_t* pSendReturnMessage)
{
	RC_t result = RC_SUCCESS;
	CANOPEN_sdo_record_t* r = (CANOPEN_sdo_record_t*) pInPayload;

	//Check for correct size of payload
	if (inPayloadSize != sizeof(CANOPEN_sdo_record_t))
	{
		result = RC_ERROR_BAD_PARAM;
		DET_stop(AUTOCORE, MSG_RX_PAYLOAD, r->nodeId);
	}
	else
	{
		//Input data is valid, let's calculate the return size

		//For Canopen, we willl always return a status at least
		*pSendReturnMessage = TRUE;

		//Cast the void pointer to real object
		CANOPEN_sdo_record_t* r = (CANOPEN_sdo_record_t*) pInPayload;

		uint32_t returnPayloadSize = MSGWRAPPER_INVALIDPAYLOADSIZE;

		for (uint32_t i = 0; i < sizeof(MHCANOPEN__returnConfig) / sizeof(MHCANOPEN__returnConfig_t); i++)
		{
			if ((r->nodeId == MHCANOPEN__returnConfig[i].node) && (r->index == MHCANOPEN__returnConfig[i].index))
			{
				//Correct entry found
				returnPayloadSize = MHCANOPEN__returnConfig[i].size;
				break;
			}

		}

		*pOutPayloadSize = returnPayloadSize;

		//This error will be thrown in case of a missing size definition
		if (MSGWRAPPER_INVALIDPAYLOADSIZE == returnPayloadSize)
		{
			result = RC_ERROR_NOT_MATCH;
			DET_stop(AUTOCORE, MSG_RX_RETURN, r->nodeId);
		}

	}

	return result;
}

} //extern C
