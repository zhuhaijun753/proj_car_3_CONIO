/**
 * \file 	CAN.cpp
 * \author 	Thomas Barth	-	thomas.barth@h-da.de
 * \date 	03.06.2018
 * \version	0.2
 *
 * \brief AURIX CAN driver
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
#include "CAN.h"
#include "CAN_config.h"
#include "system.h"
//#include "InterruptRouter.h"
#include "logging.h"
#include "clock.h"

//-------------------------------------------------------------------- [Types]

/** \brief CAN clock selection
 *
 * The CAN module timer clock fCAN of the functional blocks of the MultiCAN+ module is derived from the asynchronous, higher precision clock fA or from the synchronous clock
 * source. The Fractional Divider is used to generate fCAN used for bit timing calculation. The frequency of fCAN is identical for all CAN nodes. The register file operate with the
 * module control clock fCLC. See also “MultiCAN+ Clock Generation” on Page 22-21.
 */
typedef enum
{
    CAN_ClockSelect_noClock  	= 0b0000, 				/**< \brief No clock supplied. */
    CAN_ClockSelect_fasyn_CAN   = 0b0001, 				/**< \brief fASYN_CAN. */
    CAN_ClockSelect_fOSC    	= 0b0010, 				/**< \brief Oscillator Clock. */
	CAN_ClockSelect_fERAY    	= 0b0100, 				/**< \brief E-Ray Clock fERAY. */
	CAN_ClockSelect_HW    		= 0b1000, 				/**< \brief Hard wire to 0. */
} CAN_ClkSel_t;


/** \brief CAN Message Object Mode
 *
 * Besides serving as a storage container for incoming and outgoing frames, message objects can be combined to build
 * gateways between the CAN nodes or to setup a FIFO buffer.
 */
typedef enum{
	CAN_MObj_std		= 	0b0000,						/**< \brief standard message object */
	CAN_MObj_rFIFO_B	= 	0b0001,						/**< \brief Receive FIFO Base Object */
	CAN_MObj_tFIFO_B	= 	0b0010,						/**< \brief Transmit FIFO Base Object */
	CAN_MObj_tFIFO_S	= 	0b0011,						/**< \brief Transmit FIFO Slave Object */
	CAN_MObj_gtw		= 	0b0100,						/**< \brief Gateway source Object */
	CAN_MObj_CANFD		= 	0b0101,						/**< \brief CANFD 64 bytes Message Mode */
} __CAN_MObj_t;


/** \brief CAN priority Classes
 *
 * #__CAN_MObj_prio_t assigns one of the four priority classes 0, 1, 2, 3 to message object n.
 * A lower PRI number defines a higher priority. Message objects with lower #__CAN_MObj_prio_t value always win acceptance filtering for frame reception
 * and transmission over message objects with higher #__CAN_MObj_prio_t value. Acceptance filtering based on identifier/mask and list position is performed only
 * between message objects of the same priority class. #__CAN_MObj_prio_t also determines the acceptance filtering method for transmission.
 */
typedef enum{
	/** \brief  Transmit acceptance filtering based on list order
	 *
	 * Transmit acceptance filtering is based on the list order. This means that message object n is considered for transmission only if there is no
	 * other message object with valid transmit request (MSGVAL & TXEN0 & TXEN1 = 1) somewhere before this object in the list.
	*/
    CAN_MOBJ_prio_tl	=0b01,

	/** \brief  Transmit acceptance filtering based on CAN identifier
	 *
	 * Transmit acceptance filtering is based on the CAN identifier. This means, message object n is considered for transmission only if there is no
	 * other message object with higher priority identifier + IDE + DIR (with respect to CAN arbitration rules) somewhere in the list.
	*/
    CAN_MOBJ_prio_ti	=0b10,

	/** \brief  Transmit acceptance filtering is based on the list order
	 *
	 * Transmit acceptance filtering is based on the list order (as #CAN_MOBJ_prio_tl)
	*/
    CAN_MOBJ_prio_lo	=0b11,
} __CAN_MObj_prio_t;


