/*
 * cyclic_runnable_table.cpp
 *
 *  Created on: May 4, 2016
 *      Author: Shriki
 */
/******************************************************************************/
/*-------------------------------Includes-------------------------------------*/
/******************************************************************************/
#include "rte_time.h"
#include "Gpt_timer.h"
#include "UART.h"
#include "swc_control.h"
#include "PORT.h"
#include "rte_time_type.h"
#include "CRingBuffer.h"

/******************************************************************************/
/*-------------------------------definitions----------------------------------*/
/******************************************************************************/
#define RTE_TIME_PREAMBLE			(0x5A)
#define RTE_TIME_DATA_LENGTH		(3)
#define RTE_MAX_BUFFERS		    	((sizeof(RTE_TIME_timings_t) * RTE_NO_OF_PARAMS * RTE_PRESENT_TOTAL_TASKS) * RTE_NO_OF_BUFFERS)
/******************************************************************************/
/*-------------------------------Const Variables------------------------------*/
/******************************************************************************/
/*
 * @\var	: RTE_End1 & RTE_End2
 * @\type	: RTE_TIME_timings_t
 * @\brief	: Ring Buffer end of protocol data.
 * @\detail : This is needed as ring buffer is a template class and expects this.
 * 			  However, this is not used in this application.
 */
const RTE_TIME_timings_t RTE_End1 = {0,0,0,0,0,0,0};
const RTE_TIME_timings_t RTE_End2 = {0,0,0,0,0,0,0};

const uint16_t endOfRteDataPkt    = 0xAAAA;
const uint16_t lengthOfRteDataPkt = sizeof(RTE_TIME_timings_t) + sizeof(endOfRteDataPkt);

/******************************************************************************/
/*--------------------------extern global variables---------------------------*/
/******************************************************************************/
/*
 * @\var	: RTE_TIME_Task_C0_CommunicationTimings, RTE_TIME_Task_C0_Safety_InputTimings,
 * 			  RTE_TIME_Task_C0_Safety_LogicTimings, RTE_TIME_Task_C1_ControlTimings.
 * @\type	: CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS>
 * @\brief	: Ring Buffer to store each task and runnable timings.
 * @\detail	: User Should create a ring buffer if new tasks gets added with its name.
 */
extern CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS> RTE_TIME_Task_C0_CommunicationTimings;
extern CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS> RTE_TIME_Task_C0_Safety_InputTimings;
extern CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS> RTE_TIME_Task_C0_Safety_LogicTimings;
extern CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS> RTE_TIME_Task_C1_ControlTimings;

/*
 * @\var	: RTE_TIME_taskTimeTable
 * @\type	: CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS>* const
 * @\brief	: Array (table) of ring buffer's addresses stored in FLASH to give access to all cores.
 * @\detail	: Only addresses are visible not actual buffer.
 */
extern CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS>* const  RTE_TIME_taskTimeTable[];

/******************************************************************************/
/*-------------------------------enum variables-------------------------------*/
/******************************************************************************/
/**
 * @\def	: union varType
 * @\brief	: This is to convert the different types of data.
 */
typedef union
{
	uint8_t byte[4];
	uint16_t word[2];
	uint32_t doubleWord;
}type_convert_t;

/*
 * @\def	: typedef enum eMsbLsbpos_t
 * @\brief	: Used for internal readability. Used to send 32, 16 and 8 bit data from an array.
 */
typedef enum
{
	u16Lsb = 0,
	u16Msb = 1,
	u32Lsb = 2,
	u32Msb = 3
}eMsbLsbpos_t;

/*
 * @\def	: typedef enum rteCommands_t
 * @\brief	: RTE TIME measurements Communication Commands (request cmds)
 */
typedef enum
{
	eNop 			  = 0x00,
	eSendBooupMsg 	  = 0x01,
	eSendRealTimeMsg  = 0x02,
	eStopTx 		  = 0x03,
	eClearBuffer	  = 0x04,
	eRunnableMsgCmd	  = 0x10,
	eTaskStartCmd	  = 0x11,
	eTaskEndCmd		  = 0x12,
	eRunnableStartCmd = 0x13,
	eRunnableEndCmd   = 0x14,
	ePktPreamble      = 0x5A,
	eRBufferOverFlow  = 0xFF
}rteCommands_t;

