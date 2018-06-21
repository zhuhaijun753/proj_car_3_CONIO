/**
 * \file	QSPI_priv.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	07.05.2017
 * \version	0.1
 *
 * \brief QSPI driver private defines
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

#ifndef SRC_BSW_DRV_QSPI_PRIV_QSPI_PRIV_H_
#define SRC_BSW_DRV_QSPI_PRIV_QSPI_PRIV_H_

#include "QSPI.h"

#define	QSPI_MODULE_PACKET_QUEUE_DEPTH (16)

/** \brief QSPI Module State */
typedef enum
{
	QSPI_module_IsNotInitialized = 0,		/**< Not Initialized */
	QSPI_module_IsIdle,						/**< Idle */
	QSPI_module_IsExchanging				/**< Exchanging */
}QSPI_module_state_t;

/** \brief QSPI Channel State */
typedef enum
{
	QSPI_channel_IsNotInitialized = 0,		/**< Not Initialized */
	QSPI_channel_IsInitialized				/**< Initialized */
}QSPI_channel_state_t;

/** \brief QSPI packet write State */
typedef enum _QSPI_packet_write_state_
{
	QSPI_packet_TOBE_WRITTEN = 0,			/**< To be written */
	QSPI_packet_BOS_WRITTEN,				/**< Configuration written */
	QSPI_packet_EOS_WRITTEN,				/**< End of stream written */
	QSPI_packet_WRITTEN						/**< Completed*/
}QSPI_packet_write_state_t;

/** \brief QSPI packet read State */
typedef enum _QSPI_packet_read_state_
{
	QSPI_packet_TOBE_READ = 0,				/**< To be read */
	QSPI_packet_READING,					/**< Reading*/
	QSPI_packet_READ						/**< Completed */
}QSPI_packet_read_state_t;

/** \brief QSPI packet State */
typedef union
{
	QSPI_packet_write_state_t 	mWR;		/**< write state */
	QSPI_packet_read_state_t    mRD;		/**< read state */
}QSPI_packet_state_u;

/** \brief QSPI error timing */
typedef enum
{
	QSPI_error_NONE		= 0x000,
	QSPI_error_PAREEN 	= 0x001,			/**< Pairity error */
	QSPI_error_UCON		= 0x002,			/**< Unexpected Configuration error*/
	QSPI_error_BR		= 0x004,			/**< BaudRate error */
	QSPI_error_TXOF		= 0x008,			/**< TX overflow error */
	QSPI_error_TXUF		= 0x010,			/**< TX underflow error */
	QSPI_error_RXOF 	= 0x020,			/**< RX overflow error */
	QSPI_error_RXUF 	= 0x040,			/**< RX underflow error */
	QSPI_error_ETO		= 0x080,			/**< EXPECT phase time-out error */
	QSPI_error_SLSIEN	= 0x100,			/**< SLSI misplaced inactivation enable */
	QSPI_error_ALL		= 0x1FF,
}QSPI_error_t;

/** \brief Slave select output timing */
typedef enum
{
	SpiIf_SlsoTiming_0 = 0,             /**< zero delay clock */
	SpiIf_SlsoTiming_1,                 /**< 1 half-clock delay */
	SpiIf_SlsoTiming_2,                 /**< 2 half-clock delay */
	SpiIf_SlsoTiming_3,                 /**< 3 half-clock delay */
	SpiIf_SlsoTiming_4,                 /**< 4 half-clock delay */
	SpiIf_SlsoTiming_5,                 /**< 5 half-clock delay */
	SpiIf_SlsoTiming_6,                 /**< 6 half-clock delay */
	SpiIf_SlsoTiming_7                  /**< 7 half-clock delay */
} QSPI_SlsoTiming;

/** \brief QSPI transaction private */
typedef struct _QSPI_transaction_priv_
{
	void				*mpData;	/**< Data buffer */
	sint32_t			mnLength;	/**< Data Length in byte units */
	sint32_t			mnPending; 	/**< Data Pending in datawidth (1, 2, 4 byte) units */
	sint32_t			mnConsumed; /**< Data Consumed in datawidth (1, 2, 4 byte) units */
	QSPI_packet_state_u mState;		/**< Packet transaction state */
}QSPI_transaction_priv_t;

/** \brief QSPI Packet */
typedef struct _QSPI_packet_
{
	uint16_t				mnChannelID;		/**< Channel ID */
	uint16_t				mnDataWidth;		/**< Data Width */
	QSPI_exchange_t			mType;				/**< transaction type */

	QSPI_transaction_priv_t	mTransaction[2];	/**< transactions - [0] - read; [1] - write */

	fPkt_OnExchange			mfOnExchange;		/**< Callback - on exchange */
	void					*mpUserData;		/**< Callback user data */
}QSPI_packet_t;


typedef struct _QSPI_module_priv_ QSPI_module_priv_t;

