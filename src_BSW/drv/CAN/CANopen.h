/**
 * \file	CANopen.h
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	11.01.2017
 * \version	0.7
 *
 * \brief CANopen Core with basic protocol definitions and global functions
 *
 * Changelog:\n
 *      0.7 11.01.17 Barth
 *              - renamed CANopen_CAN_MSG_t in #CANopen_CAN_MSG_t
 *              - added union for U8 and U32 access to CANopen_CAN_MSG_t#data
 *              - the node-ID is no longer passed to the init function but is a symol #CANOPEN_CFG_NODE_ID in #CANopen_config.h
 *      0.6 01.12.16 Barth  
 *              - moved #CANopen_Client_Tick to Client
 *              - Improved dataflow between stack and OD
 *              - Added PSoC acceptance filter mechanism to allow assignment of RX mailboxes to CANopen function-codes
 *      0.5 20.09.16 Barth  - Improved CAN-Frame passing to handler functions \n
 *      0.4 16.06.16 Barth  - Added NMT requests to server.\n
 *			                - Implemented user-argument for getsetSDO\n\n
 *
 *      0.3 01.06.16 Barth  - Distinguish between PSoC and AURIX to make this file usable on both platforms.\n\n
 *
 *      0.2 18.05.16 Barth  - Added a spinlock around the CAN_Send on PSoCs \n\n
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

#ifndef _CANOPEN_H
#define _CANOPEN_H

//-------------------------------------------------------------------- [Header]
#include "global.h"

//-------------------------------------------------------------------- [Defines]
    
#define CANOPEN_NODE_ID_MIN             0x1                             /**< \brief   The lowest possible Node-ID*/
#define CANOPEN_NODE_ID_MAX             0x7f                            /**< \brief   The highest possible Node-ID*/

#define CANOPEN_TX_PARAMS               CANopen_CAN_MSG_t* CAN_out      /**< \brief   Parameters for TX Function*/
#define CANOPEN_RX_CALLBACK_PARAMS      CANopen_CAN_MSG_t* CAN_in       /**< \brief   Parameters for RX Callback functions*/

#define CANOPEN_NMT_DLC                 0x2                             /**< \brief   DLC for a NMT message*/
#define CANOPEN_NMT_BROADCAST           0x0                             /**< \brief   Broadcast NodeID for NMT*/
#define CANOPEN_NMT_FCT_CODE			0x0					            /**< \brief	  Function code for NMT messages*/
#define CANOPEN_NMT_NODE_ID				0x0					            /**< \brief	  Node Id that is handling NMT messages*/
#define CANOPEN_NMT_BYTE_FCT            0x0                             /**< \brief   Position of the function-byte in the NMT CAN frame*/
#define CANOPEN_NMT_BYTE_NODE           0x1                             /**< \brief   Position of the NodeID in the NMT CAN frame*/

#define CANOPEN_SDO_DLC                 0x8                             /**< \brief   SDO Message DLC(length)*/
#define CANOPEN_SDO_BYTE_FCT            0x0                             /**< \brief   CAN Byte that has the function-code*/
#define CANOPEN_FCT_MSK                 0x780                           /**< \brief   Masks CANopen Function code */
#define CANOPEN_NODE_ID_MSK				0x7F							/**< \brief   Masks CANopen Node ID */

#define CANOPEN_TIME_DLC                0x8                             /**< \brief DLC for the TIME message*/

#define CANOPEN_BOOTUP_DLC              0x1                             /**< \brief DLC for the Bootup Message*/
#define CANOPEN_BOOTUP_BYTE             0x0                             /**< \brief Byte-ID of the Bootup Message*/
#define CANOPEN_BOOTUP_MSG              0x0                             /**< \brief Content for the Bootup Message*/
    
#define CANOPEN_NODEGUARD_DLC           0x1                             /**< \brief DLC for Node guard replies*/
#define CANOPEN_NODEGUARD_BYTE          0x0                             /**< \brief Data Byte position for node guard replies*/
#define CANOPEN_NODEGUARD_DATA          0x7f                            /**< \brief Data Byte for node guard replies*/
#define CANOPEN_NODEGUARD_TB_OFF        0x7                             /**< \brief Offset to toggle bit in node guard replies*/
    