/******************************************************************************/
/*--------------------------User pragma section variables---------------------*/
/******************************************************************************/
/**
 * Ringbuffers storing the runnables associated with each tasks
 */
#pragma section ".data.rtetime.ringbufferC0"
CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS> RTE_TIME_Task_C0_CommunicationTimings(RTE_End1, RTE_End2);
CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS> RTE_TIME_Task_C0_Safety_InputTimings(RTE_End1, RTE_End2);
CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS> RTE_TIME_Task_C0_Safety_LogicTimings(RTE_End1, RTE_End2);

volatile boolean_t taskInputFlag = 0;
#pragma section // ".data.rtetime.ringbufferC0"

#pragma section ".data.rtetime.ringbufferC1"
CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS> RTE_TIME_Task_C1_ControlTimings(RTE_End1, RTE_End2);
#pragma section // ".data.rtetime.ringbufferC1"

#pragma section ".data.rtetime.ringbufferC2"
//Add CRingbuffers required for Tasks created in Core-2
#pragma section //".data.rtetime.ringbufferC2"


#pragma section ".rodata"
/**
 * Central data structure for all cores
 */
CRingBuffer<RTE_TIME_timings_t, RTE_MAX_BUFFERS>* const RTE_TIME_taskTimeTable[] = {
		&RTE_TIME_Task_C0_CommunicationTimings,
		&RTE_TIME_Task_C0_Safety_InputTimings,
		&RTE_TIME_Task_C0_Safety_LogicTimings,
		&RTE_TIME_Task_C1_ControlTimings
};
#pragma section // ".rodata"

const uint16_t RTE_TIME_taskTimeTableSize = 4;//sizeof(RTE_TIME_taskTimeTable)/sizeof(RTE_TIME_timings_t);


/**
 * section ".data.uart_rte" is used to provide access to Task_C0_Init task and protect from others.
 * This is put under the symbol GLOBAL_SYSTEM_RAM_BEGIN and GLOBAL_SYSTEM_RAM_END in pxros.ld linker file.
 */
#pragma section ".data.rtetime.system"
/**
 * @var		:	volatile uint16_t gRteTimeMsbCntr
 * @brief	:	Holds the GPT T3 Overflow counts
 */
volatile uint16_t gRteTimeMsbCntr = 0;

uint8_t RBsize = sizeof(RTE_TIME_timings_t);

/**
 * @var		:	volatile uint16_t gRteTimeMsbCntr
 * @brief	:	Holds the GPT T3 Overflow counts
 */
RTE_TIME_initData_t RTE_TIME_appTaskInitDetails[RTE_MAX_CORES][RTE_MAX_TASKS_PER_CORE];

/**
 * @var		:	char* RTE_TaskNamesTable
 * @brief	:	Holds the Task Names. To be removed after PXROS removes name storage restriction.
 */
const char *RTE_TaskNamesTable[6] = {"Task_C0_Safety_Input", "Task_C0_Safety_Logic", "Task_C0_Communication", "Task_C0_State", //Core_0 tasks names
									 "Task_C1_Control", 																	   //Core_1 tasks names
									 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	   //Core_2 task names
									 '\0'
									};

uint8_t RTE_TIME_noOfAppTasksInCore0 = 0;
uint8_t RTE_TIME_noOfAppTasksInCore1 = 0;
uint8_t RTE_TIME_noOfAppTasksInCore2 = 0;
//Set Initially with NOP command.
uint8_t RTE_TIME_comCommand          = eNop;

//TIME MEASUREMENT GLOBAL FLAGS
volatile boolean_t rteBufferOverflowFlag = 0;
boolean_t RTE_TIME_fillDataFlag		= 0;


#pragma section		// ".data.rtetime.system"

/******************************************************************************/
/*---------------------------Local Functions----------------------------------*/
/******************************************************************************/
/*@brief \:	This function sends the task details stored in a table. This table is maintained manually at the moment.
 * This is because of limitations of PXROS to read out the task names with its ID during task creation.
 * After fixing the PXROS this can be removed.*/
static void fillTaskName(uint8_t bufferSize, char* destBuffer, const char* srcBuffer);

/**
 * @\func	: static void RTE_TIME_SetCommand(uint8_t rteCmdData);
 * @\brief	: Called by Communication Task when RTE_TIME related command is received from GUI.
 */
