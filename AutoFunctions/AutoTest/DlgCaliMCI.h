#if !defined(AFX_DLGCALIMCI_H__EBB1413F_4BA1_4442_AB36_34903884B421__INCLUDED_)
#define AFX_DLGCALIMCI_H__EBB1413F_4BA1_4442_AB36_34903884B421__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliMCI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliMCI dialog

class CDlgCaliMCI : public CDialog
{
// Construction
public:
	CDlgCaliMCI(CWnd* pParent = NULL);   // standard constructor

	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	InitParams();
	void	EnableParams( int nIndex, BOOL bEnable );
	void	Report( CString filename );

	int		m_nChnIndex1, m_nChnIndex2[4];	// 1为记录位置数组的索引，2为6ADC通道索引
	CString	m_strRes;

	void	TimerChangeFineMagX( UINT nIDEvent );
	void	TimerChangeFineMagY( UINT nIDEvent );
	void	TimerChangeMCIShiftX( UINT nIDEvent );
	void	TimerChangeMCIShiftY( UINT nIDEvent );

// Dialog Data
	//{{AFX_DATA(CDlgCaliMCI)
	enum { IDD = IDD_Cali_MCI };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliMCI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliMCI)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSaveGraph();
	afx_msg void OnStartFineMagX();
	afx_msg void OnStartFineMagY();
	afx_msg void OnStartShiftX();
	afx_msg void OnStartShiftY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALIMCI_H__EBB1413F_4BA1_4442_AB36_34903884B421__INCLUDED_)
