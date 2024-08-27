// DlgAutoTestTab.cpp : implementation file
//

#include "stdafx.h"
#include "AutoTest.h"
#include "DlgAutoTestTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestTab dialog


CDlgAutoTestTab::CDlgAutoTestTab(UINT nID, CWnd* pParent /*=NULL*/)
	: CDialog(nID, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTestTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nTotalPage = 0;
}

CDlgAutoTestTab::~CDlgAutoTestTab()
{
	DestroyWindow();
}


void CDlgAutoTestTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTestTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTestTab, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTestTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestTab message handlers

BOOL CDlgAutoTestTab::AddPage(CDialog* pDialog, CButton* pButton)
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
void CDlgAutoTestTab::InitPagesShow( int nIndex )
{
	InitButtonsShow();
	InitDialogShow( nIndex );
}

///////////////////////////////////////////////////////////////
//show all the buttons
void CDlgAutoTestTab::InitButtonsShow()
{
	for(int i = 0; i < m_nTotalPage; i ++)
	{
		CButton* pButton = NULL;

		if(m_ButtonMap.Lookup(i, pButton)==FALSE)
			return;

		if(pButton != NULL)
			pButton->SetWindowPos(NULL, 20+BUTTON_INTV*i, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
//			pButton->SetWindowPos(&wndTop, 20+100*i, -22, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
}

///////////////////////////////////////////////////////////
//display the first dialog and hide all other dialog
//this is the default display of dialog tab
void CDlgAutoTestTab::InitDialogShow( int nIndex )
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
void CDlgAutoTestTab::HideAllPages()
{
	for(int i = 0; i < m_nTotalPage; i ++)
	{
		CDialog* pDialog;
		m_DialogMap.Lookup(i, pDialog);

		pDialog->ShowWindow(SW_HIDE);
	}
}

BOOL CDlgAutoTestTab::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CDlgAutoTestTab::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( !m_DialogMap.IsEmpty() )
		m_DialogMap.RemoveAll();
	if( !m_ButtonMap.IsEmpty() )
		m_ButtonMap.RemoveAll();
	
	return CDialog::DestroyWindow();
}
