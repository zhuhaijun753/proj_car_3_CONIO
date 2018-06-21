/**
 * \file	CANopen_Client.h
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	11.01.2017
 * \version	0.7
 *
 * \brief Functions to read and write Objects at other CANopen Nodes via SDO
 *	
 * The CAN_da client is used to read and write Objects of remote CANopen-Nodes via SDO. 
 * It supports multiple modes of operation:                               
 *      - Blocking: Wait for the remote node to respond                             
 *      - Callback: A callback function, specified by the user is called when there
 *                  is a response or in case an error occurred.
 *      - FaF:      "Fire and Forget" can be used when writing objects via SDO      
 *                  the client will not wait for an response in this case
 * 																					
 *  The CAN_da client is equipped with an timeout mechanism, which will make sure that read/write accesses are done in time.
 *  In order to use this mechanism, the function #CANopen_Client_Tick needs to be called periodically.
 *                                                                                  
 *  User defined Callback functions need to be in the format:                       
 *  RC <name>(CANopen__SDO_scheduler_element_t*)
 *  Inside of #CANopen_SDO_dispatcher_element_t the user can find all the information about the transfer.
 *  Only if the user returns a #RC_SUCCESS, the transfer is finished, if any other value is returned, the transfer remains in the dispatcher.           
 * 																					
 * 	Changelog\n
 *      0.6 11.01.17 Barth	- moved #CANopen_Client_Tick to Client
                            - added CANOPEN_TICK_INTERVAL
 * 		0.6 12.12.16 Barth	- return if CAN TX failed
 *                          - #CANopen_getSDO and #CANopen_setSDO automatically take the address and length of a given variable.
 *		0.5 22.09.16 Barth	- In any other mode than "fire and forget", the dispatcher entry is deleted if the CAN-Driver is not accepting the message. The result was checked before the message was passed to the driver. fixed...														
 * 		0.4	16.06.16 Barth	- added NMT request to the client \n
 *							- Implemented user-argument for getsetSDO \n\n
 *
 * 		0.3 20.05.16 Barth  - added "listen only" mode for all read operations to a specific node/object on the bus \n
 * 							- improved dispatcher list logic \n
 * 							- Blocking/Spinlock transfer mode is using flag in the Dispatcher Entry instead of global variable.	This way several Tasks can use the blocking transfer.\n
 *                      	- Distinguish between PSoC and AURIX to make this file usable on both platforms.\n\n
 *
 *      0.2 16.05.16 Barth  - changed segmented SDO length to cover bytes 4 to 7\n
 * 							- added proprietary block transfer mode\n\n
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

#ifndef _CANOPEN_CLIENT_H
#define _CANOPEN_CLIENT_H

//-------------------------------------------------------------------- [Header]
#include "CANopen.h"
#include "CAN_config.h"

//-------------------------------------------------------------------- [Defines]
    
/** \brief Macro for #CANopen__getsetSDO for uploads
  *
  * #CANopen_getsetSDO requires some arguments, which are rarely used and which can confuse the user.
  * #CANopen_getSDO provides an easy to use interface for CANopen SDO read operations.
  * \param  cb      Callback function passed to CANopen_getsetSDO#callback
  * \param  value   Variable to be transmitted. The address is of the variable is passed to CANopen_getsetSDO#value, the #sizeof is passed to CANopen_getsetSDO#value_length.
  */
#define CANopen_getSDO( ...) CANopen_getsetSDO(__VA_ARGS__, CANOPEN_SDO_DIR_UP)

/** \brief Same as #CANopen_getSDO but address and sizeof the variable are taken automatically*/
#define CANopen_getSDO_lvar(cb, val, ...) CANopen_getsetSDO(cb, &val, sizeof(val), __VA_ARGS__, CANOPEN_SDO_DIR_UP)
    
/** \brief Macro for #CANopen__getsetSDO for downloads
  *
  * #CANopen_getsetSDO requires some arguments, which are rarely used and which can confuse the user.
  * #CANopen_setSDO provides an easy to use interface for CANopen SDO write operations.
  * \param  cb      Callback function passed to CANopen_getsetSDO#callback
  * \param  value   Variable to be transmitted. The address is of the variable is passed to CANopen_getsetSDO#value, the #sizeof is passed to CANopen_getsetSDO#value_length.
  */
#define CANopen_setSDO( ...) CANopen_getsetSDO(__VA_ARGS__, CANOPEN_SDO_DIR_DN)

/** \brief Same as #CANopen_setSDO but address and sizeof the variable are taken automatically*/
#define CANopen_setSDO_lvar(cb, val, ...) CANopen_getsetSDO(cb, &val, sizeof(val),__VA_ARGS__, CANOPEN_SDO_DIR_DN)
    
