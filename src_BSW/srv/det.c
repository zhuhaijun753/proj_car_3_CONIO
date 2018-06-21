/*
 * det.c
 *
 *  Created on: May 14, 2014
 *      Author: fromm
 */

#include <machine/intrinsics.h>
#include "register.h"

#include "det.h"
#include "det_time.h"


/********************************************[ Data structures ]*****************************************/

//Data structure for storing the error data (BLOCKED mode)
typedef struct {
	DET_message_t m_message;
	unsigned int m_uservalue;

} DET__info_t;

/********************************************[ file local data ]*****************************************/

#pragma section ".DET.data"														// Read / write data

static volatile DET__info_t DET__crashData[3] = {{EMPTY,0}};					// Data structure for error message (BLOCKED mode)
static char* DET__functionName = 0;												// Name of the last safety function

static boolean_t DET_turnOffEngine = FALSE;										// State variable ensuring the engines to be turned off in case of fata error

static volatile DET__infoarray_t DET__buffer[DET_ARRAYSIZE]= {{0,NULL,0,0}};	//Data structure for error message (REPORT mode)
static volatile uint16_t DET__age = 0;											//Age of the last error message added (to control update of display)
static volatile uint16_t DET__readindex 	= 0;								//Next position of array to read
static volatile uint16_t DET__fill_level	= 0;								//Next free position of array


volatile int DET___protection1;
volatile int DET___protection2;

#pragma section

#pragma section ".rodata"														//const read only data

const char * const DET__MESSAGESTRING[] = {										//string representations of the enums
		DET_MSG_DEF(DET_DEF_ENUM_STR)
};

const DET_mode_t DET_ERRMODE[] = {												//Blocked or report mode of the enums
		DET_MSG_DEF(DET_DEF_ENUM_MODE)
};



#pragma section

/********************************************[ function implementations ]*****************************************/

/*
 * The DET_stop will fill the Message into a buffer
 * If the Mode corresponding to message is BLOCKED the data is filled to the buffer and the core halts means the error is critical
 * If the Mode corresponding to message is REPORT the data is only filled into the buffer
 * Once the buffer is full the core is halted.
 * Fill level is to indicate how much data is present buffer with respect to the DISPLAYSIZE as the DISPLACESIZE represents the 'Last N'
 * 			updated Errors as DISPLAYSIZE is limited to fit on a small screen
 *
 * @param core The core which is currently being executed
 * @param DET_message Error message of the DET module
 * @param uservalue Error message from the user
 */
void DET_stop(unsigned int core, DET_message_t message, unsigned int uservalue)
{



	if (core == AUTOCORE)
	{
		core = __MFCR(CPU_CORE_ID);
	}

	if((DET__fill_level != DET_ARRAYSIZE))  //Copy valid Core info , message and uservalue into DET__crashArray
	{
		//Reporting block
		//DET__fill_level = ((DET__fill_level++) >= (DET_DISPLAYSIZE)) ? (DET_DISPLAYSIZE) : DET__fill_level; // till the buffer is not full once, fill_level indicates how much valid data is present in the buffer
		DET__buffer[DET__fill_level].core = (uint8_t)core;
		DET__buffer[DET__fill_level].m_message = message;
		DET__buffer[DET__fill_level].m_uservalue = uservalue;
		DET__buffer[DET__fill_level].m_timestamp = DET_TIME_get2();//DET__fill_level;//RTE_TIME_GetTime();	//RTE_TIME_getTime gets the time stamp in terms of tick; 1 tick=.647us
		DET__fill_level = (DET__fill_level+1);    			//Increment the write index modulus is applied for the round over feature of the index

		DET__age = 0;

	}
	else
	{
		//Buffer is full
		//Todo: Maybe an overwrite of the error messages makes more sense?
		DET_turnOffEngine = TRUE;
		//asm("debug");
	}
	if (DET_ERRMODE[message]== BLOCKED)
	{
		//Critical error, let's stop the car
		DET_turnOffEngine = TRUE;
		//asm("debug");

	}

}


/**
 * Will store the adress of a given Function name
 */
extern void DET_setFunctionName(const char* data)
{
		DET__functionName = data;
}

