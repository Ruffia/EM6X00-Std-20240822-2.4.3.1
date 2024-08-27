#if !defined(AFX_DLGINPUT_H__FF5E3CDC_A203_49C8_AA94_8F307765633B__INCLUDED_)
#define AFX_DLGINPUT_H__FF5E3CDC_A203_49C8_AA94_8F307765633B__INCLUDED_

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
	void	Localize();

	BOOL	m_bPassword;
	CString m_strInput;
	char	m_cPathLocalize[256];

// Dialog Data
	//{{AFX_DATA(CDlgInput)
	enum { IDD = IDD_DIALOG_Input };
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

#endif // !defined(AFX_DLGINPUT_H__FF5E3CDC_A203_49C8_AA94_8F307765633B__INCLUDED_)
