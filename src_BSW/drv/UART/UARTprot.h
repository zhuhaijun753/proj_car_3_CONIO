/*
 * UARTprot.h
 *
 *  Created on: 15.06.2016
 *      Author: Fromm
 */

#ifndef UARTPROT_H_
#define UARTPROT_H_

//***********************************************************************************//
//**************** Header ***********************************************************//
//***********************************************************************************//
#include "global.h"


#include "signal_global.h"
#include "cardataprotocol_type.h"
#include "rte_time.h"
#include "rte_time_type.h"
#include "CRingBuffer.h"

//***********************************************************************************//
//**************** Ringbuffer objects ***********************************************//
//***********************************************************************************//

extern CARDATAPROTOCOL_rawBuffer_t UARTPROT_slamComRxBuffer;
extern CARDATAPROTOCOL_rawBuffer_t UARTPROT_slamComTxBuffer;

extern CARDATAPROTOCOL_rawBuffer_t UARTPROT_remoteRxBuffer;
extern CARDATAPROTOCOL_rawBuffer_t UARTPROT_remoteTxBuffer;

typedef CRingBuffer<uint8_t,20> RTETIME_RxBuffer_t;
extern RTETIME_RxBuffer_t UARTPROT_rteRxBuffer;

#ifdef  __cplusplus
extern "C"
{
#endif


//***********************************************************************************//
//**************** Global functions for the remote control **************************//
//***********************************************************************************//


/**
 * Initialises the ringbuffers for the remote communication
 */
void UARTPROT_remote_init();

/**
 * Transmits one protocol.
 * Transmission of first byte is done explicitly, rest is handled by TXISR
 * @param pTxBuffer: Double-Pointer to transmission ringbuffer
 * @param index: ignored
 */
RC_t UARTPROT_remote_transmit(const CARDATAPROTOCOL_raw_t * const pTxBuffer, const SIG_IDX_TYPE index);

/**
 * Transmission of next bytes of protocol, will be called inside TXISR
 * @param pTxBuffer: Pointer to transmission ringbuffer
 */
void UARTPROT_remote_transmit_isr(const CARDATAPROTOCOL_raw_t pTxBuffer);

/**
 * Reception of protocol, data will be stored in the provided ringbuffer, will be called inside RXISR
 * @param IN pRxBuffer: Pointer to reception ringbuffer
 * @param OUT fullProtocol: True in case a full protocol was received, false otherwise
 * @return: true in case byte was stored successfully, false in case of buffer overflow
 */
bool UARTPROT_remote_receive_isr(const CARDATAPROTOCOL_raw_t pRxBuffer, bool& fullProtocol);


//***********************************************************************************//
//**************** Global functions for the rte timing ******************************//
//***********************************************************************************//

/**
 * Initialises the ringbuffers for the remote communication
 */
void UARTPROT_rte_init();

/**
 * Reception of protocol, data will be stored in the provided ringbuffer, will be called inside RXISR
 * @param pRxBuffer: Pointer to reception ringbuffer
 * @return: Indicates complete protocol
 */
bool UARTPROT_rte_receive_isr( RTETIME_RxBuffer_t* const pRxBuffer, bool& fullProtocol);

#ifdef  __cplusplus
} //extern "C"
#endif


#endif /* UARTPROT_H_ */
