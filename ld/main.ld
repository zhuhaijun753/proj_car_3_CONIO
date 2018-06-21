/**
 * Project:	Student Car v3
 *
 * \file	main.ld
 *
 * \authors	HighTech EDV Systeme GmbH
 *			Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 *
 * \version	0.1 	03.05.2016
 *
 * @brief Main Linker-Description File
 *
 * Assigning data and code to specific memory locations is a very important aspect of multicore software development.
 * This file is a so called "linker-description"(ld)-file. This file instructs the linker where to put which piece of data/code.
 * In the code, the user will use "#pragma section" directives to create so called "Input-Sections".
 * These Input-Sections will be mapped to "Output-Sections" in the linker description files.
 * This file is the entry point, but other ld-files (and the x-file for the memory layout) can be included utilizing the INCLUDE-command.
 *
 * Output sections can contain multiple input sections and are mapped to a specific memory. The "." is a reference to the current location, 
 * it can be read and written.
 *
 * Example:
 *	.my_output_section (opt)(<location>) : (opt)FLAGS(<flags>)	<- 	Name of the output section. 
 *	{
 * 		__MY_SECTION_BEGIN = . ;		<-	Read the current address and write it into __MY_SECTION_BEGIN
 *   	*(.my_input_section)			<-	A input section that shall be linked to this output section
 *   	. = ALIGN(8) ;					<-	Set the current address to the next address divisible by 8 (used for alignment)
 *		(.my__other_input_section)		<-	A input section that shall be linked to this output section
 *   	__MY_SECTION_END = . ;			<-	Read the current address and write it into __MY_SECTION_END
 * } > (opt)(<dest>AT>) mem_flash		<-	This output section shall be linked to the memory "mem_flash".
 *  
 * <location> 	Optional: 	The exact address where this section should be linked to. 
 * <flags>		Optional: 	Flags like "readonly". The flags are not save! See the HighTech Manual for more information.
 * <dest>		Optional: 	Data/Code can be linked to 2 memories at once. For example initialized variables(stored in ROM) need to be transfered to RAM.
 *							In this case <dest> is the destination and the second memory is the source.
 *
 * The symbols from the example above (__MY_SECTION_BEGIN, __MY_SECTION_END) can be accessed from code.
 * This means that those symbols can be used to set up e.g. memory protection mechanisms or similar.
 *
 * Another usage of the linker-symbols are the Clear- and Copy-Tables.
 * These tables are accessed during startup of the Cores and perform the memory initialization.
 *
 * The Clear-Table is a array with 2 columns, the first column is the start-address and the second column is the length of the area to be cleared (bits set to 0).
 * e.g. LONG(0 + ADDR(.my_output_section));     LONG(SIZEOF(.my_output_section));  
 *
 * The Copy-Table is a array with 3 columns, the first column is the source-address and the second column is the destination-address and the third columns is the size
 * of the area to be copied. The output section needs to be defined with "AT>" and 2 memory locations, so that there is a source and destination addresss available.
 * e.g.  LONG(LOADADDR(.my_output_section));   LONG(0 + ADDR(.my_output_section)); 					LONG(SIZEOF(.my_output_section));
 *
 *	Changelog:
 *
 */

/* ----------------------[ Linker Configuration ]---------------------------------------*/
OUTPUT_FORMAT("elf32-tricore")			/* Target Format */
OUTPUT_ARCH(tricore)					/* Target Architecture */
ENTRY(_start)							/* entry point of the execution */

/* ----------------------[ Symbols needed on global level ]-----------------------------*/
PxTricSystemRamBegin	= 0x50000000;
PxTricSystemRamEnd   	= 0xe0000000;

PERIPHERAL_START 		= 0xf0000000;
PERIPHERAL_END   		= 0xffffffff;
NOT_USED         		= 0x00000000;

/* ----------------------[ Include the memory definition ]------------------------------*/
INCLUDE ../ld/memory-tc29xx.x

/* ----------------------[ Stack and Heap configuration ]-------------------------------*/
__ISTACK_SIZE = 256;
__USTACK_SIZE = 1K;
__HEAP_SIZE = 4096 ; /* In the last version 4K */
__CSA_SIZE = 16K;

