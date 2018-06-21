/*
 * Task_Init.c
 *
 *  Created on: Dec 18, 2014
 *      Author: fromm
 */

#include "core.h"
#include "pxdef.h"

#include "symbols.h"

#include "Task_Init.h"




/*******************************************************************************************************
 * Magic defines for creation of C-names for symbols provided by the Linker
 *******************************************************************************************************/

#define _DEF_SYM(n,v) \
   __asm__ (".global       " #n );     \
   __asm__ (".set  " #n "," #v);   \
   __asm__ (".type " #n ",STT_OBJECT")

#define DEF_SYM(name,val) _DEF_SYM(name,val)




/*******************************************************************************************************
 * Kernel memory sections
 *******************************************************************************************************/


//============================== Kernel Memory =========================================================
// memory area for the system memory class, used to allocated TCBs
// Size Calculation:
//	- TCB (400byte per Task)


//#pragma section ".CPU0.pxros.sysmem" awB
#pragma section ".systemmemory.0" 8 awBc0
PxAligned_t Sysmem_CORE0[(SYSMEMSIZE_CORE0 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section

//#pragma section ".CPU1.pxros.sysmem" awB
#pragma section ".systemmemory.1" 8 awBc1
PxAligned_t Sysmem_CORE1[(SYSMEMSIZE_CORE1 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section

//#pragma section ".CPU2.pxros.sysmem" awB
#pragma section ".systemmemory.2" 8 awBc2
PxAligned_t Sysmem_CORE2[(SYSMEMSIZE_CORE2 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section

//============================= Kernel Object Memory ====================================================
// memory area for the PXROS objects (messages, tasks, periodic events etc...)
// This memory section will be allocated by the Linker using the symbols below
// Size Calculation:
//   - Number of objects per core as defined in the corresponding Task_CN_Init.h file

#define NUM_OF_PXOBJS	(NUM_OF_PXOBJS_CORE0 + NUM_OF_PXOBJS_CORE1 + NUM_OF_PXOBJS_CORE2)
DEF_SYM(__NUM_OF_PXOBJS__, NUM_OF_PXOBJS);

//Mapping the name used in the task to the current OS symbolic name
#define CORE0_NUM_OF_PXOBJS (NUM_OF_PXOBJS_CORE0)
#define CORE1_NUM_OF_PXOBJS (NUM_OF_PXOBJS_CORE1)
#define CORE2_NUM_OF_PXOBJS (NUM_OF_PXOBJS_CORE2)

DEF_SYM(__NUM_OF_PXOBJS__CPU0_, CORE0_NUM_OF_PXOBJS);
DEF_SYM(__NUM_OF_PXOBJS__CPU1_, CORE1_NUM_OF_PXOBJS);
DEF_SYM(__NUM_OF_PXOBJS__CPU2_, CORE2_NUM_OF_PXOBJS);

#define PXROS_NAMESIZE      0   /* we don't need additional object name storage */
DEF_SYM(__PXROS_NAMESIZE__, PXROS_NAMESIZE);

//============================= Task Memory ===========================================================
// memory area for the content of the message objects (sender objects)
// in order to avoid timing impacts on the cores, it is recommended to allocate this memory on the LMU
//   - Depending on number and size of message objects

#pragma section ".GLOBAL.pxros.taskmem" awB
PxAligned_t Taskmem_Core0[(TASKMEMSIZE_CORE0 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
PxAligned_t Taskmem_Core1[(TASKMEMSIZE_CORE1 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
PxAligned_t Taskmem_Core2[(TASKMEMSIZE_CORE2 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section


/*******************************************************************************************************
 * System (Kernel) Protection Areas
 *******************************************************************************************************/


//========================================== CPU0 Protection sets  ==============================

//Kernel code access
const PxCodeProtectSet_T _cpu0_sys_code_protection = {

		{
				//Range 0 - The complete flash area
				{(PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END },

				//Range 1 - The trap table
				{(PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END },

				//Range 2 - the interrupt table
				{(PxUInt_t)__INT_TAB_BEGIN, (PxUInt_t)__INT_TAB_END },

				//Range 4 - not used
				{(PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED},
		},


		.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 1,.dp2 = 1,.dp3 = 0}

};


//Task Code Access
const PxCodeProtectSet_T _cpu0_task_code_protection = {

		{
				//Range 0 - The complete flash area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END } ,

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

		},


	.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 0,.dp2 = 0,.dp3 = 0 },
};


//Kernel data access
const PxDataProtectSet_T _cpu0_sys_data_protection = {
		{
		 /* Range 0 read only data */
		 { (PxUInt_t)&PxTricSystemRodataLowerBound, (PxUInt_t)&PxTricSystemRodataUpperBound },


		 /* Range 1 kernel private data (read/write) */
		 { (PxUInt_t)PxTricSystemDataLowerBound_CPU0_, (PxUInt_t)PxTricSystemDataUpperBound_CPU0_ },

		 /* Range 2 the CSA area of CPU0 (read/write) */
		 { (PxUInt_t)__CSA_BEGIN, (PxUInt_t)__CSA_END },

		 /* Range 3 Objectlist of CPU1 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU1_, (PxUInt_t)PxGlobalMem_end_CPU1_ },

		 /* Range 4 Objectlist of CPU2 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU2_, (PxUInt_t)PxGlobalMem_end_CPU2_ },

		 /* Range 5 the SFR area */
		 { (PxUInt_t)PERIPHERAL_START, (PxUInt_t)PERIPHERAL_END },

		 /* Range 6 not used */
		 { (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,

		 /* Range 7 initialize to 0, used by Kernel */
		 { 0,0 }
		},
		/* the DPRE 0..7 readable */
		.dpmr.dpre.bits = {
				.dp0 = 1, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 0, .dp7 = 1
		},
		/* the DPWE 1..7 writable */
		.dpmr.dpwe.bits = {
				.dp0 = 0, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 0, .dp7 = 1
		}
};


//========================================== CPU1 Protection sets  ==============================

//Kernel code access
const PxCodeProtectSet_T _cpu1_sys_code_protection = {

		{
				//Range 0 - The complete flash area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END },

				//Range 1 - The trap table
				{ (PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END },

				//Range 2 - the interrupt table
				{ (PxUInt_t)__INT_TAB_BEGIN, (PxUInt_t)__INT_TAB_END },

				//Range 4 - not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,
		},


	.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 1,.dp2 = 1,.dp3 = 0,
	},
};

//Task code access
const PxCodeProtectSet_T _cpu1_task_code_protection = {


		{
				//The Code area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

		},


		.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 0,.dp2 = 0,.dp3 = 0 }
};


//Kernel data access
const PxDataProtectSet_T _cpu1_sys_data_protection = {
		{
		 /* Range 0 read only data */
		 { (PxUInt_t)PxTricSystemRodataLowerBound, (PxUInt_t)PxTricSystemRodataUpperBound },

		 /* Range 1 kernel private data (read/write) */
		 { (PxUInt_t)PxTricSystemDataLowerBound_CPU1_, (PxUInt_t)PxTricSystemDataUpperBound_CPU1_ },


		 /* Range 2 the CSA area of CPU0 (read/write) */
		 { (PxUInt_t)__CSA_BEGIN, (PxUInt_t)__CSA_END },

		 /* Range 3 Objectlist of CPU0 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU0_, (PxUInt_t)PxGlobalMem_end_CPU0_ },

		 /* Range 4 Objectlist of CPU2 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU2_, (PxUInt_t)PxGlobalMem_end_CPU2_ },

		 /* Range 5 the SFR area */
		 { (PxUInt_t)PERIPHERAL_START, (PxUInt_t)PERIPHERAL_END },

		 /* Range 6 not used */
		 { (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,


		 /* 7 initialize to 0, used by Kernel */
		 { 0,0 }
		},


		/* the DPRE 0..7 readable */
		.dpmr.dpre.bits = {
				.dp0 = 1, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 0, .dp7 = 1
		},
		/* the DPWE 1..5,7 writable */
		.dpmr.dpwe.bits = {
				.dp0 = 0, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 0, .dp7 = 1
		}
};


//========================================== CPU2 Protection sets  ==============================

//Kernel code access
const PxCodeProtectSet_T _cpu2_sys_code_protection = {

		{
				//Range 0 - The complete flash area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END },

				//Range 1 - The trap table
				{ (PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END} ,

				//Range 2 - the interrupt table
				{ (PxUInt_t)__INT_TAB_BEGIN, (PxUInt_t)__INT_TAB_END} ,

				//Range 4 - not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,

		},


	.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 1,.dp2 = 1,.dp3 = 0,
	},
};

//Task code access
const PxCodeProtectSet_T _cpu2_task_code_protection = {


		{
				//Core area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END} ,

				//Not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED },

				//Not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED },

				//Not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,

		},


		.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 0,.dp2 = 0,.dp3 = 0,
		},
};


//Kernel data access
const PxDataProtectSet_T _cpu2_sys_data_protection = {


		{
		 /* Range 0 read only data */
		 { (PxUInt_t)PxTricSystemRodataLowerBound, (PxUInt_t)PxTricSystemRodataUpperBound },


		 /* Range 1 kernel private data (read/write) */
		 { (PxUInt_t)PxTricSystemDataLowerBound_CPU2_, (PxUInt_t)PxTricSystemDataUpperBound_CPU2_ },


		 /* Range 2 the CSA area of CPU2 (read/write)  */
		 { (PxUInt_t)__CSA_BEGIN, (PxUInt_t)__CSA_END },

		 /* Range 3 Objectlist of CPU0 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU0_, (PxUInt_t)PxGlobalMem_end_CPU0_ },

		 /* Range 4 Objectlist of CPU2 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU1_, (PxUInt_t)PxGlobalMem_end_CPU1_ },

		 /* Range 5 the SFR area */
		 { (PxUInt_t)PERIPHERAL_START, (PxUInt_t)PERIPHERAL_END },

		 /* Range 6 not used */
		 { (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,

		 /* 7 initialize to 0, used by Kernel */
		 { 0,0 }
		},


		/* the DPRE 0..7 readable */
		.dpmr.dpre.bits = {
				.dp0 = 1, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 0, .dp7 = 1
		},
		/* the DPWE 1..5,7 writable */
		.dpmr.dpwe.bits = {
				.dp0 = 0, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 0, .dp7 = 1
		}
};



/*******************************************************************************************************
 * Core spec created as array out of the three individual specs
 *******************************************************************************************************/

//We are on a multicore system....
#define MULTI_CORE

//Required for starting core 1 and 2
extern void _start(void);

/* the PXROS System specification */
const PxInitSpec_T InitSpec[PX_NO_OF_CORES] =
{
    [0] =
    {
        .is_sysmc_type      = PXMcVarsizedAligned,
        .is_sysmc_size      = 8,
        .is_sysmc_blk       = Sysmem_CORE0,
        .is_sysmc_blksize   = SYSMEMSIZE_CORE0,

        .is_obj_number      = CORE0_NUM_OF_PXOBJS,
        .is_obj_namelength  = PXROS_NAMESIZE,
        .is_inittask        = &InitTaskSpec_CORE0,
        .is_monitoring      = PXMonitorMemory,
        .is_schedext        = 0,

        .is_objmc_type      = PXMcVarsizedAligned,
        .is_objlmc_size     = 8,
        .is_objmc_blk       = PxObjmem_CPU0_,
        .is_objmc_blksize   = (PxSize_t)PX_OBJMEMSIZE_CPU0_,

        .is_taskmc_type     = PXMcVarsizedAdjusted,
        .is_taskmc_size     = 8,
        .is_taskmc_blk      = Taskmem_Core0,
        .is_taskmc_blksize  = TASKMEMSIZE_CORE0,

        /* Core0 is the initial core and has not to be started */
        .is_core_start      = 0,

        /* the system stack */
        .is_system_stack = PXROS_SYSTEM_STACK_BEGIN_CPU0_,
        .is_system_stack_size = (PxUInt_t)PXROS_SYSTEM_STACK_SIZE_CPU0_,

        /* the protection definition */
        .is_sys_code = &_cpu0_sys_code_protection,
        .is_sys_data = &_cpu0_sys_data_protection,
        .is_task_code = &_cpu0_task_code_protection,
    },
#if PX_NO_OF_CORES > 1
    [1] =
    {
        .is_sysmc_type      = PXMcVarsizedAligned,
        .is_sysmc_size      = 8,
        .is_sysmc_blk       = Sysmem_CORE1,
        .is_sysmc_blksize   = SYSMEMSIZE_CORE1,

        .is_objmc_type      = PXMcVarsizedAligned,
        .is_objlmc_size     = 8,
        .is_objmc_blk       = PxObjmem_CPU1_,
        .is_objmc_blksize   = (PxSize_t)PX_OBJMEMSIZE_CPU1_,

        .is_obj_number      = CORE1_NUM_OF_PXOBJS,
        .is_obj_namelength  = PXROS_NAMESIZE,
        .is_inittask        = &InitTaskSpec_CORE1,
        .is_monitoring      = PXMonitorMemory,
        .is_schedext        = 0,

        .is_taskmc_type     = PXMcVarsizedAdjusted,
        .is_taskmc_size     = 8,
        .is_taskmc_blk      = Taskmem_Core1,
        .is_taskmc_blksize  = TASKMEMSIZE_CORE1,

        /* Core1 has to be started */
#ifdef MULTI_CORE
        .is_core_start      = (unsigned int)_start,
#else
        .is_core_start      = 0,
#endif
        /* the system stack */
        .is_system_stack = PXROS_SYSTEM_STACK_BEGIN_CPU1_,
        .is_system_stack_size = (PxUInt_t)PXROS_SYSTEM_STACK_SIZE_CPU1_,

        /* the protection definition */
        .is_sys_code = &_cpu1_sys_code_protection,
        .is_sys_data = &_cpu1_sys_data_protection,
        .is_task_code = &_cpu1_task_code_protection,
    },
    [2] =
    {
        .is_sysmc_type      = PXMcVarsizedAligned,
        .is_sysmc_size      = 8,
        .is_sysmc_blk       = Sysmem_CORE2,
        .is_sysmc_blksize   = SYSMEMSIZE_CORE2,

        .is_obj_number      = CORE2_NUM_OF_PXOBJS,
        .is_obj_namelength  = PXROS_NAMESIZE,
        .is_inittask        = &InitTaskSpec_CORE2,
        .is_monitoring      = PXMonitorMemory,
        .is_schedext        = 0,

        .is_objmc_type      = PXMcVarsizedAligned,
        .is_objlmc_size     = 8,
        .is_objmc_blk       = PxObjmem_CPU2_,
        .is_objmc_blksize   = (PxSize_t)PX_OBJMEMSIZE_CPU2_,

        .is_taskmc_type     = PXMcVarsizedAdjusted,
        .is_taskmc_size     = 8,
        .is_taskmc_blk      = Taskmem_Core2,
        .is_taskmc_blksize  = TASKMEMSIZE_CORE2,

        /* Core2 has to be started */
#ifdef MULTI_CORE
        .is_core_start      = (unsigned int)_start,
#else
        .is_core_start      = 0,
#endif
        /* the system stack */
        .is_system_stack = PXROS_SYSTEM_STACK_BEGIN_CPU2_,
        .is_system_stack_size = (PxUInt_t)PXROS_SYSTEM_STACK_SIZE_CPU2_,


        /* the protection definition */
        .is_sys_code = &_cpu2_sys_code_protection,
        .is_sys_data = &_cpu2_sys_data_protection,
        .is_task_code = &_cpu2_task_code_protection,
    }
#endif /* CORE_COUNT */
};

const PxInitSpecsArray_t InitSpecsArray =
{
    &InitSpec[0],
    &InitSpec[1],
    &InitSpec[2]
};


