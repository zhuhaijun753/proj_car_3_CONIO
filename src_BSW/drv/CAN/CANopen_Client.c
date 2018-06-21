/**
 * \file	CANopen_Client.c
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	11.01.2017
 * \version	0.7
 *
 * \brief CANopen client implementation
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
#include "CANopen_Client.h"
#include "CAN_config.h"
#include <string.h>		//memset
//-------------------------------------------------------------------- [Defines]

/** \brief Parameters to add a new entry into the list of awaited responses*/
#define CANOPEN__SDO_ADD_DISP_PARAM     CANopen__SDO_transfer_mode_t transfer_mode, CANopen__SDO_transfer_dir_t direction, uint16_t COB_ID,  uint8_t node, uint16_t index, uint8_t subindex, void* value, uint16_t value_length, RC_t (*callback) (CANOPEN_SDO_CALLBACK_PARAMS), uint32_t timeout, uint8_t flags, CANOPEN_CFG_SDO_U_T usr_arg, CANOPEN_CFG_SDO_DISP_IT* p_sched_index

/** \brief Parameters for sub_handlers of SDO responses*/
#define CANOPEN__SDO_C_SUBHDL_PARAMS    CANopen_CAN_MSG_t* CAN_in, CANOPEN_CFG_SDO_DISP_IT* i, CANOPEN_CFG_SDO_DISP_IT* j, boolean_t* __callHandler, uint32_t* __received_length, boolean_t* __stop_sched_iteration, RC_t* __result
#define CANOPEN__DISP_ITEM(x)	        CANopen__SDO_Dispatcher[x].item

//-------------------------------------------------------------------- [types]
/** \brief Entry Type of dispatcher list entries. */
typedef struct{
    boolean_t                           isActive;          	/**< \brief Tell the dispatcher if this slot is available or not   */
    CANopen_SDO_dispatcher_element_t    item;               /**< \brief Details about the SDO transfer */
}CANopen__SDO_dispatcher_slot_t;

//-------------------------------------------------------------------- [stack data]
#if(ARCHITECTURE==ARCH_AURIX_1G)
#pragma section ".com.canopen"
#endif

/** \brief Number of active entries in the dispatcher list*/
volatile CANOPEN_CFG_SDO_DISP_IT         CANopen__SDO_Dispatcher_itemCount=0;

/** \brief SDO Dispatcher List*/
volatile CANopen__SDO_dispatcher_slot_t CANopen__SDO_Dispatcher[CANOPEN_CFG_SDO_DISP_SIZE]={{FALSE,{0,0,0,0,0,0,0,0,0,0,0,0,0,0}}};

#if(ARCHITECTURE==ARCH_AURIX_1G)
#pragma section
#endif

//-------------------------------------------------------------------- [local prototypes]

/** \brief Adds a awaited response into the dispatcher list. */
static CANopen_SDO_transfer_RET_t   CANopen__SDO_ListAdd            (CANOPEN__SDO_ADD_DISP_PARAM);

/** \brief Deletes an awaited response from the dispatcher list */
static void                         CANopen__SDO_ListDelete         (CANOPEN_CFG_SDO_DISP_IT);

/** \brief Internal callback stub */
static RC_t                         CANopen__SDO_iCallback          (CANOPEN_SDO_CALLBACK_PARAMS);

/** \brief The spinlock for blocking get/set operations */
static CANopen_SDO_transfer_RET_t   CANopen__SDO_WaitResponce       (CANOPEN_CFG_SDO_DISP_IT);

/** \brief Sub Handler for Upload segment responses */
static void                         CANopen__SDO_sur_hdl            (CANOPEN__SDO_C_SUBHDL_PARAMS);

/** \brief Sub Handler for Initiate upload responses */
static void                         CANopen__SDO_iur_hdl            (CANOPEN__SDO_C_SUBHDL_PARAMS);

/** \brief Sub Handler for block transfer */
static void                         CANopen__SDO_blk_hdl            (CANOPEN__SDO_C_SUBHDL_PARAMS);

/** \brief Sub Handler for Client Block upload */
static void                         CANopen__SDO_cbu_hdl            (CANOPEN__SDO_C_SUBHDL_PARAMS);

/** \brief Send SDO CAN Frame */
INLINE RC_t                         CANopen__send_SDO_response      (uint8_t node_ID, uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3,uint8_t byte4,uint8_t byte5,uint8_t byte6,uint8_t byte7);


//-------------------------------------------------------------------- [SDO]