#define CANOPEN_SYNC_DLC_NOINC          0x0                             /**< \brief DLC for the SYNC message without incremented value*/
#define CANOPEN_SYNC_CP_SHIFT           0x1e                            /**< \brief defines the bit-position where to find the CONSUMER/PRODUCER bit in the COB-ID*/
#define CANOPEN_SYNC_CP_MSK             BIT(CANOPEN_SYNC_CP_SHIFT)      /**< \brief mask for detection of the SYNC C/P bit*/

#define CANOPEN_SDO_FCT_DS_US           0x0                             /**< \brief   Client->Server: Download segment request  | Server->Client: Upload segment response*/
#define CANOPEN_SDO_FCT_DR_DS           0x1                             /**< \brief   Client->Server: Initiate download request | Server->Client: Download segment response*/
#define CANOPEN_SDO_FCT_IU_IU           0x2                             /**< \brief   Client->Server: Initiate upload request   | Server->Client: Initiate upload response*/
#define CANOPEN_SDO_FCT_US_ID           0x3                             /**< \brief   Client->Server: Upload segment request    | Server->Client: Initiate download response*/
#define CANOPEN_SDO_FCT_AB              0x4                             /**< \brief   abort command*/
#define CANOPEN_SDO_FCT_BU_BD           0x5                             /**< \brief   Client->Server: block upload request      | Server->Client: block download response*/
#define CANOPEN_SDO_FCT_BD_BU           0x6                             /**< \brief   Client->Server: block download request    | Server->Client: block upload response*/
    
#define CANOPEN_SDO_BYTE_BLK_S          0x4                             /**< \brief   The Block size in SDO requests is found at this byte*/
#define CANOPEN_SDO_BYTE_PST            0x5                             /**< \brief   The Protocol switch threshold for block SDO requests is found at this byte*/
#define CANOPEN_SDO_BYTE_NB0            0x4                             /**< \brief   Byte Number of n for the data length to be transmitted in normal (segmented) transfer*/
#define CANOPEN_SDO_BYTE_NB0_SHFT       0x0                             /**< \brief   Shift of n byte 0*/   
#define CANOPEN_SDO_BYTE_NB1            0x5                             /**< \brief   Byte Number of n for the data length to be transmitted in normal (segmented) transfer*/
#define CANOPEN_SDO_BYTE_NB1_SHFT       0x1*BYTE_L                      /**< \brief   Shift of n byte 1*/  
#define CANOPEN_SDO_BYTE_NB2            0x6                             /**< \brief   Byte Number of n for the data length to be transmitted in normal (segmented) transfer*/
#define CANOPEN_SDO_BYTE_NB2_SHFT       0x2*BYTE_L                      /**< \brief   Shift of n byte 2*/ 
#define CANOPEN_SDO_BYTE_NB3            0x7                             /**< \brief   Byte Number of n for the data length to be transmitted in normal (segmented) transfer*/
#define CANOPEN_SDO_BYTE_NB3_SHFT       0x3*BYTE_L                      /**< \brief   Shift of n byte 3*/ 
#define CANOPEN_SDO_BYTE_FCT_CMD_MSK    0xe0                            /**< \brief   Masks the Command specifier [7..5] of the function code*/
#define CANOPEN_SDO_BYTE_FCT_CMD_SHIFT  0x5                             /**< \brief   Shift for the the Command specifier [7..5] of the function code*/

