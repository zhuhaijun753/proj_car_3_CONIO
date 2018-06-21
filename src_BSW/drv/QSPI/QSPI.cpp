/**
 * \file	QSPI.c
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	07.05.2017
 * \version	0.1
 *
 * \brief QSPI driver implementation
 *
 * Changelog:\n
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
#include "global.h"
#include "system.h"
#include "register.h"
#include <string.h>
#include "clock.h"

#include "QSPI_cfg.h"
#include "priv/QSPI_priv.h"

//#include "InterruptRouter.h"


#pragma section ".com.spi"
/** \brief QSPI Driver global data structure */
static QSPI_module_priv_t 	   QSPI_mod[DERIVATE_QSPI_NUM_MOD];
#pragma section

/*
 * QSPI RX ISR.
 * @param module_num : module ID
 * @return void
 */
void _QSPI_RX_ISR(int module_num)
{
}

/*
 * QSPI TX ISR.
 * @param module_num : module ID
 * @return void
 */
void _QSPI_TX_ISR(int module_num)
{
	volatile RC_t ret;

	if (DERIVATE_QSPI_NUM_MOD > module_num)
	{
		ret = sQSPI_module_transact(&QSPI_mod[module_num], QSPI_exchange_write);
		if (ret != RC_ERROR_BUFFER_EMTPY && ret != RC_SUCCESS)
		{
			asm("nop");
		}
	}

	// Handling of incorrect module_num error required?
}

/*
 * QSPI ERROR ISR.
 * @param module_num : module ID
 * @return void
 */
void _QSPI_ERROR_ISR(int module_num)
{
	if (DERIVATE_QSPI_NUM_MOD > module_num)
	{
		sQSPI_module_errorHandler(&QSPI_mod[module_num]);
	}

	// Handling of incorrect module_num error required?
}

/*
 * QSPI PHASE TRANSITION ISR.
 * @param module_num : module ID
 * @return void
 */
void _QSPI_PHASE_TRANSITION_ISR(int arg)
{
	RC_t ret;

	if (DERIVATE_QSPI_NUM_MOD > arg)
	{
		// This ISR is generated on RBF (Receive Buffer Filled) Event.
		// This is used for Reading packets.

		ret = sQSPI_module_transact(&QSPI_mod[arg], QSPI_exchange_read);
		if (ret != RC_ERROR_BUFFER_FULL && ret != RC_SUCCESS)
		{
			asm("nop");
		}
	}

	// Handling of incorrect module_num error required?
}


RC_t QSPI_module_init(QSPI_Module_ID_t ModuleID, QSPI_module_t *apModule)
{
	QSPI_module_priv_t				*pModule = NULL;

	if (DERIVATE_QSPI_NUM_MOD <= ModuleID)
		return RC_ERROR_BAD_DATA;

	pModule = &QSPI_mod[ModuleID];

//	//Check if module is already initialized
//	if (QSPI_module_IsNotInitialized != pModule->mState)
//	{
//		if(apModule!=NULL)
//			apModule->mpPriv 			= (void *)pModule;
//		return RC_SUCCESS;
//	}

	// Clearing the  module contents
	memset(pModule, 0, sizeof(QSPI_module_priv_t));

	//Initializing module registers
	sQSPI_module_init(ModuleID);

	pModule->mpConfig 			= &QSPI_cfg[ModuleID];
	pModule->mnBAUD2Freq		= Clock_get_Frequency(fBAUD2);
	pModule->mnChannelMsk 		= 0;
	pModule->mnModuleID			= ModuleID;

	//Initializing module data handling structures
	pModule->mnPktQRD 			= 0;
	pModule->mnPktQWR 			= 0;
	pModule->mnPktCnt 			= 0;

	pModule->mState 			= QSPI_module_IsIdle;

	if(apModule!=NULL)
		apModule->mpPriv 			= (void *)pModule;

	return RC_SUCCESS;
}


RC_t QSPI_channel_init(QSPI_Module_ID_t ModuleID, QSPI_Channel_ID_t ChannelID,  QSPI_channel_t *p_channel_hdl)
{
	QSPI_module_priv_t				*pModule = NULL;
	QSPI_channel_priv_t				*pChannel = NULL;
	const QSPI_module_cfg_t 		*pModuleCfg = NULL;


	if ((DERIVATE_QSPI_NUM_CHAN_PER_MOD <= ChannelID) ||(DERIVATE_QSPI_NUM_MOD <= ModuleID))
		return RC_ERROR_BAD_DATA;

	if (QSPI_module_IsNotInitialized == QSPI_mod[ModuleID].mState)
		return RC_ERROR_INVALID_STATE;

	pModule 	= &QSPI_mod[ModuleID];
	pModuleCfg 	= pModule->mpConfig;
	pChannel 	= &pModule->mpChannels[ChannelID];

	// Check for already initialized
	if (QSPI_channel_IsInitialized == pChannel->mState)
		return RC_ERROR_INVALID_STATE;

	// Clearing the channel contents
	memset(pChannel, 0, sizeof(QSPI_channel_priv_t));

	pChannel->mpConfig 		= &pModuleCfg->channelcfg[ChannelID % DERIVATE_QSPI_NUM_ECON_PER_MOD];
	pChannel->mpParent		= pModule;
	pChannel->mnChannelID 	= ChannelID;

	if (FALSE == pChannel->mpConfig->enable)
		return RC_ERROR_INVALID_STATE;

	sQSPI_channel_calculateBasicConfig(pChannel);

	pChannel->mState 		= QSPI_channel_IsInitialized;

	// adding the channel to the module link list
	pModule->mnChannelMsk	|= BIT_1 << pChannel->mnChannelID;

	if(p_channel_hdl!=NULL)
		p_channel_hdl->mpPriv = (void *)pChannel;

	return RC_SUCCESS;
}

