/**
 * \file 	CANopen_messageWrapper.h
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	21.12.2017
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
#ifndef CANOPEN_MESSAGEWRAPPER_H_
#define CANOPEN_MESSAGEWRAPPER_H_

#include "global.h"
#include "signal_global.h"
#include "CANopen_Client.h"


#include "CANopen_Client.h"
#ifdef  __cplusplus
extern "C"
{
#endif


/** \brief structure used for message transfer of SDO objects */
typedef struct
{
	uint8_t 						nodeId;
	uint32_t						data;
	uint16_t 						index;
	uint8_t 						subindex;
	uint32_t 						timeout;
	CANopen__SDO_transfer_dir_t 	direction;
} CANOPEN_sdo_record_t;


/**********************************************************************************
/** Message (asynchronous) Functions
 **********************************************************************************/

/**
 * This function will be called by the central message dispatcher and will forward the data to the CanOpen getSetSdo function
 * \param void * const pInPayload								IN: Pointer to the payload of the incoming message object
 * \param uint32_t inPayloadSize									IN: Size of the incoming messagy payload
 * \param void * const pOutPayload									IN/OUT: Buffer for the return message payload
 * \param uint32_t outPayloadSize									IN: Size of the return message payload
 */
RC_t MSG_rx_getSetSdoBlocked(void * const pInPayload, uint32_t inPayloadSize, void * const pOutPayload, uint32_t outPayloadSize);

/**
 * This function will calculate the payload size of the return message.
 * \param void const * const pInPayload		IN: Payload of the message, required if the return message size depends on the message content
 * \param uint32_t inPayloadSize			IN: Size of the incomping payload
 * \param uint32_t* pOutPayloadSize			OUT: Payload size of the returned answer
 * \param boolean_t* pSendReturnMessage		OUT: Will an answer be send (TRUE) or not (FALSE)
 */
RC_t MSG_rxconf_getSetSdoBlocked(void const * const pInPayload, uint32_t inPayloadSize, uint32_t* pOutPayloadSize, boolean_t* pSendReturnMessage);

#ifdef  __cplusplus
}
#endif


#endif /* CANOPEN_MESSAGEWRAPPER_H_ */
