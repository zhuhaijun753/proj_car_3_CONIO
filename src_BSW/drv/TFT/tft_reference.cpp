/**
 * \file	tft_reference.cpp
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	07.05.2017
 * \version	0.1
 *
 * \brief TFT driver reference implememtation
 *
 * Changelog:\n
 *
 * \copyright Copyright ©2016
 * Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
 * (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
 * this paragraph and the following paragraph appear in all copies, modifications, and distributions.
 * Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
 *
 * \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
 * The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
 * This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
 */

#include "QSPI.h"
#include "PORT.h"
#include "system.h"

#include "tft_reference.h"

/** \brief TFT QSPI clock pin	*/
#define QSPI0_SCLK_PIN		Port20_Pin11

/** \brief TFT Driver */
typedef struct
{
	QSPI_channel_t 		mChannel;										/**< TFT QSPI channel */
	uint16_t			mnID;											/**< TFT ID */
	uint16_t			mnScratchBuf[TFT_XSIZE * (TFT_YSIZE >> 3)];		/**< Scratch buffer */
	uint16_t			mnScratchBufLen;								/**< Scratch buffer length */
	uint16_t			mnScratchBufPitch;								/**< Scratch buffer pitch */
	RC_t				mError;											/**< Error */

	union
	{
		struct
		{
			uint8_t 	D0:1;
			uint8_t 	D1:1;
			uint8_t 	D2:1;
			uint8_t 	D3:1;
			uint8_t 	D4:1;
			uint8_t 	D5:1;
			uint8_t 	D6:1;
			uint8_t 	D7:1;
		}bits;

		uint8_t			byte;
	}mMADCTL;

	TFT_orientation_t mOrientation;
}TFT_OnBoard_Driver_t;

#pragma section ".data.tft"
/** \brief TFT Driver handle */
TFT_OnBoard_Driver_t gTFTOnBoard;
#pragma section


#ifdef __cplusplus
extern "C"
{
#endif

/*
 * QSPI Application callback
 * @param aResult 		- QSPI transaction result
 * @param apUserData 	- user data
 * @return void
 */
static void tft_app_cb(QSPI_exchangeResult_t aResult, void *apUserData);

/*
 * release Resource
 * @param apRes		- resource
 * @return void
 */
static void tft_app_release(volatile boolean_t *apRes);

/*
 * wait for Resource
 * @param apRes		- resource
 * @return void
 */
static void tft_app_wait(volatile boolean_t *apRes);

/*
 * sleep for specified time
 * @param ms		- time in ms
 * @return void
 */
static void tft_sleep_ms(uint32_t ms);

/*
 * Write to ILI9341 register
 * @param regaddr 	- register to write
 * @param puiData 	- write pointer
 * @param count 	- length to read (16-bit values)
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
static RC_t tft_write_data_ili9341 (uint8_t regaddr, uint16_t *puiData, uint32_t count);

/*
 * Read from ILI9341 register
 * @param regaddr 	- register to read
 * @param puiData 	- read pointer
 * @param count 	- length to read (16-bit values)
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
static RC_t tft_read_data_ili9341 (uint8_t regaddr, uint16_t *puiData, uint32_t count);

/*
 * Send a buffer to TFT
 * @param apBuf - buffer pointer
 * @param numberOfPixel - buffer size in pixels
 * @param x - x position
 * @param y - y position
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
static RC_t tft_flush_buff(uint16_t *apBuf, uint32_t numberOfPixel, sint32_t x, sint32_t y);

/*
 * Terminate endless transfer mode on ILI9341
 * @param NIL
 * @return void
 */
static void tft_terminate_endless_transfer (void);

#ifdef __cplusplus
}
#endif

/*
 * release Resource
 * @param apRes		- resource
 * @return void
 */
static void tft_app_release(volatile boolean_t *apRes)
{
	// Update Value
	*((volatile boolean_t *)apRes) = TRUE;
}

/*
 * wait for Resource
 * @param apRes		- resource
 * @return void
 */
