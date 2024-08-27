// MainFrm.cpp : implementation of the CMainFrame class
//
// Last Modify : 2011.07.15

/////////////////////////////////////////////////////////////////////////////
// 11.07.15 增加保护，当高压未退至0时不允许关闭软件
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "ScanDoc.h"
#include "ScanView.h"
#include "ImageDoc.h"
#include "ImageView.h"
#include "DlgPrompt.h"
#include "ProductVersionManager.h"
#include "StageXYManager.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "BoardComm.h"
#include "GlobalParamSetting.h"
#include "MCBControler.h"
#include "SemCtrlInitializer.h"
#include "Scenario.h"
#include "ScenarioManageDlg.h"
#include "HVPowerDataManager.h"
#include "ScanParameterManager.h"
#include "SEMLog/DlgSEMLog_Data.h"
#include "SEMLog/DlgSEMLog_DataCurve.h"
#include "SEMPanel/ControlPanel2Dlg.h"
#include <memory>
using namespace std;
#include "DBDataManager.h"
#include "SEMLog/AlarmManager.h"
#include "SEMLog/DlgSEMLog_Settings.h"
#include "SEMLog/RealTimeDataManager.h"
#include "D22StatusDetector.h"
#include "AppLogManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_COMMAND(ID_OPTIONS_DEBUG, OnOptionsDebug)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DEBUG, OnUpdateOptionsDebug)
	ON_COMMAND(ID_Options_TestParameter, OnOptionsTestParameter)
	ON_UPDATE_COMMAND_UI(ID_Options_TestParameter, OnUpdateOptionsTestParameter)
	ON_COMMAND(ID_Options_ManageScenario, OnOptionsManageScenario)
	ON_UPDATE_COMMAND_UI(ID_Options_ManageScenario, OnUpdateOptionsManageScenario)
	ON_COMMAND(ID_OPTIONS_Log, OnOptionsLog)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_Log, OnUpdateOptionsLog)
	ON_COMMAND(ID_Log_Show, OnLog_Show)
	ON_UPDATE_COMMAND_UI(ID_Log_Show, OnUpdateLog_Show)
	ON_COMMAND(ID_Log_Settings, OnLog_Settings)
	ON_UPDATE_COMMAND_UI(ID_Log_Settings, OnUpdateLog_Settings)
	ON_COMMAND(ID_Log_AdvancedSettings, OnLog_AdvancedSettings)
	ON_UPDATE_COMMAND_UI(ID_Log_AdvancedSettings, OnUpdateLog_AdvancedSettings)
	ON_WM_NCLBUTTONDBLCLK()
	ON_COMMAND(ID_VIEW_PANEL_1, OnViewPanel1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_1, OnUpdateViewPanel1)
	ON_COMMAND(ID_VIEW_PANEL_2, OnViewPanel2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_2, OnUpdateViewPanel2)
	ON_COMMAND(ID_VIEW_PANEL_3, OnViewPanel3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_3, OnUpdateViewPanel3)
	ON_COMMAND(ID_VIEW_PANEL_4, OnViewPanel4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_4, OnUpdateViewPanel4)
	ON_COMMAND(ID_VIEW_PANEL_5, OnViewPanel5)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_5, OnUpdateViewPanel5)
	ON_COMMAND(ID_VIEW_PANEL_6, OnViewPanel6)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_6, OnUpdateViewPanel6)
	ON_COMMAND(ID_VIEW_PANEL_7, OnViewPanel7)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_7, OnUpdateViewPanel7)
	ON_COMMAND(ID_VIEW_PANEL_8, OnViewPanel8)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_8, OnUpdateViewPanel8)
	ON_COMMAND(ID_VIEW_PANEL_9, OnViewPanel9)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_9, OnUpdateViewPanel9)
	ON_COMMAND(IDC_PANEL_MAIN_Btn_LastStep, OnPANELMAINBtnLastStep)
	ON_COMMAND(IDC_PANEL_MAIN_Btn_NextStep, OnPANELMAINBtnNextStep)
	ON_WM_NCRBUTTONDOWN()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_VIEW_BAR_0, OnViewBar0)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BAR_0, OnUpdateViewBar0)
	ON_COMMAND(ID_VIEW_BAR_1, OnViewBar1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BAR_1, OnUpdateViewBar1)
	//ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_SPEED, OnSelchangeComboSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Debug_VideoGraph, OnDeltaposSPINVideoGraph)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Debug_VideoGraph_Offset, OnDeltaposSPINVideoGraphOffset)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PixRate, OnDeltaposSPINPixRate)
	ON_WM_DEVICECHANGE()
	ON_MESSAGE(WM_USER_MC600_UpdateValue, OnStageMC600msgChanged)
	ON_MESSAGE(WM_USER_KYKYComm_DisplayDebugMessage,OnDisplayDebugMessage)
	ON_MESSAGE(WM_USER_KYKYComm_AutoStageXY_FI_Init,OnAutoStageXY_FI_Init)	
	ON_MESSAGE(WM_USER_KYKYComm_SyncFineMag,OnSyncFineMag)	
	ON_MESSAGE(WM_USER_KYKYComm_CoUpdateScan,OnCoUpdateScan)
	ON_MESSAGE(WM_USER_KYKYComm_UpdateBeemAlignmentX,OnUpdateBeemAlignmentX)
	ON_MESSAGE(WM_USER_KYKYComm_UpdateBeemAlignmentY,OnUpdateBeemAlignmentY)
	ON_MESSAGE(WM_USER_KYKYComm_ShowDFocusCtrl,OnShowDFocusCtrl)
	ON_MESSAGE(WM_USER_KYKYComm_SyncContrastCtrlText,OnSyncContrastCtrlText)
	ON_MESSAGE(WM_USER_ControlPanel_AutoBrightContrast,OnControlPanelAutoBrightContrast)
	ON_MESSAGE(WM_USER_ControlPanel_AutoFocus,OnControlPanelAutoFocus)
	ON_MESSAGE(WM_USER_ControlPanel_ChangeScanSpeed,OnControlPanelChangeScanSpeed)
	ON_MESSAGE(WM_USER_ControlPanel_PhotoPreview,OnControlPanelPhotoPreview)
	ON_MESSAGE(WM_USER_ControlPanel_SyncScan,OnControlPanelSyncScan)
	ON_MESSAGE(WM_USER_ControlPanel_StopScan,OnControlPanelStopScan)
	ON_MESSAGE(WM_USER_ControlPanel_ChangeScan,OnControlPanelChangeScan)
	ON_MESSAGE(WM_MsgCheckVACAlarm,OnCheckVACAlarm)
	ON_CBN_DROPDOWN(IDC_COMBO_Beam_Target, OnDropdownComboBeamTarget)
	ON_BN_CLICKED(IDC_BUTTON_Beam_Detection,OnBtnBeamDetection)
	ON_BN_CLICKED(IDC_BUTTON_Beam_Setup,OnBtnBeamSetup)
	ON_CBN_SELCHANGE(IDC_COMBO_Beam_Target, OnSelchangeBeamD22Target)
	ON_CBN_SELCHANGE(IDC_COMBO_Beam_Port, OnSelchangeBeamD22Port)
	ON_CBN_SELCHANGE(IDC_COMBO_Beam_Aper, OnSelchangeBeamD22Aperture)
	ON_BN_CLICKED(IDC_STATIC_Beam_Unit,OnBtnBeamSaveXYpos)
	ON_MESSAGE(WM_USER_D22_UpdateValue, OnUpdateD22Value)
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
	m_nSemMessageTimer		= 0;
	m_nSemAutosaveTimer		= 0;
	m_nReadStatusTimer		= 0;
	m_nCPS_SemMessageTimer	= 0;
	m_nMCB_SemMessageTimer	= 0;
	m_nFilaLifeTimer		= 0;
	m_nAppLifeTimer			= 0;

	// 09.12.10 增加调试状态功能
	m_hAccelDebug		= NULL;
	// 09.12.10 增加调试状态功能
}

CMainFrame::~CMainFrame()
{
	if(m_wndImpPanelLog.m_hWnd != NULL)
		m_wndImpPanelLog.DestroyWindow();
	if(m_wndImpPanelThumb.m_hWnd != NULL)
		m_wndImpPanelThumb.DestroyWindow();
	if(m_wndImpPanelAll2.m_hWnd != NULL)
		m_wndImpPanelAll2.DestroyWindow();
	if( m_dlgStage.m_hWnd != NULL )		// 15.11 自动台
		m_dlgStage.DestroyWindow();

	if( m_MainPanelDebug.m_hWnd != NULL )
		m_MainPanelDebug.DestroyWindow();
	if( m_dlgGraph.m_hWnd != NULL )
		m_dlgGraph.DestroyWindow();
	if( m_dlgDiffGraph.m_hWnd != NULL )
		m_dlgDiffGraph.DestroyWindow();
	if( m_dlgAutotest.m_hWnd != NULL )
		m_dlgAutotest.DestroyWindow();
	if( m_dlgCali.m_hWnd != NULL )
		m_dlgCali.DestroyWindow();
	if( m_HistoBar.m_hWnd != NULL )
		m_HistoBar.DestroyWindow();

	if( m_dlgDebug.m_hWnd != NULL )
		m_dlgDebug.DestroyWindow();
	if( m_dlgDebugVIP.m_hWnd != NULL )
		m_dlgDebugVIP.DestroyWindow();
	if( m_dlgDebugR.m_hWnd != NULL )
		m_dlgDebugR.DestroyWindow();

	CD22StatusDetector::Instance().Release();
	m_BeamFont.DeleteObject();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableDocking(CBRS_ALIGN_ANY);

//--10.04 新界面面板实验---------------
	_CreatePanelLog();
	_CreatePanelThumb();


//--控制面板---------------------------
	theApp.m_strLogo = "KYKY_EM6X00";
	_LoadSettingsFromRegistry();

	_CreatePanelAll2();
	if( CControlLayer::Instance().m_nPAType > 0 )
		_CreatePanelBeam_short();	//_CreatePanelBeam();
	_CreateDlgAutoTest();

	//--Calibration------------------------
	_CreateDlgCalibration();

	//--DebugVIP------------------------
	_CreateDlgDebugVIP();

	//--Debug R & D---------------------
	_CreateDlgDebugR();

//--10.04 新界面面板实验---------------

//--Graph------------------------------
	_CreateDlgGraph();

//--GraphDiff--------------------------
	_CreateDlgDiffGraph();


//--Debug------------------------------
	_CreateDlgDebug();

//	m_dlgDebug.ShowWindow(SW_SHOW);

//--Histogram bar----------------------
	_CreateHistogramBar();

	//--15.11 Auto Stage-------------------
	_CreateDlgStage();

	return 0;
}

// 11.07.15 增加保护，当高压未退至0时不允许关闭软件
void CMainFrame::SetEnableClose( BOOL bEnable )
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

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// Create a child window without the maximize button 
	cs.style &= ~WS_MAXIMIZEBOX;

	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	cs.style &= ~FWS_ADDTOTITLE;