#define CANOPEN_SDO_BLK_CS_MSK          0x3                             /**< \brief   Masks the Client sub-command in a block response from the client*/
#define CANOPEN_SDO_BLK_S               0x1                             /**< \brief   Block transfer size indicator*/ 
#define CANOPEN_SDO_BLK_S_SHIFT         0x1                             /**< \brief   Shift to block transfer size indicator*/ 
#define CANOPEN_SDO_BLK_SS_MSK          0x1                             /**< \brief   Block transfer server sub-command mask*/
#define CANOPEN_SDO_BLK_C               0x1                             /**< \brief   no more segments to be uploaded*/ 
#define CANOPEN_SDO_BLK_C_SHIFT         0x7                             /**< \brief   Shift to "no more segments to be uploaded"*/ 
#define CANOPEN_SDO_BLK_DATA_S_OFF      0x1                             /**< \brief   Offset to the first data byte in a block transfer*/ 
#define CANOPEN_SDO_BLK_DATA_PF         0x7                             /**< \brief   Number of data-bytes per block-segment frame*/ 
#define CANOPEN_SDO_BLK_SEGNO_MIN       0x1                             /**< \brief   smallest segment number for block transfer*/
#define CANOPEN_SDO_BLK_SEGNO_MAX       0x7f                            /**< \brief   biggest segment number for block transfer*/
#define CANOPEN_SDO_BLK_CS_BU           0x2                             /**< \brief   Client sub-command block upload response*/
#define CANOPEN_SDO_BLK_CS_SU           0x3                             /**< \brief   Client sub-command start upload*/

/** \brief   CLient Block ACKSEG byte
 *
 * sequence number of last segment that was received successfully during the last block upload. If ACKSEG is set to 0 the client indicates the server
 * that the segment with the sequence number 1 was not received correctly and all segments shall be retransmitted by the server.
 */
#define CANOPEN_SDO_BLK_BYTE_ACKSEG     0x1

/** \brief   Client Block BLKSIZE byte
 *
 * Number of segments per block that shall be used by server for the following block upload with 0 < BLKSIZE < 128.
 */
#define CANOPEN_SDO_BLK_BYTE_BLKSIZE    0x2

 /** \brief   Upload segment response Command shifted*/
#define CANOPEN_SDO_FCT_DS_US_SHIFTED   (CANOPEN_SDO_FCT_DS_US<<CANOPEN_SDO_BYTE_FCT_CMD_SHIFT)

/** \brief   Download Request Command shifted*/
#define CANOPEN_SDO_FCT_DR_DS_SHIFTED   (CANOPEN_SDO_FCT_DR_DS<<CANOPEN_SDO_BYTE_FCT_CMD_SHIFT)

/** \brief   Upload Command (Request and response) shifted*/
#define CANOPEN_SDO_FCT_IU_IU_SHIFTED   (CANOPEN_SDO_FCT_IU_IU<<CANOPEN_SDO_BYTE_FCT_CMD_SHIFT)

/** \brief   Download response Command shifted*/
#define CANOPEN_SDO_FCT_US_ID_SHIFTED   (CANOPEN_SDO_FCT_US_ID<<CANOPEN_SDO_BYTE_FCT_CMD_SHIFT)

/** \brief   Abort Command shifted*/
#define CANOPEN_SDO_FCT_AB_SHIFTED      (CANOPEN_SDO_FCT_AB<<CANOPEN_SDO_BYTE_FCT_CMD_SHIFT)

/** \brief   Client Block upload Command shifted*/
#define CANOPEN_SDO_FCT_BU_BD_SHIFTED   (CANOPEN_SDO_FCT_BU_BD<<CANOPEN_SDO_BYTE_FCT_CMD_SHIFT)

/** \brief   Server Block upload Command shifted*/
#define CANOPEN_SDO_FCT_BD_BU_SHIFTED   (CANOPEN_SDO_FCT_BD_BU<<CANOPEN_SDO_BYTE_FCT_CMD_SHIFT)

/** \brief   Block transfer size indicator shifted*/
#define CANOPEN_SDO_BLK_S_SHIFTED       (CANOPEN_SDO_BLK_S<<CANOPEN_SDO_BLK_S_SHIFT)


/** \brief  "no more segments" indicator shifted*/
#define CANOPEN_SDO_BLK_C_SHIFTED       (CANOPEN_SDO_BLK_C<<CANOPEN_SDO_BLK_C_SHIFT)
    
