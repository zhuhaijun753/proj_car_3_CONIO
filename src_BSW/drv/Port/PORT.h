/**
 * \file	Port.h
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	07.05.2017
 * \version	0.2
 *
 * \brief Port manipulation functionalities
 *
 * Changelog:\n
 *	- 0.2 07.05.17 Rahul Raj
 *		- Added 'Port_t' enum and pin/port control APIs\n\n
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

#ifndef PORT_H_
#define PORT_H_

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

#include "global.h"
#include "register.h"

#include "PORT_cfg.h"

/** \brief Start-Address of PORT-20 Module registers */
#define	PORT_20_BEGIN		((unsigned int)&MODULE_P20)
/** \brief End-Address of PORT-20 Module registers */
#define	PORT_20_END    		((unsigned int)&MODULE_P20 + sizeof(Ifx_P))

typedef uint8_t		PinData_t;
typedef uint16_t 	PortData_t;

/*******************************************************************************
**                      Pin Properties				                          **
*******************************************************************************/
typedef enum
{
	portPin_prop_Mode = 0,
	portPin_prop_PES,
	portPin_prop_PadDriver
}portPin_prop_t;

/*******************************************************************************
**                      Pins						                          **
*******************************************************************************/
typedef enum
{
	Port00_Pin0,
	Port00_Pin1,
	Port00_Pin2,
	Port00_Pin3,
	Port00_Pin4,
	Port00_Pin5,
	Port00_Pin6,
	Port00_Pin7,
	Port00_Pin8,
	Port00_Pin9,
	Port00_Pin10,
	Port00_Pin11,
	Port00_Pin12,
	Port00_Pin13,
	Port00_Pin14,
	Port00_Pin15,

	Port01_Pin0,
	Port01_Pin1,
	Port01_Pin2,
	Port01_Pin3,
	Port01_Pin4,
	Port01_Pin5,
	Port01_Pin6,
	Port01_Pin7,
	Port01_Pin8,
	Port01_Pin9,
	Port01_Pin10,
	Port01_Pin11,
	Port01_Pin12,
	Port01_Pin13,
	Port01_Pin14,
	Port01_Pin15,

	Port02_Pin0,
	Port02_Pin1,
	Port02_Pin2,
	Port02_Pin3,
	Port02_Pin4,
	Port02_Pin5,
	Port02_Pin6,
	Port02_Pin7,
	Port02_Pin8,
	Port02_Pin9,
	Port02_Pin10,
	Port02_Pin11,
	Port02_Pin12,
	Port02_Pin13,
	Port02_Pin14,
	Port02_Pin15,

	Port10_Pin0,
	Port10_Pin1,
	Port10_Pin2,
	Port10_Pin3,
	Port10_Pin4,
	Port10_Pin5,
	Port10_Pin6,
	Port10_Pin7,
	Port10_Pin8,
	Port10_Pin9,
	Port10_Pin10,
	Port10_Pin11,
	Port10_Pin13,
	Port10_Pin14,
	Port10_Pin15,

	Port11_Pin0,
	Port11_Pin1,
	Port11_Pin2,
	Port11_Pin3,
	Port11_Pin4,
	Port11_Pin5,
	Port11_Pin6,
	Port11_Pin7,
	Port11_Pin8,
	Port11_Pin9,
	Port11_Pin10,
	Port11_Pin11,
	Port11_Pin12,
	Port11_Pin13,
	Port11_Pin14,
	Port11_Pin15,

	Port12_Pin0,
	Port12_Pin1,

	Port13_Pin0,
	Port13_Pin1,
	Port13_Pin2,
	Port13_Pin3,
	Port13_Pin4,
	Port13_Pin5,
	Port13_Pin6,
	Port13_Pin7,
	Port13_Pin9,
	Port13_Pin10,
	Port13_Pin11,
	Port13_Pin12,
	Port13_Pin13,
	Port13_Pin14,
	Port13_Pin15,

	Port14_Pin0,
	Port14_Pin1,
	Port14_Pin2,
	Port14_Pin3,
	Port14_Pin4,
	Port14_Pin5,
	Port14_Pin6,
	Port14_Pin7,
	Port14_Pin8,
	Port14_Pin9,
	Port14_Pin10,
	Port14_Pin11,
	Port14_Pin12,
	Port14_Pin13,
	Port14_Pin14,
	Port14_Pin15,

	Port15_Pin0,
	Port15_Pin1,
	Port15_Pin2,
	Port15_Pin3,
	Port15_Pin4,
	Port15_Pin5,
	Port15_Pin6,
	Port15_Pin7,
	Port15_Pin8,
	Port15_Pin10,
	Port15_Pin11,
	Port15_Pin12,
	Port15_Pin13,
	Port15_Pin14,
	Port15_Pin15,

	Port20_Pin0,
	Port20_Pin1,
	Port20_Pin2,
	Port20_Pin3,
	Port20_Pin6,
	Port20_Pin7,
	Port20_Pin8,
	Port20_Pin9,
	Port20_Pin10,
	Port20_Pin11,
	Port20_Pin12,
	Port20_Pin13,
	Port20_Pin14,

	Port21_Pin0,
	Port21_Pin1,
	Port21_Pin2,
	Port21_Pin3,
	Port21_Pin4,
	Port21_Pin5,
	Port21_Pin6,
	Port21_Pin7,

	Port22_Pin0,
	Port22_Pin1,
	Port22_Pin2,
	Port22_Pin3,
	Port22_Pin4,
	Port22_Pin5,
	Port22_Pin6,
	Port22_Pin7,
	Port22_Pin8,
	Port22_Pin9,
	Port22_Pin10,
	Port22_Pin11,

	Port23_Pin0,
	Port23_Pin1,
	Port23_Pin2,
	Port23_Pin3,
	Port23_Pin4,
	Port23_Pin5,
	Port23_Pin6,
	Port23_Pin7,

	Port24_Pin0,
	Port24_Pin1,
	Port24_Pin2,
	Port24_Pin3,
	Port24_Pin4,
	Port24_Pin5,
	Port24_Pin6,
	Port24_Pin7,
	Port24_Pin8,
	Port24_Pin9,
	Port24_Pin10,
	Port24_Pin11,
	Port24_Pin12,
	Port24_Pin13,
	Port24_Pin14,
	Port24_Pin15,

	Port25_Pin0,
	Port25_Pin1,
	Port25_Pin2,
	Port25_Pin3,
	Port25_Pin4,
	Port25_Pin5,
	Port25_Pin6,
	Port25_Pin7,
	Port25_Pin8,
	Port25_Pin9,
	Port25_Pin10,
	Port25_Pin11,
	Port25_Pin12,
	Port25_Pin13,
	Port25_Pin14,
	Port25_Pin15,

	Port26_Pin0,

	Port30_Pin0,
	Port30_Pin1,
	Port30_Pin2,
	Port30_Pin3,
	Port30_Pin4,
	Port30_Pin5,
	Port30_Pin6,
	Port30_Pin7,
	Port30_Pin8,
	Port30_Pin9,
	Port30_Pin10,
	Port30_Pin11,
	Port30_Pin12,
	Port30_Pin13,
	Port30_Pin14,
	Port30_Pin15,

	Port31_Pin0,
	Port31_Pin1,
	Port31_Pin2,
	Port31_Pin3,
	Port31_Pin4,
	Port31_Pin5,
	Port31_Pin6,
	Port31_Pin7,
	Port31_Pin8,
	Port31_Pin9,
	Port31_Pin10,
	Port31_Pin11,
	Port31_Pin12,
	Port31_Pin13,
	Port31_Pin14,
	Port31_Pin15,

	Port32_Pin0,
	Port32_Pin1,
	Port32_Pin2,
	Port32_Pin3,
	Port32_Pin4,
	Port32_Pin5,
	Port32_Pin6,
	Port32_Pin7,

	Port33_Pin0,
	Port33_Pin1,
	Port33_Pin2,
	Port33_Pin3,
	Port33_Pin4,
	Port33_Pin5,
	Port33_Pin6,
	Port33_Pin7,
	Port33_Pin8,
	Port33_Pin9,
	Port33_Pin10,
	Port33_Pin11,
	Port33_Pin12,
	Port33_Pin13,
	Port33_Pin14,
	Port33_Pin15,

	Port34_Pin1,
	Port34_Pin2,
	Port34_Pin3,
	Port34_Pin4,
	Port34_Pin5,

	Port40_Pin0,
	Port40_Pin1,
	Port40_Pin2,
	Port40_Pin3,
	Port40_Pin4,
	Port40_Pin5,
	Port40_Pin6,
	Port40_Pin7,
	Port40_Pin8,
	Port40_Pin9,
	Port40_Pin10,
	Port40_Pin11,
	Port40_Pin12,
	Port40_Pin13,
	Port40_Pin14

}PortPin_t;

