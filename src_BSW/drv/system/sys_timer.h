/**
 * \file 	sys_timer.h
 * \author 	Thomas Barth - thomas.barth@h-da.de
 * \date 	04.06.2018
 * \version 0.1
 *
 * \brief System Timer interface
 *
 * The STM is an upward counter, running at frequency fSTM. In case of an Application Reset, the STM is reset if bit SCU_ARSTDIS.STMxDIS is cleared.
 * After reset, the STM is enabled and immediately starts counting up. It is not possible to affect the content of the timer during normal operation.
 * The timer registers can only be read but not written to. The STM can be optionally disabled for power-saving purposes, or suspended for debugging purposes.
 * In suspend mode, the STM clock is stopped but all registers are still readable.
 *
 * Each Core has an own STM. On the TC29x we have 3 cores with 3 STMs (0-2).
 * Each STM has 64 bit. There are 6 different views on the STM counter:
 *
 * TIM 0: no shift, LSB toggles with fSTM
 * TIM 1: shift by 4 	(fSTM/16)
 * TIM 2: shift by 8 	(fSTM/256)
 * TIM 3: shift by 12 	(fSTM/4096)
 * TIM 4: shift by 16 	(fSTM/65536)
 * TIM 5: shift by 20 	(fSTM/1048576)
 * TIM 6: shift by 32 	(fSTM/4294967296)
 *
 * This driver provides 32 and 64 bit read operations. In case of a 64 bit read, the higher bits are captured during the low bit read to ensure validity.
 *
 * Changelog:\n
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
#ifndef _SYS_TIMER_H_
#define _SYS_TIMER_H_

#include "global.h"
#include "clock.h"
#include "spinlock.h"

#include TC_INCLUDE(TCPATH/IfxStm_bf.h)
#include TC_INCLUDE(TCPATH/IfxStm_reg.h)
#include TC_INCLUDE(TCPATH/IfxStm_regdef.h)

#define TIM_0_SHFT				0										/** Number of bits the TIM 0 register is shifted from the beginning of the STM */
#define TIM_1_SHFT				4										/** Number of bits the TIM 1 register is shifted from the beginning of the STM */
#define TIM_2_SHFT				8										/** Number of bits the TIM 2 register is shifted from the beginning of the STM */
#define TIM_3_SHFT				12										/** Number of bits the TIM 3 register is shifted from the beginning of the STM */
#define TIM_4_SHFT				16										/** Number of bits the TIM 4 register is shifted from the beginning of the STM */
#define TIM_5_SHFT				20										/** Number of bits the TIM 5 register is shifted from the beginning of the STM */
#define TIM_6_SHFT				32										/** Number of bits the TIM 6 register is shifted from the beginning of the STM */


//--------- [API]

#define STM_0					0										/**< \brief Select STM 0 (Core 0) */
#define STM_1					1										/**< \brief Select STM 1 (Core 1) */
#define STM_2					2										/**< \brief Select STM 2 (Core 2) */

#define TIM_0					0										/**< \brief Select TIM 0: no shift, LSB toggles with fSTM) */
#define TIM_1					1										/**< \brief Select TIM 1: shift by 4 	(fSTM/16) 			\see #STM_getTIM0_freq */
#define TIM_2					2										/**< \brief Select TIM 2: shift by 8 	(fSTM/256) 			\see #STM_getTIM1_freq */
#define TIM_3					3										/**< \brief Select TIM 3: shift by 12 	(fSTM/4096) 		\see #STM_getTIM2_freq */
#define TIM_4					4										/**< \brief Select TIM 4: shift by 16 	(fSTM/65536) 		\see #STM_getTIM3_freq */
#define TIM_5					5										/**< \brief Select TIM 5: shift by 20 	(fSTM/1048576) 		\see #STM_getTIM4_freq */
#define TIM_6					6										/**< \brief Select TIM 6: shift by 32 	(fSTM/4294967296)	\see #STM_getTIM5_freq */


