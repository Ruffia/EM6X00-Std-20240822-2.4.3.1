// CSImg.cpp : implementation file
//

#include "stdafx.h"
#include "CSImg.h"
#include "CommFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HANDLE g_hListen;

/////////////////////////////////////////////////////////////////////////////
// CCSImg

CCSImg::CCSImg()
{
}

CCSImg::CCSImg(HWND handle)
{
	m_destHandle = handle;
}

CCSImg::~CCSImg()
{
	Close();
}

BOOL CCSImg::SetTimeOut(UINT uTimeOut)
{
	m_nTimerID = SetTimer(NULL, 0, uTimeOut, NULL);
	return m_nTimerID;
}

BOOL CCSImg::KillTimeOut()
{
	return KillTimer(NULL, m_nTimerID);
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CCSImg, CSocket)
	//{{AFX_MSG_MAP(CCSImg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CCSImg member functions

BOOL CCSImg::Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr /* = NULL */, int* lpSockAddrLen /* = NULL */)
{
	SetTimeOut(10000);
	BOOL nRecv = CSocket::Accept(rConnectedSocket, lpSockAddr, lpSockAddrLen);
	KillTimeOut();
	return nRecv;
}

void CCSImg::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

	CSocket::OnAccept(nErrorCode);
}

void CCSImg::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnReceive(nErrorCode);
	//通过自定义消息WM_THREAD和消息参数WP_RECV通知线程去读取数据
	//由Socket绑定的线程通过消息去通知接收数据
	if (m_pThrd)
	{
		m_pThrd->PostThreadMessage(WM_USER_KYKYComm_Thread_Read, 0, 0);
	}
}

int CCSImg::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	SetTimeOut(1000000);    // 1000s
	int nRecv = CSocket::Receive(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nRecv;
}

int CCSImg::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	SetTimeOut(100000);    //100s
	int nSend = CSocket::Send(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nSend;
}

BOOL CCSImg::OnMessagePending() 
{
	// TODO: Add your specialized code here and/or call the base class
	MSG msg;
	if (::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
	{
		if (msg.wParam == (UINT)m_nTimerID)
		{
			// Remove the message and call CancelBlockingCall
			::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
			return FALSE;   //no need for idle time processing
		}
	}
	return CSocket::OnMessagePending();
}