static void RTE_TIME_SetCommand(uint8_t rteCmdData);

/**
 * @\func	: static void RTE_TIME_ClearRingBuffer(void);
 * @\brief	: Clear the Ring Buffer for restart of Time Measurement. This is use full When Ring Buffers overflow.
 */
static void RTE_TIME_ClearRingBuffer(void);

/******************************************************************************/
/*---------------------------Global Functions---------------------------------*/
/******************************************************************************/
/**
 * @\func	:	void RTE_TimerInit(void)
 * @\brief	: 	Initialises the GPT timer T3 module for RTE time measurements.
 * @\params	:	void
 * @\return	:	void
 */
void RTE_TIME_TimerInit(void)
{
	//Initialize GPT12_T3
	GPT12_Init(GPT12_T3);
	GPT12_StartStop(GPT12_T3,Stop);
}

/**
 * @\func	:	void RTE_MsbTicksIncr(void)
 * @\brief	: 	This is the handler for GPT T3 module.
 * @\detail	:	This keeps MSB counts of T3 overflow.
 * @\params	:	void
 * @\return	:	void
 */
void RTE_TIME_MsbTicksIncr(void)
{
	gRteTimeMsbCntr++;
}

/**
 * @\func	:	uint32_t RTE_GetTime(void)
 * @\brief	: 	Read the Current timer value and return with MSB ticks
 * @\params	:	void
 * @\return	:	uint32_t
 */
uint32_t RTE_TIME_GetTime(void)
{
	return ((GPT12_TimerRead(GPT12_T3) & 0x0000FFFF) | (uint32_t)(gRteTimeMsbCntr << 16));
}

/**
 * @\func	:	void RTE_TIME_InitDataTx(void)
 * @\brief	: 	Transmit the Tasks name & their Ids on uartusb
 * @\detail	:	this is gets called when related command is sent by GUI.
 * @\params	:	void
 * @\return	:	void
 */
void RTE_TIME_InitDataTx(void)
{
	uint8_t coreIndex = 0;
	uint8_t taskIndex = 0;
	uint8_t noOftasks = 0;
	uint8_t charCnt = 0;
	type_convert_t baseType;

	uint8_t noOfTasksCreatedBuffer[RTE_MAX_CORES] = {RTE_TIME_noOfAppTasksInCore0, RTE_TIME_noOfAppTasksInCore1, RTE_TIME_noOfAppTasksInCore2};

	//The befow for loop can be deleted after PXROS extends its task name storage capacity
	//The following for loop is a workaround
	uint8_t nameTableCnt = 0;
	for(coreIndex = 0; coreIndex < RTE_MAX_CORES; coreIndex++)
	{
		noOftasks = noOfTasksCreatedBuffer[coreIndex];
		for(taskIndex = 0; taskIndex < noOftasks; taskIndex++)
		{
			fillTaskName(RTE_TASKNAME_SIZE, &(RTE_TIME_appTaskInitDetails[coreIndex][taskIndex].taskName[0]), RTE_TaskNamesTable[nameTableCnt]);
			nameTableCnt++;
		}
	}

	for(coreIndex = 0; coreIndex < RTE_MAX_CORES; coreIndex++)
	{
		noOftasks = noOfTasksCreatedBuffer[coreIndex];
		for(taskIndex = 0; taskIndex < noOftasks; taskIndex++)
		{
			while(UART_WriteData(uartusb, (uint8_t)ePktPreamble) != RC_SUCCESS);//Start of a Frame eSendBooupMsg
			while(UART_WriteData(uartusb, 0x00) != RC_SUCCESS);// dummy lngth TODO: check what to add for this length
			while(UART_WriteData(uartusb, (uint8_t)eSendBooupMsg) != RC_SUCCESS);
			while(UART_WriteData(uartusb, coreIndex) != RC_SUCCESS);

			baseType.doubleWord = 0;
			baseType.doubleWord = ((RTE_TIME_appTaskInitDetails[coreIndex])+taskIndex)->taskID.id;
			while(UART_WriteData(uartusb, baseType.byte[u16Msb]) != RC_SUCCESS); //TaskId MSB
			while(UART_WriteData(uartusb, baseType.byte[u16Lsb]) != RC_SUCCESS); //TaskId LSB
			while((((RTE_TIME_appTaskInitDetails[coreIndex])+taskIndex)->taskName[charCnt] != '\0') && (charCnt < 20))
			{
				while(UART_WriteData(uartusb, ((RTE_TIME_appTaskInitDetails[coreIndex])+taskIndex)->taskName[charCnt]) != RC_SUCCESS);
				charCnt++;
			}
			baseType.doubleWord = (uint32_t)endOfRteDataPkt;
			while(UART_WriteData(uartusb, baseType.byte[u16Msb]) != RC_SUCCESS);
			while(UART_WriteData(uartusb, baseType.byte[u16Lsb]) != RC_SUCCESS);
			charCnt = 0;
		}
	}
}