/** \brief CAN Control Panel commands
 *
 * The list structure cannot be modified directly by write accesses to the LIST registers and the PPREV, PNEXT and LIST bit fields in the Message Object Status Registers, as they
 * are read only. The list structure is managed by and limited to the list controller inside the MultiCAN+ module. The list controller is controlled via a command panel allowing the
 * user to issue list allocation commands to the list controller. The list controller has two main purposes:
 *
 * 	-	1. Ensure that all operations that modify the list structure result in a consistent list structure.
 * 	-	2. Present maximum ease of use and flexibility to the user.
 *
 * 	The list controller and the associated command panel allows the programmer to concentrate on the final properties of the list, which are characterized by the allocation
 * 	of message objects to a CAN node, and the ordering relation between objects that are allocated to the same list. The process of list (re-)building is done in the list controller.
 */
typedef enum{
	/** \brief No operation.
	 *
	 * PANAR2 -;	PANAR 1 -;
	 * Writing 00H to PANCMD has no effect. No new command is started.
	 */
	__CAN_PANCMD_NOP          	= 0x0,

	/** \brief Initialize Lists
	 *
	 * PANAR2 Result -> Bit 7: ERR Bit 6-0: undefined;	PANAR 1 -;
	 * Run the initialization sequence to reset the CTRL and LIST fields of all message objects. List registers LIST[7:0] are set to their reset values.
	 * This results in the deallocation of all message objects. The initialization command requires that bits NCRx.INIT and NCRx.CCE are set for all CAN nodes.
	 * Bit 7 of PANAR2 (ERR) reports the success of the operation:
	 * 	- 	0B Initialization was successful
	 * 	-	1B Not all NCRx.INIT and NCRx.CCE bits are set. Therefore, no initialization is performed.
	 * 	The initialize lists command is automatically performed with each reset of the MultiCAN+ module, but with the exception that all message object registers are reset, too.
	 */
	__CAN_PANCMD_INIT_L         = 0x1,

	/** \brief Static Allocate
	 *
	 * PANAR2 Argument -> List index;	PANAR 1 Argument -> Message Object Number;
	 * Allocate message object to a list. The message object is removed from the list that it currently belongs to, and appended to the end of the list, given by PANAR2.
	 * This command is also used to deallocate a message object. In this case, the target list is the list of unallocated elements (PANAR2 = 0).
	 */
	__CAN_PANCMD_STATIC_A       = 0x2,

	/** \brief Dynamic Allocate
	 *
	 * PANAR2 Argument -> List Index Result -> Bit 7: ERR Bit 6-0: undefined;	PANAR 1 Argument -> Message Object Number;
	 * Allocate the first message object of the list of unallocated objects to the selected list. The message object is appended to the end of the list. The message number
	 * of the message object is returned in PANAR1. An ERR bit (bit 7 of PANAR2) reports the success of the operation:
	 * 	-	0B Success.
	 * 	-	1B The operation has not been performed because the list of unallocated elements was empty.
	 */
	__CAN_PANCMD_DYN_A         	= 0x3,

	/** \brief Static Insert Before
	 *
	 * PANAR2 Argument -> Destination Object Number;	PANAR 1 Argument -> Source Object Number;
	 * Remove a message object (source object) from the list that it currently belongs to, and insert it before a given destination object into the list structure of
	 * the destination object. The source object thus becomes the predecessor of the destination object.
	 */
	__CAN_PANCMD_SIB         	= 0x4,

	/** \brief Dynamic Insert Before
	 *
	 * PANAR2 Argument -> Destination Object Number Result -> Bit 7: ERR Bit 6-0: undefined	PANAR 1 Argument -> Object Number of inserted object;
	 * Insert a new message object before a given destination object. The new object is taken from the list of unallocated elements (the first element is chosen).
	 * The number of the new object is delivered as a result to PANAR1. An ERR bit (bit 7 of PANAR2) reports the success of the operation:
	 * 	-	0B Success.
	 * 	-	1B The operation has not been performed because the list of unallocated elements was empty.
	 */
	__CAN_PANCMD_DIB         	= 0x5,

	/** \brief Static Insert Behind
	 *
	 * PANAR2 Argument -> Destination Object Number;	PANAR 1 Argument -> Source Object Number;
	 * Remove a message object (source object) from the list that it currently belongs to, and insert it behind a given destination object into the list structure of
	 * the destination object. The source object thus becomes the successor of the destination object.
	 */
	__CAN_PANCMD_SIA         	= 0x6,

	/** \brief Dynamic Insert Behind
	 *
	 * PANAR2 Argument -> Destination Object Number Result -> Bit 7: ERR Bit 6-0: undefined	PANAR 1 Argument -> Object Number of inserted object;
	 * Insert a new message object behind a given destination object. The new object is taken from the list of unallocated elements (the first element is chosen).
	 * The number of the new object is delivered as result to PANAR1. An ERR bit (bit 7 of PANAR2) reports the success of the operation:
	 * 	-	0B Success.
	 * 	-	1B The operation has not been performed because the list of unallocated elements was empty
	 */
	__CAN_PANCMD_DIA         	= 0x7,
}__CAN_PANCMD_t;


