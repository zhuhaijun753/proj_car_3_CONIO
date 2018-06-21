/*
 * CTrace.h
 *
 *  Created on: 18.06.2016
 *      Author: Fromm
 */

#ifndef CTRACE_H_
#define CTRACE_H_

#include "global.h"

#define CREATETRACE(NAME, DATATYPE, SIZE)										\
	CTrace<DATATYPE, SIZE> NAME __attribute__ ((section ( ".trace.data" )));	\

template <class DATATYPE, int SIZE>
class CTrace {
private:
	DATATYPE m_data[SIZE];
	uint32_t m_nextFreePos;
public:
	CTrace()
	{
		init();
	}
	void init()
	{
		m_nextFreePos = 0;
	}

	void add(DATATYPE const& data)
	{
		if (m_nextFreePos < SIZE)
		{
			m_data[m_nextFreePos++] = data;
		}
	}
};

#endif /* CTRACE_H_ */
