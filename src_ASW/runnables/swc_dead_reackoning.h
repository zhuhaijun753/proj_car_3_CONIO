/**
* \file swc_dead_reckoning.h
* \author Fromm
* \date 04.06.2016
*
* \brief Simple dead reckoning functions using the incremental decoders of the engines
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

#ifndef SWC_DEAD_REACKONING_H_
#define SWC_DEAD_REACKONING_H_

#include "runnable_table_type.h"

/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

/**
 * @brief Init function for dead reckoning
 * This function currently is empty
 * @return void
 */
void DR_init();

/**
 * @brief De-Init function for dead reckoning
 * This function currently is empty
 * @return void
 */
void DR_deinit();

/*****************************************************************************************
 * Safety Function Implementations
 *****************************************************************************************/

/**
 * @brief Read decoder value from driver
 * Safety runnable to get decoder values. Will operate CanOpen from core 0
 * The signal will also be used to check the output of the engine driver later on
 * @return Error code indicating wether a valid values was read
 */
RT_safety_t DR_run_getDecoder();

/**
 * @brief Read decoder value from driver using asynchronous messaging
 * Safety runnable to get decoder values. Will operate CanOpen from core 0
 * The signal will also be used to check the output of the engine driver later on
 * @return Error code indicating wether a valid values was read
 */
RT_safety_t DR_run_getDecoder_V2();

/*****************************************************************************************
 * Non-Safety Function Implementations
 *****************************************************************************************/


/**
 * @brief Non safety runnable to calculate position
 * Non safety runnable to calculate position
 * @return void
 */
void DR_run_calculatePosition();

/**
 * @brief Non safety runnable to report position
 * Non safety runnable to report position * @return void
 */

void DR_run_reportPosition();




#endif /* SWC_DEAD_REACKONING_H_ */
