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

// 11.06.16 执行TRACE语句
//#define	EXECUTE_TRACE

//输出CRect 信息
void OutPutRect(char* sRectName, const CRect& rect, char* sAddition = "");

//输出CPoint 信息
void OutPutPoint(char* sPtName, const CPoint& pt, char* sAddition = "");


class AFX_EXT_CLASS CRectManager  
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

	void  Iterate(char* sListName = "");

};

#endif // !defined(AFX_RECTMANAGER_H__6165A56B_A2EE_4598_B2C9_7E444279D2A4__INCLUDED_)
