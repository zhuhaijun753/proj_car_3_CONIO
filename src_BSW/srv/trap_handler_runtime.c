/**
 * Project:	Student Car v3
 *
 * \file	trap_handler_runtime.c
 * \author	Thomas Barth	thomas.barth@h-da.de
 * \version	0.4
 *
 *  @brief Trap-Handler for OS-runtime traps
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "trap_handler_runtime.h"

#include "pxdef.h"
#include "machine/intrinsics.h"
#include "core.h"

/*******************************************************************************
**                      Defines                                               **
*******************************************************************************/
#define TRAP_PxTRAPABORT_STOP	/**< @brief If defined, a trap handled by PxTrapAbort will execute a "debug" instruction */

#define TRAP_0_STOP				/**< @brief If defined, the trap-handler for class 0 traps will execute a "debug" instruction */
#define TRAP_1_STOP				/**< @brief If defined, the trap-handler for class 1 traps will execute a "debug" instruction */
#define TRAP_2_STOP				/**< @brief If defined, the trap-handler for class 2 traps will execute a "debug" instruction */
#define TRAP_3_STOP				/**< @brief If defined, the trap-handler for class 3 traps will execute a "debug" instruction */
#define TRAP_4_STOP				/**< @brief If defined, the trap-handler for class 4 traps will execute a "debug" instruction */
#define TRAP_5_STOP				/**< @brief If defined, the trap-handler for class 5 traps will execute a "debug" instruction */
#define TRAP_6_STOP				/**< @brief If defined, the trap-handler for class 6 traps will execute a "debug" instruction */
#define TRAP_7_STOP				/**< @brief If defined, the trap-handler for class 7 traps will execute a "debug" instruction */

/** @brief Stores the available Information about a trap in #TRAP_RUNTIME_DETAIL  */
#define TRAP_STORE_DETAILS																\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].trapclass=trapclass;						\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].tin=tin;									\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].task_id=task_id;							\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].arg=arg;									\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].p_CSA=p_CSA;								\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].DSTR.U=dstr /*__MFCR(CPU_DSTR)*/;						\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].DATR.U=__MFCR(CPU_DATR);						\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].DEADD.U=deadd /*__MFCR(CPU_DEADD)*/;					\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].DIEAR.U=__MFCR(CPU_DIEAR);					\
	TRAP_RUNTIME_DETAIL[__MFCR(CPU_CORE_ID)].DIETR.U=__MFCR(CPU_DIETR);					\

/*******************************************************************************
**                      Declarations                                          **
*******************************************************************************/
#pragma section ".TRAP.data"
/**@brief Trap details of the last trap in OS context */
static struct{
	PxUInt_t 	    trapclass;      		/**< @brief A Trap class is group of traps that have a similar trigger  */
	PxUInt_t        tin;    				/**< @brief The TIN is the "Trap Identification number" */
	PxUInt_t        task_id;        		/**< @brief Task ID in which the Trap was triggered (be carefull with asynchronous traps!) */
	PxUInt_t        arg;       				/**< @brief Arguments passed by the user in #PxTrapInstallHandler */
	TC_CSA_t*       p_CSA;       			/**< @brief Pointer to the CSA */

