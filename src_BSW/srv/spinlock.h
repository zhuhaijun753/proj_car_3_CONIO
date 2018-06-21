/**
 * \file 	spinlock.h
 * \author 	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \version	0.2
 * \date 	12.06.2018
 *
 * \brief Tricore atomic swap implementation for use in spinlocks
 *
 * On multicore controllers the user can not use simple while-loops to check if a semaphore is available or not.
 * The reason is that there are multiple instructions to read a semaphore and once it becomes free, multiple CPUs could claim it.
 * A solution to that problem are the SWAP instructions that work in an atomic way (in only one instruction).
 * The bus-arbitration permits multiple access to a single memory location at a time.
 *
 *	Changelog:\n
 *	- 0.2 12.06.2018 Barth
 *              - removed SWAP_M macro
 *              - added #Lock_get
 *              - added #Lock_release
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

#ifndef SPINLOCK_H_
#define SPINLOCK_H_

/*******************************************************************************
 **                      Includes                                              **
 *******************************************************************************/

#include "global.h"

/*******************************************************************************
 **                      Defines                                               **
 *******************************************************************************/

#define LOCK_FREE	 	0x0
#define LOCK_TAKEN 		0x1

//The NOPs in this macro are inserted to prevent a bus starve during spinlock
#define BUS_DELAY		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\

typedef uint32_t lock_t;

/*******************************************************************************
 **                      Spinlock Macros                                       **
 *******************************************************************************/

/**
 * \brief Try to get a lock with timeout protection
 *
 * \param p_lock 	Pointer to the lock location. WARNING! for performance reasons, this parameter is not checked for validity!
 * \param timeout 	Timeout value for obtaining the lock.
 * \return 			#RC_SUCCESS			Lock was obtained
 * 					#RC_ERROR_TIME_OUT	Lock could not be obtained in time
 * \warning 		For performance reasons, the parameters are not checked for validity!
 */
INLINE RC_t Lock_get_prot(volatile lock_t* p_lock, uint32_t timeout){

	//this value is the value we want to write into the lock
	lock_t __SWAP_M_result;

	//stores how long we already wait to get the lock
	uint32_t __cycles_passed = 0;

	do{
		//write lock value
		__SWAP_M_result = LOCK_TAKEN;

		//perform swap
		__asm__ volatile (
				"swap.w [%1]0,%0"
				: "+d" (__SWAP_M_result)
				: "a" (p_lock)
				: "memory");

		//check if we got the lock
		if (__SWAP_M_result == LOCK_TAKEN){

			//check if we have a timeout
			if (++__cycles_passed > timeout)
				return RC_ERROR_TIME_OUT;

			//bus starve protection
			BUS_DELAY
		}
	//retry
	} while (__SWAP_M_result == LOCK_TAKEN);

	return RC_SUCCESS;
}

/**
 * \brief Try to get a lock WITHOUT timeout protection
 *
 * \param p_lock 	Pointer to the lock location. WARNING! for performance reasons, this parameter is not checked for validity!
 * \return 			#RC_SUCCESS			Lock was obtained
 * 					#RC_ERROR_TIME_OUT	Lock could not be obtained in time
 * \warning 		For performance reasons, the parameters are not checked for validity!
 * \warning		If the lock is not freed by another instance, this function will not exit and might lock the system!
 */
INLINE RC_t Lock_get_unprot(volatile lock_t* p_lock){

	//this value is the value we want to write into the lock
	lock_t __SWAP_M_result;

	do{
		//write lock value
		__SWAP_M_result = LOCK_TAKEN;

		//perform swap
		__asm__ volatile (
				"swap.w [%1]0,%0"
				: "+d" (__SWAP_M_result)
				: "a" (p_lock)
				: "memory");

		//check if we got the lock
		if (__SWAP_M_result == LOCK_TAKEN){
			//bus starve protection
			BUS_DELAY
		}
	//retry
	} while (__SWAP_M_result == LOCK_TAKEN);

	return RC_SUCCESS;
}

/**
 * \brief Release the lock
 *
 * \param p_lock 	Pointer to the lock location. WARNING! for performance reasons, this parameter is not checked for validity!
 * \return 			#RC_SUCCESS			Lock was freed
 * \warning 		For performance reasons, the parameters are not checked for validity!
 */
INLINE RC_t Lock_release(volatile lock_t* p_lock){

	*p_lock = LOCK_FREE;

	return RC_SUCCESS;
}

#endif /* SPINLOCK_H_ */

