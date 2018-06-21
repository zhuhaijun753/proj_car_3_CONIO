/**
 * \file	PORT.cpp
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	07.05.2017
 * \version	0.2
 *
 * \brief Port manipulation functionalities
 *
 * Changelog:\n
 *	- 0.2 07.05.17 Rahul Raj
 *		- Added PORTPIN prop modification APIs\n\n
 *	- 0.1 31.05.15 Barth
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
#include <machine/wdtcon.h>
#include "PORT.h"
#include "PORT_cfg.h"
#include "system.h"

/*******************************************************************************
**                      Defines                                               **
*******************************************************************************/

#define PORT_PINSPERIOCR	0x4
#define PORT_IOCR_OFFSET	0x4
#define PORT_IOCR_PC_OFFSET	0x3
#define PORT_IOCR_PC_WIDTH	0x5
#define PORT_IOCR_MASK 		0x1f
#define PORT_PINSPERPDR		0x8
#define PORT_PDR_OFFSET		0x4
#define PORT_PDR_WIDTH 		0x4
#define PORT_PDR_MASK 		0xf

/*******************************************************************************
**                      Local Function Declaration                            **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definition                           **
*******************************************************************************/

#pragma section ".text.PORT" ax

RC_t PORT_Init(void)
{

	uint32_t i;

	//Pointer to IOCR register
	uint32_t* p_IOCR;

	//Pointer to PDR register
	uint32_t* p_PDR;

	//temporal copy of IOCR for modification
	uint32_t temp_IOCR;

	//temporal copy of PDR for modification
	uint32_t temp_PDR;



	//iterate trough pins
	for( i = 0; i < DERIVATE_NUM_PINS; i++)
	{

		unlock_wdtcon();

		//Set initial state for output pins
		if( (PORT_config[i].mode == _outputPushPullGeneral) || (PORT_config[i].mode == _outputOpenDrainGeneral) )
		{
			if(PORT_config[i].init_state == _cleared)
				PORT_config[i].p_module->OMR.U = BIT_1 << ( PORT_config[i].pin_no + IFX_P_OMR_PCL0_OFF );
			else
				PORT_config[i].p_module->OMR.U = BIT_1 << (PORT_config[i].pin_no);
		}

		//calculate pointer to IOCR
		p_IOCR = (uint32_t*) ( (uint32_t) &PORT_config[i].p_module->IOCR0 + ( (PORT_config[i].pin_no / PORT_PINSPERIOCR) * PORT_IOCR_OFFSET ) );

		//get copy
		temp_IOCR = *p_IOCR;

		//clear old bits
		temp_IOCR &= ~( PORT_IOCR_MASK << ( ( PORT_config[i].pin_no % PORT_PINSPERIOCR ) * ( PORT_IOCR_PC_WIDTH + PORT_IOCR_PC_OFFSET ) + PORT_IOCR_PC_OFFSET ) );

		//temp_IOCR new bits
		temp_IOCR |= PORT_config[i].mode << ( ( PORT_config[i].pin_no % PORT_PINSPERIOCR ) * ( PORT_IOCR_PC_WIDTH + PORT_IOCR_PC_OFFSET ) + PORT_IOCR_PC_OFFSET);

		//write to SFR
		*p_IOCR = temp_IOCR;

		//check if PES is active and set corresponding bit
		if( PORT_config[i].es_active == PES_active )
			PORT_config[i].p_module->ESR.U |= BIT_1 << PORT_config[i].pin_no;
		else
			PORT_config[i].p_module->ESR.U &= ~( BIT_1 << PORT_config[i].pin_no );

		//calculate pointer to PDR
		p_PDR = (uint32_t*) ( (uint32_t) &PORT_config[i].p_module->PDR0 + ( (PORT_config[i].pin_no / PORT_PINSPERPDR) * PORT_PDR_OFFSET ) );

		//get copy
		temp_PDR = *p_PDR;

		//clear old bits
		temp_PDR &= ~( PORT_PDR_MASK << ( ( PORT_config[i].pin_no % PORT_PINSPERPDR ) * ( PORT_PDR_WIDTH ) ) );

		//temp_IOCR new bits
		temp_PDR |= PORT_config[i].padmode << ( ( PORT_config[i].pin_no % PORT_PINSPERPDR ) * ( PORT_PDR_WIDTH ) );

		//write to SFR
		*p_PDR = temp_PDR;

		lock_wdtcon();

	}

	return RC_SUCCESS;
}

/*
 * Modifies Pin Properties.
 * @param property 	Property to be modified
 * @param value 	Property value
 * @return RC_SUCCESS on success, error-codes otherwise
 */
RC_t PORTPIN_modify_prop(PortPin_t pin, portPin_prop_t property, uint8_t value)
{
	switch (property)
	{
	case portPin_prop_Mode:
	{
		uint32_t* p_IOCR = NULL;
		uint32_t temp_IOCR = 0, 
				 mask = 0;

		p_IOCR = (uint32_t*) ( (uint32_t) &PORT_config[pin].p_module->IOCR0 + ( (PORT_config[pin].pin_no / PORT_PINSPERIOCR) * PORT_IOCR_OFFSET ) );

		//mask for other IOCR fields
		mask = ~( PORT_IOCR_MASK << ( ( PORT_config[pin].pin_no % PORT_PINSPERIOCR ) * ( PORT_IOCR_PC_WIDTH + PORT_IOCR_PC_OFFSET ) + PORT_IOCR_PC_OFFSET ) );

		//temp_IOCR new bits
		temp_IOCR = value << ( ( PORT_config[pin].pin_no % PORT_PINSPERIOCR ) * ( PORT_IOCR_PC_WIDTH + PORT_IOCR_PC_OFFSET ) + PORT_IOCR_PC_OFFSET);

		//write to SFR
		*p_IOCR = ((*p_IOCR & mask) | temp_IOCR);

	}
	break;

	case portPin_prop_PES:
	case portPin_prop_PadDriver:
	default:
	{
		// Not handled
	}
	break;


	}

	return RC_SUCCESS;
}

/*
 * Reverts Pin Properties to default values.
 * @param property 	Property to be reverted
 * @return RC_SUCCESS on success, error-codes otherwise
 */
RC_t PORTPIN_revert_prop(PortPin_t pin, portPin_prop_t property)
{
	switch (property)
	{
	case portPin_prop_Mode:
	{
		uint32_t* p_IOCR = NULL;
		uint32_t temp_IOCR = 0, 
				 mask = 0;

		p_IOCR = (uint32_t*) ( (uint32_t) &PORT_config[pin].p_module->IOCR0 + ( (PORT_config[pin].pin_no / PORT_PINSPERIOCR) * PORT_IOCR_OFFSET ) );

		//mask for other IOCR fields
		mask = ~( PORT_IOCR_MASK << ( ( PORT_config[pin].pin_no % PORT_PINSPERIOCR ) * ( PORT_IOCR_PC_WIDTH + PORT_IOCR_PC_OFFSET ) + PORT_IOCR_PC_OFFSET ) );

		//temp_IOCR new bits
		temp_IOCR =  PORT_config[pin].mode << ( ( PORT_config[pin].pin_no % PORT_PINSPERIOCR ) * ( PORT_IOCR_PC_WIDTH + PORT_IOCR_PC_OFFSET ) + PORT_IOCR_PC_OFFSET);

		//write to SFR
		*p_IOCR = ((*p_IOCR & mask) | temp_IOCR);

	}
	break;

	case portPin_prop_PES:
	case portPin_prop_PadDriver:
	default:
	{
		// Not handled
	}
	break;

	}

	return RC_SUCCESS;
}



#pragma section
