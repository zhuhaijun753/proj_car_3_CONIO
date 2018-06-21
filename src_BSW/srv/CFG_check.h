/**
 * \file 	CFG_check.h
 * \author 	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	18.12.2016
 *
 * \brief Configuration integrity check
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
#ifndef SRC_BSW_SRV_CFG_CHECK_H_
#define SRC_BSW_SRV_CFG_CHECK_H_

#include "global.h"

/**
 * \brief Check the integrity of the system configuration
 *
 * Checks performed:
 * 	-	Casts:
 * 		-	CAN frame cast from AURIX to CO frame format
 *
 * 	-	Module configuration:
 * 		-	Interrupt Router configuration
 *
 * \return  #RC_SUCCESS configuration is valid
 * 			#RC_ERROR	configuration is invalid
 */
RC_t	CFG_check(void);


#endif /* SRC_BSW_SRV_CFG_CHECK_H_ */
