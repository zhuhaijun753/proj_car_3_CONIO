/**
* \file swc_remote.h
* \author Fromm, Team Remote
* \date 04.06.2016
*
* \brief swc_remote
*
* Software component which is processing the incoming remote data command.
*
* \todo Check, if the function REMOTE_run_readAndCheckProtocol should be shifted into the saftey runnable
* \todo Add error handling
* \todo Add timeout in case of normal driving state
*
* \warning Depending on the data, handling will be performed by the central state machine or the SWC itself
*
*  Changelog:\n
*  - 1.0; 15.8.16; Team Remote
*            - Initial version
*  - 1.1; 16.8.16; Fromm
*            - Moved the connection handling to the state machine
*  - 1.2; 18.8.16; Fromm
*            - Changed the return value of REMOTE_run_readAndCheckProtocol
*
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

/*
 * swc_remote.h
 *
 *  Created on: 04.06.2016
 *      Author: Fromm
 */

#ifndef SWC_REMOTE_H_
#define SWC_REMOTE_H_

#include "runnable_table_type.h"

/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

/**
 * @brief Init of the remote system
 * Currently empty
 * @return void
 */
void REMOTE_init();

/**
 * @brief De-Init of the remote system
 * Currently empty
 * @return void
 */
void REMOTE_deinit();

/*****************************************************************************************
 * Safety Function Implementations
 *****************************************************************************************/





/**
 * @brief Read and checks the content of a remote protocol.
 * The runnable will be triggered by the reception of complete remote protocol
 * The runnable will set the value of the Joystick signal and will send events to the state machine task
 */
RT_safety_t REMOTE_run_readAndCheckProtocol();

/**
 * @brief Safety function to verify the values of the joystick
 * This function checks the age of the joystick signal
 * @return requested safety state
 */
RT_safety_t REMOTE_run_checkJoystickAge();

/**
 * @brief Safety function to verify the values of the joystick
 * This function checks values of the joystick signal
 * @return requested safety state
 */
RT_safety_t REMOTE_run_checkJoystickData();

/**
 * @brief Safety function to verify joystick is in zero position before the car starts to drive
 * Before leaving the prerun state, the joystick has to be moved into zero position
 * @return requested safety state
 */
RT_safety_t REMOTE_run_checkJoystickZero();







#endif /* SWC_REMOTE_H_ */