// SDO Dispatcher Flags
#define CANOPEN_DISP_FLAGS_N         	0x0         							/**< \brief no flags set*/
#define CANOPEN_DISP_FLAGS_R         	BIT(0)        							/**< \brief SDO transfer has ended */
#define CANOPEN_DISP_FLAGS_T         	BIT(1)         							/**< \brief SDO transfer has timed out */
#define CANOPEN_DISP_FLAGS_A         	BIT(2)         							/**< \brief SDO transfer was aborted */
#define CANOPEN_DISP_FLAGS_O         	BIT(3)         							/**< \brief Overflow, response was bigger than value-container */
#define CANOPEN_DISP_FLAGS_TG        	BIT(4)        							/**< \brief Toggle bit for segmented transfer, not to be used by user*/
#define CANOPEN_DISP_FLAGS_TG_SHIFT  	BIT(4)         							/**< \brief bit position of the toggle flag*/
#define CANOPEN_DISP_FLAGS_E         	BIT(5)        							/**< \brief SDO transfer failed because of an internal error */
#define CANOPEN_DISP_FLAGS_B         	BIT(6)        							/**< \brief SDO transfer is using block-mode */
 
 /** \brief Paramters passed to the handler functions specified by the user in #CANopen_getSDO and #CANopen_setSDO*/   
#define CANOPEN_SDO_CALLBACK_PARAMS     volatile CANopen_SDO_dispatcher_element_t const * const SDO_info

/** \brief parameters for getsetSDO*/
#define CANOPEN_GETSETSDO_PARAMS        RC_t (*callback) (CANOPEN_SDO_CALLBACK_PARAMS),void* value, uint32_t value_length, uint8_t nodeId, uint16_t Index, uint8_t subindex, uint32_t timeout, CANOPEN_CFG_SDO_U_T usr_arg, CANopen__SDO_transfer_dir_t direction

//-------------------------------------------------------------------- [types]
/** \brief Return type for #CANopen_getSDO and #CANopen_setSDO */
typedef enum{
    CANOPEN_SDO_SUCCESS                 =0x0,   								/**< \brief SDO get/set succeeded */
    CANOPEN_SDO_ILLEGAL_NODE_ID         =0x1,   								/**< \brief Node ID violated #CANOPEN_NODE_ID_MIN or #CANOPEN_NODE_ID_MIN */
    CANOPEN_SDO_ILLEGAL_VALUE_LENGTH    =0x2,   								/**< \brief Specified value length violated #CANOPEN_SDO_MIN_DLENGTH */
    CANOPEN_SDO_TIMEOUT                 =0x3,   								/**< \brief SDO transfer timed out */
    CANOPEN_SDO_ABORT                   =0x4,   								/**< \brief SDO was aborted */
    CANOPEN_SDO_DISPATCHER_FULL         =0x5,   								/**< \brief Can not schedule SDO transfer, Scheduler is full */
    CANOPEN_SDO_ILLEGAL_TIMEOUT         =0x6,   								/**< \brief Specified Timeout is illegal (must not be0) */
    CANOPEN_SDO_VALUE_SIZE              =0x7,   								/**< \brief The SDO Size reported by the server, exceeds the size indicated by the user */
    CANOPEN_SDO_INTERNAL_ERROR          =0x8,   								/**< \brief SDO transfer failed because of an internal error */
    CANOPEN_SDO_CALLBACK_MISSING        =0x9,   								/**< \brief The selected transfer mode requires a callback-function */
    CANOPEN_SDO_IMPLEMENTATION_MISSING  =0xa,   								/**< \brief The feature the user wants to use is currently not implemented (e.g. downloading more than 4 bytes) */
    CANOPEN_SDO_PARAM_ERROR             =0xb,    								/**< \brief The given parameters are not plausible. */
	CANOPEN_SDO_TX_ERROR				=0xc,									/**< \brief Could not send CAN frame. */
	CANOPEN_SDO_UNINITALIZED			=0xd,									/**< \brief Stack uninitialized, call CANopen_Init first. */
}CANopen_SDO_transfer_RET_t;      

/** \brief Indicator if the scheduler element is a up- or download */
typedef enum{
    CANOPEN_SDO_DIR_UP  =0x1,           										/**< \brief Message is a upload (getSDO)*/
    CANOPEN_SDO_DIR_DN  =0x2            										/**< \brief Message is a download  (setSDO)*/
}CANopen__SDO_transfer_dir_t;

/** \brief Defines the mode of a SDO transfer */
typedef enum{
    CANOPEN_SDO_mode_expedited,     											/**< \brief Message will be transmitted in expedited mode*/
    CANOPEN_SDO_mode_normal,        											/**< \brief Message will be transmitted in normal/segmented mode*/
    CANOPEN_SDO_mode_block,         											/**< \brief Message will be transmitted in block-transfer mode*/
    CANOPEN_SDO_mode_fireandforget,      										/**< \brief Message will be transmitted in "fire and forget" mode. Only applicable for setSDO with length <=4byte */
    CANOPEN_SDO_mode_listen_only         										/**< \brief The callback-handler will be called whenever a message with content is found on the bus*/
}CANopen__SDO_transfer_mode_t;


