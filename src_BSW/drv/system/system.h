/**
 * Project:	Student Car v3
 *
 * \file	system.h
 *
 * \author	Willi Theiﬂ		HighTec EDV Systeme GmbH
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \author	Martin Musiol 	Infineon				-	martin.musiol@infineon.com
 *
 * \version	0.3 	14.10.2016
 *
 * \brief Basic system control API declaration
 *
 *  A API providing general system control functions like
 *  - PLL control
 *  - interrupt enable/disable
 *  - access protection enable/disable
 *  - software reset
 *  - power management
 *
 *	Changelog:\n
 *	- 0.4	25.04.2017 Barth
 *		-	SYSTEM_DisableInterrupts now returns the previous interrupt state
 *		- 	added #SYSTEM_restoreInterrupts
 *		-	\todo: make interrupt controll functions inline
 *		- 	made #SYSTEM_GetCoreId inline and the return type #SYSTEM_GetCoreId
 *  - 0.3	14.10.2016 Barth
 *		- Added #SYSTEM_DelayUs
 *	- 0.2	05.10.2016 Barth
 *		- Modified #disable_external_watchdog to work with the TLF B-Step.
 *	- 0.1	05.07.2016 Barth
 *		- Removed Clock/PLL related functions and moved them to #clock.h / #clock.c
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


#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include "global.h"
#include <machine/wdtcon.h>
#include "register.h"


/** \\brief  Clock Control Register */
typedef struct
{
    unsigned int DISR:1;                    /**< \brief [0:0] Module Disable Request Bit (rw) */
    unsigned int DISS:1;                    /**< \brief [1:1] Module Disable Status Bit (rh) */
    unsigned int reserved_2:1;              /**< \brief \internal Reserved */
    unsigned int EDIS:1;                    /**< \brief [3:3] Sleep Mode Enable Control (rw) */
    unsigned int reserved_4:28;             /**< \brief \internal Reserved */
} CLC_Bits;

/** \\brief  Kernel Reset Register 0 */
typedef struct
{
    unsigned int RST:1;                     /**< \brief [0:0] Kernel Reset (rwh) */
    unsigned int RSTSTAT:1;                 /**< \brief [1:1] Kernel Reset Status (rh) */
    unsigned int reserved_2:30;             /**< \brief \internal Reserved */
} KRST0_Bits;

/** \\brief  Kernel Reset Register 1 */
typedef struct
{
    unsigned int RST:1;                     /**< \brief [0:0] Kernel Reset (rwh) */
    unsigned int reserved_1:31;             /**< \brief \internal Reserved */
} KRST1_Bits;

/** \\brief  Kernel Reset Status Clear Register */
typedef struct
{
    unsigned int CLR:1;                     /**< \brief [0:0] Kernel Reset Status Clear (w) */
    unsigned int reserved_1:31;             /**< \brief \internal Reserved */
} KRSTCLR_Bits;


//***********************************************************************************//
//****************** Basic System functions *****************************************//
//***********************************************************************************//

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*! \brief System initialization
 *
 *  Do basic system initialization:
 *  - clock system init
 *  - TFL init
 *  - DTS init
 */
RC_t SYSTEM_Init(void);

/*! \brief Get core id of current core
 *
 * \return the current core id
 */
INLINE CpuId_t SYSTEM_GetCoreId(void){
	return (CpuId_t)(_mfcr(CPU_CORE_ID) & IFX_CPU_CORE_ID_CORE_ID_MSK);
}

/*! \brief Delays the application for a defined time (us)
 *
 * When calling this function, the system timer (STM) of the calling CPU will be used to delay the execution.
 * \param us_delay Delay in µs. Can not be smaller than #SYSTEM_CFG_DELAY_MIN
 * \note STM Compare 0 is used in this function.
 * \note Due to configuration overhead, the functions needs ~0.5-1µs longer than the given parameter. It is recommend to evaluate the functionality for critical scenarios.
 */
void SYSTEM_DelayUs(uint32_t us_delay);

//***********************************************************************************//
//****************** Interrupts *****************************************************//
//***********************************************************************************//

/*! \brief Check if interrupts are enabled
 */
INLINE boolean_t SYSTEM_AreInterruptsEnabled(void){
	Ifx_CPU_ICR reg;
	reg.U = __mfcr(CPU_ICR);
	return reg.B.IE != 0;
}

/*! \brief Globally enable interrupts
 */
INLINE void SYSTEM_EnableInterrupts(void){
	_enable();
}

/*! \brief Globally disable interrupts
 * \return previous state
 */
INLINE boolean_t SYSTEM_DisableInterrupts(void){
	boolean_t enabled;
    enabled = SYSTEM_AreInterruptsEnabled();
    _disable();
    _nop();
    return enabled;
}

/*! \brief Restore previous interrupt state received from #SYSTEM_DisableInterrupts
 * \param enabled	holds the previous interrupt state, if it is true, #SYSTEM_EnableInterrupts is called.
 */
