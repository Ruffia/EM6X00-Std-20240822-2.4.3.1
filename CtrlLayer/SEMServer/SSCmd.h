#if !defined(AFX_SSCMD_H__7E28846B_D8E3_4C6D_8A13_1B6E21CF47DE__INCLUDED_)
#define AFX_SSCMD_H__7E28846B_D8E3_4C6D_8A13_1B6E21CF47DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SSCmd.h : header file
//
#include "afxsock.h"

/////////////////////////////////////////////////////////////////////////////
// CSSCmd command target

class CSSCmd : public CSocket
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
	CSSCmd();
	CSSCmd(HWND handle);
	virtual ~CSSCmd();

	virtual	BOOL	Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

private:
	BOOL	SetTimeOut(UINT uTimeOut);
	BOOL	KillTimeOut();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSCmd)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual BOOL OnMessagePending();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSSCmd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSCMD_H__7E28846B_D8E3_4C6D_8A13_1B6E21CF47DE__INCLUDED_)