/* #####################[ 1. Section accessible by all cores, shared data and code ]############################*/
SECTIONS
{
 /*
  * The startup code should be placed where the CPU expects it after a reset, so we try to locate it first.
  */
   
   	/*This section is always required as Boot mode header 0 address absolutely restricted at address 0x80000000*/
	.bmhd_0 (0x80000000) : FLAGS(arl)
	{
		KEEP (*(.startup.bmhd))
	} > mem_flash0
	
	/*This section is always required as user start address absolutely restricted at address 0x80000020*/
	.startup (0x80000020) : FLAGS(rxl)
	{
		. = ALIGN(4);
		KEEP (*(.startup.text));
		. = ALIGN(4);
	} > mem_flash0
	
	/* After application reset, the BTV will point to 0xA(8)0000100, therefore the initial trap-table shall be linked there to fetch early errors */
 	.traptable_init_out (0x80000100)  :
  	{
		__INIT_TRAPTAB_BEGIN = .;
		KEEP (*(.traptable_init_in));
		. = ALIGN(8) ;
		__INIT_TRAPTAB_END = . ;
  	} > mem_flash0
  	
  	.interrupttable_init_out (0x80003FE0)  : /*0x80003FE0 */ 
  	{
  		__INIT_INT_TAB_BEGIN = . ;
		KEEP (*(.interrupttable_init_in));
		. = ALIGN(8) ;
		__INIT_INT_TAB_END = . ;
  	} > mem_flash0
  
/*
 * Allocate space for absolute addressable sections; this requires that"mem_dram" starts at a TriCore segment (256M) and points to
 * some RAM area!  If these conditions are not met by your particular hardware setup, you should either not use absolute data, or you
 * must move .zdata*,.zbss*,.bdata*,.bbss* input sections to some appropriate memory area.
 */
   
	/*
	* Allocate trap and interrupt vector tables for PXROS.
	*/
	.px_traptable  :
	{
		__TRAP_TAB_BEGIN = . ;
		*(.traptab)
		. = ALIGN(8) ;
		__TRAP_TAB_END = . ;
	} > mem_flash1


	.px_inttab(0x80202000)  :
	{
		__INT_TAB_BEGIN = . ;
		*(.inttab)
		. = ALIGN(8) ;
		__INT_TAB_END = . ;
	} > mem_flash1
	
	/*
	 * Code executed before calling main
	 */
	.init  :
	{
		PROVIDE(__init_start = .);
		

		
		KEEP(*(.init)) /*Code executed before calling main*/ 
		KEEP(*(.init*))
	
		PROVIDE(__init_end = .);
		. = ALIGN(8);
		
	} > mem_flash0 /* PMU_PFLASH0: Program Flash Memory (PFLASH0) */ 

	/*
	 * Code executed before exiting program
	 */
	.fini  :
	{
		PROVIDE(__fini_start = .);
		
		
		KEEP(*(.fini)) /*Code executed before exiting program*/ 
		KEEP(*(.fini*))
		
		
		PROVIDE(__fini_end = .);
		. = ALIGN(8);
		
	} > mem_flash0 /* PMU_PFLASH0: Program Flash Memory (PFLASH0) */ 
  
/*
 * Allocate .text and other read-only sections.
 */
	.text  :
	{
		__TEXT_BEGIN = . ;
		*(.text)
		*(.text.*)
		*(.lwip.text*)
		*(.gnu.linkonce.t.*)
		/*
		 * .gnu.warning sections are handled specially by elf32.em.
		 */
		*(.gnu.warning)

		. = ALIGN(8);
		__TEXT_END = . ;
	} > mem_flash0 = 0

	.lwip.text :
	{
		*(.lwip.text)
	} > mem_flash0


/* extra section due to incompatible flags */
    .jcr :
    {
      *(.jcr)
      . = ALIGN(8);
    } > mem_flash0_nc


	.rodata   : FLAGS(al)
	{
		__RODATA_BEGIN = . ;
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r.*)
		*(.rodata1)
		*(.lwip.rodata*)
		*(.toc)
		/* *(.jcr) */
		. = ALIGN(8);
	} > mem_flash0_nc


	.sdata2  :
	{
		*(.sdata.rodata)
		*(.sdata.rodata.*)
		*(.srodata)
        *(.srodata.*)
        *(.srodata.pxros_system.*)
		. = ALIGN(8);
		__RODATA_END = . ;
	} > mem_flash0_nc /* war cached */
	
	/*
	* C++ exception handling tables.  NOTE: gcc emits .eh_frame
	* sections when compiling C sources with debugging enabled (-g).
	* If you can be sure that your final application consists
	* exclusively of C objects (i.e., no C++ objects), you may use
	* the -R option of the "strip" and "objcopy" utilities to remove
	* the .eh_frame section from the executable.
	*/
	.eh_frame  :
	{
		*(.gcc_except_table)
		__EH_FRAME_BEGIN__ = . ;
		KEEP (*(.eh_frame))
		__EH_FRAME_END__ = . ;
		. = ALIGN(8);
	} > mem_flash0
	
	/*
	 * Section for constructors
	 */
	.ctors          : FLAGS(al)
	{
		__CTOR_LIST__ = . ;


		LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2);
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o ) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
		LONG(0) ;
		__CTOR_END__ = . ;
		 . = ALIGN(8);
	}  > mem_flash0
	

	/*
	 * Section for destructors
	 */
	.dtors          : FLAGS(al)
	{
		__DTOR_LIST__ = . ;

		LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2);
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o ) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
		LONG(0) ;

		__DTOR_END__ = . ;
		. = ALIGN(8);
	} > mem_flash0
}