/**
 * Will return the string representing the name of a function, typically a safety function
 */
extern const char* DET_getFunctionName()
{
	return DET__functionName;
}



/**
 * Returns the fill level of the buffer
 */
uint32_t DET_getFillLevel()
{
		return DET__fill_level;
}

/**
 * returns if DET was updated after the last read operation
 */
boolean_t DET_isNew()
{
		return (DET__age == 0) ? TRUE : FALSE;
}

/**
 * returns if DET was updated after the last read operation
 */
RC_t DET_clr_age()
{
		DET__age = 0;
		return RC_SUCCESS;
}

/*
 *
 * get_first takes the first value from the updated series of data
 * Initialised read_index to startindex which points to the first value to be read
 *
 * @param ArrayDump Used to return the the first entity of the list into a Temporary data structure
 */
RC_t DET_get_first(DET__dumpinfoarray_t* ArrayDump)
{
		DET__readindex = 0;

		return DET_read(ArrayDump, NULL);
}

/*
 *
 * get the next value from the buffer
 * @param ArrayDump Used to return the next entity of the list into a Temporary data structure
 */
RC_t DET_get_next(DET__dumpinfoarray_t* ArrayDump, sint16_t *apIdx)
{
		return DET_read(ArrayDump, apIdx);
}

/*
 *
 * read one data structure from the set of latest updated value
 * This should be done only after on read cycle not to run into inconsistent behaviour
 * 	@param ArrayDump Used to return the  entity from the buffer into a Temporary data structure
 */
RC_t DET_read(DET__dumpinfoarray_t* ArrayDump, sint16_t *apIdx)
{

	DET__age++;

	//if ((DET__readindex != (DET_startindex+DET__fill_level))&&(DET__fill_level!=0))		// copy data from the buffer to the Dump array
	if (DET__readindex < DET__fill_level)
	{
		 ArrayDump->core = DET__buffer[DET__readindex].core;
		 ArrayDump->mp_message = DET__MESSAGESTRING[DET__buffer[DET__readindex].m_message]; // convert the det_message enum to a string for display
		 ArrayDump->m_uservalue = DET__buffer[DET__readindex].m_uservalue;
		 ArrayDump->m_timestamp = DET__buffer[DET__readindex].m_timestamp;//(uint32_t)((((float32_t)DET__buffer[(DET__readindex)].m_timestamp+1)/(float32_t)1000)*0.647);  // convert the ticks to micro seconds as one tick corresponds to 0.674 us

		 if (NULL != apIdx)
			 *apIdx = DET__readindex;

		 DET__readindex = (DET__readindex+1);

		 return RC_SUCCESS;
	 }

	 return RC_ERROR_BUFFER_EMTPY;
}

/*
 *
 * read one data entry by index
 *  @prerequisite - use DET_getFillLevel API for the numer of entries present
 * 	@param ArrayDump Used to return the  entity from the buffer into a Temporary data structure
 * 	@param anIdx specifies the index to be accessed
 */
RC_t DET_readByIndex(DET__dumpinfoarray_t* ArrayDump, sint16_t anIdx)
{
	//incrementing age on every read
	DET__age++;

	if ((anIdx < 0) || (anIdx >= DET__fill_level))
		return RC_ERROR_RANGE;

	ArrayDump->core = DET__buffer[anIdx].core;
	ArrayDump->mp_message = DET__MESSAGESTRING[DET__buffer[anIdx].m_message]; // convert the det_message enum to a string for display
	ArrayDump->m_uservalue = DET__buffer[anIdx].m_uservalue;
	ArrayDump->m_timestamp = DET__buffer[anIdx].m_timestamp;//(uint32_t)((((float32_t)DET__buffer[anIdx].m_timestamp+1)/(float32_t)1000)*0.647);  // convert the ticks to micro seconds as one tick corresponds to 0.674 us

	return RC_SUCCESS;
}



/**
 * Check if the flag for turning off the engines has been set
 */
boolean_t DET_ShutDownEngines()
{
	//To make sure that command is processed only once
	boolean_t result = DET_turnOffEngine;
	DET_turnOffEngine = FALSE;

	return result;
}

