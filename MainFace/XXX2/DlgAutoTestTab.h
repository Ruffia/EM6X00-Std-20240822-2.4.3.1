#if !defined(AFX_DLGAUTOTESTTAB_H__8BD1E82D_A2A5_44D8_8A15_182755432768__INCLUDED_)
#define AFX_DLGAUTOTESTTAB_H__8BD1E82D_A2A5_44D8_8A15_182755432768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTestTab.h : header file
//

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestTab dialog

class CDlgAutoTestTab : public CDialog
{
// Construction
public:
	CDlgAutoTestTab(UINT nID, CWnd* pParent /*= NULL*/);   // standard constructor
	~CDlgAutoTestTab();

// Dialog Data
	//{{AFX_DATA(CDlgAutoTestTab)
//	enum { IDD = IDD_AutoTest_TabDlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTestTab)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTestTab)
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

#endif // !defined(AFX_DLGAUTOTESTTAB_H__8BD1E82D_A2A5_44D8_8A15_182755432768__INCLUDED_)
