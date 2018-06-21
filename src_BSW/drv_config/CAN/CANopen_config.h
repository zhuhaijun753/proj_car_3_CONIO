/**
 * \file	CANopen_config.h
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	11.010.2017
 * \version	0.7
 *
 * \brief CANopen configuration header
 *
 *	Changelog: \n
 *		0,7 11.01.17 Barth	- Made #CANopen__SendCANMsg an inline function, defined in this file.
 *      0.6 01.12.16 Barth 	- Modified RX Mailbox configuration. Added acceptance filter to assign mailboxes to CANopen Function-Codes\n\n
 *      0.3 01.06.16 Barth 	- Distinguish between PSoC and AURIX to make this file usable on both platforms.\n\n
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
#ifndef _CANOPEN_CONFIG_H_
#define _CANOPEN_CONFIG_H_

//-------------------------------------------------------------------- [Common Includes]
#include "CANopen.h"
#include "CAN_config.h"

//-------------------------------------------------------------------- [Stack Configuration]

/** \brief NodeID of this CANopen Node
 *
 *  TODO */
#define CANOPEN_CFG_NODE_ID             6

/** \brief #CANopen_Client_Tick interval
 *
 *  This symbol defines the interval in ms in which #CANopen_Client_Tick is called.
 *  This tick interval is used as base for timeout calculations in #CANopen_getsetSDO, where the TTL of a ongoing transfer is decremented by this value
 *  If the TTL is less that the half of #CANOPEN_CFG_TICK_INTERVAL, the transfer immediately times out */
#define CANOPEN_CFG_TICK_INTERVAL	    10

/** \brief Enables (ON) or disables (OFF) the CANopen frame counter.
 *
 * If the frame counter is enabled, the number of transmitted, received , unsuccessful transmitted and unsuccessfully processed frames can
 * be read using #CANopen_getFrameCnt.
 * \note	32 byte of memory will be used if frame counting is enabled
 */
#define CANOPEN_CFG_USE_FRAME_CNT		ON

/** \brief Enables (ON) or disables (OFF)the #CAN__TX_hasError flag.
 *
 * CANopen can set a error flag if a CAN TX error has been encountered. The flag is set in #CANopen__TXBufErrorHdl.
 * The user has to clear the flag (e.g. in a CAN TX ISR) manually.
 * \warning #CANopen_getsetSDO will return #CANOPEN_SDO_TX_ERROR as long as the flag is set.
 */
#define CANOPEN_CFG_USE_TX_BLOCK		OFF

/** \brief Enables (ON) or disables (OFF) LED activity indication.
 *
 * Activity of the Stack can be visualized using the LED activity indication.
 * #CANOPEN_TX_LED_MACRO, #CANOPEN_RX_LED_MACRO, CANOPEN_TX_ER_LED_MACRO or #CANOPEN_RX_ER_LED_MACRO
 * are called with each call of #CANopen_incFrameCnt
 * \note The indication works independently from #CANOPEN_CFG_USE_FRAME_CNT
 */
#define CANOPEN_CFG_USE_CAN_LED			OFF

#define CANOPEN_CFG_OD_VT               uint32_t                     		/**< \brief Object dictionary value type. \warning Must have same or bigger width as a pointer on the target*/
#define CANOPEN_CFG_SDO_U_T			    uint32_t							/**< \brief Data-type of the user argument, that can be passed to #CANopen_getsetSDO and then will be given to the callback.*/
#define CANOPEN_CFG_SDO_DISP_IT         uint16_t							/**< \brief Data-type of the dispatcher index and therefore the iterators*/
#define CANOPEN_CFG_SDO_DISP_SIZE       0xa									/**< \brief Number of elements for the dispatcher. Must not exceed range of #CANOPEN_CFG_SDO_DISP_IT !*/
#define CANOPEN_CFG_SDO_MIN_DLENGTH     0x1									/**< \brief Minimum data-length for SDO set/get*/
#define CANOPEN_CFG_TRANSFER_M_THRES    0x14								/**< \brief Number of bytes after which the block-transfer will be used instead of the segmented/normal transfer*/
#define CANOPEN_CFG_SDO_BLK_SZ          0x20								/**< \brief Number of blocks the client will receive in a block transfer before it sends an acknowledge*/

/** \brief Block transfer PST
 *
 * If the size of the data in bytes is less or equal "PST" the server may switch to the normal SDO	upload protocol by 	transmitting the server response of the normal/segmented SDO upload.
 * If the value is 0, the server can't switch the protocol.*/
#define CANOPEN_CFG_SDO_BLK_PST		0x0
    
//-------------------------------------------------------------------- [Config check]
#if(CANOPEN_CFG_NODE_ID<CANOPEN_NODE_ID_MIN||node_id>CANOPEN_NODE_ID_MAX)
    #error "Invalid Node ID"
