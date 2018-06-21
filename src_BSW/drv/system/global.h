/**
 * \file	global.h
 * \author	Linde Material Handling
 * \author	Prof. P. Fromm	Hochschule Darmstadt
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	12.01.2017
 * \version	0.6
 *
 * \brief Global definitions for datatypes and definitions
 *
 *	Changelog:\n
 *	- 0.6 12.01.17 Barth
 *              - Added #BIT Macro
 *	- 0.5 13.12.16 Barth
 *				- Advanced documentation
 *				- removed STDLIB and STRING from global headers
 *				- cleaned up backward compatibility symbols. U16 etc.
 *
 *	- 0.4 05.07.16 Barth    
 *				- Added CEIL Macros\n\n
 *
 *	- 0.3 02.06.16 Barth    
 * 				- Removed symbol TC29XB\n
 *		        - Added uint32_t_MAX\n
 *		        - Removed inline functions: _min, _max, _ldmst, _getA11, _swap. Moved _minu to driver.\n
 *		        - Adapted file to work on both, ARUIX and PSoC\n
 *		        - Added RC_t enums: RC_ERROR_BAD_PARAM, RC_ERROR_BAD_DATA, RC_ERROR_CANCELED, RC_ERROR_INVALID_STATE, RC_UNKNOWN\n\n
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


#ifndef GLOBAL_H_
#define GLOBAL_H_

//-------------------------------------------------------------------- [global includes]
#include "derivate.h"

//-------------------------------------------------------------------- [OS selection]

#define OS_PXROS        1                           /**< \brief target runs PxROS*/
#define OS_FREERTOS     2                           /**< \brief target runs FreeRTOS*/
    
//-------------------------------------------------------------------- [Target specific includes]
#if(ARCHITECTURE==ARCH_AURIX_1G)
	#include <machine/intrinsics.h>
	#include "system_configuration.h"
#elif(ARCHITECTURE== ARCH_PSOC)
    #include <project.h>    //Delay etc
#elif(ARCHITECTURE== ARCH_ESP32)

#else
#error "Unknown Target"
#endif

//-------------------------------------------------------------------- [target specific macros]

#if(ARCHITECTURE==ARCH_AURIX_1G)

	#define TCPATH			DERIVATE_NAME	        /**< \brief TC_Path Generator */
	#define TC_STR(s)		# s				        /**< \brief TC_Path Generator Stringification stage 2 */
	#define TC_INCLUDE(f)	TC_STR(f)		        /**< \brief TC_Path Generator Stringification stage 1 */

	#define DELAY_MS(ms)    SYSTEM_DelayUs(ms*1000)	/**< \brief blocking milliseconds delay using AURIX STM timer*/
	#define DELAY_US(us)    SYSTEM_DelayUs(us)		/**< \brief blocking microseconds delay using AURIX STM timer*/

#elif(ARCHITECTURE== ARCH_PSOC)

	#define OS              0             /**< \brief OS selection*/

    #if(OS==OS_FREERTOS)
        /** \brief milliseconds Delay Macro for the use with FreeRTOS*/
        #define DELAY_MS(ms)                \
        {/*current time buffer*/            \
        TickType_t __LastWakeTime;          \
        /*get current time*/                \
        __LastWakeTime=xTaskGetTickCount ();\
    	/* set up delay*/                   \
        vTaskDelayUntil( &__LastWakeTime,   \
        ms*portTICK_PERIOD_MS);}            \
        
        #define DELAY_US(us)    CyDelayUs(us)       /**< \brief microseconds Delay Macro for the use with FreeRTOS. Uses blocking Cypress implementation*/
    #else
        #define DELAY_MS(ms)    CyDelay(ms)         /**< \brief milliseconds Delay Macro for the use with baremetal PSoC*/
        #define DELAY_US(us)    CyDelayUs(us)       /**< \brief microseconds Delay Macro for the use with baremetal PSoC*/
    #endif
