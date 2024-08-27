// Last Modify : 2010.09.02

/////////////////////////////////////////////////////////////////////////////
// 10.09.02 调试窗口一打开，就自动记录所有操作日志
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGIMPPANELLOG_H__7516032C_EA3D_4C7E_A768_8B18448A2060__INCLUDED_)
#define AFX_DLGIMPPANELLOG_H__7516032C_EA3D_4C7E_A768_8B18448A2060__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImpPanelLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelLog dialog

class CDlgImpPanelLog : public CDialog
{
// Construction
public:
	CDlgImpPanelLog(CWnd* pParent = NULL);   // standard constructor
	void	Message(LPCTSTR lpszMessage);
	void	AdjustCtrlsPos();
	void	Localize(void);

	// 10.09.02 调试窗口一打开，就自动记录所有操作日志
	void	InitAutoLog();
	void	CloseAutoLog();
	CFile	m_fAutoLogFile;
	int		m_nLoopCounts;
	// 10.09.02 调试窗口一打开，就自动记录所有操作日志

// Dialog Data
	//{{AFX_DATA(CDlgImpPanelLog)
	enum { IDD = IDD_IMP_PANEL_LOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImpPanelLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImpPanelLog)
	virtual BOOL OnInitDialog();
	afx_msg void OnLogClear();
	afx_msg void OnLogSave();
	afx_msg void OnLogSelect();
	afx_msg void OnLogMsg();
	afx_msg void OnPANELLOGBtnMsgRG();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPPANELLOG_H__7516032C_EA3D_4C7E_A768_8B18448A2060__INCLUDED_)
