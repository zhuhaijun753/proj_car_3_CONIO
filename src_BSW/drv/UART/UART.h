/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: UART.h
 * Desc: UART driver for Infineon Tricore Aurix
 ******************************************************************************/

#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "global.h"
#include "register.h"
#include "port.h"

/******************************************************************************/
/*----------------------------------Memory regions----------------------------------*/
/******************************************************************************/
// Begin and End region of UART0 register
#define	PORT_UART_0_BEGIN	((unsigned int)&MODULE_ASCLIN0)
#define	PORT_UART_0_END		((unsigned int)&MODULE_ASCLIN0 + sizeof(Ifx_ASCLIN))
// Begin and End region of UART1 register
#define	PORT_UART_123_BEGIN	((unsigned int)&MODULE_ASCLIN1)
#define	PORT_UART_123_END		((unsigned int)&MODULE_ASCLIN1 + sizeof(Ifx_ASCLIN))
// Begin and End region of UART2 register
#define	PORT_UART_45_BEGIN	((unsigned int)&MODULE_ASCLIN2)
#define	PORT_UART_45_END		((unsigned int)&MODULE_ASCLIN2 + sizeof(Ifx_ASCLIN))
// Begin and End region of UART3 register
#define	PORT_UART_6_BEGIN	((unsigned int)&MODULE_ASCLIN3)
#define	PORT_UART_6_END		((unsigned int)&MODULE_ASCLIN3 + sizeof(Ifx_ASCLIN))

//=====================================================================================

#define UART_CHANNELS_NUM		7

typedef enum			//PORT-PIN TC297B			//Peripheral Application kit
{
	uartusb = 0,			// TX - P14.0  RX - P14.1      MicroUSB
	uart1,				// TX - P20.10 RX - P15.5 	   TX - X102(38) RX - X102(34)
	uart2,				// TX - P02.02 RX - P02.3 	   TX - X103(15) RX - X103(16)
	uart3,				// TX - P33.12 RX - P14.8 	   TX - X102(18) RX - X103(07)
	uart4,				// TX - P02.00 RX - P02.1 	   TX - X103(13) RX - X103(14)
	uart5,				// TX - P33.09 RX - P33.12 	   TX - X103(6)  RX - X102(18)
	uart6				// TX - P00.00 RX - P00.1 	   TX - X103(22) RX - X103(23)
} uartChannel_t;
/**  Type of parity bit attached to data bits
 */
typedef enum
{
    ParEven = 0,  /*  even parity */
    ParOdd  = 1,   /*  odd parity */
	ParNone = 2
} UART_ParityType_t;
/**  Datalength: Number of bits per transfer */
typedef enum
{
    __7 = 6,          /*  number of bits per transfer 7  */
    __8 = 7,          /*  number of bits per transfer 8  */
} UART_DataLength_t;
/**  Defines the filling level that triggers a drain (RX) interrupt or DMA access\n
 */
typedef enum
{
    RxInt_1,     /*  RX FIFO level 1  */
    RxInt_2,     /*  RX FIFO level 2  */
    RxInt_3,     /*  RX FIFO level 3  */
    RxInt_4,     /*  RX FIFO level 4  */
    RxInt_5,     /*  RX FIFO level 5  */
    RxInt_6,     /*  RX FIFO level 6  */
    RxInt_7,     /*  RX FIFO level 7  */
    RxInt_8,     /*  RX FIFO level 8  */
    RxInt_9,     /*  RX FIFO level 9  */
    RxInt_10,    /*  RX FIFO level 10  */
    RxInt_11,    /*  RX FIFO level 11  */
    RxInt_12,    /*  RX FIFO level 12  */
    RxInt_13,    /*  RX FIFO level 13  */
    RxInt_14,    /*  RX FIFO level 14  */
    RxInt_15,    /*  RX FIFO level 15  */
    RxInt_16,    /*  RX FIFO level 16  */
} UART_RxFifoIntLevel_t;
/**  Defines the filling level that triggers a refill (TX) interrupt
 */
