#if !defined(AFX_SEMCTRLDLGEDIT_H__760ED657_1330_4176_B25E_0A3550B3A70E__INCLUDED_)
#define AFX_SEMCTRLDLGEDIT_H__760ED657_1330_4176_B25E_0A3550B3A70E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemCtrlDlgEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgEdit dialog

class CSemCtrlDlgEdit : public CDialog
{
// Construction
public:
	CSemCtrlDlgEdit(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

	CPoint	m_ptPos;
	void	ClearTextBuff();

// Dialog Data
	//{{AFX_DATA(CSemCtrlDlgEdit)
	enum { IDD = IDD_SEMCTL_EDIT };
	CString	m_csText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCtrlDlgEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSemCtrlDlgEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCTRLDLGEDIT_H__760ED657_1330_4176_B25E_0A3550B3A70E__INCLUDED_)
