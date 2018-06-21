/**
 * \file	QSPI_cfg.h
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date	07.05.2017
 * \version	0.2
 *
 * \brief CANopen Core with basic protocol definitions and global functions
 *
 * Changelog:\n
 *	- 0.2 07.05.17 Rahul Raj
 *      - Defined pairity & phase transition events enum;
 * 		- Added fields for Interrupt module and phase transition events \n\n
 * 	- 0.1 29.07.16 Barth
 * 		- Initial version
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

#ifndef QSPI_CFG_H_
#define QSPI_CFG_H_

#include "global.h"
#include "register.h"
#include "PORT.h"

/** \brief QSPI Sampling point selection
 */
typedef enum{
    QSPI_CPHA_LEADING=0x0,		 /**< Sample on leading clock edge CPHA=0 */
    QSPI_CPHA_TRAILING=0x1,		 /**< Sample on trailing clock edge CPHA=1 */
} QSPI_CPHA_t;

/** \brief QSPI Clock polarity selection
 */
typedef enum{
    QSPI_CPOL_LOW=0x0,		 	/**< Clock is idle LOW CPOL=0 */
    QSPI_CPOL_HIGH=0x1,		 	/**< Clock is idle HIGH CPOL=1 */
} QSPI_CPOL_t;

/** \brief QSPI Shift direction
 */
typedef enum{
    QSPI_SHFT_LSB_FIRST=0x0,	/**< Shift LSB first */
    QSPI_SHFT_MSB_FIRST=0x1,	/**< Shift MSB first */
} QSPI_SHFT_t;


/** \brief QSPI Pairity Mode
 */
typedef enum{
	QSPI_PAIRITYMODE_EVEN = 0x0,	/**< Even Pairity */
	QSPI_PAIRITYMODE_ODD = 0x1,		/**< Odd Pairity */
}QSPI_PairityMode_t;

/** \brief QSPI Phase Transition events
 */
typedef enum{
	QSPI_PHASETRANSITION_BUSY 	= 0b000,	/**< End of Wait Phase */
	QSPI_PHASETRANSITION_SLCKPC = 0b001,
	QSPI_PHASETRANSITION_SOF 	= 0b010,	/**< Start of frame */
	QSPI_PHASETRANSITION_TBE 	= 0b011,	/**< TX Buffer Empty */
	QSPI_PHASETRANSITION_RBF 	= 0b100,	/**< RX Buffer Full */
	QSPI_PHASETRANSITION_EOF 	= 0b101,	/**< End of frame */
	QSPI_PHASETRANSITION_DNA	= 0b110,
	QSPI_PHASETRANSITION_CONT 	= 0b111,
	QSPI_PHASETRANSITION_NONE   = 0b1000  	// Personal
}QSPI_PhaseTransition_t;
/**
 * \brief QSPI Channel Configuration structure type
 */
typedef struct{
	boolean_t			enable;			/**< Defines if this channel is enabled  */
	uint32_t 			baudrate;		/**< SPI Channel Baudrate  */
	QSPI_CPHA_t			CPHA;			/**< SPI Channel Clock Phase (rising or falling) */
	QSPI_CPOL_t			CPOL;			/**< SPI Channel Clock Polarity (high or low) */
	boolean_t			AutoCS;			/**< SPI Channel 1 = chip select is controlled by the hardware module or, 0 = by software. */
	boolean_t			invertCS;		/**< SPI Channel CS inversion 0= active high 1=active low */
	QSPI_SHFT_t			LSB_first;
	uint8_t				data_length;	/**< SPI Channel Data Bits */
}QSPI_channel_cfg_t;


/**
 * \brief QSPI Configuration structure type
 */
typedef struct{
	boolean_t				enable;					/**< Defines if this module is enabled  */
	Ifx_QSPI* 				p_module;				/**< Pointer to the QSPI-Module */
	RxSelect_t				PISEL_MRIS;				/**< Pin Select for Master Mode. Slave mode is not implemented */
	uint32_t				MasterBaud;				/**< The maximum Baud Rate for this module. It can be divided for each channel */
	Ifx_SRC_QSPI			*p_src_module;			/**< Pointer to the Int. module  */
	uint8_t					txFIFOint_thres;		/**< Defines the TX Empty level for Int.  */
	uint8_t					rxFIFOint_thres;		/**< Defines the RX Fill level for Int.  */
	boolean_t				pt1Int_enable;			/**< Defines if PhaseTransition-1 Int. is enabled  */
	boolean_t				pt2Int_enable;			/**< Defines if PhaseTransition-2 Int. is enabled  */
	QSPI_PhaseTransition_t	pt1Int_event;			/**< Defines the PhaseTransition-1 Event */
	QSPI_PhaseTransition_t	pt2Int_event;			/**< Defines the PhaseTransition-2 Event */
	QSPI_channel_cfg_t		channelcfg[DERIVATE_QSPI_NUM_ECON_PER_MOD];

}QSPI_module_cfg_t;


EXTERN const QSPI_module_cfg_t QSPI_cfg[DERIVATE_QSPI_NUM_MOD];

/* Pins TC29
 * AppKit QSPI3	MRST3A@P02.05, MTSR3@P02.06, SCLK3@P02.07 SLSO30@P02.04
 *
 * QSPI0 	MRST0 a-h	P20.12; P22.9; 	P22.06; 	-; 		-; 		-;		-;		-;
 * 			MTSR0		P20.12;	P20.14;	P22.05; P22.10;
 * 			SCLK0		P20.11;	P20.13;	P22.07;	P22.08;
 * 			SLSO0 0-13	P20.08; P20.09; P20.13; P11.01; P11.11; P11.02; P20.10; P33.05; P20.06; P20.03; P22.11; P23.06; P22.04; P15.00;
 *
 * QSPI1 	MRST1 a-h	P10.01; P11.3; 		-; 		-; 		-; 		-;		-;		-;
 *
 * QSPI2 	MRST2 a-h	P15.04;	P15.07; P21.02; P34.04; P15.02; 	-;		-;		-;
 * 			MTSR2		P15.05; P15.06; P34.5;		-; 		-; 		-;		-;		-;
 * 			SCLK2		P15.03; P15.06;
 * 			SLSO2 0-13	P15.02; P14.02; P14.06; P14.03; P14.07; P15.01;	P33.13;	P20.10; P20.06; P20.03; P33.02; P33.06; P32.06; 	-;
 *
 * QSPI3 	MRST3 a-h	P02.05; P10.07; P01.05; 	-; 		-; 		-;		-;		-;
 * 			MTSR3		P01.06; P02.06; P10.06;		-; 		-; 		-;		-;		-;
 * 			SCLK3		P01.07; P02.07; P10.08;		-; 		-; 		-;		-;		-;
 * 			SLSO3 0-13	P02.04; P02.00;	P02.01;	P00.05;	P00.02;	P02.08;	P00.08;	P00.09;	P10.05;	P01.03;	P01.04;	 	-; 		-;
 *
 * QSPI4 	MRST4 a-h	P33.13; P22.01; P21.02; P21.00; 	-; 		-;		-;		-;
 *
 * QSPI5 	MRST5 a-h	P15.10; 	-; 		-; 		-; 		-; 		-;		-;		-;
 *
 */

#endif /* QSPI_CFG_H_ */
