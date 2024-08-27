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

void OutPutRect(char* sRectName, const CRect& rect, char* sAddition)
{
	char sz[256] = {0};
	sprintf_s(sz,256,"%s,%s.left = %d,%s.top = %d,%s.right = %d,%s.bottom = %d\n",
		sAddition, sRectName,rect.left,sRectName,rect.top, sRectName,rect.right, sRectName, rect.bottom);
	OutputDebugString(sz);
}


void OutPutPoint(char* sPtName,const CPoint& pt, char* sAddition)
{
	char sz[256] = {0};
	sprintf_s(sz,256,"%s,%s.x = %d,%s.y = %d\n",sAddition, sPtName,pt.x, sPtName,pt.y);
	OutputDebugString(sz);
}

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

	if( !m_RectList.IsEmpty() )	// 再保护一下，以防空列表的RemoveHead
		rcTemp = m_RectList.RemoveHead();
	
	m_mutexList.Unlock();

	return rcTemp;
}


void  CRectManager::Iterate(char* sListName)
{
	if (!m_RectList.IsEmpty())
	{
		char s[256] = {0};
		sprintf_s(s,256,"%s\n",sListName);
		OutputDebugString(s);
	}

	POSITION pos = m_RectList.GetHeadPosition();
	for (int i=0;i < m_RectList.GetCount();i++)
	{
		CRect& rcDestImageLay = m_RectList.GetNext(pos);
		char sz[256] = {0};
		sprintf_s(sz,256,"rcDestImageLay.left = %d,rcDestImageLay.top = %d,rcDestImageLay.right = %d,rcDestImageLay.bottom = %d\n",
			rcDestImageLay.left,rcDestImageLay.top,rcDestImageLay.right,rcDestImageLay.bottom);
		OutputDebugString(sz);
	}
}