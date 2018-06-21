/**
 * \file 	CAN.h
 * \author 	Thomas Barth	-	thomas.barth@h-da.de
 * \date 	03.06.2018
 * \version	0.2
 *
 * \brief AURIX CAN driver
 *
 * Changelog:\n
 *      0.2 03.06.18 Barth
 *              - Remember software mailbox ID to easy assignment of frames to handler funcitons.
 *      0.1 12.12.16 Barth
 *              - Initial documentation
 *
 * \todo #PORT_CAN_BEGIN and #PORT_CAN_END define access to the whole module. It needs to be checked if subset symbols might be better.
 * \todo RX/TX - FIFO
 * \copyright Copyright ©2018
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
#ifndef CAN_H
#define CAN_H


//-------------------------------------------------------------------- [Includes]
#include "register.h"
#include "global.h"

#ifdef PXROS
	#include "pxdef.h"
#endif /* PXROS */

//-------------------------------------------------------------------- [Defines]

/** \brief Start-Address of CAN Module registers */
#define	MP_CAN_M_BEGIN	((unsigned int)&MODULE_CAN)

/** \brief End-Address of CAN Module registers */
#define	MP_CAN_M_END	((unsigned int)&MODULE_CAN + sizeof(Ifx_CAN))

/** \brief Start-Address of CAN List registers */
#define	MP_CAN_L_BEGIN	((unsigned int)&MODULE_CAN.LIST)

/** \brief End-Address of CAN List registers */
#define	MP_CAN_L_END	((unsigned int)&MODULE_CAN + sizeof(MODULE_CAN.LIST))

/** \brief Start-Address of CAN Message object registers */
#define	MP_CAN_MO_BEGIN	((unsigned int)&MODULE_CAN.MO)

/** \brief End-Address of CAN Message object registers */
#define	MP_CAN_MO_END	((unsigned int)&MODULE_CAN + sizeof(MODULE_CAN.MO))

//-------------------------------------------------------------------- [Types]

/** \brief CAN frame structure */
typedef struct{
	uint32_t			MsgID;		/**< \brief Message Identifier*/
	uint32_t			DLC;		/**< \brief Data Length Container*/
	Ifx_CAN_MO_DATAL 	dataL;		/**< \brief data bytes low (0-3)*/
	Ifx_CAN_MO_DATAH 	dataH;		/**< \brief data bytes high (4-7*/
} CAN_Frame_t;

/** \brief CAN RX message container*/
typedef struct{
	/** \brief Pointer to the message object that received the frame
	 *
	 * The message object provides additional information, such as the receiving node (Ifx_CAN_MO_STAT_Bits.LIST-1).
	 * \see 	#Ifx_CAN_MO
	 * \warning	As this is a pointer to the message object, data might be corrupted while reading it.
	 *			In order to work with the data, a deep copy needs to be done. If the Ifx_CAN_MO_STAT_Bits.NEWDAT flag is set,
	 *			a new frame was received and frame specific data might be corrupted.
	 */
	Ifx_CAN_MO*		p_MObj;
	CAN_Frame_t 	frame;			/**< \brief Received CAN frame*/
} CAN_RX_t;


//-------------------------------------------------------------------- [Global prototypes]

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * \brief Initialize the CAN Module
 *
 * This function initializes the CAN Module, the CAN Nodes and the corresponding CAN message objects.
 * #CAN__initModule, #CAN__NodeInit and #CAN__MObjInit are called.
 * In this configuration, all nodes share CAN Interrupt Lines 0 and 1. Line 0 for TX and Line 1 for RX.
 *
 * Because this function initializes the object lists, it has to wait for the control panel to be ready.
 * The timeout for this operation is defined at #CAN_CFG_INI_TO.
 *
 * \warning	This function requires a valid configuration of #SRC_CANINT0 #SRC_CANINT1 and Port.
 * \return	#RC_SUCCESS			CAN was initialized successfully
 * 			#RC_ERROR_TIME_OUT	#CAN__initModule, #CAN__NodeInit, #CAN__MObjInit or panel control command timed out.
 * 			#RC_ERROR_BAD_PARAM	Illegal Node-ID
 * 			#RC_ERROR_BAD_DATA	Illegal configuration in #CANopen_Rx_Mailboxes. Number of objects exceeds #DERIVATE_NUM_CAN_OBJ
 */
RC_t CAN_init(uint8_t NodeID);

/**
 * \brief Read a frame from the CAN module
 *
 * If a pending message is found, it is copied into #rx_msg.
 * See #CAN_RX_t for more information about how to gather information about the receiver.
 * \param 	rx_msg	RX message container, see #CAN_RX_t
 * \param 	mailbox ID (#CANopen_Rx_Mailboxes) with which the frame is associated.
 * \warning	If a new frame is received while reading the buffer, the read procedure is aborted and the new frame is read.
 * \return	#RC_SUCCESS				Message was read
 * 			#RC_ERROR_READ_FAILS	No pending messages in the Module
 * 			#RC_ERROR_INVALID_STATE	#CAN_init was not finished successfully
 */
RC_t CAN_Frame_Read(CAN_RX_t* rx_msg, uint8_t* mbx_id);

/**
 * \brief Write a frame to the CAN queue
 *
 * Sends a CAN message to the TX objects a given node
 * \param 	node	Node, which shall send the message
 * \param 	tx_msg	TX message container, see #CAN_RX_t
 * \return	#RC_SUCCESS				Message was send
 * 			#RC_ERROR_BUFFER_FULL	Output buffer is full
 * 			#RC_ERROR_INVALID_STATE	#CAN_init was not finished successfully
 */
RC_t CAN_Frame_Write(uint8_t node, CAN_Frame_t* tx_msg);

/**
 * \brief Write a frame to the CAN module
 *
 * Sends a CAN message to the TX objects a given node
 * \param 	tx_msg	TX message container, see #CAN_RX_t
 * \warning This function is not intended to be used directly. Use #CAN_Frame_Write instead.
 */
void CAN__frame_write_HW(CAN_Frame_t* tx_msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* CAN_H */