#endif
//-------------------------------------------------------------------- [LED activity indicator configuration]
#if(CANOPEN_CFG_USE_CAN_LED==ON)

    /** \brief Macro for CANopen TX LED events*/
    #define CANOPEN_TX_LED_MACRO        LED_Trigger_Write(LED_Trigger_Read()^CAN_LED_TX_ID);

    /** \brief Macro for CANopen RX LED events*/
    #define CANOPEN_RX_LED_MACRO        LED_Trigger_Write(LED_Trigger_Read()^CAN_LED_RX_ID);

    /** \brief Macro for CANopen TX ERROR LED events*/
    #define CANOPEN_TX_ER_LED_MACRO     LED_Trigger_Write(LED_Trigger_Read()^CAN_LED_ERROR_ID);

    /** \brief Macro for CANopen RX ERROR LED events*/
    #define CANOPEN_RX_ER_LED_MACRO     LED_Trigger_Write(LED_Trigger_Read()^CAN_LED_ERROR_ID);

    #define CAN_LED_TX_ID         		BIT(0)                             	/**< \brief A Hardware Module with 3 outputs is used to signal messages with LEDs, this number indicates the module output for the TX event*/
    #define CAN_LED_RX_ID               BIT(1)                             	/**< \brief A Hardware Module with 3 outputs is used to signal messages with LEDs, this number indicates the module output for the RX event*/
    #define CAN_LED_ERROR_ID            BIT(2)                             	/**< \brief A Hardware Module with 3 outputs is used to signal messages with LEDs, this number indicates the module output for the ERROR event*/

#else //(CANOPEN_CFG_USE_CAN_LED==ON)
    
    /** \brief Empty Macro for CANopen TX LED events*/
    #define CANOPEN_TX_LED_MACRO
    
    /** \brief Empty Macro for CANopen RX LED events*/
    #define CANOPEN_RX_LED_MACRO
    
    /** \brief Empty Macro for CANopen TX ERROR LED events*/
    #define CANOPEN_TX_ER_LED_MACRO
    
    /** \brief Empty Macro for CANopen RX ERROR LED events*/
    #define CANOPEN_RX_ER_LED_MACRO
#endif

//-------------------------------------------------------------------- [Common Types]

/** \brief Mailbox Flags */
typedef enum{
	CANopen_range_fct          =0,                                			/**< \brief Accept all node-IDs for a function code*/
	CANopen_range_node         =BIT(0), 		                            /**< \brief Accept only messages with matching function code and to own node-ID*/
	CANopen_range_explicit     =BIT(1),  		                            /**< \brief Accept only messages with given ID*/
}CANopen_Mbx_range_t;

//-------------------------------------------------------------------- [function reference]
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/**
 * \brief Processes SDO frames
 *
 * This function interprets SDO messages and is communicating with the OD
 * \param   CANOPEN_RX_CALLBACK_PARAMS See #CANOPEN_RX_CALLBACK_PARAMS
 * \return  #RC_SUCCESS                 frame was handled
 *          #RC_ERROR_INVALID_STATE     SDO service is not active.
 *          #RC_ERROR_BAD_PARAM         Invalid parameter
 *          #RC_ERROR_BAD_DATA          frame couldn't be handled
 */
extern RC_t CANopen_SDO_Hdl(CANOPEN_RX_CALLBACK_PARAMS);

/**
 * \brief Handles SDO response messages from a CANopen Server
 */
extern RC_t CANopen_SDO_resp_hdl(CANOPEN_RX_CALLBACK_PARAMS);

/**
 * \brief Handles PDO messages from a CANopen Server
 */
extern RC_t	CANopen_PDO_hdl(CANOPEN_RX_CALLBACK_PARAMS);

/**
 * \brief Handler for NMT messages
 *
 * Is receiving NMT messages and is checking if this node is addressed and if the message has the correct format.
 * If the message is addressed to this node and has the correct format, the command is forwarded to the NMT state-machine
 * \param   see #CANOPEN_RX_CALLBACK_PARAMS
 * \return  #RC_SUCCESS         The result of #CANopen__NMTExec is forwarded if this node was addressed and the format is correct.
 *          #RC_ERROR_BAD_PARAM Incorrect format
 */
extern RC_t CANopen_NMTHdl(CANOPEN_RX_CALLBACK_PARAMS);

/**
 * \brief Handler for Node Guard RTR requests
 *
 * The node guarding object can be used to query the momentary state of the device. To do this, by
 * setting a remote frame, the master sends a request (request message frame) on the guarding identifier of the node to be monitored. 
 * This then replies with the guarding message, which contains the current status of the node and a toggle bit.
 * \param       see #CANOPEN_RX_CALLBACK_PARAMS
 * \return      result of #CANopen__SendCANMsg
 * \warning     This handler only have to responce on RTR frames. Not on regular frames (Heartbeat/Bootup)
 */
extern RC_t CANopen_NG_Hdl(CANOPEN_RX_CALLBACK_PARAMS);

/**
 * \brief Is called if a message could not be passed to the CAN driver
 *
 * \return  shall be #RC_SUCCESS but can be defined by the user
 */
extern RC_t CANopen__TXBufErrorHdl();

/**
 * \brief Increments a frame counter
 */
extern RC_t CANopen_incFrameCnt(CANopen_Frame_cnt_t ctr);

/**
 * \brief Catches CAN frames, which could not be processed by the assignes handler.
 */
extern RC_t CANopen__CatchHdl(CANOPEN_RX_CALLBACK_PARAMS);

#ifdef __cplusplus
}
#endif /* __cplusplus */
    
#endif //_CANOPEN_CONFIG_H_
/* [] END OF FILE */
