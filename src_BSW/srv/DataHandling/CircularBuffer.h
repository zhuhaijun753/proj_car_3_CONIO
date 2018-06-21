/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: CircularBuffer.h
 * Desc: Circular buffer
 ******************************************************************************/

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "global.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/*  Circular buffer current status */
typedef enum
{
	buffAvailable,
	buffNotavailable
}buffState_t;

/* Circular buffer definition. */
typedef struct
{
    void  			*base;                  /* buffer base address */
    uint16_t 		lowindex;                  /* buffer current index */
    uint16_t 		highindex;                  /* buffer current index */
    uint16_t 		length;                 /* buffer length */
    volatile buffState_t		state;					/* Buffer state */
} CircularBuffer_t;

/*  Circular buffer return status */
typedef enum
{
	buffwrite_ok,
	buffread_ok,
	bufffull,
	buffempty,
	buffbusy,
	buffDONOTEXIST
} buffReturn_t;

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** Read Data from circular buffer
 * @param: pointer to circular Buffer
 * @param: Pointer to data
 * @param: Count of bytes of data to be read
 * @return: status of read
 */
EXTERN buffReturn_t CircularBuffer_readBytes(CircularBuffer_t *buffer, uint8_t *data, uint8_t count);

/** Write Data to circular buffer
 * @param: pointer to circular Buffer
 * @param: Pointer to data
 * @param: Count of bytes of data to be written
 * @return: status of write
 */
EXTERN buffReturn_t CircularBuffer_writeBytes(CircularBuffer_t *buffer, const uint8_t *data, uint8_t count);

/** Create circular buffer
 * @param: Count of bytes of data for circular buffer
 * @return: Pointer to circular buffer created
 */
EXTERN CircularBuffer_t *CircularBuffer_createBytes(uint8_t count);


/** Initilization of circular buffer(In case existing data array has to be converted to circular buffer)
 * @param: Pointer to data array
 * @param: Count of bytes of data for circular buffer
 * @return: Pointer to circular buffer created
 */
EXTERN CircularBuffer_t *CircularBuffer_initBytes(uint32_t* dataArray, uint8_t count);

/** De Initilization of circular buffer
 * @param: Pointer to circular buffer
 * @return: None
 */
EXTERN void CircularBuffer_delete(CircularBuffer_t *buffer);

/** Flushing of circular buffer
 * @param: Pointer to circular buffer
 * @return: status
 */
EXTERN buffReturn_t CircularBuffer_flush(CircularBuffer_t *buffer);

/******************************************************************************/
/*-------------------------      Inline Functions    -------------------------*/
/******************************************************************************/

/** Fill level of circular buffer
 * @param: Pointer to circular buffer
 * @return: level
 */
INLINE uint16_t CircularBuffer_getReadcount(CircularBuffer_t *buffer)
{
    if(buffer->highindex >= buffer->lowindex)
    {
    	return(buffer->highindex - buffer->lowindex);
    }
    else
    {
    	return(buffer->highindex + (buffer->length - buffer->lowindex));
    }
}

/** Empty level of circular buffer
 * @param: Pointer to circular buffer
 * @return: level
 */
INLINE uint16_t CircularBuffer_getWritecount(CircularBuffer_t *buffer)
{
    if(buffer->highindex >= buffer->lowindex)
    {
    	return(buffer->length - (buffer->highindex - buffer->lowindex));
    }
    else
    {
    	return(buffer->lowindex - buffer->highindex);
    }
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
