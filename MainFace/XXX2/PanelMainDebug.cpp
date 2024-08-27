// PanelMainDebug.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2009.04.29

#include "stdafx.h"
#include "XXX2.h"
#include "PanelMainDebug.h"
#include "ControlLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelMainDebug dialog


CPanelMainDebug::CPanelMainDebug()
{
	// init PageTable;
	//////////////////////////////////////////////////////////////
	const PageAssamblePanel PageTable[] = 
	{
		{&m_PageDebug2, CDlgImpPanelDebug2::IDD, IDS_SEMCTRL_PagePanel, IDS_SEMCTRL_PagePanel },
//		{&m_PageDebugA, CDlgImpPanelDebugA::IDD, IDS_SEMCTRL_PagePanel, IDS_SEMCTRL_PagePanel },
		{&m_PagePanelMCI, CPanelMCI::IDD, IDS_SEMCTRL_PagePanel, IDS_SEMCTRL_PagePanel },
		{&m_PagePanelSL, CPanelSL::IDD, IDS_SEMCTRL_PagePanel, IDS_SEMCTRL_PagePanel },
		{&m_PagePanelVS, CPanelVS::IDD, IDS_SEMCTRL_PagePanel, IDS_SEMCTRL_PagePanel },
	};
	m_nPageTableLength = sizeof( PageTable ) / sizeof( PageAssamblePanel );
	m_nSelectPage = 0;
	
	m_pPageTable = new PageAssamblePanel[m_nPageTableLength];
	memcpy( m_pPageTable, PageTable, sizeof( PageTable ) );

	m_pActivePage = NULL;
}

CPanelMainDebug::~CPanelMainDebug()
{
	delete m_pPageTable;
}



void CPanelMainDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSemCtrlDlgConsole)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPanelMainDebug, CDialogBar)
	//{{AFX_MSG_MAP(CPanelMainDebug)
	ON_BN_CLICKED(IDC_PANEL_MAIN_Btn_LastStep, OnLastStep)
	ON_BN_CLICKED(IDC_PANEL_MAIN_Btn_NextStep, OnNextStep)
	ON_BN_CLICKED(IDC_BUTTON_Reset_Communicate, OnResetCommunicate)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelMainDebug message handlers

void CPanelMainDebug::OnDestroy() 
{
	CDialogBar::OnDestroy();
	// TODO: Add your message handler code here
}

LONG CPanelMainDebug::OnInitDialog ( UINT wParam, LONG lParam) 
{
	if ( !HandleInitDialog(wParam, lParam) || !UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
		return FALSE;
	}
	return TRUE;
}

void CPanelMainDebug::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;
} 

BOOL CPanelMainDebug::Create( CWnd*pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID )
{
	if ( !CDialogBar::Create( pParentWnd, nIDTemplate, nStyle, nID ))
		return FALSE;
	
	if (!CreateCtlDlgs())
		return FALSE;
	
//	Init();

	Localize();

	return TRUE;
}

BOOL CPanelMainDebug::CreateCtlDlgs()
{
	CDialog*	pPage;
	UINT		nIDD;
	for( int i=0; i<m_nPageTableLength; i++ )
	{
		pPage	= m_pPageTable[i].pPageDlg;
		nIDD	= m_pPageTable[i].nIDD;
		if (!pPage->Create(nIDD, this))
			return FALSE;
	}
	return TRUE;
}

BOOL CPanelMainDebug::Init() 
{
	AdjustMainDlg();
	AdjustAllCtlDlg();
	AdjustCtrlsPos();
	SelchangePage( 0 );
	
	return TRUE;
}

//2008.01.02
void CPanelMainDebug::AdjustMainDlg()
{
	//根据屏幕分辨率动态调整对话框布局
	int nScreenX = GetSystemMetrics( SM_CXSCREEN );

	CRect wndRect;
	CWnd* pRect = GetDlgItem(IDC_PANEL_MAIN_STATIC_RECT);
	ASSERT(pRect != NULL);
	pRect->GetWindowRect((LPRECT) wndRect);
	ScreenToClient((LPRECT) wndRect);
//	if( wndRect.right != nScreenX )
//		wndRect.right = nScreenX;

	pRect->SetWindowPos(NULL, wndRect.left, wndRect.top, wndRect.Width(), wndRect.Height(), SWP_NOZORDER);
/*
	// 2009.04.29再做调整
	// 调试过程中感觉：如调用MoveWindow时仍使用wndRect，则不起作用
	int nRight = 800;
//	if( wndRect.right != nScreenX )
//		nRight = nScreenX;
//	ScreenToClient((LPRECT) wndRect);
	int nLeft = wndRect.left;
	nRight = (nRight != wndRect.right) ? nRight : wndRect.right;
	int nTop = wndRect.top;
	int nBottom = wndRect.bottom;
//	pRect->MoveWindow( (LPRECT)(CRect(nLeft, nTop, nRight, nBottom)), TRUE );
	pRect->SetWindowPos( NULL, nLeft, nTop, nRight-nLeft, nBottom-nTop, SWP_NOZORDER );*/
}

void CPanelMainDebug::AdjustAllCtlDlg()
{
	CDialog*	pPage;
	for( int i=0; i<m_nPageTableLength; i++ )
	{
		pPage	= m_pPageTable[i].pPageDlg;
		AdjustCtlDlgPos( pPage );
		pPage->ShowWindow(SW_HIDE);
	}
}