#define STM_getTIM0_freq()		(Clock_get_Frequency(fSTM)>>TIM_0_SHFT)	/**< \brief Get the current TIM0 frequency */
#define STM_getTIM1_freq()		(Clock_get_Frequency(fSTM)>>TIM_1_SHFT)	/**< \brief Get the current TIM1 frequency */
#define STM_getTIM2_freq()		(Clock_get_Frequency(fSTM)>>TIM_2_SHFT)	/**< \brief Get the current TIM2 frequency */
#define STM_getTIM3_freq()		(Clock_get_Frequency(fSTM)>>TIM_3_SHFT)	/**< \brief Get the current TIM3 frequency */
#define STM_getTIM4_freq()		(Clock_get_Frequency(fSTM)>>TIM_4_SHFT)	/**< \brief Get the current TIM4 frequency */
#define STM_getTIM5_freq()		(Clock_get_Frequency(fSTM)>>TIM_5_SHFT)	/**< \brief Get the current TIM5 frequency */
#define STM_getTIM6_freq()		(Clock_get_Frequency(fSTM)>>TIM_6_SHFT)	/**< \brief Get the current TIM6 frequency */

/**
 * \brief Get a 32Bit STM Timer value
 *
 * \param 	STM_sel	The STM to read (STM_0 - STM_2)
 * \param 	TIM_sel	The TIM to read (TIM_0 - TIM_6)
 * \return The current timer value
 */
#define STM_READ_32(STM_sel,TIM_sel,a)		_STM_READ_32(STM_sel,TIM_sel,a)

/**
 * \brief Get a 64Bit STM Timer value
 *
 * \param 	STM_sel	The STM to read (STM_0 - STM_2)
 * \param 	TIM_sel	The TIM to read (TIM_0 - TIM_6)
 * \return The current timer value
 */
#define STM_READ_64(STM_sel,TIM_sel,a) 		_STM_READ_64(STM_sel,TIM_sel,a)

//--------- [CFG]
/** \brief For 64 bit access we need spinlocks. This value defines the default timeout for such spinlocks */
#define STM_SPINLOCK_TO						100

//--------- [Memory protection helpers]

/** \brief STM 0 start address */
#define STM0_ADR_START						(&MODULE_STM0)

/** \brief STM 0 end address */
#define STM0_ADR_END						(((uint32_t)(&MODULE_STM0))+sizeof(Ifx_STM))

/** \brief STM 1 start address */
#define STM1_ADR_START						(&MODULE_STM1)

/** \brief STM 1 end address */
#define STM1_ADR_END						(((uint32_t)(&MODULE_STM1))+sizeof(Ifx_STM))

/** \brief STM 2 start address */
#define STM2_ADR_START						(&MODULE_STM2)

/** \brief STM 2 end address */
#define STM2_ADR_END						(((uint32_t)(&MODULE_STM2))+sizeof(Ifx_STM))

//--------- [internal. Do not use modify. Do not use directly]-------------------------------------------


#define _STM_READ_32(s,t,a) 					_STM_32_STM##s##_TIM##t(a)
#define _STM_READ_64(s,t,a) 					_STM_64_STM##s##_TIM##t(a)

extern volatile uint32_t	g_STM0_lock;
extern volatile uint32_t	g_STM1_lock;
extern volatile uint32_t	g_STM2_lock;

