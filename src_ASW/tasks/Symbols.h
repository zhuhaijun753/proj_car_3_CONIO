/*************************************
 * System symbol definitions		 *
 *************************************/

#ifndef SYMBOLS_H_
#define SYMBOLS_H_


//Constant data PXROS
extern PxUInt_t PxTricSystemRodataLowerBound[];
extern PxUInt_t PxTricSystemRodataUpperBound[];

//Not used any more for v7.0.2
//Start of PXROS memory. Belongs to GlobalObjMem and GLOBALOBJMEMSIZE. Contains additional global state data.
//extern PxUInt_t GLOBAL_SYSTEM_RAM_BEGIN[];
//extern PxUInt_t GLOBAL_SYSTEM_RAM_END[];


//Added manualle for v7
extern PxUInt_t PxTricSystemDataLowerBound_CPU0_[];
extern PxUInt_t PxTricSystemDataUpperBound_CPU0_[];

extern PxUInt_t PxTricSystemDataLowerBound_CPU1_[];
extern PxUInt_t PxTricSystemDataUpperBound_CPU1_[];

extern PxUInt_t PxTricSystemDataLowerBound_CPU2_[];
extern PxUInt_t PxTricSystemDataUpperBound_CPU2_[];


//Context Save Area
extern PxUInt_t __CSA_BEGIN[];
extern PxUInt_t __CSA_END[];

//Not used any more for v7.0.2
//PXROS kernel data (short adressed)
//extern PxUInt_t PxTricSystemDataLowerBound[];
//extern PxUInt_t PxTricSystemDataUpperBound[];

//Complete LMU RAM range
extern PxUInt_t LDRAM_START[];
extern PxUInt_t LDRAM_END[];

//Peripheral Memory
extern PxUInt_t PERIPHERAL_START[];
extern PxUInt_t PERIPHERAL_END[];

//Flash memory
extern PxUInt_t __TEXT_BEGIN[];
extern PxUInt_t __TEXT_END[];

//Trab table 
extern PxUInt_t __TRAP_TAB_BEGIN[];
extern PxUInt_t __TRAP_TAB_END[];

//Interrupt trap table
extern PxUInt_t __INT_TAB_BEGIN[];
extern PxUInt_t __INT_TAB_END[];

//Size of PXROS system meory
extern PxUInt_t GLOBALOBJMEMSIZE[];

//Nullpointer, used to indicate a not used region
extern PxUInt_t NOT_USED[];

//System memory to store PXROS objects and their names (LMU)
extern PxUInt_t GlobalObjmem[];

//Local scratch pad memory of core 0
extern PxUInt_t DATA_0_START[];
extern PxUInt_t DATA_0_END[];

//Local scratch pad memory of core 1
extern PxUInt_t DATA_1_START[];
extern PxUInt_t DATA_1_END[];

//Local scratch pad memory of core 2
extern PxUInt_t DATA_2_START[];
extern PxUInt_t DATA_2_END[];

//Memory range for the name server 
extern PxUInt_t PxNameServerSpaceBase[];
extern PxUInt_t PxNameServerSpaceEnd[];

//Sync data and PXROS taskmem e.g. for message objects
//extern PxUInt_t GLOBAL_TASK_BEGIN[];
//extern PxUInt_t GLOBAL_TASK_END[];

//rodata section required for const data access
extern PxUInt_t __RODATA_BEGIN[];
extern PxUInt_t __RODATA_END[];

//CAN Peripheral Registers
extern PxUInt_t MODULE_CAN_BEGIN[];
extern PxUInt_t MODULE_CAN_END[];

//UART / ASCLIN 0 Peripheral Registers
extern PxUInt_t PORT_UART_0_BEGIN[];
extern PxUInt_t PORT_UART_0_END[];

//UART / ASCLIN 1 Peripheral Registers
extern PxUInt_t PORT_UART_1_BEGIN[];
extern PxUInt_t PORT_UART_1_END[];

//UART / ASCLIN 2 Peripheral Registers
extern PxUInt_t PORT_UART_2_BEGIN[];
extern PxUInt_t PORT_UART_2_END[];


//UART / ASCLIN 3s Peripheral Registers
extern PxUInt_t PORT_UART_3_BEGIN[];
extern PxUInt_t PORT_UART_3_END[];

