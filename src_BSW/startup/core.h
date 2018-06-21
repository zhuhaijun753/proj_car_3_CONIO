/**
 * Project:	Student Car v3
 *
 * \file	core.h
 *
 * \author	Horst Lehser	HighTec EDV-Systeme GmbH
 *
 * \version	0.1 	14.05.2016
 *
 *  @brief CORE initialization structure
 *
 *
 *	Changelog:
 *
 */

#ifndef _CORE_H_
#define _CORE_H_

#include "register.h"
#include "global.h"

/**
 * @brief ClearTable for each CPU
 *
 * The cleartable is generated in the linker and will clear (set 0) all RAM regions that are defined.
 */
typedef struct cleartable {
  uint32_t *base;          // base address of this area
  sint32_t size;           // size of this area
} ClearTable_t;

/**
 * @brief CopyTable for each CPU
 *
 * The CopyTable is generated in the linker and copy the defined memory ranges from ROM to RAM
 */
typedef struct copytable {
  uint32_t *src;           // base address of the initial data
  uint32_t *dst;           // Destination address to copy to
  sint32_t size;           // size of the area to copy
} CopyTable_t;


/**
 * @brief Basic CPU configuration-set
 */
typedef struct coreinit {
	const ClearTable_t	*cleartable;     		// the clear table of this CPU
	const CopyTable_t   *copytable;      		// the copy table
	uint32_t         	*istack;        		// the interrupt stack
	uint32_t         	*ustack;        		// the initial userstack
	uint32_t         	*smallA0;       		// base of the small data area with base A0
	uint32_t         	*smallA1;       		// base of the small data area with base A1
	uint32_t        	*smallA8;       		// base of the small data area with base A8
	uint32_t         	*smallA9;       		// base of the small data area with base A9
	uint32_t         	csaBase;       			// base address of the CSA area
	uint32_t         	csaSize;        		// size of the CSA area
	uint32_t			*InterruptTable;		// Interrupt-Table address
	uint32_t			*TrapTable;				// Trap-Table address
	volatile Ifx_SCU_WDTCPU_CON0  *wdtCon0;     // watchdog register base (address of wdtcon0)
	volatile Ifx_SCU_WDTCPU_CON1  *wdtCon1;     // watchdog register base (address of wdtcon1)
} CoreInit_t;

/**
 *@brief CSA structure
 */
typedef struct {
	uint32_t reg[16];
} csa_t;

/**
 * @brief PCXI register definition
 */
typedef union {
	struct {
		uint32_t pcxo:16;
		uint32_t pcxs:4;
		uint32_t res:12;
	} bits;
	uint32_t reg;
} pcxi_t;


#endif // _CORE_H_
