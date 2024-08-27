#if !defined(AFX_IONPUMPCCG_H__C3E269D0_B53E_4E0D_BB7C_E785A5B55870__INCLUDED_)
#define AFX_IONPUMPCCG_H__C3E269D0_B53E_4E0D_BB7C_E785A5B55870__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IonPumpCCG.h : header file
//

#include <VACobtain.h>
#include "SerialPort.h"

/////////////////////////////////////////////////////////////////////////////
// CIonPumpCCG thread

class CIonPumpCCG : public CWinThread
{
	DECLARE_DYNCREATE(CIonPumpCCG)
protected:
	BOOL		m_bReady;

	DWORD		m_lastIPG;
	DWORD		m_lastIPC;
	DWORD		m_lastCCG;

	HANDLE		m_hEventKill;
	HANDLE		m_hEventDead;

	HWND		m_hMain;
	CfgVAC_CCGIPG	m_cfg;

	CSerialPort	m_IPG;
	CSerialPort	m_IPC;
	CSerialPort	m_CCG;

// Attributes
public:
	HANDLE	m_hFilIPG;
	HANDLE	m_hFilIPC;
	int		m_nIPGLoops;
	int		m_nIPCLoops;

// Operations
public:
	CIonPumpCCG();           // protected constructor used by dynamic creation
	BOOL	Close();
	void	ClearDevice();
	BOOL	Reset( CWnd* pWnd );
	BOOL	Init();
	BOOL	isReady(){ return m_bReady; };
	void	KillThread();

	void	SetParam( int index, int nValue );

	double	ScientificToDouble(CString strVal);
	void	ReceiveText( CString& strData );
	CString	ConvertReceiveData( CString strData );
	CString	ConvertComData( CString strData, BOOL bIonP1 = TRUE );

	int		ConvertReceiveData2( BYTE* rxdata, int nLen, CString& strOut );
	CString	ConvertReceiveData2( CString strIn );
	CString ConvertComData2( CString strIn, CByteArray& baData );

	CString ConvertCommData_Win( CString strWin, char cCmd, char cType, CString strData, CByteArray& baData );
	CString	ConvertReceiveData_Win( CString strIn );
	int		GetFieldLength_Win( CString strWin );
	void	LogIPG_IPC( int nType, CString strVacuum );

private:
	void	MainLoop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIonPumpCCG)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	virtual ~CIonPumpCCG();
	virtual void Delete();

	// Generated message map functions
	//{{AFX_MSG(CIonPumpCCG)
	afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IONPUMPCCG_H__C3E269D0_B53E_4E0D_BB7C_E785A5B55870__INCLUDED_)