/**
 * \brief RC_t QSPI_channel_exchange(QSPI_channel_t *apChannel,  const QSPI_exchange_t aType,
 *						  const QSPI_exchange_param_t *apParam)
 *
 * QSPI channel data exchange.
 *
 * \param apChannel [IN]	: channel handle.
 * \param aType  	[IN] 	: transaction type
 * \param apParam  	[IN] 	: transaction param
 * \return #RC_ERROR_BAD_PARAM 		if invalid(NULL) arguments are passed
 * \return #RC_ERROR_BAD_DATA		if invalid data is passed
 * \return #RC_ERROR_INVALID_STATE	if channel is already not initialized
 * \return #RC_ERROR_BUFFER_FULL	if internal buffer is full
 * \return #RC_SUCCESS 				otherwise
 *
 */
RC_t QSPI_channel_exchange(QSPI_channel_t *apChannel,  const QSPI_exchange_t aType,
						  const QSPI_exchange_param_t *apParam)
{
	QSPI_module_priv_t *pModule 	= NULL;
	QSPI_channel_priv_t *pChannel 	= NULL;
	QSPI_packet_t *pPkt = NULL;
	RC_t retCode = RC_ERROR;

	// Sanity check for parameters
	if ((NULL == apChannel) || (NULL == apParam))
		return RC_ERROR_BAD_PARAM;

	pChannel 	= (QSPI_channel_priv_t *)apChannel->mpPriv;

	// Check State
	if (QSPI_channel_IsNotInitialized == pChannel->mState)
		return RC_ERROR_INVALID_STATE;

	pModule 	= (QSPI_module_priv_t *)pChannel->mpParent;

	if (pModule->mnPktCnt < QSPI_MODULE_PACKET_QUEUE_DEPTH)
	{
		pPkt = &pModule->mpPktQ[pModule->mnPktQWR];					//@PF We write the data to QWR position

		sQSPI_packet_reset(pPkt);

		pPkt->mType			= aType;
		pPkt->mnChannelID 	= pChannel->mnChannelID;
		pPkt->mnDataWidth 	= apParam->mnDataWidth;

		if (QSPI_exchange_read == pPkt->mType)
		{
			if ((NULL == apParam->mpReadPtr) ||	(0 >= apParam->mnLength))
				return RC_ERROR_BAD_DATA;

			pPkt->mTransaction[QSPI_exchange_read].mpData		= apParam->mpReadPtr;
			pPkt->mTransaction[QSPI_exchange_read].mnLength		= apParam->mnLength;
			pPkt->mTransaction[QSPI_exchange_read].mState.mRD 	= QSPI_packet_TOBE_READ;

			// Setting Write Params for dummy write
			pPkt->mTransaction[QSPI_exchange_write].mpData		= NULL;
			pPkt->mTransaction[QSPI_exchange_write].mnLength	= apParam->mnLength;

			pPkt->mTransaction[QSPI_exchange_write].mState.mWR 	= QSPI_packet_TOBE_WRITTEN;
		}
		else if (QSPI_exchange_write == pPkt->mType)
		{
			if ((NULL == apParam->mpWritePtr) || (0 >= apParam->mnLength))
				return RC_ERROR_BAD_DATA;

			pPkt->mTransaction[QSPI_exchange_write].mpData		= apParam->mpWritePtr;
			pPkt->mTransaction[QSPI_exchange_write].mnLength	= apParam->mnLength;
			pPkt->mTransaction[QSPI_exchange_write].mState.mWR 	= QSPI_packet_TOBE_WRITTEN;

			// Setting Read Params for dummy read
			pPkt->mTransaction[QSPI_exchange_read].mpData		= NULL;
			pPkt->mTransaction[QSPI_exchange_read].mnLength		= apParam->mnLength;
			pPkt->mTransaction[QSPI_exchange_read].mState.mRD 	= QSPI_packet_TOBE_READ;
		}
		else
		{
			if ((NULL == apParam->mpWritePtr) || (NULL == apParam->mpReadPtr) || (0 >= apParam->mnLength))
				return RC_ERROR_BAD_DATA;

			pPkt->mTransaction[QSPI_exchange_write].mpData		= apParam->mpWritePtr;
			pPkt->mTransaction[QSPI_exchange_write].mnLength	= apParam->mnLength;
			pPkt->mTransaction[QSPI_exchange_write].mState.mWR 	= QSPI_packet_TOBE_WRITTEN;

			// Setting Read Params for dummy read
			pPkt->mTransaction[QSPI_exchange_read].mpData		= apParam->mpReadPtr;
			pPkt->mTransaction[QSPI_exchange_read].mnLength		= apParam->mnLength;
			pPkt->mTransaction[QSPI_exchange_read].mState.mRD 	= QSPI_packet_TOBE_READ;
		}


		pPkt->mfOnExchange	= apParam->mfOnExchange;
		pPkt->mpUserData	= apParam->mpUserData;

		pModule->mnPktQWR = (pModule->mnPktQWR + 1) % QSPI_MODULE_PACKET_QUEUE_DEPTH;
		pModule->mnPktCnt++;

		retCode = RC_SUCCESS;
	}
	else
	{
		retCode = RC_ERROR_BUFFER_FULL;
	}

	if (QSPI_module_IsIdle == pModule->mState)
	{
		// TX ISR is triggered manually to kick-off Packet transmission.
		// This is done by writing the STATUS.TXF flag
		// Not sure if this is a good practice.
		sQSPI_module_TxISR_trigger(pModule);
	}

	return retCode;
}

