/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: DTS.c
 * Desc: Die temperature driver
 ******************************************************************************/

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "DTS.h"

/******************************************************************************/
/*---------------------------------- MACROS ----------------------------------*/
/******************************************************************************/

#define MIN_DIE_TEMPERATURE 	(-40.0)
#define MAX_DIE_TEMPERATURE 	( 60.0)


/******************************************************************************/
/*-----------------------------Global Variables-------------------------------*/
/******************************************************************************/
#pragma section ".data.peripheral"

float32_t DieTemperature;

#pragma section
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/**************************************************/
/* ==== Inline functions for register access ==== */
/**************************************************/
#pragma section ".text.DTS" ax

INLINE void DTS_disableSensorControl(void)
{
    MODULE_SCU.DTSCON.B.SLCK = 1;
    MODULE_SCU.DTSLIM.B.SLCK = 1;
}

INLINE void DTS_enableSensor(void)
{
    MODULE_SCU.DTSCON.B.PWD = 0;
}

INLINE volatile Ifx_SRC_SRCR *DTS_getSrcPointer(void)
{
    return &SRC_SCUDTS;
}

INLINE uint16_t DTS_getTemperatureValue(void)
{
    return (uint16_t)MODULE_SCU.DTSSTAT.B.RESULT;
}

INLINE boolean_t DTS_isBusy(void)
{
    return MODULE_SCU.DTSSTAT.B.BUSY == 1 ? TRUE : FALSE;
}


/**************************************************/
/* ====            Local functions           ==== */
/**************************************************/

/** Converts a temperature value in Celsius to DTS value
 * @param: temperatureValue the temperature in Celsius
 * @return: the appr. DTS value
 */
static uint16_t DTS_convertFromCelsius(float32_t temperatureValue)
{
	//Convert temperature to DTS
    sint32_t DTSValue = (sint32_t)((temperatureValue + 285.5) / 0.467);

    // Adjust value calculated to the range
    if (DTSValue < 0)
    {
        DTSValue = 0;
    }
    else if (DTSValue > 1023)
    {
        DTSValue = 1023;
    }

    return (uint16_t)DTSValue;
}

/** Converts the measurement value returned from DTS to Celsius
 * @param: DTSValue measurement value returned from DTS
 * @return: temperature in Celsius
 */
static float32_t DTS_convertToCelsius(uint16_t DTSValue)
{
    return ((float32_t)DTSValue * 0.467) - 285.5;
}

/** Returns the converted temperature in Celsius
 * @return: The temperature based on the DTS temperature value
 */
static float32_t DTS_getTemperatureCelsius(void)
{
    return DTS_convertToCelsius(DTS_getTemperatureValue());
}

/**************************************************/
/* ====            Global functions          ==== */
/**************************************************/

RC_t DTS_initModule()
{
    DTS_enableSensor();

    /* wait for two measurements before enabling the limits */
    {
        int i;

        /* disable limits */
        MODULE_SCU.DTSLIM.B.LOWER = 0;
        MODULE_SCU.DTSLIM.B.UPPER = 1023;

        /* wait until DTS is ready */
        while (!DTS_isReady())
        {}

        /* two dummy measurements */
        for (i = 0; i < 2; ++i)
        {
        	DTS_startSensor();

            while (DTS_isBusy())
            {}
        }
    }

    /* change to the requested limits */
    MODULE_SCU.DTSLIM.B.LOWER = DTS_convertFromCelsius(MIN_DIE_TEMPERATURE);
    MODULE_SCU.DTSLIM.B.UPPER = DTS_convertFromCelsius(MAX_DIE_TEMPERATURE);

    /* lock configuration */

    DTS_disableSensorControl();

    /* enable DTS IRQ */
    volatile Ifx_SRC_SRCR *src = DTS_getSrcPointer();
    //SRC_init(src, cpu0, ISR_DTS_PRIO);
    //SRC_enable(src);
    src->B.SRPN = ISR_DTS_PRIO; //Set up the priority
    src->B.TOS = cpu0; //Select the core
    src->B.CLRR = 1; //Clear pending request
    src->B.SRE = 1; //Enable the SRN

    return RC_SUCCESS;
}

void DTS_startSensor(void)
{
    MODULE_SCU.DTSCON.B.START = 1;
}

boolean_t DTS_isReady(void)
{
    return MODULE_SCU.DTSSTAT.B.RDY == 1 ? TRUE : FALSE;
}

void DTS_isr(void)
{
	volatile Ifx_SRC_SRCR *src = DTS_getSrcPointer();
	// Update global temperature variable with sensor data
	DieTemperature = DTS_getTemperatureCelsius();
	src->B.CLRR = 1; //Clear pending request
}

#pragma section
