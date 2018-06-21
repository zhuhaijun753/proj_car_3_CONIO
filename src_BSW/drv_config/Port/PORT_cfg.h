/**
 * \file	PORT_cfg.h
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	08.06.2015
 * \version	0.1
 *
 * \brief Port Pin configuration
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

#ifndef PORT_CFG_H_
#define PORT_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

#include "global.h"
#include "register.h"

/*******************************************************************************
**                      Enums                                                 **
*******************************************************************************/

typedef enum{

	//Input
	_inputNoPullDevice      	= 0,      	//No input pull device connected, tri-state mode
	_inputPullDown          	= 0x1,		//Input pull-down device connected
	_inputPullUp            	= 0x2,  	//Input pull-up device connected

	//Output, Push-pull
	_outputPushPullGeneral  	= 0x10,  	//General-purpose output
	_outputPushPullAlt1     	= 0x11,  	//Alternate output function 1
	_outputPushPullAlt2     	= 0x12, 	//Alternate output function 2
	_outputPushPullAlt3     	= 0x13,  	//Alternate output function 3
	_outputPushPullAlt4     	= 0x14,  	//Alternate output function 4
	_outputPushPullAlt5     	= 0x15,  	//Alternate output function 5
	_outputPushPullAlt6     	= 0x16,  	//Alternate output function 6
	_outputPushPullAlt7     	= 0x17,  	//Alternate output function 7

	//Output, Open-drain
	_outputOpenDrainGeneral 	= 0x18,  	//General-purpose output
	_outputOpenDrainAlt1    	= 0x19,  	//Alternate output function 1
	_outputOpenDrainAlt2    	= 0x1A,  	//Alternate output function 2
	_outputOpenDrainAlt3    	= 0x1B,  	//Alternate output function 3
	_outputOpenDrainAlt4    	= 0x1C,  	//Alternate output function 4
	_outputOpenDrainAlt5    	= 0x1D,  	//Alternate output function 5
	_outputOpenDrainAlt6    	= 0x1E,  	//Alternate output function 6
	_outputOpenDrainAlt7    	= 0x1F  	//Alternate output function 7
} portMode_t;

typedef enum{
	PES_active							=0x0,		//Pin is connected to PES
	PES_inactive						=0x1		//Pin is not connected to PES
} portPES_t;


typedef enum{
	_set					=0x0,		//Pin is set after initialization
	_cleared				=0x1		//Pin is cleared after initialization
} portInit_t;

typedef enum
{
    cmosAutomotiveSpeed1 = 0,  /**< \brief Speed grade 1. */
    cmosAutomotiveSpeed2 = 1,  /**< \brief Speed grade 2. */
    cmosAutomotiveSpeed3 = 2,  /**< \brief Speed grade 3. */
    cmosAutomotiveSpeed4 = 3,  /**< \brief Speed grade 4. */
    ttlSpeed1            = 8,  /**< \brief Speed grade 1. */
    ttlSpeed2            = 9,  /**< \brief Speed grade 2. */
    ttlSpeed3            = 10, /**< \brief Speed grade 3. */
    ttlSpeed4            = 11  /**< \brief Speed grade 4. */
}portPadDriver_t ;

/*******************************************************************************
**                      Declarations                                          **
*******************************************************************************/
typedef struct{
	Ifx_P* 				p_module;				// Pointer to Port register
	uint8_t				pin_no;					// Pin Number at this port
	portMode_t 			mode;					// Drive Mode of pin
	portPES_t 			es_active;				// determine if the Port Emergency Stop (PES) is active for this pin
	portInit_t			init_state;				// The state output pins will have after initialization
	portPadDriver_t		padmode;
}portPin_cfg_t;

EXTERN portPin_cfg_t const PORT_config[DERIVATE_NUM_PINS];

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* PORT_CFG_H_ */
