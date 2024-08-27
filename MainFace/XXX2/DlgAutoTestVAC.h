#if !defined(AFX_DLGAUTOTESTVAC_H__3634758D_506A_457E_BC25_901985FA80E5__INCLUDED_)
#define AFX_DLGAUTOTESTVAC_H__3634758D_506A_457E_BC25_901985FA80E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTestVAC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestVAC dialog

class CDlgAutoTestVAC : public CDialog
{
// Construction
public:
	CDlgAutoTestVAC(CWnd* pParent = NULL);   // standard constructor
	~CDlgAutoTestVAC();

	void InitParams();

	CBrush m_brushError;

// Dialog Data
	//{{AFX_DATA(CDlgAutoTestVAC)
	enum { IDD = IDD_AutoTest_VAC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTestVAC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTestVAC)
	afx_msg void OnStart();
	afx_msg void OnClear();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOTESTVAC_H__3634758D_506A_457E_BC25_901985FA80E5__INCLUDED_)
