#if !defined(AFX_DLGINPUTACQ_H__9AD6D62A_A9FC_4B14_8079_428E6DAEB636__INCLUDED_)
#define AFX_DLGINPUTACQ_H__9AD6D62A_A9FC_4B14_8079_428E6DAEB636__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlginputacq.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputAcq dialog

class CDlgInputAcq : public CDialog
{
// Construction
public:
	CDlgInputAcq(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

// Dialog Data
	//{{AFX_DATA(CDlgInputAcq)
	enum { IDD = IDD_DLG_InputACQ_ForSEM };
	int		m_nHV;
	int		m_nMag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputAcq)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputAcq)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTACQ_H__9AD6D62A_A9FC_4B14_8079_428E6DAEB636__INCLUDED_)