/* #####################[ 2. Sections available (cloned) on each core ]#########################################*/
SECTIONS
{   
	/* define the CSA Memory area as an own section
	* this section will be allocated into the internal RAM
	* after the absolute addressable sections .zdata/.zbss
	* and allocate all internal memory not occupied by .zdata/.zbss
	*/
	.csa (NOLOAD) :
	{
		. = ALIGN(64);
		__CSA_BEGIN = . ;
		. +=  __CSA_SIZE;
		. = ALIGN(64);
		__CSA_END = .;
	} > mem_dspr
	
	/* CSA Symbols */
	__CSA_SIZE = __CSA_END - __CSA_BEGIN;

	/* Create the clear tables that tell the startup code which memory areas to clear */
	.CPU0.clear_sec : FLAGS(ac0)
	{
		. = ALIGN(4) ;
		PROVIDE(__clear_table_CPU0_ = .) ;
	} > mem_flash0
	
	.CPU1.clear_sec :
	{
		PROVIDE(__clear_table_CPU1_ = .) ;
	} > mem_flash0
	
	.CPU2.clear_sec :
	{
		PROVIDE(__clear_table_CPU2_ = .) ;
	} > mem_flash0

	
	/* Create the copy tables that tell the startup code which memory areas to copy */
	.CPU0.copy_sec :
	{
		PROVIDE(__copy_table_CPU0_ = .) ;
	} > mem_flash0

	.CPU1.copy_sec :
	{
		PROVIDE(__copy_table_CPU1_ = .) ;
	} > mem_flash0	

	.CPU2.copy_sec :
	{
		PROVIDE(__copy_table_CPU2_ = .) ;
	} > mem_flash0

}

/* #####################[ 3. PXROS Memory ]####################################################################*/
/* define the pxros memory areas on each core */
INCLUDE ../ld/pxros.ld

/* Sections for the task stack on each core */
INCLUDE ../ld/userSections_iROM.ld

