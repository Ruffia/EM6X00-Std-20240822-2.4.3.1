// TabDialog.cpp : implementation file
//
// Copyright 2003 Written by Wang Yingwu, Toong See Wan   
//		mailto:wangyingwu@hotmail.com
//
// This code can not be used for any commerencial usage if without author's written consent.      
//
// This file is provided "as is" with no expressed or implied warranty.  
// The author accepts no liability for any damage/loss of business that  
// this product may cause. 
//
// Wecome to report any bugs or suggestion to wangyingwu@hotmail.com
//
// History: 29 May 2003			Initial Version
//			1 Sep 2003          Add some pointer check

#include "stdafx.h"
#include "Insample0.h"
#include "TabDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabDialog dialog

CTabDialog::CTabDialog(UINT nID, CWnd* pParent /*=NULL*/)
	: CDialog(nID, pParent)
{
	//{{AFX_DATA_INIT(CTabDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nTotalPage = 0;
}

CTabDialog::~CTabDialog()
{
	DestroyWindow();
}

void CTabDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabDialog, CDialog)
	//{{AFX_MSG_MAP(CTabDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabDialog message handlers

BOOL CTabDialog::AddPage(CDialog* pDialog, CButton* pButton)
{
	ASSERT (pDialog != NULL);
	ASSERT (pButton != NULL);

	//Add dialog to dialog map
	m_DialogMap.SetAt(m_nTotalPage, pDialog);

	//add button according to button map
	m_ButtonMap.SetAt(m_nTotalPage, pButton);

	//increase the total page
	m_nTotalPage ++;

	return TRUE;
}

///////////////////////////////////////////////////////////////////
//the default display of the pages
//all buttons displayed and only first dialog displayed
void CTabDialog::InitPagesShow( int nIndex )
{
	InitButtonsShow();
	InitDialogShow( nIndex );
}

///////////////////////////////////////////////////////////////
//show all the buttons
void CTabDialog::InitButtonsShow()
{
	for(int i = 0; i < m_nTotalPage; i ++)
	{
		CButton* pButton = NULL;

		if(m_ButtonMap.Lookup(i, pButton)==FALSE)
			return;

		if(pButton != NULL)
			pButton->SetWindowPos(NULL, 20+200*i, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
//			pButton->SetWindowPos(&wndTop, 20+100*i, -22, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
}

///////////////////////////////////////////////////////////
//display the first dialog and hide all other dialog
//this is the default display of dialog tab
void CTabDialog::InitDialogShow( int nIndex )
{
	for(int i = 0; i < m_nTotalPage; i ++)
	{
		CDialog* pDialog;

		m_DialogMap.Lookup(i, pDialog);

		if(pDialog == NULL)
		{
			return;
		}

		if(i==nIndex)
		{
			pDialog->SetWindowPos(NULL, 0, BUTTON_HEIGHT, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			pDialog->ShowWindow(SW_SHOW);
		}
		else
			pDialog->SetWindowPos(NULL, 0, BUTTON_HEIGHT, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	}
}

//////////////////////////////////////////////////////////////////
//Hide all the pages
void CTabDialog::HideAllPages()
{
	for(int i = 0; i < m_nTotalPage; i ++)
	{
		CDialog* pDialog;
		m_DialogMap.Lookup(i, pDialog);

		pDialog->ShowWindow(SW_HIDE);
	}
}

BOOL CTabDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if(HIWORD(wParam)==BN_CLICKED)
	{
		for(int i = 0; i < m_nTotalPage; i ++)
		{
			CButton* pButton = NULL;
			m_ButtonMap.Lookup(i, pButton);

			if(pButton->m_hWnd==(HWND) lParam)
			{
				CDialog* pDialog;

				//get the current dilaog and display it
				if(m_DialogMap.Lookup(i, pDialog))
				{
					HideAllPages();
					pDialog->ShowWindow(SW_SHOW);
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	return CDialog::OnCommand(wParam, lParam);
}

BOOL CTabDialog::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( !m_DialogMap.IsEmpty() )
		m_DialogMap.RemoveAll();
	if( !m_ButtonMap.IsEmpty() )
		m_ButtonMap.RemoveAll();
	
	return CDialog::DestroyWindow();
}