	/**@brief DMI Synchronous Trap Flag Register.
	 *
	 * The DSTR contains synchronous trap information for the data memory system. The register is updated with trap source information to aid the localisation of faults.
	 * The register is updated whenever a valid trap is detected and the register has no bits already set. It is cleared by a write (independent of data value).
	 *
	 * Bits:
	 * 	0	->	SRE 	(Scratch Range Error)		-> 	Data access to data scratch memory region outside of physically implemented memory
	 * 	1	->	GAE 	(Global Address Error)		->  Load or store to local code scratch address outside of the lower 1MByte
	 * 	2	->	LBE		(Load Bus Error)			->	Data load from bus causing error
	 * 	6	->	CRE		(Cache Refill Error)		->	Bus error during cache refill
	 * 	14	->	DTME	(DTAG MSIST Error)			->	Access to memory mapped DTAG range outside of physically implemented memory.
	 * 	15	->	LOE		(Load Overlay Error)		->	Load to invalid overlay address
	 * 	16	->	SDE		(Segment Difference Error)	->	Load or store access where base address is in different segment to access address.
	 * 	17	->	SCE		(Segment Crossing Error)	->	Load or store access across segment boundary
	 * 	18	->	CAC		(CSFR Access Error)			->	Load or store to local CSFR space
	 * 	19	->	MPE		(Memory Protection Error)	->	Data access violating memory protection.
	 * 	20	->	CLE		(Context Location Error)	->	Context operation to invalid location.
	 * 	24	->	ALN		(Alignment Error)			->	Data access causing alignment error.*/
	Ifx_CPU_DSTR 		DSTR;

	/**@brief DMI Asynchronous Trap Flag Register.
	 *
	 * The DATR contains asynchronous trap information for the data memory system. The register is updated with trap information for DAE traps to aid the localisation of faults.
	 * The register is updated whenever a valid trap is detected and the register has no bits already set. It is cleared by a write (independent of data value).
	 * DAE traps are inhibited if the DATR register is non-zero.
	 *
	 * Bits:
	 *	3	->	SBE		(Store Bus Error)			->	Data store to bus causing error
	 *	9	->	CWE		(Cache Writeback Error)		->	Bus error during cache writeback operation.
	 *	10	->	CFE		(Cache Flush Error)			->	Bus error during cache flush operation.
	 *	14	->	SOE		(Store Overlay Error)		->	Store to invalid overlay address.*/
	Ifx_CPU_DATR 		DATR;

	/**@brief DMI Data Error Address: RegisterAddress that caused the error (only class 1,4 Traps).
	 *
	 * DEADD contains trap address information for the Data memory system. The register is updated with trap information for MEM, ALN, DSE or DAE traps to aid the localisation of faults.
	 * The register is only set whenever a trap is detected and either the DATR or DSTR registers have no bits already set.
	 * The register contents are only valid when either the DATR or DSTR register is non-zero and hence should be read prior to clearing these registers.
	 *
	 * Bits:
	 * 	[31:0]	Error Address*/
	Ifx_CPU_DEADD 		DEADD;

	/**@brief Data Integrity Error Address Register.
	 *
	 * This register contains the physical address accessed by the operation that encountered a uncorrectable data memory integrity error.
	 * This register is only updated if DIETR.IED is zero.
	 *
	 * Bits:
	 * 	[31:0]	TA		(Transaction Address)		->	Physical address being accessed by operation that encountered data integrity error.*/
	Ifx_CPU_DIEAR 		DIEAR;

	/**@brief Data Integrity Error Trap Register.
	 *
	 * Bits:
	 *	0	->	IED		(Integrity Error Detected)	->	Read Operation:
	 *														0	-> 	No data integrity error condition occurred.
	 *														1	->	Data integrity error condition detected. DIETR and DIEAR contents valid, further DIETR and DIEAR updates disabled.
	 *													Write Operation:
	 *														0	->	Clear IED bit, re-enable DIETR and DIEAR update.
	 *														1	->	No effect.
	 *	1	->	IE_T	(Integrity Error - Tag Memory)
	 *	2	->	IE_C	(Integrity Error - Cache Memory)
	 *	3	->	IE_D	(Integrity Error - Scratchpad Memory)
	 *	4	->	IE_BI	(Integrity Error - Bus Interface)
	 *	[10:5]	E_INFO	(Error Information)			-> 	f IE_BS = 1: Bus Master Tag ID of requesting master If IE_C = 1: Cache way.
	 *	11	->	IE_UNC	(Dual Bit Error Detected)
	 *	12	->	IE_SP	(Safety Protection Error Detected)
	 *	13	->	IE_BS	(Bus Slave Access Indicator)*/
	Ifx_CPU_DIETR 		DIETR;
}TRAP_RUNTIME_DETAIL[DERIVATE_NUM_CORES];
#pragma section

