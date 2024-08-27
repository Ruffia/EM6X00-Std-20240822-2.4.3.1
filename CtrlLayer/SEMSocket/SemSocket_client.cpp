// SemSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SemSocket.h"
#include "..\\SEMServer\\SerSocketThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HANDLE g_hListen;
/////////////////////////////////////////////////////////////////////////////
// CServerSocket
CSEMSocket::CSEMSocket()
{
}

CSEMSocket::CSEMSocket(HWND handle)
{
	m_destHandle = handle;
}

CSEMSocket::~CSEMSocket()
{
	Close();
}

BOOL CSEMSocket::SetTimeOut(UINT uTimeOut)
{
	m_nTimerID = SetTimer(NULL, 0, uTimeOut, NULL);
	return m_nTimerID;
}

BOOL CSEMSocket::KillTimeOut()
{
	return KillTimer(NULL, m_nTimerID);
}

BOOL CSEMSocket::OnMessagePending()
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

int CSEMSocket::Receive(void* lpBuf, int nBufLen, int nFlags )
{
	SetTimeOut(1000000);    // 1000s
	int nRecv = CSocket::Receive(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nRecv;
}

int CSEMSocket::Send(const void* lpBuf, int nBufLen, int nFlags )
{
	SetTimeOut(100000);    //100s
	int nSend = CSocket::Send(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nSend;
}

BOOL CSEMSocket::Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr /* = NULL */, int* lpSockAddrLen /* = NULL */)
{
	SetTimeOut(10000);
	BOOL nRecv = CSocket::Accept(rConnectedSocket, lpSockAddr, lpSockAddrLen);
	KillTimeOut();
	return nRecv;
}

void CSEMSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
}

void CSEMSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	//通过自定义消息WM_THREAD和消息参数WP_RECV通知线程去读取数据
	//由Socket绑定的线程通过消息去通知接收数据
	if (m_pThrd)
	{
		m_pThrd->PostThreadMessage(WM_USER_KYKYComm_Thread_Read, 0, 0);
	}

}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSEMSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSEMSocket member functions