/** \brief QSPI Channel Private */
typedef struct _QSPI_channel_priv_
{
	const QSPI_channel_cfg_t	*mpConfig;		/**< Channel config */
	QSPI_module_priv_t			*mpParent;		/**< Module handle */
	Ifx_QSPI_BACON				mBaconReg;		/**< Current Basic configuration value */
	uint16_t					mnChannelID;	/**< Channel ID */
	uint16_t					mnCurDataWidth; /**< Current Data Width */
	QSPI_channel_state_t		mState;			/**< Channel state */
}QSPI_channel_priv_t;

/** \brief QSPI Module Private */
typedef struct _QSPI_module_priv_
{
	const QSPI_module_cfg_t		*mpConfig;									/**< Module config */
	uint32_t					mnBAUD2Freq;								/**< Peripheral Bus Frequency */
	uint16_t					mnChannelMsk;								/**< Active Channel mask */
	QSPI_channel_priv_t			mpChannels[DERIVATE_QSPI_NUM_CHAN_PER_MOD]; /**< Channel handles */
	/**** CIRCULAR QUEUE ***/
	QSPI_packet_t				mpPktQ[QSPI_MODULE_PACKET_QUEUE_DEPTH];		/**< Packet Q */
	uint16_t					mnPktQRD;									/**< Packet Q read pointer */
	uint16_t					mnPktQWR;									/**< Packet Q write pointer */
	sint16_t 					mnPktCnt;									/**< Packet Q fill level */
	/***********************/
	QSPI_module_state_t	 		mState;										/**< Module state */
	uint16_t					mnModuleID;									/**< Module ID */
}QSPI_module_priv_t;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief INLINE uint16_t sQSPI_module_getError(QSPI_module_priv_t *apModule)
 *
 * Get current error.
 *
 * \param apModule : QSPI module handle.
 * \return fill level
 */
INLINE uint16_t sQSPI_module_getError(QSPI_module_priv_t *apModule)
{
	return apModule->mpConfig->p_module->STATUS.B.ERRORFLAGS;
}

/**
 * \brief INLINE uint16_t sQSPI_module_clearError(QSPI_module_priv_t *apModule)
 *
 * Clear current error.
 *
 * \param apModule : QSPI module handle.
 * \return fill level
 */
INLINE uint16_t sQSPI_module_clearError(QSPI_module_priv_t *apModule)
{
	return apModule->mpConfig->p_module->FLAGSCLEAR.U = 0xFFFFU;
}

/**
 * \brief INLINE uint32_t sQSPI_module_getTxFIFOLevel(QSPI_module_priv_t *apModule)
 *
 * Get TX FIFO fill level.
 *
 * \param apModule : QSPI module handle.
 * \return fill level
 */
INLINE uint32_t sQSPI_module_getTxFIFOLevel(QSPI_module_priv_t *apModule)
{
	return apModule->mpConfig->p_module->STATUS.B.TXFIFOLEVEL;
}

/**
 * \brief INLINE uint32_t sQSPI_module_getRxFIFOLevel(QSPI_module_priv_t *apModule)
 *
 * Get RX FIFO fill level.
 *
 * \param apModule : QSPI module handle.
 * \return fill level
 */
INLINE uint32_t sQSPI_module_getRxFIFOLevel(QSPI_module_priv_t *apModule)
{
	return apModule->mpConfig->p_module->STATUS.B.RXFIFOLEVEL;
}

/**
 * \brief void sQSPI_module_TxISR_enable(QSPI_module_priv_t *apModule)
 *
 * Enable TX ISR
 *
 * \param apModule : QSPI module handle.
 * \return void
 */
INLINE void sQSPI_module_TxISR_enable(QSPI_module_priv_t *apModule)
{
	apModule->mpConfig->p_module->GLOBALCON1.B.TXEN = 1;
}

/**
 * \brief void sQSPI_module_TxISR_disable(QSPI_module_priv_t *apModule)
 *
 * Disable TX ISR
 *
 * \param apModule : QSPI module handle.
 * \return void
 */
INLINE void sQSPI_module_TxISR_disable(QSPI_module_priv_t *apModule)
{
	apModule->mpConfig->p_module->GLOBALCON1.B.TXEN = 0;
}

/**
 * \brief void sQSPI_module_RxISR_enable(QSPI_module_priv_t *apModule)
 *
 * Enable RX ISR
 *
 * \param apModule : QSPI module handle.
 * \return void
 */
INLINE void sQSPI_module_RxISR_enable(QSPI_module_priv_t *apModule)
{
	apModule->mpConfig->p_module->GLOBALCON1.B.RXEN = 1;
}

/**
 * \brief void sQSPI_module_RxISR_disable(QSPI_module_priv_t *apModule)
 *
 * Disable RX ISR
 *
 * \param apModule : QSPI module handle.
 * \return void
 */
INLINE void sQSPI_module_RxISR_disable(QSPI_module_priv_t *apModule)
{
	apModule->mpConfig->p_module->GLOBALCON1.B.RXEN = 0;
}

