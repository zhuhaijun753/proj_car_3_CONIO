/**
* \file swc_control_0_manual.h
* \author Fromm
* \date 04.06.2016
*
* \brief Simple showcase for manual driving using a Joystick
*
*
* \todo
*
* \warning
*
*  Changelog:\n
*  - 1.1; 10.5.17; Fromm
*            - Rearranged functions
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

#ifndef SWC_CONTROL_0_MANUAL_H_
#define SWC_CONTROL_0_MANUAL_H_

/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

/**
 * @brief Init function for manual driving
 * This function currently is empty
 * @return void
 */
void CONTROL0_init_manual();

/**
 * @brief De-Init function for manual driving
 * This function currently is empty
 * @return void
 */
void CONTROL0_deinit_manual();


/*****************************************************************************************
 * Non-Safety Function Implementations
 *****************************************************************************************/

/**
 * @brief Control function for manual driving
 * This function reads the joystick data and translates it into a driving command
 * @return void
 */
void CONTROL0_run_manual();




#endif /* SWC_CONTROL_0_MANUAL_H_ */
