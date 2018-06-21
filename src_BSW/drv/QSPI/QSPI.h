/**
 * \file	QSPI.c
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	07.05.2017
 * \version	0.1
 *
 * \brief QSPI driver interface
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

#ifndef QSPI_H_
#define QSPI_H_

#include "global.h"
#include "register.h"

/** \brief Start-Address of QSPI-0 Module registers */
#define	PORT_SPI_0_BEGIN	((unsigned int)&MODULE_QSPI0)
/** \brief End-Address of QSPI-0 Module registers */
#define	PORT_SPI_0_END		((unsigned int)&MODULE_QSPI0 + sizeof(Ifx_QSPI))

/** \brief QSPI data transaction modes */
typedef enum
{
	QSPI_exchange_read = 0,		/**< Read only */
	QSPI_exchange_write,		/**< Write only */
	QSPI_exchange_readwrite		/**< Read and Write */
}QSPI_exchange_t;

/** \brief QSPI data transaction results */
typedef enum
{
	QSPI_exchangeResult_SUCCESS = 0,
	QSPI_exchangeResult_ERROR_pairity,				/**< Pairity error */
	QSPI_exchangeResult_ERROR_unexpectedConfig,		/**< Unexpected Configuration error*/
	QSPI_exchangeResult_ERROR_baudRate,				/**< BaudRate error */
	QSPI_exchangeResult_ERROR_txOverflow,			/**< TX overflow error */
	QSPI_exchangeResult_ERROR_txUnderflow,			/**< TX underflow error */
	QSPI_exchangeResult_ERROR_rxOverflow,			/**< RX overflow error */
	QSPI_exchangeResult_ERROR_rxUnderflow,			/**< RX underflow error */
	QSPI_exchangeResult_ERROR_expectTimeout,		/**< EXPECT phase time-out error */
	QSPI_exchangeResult_ERROR_slaveSelect			/**< SLSI misplaced inactivation enable */
}QSPI_exchangeResult_t;

/** \brief QSPI module identfier */
typedef enum{
	QSPI0=0,
	QSPI1,
	QSPI2,
	QSPI3,
	QSPI4,
	QSPI5,
	QSPI6
}QSPI_Module_ID_t;

/** \brief QSPI chanel identfier */
typedef enum{
	QSPI_CH0=0,
	QSPI_CH1,
	QSPI_CH2,
	QSPI_CH3,
	QSPI_CH4,
	QSPI_CH5,
	QSPI_CH6,
	QSPI_CH7,
	QSPI_CH8,
	QSPI_CH9,
	QSPI_CH10,
	QSPI_CH11,
	QSPI_CH12,
	QSPI_CH13,
	QSPI_CH14,
	QSPI_CH15,
}QSPI_Channel_ID_t;

/** \brief QSPI channel init param */
typedef struct _QSPI_channel_init_param_
{
	uint8_t 		mnModuleID;		/**< Module ID (total #DERIVATE_QSPI_NUM_MOD starting from 0) */
	uint8_t 		mnChannelID;	/**< Channel ID (total #DERIVATE_QSPI_NUM_CHAN_PER_MOD starting from 0) */
}QSPI_channel_param_t;

/** \brief QSPI module handle */
typedef struct _QSPI_module_
{
	void 		*mpPriv;		/**< pointer to private data structure */
}QSPI_module_t;

/** \brief QSPI channel handle */
typedef struct _QSPI_channel_
{
	void 		*mpPriv;		/**< pointer to private data structure */
}QSPI_channel_t;

/** \brief on Packet Exchange Callback Function Pointer */
typedef void (*fPkt_OnExchange)(QSPI_exchangeResult_t aResult, void *apUserData);

/** \brief QSPI transaction param */
typedef struct _QSPI_exchange_param_
{
	uint16_t				mnDataWidth;		/**< datawidth in bits */
	sint32_t				mnLength;			/**< Length of data exchange */
	void 					*mpWritePtr;		/**< data write pointer */
	void					*mpReadPtr;			/**< data read pointer */
	fPkt_OnExchange			mfOnExchange;		/**< on data exchange callback function pointer */
	void 					*mpUserData;		/**< callback user data */
}QSPI_exchange_param_t;

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * QSPI RX ISR.
 * @param module_num : module ID
 * @return void
 */
void _QSPI_RX_ISR(int module_num);

/*
 * QSPI TX ISR.
 * @param module_num : module ID
 * @return void
 */
void _QSPI_TX_ISR(int module_num);

/*
 * QSPI ERROR ISR.
 * @param module_num : module ID
 * @return void
 */
void _QSPI_ERROR_ISR(int module_num);

/*
 * QSPI PHASE TRANSITION ISR.
 * @param module_num : module ID
 * @return void
 */
void _QSPI_PHASE_TRANSITION_ISR(int module_num);

/**** QSPI Module APIs *************/

/**
 * \brief Initialize a QSPI module.
 *
 * \param 	ModuleID 	: module to be initialized
 * \param 	p_module_hdl: module handle
 * \return 	#RC_ERROR_BAD_DATA	if invalid data is passed
 * \return 	#RC_SUCCESS 			otherwise
 */
RC_t QSPI_module_init(QSPI_Module_ID_t ModuleID, QSPI_module_t *p_module_hdl);

/**
 * \brief Initialize a QSPI channel.
 *
 * \param 	ModuleID		Module of the channel
 * \param 	ChannelID 		Channel to be initialized
 * \param 	p_channel_hdl 	Channel handle
 * \return #RC_ERROR_BAD_PARAM 		if invalid(NULL) arguments are passed
 * \return #RC_ERROR_BAD_DATA		if invalid data is passed
 * \return #RC_ERROR_INVALID_STATE	if channel is already initialized or disabled
 * \return #RC_SUCCESS 				otherwise
 *
 */
RC_t QSPI_channel_init(QSPI_Module_ID_t ModuleID, QSPI_Channel_ID_t ChannelID,  QSPI_channel_t *p_channel_hdl);

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
RC_t QSPI_channel_exchange(QSPI_channel_t *apChannel,  const QSPI_exchange_t aType, const QSPI_exchange_param_t *apParam);

/**
 * \brief RC_t QSPI_channel_deinit(QSPI_channel_t *apChannel)
 *
 * De-initialize a QSPI channel.
 *
 * \param apChannel [IN]	: channel handle.
 * \return #RC_SUCCESS
 *
 */
RC_t QSPI_channel_deinit(QSPI_channel_t *apChannel);

/**
 * \brief RC_t QSPI_module_deinit(QSPI_module_t *apModule)
 *
 * De-initialize a QSPI module.
 *
 * \param apModule [IN]	: module handle.
 * \return #RC_SUCCESS
 *
 */
RC_t QSPI_module_deinit(QSPI_module_t *apModule);

#ifdef __cplusplus
}
#endif

#endif /* QSPI_H_ */