typedef enum
{
    TxInt_0,     /*  TX FIFO level 0  */
    TxInt_1,     /*  TX FIFO level 1  */
    TxInt_2,     /*  TX FIFO level 2  */
    TxInt_3,     /*  TX FIFO level 3  */
    TxInt_4,     /*  TX FIFO level 4  */
    TxInt_5,     /*  TX FIFO level 5  */
    TxInt_6,     /*  TX FIFO level 6  */
    TxInt_7,     /*  TX FIFO level 7  */
    TxInt_8,     /*  TX FIFO level 8  */
    TxInt_9,     /*  TX FIFO level 9  */
    TxInt_10,    /*  TX FIFO level 10  */
    TxInt_11,    /*  TX FIFO level 11  */
    TxInt_12,    /*  TX FIFO level 12  */
    TxInt_13,    /*  TX FIFO level 13  */
    TxInt_14,    /*  TX FIFO level 14  */
    TxInt_15,    /*  TX FIFO level 15  */
} UART_TxFifoIntLevel_t;


/** @HD: CTS Enable bit
 */
typedef enum
{
	CtsDisable = 0,
	CtsEnable
} UART_CtsEnable_t;

/** @HD: CTS Polarity configuration
 */
typedef enum
{
	HighActive = 0,
	LowActive
} UART_CtsPolarity_t;

/**  Number of stop bits
 */
typedef enum
{
    StopBit_1 = 1,     /*  number of stop bits 1  */
    StopBit_2 = 2,     /*  number of stop bits 2  */
} UART_StopBit_t;
/**  Structure for baudrate
 */
typedef enum
{
    uart_9600 	= 9600,
    uart_19200 	= 19200,
    uart_38400	= 38400,
    uart_57600 	= 57600,
    uart_115200 = 115200
} UART_Baud_t;


/**  Configuration structure of the module
 */
typedef struct
{
    UART_Baud_t         		baudrate;           /*  structure for baudrate */
    UART_DataLength_t     		dataLength;         /*  DATCON.DATALENGTH, data length, number of bits per transfer */
    UART_StopBit_t        		stopBit;            /*  FRAMECON.STOP, number of stop bits */
    UART_ParityType_t     		parityType;         /*  FRAMECON.ODD, parity type (even or odd) */
    UART_CtsEnable_t			ctsEnable;			/*  IOCR.CTSEN CTS Enable*/
    UART_CtsPolarity_t			ctsPolarity;			/*  IOCR.RCPOL CTS polarity (HighActive or LowActive)*/
    UART_TxFifoIntLevel_t 		txFifoInterruptLevel;     /*  TXFIFOCON.INTLEVEL, Tx FIFO interrupt level */
    UART_RxFifoIntLevel_t 		rxFifoInterruptLevel;     /*  RXFIFOCON.INTLEVEL, Rx FIFO interrupt level */
} uartConfig_t;


/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/
/** Initialises the UART
 */
void UART_init();
/*
 * reads data from Rx register of the corresponding channel
 * @param: channel		uart channel number
 * @param: data			data received from the UART register
 * @return:	RC_t		RC_SUCCESS				: Transmission is successfu
 * 						RC_ERROR_BUFFER_EMTPY	: No data to read
 * 						RC_ERROR_READ_FAILS		: UART channel is disabled
 */
RC_t UART_ReadData(uartChannel_t channel, uint8_t* data);

/*
 * Writes data to Tx register of the corresponding channel
 * @param:	channel		uart channel number
 * @param:	data		data to be transmitted
 * @return:	RC_t		RC_SUCCESS				: Transmission is successfu
 * 						RC_ERROR_BUFFER_FULL	: TxFIFO is full
 * 						RC_ERROR_WRITE_FAILS	: UART channel is disabled
 */
RC_t UART_WriteData(uartChannel_t channel, uint8_t data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UART_H */