#define CANOPEN_SDO_BYTE_FCT_N_SHIFT    0x2                             /**< \brief Shift to n (Bytes with no data) [3..2] of the function code*/
#define CANOPEN_SDO_BYTE_FCT_N_MASK     0xc                             /**< \brief Masks n (Bytes with no data) [3..2] of the function code*/
#define CANOPEN_SDO_BYTE_FCT_E_MSK      0x2                             /**< \brief Masks the transfer Type [1] of the function code*/
#define CANOPEN_SDO_BYTE_FCT_S_MSK      0x1                             /**< \brief Masks the Size Indicator [0] of the function code*/
#define CANOPEN_SDO_BYTE_FCT_C_MASK    	0x1                             /**< \brief Masks the end of segmented transfer bit [0] of the function code*/
#define CANOPEN_SDO_BYTE_FCT_T_SHIFT    0x4                             /**< \brief Position of the toggle bit [4] of the function code*/

/** \brief   Masks the toggle bit [4] of the function code*/
#define CANOPEN_SDO_BYTE_FCT_T_MSK      BIT(CANOPEN_SDO_BYTE_FCT_T_SHIFT)

#define CANOPEN_SDO_BYTE_FCT_SN_SHIFT   0x1                             /**< \brief segmented number of bytes in seg-data shift*/
#define CANOPEN_SDO_BYTE_FCT_SN_MSK     0x6                             /**< \brief	segmented number of bytes in seg-data */

#define CANOPEN_SDO_BYTE_IH             0x2                             /**< \brief CAN Byte that has the high-byte of the index*/
#define CANOPEN_SDO_BYTE_IL             0x1                             /**< \brief CAN Byte that has the low-byte of the index*/
#define CANOPEN_SDO_BYTE_S              0x3                             /**< \brief CAN Byte that has the sub-index*/
#define CANOPEN_SDO_BYTE_M              0x4                             /**< \brief CAN Byte that represents the multiplicator (m) in normal transfer mode*/
#define CANOPEN_SDO_FCT_AB_LENGTH       0x4                             /**< \brief SDO Error message length*/

#define CANOPEN_SDO_BYTE_DATA_START     0x04                            /**< \brief CAN Byte where the data starts*/
#define CANOPEN_SDO_BYTE_DATA_END       0x07                            /**< \brief CAN Byte where the data ends*/
#define CANOPEN_SDO_DATA_INVALID        0x00                            /**< \brief CAN Byte for unused bytes*/

#define CANOPEN_SDO_SEQ_TG_DEF          FALSE                           /**< \brief Default state of the toggle bit for segmented transfer*/
#define CANOPEN_SDO_SEQ_ML              0x7                             /**< \brief Maximum data-length of a segmented SDO transfer*/
#define CANOPEN_SDO_SEQ_DS              0x1                             /**< \brief Byte at which the data of a segmented transfer starts*/

#define CANOPEN_SDO_DATA_MLENGTH        0x4                             /**< \brief Maximum Length of data in expedited SDO packet*/
#define CANOPEN_SDO_AB_DLC              0x4                             /**< \brief Length of abort message*/

#define CANOPEN_SDO_SEG_BYTE_F          0x1                             /**< \brief  First data byte in segmented SDO transfer*/
#define CANOPEN_SDO_SEG_BYTE_L          0x7                             /**< \brief  Data-length in segmented SDO transfer*/
    
//-------------------------------------------------------------------- [types]

/**
 * \brief NMT Commands.
 *
 * Collection of the available NMT commands, the possible transitions(see CANopen standard) are parenthesized.
 */
