#if !defined(AFX_DLGDITHER_H__FCFC4DC1_FB70_11D5_8DD7_00E07D8144D0__INCLUDED_)
#define AFX_DLGDITHER_H__FCFC4DC1_FB70_11D5_8DD7_00E07D8144D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDither.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDither dialog
class CDlgDither : public CDialog
{
// Construction
public:
	BYTE m_method;
	CDlgDither(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDither)
	enum { IDD = IDD_DLG_Dither };
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CButton	m_r8;
	CButton	m_r7;
	CButton	m_r6;
	CButton	m_r5;
	CButton	m_r4;
	CButton	m_r3;
	CButton	m_r2;
	CButton	m_r1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDither)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDither)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDITHER_H__FCFC4DC1_FB70_11D5_8DD7_00E07D8144D0__INCLUDED_)