SECTIONS
{  

	/* memory area of the name server (runs on CPU0) */
	.nameSrv :
	{
		. = ALIGN(8);
		PxNameServerSpaceBase = .;
		. += 4096;
		PxNameServerSpaceEnd = .;
	} > glb_mem_dsprcpu0
	

	/* PXROS System Memory */
	.CPU0.system : FLAGS(awc0)
	{
		CPU0_SYSMEM_BEGIN = . ;
		*(.CPU0.pxros.sysmem)
		. = ALIGN(8) ;
		CPU0_SYSMEM_END = . ;    
	} > glb_mem_dsprcpu0

	.CPU1.system : FLAGS(awc1)
	{
		CPU1_SYSMEM_BEGIN = . ;
		*(.CPU1.pxros.sysmem)
		. = ALIGN(8) ;
		CPU1_SYSMEM_END = . ; 
	} > glb_mem_dsprcpu1

	.CPU2.system : FLAGS(awc2)
	{
		CPU2_SYSMEM_BEGIN = . ;
		*(.CPU2.pxros.sysmem)
		. = ALIGN(8) ;
		CPU2_SYSMEM_END = . ;
	} > glb_mem_dsprcpu2



/* TODO: check wahts left over */
/*
	.peripheral : FLAGS(aw)
	{
		PERIPHERAL_DATA_BEGIN = . ;
		*(.data.peripheral)
		PERIPHERAL_DATA_END = . ; 
	} > glb_mem_dsprcpu0

	.monitor : FLAGS(aw)
	{
		MONITOR_DATA_BEGIN = . ;
		*(.data.Monitor)
		MONITOR_DATA_END = . ; 
	} > glb_mem_dsprcpu0 AT>mem_flash0

	.CAN : FLAGS(aw)
	{
		CAN_DATA_BEGIN = . ;
		*(.data.CAN)
		*(.data.Car)
		CAN_DATA_END = . ; 
	} > glb_mem_dsprcpu0

	.UART : FLAGS(aw)
	{
		UART_DATA_BEGIN = . ;
		*(.data.Remote)
		UART_DATA_END = . ; 
	} > glb_mem_dsprcpu0
	
	.application : FLAGS(aw)
	{
		APPLICATION_DATA_BEGIN = . ;
		*(.data.Application)
		APPLICATION_DATA_END = . ; 
	} > glb_mem_dsprcpu1  AT>mem_flash0

*/

	/* areas have to be initialized to ZERO */
	.CPU0.clear_sec :
	{
		LONG(0 + ADDR(.nameSrv));LONG(SIZEOF(.nameSrv));
		LONG(0 + ADDR(.CPU0.system));LONG(SIZEOF(.CPU0.system));
	} > mem_flash0

	.CPU1.clear_sec :
	{
		LONG(0 + ADDR(.CPU1.system));LONG(SIZEOF(.CPU1.system));
	} > mem_flash0

	.CPU2.clear_sec :
	{
		LONG(0 + ADDR(.CPU2.system));LONG(SIZEOF(.CPU2.system));
	} > mem_flash0
}

