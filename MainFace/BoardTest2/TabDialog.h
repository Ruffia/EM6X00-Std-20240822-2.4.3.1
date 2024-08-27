///////////////////////////////////////////////////////////////////////////////////////////
// 
// TabDialog
//
// Copyright 2003 Written by Wang Yingwu, Toong See Wan   
//		mailto:wangyingwu@hotmail.com
//
// This code can not be used for any commerencial usage without author's written consent.      
//
// This file is provided "as is" with no expressed or implied warranty.  
// The author accepts no liability for any damage/loss of business that  
// this product may cause. 
//
// Wecome to report any bugs or suggestions to wangyingwu@hotmail.com
//
// History: 29 May 2003			Initial Version
//
//////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABDIALOG_H__B0253332_216E_4954_832A_200D7B58D3DC__INCLUDED_)
#define AFX_TABDIALOG_H__B0253332_216E_4954_832A_200D7B58D3DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDialog.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CTabDialog dialog

class CTabDialog : public CDialog
{
// Construction
public:
	CTabDialog(UINT nID, CWnd* pParent /*= NULL*/);   // standard constructor
	~CTabDialog();

// Dialog Data
	//{{AFX_DATA(CTabDialog)
//	enum { IDD = IDD_TABDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabDialog)
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

#endif // !defined(AFX_TABDIALOG_H__B0253332_216E_4954_832A_200D7B58D3DC__INCLUDED_)
