/*
 * cyclic_runnable_table.cpp
 *
 *  Created on: May 4, 2016
 *      Author: Shriki
 */
/******************************************************************************/
/*-------------------------------Includes-------------------------------------*/
/******************************************************************************/
#include "runnable_table.h"
#include "pxdef.h"
#include "SystemEvents.h"
#include "PORT.h"
#include "det.h"


/******************************************************************************/
/*---------------------------Local Functions----------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*---------------------------Global Functions---------------------------------*/
/******************************************************************************/


/**
 * @\func	:	void RunnablesInit(runnablePtr_t* pTableInit, uint16_t tableSize)
 * @\brief	: 	Executes/Calls the runnable init functions put in the task table.
 * @\detail	:	Task's Init table address and size must be passed.
 * @\params	:	runnablePtr_t* pTableInit, uint16_t tableSize
 * @\return	:	void
 */
void RT_initRunExecute(const RT_initTable_t* pTableInit, uint16_t tableSize)
{
	for(uint16_t i = 0; i <tableSize; i++)
	{
		pTableInit[i].initRunnable();
	}
}



/**
 * @\func	:	RT_cyclicRunExecute10ms(const RT_cyclicSafetyTable_t*  pTableCyclic, uint16_t tableSize, uint32_t taskTime,
 * 				PxTask_t taskId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex, PxTask_t stateTaskId)
 * @\brief	: 	Executes/Calls the runnables put in the task runnable table &
 * 				stores the task details only if the instance has some runnables to execute.
 * @\detail	:	This is the workaround function for RT_cyclicRunExecute(). This function is called by Tasks which
 * 				have 10ms as their activation time. This is critical as time measurement buffers overflow if
 * 				activation time is reduced.
 * @\params	:	const RT_cyclicSafetyTable_t*  pTableCyclic, uint16_t tableSize, uint32_t taskTime,
 *				PxTask_t taskId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex, PxTask_t stateTaskId
 * @\return	:	void
 */