//-------------------------------------------------------------------- [local prototypes]

/**
 * \brief Initializes the CAN Module
 *
 * Activates and configures the clock and disables sleep mode requests for CAN.
 * This function performs a kernel reset of the CAN module, which also resets the CAN object list and all nodes
 * \return  #RC_SUCCESS			Init of CAN Module succeeded
 *          #RC_ERROR_TIME_OUT	CLC activation or kernel reset timed out
 * \warning	This function requires ENDINIT and might cause ENDINIT Traps
 */
static RC_t CAN__initModule();

/**
 * \brief Initializes a CAN Node
 *
 * The AURIX provides a number of CAN Nodes, that is defined in #DERIVATE_NUM_CAN_N.
 * This function initializes the Node, according to its entry in #CANNodeConfig
 * \param	node				Node to be configures. Must not be greater or equal #DERIVATE_NUM_CAN_N
 * \return  #RC_SUCCESS			Initialization of CAN Node succeeded or node is inactive
 *          #RC_ERROR_BAD_PARAM	Illegal Node-ID
 */
static RC_t CAN__NodeInit(uint8_t node);

/**
 * \brief Initializes all message objects
 *
 * The AURIX provides a number of CAN Message object, which is defined in #DERIVATE_NUM_CAN_OBJ.
 * These message objects define which message frames are accepted and stores the data of the frames.
 * Message objects are used for send and receive operation and can be used to build FIFO buffers or gateways
 * This function initializes the objects based on #CANopen_Rx_Mailboxes.
 * \param	NodeID				CANopen Node-Id. The Node ID is required to calculate the matching identifiers for the objects.
 * \return  #RC_SUCCESS			Initialization of CAN Objects succeeded
 *          #RC_ERROR_BAD_DATA	Illegal configuration in #CANopen_Rx_Mailboxes. Number of objects exceeds #DERIVATE_NUM_CAN_OBJ
 */
static RC_t CAN__MObjInit(uint8_t NodeID);

/**
 * \brief Sends a command to the CAN Control panel
 *
 * The CAN message objects are organized in lists, in order to assign them to certain nodes.
 * This assignment is managed be the CAN Control panel. This function sends commands specified in #__CAN_PANCMD_t
 * \param	cmd					Command to be send, see #__CAN_PANCMD_t.
 * \param	arg1				Argument/Result 1
 * \param	arg2				Argument/Result 2
 * \return  #RC_SUCCESS			Command was executed
 *          #RC_ERROR_BUSY		Command panel is busy
 *          #RC_ERROR_TIME_OUT	Command timed out. Timeout is given by #CAN_CFG_PAN_TO
 * \warning	This functions uses a spinlock to wait for the command to be executed
 */
static RC_t __CAN_PANCMD(__CAN_PANCMD_t cmd, uint8_t* arg1, uint8_t* arg2);

//-------------------------------------------------------------------- [local data]

//message object selector
#pragma section ".com.can"

static uint8_t	CAN__MObj_SEL[DERIVATE_NUM_CAN_N]={CANOPEN_EOR_ID};
service_state_t CAN__state=service_UNDEFINED;
CAN_TX_BUF_t CAN_TX_BUF;
//TODO: CAN_TX_BUF_t CAN_TX_BUF;
#pragma section

//-------------------------------------------------------------------- [local functions]