/**
 * @\func	:	void RTE_TIME_SetCommand(uint8_t rteCmdData)
 * @\brief	: 	Sets the RTE Timing command
 * @\detail	:	this gets called from the communication task once it receives full protocol.
 * @\params	:	uint8_t rteCmdData
 * @\return	:	void
 */
void RTE_TIME_SetCommand(uint8_t rteCmdData)
{
	RTE_TIME_comCommand = (uint8_t)eNop;
	RTE_TIME_comCommand = rteCmdData;
	if(RTE_TIME_comCommand == eSendRealTimeMsg)
	{
		GPT12_Reload(GPT12_T3);
		GPT12_StartStop(GPT12_T3, Start);
		RTE_TIME_fillDataFlag = 1;
	}
	else
	{
		GPT12_StartStop(GPT12_T3, Stop);
		RTE_TIME_fillDataFlag = 0;
	}
}

/**
 * @\func	:	void RTE_TIME_checkAndSetComCommand(uint8_t *pProtocolBuffer)
 * @\brief	: 	validate the recived data and set the com command
 * @\detail	:	-
 * @\params	:	uint8_t *pProtocolBuffer
 * @\return	:	void
 */
void RTE_TIME_checkAndSetComCommand(uint8_t *pProtocolBuffer)
{
	if((*(pProtocolBuffer++) == RTE_TIME_PREAMBLE) && (*(pProtocolBuffer++) == RTE_TIME_DATA_LENGTH))
	{
		RTE_TIME_SetCommand(*pProtocolBuffer);
	}
}

/**
 * @\func	:	void RTE_TIME_fillTaskStartInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index)
 * @\brief	: 	Fills the Task respective ring buffer with the task start time details.
 * @\detail	:	this gets called from the runnable execute which gets called by every task.
 * @\params	:	uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index
 * @\return	:	void
 */
void RTE_TIME_fillTaskStartInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index)
{
	RTE_TIME_timings_t lRteData;
	lRteData.runnableAddr = pRunnable;
	lRteData.cpuCoreId = coreId;
	lRteData.taskId    = taskId.id;
	/*!"RTE_TIME_GetTime() + 1" is to add (1*0.640)uSec offset to the task end time.
	 * This is to eliminate the chances of having both previous task end  & current task start time as same.
	 * If both times are same then this creates problems in GUI chart plotter during Sorting with respect to time.
	 * The reason behind this occurances chances is due to 300MHz system clock and timer resolution is 2.56uSec,
	 * which is too high enough to execute a runnable.
	 * This offset will be subtracted in the GUI plotter after the final sorting. */
	lRteData.time      = RTE_TIME_GetTime() + 1;
	lRteData.taskDataFlag = 1;
	lRteData.startTimeFlag = 1;
	lRteData.endTimeFlag   = 0;
	if(RTE_TIME_taskTimeTable[index]->writeData(lRteData) == false)
	{
		rteBufferOverflowFlag = 1;
	}
}

/**
 * @\func	:	void RTE_TIME_fillTaskEndInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index)
 * @\brief	: 	Fills the Task respective ring buffer with the task end time details.
 * @\detail	:	this gets called from the runnable execute which gets called by every task.
 * @\params	:	uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index
 * @\return	:	void
 */
