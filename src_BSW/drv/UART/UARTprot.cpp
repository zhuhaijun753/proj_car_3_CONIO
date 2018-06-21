/**
* \file UARTprot.cpp
* \author P. Fromm
* \date 15.06.2016
*
* \brief UARTprot
*
*  Changelog:\n
*  - 1.0 ; 17-Aug-2016; Gunaseelan
*            -  changed #define REMOTEPORT to uart2 from uart1
*
* \copyright Copyright ©2016
* Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
* Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
* (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
* this paragraph and the following paragraph appear in all copies, modifications, and distributions.
* Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
*
* This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
* The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
* This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
*/

#include "UARTprot.h"
#include "UART.h"

//***********************************************************************************//
//**************** Ringbuffer objects ***********************************************//
//***********************************************************************************//

#define RTEPORT    uartusb
#define REMOTEPORT uart4

#pragma section ".com.uart"

//Ringbuffer which will contain the data coming from the Zigbee port
CARDATAPROTOCOL_rawBuffer_t UARTPROT_remoteRxBuffer(0x0d,0x0e);
CARDATAPROTOCOL_rawBuffer_t UARTPROT_remoteTxBuffer(0x0d,0x0e);

RTETIME_RxBuffer_t UARTPROT_rteRxBuffer(0x0d,0x0e);
#pragma section

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
void UARTPROT_remote_init()
{
	UARTPROT_remoteRxBuffer.init(0x0d,0x0e);
	UARTPROT_remoteTxBuffer.init(0x0d,0x0e);
}

/**
 * Transmits one protocol.
 * Transmission of first byte is done explicitly, rest is handled by TXISR
 * @param pTxBuffer: Double-Pointer to transmission ringbuffer
 * @param index: ignored
 * return RC_SUCCESS
 */
RC_t UARTPROT_remote_transmit(const CARDATAPROTOCOL_raw_t * const pTxBuffer, const SIG_IDX_TYPE index)
{
	uint8_t data;
	if ((*pTxBuffer)->readData(data))
	{
		//Todo: CHeck if interrupt threshold is reached with 1 byte
		//Todo: Schreibe solange, bis HW-Fifo voll ist
		return UART_WriteData(REMOTEPORT, data);

	}
	return RC_ERROR_BUFFER_EMTPY;
}

/**
 * Transmission of next bytes of protocol, will be called inside TXISR
 * @param pTxBuffer: Pointer to transmission ringbuffer
 */
void UARTPROT_remote_transmit_isr(const CARDATAPROTOCOL_raw_t pTxBuffer)
{
	uint8_t data;
	if (pTxBuffer->readData(data))
	{
		UART_WriteData(REMOTEPORT, data);
	}
}

/**
 * Reception of protocol, data will be stored in the provided ringbuffer, will be called inside RXISR
 * @param pRxBuffer: Pointer to reception ringbuffer
 * @return: Indicates complete protocol
 */
bool UARTPROT_remote_receive_isr(const CARDATAPROTOCOL_raw_t pRxBuffer, bool& fullProtocol)
{
	uint8_t data;

	//Read data from UART
	UART_ReadData(REMOTEPORT,&data);

	//Store it in ringbuffer
	bool result = pRxBuffer->writeData(data);

	//Check if complete protocol has been received
	fullProtocol = (bool)(pRxBuffer->checkEndOfProtocol());

	//Return error in case of full buffer
	return result;


}



//***********************************************************************************//
//**************** Global functions for the rte timing ******************************//
//***********************************************************************************//

/**
 * Initialises the ringbuffers for the remote communication
 */
void UARTPROT_rte_init()
{
	UARTPROT_rteRxBuffer.init(0x0d,0x0e);
}

/**
 * Reception of protocol, data will be stored in the provided ringbuffer, will be called inside RXISR
 * @param pRxBuffer: Pointer to reception ringbuffer
 * @return: Indicates complete protocol
 */
bool UARTPROT_rte_receive_isr(RTETIME_RxBuffer_t* const pRxBuffer, bool& fullProtocol)
{
	uint8_t data;

	//Read data from UART
	UART_ReadData(RTEPORT,&data);

	//Store it in ringbuffer
	bool result = pRxBuffer->writeData(data);

	//Check if complete protocol has been received
	fullProtocol = (bool)(pRxBuffer->checkEndOfProtocol());

	//Return error in case of full buffer
	return result;

}


#ifdef  __cplusplus
} //extern "C"
#endif
