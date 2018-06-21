/**
 * \file	CANopen.c
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	11.01.2017
 * \version	0.7
 *
 * \brief CANopen core implementation
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

//-------------------------------------------------------------------- [header]
#include "CANopen.h"
#include "CAN_config.h"

//-------------------------------------------------------------------- [Local Data]
#if(ARCHITECTURE==ARCH_AURIX_1G)
#pragma section ".com.canopen"
#endif

static      CANopen_NMT_states_t    CANopen__NMT_state  = 	CANopen_NMT_STATE_Initialization;   /**< \brief The NMT-Statemachine status variable */
			service_state_t			CANopen__Stack_state=service_UNDEFINED;						/**< \brief Stack state, see#service_state_t. */
static      boolean_t               CANopen__NG_tBIT=BIT_0;                                     /**< \brief Node guard toggle bit */

#if(CANOPEN_CFG_USE_TX_BLOCK==ON)
volatile    boolean_t               CAN__TX_hasError=FALSE;                                     /**< \brief TX full flag. */
#endif

#if(CANOPEN_CFG_USE_FRAME_CNT==ON)
static      uint64_t                CANopen__TX_Frames=0;                                       /**< \brief Number of transmitted CAN frames. */
static      uint64_t                CANopen__RX_Frames=0;                                       /**< \brief Number of received CAN frames. */
static      uint64_t                CANopen__TX_ER_Frames=0;                                    /**< \brief Number of erroneous TX CAN frames. */
static      uint64_t                CANopen__RX_ER_Frames=0;                                    /**< \brief Number of erroneous RX CAN frames. */
#endif

#if(ARCHITECTURE==ARCH_AURIX_1G)
#pragma section
#endif
//-------------------------------------------------------------------- [local prototypes]

/**
 * \brief Send bootup message for this node
 *
 * When the NMT state machine does a transition from initialization to pre-operational,
 * the bootup message is send. This message indicates other nodes that this node has
 * been initialized successfully.
 * \return  the result of #CANopen__SendCANMsg is routed to the return value of this function
 */
static RC_t CANopen__SendBootupMsg(void);

/**
 * \brief Execute a NMT command (do a transition)
 *
 * CANopen defines several NMT-FSM command that can be found in CANopen_NMT_commands_t.
 * A description on the effect of the several commands can be found with the declaration of #CANopen_NMT_commands_t.
 * \param   command please see #CANopen_NMT_commands_t
 * \return  RC_SUCESS    Command succeeded
 *          RC_ERROR_CANCELED  Command failed
 */
static RC_t CANopen__NMTExec(CANopen_NMT_commands_t command);

//-------------------------------------------------------------------- [References]

#if(ARCHITECTURE==ARCH_PSOC)
/**
 * \brief Initializes the PSoC CAN Module
 *
 * \param   node_id NodeID of this node, needs to be in the range 1-127.
 * \return  #RC_SUCCESS       	Operation succeeded
 *          #RC_ERROR_BAD_PARAM Invalid NodeID
 *          #RC_ERROR_BAD_DATA	Inconsistent configuration in CANopen_cfg.c
 *          #RC_ERROR_CANCELED  NMT-FSM transition failed
 */
extern RC_t CAN__Init_PSoC(uint8_t node_id);
#endif
//-------------------------------------------------------------------- [Stack Init]

RC_t CANopen_Init(void){

    //result buffer
    RC_t __result;


#if(ARCHITECTURE==ARCH_AURIX_1G)

    __result=CAN_init(CANOPEN_CFG_NODE_ID);

    if(__result!=RC_SUCCESS)
        return __result;

#elif(ARCHITECTURE==ARCH_PSOC)

    __result=CAN__Init_PSoC(CANOPEN_CFG_NODE_ID);

    if(__result!=RC_SUCCESS)
        return __result;

#elif(ARCHITECTURE==ARCH_ESP32)

    //The node-ID is not passed to the CAN driver, as we do not use acceptance filtering on the ESP32.
    __result=CAN_init();

    if(__result!=RC_SUCCESS)
        return __result;

#else
#error "Unknown Target"
#endif

    //set stack state
    CANopen__Stack_state=service_READY;

    //We are done with the initialization, change state machine status. The result will be passed to the application.
    return CANopen__NMTExec(CANopen_NMT_CMD_INIT_DONE);

} /* CANopen_Init */


//-------------------------------------------------------------------- [global Getters]

boolean_t CANopen__isSDOenable(void){
    if(CANopen_GetNMTState()==CANopen_NMT_STATE_Operational||CANopen_GetNMTState()==CANopen_NMT_STATE_PreOperational)
        return TRUE;
    else
        return FALSE;
}

CANopen_NMT_states_t CANopen_GetNMTState(void){
    return CANopen__NMT_state;
}