/** \brief Datatype of the structure that is given to the user-handler given to #CANopen_getSDO and #CANopen_setSDO */
typedef struct CANopen_SDO_dispatcher_element_t CANopen_SDO_dispatcher_element_t;
struct CANopen_SDO_dispatcher_element_t{
    CANopen__SDO_transfer_mode_t    transfer_mode;                              /**< \brief The mode with which the message is transmitted */
    CANopen__SDO_transfer_dir_t     direction;                                  /**< \brief Direction of the transfer */
    uint16_t                        COB_ID;                                     /**< \brief awaited COB-ID */
    uint8_t                         node;                                       /**< \brief awaited Node-ID */
    uint16_t                        index;                                      /**< \brief awaited Index */
    uint8_t                         subIndex;                                   /**< \brief awaited SubIndex */
    void*                           value;                                      /**< \brief Pointer to the data-container (has to be provided by the user) */
    uint32_t                        length;                                     /**< \brief length of data in bytes */
    uint32_t                        transmitted;                                /**< \brief number of bytes that already have been transmitted */
    RC_t                            (*callback) (CANOPEN_SDO_CALLBACK_PARAMS);  /**< \brief callback function that is called if the message is received*/
    uint8_t                         flags;                                      /**< \brief Flags, see: #CANOPEN_DISP_FLAGS_R #CANOPEN_DISP_FLAGS_T #CANOPEN_DISP_FLAGS_A #CANOPEN_DISP_FLAGS_O #CANOPEN_DISP_FLAGS_E */
    uint32_t                        abort_code;                                 /**< \brief in case of an SDO abort, the abort code will be stored here */
    uint32_t                        TTL;                                        /**< \brief Time to live in ms. */
    CANOPEN_CFG_SDO_U_T		        usr_arg;									/**< \brief Argument passed to CANopen_getsetSDO  */
};

//-------------------------------------------------------------------- [Prototypes]
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/**
 * \brief Gets (upload) or sets (download) a value to a CANopen-Node
 *
 * This function is the interface between the user code and other CANopen-Nodes.
 * It allows to receive and send values to other CANopen-Nodes. The value that has to be read/written
 * is specified with Index and SubIndex. Please check the manual of the manufacturer if you want to 
 * know which value is stored at which Index/Sub-index.
 * This functions supports expedited and normal (segmented) data transfer. When data bigger than 4 bytes has to
 * read or written, normal mode is used, otherwise expedited.
 *
 * This function supports two modes: blocking and callback.
 * 	-	In blocking mode, an internal spinlock is used to wait for the server to reply.
 *  -	In callback mode, the function returns and the given callback is called once a reply was received or an error was detected.
 *
 * In order to use callback mode, a callback function has to be passed to the function, otherwise blocking mode becomes active.
 * The user has to specify a timeout in milliseconds. If the transfer has not finished in this time, the spinlock will be released
 * or the callback function will be called, depending on the current mode.
 *
 * \param   callback        Callback function to be called in callback mode, required arguments of the callback function are defined in #CANOPEN_SDO_CALLBACK_PARAMS
 * \param   value           Pointer to a value container that stores the value to be written or to be read
 * \param   value_length    Length of the value container in bytes
 * \param   nodeId          NodeID which will be addressed
 * \param   Index           Index which will be addressed
 * \param   subindex        SubIndex which will be addressed
 * \param   timeout         Timeout in milliseconds. If the transfer did not finish in this time period, the handler will be called or the spinlock will be released, depending on the mode.
 * \param   usr_arg       	Argument that will be passed to the dispatcher and will be later on passed to the callback-function as it is part of #CANopen_SDO_dispatcher_element_t.
 * \param   direction       Indicates if this is a read or write information (#CANopen__SDO_transfer_dir_t), the user might want to use #CANopen_getSDO and #CANopen_setSDO.
 * \return  In blocking mode the return value also stores information about timeout etc, please check #CANopen_SDO_transfer_RET_t
 */
CANopen_SDO_transfer_RET_t CANopen_getsetSDO(CANOPEN_GETSETSDO_PARAMS);

/**
 * \brief Sends a NMT command to a remote node
 *
 * The NMT state in CANopen Nodes defines which services (like PDO) are available. This function sends NMT commands to a remote node
 * \param   command         Command to be send to the remote node, see #CANopen_NMT_commands_t
 * \param   node           	Node ID that shall receive this command, set 0 to send a broadcast to all nodes on the bus.
 * \return  result of #CANopen__SendCANMsg
 */
RC_t CANopen_sendNMTcmd(CANopen_NMT_commands_t command, uint8_t node);

/**
 * \brief Trigger the internal timing mechanisms.
 *
 * The CANopen Stack needs a periodic Tick to allow timeout mechanisms to work.
 * All entries in the SDO Dispatcher have a TTL (time to live). With this trigger, the TTL of all entries is decremented by #CANOPEN_TICK_INTERVAL.
 * If a TTL reaches 0, the given handler is called and the entry is deleted if this handler is returning RC_SUCCESS.
 * \note  #CANOPEN_TICK_INTERVAL need to be set in order to allow correct TTL calculation
 */
void CANopen_Client_Tick(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CANOPEN_CLIENT_H */
/* [] END OF FILE */
