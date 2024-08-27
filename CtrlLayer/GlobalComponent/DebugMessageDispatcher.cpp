// DebugMessageDispatcher.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DebugMessageDispatcher.h"
#include "Message.h"
#include <memory>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDebugMessageDispatcher& CDebugMessageDispatcher::Instance()
{
	static CDebugMessageDispatcher Inst;
	return Inst;
}


CDebugMessageDispatcher::CDebugMessageDispatcher()
{
}

void CDebugMessageDispatcher::DisplayMsg(const CString& sMsg)
{
	static int nCount = 0;
	char sz[128] = {0};
	sprintf_s(sz,128,"PostMessage:%d\r\n",nCount++);
	//OutputDebugString(sz);
	BOOL bRet = ::SendMessage(m_hDebugMessageWnd,WM_USER_KYKYComm_DisplayDebugMessage,WPARAM(&sMsg),(LPARAM)0);
}


void CDebugMessageDispatcher::SyncFineMag(double dblVal, double dblVal2, double dblStage)
{
	int* param = new int[3];
	param[0] = (int)dblVal;
	param[1] = (int)dblVal2;
	param[2] = (int)dblStage;
	::PostMessage(m_hDebugMessageWnd,WM_USER_KYKYComm_SyncFineMag,0,LPARAM(param));
}



void CDebugMessageDispatcher::Co_UpdateScan()
{
	::PostMessage(m_hDebugMessageWnd,WM_USER_KYKYComm_CoUpdateScan,0,0);
}

void CDebugMessageDispatcher::UpdateBeemAlignmentX(int nPos, int n)
{
	::PostMessage(m_hDebugMessageWnd,WM_USER_KYKYComm_UpdateBeemAlignmentX,(WPARAM)nPos,0);
}


void  CDebugMessageDispatcher::UpdateBeemAlignmentY(int nPos, int n)
{
	::PostMessage(m_hDebugMessageWnd,WM_USER_KYKYComm_UpdateBeemAlignmentY,(WPARAM)nPos,0);
}


void CDebugMessageDispatcher::ShowDFocusCtrl(int nShow)
{
	::PostMessage(m_hDebugMessageWnd,WM_USER_KYKYComm_ShowDFocusCtrl,(WPARAM)nShow,0);
}


void  CDebugMessageDispatcher::SyncContrastCtrlText(const CString& strX, const VARIANT& Flag)
{
	CString* pstrX = new CString(strX);
	VARIANT* pFlag = new VARIANT(Flag);
	::PostMessage(m_hDebugMessageWnd,WM_USER_KYKYComm_SyncContrastCtrlText,(WPARAM)pstrX,(LPARAM)pFlag);
}