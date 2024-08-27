#if !defined(AFX_DLGAUTOTESTSCN_H__1461EAF5_7028_470A_973D_9C6EE85D0BDC__INCLUDED_)
#define AFX_DLGAUTOTESTSCN_H__1461EAF5_7028_470A_973D_9C6EE85D0BDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTestSCN.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestSCN dialog

class CDlgAutoTestSCN : public CDialog
{
// Construction
public:
	CDlgAutoTestSCN(CWnd* pParent = NULL);   // standard constructor

	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	InitParams();
	void	EnableParams( int nIndex, BOOL bEnable );

	int		m_nOrder;
	int		m_nChnIndex1, m_nChnIndex2;	// 1为记录位置数组的索引，2为6ADC通道索引
	CString	m_strRes;
	BOOL	m_bContinue;
	BOOL	m_bReverse;

	void	TimerChangeStigX( UINT nIDEvent );
	void	TimerChangeStigY( UINT nIDEvent );
	void	TimerChangeShiftX( UINT nIDEvent );
	void	TimerChangeShiftY( UINT nIDEvent );
	void	TimerChangeStigXReverse( UINT nIDEvent );
	void	TimerChangeStigYReverse( UINT nIDEvent );
	void	TimerChangeShiftXReverse( UINT nIDEvent );
	void	TimerChangeShiftYReverse( UINT nIDEvent );

// Dialog Data
	//{{AFX_DATA(CDlgAutoTestSCN)
	enum { IDD = IDD_AutoTest_SCN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTestSCN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTestSCN)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSaveGraph();
	afx_msg void OnStartStigX();
	afx_msg void OnStartStigY();
	afx_msg void OnStartShiftX();
	afx_msg void OnStartShiftY();
	afx_msg void OnStartStigX2();
	afx_msg void OnStartStigY2();
	afx_msg void OnStartShiftX2();
	afx_msg void OnStartShiftY2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOTESTSCN_H__1461EAF5_7028_470A_973D_9C6EE85D0BDC__INCLUDED_)
