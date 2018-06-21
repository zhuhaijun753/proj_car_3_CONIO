/**
 * Project:	Student Car v3
 *
 * \file	memory-tc29xx.x
 *
 * \authors	HighTech EDV Systeme GmbH
 *			Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 *
 * \version	0.1 	03.05.2016
 *
 * @brief Memory layout for the AURIX TC29x
 *
 * In this file, it is specified how hardware/memory modules are mapped the address space.
 * Each entry in #MEMORY is providing a start address and a lenght, along with flags.
 * The meaning of each flag can be read from the HighTech Manual, but it shall be noted that 
 * the flags easily can be overwritten and therefor can not be used to secure memory areas.
 *
 * Some of the address-ranges overlap for same phsyical devices. For example, the flash
 * can be accessed non cached (Page 0x8) or cached (Page 0xa). Obviously this is the same
 * phsyical memory, so the REGION_MIRROR command is used to make clear that both pages
 * address the same memory.
 *
 * The DSPR (Data RAM) and the PSPS (Code RAM) can be addressed with global and local addresses.
 * Global addresses specify which Core is addressed (Pages 0x7,0x6,0x5 for CPU 0,1,2), while
 * addresses on page 0xc and 0xd always point to the own RAM.
 * Local addressing therefore shall be used carefully as it easily can corrupt data or cause access-right violations.
 *  
 *	Changelog:
 *
 */
 
/* Derivate identifier 0x2700 -> TC27x, 0x2900 -> TC29x */
__TRICORE_DERIVATE_MEMORY_MAP__ = 0x2900;

/* minimal heap size (C library) */
__HEAP_SIZE = DEFINED (__HEAP_SIZE) ? __HEAP_SIZE : 16K;

/******************************************************************/
/*************   Memory definition  *******************************/
/******************************************************************/
MEMORY
{
/******************************************************/
/*************   FLASH  *******************************/
/******************************************************/
/* cached */
	mem_flash0 		(rx): 	org = 0x80000000, len = 2M
	mem_flash1 		(rx): 	org = 0x80200000, len = 2M
	mem_flash2 		(rx): 	org = 0x80400000, len = 2M
	mem_flash3 		(rx): 	org = 0x80600000, len = 2M
/* non cached */
	mem_flash0_nc	(rx): 	org = 0xA0000000, len = 2M
	mem_flash1_nc	(rx): 	org = 0xA0200000, len = 2M
	mem_flash2_nc	(rx): 	org = 0xA0400000, len = 2M
	mem_flash3_nc	(rx): 	org = 0xA0600000, len = 2M
	
/******************************************************/
/*************   Emulation Device Memory  *************/
/******************************************************/
	ed_mem 			(rx):  	org = 0x9f000000, len = 1M
 
/******************************************************/
/*************   Extended Memory **********************/
/*    WARNING: TC29x only and overlapps ed_mem!  	  */
/******************************************************/
	mem_EX 			(rx):  	org = 0x9F000000, len = 2M
	mem_EX_nc 		(rx):  	org = 0xBF000000, len = 2M
 
/******************************************************/
/*************   CPU RAM  *****************************/
/******************************************************/
/* PSPR GLOBAL */
	glb_mem_psprcpu0(rx): 	org = 0x70100000, len = 32K
	glb_mem_psprcpu1(rx): 	org = 0x60100000, len = 32K
	glb_mem_psprcpu2(rx): 	org = 0x50100000, len = 32K

/* PSPR LOCAL */
	mem_psprcpu0 	(rxc0):	org = 0xc0000000, len = 32K
	mem_psprcpu1 	(rxc1): org = 0xc0000000, len = 32K
	mem_psprcpu2 	(rxc2): org = 0xc0000000, len = 32K
	mem_pspr       	(rx): 	org = 0xc0000000, len = 32K

/* DSPR GLOBAL */
	glb_mem_dsprcpu0(w!x): 	org = 0x70000000, len = 120K
	glb_mem_dsprcpu1(w!x): 	org = 0x60000000, len = 240K
	glb_mem_dsprcpu2(w!x): 	org = 0x50000000, len = 240K
	
/* DSPR LOCAL */
	mem_dsprcpu0 (wc0!x):	org = 0xd0000000, len = 120K
	mem_dsprcpu1 (wc1!x):	org = 0xd0000000, len = 240K
	mem_dsprcpu2 (wc2!x):	org = 0xd0000000, len = 240K
	mem_dspr     (w!x):		org = 0xd0000000, len = 120K 
	
/* TODO: required? */
  /*csa_mem (w!x): org = 0xd0000000, len = 16K
  pxros_mem (w!x): org = 0xd0004000, len = 20K*/
  
/******************************************************/
/*************   LMU RAM  *****************************/
/******************************************************/
	lmu_sram		(wx):	org = 0x90000000, len = 32K
	lmu_sram_nc 	(wx):	org = 0xB0000000, len = 32K
		
/******************************************************/
/*************   LGTM/MCS  ****************************/
/******************************************************/
/* local view on GTM/MCS data and code memory */
	mem_mcs0_code (rx): org = 0x0000, len = 4K
	mem_mcs0_data (w):  org = 0x1000, len = 2K

/* TriCore view on GTM/MCS data and code memory */
	glb_mcs0_code (rx): org = 0xf0138000, len = 4K
	glb_mcs0_data (w):  org = 0xf0139000, len = 2K
}


