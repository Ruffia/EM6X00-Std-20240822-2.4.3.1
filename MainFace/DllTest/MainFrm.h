// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__6FDFFB28_ED34_4718_A962_DD3F56F26ACA__INCLUDED_)
#define AFX_MAINFRM_H__6FDFFB28_ED34_4718_A962_DD3F56F26ACA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommStagePod.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	UINT	m_nTest;
	CCommStagePod	m_StagePod;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTestAutoTest();
	afx_msg void OnTestMobileUScale();
	afx_msg void OnTestCamera2();
	afx_msg void OnTestXYstage();
	afx_msg void OnTestCamera3();
	afx_msg void OnLanguageTest();
	afx_msg void OnTestStagePod();
	afx_msg void OnTestMC6600();
	//}}AFX_MSG
	afx_msg void OnTestChanged(UINT nID);
	afx_msg void OnUpdateTestChanged(CCmdUI* pCmdUI);
	afx_msg LRESULT OnUpdateStagePod(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6FDFFB28_ED34_4718_A962_DD3F56F26ACA__INCLUDED_)
