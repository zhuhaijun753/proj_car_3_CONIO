/**
 * \file	QSPI_cfg.c
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date	07.05.2017
 * \version	0.2
 *
 * \brief CANopen Core with basic protocol definitions and global functions
 *
 * Changelog:\n
 *	- 0.2 07.05.17 Rahul Raj
 *		- Added fields for Interrupt module and phase transition events
 *		- Removed interrupt priority definitions \n\n
 *	- 0.1 29.07.16 Barth
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

#include "QSPI_cfg.h"
#include "register.h"

const QSPI_module_cfg_t QSPI_cfg[DERIVATE_QSPI_NUM_MOD] =
{
//Module 0
	{
		TRUE,						//Module enable
		&MODULE_QSPI0,				//Pointer to the I2C-Module
		RxSel_a,					//Pin selection
		200000000,					//Max BaudRate - 200M, atleast 4 * max(ChannelBaud)
		&MODULE_SRC.QSPI.QSPI[0],	//Int. module
		0x00,						//TX FIFO Interrupt threshold
		0x00,						//RX FIFO Interrupt threshold
		TRUE,						//PhaseTransition-1 Event Int. enable
		FALSE,						//PhaseTransition-2 Event Int. enable
		QSPI_PHASETRANSITION_RBF,	//PhaseTransition-1 Event
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-2 Event
		{
		//Channel 0/8 Module 0
			{
				TRUE,					//channel enable - TFT
				48000000,				//Baudrate - 48M
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_MSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 1/9 Module 0
			{
				TRUE,					//channel enable - Touch
				2000000,				//Baudrate - 2M
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_MSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 2/10 Module 0
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 3/11 Module 0
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 4/12 Module 0
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 5/13 Module 0
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 6/14 Module 0
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 7/15 Module 0
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			}

		}
	},
//Module 1
	{
		FALSE,						//Module enable
		&MODULE_QSPI1,				//Pointer to the I2C-Module
		RxSel_a,					//Pin selection
		10000000,					//Max BaudRate
		&MODULE_SRC.QSPI.QSPI[1],	//Int. module
		0,							//TX FIFO Interrupt threshold
		0,							//RX FIFO Interrupt threshold
		FALSE,						//PhaseTransition-1 Event Int. enable
		FALSE,						//PhaseTransition-2 Event Int. enable
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-1 Event
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-2 Event
		{
		//Channel 0/8 Module 1
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 1/9 Module 1
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 2/10 Module 1
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 3/11 Module 1
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 4/12 Module 1
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 5/13 Module 1
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 6/14 Module 1
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 7/15 Module 1
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			}

		}
	},
//Module 2
	{
		FALSE,						//Module enable
		&MODULE_QSPI2,				//Pointer to the I2C-Module
		RxSel_a,					//Pin selection
		10000000,					//Max BaudRate
		&MODULE_SRC.QSPI.QSPI[2],	//Int. module
		0,							//TX FIFO Interrupt threshold
		0,							//RX FIFO Interrupt threshold
		FALSE,						//PhaseTransition-1 Event Int. enable
		FALSE,						//PhaseTransition-2 Event Int. enable
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-1 Event
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-2 Event
		{
		//Channel 0/8 Module 2
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 1/9 Module 2
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 2/10 Module 2
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 3/11 Module 2
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 4/12 Module 2
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 5/13 Module 2
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 6/14 Module 2
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 7/15 Module 2
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			}

		}
	},
//Module 3
	{
		TRUE,						//Module enable
		&MODULE_QSPI3,				//Pointer to the I2C-Module
		RxSel_a,					//Pin selection
		10000000,					//Max BaudRate
		&MODULE_SRC.QSPI.QSPI[3],	//Int. module
		0,							//TX FIFO Interrupt threshold
		0,							//RX FIFO Interrupt threshold
		FALSE,						//PhaseTransition-1 Event Int. enable
		FALSE,						//PhaseTransition-2 Event Int. enable
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-1 Event
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-2 Event
		{
		//Channel 0/8 Module 3
			{
				TRUE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_TRAILING,		//CPHA
				QSPI_CPOL_HIGH,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 1/9 Module 3
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 2/10 Module 3
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 3/11 Module 3
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 4/12 Module 3
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 5/13 Module 3
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 6/14 Module 3
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 7/15 Module 3
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			}

		}
	},
//Module 4
	{
		FALSE,						//Module enable
		&MODULE_QSPI4,				//Pointer to the I2C-Module
		RxSel_a,					//Pin selection
		10000000,					//Max BaudRate
		&MODULE_SRC.QSPI.QSPI[4],	//Int. module
		0,							//TX FIFO Interrupt threshold
		0,							//RX FIFO Interrupt threshold
		FALSE,						//PhaseTransition-1 Event Int. enable
		FALSE,						//PhaseTransition-2 Event Int. enable
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-1 Event
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-2 Event
		{
		//Channel 0/8 Module 4
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 1/9 Module 4
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 2/10 Module 4
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 3/11 Module 4
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 4/12 Module 4
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 5/13 Module 4
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 6/14 Module 4
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 7/15 Module 4
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			}

		}
	},
	//Module 5
	{
		FALSE,						//Module enable
		&MODULE_QSPI5,				//Pointer to the I2C-Module
		RxSel_a,					//Pin selection
		10000000,					//Max BaudRate
		&MODULE_SRC.QSPI.QSPI[5],	//Int. module
		0,							//TX FIFO Interrupt threshold
		0,							//RX FIFO Interrupt threshold
		FALSE,						//PhaseTransition-1 Event Int. enable
		FALSE,						//PhaseTransition-2 Event Int. enable
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-1 Event
		QSPI_PHASETRANSITION_NONE,	//PhaseTransition-2 Event
		{
		//Channel 0/8 Module 5
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 1/9 Module 5
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 2/10 Module 5
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 3/11 Module 5
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 4/12 Module 5
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 5/13 Module 5
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 6/14 Module 5
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			},
		//Channel 7/15 Module 5
			{
				FALSE,					//channel enable
				1000000,				//Baudrate
				QSPI_CPHA_LEADING,		//CPHA
				QSPI_CPOL_LOW,			//CPOL
				TRUE,					//Auto-CS
				FALSE,					//Invert-CS
				QSPI_SHFT_LSB_FIRST,	//Shift-Direction
				8,						//Data-Length
			}

		}
	}
};
