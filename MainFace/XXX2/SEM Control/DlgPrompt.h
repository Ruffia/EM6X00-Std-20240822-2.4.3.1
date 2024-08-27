#if !defined(AFX_DLGPROMPT_H__20370FC1_270A_460B_805F_400D8B3C864E__INCLUDED_)
#define AFX_DLGPROMPT_H__20370FC1_270A_460B_805F_400D8B3C864E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPrompt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPrompt dialog

class CDlgPrompt : public CDialog
{
// Construction
public:
	CDlgPrompt(CWnd* pParent = NULL);   // standard constructor
	void	AdjustCtrlsPos();
	void	Localize(void);

	BYTE	m_byteType;
	CString	m_strTitle;

// Dialog Data
	//{{AFX_DATA(CDlgPrompt)
	enum { IDD = IDD_DLG_Prompt };
	CString	m_strMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPrompt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPrompt)
	virtual BOOL OnInitDialog();
	afx_msg void OnYes();
	afx_msg void OnNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROMPT_H__20370FC1_270A_460B_805F_400D8B3C864E__INCLUDED_)