//SPI 0 used for TFT and Touchscreen
extern PxUInt_t PORT_SPI_0_BEGIN[];
extern PxUInt_t PORT_SPI_0_END[];

//GPIO 20 used for TFT
extern PxUInt_t PORT_20_BEGIN[];
extern PxUInt_t PORT_20_END[];

//Dunkumware Libraries
extern PxUInt_t LIBDNK_DATA_BEGIN[];
extern PxUInt_t LIBDNK_DATA_END[];


extern PxUInt_t __CPU0_SL_SAFETY_STATE_BEGIN[];
extern PxUInt_t __CPU0_SL_SAFETY_STATE_END[];

extern PxUInt_t __CPU0_TASK_BEGIN[];
extern PxUInt_t __CPU0_TASK_END[];
extern PxUInt_t __CPU0_SL_SAFETY_BEGIN[];
extern PxUInt_t __CPU0_SL_SAFETY_END[];
extern PxUInt_t __CPU1_SL_CONTROL_BEGIN[];
extern PxUInt_t __CPU1_SL_CONTROL_END[];
extern PxUInt_t __GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN[];
extern PxUInt_t __GLOBAL_RW_INIT_RO_AFTERWARDS_END[];
extern PxUInt_t __GLOBAL_TASK_BEGIN[];
extern PxUInt_t __GLOBAL_TASK_END[];
extern PxUInt_t __CPU2_TASK_BEGIN[];
extern PxUInt_t __CPU2_TASK_END[];
extern PxUInt_t __CPU1_TASK_BEGIN[];
extern PxUInt_t __CPU1_TASK_END[];
extern PxUInt_t __CPU0_DATAPOOL_BEGIN[];
extern PxUInt_t __CPU0_DATAPOOL_END[];
extern PxUInt_t __CPU1_DATAPOOL_BEGIN[];
extern PxUInt_t __CPU1_DATAPOOL_END[];
extern PxUInt_t __CPU2_DATAPOOL_BEGIN[];
extern PxUInt_t __CPU2_DATAPOOL_END[];

extern PxUInt_t __CPU2_SL_MISC_BEGIN[];
extern PxUInt_t __CPU2_SL_MISC_END[];

extern PxUInt_t __CPU2_TFT_BEGIN[];
extern PxUInt_t __CPU2_TFT_END[];

// New symbols due to PxROS port
extern const PxUInt_t PxGlobalMem_begin_CPU0_[];
extern const PxUInt_t PxGlobalMem_end_CPU0_[];

extern const PxUInt_t PxGlobalMem_begin_CPU1_[];
extern const PxUInt_t PxGlobalMem_end_CPU1_[];

extern const PxUInt_t PxGlobalMem_begin_CPU2_[];
extern const PxUInt_t PxGlobalMem_end_CPU2_[];


extern const uint32_t            PXROS_SYSTEM_STACK_CPU0_[];					/**< @brief XROS Stack that will be placed in the corresponding DSPRs*/
extern const uint32_t            PXROS_SYSTEM_STACK_CPU1_[];					/**< @brief XROS Stack that will be placed in the corresponding DSPRs*/
extern const uint32_t            PXROS_SYSTEM_STACK_CPU2_[];					/**< @brief XROS Stack that will be placed in the corresponding DSPRs*/

extern const PxUInt_t PXROS_SYSTEM_STACK_BEGIN_CPU0_[];		//Identical to PXROS_SYSTEM_STACK_CPU0_
extern const PxUInt_t PXROS_SYSTEM_STACK_BEGIN_CPU1_[];
extern const PxUInt_t PXROS_SYSTEM_STACK_BEGIN_CPU2_[];
extern const PxUInt_t PXROS_SYSTEM_STACK_SIZE_CPU0_[];
extern const PxUInt_t PXROS_SYSTEM_STACK_SIZE_CPU1_[];
extern const PxUInt_t PXROS_SYSTEM_STACK_SIZE_CPU2_[];

extern const PxUInt_t PX_OBJMEMSIZE_CPU0_[];
extern const PxUInt_t PxObjmem_CPU0_[];

extern const PxUInt_t PX_OBJMEMSIZE_CPU1_[];
extern const PxUInt_t PxObjmem_CPU1_[];

extern const PxUInt_t PX_OBJMEMSIZE_CPU2_[];
extern const PxUInt_t PxObjmem_CPU2_[];





#endif /* SYMBOLS_H_ */
