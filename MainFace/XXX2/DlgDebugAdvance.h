#if !defined(AFX_DLGDEBUGADVANCE_H__E86CCB35_003E_416E_80E1_C40F752419B9__INCLUDED_)
#define AFX_DLGDEBUGADVANCE_H__E86CCB35_003E_416E_80E1_C40F752419B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDebugAdvance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugAdvance dialog

class CDlgDebugAdvance : public CDialog
{
// Construction
public:
	CDlgDebugAdvance(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

	/////////////////////////////////////////////
	void	WobDACSB_Change( int nPos, BOOL bUpdate );
	void	WobDACSB_Change2( int nPos );
	void	WobDAC_Output( int nValue );
	void	WobFreqSB_Change( int nPos );
	void	CalcWobStep();

	int		m_nAutoWobblerMin, m_nAutoWobblerMax, m_nAutoWobblerStep;
	BOOL	m_bAutoWobblerOn, m_bAutoWobblerDir;	// direction
	BOOL	m_bL2Comp;
	int		m_nAutoWobblerLast;
	int		m_nTimerWob;
	/////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CDlgDebugAdvance)
	enum { IDD = IDD_Debug_Advance };
	CIPAddressCtrl	m_ipAddress;
	BOOL	m_bLEDs;
	CScrollBar	m_scrollWobDAC;
	CScrollBar	m_scrollWobFreq;
	//}}AFX_DATA

	BOOL	m_bLEDsbak;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDebugAdvance)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDebugAdvance)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnReadBoardIDs();
	afx_msg void OnCHECKTestLEDs();
	afx_msg void OnBUTTONResetSubBoards();
	afx_msg void OnBUTTONResetFT232();
	afx_msg void OnBUTTONResetDI16A();
	afx_msg void OnBUTTONDiagnose();
	afx_msg void OnBUTTONDiagnoseSetup();
	afx_msg void OnL2CompDisable();
	afx_msg void OnSetWobMinCmd();
	afx_msg void OnSetAutoWobbleCmd();
	afx_msg void OnSetWobMaxCmd();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEBUGADVANCE_H__E86CCB35_003E_416E_80E1_C40F752419B9__INCLUDED_)
