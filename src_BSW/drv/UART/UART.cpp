/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: UART.c
 * Desc: UART driver for Infineon Tricore Aurix
 ******************************************************************************/

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "UART.h"
#include "UART_config.h"
//#include "SRC.h"
#include "system.h"
#include "system_configuration.h"
//#include "InterruptRouter.h"
#include "clock.h"

/******************************************************************************/
/*----------------------------------Global typedefs----------------------------------*/
/******************************************************************************/
/**  Selection of clock source
 */
typedef enum {
	UART_ClockSource_noClock = 0, /*  no clock will be supplied */
	UART_ClockSource_kernelClock = 1, /*  fclc will be supplied */
	UART_ClockSource_oscillatorClock = 2, /*  XTAL oscillator clock foso0 will be supplied */
	UART_ClockSource_flexRayClock = 4, /*  feray will be supplied */
	UART_ClockSource_ascFastClock = 8, /*  fUARTf wiil be supplied */
	UART_ClockSource_ascSlowClock = 16, /*  fUARTs will be supplied */
} UART_ClockSource_t;

/**  Defines the basic operating mode of the module. Changing the mode must be done by switching first to initialize mode, and then to the other mode.
 */
typedef enum {
	UART_FrameMode_initialise = 0, /*  initialise mode */
	UART_FrameMode_asc = 1, /*  asc mode */
	UART_FrameMode_spi = 2, /*  spi mode */
	UART_FrameMode_lin = 3 /*  lin mode */
} UART_FrameMode_t;

/**  Defines the duration of the IDLE delay in bit times.
 * This is the pause inserted between transmission of bytes.
 */
typedef enum {
	UART_IdleDelay_0, /*  idle delay in 0 bit times  */
	UART_IdleDelay_1, /*  idle delay in 1 bit times  */
	UART_IdleDelay_2, /*  idle delay in 2 bit times  */
	UART_IdleDelay_3, /*  idle delay in 3 bit times  */
	UART_IdleDelay_4, /*  idle delay in 4 bit times  */
	UART_IdleDelay_5, /*  idle delay in 5 bit times  */
	UART_IdleDelay_6, /*  idle delay in 6 bit times  */
	UART_IdleDelay_7, /*  idle delay in 7 bit times  */
} UART_IdleDelay_t;

/**  Defines the bit length in ticks in the range of 1 to 16. The lengths of 1 to 3 are not allowed
 */
typedef enum {
	UART_OversamplingFactor_4 = 3, /*  oversampling factor 4 */
	UART_OversamplingFactor_5 = 4, /*  oversampling factor 5 */
	UART_OversamplingFactor_6 = 5, /*  oversampling factor 6 */
	UART_OversamplingFactor_7 = 6, /*  oversampling factor 7 */
	UART_OversamplingFactor_8 = 7, /*  oversampling factor 8 */
	UART_OversamplingFactor_9 = 8, /*  oversampling factor 9 */
	UART_OversamplingFactor_10 = 9, /*  oversampling factor 10 */
	UART_OversamplingFactor_11 = 10, /*  oversampling factor 11 */
	UART_OversamplingFactor_12 = 11, /*  oversampling factor 12 */
	UART_OversamplingFactor_13 = 12, /*  oversampling factor 13 */
	UART_OversamplingFactor_14 = 13, /*  oversampling factor 14 */
	UART_OversamplingFactor_15 = 14, /*  oversampling factor 15 */
	UART_OversamplingFactor_16 = 15 /*  oversampling factor 16 */
} UART_OversamplingFactor_t;

/**  Defines the number of bytes read from the Rx FIFO with one FPI bus read\n
 */
typedef enum {
	UART_RxFifoOutletWidth_0, /*  number of bytes 0  */
	UART_RxFifoOutletWidth_1, /*  number of bytes 1  */
	UART_RxFifoOutletWidth_2, /*  number of bytes 2  */
	UART_RxFifoOutletWidth_3, /*  number of bytes 3  */
} UART_RxFifoOutletWidth_t;

/**  Sample point position
 */
