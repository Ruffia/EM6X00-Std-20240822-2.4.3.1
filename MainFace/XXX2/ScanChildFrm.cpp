// ScanChildFrm.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2009.12.10

/////////////////////////////////////////////////////////////////////////////
// 09.11.19 将所有LoadString处均改为GetResString
// 09.12.10 增加调试状态功能
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"
#include "ScanChildFrm.h"
#include "MainFrm.h"
#include "ScanDoc.h"
#include "ScanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern const UINT nSemMessageTimerID;
extern const UINT nSemMessageTimerElapse;

extern const UINT nSemAutosaveTimerID;
extern const UINT nSemAutosaveTimerElapse;

void RecalcControlBar( CFrameWnd* pParentWnd, CControlBar* destBar, CControlBar* srcBar )
{
	// 使用MFC来调整所有工具栏的尺寸，确保GetWindowRec准确
	CRect rect(0,0,0,0);
	DWORD dw = 0;
	UINT n=0;
	pParentWnd->RecalcLayout(TRUE);
	srcBar->GetWindowRect(&rect);
	rect.OffsetRect(10,0);
	dw = srcBar->GetBarStyle();
	n = 0;
	n = (dw & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw & CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw & CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw & CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	pParentWnd->DockControlBar(destBar, n, &rect);
}

/////////////////////////////////////////////////////////////////////////////
// CScanChildFrame

IMPLEMENT_DYNCREATE(CScanChildFrame, CMDIChildWnd)

CScanChildFrame::CScanChildFrame()
{
	// IMP 09.12
	m_bFullScreenMode	= FALSE;
	m_bChildMax			= FALSE;
	// IMP 09.12

/*	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
*/
}

CScanChildFrame::~CScanChildFrame()
{
}

BEGIN_MESSAGE_MAP(CScanChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CScanChildFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_VIEW_ALL_PARAM, OnViewAllParam)
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_ZOOM_FULL_SCREEN, OnViewZoomFullScreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_FULL_SCREEN, OnUpdateViewZoomFullScreen)
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
	//ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)   
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)   
END_MESSAGE_MAP()

static UINT scan_indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CScanChildFrame message handlers

int CScanChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//////////////////////////////////////////////////////////////////////////
	EnableDocking(CBRS_ALIGN_ANY);
	
	CString str;
/*
//--全屏栏-----------------------------
	if (!m_wndToolBarFull.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarFull.LoadToolBar(IDR_FULLSCREEN))
	{
		TRACE0("Failed to create toolbar3\n");
		return -1;      // fail to create
	}
	m_wndToolBarFull.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBarFull);
	str = "Full";
	m_wndToolBarFull.SetWindowText( str );
*/
	return 0;
}

// 11.07.15 增加保护，当高压未退至0时不允许关闭软件
void CScanChildFrame::SetEnableClose( BOOL bEnable )
{
	if( bEnable )
	{
		// 10.05.20 禁用之防止用户错误操作点击关闭图标
		CMenu* pMenu = this->GetSystemMenu(FALSE);
		pMenu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND|MF_GRAYED);
	}
	else
	{
		// 10.05.20 禁用之防止用户错误操作点击关闭图标
		CMenu* pMenu = this->GetSystemMenu(FALSE);
		pMenu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND|MF_ENABLED);
	}
}
// 11.07.15 增加保护，当高压未退至0时不允许关闭软件

void CScanChildFrame::Localize(void)
{
}

BOOL CScanChildFrame::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)   
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
//	TCHAR   szFullText[256];
	CString   strTipText = _T("");
	UINT   nID   =   pNMHDR->idFrom;

	if( pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND)   ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID;// = _AfxGetDlgCtrlID((HWND)nID);
	}

	if(nID != 0)	// will be zero on a separator
	{   
/*		// don't handle the message if no string resource found
		if(AfxLoadString(nID, szFullText) == 0)
			return FALSE;
		// this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
*/
		if( nID >= ID_FILE_NEW )	// system menu(0xE100 --> 0xE810)
			strTipText = GetResString( nID -ID_FILE_NEW +IDS_TOOLTIP_FILE_NEW );
		else
			strTipText = GetResString( nID -ID_ADJUSTOR_BEAM_ALIGNMENT +IDS_TOOLTIP_ADJUSTOR_BEAM_ALIGNMENT );
	}

	int nMaxLenth = strTipText.GetLength() +1;
	#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, nMaxLenth );//_countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, nMaxLenth );//_countof(pTTTW->szText));
	#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, nMaxLenth );//_countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, nMaxLenth );//_countof(pTTTW->szText));
	#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
				SWP_NOACTIVATE |SWP_NOSIZE |SWP_NOMOVE |SWP_NOOWNERZORDER);

	return TRUE;		// message was handled
}   

