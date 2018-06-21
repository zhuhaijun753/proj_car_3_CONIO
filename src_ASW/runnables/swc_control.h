/**
* \file swc_control.h
* \author Fromm
* \date 04.06.2016
*
* \brief Wrapper around alternative driving functions, which are selected through the remote control
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
#ifndef SWC_CONTROL_H_
#define SWC_CONTROL_H_

#include "runnable_table_type.h"

/*****************************************************************************************
 * Non-Safety Function Implementations
 *****************************************************************************************/

/**
 * @brief Central Wrapper
 * Calls the run mode selected through the state machine
 * @return void
 */
void CONTROL_run_selectedFeature();


/**
 * @brief Simple GUI service for showing a message on the remote control
 * Shows a string message on the GUI
 * @return void
 */
void CONTROL_showMessage(char* message);



#endif /* SWC_CONTROL_H_ */
