#if !defined(AFX_CCG_H__F65CAE6E_BBB4_440C_B267_0DF1917A769A__INCLUDED_)
#define AFX_CCG_H__F65CAE6E_BBB4_440C_B267_0DF1917A769A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ccg.h : header file
//

#include <VACobtain.h>
#include "Serial.h"

/////////////////////////////////////////////////////////////////////////////
// CCcg thread

class CCcg : public CWinThread
{
	DECLARE_DYNCREATE(CCcg)
protected:
	HANDLE		m_hEventKill;
	HANDLE		m_hEventDead;

	HWND		m_hMain;
	CfgVAC_IP_CCG	m_cfg;
	DWORD		m_lastTicks;

	CSerial		m_CCG;

// Attributes
public:
	HANDLE	m_hFil;
	int		m_nLoops;

// Operations
public:
	CCcg();           // protected constructor used by dynamic creation
	BOOL	Close();
	void	ClearDevice();
	BOOL	Reset( CWnd* pWnd );
	BOOL	Init();
	BOOL	isReady(){ return m_cfg.bReady; };
	void	KillThread();

	void	SetParam( int index, int nValue );

private:
	void	MainLoop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCcg)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	virtual ~CCcg();
	virtual void Delete();

	// Generated message map functions
	//{{AFX_MSG(CCcg)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCG_H__F65CAE6E_BBB4_440C_B267_0DF1917A769A__INCLUDED_)