/*******************************************************************************
**                      Private Function Definition                           **
*******************************************************************************/

static void trap_entry_hook(PxUInt_t trapclass, PxUInt_t tin , PxUInt_t task_id, PxUInt_t arg,	TC_CSA_t* p_CSA, PxUInt_t dstr, PxUInt_t* deadd){
	TRAP_STORE_DETAILS
}

/*******************************************************************************
**                      PxTrapAbort Overloading 	                          **
*******************************************************************************/
void PxTrapAbort(int trapclass, int tin, TC_CSA_t * p_CSA){

	//provide dummy data to support #TRAP_STORE_DETAILS
	PxUInt_t task_id =0 , arg=0;
	PxUInt_t dstr = 0;
	PxUInt_t* deadd = 0;

	TRAP_STORE_DETAILS

#ifdef TRAP_PxTRAPABORT_STOP
	asm("debug");
#endif

}

/*******************************************************************************
**                      Trap Handlers 				                          **
*******************************************************************************/

/* TRAP CLASS -	0	*/
PxBool_t Px_trap_hdl_0(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA){

	trap_entry_hook(trapTin.t.trapno,trapTin.t.tin, runtaskId, arg, p_CSA, dstr, deadd);

	switch (trapTin.t.tin) {
		case 0:
			/* Virtual Address Fill Trap (VAF)
			 * The VAF trap is generated when the MMU is enabled and the virtual address referenced by an instruction does
			 * not have a page entry in the MMU Translation Lookaside Buffer (TLB).   */
			break;

		case 1:
			/* Virtual Address Protection Trap (VAP)
			 * The VAP trap is generated (when the MMU is enabled) by a memory access undergoing PTE translation that is
			 * not permitted by the PTE protection settings, or by a User-0 mode access toan upper segment that does not have
			 * the privileged peripheral property. */
			break;

		default:
			/* Halt the execution if debug mode enabled.*/
			asm("debug");
			break;
		}

#ifdef TRAP_0_STOP
	asm("debug");
#endif

	return true;
}

/* TRAP CLASS -	1	*/
//PxBool_t Px_trap_hdl_1(PxUInt_t trapclass, PxUInt_t tin , PxUInt_t task_id, PxUInt_t arg,	TC_CSA_t* p_CSA){
PxBool_t Px_trap_hdl_1(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA){

	trap_entry_hook(trapTin.t.trapno,trapTin.t.tin, runtaskId, arg, p_CSA, dstr, deadd);

	switch (trapTin.t.tin) {
		case 1:
			/* Privilege Instruction Trap	(PRIV)
			 * A program executing in one of the User modes (User-0 orUser-1 mode) attempted to execute an instruction not allowed by that mode.
			 * A table of instructions which are restricted to Supervisor mode or User-1 mode, is supplied in the Instruction Set chapter of Volume 2
			 * of the TC16 Architecture Manual Vol1.*/
			break;

		case 2:
			/* Memory Protection Read Trap (MPR)
			 * The MPR trap is generated when the memory protection system is enabled and the effective address of a load, LDMST, SWAP or ST.T instruction does not lie
			 * within any range with read permissions enabled. This trap is not generated when an access violation occurs during a context save/restore operation.
			 *
			 * Whenever a data memory protection trap occurs the DSTR (Data synchronous trap register) and the DEADD (Data Error Address Register) are updated. */
			break;

		case 3:
			/* Memory Protection Write Trap (MPW)
			 * The MPW trap is generated when the memory protection system is enabled and the effective address of a store,LDMST, SWAP or ST.T instruction does not
			 * lie within any range with write permissions enabled. This trap is not generated when an access violation occurs during a context save/restore operation.
			 *
			 * Whenever a data memory protection trap occurs the DSTR (Data synchronous trap register) and the DEADD (Data Error Address Register) are updated.	 */
			break;

		case 4:
			/* Memory Protection Execution Trap (MPX)
			 * The MPX trap is generated when the memory protection system is enabled and the PC does not lie within any range with execute permissions enabled.
			 *
			 * Whenever a data memory protection trap occurs the DSTR (Data synchronous trap register) and the DEADD (Data Error Address Register) are updated. */
			break;

		case 5:
			/* Memory Protection Peripheral Access Trap (MPP)
			 * A program executing in User-0 mode attempted a load or store access to a segment is configured to be a peripheral segment.
			 *
			 * Whenever a data memory protection trap occurs the DSTR (Data synchronous trap register) and the DEADD (Data Error Address Register) are updated. */
			break;

		case 6:
			/* Memory Protection Null Address Trap (MPN)
			 * The MPN trap is generated whenever any program attempts a load / store operation to effective address zero.
			 *
			 * Whenever a data memory protection trap occurs the DSTR (Data synchronous trap register) and the DEADD (Data Error Address Register) are updated. */
			break;

		case 7:
			/* Global Register Write Protection Trap (GRWP)
			 * A program attempted to modify one of the global address registers (A[0], A[1], A[8] orA[9]) when it did not have permission to do so.*/
			break;

	default:
		/* Halt the execution if debug mode enabled.*/
		asm("debug");
		break;
	}


#ifdef TRAP_1_STOP
	asm("debug");
#endif

	return true;
}