/**
 * \brief void sQSPI_module_TxISR_trigger(QSPI_module_priv_t *apModule)
 *
 * Trigger TX ISR
 *
 * \param apModule : QSPI module handle.
 * \return void
 */
INLINE void sQSPI_module_TxISR_trigger(QSPI_module_priv_t *apModule)
{
	apModule->mpConfig->p_module->STATUS.B.TXF = 0;
	apModule->mpConfig->p_module->STATUS.B.TXF = 1;
}

/**
 * \brief void sQSPI_module_RxISR_trigger(QSPI_module_priv_t *apModule)
 *
 * Trigger RX ISR
 *
 * \param apModule : QSPI module handle.
 * \return void
 */
INLINE void sQSPI_module_RxISR_trigger(QSPI_module_priv_t *apModule)
{
	apModule->mpConfig->p_module->STATUS.B.RXF = 1;
}


/**
 * \brief STATIC void sQSPI_module_init(uint8_t module_num)
 *
 * initialize module registers
 *
 * \param module_num : module ID.
 * \return void
 */
STATIC void sQSPI_module_init(uint8_t module_num);

/**
 * \brief STATIC RC_t sQSPI_module_transact(QSPI_module_priv_t *apModule, QSPI_exchange_t type)
 *
 * Module transaction
 *
 * \param apModule 	: QSPI module private handle.
 * \param type		: transaction type
 * \return void
 */
STATIC RC_t sQSPI_module_transact(QSPI_module_priv_t *apModule, QSPI_exchange_t type);

/**
 * \brief STATIC RC_t sQSPI_module_errorHandler(QSPI_module_priv_t *apModule)
 *
 * Transact packet
 *
 * \param apModule 	: QSPI module private handle.
 * \return void
 */
STATIC RC_t sQSPI_module_errorHandler(QSPI_module_priv_t *apModule);

/**
 * \brief STATIC void sQSPI_module_onPacketCompletion(QSPI_module_priv_t *apModule, QSPI_exchangeResult_t aResult)
 *
 * On Packet transaction Completion
 *
 * \param apModule 		: QSPI module private handle.
 * \param aResult		: transaction result
 * \return void
 */
STATIC void sQSPI_module_onPacketCompletion(QSPI_module_priv_t *apModule, QSPI_exchangeResult_t aResult);

/**
 * \brief STATIC uint8_t sQSPI_channel_calculatePrescaler(QSPI_channel_priv_t *apChannel)
 *
 * Calculate pre-scaler value to achieve the channel baud-rate
 *
 * \param apChannel : QSPI channel private handle.
 * \return pre-scaler value
 */
STATIC uint8_t sQSPI_channel_calculatePrescaler(QSPI_channel_priv_t *apChannel);

/**
 * \brief STATIC RC_t sQSPI_channel_calculateBasicConfig(QSPI_channel_priv_t *apChannel)
 *
 * Calculate basic configuration for a channel
 *
 * \param apChannel : QSPI channel private handle.
 * \return #RC_SUCCESS
 */
STATIC RC_t sQSPI_channel_calculateBasicConfig(QSPI_channel_priv_t *apChannel);

/**
 * \brief STATIC void sQSPI_channel_writeBOS(QSPI_channel_priv_t *apChannel)
 *
 * Write Begin of Stream - Continuous mode
 *
 * \param apChannel : QSPI channel private handle.
 * \return void
 */
STATIC void sQSPI_channel_writeBOS(QSPI_channel_priv_t *apChannel);

/**
 * \brief STATIC void sQSPI_channel_writeEOS(QSPI_channel_priv_t *apChannel)
 *
 * Write End of Stream - Continuous mode
 *
 * \param apChannel : QSPI channel private handle.
 * \return void
 */
STATIC void sQSPI_channel_writeEOS(QSPI_channel_priv_t *apChannel);

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
STATIC RC_t sQSPI_channel_writePacket(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt);

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
STATIC RC_t sQSPI_channel_readPacket(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt);

/**
 * \brief STATIC void sQSPI_packet_reset(QSPI_packet_t *apPkt)
 *
 * reset Packet
 *
 * \param apPkt : pointer to packet.
 * \return void
 */
STATIC void sQSPI_packet_reset(QSPI_packet_t *apPkt);

/**
 * \brief STATIC void sQSPI_packet_readData(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
 *
 * Read Data to Packet
 *
 * \param apChannel : QSPI channel private handle.
 * \param apPkt		: Packet
 * \return void
 */
STATIC void sQSPI_packet_readData(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt);

/**
 * \brief STATIC void sQSPI_packet_writeData(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt)
 *
 * Write Data from Packet
 *
 * \param apChannel : QSPI channel private handle.
 * \param apPkt		: Packet
 * \return void
 */
STATIC void sQSPI_packet_writeData(QSPI_channel_priv_t *apChannel, QSPI_packet_t *apPkt);

#ifdef __cplusplus
}
#endif

#endif /* SRC_BSW_DRV_QSPI_PRIV_QSPI_PRIV_H_ */
