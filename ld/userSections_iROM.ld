/*iROM*/
/*internal flash configuration*/

SECTIONS
{



.CPU0.task   :
	{
		PROVIDE(__CPU0_TASK_BEGIN = .);

		*(.Task_C0_Init.stack) 
		*(.Task_C0_Safety_Input) 
		*(.Task_C0_Safety_Logic) 
		*(.Task_C0_Communication) 
		*(.Task_C0_ReleaseServer.data) 
		*(.Task_C0_State) 
		*(.Task_C0_Safety_System)

		. = ALIGN(8);
		PROVIDE(__CPU0_TASK_END = .);

	} > glb_mem_dsprcpu0  


.CPU0.sl_safety   :
	{
		PROVIDE(__CPU0_SL_SAFETY_BEGIN = .);

		*(.sl_safety) 



		. = ALIGN(8);
		PROVIDE(__CPU0_SL_SAFETY_END = .);

	} > glb_mem_dsprcpu0  AT > mem_flash0 


.CPU0.sl_safety_state   :
	{
		PROVIDE(__CPU0_SL_SAFETY_STATE_BEGIN = .);

		*(.sl_safety_state) 

		. = ALIGN(8);
		PROVIDE(__CPU0_SL_SAFETY_STATE_END = .);

	} > glb_mem_dsprcpu0  AT > mem_flash0 

.CPU1.sl_control   :
	{
		PROVIDE(__CPU1_SL_CONTROL_BEGIN = .);

		*(.sl_control) 


		. = ALIGN(8);
		PROVIDE(__CPU1_SL_CONTROL_END = .);

	} > glb_mem_dsprcpu1  AT > mem_flash0 
	
.CPU2.sl_misc   :
	{
		PROVIDE(__CPU2_SL_MISC_BEGIN = .);

		*(.sl_misc) 


		. = ALIGN(8);
		PROVIDE(__CPU2_SL_MISC_END = .);

	} > glb_mem_dsprcpu2  AT > mem_flash0 


.GLOBAL.RW_init_RO_afterwards   :
	{
		PROVIDE(__GLOBAL_RW_INIT_RO_AFTERWARDS_BEGIN = .);

		*(.Task_Init_Lock.data)
		*(.EventDispatcher_Table) 

		. = ALIGN(8);
		PROVIDE(__GLOBAL_RW_INIT_RO_AFTERWARDS_END = .);

	} > lmu_sram_nc  AT > mem_flash0 


.GLOBAL.task   :
	{
		PROVIDE(__GLOBAL_TASK_BEGIN = .);

		*(.TRAP.data)
		*(.SYNC.Task_Sync.data) 
		*(.GLOBAL.pxros.taskmem) 
		*(.DET.data)
		*(.data.det_time)
		*(.trace.data) 
		*(.data.rtetime.system)
		*(.drv.isr.data)
		*(.STM.locks)		
		
		. = ALIGN(8);
		PROVIDE(__GLOBAL_TASK_END = .);

	} > lmu_sram_nc  AT > mem_flash0 

.GLOBAL.exmem   :
	{
		PROVIDE(__GLOBAL_EMEM_BEGIN = .);


		. = ALIGN(8);
		PROVIDE(__GLOBAL_EMEM_END = .);

	} > mem_EX_nc  AT > mem_flash3 

.CPU2.task   :
	{
		PROVIDE(__CPU2_TASK_BEGIN = .);

		*(.Task_C2_Init.stack) 
		*(.Task_C2_ReleaseServer.data) 
		*(.Task_C2_TFT.stack) 

		. = ALIGN(8);
		PROVIDE(__CPU2_TASK_END = .);

	} > glb_mem_dsprcpu2  


.CPU1.task   :
	{
		PROVIDE(__CPU1_TASK_BEGIN = .);

		*(.Task_C1_Init.stack) 
		*(.Task_C1_ReleaseServer.data) 
		*(.Task_C1_Control) 

		. = ALIGN(8);
		PROVIDE(__CPU1_TASK_END = .);

	} > glb_mem_dsprcpu1  


.CPU0.datapool   :
	{
		PROVIDE(__CPU0_DATAPOOL_BEGIN = .);

		*(.com.canopen)
		*(.com.canopen.datadictionary.safety)
		*(.com.canopen.datadictionary.control)
		*(.com.uart) 
		*(.com.can) 
		*(.data.rtetime.ringbufferC0)
		*(.data.rtetime.system)
		*(.data.remotectrl)
		


		. = ALIGN(8);
		PROVIDE(__CPU0_DATAPOOL_END = .);

	} > glb_mem_dsprcpu0  

.CPU1.datapool   :
	{
		PROVIDE(__CPU1_DATAPOOL_BEGIN = .);

		*(.data.rtetime.ringbufferC1)

		. = ALIGN(8);
		PROVIDE(__CPU1_DATAPOOL_END = .);

	} > glb_mem_dsprcpu1
	
.CPU2.datapool   :
	{
		PROVIDE(__CPU2_DATAPOOL_BEGIN = .);

		*(.data.rtetime.ringbufferC2)


		. = ALIGN(8);
		PROVIDE(__CPU2_DATAPOOL_END = .);

	} > glb_mem_dsprcpu2

.CPU2.tft   :
	{
		PROVIDE(__CPU2_TFT_BEGIN = .);

		*(.Task_C2_TFT.data) 
		*(.com.spi)
		*(.data.tft)
		*(.data.conio)
		*(.data.ui)


		. = ALIGN(8);
		PROVIDE(__CPU2_TFT_END = .);


	} > glb_mem_dsprcpu2  AT > mem_flash0 
}