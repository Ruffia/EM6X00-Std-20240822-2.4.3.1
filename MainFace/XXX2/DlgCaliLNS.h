#if !defined(AFX_DLGCALILNS_H__F3B6DC2B_57E6_4174_973B_C1035D13381C__INCLUDED_)
#define AFX_DLGCALILNS_H__F3B6DC2B_57E6_4174_973B_C1035D13381C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliLNS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliLNS dialog

class CDlgCaliLNS : public CDialog
{
// Construction
public:
	CDlgCaliLNS(CWnd* pParent = NULL);   // standard constructor

	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	InitParams();
	void	EnableParams( int nIndex, BOOL bEnable );
	void	Report( CString filename );

	int		m_nChnIndex1, m_nChnIndex2[4];	// 1为记录位置数组的索引，2为6ADC通道索引
	CString	m_strRes;

	void	TimerChangeCL( UINT nIDEvent );
	void	TimerChangeOL( UINT nIDEvent );
	void	TimerChangeALX( UINT nIDEvent );
	void	TimerChangeALY( UINT nIDEvent );

// Dialog Data
	//{{AFX_DATA(CDlgCaliLNS)
	enum { IDD = IDD_Cali_LNS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliLNS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliLNS)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSaveGraph();
	afx_msg void OnStartCL();
	afx_msg void OnStartOL();
	afx_msg void OnStartALX();
	afx_msg void OnStartALY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALILNS_H__F3B6DC2B_57E6_4174_973B_C1035D13381C__INCLUDED_)