typedef enum {
	UART_SamplePointPosition_1 = 1, /*  sample point position at 1 */
	UART_SamplePointPosition_2 = 2, /*  sample point position at 2 */
	UART_SamplePointPosition_3 = 3, /*  sample point position at 3 */
	UART_SamplePointPosition_4 = 4, /*  sample point position at 4 */
	UART_SamplePointPosition_5 = 5, /*  sample point position at 5 */
	UART_SamplePointPosition_6 = 6, /*  sample point position at 6 */
	UART_SamplePointPosition_7 = 7, /*  sample point position at 7 */
	UART_SamplePointPosition_8 = 8, /*  sample point position at 8 */
	UART_SamplePointPosition_9 = 9, /*  sample point position at 9 */
	UART_SamplePointPosition_10 = 10, /*  sample point position at 10 */
	UART_SamplePointPosition_11 = 11, /*  sample point position at 11 */
	UART_SamplePointPosition_12 = 12, /*  sample point position at 12 */
	UART_SamplePointPosition_13 = 13, /*  sample point position at 13 */
	UART_SamplePointPosition_14 = 14, /*  sample point position at 14 */
	UART_SamplePointPosition_15 = 15 /*  sample point position at 15 */
} UART_SamplePointPosition_t;

/**  Number of samples per bit
 */
typedef enum {
	UART_SamplesPerBit_one = 0, /*  one sample per bit */
	UART_SamplesPerBit_three = 1 /*  three samples per bit */
} UART_SamplesPerBit_t;

/**  Defines the number of bytes written to the Tx FIFO with one FPI bus write
 */
typedef enum {
	UART_TxFifoInletWidth_0, /*  number of bytes 0  */
	UART_TxFifoInletWidth_1, /*  number of bytes 1  */
	UART_TxFifoInletWidth_2, /*  number of bytes 2  */
	UART_TxFifoInletWidth_3, /*  number of bytes 3  */
} UART_TxFifoInletWidth_t;

typedef struct {
	bool Enable;
	volatile Ifx_ASCLIN* module; /*  Base address */
	RxSelect_t RxMux; /*  RX Port control */
	RxSelect_t CtsMux; /*  CTS Port Control */
	volatile Ifx_SRC_SRCR* SrcReg[3];
} UART_t;
/******************************************************************************/
/*----------------------------- Global data ----------------------------------*/
/******************************************************************************/
const UART_t UART_tbl[UART_CHANNELS_NUM] =
		{
#if (ASCLIN0 == UARTUSB)
				{ TRUE, &MODULE_ASCLIN0, RxSel_a, RxSel_a, { &SRC_ASCLIN0RX,
						&SRC_ASCLIN0TX, &SRC_ASCLIN0ERR } },
#else
				{	FALSE,0,(RxSelect_t)0, (RxSelect_t) 0, {0,0,0}},
#endif

#if (ASCLIN1 == UART1)
				{	TRUE, &MODULE_ASCLIN1, RxSel_b, RxSel_a, {&SRC_ASCLIN1RX, &SRC_ASCLIN1TX,&SRC_ASCLIN1ERR}},
#else
				{ FALSE, 0, (RxSelect_t) 0, (RxSelect_t) 0, { 0, 0, 0 } },
#endif
#if (ASCLIN1 == UART2)
				{ TRUE, &MODULE_ASCLIN1, RxSel_g, RxSel_a, { &SRC_ASCLIN1RX,
						&SRC_ASCLIN1TX, &SRC_ASCLIN1ERR } },
#else
				{	FALSE,0,(RxSelect_t)0, (RxSelect_t) 0, {0,0,0}},
#endif
#if (ASCLIN1 == UART3)
				{	TRUE, &MODULE_ASCLIN1, RxSel_d, RxSel_a, {&SRC_ASCLIN1RX, &SRC_ASCLIN1TX,&SRC_ASCLIN1ERR}},
#else
				{ FALSE, 0, (RxSelect_t) 0, (RxSelect_t) 0, { 0, 0, 0 } },
#endif
#if (ASCLIN2 == UART4)
				{ TRUE, &MODULE_ASCLIN2, RxSel_b, RxSel_a, { &SRC_ASCLIN2RX,
						&SRC_ASCLIN2TX, &SRC_ASCLIN2ERR } },
#else
				{	FALSE,0,(RxSelect_t)0, (RxSelect_t) 0, {0,0,0}},
#endif
#if (ASCLIN2 == UART5)
				{	TRUE,&MODULE_ASCLIN2, RxSel_e, RxSel_a, {&SRC_ASCLIN2RX, &SRC_ASCLIN2TX,&SRC_ASCLIN2ERR}},
#else
				{ FALSE, 0, (RxSelect_t) 0, (RxSelect_t) 0, { 0, 0, 0 } },
#endif
#if (ASCLIN3 == UART6)
				{ TRUE, &MODULE_ASCLIN3, RxSel_e, RxSel_a, { &SRC_ASCLIN3RX,
						&SRC_ASCLIN3TX, &SRC_ASCLIN3ERR } },
#else
		{	FALSE,0,(RxSelect_t)0, (RxSelect_t) 0, {0,0,0}}
#endif
	};

