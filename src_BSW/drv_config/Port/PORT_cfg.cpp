/**
 * \file	PORT_cfg.c
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	07.05.2017
 * \version	0.3
 *
 * \brief Port Pin configuration
 *
 * Changelog:\n
 *	- 0.3 07.05.17 Rahul Raj
 *		- QSPI0, TFT, Touch pin assignments \n\n
 *	- 0.2
 *		- UART pin assignments? \n\n
 *	- 0.1 08.06.15 Barth
 *		- Initial version
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

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PORT_cfg.h"
#include "system_configuration.h"

#define PORT_DEFAULT_MODE				_inputPullUp

/*******************************************************************************
**                      Configuration                                         **
*******************************************************************************/


portPin_cfg_t const PORT_config[DERIVATE_NUM_PINS] =
{
		//Port 00

#if (ASCLIN3 == UART6)
		{(Ifx_P *)&MODULE_P00, 0,	_outputPushPullAlt3, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 00 Pin 0
#else
		{(Ifx_P *)&MODULE_P00, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 0
#endif

#if (ASCLIN3 == UART6)
		{(Ifx_P *)&MODULE_P00, 1,	_inputPullUp, 						PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 00 Pin 0
#else
		{(Ifx_P *)&MODULE_P00, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 1
#endif

		{(Ifx_P *)&MODULE_P00, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 2
		{(Ifx_P *)&MODULE_P00, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 3
		{(Ifx_P *)&MODULE_P00, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 4
		{(Ifx_P *)&MODULE_P00, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 5
		{(Ifx_P *)&MODULE_P00, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 6
		{(Ifx_P *)&MODULE_P00, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 7
		{(Ifx_P *)&MODULE_P00, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 8
		{(Ifx_P *)&MODULE_P00, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 9
		{(Ifx_P *)&MODULE_P00, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 10
		{(Ifx_P *)&MODULE_P00, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 11
#if (ASCLIN3 == UART6)
		{(Ifx_P *)&MODULE_P00, 12,	_inputNoPullDevice, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 00 Pin 12
#else
		{(Ifx_P *)&MODULE_P00, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 12
#endif
		{(Ifx_P *)&MODULE_P00, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 13
		{(Ifx_P *)&MODULE_P00, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 14
		{(Ifx_P *)&MODULE_P00, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 00 Pin 15

		//Port 01
		{(Ifx_P *)&MODULE_P01, 0,	PORT_DEFAULT_MODE,	 				PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 0
		{(Ifx_P *)&MODULE_P01, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 1
		{(Ifx_P *)&MODULE_P01, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 2
		{(Ifx_P *)&MODULE_P01, 3,	_outputPushPullAlt5, 				PES_inactive,		_cleared, cmosAutomotiveSpeed2},					//Port 01 Pin 3
		{(Ifx_P *)&MODULE_P01, 4,	_inputPullUp, 				        PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 4
		{(Ifx_P *)&MODULE_P01, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 5
		{(Ifx_P *)&MODULE_P01, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 6
		{(Ifx_P *)&MODULE_P01, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 7
		{(Ifx_P *)&MODULE_P01, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 8
		{(Ifx_P *)&MODULE_P01, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 9
		{(Ifx_P *)&MODULE_P01, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 10
		{(Ifx_P *)&MODULE_P01, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 11
		{(Ifx_P *)&MODULE_P01, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 12
		{(Ifx_P *)&MODULE_P01, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 13
		{(Ifx_P *)&MODULE_P01, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 14
		{(Ifx_P *)&MODULE_P01, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 01 Pin 15

		//Port 02

#if (ASCLIN2 == UART4)
		{(Ifx_P *)&MODULE_P02, 0,	_outputPushPullAlt2, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 02 Pin 0 - UART4 TX
#else
		{(Ifx_P *)&MODULE_P02, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 0
#endif

#if (ASCLIN2 == UART4)
		{(Ifx_P *)&MODULE_P02, 1,	_inputPullUp, 						PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 02 Pin 1 - UART4 RX
#else
		{(Ifx_P *)&MODULE_P02, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 1
#endif

#if (ASCLIN1 == UART2)
		{(Ifx_P *)&MODULE_P02, 2,	_outputPushPullAlt2, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 02 Pin 2 - UART2 TX
#else
		{(Ifx_P *)&MODULE_P02, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 2
#endif

#if (ASCLIN1 == UART2)
		{(Ifx_P *)&MODULE_P02, 3,	_inputPullUp, 						PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 02 Pin 3 - UART2 RX
#else
		{(Ifx_P *)&MODULE_P02, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 3
#endif

		{(Ifx_P *)&MODULE_P02, 4, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 4
		{(Ifx_P *)&MODULE_P02, 5, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 5
		{(Ifx_P *)&MODULE_P02, 6, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 6
		{(Ifx_P *)&MODULE_P02, 7, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 7
		{(Ifx_P *)&MODULE_P02, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 8
		{(Ifx_P *)&MODULE_P02, 9, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 9
		{(Ifx_P *)&MODULE_P02, 10, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 10
		{(Ifx_P *)&MODULE_P02, 11, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 11
		{(Ifx_P *)&MODULE_P02, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 12
		{(Ifx_P *)&MODULE_P02, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 13
		{(Ifx_P *)&MODULE_P02, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 14
		{(Ifx_P *)&MODULE_P02, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 02 Pin 15

		//Port 10
		{(Ifx_P *)&MODULE_P10, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 0
		{(Ifx_P *)&MODULE_P10, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 1
		{(Ifx_P *)&MODULE_P10, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 2
		{(Ifx_P *)&MODULE_P10, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 3
		{(Ifx_P *)&MODULE_P10, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 4
		{(Ifx_P *)&MODULE_P10, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 5
		{(Ifx_P *)&MODULE_P10, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 6
#if (ASCLIN2 == UART4)
		{(Ifx_P *)&MODULE_P10, 7,	_inputNoPullDevice, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 10 Pin 7 CTS for ASCLIN2 Zigbee
#else
		{(Ifx_P *)&MODULE_P10, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 7
#endif
		{(Ifx_P *)&MODULE_P10, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 8
		{(Ifx_P *)&MODULE_P10, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 9
		{(Ifx_P *)&MODULE_P10, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 10
		{(Ifx_P *)&MODULE_P10, 11, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 11
		{(Ifx_P *)&MODULE_P10, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 13
		{(Ifx_P *)&MODULE_P10, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 14
		{(Ifx_P *)&MODULE_P10, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 10 Pin 15


		//Port 11
		{(Ifx_P *)&MODULE_P11, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 0
		{(Ifx_P *)&MODULE_P11, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 1
		{(Ifx_P *)&MODULE_P11, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 2
		{(Ifx_P *)&MODULE_P11, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 3
		{(Ifx_P *)&MODULE_P11, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 4
		{(Ifx_P *)&MODULE_P11, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 5
		{(Ifx_P *)&MODULE_P11, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 6
		{(Ifx_P *)&MODULE_P11, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 7
		{(Ifx_P *)&MODULE_P11, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 8
		{(Ifx_P *)&MODULE_P11, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 9
		{(Ifx_P *)&MODULE_P11, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 11
		{(Ifx_P *)&MODULE_P11, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 11
		{(Ifx_P *)&MODULE_P11, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 12
		{(Ifx_P *)&MODULE_P11, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 13
		{(Ifx_P *)&MODULE_P11, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 14
		{(Ifx_P *)&MODULE_P11, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 11 Pin 15

		//Port 12
		{(Ifx_P *)&MODULE_P12, 0, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 12 Pin 0
		{(Ifx_P *)&MODULE_P12, 1, 	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 12 Pin 1

		//Port 13
		{(Ifx_P *)&MODULE_P13, 0,	_outputPushPullGeneral, 			PES_inactive,		_set, 	  cmosAutomotiveSpeed4},					//Port 13 Pin 0
		{(Ifx_P *)&MODULE_P13, 1,	_outputPushPullGeneral, 			PES_inactive,		_set, 	  cmosAutomotiveSpeed4},					//Port 13 Pin 1
		{(Ifx_P *)&MODULE_P13, 2,	_outputPushPullGeneral, 			PES_inactive,		_set, 	  cmosAutomotiveSpeed4},					//Port 13 Pin 2
		{(Ifx_P *)&MODULE_P13, 3,	_outputPushPullGeneral, 			PES_inactive,		_set, 	  cmosAutomotiveSpeed4},					//Port 13 Pin 3
		{(Ifx_P *)&MODULE_P13, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 4
		{(Ifx_P *)&MODULE_P13, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 5
		{(Ifx_P *)&MODULE_P13, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 6
		{(Ifx_P *)&MODULE_P13, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 7
		{(Ifx_P *)&MODULE_P13, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 9
		{(Ifx_P *)&MODULE_P13, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 11
		{(Ifx_P *)&MODULE_P13, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 11
		{(Ifx_P *)&MODULE_P13, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 12
		{(Ifx_P *)&MODULE_P13, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 13
		{(Ifx_P *)&MODULE_P13, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 14
		{(Ifx_P *)&MODULE_P13, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 13 Pin 15


		//Port 14
#if (ASCLIN0 == UARTUSB)
		{(Ifx_P *)&MODULE_P14, 0,	_outputPushPullAlt2, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 14 Pin 0
#else
		{(Ifx_P *)&MODULE_P14, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 0
#endif

#if (ASCLIN0 == UARTUSB)
		{(Ifx_P *)&MODULE_P14, 1,	_inputPullUp, 						PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 14 Pin 0
#else
		{(Ifx_P *)&MODULE_P14, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 1
#endif
		{(Ifx_P *)&MODULE_P14, 2,	_outputPushPullAlt3, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 14 Pin 2 - SPI2 / TLF35584 - Chip Select
		{(Ifx_P *)&MODULE_P14, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 3
		{(Ifx_P *)&MODULE_P14, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 4
		{(Ifx_P *)&MODULE_P14, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 5
		{(Ifx_P *)&MODULE_P14, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 6
		{(Ifx_P *)&MODULE_P14, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 7

#if (ASCLIN1 == UART3)
		{(Ifx_P *)&MODULE_P14, 8,	_inputPullUp, 						PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 14 Pin 8
#else
		{(Ifx_P *)&MODULE_P14, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 8
#endif

		{(Ifx_P *)&MODULE_P14, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 9
		{(Ifx_P *)&MODULE_P14, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 10
		{(Ifx_P *)&MODULE_P14, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 11
		{(Ifx_P *)&MODULE_P14, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 12
		{(Ifx_P *)&MODULE_P14, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 13
		{(Ifx_P *)&MODULE_P14, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 14
		{(Ifx_P *)&MODULE_P14, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 14 Pin 15

		//Port 15
		{(Ifx_P *)&MODULE_P15, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 0
		{(Ifx_P *)&MODULE_P15, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 1
		{(Ifx_P *)&MODULE_P15, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 2
		{(Ifx_P *)&MODULE_P15, 3,	_outputPushPullAlt3, 				PES_inactive,		_cleared, ttlSpeed1			  },					//Port 15 Pin 3 -- SPI2 / TLF35584 - SCLK
		{(Ifx_P *)&MODULE_P15, 4,	PORT_DEFAULT_MODE,	 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 15 Pin 4
#if (ASCLIN1 == UART1)
		{(Ifx_P *)&MODULE_P15, 5,	_inputPullUp, 						PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 15 Pin 5
#else
		{(Ifx_P *)&MODULE_P15, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4}, 					//Port 15 Pin 5
#endif
		{(Ifx_P *)&MODULE_P15, 6,	_outputPushPullAlt3, 				PES_inactive,		_cleared, ttlSpeed1			  },					//Port 15 Pin 6 -- SPI2 / TLF35584 - MTSR
		{(Ifx_P *)&MODULE_P15, 7,	   _inputPullUp, 					PES_inactive,		_cleared, ttlSpeed1			  },					//Port 15 Pin 7 -- SPI2 / TLF35584 - MRST
		{(Ifx_P *)&MODULE_P15, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 8
		{(Ifx_P *)&MODULE_P15, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 10
		{(Ifx_P *)&MODULE_P15, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 11
		{(Ifx_P *)&MODULE_P15, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 12
		{(Ifx_P *)&MODULE_P15, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 13
		{(Ifx_P *)&MODULE_P15, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 14
		{(Ifx_P *)&MODULE_P15, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 15 Pin 15

		//Port 20
		{(Ifx_P *)&MODULE_P20, 0,		_inputPullUp, 					PES_inactive,		_cleared,  cmosAutomotiveSpeed4},					//Port 20 Pin 0 - Touch - PendingIRQ
		{(Ifx_P *)&MODULE_P20, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 20 Pin 1
		{(Ifx_P *)&MODULE_P20, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 20 Pin 2
		//QSPI0 - SLSO9(Touch)
		{(Ifx_P *)&MODULE_P20, 3,	_outputPushPullAlt3, 				PES_inactive,		_set, 	  cmosAutomotiveSpeed1},					//Port 20 Pin 3 - Touch Controller Chip Select
		//QSPI0 - SLSO8(TFT)
		{(Ifx_P *)&MODULE_P20, 6,	_outputPushPullAlt3, 				PES_inactive,		_set, 	  cmosAutomotiveSpeed1},					//Port 20 Pin 6 - TFT Controller Chip Select

		//CAN
		{(Ifx_P *)&MODULE_P20, 7,		 _inputPullUp, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 20 Pin 7 - CAN0 Receiver
		{(Ifx_P *)&MODULE_P20, 8,	_outputPushPullAlt5, 				PES_inactive,		_cleared, cmosAutomotiveSpeed2},					//Port 20 Pin 8 - CAN0 Transmitter

		{(Ifx_P *)&MODULE_P20, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 20 Pin 9
#if (ASCLIN1 == UART1)
		{(Ifx_P *)&MODULE_P20, 10,	_outputPushPullAlt2, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 20 Pin 10
#else
		{(Ifx_P *)&MODULE_P20, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 20 Pin 10
#endif

		//QSPI0 - SCLK
		{(Ifx_P *)&MODULE_P20, 11,	_outputPushPullAlt3, 				PES_inactive,		_cleared, ttlSpeed1},					//Port 20 Pin 11
		//QSPI0 - MISO
		{(Ifx_P *)&MODULE_P20, 12,		_inputPullUp, 					PES_inactive,		_cleared, ttlSpeed1},					//Port 20 Pin 12
		//TFT - Backlight
		{(Ifx_P *)&MODULE_P20, 13,	_outputPushPullGeneral, 			PES_inactive,		_set, 	  ttlSpeed1},					//Port 20 Pin 13
		//QSPI0 - MOSI
		{(Ifx_P *)&MODULE_P20, 14,	_outputPushPullAlt3, 				PES_inactive,		_cleared, ttlSpeed1},					//Port 20 Pin 14

		//Port 21
		{(Ifx_P *)&MODULE_P21, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 21 Pin 0
		{(Ifx_P *)&MODULE_P21, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 21 Pin 1
		{(Ifx_P *)&MODULE_P21, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 21 Pin 2
		{(Ifx_P *)&MODULE_P21, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 21 Pin 3
		{(Ifx_P *)&MODULE_P21, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 21 Pin 4
		{(Ifx_P *)&MODULE_P21, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 21 Pin 5
		{(Ifx_P *)&MODULE_P21, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 21 Pin 6
		{(Ifx_P *)&MODULE_P21, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 21 Pin 7

		//Port 22
		{(Ifx_P *)&MODULE_P22, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 0
		{(Ifx_P *)&MODULE_P22, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 1
		{(Ifx_P *)&MODULE_P22, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 2
		{(Ifx_P *)&MODULE_P22, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 3
		{(Ifx_P *)&MODULE_P22, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 4
		{(Ifx_P *)&MODULE_P22, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 5
		{(Ifx_P *)&MODULE_P22, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 6
		{(Ifx_P *)&MODULE_P22, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 7
		{(Ifx_P *)&MODULE_P22, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 8
		{(Ifx_P *)&MODULE_P22, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 9
		{(Ifx_P *)&MODULE_P22, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 10
		{(Ifx_P *)&MODULE_P22, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 22 Pin 11

		//Port 23
		{(Ifx_P *)&MODULE_P23, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 23 Pin 0
		{(Ifx_P *)&MODULE_P23, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 23 Pin 1
		{(Ifx_P *)&MODULE_P23, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 23 Pin 2
		{(Ifx_P *)&MODULE_P23, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 23 Pin 3
		{(Ifx_P *)&MODULE_P23, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 23 Pin 4
		{(Ifx_P *)&MODULE_P23, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 23 Pin 5
		{(Ifx_P *)&MODULE_P23, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 23 Pin 6
		{(Ifx_P *)&MODULE_P23, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 23 Pin 7

		//Port 24
		{(Ifx_P *)&MODULE_P24, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 0
		{(Ifx_P *)&MODULE_P24, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 1
		{(Ifx_P *)&MODULE_P24, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 2
		{(Ifx_P *)&MODULE_P24, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 3
		{(Ifx_P *)&MODULE_P24, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 4
		{(Ifx_P *)&MODULE_P24, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 5
		{(Ifx_P *)&MODULE_P24, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 6
		{(Ifx_P *)&MODULE_P24, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 7
		{(Ifx_P *)&MODULE_P24, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 8
		{(Ifx_P *)&MODULE_P24, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 9
		{(Ifx_P *)&MODULE_P24, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 10
		{(Ifx_P *)&MODULE_P24, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 11
		{(Ifx_P *)&MODULE_P24, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 12
		{(Ifx_P *)&MODULE_P24, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 13
		{(Ifx_P *)&MODULE_P24, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 14
		{(Ifx_P *)&MODULE_P24, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 24 Pin 15

		//Port 25
		{(Ifx_P *)&MODULE_P25, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 0
		{(Ifx_P *)&MODULE_P25, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 1
		{(Ifx_P *)&MODULE_P25, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 2
		{(Ifx_P *)&MODULE_P25, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 3
		{(Ifx_P *)&MODULE_P25, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 4
		{(Ifx_P *)&MODULE_P25, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 5
		{(Ifx_P *)&MODULE_P25, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 6
		{(Ifx_P *)&MODULE_P25, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 7
		{(Ifx_P *)&MODULE_P25, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 8
		{(Ifx_P *)&MODULE_P25, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 9
		{(Ifx_P *)&MODULE_P25, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 10
		{(Ifx_P *)&MODULE_P25, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 11
		{(Ifx_P *)&MODULE_P25, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 12
		{(Ifx_P *)&MODULE_P25, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 13
		{(Ifx_P *)&MODULE_P25, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 14
		{(Ifx_P *)&MODULE_P25, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 25 Pin 15

		//Port 26
		{(Ifx_P *)&MODULE_P26, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 26 Pin 0

		//Port 30
		{(Ifx_P *)&MODULE_P30, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 0
		{(Ifx_P *)&MODULE_P30, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 1
		{(Ifx_P *)&MODULE_P30, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 2
		{(Ifx_P *)&MODULE_P30, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 3
		{(Ifx_P *)&MODULE_P30, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 4
		{(Ifx_P *)&MODULE_P30, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 5
		{(Ifx_P *)&MODULE_P30, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 6
		{(Ifx_P *)&MODULE_P30, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 7
		{(Ifx_P *)&MODULE_P30, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 8
		{(Ifx_P *)&MODULE_P30, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 9
		{(Ifx_P *)&MODULE_P30, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 10
		{(Ifx_P *)&MODULE_P30, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 11
		{(Ifx_P *)&MODULE_P30, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 12
		{(Ifx_P *)&MODULE_P30, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 13
		{(Ifx_P *)&MODULE_P30, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 14
		{(Ifx_P *)&MODULE_P30, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 30 Pin 15

		//Port 31
		{(Ifx_P *)&MODULE_P31, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 0
		{(Ifx_P *)&MODULE_P31, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 1
		{(Ifx_P *)&MODULE_P31, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 2
		{(Ifx_P *)&MODULE_P31, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 3
		{(Ifx_P *)&MODULE_P31, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 4
		{(Ifx_P *)&MODULE_P31, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 5
		{(Ifx_P *)&MODULE_P31, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 6
		{(Ifx_P *)&MODULE_P31, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 7
		{(Ifx_P *)&MODULE_P31, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 8
		{(Ifx_P *)&MODULE_P31, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 9
		{(Ifx_P *)&MODULE_P31, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 10
		{(Ifx_P *)&MODULE_P31, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 11
		{(Ifx_P *)&MODULE_P31, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 12
		{(Ifx_P *)&MODULE_P31, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 13
		{(Ifx_P *)&MODULE_P31, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 14
		{(Ifx_P *)&MODULE_P31, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 31 Pin 15

		//Port 32
		{(Ifx_P *)&MODULE_P32, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 32 Pin 0
		{(Ifx_P *)&MODULE_P32, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 32 Pin 2
		{(Ifx_P *)&MODULE_P32, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 32 Pin 3
		{(Ifx_P *)&MODULE_P32, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 32 Pin 4
		{(Ifx_P *)&MODULE_P32, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 32 Pin 5
		{(Ifx_P *)&MODULE_P32, 6,	PORT_DEFAULT_MODE,	 				PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 32 Pin 6
		{(Ifx_P *)&MODULE_P32, 7,	PORT_DEFAULT_MODE,	 				PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 32 Pin 7

		//Port 33
		{(Ifx_P *)&MODULE_P33, 0,	PORT_DEFAULT_MODE,	 				PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 0
		{(Ifx_P *)&MODULE_P33, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 1
		{(Ifx_P *)&MODULE_P33, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 2
		{(Ifx_P *)&MODULE_P33, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 3
		{(Ifx_P *)&MODULE_P33, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 4
		{(Ifx_P *)&MODULE_P33, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 5
		{(Ifx_P *)&MODULE_P33, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 6
		{(Ifx_P *)&MODULE_P33, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 7

#if (ASCLIN2 == UART5)
		{(Ifx_P *)&MODULE_P33, 8,	_inputPullUp, 						PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 33 Pin 8 - UART5 RX
#else
		{(Ifx_P *)&MODULE_P33, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 8
#endif

#if (ASCLIN2 == UART5)
		{(Ifx_P *)&MODULE_P33, 9,	_outputPushPullAlt2, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 33 Pin 9 - UART5 TX
#else
		{(Ifx_P *)&MODULE_P33, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 9
#endif

		{(Ifx_P *)&MODULE_P33, 10,	PORT_DEFAULT_MODE,					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 10
		{(Ifx_P *)&MODULE_P33, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 11

#if (ASCLIN1 == UART3)
		{(Ifx_P *)&MODULE_P33, 12,	_outputPushPullAlt2, 				PES_inactive,		_cleared, cmosAutomotiveSpeed1},					//Port 33 Pin 12
#else
		{(Ifx_P *)&MODULE_P33, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 12
#endif

		{(Ifx_P *)&MODULE_P33, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 13
		{(Ifx_P *)&MODULE_P33, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 14
		{(Ifx_P *)&MODULE_P33, 15,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 33 Pin 15

		//Port 34
		{(Ifx_P *)&MODULE_P34, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 34 Pin 1
		{(Ifx_P *)&MODULE_P34, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 34 Pin 2
		{(Ifx_P *)&MODULE_P34, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 34 Pin 3
		{(Ifx_P *)&MODULE_P34, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 34 Pin 4
		{(Ifx_P *)&MODULE_P34, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 34 Pin 5

		//Port 40
		{(Ifx_P *)&MODULE_P40, 0,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 0
		{(Ifx_P *)&MODULE_P40, 1,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 1
		{(Ifx_P *)&MODULE_P40, 2,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 2
		{(Ifx_P *)&MODULE_P40, 3,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 3
		{(Ifx_P *)&MODULE_P40, 4,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 4
		{(Ifx_P *)&MODULE_P40, 5,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 5
		{(Ifx_P *)&MODULE_P40, 6,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 6
		{(Ifx_P *)&MODULE_P40, 7,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 7
		{(Ifx_P *)&MODULE_P40, 8,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 8
		{(Ifx_P *)&MODULE_P40, 9,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 9
		{(Ifx_P *)&MODULE_P40, 10,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 10
		{(Ifx_P *)&MODULE_P40, 11,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 11
		{(Ifx_P *)&MODULE_P40, 12,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 12
		{(Ifx_P *)&MODULE_P40, 13,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4},					//Port 40 Pin 13
		{(Ifx_P *)&MODULE_P40, 14,	PORT_DEFAULT_MODE, 					PES_inactive,		_cleared, cmosAutomotiveSpeed4}						//Port 40 Pin 14
};


