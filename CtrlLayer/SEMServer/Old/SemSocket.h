#if !defined(AFX_SEMSOCKET_H__F9F4EEFF_D43A_490C_84D9_72CEB0FBF314__INCLUDED_)
#define AFX_SEMSOCKET_H__F9F4EEFF_D43A_490C_84D9_72CEB0FBF314__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemSocket.h : header file
//
#include "afxsock.h"

/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target

class CSEMSocket : public CSocket
{
// Operations
public:
    CSEMSocket();
	CSEMSocket(HWND handle);

	virtual	~CSEMSocket();
	virtual	void	OnAccept(int nErrorCode);   //Client�иú���ʵ�ֱ�ע�͵�����Ϊruntimeclass��ͻ
	virtual	void	OnReceive(int nErrorCode);
	virtual	int		Send(const void* lpBuf, int nBufLen, int nFlags = 0 );
	virtual	int		Receive(void* lpBuf, int nBufLen, int nFlags = 0 );
	virtual	BOOL	Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

protected:
	virtual	BOOL	OnMessagePending();

private:
	BOOL	SetTimeOut(UINT uTimeOut);
	BOOL	KillTimeOut();

// Attributes
public:
	HWND		m_destHandle;   
	CWinThread*	m_pThrd;		// ��CSocket���ڵ��߳�
//	int			m_thrdIndex;	// ��CSocket�����̵߳�index
	int			m_nPort;
	
private:
	int			m_nTimerID;

// Overrides
public:
// Implementation
//protected:
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMSOCKET_H__F9F4EEFF_D43A_490C_84D9_72CEB0FBF314__INCLUDED_)
