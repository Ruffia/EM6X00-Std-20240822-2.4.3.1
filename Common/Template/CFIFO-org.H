// FIFO.h : main header file for the Z80COMM DLL
//

#if !defined(_FIFO_H_)
#define _FIFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define TEST_LOCK


/////////////////////////////////////////////////////////////////////////////
// CFIFO<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class CFIFO : private CList<TYPE, ARG_TYPE>
{
public:
// Construction
	CFIFO(int nFIFOSize = 20);

public:
// Attributes (head and tail)
	// count of elements
	int GetFIFOCount();
	int GetFIFOSize() const;
	BOOL IsFIFOEmpty();
	BOOL IsFIFOFull();

// Operations

	// get head or tail (and remove it) - don't call on empty list !
	BOOL CopyHead( ARG_TYPE Value );
	BOOL Output( ARG_TYPE Value );
	
	// add before head or after tail
	POSITION Input(ARG_TYPE newElement);
	
	// remove all elements
	void ClearFIFO();



// Implementation
protected:
	int					m_nFIFOSize;

#ifndef TEST_LOCK
	CRITICAL_SECTION	m_csFIFOLock;
#else
	volatile BOOL		m_blFIFOLock;
#endif // TEST_LOCK

public:
	~CFIFO();
};

/////////////////////////////////////////////////////////////////////////////
// CFIFO<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
AFX_INLINE int CFIFO<TYPE, ARG_TYPE>::GetFIFOCount()
	{ 
	int iRet;

	__try
	{
#ifndef TEST_LOCK
		EnterCriticalSection(&m_csFIFOLock);
#else
		while( m_blFIFOLock ) Sleep(1);
		m_blFIFOLock = TRUE;
#endif // TEST_LOCK
		// critical operations
		iRet = CList<TYPE, ARG_TYPE>::GetCount();
	}
	__finally
	{
#ifndef TEST_LOCK
		LeaveCriticalSection(&m_csFIFOLock);
#else
		m_blFIFOLock = FALSE;
#endif // TEST_LOCK
	}

	return iRet;
	}

template<class TYPE, class ARG_TYPE>
AFX_INLINE int CFIFO<TYPE, ARG_TYPE>::GetFIFOSize() const
	{ return m_nFIFOSize; }

template<class TYPE, class ARG_TYPE>
AFX_INLINE BOOL CFIFO<TYPE, ARG_TYPE>::IsFIFOEmpty()
	{
	BOOL blRet = TRUE;

	__try
	{
#ifndef TEST_LOCK
		EnterCriticalSection(&m_csFIFOLock);
#else
		while( m_blFIFOLock ) Sleep(1);
		m_blFIFOLock = TRUE;
#endif // TEST_LOCK
		// critical operations
		blRet = CList<TYPE, ARG_TYPE>::IsEmpty();
	}
	__finally
	{
#ifndef TEST_LOCK
		LeaveCriticalSection(&m_csFIFOLock);
#else
		m_blFIFOLock = FALSE;
#endif // TEST_LOCK
	}

	return blRet; 
	}

template<class TYPE, class ARG_TYPE>
AFX_INLINE BOOL CFIFO<TYPE, ARG_TYPE>::IsFIFOFull()
	{
	BOOL blRet = TRUE;

	__try
	{
#ifndef TEST_LOCK
		EnterCriticalSection(&m_csFIFOLock);
#else
		while( m_blFIFOLock ) Sleep(1);
		m_blFIFOLock = TRUE;
#endif // TEST_LOCK
		// critical operations
		blRet = CList<TYPE, ARG_TYPE>::GetCount() >= m_nFIFOSize;
	}
	__finally
	{
#ifndef TEST_LOCK
		LeaveCriticalSection(&m_csFIFOLock);
#else
		m_blFIFOLock = FALSE;
#endif // TEST_LOCK
	}

	return blRet; 
	}

/////////////////////////////////////////////////////////////////////////////
// CFIFO<TYPE, ARG_TYPE> functions

