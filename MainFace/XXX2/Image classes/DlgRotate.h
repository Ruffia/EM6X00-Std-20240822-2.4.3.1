#if !defined(AFX_DLGROTATE_H__4DDB3383_9804_11D5_8DD4_D76CCF55323D__INCLUDED_)
#define AFX_DLGROTATE_H__4DDB3383_9804_11D5_8DD4_D76CCF55323D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRotate.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgRotate dialog
class CDlgRotate : public CDialog
{
// Construction
public:
	long m_method;
	long m_overflow;
	BYTE m_keepsize;
	CDlgRotate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRotate)
	enum { IDD = IDD_DLG_Rotate };
	CButton	m_ch1;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	float	m_angle;
	CComboBox inMethod;
	CComboBox ofMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRotate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRotate)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROTATE_H__4DDB3383_9804_11D5_8DD4_D76CCF55323D__INCLUDED_)
