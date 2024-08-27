// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DllTest.h"

#include "MainFrm.h"
#include "DlgMobileUScale.h"
#include "DlgCCDtest.h"

#include "ExportAutoTestFunc.h"
// #pragma comment(lib, "..\\..\\Bin\\AutoTest.lib")

#include "ExportCameraFunc.h"
//#pragma comment(lib, "..\\..\\Bin\\Options_Camera.lib")

//#include "ExportXYStageFunc.h"
//#pragma comment(lib, "..\\..\\Bin\\Options_XYStage.lib")

#include "ExportStageMC6600Func.h"

#include "DlgLanguage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_Test_AutoTest, OnTestAutoTest)
	ON_COMMAND(ID_Test_MobileUScale, OnTestMobileUScale)
	ON_COMMAND(ID_Test_Camera2, OnTestCamera2)
	ON_COMMAND(ID_Test_XYstage, OnTestXYstage)
	ON_COMMAND(ID_Test_Camera3, OnTestCamera3)
	ON_COMMAND(ID_Language_Test, OnLanguageTest)
	ON_COMMAND(ID_Test_StagePod, OnTestStagePod)
	ON_COMMAND(ID_Test_MC6600, OnTestMC6600)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_Test_1, ID_Test_4, OnTestChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_Test_1, ID_Test_4, OnUpdateTestChanged)
	ON_MESSAGE(WM_USER_StagePod_Update, OnUpdateStagePod)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_nTest = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//设定窗口区大小为800*600
	cs.cx	= 800;
	cs.cy	= 600;
	//设定客户区大小为800*600
	CRect rect(0,0,800,600);
	::AdjustWindowRectEx(&rect,cs.style,TRUE,cs.dwExStyle);
	cs.cx	= rect.Width();
	cs.cy	= rect.Height();

	//改变视窗的位置让它固定在屏幕中间
	CRect rcArea;
	SystemParametersInfo( SPI_GETWORKAREA, NULL, &rcArea, NULL );
	cs.x = rcArea.left + ( rcArea.Width() - cs.cx ) / 2;
	cs.y = rcArea.top + ( rcArea.Height() - cs.cy ) / 2;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnTestAutoTest() 
{
	// TODO: Add your command handler code here
	ShowAutoTestDlg(AfxGetMainWnd()->m_hWnd, (void*)(&(theApp.m_SemCtrl)));
}

void CMainFrame::OnTestCamera2() 
{
	// TODO: Add your command handler code here
	ShowCameraWnd(AfxGetMainWnd()->m_hWnd);
}

void CMainFrame::OnTestCamera3() 
{
	// TODO: Add your command handler code here
	CDlgCCDtest dlg;
	dlg.DoModal();
}

void CMainFrame::OnTestXYstage() 
{
	// TODO: Add your command handler code here
	ShowXYStageDlg(AfxGetMainWnd()->m_hWnd);
}

//调节器类型改变
void CMainFrame::OnTestChanged(UINT nID)
{
	m_nTest = nID - ID_Test_1;
}

void CMainFrame::OnUpdateTestChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_nTest == pCmdUI->m_nID -ID_Test_1 );
}

void CMainFrame::OnTestMobileUScale() 
{
	// TODO: Add your command handler code here
	CDlgMobileUScale dlg;
	dlg.DoModal();
}

void CMainFrame::OnLanguageTest() 
{
	CDlgLanguage dlg;
	dlg.DoModal();
}

void CMainFrame::OnTestMC6600() 
{
	// TODO: Add your command handler code here
	StageMC6600_ShowWnd( this->m_hWnd );
}

void CMainFrame::OnTestStagePod() 
{
	// TODO: Add your command handler code here
	static BOOL bOpen = TRUE;
	if( bOpen )
	{
		m_StagePod.InitHWND( m_hWnd );
		m_StagePod.Open();
	}
	else
		m_StagePod.Close();
	bOpen = !bOpen;
}

unsigned short g_nStagePodState[3];
short g_nStagePodSetTrack1[3], g_nStagePodSetTrack2[3];

void  StagePodInit()
{
	for(int i =0;i<3;i++)
	{
		g_nStagePodSetTrack1[i] =0;
		g_nStagePodSetTrack2[i] =0;
	}
}

void StagePodTrack(short nIndex)
{
	BOOL    bNum;
	for(int i=0;i<3;i++ )
	{
		if(g_nStagePodSetTrack1[i] == 0)
		{
			if(g_nStagePodState[i]-StagePod_Stop > 100 )
				g_nStagePodSetTrack1[i] =  1;
			else if (g_nStagePodState[i]-StagePod_Stop < -100 )
				g_nStagePodSetTrack1[i] =  -1;
			bNum = TRUE;
		}
		else
		{
			if(g_nStagePodState[i]-StagePod_Stop > 100  )
				g_nStagePodSetTrack2[i] =  1;
			else if (g_nStagePodState[i]-StagePod_Stop < -100 )
				g_nStagePodSetTrack2[i] =  -1;
			bNum = FALSE;
		}
	}

	if((g_nStagePodSetTrack1[0] == g_nStagePodSetTrack2[0])
		&& (g_nStagePodSetTrack1[1] == g_nStagePodSetTrack2[1])
		&& (g_nStagePodSetTrack1[2] == g_nStagePodSetTrack2[2]))
		return;

	if(!bNum)
	{
		for(int i=0;i<3;i++)
			g_nStagePodSetTrack1[i] = g_nStagePodSetTrack2[i];
	}

	switch( nIndex )
	{
	case	100:
		StageMC6600_StopTracklocus();
		StagePodInit();
		break;
	case	1:
		StageMC6600_SetTracklocus1(g_nStagePodState[0], g_nStagePodState[1], g_nStagePodState[2]);
		break;
	case    2:
		StageMC6600_SetTracklocus2(g_nStagePodState[0], g_nStagePodState[1]);
		break;
	} 
}

LRESULT CMainFrame::OnUpdateStagePod(WPARAM wParam, LPARAM lParam)
{
	CString str;
	switch( wParam )
	{
	case	0:
		{
			BOOL bReady = (BOOL)lParam;
			if( bReady )
				OutputDebugString( "Stage Pod is ready!\n");
			else
				OutputDebugString( "Stage Pod is NOT ready!\n");
		}
		break;
		
	case	1:
	case	2:
		{
			unsigned short* state = (unsigned short *)lParam;
			for( int i=0; i<3; i++ )
				g_nStagePodState[i] = state[i];
			StagePodTrack( (short)wParam );
		}
		break;

	case	100:
		{
			for( int i=0; i<3; i++ )
				g_nStagePodState[i] = 0;
			StagePodTrack( (short)wParam );
		}
		break;
	}
	return 0;
}
