SECTIONS
{
  /* PXROS data will be addressed relative to A9 */

  /* allocate space for the pxros data sections .sdata4.pxros and .sbss4.pxros in the
   * core specific memory area CORE_SYM(PXDATA). i.e PXDATA_CPU0_, PXDATA_CPU1_ etc.
   */
  CORE_SEC(.sbss4.pxros) : FLAGS(aws)
  {
    CORE_SYM(_SMALL_DATA4) = 0x8000;
    CORE_SYM(PxTricSystemDataLowerBound) = .;
    CORE_SYM(PXROS_SYSTEM_STACK) = ADDR(CORE_SEC(.sdata4.pxros)) + PXROS_SYSTEM_STACK - ADDR(.sdata4.pxros);
    CORE_SYM(PXROS_SYSTEM_STACK_BEGIN) = ADDR(CORE_SEC(.sdata4.pxros)) + PXROS_SYSTEM_STACK_BEGIN - ADDR(.sdata4.pxros);
    CORE_SYM(PXROS_SYSTEM_STACK_SIZE) = PXROS_SYSTEM_STACK_SIZE;
    . += SIZEOF(.sbss4.pxros);
  } > CORE_SYM(PXDATA)

  CORE_SEC(.sdata4.pxros) : FLAGS(aws)
  {
    . += SIZEOF(.sdata4.pxros);
    CORE_SYM(PxTricSystemDataUpperBound) = .;
  } > CORE_SYM(PXDATA)
  

  /*
   * allocate space for the core specific global data and the objectlists into the
   * core specific memory area CORE_SYM(PXDATA). i.e PXDATA_CPU0_, PXDATA_CPU1_ etc.
   */
  CORE_SYM(PX_OBJMEMSIZE) = ALIGN ((__PXOBJ_SIZE__ + __PXROS_NAMESIZE__) * CORE_SYM(__NUM_OF_PXOBJS_), 8);
  
  CORE_SEC(.pxros_global_mem) (NOLOAD) : ALIGN(8) FLAGS(aw)
  { 
    CORE_SYM(PxGlobalMem_begin) = .;
    CORE_SYM(PxObjmem) = .;
    . += CORE_SYM(PX_OBJMEMSIZE);
    
    CORE_SYM(PxGlobalMem_end) = .;
  } > CORE_SYM(PXDATA)
  
  /* copy the image for .sdata4.pxros */
  CORE_SEC(.copy_sec) :
  {
    LONG(LOADADDR(.sdata4.pxros)); LONG(0 + ADDR(CORE_SEC(.sdata4.pxros))); LONG(SIZEOF(CORE_SEC(.sdata4.pxros)));
  } > RODATA_MEM
  
  /* clear the bss section .sbss4.pxros */
  CORE_SEC(.clear_sec) :
  {
    LONG(0 + ADDR(CORE_SEC(.sbss4.pxros))); LONG(SIZEOF(CORE_SEC(.sbss4.pxros)));
  } > RODATA_MEM
}
