#if !defined(AFX_DLGAUTOTESTPOWER_H__FC0C8B05_1E96_488C_851F_41C75AAB27A9__INCLUDED_)
#define AFX_DLGAUTOTESTPOWER_H__FC0C8B05_1E96_488C_851F_41C75AAB27A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTestPower.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestPower dialog

class CDlgAutoTestPower : public CDialog
{
// Construction
public:
	CDlgAutoTestPower(CWnd* pParent = NULL);   // standard constructor
	~CDlgAutoTestPower();
	void	Start1();
	void	Start2();
	void	Start3();
	void	StartSCN();
	void	StartLNS();
	void	StartVAC();
	void	StartSPR();
	void	StartSPR01();
	void	StartSPR02();
	void	StartSPR1();
	void	StartMCI();
	BOOL	StartReadAnalyze( int nMID, logParam* lP );	// ADC in Sub-boards
	BOOL	StartReadAnalyze( unsigned short* pPwr );	// MCI Pwr
	void	PwrVconV( unsigned short nPwr );

	void	InitParams();

	CBrush	m_brushError;

// Dialog Data
	//{{AFX_DATA(CDlgAutoTestPower)
	enum { IDD = IDD_AutoTest_Power };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTestPower)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTestPower)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStart();
	afx_msg void OnClear();
	afx_msg void OnReport();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOTESTPOWER_H__FC0C8B05_1E96_488C_851F_41C75AAB27A9__INCLUDED_)