typedef enum{
    CANopen_NMT_CMD_INIT_DONE           = 0x0,                          /**< \brief  0x00, Network command: Initialization done, enter PreOP (2) */
    CANopen_NMT_CMD_START_NODE          = 0x1,                          /**< \brief  0x01, Network command: Try to set NMT-FSM to Operational (3,6) */
    CANopen_NMT_CMD_STOP_NODE           = 0x2,                          /**< \brief  0x02, Network command: Try to set NMT-FSM to Stopped (5,8) */
    CANopen_NMT_CMD_ENTER_PREOP         = 0x80,                         /**< \brief  0x80, Network command: Try to set NMT-FSM to Pre-Operational (4,7) */
    CANopen_NMT_CMD_RESET_NODE          = 0x81,                         /**< \brief  0x81, Network command: Reset the Node ((9,10,11) */
    CANopen_NMT_CMD_RESET_COM           = 0x82,                         /**< \brief  0x82, Network command: Reset the Communication Stack (12,13,14) */
}CANopen_NMT_commands_t;

/**
 * \brief NMT States.
 *
 * The different states of the NMT state-machine.
 * The state defines the available services.
 *
 *   Service PreOperational  Operational Stopped
 *   | PDO   | no            | yes       | no
 *   | SDO   | yes           | yes       | no
 *   | SYNC  | yes           | yes       | no
 *   | TIME  | yes           | yes       | no
 *   | EMCY  | yes           | yes       | no
 *   | NMT   | yes           | yes       | yes
 *   | ERROR | yes           | yes       | yes
 */
typedef enum{
    CANopen_NMT_STATE_Initialization      = 0x0,                        /**< \brief 0 old 0x0, Network management: Initialization state after POR */
    CANopen_NMT_STATE_Stopped             = 0x4,                         /**< \brief 4 old 0x3, Network management: Stopped, only NMT and Error-control available */
    CANopen_NMT_STATE_Operational         = 0x5,                        /**< \brief 5  old 0x2, Network management: Operational, SDO and PDO available */
    CANopen_NMT_STATE_PreOperational      = 0x7F                       /**< \brief 7F old 0x1, Network management: Initialization done. SDO available */

}CANopen_NMT_states_t;

/**
 * \brief Default CANopen identifiers.
 *
 * Default CANopen identifiers for CANopen communication objects.
 * Note that the NodeID needs to be added for certain identifiers.
 */
typedef enum{
    CANopen_default_CANID_NMT_SERVICE       = 0x000,                    /**< \brief  0x000, Network management */
    CANopen_default_CANID_SYNC              = 0x080,                    /**< \brief  0x080, Synchronous message */
    CANopen_default_CANID_EMERGENCY         = 0x080,                    /**< \brief  0x081, Emergency messages (+NodeID) */
    CANopen_default_CANID_TIME_STAMP        = 0x100,                    /**< \brief  0x100, Time stamp message */
    CANopen_default_CANID_TPDO_1            = 0x180,                    /**< \brief  0x180, Default TPDO1 (+NodeID) */
    CANopen_default_CANID_RPDO_1            = 0x200,                    /**< \brief  0x200, Default RPDO1 (+NodeID) */
    CANopen_default_CANID_TPDO_2            = 0x280,                    /**< \brief  0x280, Default TPDO2 (+NodeID) */
    CANopen_default_CANID_RPDO_2            = 0x300,                    /**< \brief  0x300, Default RPDO2 (+NodeID) */
    CANopen_default_CANID_TPDO_3            = 0x380,                    /**< \brief  0x380, Default TPDO3 (+NodeID) */
    CANopen_default_CANID_RPDO_3            = 0x400,                    /**< \brief  0x400, Default RPDO3 (+NodeID) */
    CANopen_default_CANID_TPDO_4            = 0x480,                    /**< \brief  0x480, Default TPDO4 (+NodeID) */
    CANopen_default_CANID_RPDO_4            = 0x500,                    /**< \brief  0x500, Default RPDO5 (+NodeID) */
    CANopen_default_CANID_TSDO              = 0x580,                    /**< \brief  0x580, SDO response from server (+NodeID) */
    CANopen_default_CANID_RSDO              = 0x600,                    /**< \brief  0x600, SDO request from client (+NodeID) */
    CANopen_default_CANID_HEARTBEAT         = 0x700,                    /**< \brief  0x700, Heartbeat message */
    CANopen_default_CANID_NODEGUARD         = 0x700,                    /**< \brief  0x700, Nodeguard RTR message */
    CANopen_default_CANID_BOOTUP            = 0x700,                    /**< \brief  0x700, Boot-Up Message (+NodeID) */
}CANopen_default_CANIDs_t;

