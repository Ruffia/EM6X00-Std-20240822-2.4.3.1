// SSCmd.cpp : implementation file
//

#include "stdafx.h"
#include "SSCmd.h"
#include "SSCmdThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSCmd

CSSCmd::CSSCmd()
{
}

CSSCmd::CSSCmd(HWND handle)
{
	m_destHandle = handle;
}

CSSCmd::~CSSCmd()
{
	Close();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSSCmd, CSocket)
	//{{AFX_MSG_MAP(CSSCmd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSSCmd member functions

BOOL CSSCmd::SetTimeOut(UINT uTimeOut)
{
	m_nTimerID = SetTimer(NULL, 0, uTimeOut, NULL);
	return m_nTimerID;
}

BOOL CSSCmd::KillTimeOut()
{
	return KillTimer(NULL, m_nTimerID);
}

BOOL CSSCmd::OnMessagePending()
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
	{
		if (msg.wParam == (UINT)m_nTimerID)
		{
			//Remove the message and call CancelBlockingCall
			::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
			return FALSE;   //no need for idle time processing
		}
	}
	return CSocket::OnMessagePending();
}

int CSSCmd::Receive(void* lpBuf, int nBufLen, int nFlags )
{
	SetTimeOut(1000000);   //1000s
	int nRecv = CSocket::Receive(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nRecv;
}

int CSSCmd::Send(const void* lpBuf, int nBufLen, int nFlags )
{
	SetTimeOut(100000);  //100s
	int nSend = CSocket::Send(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nSend;
}

BOOL CSSCmd::Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr /* = NULL */, int* lpSockAddrLen /* = NULL */)
{
	SetTimeOut(10000);
	BOOL nRecv = CSocket::Accept(rConnectedSocket, lpSockAddr, lpSockAddrLen);
	KillTimeOut();
	return nRecv;
}

void CSSCmd::OnAccept(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		//创建一个连接Socket
		CSSCmd connectSocket(m_destHandle);
		Accept(connectSocket);    //connectSocket将和客户端连接
		//创建一个线程
		CSSCmdThread* pThread = (CSSCmdThread*)AfxBeginThread(RUNTIME_CLASS(CSSCmdThread),
								THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

		pThread->m_destHandle	= m_destHandle;
		pThread->m_nPort		= m_nPort;
		PostMessage(m_destHandle,WM_USER_KYKYComm_Thread_Send_Connect,0,LPARAM(pThread));   //发送连接线程的句柄给主程序

		pThread->m_hSocket = connectSocket.Detach();
		//线程编号，通过这个在通信时区分不同的客户端
		//如何区别各工作线程（与客户端连接通信的Socket? 通过主程序（主线程）中的ID标识
//		pThread->m_thrdIndex = m_pSerDlg->m_thrdIndex++;
		//启动线程
		pThread->ResumeThread();
		connectSocket.Close();
		pThread->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_Connect_Success, 0);
	}
	
	CSocket::OnAccept(nErrorCode);
}

void CSSCmd::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	//通过自定义消息WM_THREAD和消息参数WP_RECV通知线程去读取数据
	//由Socket绑定的线程通过消息去通知接收数据
	if (m_pThrd)
	{
		m_pThrd->PostThreadMessage(WM_USER_KYKYComm_Thread_Read, 0, 0);
	}
}
