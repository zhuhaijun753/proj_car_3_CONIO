/**
 * \file	tft_reference.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	07.05.2017
 * \version	0.1
 *
 * \brief TFT driver reference header
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

#ifndef SRC_BSW_DRV_TOUCH_TFT_REFERENCE_H_
#define SRC_BSW_DRV_TOUCH_TFT_REFERENCE_H_

#include "global.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TFT_XSIZE 320             //!< x dimension of tft display
#define TFT_YSIZE 240             //!< y dimension of tft display

typedef enum
{
	TFT_orientation_portrait = 0,
	TFT_orientation_revPortrait,
	TFT_orientation_landscape,
	TFT_orientation_revLandscape
}TFT_orientation_t;

/*
 * Initialize TFT QSPI channel and TFT.
 * @param NIL
 * @return #RC_SUCCESS on success,
 * 		   #RC_ERROR on unsupported TFT
 */
RC_t TFT_init (void);

/*
 * Set pixel position
 * @param x - x position
 * @param y - y position
 * @return void
 */
RC_t TFT_display_setxy (uint32_t x, uint32_t y);

/*
 * Set Column address range
 * @param c1 - column start
 * @param c2 - column end
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_display_CASET (uint16_t c1, uint16_t c2);

/*
 * Set Row address range
 * @param r1 - row start
 * @param r2 - row end
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_display_PASET (uint16_t r1, uint16_t r2);

/*
 * Draw pixel
 * @param p - x position
 * @param q - y position
 * @param color - pixel
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_set_pixel(uint32_t p, uint32_t q, uint16_t color);

/*
 * Paint a line.
 * @param NIL
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_paint_line(sint32_t x, sint32_t y, uint32_t numberOfPixel, uint16_t color);

/*
 * Paint complete frame
 * @param color - fill color
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_paint_frame(uint16_t color);

/*
 * Set Orientation
 * @param orientation - display orientation
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_set_orientation(TFT_orientation_t orientation);

/*
 * Set the vertical scroll Area
 * @param tfa - top fixed area
 * @param bfa - bottom fixed area
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_set_verticalScrollArea(uint16_t tfa, uint16_t bfa);

/*
 * Set the vertical scroll Position
 * @param line - lines to scroll
 * @return #RC_SUCCESS on success,
 * 		   error codes otherwise
 */
RC_t TFT_set_verticalScrollPos(uint16_t line);

#ifdef __cplusplus
}
#endif

#endif /* SRC_BSW_DRV_TOUCH_TFT_REFERENCE_H_ */
