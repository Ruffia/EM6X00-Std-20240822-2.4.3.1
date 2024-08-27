#if !defined(_TIMESTATISTIC_H__INCLUDED_)
#define _TIMESTATISTIC_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>		// system timer support

#ifdef _DEBUG
//const DWORD dwZ80COMMTimer_Default			= 0xFFFFFFF0;	//ms
//const DWORD dwZ80COMMTimer_WaitEnd			= 0xFFFFFFF0;	//ms
const DWORD dwZ80COMMTimer_Default			= 50;	//ms
const DWORD dwZ80COMMTimer_WaitEnd			= 1000;	//ms
#endif

#ifndef _DEBUG
const DWORD dwZ80COMMTimer_Default			= 50;	//ms
const DWORD dwZ80COMMTimer_WaitEnd			= 1000;	//ms
#endif

class CTimer // unit : ms
{
private:
	DWORD	m_TickCountLimit;
	DWORD	m_TickCountStart;

public:
	~CTimer(){};

	CTimer( DWORD dwTimer = dwZ80COMMTimer_Default )
	{
		Reset( dwTimer );
	};

	void Reset( DWORD dwTimer = dwZ80COMMTimer_Default )
	{
		m_TickCountLimit = dwTimer;
		m_TickCountStart = timeGetTime();
	};

	BOOL IsTimeout()
	{
		return (DWORD)abs(timeGetTime() - m_TickCountStart) > m_TickCountLimit;
	};
};


class CLoopRate // unit : 1000 ms
{
private:
	DWORD		m_dwLoopRate;
	DWORD		m_dwLoopTimer;
	DWORD		m_dwLoopCount;

public:
	~CLoopRate(){};
	CLoopRate()
	{
		m_dwLoopRate	= 0;
		m_dwLoopTimer	= 0;
		m_dwLoopCount	= 0;
	};

	long CountLoopRate()
	{
		DWORD	thisTickCount;
		DWORD   dwTickCountDifferentiation;

		m_dwLoopCount ++;

		thisTickCount = timeGetTime();
		dwTickCountDifferentiation = abs(thisTickCount - m_dwLoopTimer);

		if( dwTickCountDifferentiation >= 1000 )
		{
			m_dwLoopRate = (m_dwLoopCount*1000)/dwTickCountDifferentiation;
			m_dwLoopTimer = thisTickCount;
			m_dwLoopCount = 0;

			return 1;
		}

		return 0;
	}

	DWORD GetLoopRate()
	{
		DWORD	thisTickCount;
		DWORD   dwTickCountDifferentiation;

		thisTickCount = timeGetTime();
		dwTickCountDifferentiation = abs(thisTickCount - m_dwLoopTimer);

		if( dwTickCountDifferentiation >= 1000 )
		{
			m_dwLoopRate = (m_dwLoopCount*1000)/dwTickCountDifferentiation;
			m_dwLoopTimer = thisTickCount;
			m_dwLoopCount = 0;
		}
	
		return m_dwLoopRate; 
	};
};

#endif //_TIMESTATISTIC_H__INCLUDED_