void RT_cyclicRunExecute(const RT_cyclicTable_t*  pTableCyclic, uint16_t tableSize, uint32_t taskTime, uint32_t cycleTime,
		PxTask_t taskId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex)
{
#ifdef RTE_TIMING_ANALYSIS
	uint8_t cnt = 0;


	//Correct task time and offset time based on cycle time of the call

	for(uint16_t tableIndex = 0; tableIndex < tableSize; tableIndex++)
	{
		if(((cycleTime * (taskTime - (pTableCyclic[tableIndex].offsetTime / cycleTime))) %  pTableCyclic[tableIndex].cyclicTime) == 0)
			cnt++;
	}

	if((RTE_TIME_fillDataFlag == 1) && (cnt > 0))
		RTE_TIME_fillTaskStartInfo((uint8_t)SYSTEM_GetCoreId(), taskId, 0, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
	for(uint16_t tableIndex = 0; tableIndex < tableSize; tableIndex++)
	{
		if(((cycleTime * (taskTime - (pTableCyclic[tableIndex].offsetTime / cycleTime))) %  pTableCyclic[tableIndex].cyclicTime) == 0)
		{
			if (pTableCyclic[tableIndex].runnable != 0)
			{

#ifdef RTE_TIMING_ANALYSIS
				if(RTE_TIME_fillDataFlag)
					RTE_TIME_fillRunnableStartInfo((uint8_t)SYSTEM_GetCoreId(), taskId, pTableCyclic[tableIndex].runnable, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
				pTableCyclic[tableIndex].runnable();
#ifdef RTE_TIMING_ANALYSIS
				if(RTE_TIME_fillDataFlag)
					RTE_TIME_fillRunnableEndInfo((uint8_t)SYSTEM_GetCoreId(), taskId, pTableCyclic[tableIndex].runnable, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
			}
		}
	}
#ifdef RTE_TIMING_ANALYSIS
	if((RTE_TIME_fillDataFlag == 1) && (cnt > 0))
		RTE_TIME_fillTaskEndInfo((uint8_t)SYSTEM_GetCoreId(), taskId, 0, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
}

/**
 * @\func	:	RT_eventRunExecute(const RT_eventTable_t*  pTableEvent, uint16_t tableSize, uint32_t eventMask,
 * 								   PxTask_t senderId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex)
 * @\brief	: 	Executes/Calls the runnables put in the task runnable table.
 * @\detail	:	This function is called by Tasks which have eventTables configured.
 * @\params	:	const RT_eventTable_t*  pTableEvent, uint16_t tableSize, uint32_t eventMask,
 *              PxTask_t senderId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex
 * @\return	:	void
 */
void RT_eventRunExecute(const RT_eventTable_t*  pTableEvent, uint16_t tableSize, uint32_t eventMask, PxTask_t senderId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex)
{
#ifdef RTE_TIMING_ANALYSIS
	uint8_t cnt = 0;
	for(uint16_t tableIndex = 0; tableIndex < tableSize; tableIndex++)
	{
		if(eventMask & pTableEvent[tableIndex].evMask)
			cnt++;
	}
	if((RTE_TIME_fillDataFlag == 1) && (cnt > 0))
		RTE_TIME_fillTaskStartInfo((uint8_t)SYSTEM_GetCoreId(), senderId, 0, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS

	//Loop through all runnables
	for(uint16_t tableIndex = 0; tableIndex < tableSize; tableIndex++)
	{
		//Runnable for the corresponding event has been found
		if (eventMask & pTableEvent[tableIndex].evMask)
		{
			//Call the runnable
			if (pTableEvent[tableIndex].runnable != 0)
			{
				/*!Added By Shrikant*/
#ifdef RTE_TIMING_ANALYSIS
			if(RTE_TIME_fillDataFlag)
				RTE_TIME_fillRunnableStartInfo((uint8_t)SYSTEM_GetCoreId(), senderId, (RT_runnablePtr_t)pTableEvent[tableIndex].runnable, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
				/*!End*/
				pTableEvent[tableIndex].runnable();

				/*!Added By Shrikant*/
#ifdef RTE_TIMING_ANALYSIS
			if(RTE_TIME_fillDataFlag)
				RTE_TIME_fillRunnableEndInfo((uint8_t)SYSTEM_GetCoreId(), senderId, (RT_runnablePtr_t)pTableEvent[tableIndex].runnable, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
				/*!End*/
			}
		} //if runnable called
	} //loop through all runnables

#ifdef RTE_TIMING_ANALYSIS
	if((RTE_TIME_fillDataFlag == 1) && (cnt > 0))
		RTE_TIME_fillTaskEndInfo((uint8_t)SYSTEM_GetCoreId(), senderId, 0, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
}

/**
 * @\func	:	RT_cyclicRunSafetyExecute10ms(const RT_cyclicSafetyTable_t*  pTableCyclic, uint16_t tableSize, uint32_t taskTime,
 *				PxTask_t taskId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex, PxTask_t stateTaskId)
 * @\brief	: 	Executes/Calls the runnables put in the safety task runnable table.
 * @\detail	:	This is the workaround function for RT_cyclicRunSafetyExecute(). This function is called by Tasks
 * 				which have 10ms as their activation time. This is critical as time measurement buffers overflow if
 * 				activation time is reduced.
 * @\params	:	const RT_cyclicSafetyTable_t*  pTableCyclic, uint16_t tableSize, uint32_t taskTime,
 *				PxTask_t taskId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex, PxTask_t stateTaskId
 * @\return	:	void
 */
RT_safety_t RT_cyclicRunSafetyExecute(const RT_cyclicSafetyTable_t*  pTableCyclic, uint16_t tableSize, uint32_t taskTime, uint32_t cycleTime,
		PxTask_t CallerTaskId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex, PxTask_t stateTaskId)
{

	boolean_t tableResult = true;
	bool atLeastOneRunnableFired = false;
	RT_safety_t result = SAFENOCHECK;

#ifdef RTE_TIMING_ANALYSIS
	uint8_t cnt = 0;
	for(uint16_t tableIndex = 0; tableIndex < tableSize; tableIndex++)
	{
		if(((cycleTime * (taskTime - (pTableCyclic[tableIndex].offsetTime / cycleTime))) %  pTableCyclic[tableIndex].cyclicTime) == 0)
		cnt++;
	}

	if((RTE_TIME_fillDataFlag == 1) && (cnt > 0))
		RTE_TIME_fillTaskStartInfo((uint8_t)SYSTEM_GetCoreId(), CallerTaskId, 0, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS



	//Loop through the table and check which runnable will be called
	//If a safety runnable returns a not ok result, the corresponding event will be fired to the state task
	//If all safety runnables return an ok result, the event AllOk will be fired to the safety task
	//To avoid aprupt state flips, the state machine has implemented a hysteresis behaviour
	//To be discussed:
	//   - How to deal with runnables having different time bases? Currently, the result of not called runnables is ignored
	for(uint16_t tableIndex = 0; tableIndex < tableSize; tableIndex++)
	{
		if(((cycleTime * (taskTime - (pTableCyclic[tableIndex].offsetTime / cycleTime))) %  pTableCyclic[tableIndex].cyclicTime) == 0)
		{

			//Call the runnable
			if (pTableCyclic[tableIndex].runnable != 0)
			{

#ifdef RTE_TIMING_ANALYSIS
			if(RTE_TIME_fillDataFlag)
				RTE_TIME_fillRunnableStartInfo((uint8_t)SYSTEM_GetCoreId(), CallerTaskId, (RT_runnablePtr_t)pTableCyclic[tableIndex].runnable, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS

				atLeastOneRunnableFired = true;
				RT_safety_t runnableResult = pTableCyclic[tableIndex].runnable();
				switch (runnableResult)
				{
				case SAFEOK :
					if (result > SAFEERROR1)result = SAFEOK; //Do not override SAFEERROR2
					break; //Do nothing
				case SAFEERROR1 :
					PxTaskSignalEvents(stateTaskId, ev_Task_C0_State_11_ErrorRequestError1);
					if (result > SAFEERROR2)result = SAFEERROR1; //Do not override SAFEERROR2
					tableResult = false;
					DET_setFunctionName(pTableCyclic[tableIndex].name);
					break;
				case SAFEERROR2 :
					PxTaskSignalEvents(stateTaskId, ev_Task_C0_State_12_ErrorRequestError2);
					tableResult = false;
					result = SAFEERROR2;
					DET_setFunctionName(pTableCyclic[tableIndex].name);
					break;
				default:
					PxTaskSignalEvents(stateTaskId, ev_Task_C0_State_12_ErrorRequestError2);
					tableResult = false;
					break;
				}
#ifdef RTE_TIMING_ANALYSIS
			if(RTE_TIME_fillDataFlag)
				RTE_TIME_fillRunnableEndInfo((uint8_t)SYSTEM_GetCoreId(), CallerTaskId, (RT_runnablePtr_t)pTableCyclic[tableIndex].runnable, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS

			} //if runnable called

		} //for loop all runnables

		if ((tableResult == true) && (atLeastOneRunnableFired == true))
		{
			//All runnables communicated safety ok
			PxTaskSignalEvents(stateTaskId, ev_Task_C0_State_10_ErrorAllOk);
			asm("nop");
		}

	}
#ifdef RTE_TIMING_ANALYSIS
	//	if(cnt > 0)
	if((RTE_TIME_fillDataFlag == 1) && (cnt > 0))
		RTE_TIME_fillTaskEndInfo((uint8_t)SYSTEM_GetCoreId(), CallerTaskId, 0, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS

	return result;
}



//Todo: add rte timing analysis
RT_safety_t RT_eventRunSafetyExecute(const RT_eventSafetyTable_t*  pTableEvent, uint16_t tableSize, uint32_t eventMask, PxTask_t stateTaskId, PxTask_t senderId, RTE_TIME_taskRingBufferIndex_t rteTimeRBindex)
{

	RT_safety_t result = SAFENOCHECK;
	bool atLeastOneRunnableFired = false;	//Check if at least one runnable has been fired
	boolean_t tableResult = true;			//Check if no event has been fired form the loop. This ensures, that SafeOk is only fired if nor error event has been fired
/*!Added by Shrikant*/
#ifdef RTE_TIMING_ANALYSIS
	uint8_t cnt = 0;
	for(uint16_t tableIndex = 0; tableIndex < tableSize; tableIndex++)
	{
		if(eventMask & pTableEvent[tableIndex].evMask)
			cnt++;
	}
	if((RTE_TIME_fillDataFlag == 1) && (cnt > 0))
		RTE_TIME_fillTaskStartInfo((uint8_t)SYSTEM_GetCoreId(), senderId, 0, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
/*!End*/
	//Loop through all runnables
	for(uint16_t tableIndex = 0; tableIndex < tableSize; tableIndex++)
	{
		//Runnable for the corresponding event has been found
		if (eventMask & pTableEvent[tableIndex].evMask)
		{
			//Memorise that at least on runnables has been fired
			atLeastOneRunnableFired = true;

			//Call the runnable
			if (pTableEvent[tableIndex].runnable != 0)
			{
				/*!Added By Shrikant*/
#ifdef RTE_TIMING_ANALYSIS
			if(RTE_TIME_fillDataFlag)
				RTE_TIME_fillRunnableStartInfo((uint8_t)SYSTEM_GetCoreId(), senderId, (RT_runnablePtr_t)pTableEvent[tableIndex].runnable, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
				/*!End*/
				atLeastOneRunnableFired = true;
				RT_safety_t runnableResult = pTableEvent[tableIndex].runnable();
				switch (runnableResult)
				{
				case SAFEOK :
					if (result > SAFEERROR1) result = SAFEOK; //Do not override SAFEERROR1
					break;

				case SAFEERROR1 :
					PxTaskSignalEvents(stateTaskId, ev_Task_C0_State_11_ErrorRequestError1);
					if (result > SAFEERROR2) result = SAFEERROR1; //Do not override SAFEERROR2
					tableResult = false;
					DET_setFunctionName(pTableEvent[tableIndex].name);
					break;
				case SAFEERROR2 :
					PxTaskSignalEvents(stateTaskId, ev_Task_C0_State_12_ErrorRequestError2);
					tableResult = false;
					DET_setFunctionName(pTableEvent[tableIndex].name);
					break;
				default:
					PxTaskSignalEvents(stateTaskId, ev_Task_C0_State_12_ErrorRequestError2);
					tableResult = false;
					break;
				}
				/*!Added By Shrikant*/
#ifdef RTE_TIMING_ANALYSIS
			if(RTE_TIME_fillDataFlag)
				RTE_TIME_fillRunnableEndInfo((uint8_t)SYSTEM_GetCoreId(), senderId, (RT_runnablePtr_t)pTableEvent[tableIndex].runnable, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
				/*!End*/
			}
		} //if runnable called
	} //loop through all runnables

	if ((tableResult == true) && (atLeastOneRunnableFired == true))
	{
		//All runnables communicated safety ok
		PxTaskSignalEvents(stateTaskId, ev_Task_C0_State_10_ErrorAllOk);
	}
	/*!Added by Shrikant*/
#ifdef RTE_TIMING_ANALYSIS
	if((RTE_TIME_fillDataFlag == 1) && (cnt > 0))
		RTE_TIME_fillTaskEndInfo((uint8_t)SYSTEM_GetCoreId(), senderId, 0, rteTimeRBindex);
#endif //RTE_TIMING_ANALYSIS
	/*!End*/
}