//**************** SDO Requests *****************************************************//
CANopen_SDO_transfer_RET_t CANopen_getsetSDO(CANOPEN_GETSETSDO_PARAMS){

    //Transfer Mode of the message
    CANopen__SDO_transfer_mode_t    __transfer_mode;
    
    //determines if this request will use callback or blocking mode
    boolean_t __isBlockingSDO;
    
    //flags that will be passed to the Dispatcher
    uint8_t __flags=CANOPEN_DISP_FLAGS_N;
    
    //iterator for CAN-Message bytes
    uint8_t i;
    
    //result of the OD operation
    CANopen_SDO_transfer_RET_t _result=CANOPEN_SDO_SUCCESS;

    //prepare message to be send to node
    CANopen_CAN_MSG_t _msg={
        CANopen_default_CANID_RSDO+nodeId,  //COB-ID
        CANOPEN_SDO_DLC,                    //DLC
        {.u8={CANOPEN_SDO_DATA_INVALID}} 	//payload
    };
    
    //the List-Index will be stored in this variable. Initialization without effect but suppresses warnings.
    CANOPEN_CFG_SDO_DISP_IT __dispatcher_index=0;
    
#if(CANOPEN_CFG_USE_TX_BLOCK==ON)
    //Check if hardware has errors
	if(CAN__TX_hasError==TRUE)
		return CANOPEN_SDO_TX_ERROR;
#endif

	//Check stack state
	if(CANopen__Stack_state!=service_READY)
		return CANOPEN_SDO_UNINITALIZED;

    //Check if nodeID is in range
    if(nodeId<CANOPEN_NODE_ID_MIN||nodeId>CANOPEN_NODE_ID_MAX)
        return CANOPEN_SDO_ILLEGAL_NODE_ID;
    
    //check if the length is bigger than #CANOPEN_CFG_SDO_MIN_DLENGTH
    if(value_length<CANOPEN_CFG_SDO_MIN_DLENGTH)
        return CANOPEN_SDO_ILLEGAL_VALUE_LENGTH;    
    
    //check if this is "fire and forget message" or "read only"
    if(timeout==0){

        /* "fire and forget" and "listen only" modes utilize the expedited message transfer that can transport only 4 byte.
            Thus the length can not be bigger that 4*/
        if(value_length>CANOPEN_SDO_DATA_MLENGTH)
            return CANOPEN_SDO_ILLEGAL_VALUE_LENGTH;
        
        switch(direction){
            case CANOPEN_SDO_DIR_UP:
                __transfer_mode=CANOPEN_SDO_mode_listen_only;
                break;
            
            case CANOPEN_SDO_DIR_DN:
                __transfer_mode=CANOPEN_SDO_mode_fireandforget;
            break;
            
            default:
                return CANOPEN_SDO_ILLEGAL_TIMEOUT;
        }
    }
    else{
        //depending on the value-size a transfer mode has to be chosen
        
        //does the message fit in one frame?
        if(value_length>CANOPEN_SDO_DATA_MLENGTH){
            //if it is so big that block transfer mode is more efficient
            if(value_length<=CANOPEN_CFG_TRANSFER_M_THRES)
                __transfer_mode=CANOPEN_SDO_mode_normal;
            else
                __transfer_mode=CANOPEN_SDO_mode_block;
        }
        else{
            //message fits in one frame
            __transfer_mode=CANOPEN_SDO_mode_expedited;
        }
    }

    //check if we have a callback function, otherwise a spinlock is used to wait for the response (can not be used in "listen only" and "fire and forget" mode)
    if(callback==NULL){
        
        switch(__transfer_mode){
            case CANOPEN_SDO_mode_expedited:
                __isBlockingSDO=TRUE;
                break;
            
            case CANOPEN_SDO_mode_normal:
                __isBlockingSDO=TRUE;
                break;
            
            case CANOPEN_SDO_mode_block:
                __isBlockingSDO=TRUE;
                break;
            
            case CANOPEN_SDO_mode_fireandforget:
                __isBlockingSDO=FALSE;
                break;
            
            case CANOPEN_SDO_mode_listen_only:
               //error, "listen only" mode requires a callback function
                return CANOPEN_SDO_CALLBACK_MISSING;
                break;
            
            default:
                return CANOPEN_SDO_IMPLEMENTATION_MISSING;
        }
    }
    else{
        //TODO: if the user gave a callback for "fire and forget", it will not be called. Maybe an error should be given back in this case
        __isBlockingSDO=FALSE;
    }
                
    //download request (write)
    if(direction==CANOPEN_SDO_DIR_DN){
        
        switch(__transfer_mode){
        case CANOPEN_SDO_mode_expedited:
            // Download request function byte S=1 E=1
            _msg.data.u8[CANOPEN_SDO_BYTE_FCT]=CANOPEN_SDO_FCT_DR_DS_SHIFTED|CANOPEN_SDO_BYTE_FCT_S_MSK|CANOPEN_SDO_BYTE_FCT_E_MSK;     
            
            //set length code
            _msg.data.u8[CANOPEN_SDO_BYTE_FCT]|=(CANOPEN_SDO_DATA_MLENGTH-value_length)<<CANOPEN_SDO_BYTE_FCT_N_SHIFT;
            break;
        
        case CANOPEN_SDO_mode_normal:
            //this mode is currently not implemented for downloads
            return CANOPEN_SDO_IMPLEMENTATION_MISSING;
            break;
        
        case CANOPEN_SDO_mode_block:
            //this mode is currently not implemented for downloads
            return CANOPEN_SDO_IMPLEMENTATION_MISSING;
            break;
        
        case CANOPEN_SDO_mode_fireandforget:
            // Download request function byte S=1 E=1
            _msg.data.u8[CANOPEN_SDO_BYTE_FCT]=CANOPEN_SDO_FCT_DR_DS_SHIFTED|CANOPEN_SDO_BYTE_FCT_S_MSK|CANOPEN_SDO_BYTE_FCT_E_MSK;            
            //set length code
            _msg.data.u8[CANOPEN_SDO_BYTE_FCT]|=(CANOPEN_SDO_DATA_MLENGTH-value_length)<<CANOPEN_SDO_BYTE_FCT_N_SHIFT;
            break;
        
        case CANOPEN_SDO_mode_listen_only:
            // "Listen only" can only be used with uploads (read)
            return CANOPEN_SDO_PARAM_ERROR;
            break;
        
        default:
            return CANOPEN_SDO_IMPLEMENTATION_MISSING;
        }
        
    }
    //Upload request (read)
    else{
        
        switch(__transfer_mode){
        case CANOPEN_SDO_mode_expedited:
            _msg.data.u8[CANOPEN_SDO_BYTE_FCT]=CANOPEN_SDO_FCT_IU_IU_SHIFTED;
            break;
        
        case CANOPEN_SDO_mode_normal:
            _msg.data.u8[CANOPEN_SDO_BYTE_FCT]=CANOPEN_SDO_FCT_IU_IU_SHIFTED;
            break;
        
        case CANOPEN_SDO_mode_block:
            //Set block upload command
            /*TODO  This will work, but it is nicer to manually set the bits mentioned below.
                    Bit 2       ->  0   ->  Client CRC disabled. (1-> Client CRC enabled)
                    Bit 1..0    ->  0   ->  initiate upload request  
            */
            _msg.data.u8[CANOPEN_SDO_BYTE_FCT]=CANOPEN_SDO_FCT_BU_BD_SHIFTED;
            
            //define block-size (maximum number of segments per block)
            _msg.data.u8[CANOPEN_SDO_BYTE_BLK_S]=CANOPEN_CFG_SDO_BLK_SZ;
            
            //define the protocol switch threshold for the server
            _msg.data.u8[CANOPEN_SDO_BYTE_PST]=CANOPEN_CFG_SDO_BLK_PST;
            break;
        
        case CANOPEN_SDO_mode_fireandforget:
            // "Listen only" can only be used with downloads (write)
            return CANOPEN_SDO_PARAM_ERROR;
            break;
        
        case CANOPEN_SDO_mode_listen_only:
            // no CAN-Frame needs to be prepared
            break;
        
        default:
            return CANOPEN_SDO_IMPLEMENTATION_MISSING;
        }

    }
    
    //(s)index TODO: doesn't need to be done for "listen only"
    _msg.data.u8[CANOPEN_SDO_BYTE_IL]  =LO8(Index);
    _msg.data.u8[CANOPEN_SDO_BYTE_IH]  =HI8(Index);
    _msg.data.u8[CANOPEN_SDO_BYTE_S]   =subindex;
    
    //in case of download request, fill data-bytes of CAN message TODO: this only is true for "fire and forget" and expedited upload (other modes currently not implemented for download)
    if(direction==CANOPEN_SDO_DIR_DN){
        for(i=0;i<value_length;i++)
            _msg.data.u8[CANOPEN_SDO_BYTE_DATA_START+i]=*(((uint8_t*)value)+i);
    }

    //if we do not have a callback function, we set up the handler for blocking transfers
    if(__isBlockingSDO==TRUE)
        callback=CANopen__SDO_iCallback;
    
    //if this is no "fire and forget" message, it is added to the dispatcher
    if(__transfer_mode!=CANOPEN_SDO_mode_fireandforget){
        //insert entry into list of awaited messages
        _result=CANopen__SDO_ListAdd(
            __transfer_mode,                        //transfer mode
            direction,                              //direction
            CANopen_default_CANID_TSDO+nodeId,      //COB-ID
            nodeId,                                 //Node-ID
            Index,                                  //Index
            subindex,                               //Sub-Index
            value,                                  //Pointer to value-container
            value_length,                           //length of value in bytes
            callback,                               //callback function if transfer finishes or is aborted
            timeout,                                //timeout of this request in ms
            __flags,                                //flags for the dispatcher
            usr_arg,								//user-argument
            &__dispatcher_index);                   //returns the dispatcher index of the request
        
        //if insert operation failed, inform user
        if(_result!=CANOPEN_SDO_SUCCESS)
            return _result;
    }
    
    if(__transfer_mode!=CANOPEN_SDO_mode_listen_only){
        //if CAN send operation failed
        if(_result!=CANOPEN_SDO_SUCCESS){
            //we need to delete it from dispatching
            if(__transfer_mode!=CANOPEN_SDO_mode_fireandforget)
                CANopen__SDO_ListDelete(__dispatcher_index);
            
            //return result to user
            return _result;
        }
        
        //send request via CAN
        if(CANopen__SendCANMsg(&_msg)!=RC_SUCCESS){
        	 CANopen__SDO_ListDelete(__dispatcher_index);
        	return CANOPEN_SDO_TX_ERROR;
        }

        //is this is a blocking request, we wait for the response
        if(__isBlockingSDO==TRUE)
            _result=CANopen__SDO_WaitResponce(__dispatcher_index);
    }

    //Return result
    return _result;
}