BOOL CScanChildFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~WS_OVERLAPPED;
	cs.style &= ~WS_SYSMENU;
	cs.style &= ~WS_BORDER;
	cs.style &= ~WS_CAPTION;
	cs.style &= ~WS_THICKFRAME;
	cs.style &= ~WS_DLGFRAME;
	cs.dwExStyle &= ~WS_EX_DLGMODALFRAME;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle &= ~WS_EX_WINDOWEDGE;
	cs.dwExStyle &= ~WS_EX_STATICEDGE;

	cs.dwExStyle = 0;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CScanChildFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
//	nCmdShow = SW_SHOWMAXIMIZED;
/*	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CMDIChildWnd :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
*/	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CScanChildFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

void CScanChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if( nID == 0xF093 )				// 10.05.20 禁用之禁用单击程序图标时出现的系统菜单
		return;
	if( nID >= 0xF001 && nID <= 0xF008 )
		return;

	CMDIChildWnd :: OnSysCommand( nID, lParam );
}

void CScanChildFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( nHitTest == HTCAPTION )		// 10.05.20 禁用之防止用户双击标题栏后还原窗口大小
		return;

	CMDIChildWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CScanChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//	if( nType == SIZE_MAXIMIZED )
	{
		LONG style = ::GetWindowLong( m_hWnd, GWL_STYLE );
		style &= ~WS_MAXIMIZEBOX;	// 10.05.20 禁用之禁用还原/最大化按钮
		::SetWindowLong( m_hWnd, GWL_STYLE, style);
	}
}

void CScanChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	theApp.m_blExit = TRUE;

	if(m_bFullScreenMode)	// IMP 09.12
		FullScreenModeOff();   

	CMDIChildWnd::OnClose();
}

void CScanChildFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CScanChildFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMDIChildWnd::DestroyWindow();
}

//下拉菜单
void CScanChildFrame::OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	return;
	CWnd *pWnd;
	UINT nID;
	// Switch on button command id's.
	switch (pnmtb->iItem)
	{
	case ID_SCAN_TYPE_ACTIVERECT:
//		pWnd = &m_wndScanCtrlBar;
//		nID = IDR_SCAN_TYPE;
		break;
	default:
		return;
	}
	// load and display popup menu
	CRect rc;
	pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);

	CMenu menu;
	menu.LoadMenu(nID);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, rc.left, rc.bottom, this);
}

void CScanChildFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnTimer(nIDEvent);
}

void CScanChildFrame::OnViewAllParam() 
{
	// TODO: Add your command handler code here
// 	CParamDlg dlg( this );
// 	dlg.DoModal();
}



// IMP 09.12 START

void CScanChildFrame::OnViewZoomFullScreen() 
{
	// TODO: Add your command handler code here
	if( m_bFullScreenMode )
		FullScreenModeOff();
	else
		FullScreenModeOn();
}

void CScanChildFrame::OnUpdateViewZoomFullScreen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bFullScreenMode);	
}

