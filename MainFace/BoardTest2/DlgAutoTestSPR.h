#if !defined(AFX_DLGAUTOTESTSPR_H__69B94760_108F_4ACB_A407_E1C32A1F3153__INCLUDED_)
#define AFX_DLGAUTOTESTSPR_H__69B94760_108F_4ACB_A407_E1C32A1F3153__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTestSPR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestSPR dialog

class CDlgAutoTestSPR : public CDialog
{
// Construction
public:
	CDlgAutoTestSPR(CWnd* pParent = NULL);   // standard constructor

	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	InitParams();
	void	EnableParams( int nIndex, BOOL bEnable );

	int		m_nOrder;
	int		m_nChnIndex1, m_nChnIndex2;	// 1为记录位置数组的索引，2为6ADC通道索引
	int		m_nSegIndex;				// 在做哪个功能的AutoTest
	CString	m_strRes;
	BOOL	m_bContinue;
	BOOL	m_bReverse;

	void	TimerChangeContrast( UINT nIDEvent );
	void	TimerChangeContrastReverse( UINT nIDEvent );

	void	SPRDACupdate( int nPos );

// Dialog Data
	//{{AFX_DATA(CDlgAutoTestSPR)
	enum { IDD = IDD_AutoTest_SPR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTestSPR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTestSPR)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSaveGraph();
	afx_msg void OnStartContrast();
	afx_msg void OnStartContrast2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOTESTSPR_H__69B94760_108F_4ACB_A407_E1C32A1F3153__INCLUDED_)
