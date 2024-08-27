#if !defined(AFX_DLGAUTOTESTLNS_H__BED4A929_FEC6_4CA2_A4DD_183C241DE758__INCLUDED_)
#define AFX_DLGAUTOTESTLNS_H__BED4A929_FEC6_4CA2_A4DD_183C241DE758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTestLNS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestLNS dialog

class CDlgAutoTestLNS : public CDialog
{
// Construction
public:
	CDlgAutoTestLNS(CWnd* pParent = NULL);   // standard constructor

	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	InitParams();
	void	EnableParams( int nIndex, BOOL bEnable );

	int		m_nOrder;
	int		m_nChnIndex1, m_nChnIndex2;	// 1为记录位置数组的索引，2为6ADC通道索引
	CString	m_strRes;
	BOOL	m_bContinue;
	BOOL	m_bReverse;

	void	TimerChangeCL( UINT nIDEvent );
	void	TimerChangeOL( UINT nIDEvent );
	void	TimerChangeALX( UINT nIDEvent );
	void	TimerChangeALY( UINT nIDEvent );
	void	TimerChangeCLReverse( UINT nIDEvent );
	void	TimerChangeOLReverse( UINT nIDEvent );
	void	TimerChangeALXReverse( UINT nIDEvent );
	void	TimerChangeALYReverse( UINT nIDEvent );

	void	L1DACSB_Change( int nPos );
	void	L2SDACSB_Change( int nPos );
	void	XalDACSB_Change( int nPos );
	void	YalDACSB_Change( int nPos );

// Dialog Data
	//{{AFX_DATA(CDlgAutoTestLNS)
	enum { IDD = IDD_AutoTest_LNS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTestLNS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTestLNS)
	afx_msg void OnStart();
	afx_msg void OnStartCL();
	afx_msg void OnStartOL();
	afx_msg void OnStartALX();
	afx_msg void OnStartALY();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSaveGraph();
	afx_msg void OnStartCL2();
	afx_msg void OnStartOL2();
	afx_msg void OnStartALX2();
	afx_msg void OnStartALY2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOTESTLNS_H__BED4A929_FEC6_4CA2_A4DD_183C241DE758__INCLUDED_)
