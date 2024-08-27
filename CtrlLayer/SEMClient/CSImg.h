#if !defined(AFX_CSIMG_H__C5BC8A30_79AD_4840_95FF_AA8287ADA4AA__INCLUDED_)
#define AFX_CSIMG_H__C5BC8A30_79AD_4840_95FF_AA8287ADA4AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSImg.h : header file
//
#include "afxsock.h"

/////////////////////////////////////////////////////////////////////////////
// CCSImg command target

class CCSImg : public CSocket
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
	CCSImg();
	CCSImg(HWND handle);
	virtual ~CCSImg();

	virtual	BOOL	Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

private:
	BOOL	SetTimeOut(UINT uTimeOut);
	BOOL	KillTimeOut();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSImg)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual BOOL OnMessagePending();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CCSImg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSIMG_H__C5BC8A30_79AD_4840_95FF_AA8287ADA4AA__INCLUDED_)