/* TRAP CLASS -	2	*/
PxBool_t Px_trap_hdl_2(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA){

	trap_entry_hook(trapTin.t.trapno,trapTin.t.tin, runtaskId, arg, p_CSA, dstr, deadd);

	switch (trapTin.t.tin) {
		case 1:
			/* Illegal Opcode Trap (IOPC)
			 * An invalid instruction opcode was encountered. An invalid opcode is one that does not correspond to any instruction known to the implementation.*/
			break;

		case 2:
			/* Unimplemented Opcode Trap (UOPC)
			 * An unimplemented opcode was encountered. An unimplemented opcode corresponds to a known instruction that is not implemented in a given hardware implementation.
			 * The instruction may be implemented via software emulation in the trap handler.
			 * Example UOPC conditions are:
			 * 	• A MMU instruction if the MMU is not present.
			 * 	• A FPU instruction if the FPU is not present.
			 * 	• An external coprocessor instruction if the external coprocessor is not present.*/
			break;

		case 3:
			/* Invalid Operand Specification Trap (OPD)
			 * The OPD trap may be raised for instructions that take an even-odd register pair as an operand, if the operand specifier is odd.
			 * The OPD trap may also be raised for other cases where operands are invalid.
			 * Implementations are not architecturally required to raise this trap, and may treat invalid operands in an implementation defined manner.*/
			break;

		case 4:
			/* Data Address Alignment Trap (ALN)
			 * An ALN trap is raised when the address for a data memoryoperation does not conform to the required alignment rules. See “Alignment Requirements” on Page 2-4,
			 * for more information on these rules. An ALN trap is also raised when the size, length or index of a circular buffer is incorrect.
			 *
			 * Whenever an ALN trap occurs, the DSTR (Data Synchronous Trap Register) and the DEADD (Data Error Address Register) CSFRs are updated.*/
			break;

		case 5:
			/* Invalid Local Memory Address Trap (MEM)
			 * The MEM trap is raised when the address of an access can be determined to either violate an architectural constraint or an implementation constraint.
			 * Defined MEM trap subclasses are different segment, segment crossing, CSFR access, CSA restriction and scratch range.
			 * An implementation must define which implementation constraint MEM traps it will raise, or the alternative behavior if the MEM trap is not raised.
			 * It must also document any other implementation specific MEM traps it will raise.
			 * Architectural constraints which will raise the MEM trap are:
			 * 	• An addressing mode that adds an offset to a base address results in an effective address that is in a different segment to the base address (different segment).
			 * 	• A data element is accessed with an address, such that the data object spans the end of one segment and the beginning of another segment (segment crossing)
			 *
			 * Implementation constraints which can raise the MEM trap are
			 * 	• A memory address is used to access a Core SFR (CSFR) rather than using a MTCR/MFCR instruction (CSFR access)
			 * 	• A memory address is used for a CSA access and it is not valid for the implementation to place CSA there (CSA restriction)
			 * 	• An access to Scratch memory is attempted using a memory address which lies outside the implemented region of memory (scratch range error).
			 *
			 * Whenever a MEM trap occurs, the DSTR (Data Synchronous Trap Register) and the DEADD (Data Error Address Register) CSFRs are updated.			 */
			break;

		default:
			/* Halt the execution if debug mode enabled.*/
			asm("debug");
			break;
		}

#ifdef TRAP_2_STOP
	asm("debug");
#endif

	return true;
}