RC_t CANopen_sendNMTcmd(CANopen_NMT_commands_t command, uint8_t node){
	
    CANopen_CAN_MSG_t __msg={
        CANOPEN_NMT_FCT_CODE+CANOPEN_NMT_NODE_ID,   //COB-ID
        CANOPEN_NMT_DLC,                            //DLC
        {.u8={CANOPEN_SDO_DATA_INVALID}}            //payload
    };
    	
	__msg.data.u8[CANOPEN_NMT_BYTE_FCT]	=(uint8_t)command;
	__msg.data.u8[CANOPEN_NMT_BYTE_NODE]	= node;

	return CANopen__SendCANMsg(&__msg);
}
//**************** SDO Responses ****************************************************//

RC_t CANopen_SDO_resp_hdl(CANOPEN_RX_CALLBACK_PARAMS){

    //iterators for the dispatcher
    CANOPEN_CFG_SDO_DISP_IT i,j, active_entries=0;
    
    //extracted length code of SDO messages
    uint32_t __received_length=0;
    
    //determines if the defined message handler shall be called 
    boolean_t  __callHandler=FALSE;

    //determines if the dispatcher iteration can be stopped after a handler-call
    boolean_t __stop_sched_iteration=FALSE;
    
    //storage for external function call results
    RC_t __result;
    
    //determines if index/sub-index match (can not be applied to all transfer modes)
    boolean_t  __sidx_mtch;

    //only handle SDO responses here
    if(CAN_in->DLC!=CANOPEN_SDO_DLC)
    	return RC_ERROR_BAD_PARAM;

	//iterate trough the list of awaited responses
	for(i=0;i<CANOPEN_CFG_SDO_DISP_SIZE;i++){
		if(CANopen__SDO_Dispatcher[i].isActive==TRUE){
			//increment number of active entries
			active_entries++;

			//check for the COB-ID
			if( CANOPEN__DISP_ITEM(i).COB_ID==CAN_in->COB_ID){

				//check if this is a block upload
				if((CANOPEN__DISP_ITEM(i).flags&CANOPEN_DISP_FLAGS_B)&&(CANOPEN__DISP_ITEM(i).direction==CANOPEN_SDO_DIR_UP)){
					//call block-transfer handler
					CANopen__SDO_blk_hdl(CAN_in,&i,&j,&__callHandler,&__received_length,&__stop_sched_iteration,&__result);
				}/* block transfer */
				else
				{
					//check if index and sub-index match
					if(CANOPEN__DISP_ITEM(i).index==CONCAT_2BYTES(CAN_in->data.u8[CANOPEN_SDO_BYTE_IH], CAN_in->data.u8[CANOPEN_SDO_BYTE_IL])&&
					CANOPEN__DISP_ITEM(i).subIndex==CAN_in->data.u8[CANOPEN_SDO_BYTE_S])
						__sidx_mtch=TRUE;
					else
						__sidx_mtch=FALSE;

					//check function code
					switch((CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&CANOPEN_SDO_BYTE_FCT_CMD_MSK)>>CANOPEN_SDO_BYTE_FCT_CMD_SHIFT){

						// 0: Upload segment response
						case CANOPEN_SDO_FCT_DS_US:
							//check if this is an upload
							if(CANOPEN__DISP_ITEM(i).direction==CANOPEN_SDO_DIR_UP)
								//Handle response
								CANopen__SDO_sur_hdl(CAN_in,&i,&j,&__callHandler,&__received_length,&__stop_sched_iteration,&__result);
							break; /* CANOPEN_SDO_FCT_DS_US */

						//1: Download segment response
						case CANOPEN_SDO_FCT_DR_DS:
							//TODO
							break;/* CANOPEN_SDO_FCT_DR_DS */

						//2: Initiate upload response
						case CANOPEN_SDO_FCT_IU_IU:
							 //check if (s)index is correct and this is the entry for an upload
							if(__sidx_mtch&&CANOPEN__DISP_ITEM(i).direction==CANOPEN_SDO_DIR_UP)
								//Handle response
								CANopen__SDO_iur_hdl(CAN_in,&i,&j,&__callHandler,&__received_length,&__stop_sched_iteration,&__result);
							break;/* CANOPEN_SDO_FCT_IU_IU */

						//3: Initiate download response (Download Acknowledge)
						case CANOPEN_SDO_FCT_US_ID:
							//check if (s)index is correct and this is the entry for an download
							if(__sidx_mtch&&CANOPEN__DISP_ITEM(i).direction==CANOPEN_SDO_DIR_DN)
								//check if we wait for an download response, if not ignore message.
								if(CANOPEN__DISP_ITEM(i).direction==CANOPEN_SDO_DIR_DN)
									__callHandler=TRUE;

							break;/* CANOPEN_SDO_FCT_US_ID */

						//4: Abort
						case CANOPEN_SDO_FCT_AB:
							//we have to process abort messages in any case
							__callHandler=TRUE;

							//set Abort flag
							CANOPEN__DISP_ITEM(i).flags|=CANOPEN_DISP_FLAGS_A;

							//read abort-code
							for(j=0;j<CANOPEN_SDO_FCT_AB_LENGTH;j++)
								CANOPEN__DISP_ITEM(i).abort_code|=CAN_in->data.u8[CANOPEN_SDO_BYTE_DATA_START+j]<<(j*BYTE_L);

							//if the value-container can store the abort-code, also store it there too (for blocking mode)
							if(CANOPEN__DISP_ITEM(i).length>=CANOPEN_SDO_AB_DLC){
								//set length
								CANOPEN__DISP_ITEM(i).length=CANOPEN_SDO_AB_DLC;

								//set value to abort code
								*(uint32_t*)CANOPEN__DISP_ITEM(i).value=CANOPEN__DISP_ITEM(i).abort_code;
							}
							break;/* CANOPEN_SDO_FCT_AB */

						//5: Client block download
						case CANOPEN_SDO_FCT_BU_BD:
							//TODO
							break;/* CANOPEN_SDO_FCT_BU_BD */

						//6: Client block upload
						case CANOPEN_SDO_FCT_BD_BU:
							//check if (s)index is correct and this entry is in upload mode
							if(__sidx_mtch&&CANOPEN__DISP_ITEM(i).direction==CANOPEN_SDO_DIR_UP)
								//call Client block upload handler
								CANopen__SDO_cbu_hdl(CAN_in,&i,&j,&__callHandler,&__received_length,&__stop_sched_iteration,&__result);
							break;/* CANOPEN_SDO_FCT_BD_BU */

						//illegal function code
						default:
							return RC_ERROR_BAD_DATA;
							break;

					}/*check function code*/
				}/* no block transfer */

				//call handler if requested
				if(__callHandler==TRUE){

					//set "ready" flag
					CANOPEN__DISP_ITEM(i).flags|=CANOPEN_DISP_FLAGS_R;

					if(CANOPEN__DISP_ITEM(i).callback(&CANOPEN__DISP_ITEM(i))==RC_SUCCESS)
						//if the callback succeeded we can delete the entry from the list
						CANopen__SDO_ListDelete(i);

					//we are done, break the search-loop
					break;
				}

				//stop dispatcher iteration
				if(__stop_sched_iteration==TRUE)
					break;
			} /* COB-ID match */

			//if the number of processed active entries is bigger of equal the number of total active entries, we can stop here
			if(active_entries>=CANopen__SDO_Dispatcher_itemCount)
				break;
		} /* dispatcher entry is active */
	} /* dispatcher entry iteration */

    return RC_SUCCESS;
}

