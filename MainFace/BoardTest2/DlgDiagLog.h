#if !defined(AFX_DLGDIAGLOG_H__7E9D9594_94EF_45FF_BD3F_39572B9F278B__INCLUDED_)
#define AFX_DLGDIAGLOG_H__7E9D9594_94EF_45FF_BD3F_39572B9F278B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDiagLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDiagLog dialog

class CDlgDiagLog : public CDialog
{
// Construction
public:
	CDlgDiagLog(CWnd* pParent = NULL);   // standard constructor

	void	InitReadADCsParams();
	void	InitSetChangingParams();
	void	InitChangingParams();
	void	InitLoggingParams();
	void	SaveSetChangingParams();
	void	SaveChangingParams( char* path );
	void	SaveLoggingParams( char* path );

// Dialog Data
	//{{AFX_DATA(CDlgDiagLog)
	enum { IDD = IDD_Diagnose_Logging_Setup };
	int		m_nAveCounts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDiagLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDiagLog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClearAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDIAGLOG_H__7E9D9594_94EF_45FF_BD3F_39572B9F278B__INCLUDED_)
