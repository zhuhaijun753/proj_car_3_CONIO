/**
 * \file 	sys_timer.c
 * \author 	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \version	0.2
 * \date 	12.06.2018
 *
 * \brief System Timer interface
 *
 * \copyright Copyright ©2018
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

#include "spinlock.h"

#pragma section ".STM.locks"
volatile lock_t	g_STM0_lock=LOCK_FREE;
#pragma section

#pragma section ".STM.locks"
volatile lock_t	g_STM1_lock=LOCK_FREE;
#pragma section

#pragma section ".STM.locks"
volatile lock_t	g_STM2_lock=LOCK_FREE;
#pragma section
