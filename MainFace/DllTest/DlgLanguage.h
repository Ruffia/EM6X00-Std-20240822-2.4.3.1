#if !defined(AFX_DLGLANGUAGE_H__C79252BE_450C_4EA4_A642_0C51F82DF664__INCLUDED_)
#define AFX_DLGLANGUAGE_H__C79252BE_450C_4EA4_A642_0C51F82DF664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLanguage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLanguage dialog

class CDlgLanguage : public CDialog
{
// Construction
public:
	CDlgLanguage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLanguage)
	enum { IDD = IDD_DIALOG_Language };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLanguage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLanguage)
	afx_msg void OnBUTTONBrowsehFile();
	afx_msg void OnBUTTONBrowsercFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLANGUAGE_H__C79252BE_450C_4EA4_A642_0C51F82DF664__INCLUDED_)
