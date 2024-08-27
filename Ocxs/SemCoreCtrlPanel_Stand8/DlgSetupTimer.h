#if !defined(AFX_DLGSETUPTIMER_H__7FD9A7F4_EC45_4985_A867_A5342BFF6781__INCLUDED_)
#define AFX_DLGSETUPTIMER_H__7FD9A7F4_EC45_4985_A867_A5342BFF6781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupTimer.h : header file
//
#include "ColorCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupTimer dialog

class CDlgSetupTimer : public CDialog
{
// Construction
public:
	CDlgSetupTimer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetupTimer)
	enum { IDD = IDD_DIALOG_SEMCORECTRLPANEL_STAND8_DLG_SetupTimer };
	CColorCtrl<CStatic> m_ctrlWarning;
	UINT	m_nTimer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetupTimer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetupTimer)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUPTIMER_H__7FD9A7F4_EC45_4985_A867_A5342BFF6781__INCLUDED_)