/* #####################[ 4. Global data section accessed by all cores ]#######################################*/
SECTIONS
{
	.libdnk.data :
	{
		LIBDNK_DATA_BASE = . ;
		*libdnk_c.a: (*.data*)
	} > lmu_sram_nc AT>mem_flash0

	.libdnk.bss :
	{
		*libdnk_c.a: (*.bss*)
		LIBDNK_DATA_END = . ;
	} > lmu_sram_nc 

	.data :
	{
		. = ALIGN(8) ;
		DATA_BASE = . ;
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d.*)
		SORT(CONSTRUCTORS)
		. = ALIGN(8) ;
		DATA_END = . ;
	} > lmu_sram_nc AT> mem_flash0

	/* Allocate space for BSS sections.	*/
	.bss  (NOLOAD) :
	{
		BSS_BASE = . ;
		*(.bss)
		*(.bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(8) ;
		BSS_END = .;
		__HEAP = . ;
		. += __HEAP_SIZE ;
		__HEAP_END = . ;
	} > glb_mem_dsprcpu0

	/* global data areas will be initialized by CPU0 */
	.CPU0.clear_sec :
	{
		LONG(0 + ADDR(.bss));     LONG(SIZEOF(.bss));
		LONG(0 + ADDR(.libdnk.bss));    LONG(SIZEOF(.libdnk.bss));
	} > mem_flash0

	.CPU0.copy_sec :
	{
		LONG(LOADADDR(.data));    						LONG(0 + ADDR(.data)); 							LONG(SIZEOF(.data));
		LONG(LOADADDR(.libdnk.data));    				LONG(0 + ADDR(.libdnk.data));		 			LONG(SIZEOF(.libdnk.data));
		LONG(LOADADDR(.GLOBAL.RW_init_RO_afterwards));  LONG(0 + ADDR(.GLOBAL.RW_init_RO_afterwards)); 	LONG(SIZEOF(.GLOBAL.RW_init_RO_afterwards)); 
		LONG(LOADADDR(.GLOBAL.task));					LONG(0 + ADDR(.GLOBAL.task));			 		LONG(SIZEOF(.GLOBAL.task));
		LONG(LOADADDR(.CPU0.sl_safety));				LONG(0 + ADDR(.CPU0.sl_safety));		 		LONG(SIZEOF(.CPU0.sl_safety));
		LONG(LOADADDR(.CPU0.sl_safety_state));			LONG(0 + ADDR(.CPU0.sl_safety_state));	 		LONG(SIZEOF(.CPU0.sl_safety_state));
/*		LONG(LOADADDR(.monitor));    					LONG(0 + ADDR(.monitor));						LONG(SIZEOF(.monitor)); */

		LONG(LOADADDR(.GLOBAL.exmem));    				LONG(0 + ADDR(.GLOBAL.exmem)); 					LONG(SIZEOF(.GLOBAL.exmem));
	} > mem_flash0

	.CPU1.copy_sec :
	{
/*		LONG(LOADADDR(.application));    				LONG(0 + ADDR(.application));				LONG(SIZEOF(.application));*/
		LONG(LOADADDR(.CPU1.sl_control));				LONG(0 + ADDR(.CPU1.sl_control));		 		LONG(SIZEOF(.CPU1.sl_control));
	} > mem_flash0
	
	.CPU2.copy_sec :
	{
		LONG(LOADADDR(.CPU2.sl_misc));					LONG(0 + ADDR(.CPU2.sl_misc));		 		LONG(SIZEOF(.CPU2.sl_misc));
	} > mem_flash0
}

/* #####################[ 5. Cloase Clear/Copy Tables and provide symbols about memory utilization ]###########*/
SECTIONS
{
	/* Clear tables close */
	.CPU0.clear_sec :
	{
		LONG(-1);                 LONG(-1);
		. = ALIGN(8);
	} > mem_flash0
	
	.CPU1.clear_sec :
	{
		LONG(-1);                 LONG(-1);
		. = ALIGN(8);
	} > mem_flash0

	.CPU2.clear_sec :
	{
		LONG(-1);                 LONG(-1);
		. = ALIGN(8);
	} > mem_flash0
	
	/* Copy tables close */
	.CPU0.copy_sec :
	{
		LONG(-1);                 LONG(-1);                  LONG(-1);
		. = ALIGN(8);
	} > mem_flash0

	.CPU1.copy_sec :
	{
		LONG(-1);                 LONG(-1);                  LONG(-1);
		. = ALIGN(8);
	} > mem_flash0

	.CPU2.copy_sec :
	{
		LONG(-1);                 LONG(-1);                  LONG(-1);
		. = ALIGN(8);
	} > mem_flash0


	/* Symbols */
	_end = __HEAP_END ;

	DATA_0_START  = ORIGIN(glb_mem_dsprcpu0);
	DATA_0_END    = ORIGIN(glb_mem_dsprcpu0) + LENGTH(glb_mem_dsprcpu0);
	DATA_1_START  = ORIGIN(glb_mem_dsprcpu1);
	DATA_1_END    = ORIGIN(glb_mem_dsprcpu1) + LENGTH(glb_mem_dsprcpu1);
	DATA_2_START  = ORIGIN(glb_mem_dsprcpu2);
	DATA_2_END    = ORIGIN(glb_mem_dsprcpu2) + LENGTH(glb_mem_dsprcpu2);

	/* old naming conventions */
	INIT_DATA_BASE_0  = ORIGIN(glb_mem_dsprcpu0);
	INIT_DATA_END_0   = ORIGIN(glb_mem_dsprcpu0) + LENGTH(glb_mem_dsprcpu0);
	INIT_DATA_BASE_1  = ORIGIN(glb_mem_dsprcpu1);
	INIT_DATA_END_1   = ORIGIN(glb_mem_dsprcpu1) + LENGTH(glb_mem_dsprcpu1);
	INIT_DATA_BASE_2  = ORIGIN(glb_mem_dsprcpu2);
	INIT_DATA_END_2   = ORIGIN(glb_mem_dsprcpu2) + LENGTH(glb_mem_dsprcpu2);

	INIT_DATA_BASE_0_START = ORIGIN(glb_mem_dsprcpu0);
	INIT_DATA_BASE_0_END   = ORIGIN(glb_mem_dsprcpu0) + LENGTH(glb_mem_dsprcpu0);
	INIT_DATA_BASE_1_START = ORIGIN(glb_mem_dsprcpu1);
	INIT_DATA_BASE_1_END   = ORIGIN(glb_mem_dsprcpu1) + LENGTH(glb_mem_dsprcpu1);
	INIT_DATA_BASE_2_START = ORIGIN(glb_mem_dsprcpu2);
	INIT_DATA_BASE_2_END   = ORIGIN(glb_mem_dsprcpu2) + LENGTH(glb_mem_dsprcpu2);

	/* Die .rodata section reicht nicht aus */
	PxTricSystemRodataLowerBound = __RODATA_BEGIN; /* ADDR(.rodata); */
	PxTricSystemRodataUpperBound = __RODATA_END; /* ADDR(.rodata) + SIZEOF(.rodata); */
} 