RC_t CANopen_getFrameCnt(CANopen_Frame_cnt_t ctr,uint64_t* p_ctr_val){

#if(CANOPEN_CFG_USE_FRAME_CNT==ON)

    //check if the pointer is valid
    if(p_ctr_val==NULL)
        return RC_ERROR_BAD_PARAM;

    //get value
    switch(ctr){
        case CANopen_TX_Frames:
            *p_ctr_val=CANopen__TX_Frames;
            break;
        case CANopen_RX_Frames:
            *p_ctr_val=CANopen__RX_Frames;
            break;
        case CANopen_TX_ER_Frames:
            *p_ctr_val=CANopen__TX_ER_Frames;
            break;
        case CANopen_RX_ER_Frames:
            *p_ctr_val=CANopen__RX_ER_Frames;
            break;
        default:
            return RC_ERROR_BAD_PARAM;
        break;
    }
    return RC_SUCCESS;
#else
    (void)ctr;
    (void)p_ctr_val;

    return RC_ERROR_INVALID_STATE;
#endif
}

//-------------------------------------------------------------------- [Setters]

RC_t CANopen_incFrameCnt(CANopen_Frame_cnt_t ctr){

#if(CANOPEN_CFG_USE_FRAME_CNT==ON)
    //increment value
    switch(ctr){
        case CANopen_TX_Frames:
            CANopen__TX_Frames++;
            CANOPEN_TX_LED_MACRO
            break;
        case CANopen_RX_Frames:
            CANopen__RX_Frames++;
            CANOPEN_RX_LED_MACRO
            break;
        case CANopen_TX_ER_Frames:
            CANopen__TX_ER_Frames++;
            CANOPEN_TX_ER_LED_MACRO
            break;
        case CANopen_RX_ER_Frames:
            CANopen__RX_ER_Frames++;
            CANOPEN_RX_ER_LED_MACRO
            break;
        default:
            return RC_ERROR_BAD_PARAM;
        break;
    }
    return RC_SUCCESS;
#else //(CANOPEN_CFG_USE_FRAME_CNT==ON)

    //TODO: check if the compiler is smart enough to optimize and empty switchcase if no LED macros are set/CANOPEN_CFG_USE_CAN_LED is set to "OFF"
	switch(ctr){
		case CANopen_TX_Frames:
			CANOPEN_TX_LED_MACRO
			break;
		case CANopen_RX_Frames:
			CANOPEN_RX_LED_MACRO
			break;
		case CANopen_TX_ER_Frames:
			CANOPEN_TX_ER_LED_MACRO
			break;
		case CANopen_RX_ER_Frames:
			CANOPEN_RX_ER_LED_MACRO
			break;
		default:
			return RC_ERROR_BAD_PARAM;
		break;
	}

    return RC_ERROR_INVALID_STATE;
#endif
}

//-------------------------------------------------------------------- [NMT]

RC_t CANopen_NMTHdl(CANOPEN_RX_CALLBACK_PARAMS){

    //Check if this node was addressed, can be either the NodeId or a broadcast
    if(!(CAN_in->data.u8[CANOPEN_NMT_BYTE_NODE]==CANOPEN_CFG_NODE_ID||CAN_in->data.u8[CANOPEN_NMT_BYTE_NODE]==CANOPEN_NMT_BROADCAST))
        return RC_SUCCESS;

    //Check if DLC has the right size
    if(CAN_in->DLC!=CANOPEN_NMT_DLC)
        return RC_ERROR_BAD_PARAM;

    //Execute NMT Command
    return CANopen__NMTExec(CAN_in->data.u8[CANOPEN_NMT_BYTE_FCT]);
}

