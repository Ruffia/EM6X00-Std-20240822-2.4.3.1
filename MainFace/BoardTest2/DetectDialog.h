#if !defined(AFX_DETECTDIALOG_H__465E753F_ED9C_40E0_B644_88DFAE627CE2__INCLUDED_)
#define AFX_DETECTDIALOG_H__465E753F_ED9C_40E0_B644_88DFAE627CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetectDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DetectDialog dialog

class DetectDialog : public CDialog
{
// Construction
public:
	DetectDialog(CWnd* pParent = NULL);   // standard constructor
	int		index;
	bool	bFoundFT232;
	bool	bFoundDI16A;

// Dialog Data
	//{{AFX_DATA(DetectDialog)
	enum { IDD = IDD_DETECT_DIALOG };
	CListBox	m_deviceList;
	CString	m_message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DetectDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DetectDialog)
	afx_msg void OnDetectBtn();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETECTDIALOG_H__465E753F_ED9C_40E0_B644_88DFAE627CE2__INCLUDED_)
