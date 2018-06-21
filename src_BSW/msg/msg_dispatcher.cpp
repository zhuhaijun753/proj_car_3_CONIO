/**
 * \file 	msg_dispatcher.cpp
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



#include "pxdef.h"
#include "det.h"
#include "global.h"

#include "msg_type.h"
#include "msg_dispatcher.h"


/**********************************************************************************************************************************************
/** Type definitions
 **********************************************************************************************************************************************/





/**********************************************************************************************************************************************
/** Static functions
 **********************************************************************************************************************************************/


/**
 * Searches the function pointer corresponding to the port
 * \param MSG_port_t entry									IN: Port identifier to be searched for
 * \param MSG_port_callback_t const * const dispatchertable	IN: Pointer to the task's dispatcher table
 * \param uint32_t dispatchertablesize						IN: Dispatchertable size
 * \param MSG_receiver_processPayload_hdl_t* payload_hdl	OUT: Function Pointer to Payload handler
 * \param MSG_receiver_getReturnMessageConfig_hdl_t* returnMessageConfig_hdl
 * 															OUT: Function Pointer to return message config handler
 *
 * \return RC code
 */
static RC_t MSG__getReceiverCallback(MSG_port_t entry, MSG_port_callback_t const * const dispatchertable, uint32_t dispatchertablesize,
		MSG_receiver_processPayload_hdl_t* payload_hdl, MSG_receiver_getReturnMessageConfig_hdl_t* returnMessageConfig_hdl);


/**********************************************************************************************************************************************
/** Function implementations
 **********************************************************************************************************************************************/

/**
 * Generic function sending out a message using the entry of the dispatcher table
 * @param const void * const record						: Pointer to the message object
 * @param const uint8_t length							: Length of the message object
 */
RC_t MSG_send(const void * const record, const uint32_t length, MSG_port_t senderPort, MSG_port_t receiverPort, MSG_getTaskId_callback_t receiverTask)

{

	if (0 == receiverTask) return RC_ERROR_BAD_PARAM;

	RC_t result;

	//Let's build the metadata
	MSG_metadata_t metadata;
	metadata.x.senderPort = (uint16_t)senderPort;
	metadata.x.receiverPort = (uint16_t)receiverPort;
	metadata.x.status	= 0;
	metadata.x.empty = 0;


	//Get the handler to the mailbox
	PxTask_t rxTask = receiverTask();
	PxMbx_t receiver_Mbx = PxTaskGetMbx( rxTask );

	//Get the handler to the receiver mailbox
	 PxMbx_t receiverMailbox;

	//Create message
	//Todo: Optimisation potential, consider using receiver mailbox....
	PxMsg_t pxmsg = PxMsgRequest(length, PXMcTaskdefault, PXOpoolTaskdefault);


	//Send message
	if (PXERR_NOERROR == PxMsgIdError(pxmsg))
	{
		//Set the metadata of the message
		PxMsgSetMetadata(pxmsg, metadata.u64);

		//Get pointer to data area of the message
		uint8_t* p_msg =  (uint8_t*) PxMsgGetData(pxmsg);
		uint8_t* p_source = (uint8_t*) record;

		//Copy data into the message
		//As we do not know the object type, we have to copy it bytewise
		for (uint32_t i = 0; i < length; i++)
		{
			p_msg[i] = p_source[i];
		}

		//Send out the message
		pxmsg = PxMsgSend(pxmsg, receiver_Mbx);
	}
	else
	{
		DET_stop(AUTOCORE, MSG_TX, (uint32_t)senderPort);
		return RC_ERROR;
	}
	return RC_SUCCESS;
}


/**
 * Will process the message on receiver side
 */
