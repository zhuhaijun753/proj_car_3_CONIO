/*
 * CRingBuffer.cpp
 *
 *  Created on: 19.05.2016
 *      Author: Fromm
 */



#include "CRingBuffer.h"

/********************************************[ Init Functions ]*****************************************/

#pragma section ".ctors"
template <typename DATA, int SIZE>
CRingBuffer<DATA, SIZE>::CRingBuffer(DATA const& eop1, DATA const& eop2)
{

	init(eop1, eop2);

}
#pragma section

//template class CRingBuffer<uint8_t, 20>;

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

	if(((readcount != 0x00)) && (readcount <= SIZE))
	{
		for (uint16_t i = 0; i < readcount; i++)
		{
			data[i] = m_pBuffer[m_readindex];
			m_readindex = (m_readindex+1) % SIZE;

			m_fillLevel--;
		}

		return true;
	}

	return false;
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
	uint16_t index = m_readindex;
	uint16_t count = 0;

	while(count <= getFilledCount())
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



