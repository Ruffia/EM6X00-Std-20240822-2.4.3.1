// ImageChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "ImageChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void RecalcControlBar( CFrameWnd* pParentWnd, CControlBar* destBar, CControlBar* srcBar );

/////////////////////////////////////////////////////////////////////////////
// CImageChildFrame

IMPLEMENT_DYNCREATE(CImageChildFrame, CMDIChildWnd)

CImageChildFrame::CImageChildFrame()
{
}

CImageChildFrame::~CImageChildFrame()
{
}

BOOL CImageChildFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.cx	= 512;
	cs.cy	= 512;


	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
//	return CMDIChildWnd::PreCreateWindow(cs);
}


BEGIN_MESSAGE_MAP(CImageChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CImageChildFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_RETURN_TO_SCAN, OnWindowReturnToScan)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)   
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)   
END_MESSAGE_MAP()

static UINT indicators[] =
{
//	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_OPEN_TIME,		//ID_INDICATOR2,
	ID_INDICATOR_IMAGE_PROPERTY,//ID_INDICATOR3,
	ID_INDICATOR_IMAGE_MOUSEMOVE,//ID_INDICATOR1,
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CImageChildFrame message handlers

int CImageChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableDocking(CBRS_ALIGN_ANY);
	
	// TODO: Add your specialized creation code here
/*	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_3K9_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
	if (!m_wndImageBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndImageBar.LoadToolBar(IDR_IMGTYPE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndImageBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndImageBar);
//	RecalcControlBar( this, &m_wndImageBar, &m_wndToolBar );
	DockControlBar(&m_wndImageBar, AFX_IDW_DOCKBAR_LEFT);

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

void CImageChildFrame::OnWindowReturnToScan() 
{
	// TODO: Add your command handler code here
	((CMainFrame *)theApp.GetMainWnd())->ActivateMDIChild();
}

BOOL CImageChildFrame::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)   
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
