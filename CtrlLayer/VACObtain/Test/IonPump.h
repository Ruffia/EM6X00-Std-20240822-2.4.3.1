#if !defined(AFX_IONPUMP_H__A8A74A7A_4A16_40CF_BCA0_2AA13F556F25__INCLUDED_)
#define AFX_IONPUMP_H__A8A74A7A_4A16_40CF_BCA0_2AA13F556F25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IonPump.h : header file
//

#include <VACobtain.h>
#include "Serial.h"

/////////////////////////////////////////////////////////////////////////////
// CIonPump thread

class CIonPump : public CWinThread
{
	DECLARE_DYNCREATE(CIonPump)
protected:
	HANDLE		m_hEventKill;
	HANDLE		m_hEventDead;

	HWND		m_hMain;
	CfgVAC_IP_CCG	m_cfg;
	DWORD		m_lastTicks;

	CSerial		m_IP;
	CString		m_strData, m_strDataSaved;

// Attributes
public:
	HANDLE	m_hFil;
	int		m_nLoops;

// Operations
public:
	CIonPump();           // protected constructor used by dynamic creation
	BOOL	Close();
	void	ClearDevice();
	BOOL	Reset( CWnd* pWnd );
	BOOL	Init();
	BOOL	isReady(){ return m_cfg.bReady; };
	void	KillThread();

	void	SetParam( int index, int nValue );

	double	ScientificToDouble(CString strVal);
	double	ScientificToDouble(CString strVal, double& dValue, int& nExp);

	void	ReceiveText( CString& strData );
	int		ConvertReceiveData( CString strData, CString& strOut );
	CString	ConvertComData( CString strData, BOOL bIonP1 = TRUE );

	int		ConvertReceiveData2( BYTE* rxdata, int nLen, CString& strOut );
	int		ConvertReceiveData2( CString strIn, CString& strOut );
	CString ConvertComData2( CString strIn, CByteArray& baData );
	int		ConvertComData22( CString strIn, char* data );

	CString ConvertCommData_Win( CString strWin, char cCmd, char cType, CString strData, CByteArray& baData );
	int		ConvertReceiveData_Win( CString strIn, CString& strOut );
	int		GetFieldLength_Win( CString strWin );

	void	LogIPG_IPC( int nType, CString strVacuum );

	int		AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp );

private:
	void	MainLoop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIonPump)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIonPump();
	virtual void Delete();

protected:
	// Generated message map functions
	//{{AFX_MSG(CIonPump)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IONPUMP_H__A8A74A7A_4A16_40CF_BCA0_2AA13F556F25__INCLUDED_)