RC_t MSG_receive(PxMsg_t const * const msg_in, MSG_port_callback_t const * const dispatchertable, uint32_t dispatchertablesize)
{

	RC_t resultReturnConfigHandler = RC_SUCCESS;
	RC_t resultPayloadHandler = RC_SUCCESS;
	RC_t resultDispatcherEntry = RC_SUCCESS;

	PxMsg_t outMsg;				//optional return message handler

	//***************************** Process the incoming message content and metadata

	//Lets first get the metada
	MSG_metadata_t inMetadata;

	inMetadata.u64 = PxMsgGetMetadata(*msg_in);

	//Let's get the incoming message data
	void * pInMessagePayload = PxMsgGetData(*msg_in);
	uint32_t inMessageSize = PxMsgGetSize(*msg_in);

	 //memorize the sender mailbox for the answer
	 PxMbx_t receiver_Mbx = PxTaskGetMbx(PxMsgGetSender(*msg_in));

	//***************************** Get the required handlers for processing the message

	//Now we have to locate the function to be called
	MSG_receiver_processPayload_hdl_t payload_hdl = 0;
	MSG_receiver_getReturnMessageConfig_hdl_t returnMessageConfig_hdl = 0;
	resultDispatcherEntry = MSG__getReceiverCallback((MSG_port_t)inMetadata.x.receiverPort, dispatchertable, dispatchertablesize, &payload_hdl, &returnMessageConfig_hdl);


	//***************************** Call the return (out) message config handler and create the buffer for a return message if required

	//Check for answer message configuration
	uint32_t outMessageSize = 0;
	boolean_t outMessageTransfer = FALSE;
	uint8_t*  pOutMessagePayload = 0;

	//Let's call the returnMessageConfig Handler if it has been set
	//If the handler returns a valid size and transfer information, we will create the return message buffer
	//The buffer then will be filled by the payload_hdl call in thee next step
	if (0 != returnMessageConfig_hdl)
	{
		if (RC_SUCCESS != (resultReturnConfigHandler = returnMessageConfig_hdl(pInMessagePayload, inMessageSize, &outMessageSize, &outMessageTransfer)))
		{
			//TODO some improved error handling later on
			//No return message will be sent due to error in handler call
			outMessageSize = 0;
			outMessageTransfer = FALSE;
		}
		else
		{
			//Correct entry found
			//outMessageSize and outMessageTransfer has been set by the returnMessageConfig_hdl call
			if ((TRUE == outMessageTransfer) )
			{
				//Let's create the return message
				outMsg = PxMsgRequest(outMessageSize, PXMcTaskdefault, PXOpoolTaskdefault);

				//CHeck if buffer was allocated
				if (PXERR_NOERROR != PxMsgIdError(outMsg))
				{
					//Oops, an error occured while allocating memory
					outMessageSize = 0;
					outMessageTransfer = FALSE;
					DET_stop(AUTOCORE, MSG_RX_BUFFER, PxGetError());
				}

				pOutMessagePayload = (uint8_t*) PxMsgGetData(outMsg);
			}
			else
			{
				//Nothing needs to be done, as no return message will be sent
			}
		}
	}
	else
	{
		//Nothing needs to be done, as no return message will be sent
	}

	//***************************** Call the in message payload handler and fill the buffer for the return message

	if (0 != payload_hdl)
	{
		resultPayloadHandler = payload_hdl(pInMessagePayload, inMessageSize, pOutMessagePayload, outMessageSize);
	}

	//***************************** Incoming message release

	 //Now we can release the incoming message
	 PxMsgRelease(*msg_in);


	//***************************** Create and send the return message


	if (TRUE == outMessageTransfer && PxMsgIdIsValid(outMsg))
	{
		//Answer has to be send to senderport
		//Sender and receiver port are now inverted
		MSG_metadata_t outMetadata;

		outMetadata.x.receiverPort = inMetadata.x.senderPort;
		outMetadata.x.senderPort = inMetadata.x.receiverPort;
		outMetadata.x.status = (uint16_t) resultPayloadHandler;
		outMetadata.x.empty = 0;

		PxMsgSetMetadata(outMsg, outMetadata.u64);

		//Send out the message
		outMsg = PxMsgSend(outMsg, receiver_Mbx);

	}


	// TODO How to deal with asynchronous functions, e.g. Canopen call with a handler
	// One idea would be to set the return message to 0 and have it replaced by a handler call

}


/**
 * Searches the function pointer corresponding to the port
 * \param MSG_port_t entry									IN: Port identifier to be searched for
 * \param MSG_port_callback_t const * const dispatchertable	IN: Pointer to the task's dispatcher table
 * \param uint32_t dispatchertablesize						IN: Dispatchertable size
 * \param MSG_receiver_processPayload_hdl_t* payload_hdl	OUT: Function Pointer to Payload handler
 * \param MSG_receiver_getReturnMessageConfig_hdl_t* returnMessageConfig_hdl
 * 															OUT: Function Pointer to return message config handler
 *
 * \return RC code
 */
static RC_t MSG__getReceiverCallback(MSG_port_t entry, MSG_port_callback_t const * const dispatchertable, uint32_t dispatchertablesize,
		MSG_receiver_processPayload_hdl_t* payload_hdl, MSG_receiver_getReturnMessageConfig_hdl_t* returnMessageConfig_hdl)
{
	for (uint32_t i = 0; i <  dispatchertablesize; i++)
	{
		if (dispatchertable[i].port == entry)
		{
			//Correct entry found
			*payload_hdl = dispatchertable[i].payload_hdl;
			*returnMessageConfig_hdl = dispatchertable[i].returnconfig_hdl;
			return RC_SUCCESS;
		}
	}

	//Nothing found, error
	return RC_ERROR_NOT_MATCH;
}