/*******************************************************************************
**                      Ports						                          **
*******************************************************************************/

typedef enum
{
	Port00 = Port00_Pin0,	/** pins: 16 **/
	Port01 = Port01_Pin0,	/** pins: 16 **/
	Port02 = Port02_Pin0,	/** pins: 16 **/
	Port10 = Port10_Pin0,	/** pins: 15 **/
	Port11 = Port11_Pin0,	/** pins: 16 **/
	Port12 = Port12_Pin0, 	/** pins:  2 **/
	Port13 = Port13_Pin0,	/** pins: 15 **/
	Port14 = Port14_Pin0, 	/** pins: 16 **/
	Port15 = Port15_Pin0, 	/** pins: 15 **/
	Port20 = Port20_Pin0,	/** pins: 13 **/
	Port21 = Port21_Pin0,	/** pins:  8 **/
	Port22 = Port22_Pin0, 	/** pins: 12 **/
	Port23 = Port23_Pin0,	/** pins:  8 **/
	Port24 = Port24_Pin0, 	/** pins: 16 **/
	Port25 = Port25_Pin0, 	/** pins: 16 **/
	Port26 = Port26_Pin0, 	/** pins: 16 **/
	Port30 = Port30_Pin0,	/** pins:  1 **/
	Port31 = Port31_Pin0,	/** pins: 16 **/
	Port32 = Port32_Pin0, 	/** pins:  7 **/
	Port33 = Port33_Pin0,	/** pins: 16 **/
	Port34 = Port34_Pin1, 	/** pins:  5 **/
	Port40 = Port40_Pin0 	/** pins: 15 **/
}Port_t;