#elif(ARCHITECTURE==ARCH_ESP32)
	     /** \brief milliseconds Delay Macro for the use with FreeRTOS*/
        #define DELAY_MS(ms)                \
        {/*current time buffer*/            \
        TickType_t __LastWakeTime;          \
        /*get current time*/                \
        __LastWakeTime=xTaskGetTickCount ();\
    	/* set up delay*/                   \
        vTaskDelayUntil( &__LastWakeTime,   \
        ms*portTICK_PERIOD_MS);}            \

#else /*(ARCHITECTURE== ARCH_PSOC)*/
	#error "Unknown Target"
#endif

//-------------------------------------------------------------------- [Datatypes and related definitions]

typedef signed 		char    	sint8_t;            /**< \brief         -128 .. +127            */
typedef unsigned 	char  		uint8_t;            /**< \brief            0 .. 255             */
typedef signed 		short   	sint16_t;           /**< \brief       -32768 .. +32767          */
typedef unsigned 	short 		uint16_t;           /**< \brief            0 .. 65535           */
typedef signed 		long    	sint32_t;           /**< \brief  -2147483648 .. +2147483647     */
typedef unsigned 	long  		uint32_t;           /**< \brief            0 .. 4294967295      */
typedef 			float   	float32_t;	        /**< \brief  single precision floating point number (4 byte) */
typedef 			double  	float64_t;	        /**< \brief  double precision floating point number (8 byte) */
typedef signed 		long long   sint64_t;           /**< \brief -9223372036854775808 .. +9223372036854775807     */
typedef unsigned 	long long 	uint64_t;           /**< \brief                    0 .. 18446744073709551615     */
typedef unsigned	char  		boolean_t; 	        /**< \brief  for use with TRUE/FALSE        */
typedef 			char    	char_t;		        /**< \brief	Character Datatype*/

#define uint8_t_MAX 	0xFF                        /**< \brief	maximum value of uint8_t*/
#define uint16_t_MAX 	0xFFFF                      /**< \brief	maximum value of uint16_t*/
#define uint32_t_MAX 	0xFFFFFFFF                  /**< \brief	maximum value of uint32_t*/

#ifndef TRUE
    #define TRUE        ((boolean_t) 1)             /**< \brief	Value is true (boolean_t type) */
#endif

#ifndef FALSE
    #define FALSE       ((boolean_t) 0)             /**< \brief	Value is true (boolean_t type) */
#endif

#ifndef ON
    #define ON        	1             				/**< \brief	Parameter is ON */
#endif

#ifndef OFF
    #define OFF       	0             				/**< \brief	Parameter is OFF */
#endif

#define NULL_PTR        ((pvoid_t)0)                /**< \brief NULL pointer */

#ifndef NULL
	#define NULL 0									/**< \brief NULL */
#endif

typedef const char        *pchar_t;                 /**< \brief const char pointer                               */
typedef void              *pvoid_t;                 /**< \brief void pointer                                     */
typedef volatile void     *vvoid_t;                 /**< \brief volatile void pointer                            */

typedef void(*FctPtr_void)(void);			        /**< \brief Function Pointer with void 	argument*/
typedef void(*FctPtr_int) (sint32_t);		        /**< \brief Function Pointer with sint32 argument*/


//-------------------------------------------------------------------- [global enumerations]

/**
 * \brief Return types for RTE and non RTE functions, to indicate the result of a operation
 */
