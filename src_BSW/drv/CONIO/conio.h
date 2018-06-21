/**
 * \file	conio.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief conio base header
 *
 * <detailed description what the file (header and/or source it belongs to) does>
 *
 * \note <notes>
 * \todo <todos>
 * \warning <warnings, e.g. dependencies, order of execution etc.>
 *
 *  Changelog:\n
 *  - <version; data of change; author>
 *            - <description of the change>
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
#ifndef SRC_BSW_DRV_CONIO_CONIO_H_
#define SRC_BSW_DRV_CONIO_CONIO_H_

//-------------------------------------------------------------------- [Includes]
#include "GFX/CConioGFX.h"

#define CONIO_PORTRAIT 		(1)
#define CONIO_REV_PORTRAIT 	(2)
#define CONIO_LANDSCAPE		(3)
#define CONIO_REV_LANDSCAPE	(4)

#define CONIO_MODE CONIO_LANDSCAPE


//-------------------------------------------------------------------- [Types]
/** \brief Pixel data structure */
typedef struct
{
	uint16_t 		mnX;			/**< X position */
	uint16_t 		mnY;			/**< Y position */
}CONIO_PIXEL_t;

/** \brief RGB565 color codes */
typedef enum
{
	RGB565_BLACK 	= 0x0000,		/**< black */
	RGB565_RED		= 0xF800,		/**< red */
	RGB565_GREEN 	= 0x7E00,		/**< green */
	RGB565_BLUE		= 0x001F,		/**< blue */
	RGB565_WHITE 	= 0xFFFF,		/**< white */
	RGB565_ORANGE	= 0xFD20,		/**< orange */
}CONIO_COLOR_rgb565_t;

//-------------------------------------------------------------------- [Global data]
/** \brief Graphics handle */
EXTERN CConioGFX gGFXHndl;

#endif /* SRC_BSW_DRV_CONIO_CONIO_H_ */