#include "UART_config.h"

/******************************************************************************/
/*---------------------------------- Macros ----------------------------------*/
/******************************************************************************/

#define PRESCALAR_FACTOR 		10

#define HARDCODED_SAMPLEPOINT_CONFIG 	UART_SamplePointPosition_8

#define FIXED_UART_DENOMINATOR 		(0x0C35u)

#define UART_FIFO_MAXLEVEL			16
#define UART_FIFO_MINLEVEL			0

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** Return minimum of two unsigned integers
 */
INLINE uint32_t UART__minu(uint32_t a, uint32_t b) {
	uint32_t res;
	__asm__ volatile ("min.u %0, %1, %2": "=d" (res) : "d" (a), "d" (b));
	return res;
}

/*
 * Function to set clock source
 * @param	uartreg		Uart channel register
 * @param	Clocksource	clock source to be set
 * @return	None
 */
static void UART__setClockSource(Ifx_ASCLIN *uartreg,
		UART_ClockSource_t clockSource);
/*
 * Function to set bit timing and baud rate
 * @param	uartreg			uart channel register
 * @param	baudrate		baudrate to be defined
 * @param	oversampling	Oversampling
 * @param	samplepoint		number of points to be sampled
 * @param	medianFilter	median filter
 * return	None
 */
static void UART__setBitTiming(Ifx_ASCLIN *uartreg, UART_Baud_t baudrate,
		UART_OversamplingFactor_t oversampling,
		UART_SamplePointPosition_t samplepoint,
		UART_SamplesPerBit_t medianFilter);

/**************************************************/
/* ====            Static functions           ==== */
/**************************************************/

/*
 * Function to set clock source
 * @param	uartreg		Uart channel register
 * @param	Clocksource	clock source to be set
 * @return	None
 */
static void UART__setClockSource(Ifx_ASCLIN *uartreg,
		UART_ClockSource_t clockSource) {
	uartreg->CSR.B.CLKSEL = clockSource; /* selects the given clock source*/

	/* Waits TW or polls for CSR.CON to change */
	if (clockSource == UART_ClockSource_noClock) {
		while (uartreg->CSR.B.CON != 0U) {
		}
	} else {
		while (uartreg->CSR.B.CON != 1U) {
		}
	}
}
/*
 * Function to set bit timing and baud rate
 * @param	uartreg			uart channel register
 * @param	baudrate		baudrate to be defined
 * @param	oversampling	Oversampling
 * @param	samplepoint		number of points to be sampled
 * @param	medianFilter	median filter
 * return	None
 */
