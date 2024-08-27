#if !defined(AFX_IMAGECHILDFRAME_H__8758B29F_10EC_4FAE_BE9E_F02B135F0AE8__INCLUDED_)
#define AFX_IMAGECHILDFRAME_H__8758B29F_10EC_4FAE_BE9E_F02B135F0AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageChildFrame frame

class CImageChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CImageChildFrame)
protected:
	CImageChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar	m_wndReturnBar;
	CToolBar	m_wndImageBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageChildFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CImageChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CImageChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowReturnToScan();
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText(UINT   nID,   NMHDR*   pNMHDR,   LRESULT*   pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGECHILDFRAME_H__8758B29F_10EC_4FAE_BE9E_F02B135F0AE8__INCLUDED_)