//	m_strTitle = "Test Title";
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::ActivateFrame(int nCmdShow) 
{
/*	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CFrameWnd :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
*/	CFrameWnd :: ActivateFrame(nCmdShow);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_Debug_VideoGraph:
			m_wndVideoGraphOffsetEdit.GetWindowText( str );
			break;
		case	IDC_EDIT_PixRate:
			m_wndPixRateEdit.GetWindowText( str );
			break;
		default:
			break;
		}
		if( str.IsEmpty() )
			return CFrameWnd::PreTranslateMessage(pMsg);
		double dPos = atof( str );
		switch( nID )
		{
		case	IDC_EDIT_Debug_VideoGraph:
			m_debugOSC.dOffset = dPos;
			CSemVirtualMCICard::Instance().SetParams( 9, (int)(dPos * 10000) );
			break;
		case	IDC_EDIT_PixRate:
			m_nusbPixRate = (int)(dPos *2);
			UpdatePixRate( m_nusbPixRate );
			break;
		default:
			break;
		}
		
		return TRUE;
	}

	// 检查是否为鼠标消息
	if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
	{
		// 特别处理鼠标左键按下消息
		if (pMsg->message == WM_LBUTTONUP)
		{
			// 在这里添加您的自定义处理代码
			// 例如，获取鼠标点击位置并作出响应
			CPoint point(pMsg->pt);
			CScanView* pView = (CScanView*)theApp.GetScanView();
			if( pView )
			{
				pView->SetActiveAreaParam_FromMainFrm(point);
				return FALSE;   // 处理完成，不再传递给基类或其他处理器
			}

			// 如果您希望在此处处理消息并阻止它被进一步传递，
			// 可以直接返回TRUE。否则，返回FALSE 让消息继续传递。
			// 下面的返回语句注释掉了，表示不拦截消息，仅作演示。
			// return TRUE;
		}
	}

	return CFrameWnd :: PreTranslateMessage( pMsg );
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if( nID == 0xF093 )				// 10.05.20 禁用之禁用单击程序图标时出现的系统菜单
		return;
	if( nID == 0xF012 )				// 10.05.20 禁用之禁用单击程序标题栏时可移动程序窗口
		return;

	CFrameWnd :: OnSysCommand( nID, lParam );
}

void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( nHitTest == HTCAPTION )		// 10.05.20 禁用之防止用户双击标题栏后还原窗口大小
		return;
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMainFrame::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( nHitTest == HTCAPTION )		// 10.05.20 禁用之防止用户右击标题栏时出现的系统菜单
		return;
	CMDIFrameWnd::OnNcRButtonDown(nHitTest, point);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd :: OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
/*	if( nType == SIZE_MAXIMIZED )
	{
		LONG style = ::GetWindowLong( m_hWnd, GWL_STYLE );
		style &= ~WS_MAXIMIZEBOX;	// 10.05.20 禁用之禁用还原/最大化按钮
		::SetWindowLong( m_hWnd, GWL_STYLE, style);
	}*/
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent )
	{
	case nSemMessageTimerID:
//		theApp.PumpSemMessage();
		theApp.PumpUSBSemMessage();
		break;

	case nSemAutosaveTimerID:
		CSemCtrlInitializer::Instance().AutoSaveSemCtrl();	// 2004-6-2_自动保存并更新控制量
		_SaveScenario();
		break;

	case nMCB_SemMessageTimerID:
		CMCBControler::Instance().PumpMCBSemMessage();
		break;

	case	130:
//		theApp.LogHVVAC();
		break;

	case	200:
		LoggingFilamentLifetime();
		break;

	case	201:
		LoggingAppLifetime();
		break;

	case 1:
	case 2:
	case 3:
	case 4:
		Beam_Timer( nIDEvent );
		break;

	default:
		CFrameWnd :: OnTimer(nIDEvent);
		break;
	}
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDlgPrompt dlg;
	dlg.m_byteType = (BYTE)0x04;
	dlg.m_strMsg = GetResString( IDS_SD_PROMPT_EXIT );;
	dlg.m_strTitle = GetResString( IDS_SD_PROMPT_DOSAVE_TITLE );
	if( dlg.DoModal() != IDYES )
		return;
	theApp.m_blExit = TRUE;

#ifdef VATI_EXTENSIONS
    // get the window & save
    AfxGetApp()->WriteProfileInt ( _T("Screen"), _T("maximized"), IsZoomed()) ;
	if ( !IsZoomed() && !IsIconic() )
	{
		CRect rect;
		GetWindowRect ( & rect ) ;

		int xScreen = GetSystemMetrics(SM_CXSCREEN) - 100;
		int yScreen = GetSystemMetrics(SM_CYSCREEN) - 100;
		if (rect.left > xScreen)
			rect.left = xScreen;
		if (rect.top > yScreen)
			rect.top = yScreen;

		AfxGetApp()->WriteProfileInt ( _T("Screen"), _T("left"),   rect.left ) ;
		AfxGetApp()->WriteProfileInt ( _T("Screen"), _T("top"),    rect.top ) ;
		AfxGetApp()->WriteProfileInt ( _T("Screen"), _T("right"),  rect.right ) ;
		AfxGetApp()->WriteProfileInt ( _T("Screen"), _T("bottom"), rect.bottom ) ;
	}
	// save last used font data for Text tool
    AfxGetApp()->WriteProfileString( _T("TextTool"), _T("lfFaceName"), theApp.m_text.lfont.lfFaceName );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("lfCharSet"), theApp.m_text.lfont.lfCharSet );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("lfWeight"), theApp.m_text.lfont.lfWeight );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("lfItalic"), theApp.m_text.lfont.lfItalic );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("lfUnderline"), theApp.m_text.lfont.lfUnderline );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("fcolor"), theApp.m_text.fcolor );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("bcolor"), theApp.m_text.bcolor );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("opacity"), (int)(100.*theApp.m_text.b_opacity) );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("r_radius"), theApp.m_text.b_round );
	AfxGetApp()->WriteProfileInt( _T("TextTool"), _T("antialias"), theApp.m_text.smooth );
	AfxGetApp()->WriteProfileInt( _T("Options") , _T("JpegQualityI"),(int)(theApp.m_optJpegQuality));
	AfxGetApp()->WriteProfileInt( _T("Options") , _T("JpegQualityF"),(int)(0.5f+1000.0f*(theApp.m_optJpegQuality-(int)(theApp.m_optJpegQuality))));
	AfxGetApp()->WriteProfileInt( _T("Options") , _T("JpegOptions"),theApp.m_optJpegOptions);
	AfxGetApp()->WriteProfileInt( _T("Options") , _T("RawOptions"),theApp.m_optRawOptions);
//	AfxGetApp()->WriteProfileInt( _T("General") , _T("DocType"),theApp.nDocType);
//	AfxGetApp()->WriteProfileInt( _T("General") , _T("MenuStyle"),BCMenu::GetMenuDrawMode());
#endif

	// 将自动台XYT向归零，因时间较长，提示用户进度条
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
		pView->AutoStage_Clear(FALSE);
	///////////////////////////////////////////////////////////////////////////

	CFrameWnd :: OnClose();
}

void CMainFrame::OnDestroy() 
{
	if ( m_nSemMessageTimer != 0 )
	{
		KillTimer( m_nSemMessageTimer );
		m_nSemMessageTimer = 0;
	}
	if ( m_nSemAutosaveTimer != 0 )
	{
		KillTimer( m_nSemAutosaveTimer );
		m_nSemAutosaveTimer = 0;
	}
	if ( m_nReadStatusTimer != 0 )
	{
		KillTimer( m_nReadStatusTimer );
		m_nReadStatusTimer = 0;
	}
	if ( m_nMCB_SemMessageTimer != 0 )
	{
		KillTimer( m_nMCB_SemMessageTimer );
		m_nMCB_SemMessageTimer = 0;
	}
	if ( m_nCPS_SemMessageTimer != 0 )
	{
		KillTimer( m_nCPS_SemMessageTimer );
		m_nCPS_SemMessageTimer = 0;
	}
	if ( m_nFilaLifeTimer != 0 )
	{
		KillTimer( m_nFilaLifeTimer );
		m_nFilaLifeTimer = 0;
	}
	if ( m_nAppLifeTimer != 0 )
	{
		KillTimer( m_nAppLifeTimer );
		m_nAppLifeTimer = 0;
	}

	CFrameWnd :: OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	
/*	bool bFinalFullScreenMode =
		CExtPopupMenuWnd::g_bFullScreenMode ? true : false;
	VERIFY( _DoBarStateSerialization(bFinalFullScreenMode,true) );
	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
*/	
	return CFrameWnd :: DestroyWindow();
}

//下拉菜单
void CMainFrame::OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	// load and display popup menu
	CMenu menu;
	menu.LoadMenu(IDR_RULER);
	CMenu* submenu = menu.GetSubMenu(0);
	//Rename menu item.
	submenu->ModifyMenu(ID_OTHERS_RULER_Add, MF_BYCOMMAND, ID_OTHERS_RULER_Add,
		GetResString( IDS_MENUITEM_OTHERS_RULER_Add ));

	CRect rc;
	m_wndScanCtrlBar.SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	m_wndScanCtrlBar.ClientToScreen(&rc);

	submenu->TrackPopupMenu(TPM_LEFTALIGN, rc.left, rc.bottom, this, &rc);
}

void CMainFrame::Message(LPCTSTR lpszMessage)
{
	if( m_wndImpPanelLog.m_hWnd != NULL )
		m_wndImpPanelLog.Message(lpszMessage);

/*	if( m_dRatio < 1.25 )
	{
		CViewImpPanelAll* pPanel = (CViewImpPanelAll*)( m_pwndSplitter->GetPane(0,1) );
		if( pPanel->m_hWnd != NULL )
			pPanel->Localize();
	}
	else
	{
		CViewImpPanelAll2* pPanel = (CViewImpPanelAll2*)( m_pwndSplitter->GetPane(0,1) );
		if( pPanel->m_hWnd != NULL )
			pPanel->Message( lpszMessage );
	}*/
}

void CMainFrame::Message2(LPCTSTR lpszMessage)
{
	if( m_wndImpPanelAll2.m_hWnd != NULL )
		m_wndImpPanelAll2.Message2(lpszMessage);
}