static void UART__setBitTiming(Ifx_ASCLIN *uartreg, UART_Baud_t baudrate,
		UART_OversamplingFactor_t oversampling,
		UART_SamplePointPosition_t samplepoint,
		UART_SamplesPerBit_t medianFilter) {
	// Baud rate calculation formula
	/*				   system_frequncy * Numerator
	 * Baud rate = ------------------------------------
	 * 				(PRESCALAR+1) * Denominator * (Oversampling+1)
	 */
	float64_t temp = 0;
	uint32_t sysclk;
	//Get current clock source
	UART_ClockSource_t source = (UART_ClockSource_t) uartreg->CSR.B.CLKSEL;

	// Assign no clock
	UART__setClockSource(uartreg, UART_ClockSource_noClock);

	// Load Fixed denominator value
	uartreg->BRG.B.DENOMINATOR = FIXED_UART_DENOMINATOR;

	temp = baudrate * FIXED_UART_DENOMINATOR;
	sysclk = Clock_get_Frequency(fSPB);
	temp /= sysclk;
	temp *= (PRESCALAR_FACTOR * (oversampling + 1));

	// Load numerator based on baudrate selected
	uartreg->BRG.B.NUMERATOR = (uint32_t) temp;

	// Set the Oversampling rate
	uartreg->BITCON.B.OVERSAMPLING = oversampling;

	// Set the sampling point
	uartreg->BITCON.B.SAMPLEPOINT = samplepoint;

	// Set the median filter
	uartreg->BITCON.B.SM = medianFilter ? 1 : 0;

	// Reconfigure clock souce as the current one
	UART__setClockSource(uartreg, source);

}

/*
 * Init uart modules
 * @param: channel Uart channel number
 * @return: void
 * \warning This function will only enable the interrupt bits if there is an entry in the interrupt table
 */
static void UART__initModule(uartChannel_t channel) {
	/* pointer to UART registers*/
	Ifx_ASCLIN *UARTReg = UART_tbl[channel].module;

	SYSTEM_UnlockENDINIT(); /* clears the endinit protection*/
	UARTReg->CLC.B.DISR = 0; /* enables the module*/
	SYSTEM_LockENDINIT(); /* sets the endinit protection back on*//* enabling the module*/

	UART__setClockSource(UARTReg, UART_ClockSource_noClock); /* disabling the clock*/
	UARTReg->FRAMECON.B.MODE = UART_FrameMode_initialise; /* setting the module in Initialise mode*/
	UARTReg->BITCON.B.PRESCALER = (uint16_t) PRESCALAR_FACTOR - 1; /* sets the prescaler */

	UART__setClockSource(UARTReg, UART_ClockSource_kernelClock); /* temporary set the clock source for baudrate configuration*/
	UART__setBitTiming(UARTReg, /* setting the baudrate bit fields to generate the required baudrate*/
	UART_Config[channel].baudrate, UART_OversamplingFactor_16,
	HARDCODED_SAMPLEPOINT_CONFIG, UART_SamplesPerBit_three);
	UART__setClockSource(UARTReg, UART_ClockSource_noClock); /* disabling the clock again*/
	// Loob back mode
	UARTReg->IOCR.B.LB = FALSE;
	/* setting parity type (odd/even)*//* diaable the loopback mode */
	if (UART_Config[channel].parityType != ParNone) {
		UARTReg->FRAMECON.B.PEN = TRUE;
		UARTReg->FRAMECON.B.ODD = UART_Config[channel].parityType;
	} else {
		UARTReg->FRAMECON.B.PEN = FALSE;/* disabling parity */
	}

	UARTReg->FRAMECON.B.STOP = UART_Config[channel].stopBit; /* setting the stop bit */
	UARTReg->DATCON.B.DATLEN = UART_Config[channel].dataLength; /* setting the data length */
	UARTReg->TXFIFOCON.B.INW = UART_TxFifoInletWidth_1; /* setting Tx FIFO inlet width */
	UARTReg->RXFIFOCON.B.OUTW = UART_RxFifoOutletWidth_1; /* setting Rx FIFO outlet width */
	UARTReg->FRAMECON.B.IDLE = UART_IdleDelay_7; /* setting idle delay */
	UARTReg->TXFIFOCON.B.INTLEVEL = UART__minu(
			UART_Config[channel].txFifoInterruptLevel, 15); /* setting Tx FIFO level at which a Tx interrupt will be triggered*/
	UARTReg->RXFIFOCON.B.INTLEVEL = UART__minu(
			UART_Config[channel].rxFifoInterruptLevel, 15); /* setting Rx FIFO interrupt level at which a Rx interrupt will be triggered*/
	UARTReg->FRAMECON.B.MODE = UART_FrameMode_asc; /* selecting the frame mode*/

	UARTReg->IOCR.B.ALTI = UART_tbl[channel].RxMux; /* Receiver Pin mapping */
	UARTReg->IOCR.B.CTS = UART_tbl[channel].CtsMux; /* @HD: CTS pin mapping */
	UARTReg->IOCR.B.RCPOL = UART_Config[channel].ctsPolarity; /*  @HD: CTS polarity*/
	UARTReg->IOCR.B.CTSEN = UART_Config[channel].ctsEnable; /*  @HD: CTS enabling*/

	UART__setClockSource(UARTReg, UART_ClockSource_kernelClock); /* select the clock source*/

	UARTReg->FLAGSENABLE.U = 0x00000000; /* disable all flags */
	UARTReg->FLAGSCLEAR.U = 0xFFFFFFFF; /* clear all flags */

	//Enable Error flags
	UARTReg->FLAGSENABLE.B.PEE = TRUE;
	UARTReg->FLAGSENABLE.B.FEE = TRUE;
	UARTReg->FLAGSENABLE.B.RFOE = TRUE;
	UARTReg->FLAGSENABLE.B.RFUE = TRUE;
	UARTReg->FLAGSENABLE.B.TFOE = TRUE;

	/* initialize the interrupts */
	UARTReg->FLAGSENABLE.B.RFLE = TRUE;
	UARTReg->FLAGSENABLE.B.TFLE = TRUE;

	UARTReg->RXFIFOCON.B.ENI = TRUE;  		// enabling Rx FIFO for receiving
	UARTReg->TXFIFOCON.B.ENO = TRUE; 		// enabling Tx FIFO for transmitting

	UARTReg->RXFIFOCON.B.FLUSH = 1;              			// flushing Rx FIFO
	UARTReg->TXFIFOCON.B.FLUSH = 1;              			// flushing Tx FIFO

}
/**************************************************/
/* ====            Global functions          ==== */
/**************************************************/
/** Initialises the UART
 */
