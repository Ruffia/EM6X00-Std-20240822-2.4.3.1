// Last Modify : 2009.03.17

#if !defined(AFX_SHEETSETUP_H__8F976463_0060_45B7_A212_B509F5E2492C__INCLUDED_)
#define AFX_SHEETSETUP_H__8F976463_0060_45B7_A212_B509F5E2492C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetSetup.h : header file
//

#include "PageScanPhoto.h"
#include "PageLangOther.h"
#include "PageSys.h"

/////////////////////////////////////////////////////////////////////////////
// CSheetSetup

class CSheetSetup : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheetSetup)

// Construction
public:
	CSheetSetup(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetSetup(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	void	AddControlPages(void);

// Attributes
public:
	CPageScanPhoto	m_pageScanPhoto;
	CPageLangOther	m_pageLangOther;
	CPageSys		m_pageSys;

	CFont			m_Font;
	HICON			m_hIconSheet;

// Operations
public:
	void	Localize(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetSetup)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSheetSetup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetSetup)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETSETUP_H__8F976463_0060_45B7_A212_B509F5E2492C__INCLUDED_)
