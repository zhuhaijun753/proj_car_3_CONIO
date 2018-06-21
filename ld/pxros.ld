_MASTER_CORE_ = DEFINED (_MASTER_CORE_) ? _MASTER_CORE_ : 0;

SECTIONS
{
  /*
   * the section .sbss4.pxros and .sdata4.pxros are only used to create section
   * images. the real section allocation will be done in pxros-core.ld into the
   * core specific sections CORE_SEC(.sdata4.pxros) and CORE_SEC(sbss4.pxros)
   */
  PXROS_SYSTEM_STACK_SIZE = DEFINED (PXROS_SYSTEM_STACK_SIZE) ? PXROS_SYSTEM_STACK_SIZE : 0x100;
  SDA_SECTION ( .sbss4.pxros , _SMALL_DATA4_ , a9 )
  .sbss4.pxros 0 (NOLOAD) : FLAGS(aws)
  {
    _SMALL_DATA4_ = 0x8000;
    *(.pxros_system_begin*)
    . = ALIGN(8);                               /* align the stack to 8 byte */
    *(.sbss.pxros_system_stack*)
    *(.sbss.system.*)
  } 

  SDA_SECTION ( .sdata4.pxros , _SMALL_DATA4_ , a9 )  
  .sdata4.pxros ADDR(.sbss4.pxros) + SIZEOF(.sbss4.pxros) : FLAGS(awls)
  {
    *(.sdata.pxros_system)
    *(.sdata.pxros_system.*)
    *(.pxros_system_end*)
    . = ALIGN(8);
  }  AT>CODE_MEM

  .pxinit :
  {
    . = ALIGN(4);
    PROVIDE(__PxInitializeTable = .);
    KEEP (*(.pxinit*))
    LONG(0);
  } > RODATA_MEM  

}

CORE_ID = CPU0;
INCLUDE ..\ld\pxros-core.ld
CORE_ID = CPU1;
INCLUDE ..\ld\pxros-core.ld
CORE_ID = CPU2;
INCLUDE ..\ld\pxros-core.ld
CORE_ID = GLOBAL;