static void tft_app_wait(volatile boolean_t *apRes)
{
	//@PF - Hack - read and write index of package ringbuffer tend to get out of sync, causing a permament blocking...
	// It seems to work without blocking...
	return;
	// Busy Wait
	while (TRUE != *apRes);
}

/*
 * QSPI Application callback
 * @param aResult 		- QSPI transaction result
 * @param apUserData 	- user data
 * @return void
 */
static void tft_app_cb(QSPI_exchangeResult_t aResult, void *apUserData)
{
	if (QSPI_exchangeResult_SUCCESS == aResult)
	{
		if (NULL != apUserData)
		{
			tft_app_release((volatile boolean_t *)apUserData);
		}
	}
	else
	{
		gTFTOnBoard.mError = RC_ERROR;
	}
}

/*
 * sleep for specified time
 * @param ms		- time in ms
 * @return void
 */
static void tft_sleep_ms(uint32_t ms)
{
	volatile long j = 0;

	long n = 72000 * ms,
			 i = 0;

	for (i = 0; i < n; i++)
		j = i;
}


/*
 * Initialize TFT QSPI channel and TFT.
 * @param NIL
 * @return #RC_SUCCESS on success,
 * 		   #RC_ERROR on unsupported TFT
 */
RC_t TFT_init (void)
{
	uint16_t tft_id = 0;
	RC_t retCode = RC_ERROR;

	uint16_t uwData[16] = {0};

	retCode = QSPI_channel_init(QSPI0,QSPI_CH8,&gTFTOnBoard.mChannel);
	if (RC_SUCCESS != retCode)
	{
		return retCode;
	}

    // check for ILI9341 controller
	tft_read_data_ili9341 (0xD3, &uwData[0], 5);
	if ((uwData[3] == 0x0093) && (uwData[4] == 0x0041))
	{
		tft_id = 0x9341;
	}

	if (tft_id == 0x9341)
    {
        // this is an ILI9341 controller
    	// we will send max. 15 values + 1 value for last write


        /************* Start Initial Sequence **********/
    	uwData[0] = 0x0000;
    	uwData[1] = 0x0083;
    	uwData[2] = 0x0030;
    	uwData[3] = 0x0000;
    	tft_write_data_ili9341(0xCF, &uwData[0], 4); // setting from display supplier

    	uwData[0] = 0x0064;
    	uwData[1] = 0x0003;
    	uwData[2] = 0x0012;
    	uwData[3] = 0x0081;
     	uwData[4] = 0x0000;
    	tft_write_data_ili9341(0xED, &uwData[0], 5); // setting from display supplier

    	uwData[0] = 0x0085;
    	uwData[1] = 0x0000;
    	uwData[2] = 0x0078;
    	uwData[3] = 0x0000;
    	tft_write_data_ili9341(0xE8, &uwData[0], 4); // setting from display supplier

    	uwData[0] = 0x0039;
    	uwData[1] = 0x002C;
    	uwData[2] = 0x0000;
    	uwData[3] = 0x0034;
    	uwData[4] = 0x0002;
    	uwData[5] = 0x0000;
    	tft_write_data_ili9341(0xCB, &uwData[0], 6); // setting from display supplier

    	uwData[0] = 0x0020;
    	uwData[1] = 0x0000;
    	tft_write_data_ili9341(0xF7, &uwData[0], 2); // setting from display supplier

    	uwData[0] = 0x0000;
    	uwData[1] = 0x0000;
    	uwData[2] = 0x0000;
    	tft_write_data_ili9341(0xEA, &uwData[0], 3); // setting from display supplier

    	uwData[0] = 0x0019;  // VRH[5:0]
    	uwData[1] = 0x0000;
    	tft_write_data_ili9341(0xC0, &uwData[0], 2); // Power Control 1

    	uwData[0] = 0x0011;  // SAP[2:0];BT[3:0]
    	uwData[1] = 0x0000;
    	tft_write_data_ili9341(0xC1, &uwData[0], 2); // Power Control 2

    	uwData[0] = 0x0031;
    	uwData[1] = 0x003C;
    	uwData[2] = 0x0000;
    	tft_write_data_ili9341(0xC5, &uwData[0], 3); // VCM Control 1

    	uwData[0] = 0x00B0;
    	uwData[1] = 0x0000;
    	tft_write_data_ili9341(0xC7, &uwData[0], 2); // VCM Control 2

    	uwData[0] = 0x0000;
		uwData[1] = 0x0000;
		uwData[2] = 0x0001;
		uwData[3] = 0x003F;
		uwData[4] = 0x0000;
		tft_write_data_ili9341(0x30, &uwData[0], 5); // Partial Area

    	uwData[0] = 0x0028;  // MV=1; BGR=1
    	uwData[1] = 0x0000;
    	tft_write_data_ili9341(0x36, &uwData[0], 2); // Memory Access Control

    	uwData[0] = 0x0055;  // 16 bit
    	uwData[1] = 0x0000;
    	tft_write_data_ili9341(0x3A, &uwData[0], 2); // Pixel Format Set

    	uwData[0] = 0x0000;
    	uwData[1] = 0x0017;
    	//uwData[1] = 0x0018;
    	uwData[2] = 0x0000;
    	tft_write_data_ili9341(0xB1, &uwData[0], 3); // Frame Control (in Normal Mode)

    	uwData[0] = 0x000A;
    	uwData[1] = 0x00A2;
    	uwData[2] = 0x0000;
    	tft_write_data_ili9341(0xB6, &uwData[0], 3); // Display Function Control

    	uwData[0] = 0x0001;
    	uwData[1] = 0x0030;
    	uwData[2] = 0x0000;
    	tft_write_data_ili9341(0xF6, &uwData[0], 3); // Interface Control

    	uwData[0] = 0x0000;  // Gamma Function Disable
    	uwData[1] = 0x0000;
    	tft_write_data_ili9341(0xF2, &uwData[0], 2);  // setting from display supplier

    	uwData[0] = 0x0001;     //Gamma curve selected
    	uwData[1] = 0x0000;
    	tft_write_data_ili9341(0x26, &uwData[0], 2);  // Gamma Set

    	uwData[0]  = 0x000F;
    	uwData[1]  = 0x0026;
    	uwData[2]  = 0x0022;
    	uwData[3]  = 0x000A;
    	uwData[4]  = 0x0010;
    	uwData[5]  = 0x000A;
    	uwData[6]  = 0x004C;
    	uwData[7]  = 0x00CA;
    	uwData[8]  = 0x0036;
    	uwData[9]  = 0x0000;
    	uwData[10] = 0x0015;
    	uwData[11] = 0x0000;
    	uwData[12] = 0x0010;
    	uwData[13] = 0x0010;
    	uwData[14] = 0x0000;
    	uwData[15] = 0x0000;
    	tft_write_data_ili9341(0xE0, &uwData[0], 16); // Positive Gamma Correction

    	uwData[0]  = 0x0000;
    	uwData[1]  = 0x0019;
    	uwData[2]  = 0x001B;
    	uwData[3]  = 0x0005;
    	uwData[4]  = 0x000F;
    	uwData[5]  = 0x0005;
    	uwData[6]  = 0x0033;
    	uwData[7]  = 0x0035;
    	uwData[8]  = 0x0049;
    	uwData[9]  = 0x000F;
    	uwData[10] = 0x001F;
    	uwData[11] = 0x000F;
    	uwData[12] = 0x003F;
    	uwData[13] = 0x003F;
    	uwData[14] = 0x000F;
    	uwData[15] = 0x0000;
    	tft_write_data_ili9341(0xE1, &uwData[0], 16); // Negative Gamma Correction

    	uwData[0] = 0x0000;
    	uwData[1] = 0x0000;
    	uwData[2] = ((TFT_XSIZE-1) & 0xFF00)>>8;
    	uwData[3] = (TFT_XSIZE-1) & 0x00FF;
    	uwData[4] = 0x0000;
    	tft_write_data_ili9341(0x2A, &uwData[0], 5);  // Column Address Set

    	uwData[0] = 0x0000;
    	uwData[1] = 0x0000;
    	uwData[2] = ((TFT_YSIZE-1) & 0xFF00)>>8;
    	uwData[3] = (TFT_YSIZE-1) & 0x00FF;
    	uwData[4] = 0x0000;
    	tft_write_data_ili9341(0x2B, &uwData[0], 5);  // Page Address Set

    	uwData[0] = 0x0000;
    	tft_write_data_ili9341(0x11, &uwData[0], 1);  // Exit Sleep

    	tft_sleep_ms (120);

    	uwData[0] = 0x0000;
    	tft_write_data_ili9341(0x29, &uwData[0], 1);  // Display on

    	tft_read_data_ili9341(0x0B, &uwData[0], 1); //Read MADCTL;
    	gTFTOnBoard.mMADCTL.byte	= (uint8_t)uwData[0];
    	gTFTOnBoard.mOrientation 	= TFT_orientation_revLandscape;

    	gTFTOnBoard.mnID 				= tft_id;
        gTFTOnBoard.mnScratchBufPitch	= (TFT_YSIZE >> 3);
        gTFTOnBoard.mnScratchBufLen 	= TFT_XSIZE * gTFTOnBoard.mnScratchBufPitch;

        return RC_SUCCESS;
    }

    return RC_ERROR;
}

