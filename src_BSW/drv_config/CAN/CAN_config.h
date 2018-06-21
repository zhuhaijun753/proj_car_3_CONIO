/**
 * \file	CAN_config.h
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	11.010.2017
 * \version	0.7
 *
 * \brief CAN configuration header
 *
 *	Changelog: \n
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
#ifndef _CAN_CONFIG_H_
#define _CAN_CONFIG_H_

#include "CANopen_config.h"

#include "Port.h"				//RxSelect
#include "CAN.h"				//CAN_Frame_Write
#ifdef __cplusplus
#include "CRingBuffer.h"
#endif /* __cplusplus */

//-------------------------------------------------------------------- [Target specific Definitions]

#define CANOPEN_EOR_ID          uint8_t_MAX 		                       	/**< \brief Indicates the end of a list (e.g. configuration table)*/

//-------------------------------------------------------------------- [Configuration]
/** \brief CAN TX Ringbuffer size */
#define CAN_TX_BUFFER_SIZE		10

/** \brief Node that shall be used for TX messages from CANopen */
#define CANOPEN_TX_NODE			0

/** \brief CAN clock divider mode selection
 *
 * normal divider mode 		(01b): fCAN=fA*1/n 		-- with n = 1024 - #CAN_CFG_FDR_STEP
 * fractional divider mode 	(10b): fCAN=fA*n/1024 	-- with n = #CAN_CFG_FDR_STEP (0-1023)
 */
#define CAN_CFG_CD_DM			0b01

/** \brief Divide Prescaler Clock by 8 selection
 *
 * 	0B	-> 	A time quantum lasts (BRP+1) clock cycles.
 * 	1B	->	A time quantum lasts 8 × (BRP+1) clock cycles.
 */
#define CAN_CFG_DIV8			0b0

/** \brief Time Segment Before Sample Point
 *
 * (TSEG1 + 1) time quanta is the user-defined nominal time between the end of the synchronization segment and the sample point. It includes the propagation
 * segment, which takes into account signal propagation delays. The time segment may be lengthened due to re-synchronization.
 * Valid values for TSEG1 are 2 to 15.
 */
#define CAN_CFG_TSEG1			0x0E

/** \brief Time Segment After Sample Point
 *
 * (TSEG2 + 1) time quanta is the user-defined nominal time between the sample point and the start of the next synchronization segment.
 * It may be shortened due to re-synchronization.
 * Valid values for TSEG2 are 1 to 7.
 */
#define CAN_CFG_TSEG2			0x03

#define CAN_CFG_SJW				0x03										/**< \brief (Re) Synchronization Jump Width : (SJW + 1) time quanta are allowed for resynchronizations */
#define CAN_CFG_CLKSEL			CAN_ClockSelect_fasyn_CAN					/**< \brief CAN clock selection, see #CAN_ClkSel_t */
#define CAN_CFG_FDR_STEP		1023										/**< \brief CAN clock divider value, see#CAN_CFG_FD_DM*/
#define CAN_CFG_PAN_TO			200											/**< \brief Timeout (in iterations) for CAN Panel commands */
#define CAN_CFG_INI_TO			1000										/**< \brief Timeout (in iterations) for CAN Panel initialization */
#define CAN_CFG_RX_IR_L			1											/**< \brief Interrupt line for RX interrupts */
#define CAN_CFG_TX_IR_L			0											/**< \brief Interrupt line for TX interrupts */
#define CAN_ACR_ID_SHIFT        (29-11)                                     /**< \brief shift required in the address control registers. For 11bit IDs, its 29-11, for 29 its 0*/
#define CAN_AMR_NONE			IFX_CAN_MO_AMR_AM_MSK						/**< \brief Acceptance mask if all ID bits have to match the arbitration register*/

//-------------------------------------------------------------------- [Target specific types]
#ifdef __cplusplus
typedef CRingBuffer<CAN_Frame_t,CAN_TX_BUFFER_SIZE> CAN_TX_BUF_t;
extern CAN_TX_BUF_t CAN_TX_BUF;
#endif /* __cplusplus */