void RTE_TIME_fillTaskEndInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index)
{
	RTE_TIME_timings_t lRteData;
	lRteData.runnableAddr = pRunnable;
	lRteData.cpuCoreId = coreId;
	lRteData.taskId    = taskId.id;
	/*!"RTE_TIME_GetTime() + 4" is to add (4*0.640)uSec offset to the task end time.
	 * This is to eliminate the chances of having both runnable end & task end time as same.
	 * If both times are same then this creates problems in GUI chart plotter during Sorting with respect to time.
	 * The reason behind this occurances chances is due to 300MHz system clock and timer resolution is 2.56uSec,
	 * which is too high enough to execute a runnable.
	 * This offset will be subtracted in the GUI plotter after the final sorting. */
	lRteData.time      = RTE_TIME_GetTime() + 4;
	lRteData.taskDataFlag = 1;
	lRteData.startTimeFlag = 0;
	lRteData.endTimeFlag   = 1;
	if(RTE_TIME_taskTimeTable[index]->writeData(lRteData) == false)
	{
		rteBufferOverflowFlag = 1;
	}
}

/**
 * @\func	:	void RTE_TIME_fillRunnableStartInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index)
 * @\brief	: 	Fills the Task respective ring buffer with the runnables start time details.
 * @\detail	:	this gets called from the runnable execute which gets called by every task.
 * @\params	:	uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index
 * @\return	:	void
 */
void RTE_TIME_fillRunnableStartInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index)
{
	RTE_TIME_timings_t lRteData;
	lRteData.runnableAddr = pRunnable;
	lRteData.cpuCoreId = coreId;
	lRteData.taskId    = taskId.id;
	/*!"RTE_TIME_GetTime() + 2" is to add (2*0.640)uSec offset to the task end time.
	 * This is to eliminate the chances of having both runnable start & task start time as same.
	 * If both times are same then this creates problems in GUI chart plotter during Sorting with respect to time.
	 * The reason behind this occurances chances is due to 300MHz system clock and timer resolution is 2.56uSec,
	 * which is too high enough to execute a runnable.
	 * This offset will be subtracted in the GUI plotter after the final sorting. */
	lRteData.time      = RTE_TIME_GetTime() + 2;
	lRteData.taskDataFlag = 0;
	lRteData.startTimeFlag = 1;
	lRteData.endTimeFlag   = 0;
	if(RTE_TIME_taskTimeTable[index]->writeData(lRteData) == false)
	{
		rteBufferOverflowFlag = 1;
	}
}

/**
 * @\func	:	void RTE_TIME_fillRunnableEndInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index)
 * @\brief	: 	Fills the Task respective ring buffer with the runnables end time details.
 * @\detail	:	this gets called from the runnable execute which gets called by every task.
 * @\params	:	uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index
 * @\return	:	void
 */
void RTE_TIME_fillRunnableEndInfo(uint8_t coreId, PxTask_t taskId, RT_runnablePtr_t pRunnable, RTE_TIME_taskRingBufferIndex_t index)
{
	RTE_TIME_timings_t lRteData;
	lRteData.runnableAddr = pRunnable;
	lRteData.cpuCoreId = coreId;
	lRteData.taskId    = taskId.id;
	/*!"RTE_TIME_GetTime() + 3" is to add (3*0.640)uSec offset to the task end time.
	 * This is to eliminate the chances of having both runnable end & task end time as same.
	 * If both times are same then this creates problems in GUI chart plotter during Sorting with respect to time.
	 * The reason behind this occurances chances is due to 300MHz system clock and timer resolution is 2.56uSec,
	 * which is too high enough to execute a runnable.
	 * This offset will be subtracted in the GUI plotter after the final sorting. */
	lRteData.time      = RTE_TIME_GetTime() + 3;
	lRteData.taskDataFlag = 0;
	lRteData.startTimeFlag = 0;
	lRteData.endTimeFlag   = 1;
	if(RTE_TIME_taskTimeTable[index]->writeData(lRteData) == false)
	{
		rteBufferOverflowFlag = 1;
	}
}

/**
 * @see			void RTE_TIME_SendOutRteData(void)
 * @brief		Sends out the RTE Ring buffer data on UARTUSB byte by byte
 * @param		void
 * @return		void
 */
