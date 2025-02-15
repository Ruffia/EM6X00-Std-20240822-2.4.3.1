#if !defined(AFX_DLGFLOODFILL_H__824613A9_3221_4A4E_8131_D0A6BE7BC9CF__INCLUDED_)
#define AFX_DLGFLOODFILL_H__824613A9_3221_4A4E_8131_D0A6BE7BC9CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFloodFill.h : header file
//
#include "resource.h"
#include "xTargetButton.h"

class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CDlgFloodFill dialog

class CDlgFloodFill : public CDialog
{
// Construction
public:
	BOOL Create();
	CDlgFloodFill(CMainFrame* pParent);
	CDlgFloodFill(CWnd* pParent = NULL);   // standard constructor

	COLORREF m_color;

// Dialog Data
	//{{AFX_DATA(CDlgFloodFill)
	enum { IDD = IDD_DLG_Flood };
	CxTargetButton	m_btnSelColor;
	BYTE	m_tol;
	BOOL	m_select;
	BYTE	m_opacity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFloodFill)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMainFrame* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CDlgFloodFill)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFLOODFILL_H__824613A9_3221_4A4E_8131_D0A6BE7BC9CF__INCLUDED_)
