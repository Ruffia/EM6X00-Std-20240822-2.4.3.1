// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "XXX2.h"

#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_RETURN_TO_SCAN, OnWindowReturnToScan)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)   
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)   
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here

}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx	= 1024 +10*2;
	cs.cy	= 768 +32+24+20+8;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if( GetParent()->GetWindow(GW_CHILD) == this )
//		nCmdShow = SW_SHOWMAXIMIZED;
/*	nCmdShow = SW_SHOWMAXIMIZED;
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CMDIChildWnd :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}*/
	CMDIChildWnd::ActivateFrame(nCmdShow);
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

static UINT indicators[] =
{
//	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_IMAGE_INFO,
	ID_INDICATOR_OPEN_TIME,		//ID_INDICATOR2,
	ID_INDICATOR_IMAGE_PROPERTY,//ID_INDICATOR3,
	ID_INDICATOR_IMAGE_MOUSEMOVE,//ID_INDICATOR1,
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableDocking(CBRS_ALIGN_ANY);
	
	// TODO: Add your specialized creation code here
	if (!m_wndImageBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndImageBar.LoadToolBar(IDR_IMGTYPE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndImageBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndImageBar);
	DockControlBar(&m_wndImageBar, AFX_IDW_DOCKBAR_TOP);

/*	if (!m_wndReturnBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndReturnBar.LoadToolBar(IDR_RETURN))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndReturnBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndReturnBar);
	RecalcControlBar( this, &m_wndReturnBar, &m_wndImageBar );
*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	int nCount = m_wndStatusBar.GetCount();
	for( int i=0; i<nCount; i++ )
		m_wndStatusBar.SetPaneText(i, "");

	return 0;
}

void CChildFrame::OnWindowReturnToScan() 
{
	// TODO: Add your command handler code here
	((CMainFrame *)theApp.GetMainWnd())->ActivateMDIChild();
}

BOOL CChildFrame::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)   
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
		if( nID == ID_APP_ABOUT )
			strTipText = GetResString( IDS_TOOLTIP_APP_ABOUT_3K9 );
		else if( nID >= ID_FILE_NEW )	// system menu(0xE100 --> 0xE810)
			strTipText = GetResString( nID -ID_FILE_NEW +IDS_TOOLTIP_FILE_NEW );
		else
			strTipText = GetResString( nID -ID_CXIMAGE_ALPHAPALETTETOGGLE +IDS_TOOLTIP_CXIMAGE_ALPHAPALETTETOGGLE );
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
/*
void CChildFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMaxTrackSize	= CPoint(768, 768);
	lpMMI->ptMinTrackSize	= CPoint(128, 128);

	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
}
*/
void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
/*	
	// TODO: Add your message handler code here
	CMainFrame* pMain = (CMainFrame *)theApp.GetMainWnd();
	CRect rect, rectCtrl;
	pMain->GetClientRect( &rect );
	ClientToScreen( &rect );

	if( (pMain->m_wndScanCtrlBar.m_hWnd != NULL)
		&& ((pMain->m_wndScanCtrlBar.GetStyle() & WS_VISIBLE) != 0) )
	{
		pMain->m_wndScanCtrlBar.GetClientRect( (LPRECT)rectCtrl );
		rect.top += rectCtrl.Height();
	}
	int nHeight = rect.bottom -rect.top;

	SetWindowPos(NULL,
		0,	//rect.left,
		0,
		512,//rect.Width(),
		512,
		SWP_NOZORDER );*/
}