static RC_t CAN__initModule(){

	//result buffer
	RC_t __result;

	//enable module, using default timeout
	__result=SYSTEM_Module_CLC_Start((CLC_Bits*)&MODULE_CAN.CLC,0);

	//check for timeouts
	if(__result!=RC_SUCCESS)
		return __result;

	/* perform a kernel reset
	 * This will also reset the message object list by assigning all message objects to list 0.
	 * Also, this will put all CAN Nodes into INIT mode and release the write protection.
	 */
	__result=SYSTEM_Module_KRESET((KRST0_Bits*)&MODULE_CAN.KRST0,(KRST1_Bits*)&MODULE_CAN.KRST1,(KRSTCLR_Bits*)&MODULE_CAN.KRSTCLR,0);

	//check for timeouts
	if(__result!=RC_SUCCESS)
		return __result;

//Set up timing
	SYSTEM_UnlockENDINIT();

	/* select clock
	 * The reconfiguration of the clock source has to be done by using two writes:
	 * first a write of zero to the CLKSEL bit field, and then a second write defining the new clock source
	 */
	MODULE_CAN.MCR.B.CLKSEL = CAN_ClockSelect_noClock;
	MODULE_CAN.MCR.B.CLKSEL = CAN_CFG_CLKSEL;

	//CAN clock divider control
	MODULE_CAN.FDR.B.DM 	= CAN_CFG_CD_DM;
	MODULE_CAN.FDR.B.STEP 	= CAN_CFG_FDR_STEP;

	SYSTEM_LockENDINIT();
//timing is set up

	//we use static allocation for the message pending bits
	MODULE_CAN.MCR.B.MPSEL=0x0;

	//Module initialized
	return RC_SUCCESS;
}

static RC_t CAN__NodeInit(uint8_t node){

	//local buffer for bit timing configuration which shall be written at once
	Ifx_CAN_N_BTR ___btr;

	//check if node-id is valid
	if(node>=DERIVATE_NUM_CAN_N)
		return RC_ERROR_BAD_PARAM;

	//configure CAN node bus association
	if(CANNodeConfig[node].baudrate!=CAN_inactive){
		if(CANNodeConfig[node].baudrate!=CAN_loopback){
			MODULE_CAN.N[node].PCR.B.RXSEL=CANNodeConfig[node].rxSelect;
			MODULE_CAN.N[node].PCR.B.LBM=OFF;
		}
		else
			MODULE_CAN.N[node].PCR.B.LBM=ON;
	}
	//Node is inactive
	else
		return RC_SUCCESS;

	//clear all flags
	___btr.U=0;

	//Prescale Multiplier configuration
	___btr.B.DIV8  = CAN_CFG_DIV8;

	//calculate prescaler
	switch(CANNodeConfig[node].baudrate){

#if(CAN_CFG_CD_DM==0b01&&CAN_CFG_DIV8==0b0)
		case CAN_1Mbps:
			___btr.B.BRP   = ((Clock_get_Frequency(fCAN)/(1024-CAN_CFG_FDR_STEP))/20000000)-1;
			break;
		case CAN_500kbps:
			___btr.B.BRP   = ((Clock_get_Frequency(fCAN)/(1024-CAN_CFG_FDR_STEP))/40000000)-1;
			break;
		case CAN_250kbps:
			___btr.B.BRP   = ((Clock_get_Frequency(fCAN)/(1024-CAN_CFG_FDR_STEP))/80000000)-1;
			break;
		case CAN_125kbps:
			___btr.B.BRP   = ((Clock_get_Frequency(fCAN)/(1024-CAN_CFG_FDR_STEP))/160000000)-1;
			break;

		default:
			___btr.B.BRP=0;
#else
#error "This configuration mode is not fully implemented"
#endif
	}

	//set up sampling points
	___btr.B.SJW   = CAN_CFG_SJW;
	___btr.B.TSEG1 = CAN_CFG_TSEG1;
	___btr.B.TSEG2 = CAN_CFG_TSEG2;

	//write bit timing configuration
	MODULE_CAN.N[node].BTR.U = ___btr.U;

	//node Init done
	return RC_SUCCESS;
}