typedef enum
{
	RC_SUCCESS              =  0,                   /**< \brief Function was successfully completed.                            	*/
	RC_ERROR                = -1,                   /**< \brief Non specific error.                                             	*/
	RC_ERROR_NULL           = -2,                   /**< \brief A pointer was NULL when a non-NULL pointer was expected.        	*/
	RC_ERROR_ZERO           = -3,                   /**< \brief A value was zero when no zero can be accepted.                  	*/
	RC_ERROR_MEMORY         = -4,                   /**< \brief Out of memory.                                                  	*/
	RC_ERROR_RANGE          = -5,                   /**< \brief A value was out of Range.                                       	*/
	RC_ERROR_OVERRUN        = -6,                   /**< \brief A value was too big.                                            	*/
	RC_ERROR_UNDERRUN       = -7,                   /**< \brief A value was too small                                           	*/
	RC_ERROR_BUFFER_FULL    = -8,                   /**< \brief A buffer was full.                                              	*/
	RC_ERROR_BUFFER_EMTPY   = -9,                   /**< \brief A buffer was empty.                                             	*/
	RC_ERROR_BUSY           = -10,                  /**< \brief A resource was busy.                                            	*/
	RC_ERROR_TIME_OUT       = -11,                  /**< \brief Something take too much time.                                   	*/
	RC_ERROR_OPEN           = -12,                  /**< \brief A device can't be opened.                                       	*/
	RC_ERROR_CHECKSUM       = -13,                  /**< \brief Wrong checksum.                                                 	*/
	RC_ERROR_READ_ONLY      = -14,                  /**< \brief Object is read only.                                            	*/
	RC_ERROR_WRITE_ONLY     = -15,                  /**< \brief Object is write only.                                           	*/
	RC_ERROR_INVALID        = -16,                  /**< \brief Object is invalid.                                              	*/
	RC_ERROR_READ_FAILS     = -17,                  /**< \brief Could not read from object.                                     	*/
	RC_ERROR_WRITE_FAILS    = -18,                  /**< \brief Could not write to the object.                                  	*/
	RC_ERROR_NOT_MATCH      = -19,                  /**< \brief Could not write to the object.								 	    */
	RC_ERROR_BAD_PARAM      = -20,                  /**< \brief Parameters passed to the function are invalid or illegal.           */
	RC_ERROR_BAD_DATA       = -21,                  /**< \brief Invalid global data which is required by the function.              */
	RC_ERROR_CANCELED       = -22,                  /**< \brief operation was canceled.                                             */
	RC_ERROR_INVALID_STATE  = -23,                  /**< \brief The operation can not be performed because of a FSM-state           */
    RC_ERROR_UNKNOWN        = -24,                  /**< \brief Unknown error.                                                      */
}RC_t;

/** \brief CPU Core number */
typedef enum{
	cpu0=0,	                                        /**< \brief Core/CPU 0*/
	cpu1=1,	                                        /**< \brief Core/CPU 1*/
	cpu2=2	                                        /**< \brief Core/CPU 2*/
}CpuId_t;

/** \brief Transmission states for communication services */
typedef enum{
    transmission_UNDEFINED,                         /**< \brief Undefined state.*/
    transmission_PENDING,                           /**< \brief Request is pending.*/
    transmission_ACTIVE,                            /**< \brief Request is currently processed.*/
    transmission_COMPLETED,                         /**< \brief Request is completed.*/
    transmission_ERROR,                             /**< \brief Request failed due to communication error */
    transmission_ABORTED                            /**< \brief Request was aborted.*/
}transmission_state_t;

/** \brief Services states */
typedef enum{
    service_UNDEFINED,                              /**< \brief Undefined state.*/
    service_READY,                                  /**< \brief Service is ready.*/
    service_BUSY,                                   /**< \brief Service is busy.*/
    service_UNAVAILABLE,                            /**< \brief Service is unavailable.*/
}service_state_t;


//-------------------------------------------------------------------- [Bit manipulation helpers]

#ifndef BIT_0
#   define BIT_0 0x0                                /**< \brief Bit 0*/
#endif

#ifndef BIT_1
#   define BIT_1  0x1                               /**< \brief Bit 1*/
#endif

#ifndef BIT
	#define BIT(x)	(BIT_1<<x)						/**< \brief 1 shifted by given parameter*/
#endif

/** \brief Union for accessing single bits in a byte*/
typedef union{
	uint8_t byte;                                   /**< \brief whole byte*/
	struct{
		unsigned char _7 : 1;                       /**< \brief Bit 7*/
		unsigned char _6 : 1;                       /**< \brief Bit 6*/
		unsigned char _5 : 1;                       /**< \brief Bit 5*/
		unsigned char _4 : 1;                       /**< \brief Bit 4*/
		unsigned char _3 : 1;                       /**< \brief Bit 3*/
		unsigned char _2 : 1;                       /**< \brief Bit 2*/
		unsigned char _1 : 1;                       /**< \brief Bit 1*/
		unsigned char _0 : 1;                       /**< \brief Bit 0*/
	} bit;                                          /**< \brief single bits*/
} u8bitfield_t;