/******************************************************************/
/*************   Mirroring  ***************************************/
/* Mirror the addresses as they describe the same physical memory */
/******************************************************************/
/* Flash */
	REGION_MIRROR 	(mem_flash0 	mem_flash0_nc)
	REGION_MIRROR 	(mem_flash1 	mem_flash1_nc)
	REGION_MIRROR 	(mem_flash2 	mem_flash2_nc)
	REGION_MIRROR 	(mem_flash3 	mem_flash3_nc)
	
/* PSPR */
	REGION_MIRROR	(mem_psprcpu0 	glb_mem_psprcpu0 	mem_pspr)
	REGION_MIRROR 	(mem_psprcpu1 	glb_mem_psprcpu1 	mem_pspr)
	REGION_MIRROR 	(mem_psprcpu2 	glb_mem_psprcpu2 	mem_pspr)
	
/* DSPR */
	REGION_MIRROR 	(mem_dsprcpu0 	glb_mem_dsprcpu0 	mem_dspr)
	REGION_MIRROR 	(mem_dsprcpu1 	glb_mem_dsprcpu1 	mem_dspr)
	REGION_MIRROR 	(mem_dsprcpu2 	glb_mem_dsprcpu2 	mem_dspr)
	
/* LMU */
	REGION_MIRROR 	(lmu_sram 		lmu_sram_nc)
	
/* EMEM */
	REGION_MIRROR 	(mem_EX 		mem_EX_nc)

/******************************************************************/
/*************   REGION_MAP? not in manual.. TODO: Needed? ********/
/******************************************************************/

/* map local memory address to the corresponding global address */
	REGION_MAP("CPU0", ORIGIN(mem_dsprcpu0),  LENGTH(mem_dsprcpu0),  ORIGIN(glb_mem_dsprcpu0))
	REGION_MAP("CPU1", ORIGIN(mem_dsprcpu1),  LENGTH(mem_dsprcpu1),  ORIGIN(glb_mem_dsprcpu1))
	REGION_MAP("CPU2", ORIGIN(mem_dsprcpu2),  LENGTH(mem_dsprcpu2),  ORIGIN(glb_mem_dsprcpu2))
	REGION_MAP("CPU0", ORIGIN(mem_psprcpu0),  LENGTH(mem_psprcpu0),  ORIGIN(glb_mem_psprcpu0))
	REGION_MAP("CPU1", ORIGIN(mem_psprcpu1),  LENGTH(mem_psprcpu1),  ORIGIN(glb_mem_psprcpu1))
	REGION_MAP("CPU2", ORIGIN(mem_psprcpu2),  LENGTH(mem_psprcpu2),  ORIGIN(glb_mem_psprcpu2))
	
/******************************************************************/
/*************   REGION_ALIAS for compatibility PXROS      ********/
/******************************************************************/
	
	
	REGION_ALIAS("CODE_MEM",				mem_flash0)
	REGION_ALIAS("RODATA_MEM",				mem_flash0)
	REGION_ALIAS("PXROS_GLOBAL_MEM",		lmu_sram_nc)
	REGION_ALIAS("PXDATA_CPU0_",			glb_mem_dsprcpu0)
	REGION_ALIAS("PXDATA_CPU1_",			glb_mem_dsprcpu1)
	REGION_ALIAS("PXDATA_CPU2_",			glb_mem_dsprcpu2)
	