void CMainFrame::Localize(void)
{
	if( m_dlgDebug.m_hWnd != NULL )	// 09.12.10 增加调试状态功能
		m_dlgDebug.Localize();
	if( m_dlgCali.m_hWnd != NULL )
		m_dlgCali.Localize();
	if( m_dlgDebugVIP.m_hWnd != NULL )
		m_dlgDebugVIP.Localize();

	if( m_wndImpPanelLog.m_hWnd != NULL )
		m_wndImpPanelLog.Localize();
	if( m_wndImpPanelThumb.m_hWnd != NULL )
		m_wndImpPanelThumb.Localize();
	if( m_wndImpPanelAll2.m_hWnd != NULL )
		m_wndImpPanelAll2.Localize();
	if( m_dlgStage.m_hWnd != NULL )	// 15.11 自动台
		m_dlgStage.Localize();

	if( m_MainPanelDebug.m_hWnd != NULL )
	{
		m_MainPanelDebug.Localize();
		m_MainPanelDebug.m_PageDebug2.Localize();
//		m_MainPanelDebug.m_PageDebugA.Localize();
//		m_MainPanelDebug.m_PagePanelMCI.Localize();
//		m_MainPanelDebug.m_PagePanelSL.Localize();
//		m_MainPanelDebug.m_PagePanelVS.Localize();
	}

/*	if( m_MainPanel.m_hWnd != NULL )
	{
		m_MainPanel.Localize();
//		m_MainPanel.m_PagePanelAdjust.Localize();
//		m_MainPanel.m_PagePanelAccessory.Localize();
//		m_MainPanel.m_PagePanelLog.Localize();
		m_MainPanel.m_PagePanelThumb.Localize();
	}
*/
	//束流检测-语言包
	if( CControlLayer::Instance().m_nPAType > 0 && m_wndBeamCtrlBar.m_hWnd != NULL )
	{
		m_staticBeamTar.SetWindowText(GetResString(IDC_STATIC_Beam_Target) );
		m_staticBeamCur.SetWindowText(GetResString(IDC_STATIC_Beam_Current));
		m_staticBeamPort.SetWindowText(GetResString(IDC_STATIC_Beam_Port));
		m_staticBeamAper.SetWindowText(GetResString(IDC_STATIC_Beam_Aper));
		m_btnBeamDetection.SetCaption(GetResString(IDC_BUTTON_Beam_Detection));
		//m_btnBeamDetection.SetWindowText(GetResString(IDC_BUTTON_Beam_Detection));
		m_btnBeamSaveXYpos.SetWindowText(GetResString(IDC_STATIC_Beam_Unit));
	}

	// Get handle to the Window pop-up menu.
	CMenu* menubar = GetMenu();
	CMenu* wmenu = CMenu::FromHandle(GetWindowMenuPopup(menubar->GetSafeHmenu()));
	if (wmenu == NULL)
		return;

	// Get the caption of the specified MDI pChild window.
	CMDIChildWnd* pChild = MDIGetActive();
	CMDIChildWnd* pChildScan = NULL;
	do
	{
		CString str;
		pChild->GetWindowText(str);
		if (str == "SemScan")
			pChildScan = pChild;
		else
		{
			CImageView *pView = (CImageView *) pChild->GetActiveView();
			if( pView->m_wLanguageID != CGlobalParamSetting::Instance().m_wLanguageID )
				pView->Localize();
		}

		pChild = (CMDIChildWnd*) pChild->GetWindow(GW_HWNDNEXT);
	} while (pChild);

	if( pChildScan != NULL )
	{
		// Get the active view attached to the active MDI pChild
		// window.
		CScanView *pView = (CScanView *) pChildScan->GetActiveView();
		if( pView->m_wLanguageID != CGlobalParamSetting::Instance().m_wLanguageID )
			pView->Localize();
	}
}

void CMainFrame::AdjustPanelsPos()
{
	//根据屏幕分辨率动态调整对话框布局

	//将对话框位置设置到客户区右上角
	CRect rect, rectCtrl(0,0,0,0);
	GetClientRect( &rect );
	ClientToScreen( &rect );

	if( (m_wndScanCtrlBar.m_hWnd != NULL)
		&& ((m_wndScanCtrlBar.GetStyle() & WS_VISIBLE) != 0) )
	{
		m_wndScanCtrlBar.GetClientRect( (LPRECT)rectCtrl );
		rect.top += rectCtrl.Height();
	}
	int nHeight = rect.bottom -rect.top;

/*	if( theApp.m_dwOpr > 0 )
	{
		int nScreenX = GetSystemMetrics( SM_CXSCREEN );
		m_wndImpPanelLog.GetClientRect( &rectCtrl );
		ClientToScreen( &rectCtrl );
		m_wndImpPanelLog.SetWindowPos(NULL,
			10+540+20,
			rect.bottom -180,
			540,
			rectCtrl.Height() +28,
			SWP_NOZORDER );
		m_wndImpPanelLog.AdjustCtrlsPos();
		
		m_wndImpPanelThumb.GetClientRect( &rectCtrl );
		ClientToScreen( &rectCtrl );
		m_wndImpPanelThumb.SetWindowPos(NULL,
			10,
			rect.bottom -180,
			540,
			rectCtrl.Height() +28,
			SWP_NOZORDER );
		m_wndImpPanelThumb.AdjustCtrlsPos();
	}
	else
*/	{
		m_wndImpPanelLog.GetClientRect( &rectCtrl );
		ClientToScreen( &rectCtrl );

		CRect rectAll2(0,0,0,0);
		if(m_wndImpPanelAll2.m_hWnd != NULL)
			m_wndImpPanelAll2.GetClientRect( &rectAll2 );
		int nScreenX = GetSystemMetrics( SM_CXSCREEN );
		int nWidth = (nScreenX -rectAll2.Width() -5) /2 -10;

		m_wndImpPanelLog.SetWindowPos(NULL,
			5 +nWidth +15,
			rect.bottom -rect.Height() /6,
			nWidth,
			rect.Height() /6,
			SWP_NOZORDER );
		m_wndImpPanelLog.AdjustCtrlsPos();

		m_wndImpPanelThumb.GetClientRect( &rectCtrl );
		ClientToScreen( &rectCtrl );
		m_wndImpPanelThumb.SetWindowPos(NULL,
			5,
			rect.bottom -rect.Height() /6,
			nWidth,
			rect.Height() /6,
			SWP_NOZORDER );
		m_wndImpPanelThumb.AdjustCtrlsPos();

		m_dlgDebugR.GetClientRect( &rectCtrl );
		ClientToScreen( &rectCtrl );
		m_dlgDebugR.SetWindowPos(NULL,
			(nScreenX -rectAll2.Width() -rectCtrl.Width()) /2,
			rect.bottom -rectCtrl.Height() -32,
			rectCtrl.Width() + 17,
			rectCtrl.Height() + 40,
			SWP_NOZORDER );
	}

	if(m_wndImpPanelAll2.m_hWnd != NULL)
	{
		m_wndImpPanelAll2.GetClientRect( &rectCtrl );
		ClientToScreen( &rectCtrl );
		m_wndImpPanelAll2.SetWindowPos(NULL,
			rect.right - rectCtrl.Width(),
			rect.top,
			rectCtrl.Width(),
			nHeight,
			SWP_NOZORDER );
		m_wndImpPanelAll2.AdjustCtrlsPos();
	}

	if( m_dlgCali.m_hWnd != NULL )
	{
		if(m_wndImpPanelAll2.m_hWnd != NULL)
		{
			m_wndImpPanelAll2.GetClientRect( &rectCtrl );
			ClientToScreen( &rectCtrl );
		}
		CRect rectCtrl2(0,0,282,520);
		m_dlgCali.GetWindowRect( &rectCtrl2 );
		ClientToScreen( &rectCtrl2 );
		m_dlgCali.SetWindowPos(NULL,
			rect.right - rectCtrl.Width() - rectCtrl2.Width() -4*2,
			rect.top +16,
			rectCtrl2.Width(),
			rectCtrl2.Height(),
			SWP_NOZORDER );
	}

	if( m_dlgDebug.m_hWnd != NULL )
	{
		if(m_wndImpPanelAll2.m_hWnd != NULL)
		{
			m_wndImpPanelAll2.GetClientRect( &rectCtrl );
			ClientToScreen( &rectCtrl );
		}
		CRect rectCtrl2;
		m_dlgDebug.GetClientRect( &rectCtrl2 );
		ClientToScreen( &rectCtrl2 );
		m_dlgDebug.SetWindowPos(NULL,
			rect.right - rectCtrl.Width() - rectCtrl2.Width() -4*2,
			rect.top +16,
			rectCtrl2.Width() + 10,
			rectCtrl2.Height() +28,
			SWP_NOZORDER );
	}
}

//几个工具栏的显隐

