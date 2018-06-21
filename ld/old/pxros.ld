_MASTER_CORE_ = DEFINED (_MASTER_CORE_) ? _MASTER_CORE_ : 0;

SECTIONS
{
  /*
   * the section .sbss4.pxros ans .sdata4.pxros are only used to create section
   * images. the real section allocation will be done in pxros-core.ld into the
   * core specific sections CORE_SEC(.sdata4.pxros) and CORE_SEC(sbss4.pxros)
   */
  PXROS_SYSTEM_STACK_SIZE = 0x100;
  SDA_SECTION ( .sbss4.pxros , _SMALL_DATA4_ , a9 )
  .sbss4.pxros  (NOLOAD) : FLAGS(aws)
  {
    _SMALL_DATA4_ = 0x8000;
    *(.pxros_system_begin*)
    PXROS_SYSTEM_STACK_BEGIN = .;
    . +=  PXROS_SYSTEM_STACK_SIZE;
    . = ALIGN(8);                               /* align the stack to 8 byte */
    PXROS_SYSTEM_STACK = .;                     /* Initial value for the system stack */
    *(.systemmemory.*)
    *(.sbss.system.*)
  } > CODE_MEM  

  SDA_SECTION ( .sdata4.pxros , _SMALL_DATA4_ , a9 )  
  .sdata4.pxros ADDR(.sbss4.pxros) + SIZEOF(.sbss4.pxros) : FLAGS(aws)
  {
    *(.sdata.pxros_system_stack*)
    *(.sdata.pxros_system)
    *(.sdata.pxros_system.*)
    *(.pxros_system_end*)
    . = ALIGN(8);
  } > CODE_MEM 

  .pxinit :
  {
    . = ALIGN(4);
    PROVIDE(__PxInitializeTable = .);
    KEEP (*(.pxinit*))
    LONG(0);
  } > RODATA_MEM  

  .pxros_global (NOLOAD) : ALIGN(8) FLAGS(aw)
  {
    GLOBAL_SYSTEM_RAM_BEGIN = .;
    *(.global_state*)
    *(.global*)
    *(.TRAP.data)	/* Added by PF */
    . = ALIGN(8);
    GLOBAL_SYSTEM_RAM_END = .;
  } > PXROS_GLOBAL_MEM
  
  	/* PxROS clear. Clear of pxros_global was previously done in main with symbol #GLOBAL_SYSTEM_RAM_BEGIN added by PF*/
	.CPU0.clear_sec :
	{
		LONG(0 + ADDR(.pxros_global));LONG(SIZEOF(.pxros_global));
	} > mem_flash0
  
}

CORE_ID = CPU0;
INCLUDE ..\ld\pxros-core.ld
CORE_ID = CPU1;
INCLUDE ..\ld\pxros-core.ld
CORE_ID = CPU2;
INCLUDE ..\ld\pxros-core.ld
CORE_ID = GLOBAL;
