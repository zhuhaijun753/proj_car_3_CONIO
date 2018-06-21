/**
 * \file 	msg_dispatcher.h
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
#ifndef MSG_DISPATCHER_H_
#define MSG_DISPATCHER_H_

#include "global.h"
#include "pxdef.h"
#include "msg_type.h"
#include "msg_config.h"



/**
 * Generic function sending out a message using the entry of the dispatcher table
 * \param const void * const record						: Pointer to the message object
 * \param const uint8_t length							: Length of the message object
 * \param MSG_port_t senderPort							: Port / Identifier of the Sender. Based on this value, the answer from the server will be routed
 * \param MSG_port_t receiverPort						: Receiver port on server side. Based on the value, the message will be routed
 * \param MSG_getTaskId_callback_t receiverTask			: Function Pointer to get id and private mailbox of the receiver task
 */
RC_t MSG_send(const void * const record, const uint32_t length, MSG_port_t senderPort, MSG_port_t receiverPort, MSG_getTaskId_callback_t receiverTask);


/**
 * Will process the message on receiver side
 * \param PxMsg_t const * const msg							: Message to be processed
 * \param MSG_port_callback_t const * const dispatchertable	: Pointer to the task's dispatcher table
 * \param uint32_t dispatchertablesize						: Dispatchertable size
 */
RC_t MSG_receive(PxMsg_t const * const msg, MSG_port_callback_t const * const dispatchertable, uint32_t dispatchertablesize);


#endif /* MSG_DISPATCHER_H_ */
