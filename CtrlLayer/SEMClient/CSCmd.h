#if !defined(AFX_CSCMD_H__A6B4024C_37AE_4712_B7F2_455605437E71__INCLUDED_)
#define AFX_CSCMD_H__A6B4024C_37AE_4712_B7F2_455605437E71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSCmd.h : header file
//
#include "afxsock.h"

/////////////////////////////////////////////////////////////////////////////
// CCSCmd command target

class CCSCmd : public CSocket
{
// Attributes
public:
	HWND		m_destHandle;   
	CWinThread*	m_pThrd;		// 该CSocket所在的线程
//	int			m_thrdIndex;	// 该CSocket所属线程的index
	int			m_nPort;

private:
	int			m_nTimerID;

// Operations
public:
	CCSCmd();
	CCSCmd(HWND handle);
	virtual ~CCSCmd();

	virtual	BOOL	Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

private:
	BOOL	SetTimeOut(UINT uTimeOut);
	BOOL	KillTimeOut();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSCmd)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual BOOL OnMessagePending();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CCSCmd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSCMD_H__A6B4024C_37AE_4712_B7F2_455605437E71__INCLUDED_)
