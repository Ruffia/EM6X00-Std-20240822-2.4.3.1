#if !defined(AFX_DLGCALICOADJUST_H__4DB0A3EE_6413_42A7_9E6E_D9D558F8D1E8__INCLUDED_)
#define AFX_DLGCALICOADJUST_H__4DB0A3EE_6413_42A7_9E6E_D9D558F8D1E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliCoadjust.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliCoadjust dialog

class CDlgCaliCoadjust : public CDialog
{
// Construction
public:
	CDlgCaliCoadjust(CWnd* pParent = NULL);   // standard constructor
	void	UpdateCaliData();

	double	m_dCo_Ka;
	double	m_dCo_Kb;

// Dialog Data
	//{{AFX_DATA(CDlgCaliCoadjust)
	enum { IDD = IDD_Cali_Coadjust };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliCoadjust)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliCoadjust)
	afx_msg void OnBUTTONATCaliCoadjust1();
	afx_msg void OnBUTTONATCaliCoadjust2();
	afx_msg void OnBUTTONATCaliCoadjust3();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckChanged(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALICOADJUST_H__4DB0A3EE_6413_42A7_9E6E_D9D558F8D1E8__INCLUDED_)