void RTE_TIME_SendOutRteData(void)
{
	type_convert_t baseType;
	RTE_TIME_timings_t rteData;

	if(RTE_TIME_comCommand == eNop)
	{
		/*do nothing*/
	}
	else if(RTE_TIME_comCommand == (uint8_t)eSendBooupMsg)
	{
		//Send the Boot up msg (task IDs and associated task Name)
		RTE_TIME_InitDataTx();
		//Boot up msg shall be sent only once. So, assign back to NOP
		RTE_TIME_comCommand = eNop;
	}
	else if(RTE_TIME_comCommand == (uint8_t)eClearBuffer)
	{
		RTE_TIME_ClearRingBuffer();
		RTE_TIME_comCommand = eNop;
	}
	else if((RTE_TIME_comCommand == (uint8_t)eSendRealTimeMsg) || (RTE_TIME_comCommand == (uint8_t)eStopTx))
	{
		for(uint8_t index = 0; index < RTE_TIME_taskTimeTableSize; index++)
		{
			while(RTE_TIME_taskTimeTable[index]->getFilledCount())
			{
				RTE_TIME_taskTimeTable[index]->readData(rteData);
				while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)ePktPreamble));
				if(rteData.taskDataFlag == 1)
				{
					while(RC_SUCCESS != UART_WriteData(uartusb, 0x0A));//10 bytes in Task packet
					if(rteData.startTimeFlag == 1)
					{
						if(!rteBufferOverflowFlag)
							while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)eTaskStartCmd));
						else
							while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)eRBufferOverFlow));
					}
					else if(rteData.endTimeFlag == 1)
					{
						if(!rteBufferOverflowFlag)
							while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)eTaskEndCmd));
						else
							while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)eRBufferOverFlow));
					}else{;}

					baseType.doubleWord = (uint32_t)rteData.taskId;
					while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Msb]));
					while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Lsb]));
					while(RC_SUCCESS != UART_WriteData(uartusb, rteData.cpuCoreId));
				}
				else
				{
					while(RC_SUCCESS != UART_WriteData(uartusb, 0x0E));//14 bytes in Runnable Packet
					if(rteData.startTimeFlag == 1)
					{
						if(!rteBufferOverflowFlag)
							while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)eRunnableStartCmd));
						else
							while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)eRBufferOverFlow));
					}
					else if(rteData.endTimeFlag == 1)
					{
						if(!rteBufferOverflowFlag)
							while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)eRunnableEndCmd));
						else
							while(RC_SUCCESS != UART_WriteData(uartusb, (uint8_t)eRBufferOverFlow));
					}else{;}
					baseType.doubleWord = (uint32_t)rteData.taskId;
					while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Msb]));
					while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Lsb]));
					while(RC_SUCCESS != UART_WriteData(uartusb, rteData.cpuCoreId));
					baseType.doubleWord = 0;
					baseType.doubleWord = (uint32_t)rteData.runnableAddr;
					while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u32Msb]));
					while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u32Lsb]));
					while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Msb]));
					while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Lsb]));
				}

				baseType.doubleWord = (uint32_t)rteData.time;
				while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u32Msb]));
				while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u32Lsb]));
				while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Msb]));
				while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Lsb]));

				baseType.doubleWord = (uint32_t)endOfRteDataPkt;
				while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Msb]));
				while(RC_SUCCESS != UART_WriteData(uartusb, baseType.byte[u16Lsb]));
			}
		}
	}else{;}
}

/**
 * @see			void fillTaskName()
 * @brief		copies the Task names from srcBuffer to destBuffer.
 * @param		char *srcBuffer, char *destBuffer, uint8_t bufferSize
 * @return		void
 */
static void fillTaskName(uint8_t bufferSize, char* destBuffer, const char* srcBuffer)
{
	uint8_t i = 0;
	while((i < bufferSize) && (srcBuffer[i] != '\0'))
	{
		destBuffer[i] = srcBuffer[i];
		i++;
	}
}

/**
 * @see			void RTE_TIME_ClearRingBuffer(void)
 * @brief		Clear the Ring Buffer of Time Measurements
 * @param		void
 * @return		void
 */
void RTE_TIME_ClearRingBuffer(void)
{
	RTE_TIME_timings_t rteData;
	//Clear all task associated ring buffers
	for(uint8_t index = 0; index < RTE_TIME_taskTimeTableSize; index++)
	{
		//read until fillLevel becomes Null.
		while(RTE_TIME_taskTimeTable[index]->getFilledCount())
		{
			//Reading out the data from Ring Buffer clears the buffer data by decrementing fillLevel to 0.
			RTE_TIME_taskTimeTable[index]->readData(rteData);
		}
	}
	//clear the buffer overflow flag to restart the measurement.
	rteBufferOverflowFlag = 0;
}
