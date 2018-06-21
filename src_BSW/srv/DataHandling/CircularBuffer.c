/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: CircularBuffer.c
 * Desc: Circular buffer
 ******************************************************************************/

#include "CircularBuffer.h"

CircularBuffer_t *CircularBuffer_createBytes(uint8_t count)
{
	CircularBuffer_t* buffer = NULL_PTR;

	// Dynamically allocate required amount of memory for Circular buffer structure
	buffer = (CircularBuffer_t*) malloc(sizeof(CircularBuffer_t));

	// If allocation is not successful
	if(buffer == NULL_PTR)
	{
		// return null pointer
		return NULL_PTR;
	}

	// Dynamically allocate required amount of memory for storage array
	buffer->base = (uint8_t*) malloc(count * sizeof(uint8_t));

	// If allocation is not successful
	if(buffer->base == NULL_PTR)
	{
		// De allocate memory for circular buffer structure
		free(buffer);

		// return null pointer
		return NULL_PTR;
	}

	// Initialize buffer index
	buffer->lowindex = 0;

	// Initialize buffer index
	buffer->highindex = 0;

	// Initialize buffer length
	buffer->length = count;

	// Initialize buffer state
	buffer->state = buffAvailable;

	//// Flush buffer and check return status
	if(CircularBuffer_flush(buffer) == buffwrite_ok)
	{
		return buffer;
	}
	else// If flush was not successful, deallocate memory for buffer
	{
		free(buffer->base);

		free(buffer);

		// return null pointer
		return NULL_PTR;
	}
}

CircularBuffer_t *CircularBuffer_initBytes(uint32_t* dataArray, uint8_t count)
{
	CircularBuffer_t* buffer = NULL_PTR;

	// Dynamically allocate required amount of memory for Circular buffer structure
	buffer = (CircularBuffer_t*) malloc(sizeof(CircularBuffer_t));

	// If allocation is not successful
	if(buffer == NULL_PTR)
	{
		// return null pointer
		return NULL_PTR;
	}

	// Dynamically allocate required amount of memory for storage array
	buffer->base = (uint8_t*) dataArray;

	// If allocation is not successful
	if(buffer->base == NULL_PTR)
	{
		// De allocate memory for circular buffer structure
		free(buffer);

		// return null pointer
		return NULL_PTR;
	}

	// Initialize buffer index
	buffer->lowindex = 0;

	// Initialize buffer index
	buffer->highindex = 0;

	// Initialize buffer length
	buffer->length = count;

	// Initialize buffer state
	buffer->state = buffAvailable;

	//// Flush buffer and check return status
	if(CircularBuffer_flush(buffer) == buffwrite_ok)
	{
		return buffer;
	}
	else// If flush was not successful, deallocate memory for buffer
	{

		free(buffer);

		// return null pointer
		return NULL_PTR;
	}

}

buffReturn_t CircularBuffer_flush(CircularBuffer_t *buffer)
{
	// Check if buffer pointer is valid
    if(buffer == NULL_PTR)
    {
    	return buffDONOTEXIST;
    }

    // Check if buffer is not busy
	if(	buffer->state == buffNotavailable)
	{
		return buffbusy;
	}

	// Make the buffer stste busy
	buffer->state = buffNotavailable;

	// Initialize buffer index
	buffer->lowindex = 0;

	// Initialize buffer index
	buffer->highindex = 0;

	// Make buffer state available
	buffer->state = buffAvailable;

	// return status
	return buffwrite_ok;
}

buffReturn_t CircularBuffer_readBytes(CircularBuffer_t *buffer, uint8_t *data, uint8_t count)
{

    uint8_t *Dest = (uint8_t *)data;

    // check if buffer pointer is valid
	if(buffer == NULL_PTR)
	{
		return buffDONOTEXIST;
	}

	// check if buffer is busy
    if(buffer->state != buffAvailable)
    {
    	return buffbusy;
    }

    // check if read count id more than existing data count
    if(((buffer->highindex >= buffer->lowindex) && ((buffer->highindex - buffer->lowindex) < count) ) ||
    		((buffer->highindex < buffer->lowindex) && ((buffer->highindex + (buffer->length - buffer->lowindex)) < count)) )
    {
    	return buffempty;
    }

    // Make buffer status busy
    buffer->state = buffNotavailable;

    do
    {
        count--;

        // copy first byte
        *Dest = ((uint8_t *)buffer->base)[buffer->lowindex];

        buffer->lowindex = ((buffer->lowindex + 1) % buffer->length);

        // point to next destination address
        Dest  = &Dest[1];

    } while (count > 0);

    // make buffer state available
    buffer->state = buffAvailable;

    // Return status of read
    return buffread_ok;
}


buffReturn_t CircularBuffer_writeBytes(CircularBuffer_t *buffer, const uint8_t *data, uint8_t count)
{
    const uint8_t *source = (const uint8_t *)data;

    // Check if pointer is valid
    if(buffer == NULL_PTR)
    {
    	return buffDONOTEXIST;
    }

    // Check if buffer is available
    if(buffer->state != buffAvailable)
    {
    	return buffbusy;
    }

    // Check if buffer has sufficient memory to store data
    if(((buffer->highindex >= buffer->lowindex) && ((buffer->length - (buffer->highindex - buffer->lowindex)) < count)) ||
    		((buffer->highindex < buffer->lowindex) && ((buffer->lowindex - buffer->highindex) < count)) )
    {
    	return bufffull;
    }

    // Make buffer state not available
    buffer->state = buffNotavailable;

    do
    {
        count--;
        //copy one byte to buffer
        ((uint8_t *)buffer->base)[buffer->highindex] = *source;
        //move source array pointer to next byte
        source                                 = &source[1];
        // increment index
        buffer->highindex = ((buffer->highindex + 1 ) % buffer->length);

    } while (count > 0);

    //make buffer state available
    buffer->state = buffAvailable;

    // return status of buffer write
    return buffwrite_ok;
}

void CircularBuffer_delete(CircularBuffer_t *buffer)
{
	// deallocate data array memory
	free(buffer->base);

	// deallocate buffer memory
	free(buffer);
}