static RC_t CAN__MObjInit(uint8_t NodeID){

	//message object iterator
	uint8_t		__MObj_i=0;

	//mailbox iterator
	uint16_t	__Mbx_i=0;

	//CAN Node iterator
	uint8_t		__node_i;

	//CAN List buffer for Panel command
	uint8_t		__can_L_id;

	//configuration list entry iterator
	uint8_t		__cfg_lst_i=0;

//TX Mailboxes
	//each active node gets 1 TX mailbox
	for(__node_i=0;__node_i<DERIVATE_NUM_CAN_N;__node_i++)
		if(CANNodeConfig[__node_i].baudrate!=CAN_inactive){

			//Set all of this mailboxes as standard mailbox
			MODULE_CAN.MO[__node_i].FCR.B.MMC=CAN_MObj_std;

			//enable TX Interrupt from Mailbox
			MODULE_CAN.MO[__node_i].FCR.B.TXIE=ON;

			//select Interrupt line for TX messages
			MODULE_CAN.MO[__node_i].IPR.B.TXINP=CAN_CFG_TX_IR_L;

			//set up all objects to only send standard CAN format (11Bit ID)
			MODULE_CAN.MO[__node_i].AR.B.IDE=OFF;

			//clear RTR
			MODULE_CAN.MO[__node_i].CTR.U=BIT(IFX_CAN_MO_CTR_SETDIR_OFF);

			//set up internal arbitration based on the message-ID
			MODULE_CAN.MO[__node_i].AR.B.PRI=CAN_MOBJ_prio_ti;

			//set Mailbox into TX Mode
			MODULE_CAN.MO[__node_i].CTR.U=BIT(IFX_CAN_MO_CTR_RESTXRQ_OFF)|BIT(IFX_CAN_MO_CTR_SETTXEN1_OFF)|BIT(IFX_CAN_MO_CTR_SETTXEN0_OFF);

			//Add object the Node Object-List
			__can_L_id=__node_i+1;
			__CAN_PANCMD(__CAN_PANCMD_STATIC_A,&__node_i,&__can_L_id);

	}

	//FIFO Mailboxes start after TX objects
	__MObj_i=DERIVATE_NUM_CAN_N;

	//while we have entries in CANopen_Tx_Mailboxes
	while(CANopen_Tx_Mailboxes[__cfg_lst_i].Node!=CANOPEN_EOR_ID){

		//check if specified node is active
		if(CANNodeConfig[CANopen_Tx_Mailboxes[__cfg_lst_i].Node].baudrate!=CAN_inactive){

				//check if we need additional mailboxes
				if(CANopen_Tx_Mailboxes[__cfg_lst_i].buffer_depth!=1){

					//set base object bottom
					MODULE_CAN.MO[CANopen_Tx_Mailboxes[__cfg_lst_i].Node].FGPR.B.BOT=__MObj_i;

					//set base object CUR to first slave object
					MODULE_CAN.MO[CANopen_Tx_Mailboxes[__cfg_lst_i].Node].FGPR.B.CUR=__MObj_i;

					//set base object SEL to first object
					//MODULE_CAN.MO[CANopen_Tx_Mailboxes[__cfg_lst_i].Node].FGPR.B.SEL=CANopen_Tx_Mailboxes[__cfg_lst_i].Node;
					CAN__MObj_SEL[CANopen_Tx_Mailboxes[__cfg_lst_i].Node]=CANopen_Tx_Mailboxes[__cfg_lst_i].Node;

					for(__Mbx_i=1;__Mbx_i<CANopen_Tx_Mailboxes[__cfg_lst_i].buffer_depth; __Mbx_i++){

						//Set mailbox as standard objects
						MODULE_CAN.MO[__MObj_i].FCR.B.MMC=CAN_MObj_std;

						//Set base object for all slave objects
						MODULE_CAN.MO[__MObj_i].FGPR.B.CUR=CANopen_Tx_Mailboxes[__cfg_lst_i].Node;

						//enable TX Interrupt from Mailbox
						MODULE_CAN.MO[__MObj_i].FCR.B.TXIE=ON;

						//select Interrupt line for TX messages
						MODULE_CAN.MO[__MObj_i].IPR.B.TXINP=CAN_CFG_TX_IR_L;

						//set up all objects to only send standard CAN format (11Bit ID)
						MODULE_CAN.MO[__MObj_i].AR.B.IDE=OFF;

						//clear RTR
						MODULE_CAN.MO[__MObj_i].CTR.U=BIT(IFX_CAN_MO_CTR_SETDIR_OFF);

						//set up internal arbitration based on the message-ID
						MODULE_CAN.MO[__MObj_i].AR.B.PRI=CAN_MOBJ_prio_ti;

						//set Mailbox into TX Mode
						MODULE_CAN.MO[__MObj_i].CTR.U=BIT(IFX_CAN_MO_CTR_RESTXRQ_OFF)|BIT(IFX_CAN_MO_CTR_SETTXEN1_OFF)|BIT(IFX_CAN_MO_CTR_SETTXEN0_OFF)|BIT(IFX_CAN_MO_CTR_SETMSGVAL_OFF);

						//Add object the Node Object-List
						__can_L_id=CANopen_Tx_Mailboxes[__cfg_lst_i].Node+1;
						__CAN_PANCMD(__CAN_PANCMD_STATIC_A,&__MObj_i,&__can_L_id);

						//increment object count
						__MObj_i++;
					}

					//set base object TOP
					MODULE_CAN.MO[CANopen_Tx_Mailboxes[__cfg_lst_i].Node].FGPR.B.TOP=__MObj_i-1;
				}
			}
		//go to next entry
		__cfg_lst_i++;
	}


//RX Mailboxes
	//iterate trough RX mailbox list
	__cfg_lst_i=0;
	while(CANopen_Rx_Mailboxes[__cfg_lst_i].Node!=CANOPEN_EOR_ID){

		//check if mailbox number exceeded number of available message objects
		if(__MObj_i==(DERIVATE_NUM_CAN_OBJ-1))
			return RC_ERROR_BAD_DATA;

		//In our approach, the message pending bit position is defined by the message id
		MODULE_CAN.MO[__MObj_i].IPR.B.MPN=__MObj_i;

		//association between hard and soft mailboxes. For now we store the soft mailbox ID in the software pointer register.
		//This is only possible because we have not FIFO active
		//TODO: FIFO configuration
		MODULE_CAN.MO[__MObj_i].FGPR.B.SEL=__cfg_lst_i;

		//set MSPDND mask for message pending bit
		MODULE_CAN.MSIMASK.B.IM|=BIT(LO4(__MObj_i));

		//enable RX Interrupt from Mailbox
		MODULE_CAN.MO[__MObj_i].FCR.B.RXIE=ON;

		//select Interrupt line for RX messages
		MODULE_CAN.MO[__MObj_i].IPR.B.RXINP=CAN_CFG_RX_IR_L;

		//set up object to only accept messages with matching IDE Bit
		MODULE_CAN.MO[__MObj_i].AMR.B.MIDE=ON;

		//set up all objects to only accept standard CAN format (11Bit ID)
		MODULE_CAN.MO[__MObj_i].AR.B.IDE=OFF;

		//set up internal arbitration based on the message-ID
		MODULE_CAN.MO[__MObj_i].AR.B.PRI=CAN_MOBJ_prio_ti;

		//configure RTR acceptance filtering and set Mailbox into RX Mode
		if(CANopen_Rx_Mailboxes[__cfg_lst_i].RTR==TRUE)
			MODULE_CAN.MO[__MObj_i].CTR.U=BIT(IFX_CAN_MO_CTR_SETRXEN_OFF)|BIT(IFX_CAN_MO_CTR_SETMSGVAL_OFF)|BIT(IFX_CAN_MO_CTR_SETDIR_OFF);
		else
			MODULE_CAN.MO[__MObj_i].CTR.U=BIT(IFX_CAN_MO_CTR_SETRXEN_OFF)|BIT(IFX_CAN_MO_CTR_SETMSGVAL_OFF);

		//set up arbitration and address masking
		switch(CANopen_Rx_Mailboxes[__cfg_lst_i].range){
			case CANopen_range_fct:
				//Set Address arbitration register
				MODULE_CAN.MO[__MObj_i].AR.B.ID= (((uint32_t)CANopen_Rx_Mailboxes[__cfg_lst_i].fctCODE)<<CAN_ACR_ID_SHIFT);

				// Set the Address masking register
				MODULE_CAN.MO[__MObj_i].AMR.B.AM= (CANOPEN_FCT_MSK<<CAN_ACR_ID_SHIFT);
				break;

			 case CANopen_range_node:
				//Set Address arbitration register
				 MODULE_CAN.MO[__MObj_i].AR.B.ID= (((uint32_t)CANopen_Rx_Mailboxes[__cfg_lst_i].fctCODE+NodeID)<<CAN_ACR_ID_SHIFT);

				// Set the Address masking register
				MODULE_CAN.MO[__MObj_i].AMR.B.AM= CAN_AMR_NONE;
				break;

			case CANopen_range_explicit:
				//Set Address arbitration register
				MODULE_CAN.MO[__MObj_i].AR.B.ID=  (((uint32_t)CANopen_Rx_Mailboxes[__cfg_lst_i].fctCODE)<<CAN_ACR_ID_SHIFT);

				// Set the Address masking register
				MODULE_CAN.MO[__MObj_i].AMR.B.AM= CAN_AMR_NONE;
				break;
			//illegal enum
			default:
				return RC_ERROR_BAD_DATA;
		}

		//Add object the Node Object-List
		__can_L_id=CANopen_Rx_Mailboxes[__cfg_lst_i].Node+1;
		__CAN_PANCMD(__CAN_PANCMD_STATIC_A,&__MObj_i,&__can_L_id);

		//increment object ID
		__MObj_i++;

		//go to next list entry
		__cfg_lst_i++;
	}

	return RC_SUCCESS;
}