/**
 * \brief CAN Message Frame Type
 */
typedef struct{
    uint32_t        COB_ID;                                             /**< \brief  COBId or CAN-Message ID. A combination of the CANopen Function Code an the Node-ID */
    uint32_t        DLC;                                                /**< \brief  Data Length Code -> Length of the CAN-Message */
    union{
        uint8_t     u8[8];                                              /**< \brief  Data U8 access */
        uint32_t    u32[2];                                             /**< \brief  Data U32 access */
    }data;                                                              /**< \brief  Data Container */
} CANopen_CAN_MSG_t;

/**
 * \brief CANopen Frame counter enumeration.
 *
 * Within this enumeration, the frame counter is selected
 * \note    #CANOPEN_CFG_USE_FRAME_CNT needs to be set to "ON" to enable frame counting
 */
typedef enum{
    CANopen_TX_Frames,                                                  /**< \brief  Number of CAN frames that have been passed successfully to the CAN hardware */
    CANopen_RX_Frames,                                                  /**< \brief  Number of CAN frames that have been processed successfully */
    CANopen_TX_ER_Frames,                                               /**< \brief  Number of CAN frames that could not be send to the CAN hardware */
    CANopen_RX_ER_Frames,                                               /**< \brief  Number of CAN frames that could not be processed */
}CANopen_Frame_cnt_t;

/** \brief TX error flag.
 *
 * CANopen can set a error flag if a CAN TX error has been encountered. The flag is set in #CANopen__TXBufErrorHdl.
 * The user has to clear the flag (e.g. in a CAN TX ISR) manually.
 * \warning #CANopen_getsetSDO will return #CANOPEN_SDO_TX_ERROR as long as the flag is set.
 * \note	only active if #CANOPEN_CFG_USE_TX_BLOCK is set to "ON"
 */
extern volatile    boolean_t             CAN__TX_hasError;

/** \brief Stack state */
extern 			service_state_t			CANopen__Stack_state;

//-------------------------------------------------------------------- [Prototypes]

/**
 * \brief Initialization of the CANopen-Stack including Hardware Configuration.
 *
 * This function checks the consistency of the configuration defined in CANopen_cfg.c.
 * and is configuring the hardware according to this configuration.
 * The COB-IDs of the Mailboxes are configured with respect to #CANOPEN_CFG_NODE_ID in #CANopen_config.h
 * \warning This function also initializes the CAN hardware, thus the hardware needs to be uninitialized before calling this function.
 * \return  #RC_SUCCESS       	Operation succeeded
 *          #RC_ERROR_BAD_DATA	Inconsistent configuration in CANopen_cfg.c
 *          #RC_ERROR_CANCELED  NMT-FSM transition failed
 */
RC_t CANopen_Init(void);

/**
 * \brief Get current NMT-FSM state
 *
 * This function will return the current state of the NMT-statemachine.
 * See #CANopen_NMT_states_t for the possible states.
 * \return  The current NMT-FSM state, see #CANopen_NMT_states_t
 */
CANopen_NMT_states_t CANopen_GetNMTState(void);

/**
 * \brief Get a frame counter value
 *
 * \param   ctr                     counter to be read, see #CANopen_Frame_cnt_t
 * \param   p_ctr_val               pointer to which the value shall be written
 * \return  #RC_SUCCESS             if the counter was read
 *          #RC_ERROR_BAD_PARAM     if illegal \param ctr or \param p_ctr_val
 *          #RC_ERROR_INVALID_STATE if #CANOPEN_CFG_USE_FRAME_CNT is not set to "ON"
 */
RC_t CANopen_getFrameCnt(CANopen_Frame_cnt_t ctr,uint64_t* p_ctr_val);

#endif /* _CANOPEN_H */
/* [] END OF FILE */
