#if !defined(AFX_DLGREMARK_H__5EA3CB0A_77CA_4C8A_96DF_32A084D16AF9__INCLUDED_)
#define AFX_DLGREMARK_H__5EA3CB0A_77CA_4C8A_96DF_32A084D16AF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRemark.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRemark dialog

class CDlgRemark : public CDialog
{
// Construction
public:
	CDlgRemark(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

// Dialog Data
	//{{AFX_DATA(CDlgRemark)
	enum { IDD = IDD_SEMCTL_Remark };
	CString	m_csRemark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRemark)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRemark)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREMARK_H__5EA3CB0A_77CA_4C8A_96DF_32A084D16AF9__INCLUDED_)
