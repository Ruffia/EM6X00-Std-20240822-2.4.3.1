#if !defined(AFX_CSIMGTHREAD_H__85E3F359_6ECC_42DE_8F9C_80BC2C84C2C6__INCLUDED_)
#define AFX_CSIMGTHREAD_H__85E3F359_6ECC_42DE_8F9C_80BC2C84C2C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSImgThread.h : header file
//

#include "CSImg.h"
#include "CommFunctions.h"

/////////////////////////////////////////////////////////////////////////////
// CCSImgThread thread

class CCSImg;
class CCSImgThread : public CWinThread
{
	DECLARE_DYNCREATE(CCSImgThread)
protected:
	CCSImgThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CString		m_strIPAddr;
	CCSImg*		m_pCSocketImg;		// 附加在这个线程上的CSocket
	int			m_nConnectedImg;	// 已和服务端连接为1，未连接为0

// Operations
public:
	virtual ~CCSImgThread();
	void	SetServerIP(CString sIP);
	void	Reset(LPVOID ptr);
	bool	GetServerStatus(int* status);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSImgThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCSImgThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMsgRead(WPARAM wParam,LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSIMGTHREAD_H__85E3F359_6ECC_42DE_8F9C_80BC2C84C2C6__INCLUDED_)