/*
 * Paint a line.
 * @param NIL
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_paint_line(sint32_t x, sint32_t y, uint32_t numberOfPixel, uint16_t color)
{
	uint16_t i = 0;

	for (i = 0; i < numberOfPixel; i++)
		gTFTOnBoard.mnScratchBuf[i] = color;

	return tft_flush_buff(gTFTOnBoard.mnScratchBuf, numberOfPixel, x, y);
}

/*
 * Paint complete frame
 * @param color - fill color
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_paint_frame(uint16_t color)
{
	RC_t retCode = RC_ERROR;
	uint16_t i = 0;

	for (i = 0; i < gTFTOnBoard.mnScratchBufLen; i++)
		gTFTOnBoard.mnScratchBuf[i] = color;

	for (i = 0; i < 8; i++)
	{
		retCode = tft_flush_buff(gTFTOnBoard.mnScratchBuf, gTFTOnBoard.mnScratchBufLen, 0, i * gTFTOnBoard.mnScratchBufPitch);
		if (RC_SUCCESS != retCode)
			return retCode;
	}

	return RC_SUCCESS;
}


/*
 * Draw pixel
 * @param p - x position
 * @param q - y position
 * @param color - pixel
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_set_pixel(uint32_t p, uint32_t q, uint16_t color)
{
	gTFTOnBoard.mnScratchBuf[0] = color;

	return tft_flush_buff(gTFTOnBoard.mnScratchBuf, 1, p, q);
}

/*
 * Set Orientation
 * @param orientation - display orientation
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_set_orientation(TFT_orientation_t orientation)
{
	RC_t ret = RC_ERROR;
	uint16_t uwData[2] = {0};

	if (TFT_orientation_revLandscape == orientation)
	{
		uwData[0] = 0x0028;  // MV=1; BGR=1
		uwData[1] = 0x0000;
		ret = tft_write_data_ili9341(0x36, &uwData[0], 2); // Memory Access Control
	}
	else if (TFT_orientation_landscape == orientation)
	{
		uwData[0] = 0x00E8;  // MY=1; MX=1; MV=1; BGR=1
		uwData[1] = 0x0000;
		ret = tft_write_data_ili9341(0x36, &uwData[0], 2); // Memory Access Control
	}
	else if (TFT_orientation_portrait == orientation)
	{
		uwData[0] = 0x0088;  // MY=1; BGR=1
		uwData[1] = 0x0000;
		ret = tft_write_data_ili9341(0x36, &uwData[0], 2); // Memory Access Control
	}

	if (RC_SUCCESS == ret)
		gTFTOnBoard.mOrientation = orientation;

	return ret;
}

/*
 * Set the vertical scroll Area
 * @param tfa - top fixed area
 * @param bfa - bottom fixed area
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_set_verticalScrollArea(uint16_t tfa, uint16_t bfa)
{
	uint16_t uwData[7] = {0};

	if (TFT_orientation_portrait == gTFTOnBoard.mOrientation)
	{
		sint16_t scrollArea = TFT_XSIZE - tfa - bfa;
		if (0 > scrollArea)
			return RC_ERROR_BAD_PARAM;

		uwData[0] = ((tfa >> 8) & 0xFF);
		uwData[1] = (tfa & 0xFF);
		uwData[2] = (((uint16_t)scrollArea >> 8) & 0xFF);
		uwData[3] = ((uint16_t)scrollArea & 0xFF);
		uwData[4] = ((bfa >> 8) & 0xFF);
		uwData[5] = (bfa & 0xFF);
		uwData[6] = 0x0000;

		return tft_write_data_ili9341(0x33, &uwData[0], 7);
	}

	return RC_ERROR;
}

/*
 * Set the vertical scroll Position
 * @param line - lines to scroll
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_set_verticalScrollPos(uint16_t line)
{
	uint16_t uwData[3] = {0};

	if (TFT_orientation_portrait == gTFTOnBoard.mOrientation)
	{
		uwData[0] = ((line >> 8) & 0xFF);
		uwData[1] = (line & 0xFF);;
		uwData[2] = 0x0000;
		return tft_write_data_ili9341(0x37, &uwData[0], 3);
	}

	return RC_ERROR;
}

/*
 * Send a buffer to TFT
 * @param apBuf - buffer pointer
 * @param numberOfPixel - buffer size in pixels
 * @param x - x position
 * @param y - y position
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
static RC_t tft_flush_buff(uint16_t *apBuf, uint32_t numberOfPixel, sint32_t x, sint32_t y)
{
	RC_t retCode = RC_ERROR;

	QSPI_exchange_param_t tftqspi_write_param;
	volatile boolean_t bResource1 = FALSE,
					   bResource2 = FALSE;

	uint16_t tx_data;

	gTFTOnBoard.mError = RC_SUCCESS;

	TFT_display_setxy (x, y);

	if (gTFTOnBoard.mnID == 0x9341)
		tx_data = (0x1 << 8) | 0x2C; // register 0x2C on ILI9341
	else
		tx_data = (0x1 << 8) | 0x22; // register 0x22 on other

	tftqspi_write_param.mpWritePtr 		= &tx_data;
	tftqspi_write_param.mnLength 		= sizeof(uint16_t);
	tftqspi_write_param.mnDataWidth 	= 10;
	tftqspi_write_param.mfOnExchange 	= NULL;
	tftqspi_write_param.mpUserData	  	= NULL;

	retCode = QSPI_channel_exchange(&gTFTOnBoard.mChannel, QSPI_exchange_write, &tftqspi_write_param);
	if (RC_SUCCESS != retCode)
		return retCode;

	tftqspi_write_param.mpWritePtr 		= apBuf;
	tftqspi_write_param.mnLength 		= numberOfPixel * sizeof(uint16_t);
	if (0 == (numberOfPixel % 2))
		tftqspi_write_param.mnDataWidth 	= 32;
	else
		tftqspi_write_param.mnDataWidth 	= 16;
	tftqspi_write_param.mpUserData	  	= (void *)&bResource1;
	tftqspi_write_param.mfOnExchange 	= &tft_app_cb;


	retCode = QSPI_channel_exchange(&gTFTOnBoard.mChannel, QSPI_exchange_write, &tftqspi_write_param);
	if (RC_SUCCESS != retCode)
		return retCode;

	//@PF added
	tft_app_wait(&bResource1);

	//Dummy Write for Last Data
	tftqspi_write_param.mpWritePtr		= &tx_data;
	tftqspi_write_param.mnLength 		= sizeof(uint16_t);
	tftqspi_write_param.mnDataWidth 	= 16;
	tftqspi_write_param.mpUserData	  	= (void *)&bResource2;
	tftqspi_write_param.mfOnExchange 	= &tft_app_cb;


	retCode = QSPI_channel_exchange(&gTFTOnBoard.mChannel, QSPI_exchange_write, &tftqspi_write_param);
	if (RC_SUCCESS != retCode)
		return retCode;

	tft_app_wait(&bResource2);

	retCode = gTFTOnBoard.mError;
	gTFTOnBoard.mError = RC_SUCCESS;

	tft_terminate_endless_transfer();

	return retCode;
}

/*
 * Read from ILI9341 register
 * @param regaddr 	- register to read
 * @param puiData 	- read pointer
 * @param count 	- length to read (16-bit values)
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
static RC_t tft_read_data_ili9341 (uint8_t regaddr, uint16_t *puiData, uint32_t count)
{
	RC_t retCode = RC_ERROR;
	uint16_t tx_data = 0;

	QSPI_exchange_param_t tftqspi_write_param,
						  tftqspi_read_param;

	volatile boolean_t bResource1 = FALSE,
					   bResource2 = FALSE;

	gTFTOnBoard.mError = RC_SUCCESS;

	tx_data = ((0x3 << 8) | regaddr);

	tftqspi_write_param.mpWritePtr 		= &tx_data;
	tftqspi_write_param.mnLength 		= sizeof(uint16_t);
	tftqspi_write_param.mnDataWidth 	= 10;
	tftqspi_write_param.mpUserData	  	= (void *)&bResource1;
	tftqspi_write_param.mfOnExchange 	= &tft_app_cb;


	retCode = QSPI_channel_exchange(&gTFTOnBoard.mChannel, QSPI_exchange_write, &tftqspi_write_param);
	if (RC_SUCCESS != retCode)
		return retCode;

	//tft_app_wait(&bResource1);

	tftqspi_read_param.mpReadPtr		= puiData;
	tftqspi_read_param.mnLength 		= count * sizeof(uint16_t);
	tftqspi_read_param.mnDataWidth 		= 16;
	tftqspi_read_param.mpUserData	  	= (void *)&bResource2;
	tftqspi_read_param.mfOnExchange 	= &tft_app_cb;


	retCode = QSPI_channel_exchange(&gTFTOnBoard.mChannel, QSPI_exchange_read, &tftqspi_read_param);
	if (RC_SUCCESS != retCode)
		return retCode;

	tft_app_wait(&bResource2);

	retCode = gTFTOnBoard.mError;
	gTFTOnBoard.mError = RC_SUCCESS;

	tft_terminate_endless_transfer();

	return retCode;
}

/*
 * Write to ILI9341 register
 * @param regaddr 	- register to write
 * @param puiData 	- write pointer
 * @param count 	- length to read (16-bit values)
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
static RC_t tft_write_data_ili9341 (uint8_t regaddr, uint16_t *puiData, uint32_t count)
{
	RC_t retCode = RC_ERROR;
    uint16_t tx_data;

    QSPI_exchange_param_t tftqspi_write_param;
    volatile boolean_t bResource = FALSE;

    gTFTOnBoard.mError = RC_SUCCESS;

    tx_data = (0x1 << 8) | regaddr;

    tftqspi_write_param.mpWritePtr 		= &tx_data;
    tftqspi_write_param.mnLength 		= sizeof(uint16_t);
    tftqspi_write_param.mnDataWidth 	= 10;
    tftqspi_write_param.mfOnExchange 	= NULL;
    tftqspi_write_param.mpUserData	  	= NULL;

    retCode = QSPI_channel_exchange(&gTFTOnBoard.mChannel, QSPI_exchange_write, &tftqspi_write_param);
	if (RC_SUCCESS != retCode)
		return retCode;

    //tft_app_wait(&bResource);
    //bResource = FALSE;

    tftqspi_write_param.mpWritePtr		= puiData;
    tftqspi_write_param.mnLength 		= count * sizeof(uint16_t);
    tftqspi_write_param.mnDataWidth 	= 16;
    tftqspi_write_param.mpUserData	  	= (void *)&bResource;
    tftqspi_write_param.mfOnExchange 	= &tft_app_cb;


    retCode = QSPI_channel_exchange(&gTFTOnBoard.mChannel, QSPI_exchange_write, &tftqspi_write_param);
	if (RC_SUCCESS != retCode)
		return retCode;

    tft_app_wait(&bResource);

    retCode = gTFTOnBoard.mError;
    gTFTOnBoard.mError = RC_SUCCESS;

    tft_terminate_endless_transfer();

    return retCode;
}

/*
 * Terminate endless transfer mode on ILI9341
 * @param NIL
 * @return void
 */