INLINE RC_t _STM_32_STM0_TIM0(uint32_t* p_value){*p_value=STM0_TIM0.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM0_TIM1(uint32_t* p_value){*p_value=STM0_TIM1.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM0_TIM2(uint32_t* p_value){*p_value=STM0_TIM2.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM0_TIM3(uint32_t* p_value){*p_value=STM0_TIM3.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM0_TIM4(uint32_t* p_value){*p_value=STM0_TIM4.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM0_TIM5(uint32_t* p_value){*p_value=STM0_TIM5.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM0_TIM6(uint32_t* p_value){*p_value=STM0_TIM6.U; return RC_SUCCESS;}

INLINE RC_t _STM_32_STM1_TIM0(uint32_t* p_value){*p_value=STM1_TIM0.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM1_TIM1(uint32_t* p_value){*p_value=STM1_TIM1.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM1_TIM2(uint32_t* p_value){*p_value=STM1_TIM2.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM1_TIM3(uint32_t* p_value){*p_value=STM1_TIM3.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM1_TIM4(uint32_t* p_value){*p_value=STM1_TIM4.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM1_TIM5(uint32_t* p_value){*p_value=STM1_TIM5.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM1_TIM6(uint32_t* p_value){*p_value=STM1_TIM6.U; return RC_SUCCESS;}

INLINE RC_t _STM_32_STM2_TIM0(uint32_t* p_value){*p_value=STM2_TIM0.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM2_TIM1(uint32_t* p_value){*p_value=STM2_TIM1.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM2_TIM2(uint32_t* p_value){*p_value=STM2_TIM2.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM2_TIM3(uint32_t* p_value){*p_value=STM2_TIM3.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM2_TIM4(uint32_t* p_value){*p_value=STM2_TIM4.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM2_TIM5(uint32_t* p_value){*p_value=STM2_TIM5.U; return RC_SUCCESS;}
INLINE RC_t _STM_32_STM2_TIM6(uint32_t* p_value){*p_value=STM2_TIM6.U; return RC_SUCCESS;}

INLINE RC_t _STM_64_STM0_TIM0(uint64_t* p_value){

	if((Lock_get_prot(&g_STM0_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM0_TIM0.U;
	*p_value|=((uint64_t)STM0_CAP.U)<<(32-TIM_0_SHFT);

	Lock_release(&g_STM0_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM0_TIM1(uint64_t* p_value){

	if((Lock_get_prot(&g_STM0_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM0_TIM1.U;
	*p_value|=((uint64_t)STM0_CAP.U)<<(32-TIM_1_SHFT);

	Lock_release(&g_STM0_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM0_TIM2(uint64_t* p_value){

	if((Lock_get_prot(&g_STM0_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM0_TIM2.U;
	*p_value|=((uint64_t)STM0_CAP.U)<<(32-TIM_2_SHFT);

	Lock_release(&g_STM0_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM0_TIM3(uint64_t* p_value){

	if((Lock_get_prot(&g_STM0_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM0_TIM3.U;
	*p_value|=((uint64_t)STM0_CAP.U)<<(32-TIM_3_SHFT);

	Lock_release(&g_STM0_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM0_TIM4(uint64_t* p_value){

	if((Lock_get_prot(&g_STM0_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM0_TIM4.U;
	*p_value|=((uint64_t)STM0_CAP.U)<<(32-TIM_4_SHFT);

	Lock_release(&g_STM0_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM0_TIM5(uint64_t* p_value){

	if((Lock_get_prot(&g_STM0_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM0_TIM5.U;
	*p_value|=((uint64_t)STM0_CAP.U)<<(32-TIM_5_SHFT);

	Lock_release(&g_STM0_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM0_TIM6(uint64_t* p_value){

	if((Lock_get_prot(&g_STM0_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM0_TIM6.U;
	*p_value|=((uint64_t)STM0_CAP.U)<<(32-TIM_6_SHFT);

	Lock_release(&g_STM0_lock);

	return RC_SUCCESS;
}



INLINE RC_t _STM_64_STM1_TIM0(uint64_t* p_value){

	if((Lock_get_prot(&g_STM1_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM1_TIM0.U;
	*p_value|=((uint64_t)STM1_CAP.U)<<(32-TIM_0_SHFT);

	Lock_release(&g_STM1_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM1_TIM1(uint64_t* p_value){

	if((Lock_get_prot(&g_STM1_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM1_TIM1.U;
	*p_value|=((uint64_t)STM1_CAP.U)<<(32-TIM_1_SHFT);

	Lock_release(&g_STM1_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM1_TIM2(uint64_t* p_value){

	if((Lock_get_prot(&g_STM1_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM1_TIM2.U;
	*p_value|=((uint64_t)STM1_CAP.U)<<(32-TIM_2_SHFT);

	Lock_release(&g_STM1_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM1_TIM3(uint64_t* p_value){

	if((Lock_get_prot(&g_STM1_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM1_TIM3.U;
	*p_value|=((uint64_t)STM1_CAP.U)<<(32-TIM_3_SHFT);

	Lock_release(&g_STM1_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM1_TIM4(uint64_t* p_value){

	if((Lock_get_prot(&g_STM1_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM1_TIM4.U;
	*p_value|=((uint64_t)STM1_CAP.U)<<(32-TIM_4_SHFT);

	Lock_release(&g_STM1_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM1_TIM5(uint64_t* p_value){

	if((Lock_get_prot(&g_STM1_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM1_TIM5.U;
	*p_value|=((uint64_t)STM1_CAP.U)<<(32-TIM_5_SHFT);

	Lock_release(&g_STM1_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM1_TIM6(uint64_t* p_value){

	if((Lock_get_prot(&g_STM1_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM1_TIM6.U;
	*p_value|=((uint64_t)STM1_CAP.U)<<(32-TIM_6_SHFT);

	Lock_release(&g_STM1_lock);

	return RC_SUCCESS;
}



INLINE RC_t _STM_64_STM2_TIM0(uint64_t* p_value){

	if((Lock_get_prot(&g_STM2_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM2_TIM0.U;
	*p_value|=((uint64_t)STM2_CAP.U)<<(32-TIM_0_SHFT);

	Lock_release(&g_STM2_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM2_TIM1(uint64_t* p_value){

	if((Lock_get_prot(&g_STM2_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM2_TIM1.U;
	*p_value|=((uint64_t)STM2_CAP.U)<<(32-TIM_1_SHFT);

	Lock_release(&g_STM2_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM2_TIM2(uint64_t* p_value){

	if((Lock_get_prot(&g_STM2_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM2_TIM2.U;
	*p_value|=((uint64_t)STM2_CAP.U)<<(32-TIM_2_SHFT);

	Lock_release(&g_STM2_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM2_TIM3(uint64_t* p_value){

	if((Lock_get_prot(&g_STM2_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM2_TIM3.U;
	*p_value|=((uint64_t)STM2_CAP.U)<<(32-TIM_3_SHFT);

	Lock_release(&g_STM2_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM2_TIM4(uint64_t* p_value){

	if((Lock_get_prot(&g_STM2_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM2_TIM4.U;
	*p_value|=((uint64_t)STM2_CAP.U)<<(32-TIM_4_SHFT);

	Lock_release(&g_STM2_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM2_TIM5(uint64_t* p_value){

	if((Lock_get_prot(&g_STM2_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM2_TIM5.U;
	*p_value|=((uint64_t)STM2_CAP.U)<<(32-TIM_5_SHFT);

	Lock_release(&g_STM2_lock);

	return RC_SUCCESS;
}

INLINE RC_t _STM_64_STM2_TIM6(uint64_t* p_value){

	if((Lock_get_prot(&g_STM2_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)
		return RC_ERROR_TIME_OUT;

	*p_value=STM2_TIM6.U;
	*p_value|=((uint64_t)STM2_CAP.U)<<(32-TIM_6_SHFT);

	Lock_release(&g_STM2_lock);

	return RC_SUCCESS;
}


/* code generator
	for(int stm=0;stm<3;stm++){
		for(int tim=0;tim<7;tim++)
			cout<<"INLINE RC_t _STM_32_STM"<<stm<<"_TIM"<<tim<<"(uint32_t* p_value){*p_value=STM"<<stm<<"_TIM"<<tim<<".U; return RC_SUCCESS;}"<<endl;
		cout<<endl;
	}


	for(int stm=0;stm<3;stm++){
		for(int tim=0;tim<7;tim++){
			//64 start
			cout<<"\n\
INLINE RC_t _STM_64_STM"<<stm<<"_TIM"<<tim<<"(uint64_t* p_value){\n\
\n\
	if((Lock_get_prot(&g_STM"<<stm<<"_lock,STM_SPINLOCK_TO))!=RC_SUCCESS)\n\
		return RC_ERROR_TIME_OUT;\n\
\n\
	*p_value=STM"<<stm<<"_TIM"<<tim<<".U;\n\
	*p_value|=((uint64_t)STM"<<stm<<"_CAP.U)<<(32-TIM_"<<tim<<"_SHFT);\n\
\n\
	Lock_release(&g_STM"<<stm<<"_lock);\n\
\n\
	return RC_SUCCESS;\n\
}"<<endl;
			//64 end
		}

	cout<<endl<<endl;
	}
 */

#endif /* _SYS_TIMER_H_ */