void CPanelMainDebug::AdjustCtlDlgPos(CWnd* pCtlDlg)
{
	CRect	wndFrame;
	CRect	wndRect;
	int		x0, y0;	
	
	pCtlDlg->GetWindowRect((LPRECT) wndRect);
	ScreenToClient((LPRECT) wndRect);
	
	CWnd* pFrame = GetDlgItem(IDC_PANEL_MAIN_STATIC_RECT);
	ASSERT(pFrame != NULL);
	pFrame->GetWindowRect((LPRECT) wndFrame);
	ScreenToClient((LPRECT) wndFrame);
	
/*	//原程序:
	if (wndFrame.Width() >= wndRect.Width())
		x0 = (wndFrame.Width() - wndRect.Width()) / 2;
	else
		x0 = 0;
	
	if (wndFrame.Height() >= wndRect.Height()) 
		y0 = (wndFrame.Height() - wndRect.Height()) / 2;
	else
		y0 = 0;
	
	x0 += wndFrame.left;
	y0 += wndFrame.top;
	
	pCtlDlg->SetWindowPos(NULL, x0, y0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
*/	
	//现程序
	x0 = 0;
	y0 = 0;
	BOOL bAdjustSize = FALSE;
//	if (wndFrame.Width() < wndRect.Width())
		bAdjustSize = TRUE;
	
	if (wndFrame.Height() < wndRect.Height()) 
		bAdjustSize = TRUE;
	
	x0 += wndFrame.left;
	y0 += wndFrame.top;
	
	if( bAdjustSize )
		pCtlDlg->SetWindowPos( NULL, x0, y0, wndFrame.Width(), wndFrame.Height(), SWP_NOZORDER );
	else
		pCtlDlg->SetWindowPos(NULL, x0, y0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//现程序
}

//2009.03.12
void CPanelMainDebug::AdjustCtrlsPos()
{
	return;
}

BOOL CPanelMainDebug::IsActivePage( int nIndex )
{
	if ( nIndex < 0 || nIndex > m_nPageTableLength-1 )
		return FALSE;

	CDialog*	pPage	= m_pPageTable[nIndex].pPageDlg;
	if( pPage == m_pActivePage )
		return TRUE;

	return FALSE;
}

void CPanelMainDebug::ChangeActivePage(CDialog* pNewActivePage)
{
	// TODO: Add your control notification handler code here
	if (m_pActivePage != pNewActivePage)
	{
		if (m_pActivePage != NULL)
			m_pActivePage->ShowWindow(SW_HIDE);
		
		m_pActivePage = pNewActivePage;
		m_pActivePage->ShowWindow(SW_SHOW);
	}
}

int CPanelMainDebug::SelchangePage( int nIndex )
{
	if( nIndex < 0 )
		return 0;
	int			nNewIndex		= nIndex;

	//======2004-5-14_循环切换控制面板=====================
	// 修改标示：2004-5-14_循环切换控制面板
	// 修改日期：2004年5月14日
	// 修改人：罗明华
	// 修改原因：修改控制界面（循环切换控制面板）。

	// 本次修改前代码副本
	/*----------------------------------------------------- 
	/////////////////////////////////////////////////////////////
	if ( nNewIndex < 0 ) nNewIndex = 0;
	if ( nNewIndex > m_nPageTableLength-1 ) nNewIndex = m_nPageTableLength-1;
	/////////////////////////////////////////////////////////////
	-----------------------------------------------------*/

	// 本次修改后代码
	//-----------------------------------------------------
	/////////////////////////////////////////////////////////////
	if ( nNewIndex < 0 ) nNewIndex = m_nPageTableLength-1;
	if ( nNewIndex > m_nPageTableLength-1 ) nNewIndex = 0;
	/////////////////////////////////////////////////////////////
	//-----------------------------------------------------
	//======2004-5-14_循环切换控制面板=====================

	/////////////////////////////////////////////////////////////
	UINT		nMsgID			= m_pPageTable[nNewIndex].nMsgID;
	CDialog*	pNewActivePage	= m_pPageTable[nNewIndex].pPageDlg;
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	CString		csMsg = _T("");

	if ( pNewActivePage != NULL && pNewActivePage->m_hWnd != NULL )
		ChangeActivePage( pNewActivePage );
	/////////////////////////////////////////////////////////////

	m_nSelectPage = nNewIndex;
	return nNewIndex;
}

void CPanelMainDebug::OnLastStep() 
{
	// TODO: Add your control notification handler code here
	m_nSelectPage = SelchangePage( m_nSelectPage-1 );
}

void CPanelMainDebug::OnNextStep() 
{
	// TODO: Add your control notification handler code here
	m_nSelectPage = SelchangePage( m_nSelectPage+1 );
}

void CPanelMainDebug::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	CDialogBar::OnLButtonDown(nFlags, point);
}

void CPanelMainDebug::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	CDialogBar::OnLButtonDblClk(nFlags, point);
}

void CPanelMainDebug::OnSize(UINT nType, int cx, int cy) 
{
	//根据屏幕分辨率动态调整对话框布局
/*	int nScreenX = 800;//GetSystemMetrics( SM_CXSCREEN );
	int nScreenY = 600;//GetSystemMetrics( SM_CYSCREEN ) /10;
	CDialogBar::OnSize(nType, nScreenX, nScreenY);
*/	
	// TODO: Add your message handler code here
	CDialogBar::OnSize(nType, cx, cy);
}

void CPanelMainDebug::OnResetCommunicate() 
{
	// Reset / Initialize Board Communication Circuit
	CSemCtrl::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
}