RC_t CANopen__NMTExec(CANopen_NMT_commands_t command){

    //state buffer for transition detection
    CANopen_NMT_states_t __old_state=CANopen__NMT_state;

    //Transition Reactions are based on the current state
    switch(CANopen__NMT_state)
    {
        //Current State: INITIALISATION
        case CANopen_NMT_STATE_Initialization:
            switch(command){
                //Transition 2: Initialization finished
                case CANopen_NMT_CMD_INIT_DONE:
                    CANopen__SendBootupMsg();
                    CANopen__NMT_state=CANopen_NMT_STATE_PreOperational;
                    break;

                // illegal command or NMT-FSM was already in requested state
                default:
                    return RC_ERROR_CANCELED;
            }
            break;

        //Current State: PRE-OPERATIONAL
        case CANopen_NMT_STATE_PreOperational:
            switch(command){
                //Transition 3: Enter Operational State Indicator
                case CANopen_NMT_CMD_START_NODE:
                    CANopen__NMT_state=CANopen_NMT_STATE_Operational;
                    break;

                //Transition 5: Stop Remote Indicator
                case CANopen_NMT_CMD_STOP_NODE:
                    CANopen__NMT_state=CANopen_NMT_STATE_Stopped;
                    break;

                //Transition 11: Reset Node
                case CANopen_NMT_CMD_RESET_NODE:
                    //CANopenODAppReset();
                    break;

                //Transition 14: Reset Communication
                case CANopen_NMT_CMD_RESET_COM:
                    //CANopenODComReset();
                    break;

                // illegal command or NMT-FSM was already in requested state
                default:
                    return RC_ERROR_CANCELED;
            }
            break;

        //Current State: OPERATIONAL
        case CANopen_NMT_STATE_Operational:
            switch(command){

                //Transition 4: Enter Pre-Operational State Indicator
                case CANopen_NMT_CMD_ENTER_PREOP:
                    CANopen__NMT_state=CANopen_NMT_STATE_PreOperational;
                    break;

                //Transition 8: Stop Remote Node indicator
                case CANopen_NMT_CMD_STOP_NODE:
                    CANopen__NMT_state=CANopen_NMT_STATE_Stopped;
                    break;

                //Transition 9: Reset Node Indicator
                case CANopen_NMT_CMD_RESET_NODE:
                    //CANopenODAppReset();
                    break;

                //Transition 12: Reset Communication
                case CANopen_NMT_CMD_RESET_COM:
                    //return CANopenODComReset();
                    break;

                // illegal command or NMT-FSM was already in requested state
                default:
                    return RC_ERROR_CANCELED;
            }
            break;

        //Current State: STOPPED
        case CANopen_NMT_STATE_Stopped:
            switch(command){

                //Transition 6: Start Remote Node indicator
                case CANopen_NMT_CMD_START_NODE:
                    CANopen__NMT_state=CANopen_NMT_STATE_Operational;
                    break;

                //Transition 7: Enter Pre-Operational State Indicator
                case CANopen_NMT_CMD_ENTER_PREOP:
                    CANopen__NMT_state=CANopen_NMT_STATE_PreOperational;
                    break;

                //Transition 10: Reset Node Indicator
               case CANopen_NMT_CMD_RESET_NODE:
                    //CANopenODAppReset();
                    break;

                //Transition 13: Reset Communication
                case CANopen_NMT_CMD_RESET_COM:
                    //return CANopenODComReset();
                    break;

                // illegal command or NMT-FSM was already in requested state
                default:
                    return RC_ERROR_CANCELED;
            }
            break;

        //Unknown current state, stop hacking around! ;)
        default:
            return RC_ERROR_CANCELED;
    }

    //new state handlers
    if(__old_state!=CANopen__NMT_state)
        switch(CANopen__NMT_state){
            case CANopen_NMT_STATE_Initialization:

                break;

            case CANopen_NMT_STATE_PreOperational:

                break;

            case CANopen_NMT_STATE_Operational:

                break;
            case CANopen_NMT_STATE_Stopped:

                break;

        }

    return RC_SUCCESS;
}

RC_t CANopen__SendBootupMsg(void){
    //data container for CAN message
    CANopen_CAN_MSG_t msg={CANopen_default_CANID_BOOTUP+CANOPEN_CFG_NODE_ID,CANOPEN_BOOTUP_DLC,{.u8={CANOPEN_SDO_DATA_INVALID}}};

    //set bootup identifier
    msg.data.u8[CANOPEN_BOOTUP_BYTE]=CANOPEN_BOOTUP_MSG;

    return CANopen__SendCANMsg(&msg);
}

//-------------------------------------------------------------------- [Heartbeat/Node guarding]
RC_t CANopen_NG_Hdl(CANOPEN_RX_CALLBACK_PARAMS){
   
	(void)CAN_in;

    //CAN send result, will be given back
    RC_t __result;
    
    //send frame
    __result=CANopen__SendCANMsg(&(CANopen_CAN_MSG_t){
        CANopen_default_CANID_NODEGUARD+CANOPEN_CFG_NODE_ID,    //COB_ID
        CANOPEN_NODEGUARD_DLC,                                  //DLC
        {   //Dadat
            .u8[CANOPEN_NODEGUARD_BYTE]=CANOPEN_NODEGUARD_DATA|(CANopen__NG_tBIT<<CANOPEN_NODEGUARD_TB_OFF)
        }
    });
    
    //toggle bit
    CANopen__NG_tBIT = !CANopen__NG_tBIT;

    return __result;
}

//-------------------------------------------------------------------- [Other Handlers]

RC_t CANopen__TXBufErrorHdl(){

#if(CANOPEN_CFG_USE_TX_BLOCK==ON)
	CAN__TX_hasError=TRUE;
#endif

#if(ARCHITECTURE==ARCH_PSOC)

    #if (OS==OS_FREERTOS)
	//TODO: remove...
    while(CAN__TX_hasError){
        //Indicate error
        CANOPEN_TX_ER_LED_MACRO
    }
    #endif
#endif

    return RC_ERROR;
}

RC_t CANopen__CatchHdl(CANOPEN_RX_CALLBACK_PARAMS){
    (void)CAN_in;

    return RC_SUCCESS;
}

/* [] END OF FILE */