/* TRAP CLASS -	3	*/
PxBool_t Px_trap_hdl_3(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA){

	trap_entry_hook(trapTin.t.trapno,trapTin.t.tin, runtaskId, arg, p_CSA, dstr, deadd);

	switch (trapTin.t.tin) {
		case 1:
			/* Free Context list Depletion Trap (FCD)
			 * The FCD trap is generated after a context save operation, when the operation causes the free context list to become ‘almost empty’. The ‘almost empty’ condition
			 * is signaled when the CSA used for the save operation is the one pointed to by the context list limit register LCX.
			 * The operation responsible for the context save completes normally and then the FCD trap is taken.
			 * See the Architecture Manual vo1 for more information*/
			break;

		case 2:
			/* Call Depth Overflow Trap (CDO)
			 * A program attempted to execute a CALL instruction with the Call Depth counter enabled and the call depth count value (PSW.CDC.COUNT) at its maximum value.
			 * Call Depth Counting guards against context list depletion, by enabling the OS to detect ‘runawayrecursion’ in executing tasks.*/
			break;

		case 3:
			/* Call Depth Underflow Trap occurred (CDU)
			 * A program attempted to execute a RET (return) instruction with the Call Depth counter enabled and the call depth count value (PSW.CDC.COUNT) at zero.
			 * A call depth underflow does not necessarily reflect a software error in the currently executing task. An OS can achieve finer granularity in call depth counting
			 * by using a deliberately narrow Call Depth Counter, and incrementing or decrementing a separate software counter for the current task on* each call depth overflow
			 * or underflow trap. A program error would be indicated only if the software counter were already zero when the CDU trap occurred. */
			break;

		case 4:
			/* Free context list underflow Trap (FCU)
			 * The FCU trap is taken when a context save operation is attempted but the free context list is found to be empty (i.e. the FCX register contents are null).
			 * The FCU trap is also taken if any error is encountered during a context save or restore operation. The context operation cannot be completed. Instead a forced jump
			 * is made to the FCU trap handler and D15 updated with the FCU TIN value. In failing to complete the context save or restore, architectural state is lost,
			 * so the occurrence of an FCU trap is a non-recoverable system error. The FCU trap handler should ultimately initiate a system reset. */
			break;

		case 5:
			/* call stack underflow trap Trap (CSU)
			 * Raised when a context restore operation is attempted and when the contents of the PCX register were null.This trap indicates a system software error (kernel or OS)
			 * in task setup or context switching among software managed tasks (SMTs). No software error or combination of errors in a user task can generate this condition,
			 * unless the task has been allowed write permission to the context save areas which, in itself, can be regarded as a system software error. */
			break;

		case 6:
			/* context type Trap (CTYP)
			 * Raised when a context restore operation is attempted but the context type, as indicated by the PCXI.UL bit, is incorrect for the type of restore attempted;
			 * i.e. a restore lower context is attempted when PCXI.UL == 1, or a restore upper context is attempted when PCXI.UL == 0. As with the CSU trap,
			 * this indicates a system software error in context list management. */
			break;

		case 7:
			/* Nesting error Trap (NEST)
			 * A program attempted to execute an RFE (return from exception) instruction with the Call Depth counter enabled and the call depth count value (PSW.CDC.COUNT) non-zero.
			 * The return from an interrupt or trap handler should* normally occur within the body of the interrupt or trap handler itself, or in code to which the handler has branched,
			 * rather than code called from the handler. If this is not the case there will be one or more saved contexts on the residual call chain that must be popped and
			 * returned to the free list, before the RFE can be legitimately issued. */
			break;

		default:
			/* Halt the execution if debug mode enabled.*/
			asm("debug");
			break;
		}


#ifdef TRAP_3_STOP
	asm("debug");
#endif

	return true;
}

