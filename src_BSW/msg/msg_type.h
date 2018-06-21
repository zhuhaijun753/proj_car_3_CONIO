/**
 * \file 	msg_type.h
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
#ifndef MSG_TYPE_H_
#define MSG_TYPE_H_

#include "global.h"
#include "pxdef.h"

/**************************************************************************************
 * Defines
 **************************************************************************************/

#define MSGWRAPPER_INVALIDPAYLOADSIZE 0xffffffff	/**< Indicates an invalid size */


/**************************************************************************************
 * Function Pointer Types
 **************************************************************************************/

/**
 * This function will be called by the central message dispatcher and will forward the data to the driver function
 * \param void * const pInPayload								IN: Pointer to the payload of the incoming message object
 * \param uint32_t inPayloadSize									IN: Size of the incoming messagy payload
 * \param void * const pOutPayload									IN/OUT: Buffer for the return message payload
 * \param uint32_t outPayloadSize									IN: Size of the return message payload
 */
typedef RC_t (*MSG_receiver_processPayload_hdl_t) (void * const pInPayload, uint32_t inPayloadSize, void * const pOutPayload, uint32_t outPayloadSize);


/**
 * This function will calculate the payload size of the return message.
 * This needs to be done in an extra function, because the processPayload_callback requires a memory block to store the return message data.
 * \param void const * const pInPayload		IN: Payload of the message, required if the return message size depends on the message content
 * \param uint32_t inPayloadSize			IN: Size of the incomping payload
 * \param uint32_t* pOutPayloadSize			OUT: Payload size of the returned answer
 * \param boolean_t* pSendReturnMessage		OUT: Will an answer be send (TRUE) or not (FALSE)
 */
typedef RC_t (*MSG_receiver_getReturnMessageConfig_hdl_t)(void const * const pInPayload, uint32_t inPayloadSize, uint32_t* pOutPayloadSize, boolean_t* pSendReturnMessage);


/**
 * Function Pointer to getTaskId function
 */
typedef PxTask_t (*MSG_getTaskId_callback_t)();

/**************************************************************************************
 * Other types
 **************************************************************************************/

//Note:
// MSG_port_callback_t is declared in msg_config.h



/**
 * Union for simplified access to the 64bit metadata
 * As the enum size depends on the number of elements, we cast the msg_port_t to an uint16_t. This should be large enough
 */
typedef union
{
	uint64_t	u64;
	struct{
	uint16_t 	senderPort;			/**< sender port of the message. Will be used to send the answer */
	uint16_t 	receiverPort;		/**< receiver port of the message. Will be used to route the message content */
	uint16_t	status;				/**< status information, e.g. return code of a driver being called on receiver side */
	uint16_t	empty;				/**< can be used for any later meta information */
	} x;


} MSG_metadata_t;

#endif /* MSG_TYPE_H_ */
