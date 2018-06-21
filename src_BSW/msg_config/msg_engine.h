/**
 * \file 	cdd_engine.h
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
#ifndef CDD_ENGINE_H_
#define CDD_ENGINE_H_

#include "global.h"
#include "signal_global.h"
#include "enginedecoder_type.h"

/**
 * The function creates the SDO requests for the decoders and sends them through the message handler to the communication task
 * @param ENGINEDECODER_V2_data_t * const decoderdata 	: The function works non blocking, i.e. upon termination the "decoderdata" parameter is not updated, but set to 0 values and the signal itself is set to INVALID
 * @param const SIG_IDX_TYPE index 						: The index parameter is the index of the dispatcher table used for the communication
 */
RC_t MSG_tx_decoderToCanopen(ENGINEDECODER_raw_t * const decoderdata, const SIG_IDX_TYPE index);

/**
 * This function will be called by the central message dispatcher and will assemble the decoder signal out out the individual SDO reply messages
 * \param void * const pInPayload								IN: Pointer to the payload of the incoming message object
 * \param uint32_t inPayloadSize									IN: Size of the incoming messagy payload
 * \param void * const pOutPayload									IN/OUT: Buffer for the return message payload
 * \param uint32_t outPayloadSize									IN: Size of the return message payload
 */
RC_t MSG_rx_decoderFromCanopen( void * const pInPayload, uint32_t inPayloadSize, void * const pOutPayload, uint32_t outPayloadSize );



/**
 * The function uses the updated decoder values for the calculation of the current position using a dead reckoning approach
 * \param void * const pInPayload								IN: Pointer to the payload of the incoming message object
 * \param uint32_t inPayloadSize									IN: Size of the incoming messagy payload
 * \param void * const pOutPayload									IN/OUT: Buffer for the return message payload
 * \param uint32_t outPayloadSize									IN: Size of the return message payload
 */
RC_t MSG_rx_decoderDeadreckoning( void * const pInPayload, uint32_t inPayloadSize, void * const pOutPayload, uint32_t outPayloadSize );

#endif /* CDD_ENGINE_H_ */