static void tft_terminate_endless_transfer (void)
{
	// REQUIRED AS PER APPKIT SPECIFICATION!
	// Quoting:
	// "The endless mode can only be leave by a rising edge of SCLK0 with SLSO08 set to high.
	//	This can be done by a dummy transfer to any other chip select or by a transfer from the
	//	touch controller"

	PORTPIN_clr(QSPI0_SCLK_PIN);

	PORTPIN_modify_prop(QSPI0_SCLK_PIN, portPin_prop_Mode, _outputPushPullGeneral);

	PORTPIN_toggle(QSPI0_SCLK_PIN);

	PORTPIN_toggle(QSPI0_SCLK_PIN);

	PORTPIN_revert_prop(QSPI0_SCLK_PIN, portPin_prop_Mode);
}

/*
 * Set pixel position
 * @param x - x position
 * @param y - y position
 * @return void
 */
RC_t TFT_display_setxy (uint32_t x, uint32_t y)
{
	uint16_t uwData[3];
	RC_t retCode;

	uwData[0] = (uint16_t) ((x >> 8) & 0xFF);
	uwData[1] = (uint16_t) (x & 0xFF);
	uwData[2] = 0x0000;
	retCode = tft_write_data_ili9341(0x2A, &uwData[0], 3);  // Column Address Set, we change only the start (2 Parameters)
	if (RC_SUCCESS != retCode)
		return retCode;

	uwData[0] = (uint16_t) ((y >> 8) & 0xFF);
	uwData[1] = (uint16_t) (y & 0xFF);
	uwData[2] = 0x0000;
	retCode = tft_write_data_ili9341(0x2B, &uwData[0], 3);  // Page Address Set, we change only the start (2 Parameters)

	return retCode;
}

