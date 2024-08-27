#if !defined(AFX_DLGAUTOTESTMCI_H__1A5CA54F_F96B_4C1D_8497_0250EBC369B2__INCLUDED_)
#define AFX_DLGAUTOTESTMCI_H__1A5CA54F_F96B_4C1D_8497_0250EBC369B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTestMCI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestMCI dialog

class CDlgAutoTestMCI : public CDialog
{
// Construction
public:
	CDlgAutoTestMCI(CWnd* pParent = NULL);   // standard constructor

	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	InitParams();
	void	EnableParams( int nIndex, BOOL bEnable );

	int		m_nOrder;
	int		m_nChnIndex1, m_nChnIndex2;	// 1为记录位置数组的索引，2为6ADC通道索引
	int		m_nSegIndex;				// 在做哪个功能的AutoTest
	CString	m_strRes;
	BOOL	m_bContinue;
	BOOL	m_bReverse;

	void	TimerChangeFineMagX( UINT nIDEvent );
	void	TimerChangeFineMagY( UINT nIDEvent );
	void	TimerChangeMCIShiftX( UINT nIDEvent );
	void	TimerChangeMCIShiftY( UINT nIDEvent );
	void	TimerChangeFineMagXReverse( UINT nIDEvent );
	void	TimerChangeFineMagYReverse( UINT nIDEvent );
	void	TimerChangeMCIShiftXReverse( UINT nIDEvent );
	void	TimerChangeMCIShiftYReverse( UINT nIDEvent );

	void	Det45SB_Change( int nPos, int nChn );
	void	SegDShiftXSB_Change( int nPos );
	void	SegDShiftYSB_Change( int nPos );
	void	XshiftDACSB_Change( int nPos );
	void	YshiftDACSB_Change( int nPos );

// Dialog Data
	//{{AFX_DATA(CDlgAutoTestMCI)
	enum { IDD = IDD_AutoTest_MCI };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTestMCI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTestMCI)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSaveGraph();
	afx_msg void OnStartFineMagX();
	afx_msg void OnStartFineMagY();
	afx_msg void OnStartShiftX();
	afx_msg void OnStartShiftY();
	afx_msg void OnStartFineMagX2();
	afx_msg void OnStartFineMagY2();
	afx_msg void OnStartShiftX2();
	afx_msg void OnStartShiftY2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOTESTMCI_H__1A5CA54F_F96B_4C1D_8497_0250EBC369B2__INCLUDED_)