template<class TYPE, class ARG_TYPE>
CFIFO<TYPE, ARG_TYPE>::CFIFO(int nFIFOSize)
{
	m_nFIFOSize = nFIFOSize;
#ifndef TEST_LOCK
	InitializeCriticalSection(&m_csFIFOLock);
#else
	m_blFIFOLock = FALSE;
#endif // TEST_LOCK
}

template<class TYPE, class ARG_TYPE>
CFIFO<TYPE, ARG_TYPE>::~CFIFO()
{
#ifndef TEST_LOCK
	DeleteCriticalSection(&m_csFIFOLock);
#endif // TEST_LOCK
}

template<class TYPE, class ARG_TYPE>
BOOL CFIFO<TYPE, ARG_TYPE>::CopyHead( ARG_TYPE Value )
{
	BOOL returnValue = FALSE;

	if( GetFIFOCount() > 0 )
	{
		__try
		{
#ifndef TEST_LOCK
		EnterCriticalSection(&m_csFIFOLock);
#else
		while( m_blFIFOLock ) Sleep(1);
		m_blFIFOLock = TRUE;
#endif // TEST_LOCK
			// critical operations
			Value = CList<TYPE, ARG_TYPE>::GetHead();
			returnValue = TRUE;
		}
		__finally
		{
#ifndef TEST_LOCK
		LeaveCriticalSection(&m_csFIFOLock);
#else
		m_blFIFOLock = FALSE;
#endif // TEST_LOCK
		}
	}

	return returnValue;
}

template<class TYPE, class ARG_TYPE>
BOOL CFIFO<TYPE, ARG_TYPE>::Output( ARG_TYPE Value )
{
	BOOL returnValue = FALSE;

	if( GetFIFOCount() > 0 )
	{
		TRY
		{
#ifndef TEST_LOCK
			EnterCriticalSection(&m_csFIFOLock);
#else
			while( m_blFIFOLock ) Sleep(1);
			m_blFIFOLock = TRUE;
#endif // TEST_LOCK
			// critical operations
			Value = CList<TYPE, ARG_TYPE>::RemoveHead();
			returnValue = TRUE;
#ifndef TEST_LOCK
			LeaveCriticalSection(&m_csFIFOLock);
#else
			m_blFIFOLock = FALSE;
#endif // TEST_LOCK
		}
		END_TRY 
		/*
		__try
		{
			EnterCriticalSection(&m_csFIFOLock);
			// critical operations
			Value = CList<TYPE, ARG_TYPE>::RemoveHead();
			returnValue = TRUE;

		}
		__finally
		{
			LeaveCriticalSection(&m_csFIFOLock);
		}
		*/
	}

	return returnValue;
}

template<class TYPE, class ARG_TYPE>
POSITION CFIFO<TYPE, ARG_TYPE>::Input(ARG_TYPE newElement)
{
	POSITION returnValue;

	if( GetFIFOCount() >= m_nFIFOSize )
	{ 
		returnValue = (POSITION)-1;
	}
	else
	{
		__try
		{
#ifndef TEST_LOCK
			EnterCriticalSection(&m_csFIFOLock);
#else
			while( m_blFIFOLock ) Sleep(1);
			m_blFIFOLock = TRUE;
#endif // TEST_LOCK
			// critical operations
			returnValue = CList<TYPE, ARG_TYPE>::AddTail( newElement );
		}
		__finally
		{
#ifndef TEST_LOCK
			LeaveCriticalSection(&m_csFIFOLock);
#else
			m_blFIFOLock = FALSE;
#endif // TEST_LOCK
		}
	}
	return returnValue;
}

template<class TYPE, class ARG_TYPE>
void CFIFO<TYPE, ARG_TYPE>::ClearFIFO()
{
	__try
	{
#ifndef TEST_LOCK
		EnterCriticalSection(&m_csFIFOLock);
#else
		while( m_blFIFOLock ) Sleep(1);
		m_blFIFOLock = TRUE;
#endif // TEST_LOCK
		// critical operations
		CList<TYPE, ARG_TYPE>::RemoveAll();
	}
	__finally
	{
#ifndef TEST_LOCK
		LeaveCriticalSection(&m_csFIFOLock);
#else
		m_blFIFOLock = FALSE;
#endif // TEST_LOCK
	}
}

#endif // _FIFO_H_