INLINE RC_t CANopen__send_SDO_response(uint8_t node_ID, uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3,uint8_t byte4,uint8_t byte5,uint8_t byte6,uint8_t byte7){

    //send request
    return CANopen__SendCANMsg(&(CANopen_CAN_MSG_t){
        CANopen_default_CANID_RSDO+node_ID,   				        //COB-ID
        CANOPEN_SDO_DLC,                            		        //DLC
        {.u8={byte0,byte1,byte2,byte3,byte4,byte5,byte6,byte7}}     //payload
    });
}

static void CANopen__SDO_cbu_hdl(CANOPEN__SDO_C_SUBHDL_PARAMS){
    
    (void)j;
    (void)__result;
    
    /*TODO: Check the flags of the command
    2   sc  ->  Server CRC support
    1   s   ->  Size indicator
    */
    if((CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&CANOPEN_SDO_BLK_SS_MSK)==0){

        //extract value-length
        *__received_length=
            (CAN_in->data.u8[CANOPEN_SDO_BYTE_NB0]<<CANOPEN_SDO_BYTE_NB0_SHFT)+
            (CAN_in->data.u8[CANOPEN_SDO_BYTE_NB1]<<CANOPEN_SDO_BYTE_NB1_SHFT)+
            (CAN_in->data.u8[CANOPEN_SDO_BYTE_NB2]<<CANOPEN_SDO_BYTE_NB2_SHFT)+
            (CAN_in->data.u8[CANOPEN_SDO_BYTE_NB3]<<CANOPEN_SDO_BYTE_NB3_SHFT);
       
        //check if the length of the remote object exceeds the length of the given container
        if(*__received_length<=CANOPEN__DISP_ITEM(*i).length){
            
            //set the flag for the Dispatcher, so it knows that this transfer is using block-mode
            CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_B;
            
            //Send Response
            if(CANopen__send_SDO_response(
                CANOPEN__DISP_ITEM(*i).node,                      //remote node
                CANOPEN_SDO_FCT_BU_BD_SHIFTED|CANOPEN_SDO_BLK_CS_SU,     //Block upload command with client subcommand: start upload
                0,                                                          //byte 1
                0,                                                          //byte 2
                0,                                                          //byte 3
                0,                                                          //byte 4
                0,                                                          //byte 5
                0,                                                          //byte 6
                0                                                           //byte 7
            //CAN TX failed
            )!=RC_SUCCESS){
            	*__callHandler=TRUE;

            	//set error flag
            	CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_E;
            }
        }                                
        //length exceeds local container
        else{
            *__callHandler=TRUE;
            
            //set overflow flag
            CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_O;
        }
    }
    else
    //end block upload response
    {}
    
    *__stop_sched_iteration=TRUE;
}

