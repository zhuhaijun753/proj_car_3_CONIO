/**
 * \file	conio_text.cpp
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief conio text driver implementation
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

//-------------------------------------------------------------------- [header]

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cstdlib>

#include "conio_text.h"
#include "GFX/Adafruit_GFX/gfxfont.h"

/**
 *  Including a single font as default.
 *  No option at run-time to choose specific font provided,
 *  as this would imply including all Font specific consts
 *  to the binary, thereby increasing the size of the compiled
 *  binary!
 *
 *  If this font does not look good, please choose a different
 *  font!
 */
#include "GFX/Adafruit_GFX/Fonts/FreeMono9pt7b.h"

//-------------------------------------------------------------------- [Defines]

/** IS_CHAR_NORMAL **/
#define CHAR_SPECIAL_TAB  			0x09		/**< TAB ASCII value **/
#define CHAR_SPECIAL_LF  			0x0A		/**< Line Feed ASCII value **/
#define CHAR_SPECIAL_CR  			0x0D		/**< Carriage Return ASCII value **/
#define CHAR_SPECIAL_PERCENT 		0x25		/**< % ASCII value **/

/** IS_CHAR_SPECIAL_PERCENT **/
#define CHAR_SPECIAL_CHAR			'c'			/**< char format identifier **/
#define CHAR_SPECIAL_INT_SIGNED1	'i'			/**< signed int format identifier **/
#define CHAR_SPECIAL_INT_SIGNED2	'd'			/**< signed int format identifier **/
#define CHAR_SPECIAL_INT_UNSIGNED	'u'			/**< unsigned int format identifier **/
#define CHAR_SPECIAL_FLOAT			'f'			/**< float format identifier **/
#define CHAR_SPECIAL_STRING			's'			/**< string format identifier **/


#define CONIO_CHARACTER_SIZE		(1)		 //times the font-size

//-------------------------------------------------------------------- [Types]

/** \brief Text Driver type */
typedef struct
{
	const GFXfont		*mpFont; 		/**< font handle **/
	CConioGFX			*mpGFXHndl;		/**< graphics handle **/
	uint16_t			mnFontW;		/**< font width **/
	uint16_t			mnFontH;		/**< font height **/

}CONIO_TEXT_driver_t;

/** \brief Text Driver write state type */
typedef enum
{
	IS_CHAR_NORMAL = 0,					/**< handling normal character  **/
	IS_CHAR_SPECIAL_PERCENT,			/**< waiting for character after % **/

}CONIO_TEXT_writeState_t;

//-------------------------------------------------------------------- [local data]

#pragma section ".data.conio"
STATIC CONIO_TEXT_driver_t gConioTextDriver;
#pragma section

const uint32_t gPower10[] 	= {1000000000, 100000000, 10000000,
						   	   1000000, 100000, 10000, 1000, 100, 10, 1};

//-------------------------------------------------------------------- [local prototypes]

/**
 * \brief STATIC void sint_to_str(char_t *s, sint32_t d)
 *
 * Convert signed integer to string
 *
 * \param	s	: string pointer
 * \param	d 	: signed int value
 * \return 	void
 *
 */
STATIC void sint_to_str(char_t *s, sint32_t d);

/**
 * \brief STATIC void uint_to_str(char_t *s, uint32_t u)
 *
 * Convert unsigned integer to string
 *
 * \param	s	: string pointer
 * \param	u 	: unsigned int value
 * \return 	void
 *
 */
STATIC void uint_to_str(char_t *s, uint32_t u);


//-------------------------------------------------------------------- [global functions]

/**
 * \brief RC_t CONIO_TEXT_init()
 *
 * Initialize text environment.
 *
 * \param	void
 * \return #RC_SUCCESS
 *
 */
RC_t CONIO_TEXT_init()
{
	memset((void *)&gConioTextDriver, 0, sizeof(CONIO_TEXT_driver_t));

	gConioTextDriver.mpFont 	= &FreeMono9pt7b;
	// Getting the cursor advance params!
	// W - getting the advance of first entry in table
	gConioTextDriver.mnFontW	= (uint16_t)gConioTextDriver.mpFont->glyph[0].xAdvance;
	gConioTextDriver.mnFontH 	= (uint16_t)gConioTextDriver.mpFont->yAdvance;

	gConioTextDriver.mpGFXHndl 	= &gGFXHndl;

	gConioTextDriver.mpGFXHndl->setFont(gConioTextDriver.mpFont);

	return RC_SUCCESS;
}