/* Input multiplexer definition used in PinMaps
 */
typedef enum
{
    RxSel_a = 0x0,
    RxSel_b,
    RxSel_c,
    RxSel_d,
    RxSel_e,
    RxSel_f,
    RxSel_g,
    RxSel_h
}RxSelect_t;

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Sets up all GPIO ports with a given configuration set
 * @param cfg 	Configuration set
 * @return pin	pin to be set
 */
RC_t PORT_Init(void);

/*
 * Modifies Pin Properties.
 * @param property 	Property to be modified
 * @param value 	Property value
 * @return RC_SUCCESS on success, error-codes otherwise
 */
RC_t PORTPIN_modify_prop(PortPin_t pin, portPin_prop_t property, uint8_t value);

/*
 * Reverts Pin Properties to default values.
 * @param property 	Property to be reverted
 * @return RC_SUCCESS on success, error-codes otherwise
 */
RC_t PORTPIN_revert_prop(PortPin_t pin, portPin_prop_t property);

/**
 * \brief RC_t PORTPIN_get(PortPin_t anPinID, PinData_t *apVal)
 *
 * Get a GPIO pin value.
 *
 * \param anPinID : identifier of pin to read, must not exceed #PORT_NO_OF_PINS -1. pin identifier starts with 0.
 * \param apVal	  : pointer to store the value read
 * \return #RC_ERROR_BAD_PARAM if \p anPinID exceeded #PORT_NO_OF_PINS -1 or if \p apVal is #NULL
 * \return #RC_SUCCESS otherwise
 *
 */
INLINE RC_t PORTPIN_get(PortPin_t anPinID, PinData_t *apVal)
{
	if ((DERIVATE_NUM_PINS <= anPinID) || (NULL == apVal))
		return RC_ERROR_BAD_PARAM;

	*apVal = (PinData_t)((PORT_config[anPinID].p_module->IN.U &
						 ((uint32_t)BIT_1 << PORT_config[anPinID].pin_no)) >> PORT_config[anPinID].pin_no);

	return RC_SUCCESS;
}

/**
 * \brief RC_t PORTPIN_set(PortPin_t anPinID)
 *
 * Set a GPIO pin.
 *
 * \param anPinID : identifier of pin to be set, must not exceed #DERIVATE_NUM_PINS -1. pin identifier starts with 0.
 * \param apVal	  : pointer to store the value read, must be a valid memory address
 * \return #RC_ERROR_BAD_PARAM if \p anPinID exceeded #DERIVATE_NUM_PINS -1 or if \p apVal is #NULL
 * \return #RC_SUCCESS otherwise
 *
 */
