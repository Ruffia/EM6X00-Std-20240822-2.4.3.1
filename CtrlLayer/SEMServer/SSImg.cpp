// SSImg.cpp : implementation file
//

#include "stdafx.h"
#include "SSImg.h"
#include "SSImgThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSImg

CSSImg::CSSImg()
{
}

CSSImg::CSSImg(HWND handle)
{
	m_destHandle = handle;
}

CSSImg::~CSSImg()
{
	Close();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSSImg, CSocket)
	//{{AFX_MSG_MAP(CSSImg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSSImg member functions

BOOL CSSImg::SetTimeOut(UINT uTimeOut)
{
	m_nTimerID = SetTimer(NULL, 0, uTimeOut, NULL);
	return m_nTimerID;
}

BOOL CSSImg::KillTimeOut()
{
	return KillTimer(NULL, m_nTimerID);
}

BOOL CSSImg::OnMessagePending()
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

int CSSImg::Receive(void* lpBuf, int nBufLen, int nFlags )
{
	SetTimeOut(1000000);   //1000s
	int nRecv = CSocket::Receive(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nRecv;
}

int CSSImg::Send(const void* lpBuf, int nBufLen, int nFlags )
{
	SetTimeOut(100000);  //100s
	int nSend = CSocket::Send(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nSend;
}

BOOL CSSImg::Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr /* = NULL */, int* lpSockAddrLen /* = NULL */)
{
	SetTimeOut(10000);
	BOOL nRecv = CSocket::Accept(rConnectedSocket, lpSockAddr, lpSockAddrLen);
	KillTimeOut();
	return nRecv;
}

void CSSImg::OnAccept(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		//����һ������Socket
		CSSImg connectSocket(m_destHandle);
		Accept(connectSocket);    //connectSocket���Ϳͻ�������
		//����һ���߳�
		CSSImgThread* pThread = (CSSImgThread*)AfxBeginThread(RUNTIME_CLASS(CSSImgThread),
								THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

		pThread->m_destHandle	= m_destHandle;
		pThread->m_nPort		= m_nPort;
		PostMessage(m_destHandle,WM_USER_KYKYComm_Thread_Send_Connect,1,LPARAM(pThread));   //���������̵߳ľ����������

		pThread->m_hSocket = connectSocket.Detach();
		//�̱߳�ţ�ͨ�������ͨ��ʱ���ֲ�ͬ�Ŀͻ���
		//�������������̣߳���ͻ�������ͨ�ŵ�Socket? ͨ�����������̣߳��е�ID��ʶ
//		pThread->m_thrdIndex = m_pSerDlg->m_thrdIndex++;
		//�����߳�
		pThread->ResumeThread();
		connectSocket.Close();
		pThread->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_Connect_Success, 0);
	}

	CSocket::OnAccept(nErrorCode);
}

void CSSImg::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	//ͨ���Զ�����ϢWM_THREAD����Ϣ����WP_RECV֪ͨ�߳�ȥ��ȡ����
	//��Socket�󶨵��߳�ͨ����Ϣȥ֪ͨ��������
	if (m_pThrd)
	{
		m_pThrd->PostThreadMessage(WM_USER_KYKYComm_Thread_Read, 0, 0);
	}
}