void CScanChildFrame::FullScreenModeOn()
{
	// available only if there is an active doc
//	CMDIChildWnd* pChild = MDIGetActive();
//	if( !pChild ) return;

	// first create the new toolbar
	// this will contain the full-screen off button
	m_pwndFullScreenBar = new CToolBar;
	m_pwndFullScreenBar->Create( this );
	m_pwndFullScreenBar->LoadToolBar( IDR_FULLSCREEN );
	m_pwndFullScreenBar->SetBarStyle( m_pwndFullScreenBar->GetBarStyle() |
								CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	// to look better:
	m_pwndFullScreenBar->ModifyStyle(0, TBSTYLE_FLAT);
	m_pwndFullScreenBar->EnableDocking( 0 );
	// place the full-screen off button somewhere:
	CPoint pt( 750, 200 );
	FloatControlBar( m_pwndFullScreenBar, pt);

	// now save the old positions of the main and child windows
	GetWindowRect( &m_mainRect );

	// remove the caption of the mainWnd:
	LONG style = ::GetWindowLong( m_hWnd, GWL_STYLE );
	style &= ~WS_CAPTION;
	style &= ~WS_SYSMENU;
	style &= ~WS_BORDER;
	style &= ~WS_THICKFRAME;
	::SetWindowLong( m_hWnd, GWL_STYLE, style );
	int screenx = GetSystemMetrics( SM_CXSCREEN );
	int screeny = GetSystemMetrics( SM_CYSCREEN );

	// resize:
	SetWindowPos( NULL, 0, 0, screenx, screeny, SWP_NOZORDER );
	//SetWindowPos( NULL, -5, -5, 80, 80, SWP_NOZORDER);
	style = ::GetWindowLong( m_hWnd, GWL_STYLE );
	m_bChildMax = (style & WS_MAXIMIZE) ? TRUE : FALSE;
	// note here: m_bMainMax is not needed since m_hWnd only
	// changed its caption...

	///***************
	//Save the Original Menu and set menu to NULL
	ASSERT( m_OrgMenu.GetSafeHmenu() == NULL );
	CMenu* pOldMenu = GetMenu();
	m_OrgMenu.Attach( pOldMenu->Detach() );
	SetMenu( (CMenu*)NULL );
	//************   

	// and maximize the child window
	// it will remove its caption, too.
	//pChild->
	this->ShowWindow( SW_SHOWMAXIMIZED );
	style = ::GetWindowLong( m_hWnd, GWL_STYLE );
	style &= ~WS_CAPTION;
	::SetWindowLong( m_hWnd, GWL_STYLE, style );
	//pChild->SetWindowPos( NULL, -5, -5, screenx+5, screeny+5, SWP_NOZORDER );
	ShowWindow( SW_SHOWMAXIMIZED );

	//RecalcLayout();
	m_bFullScreenMode = TRUE;

}

void CScanChildFrame::FullScreenModeOff()
{
//	pNavigateBar->ShowWindow( SW_SHOW );

	ASSERT( m_OrgMenu.GetSafeHmenu() != NULL );
	SetMenu( &m_OrgMenu );
	m_OrgMenu.Detach();

	// You can use SaveBarState() in OnClose(),
	// so remove the newly added toolbar entirely
	// in order SaveBarState() not to save its state.
	// That is why I used dynamic allocation
	delete m_pwndFullScreenBar;
	LONG style = ::GetWindowLong( m_hWnd, GWL_STYLE );
	style |= WS_CAPTION;
	style |= WS_SYSMENU;
	style |= WS_BORDER;
	style |= WS_THICKFRAME;
	style |= WS_SYSMENU;
/*	style &= ~WS_MAXIMIZEBOX;
	style &= ~WS_MINIMIZEBOX;
//	style &= ~WS_MAXIMIZE;
//	style &= ~WS_MAXIMIZE;
*/	::SetWindowLong( m_hWnd, GWL_STYLE, style);

//	CMDIChildWnd* pChild=MDIGetActive();

	style = ::GetWindowLong( m_hWnd, GWL_STYLE );
	style |= WS_CAPTION;
//	::SetWindowLong( pChild->m_hWnd, GWL_STYLE, style);
	// pchild can be NULL if the USER closed all the
	// childs during Full Screen Mode:
//	if( pChild )
	{
		MoveWindow( &m_mainRect );
		RecalcLayout();

		if( m_bChildMax )
			MDIMaximize();
		else
			MDIRestore();
	}
	m_bFullScreenMode = FALSE;
}
// IMP 09.12 END

void CScanChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
	if( bActivate )
	{
/*		CMainFrame* pMain = (CMainFrame *)theApp.GetMainWnd();
		if( pMain->m_wndScanCtrlBar.m_hWnd != NULL )
			pMain->m_wndScanCtrlBar.ShowWindow(SW_SHOW);
		if( pMain->m_wndToolBarFull.m_hWnd != NULL )
			pMain->m_wndToolBarFull.ShowWindow(SW_SHOW);
		pMain->RecalcLayout();
*/
//		pMain->MinimizeImageMDIChild();
	}
	else
	{
/*		CMainFrame* pMain = (CMainFrame *)theApp.GetMainWnd();
		if( pMain->m_wndScanCtrlBar.m_hWnd != NULL )
			pMain->m_wndScanCtrlBar.ShowWindow(SW_HIDE);
		if( pMain->m_wndToolBar.m_hWnd != NULL )
			pMain->m_wndToolBar.ShowWindow(SW_HIDE);
		if( pMain->m_wndToolBarFull.m_hWnd != NULL )
			pMain->m_wndToolBarFull.ShowWindow(SW_HIDE);
		pMain->RecalcLayout();
*/
	}
}