INLINE void SYSTEM_restoreInterrupts(boolean_t enabled){
    if (enabled != FALSE)
    	SYSTEM_EnableInterrupts();
}

//***********************************************************************************//
//****************** (S)ENDINIT *****************************************************//
//***********************************************************************************//

/*! \brief Lock the ENDINIT protection of the current core
 */
INLINE void SYSTEM_LockENDINIT(void){
    lock_wdtcon();
}

/*! \brief Unlock the ENDINIT protection of the current core
 */
INLINE void SYSTEM_UnlockENDINIT(void){
    unlock_wdtcon();
}

/*! \brief Lock the SENDINIT protection of the system
 */
INLINE void SYSTEM_LockSENDINIT(void){
    lock_safety_wdtcon();
}

/*! \brief Unlock the SENDINIT protection of the system
 */
INLINE void SYSTEM_UnlockSENDINIT(void){
    unlock_safety_wdtcon();
}

/* ! \brief unlock (S)ENDINIT
 *   \param Sel	0,1,2 core WDT(ENDINIT) 3 SWDT (SENDINIT)
 */
INLINE void SYSTEM_EnableProtectionExt(int Sel){
    if (Sel < 3)
        lock_wdtcon();          /* CPU watchdog */
    else
        lock_safety_wdtcon();   /* Safety watchdog */
}

/* ! \brief lock (S)ENDINIT
 *   \param Sel	0,1,2 core WDT(ENDINIT) 3 SWDT (SENDINIT)
 */
INLINE void SYSTEM_DisableProtectionExt(int Sel){
    if (Sel < 3)
        unlock_wdtcon();        /* CPU watchdog */
    else
        unlock_safety_wdtcon(); /* Safety watchdog */
}

//***********************************************************************************//
//****************** System *********************************************************//
//***********************************************************************************//

/*! \brief Execute software reset
 */
int SYSTEM_Reset(void);

/*! \brief Execute software reset
 */
int SYSTEM_ResetFromPxros(void);

/*! \brief Execute Idle instruction
 */
int SYSTEM_Idle(void);

/*! \brief Execute Idle instruction for a remote core
 * \param CoreID	Core to be send to idle
 * \return always 0 TODO: Check dependencies and get rid of return value
 */
int SYSTEM_IdleExt(int CoreId);

/*! \brief Execute power down function
 */
int SYSTEM_Sleep(void);

/*! \brief Debug break system
 */
void SYSTEM_DbgBreak(void);

//***********************************************************************************//
//****************** Cache **********************************************************//
//***********************************************************************************//

/*! \brief Check if instruction cache is enabled */
int SYSTEM_IsCacheEnabled(void);

/*! \brief Enable/disable instruction and data cache */
void SYSTEM_EnaDisCache(int Enable);

//***********************************************************************************//
//****************** Module Control *************************************************//
//***********************************************************************************//
/*! \brief Enables the clock for a given module, while checking for timeout. Also the sleep mode is deactivated for the given module.
 *  \param 	p_module_clc Pointer to the modules CLC register
 *  \param 	timeout		Timeout in cycles to wait for the module to become active. If set to 0, #CFG_SYSTEM_CLC_TIMEOUT will be used.
 *  \return #RC_SUCCESS if the module clock was activated \n RC_ERROR_TIME_OUT if the operation timed out.
 *  \note ENDINIT is used in this function.
 *  \note Function is under test, successfully tested with: QSPI
 *  \note sleep mode is deactivated for the selected module
 *  \warning the timeout can not exceed the ENDINIT timeout. A trap is the result if the module was not started and the given timeout was not reached.
 */
RC_t SYSTEM_Module_CLC_Start(volatile CLC_Bits* p_module_clc, uint16_t timeout);


/*! \brief Performs a Kernel reset for a given module with timeout check
 *  \param 	p_module_krst0 Pointer to the modules KRST0 register
 *  \param 	p_module_krst1 Pointer to the modules KRST1 register
 *  \param 	p_module_krstclr Pointer to the modules KRSTCLR register
 *  \param 	timeout		Timeout in cycles to wait for the module to reset. If set to 0, #CFG_SYSTEM_KRST_TIMEOUT will be used.
 *  \return #RC_SUCCESS if the module clock was reset \n RC_ERROR_TIME_OUT if the operation timed out.
 *  \note ENDINIT is used in this function.
 *  \note Function is under test, successfully tested with: QSPI
 *  \warning the timeout can not exceed the ENDINIT timeout. A trap is the result if the module was not started and the given timeout was not reached.
 */
RC_t SYSTEM_Module_KRESET(volatile KRST0_Bits* p_module_krst0, KRST1_Bits* p_module_krst1, KRSTCLR_Bits* p_module_krstclr, uint16_t timeout);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SYSTEM_H__ */