INLINE RC_t PORTPIN_set(PortPin_t anPinID)
{
	if (DERIVATE_NUM_PINS <= anPinID)
		return RC_ERROR_BAD_PARAM;

	PORT_config[anPinID].p_module->OMR.U = ((uint32_t)BIT_1 << PORT_config[anPinID].pin_no);

	return RC_SUCCESS;
}

/**
 * \brief RC_t PORTPIN_clr(PortPin_t anPinID)
 *
 * Clear a GPIO pin.
 *
 * \param anPinID : identifier of pin to be cleared, must not exceed #DERIVATE_NUM_PINS -1. pin identifier starts with 0.
 * \return #RC_ERROR_BAD_PARAM if \p anPinID exceeded #DERIVATE_NUM_PINS -1
 * \return #RC_SUCCESS otherwise
 *
 */
INLINE RC_t PORTPIN_clr(PortPin_t anPinID)
{
	if (DERIVATE_NUM_PINS <= anPinID)
		return RC_ERROR_BAD_PARAM;

	PORT_config[anPinID].p_module->OMR.U = ((uint32_t)BIT_1 << (IFX_P_OMR_PCL0_OFF + PORT_config[anPinID].pin_no));

	return RC_SUCCESS;
}

/**
 * \brief RC_t PORTPIN_toggle(PortPin_t anPinID)
 *
 * Toggle a GPIO pin.
 *
 * \param anPinID : identifier of pin to be toggled, must not exceed #DERIVATE_NUM_PINS -1. pin identifier starts with 0.
 * \return #RC_ERROR_BAD_PARAM if \p anPinID exceeded #DERIVATE_NUM_PINS -1
 * \return #RC_SUCCESS otherwise
 *
 */
INLINE RC_t PORTPIN_toggle(PortPin_t anPinID)
{
	if (DERIVATE_NUM_PINS <= anPinID)
		return RC_ERROR_BAD_PARAM;

	PORT_config[anPinID].p_module->OMR.U = (((uint32_t)BIT_1 << PORT_config[anPinID].pin_no) |
											((uint32_t)BIT_1 << (IFX_P_OMR_PCL0_OFF + PORT_config[anPinID].pin_no)));
	return RC_SUCCESS;
}

/**
 * \brief RC_t PORT_read(Port_t anPortID, PortData_t *apVal)
 *
 * Read a GPIO port.
 *
 * \param anPortID : identifier of port to be read, must not exceed #DERIVATE_NUM_PINS -1. pin identifier starts with 0.
 * \param apVal	   : pointer to store the value read, must be a valid memory address
 * \return #RC_ERROR_BAD_PARAM if \p anPortID exceeded #DERIVATE_NUM_PINS -1 or if \p apVal is #NULL
 * \return #RC_SUCCESS otherwise
 *
 */
INLINE RC_t PORT_read(Port_t anPortID, PortData_t *apVal)
{
	if ((DERIVATE_NUM_PINS <= anPortID) || (NULL == apVal))
		return RC_ERROR_BAD_PARAM;

	*apVal = (PortData_t)PORT_config[anPortID].p_module->IN.U;

	return RC_SUCCESS;
}

/**
 * \brief RC_t PORT_write(Port_t anPortID, PortData_t anVal)
 *
 * Write to a GPIO port.
 *
 * \param anPortID : identifier of port to be written, must not exceed #DERIVATE_NUM_PINS -1. pin identifier starts with 0.
 * \param anVal	   : value to be written
 * \return #RC_ERROR_BAD_PARAM if \p anPortID exceeded #DERIVATE_NUM_PINS -1
 * \return #RC_SUCCESS otherwise
 *
 */
INLINE RC_t PORT_write(Port_t anPortID, PortData_t anVal)
{
	if (DERIVATE_NUM_PINS <= anPortID)
		return RC_ERROR_BAD_PARAM;

	PORT_config[anPortID].p_module->OMR.U = ((uint32_t)anVal | (~(uint32_t)anVal << IFX_P_OMR_PCL0_OFF));

	return RC_SUCCESS;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PORT_H_ */
