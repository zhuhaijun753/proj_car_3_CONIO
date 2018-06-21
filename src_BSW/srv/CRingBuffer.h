/**
* \file CRingBuffer.h
* \author Guna, Fromm
* \date 02-May-2016
*
* \brief CRingBuffer
*
* Template file to provide a ringbuffer for any data type.
* The Ringbuffer is able to detect and signal a completely received protocol, which
* is identified by 2 EOP bytes.
*
* \note <notes>
* \todo <todos>
* \warning  Assumption of use: a protocol is read out before the next full protocol is added. Otherwise the eopFlag is cleared in a wrong manner
*
*  Changelog:\n
*  - 1.2; 27.02.17 Barth
*  			- Added Constructor without EOP
*  - 1.1; 16.8.16; Fromm
*            - Corrected length parameter in readProtocol
*  - 1.0; 2.5.16; Guna
*            - Initial release
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



#ifndef SRC_BSW_SRV_DATAHANDLING_CRingBuffer_H_
#define SRC_BSW_SRV_DATAHANDLING_CRingBuffer_H_

#include "global.h"

template <typename DATA, int SIZE>
class CRingBuffer
{
private:
	DATA			m_pBuffer[SIZE];
	uint16_t		m_writeindex;
	uint16_t		m_readindex;
	uint16_t		m_fillLevel;

	DATA			m_eop1;
	DATA			m_eop2;



public:

	/********************************************[ Init Functions ]*****************************************/

	CRingBuffer(DATA const& eop1, DATA const& eop2);

	CRingBuffer();

	/**
	 * clears and initialises the ringbuffer
	 */
	void 	init(DATA const& eop1, DATA const& eop2);

	/*****************************************[ Read / Write Functions ]*************************************/

	/**
	 * Read the next data element from the ringbuffer
	 * Return false in case no data is available
	 */
	boolean_t 	readData(DATA& data);

	/**
	 * Write data to the ringbuffer
	 * Returns false in case ringbuffer is full
	 */
	boolean_t	writeData(DATA const& data);

	/**
	 * Read a complete protocol from the buffer
	 */
	boolean_t	readFullProtocol( DATA* const data,uint16_t* numberOfElements);

	/**
	 * Write a complete protocol (stream of data) to the buffer
	 * @param data: pointer to buffer (OUT)
	 * @param numberOfElements: size of buffer (in)
	 * @param number of elements: number of elements (out)
	 */
	boolean_t	writeFullProtocol(DATA const* const data, uint16_t numberOfElements);

	/********************************************[ Helper Functions ]***************************************/

	/**
	 * Return the number of filled elements
	 */
	uint16_t getFilledCount();

	/**
	 * Return the number of avalble empty elements
	 */
	uint16_t getEmptyCount();

	/**
	 * Checks the content of the buffer and sets the m_eopflag if a full protocol is in
	 */
	boolean_t checkEndOfProtocol();

private:

	/**
	 * Calculates the length of the protocol incl. the 2 EOP characters
	 */
	uint16_t	getProtocolLength();


};


/********************************************[ Init Functions ]*****************************************/

#pragma section ".ctors"
template <typename DATA, int SIZE>
CRingBuffer<DATA, SIZE>::CRingBuffer(DATA const& eop1, DATA const& eop2)
{

	init(eop1, eop2);

}
template <typename DATA, int SIZE>
CRingBuffer<DATA, SIZE>::CRingBuffer()
{
	m_readindex = 0;
	m_writeindex = 0;
	m_fillLevel = 0;

}
#pragma section

template<typename DATA, int SIZE>
void CRingBuffer<DATA, SIZE>::init(DATA const& eop1, DATA const& eop2)
{

	m_eop1 = eop1;
	m_eop2 = eop2;

	m_readindex = 0;
	m_writeindex = 0;
	m_fillLevel = 0;

}


/********************************************[ Read / Write Functions ]*****************************************/

template <typename DATA, int SIZE>
boolean_t CRingBuffer<DATA, SIZE>::writeData(DATA const& data)
{
	if(getEmptyCount() > 0)
	{
		(m_pBuffer[m_writeindex]) = data;
		m_writeindex = (m_writeindex+1) % SIZE;
		m_fillLevel++;
		return true;
	}
	return false;
}

template <typename DATA, int SIZE>
boolean_t CRingBuffer<DATA, SIZE>::readData(DATA & data)
{
	if(getFilledCount() > 0)
	{
		data = m_pBuffer[m_readindex];
		m_readindex = (m_readindex+1) % SIZE;
		m_fillLevel--;
		return true;
	}
	return false;
}

template <typename DATA, int SIZE>
boolean_t CRingBuffer<DATA, SIZE>::writeFullProtocol( DATA const* const data, uint16_t numberOfElements)
{
	//uint16_t index = 0;

	if(getEmptyCount() >= numberOfElements)
	{
		for (uint16_t i = 0; i < numberOfElements; i++)
		{

			m_pBuffer[m_writeindex] = *(data+i);
			m_writeindex = (m_writeindex+1) % SIZE;
			m_fillLevel++;
		}
		return true;
	}
	return false;
}



template <typename DATA, int SIZE>
boolean_t CRingBuffer<DATA, SIZE>::readFullProtocol( DATA* const data,uint16_t* numberOfElements)
{
	uint16_t readcount = getProtocolLength();

	if (readcount > *numberOfElements || readcount == 0 || readcount >= SIZE)
	{
		//Error: data too big for buffer or no data
		return false;
	}


	for (uint16_t i = 0; i < readcount; i++)
	{
		data[i] = m_pBuffer[m_readindex];
		m_readindex = (m_readindex+1) % SIZE;

		m_fillLevel--;
	}
	(*numberOfElements) = readcount;
	return true;
}



/********************************************[ Helper Functions ]*****************************************/

template <typename DATA, int SIZE>
boolean_t CRingBuffer<DATA, SIZE>::checkEndOfProtocol()
{

	//No EOP
	if((m_eop1 == DATA() ) && (m_eop2 == DATA() ))
	{
		return true;
	}

	//To avoid accidentally wrong result in case of empty buffer
	if (m_fillLevel < 2) return false;


	//EOP2 is at the position of the last byte written, i.e. m_writeIndex -1, EOP1 at writeIndex - 2

	sint16_t posEop2 = (m_writeindex - 1);
	if (posEop2 < 0) posEop2 += SIZE;

	sint16_t posEop1 = (m_writeindex - 2);
	if (posEop1 < 0) posEop1 += SIZE;

	if(m_pBuffer[posEop1] == m_eop1 && m_pBuffer[posEop2] == m_eop2)
	{
		return true;
	}

	//No protocol found
	return false;
}



template <typename DATA, int SIZE>
uint16_t CRingBuffer<DATA, SIZE>::getProtocolLength()
{

	uint16_t count = 0;

	while(count < getFilledCount())
	{

		sint16_t posEop1 = (m_readindex + count) % SIZE;
		sint16_t posEop2 = (m_readindex + count + 1) % SIZE;

		if(m_pBuffer[posEop1] == m_eop1 && m_pBuffer[posEop2] == m_eop2)
		{
			return count+2; //+2 EOP characters
		}

		count++;

	}

	return 0;
}

template <typename DATA, int SIZE>
uint16_t CRingBuffer<DATA, SIZE>::getFilledCount()
{
	return m_fillLevel;
}

template <typename DATA, int SIZE>
uint16_t CRingBuffer<DATA, SIZE>::getEmptyCount()
{
	return (SIZE-m_fillLevel);
}







#endif /* SRC_BSW_SRV_DATAHANDLING_CRingBuffer_H_ */
