#if !defined(AFX_DLGCALISCN_H__5722FC31_1643_4F56_A890_65F16C5B0F55__INCLUDED_)
#define AFX_DLGCALISCN_H__5722FC31_1643_4F56_A890_65F16C5B0F55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliSCN.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSCN dialog

class CDlgCaliSCN : public CDialog
{
// Construction
public:
	CDlgCaliSCN(CWnd* pParent = NULL);   // standard constructor

	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	InitParams();
	void	EnableParams( int nIndex, BOOL bEnable );
	void	Report( CString filename );

	int		m_nChnIndex1, m_nChnIndex2[4];	// 1为记录位置数组的索引，2为6ADC通道索引
	CString	m_strRes;

	void	TimerChangeStigX( UINT nIDEvent );
	void	TimerChangeStigY( UINT nIDEvent );
	void	TimerChangeShiftX( UINT nIDEvent );
	void	TimerChangeShiftY( UINT nIDEvent );

// Dialog Data
	//{{AFX_DATA(CDlgCaliSCN)
	enum { IDD = IDD_Cali_SCN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliSCN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliSCN)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSaveGraph();
	afx_msg void OnStartStigX();
	afx_msg void OnStartStigY();
	afx_msg void OnStartShiftX();
	afx_msg void OnStartShiftY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALISCN_H__5722FC31_1643_4F56_A890_65F16C5B0F55__INCLUDED_)