void UART_init() {
	volatile uint8_t loop = 0;
	//Loop through all channels active status
	for (loop = 0; loop < UART_CHANNELS_NUM; loop++) {
		//If a particular channel is active
		if (UART_tbl[loop].Enable) {
			// Initialize the channel
			UART__initModule((uartChannel_t) loop);
		}
	}
}

/*
 * Writes data to Tx register of the corresponding channel
 * @param:	channel		uart channel number
 * @param:	data		data to be transmitted
 * @return:	RC_t		RC_SUCCESS				: Transmission is successfu
 * 						RC_ERROR_BUFFER_FULL	: TxFIFO is full
 * 						RC_ERROR_WRITE_FAILS	: UART channel is disabled
 */
RC_t UART_WriteData(uartChannel_t channel, uint8_t data) {
	if (UART_tbl[channel].Enable == TRUE) {
		if (UART_tbl[channel].module->TXFIFOCON.B.FILL < UART_FIFO_MAXLEVEL) {
			UART_tbl[channel].module->TXDATA.U = data;
			return RC_SUCCESS;
		}
		return RC_ERROR_BUFFER_FULL;
	}
	return RC_ERROR_WRITE_FAILS;
}
/*
 * reads data from Rx register of the corresponding channel
 * @param: channel		uart channel number
 * @param: data			data received from the UART register
 * @return:	RC_t		RC_SUCCESS				: Transmission is successfu
 * 						RC_ERROR_BUFFER_EMTPY	: No data to read
 * 						RC_ERROR_READ_FAILS		: UART channel is disabled
 */
RC_t UART_ReadData(uartChannel_t channel, uint8_t* data) {
	if (UART_tbl[channel].Enable == TRUE) {
		if (UART_tbl[channel].module->RXFIFOCON.B.FILL > UART_FIFO_MINLEVEL) {
			*data = UART_tbl[channel].module->RXDATA.U;
			return RC_SUCCESS;
		}
		*data = 0;
		return RC_ERROR_BUFFER_EMTPY;
	}
	return RC_ERROR_READ_FAILS;
}

