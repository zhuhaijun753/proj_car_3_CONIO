SECTIONS
{

  /* 
   * Define the kernel data 
   * .pxros.system.stack  - section for the system stack used by the kernel, interrupts and traps
   *                      - The system stack can be increased for each core by a core specific
   *                      symbol CORE_SYM(USER_SYSTEM_STACK_SIZE)
   * .sbss4.pxros         - zero initialized kernel data
   * .sdata4.pxros        - initialized kernel data
   * .systemmemory        - memory area for the system memory class, used to allocated TCBs
   * .pxros_object_mem    - memory area for the PXROS objects
   *
   * Each of these section will be allocated for each core instance as .CPUX.<section name>
   * The symbols:
   *    CORE_SYM(PxTricSystemDataLowerBound), i.e PxTricSystemDataLowerBound_CPUX_
   *    CORE_SYM(PxTricSystemDataUpperBound)
   *    will define the lower and upper address of the system area
   */
  CORE_SYM(USER_SYSTEM_STACK_SIZE) = DEFINED(CORE_SYM(USER_SYSTEM_STACK_SIZE))? CORE_SYM(USER_SYSTEM_STACK_SIZE): 0 ; 
  CORE_SEC(.pxros.system.stack) (NOLOAD): FLAGS(aw)
  {
    CORE_SYM(PxTricSystemDataLowerBound) = .;
    CORE_SYM(PXROS_SYSTEM_STACK_BEGIN) = . ;
    CORE_SYM(PXROS_SYSTEM_STACK_SIZE) = PXROS_SYSTEM_STACK_SIZE + CORE_SYM(USER_SYSTEM_STACK_SIZE);
    . += CORE_SYM(PXROS_SYSTEM_STACK_SIZE) ;
    CORE_SYM(PXROS_SYSTEM_STACK) = . ;
  } > CORE_SYM(PXDATA)

  /* 
   * PXROS data will be addressed relative to A9
   * allocate space for the pxros data sections .sdata4.pxros and .sbss4.pxros in the
   * core specific memory area CORE_SYM(PXDATA). i.e PXDATA_CPU0_, PXDATA_CPU1_ etc.
   */
  CORE_SEC(.sbss4.pxros) : FLAGS(aws)
  {
    CORE_SYM(_SMALL_DATA4) = 0x8000;
    . += SIZEOF(.sbss4.pxros);
  } > CORE_SYM(PXDATA)

  CORE_SEC(.sdata4.pxros) : FLAGS(aws)
  {
    . += SIZEOF(.sdata4.pxros);
  } > CORE_SYM(PXDATA)

  CORE_SEC(.systemmemory) (NOLOAD) : FLAGS(aw)
  {
  	*(.systemmemory.*)
  	*(CORE_SEC(.systemmemory*))
  } > CORE_SYM(PXDATA)

  /*
   * allocate space for the core specific global data and the objectlists into the
   * core specific memory area CORE_SYM(PXDATA). i.e PXDATA_CPU0_, PXDATA_CPU1_ etc.
   * if __NUM_OF_PXOBJS__CPU[012]_ is not defined no space will be allocated
   * and the space for the object memory has to be defined inside a section .objectmemory.*
   * with the appropriate core specific flags  
   */
  CORE_SYM(__NUM_OF_PXOBJS_) = DEFINED(CORE_SYM(__NUM_OF_PXOBJS_))? CORE_SYM(__NUM_OF_PXOBJS_) : 0 ;
  __PXROS_NAMESIZE__ = DEFINED(__PXROS_NAMESIZE__)? __PXROS_NAMESIZE__ : 0 ;
  CORE_SYM(__PXROS_NAMESIZE__) = DEFINED(CORE_SYM(__PXROS_NAMESIZE__))? CORE_SYM(__PXROS_NAMESIZE__) : __PXROS_NAMESIZE__ ;
  CORE_SYM(PX_OBJMEMSIZE) = ALIGN ((__PXOBJ_SIZE__ + CORE_SYM(__PXROS_NAMESIZE__)) * CORE_SYM(__NUM_OF_PXOBJS_), 8);
  
  CORE_SEC(.pxros_object_mem) (NOLOAD) : ALIGN(8) FLAGS(aw)
  { 
    CORE_SYM(PxGlobalMem_begin) = .;
    CORE_SYM(PxObjmem) = .;
  	*(.objectmemory.*)
  	*(CORE_SEC(.objectmemory*))
    . += CORE_SYM(PX_OBJMEMSIZE);
    CORE_SYM(PxGlobalMem_end) = .;
    CORE_SYM(PxTricSystemDataUpperBound) = .;
  } > CORE_SYM(PXDATA)
  
  /* copy the image for .sdata4.pxros */
  CORE_SEC(.copy_sec) :
  {
    LONG(LOADADDR(.sdata4.pxros)); LONG(0 + ADDR(CORE_SEC(.sdata4.pxros))); LONG(SIZEOF(CORE_SEC(.sdata4.pxros)));
  } > RODATA_MEM
  
  /* clear the bss section .sbss4.pxros, .systemmemory and .pxros.system.stack */
  CORE_SEC(.clear_sec) :
  {
    LONG(0 + ADDR(CORE_SEC(.sbss4.pxros))); LONG(SIZEOF(CORE_SEC(.sbss4.pxros)));
    LONG(0 + ADDR(CORE_SEC(.systemmemory))); LONG(SIZEOF(CORE_SEC(.systemmemory)));
    LONG(0 + ADDR(CORE_SEC(.pxros.system.stack))); LONG(SIZEOF(CORE_SEC(.pxros.system.stack)));
  } > RODATA_MEM
}
