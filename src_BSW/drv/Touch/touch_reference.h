/**
 * \file 	touch_reference.h
 * \author 	Rahul
 * \date 	10-Jun-2017
 *
 * \brief DEFAULT HEADER TEMPLATE
 *
 * <detailed description what the file (header and/or source it belongs to) does>
 *
 * \note <notes>
 * \todo <todos>
 * \warning <warnings, e.g. dependencies, order of execution etc.>
 *
 *  Changelog:\n
 *  - <version; data of change; author>
 *            - <description of the change>
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
#ifndef SRC_BSW_DRV_TOUCH_TOUCH_REFERENCE_H_
#define SRC_BSW_DRV_TOUCH_TOUCH_REFERENCE_H_

#include "global.h"

/*
 * Initialize Touch QSPI channel.
 * @param NIL
 * @return #RC_SUCCESS on success,
 * 		   error-codes on data transfer errors
 */
RC_t TOUCHDRIVER_init();


/*
 * Read Touch by polling.
 * @param px - x pixel
 * @param py - y pixel
 * @return #RC_SUCCESS on touch,
 * 		   #RC_ERROR on no touch
 * 		   error-codes on data transfer errors
 */
RC_t TOUCHDRIVER_read_poll(uint16_t *px, uint16_t *py);

#endif /* SRC_BSW_DRV_TOUCH_TOUCH_REFERENCE_H_ */
