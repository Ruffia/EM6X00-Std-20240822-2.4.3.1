#if !defined(AFX_DLGINPUT_H__2F0097B4_B378_443A_B550_EFAB914DD543__INCLUDED_)
#define AFX_DLGINPUT_H__2F0097B4_B378_443A_B550_EFAB914DD543__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInput dialog

class CDlgInput : public CDialog
{
// Construction
public:
	CDlgInput(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

	int		m_nTitleIndex;
	CString	m_strInput;

// Dialog Data
	//{{AFX_DATA(CDlgInput)
	enum { IDD = IDD_Input };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInput)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUT_H__2F0097B4_B378_443A_B550_EFAB914DD543__INCLUDED_)
