#if !defined(AFX_DLGMOBILEUSCALE_H__5B9E9158_FFB6_463A_A348_8A3B7BDC394A__INCLUDED_)
#define AFX_DLGMOBILEUSCALE_H__5B9E9158_FFB6_463A_A348_8A3B7BDC394A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMobileUScale.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMobileUScale dialog

class CDlgMobileUScale : public CDialog
{
// Construction
public:
	CDlgMobileUScale(CWnd* pParent = NULL);   // standard constructor
	void	CoherenceCheck();
	void	MagChange();

// Dialog Data
	//{{AFX_DATA(CDlgMobileUScale)
	enum { IDD = IDD_DIALOG_MobileUScale };
	CSpinButtonCtrl	m_Mag;
	CSpinButtonCtrl	m_Mag10;
	CSpinButtonCtrl	m_Mag1000;
	double	m_dMag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMobileUScale)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMobileUScale)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDeltaposSPINMag1000(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINMag10(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINMag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOBILEUSCALE_H__5B9E9158_FFB6_463A_A348_8A3B7BDC394A__INCLUDED_)