static RC_t __CAN_PANCMD(__CAN_PANCMD_t cmd, uint8_t* arg1, uint8_t* arg2){

	//check if command panel is busy
	if(MODULE_CAN.PANCTR.B.BUSY!=FALSE)
		return RC_ERROR_BUSY;

	//command buffer
	Ifx_CAN_PANCTR __panCMD;

	//timeout
	uint16_t __timeout=CAN_CFG_PAN_TO;

	//argument 1
	__panCMD.B.PANAR1 = *arg1;

	//argument 2
	__panCMD.B.PANAR2 = *arg2;

	//command
	__panCMD.B.PANCMD = cmd;

	//write command with arguments in single 32bit write
	MODULE_CAN.PANCTR.U = __panCMD.U;

	// wait until command has been executed and parameters got updated
	while (MODULE_CAN.PANCTR.B.BUSY != 0||MODULE_CAN.PANCTR.B.RBUSY != 0){
		if(__timeout--==0)
			return RC_ERROR_TIME_OUT;
	}

	//read back results
	__panCMD.U=MODULE_CAN.PANCTR.U;
	*arg1=__panCMD.B.PANAR1;
	*arg2=__panCMD.B.PANAR2;

	return RC_SUCCESS;
}

//-------------------------------------------------------------------- [global functions]