/* TRAP CLASS -	4	*/
PxBool_t Px_trap_hdl_4(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA){

	trap_entry_hook(trapTin.t.trapno,trapTin.t.tin, runtaskId, arg, p_CSA, dstr, deadd);

	switch (trapTin.t.tin) {
		case 1:
			/* Program Fetch Synchronous Error Trap (PSE)
			 * The PSE trap is raised when:
			 * 	• A bus error occurred because of an instruction fetch.
			 * 	• An instruction fetch targets a segment that does not have the code fetch property. */
			break;

		case 2:
			/* Data Access Synchronous Error Trap (DSE)
			 * The DSE trap is raised when:
			 * 	• Whenever a bus error occurs because of a data load operation.
			 * 	• In the case of a data load or store operation from Data scratchpad RAM (DSPR) where the access is beyond the end of the memory range.
			 * 	• In the case of an error during the data load phase of a data cache refill.
			 *
			 * Whenever a DSE trap occurs, the DSTR (Data Synchronous Trap Register) and the DEADD (Data Error Address Register) CSFRs are updated.*/
			break;

		case 3:
			/* Data Access Asynchronous Error Trap (DAE)
			 * The DAE trap is raised when the memory system reports back an error which cannot immediately be linked to a currently executing instruction. Generally this means
			 * an error returned on the system bus from a peripheral or external memory.
			 * This DAE trap is raised when:
			 * 	• A bus error occurred because of a data store operation.
			 * 	• There is an error caused by a cache management instruction.
			 * 	• There is an error caused by a cache line writeback.
			 *
			 * Whenever a non-inhibited DAE trap occurs, the DATR (Data Asynchronous Trap Register) and the DEADD (Data Error Address Register) CSFRs are updated.
			 * DAE traps are inhibited if the DATR register is non-zero.*/
			break;

		case 4:
			/* Coprocessor Asynchronous Error Trap (CAE)
			 * This CAE asynchronous trap is generated by a coprocessor to report an error. Examples of typical errors that can cause a CAE trap are unimplemented coprocessor
			 * instructions and arithmetic errors (as found in the Floating Point Unit for example). CAE is shared amongst all coprocessors in a given system.
			 * A trap handler must therefore inspect all coprocessors to determine the cause of a trap.*/
			break;

		case 5:
			/* Program Memory Integrity Error (PIE)
			 * The PIE trap is raised whenever an uncorrectable memory integrity error is detected in an instruction fetch. The trap is synchronous to the erroneous instruction.
			 * A PIE trap is raised if anyelement within the fetch group contains an unrecoverable error. Hardware is not required to localise the error to a particular instruction.
			 *
			 * The PIEAR and PIETR registers may be interrogated to determine the source of any error more precisely.*/
			break;

		case 6:
			/* Data Memory Integrity Error (DIE)
			 * The DIE trap is raised whenever an uncorrectable memory integrity error is detected in a data access. Implementations may choose to implement the DIE trap as either
			 * an asynchronous or synchronous trap. A DIE trap is raised if any element accessed by a load orstore contains an uncorrectable error.
			 * Hardware is not required to localise the error to the access width of the operation.
			 *
			 * A DIE trap is raised if any memory half word (local memory) or double word (SRI bus) accessed by a load/store operation contains an uncorrectable error.
			 * The DIEAR and DIETR registers may be interrogated to determine the source of any error more precisely.*/
			break;

		case 7:
			/* Temporal Asynchronous Error (TAE)
			 * The TAE asynchronous trap is raised by the temporal protection system whenever an active timer decrements to zero.
			 * This may be used to guard against task overrun in time critical applications.*/
			break;
		default:
			/* Halt the execution if debug mode enabled.*/
			asm("debug");
			break;
		}


#ifdef TRAP_4_STOP
	asm("debug");
#endif

	return true;
}