/**
 * \brief RC_t CONIO_TEXT_write(const CONIO_TEXT_t *apText, const char_t *apFmt, ...)
 *
 * Write text.
 *
 * \param	apText 	: text handle
 * \param 	apFmt	: variadic argument format followed by arguments
 * \return #RC_ERROR_BAD_PARAM x_pos or y_pos is beyond display limits,
 * \return #RC_SUCCESS otherwise
 *
 */
RC_t CONIO_TEXT_write(const CONIO_TEXT_t *apText, const char_t *apFmt, ...)
{
	sint32_t len = 0,
			 i = 0,
			 j = 0;

	CONIO_TEXT_writeState_t state = IS_CHAR_NORMAL;
	CONIO_PIXEL_t curPixel = {0, 0};

	if ((NULL == apFmt) || (NULL == apText))
		return RC_ERROR_BAD_PARAM;

	curPixel.mnX = apText->mCursor.mnX * gConioTextDriver.mnFontW;
	curPixel.mnY = apText->mCursor.mnY * CONIO_TEXT_Y_INC;


	gConioTextDriver.mpGFXHndl->fillRect((sint16_t)curPixel.mnX, (sint16_t)curPixel.mnY,
										  apText->mnLen * gConioTextDriver.mnFontW, gConioTextDriver.mnFontH,
										  apText->mBGColor);

	curPixel.mnY  = curPixel.mnY  + 3; //heuristically determined for the used font

	len = strlen(apFmt);

	va_list args;
	va_start(args, apFmt);

	for (i = 0; i < len; i++)
	{
		switch (state)
		{
		case IS_CHAR_NORMAL:
		{
			switch(apFmt[i])
			{
			case CHAR_SPECIAL_PERCENT:
			{
				state = IS_CHAR_SPECIAL_PERCENT;
			}
			break;

			case CHAR_SPECIAL_LF:
			case CHAR_SPECIAL_CR:
			{
				curPixel.mnY += gConioTextDriver.mnFontH;
			}
			break;

			case CHAR_SPECIAL_TAB:
			{
				for (j = 0; j < 4; j++)
				{
					curPixel.mnX += gConioTextDriver.mnFontW; //Crossing line not handled
				}
			}
			break;

			default:
			{
				gConioTextDriver.mpGFXHndl->drawChar(curPixel.mnX, curPixel.mnY, apFmt[i], apText->mColor, apText->mBGColor, CONIO_CHARACTER_SIZE);
				curPixel.mnX += gConioTextDriver.mnFontW;
			}
			break;
			}
		}
		break;

		case IS_CHAR_SPECIAL_PERCENT:
		{
			switch(apFmt[i])
			{
			case CHAR_SPECIAL_CHAR:
			{
				char_t c = va_arg(args, sint32_t); //char is promoted to int
				gConioTextDriver.mpGFXHndl->drawChar(curPixel.mnX, curPixel.mnY, c, apText->mColor, apText->mBGColor, CONIO_CHARACTER_SIZE);
				curPixel.mnX += gConioTextDriver.mnFontW;
			}

			break;

			case CHAR_SPECIAL_INT_SIGNED1:
			case CHAR_SPECIAL_INT_SIGNED2:
			{
				char_t  s[12] = {0}; // max chars
				sint16_t size = 0;

				sint32_t d = va_arg(args, sint32_t);
				//snprintf(s, 12, "%d", d);
				sint_to_str(s, d);
				size = strlen(s);

				for (j = 0; j < size; j++)
				{
					gConioTextDriver.mpGFXHndl->drawChar(curPixel.mnX, curPixel.mnY, s[j], apText->mColor, apText->mBGColor, CONIO_CHARACTER_SIZE);
					curPixel.mnX += gConioTextDriver.mnFontW;
				}
			}
			break;

			case CHAR_SPECIAL_INT_UNSIGNED:
			{
				char_t  s[12] = {0}; // max chars
				sint16_t size = 0;

				uint32_t u = va_arg(args, uint32_t);

				//snprintf(s, 12, "%u", u);
				uint_to_str(s, u);
				size = strlen(s);

				for (j = 0; j < size; j++)
				{
					gConioTextDriver.mpGFXHndl->drawChar(curPixel.mnX, curPixel.mnY, s[j], apText->mColor, apText->mBGColor, CONIO_CHARACTER_SIZE);
					curPixel.mnX += gConioTextDriver.mnFontW;
				}
			}
			break;

#if 0
			case CHAR_SPECIAL_FLOAT:
			{
				char_t  s[20] = {0}; // max chars
				sint16_t size = 0;

				double f = va_arg(args, double);

				snprintf(s, 20, "%lf", f);
				size = strlen(s);

				for (j = 0; j < size; j++)
				{
					gConioTextDriver.mpGFXHndl->drawChar(curPixel.mnX, curPixel.mnY, s[j], apText->mColor, apText->mBGColor, CONIO_CHARACTER_SIZE);
					curPixel.mnX += gConioTextDriver.mnFontW;
				}
			}
			break;
#endif

			case CHAR_SPECIAL_PERCENT:
			{
				gConioTextDriver.mpGFXHndl->drawChar(curPixel.mnX, curPixel.mnY, apFmt[i], apText->mColor,apText->mBGColor, CONIO_CHARACTER_SIZE);
				curPixel.mnX += gConioTextDriver.mnFontW;
			}
			break;

			case CHAR_SPECIAL_STRING:
			{
				sint16_t size = 0;

				const char_t  *s = va_arg(args, const char_t*);
				size = strlen(s);

				for (j = 0; j < size; j++)
				{

					gConioTextDriver.mpGFXHndl->drawChar(curPixel.mnX, curPixel.mnY, s[j], apText->mColor, apText->mBGColor, CONIO_CHARACTER_SIZE);
					curPixel.mnX += gConioTextDriver.mnFontW;
				}
			}
			break;

			default:
			{
				const char_t  *s = " !CORRUPTION! ";
				sint16_t size = 0;

				size = strlen(s);

				for (j = 0; j < size; j++)
				{
					gConioTextDriver.mpGFXHndl->drawChar(curPixel.mnX, curPixel.mnY, s[j], apText->mColor, apText->mBGColor, CONIO_CHARACTER_SIZE);
					curPixel.mnX += gConioTextDriver.mnFontW;
				}

				return RC_ERROR_BAD_DATA;
			}
			break;
			}

			state = IS_CHAR_NORMAL;
		}
		break;
		}
	}

	va_end(args);

	return RC_SUCCESS;
}