RC_t CAN_init(uint8_t NodeID){

	//node iterator
	uint8_t 	__node_i;

	//timeout buffer for PANCTR busy check
	uint16_t	__timeout;

	//result buffer
	RC_t 		__result;

	//Init CAN Module
	__result=CAN__initModule();

	//check if init of CAN succeeded
	if(__result!=RC_SUCCESS){
		CAN__state=service_UNAVAILABLE;
		return __result;
	}

	//activate Nodes
	for(__node_i = 0; __node_i < DERIVATE_NUM_CAN_N; __node_i++){
		__result=CAN__NodeInit(__node_i);
		if(__result!=RC_SUCCESS){
			CAN__state=service_UNAVAILABLE;
			return __result;
		}
	}

	//wait for the message object list to initialize
	__timeout=CAN_CFG_INI_TO;
	while(MODULE_CAN.PANCTR.B.BUSY!=FALSE){
		if(__timeout--==0){
			CAN__state=service_UNAVAILABLE;
			return RC_ERROR_TIME_OUT;
		}
	}

	//initialize Message objects
	__result=CAN__MObjInit(NodeID);

	//check if init of message objects succeeded
	if(__result!=RC_SUCCESS){
		CAN__state=service_UNAVAILABLE;
		return __result;
	}

	//Enable Interrupt lines (see #CAN_CFG_RX_IR_L and #CAN_CFG_TX_IR_L for interrupt line allocation)
	//InterruptRouter_CfgSRC(&SRC_CANINT0);
	//InterruptRouter_CfgSRC(&SRC_CANINT1);

	//reset CCE and INIT flag for CAN Nodes, so they participate on the bus
	for(__node_i = 0; __node_i < DERIVATE_NUM_CAN_N; __node_i++)
		if(CANNodeConfig[__node_i].baudrate!=CAN_inactive){
			MODULE_CAN.N[__node_i].CR.B.CCE	=OFF;
			MODULE_CAN.N[__node_i].CR.B.INIT=OFF;
		}

	//CAN_init finished successfully
	CAN__state=service_READY;
	return RC_SUCCESS;
}



