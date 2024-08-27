// Last Modify : 2008.05.09

#if !defined(AFX_GRADEDLG_H__1C5DC8E6_C28B_4E0D_BA56_83ED5B989951__INCLUDED_)
#define AFX_GRADEDLG_H__1C5DC8E6_C28B_4E0D_BA56_83ED5B989951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GradeDlg.h : header file
//
class CSemCoreCtrlPanel_Stand6Ctrl;

/////////////////////////////////////////////////////////////////////////////
// CGradeDlg dialog

class CGradeDlg : public CDialog
{
	BOOL							m_blReady;
	CSemCoreCtrlPanel_Stand6Ctrl*	m_pStand6CtrlPanel;

	void		ReportGradeChanged();
//-----2008.05.09-----
	void		NotifyGradeChanged();
//-----2008.05.09-----
	void		NotifyActionChanged( int nIndex, int nCheck );

// Construction
public:
	CGradeDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL		Bind( CSemCoreCtrlPanel_Stand6Ctrl* pStand6CtrlPanel );
//-----2008.05.09-----
	void		SyncGradeChanged( int nCtrlGrade );
//-----2008.05.09-----
	// 09.02.16 添加方法以同步文字显示
	void	SyncResString( int nResourceID, CString strRes );

	int			m_nCtrlGrade;
	CFont*		m_pFont;
	COLORREF	m_clrColor;
	BOOL		m_bAutoVisible;

// Dialog Data
	//{{AFX_DATA(CGradeDlg)
	enum { IDD = IDD_DIALOG_SEMCORECTRLPANEL_STAND6_DLG_Grade };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGradeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCoarse();
	afx_msg void OnRadioFine();
	afx_msg void OnRadioTiny();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCHECKRev();
	afx_msg void OnBUTTONAuto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADEDLG_H__1C5DC8E6_C28B_4E0D_BA56_83ED5B989951__INCLUDED_)