/**
 * \brief RC_t QSPI_channel_deinit(QSPI_channel_t *apChannel)
 *
 * De-initialize a QSPI channel.
 *
 * \param apChannel [IN]	: channel handle.
 * \return #RC_SUCCESS
 *
 */
RC_t QSPI_channel_deinit(QSPI_channel_t *apChannel)
{
	//TODO:
	return RC_SUCCESS;
}

/**
 * \brief RC_t QSPI_module_deinit(QSPI_module_t *apModule)
 *
 * De-initialize a QSPI module.
 *
 * \param apModule [IN]	: module handle.
 * \return #RC_SUCCESS
 *
 */
RC_t QSPI_module_deinit(QSPI_module_t *apModule)
{
	//TODO:
	return RC_SUCCESS;
}

/*
 * Each QSPI slave select channel has its own set of phase lengths, depending on
TBAUD2 = 1 / fBAUD2
• TSCLKz = TBAUD2 * GLOBALCON.TQ * ECONz.Q * ( A + B + C )
• TLEAD = TBAUD2 * BACON.LPRE * BACON.LEAD
• TTRAIL = TBAUD2 * BACON.TPRE * BACON.TRAIL
• TIDLEA,B = TBAUD2 * BACON.IPRE * BACON.IDLE
• TSTROBE = TBAUD2 * GLOBALCON.TQ * ECONz.Q * GLOBALCON.STROBE

In Short Data Mode, the QSPI module transmits single data with a length of 2 to 32 bits
in one frame. This mode is defined by BACON.LAST = 1 and BACON.BYTE = 0.

In Long Data Mode, the QSPI module transmits bursts of up to 32 bytes (256 bits) in one
frame. This mode is defined by programming BACON.LAST = 1 and BACON.BYTE = 1.

 */

/**
 * \brief STATIC void sQSPI_module_init(uint8_t module_num)
 *
 * initialize module registers
 *
 * \param module_num : module ID.
 * \return void
 */
