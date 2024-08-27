// RectManager.cpp: implementation of the CRectManager class.
//
// Last Modify : 2009.12.03

/////////////////////////////////////////////////////////////////////////////
// 09.12.03修改IsEmpty，加入同步语句，同时屏蔽TRACE语句；
//			但调试时未通过，又出现移动标尺时死机的情况，故仍改回原样
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RectManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CCriticalSection CRectManager::m_mutexList;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRectManager::CRectManager()
{
}

CRectManager::~CRectManager()
{
	m_RectList.RemoveAll();
}

BOOL CRectManager::Reduction( CRect rcNew )
{
	POSITION pos = m_RectList.GetHeadPosition();
	POSITION pos1, pos2;
	CRect rcTemp, rcUnit;
	long lS, lS1, lS2;
	while( pos != NULL )
	{
		pos1 = pos;
		rcTemp = m_RectList.GetNext( pos );
		rcUnit = rcTemp | rcNew;
	
		lS = rcUnit.Width() * rcUnit.Height();
		lS1 = rcTemp.Width() * rcTemp.Height();
		lS2 = rcNew.Width() * rcNew.Height();

		if( lS <= lS1 + lS2 )
		{
			if( ((pos2 = m_RectList.Find(rcTemp)) != NULL) && (pos2==pos1) )
			{
				m_RectList.RemoveAt(pos1);
			}
			m_RectList.AddTail(rcUnit);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRectManager::IsEmpty()
{
	return m_RectList.IsEmpty();
/*
	BOOL bIsEmpty = FALSE;

	m_mutexList.Lock();
	bIsEmpty = m_RectList.IsEmpty();
	m_mutexList.Unlock();

	return bIsEmpty;*/
}

void CRectManager::Add( CRect rcNew )
{
	m_mutexList.Lock();

	if ( !Reduction( rcNew ) )
	{
		m_RectList.AddTail( rcNew );
	}

	m_mutexList.Unlock();
}

CRect CRectManager::Get()
{
	CRect rcTemp;

	m_mutexList.Lock();

	rcTemp = m_RectList.RemoveHead();
	
	m_mutexList.Unlock();

	return rcTemp;
}