#ifndef BYTE_L
#	define BYTE_L          	(0x8)           		/**< \brief length of one byte ;)*/
#endif

#ifndef NIBBLE_L
#	define NIBBLE_L         (0x4)           		 /**< \brief length of one nibble*/
#endif

#define BYTE_MASK           (0xff)                  /**< \brief masks one byte*/
#define CONCAT_2BYTES(x,y)  ((x<<8)|y)        		/**< \brief Concatenates 2 bytes to an 16 bit/2byte value*/
#define ADRU32(x)           ((uint32_t)&x)          /**< \brief Casts the address of a symbol to uint32_t*/

#ifndef LO4
/** \brief Get lower 4 bits of value*/
#	define LO4(x)        	((uint8_t) ((x) & 0xFu))
#endif

#ifndef HI4
/** \brief Get higher 4 bits of value*/
#	define HI4(x)        	((uint8_t) (((x) & 0xF0u)>>4))
#endif

#if(TARGET==TARGET_AURIX)
	#ifndef LO8
	/** \brief Get lower 8 bits of value*/
	#	define LO8(x)        	((uint8_t) ((x) & 0xFFu))
	#endif

	#ifndef HI8
	/** \brief Get higher 8 bits of (16bit) value*/
	#	define HI8(x)          ((uint8_t) ((uint16_t)(x) >> 8))
	#endif

	#ifndef LO16
	/** \brief Get lower 16 bits of value*/
	#	define LO16(x)                 ((uint16_t) ((x) & 0xFFFFu))
	#endif

	#ifndef HI16
	/** \brief Get higher 16 bits of (32bit) value*/
	#	define HI16(x)                 ((uint16_t) ((uint32_t)(x) >> 16))
	#endif
#endif

//-------------------------------------------------------------------- [Compiler attributes]
/** \brief inline function*/
#define INLINE         static inline __attribute__ ((always_inline))

/** \brief extern reference*/
#define EXTERN      	extern

/** \brief static qualifier*/
#define STATIC      	static

/** \brief GCC packed attribute*/
#define PACKED         	__attribute__ ((packed))

/**< \brief GCC aligned attribute*/
#define ALIGN(n)       	__attribute__ ((aligned(n)))

/** \brief Absolute Jump */
#define __absolute_jump(adr)	__asm__ volatile ("ja "#adr)

//-------------------------------------------------------------------- [Mathematical helpers]

/** \brief Round value */
#define _roundf(X)	((((X) - (sint32_t)(X)) > 0.5) ? (1 + (sint32_t)(X)) : ((sint32_t)(X)))

/** \brief float absolute value */
#define _absf(X)    ( ((X) < 0.0) ? -(X) : (X) )

/** \brief integer absolute value */
#define _abs(X)    ( ((X) < 0) ? -(X) : (X) )

/** \brief  Binary inequality comparison X != Y*/
#define _neqf(X,Y)  ( ((X) > (Y)) ||  ((X) < (Y)) )

/** \brief  Binary lesser than or equal comparison X <= Y*/
#define _leqf(X,Y)  ( !((X) > (Y)) )

/** \brief  get the smaller value out of 2 */
#define _minf(X,Y)  ( ((X) < (Y)) ? (X) : (Y) )

/** \brief  get the smaller value out of 2 */
#define _maxf(X,Y)  ( ((X) > (Y)) ? (X) : (Y) )

/** \brief ceilling division */
#define _ceil_div(x, y) ((x) + (y) - 1) / (y)

/** \brief ceil to pos */
#define _ceil_pos(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))

/** \brief definition of PI constant */
#ifndef M_PI
#define M_PI 3.14159265359
#endif

#endif /* GLOBAL_H_ */
