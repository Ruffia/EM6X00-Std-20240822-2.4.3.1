#if !defined(AFX_DLGSETUP_H__1F274613_CD32_42E7_8890_80E1E9C24F9D__INCLUDED_)
#define AFX_DLGSETUP_H__1F274613_CD32_42E7_8890_80E1E9C24F9D__INCLUDED_

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
	void	Localize();

	int		m_nPort, m_nPort2, m_nVentSel;
	float	m_fRyp, m_fH;
	BOOL	m_bClick, m_bTrack;
	char	m_cPathLocalize[256];
	CString	m_strMsg[10];	// ×Ö·û´®Êý×é
	BOOL	m_bAdvance;
	int		m_nStageType;

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_H__1F274613_CD32_42E7_8890_80E1E9C24F9D__INCLUDED_)