static void CANopen__SDO_sur_hdl(CANOPEN__SDO_C_SUBHDL_PARAMS){
    
    (void)__received_length;
    (void)__result;
    
    //in any case, the Dispatcher does not need to check other entries
    *__stop_sched_iteration=TRUE;
    
    //check toggle bit
    if(!(CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&CANOPEN_SDO_BYTE_FCT_T_MSK)^(CANOPEN__DISP_ITEM(*i).flags&CANOPEN_DISP_FLAGS_TG)){
        //data length in this segment
        uint8_t ___data_length; 
        
        //toggle toggle bit
        CANOPEN__DISP_ITEM(*i).flags ^= 1 << CANOPEN_DISP_FLAGS_TG_SHIFT;
                                
        //check if data length is indicated
        if(((CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&CANOPEN_SDO_BYTE_FCT_SN_MSK)>>CANOPEN_SDO_BYTE_FCT_SN_SHIFT)!=0)
            ___data_length=CANOPEN_SDO_SEG_BYTE_L-((CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&CANOPEN_SDO_BYTE_FCT_SN_MSK)>>CANOPEN_SDO_BYTE_FCT_SN_SHIFT);
        //data size is not indicated, set #CANOPEN_SDO_SEG_BYTE_L as default
        else
            ___data_length=CANOPEN_SDO_SEG_BYTE_L;
                                
        //check if the received bytes exceed the expected number of bytes
        if(CANOPEN__DISP_ITEM(*i).transmitted+___data_length>CANOPEN__DISP_ITEM(*i).length){
            
            ___data_length=CANOPEN__DISP_ITEM(*i).length-CANOPEN__DISP_ITEM(*i).transmitted;
            
            //fill the remaining space
            for(*j=0;*j<___data_length;(*j)++)
                *((uint8_t*)CANOPEN__DISP_ITEM(*i).value+CANOPEN__DISP_ITEM(*i).transmitted++)=CAN_in->data.u8[*j+CANOPEN_SDO_SEG_BYTE_F];
            
            //set overflow flag
            CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_O;
                
            //Call the handler
            *__callHandler=TRUE;
        }
        //received number of bytes does not exceed expected number of bytes
        else
        {
            //extract data
            for(*j=0;*j<___data_length;(*j)++)
                *((uint8_t*)CANOPEN__DISP_ITEM(*i).value+CANOPEN__DISP_ITEM(*i).transmitted++)=CAN_in->data.u8[*j+CANOPEN_SDO_SEG_BYTE_F];
            
            //check if this was the last segment
            if(CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&CANOPEN_SDO_BYTE_FCT_C_MASK){
                //end of transfer, call handler
                *__callHandler=TRUE;
            }
            //more segments to be uploaded
            else{
                //request next segment
                uint8_t toggle_bit=0;
    
                //set toggle bit
                if(CANOPEN__DISP_ITEM(*i).flags&CANOPEN_DISP_FLAGS_TG)
                    toggle_bit=CANOPEN_SDO_BYTE_FCT_T_MSK;
                
                if(CANopen__send_SDO_response(
                                CANOPEN__DISP_ITEM(*i).node,              //remote node
                                CANOPEN_SDO_FCT_US_ID_SHIFTED|toggle_bit,         //Download Response Command with toggle bit
                                0,                                                  //byte 1
                                0,                                                  //byte 2
                                0,                                                  //byte 3
                                0,                                                  //byte 4
                                0,                                                  //byte 5
                                0,                                                  //byte 6
                                0                                                   //byte 7
				//CAN TX failed
				)!=RC_SUCCESS){
					*__callHandler=TRUE;

					//set error flag
					CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_E;
				}
            }
        }                                                  
    }
    //toggle bit error
    else{
		*__callHandler=TRUE;

		//set error flag
		CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_E;
    }
}