SECTIONS
{ 
	/* initial areas for the inittask */
	__CPU0_LDRAM = ORIGIN (glb_mem_dsprcpu0);
	__CPU0_LDRAM_SIZE = LENGTH (glb_mem_dsprcpu0);
	__CPU1_LDRAM = ORIGIN (glb_mem_dsprcpu1);
	__CPU1_LDRAM_SIZE = LENGTH (glb_mem_dsprcpu1);
	__CPU2_LDRAM = ORIGIN (glb_mem_dsprcpu2);
	__CPU2_LDRAM_SIZE = LENGTH (glb_mem_dsprcpu2);

	__CODE_MEM_BEGIN = ORIGIN(mem_flash0);
	__CODE_MEM_END   = ORIGIN(mem_flash0) + LENGTH(mem_flash0);
	PROVIDE(end = _end) ;
	/* Make sure CSA, stack and heap addresses are properly aligned.  */
	_. = ASSERT ((__CSA_BEGIN & 0x3f) == 0 , "illegal CSA start address") ;
	_. = ASSERT ((__CSA_SIZE & 0x3f) == 0 , "illegal CSA size") ;
	_. = ASSERT ((__HEAP_END & 7) == 0 , "HEAP not doubleword aligned") ;

	/* Define a default symbol for address 0.  */
	NULL = DEFINED (NULL) ? NULL : 0 ;
	
/*
 * DWARF debug sections.
 * Symbols in the DWARF debugging sections are relative to the beginning of the section, so we begin them at 0.
 */	
	/* DWARF 1 */
	.comment         0 : { *(.comment) }
	.debug           0 : { *(.debug) }
	.line            0 : { *(.line) }
	
	/* GNU DWARF 1 extensions */
	.debug_srcinfo   0 : { *(.debug_srcinfo) }
	.debug_sfnames   0 : { *(.debug_sfnames) }
	
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges   0 : { *(.debug_aranges) }
	.debug_pubnames  0 : { *(.debug_pubnames) }
	
	/* DWARF 2 */
	.debug_info      0 : { *(.debug_info) }
	.debug_abbrev    0 : { *(.debug_abbrev) }
	.debug_line      0 : { *(.debug_line) }
	.debug_frame     0 : { *(.debug_frame) }
	.debug_str       0 : { *(.debug_str) }
	.debug_loc       0 : { *(.debug_loc) }
	.debug_macinfo   0 : { *(.debug_macinfo) }
	.debug_ranges    0 : { *(.debug_ranges) }
	
	/* SGI/MIPS DWARF 2 extensions	*/
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	
	/* Optional sections that may appear regardless of relocating.	*/
	.version_info    0 : { *(.version_info) }
	.boffs           0 : { KEEP (*(.boffs)) }
}
