// Last Modify : 2009.12.10

/////////////////////////////////////////////////////////////////////////////
// 09.12.10 增加调试状态功能
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANCHILDFRM_H__E0E54360_AED5_4733_80F5_11CAE55BC265__INCLUDED_)
#define AFX_SCANCHILDFRM_H__E0E54360_AED5_4733_80F5_11CAE55BC265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanChildFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScanChildFrame frame

class CScanChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CScanChildFrame)
protected:
	CScanChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	// IMP 09.12
	BOOL		m_bFullScreenMode;
	CToolBar*	m_pwndFullScreenBar;
	CMenu		m_OrgMenu;
	CRect		m_mainRect;
	BOOL		m_bChildMax;
	void		FullScreenModeOn();
	void		FullScreenModeOff();

public:
	CToolBar				m_wndToolBarFull;

/////////////////////////////////////////////////

protected:
	CStatusBar  m_wndStatusBar;

	UINT		m_nSemMessageTimer;
	UINT		m_nSemAutosaveTimer;
/////////////////////////////////////////////////

	// Prof-uis
	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;	// window placement persistence
	// Prof-uis

// Operations
public:
	inline	CStatusBar&	  GetStatusBar() { return m_wndStatusBar; }
	void	Localize(void);
	void	AdjustPanelsPos();
	void	SetEnableClose( BOOL bEnable );		// 11.07.15

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanChildFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScanChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CScanChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnViewAllParam();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewZoomFullScreen();
	afx_msg void OnUpdateViewZoomFullScreen(CCmdUI* pCmdUI);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr);
	afx_msg BOOL OnToolTipText(UINT   nID,   NMHDR*   pNMHDR,   LRESULT*   pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANCHILDFRM_H__E0E54360_AED5_4733_80F5_11CAE55BC265__INCLUDED_)