RC_t CAN_Frame_Read(CAN_RX_t* rx_msg, uint8_t* mbx_id){

	//Message Object buffer
	uint8_t 	__MObj_i;

	//list iterator
	uint8_t		__LMOBJ_i;

	//Check if CAN had been initialized
	if(CAN__state!=service_READY)
		return RC_ERROR_INVALID_STATE;

	// Iterate trough message pending lists in static allocation MPSEL
	for(__LMOBJ_i=0;__LMOBJ_i<(DERIVATE_NUM_CAN_OBJ/sizeof(uint32_t));__LMOBJ_i++){

		//check if we have pending bits
		if(MODULE_CAN.MSID[__LMOBJ_i].B.INDEX!=0b100000){

			//get index
			__MObj_i=(__LMOBJ_i<<NIBBLE_L)|MODULE_CAN.MSID[__LMOBJ_i].B.INDEX;

			//get soft-mailbox ID
			if(mbx_id!=NULL){
				*mbx_id=MODULE_CAN.MO[__MObj_i].FGPR.B.SEL;
			}

			//read frame from hardware
			do{
				//Clear NEWDAT flag
				MODULE_CAN.MO[__MObj_i].CTR.U=BIT_1<<IFX_CAN_MO_CTR_RESNEWDAT_OFF;

				//the frame is cleared from hardware even if the user has not provided a storage container.
				if(rx_msg!=NULL){
					//Read message meta data from the message object
					rx_msg->frame.DLC	=	MODULE_CAN.MO[__MObj_i].FCR.B.DLC;
					rx_msg->frame.MsgID	=	MODULE_CAN.MO[__MObj_i].AR.B.ID>>CAN_ACR_ID_SHIFT;

					//Read message content from the message object
					rx_msg->frame.dataL.U	= MODULE_CAN.MO[__MObj_i].DATAL.U;
					rx_msg->frame.dataH.U	= MODULE_CAN.MO[__MObj_i].DATAH.U;

					//Set pointer to message object in user-argument
					rx_msg->p_MObj	= &MODULE_CAN.MO[__MObj_i];
				}

			//check if a new frame has been received in the meantime and data might got corrupted
			}while(MODULE_CAN.MO[__MObj_i].STAT.B.NEWDAT==TRUE||MODULE_CAN.MO[__MObj_i].STAT.B.RXUPD==TRUE);

			//clear pending bit
			MODULE_CAN.MSPND[__LMOBJ_i].B.PND&= ~(BIT_1 << MODULE_CAN.MSID[__LMOBJ_i].B.INDEX);

			//message was read
			return RC_SUCCESS;
		}
	}

	//we have an interrupt but no pending data, something is spooky!
	return RC_ERROR_READ_FAILS;

}

RC_t CAN_Frame_Write(uint8_t node, CAN_Frame_t* tx_msg){

	//Check if CAN had been initialized
	if(CAN__state!=service_READY)
		return RC_ERROR_INVALID_STATE;

	//check if mailbox is free
	if(MODULE_CAN.MO[CAN__MObj_SEL[node]].STAT.B.TXRQ==FALSE){
		//Mailbox is free, send frame directly
		CAN__frame_write_HW(tx_msg);
	}
	//Mailbox is not free
	else{
		//put frame into queue
		if(CAN_TX_BUF.writeData(*tx_msg) ==TRUE)
			return RC_SUCCESS;
		else
			return RC_ERROR_BUFFER_FULL;
	}

	//Increment SEL
//HACK: Iteration trough TX mailboxes is currently deactivated because of TX FIFO issues.
//	if(MODULE_CAN.MO[CAN__MObj_SEL[node]].STAT.B.PNEXT!=MODULE_CAN.MO[node].FGPR.B.TOP)
//		CAN__MObj_SEL[node]=MODULE_CAN.MO[MODULE_CAN.MO[node].FGPR.B.SEL].STAT.B.PNEXT;
//	else
//		CAN__MObj_SEL[node]=node;

	return RC_SUCCESS;
}

void CAN__frame_write_HW(CAN_Frame_t* tx_msg){

	//set message as not valid
	MODULE_CAN.MO[CAN__MObj_SEL[0]].CTR.U=BIT(IFX_CAN_MO_CTR_RESMSGVAL_OFF);

	//write message meta data to the message object
	MODULE_CAN.MO[CAN__MObj_SEL[0]].FCR.B.DLC	= tx_msg->DLC;
	MODULE_CAN.MO[CAN__MObj_SEL[0]].AR.B.ID		= tx_msg->MsgID<<CAN_ACR_ID_SHIFT;

	//write message content to the message object
	MODULE_CAN.MO[CAN__MObj_SEL[0]].DATAL.U		= tx_msg->dataL.U;
	MODULE_CAN.MO[CAN__MObj_SEL[0]].DATAH.U		= tx_msg->dataH.U;

	//set transmit request
	MODULE_CAN.MO[CAN__MObj_SEL[0]].CTR.U=BIT(IFX_CAN_MO_CTR_SETTXRQ_OFF)|BIT(IFX_CAN_MO_CTR_SETNEWDAT_OFF)|BIT(IFX_CAN_MO_CTR_RESRTSEL_OFF)|BIT(IFX_CAN_MO_CTR_SETMSGVAL_OFF);
}