void CMainFrame::OnViewBar0() 
{
	// TODO: Add your command handler code here
	if (NULL == m_wndScanCtrlBar.m_hWnd) return;
	BOOL bVisible = ((m_wndScanCtrlBar.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
		m_wndScanCtrlBar.ShowWindow(SW_HIDE);
	else
		m_wndScanCtrlBar.ShowWindow(SW_SHOW);
	AdjustPanelsPos();
}

void CMainFrame::OnUpdateViewBar0(CCmdUI* pCmdUI) 
{
	if (NULL == m_wndScanCtrlBar.m_hWnd) return;
	BOOL bVisible = ((m_wndScanCtrlBar.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

void CMainFrame::OnViewBar1() 
{
	// TODO: Add your command handler code here
	if(NULL == m_dlgDebugVIP.m_hWnd) return;
	BOOL bVisible = ((m_dlgDebugVIP.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
	{
		m_dlgDebugVIP.ShowWindow(SW_HIDE);
		if( m_wndImpPanelAll2.m_hWnd != NULL )
		{
			m_wndImpPanelAll2.GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_SetLength)->ShowWindow(SW_HIDE);
			m_wndImpPanelAll2.GetDlgItem(IDC_CHECK_Debug_HV)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_dlgDebugVIP.ShowWindow(SW_SHOW);
		if( m_wndImpPanelAll2.m_hWnd != NULL )
		{
			m_wndImpPanelAll2.GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_SetLength)->ShowWindow(SW_SHOW);
			m_wndImpPanelAll2.GetDlgItem(IDC_CHECK_Debug_HV)->ShowWindow(SW_SHOW);
		}
	}
	AdjustPanelsPos();
}

void CMainFrame::OnUpdateViewBar1(CCmdUI* pCmdUI) 
{
	if( NULL == m_dlgDebugVIP.m_hWnd ) return;
	BOOL bVisible = ((m_dlgDebugVIP.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

void CMainFrame::OnViewPanel1() 
{
	if( NULL == m_wndImpPanelAll2.m_hWnd ) return;
	BOOL bVisible = ((m_wndImpPanelAll2.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
		m_wndImpPanelAll2.ShowWindow(SW_HIDE);
	else
		m_wndImpPanelAll2.ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateViewPanel1(CCmdUI* pCmdUI) 
{
	if( NULL == m_wndImpPanelAll2.m_hWnd ) return;
	BOOL bVisible = ((m_wndImpPanelAll2.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
	pCmdUI->Enable( theApp.m_dwOpr < 1 );
}

void CMainFrame::OnViewPanel2() 
{
	if (NULL == m_dlgDebugR.m_hWnd) return;
	BOOL bVisible = ((m_dlgDebugR.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
		m_dlgDebugR.ShowWindow(SW_HIDE);
	else
		m_dlgDebugR.ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateViewPanel2(CCmdUI* pCmdUI) 
{
	if (NULL == m_dlgDebugR.m_hWnd) return;
	BOOL bVisible = ((m_dlgDebugR.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

void CMainFrame::OnViewPanel3() 
{
	if (NULL == m_HistoBar.m_hWnd) return;
	BOOL bVisible = ((m_HistoBar.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
		ShowControlBar(&m_HistoBar, FALSE, FALSE);
	else
		ShowControlBar(&m_HistoBar, TRUE, FALSE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewPanel3(CCmdUI* pCmdUI) 
{
	if (NULL == m_HistoBar.m_hWnd) return;
	BOOL bVisible = ((m_HistoBar.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

void CMainFrame::OnViewPanel4() 
{
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 )
			StageTMCM_ShowWnd(this->m_hWnd);
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 )
			StageMC6600_ShowWnd(this->m_hWnd);
		else
			StageMC600_ShowWnd(this->m_hWnd);
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage == 1 && m_dlgStage.m_hWnd != NULL )
	{
		BOOL bVisible = ((m_dlgStage.GetStyle() & WS_VISIBLE) != 0);
		if (bVisible)
			m_dlgStage.ShowWindow(SW_HIDE);
		else
			m_dlgStage.ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnUpdateViewPanel4(CCmdUI* pCmdUI) 
{
/*	if( CStageXYManager::Instance().m_Stage.nHasAutoStage == 1 && m_dlgStage.m_hWnd != NULL )
	{
		BOOL bVisible = ((m_dlgStage.GetStyle() & WS_VISIBLE) != 0);
		pCmdUI->SetCheck(bVisible);
	}*/
	pCmdUI->Enable( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 );
}

void CMainFrame::OnViewPanel5() 
{
	if(NULL == m_dlgAutotest.m_hWnd ) return;
	BOOL bVisible = ((m_dlgAutotest.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
		m_dlgAutotest.ShowWindow(SW_HIDE);
	else
		m_dlgAutotest.ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateViewPanel5(CCmdUI* pCmdUI) 
{
	if(NULL == m_dlgAutotest.m_hWnd ) return;
	BOOL bVisible = ((m_dlgAutotest.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
//	pCmdUI->Enable( theApp.m_dwOpr > 0 );
}

void CMainFrame::OnViewPanel6() 
{
	if( NULL == m_dlgCali.m_hWnd ) return;
	BOOL bVisible = ((m_dlgCali.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
	{
		m_dlgCali.ShowWindow(SW_HIDE);
		theApp.m_dwOpr = 0;
	}
	else
	{
		m_dlgCali.ShowWindow(SW_SHOW);
		theApp.m_dwOpr = 1;
	}
	CSemCoreAssemble::Instance().m_Brightness.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_Contrast.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_CondenserLens.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_BeemAlignmentX.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_BeemAlignmentY.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_ScanShiftX.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_ScanShiftY.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_StigmatorX.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_StigmatorY.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_SP_PMT.SetParams( 100, theApp.m_dwOpr );

	if( m_dlgDebugR.m_hWnd != NULL )
		m_dlgDebugR.ShowGenerate();
}

void CMainFrame::OnUpdateViewPanel6(CCmdUI* pCmdUI) 
{
	if( m_dlgCali.m_hWnd != NULL )
	{
		BOOL bVisible = ((m_dlgCali.GetStyle() & WS_VISIBLE) != 0);
		pCmdUI->SetCheck(bVisible);
	}
//	pCmdUI->Enable( theApp.m_dwOpr > 0 );
}

void CMainFrame::OnViewPanel7() 
{
	if( m_dlgGraph.m_hWnd != NULL )
		m_dlgGraph.ShowWindow( SW_SHOW );
	if( m_dlgDiffGraph.m_hWnd != NULL )
		m_dlgDiffGraph.ShowWindow( SW_SHOW );
}

void CMainFrame::OnUpdateViewPanel7(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( theApp.m_dwOpr > 0 );
}

void CMainFrame::OnViewPanel8() 
{
	if (NULL == m_wndImpPanelThumb.m_hWnd) return;
	BOOL bVisible = ((m_wndImpPanelThumb.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
		m_wndImpPanelThumb.ShowWindow(SW_HIDE);
	else
		m_wndImpPanelThumb.ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateViewPanel8(CCmdUI* pCmdUI) 
{
	if (NULL == m_wndImpPanelThumb.m_hWnd) return;
	BOOL bVisible = ((m_wndImpPanelThumb.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

void CMainFrame::OnViewPanel9() 
{
	if( NULL == m_wndImpPanelLog.m_hWnd ) return;
	BOOL bVisible = ((m_wndImpPanelLog.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
		m_wndImpPanelLog.ShowWindow(SW_HIDE);
	else
		m_wndImpPanelLog.ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateViewPanel9(CCmdUI* pCmdUI) 
{
	if (NULL == m_wndImpPanelLog.m_hWnd ) return;
	BOOL bVisible = ((m_wndImpPanelLog.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

BOOL CMainFrame::ActivateMDIChild( CString strCaption ) 
{
	// Get handle to the Window pop-up menu.
	CMenu* menubar = GetMenu();
	CMenu* wmenu = CMenu::FromHandle(GetWindowMenuPopup(menubar->GetSafeHmenu()));
	if (wmenu == NULL)
		return FALSE;

	// Get the caption of the specified MDI pChild window.
	CMDIChildWnd* pChild = MDIGetActive();
	do
	{
		CString str;
		pChild->GetWindowText(str);
		if (str == strCaption)
		{
			pChild->MDIActivate();        // or MDIActivate(pChild);
			pChild->MDIMaximize();
			pChild->SetFocus();
			return TRUE;	//break;
		}

		pChild = (CMDIChildWnd*) pChild->GetWindow(GW_HWNDNEXT);
	} while (pChild);

	return FALSE;
}

BOOL CMainFrame::DestroyMDIChild( CString strCaption ) 
{
	// Get handle to the Window pop-up menu.
	CMenu* menubar = GetMenu();
	CMenu* wmenu = CMenu::FromHandle(GetWindowMenuPopup(menubar->GetSafeHmenu()));
	if (wmenu == NULL)
		return FALSE;

	CMDIChildWnd* pChild = MDIGetActive();
	do
	{
		CString str;
		pChild->GetWindowText(str);
		if (str == strCaption)
		{
			pChild->MDIDestroy();
			return TRUE;	//break;
		}

		pChild = (CMDIChildWnd*) pChild->GetWindow(GW_HWNDNEXT);
	} while (pChild);

	return FALSE;
}

void CMainFrame::MinimizeImageMDIChild()
{
	// Get the caption of the specified MDI pChild window.
	CMDIChildWnd* pChild = MDIGetActive();
	CMDIChildWnd* pChildBak = pChild;
	do
	{
		pChild = (CMDIChildWnd*) pChildBak->GetWindow(GW_HWNDNEXT);
		if( pChildBak->IsKindOf( RUNTIME_CLASS(CChildFrame) ) )
		{
			pChildBak->ShowWindow( SW_MINIMIZE );
		}

		pChildBak = pChild;
	} while (pChild);
}

// 09.12.10 增加调试状态功能
void CMainFrame::OnOptionsDebug() 
{
	// TODO: Add your command handler code here
	if(NULL == m_dlgDebug.m_hWnd) return;
	BOOL bVisible = ((m_dlgDebug.GetStyle() & WS_VISIBLE) != 0);
	if (bVisible)
	{
		DebugClose();
		m_dlgDebug.ShowWindow(SW_HIDE);
	}
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			m_dlgDebug.m_pPageMon->VAC_UpdatePneumaticDiagram();
			if( CSemCtrl::Instance().m_nVACBoardType < 1 )
			{
				m_dlgDebug.m_pPageMon->VAC_UpdateSMvacTCAmpUpdate( 0, CVACDataManager::Instance().m_VacTCAmp.dTCfAmp );
				m_dlgDebug.m_pPageMon->VAC_UpdateSMvacTCAmpUpdate( 1, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
			}
			else if( CSemCtrl::Instance().m_nVACBoardType < 2 )
			{
				m_dlgDebug.m_pPageMon->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
				m_dlgDebug.m_pPageMon->VAC_UpdateTCAmpUpdate( 1, CVACDataManager::Instance().m_valueTC.fTC4 );
			}
			else if( CSemCtrl::Instance().m_nVACBoardType < 10 )
				m_dlgDebug.m_pPageMon->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
			m_dlgDebug.ShowWindow(SW_SHOW);
		}
		else
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			{
				m_dlgDebug.m_pPageMon3->VAC_UpdatePneumaticDiagram();
				if( CSemCtrl::Instance().m_nVACBoardType < 1 )
				{
					m_dlgDebug.m_pPageMon3->VAC_UpdateSMvacTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
					m_dlgDebug.m_pPageMon3->VAC_UpdateSMvacTCAmpUpdate( 1, CVACDataManager::Instance().m_valueTC.fTC4 );
				}
				else if( CSemCtrl::Instance().m_nVACBoardType < 2 )
				{
					m_dlgDebug.m_pPageMon3->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
					m_dlgDebug.m_pPageMon3->VAC_UpdateTCAmpUpdate( 1, CVACDataManager::Instance().m_valueTC.fTC4 );
				}
				else if( CSemCtrl::Instance().m_nVACBoardType < 10 )
					m_dlgDebug.m_pPageMon3->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
			}
			else
			{
				m_dlgDebug.m_pPageMon2->VAC_UpdatePneumaticDiagram();
				if( CSemCtrl::Instance().m_nVACBoardType < 1 )
				{
					m_dlgDebug.m_pPageMon2->VAC_UpdateSMvacTCAmpUpdate( 0, CVACDataManager::Instance().m_VacTCAmp.dTCfAmp );
					m_dlgDebug.m_pPageMon2->VAC_UpdateSMvacTCAmpUpdate( 1, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
				}
				else if( CSemCtrl::Instance().m_nVACBoardType < 2 )
				{
					m_dlgDebug.m_pPageMon2->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
					m_dlgDebug.m_pPageMon2->VAC_UpdateTCAmpUpdate( 1, CVACDataManager::Instance().m_valueTC.fTC4 );
				}
				else if( CSemCtrl::Instance().m_nVACBoardType < 10 )
					m_dlgDebug.m_pPageMon2->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
			}
			m_dlgDebug.ShowWindow(SW_SHOW);
		}
	}
}

void CMainFrame::OnUpdateOptionsDebug(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(NULL == m_dlgDebug.m_hWnd ) return;
	BOOL bVisible = ((m_dlgDebug.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
	pCmdUI->Enable(!bVisible);
}


void CMainFrame::OnOptionsTestParameter()
{
	m_dlgTestParameter.DoModal();
}

void CMainFrame::OnUpdateOptionsTestParameter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CVACDataManager::Instance().m_VacParamMain.bV1);
}


void CMainFrame::OnOptionsManageScenario()
{
	ScenarioManageDlg  dlgScenario;
	dlgScenario.DoModal();
}

void CMainFrame::OnUpdateOptionsManageScenario(CCmdUI* pCmdUI)
{
	BOOL bHVButtonEnable = (BOOL)m_wndImpPanelAll2.m_btnHV.GetCheck();
	BOOL bBiasButtonEnable = (BOOL)m_wndImpPanelAll2.m_btnBias.GetCheck();
	BOOL bFilamentButtonEnable = (BOOL)m_wndImpPanelAll2.m_btnFilament.GetCheck();
	BOOL bEnable = CVACDataManager::Instance().m_VacParamMain.bV1 && bHVButtonEnable && bBiasButtonEnable && bFilamentButtonEnable;
	pCmdUI->Enable(bEnable);
}

void CMainFrame::OnOptionsLog()
{
	CDlgSEMLog_Data  dlg;
	dlg.DoModal();
}


void CMainFrame::OnUpdateOptionsLog(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnLog_Show()
{
	CDlgSEMLog_Data  dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateLog_Show(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnLog_Settings()
{
	CDlgSEMLog_DataCurve  dlg;
	dlg.DoModal();
}


void CMainFrame::OnUpdateLog_Settings(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnLog_AdvancedSettings()
{
	CDlgSEMLog_Settings  dlg;
	dlg.DoModal();
}


void CMainFrame::OnUpdateLog_AdvancedSettings(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


// 10.01.22 增加调试状态功能参数：最佳亮度、最佳对比度值
void CMainFrame::SetupDebugParam( BOOL bDebugHV, BOOL bDebugAccel, BOOL bDebugVac, BOOL bDebugStage, int nBrightness, int nContrast )
{
	// 设置快捷键表IDR_DEBUG
/*	if( bDebugAccel == TRUE )
	{
		if( m_hAccelDebug == NULL )
			m_hAccelDebug = ::LoadAccelerators(AfxGetResourceHandle(), 
										MAKEINTRESOURCE(IDR_DEBUG));
	}
	else
	{
		if( m_hAccelDebug != NULL )
		{
			DestroyAcceleratorTable( m_hAccelDebug );
			m_hAccelDebug = NULL;
		}
	}*/
	// 设置高压不连锁状态
	// 10.01.22 增加调试状态功能参数：最佳亮度、最佳对比度值
	theApp.SetupDebugParam( bDebugHV, nBrightness, nContrast );
	// 设置真空手动操作状态
	if( m_dlgDebugVIP.m_hWnd )
		m_dlgDebugVIP.SetupDebugParam( bDebugVac );
	// 设置自动台调试状态
	if( m_dlgStage.m_hWnd != NULL )
		m_dlgStage.SetupDebugParam( bDebugStage );
	// 设置消像散快捷键
	CScanView* pView = (CScanView*)theApp.GetScanView();
	pView->m_bDebugAccel = bDebugAccel;
	// 试用期相关
	if( m_dlgDebugR.m_hWnd != NULL )
		m_dlgDebugR.SetupDebugParam( bDebugHV, bDebugVac, bDebugStage );
}

void CMainFrame::DebugClose()
{
	// 10.01.22 增加调试状态功能参数：最佳亮度、最佳对比度值
	SetupDebugParam( FALSE, FALSE, FALSE, FALSE, 1000, -1 );
	if(NULL == m_wndImpPanelAll2.m_hWnd) return;
	m_wndImpPanelAll2.GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_SetLength)->ShowWindow(SW_HIDE);
	m_wndImpPanelAll2.GetDlgItem(IDC_CHECK_Debug_HV)->ShowWindow(SW_HIDE);
	// 调试模式下任何放大倍数不禁用扫描旋转
	CSemCoreAssemble::Instance().m_Magnifier.SetParams( 50, 0 );
	theApp.DisplayMsg( "ScanRotate normal mode" );
}
// 09.12.10 增加调试状态功能

void CMainFrame::SetTimers( int nID, BOOL bSet )
{
	if( bSet )
	{
		switch( nID )
		{
		case	nSemMessageTimerID:
			m_nSemMessageTimer	= SetTimer( nSemMessageTimerID, nSemMessageTimerElapse, NULL );
			break;
		case	nSemAutosaveTimerID:
			m_nSemAutosaveTimer = SetTimer( nSemAutosaveTimerID, nSemAutosaveTimerElapse, NULL );
			break;
		case	nMCB_SemMessageTimerID:
			m_nMCB_SemMessageTimer	= SetTimer( nMCB_SemMessageTimerID, nMCB_SemMessageTimerElapse, NULL );
			break;
		}
	}
	else
	{
		switch( nID )
		{
		case	nSemMessageTimerID:
			if ( m_nSemMessageTimer != 0 )
			{
				KillTimer( m_nSemMessageTimer );
				m_nSemMessageTimer = 0;
			}
			break;
		case	nSemAutosaveTimerID:
			if ( m_nSemAutosaveTimer != 0 )
			{
				KillTimer( nSemAutosaveTimerID );
				m_nSemAutosaveTimer = 0;
			}
			break;
		case	nMCB_SemMessageTimerID:
			if ( m_nMCB_SemMessageTimer != 0 )
			{
				KillTimer( m_nMCB_SemMessageTimer );
				m_nMCB_SemMessageTimer = 0;
			}
			break;
		}
	}
}

void CMainFrame::OnSelchangeComboSpeed(void)
{
	// USB_Pixel_Rate即N，为1-2K整数
	// USB_Pixel_Clk = N * 125 ns
	// Pixel_Clk = USB_Pixel_Clk * 4 = N * 500 ns = N / 2 us
	// 组合框中显示出的是整秒的点时钟Pixel_Clk，其index = N /2
	// 实际下送值是N值，所以要将1以上的index值倍乘
	int nIndex = m_wndSpeed.GetCurSel();
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( nIndex == 0 )
		nIndex = 1;
	else
		nIndex *=2;
	pView->NotifyPixRate( nIndex );
}

void CMainFrame::OnDeltaposSPINPixRate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		m_nusbPixRate -= 1;
	else
		m_nusbPixRate += 1;

	UpdatePixRate( m_nusbPixRate );

	*pResult = 0;
}

void CMainFrame::UpdatePixRate( int& nPixRate )
{
	if( nPixRate > 511 )
		nPixRate = 511;
	if( nPixRate < 1 )
		nPixRate = 1;

	CString str;
	str.Format( "%.1f us", nPixRate /2.0 );
	m_wndPixRateEdit.SetWindowText( str );
	CScanView* pView = (CScanView*)theApp.GetScanView();
	pView->NotifyPixRate( nPixRate );
}

void CMainFrame::OnPANELMAINBtnLastStep() 
{
	// TODO: Add your command handler code here
	if(NULL == m_MainPanelDebug.m_hWnd) return;
	m_MainPanelDebug.SelchangePage(m_MainPanelDebug.m_nSelectPage -1);
	if( m_MainPanelDebug.m_nSelectPage == 0 )
		m_MainPanelDebug.m_PageDebug2.MCI_SyncParams();
	else if( m_MainPanelDebug.m_nSelectPage == 2 )
		m_MainPanelDebug.m_PagePanelMCI.MCI_SyncParams();
}

void CMainFrame::OnPANELMAINBtnNextStep() 
{
	// TODO: Add your command handler code here
	if(NULL == m_MainPanelDebug.m_hWnd) return;
	m_MainPanelDebug.SelchangePage(m_MainPanelDebug.m_nSelectPage +1);
	if( m_MainPanelDebug.m_nSelectPage == 0 )
		m_MainPanelDebug.m_PageDebug2.MCI_SyncParams();
	else if( m_MainPanelDebug.m_nSelectPage == 2 )
		m_MainPanelDebug.m_PagePanelMCI.MCI_SyncParams();
}

void CMainFrame::OnDeltaposSPINVideoGraph(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		m_debugOSC.nOscillIndex++;
	else
		m_debugOSC.nOscillIndex--;
	if( m_debugOSC.nOscillIndex > 9 )
		m_debugOSC.nOscillIndex = 9;
	if( m_debugOSC.nOscillIndex < 1 )
		m_debugOSC.nOscillIndex = 1;

	CSemVirtualMCICard::Instance().SetParams( 7, m_debugOSC.nOscillIndex );
	*pResult = 0;
}

void CMainFrame::OnDeltaposSPINVideoGraphOffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		m_debugOSC.dOffset -= 5;
	else
		m_debugOSC.dOffset += 5;

	CString str;
	str.Format( "%.4f", m_debugOSC.dOffset );
	m_wndVideoGraphOffsetEdit.SetWindowText( str );
	CSemVirtualMCICard::Instance().SetParams( 9, (int)(m_debugOSC.dOffset * 10000) );
	*pResult = 0;
}

//#define       WINVER       0x0500	// OnDeviceChange
//#include <Dbt.h>

BOOL CMainFrame::OnDeviceChange(UINT nEventType, DWORD dwData)
{
/*	DEV_BROADCAST_DEVICEINTERFACE* dbd = (DEV_BROADCAST_DEVICEINTERFACE*) dwData;

	//这里进行信息匹配,比如guid等

	//针对各个事件进行处理.
	switch (nEventType)
	{
	case DBT_DEVICEREMOVECOMPLETE:
			break;
	case DBT_DEVICEARRIVAL:
			break;
	default:
		break;
	}
*/
	return TRUE;
}

void CMainFrame::LoggingFilamentLifetime_Start()
{
	m_nFilaLifeTimer = SetTimer(200, 1000*60*3, NULL);	// 3分钟记录一次(0.05小时)
}

void CMainFrame::LoggingFilamentLifetime_End()
{
	KillTimer(200);
}

void CMainFrame::LoggingFilamentLifetime()
{
	UINT nLifetime = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, 
		g_csRegEntry_nFilaLifeTime, 0 );
	nLifetime += 3;
	theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nFilaLifeTime, nLifetime );

	if( m_dlgDebugVIP.m_hWnd != NULL )
	{
		CString str;
		str.Format( "%.1f", nLifetime /60.0 );
		m_dlgDebugVIP.GetDlgItem( IDC_PANEL_LOG_EDIT_Log )->SetWindowText( str );
	}
}

void CMainFrame::LoggingAppLifetime_Start()
{
	m_nAppLifeTimer = SetTimer(201, 1000*60*5, NULL);	// 5分钟记录一次
}

void CMainFrame::LoggingAppLifetime_End()
{
	KillTimer(201);
}

void CMainFrame::LoggingAppLifetime()
{
	CProductVersionManager::Instance().Trial_SaveTime( FALSE );
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
// StageMC600 Message

LRESULT CMainFrame::OnStageMC600msgChanged(WPARAM wParam, LPARAM lParam)
{
	// StageMC600 photo electric switch
	int nWsize		= 8;
	int nModID		= USB_MID_SPR;
	BYTE bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	WORD wData;
	if( lParam > 0 )
	{
		theApp.DisplayMsg( "Receive MC600 msg: SPR_En: J15 PE_switch ON");
		wData		= 0x43;		// Strobe IOX output low
	}
	else
	{
		theApp.DisplayMsg( "Receive MC600 msg: SPR_En: J15 PE_switch OFF");
		wData		= 0x83;		// Strobe IOX output high
	}
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	Sleep(1000);
	if( lParam > 0 )
		StageMC600_SetParam( 1, 1.0 );
	else
		StageMC600_SetParam( 1, -1.0 );
	return 0;
}


LRESULT CMainFrame::OnDisplayDebugMessage(WPARAM wParam, LPARAM lParam)
{
	CString sDebugMsg = *((CString*)(wParam));
	theApp.DisplayMsg(sDebugMsg);
	return 0L;
}

LRESULT CMainFrame::OnAutoStageXY_FI_Init(WPARAM wParam, LPARAM lParam)
{
	CScanView* pView = (CScanView*)(theApp.GetScanView());
	if(!pView) return -1L;

	CDlgPrompt dlg;
	dlg.m_byteType = (BYTE)0x02;
	dlg.m_strMsg.Format( "%s ?", GetResString( IDS_Stage_SD_TITLE_AUTO_FindIndex ) );
	if( dlg.DoModal() == IDOK )
	{
		pView->AutoStageXY_FI_Init();
	}
	else
	{
		if( m_dlgStage.m_hWnd != NULL)
			m_dlgStage.StagePosUpdate();
	}

	return 0L;
}

LRESULT CMainFrame::OnSyncFineMag(WPARAM wParam, LPARAM lParam)
{
	int* pParam = (int*)lParam;
	int param[3];
	param[0] = pParam[0];
	param[1] = pParam[1];
	param[2] = pParam[2];
	delete pParam; pParam = NULL;
	if( m_dlgCali.m_hWnd != NULL )
	{
		m_dlgCali.MCI_SyncDet45( param[0], param[1] );
		// 2021.11: 最小放大倍数档超时保护
		m_dlgCali.UpdateMagStage( param[2] );
	}

	return 0L;
}


LRESULT CMainFrame::OnCoUpdateScan(WPARAM wParam, LPARAM lParam)
{
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
		if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
			if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_AnalogPhoto )
				pView->USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );

	return 0L;
}


LRESULT CMainFrame::OnUpdateBeemAlignmentX(WPARAM wParam, LPARAM lParam)
{
	// BeemAlignment
	int nPos = (int)wParam;
	if( m_wndImpPanelAll2.m_hWnd != NULL )
	{
		VARIANT Xpos, Ypos, Flag;
		Xpos.lVal = nPos /2;
		Flag.ulVal	= (DWORD)0x01;
		m_wndImpPanelAll2.m_ctrlBeemAlignment.SyncPos( Xpos, Ypos, Flag );
	}
	return 0L;
}


LRESULT CMainFrame::OnUpdateBeemAlignmentY(WPARAM wParam, LPARAM lParam)
{
	int nPos = (int)wParam;
	if( m_wndImpPanelAll2.m_hWnd != NULL )
	{
		VARIANT Xpos, Ypos, Flag;
		Ypos.lVal = nPos /2;
		Flag.ulVal	= (DWORD)0x02;
		m_wndImpPanelAll2.m_ctrlBeemAlignment.SyncPos( Xpos, Ypos, Flag );
	}
	return 0L;
}


LRESULT CMainFrame::OnShowDFocusCtrl(WPARAM wParam, LPARAM lParam)
{
	int nShow = int(wParam);
	if( (m_MainPanelDebug.m_hWnd != NULL)
		&& (m_MainPanelDebug.m_PageDebug2.m_hWnd != NULL) )
		m_MainPanelDebug.m_PageDebug2.m_ctrlDFocus.ShowWindow( nShow );

	return 0L;
}


LRESULT CMainFrame::OnSyncContrastCtrlText(WPARAM wParam, LPARAM lParam)
{	
	CString* sMsg = (CString*)wParam;
	CString strX = *sMsg;
	delete sMsg; sMsg = NULL;

	VARIANT* pFlag = (VARIANT*)lParam;
	VARIANT Flag = *pFlag;
	delete pFlag; pFlag = NULL;

	if( m_wndImpPanelAll2.m_hWnd != NULL )
		m_wndImpPanelAll2.m_ctrlContrast.SyncText( strX, strX, Flag );

	return 0L;
}


//自动亮度对比度(手控盒)
LRESULT CMainFrame::OnControlPanelAutoBrightContrast(WPARAM wParam, LPARAM lParam)
{
	CScanView* pView = dynamic_cast<CScanView*>(theApp.GetScanView());
	if(!pView) return -1L;

	pView->FuncAutoBrightContrast();
	return 0L;
}


//自动聚焦(手控盒)
LRESULT CMainFrame::OnControlPanelAutoFocus(WPARAM wParam, LPARAM lParam)
{
	CScanView* pView = dynamic_cast<CScanView*>(theApp.GetScanView());
	if(!pView) return -1L;

	pView->FuncAutoFocus();
	return 0L;
}


LRESULT CMainFrame::OnControlPanelChangeScanSpeed(WPARAM wParam, LPARAM lParam)
{
	int nSpeedMode = (int)wParam;
	PostMessage(WM_COMMAND,ID_SCAN_SPEED_0 + nSpeedMode,NULL);
	return 0L;
}

LRESULT CMainFrame::OnControlPanelPhotoPreview(WPARAM wParam, LPARAM lParam)
{
	CScanView* pView = dynamic_cast<CScanView*>(theApp.GetScanView());
	if(!pView) return -1L;

	pView->OnPhotoPreview();
	return 0L;
}

LRESULT CMainFrame::OnControlPanelSyncScan(WPARAM wParam, LPARAM lParam)
{
	CScanView* pView = dynamic_cast<CScanView*>(theApp.GetScanView());
	if(!pView) return -1L;

	pView->OnScanTypeChanged(ID_SCAN_TYPE_CAMERA_SIMULATE);
	return 0L;
}



LRESULT CMainFrame::OnControlPanelStopScan(WPARAM wParam, LPARAM lParam)
{
	CScanView* pView = dynamic_cast<CScanView*>(theApp.GetScanView());
	if(!pView) return -1L;

	pView->OnScanTypeChanged(ID_SCAN_TYPE_FREEZE);

	return 0L;
}

LRESULT CMainFrame::OnControlPanelChangeScan(WPARAM wParam, LPARAM lParam)
{
	CScanView* pView = dynamic_cast<CScanView*>(theApp.GetScanView());
	if(!pView) return -1L;

	int nType = (int)wParam;
	if (1 == nType)
	{
		pView->OnScanTypeChanged(ID_SCAN_TYPE_ACTIVERECT);
	}
	else if(0 == nType) 
	{
		pView->OnScanTypeChanged(ID_SCAN_TYPE_NORMAL);
	}
	
	return 0L;
}

//检查CCG是否应该报警
LRESULT CMainFrame::OnCheckVACAlarm(WPARAM wParam, LPARAM lParam)
{
	__int64 _i64Time = (__int64)wParam;
/*	double*  pValue = (double*)lParam;
	CAlarmManager::Instance().CheckVAC(_i64Time,pValue[0]);
	CRealTimeDataManager::Instance().AddData(_i64Time,pValue[0],pValue[1],pValue[2],pValue[3]);
	delete[] pValue;
	pValue = NULL;
*/
	Log_HVVACData logHVVACdata = *((Log_HVVACData*)(lParam));
	CAlarmManager::Instance().CheckVAC(_i64Time,logHVVACdata.dCCG);
	CRealTimeDataManager::Instance().AddData(_i64Time,logHVVACdata.dCCG,logHVVACdata.dHV,logHVVACdata.dFil,logHVVACdata.dEMI);
	return 0L;
}


void CMainFrame::_SaveScenario() 
{
	return;
	CScenario* pScenario = CControlLayer::Instance().m_pScenario;
	pScenario->HVType = CHVPowerDataManager::Instance().m_dwHVtype;
	CString strUIValue = "";
	char sz[512] = {0};
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		strUIValue = CSemCoreAssemble::Instance().m_CPS_HV.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 3);
		pScenario->HV = atof(strUIValue.GetBuffer());

		strUIValue = CSemCoreAssemble::Instance().m_CPS_Bias.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 2);
		pScenario->Bias = atof(strUIValue.GetBuffer());

		strUIValue = CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 2);
		pScenario->FilamentCurrent = atof(strUIValue.GetBuffer());

		strUIValue = CSemCoreAssemble::Instance().m_Contrast.FormatText();
		pScenario->Contrast = atof(strUIValue.GetBuffer());
	}
	else
	{
		strUIValue = CSemCoreAssemble::Instance().m_SP_HV.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 3);
		pScenario->HV = atof(strUIValue.GetBuffer());

		strUIValue = CSemCoreAssemble::Instance().m_SP_Bias.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 2);
		pScenario->Bias = atof(strUIValue.GetBuffer());

		strUIValue = CSemCoreAssemble::Instance().m_SP_Heater.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 2);
		pScenario->FilamentCurrent = atof(strUIValue.GetBuffer());

		strUIValue = CSemCoreAssemble::Instance().m_SP_PMT.FormatText();
		pScenario->Contrast = atof(strUIValue.GetBuffer());
	}

	strUIValue = CSemCoreAssemble::Instance().m_Brightness.FormatText();
	pScenario->Brightness = atof(strUIValue.GetBuffer());

	strUIValue = CSemCoreAssemble::Instance().m_CondenserLens.FormatText();
	pScenario->CondenserLens = atof(strUIValue.GetBuffer());

	strUIValue = CSemCoreAssemble::Instance().m_ObjectiveLens.FormatText();
	pScenario->ObjectiveLens = atof(strUIValue.GetBuffer());

	strUIValue = CSemCoreAssemble::Instance().m_Magnifier.FormatText();
	int nKX = strUIValue.Find(" KX");
	if (-1 != nKX)
	{
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 3);
		pScenario->Magnifier = atof(strUIValue.GetBuffer()) * 1000;
	}
	else
	{
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 2);
		pScenario->Magnifier = atof(strUIValue.GetBuffer());
	}

	strUIValue = CSemCoreAssemble::Instance().m_BeemAlignmentX.FormatText();
	pScenario->BeemAlignmentX = atof(strUIValue.GetBuffer());

	strUIValue = CSemCoreAssemble::Instance().m_BeemAlignmentY.FormatText();
	pScenario->BeemAlignmentY = atof(strUIValue.GetBuffer());

	strUIValue = CSemCoreAssemble::Instance().m_StigmatorX.FormatText();
	pScenario->StigmatorX = atof(strUIValue.GetBuffer());

	strUIValue = CSemCoreAssemble::Instance().m_StigmatorY.FormatText();
	pScenario->StigmatorY = atof(strUIValue.GetBuffer());
}


int CMainFrame::_CreatePanelLog() 
{
	if (!m_wndImpPanelLog.Create( CDlgImpPanelLog::IDD, this ))
	{
		TRACE0("Failed to create CDlgImpPanelLog dlg\n");
		return -1;       // fail to create
	}
	m_wndImpPanelLog.ShowWindow(SW_HIDE);
	return 0;
}

int CMainFrame::_CreatePanelThumb()
{
	if (!m_wndImpPanelThumb.Create( CDlgImpPanelThumb::IDD, this ))
	{
		TRACE0("Failed to create CDlgImpPanelThumb dlg\n");
		return -1;       // fail to create
	}
	m_wndImpPanelThumb.ShowWindow(SW_HIDE);
	return 0;
}

void CMainFrame::_LoadSettingsFromRegistry()
{
	char szBuf[255];
	DWORD dwSize = 255;
	CString strLogo, strSubKey, strRegEntry = "SemType";
	strSubKey.Format( "Software\\KYKY" );
	HKEY hk;
	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
		(LPCTSTR)strSubKey,
		0, 
		KEY_ALL_ACCESS,
		&hk ) == ERROR_SUCCESS )
	{
		if( RegQueryValueEx( hk, 
			(LPCTSTR)strRegEntry,
			NULL,
			NULL,
			(LPBYTE)szBuf,
			&dwSize) == ERROR_SUCCESS )
		{
			strLogo.Format( "%s", szBuf );
			strLogo = strLogo.Left(4);
			if( strLogo == "6200" || strLogo == "6900" )
				theApp.m_strLogo.Format( "KYKY-EM%s", strLogo );
		}
	}
	RegCloseKey(hk);
	//--Debug------------------------------
	theApp.m_dwOpr = 0;
	dwSize = sizeof(DWORD);
	CString strTitle;
	strTitle.LoadString( AFX_IDS_APP_TITLE );
	strSubKey.Format( "Software\\KYKY\\%s\\%s", strTitle, g_csSettingSection );
	strRegEntry = "AdvancedOpr";
	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
		(LPCTSTR)strSubKey,
		0, 
		KEY_ALL_ACCESS,
		&hk ) == ERROR_SUCCESS )
	{
		if( RegQueryValueEx( hk, 
			(LPCTSTR)strRegEntry,
			NULL,
			NULL,
			(LPBYTE)&(theApp.m_dwOpr),
			&dwSize) == ERROR_SUCCESS )
			RegCloseKey(hk);
	}
	// 高压电源类型
	CHVPowerDataManager::Instance().m_dwHVtype = 1;
	strRegEntry = "HVtype";
	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
		(LPCTSTR)strSubKey,
		0, 
		KEY_ALL_ACCESS,
		&hk ) == ERROR_SUCCESS )
	{
		if( RegQueryValueEx( hk, 
			(LPCTSTR)strRegEntry,
			NULL,
			NULL,
			(LPBYTE)&(CHVPowerDataManager::Instance().m_dwHVtype),
			&dwSize) == ERROR_SUCCESS )
			RegCloseKey(hk);
	}

	CDBDataManager::Instance().SetHVType(CHVPowerDataManager::Instance().m_dwHVtype);
	// 16.07 V1阀控制方式
	CVACDataManager::Instance().m_dwV1CtrlMode= 0;
	strRegEntry = "V1CtrlMode";
	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
		(LPCTSTR)strSubKey,
		0, 
		KEY_ALL_ACCESS,
		&hk ) == ERROR_SUCCESS )
	{
		if( RegQueryValueEx( hk, 
			(LPCTSTR)strRegEntry,
			NULL,
			NULL,
			(LPBYTE)&(CVACDataManager::Instance().m_dwV1CtrlMode),
			&dwSize) == ERROR_SUCCESS )
			RegCloseKey(hk);
	}
}

int CMainFrame::_CreatePanelAll2()
{
	if( CProductVersionManager::Instance().m_bTrial_NormalUse )	// 在试用期内，或已获得正式版密钥
	{
		//--采集控制1(TOOLBAR)-----------------
		if (!m_wndScanCtrlBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
			| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndScanCtrlBar.LoadToolBar(IDR_SCANTYPE_BIG))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
		m_wndScanCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndScanCtrlBar);
		CString str = GetResString( IDS_SD_SCANFRM_BAR1 );
		m_wndScanCtrlBar.SetWindowText( str );

		CImageList ImageList;
		m_wndScanCtrlBar.SetSizes(CSize(40,40), CSize(32,32));
		ImageList.Create(32,32,ILC_COLOR32 | ILC_MASK,1,1);
		CBitmap bmp;
		for(int i=0; i<25; i++ )
		{
			bmp.LoadBitmap( IDB_BITMAP_Toolbar01 +i );
			ImageList.Add( &bmp, RGB(255,0,255) );
			bmp.DeleteObject();
		}
		m_wndScanCtrlBar.GetToolBarCtrl().SetImageList(&ImageList);
		ImageList.Detach();

		//设置向下的箭头
// 		m_wndScanCtrlBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
// 		DWORD dwStyle = 0;
// 		dwStyle = m_wndScanCtrlBar.GetButtonStyle(m_wndScanCtrlBar.CommandToIndex(ID_SCAN_TYPE_NORMAL));
// 		dwStyle |= TBSTYLE_DROPDOWN;
// 		m_wndScanCtrlBar.SetButtonStyle(m_wndScanCtrlBar.CommandToIndex(ID_SCAN_TYPE_NORMAL), dwStyle);

		//--All--------------------------------
		if (!m_wndImpPanelAll2.Create( CDlgImpPanelAll2::IDD, this ))
		{
			TRACE0("Failed to create CDlgImpPanelAll2 dlg\n");
			return -1;       // fail to create
		}
		m_wndImpPanelAll2.ShowWindow(SW_SHOW);
		LoggingAppLifetime_Start();
		return 0;
	}

	return 0;
}

/*
IDR_BEAM_DETECTION TOOLBAR 16, 15
	BEGIN
	BUTTON      IDC_STATIC_Beam_Target
	BUTTON      IDC_STATIC_Beam_Current
	BUTTON      IDC_BUTTON_Beam_Detection
	BUTTON      IDC_BUTTON_Beam_Setup
	BUTTON      IDC_STATIC_Beam_Port
	BUTTON      IDC_STATIC_Beam_Aper
	BUTTON      IDC_STATIC_Beam_Unit
END
*/
int CMainFrame::_CreatePanelBeam_short()
{
	//束流检测
	if (!m_wndBeamCtrlBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED) ||
		!m_wndBeamCtrlBar.LoadToolBar(IDR_BEAM_DETECTION))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//工具栏停靠
	m_wndBeamCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndBeamCtrlBar);
	m_wndBeamCtrlBar.SetSizes(CSize(40,40), CSize(32,32));
	//工具栏重新布局
	RecalcLayout();
	CRect rect;
	m_wndScanCtrlBar.GetWindowRect(rect);
	rect.OffsetRect(1,0);
	DockControlBar(&m_wndScanCtrlBar,AFX_IDW_DOCKBAR_TOP,rect);
	rect.OffsetRect(90,0);
	DockControlBar(&m_wndBeamCtrlBar,AFX_IDW_DOCKBAR_TOP,rect);
	//去掉竖线
	m_wndBeamCtrlBar.ModifyStyle(TBSTYLE_FLAT,TBSTYLE_LIST);
	//工具栏伸缩
	//m_wndBeamCtrlBar.ModifyStyle(0,CBRS_SIZE_DYNAMIC);

	m_BeamFont.CreateFont(-11, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	m_wndBeamCtrlBar.SetFont( &m_BeamFont, TRUE );

	//设置束流组合框-目标束流static
	int index = 0;
	CRect rect1, rcDebug;
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Target )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Target, TBBS_SEPARATOR, 80 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 10;
	rcDebug.top += 1;
	rcDebug.bottom = rect1.top + rect1.Height()/2 -2;
	m_staticBeamTar.Create(_T("目标束流"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Target);
	m_staticBeamTar.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamTar.ShowWindow( SW_SHOW );
	m_staticBeamTar.SetFont( &m_BeamFont, TRUE );
	//设置束流组合框-目标束流combox
	rcDebug = rect1;
	rcDebug.left += 10;
	rcDebug.top += (rect1.Height()/2 -1);
	rcDebug.bottom -= 1;
	if( !m_comboBeamTar.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_COMBO_Beam_Target ) )
		return -1;
	m_comboBeamTar.ShowWindow( SW_SHOW );
	m_comboBeamTar.EnableWindow(FALSE);
	m_comboBeamTar.SetFont( &m_BeamFont, TRUE );
	//实时束流-数值static
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Current )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Current, TBBS_SEPARATOR, 90 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 10;
	rcDebug.top += 1;
	rcDebug.bottom = rect1.top + rect1.Height()/2 -2;
	m_staticBeamCur.Create(_T("着陆束流"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Current);
	m_staticBeamCur.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamCur.ShowWindow(SW_SHOW);
	m_staticBeamCur.SetFont( &m_BeamFont, TRUE );
	//实时束流-数值
	rcDebug = rect1;
	rcDebug.left += 10;
	rcDebug.right -= 10;
	rcDebug.top += (rect1.Height()/2 -1);
	rcDebug.bottom -= 1;
	if( !m_editCurrentBeam.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_EDIT_Beam_Current ) )
		return -1;
	m_editCurrentBeam.ShowWindow(SW_SHOW);
	//m_editCurrentBeam.SetWindowText("0.0" );
	m_editCurrentBeam.EnableWindow(FALSE);
	m_editCurrentBeam.SetFont( &m_BeamFont, TRUE );
	//实时束流-单位
// 	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Unit )
// 		index ++;
// 	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Unit, TBBS_SEPARATOR, 25 );//150 );
// 	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
// 	rcDebug = rect1;
// 	rcDebug.left += 5;
// 	rcDebug.top += 10;
// 	rcDebug.bottom -= 2;
// 	rcDebug.right = rect1.left + 20;
// 	m_staticBeamUnit.Create(_T("pA"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Unit);
// 	m_staticBeamUnit.ModifyStyle(0,SS_SUNKEN);
// 	m_staticBeamUnit.ShowWindow(SW_SHOW);
	//束流检测button
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_BUTTON_Beam_Detection )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_BUTTON_Beam_Detection, TBBS_SEPARATOR, 120);//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 10;	// 5;
	rcDebug.top += 7;
	rcDebug.bottom -= 8;
	rcDebug.right = rect1.left +120;
	if( !m_btnBeamDetection.Create(_T("束流检测"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, rcDebug, &m_wndBeamCtrlBar, IDC_BUTTON_Beam_Detection ) )
		return -1;
	m_btnBeamDetection.ShowWindow(SW_SHOW);
	m_btnBeamDetection.EnableWindow(FALSE);
	m_btnBeamDetection.SetFont( &m_BeamFont, TRUE );
	//设置
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_BUTTON_Beam_Setup )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_BUTTON_Beam_Setup, TBBS_SEPARATOR, 30);//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 5;
	rcDebug.top += 7;
	rcDebug.bottom -= 10;
	rcDebug.right = rect1.left +30;
	if( !m_btnBeamSetup.Create(_T("<<"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON | ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_BUTTON_Beam_Setup ) )
		return -1;
	m_btnBeamSetup.ShowWindow(SW_SHOW);
	m_btnBeamSetup.EnableWindow(FALSE);
	m_btnBeamSetup.SetFont( &m_BeamFont, TRUE );
	//端口-static
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Port )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Port, TBBS_SEPARATOR, 80 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 10;
	rcDebug.top += 1;
	rcDebug.bottom = rect1.top + rect1.Height()/2 -2;
	m_staticBeamPort.Create(_T("端口"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Port);
	m_staticBeamPort.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamPort.ShowWindow( SW_HIDE );
	m_staticBeamPort.SetFont( &m_BeamFont, TRUE );
	//端口-combox
	rcDebug = rect1;
	rcDebug.left += 10;
	rcDebug.top += (rect1.Height()/2 -1);
	rcDebug.bottom -= 1;
	if( !m_comboBeamPort.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_COMBO_Beam_Port ) )
		return -1;
	m_comboBeamPort.ShowWindow( SW_HIDE );
	m_comboBeamPort.SetFont( &m_BeamFont, TRUE );
	//光阑-static
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Aper )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Aper, TBBS_SEPARATOR, 80 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 10;
	rcDebug.top += 1;
	rcDebug.bottom = rect1.top + rect1.Height()/2 -2;
	m_staticBeamAper.Create(_T("光阑"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Aper);
	m_staticBeamAper.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamAper.ShowWindow( SW_HIDE );
	m_staticBeamAper.SetFont( &m_BeamFont, TRUE );
	//光阑-combox
	rcDebug = rect1;
	rcDebug.left += 10;
	rcDebug.top += (rect1.Height()/2 -1);
	rcDebug.bottom -= 1;
	if( !m_comboBeamAper.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_COMBO_Beam_Aper ) )
		return -1;
	m_comboBeamAper.ShowWindow( SW_HIDE );
	m_comboBeamAper.SetFont( &m_BeamFont, TRUE );
	//保存位置-button
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Unit )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Unit, TBBS_SEPARATOR, 30 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 5;
	rcDebug.top += 7;
	rcDebug.bottom -= 10;
	rcDebug.right = rect1.left +70;
	if( !m_btnBeamSaveXYpos.Create(_T("保存位置"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON | ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_STATIC_Beam_Unit ) )
		return -1;
	m_btnBeamSaveXYpos.ShowWindow(SW_HIDE);
	m_btnBeamSaveXYpos.EnableWindow(TRUE);
	m_btnBeamSaveXYpos.SetFont( &m_BeamFont, TRUE );
	return 0;
}


/*
IDR_BEAM_DETECTION TOOLBAR 16, 15
	BEGIN
	BUTTON      IDC_STATIC_Beam_Target
	BUTTON      IDC_COMBO_Beam_Target
	BUTTON      IDC_STATIC_Beam_Current
	BUTTON      IDC_EDIT_Beam_Current
	BUTTON      IDC_STATIC_Beam_Unit
	BUTTON      IDC_BUTTON_Beam_Detection
	BUTTON      IDC_BUTTON_Beam_Setup
	BUTTON      IDC_STATIC_Beam_Port
	BUTTON      IDC_COMBO_Beam_Port
	BUTTON      IDC_STATIC_Beam_Aper
	BUTTON      IDC_COMBO_Beam_Aper
END
*/
int CMainFrame::_CreatePanelBeam()
{
	//束流检测
	if (!m_wndBeamCtrlBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED) ||
		!m_wndBeamCtrlBar.LoadToolBar(IDR_BEAM_DETECTION))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//工具栏停靠
	m_wndBeamCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndBeamCtrlBar);
	m_wndBeamCtrlBar.SetSizes(CSize(40,40), CSize(32,32));
	//工具栏重新布局
	RecalcLayout();
	CRect rect;
	m_wndScanCtrlBar.GetWindowRect(rect);
	rect.OffsetRect(1,0);
	DockControlBar(&m_wndScanCtrlBar,AFX_IDW_DOCKBAR_TOP,rect);
	rect.OffsetRect(90,0);
	DockControlBar(&m_wndBeamCtrlBar,AFX_IDW_DOCKBAR_TOP,rect);
	//去掉竖线
	m_wndBeamCtrlBar.ModifyStyle(TBSTYLE_FLAT,TBSTYLE_LIST);
	//工具栏伸缩
	//m_wndBeamCtrlBar.ModifyStyle(0,CBRS_SIZE_DYNAMIC);
	//设置束流组合框-目标束流static
	int index = 0;
	CRect rect1, rcDebug;
	m_staticBeamTar.Create(_T("目标束流"),WS_CHILD | WS_VISIBLE | SS_CENTER,CRect(0,12,100,40),&m_wndBeamCtrlBar,IDC_STATIC_Beam_Target);
	m_staticBeamTar.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamTar.ShowWindow( SW_SHOW );
	//设置束流组合框-目标束流combox
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_COMBO_Beam_Target )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_COMBO_Beam_Target, TBBS_SEPARATOR, 125);//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 58;
	rcDebug.top += 7;
	rcDebug.bottom += 8;
	rcDebug.right = rect1.left +125;
	if( !m_comboBeamTar.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_COMBO_Beam_Target ) )
		return -1;
	m_comboBeamTar.ShowWindow( SW_SHOW );
	m_comboBeamTar.EnableWindow(FALSE);
	//实时束流-数值static
 	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Current )
 		index ++;
 	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Current, TBBS_SEPARATOR, 105 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left +=1;
	rcDebug.top +=11;
	rcDebug.right = rect1.left + 105;
	m_staticBeamCur.Create(_T("着陆束流"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Current);
	m_staticBeamCur.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamCur.ShowWindow(SW_SHOW);
	//实时束流-数值
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_EDIT_Beam_Current )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_EDIT_Beam_Current, TBBS_SEPARATOR, 70 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 2;
	rcDebug.top += 9;
	rcDebug.bottom -= 12;
	rcDebug.right = rect1.left +70;
	if( !m_editCurrentBeam.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_EDIT_Beam_Current ) )
		return -1;
	m_editCurrentBeam.ShowWindow(SW_SHOW);
	//m_editCurrentBeam.SetWindowText("0.0" );
	m_editCurrentBeam.EnableWindow(FALSE);
	//实时束流-单位
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Unit )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Unit, TBBS_SEPARATOR, 25 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 5;
	rcDebug.top += 10;
	rcDebug.bottom -= 2;
	rcDebug.right = rect1.left + 20;
	m_staticBeamUnit.Create(_T("pA"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Unit);
	m_staticBeamUnit.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamUnit.ShowWindow(SW_SHOW);
	//束流检测button
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_BUTTON_Beam_Detection )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_BUTTON_Beam_Detection, TBBS_SEPARATOR, 120);//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 10;	// 5;
	rcDebug.top += 7;
	rcDebug.bottom -= 8;
	rcDebug.right = rect1.left +120;
	if( !m_btnBeamDetection.Create(_T("束流检测"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|BS_PUSHBUTTON*/, rcDebug, &m_wndBeamCtrlBar, IDC_BUTTON_Beam_Detection ) )
		return -1;
	m_btnBeamDetection.ShowWindow(SW_SHOW);
	m_btnBeamDetection.EnableWindow(FALSE);
	//设置
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_BUTTON_Beam_Setup )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_BUTTON_Beam_Setup, TBBS_SEPARATOR, 30);//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 5;
	rcDebug.top += 7;
	rcDebug.bottom -= 10;
	rcDebug.right = rect1.left +30;
	if( !m_btnBeamSetup.Create(_T("<<"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON | ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_BUTTON_Beam_Setup ) )
		return -1;
	m_btnBeamSetup.ShowWindow(SW_SHOW);
	m_btnBeamSetup.EnableWindow(FALSE);
	//端口-static
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Port )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Port, TBBS_SEPARATOR, 45 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left +=1;
	rcDebug.top += 10;
	rcDebug.right = rect1.left + 45;
	m_staticBeamPort.Create(_T("端口"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Port);
	m_staticBeamPort.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamPort.ShowWindow( SW_HIDE );
	//端口-combox
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_COMBO_Beam_Port )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_COMBO_Beam_Port, TBBS_SEPARATOR, 90 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 3;
	rcDebug.top += 7;
	rcDebug.bottom += 100;
	rcDebug.right = rect1.left +90;
	if( !m_comboBeamPort.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_COMBO_Beam_Port ) )
		return -1;
	m_comboBeamPort.ShowWindow( SW_HIDE );
	//光阑-static
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_STATIC_Beam_Aper )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_STATIC_Beam_Aper, TBBS_SEPARATOR, 65 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left +=1;
	rcDebug.top += 10;
	rcDebug.right = rect1.left + 65;
	m_staticBeamAper.Create(_T("光阑"),WS_CHILD | WS_VISIBLE | SS_CENTER,rcDebug,&m_wndBeamCtrlBar,IDC_STATIC_Beam_Aper);
	m_staticBeamAper.ModifyStyle(0,SS_SUNKEN);
	m_staticBeamAper.ShowWindow( SW_HIDE );
	//光阑-combox
	while( m_wndBeamCtrlBar.GetItemID( index ) != IDC_COMBO_Beam_Aper )
		index ++;
	m_wndBeamCtrlBar.SetButtonInfo( index, IDC_COMBO_Beam_Aper, TBBS_SEPARATOR, 70 );//150 );
	m_wndBeamCtrlBar.GetItemRect( index, &rect1 );
	rcDebug = rect1;
	rcDebug.left += 5;
	rcDebug.top += 7;
	rcDebug.bottom += 8;
	rcDebug.right = rect1.left +70;
	if( !m_comboBeamAper.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|ES_CENTER, rcDebug, &m_wndBeamCtrlBar, IDC_COMBO_Beam_Aper ) )
		return -1;
	m_comboBeamAper.ShowWindow( SW_HIDE );
	return 0;
}

int CMainFrame::_CreateDlgAutoTest()
{
	//--Auto Test--------------------------
	if (!m_dlgAutotest.Create( CDlgAutoTestMain::IDD, this ))
	{
		TRACE0("Failed to create m_dlgAutotest dlg\n");
		return -1;       // fail to create
	}
	m_dlgAutotest.ShowWindow(SW_HIDE);
	return 0;
}

int CMainFrame::_CreateDlgCalibration()
{
	if (!m_dlgCali.Create( CDlgCaliCoadjust::IDD, this ))
	{
		TRACE0("Failed to create m_dlgCali dlg\n");
		return -1;       // fail to create
	}
	m_dlgCali.ShowWindow(SW_HIDE);
	return 0;
}

int CMainFrame::_CreateDlgDebugVIP()
{
	if (!m_dlgDebugVIP.Create( CDlgImpPanelDebug::IDD, this ))
	{
		TRACE0("Failed to create m_dlgDebugVIP dlg\n");
		return -1;       // fail to create
	}
	m_dlgDebugVIP.ShowWindow(SW_HIDE);
	return 0;
}

int CMainFrame::_CreateDlgDebugR()
{
	if (!m_dlgDebugR.Create( CDlgImpPanelDebugR::IDD, this ))
	{
		TRACE0("Failed to create m_dlgDebugR dlg\n");
		return -1;       // fail to create
	}
	m_dlgDebugR.ShowWindow(SW_HIDE);
	return 0;
}

int CMainFrame::_CreateDlgGraph()
{
	if (!m_dlgGraph.Create( CDlgDiagLogGraph::IDD, this ))
	{
		TRACE0("Failed to create m_dlgGraph dlg\n");
		return -1;       // fail to create
	}
	m_dlgGraph.ShowWindow(SW_HIDE);
	return 0;
}

int CMainFrame::_CreateDlgDiffGraph()
{
	if (!m_dlgDiffGraph.Create( CDlgDiagLogDiffGraph::IDD, this ))
	{
		TRACE0("Failed to create m_dlgDiffGraph dlg\n");
		return -1;       // fail to create
	}
	m_dlgDiffGraph.ShowWindow(SW_HIDE);
	return 0;
}

int CMainFrame::_CreateDlgDebug()
{
	if (!m_dlgDebug.Create( CDlgDebugMain::IDD, this ))
	{
		TRACE0("Failed to create m_dlgDebug dlg\n");
		return -1;       // fail to create
	}
	m_dlgDebug.ShowWindow(SW_HIDE);
	return 0;
}

void CMainFrame::_CreateHistogramBar()
{
	m_HistoBar.Create(this, IDD_DLG_Histo, CBRS_TOP, ID_VIEW_PANEL_3);
	m_HistoBar.ShowWindow(SW_HIDE);
	m_HistoBar.EnableDocking(0);

	CRect rc, rcHisto, rcCtrl;
	GetWindowRect(&rc);
	m_HistoBar.GetWindowRect(&rcHisto);
	rc.left = rc.right -150;//rcHisto.Width();
	if( m_wndScanCtrlBar.m_hWnd != NULL )
	{
		m_wndScanCtrlBar.GetClientRect( (LPRECT)rcCtrl );
		rc.top += 2*rcCtrl.Height();
	}
	FloatControlBar(&m_HistoBar, CPoint(rc.left, rc.top), CBRS_ALIGN_TOP);
}

int CMainFrame::_CreateDlgStage()
{
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage == 1 )
	{
		if (!m_dlgStage.Create( CPanelStage::IDD, this ))
		{
			TRACE0("Failed to create m_dlgStage dlg\n");
			return -1;       // fail to create
		}
		if( CStageXYManager::Instance().m_Stage.xy.bReady )
			m_dlgStage.ShowWindow(SW_SHOW);
		else
			m_dlgStage.ShowWindow(SW_HIDE);
	}

	return 0;
}
