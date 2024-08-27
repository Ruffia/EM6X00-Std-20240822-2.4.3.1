#if !defined(AFX_DLGCALISPR_H__934D08C9_808E_4229_B5AF_2761C664CEE9__INCLUDED_)
#define AFX_DLGCALISPR_H__934D08C9_808E_4229_B5AF_2761C664CEE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliSPR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSPR dialog

class CDlgCaliSPR : public CDialog
{
// Construction
public:
	CDlgCaliSPR(CWnd* pParent = NULL);   // standard constructor

	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	InitParams();
	void	EnableParams( int nIndex, BOOL bEnable );
	void	Report( CString filename );

	int		m_nOrder;
	int		m_nChnIndex1, m_nChnIndex2[4];	// 1为记录位置数组的索引，2为6ADC通道索引
	CString	m_strRes;

	void	TimerChangeContrast( UINT nIDEvent );

// Dialog Data
	//{{AFX_DATA(CDlgCaliSPR)
	enum { IDD = IDD_Cali_SPR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliSPR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliSPR)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSaveGraph();
	afx_msg void OnStartContrast();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALISPR_H__934D08C9_808E_4229_B5AF_2761C664CEE9__INCLUDED_)