static void CANopen__SDO_iur_hdl(CANOPEN__SDO_C_SUBHDL_PARAMS){
    
    (void)__result;

    //check if we wait for an upload response, if not ignore message.
    if(CANOPEN__DISP_ITEM(*i).direction==CANOPEN_SDO_DIR_UP){
        
        //The dispatcher does not need to check other entries
        *__stop_sched_iteration=TRUE;
        
        //check E/S bit combination to determine which mode the server wants to send data
        switch(CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&(CANOPEN_SDO_BYTE_FCT_E_MSK|CANOPEN_SDO_BYTE_FCT_S_MSK)){

            //E=0 S=0 reserved, illegal response TODO: send abort
            case 0:
            break;/* reserved  */
            
            //E=0 S=1 normal (segmented) transfer initialization 
            case CANOPEN_SDO_BYTE_FCT_S_MSK:
            
                //extract length
                *__received_length=
                    (CAN_in->data.u8[CANOPEN_SDO_BYTE_NB0]<<CANOPEN_SDO_BYTE_NB0_SHFT)+
                    (CAN_in->data.u8[CANOPEN_SDO_BYTE_NB1]<<CANOPEN_SDO_BYTE_NB1_SHFT)+
                    (CAN_in->data.u8[CANOPEN_SDO_BYTE_NB2]<<CANOPEN_SDO_BYTE_NB2_SHFT)+
                    (CAN_in->data.u8[CANOPEN_SDO_BYTE_NB3]<<CANOPEN_SDO_BYTE_NB3_SHFT);
                
                //check if the given value-container can store the answer
                if(*__received_length<=CANOPEN__DISP_ITEM(*i).length){
                    
                    //copy received length into SDO-info container
                    CANOPEN__DISP_ITEM(*i).length=*__received_length;
                    
                    //send first upload segment request
                    uint8_t toggle_bit=0;
        
                    //set toggle bit
                    if(CANOPEN__DISP_ITEM(*i).flags&CANOPEN_DISP_FLAGS_TG)
                        toggle_bit=CANOPEN_SDO_BYTE_FCT_T_MSK;
                    
                    if(CANopen__send_SDO_response(
                                    CANOPEN__DISP_ITEM(*i).node,              	//remote node
                                    CANOPEN_SDO_FCT_US_ID_SHIFTED|toggle_bit,	//Download Response Command with toggle bit
                                    0,                                          //byte 1
                                    0,                                          //byte 2
                                    0,                                          //byte 3
                                    0,                                          //byte 4
                                    0,                                          //byte 5
                                    0,                                          //byte 6
                                    0                                           //byte 7
                    )!=RC_SUCCESS){
                        //if it was not possible to send the segment request, abort    
                        *__callHandler=TRUE;
                        
                        //set flags
                        CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_E;
                    }
                    
                    //handling done, break search loop
                    break;
                }
                //returned value-size is too big for the container given by the user
                else{
                    //TODO: send abort
                    
                    //the request failed, call the handler
                    *__callHandler=TRUE;
                    
                    //set overflow flag
                    CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_O;
                }
            
            break; /* normal (segmented) transfer  */
            
            //E=1 S=0 contains an unspecified number of bytes to be uploaded
            case CANOPEN_SDO_BYTE_FCT_E_MSK:
                //TODO Not supported by current CANopenProtocol, send abort
            break; /* unspecified number of bytes to be uploaded */
            
            //E=1 S=1 contains data of length 4-n to be downloaded. The encoding depends on the type of data referenced by index and sub-index.
            case CANOPEN_SDO_BYTE_FCT_E_MSK|CANOPEN_SDO_BYTE_FCT_S_MSK:

                //all the data is received in this frame, the handler has to be called in any case
                *__callHandler=TRUE;

                //extract value-length
                *__received_length=CANOPEN_SDO_DATA_MLENGTH-((CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&CANOPEN_SDO_BYTE_FCT_N_MASK)>>CANOPEN_SDO_BYTE_FCT_N_SHIFT);
                    
                //if the received data-length was bigger than the value-container, set overflow flag and set new size
                if(*__received_length>CANOPEN__DISP_ITEM(*i).length)
                    CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_O;
                
                //received value-length fits container, set new size and extract value
                else{
                    CANOPEN__DISP_ITEM(*i).length=*__received_length;

                    //fill value container with  with response
                    for(*j=0;*j<CANOPEN__DISP_ITEM(*i).length;(*j)++)
                        *(uint32_t*)CANOPEN__DISP_ITEM(*i).value|=CAN_in->data.u8[CANOPEN_SDO_BYTE_DATA_START+*j]<<((*j)*BYTE_L);
                }
                break; /* Expedited Transfer */                            
        } /* Switch E/S combination */
    }/* awaited response */
    else{
        //Response was not awaited, break search loop
        *__stop_sched_iteration=TRUE;
    }
}

