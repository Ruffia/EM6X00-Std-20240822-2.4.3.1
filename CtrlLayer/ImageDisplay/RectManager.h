// RectManager.h: interface for the CRectManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTMANAGER_H__6165A56B_A2EE_4598_B2C9_7E444279D2A4__INCLUDED_)
#define AFX_RECTMANAGER_H__6165A56B_A2EE_4598_B2C9_7E444279D2A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CriticalResource.h"
#include <afxmt.h>

// 11.06.16 Ö´ÐÐTRACEÓï¾ä
//#define	EXECUTE_TRACE

class CRectManager  
{
//	CList<RECT,CRect>	m_RectList;
	CList<CRect,CRect&>	m_RectList;
//	CriticalResource	m_mutexList;
	static	CCriticalSection	m_mutexList;
	
	BOOL Reduction( CRect rcNew );

public:
	CRectManager();
	virtual ~CRectManager();

	BOOL	IsEmpty();
	void	Add( CRect rcNew );
	CRect	Get();

};

#endif // !defined(AFX_RECTMANAGER_H__6165A56B_A2EE_4598_B2C9_7E444279D2A4__INCLUDED_)
