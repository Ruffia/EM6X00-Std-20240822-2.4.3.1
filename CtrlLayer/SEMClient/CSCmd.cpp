// CSCmd.cpp : implementation file
//

#include "stdafx.h"
#include "CSCmd.h"
#include "CommFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HANDLE g_hListen;

/////////////////////////////////////////////////////////////////////////////
// CCSCmd

CCSCmd::CCSCmd()
{
}

CCSCmd::CCSCmd(HWND handle)
{
	m_destHandle = handle;
}

CCSCmd::~CCSCmd()
{
	Close();
}

BOOL CCSCmd::SetTimeOut(UINT uTimeOut)
{
	m_nTimerID = SetTimer(NULL, 0, uTimeOut, NULL);
	return m_nTimerID;
}

BOOL CCSCmd::KillTimeOut()
{
	return KillTimer(NULL, m_nTimerID);
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CCSCmd, CSocket)
	//{{AFX_MSG_MAP(CCSCmd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CCSCmd member functions

BOOL CCSCmd::OnMessagePending()
{
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

int CCSCmd::Receive(void* lpBuf, int nBufLen, int nFlags )
{
	SetTimeOut(1000000);    // 1000s
	int nRecv = CSocket::Receive(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nRecv;
}

int CCSCmd::Send(const void* lpBuf, int nBufLen, int nFlags )
{
	SetTimeOut(100000);    //100s
	int nSend = CSocket::Send(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nSend;
}

BOOL CCSCmd::Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr /* = NULL */, int* lpSockAddrLen /* = NULL */)
{
	SetTimeOut(10000);
	BOOL nRecv = CSocket::Accept(rConnectedSocket, lpSockAddr, lpSockAddrLen);
	KillTimeOut();
	return nRecv;
}

void CCSCmd::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
}

void CCSCmd::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	//通过自定义消息WM_THREAD和消息参数WP_RECV通知线程去读取数据
	//由Socket绑定的线程通过消息去通知接收数据
	if (m_pThrd)
	{
		m_pThrd->PostThreadMessage(WM_USER_KYKYComm_Thread_Read, 0, 0);
	}
}