/* TRAP CLASS -	5	*/
PxBool_t Px_trap_hdl_5(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA){

	trap_entry_hook(trapTin.t.trapno,trapTin.t.tin, runtaskId, arg, p_CSA, dstr, deadd);

	switch (trapTin.t.tin) {
		case 1:
			/* Arithemetic Overflow Trap (OVF)
			 * Raised by the TRAPV instruction,if the overflow bit in the PSW is set (PSW.V == 1).*/
			break;

		case 2:
			/* Sticky Arithemetic Overflow Trap (SOVF)
			 * Raised by the TRAPSV instruction, if the sticky overflow bit in the PSW is set (PSW.SV == 1).*/
			break;

		default:
			/* Halt the execution if debug mode enabled.*/
			asm("debug");
			break;
		}

#ifdef TRAP_5_STOP
	asm("debug");
#endif

	return true;
}

/* TRAP CLASS -	6	*/
PxBool_t Px_trap_hdl_6(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA){

	trap_entry_hook(trapTin.t.trapno,trapTin.t.tin, runtaskId, arg, p_CSA, dstr, deadd);

	/* System Call Trap (SYS)
	 * The SYS trap is raised immediately after the execution of the SYSCALL instruction, to initiate a system call. The TIN that is loaded into D[15] when the trap is taken is not
	 * fixed, but is specified as an 8-bit unsigned immediate constant in the SYSCALL instruction. The return address points to the instruction immediately following the SYSCALL.*/

#ifdef TRAP_6_STOP
	asm("debug");
#endif

	return true;
}

/* TRAP CLASS -	7	*/
PxBool_t Px_trap_hdl_7(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA){

	trap_entry_hook(trapTin.t.trapno,trapTin.t.tin, runtaskId, arg, p_CSA, dstr, deadd);

	/* Non-Maskable Interrupt (NMI)
	 * The causes for raising a Non-Maskable Interrupt are implementation dependent. Typically there is an external pin that can be used to signal the NMI, but it may also be raised
	 * in response to such things as a watchdog timer interrupt, or an impending power failure. Refer to the User's Manual for a specific TriCore implementation for more details.*/

	/* SMU NMI/TRAP - Handling
	 *
	 *
	//Pointers to Alarm registers
	uint32_t* p_ald=(uint32_t*)&SMU_AD0;
	uint32_t* p_alg=(uint32_t*)&SMU_AG0;

	//Core 0 will print debug information
	if(coreID==0)
	{
		trap_entry_hook(trapclass,tin, task_id, arg, p_CSA);

		if(UART_is_init()==TRUE){

			//sprintf(_last_trap_details.message,"Dumping SMU Alarm Registers \n");
			//Dumping Alarm Status registers
			for(i=0;i<SMU_NO_OF_ALG;i++)
			{
				//sprintf(_last_trap_details.message,"Alarm Status Register for Alarm group %u: 0x%08x \n", i, *p_alg);
				p_alg++;
			}

			//sprintf(_last_trap_details.message,"\n");

			//Dumping Alarm Debug registers
			for(i=0;i<SMU_NO_OF_ALG;i++)
			{
				//sprintf(_last_trap_details.message,"Alarm Debug Register for Alarm group %u: 0x%08x \n", i, *p_ald);
				p_ald++;
			}
		}
	}*/


#ifdef TRAP_7_STOP
	asm("debug");
#endif

	//tell the OS that the Trap handling had been successfully
	return true;
}
