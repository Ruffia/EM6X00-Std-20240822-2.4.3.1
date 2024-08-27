#if !defined(_CriticalResource_H__INCLUDED_)
#define _CriticalResource_H__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CriticalResource
{
private:
	volatile BOOL	m_boolLock;

public:
	~CriticalResource(){};

	CriticalResource(BOOL boolInit = FALSE)
	{
		m_boolLock = boolInit;
	};

	BOOL IsLocked()
	{
		return m_boolLock;
	};

	void Lock()
	{
		while(m_boolLock == TRUE) Sleep(0);

		m_boolLock = TRUE;
	};

	void Unlock()
	{
		m_boolLock = FALSE;
	};
};

#endif //_CriticalResource_H__INCLUDED_