/*
 * Set Column address range
 * @param c1 - column start
 * @param c2 - column end
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_display_CASET (uint16_t c1, uint16_t c2)
{
	uint16_t uwData[5];

	uwData[0] = (uint16_t) ((c1 >> 8) & 0xFF);
	uwData[1] = (uint16_t) (c1 & 0xFF);
	uwData[2] = (uint16_t) ((c2 >> 8) & 0xFF);
	uwData[3] = (uint16_t) (c2 & 0xFF);
	uwData[4] = 0x0000;

	return tft_write_data_ili9341(0x2A, &uwData[0], 5);  // Column Address Set, we change only the start (2 Parameters)
}

/*
 * Set Row address range
 * @param r1 - row start
 * @param r2 - row end
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_display_PASET (uint16_t r1, uint16_t r2)
{
	uint16_t uwData[5];

	uwData[0] = (uint16_t) ((r1 >> 8) & 0xFF);
	uwData[1] = (uint16_t) (r1 & 0xFF);
	uwData[2] = (uint16_t) ((r2 >> 8) & 0xFF);
	uwData[3] = (uint16_t) (r2 & 0xFF);
	uwData[4] = 0x0000;

	return tft_write_data_ili9341(0x2B, &uwData[0], 5);  // Page Address Set, we change only the start (2 Parameters)
}