STATIC void sQSPI_module_init(uint8_t module_num){

	//channel iterator
	uint8_t __channel_i;
	uint8_t __channel_clkparam_abc = 0;
	sint16_t __module_tq = 0;

	//check if module number is in range
	if(module_num>=DERIVATE_QSPI_NUM_MOD)
		return;

	//check if module has an enabled configuration
	if(QSPI_cfg[module_num].enable==FALSE)
		return;

	//Enable request
	if(SYSTEM_Module_CLC_Start((CLC_Bits*)&QSPI_cfg[module_num].p_module->CLC,0)!=RC_SUCCESS)
		return;

	//perform Kernel reset
	if(SYSTEM_Module_KRESET((KRST0_Bits*)&QSPI_cfg[module_num].p_module->KRST0, (KRST1_Bits*)&QSPI_cfg[module_num].p_module->KRST1, (KRSTCLR_Bits*)&QSPI_cfg[module_num].p_module->KRSTCLR,0)!=RC_SUCCESS)
		return;


	//Pin select (only for Master Mode)
	QSPI_cfg[module_num].p_module->PISEL.B.MRIS=QSPI_cfg[module_num].PISEL_MRIS;

//Global Configuration 0
	Ifx_QSPI_GLOBALCON __globalcon = {0};

	//Global Time Quantum Length. Derived from fBAUD2 and divided by GLOBALCON.TQ
	__module_tq = Clock_get_Frequency(fBAUD2)/(QSPI_cfg[module_num].MasterBaud) - 1;
	__globalcon.B.TQ = (__module_tq < 0) ? (uint8_t)0 : (uint8_t)__module_tq;

	//No status injection for the RxFIFO
	__globalcon.B.SI=0;

	//Time-Out Value for Expect Phase expressed in TQSPI units
	__globalcon.B.EXPECT = 15;  // 2^(value+6) : timeout for expect phase in Tqspi

	//deactivate LoopBack
	__globalcon.B.LB=0;

	//no external mux. Delayed Mode off
	__globalcon.B.DEL0=0;

	//Strobe Delay for SLSO0 in Delayed Mode
	__globalcon.B.STROBE=0;

	//Stop on RxFIFO Full enabled
	__globalcon.B.SRF= 1;

	//Slave Transmit Idle State Polarity skipped as this driver is only providing master mode operation

	//Set PAUSE Mode during configuration
	__globalcon.B.EN=0;

	//Set Master Mode
	__globalcon.B.MS=0b00;

	//Automatic Reset Enable skipped as this driver is only providing master mode operation

	//write configuration
	QSPI_cfg[module_num].p_module->GLOBALCON.U =__globalcon.U;

//Global Configuration 1
	Ifx_QSPI_GLOBALCON1 __globalcon1 = {0};

	//Error enable bits: all errors disables
	__globalcon1.B.ERRORENS=QSPI_error_ALL;

	//TX Interrupt enabled//disabled
	__globalcon1.B.TXEN=1;

	//RX Interrupt enabled//disabled
	__globalcon1.B.RXEN=0;

	//PT1 Interrupt
	if (TRUE == QSPI_cfg[module_num].pt1Int_enable)
	{
		__globalcon1.B.PT1EN = 1;
		__globalcon1.B.PT1 = QSPI_cfg[module_num].pt1Int_event;
	}

	if (TRUE == QSPI_cfg[module_num].pt2Int_enable)
	{
		__globalcon1.B.PT2EN = 1;
		__globalcon1.B.PT2 = QSPI_cfg[module_num].pt2Int_event;
	}

	__globalcon1.B.RXFIFOINT = QSPI_cfg[module_num].rxFIFOint_thres;
	__globalcon1.B.TXFIFOINT = QSPI_cfg[module_num].txFIFOint_thres;

	//other interrupts are skipped and set to 0
	QSPI_cfg[module_num].p_module->GLOBALCON1.U =__globalcon1.U;

	//TBAUD2 clock period -> one module time quantum -> one channel time quantum

	//Initialize channels
	Ifx_QSPI_ECON __econ;
	for(__channel_i=0;__channel_i< DERIVATE_QSPI_NUM_ECON_PER_MOD;__channel_i++){

		//check if channel has an enabled configuration
		if(QSPI_cfg[module_num].channelcfg[__channel_i].enable==TRUE){

			//load current settings
			__econ.U = QSPI_cfg[module_num].p_module->ECON[__channel_i].U;

			//A -> Length of Bit Segment 1, skipped -> default value
			//B -> Length of Bit Segment 2, skipped -> default value
			//C -> Length of Bit Segment 3, skipped -> default value

			/***
			 * BRz = (fBAUD2) / ((TQ  + 1) * (Qz + 1) * (A + 1 + B + C))
			 * Qz = ((fBAUD2) / (BRz * (TQ  + 1) * (A + 1 + B + C))) - 1; //provides max(BR) for a module
			 */

			//time quantum fbaud2/globalTQ divided by econTQ
			__channel_clkparam_abc = __econ.B.A + __econ.B.B + __econ.B.C + 1;
			__module_tq = QSPI_cfg[module_num].p_module->GLOBALCON.B.TQ;
			__econ.B.Q=(Clock_get_Frequency(fBAUD2)) / (QSPI_cfg[module_num].channelcfg[__channel_i].baudrate * (__module_tq + 1) * (__channel_clkparam_abc)) - 1;

			//clock phase
			__econ.B.CPH=QSPI_cfg[module_num].channelcfg[__channel_i].CPHA;

			//clock polarity
			__econ.B.CPOL=QSPI_cfg[module_num].channelcfg[__channel_i].CPOL;

			//Parity, skipped -> default value
			//Permutation, skipped -> default value

			//write settings
			QSPI_cfg[module_num].p_module->ECON[__channel_i].U =__econ.U;

			//enable CS
			QSPI_cfg[module_num].p_module->SSOC.B.OEN |= BIT_1 << __channel_i;
			QSPI_cfg[module_num].p_module->SSOC.B.OEN |= BIT_1 << (__channel_i + DERIVATE_QSPI_NUM_ECON_PER_MOD);

			//invert CS
			if(QSPI_cfg[module_num].channelcfg[__channel_i].invertCS==TRUE)
			{
				QSPI_cfg[module_num].p_module->SSOC.B.AOL |= BIT_1 << __channel_i;
				QSPI_cfg[module_num].p_module->SSOC.B.AOL |= BIT_1 << (__channel_i + DERIVATE_QSPI_NUM_ECON_PER_MOD);
			}
		}
	}

	// configure interrupts
	//TODO: this will not work if we have different Interrupt tables
	//InterruptRouter_CfgSRC(&QSPI_cfg[module_num].p_src_module->TX);
	//InterruptRouter_CfgSRC(&QSPI_cfg[module_num].p_src_module->ERR);
	//if ((TRUE == QSPI_cfg[module_num].pt1Int_enable) || (TRUE == QSPI_cfg[module_num].pt2Int_enable))
		//InterruptRouter_CfgSRC(&QSPI_cfg[module_num].p_src_module->PT);

	//run
	QSPI_cfg[module_num].p_module->GLOBALCON.B.EN = 1;
}

/**
 * \brief STATIC RC_t sQSPI_module_transact(QSPI_module_priv_t *apModule, QSPI_exchange_t type)
 *
 * Module transaction
 *
 * \param apModule 	: QSPI module private handle.
 * \param type		: transaction type
 * \return void
 */