/** \brief CAN node baudrate selection */
typedef enum{
	CAN_inactive,															/**< \brief Node remains uninitialized  */
	CAN_loopback,															/**< \brief Internal loopback is activated for this node */
	CAN_1Mbps,																/**< \brief Node operates at 1MBit per second */
	CAN_500kbps,															/**< \brief Node operates at 500kBit per second */
	CAN_250kbps,															/**< \brief Node operates at 250kBit per second */
	CAN_125kbps,															/**< \brief Node operates at 125kBit per second */
} CANBaud_t;

/**
 * \brief Node configuration
 *
 * In the RX Mailbox Configuration table, the mailboxes for the CAN Controller are configured and it is defined how to handle received messages.
 * Each entry must have an unique COB_ID and a callback function that is called when a message with a matching COB_ID is received.
 */
typedef struct{
	CANBaud_t					baudrate;									/**< \brief Baudrate at which the Node shall operate, see#CANBaud_t. */
	RxSelect_t					rxSelect;									/**< \brief Pin selection of the Node, see #RxSelect_t. */
}CAN_N_cfg_t;

/**
 * \brief RX Mailbox Configuration
 *
 * In the RX Mailbox Configuration table, the mailboxes for the CAN Controller are configured and it is defined how to handle received messages.
 * Each entry must have an unique COB_ID and a callback function that is called when a message with a matching COB_ID is received.
 */
typedef struct{
	uint8_t						Node;										/**< \brief Node which shall handle messages. */
	uint16_t                    fctCODE;                                    /**< \brief CANopen function code. */
	RC_t                        (*callback) (CANOPEN_RX_CALLBACK_PARAMS);   /**< \brief callback function which is called when message with given FctCode is received. */
	CANopen_Mbx_range_t         range;                                      /**< \brief Flags to configure the Mailbox */
	boolean_t					RTR;										/**< \brief Only accept RTR messages. */
	uint8_t						buffer_depth;								/**< \brief CAN FIFO buffer depth. */
}CANopen_Rx_Mailbox_t;

/** \brief TX Mailbox Configuration */
typedef struct{
	uint8_t						Node;										/**< \brief Node which shall transmit messages. */
	uint8_t						buffer_depth;								/**< \brief CAN FIFO buffer depth. */
}CANopen_Tx_Mailbox_t;

//-------------------------------------------------------------------- [Configuration reference]
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
EXTERN const CAN_N_cfg_t CANNodeConfig[DERIVATE_NUM_CAN_N];					/**< \brief CAN Node configuration reference. */
EXTERN const CANopen_Rx_Mailbox_t CANopen_Rx_Mailboxes[];					/**< \brief CAN RX Mailbox configuration reference. */
EXTERN const CANopen_Tx_Mailbox_t CANopen_Tx_Mailboxes[];					/**< \brief CAN TX Mailbox configuration reference. */
#ifdef __cplusplus
}
#endif /* __cplusplus */
//-------------------------------------------------------------------- [Hardware Interface]

 /**
 * \brief Send a CAN Message.
 *
 * This function will pass the given data to the low-level driver.
 * If an failure occurred, CANopen__TXBufErrorHdl is called, which will determine the result of #CANopen__SendCANMsg.
 * \param   CANOPEN_TX_PARAMS   See #CANOPEN_TX_PARAMS
 * \return  #RC_SUCCESS         Operation succeeded
 *          #RC_ERROR           Message could not be send
 */
INLINE RC_t CANopen__SendCANMsg(CANOPEN_TX_PARAMS){

    //try to send CAN Message
    if(CAN_Frame_Write(CANOPEN_TX_NODE,(CAN_Frame_t*)CAN_out)!=RC_SUCCESS){

        //increment TX error frame counter
        CANopen_incFrameCnt(CANopen_TX_ER_Frames);

        //Call error handler
        return CANopen__TXBufErrorHdl();
    }

    //increment frame counter
    CANopen_incFrameCnt(CANopen_TX_Frames);

    //we are done
    return RC_SUCCESS;
}

#endif /* _CAN_CONFIG_H_ */
/* [] END OF FILE */
