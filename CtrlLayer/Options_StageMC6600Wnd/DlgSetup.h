#if !defined(AFX_DLGSETUP_H__5692744E_B99F_4412_BF38_E7B8DEC4E045__INCLUDED_)
#define AFX_DLGSETUP_H__5692744E_B99F_4412_BF38_E7B8DEC4E045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup dialog

class CDlgSetup : public CDialog
{
// Construction
public:
	CDlgSetup(CWnd* pParent = NULL);   // standard constructor
	~CDlgSetup();
	void	Localize();

	int		m_nPort, m_nVentSel;
	float	m_fRy, m_fHy, m_fHt ,m_fZsink, m_fSafeWD, m_fHs;
	BOOL	m_bClick, m_bTrack;
	char	m_cPathLocalize[256];
	CString	m_strMsg[10];	// ×Ö·û´®Êý×é
	BOOL	m_bAdvance;
	BOOL	m_bDbClick;

// Dialog Data
	//{{AFX_DATA(CDlgSetup)
	enum { IDD = IDD_PANEL_Setup };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBUTTONLoadDefault();
	afx_msg void OnBnClickedButtonAddress();
	afx_msg void OnStnClickedStaticZtwd();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonRelink();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_H__5692744E_B99F_4412_BF38_E7B8DEC4E045__INCLUDED_)
