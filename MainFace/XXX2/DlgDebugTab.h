#if !defined(AFX_DLGDEBUGTAB_H__808D8482_2AE3_4D92_AE24_7121B42C18D5__INCLUDED_)
#define AFX_DLGDEBUGTAB_H__808D8482_2AE3_4D92_AE24_7121B42C18D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDebugTab.h : header file
//

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugTab dialog

class CDlgDebugTab : public CDialog
{
// Construction
public:
	CDlgDebugTab(UINT nID, CWnd* pParent /*= NULL*/);   // standard constructor
	~CDlgDebugTab();

// Dialog Data
	//{{AFX_DATA(CDlgDebugTab)
//	enum { IDD = IDD_Debug_TabDlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDebugTab)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDebugTab)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL AddPage(CDialog* pDialog, CButton* pButton);
	void InitPagesShow( int nIndex = 0 );

private:
	void InitButtonsShow();
	void InitDialogShow( int nIndex );

	void HideAllPages();

private:
	int m_nTotalPage;
	CMap<int, int&, CDialog*, CDialog*> m_DialogMap;
	CMap<int, int&, CButton*, CButton*> m_ButtonMap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEBUGTAB_H__808D8482_2AE3_4D92_AE24_7121B42C18D5__INCLUDED_)
