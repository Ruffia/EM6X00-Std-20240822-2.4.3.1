#if !defined(AFX_DLGPROGRESS_H__ABBDD815_8F61_49F9_AD5C_0A86C7B0376A__INCLUDED_)
#define AFX_DLGPROGRESS_H__ABBDD815_8F61_49F9_AD5C_0A86C7B0376A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//

class CSemCoreCtrlPanel_Stand8Ctrl;

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog

class CDlgProgress : public CDialog
{
	UINT							m_nTimmeID;

	BOOL							m_blReady;
	CSemCoreCtrlPanel_Stand8Ctrl *	m_pStand8CtrlPanel;

// Construction
public:
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor

	BOOL Bind( CSemCoreCtrlPanel_Stand8Ctrl* pStand8CtrlPanel );

	UINT		m_nTimerElapse;

	int			m_iRangeFrom;
	int			m_iRangeTo;
	int			m_iPos;

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DIALOG_SEMCORECTRLPANEL_STAND8_DLG_Progress };
	CProgressCtrl	m_Progress;
	CString	m_csMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__ABBDD815_8F61_49F9_AD5C_0A86C7B0376A__INCLUDED_)