STATIC RC_t sQSPI_module_transact(QSPI_module_priv_t *apModule, QSPI_exchange_t type)
{
	RC_t retCode = RC_ERROR_BUFFER_EMTPY;

	// This function takes care of state management
	// if packets are available for transfer
	// could be packets in progress or new packets.
	if (apModule->mnPktCnt > QSPI_MODULE_PACKET_QUEUE_DEPTH)
	{
		retCode = RC_ERROR_BUFFER_FULL;
	}
	else if (0 < apModule->mnPktCnt)
	{
		QSPI_packet_t *pPkt = NULL;
		// pPkt is a critical section
		pPkt = &apModule->mpPktQ[apModule->mnPktQRD];

		if (QSPI_exchange_write == type)
		{
			apModule->mState = QSPI_module_IsExchanging;

			//TX-ISR triggered this routine!
			retCode = sQSPI_channel_writePacket(&apModule->mpChannels[pPkt->mnChannelID], pPkt);
		}
		else if (QSPI_exchange_read == type)
		{
			//PT-ISR triggered this routine!
			apModule->mState = QSPI_module_IsExchanging;

			// Basic Configuration has to be written before Read!
			// This is taken care by using the Phase-Transition ISR!
			// Also read-write possible race checked by (GLOBALCON.SRF == 1)
			retCode = sQSPI_channel_readPacket(&apModule->mpChannels[pPkt->mnChannelID], pPkt);
		}

		if ((QSPI_packet_READ == pPkt->mTransaction[QSPI_exchange_read].mState.mRD) &&
			(QSPI_packet_WRITTEN == pPkt->mTransaction[QSPI_exchange_write].mState.mWR))
		{
			// current packet transfer is complete
			sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_SUCCESS);
			apModule->mState = QSPI_module_IsIdle;
		}
	}

	return retCode;
}

/**
 * \brief STATIC RC_t sQSPI_module_errorHandler(QSPI_module_priv_t *apModule)
 *
 * Transact packet
 *
 * \param apModule 	: QSPI module private handle.
 * \return void
 */
STATIC RC_t sQSPI_module_errorHandler(QSPI_module_priv_t *apModule)
{
	uint16_t nError = sQSPI_module_getError(apModule);

	if (QSPI_error_NONE == nError)
		return RC_SUCCESS;

	sQSPI_module_clearError(apModule);

	if (0 != (nError | QSPI_error_PAREEN))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_pairity);

	if (0 != (nError | QSPI_error_UCON))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_unexpectedConfig);

	if (0 != (nError | QSPI_error_BR))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_baudRate);

	if (0 != (nError | QSPI_error_TXOF))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_txOverflow);

	if (0 != (nError | QSPI_error_TXUF))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_txUnderflow);

	if (0 != (nError | QSPI_error_RXOF))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_rxOverflow);

	if (0 != (nError | QSPI_error_RXUF))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_rxUnderflow);

	if (0 != (nError | QSPI_error_ETO))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_expectTimeout);

	if (0 != (nError | QSPI_error_SLSIEN))
		sQSPI_module_onPacketCompletion(apModule, QSPI_exchangeResult_ERROR_slaveSelect);

	return RC_SUCCESS;
}

/**
 * \brief STATIC void sQSPI_module_onPacketCompletion(QSPI_module_priv_t *apModule, QSPI_exchangeResult_t aResult)
 *
 * On Packet transaction Completion
 *
 * \param apModule 		: QSPI module private handle.
 * \param aResult		: transaction result
 * \return void
 */
STATIC void sQSPI_module_onPacketCompletion(QSPI_module_priv_t *apModule, QSPI_exchangeResult_t aResult)
{
	QSPI_packet_t *pPkt = &apModule->mpPktQ[apModule->mnPktQRD];

	apModule->mnPktQRD = (apModule->mnPktQRD + 1) % QSPI_MODULE_PACKET_QUEUE_DEPTH;
	apModule->mnPktCnt--;

	if (NULL != pPkt->mfOnExchange)
	{
		pPkt->mfOnExchange(aResult, pPkt->mpUserData);
	}
	else
	{
		//@PF this should not happen....
		//asm("debug");
	}
}

/**
 * \brief STATIC uint8_t sQSPI_channel_calculatePrescaler(QSPI_channel_priv_t *apChannel)
 *
 * Calculate pre-scaler value to achieve the channel baud-rate
 *
 * \param apChannel : QSPI channel private handle.
 * \return pre-scaler value
 */
STATIC uint8_t sQSPI_channel_calculatePrescaler(QSPI_channel_priv_t *apChannel)
{
	/**
	 * This logic is copied from Infineon reference code!
	 */

	float32_t  	error = 0.0,
				bestError = 10e6,
				halfBaud = apChannel->mpConfig->baudrate / 2, //QSPI channel half baud
				fQspiIn  = (float32_t)apChannel->mpParent->mnBAUD2Freq; // QSPI module frequency

	uint32_t 	i = 0;
	uint8_t 	bestPre = 0;

	/**
	 * Possible pre-scaler values are 4^0, 4^1,...,4^7.
	 * Iterating through possible pre-scaler ranges to
	 * find which pre-scaler best fits the expected channel baud-rate.
	 *
	 */

	for (i = 0; i < 8; i++)
	{
		float tempHalfBaud = fQspiIn / (1U << (4 * i));
		error = _absf(tempHalfBaud - halfBaud);

		if (_leqf(error, bestError))
		{
			bestError = error;
			bestPre   = i;
		}
	}

	return bestPre;
}