void CANopen__SDO_blk_hdl(CANOPEN__SDO_C_SUBHDL_PARAMS){
    (void)__result;
    
    //check if the segment-number is correct
    if((CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&(~CANOPEN_SDO_BLK_C_SHIFTED))==((((CANOPEN__DISP_ITEM(*i).transmitted)/CANOPEN_SDO_BLK_DATA_PF)%CANOPEN_CFG_SDO_BLK_SZ)+CANOPEN_SDO_BLK_SEGNO_MIN)){
       
        //determine bytes to read
        if((CANOPEN__DISP_ITEM(*i).length-CANOPEN__DISP_ITEM(*i).transmitted)>CANOPEN_SDO_BLK_DATA_PF)
            *__received_length=CANOPEN_SDO_BLK_DATA_PF;
        else
            //overflow protection
            if(CANOPEN__DISP_ITEM(*i).length>=CANOPEN__DISP_ITEM(*i).transmitted)
                *__received_length=CANOPEN__DISP_ITEM(*i).length-CANOPEN__DISP_ITEM(*i).transmitted;
            else{
        		*__callHandler=TRUE;

				//set overflow flag
				CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_O;
            }
                
        //read data bytes
        for(*j=0;*j<*__received_length;(*j)++)
            *((uint8_t*)CANOPEN__DISP_ITEM(*i).value+CANOPEN__DISP_ITEM(*i).transmitted++)=CAN_in->data.u8[*j+CANOPEN_SDO_SEG_BYTE_F];

        //check c bit (if more segments are coming)
        if(CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&CANOPEN_SDO_BLK_C_SHIFTED){
            //HACK: we are not actually done here, the BLOCK END protocol is missing
            *__callHandler=TRUE;
        }else{
            //if the this was the last segment of a block, we have to request a new block
            if(((CAN_in->data.u8[CANOPEN_SDO_BYTE_FCT]&(~CANOPEN_SDO_BLK_C_SHIFTED))==CANOPEN_CFG_SDO_BLK_SZ)){
                //request next block
                if(CANopen__send_SDO_response(
                    CANOPEN__DISP_ITEM(*i).node,                //remote node
                    CANOPEN_SDO_FCT_BU_BD_SHIFTED|CANOPEN_SDO_BLK_CS_BU,     //Block upload command with client sub-command: start upload
                    CANOPEN_CFG_SDO_BLK_SZ,                                     //byte set Ackseg
                    CANOPEN_CFG_SDO_BLK_SZ,                                     //byte set new blocksize
                    0,                                                          //byte 3
                    0,                                                          //byte 4
                    0,                                                          //byte 5
                    0,                                                          //byte 6
                    0                                                           //byte 7
                )!=RC_SUCCESS){
                    //TODO CAN error-handling
                }
            }
        }
    } /* check if the segment-number is correct */
    else{
        // TODO send abort to server
        
        //call handler, transfer failed
        *__callHandler=TRUE;
        
        //set error flag
        CANOPEN__DISP_ITEM(*i).flags|=CANOPEN_DISP_FLAGS_E;
    }
    
    //message was handled
    *__stop_sched_iteration=TRUE;
}
//**************** SDO Transfer Dispatching *******************************************//

CANopen_SDO_transfer_RET_t CANopen__SDO_ListAdd(CANOPEN__SDO_ADD_DISP_PARAM){
 
    //iterator for the dispatcher list
    CANOPEN_CFG_SDO_DISP_IT __slot_iterator;
    
    //if the dispatcher is full, we do not need to iterate trough the list
    if(CANopen__SDO_Dispatcher_itemCount>=CANOPEN_CFG_SDO_DISP_SIZE)
        return CANOPEN_SDO_DISPATCHER_FULL;
    
//Enter Critical Section
#if(ARCHITECTURE==ARCH_PSOC)
    uint8_t __criticalsection_state=CyEnterCriticalSection();
#endif
    
    //find the next free slot
    for(__slot_iterator=0;__slot_iterator<CANOPEN_CFG_SDO_DISP_SIZE;__slot_iterator++){
    
        //if the slot is free
        if(CANopen__SDO_Dispatcher[__slot_iterator].isActive==FALSE){
            
            //set the slot to "used"
            CANopen__SDO_Dispatcher[__slot_iterator].isActive=TRUE;
            
            //if this is an upload: reset bits in value-container
            if(direction==CANOPEN_SDO_DIR_UP)
                memset (value,0,value_length);

            //add value to Dispatcher list
            CANopen__SDO_Dispatcher[__slot_iterator].item=(CANopen_SDO_dispatcher_element_t){
                transfer_mode,      // transfer mode
                direction,          // direction of the transfer
                COB_ID,             // Awaited COB-ID
                node,               // Node ID of the communication partner
                index,              // Addressed index
                subindex,           // Addressed subIndex
                value,              // Pointer to the data-container
                value_length,       // Length of data in data-container
                0,                  // Number of bytes that already have been transmitted
                callback,           // Pointer to callback function if transmission is completed/timed-out/aborted
                flags,              // Flags of the entry
                0,                  // Abort Code
                timeout,			// TTL
                usr_arg				// User-Argument
            };

            //safe the queue-position of the added entry
            *p_sched_index=__slot_iterator;

            //increment item-Count
            CANopen__SDO_Dispatcher_itemCount++;
     
//Leave Critical Section
#if(ARCHITECTURE==ARCH_PSOC)
            CyExitCriticalSection(__criticalsection_state);
#endif   
                                
            //we are done
            return CANOPEN_SDO_SUCCESS;
        }
    }
    
    //Should be unreachable, but just in case
    return CANOPEN_SDO_DISPATCHER_FULL;
}

