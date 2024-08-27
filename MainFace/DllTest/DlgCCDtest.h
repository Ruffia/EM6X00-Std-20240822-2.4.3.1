#if !defined(AFX_DLGCCDTEST_H__EDAE7D36_B113_4EEF_80CF_92DA09C46AEE__INCLUDED_)
#define AFX_DLGCCDTEST_H__EDAE7D36_B113_4EEF_80CF_92DA09C46AEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCCDtest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCCDtest dialog

class CDlgCCDtest : public CDialog
{
// Construction
public:
	CDlgCCDtest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCCDtest)
	enum { IDD = IDD_DIALOG_CCDtest };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCCDtest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCCDtest)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCCDTEST_H__EDAE7D36_B113_4EEF_80CF_92DA09C46AEE__INCLUDED_)