/**
 * \brief STATIC RC_t sQSPI_channel_calculateBasicConfig(QSPI_channel_priv_t *apChannel)
 *
 * Calculate basic configuration for a channel
 *
 * \param apChannel : QSPI channel private handle.
 * \return #RC_SUCCESS
 */
STATIC RC_t sQSPI_channel_calculateBasicConfig(QSPI_channel_priv_t *apChannel)
{
    Ifx_QSPI_BACON *pBasicConReg = &apChannel->mBaconReg;
    uint8_t nPreScaler = 0;

    pBasicConReg->U = 0;

    nPreScaler = sQSPI_channel_calculatePrescaler(apChannel);

    pBasicConReg->B.LAST   = 0;                       	/* 1-bits Last Word in a Frame, will be set via recalcBasicConfiguration before transfer */
    pBasicConReg->B.IPRE   = nPreScaler;                /* 3-bits Prescaler for the Idle Delay */
    pBasicConReg->B.IDLE   = SpiIf_SlsoTiming_2; 		/* 3-bits Idle Delay Length */
    pBasicConReg->B.LPRE   = nPreScaler;                /* 3-bits Prescaler for the Leading Delay */
    pBasicConReg->B.LEAD   = SpiIf_SlsoTiming_4;     	/* 3-bits Leading Delay Length */
    pBasicConReg->B.TPRE   = nPreScaler;                /* 3-bits Prescaler for the Trailing Delay */
    pBasicConReg->B.TRAIL  = SpiIf_SlsoTiming_4;    	/* 2-bits Trailing Delay Length */
    pBasicConReg->B.PARTYP = QSPI_PAIRITYMODE_EVEN;
    pBasicConReg->B.UINT   = 0;                       	/* 1-bits User Interrupt at the PT1 Event in the Subsequent Frames */
    pBasicConReg->B.MSB    = apChannel->mpConfig->LSB_first;
    pBasicConReg->B.BYTE   = 0;                       	/* only support bitwise selection in B.DL */
    pBasicConReg->B.DL     = 0;						  	/* To be set on individual writes **/
    pBasicConReg->B.CS     = apChannel->mnChannelID;

    return RC_SUCCESS;
}

/**
 * \brief STATIC void sQSPI_channel_writeBOS(QSPI_channel_priv_t *apChannel)
 *
 * Write Begin of Stream - Continuous mode
 *
 * \param apChannel : QSPI channel private handle.
 * \return void
 */
STATIC void sQSPI_channel_writeBOS(QSPI_channel_priv_t *apChannel)
{
    apChannel->mBaconReg.B.LAST = 0;
    apChannel->mpParent->mpConfig->p_module->BACONENTRY.U = apChannel->mBaconReg.U;
}

/**
 * \brief STATIC void sQSPI_channel_writeEOS(QSPI_channel_priv_t *apChannel)
 *
 * Write End of Stream - Continuous mode
 *
 * \param apChannel : QSPI channel private handle.
 * \return void
 */
STATIC void sQSPI_channel_writeEOS(QSPI_channel_priv_t *apChannel)
{
    apChannel->mBaconReg.B.LAST = 1;
    apChannel->mpParent->mpConfig->p_module->BACONENTRY.U = apChannel->mBaconReg.U;
}

/**
 * \brief STATIC RC_t sQSPI_channel_writePacket(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
 *
 * Write Packet to Channel
 *
 * \param apChannel : QSPI channel private handle.
 * \param apPkt		: Packet
 * \return #RC_ERROR_RANGE 	if invalid QSPI TX FIFO level
 * \return #RC_SUCCESS		otherwise
 */