void CANopen__SDO_ListDelete(CANOPEN_CFG_SDO_DISP_IT id){
        
    //Enter Critical Section
#if(ARCHITECTURE==ARCH_PSOC)
    uint8_t __criticalsection_state=CyEnterCriticalSection();
#endif
    
    //set entry to unused
    CANopen__SDO_Dispatcher[id].isActive=FALSE;
    
    //decrement item counter
    if(CANopen__SDO_Dispatcher_itemCount>0)
        CANopen__SDO_Dispatcher_itemCount--;
    
//Leave Critical Section
#if(ARCHITECTURE==ARCH_PSOC)
    CyExitCriticalSection(__criticalsection_state);
#endif   

}

void CANopen_Client_Tick(void){

    //iteration variable
    CANOPEN_CFG_SDO_DISP_IT i, active_entries=0;
    
    //iterate trough all entries in the Dispatcher list. If TTL reaches zero, call handler with timeout flag.
    for(i=0;i<CANOPEN_CFG_SDO_DISP_SIZE;i++){
        
        //search for active entries
        if(CANopen__SDO_Dispatcher[i].isActive==TRUE){
            
            //increment "active entries" counter
            active_entries++;
            
            //only decrement entries that are not "listen only"
            if(CANOPEN__DISP_ITEM(i).transfer_mode!=CANOPEN_SDO_mode_listen_only){
                
                //check if TTL is bigger than ticktime (if TTL is over)
                if(CANOPEN__DISP_ITEM(i).TTL>CANOPEN_CFG_TICK_INTERVAL){
                    
                    //decrement TTL
                    CANOPEN__DISP_ITEM(i).TTL-=CANOPEN_CFG_TICK_INTERVAL;
                    
                    //reduce boundary jitter
                    if((CANOPEN__DISP_ITEM(i).TTL)<(CANOPEN_CFG_TICK_INTERVAL/2))
                        CANOPEN__DISP_ITEM(i).TTL=0;
                }
                //TTL is smaller than ticktime (TTL is over)
                else
                    CANOPEN__DISP_ITEM(i).TTL=0;

                //in case message timed out    
                if(CANOPEN__DISP_ITEM(i).TTL==0){
                    
                    //set timeout and ready flag
                    CANOPEN__DISP_ITEM(i).flags |= CANOPEN_DISP_FLAGS_T|CANOPEN_DISP_FLAGS_R;
                    
                    //call handler
                    if(CANOPEN__DISP_ITEM(i).callback(&CANOPEN__DISP_ITEM(i))==RC_SUCCESS){
                    
                        //if the callback returns a success, we delete the entry from the list
                        CANopen__SDO_ListDelete(i);
                    }
                }
            }
            
            //if the number of processed active entries is bigger of equal the number of total active entries, we can stop here
            if(active_entries>=CANopen__SDO_Dispatcher_itemCount)
                break;
        }
    }
}

RC_t CANopen__SDO_iCallback(CANOPEN_SDO_CALLBACK_PARAMS){
    (void)SDO_info;
    return RC_SUCCESS;
}

//**************** SDO Blocking transfer mechanism ***********************************//

CANopen_SDO_transfer_RET_t CANopen__SDO_WaitResponce(CANOPEN_CFG_SDO_DISP_IT dispatcher_id){
    
    //Spinlock waits for the R flag to be set
    while(!(CANOPEN__DISP_ITEM(dispatcher_id).flags&CANOPEN_DISP_FLAGS_R)){/* busy wait */}
        
    //timeout
    if(CANOPEN__DISP_ITEM(dispatcher_id).flags&CANOPEN_DISP_FLAGS_T)
        return CANOPEN_SDO_TIMEOUT;
    
    //abort
    if(CANOPEN__DISP_ITEM(dispatcher_id).flags&CANOPEN_DISP_FLAGS_A)
        return CANOPEN_SDO_ABORT;
    
    //value-container overflow
    if(CANOPEN__DISP_ITEM(dispatcher_id).flags&CANOPEN_DISP_FLAGS_O)
        return CANOPEN_SDO_VALUE_SIZE;
    
    //internal error
    if(CANOPEN__DISP_ITEM(dispatcher_id).flags&CANOPEN_DISP_FLAGS_E)
        return CANOPEN_SDO_INTERNAL_ERROR;

    return CANOPEN_SDO_SUCCESS;
}

/* [] END OF FILE */