/**
 * \brief RC_t CONIO_TEXT_deinit()
 *
 * De-initialize text environment.
 *
 * \param	void
 * \return #RC_SUCCESS
 *
 */
RC_t CONIO_TEXT_deinit()
{
	memset((void *)&gConioTextDriver, 0, sizeof(CONIO_TEXT_driver_t));

	return RC_SUCCESS;
}

/**
 * \brief STATIC void sint_to_str(char_t *s, sint32_t d)
 *
 * Convert signed integer to string
 *
 * \param	s	: string pointer
 * \param	d 	: signed int value
 * \return 	void
 *
 */
STATIC void sint_to_str(char_t *s, sint32_t d)
{
	sint16_t i = 0,
			 j = 0;

	boolean_t start_found = FALSE;

	const uint16_t loop_count = sizeof(gPower10) / sizeof(gPower10[0]);

	if (d == 0)
	{
		s[i++] = '0';
	}
	else
	{
		if (d < 0)
		{
			s[i++] = '-';
			d = _abs(d);
		}

		while (j < loop_count)
		{
			if (TRUE == start_found)
			{
				s[i++] = '0' + (d / gPower10[j]);
				d = d % gPower10[j];
			}
			else
			{
				s[i] = '0' + (d / gPower10[j]);
				if ('0' < s[i])
				{
					start_found = TRUE;
					d = d % gPower10[j];
					i++;
				}
			}

			j++;
		}
	}


	s[i] = '\0';
}

/**
 * \brief STATIC void uint_to_str(char_t *s, uint32_t u)
 *
 * Convert unsigned integer to string
 *
 * \param	s	: string pointer
 * \param	u 	: unsigned int value
 * \return 	void
 *
 */
STATIC void uint_to_str(char_t *s, uint32_t u)
{
	sint16_t i = 0,
			 j = 0;

	boolean_t start_found = FALSE;

	const uint16_t loop_count = sizeof(gPower10) / sizeof(gPower10[0]);

	if (u == 0)
	{
		s[i++] = '0';
	}
	else
	{
		while (j < loop_count)
		{
			if (TRUE == start_found)
			{
				s[i++] = '0' + (u / gPower10[j]);
				u = u % gPower10[j];
			}
			else
			{
				s[i] = '0' + (u / gPower10[j]);
				if ('0' < s[i])
				{
					start_found = TRUE;
					u = u % gPower10[j];
					i++;
				}
			}

			j++;
		}
	}

	s[i] = '\0';
}