STATIC RC_t sQSPI_channel_writePacket(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
{
	sint32_t nFreeSlots = 0;

	// THIS FUNCTION SHOULD BE A CRITICAL SECTION!!!!

	// Counter + exit condition
	// Write Implemented is Continuous Mode!
	// Modify here for Other Modes.

	nFreeSlots = DERIVATE_QSPI_HW_FIFO_DEPTH - sQSPI_module_getTxFIFOLevel(apChannel->mpParent);

	if (DERIVATE_QSPI_HW_FIFO_DEPTH < nFreeSlots)
		return RC_ERROR_RANGE;

	while (0 < nFreeSlots)
	{
		// This packet is to be written!
		// Packet Write State Machine
		// QSPI_packet_TOBE_WRITTEN (init) -> QSPI_packet_BOS_WRITTEN (write data) ->
		// QSPI_packet_EOS_WRITTEN (before last frame) -> QSPI_packet_WRITTEN (done)

		switch (apPkt->mTransaction[QSPI_exchange_write].mState.mWR)
		{
		case QSPI_packet_TOBE_WRITTEN:
		{
			// Starting the write in Continuous Mode
			// Writing the Basic Configuration
			apChannel->mBaconReg.B.DL = apPkt->mnDataWidth - 1;
			sQSPI_channel_writeBOS(apChannel);
			nFreeSlots--;

			if (8 >= apPkt->mnDataWidth)
				apPkt->mTransaction[QSPI_exchange_write].mnPending = apPkt->mTransaction[QSPI_exchange_write].mnLength;
			else if (16 >= apPkt->mnDataWidth)
				apPkt->mTransaction[QSPI_exchange_write].mnPending = apPkt->mTransaction[QSPI_exchange_write].mnLength >> 1; //div by 2
			else
				apPkt->mTransaction[QSPI_exchange_write].mnPending = apPkt->mTransaction[QSPI_exchange_write].mnLength >> 2; //div by 4

			apPkt->mTransaction[QSPI_exchange_write].mnConsumed = 0;

			apPkt->mTransaction[QSPI_exchange_write].mState.mWR = QSPI_packet_BOS_WRITTEN;
		}
		break;

		case QSPI_packet_BOS_WRITTEN:
		{
			// EOS has to be written before the last data frame
			if (1 < apPkt->mTransaction[QSPI_exchange_write].mnPending)
			{
				sQSPI_packet_writeData(apChannel, apPkt);
				nFreeSlots--;
			}
			else
			{
				// Writing EOS
				sQSPI_channel_writeEOS(apChannel);
				nFreeSlots--;

				apPkt->mTransaction[QSPI_exchange_write].mState.mWR = QSPI_packet_EOS_WRITTEN;
			}

		}
		break;

		case QSPI_packet_EOS_WRITTEN:
		{
			// Writing the last data frame
			sQSPI_packet_writeData(apChannel, apPkt);
			nFreeSlots--;

			apPkt->mTransaction[QSPI_exchange_write].mState.mWR = QSPI_packet_WRITTEN;//packet completed

			return RC_SUCCESS;
		}
		break;

		case QSPI_packet_WRITTEN:
		{
			return RC_SUCCESS; //packet completed
		}
		break;
		}
	}

	return RC_SUCCESS;
}

/**
 * \brief STATIC RC_t sQSPI_channel_readPacket(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
 *
 * Read Packet from Channel
 *
 * \param apChannel : QSPI channel private handle.
 * \param apPkt		: Packet
 * \return #RC_ERROR_RANGE 	if invalid QSPI TX FIFO level
 * \return #RC_SUCCESS		otherwise
 */
static RC_t sQSPI_channel_readPacket(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
{
	sint32_t nFullSlots = 0;

	// THIS FUNCTION SHOULD BE A CRITICAL SECTION!!!!
	// Counter + exit condition

	nFullSlots = sQSPI_module_getRxFIFOLevel(apChannel->mpParent);

	if (DERIVATE_QSPI_HW_FIFO_DEPTH < nFullSlots)
		return RC_ERROR_RANGE;

	while (0 < nFullSlots)
	{
		// This packet is to be read!
		// Packet Read State Machine
		// QSPI_packet_TOBE_READ (init) -> QSPI_packet_READING (read) -> QSPI_packet_READ (done)

		switch (apPkt->mTransaction[QSPI_exchange_read].mState.mRD)
		{
		case QSPI_packet_TOBE_READ:
		{
			if (8 >= apPkt->mnDataWidth)
				apPkt->mTransaction[QSPI_exchange_read].mnPending = apPkt->mTransaction[QSPI_exchange_read].mnLength;
			else if (16 >= apPkt->mnDataWidth)
				apPkt->mTransaction[QSPI_exchange_read].mnPending = apPkt->mTransaction[QSPI_exchange_read].mnLength >> 1; //div by 2
			else
				apPkt->mTransaction[QSPI_exchange_read].mnPending = apPkt->mTransaction[QSPI_exchange_read].mnLength >> 2; //div by 4

			apPkt->mTransaction[QSPI_exchange_read].mnConsumed = 0;


			apPkt->mTransaction[QSPI_exchange_read].mState.mRD = QSPI_packet_READING;
		}
		// Fall Through

		case QSPI_packet_READING:
		{
			// EOS has to be written before the last data frame
			if (0 < apPkt->mTransaction[QSPI_exchange_read].mnPending)
			{
				sQSPI_packet_readData(apChannel, apPkt);
				nFullSlots--;
			}

			if (0 == apPkt->mTransaction[QSPI_exchange_read].mnPending)
			{
				apPkt->mTransaction[QSPI_exchange_read].mState.mRD = QSPI_packet_READ;

				return RC_SUCCESS; //packet completed
			}
		}
		break;

		case QSPI_packet_READ:
		{
			return RC_SUCCESS; //packet completed
		}
		break;
		}
	}

	return RC_SUCCESS;
}

/**
 * \brief STATIC INLINE void sQSPI_packet_reset(QSPI_packet_t *apPkt)
 *
 * reset Packet
 *
 * \param apPkt : pointer to packet.
 * \return void
 */
STATIC void sQSPI_packet_reset(QSPI_packet_t *apPkt)
{
	// Reset all packet params to default values.

	apPkt->mnChannelID 			= 0;
	apPkt->mType 				= QSPI_exchange_read;
	apPkt->mfOnExchange 		= NULL;
	apPkt->mpUserData			= NULL;
	apPkt->mnDataWidth 			= 0;

	apPkt->mTransaction[QSPI_exchange_read].mpData 		= NULL;
	apPkt->mTransaction[QSPI_exchange_read].mState.mRD 	= QSPI_packet_TOBE_READ;
	apPkt->mTransaction[QSPI_exchange_read].mnLength 	= 0;
	apPkt->mTransaction[QSPI_exchange_read].mnPending 	= 0;
	apPkt->mTransaction[QSPI_exchange_read].mnConsumed 	= 0;

	apPkt->mTransaction[QSPI_exchange_write].mpData 	= NULL;
	apPkt->mTransaction[QSPI_exchange_write].mState.mWR = QSPI_packet_TOBE_WRITTEN;
	apPkt->mTransaction[QSPI_exchange_write].mnLength 	= 0;
	apPkt->mTransaction[QSPI_exchange_write].mnPending 	= 0;
	apPkt->mTransaction[QSPI_exchange_write].mnConsumed = 0;
}

/**
 * \brief STATIC void sQSPI_packet_writeData(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
 *
 * Write Data from Packet
 *
 * \param apChannel : QSPI channel private handle.
 * \param apPkt		: Packet
 * \return void
 */
STATIC void sQSPI_packet_writeData(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
{
	if (NULL == apPkt->mTransaction[QSPI_exchange_write].mpData)
	{
		//Dummy data - for Read operation
		apChannel->mpParent->mpConfig->p_module->DATAENTRY[apChannel->mnChannelID % 8].U = 0xFF;
		apPkt->mTransaction[QSPI_exchange_write].mnPending--;
		apPkt->mTransaction[QSPI_exchange_write].mnConsumed++;
		return;
	}

	if (8 >= apPkt->mnDataWidth)
	{
		uint8_t *pData = (uint8_t *)apPkt->mTransaction[QSPI_exchange_write].mpData;

		apChannel->mpParent->mpConfig->p_module->DATAENTRY[apChannel->mnChannelID % 8].U = pData[apPkt->mTransaction[QSPI_exchange_write].mnConsumed];

		apPkt->mTransaction[QSPI_exchange_write].mnPending--;
		apPkt->mTransaction[QSPI_exchange_write].mnConsumed++;
	}
	else if (16 >= apPkt->mnDataWidth)
	{
		uint16_t *pData = (uint16_t *)apPkt->mTransaction[QSPI_exchange_write].mpData;

		apChannel->mpParent->mpConfig->p_module->DATAENTRY[apChannel->mnChannelID % 8].U = pData[apPkt->mTransaction[QSPI_exchange_write].mnConsumed];

		apPkt->mTransaction[QSPI_exchange_write].mnPending--;
		apPkt->mTransaction[QSPI_exchange_write].mnConsumed++;
	}
	else
	{
		uint32_t *pData = (uint32_t *)apPkt->mTransaction[QSPI_exchange_write].mpData;

		apChannel->mpParent->mpConfig->p_module->DATAENTRY[apChannel->mnChannelID % 8].U = pData[apPkt->mTransaction[QSPI_exchange_write].mnConsumed];

		apPkt->mTransaction[QSPI_exchange_write].mnPending--;
		apPkt->mTransaction[QSPI_exchange_write].mnConsumed++;
	}
}

/**
 * \brief STATIC void sQSPI_packet_readData(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
 *
 * Read Data to Packet
 *
 * \param apChannel : QSPI channel private handle.
 * \param apPkt		: Packet
 * \return void
 */
STATIC void sQSPI_packet_readData(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
{
	if (NULL == apPkt->mTransaction[QSPI_exchange_read].mpData)
	{
		//Dummy data - for Read operation
		uint32_t nScratch = apChannel->mpParent->mpConfig->p_module->RXEXIT.U;
		apPkt->mTransaction[QSPI_exchange_read].mnPending--;
		apPkt->mTransaction[QSPI_exchange_read].mnConsumed++;
		return;
	}

	if (8 >= apPkt->mnDataWidth)
	{
		uint8_t *pData = (uint8_t *)apPkt->mTransaction[QSPI_exchange_read].mpData;

		pData[apPkt->mTransaction[QSPI_exchange_read].mnConsumed] = (uint8_t)apChannel->mpParent->mpConfig->p_module->RXEXIT.U;

		apPkt->mTransaction[QSPI_exchange_read].mnPending--;
		apPkt->mTransaction[QSPI_exchange_read].mnConsumed++;
	}
	else if (16 >= apPkt->mnDataWidth)
	{
		uint16_t *pData = (uint16_t *)apPkt->mTransaction[QSPI_exchange_read].mpData;

		pData[apPkt->mTransaction[QSPI_exchange_read].mnConsumed] = (uint16_t)apChannel->mpParent->mpConfig->p_module->RXEXIT.U;

		apPkt->mTransaction[QSPI_exchange_read].mnPending--;
		apPkt->mTransaction[QSPI_exchange_read].mnConsumed++;
	}
	else
	{
		uint32_t *pData = (uint32_t *)apPkt->mTransaction[QSPI_exchange_read].mpData;

		pData[apPkt->mTransaction[QSPI_exchange_read].mnConsumed] = (uint32_t)apChannel->mpParent->mpConfig->p_module->RXEXIT.U;

		apPkt->mTransaction[QSPI_exchange_read].mnPending--;
		apPkt->mTransaction[QSPI_exchange_read].mnConsumed++;
	}
}
