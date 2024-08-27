// SheetSetup.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2009.03.17

#include "stdafx.h"
#include "XXX2.h"
#include "SheetSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetSetup

IMPLEMENT_DYNAMIC(CSheetSetup, CPropertySheet)

CSheetSetup::CSheetSetup(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddControlPages();

	m_hIconSheet = NULL;
}

CSheetSetup::CSheetSetup(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddControlPages();

	m_hIconSheet = NULL;
}

CSheetSetup::~CSheetSetup()
{
	m_Font.DeleteObject();

	if( m_hIconSheet != NULL )
		::DestroyIcon( m_hIconSheet );
}

void CSheetSetup::AddControlPages()
{
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIconSheet = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
//	m_psh.dwFlags |= PSP_USEHICON;
//	m_psh.hIcon = m_hIconSheet;

	// 去掉“帮助”按钮
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_pageScanPhoto.m_psp.dwFlags &= ~PSP_HASHELP;
	m_pageLangOther.m_psp.dwFlags &= ~PSP_HASHELP;

//	m_pageScanPhoto.m_psp.dwFlags |= PSP_USEHICON;
//	m_pageScanPhoto.m_psp.hIcon = m_hIconSheet;

	AddPage(&m_pageScanPhoto);
	AddPage(&m_pageLangOther);
	AddPage(&m_pageSys);
}


BEGIN_MESSAGE_MAP(CSheetSetup, CPropertySheet)
	//{{AFX_MSG_MAP(CSheetSetup)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetSetup message handlers

void CSheetSetup::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL));
	GetDlgItem(ID_APPLY_NOW)->SetWindowText(GetResString(IDS_APPLY_NOW));
	GetDlgItem(IDHELP)->SetWindowText(GetResString(IDS_HELP));

	SetTitle(GetResString(IDS_SD_TITLE_PARAM_SETUP));

	// 设置各选项卡的标签
	TC_ITEM item; 
	item.mask = TCIF_TEXT; 

	CStringArray buffer; 
	buffer.Add(GetResString(IDS_3K2_PAGE_SCAN_PHOTO));
	buffer.Add(GetResString(IDS_3K2_PAGE_LANGUAGE_OTHER));
	buffer.Add(GetResString(IDS_PAGE_SYS));

	for (int i = 0; i < buffer.GetSize(); i++)
		buffer[i].Remove(_T('&'));

	CTabCtrl* tab = GetTabControl();
	for(int i = 0; i < GetPageCount(); i++) 
	{ 
		item.pszText = buffer[i].GetBuffer(20); 
		tab->SetItem (i, &item); 
		buffer[i].ReleaseBuffer();
	}
}

BOOL CSheetSetup::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// TODO: Add your specialized code here

	Localize();

	m_Font.CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	SetFont( &m_Font, TRUE );
	CWnd *pw = GetWindow(GW_CHILD);
	while(pw != NULL)
	{   
		pw->SetFont( &m_Font);
		pw = pw->GetWindow(GW_HWNDNEXT);
	}; 

	return bResult;
}

void CSheetSetup::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnClose();
}
