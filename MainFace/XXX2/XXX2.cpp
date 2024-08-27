// XXX2.cpp : Defines the class behaviors for the application.
//
// Last Modify : 2011.07.19

/////////////////////////////////////////////////////////////////////////////
// 09.12.10 增加调试状态功能
// 10.02.02 将语言设置提前到最开始，并对相应指针变量做保护
//			使所有字符串调用时所使用的语言库与注册表中的语言参数设置一致
// 10.02.08 对快照后的图片增加临时文件存储，并对文件存储做保护
// 10.07.02 增加优先存储格式
// 10.07.02 根据目前的操作系统的版本信息显示不同对话框风格
// 11.07.15 增加保护，当高压未退至0时不允许关闭软件
// 11.07.19 加入隐含项，全选时可以记录下位机发送的当前未直接记录的消息
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "IpFrame.h"
#include "XXX2Doc.h"
#include "XXX2View.h"
#include "Splash.h"

#include "SEMFileDialog.h"

#include <mmsystem.h>		// system timer support

#include "SemCtrlDlgRenewSelect.h"
#include "SEMCTRL_Label.h"

#include "ScanChildFrm.h"
#include "ScanDoc.h"
#include "ScanView.h"

#include "ImageDoc.h"
#include "ImageView.h"

#include "DlgPrompt.h"
#include "DlgInput.h"

#include "htmlhelp.h"

#include "kykyimage.h"		// dispatch
#include "AppLogManager.h"
#include "GlobalParamSetting.h"
#include "ProductVersionManager.h"
#include "StageXYManager.h"
#include "CFGFileManager.h"
#include "DebugMessageDispatcher.h"
#include "MCICardCommonSetting.h"
#include "GlobalParamSetting.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "BoardComm.h"
#include "AppOption.h"
#include "MCBControler.h"
#include "Delayer.h"
#include "GraphBuffManager.h"
#include "ScaleManager.h"
#include "CCGStatusDetector.h"
#include "CCPS3603tatusDetector.h"
#include "SpellmanStatusDetector.h"
#include "Dump.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "HVPowerDataManager.h"
#include "CommonFunctionManager.h"
#include "ScanParameterManager.h"
#include "ImageDataManager.h"
#include "DBDataManager.h"
#include "SEMPanel/PanelCoefficientMgr.h"
#include "SEMPanel/SEMPanelStatusDetector.h"
#include "SEMLog/AlarmManager.h"
#include "SEMLog/RealTimeDataManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern double	g_dScaleScanTable[];
extern int		g_nScaleScanIndex;
IKYKYImage* gpIImage = NULL;

DocType doctypes[SEM_IMAGE_FORMATS] =
{
	{ -1, TRUE, FALSE, _T("Supported files"), _T("*.sem;*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;*.ppm;*.ras;*.mng;*.jng;*.ska;*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") },
#if CXIMAGE_SUPPORT_BMP
	{ CXIMAGE_FORMAT_BMP, TRUE, TRUE, _T("BMP files"), _T("*.bmp") },
#endif
#if CXIMAGE_SUPPORT_GIF
	{ CXIMAGE_FORMAT_GIF, TRUE, TRUE, _T("GIF files"), _T("*.gif") },
#endif
#if CXIMAGE_SUPPORT_JPG
	{ CXIMAGE_FORMAT_JPG, TRUE, TRUE, _T("JPG files"), _T("*.jpg;*.jpeg") },
#endif
#if CXIMAGE_SUPPORT_PNG
	{ CXIMAGE_FORMAT_PNG, TRUE, TRUE, _T("PNG files"), _T("*.png") },
#endif
#if CXIMAGE_SUPPORT_MNG
	{ CXIMAGE_FORMAT_MNG, TRUE, TRUE, _T("MNG files"), _T("*.mng;*.jng") },
#endif
#if CXIMAGE_SUPPORT_ICO
	{ CXIMAGE_FORMAT_ICO, TRUE, TRUE, _T("ICO CUR files"), _T("*.ico;*.cur") },
#endif
#if CXIMAGE_SUPPORT_TIF
	{ CXIMAGE_FORMAT_TIF, TRUE, TRUE, _T("TIF files"), _T("*.tif;*.tiff") },
#endif
#if CXIMAGE_SUPPORT_TGA
	{ CXIMAGE_FORMAT_TGA, TRUE, TRUE, _T("TGA files"), _T("*.tga") },
#endif
#if CXIMAGE_SUPPORT_PCX
	{ CXIMAGE_FORMAT_PCX, TRUE, TRUE, _T("PCX files"), _T("*.pcx") },
#endif
#if CXIMAGE_SUPPORT_WBMP
	{ CXIMAGE_FORMAT_WBMP, TRUE, FALSE, _T("WBMP files"), _T("*.wbmp") },
#endif
#if CXIMAGE_SUPPORT_WMF
	{ CXIMAGE_FORMAT_WMF, TRUE, FALSE, _T("WMF EMF files"), _T("*.wmf;*.emf") },
#endif
#if CXIMAGE_SUPPORT_JBG
	{ CXIMAGE_FORMAT_JBG, TRUE, TRUE, _T("JBG files"), _T("*.jbg") },
#endif
#if CXIMAGE_SUPPORT_JP2
	{ CXIMAGE_FORMAT_JP2, TRUE, TRUE, _T("JP2 files"), _T("*.j2k;*.jp2") },
#endif
#if CXIMAGE_SUPPORT_JPC
	{ CXIMAGE_FORMAT_JPC, TRUE, TRUE, _T("JPC files"), _T("*.j2c;*.jpc") },
#endif
#if CXIMAGE_SUPPORT_PGX
	{ CXIMAGE_FORMAT_PGX, TRUE, TRUE, _T("PGX files"), _T("*.pgx") },
#endif
#if CXIMAGE_SUPPORT_RAS
	{ CXIMAGE_FORMAT_RAS, TRUE, TRUE, _T("RAS files"), _T("*.ras") },
#endif
#if CXIMAGE_SUPPORT_PNM
	{ CXIMAGE_FORMAT_PNM, TRUE, TRUE, _T("PNM files"), _T("*.pnm;*.pgm;*.ppm") },
#endif
#if CXIMAGE_SUPPORT_SKA
	{ CXIMAGE_FORMAT_SKA, TRUE, TRUE, _T("SKA files"), _T("*.ska") },
#endif
#if CXIMAGE_SUPPORT_RAW
	{ CXIMAGE_FORMAT_RAW, TRUE, FALSE, _T("RAW files"), _T("*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") },
#endif
#if SEMIMAGE_SUPPORT_SEM
	{ SEMIMAGE_FORMAT_SEM, TRUE, TRUE, "SEM Image files (.sem)", "*.sem" }
#endif
};

#define	FLAGBIT( flag, flag_bit )	( ( (flag) & (flag_bit) ) == (flag_bit) )

CCriticalSection CXXX2App::m_criticalMsgDisplay;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
HANDLE	g_hThreadLogAll;
DWORD	g_dwThreadLogAll;
HANDLE	g_hEventLogAll			= CreateEvent(NULL,FALSE, FALSE, _T("WORKlogall"));
HANDLE	g_hEventTeminateLogAll	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATElogall"));
HANDLE	g_hFileHVVAC	= NULL;
int		g_nLoopsHVVAC	= 0;
BOOL	g_bLogHVVAC		= FALSE;


/////////////////////////////////////////////////////////////////////////////
// CXXX2App

BEGIN_MESSAGE_MAP(CXXX2App, CWinApp)
	//{{AFX_MSG_MAP(CXXX2App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_LOG_SELECT, OnLogSelect)
	ON_COMMAND(ID_LOG_MSG, OnLogMsg)
	ON_UPDATE_COMMAND_UI(ID_LOG_MSG, OnUpdateLogMsg)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_CTRL_SACP, OnCtrlSACP)
	ON_UPDATE_COMMAND_UI(ID_CTRL_SACP, OnUpdateCtrlSACP)
	ON_COMMAND(ID_OPTIONS_SETUP_LANGUAGE_CHS, OnOptionsSetupLanguageChs)
	ON_COMMAND(ID_OPTIONS_SETUP_LANGUAGE_EN, OnOptionsSetupLanguageEn)
	ON_COMMAND(ID_CTRL_Overhaul, OnCTRLOverhaul)
	ON_UPDATE_COMMAND_UI(ID_CTRL_Overhaul, OnUpdateCTRLOverhaul)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXXX2App construction

CXXX2App::CXXX2App()
{
	// TODO: add construction code here,
	///////////////////////////////////////////////////////////
	m_blSaveGraphLay		= FALSE;	// 保存SEM格式图像时，不保存图形层。

	// Place all significant initialization in InitInstance
	m_boolHV						= FALSE;

	CAppLogManager::Instance().SetupLogs( FALSE );		// 11.07.19

	// 09.01.15 S.A.C.P.功能默认状态为不启用
	m_blSACPState					= FALSE;
	
/*	m_pSemControlDocTemplate	= NULL;
	m_pSemLogDocTemplate		= NULL;
*/	m_pSemImageDocTemplate		= NULL;
	m_pSemScanDocTemplate		= NULL;

	m_blExit					= FALSE;
	m_nImageDocCount			= 1;	//number of image documents

	m_blReady_Vacuum	= FALSE;
	m_blReady_HV		= FALSE;
	m_blReady_Filament	= FALSE;

	m_blUnlock_HV		= FALSE;
	m_blUnlock_Filament = FALSE;

	// Leo 2003.12.5 添加维修状态，此时高压控制不连锁。
	m_blOverhaulState	= FALSE;
	m_blUnlockAll		= FALSE;

	m_blInquireZ80A		= FALSE;
	m_blInquireZ80B		= TRUE;
	m_blPauseAll		= FALSE;
//	m_blPauseAll		= TRUE;

	m_dfMouseSpeed		= 1.0;	//2004-5-14_高倍像操作组合控制

	m_boolShowUScaleXY = FALSE; //2005-1-17_放大倍数XY联合校正

	m_nINT_Ticks			= GetTickCount();
	m_nINT_Counts			= 0;

	m_hTimeDelay = CreateEvent( NULL, FALSE, TRUE, "TimeDelay" );

	m_bDiagLogging		= FALSE;
	m_nSaveSrc					= 0;
}

// The one and only CXXX2App object

CXXX2App theApp;

// CXXX2App initialization
BOOL CXXX2App::InitInstance()
{
	///////////////////////////////////////////////////////
	// 内存泄漏检测
#ifdef _DEBUG
	m_msOld.Checkpoint();
#endif // _DEBUG
	_CrtDumpMemoryLeaks();
	///////////////////////////////////////////////////////

	// 10.02.02 将语言设置提前到最开始，
	// 使所有字符串调用时所使用的语言库与注册表中的语言参数设置一致
	// 比如：IDS_ALREADY_RUNNING的调用
	///////////////////////////////////////////////////////
	// Setup Language
	m_LanguageManagerCtrl.Init();
	CSemCtrl::Instance().InitLanguageManager( &m_LanguageManagerCtrl );
	CSemCoreCtrlPanelMessageTransmiter::Instance().Attach(&m_LanguageManagerCtrl);

	NSDumpFile::RunCrashHandler();

	_ReadFromRegistry();

	m_LanguageManagerCtrl.SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
	///////////////////////////////////////////////////////
	CProductVersionManager::Instance().Initialize(m_LanguageManagerCtrl);
	CProductVersionManager::Instance().Trial_GetFirstTime();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// Only allow one instance of our application to execute
	if(AlreadyRunning())
	{
#ifdef _DEBUG
//		AfxMessageBox(IDS_ALREADY_RUNNING,MB_OK|MB_ICONWARNING);
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = GetResString( IDS_ALREADY_RUNNING );
		dlg.DoModal();
#endif

		m_LanguageManagerCtrl.Close();
		return(FALSE);
	}

	// Only run with the new shell --- we need a system tray
	if(!HasNewShell())
	{
		AfxMessageBox(IDS_WRONG_SHELL,MB_OK|MB_ICONSTOP);

		m_LanguageManagerCtrl.Close();
		return(FALSE);
	}

	// CG: The following block was added by the Splash Screen component.
\
	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(g_csRegistryKey);

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	_InitDocTemplateList();

	bool bInitLogDB = CDBDataManager::Instance().InitializeLogDatabase();

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_nGdiplusToken, &gdiplusStartupInput, NULL);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

#ifdef VATI_EXTENSIONS
	// init m_text by a temporary CxImage 
	CxImage *ima = new CxImage;
	ima->InitTextInfo( &m_text );
	delete ima;

	//recall last used font data for Text tool
    _stprintf( m_text.lfont.lfFaceName, GetProfileString ( _T("TextTool"), _T("lfFaceName"), _T("Times New Roman")) );
	m_text.lfont.lfCharSet   = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfCharSet"), EASTEUROPE_CHARSET ) ;
    m_text.lfont.lfWeight    = GetProfileInt ( _T("TextTool"), _T("lfWeight"), 0 );
    m_text.lfont.lfItalic    = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfItalic"), 0 ); 
    m_text.lfont.lfUnderline = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfUnderline"), 0 ); 
    m_text.fcolor = GetProfileInt ( _T("TextTool"), _T("fcolor"), RGB( 255,255,160 ));
    m_text.bcolor = GetProfileInt ( _T("TextTool"), _T("bcolor"), RGB(   0, 80,160 ));
    m_text.opaque = (BYTE)GetProfileInt ( _T("TextTool"), _T("opaque"), 1);
    m_text.b_opacity = (float)(GetProfileInt( _T("TextTool"), _T("opacity"), 0 ))/(float)100.;  
    m_text.b_round   = (BYTE)GetProfileInt ( _T("TextTool"), _T("roundradius"), 25 );
    m_text.smooth    = (BYTE)GetProfileInt ( _T("TextTool"), _T("antialias"), 1 );

	m_optJpegQuality = GetProfileInt(_T("Options"),_T("JpegQualityI"),90) + 0.001f * GetProfileInt(_T("Options"),_T("JpegQualityF"),0);
	m_optJpegOptions = GetProfileInt(_T("Options"),_T("JpegOptions"),0);

	m_optRawOptions = GetProfileInt(_T("Options"),_T("RawOptions"),0);

//	nDocType = GetProfileInt(_T("General"),_T("DocType"),0);

//	BCMenu::SetMenuDrawMode(GetProfileInt(_T("General"),_T("MenuStyle"),1));

	m_FloodColor.rgbBlue = 255;
	m_FloodColor.rgbGreen = 255;
	m_FloodColor.rgbRed = 255;
	m_FloodColor.rgbReserved = 0;
	m_FloodTolerance = 0;
	m_FloodOpacity = 255;
	m_FloodSelect = 0;

	for(int i=0;i<25;i++) m_Filters.Kernel5x5[i]=1;
	for(int i=0;i<9;i++) m_Filters.Kernel3x3[i]=1;
	m_Filters.kSize = 3;
	m_Filters.kBias = 0;
	m_Filters.kDivisor = 9;

	m_Filters.RotateAngle = 12.345f;
	m_Filters.RotateMethod = 1;
	m_Filters.RotateOverflow = 0;
	m_Filters.RotateKeepsize = 1;

	m_Filters.ResampleSizemode = 1;
	m_Filters.ResampleFactor = 2;
	m_Filters.ResampleW = 100;
	m_Filters.ResampleH = 100;
	m_Filters.ResampleKeepRatio = 1;
	m_Filters.ResampleMethod = 2;

	m_Filters.SkewPivotX = 0;
	m_Filters.SkewPivotY = 0;
	m_Filters.SkewX =  5.5f;
	m_Filters.SkewY = -5.0f;
	m_Filters.SkewInterp = 1;

	m_Filters.DitherMethod = 0;

	m_Filters.ThreshLevel = 128;
	m_Filters.ThreshPreserveColors = 0;

	m_Filters.ColorMode = 0;
	m_Filters.ColorHSL.rgbBlue = 50;
	m_Filters.ColorHSL.rgbGreen = 12;
	m_Filters.ColorHSL.rgbRed = 50;
	m_Filters.ColorHSL.rgbReserved = 0;
	m_Filters.ColorBlue = -50;
	m_Filters.ColorGreen = 0;
	m_Filters.ColorRed = 50;
	m_Filters.ColorSolarLevel = 128;
	m_Filters.ColorSolarLink = 1;

	m_Filters.GammaLevel = 1.23f;
	m_Filters.GammaR = 0.9f;
	m_Filters.GammaG = 1.1f;
	m_Filters.GammaB = 1.2f;
	m_Filters.GammaLink = 0;

	m_Filters.IncBppBPP = 24;
	m_Filters.DecBppBPP = 4;
	m_Filters.DecBppPalMethod = 1;
	m_Filters.DecBppErrDiff = 0;
	m_Filters.DecBppLimitColors = 0;
	m_Filters.DecBppMaxColors = 256;

	m_Filters.CanvasMode = 0;
	m_Filters.CanvasW = 800;
	m_Filters.CanvasH = 600;
	m_Filters.CanvasLeft = 20;
	m_Filters.CanvasRight = 20;
	m_Filters.CanvasTop = 10;
	m_Filters.CanvasBottom = 10;
	m_Filters.CanvasCenterH = 1;
	m_Filters.CanvasCenterV = 1;
	m_Filters.CanvasKeepRatio = 0;
	m_Filters.CanvasUseImageBkg = 0;
	m_Filters.CanvasBkg = RGB(255,255,255);

	m_Filters.ShadowX = -5;
	m_Filters.ShadowY = 5;
	m_Filters.ShadowR = 7;
	m_Filters.ShadowColor = RGB(0,0,0);
	m_Filters.ShadowBkg = RGB(255,255,255);
	m_Filters.ShadowIntensity = 128;
	m_Filters.ShadowRelative = 0;
#endif;

	// The main window has been initialized, so show and update it.
//	m_pMainWnd->SetWindowText(m_strLogoDisp);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	__int64 timestamp = GetCurrentTimeStampMS();
	CAlarmManager::Instance().SetAppStartTime(timestamp);
	CRealTimeDataManager::Instance().SetAppStartTime(timestamp);

	CDebugMessageDispatcher::Instance().Attach(pMainFrame->GetSafeHwnd());
	CStageXYManager::Instance().Attach(pMainFrame->GetSafeHwnd(),&m_LanguageManagerCtrl);

	CFGfile_InitParams();

	// 11.07.19
	int nLogAll = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, 
								g_csRegEntry_nLogAll, 0 );
	if( nLogAll > 0 )
	{
		CAppLogManager::Instance().SetupLogs( TRUE );
		if( pMainFrame->m_wndImpPanelLog.m_hWnd != NULL )
			pMainFrame->m_wndImpPanelLog.InitAutoLog();
	}
	// 11.07.19

	if( pMainFrame->m_wndImpPanelThumb.m_hWnd != NULL )
		pMainFrame->m_wndImpPanelThumb.ShowWindow( TRUE );
	if( pMainFrame->m_wndImpPanelLog.m_hWnd != NULL )
		pMainFrame->m_wndImpPanelLog.ShowWindow( TRUE );
	pMainFrame->AdjustPanelsPos();

	CPanelCoefficientMgr::Instance().LoadSettings();
	int nRet = CSEMPanelStatusDetector::Instance().Initialize(pMainFrame->m_wndImpPanelAll2.m_hWnd);
	if (0 == nRet)
	{
		CSEMPanelStatusDetector::Instance().Start();
	}
	else
	{
		OutputDebugString("CSEMPanelStatusDetector Initialize LibUSB Failed. \r\n");
	}

	if( CStageXYManager::Instance().m_Stage.nHasAutoStage == 1 )
	{
		// 15.10 XY自动台
		CStageXYManager::Instance().Stage_XY_Init();
	}

	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		double dblPixelsPerMM_HORZ = CMCICardCommonSetting::Instance().m_dblPixelsPerMM_HORZ;
		double dblPixelsPerMM_VERT = CMCICardCommonSetting::Instance().m_dblPixelsPerMM_VERT;
	
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 )
		{
			StageTMCM_ShowWnd(m_pMainWnd->m_hWnd);
			StageTMCM_SetPixelsPerMM(dblPixelsPerMM_HORZ, dblPixelsPerMM_VERT);
		}
	    else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 )
		{
			StageMC6600_ShowWnd(m_pMainWnd->m_hWnd);
			StageMC6600_SetPixelsPerMM(dblPixelsPerMM_HORZ, dblPixelsPerMM_VERT);
		}
		else
		{
			StageMC600_SetParam( 0, CStageXYManager::Instance().m_Stage.nHasAutoStage );
			StageMC600_ShowWnd(m_pMainWnd->m_hWnd);
			StageMC600_SetPixelsPerMM(dblPixelsPerMM_HORZ, dblPixelsPerMM_VERT);
		}
	
	}

	if( !CSemCtrl::Instance().USB_isReady() )
	{
		CString str;
		CDlgPrompt dlg(pMainFrame);
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = "\r\n";
		str.Format("%s! ", GetResString( IDS_SD_PROMPT_VAC_00 ) );
		dlg.m_strMsg += str;
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		str.Format( "    %s %s", GetResString( IDS_SD_PROMPT_VAC_36), GetResString( IDS_SD_PROMPT_VAC_37 ) );
		dlg.m_strMsg += str;
		dlg.DoModal();
		str.Format("%s! %s %s", GetResString( IDS_SD_PROMPT_VAC_00 ), GetResString( IDS_SD_PROMPT_VAC_36), GetResString( IDS_SD_PROMPT_VAC_37 ));
		DisplayMsg(str);
	}

	if( !CBoardComm::Instance().m_bBoardsReady )
	{
		CString str;
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = "\r\n";
		str.Format("Sub Boards Error(s)!" );
		dlg.m_strMsg += str;
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		str.Format( "%s %s", GetResString( IDS_SD_PROMPT_VAC_36), GetResString( IDS_SD_PROMPT_VAC_37 ) );
		dlg.m_strMsg += str;
		dlg.DoModal();
	}

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(pMainFrame);

	Localize();

	return TRUE;
}




int CXXX2App::ExitInstance() 
{
	// 在最低放大倍数档位的一律先设置为500倍后再退出，为避免烧线圈多一重保护
	VARIANT		m_Magnifier;
	m_Magnifier	= CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	double dRateCtrl2	= 0.0;
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 11, dRateCtrl2 );
	if( m_Magnifier.dblVal < dRateCtrl2 )
	{
		m_Magnifier.dblVal = 500;
		VARIANT ClassID, SerialNumber, Flag;
		SerialNumber.lVal = -1;
		ClassID.lVal = SemControlPanelClass_Magnifier;
		Flag.lVal = 1;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, m_Magnifier, m_Magnifier, Flag );
		Sleep( 100 );
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, m_Magnifier, m_Magnifier, Flag );
	}

	CSemCtrlInitializer::Instance().AutoSaveSemCtrl();	// 2004-6-2_自动保存并更新控制量
	CAppLogManager::Instance().LogHVVACClose();

//	CSEMPanelStatusDetector::Instance().Release();

	m_LanguageManagerCtrl.Close();

   CStageXYManager::Instance().Release();
	if( gpIImage )
	{
		gpIImage->DetachDispatch();
//		gpIImage->ReleaseDispatch();
		delete gpIImage;
	}

	CDBDataManager::Instance().Close();

	CBoardComm::Instance().Release();
	CGraphBuffMgr::Instance().Release();
	CSMvac::Instance().Close();
	CSemCtrl::Instance().Release();
	CSemVirtualMCICard::Instance().Close();
	CControlLayer::Instance().Release();
	GdiplusShutdown(m_nGdiplusToken);

	///////////////////////////////////////////////////////
	// 内存泄漏检测
#ifdef _DEBUG
	m_msNew.Checkpoint();
	if( m_msDiff.Difference(m_msOld, m_msNew))
	{
		afxDump << "\nMemory Leaked :\n";
		m_msDiff.DumpStatistics();
		afxDump << "Dump Complete !\n\n";
	}
#endif // _DEBUG
	///////////////////////////////////////////////////////

	return CWinApp::ExitInstance();
}

void CXXX2App::Delay( int msecond )
{
	WaitForSingleObject( m_hTimeDelay, msecond );
}

void CXXX2App::LinkImageProcess( CThumbImageItem* pitem )
{
	if( !gpIImage )
	{
		gpIImage = new IKYKYImage;
		gpIImage->CreateDispatch("KYKYImage.Document");
	}
	if( !gpIImage )
		return;

	if( !(pitem->m_strPathName.IsEmpty()) )
		gpIImage->OpenImageFromFile( pitem->m_strPathName );
	else
		gpIImage->OpenImageFromFile( pitem->m_strTempName );
//		gpIImage->OpenImageFromUntitledCxImage( ((OLE_HANDLE*)pitem) );
}

//======2005-3-7_PC1复位处理程序==============
void CXXX2App::PC1ResetingProcess()
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	
	//======2004-8-23_控制量自动清零==================
	// HV
	{
		ClassID.lVal = SemControlPanelClass_CPS_HV;
		Flag.lVal = 1;
		Xpos.lVal = 0;
	
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);
		CScanView* pView = (CScanView*)theApp.GetScanView();
		if( pView != NULL && CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
			CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
		CHVPowerDataManager::Instance().m_dHV = CGlobalParamSetting::Instance().g_dHV2 = CGlobalParamSetting::Instance().g_dHV3 = 0.0;
	}
	// Brightness 05.06.21
	{
		ClassID.lVal = SemControlPanelClass_Brightness;
		Flag.lVal = 1;
		Xpos.lVal = 0;
	
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);
	}
	// Contrast 05.06.21
	{
		ClassID.lVal = SemControlPanelClass_Contrast;
		Flag.lVal = 1;
		Xpos.lVal = 0;
	
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);
	}
}

void CXXX2App::SetupScanShift( double dblMag )
{
	{
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag, Enabled;
		SerialNumber.lVal = -1;
	
		// ScanShift
		{
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Enabled.lVal	= 1;
			Flag.lVal		= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

			CDelayer::Instance().Delay(5);//Sleep( 5 );

			Flag.lVal		= 3;
			Xpos.lVal		= 0;
			Ypos.lVal		= 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);
		}
	}
}


void CXXX2App::AutoInitSemCtrl( BOOL bPC9Reset )// 05.06.22 加入参数以判断是否是PC9复位
{
	SetupSerialNumber();
	CScanView* pView = (CScanView*)GetScanView();
	if( pView != NULL )
	{
		if( CSemCtrl::Instance().USB_isReady() )
			pView->USB_ChangeScan( USB_ST_RasterScan );
	}

	CSemCoreAssemble::Instance().m_Brightness.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_Contrast.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_CondenserLens.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_BeemAlignmentX.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_BeemAlignmentY.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_ScanShiftX.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_ScanShiftY.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_StigmatorX.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_StigmatorY.SetParams( 100, m_dwOpr );
	CSemCoreAssemble::Instance().m_SP_PMT.SetParams( 100, m_dwOpr );

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		CSemCtrlInitializer::Instance().AutoInitSemCtrl_CPS();
	CSemCtrlInitializer::Instance().AutoInitSemCtrl_MCI();
	CSemCtrlInitializer::Instance().AutoInitSemCtrl_SPR();
	Sleep( 100 );
	CSemCtrlInitializer::Instance().AutoInitSemCtrl_LNS();
	Sleep( 100 );
	CSemCtrlInitializer::Instance().AutoInitSemCtrl_SCN( bPC9Reset );

	CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
	if( pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
	{
		char sz[512] = {0};
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"BOOL bRet = pMainFrame->m_wndImpPanelAll2.VAC_Init(); \r\n" );
		OutputDebugString(sz);
		BOOL bRet = pMainFrame->m_wndImpPanelAll2.VAC_Init();
		if( CHVPowerDataManager::Instance().m_dwHVtype > 0 )	// 3603先连接没有意义，没有回读数据
			pMainFrame->m_wndImpPanelAll2.HVPower_Init( TRUE );
		if( bRet )
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype <= 3 )	// 2021.08 先不做使用WSM30KV高压电源的大日志记录
				CAppLogManager::Instance().LogHVVACInit(pMainFrame->m_hWnd, CHVPowerDataManager::Instance().m_dwHVtype);
		}
		if( m_bHasCCD && CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showCCD] > 0 )
		{
			pMainFrame->m_wndImpPanelAll2.m_btnCCD.SetCheck(1);
			pMainFrame->m_wndImpPanelAll2.m_btnCCD.SetColor( RGB(0,128,255) );
			pMainFrame->m_wndImpPanelAll2.m_btnCCD.Invalidate();
			pMainFrame->m_wndImpPanelAll2.SPR_HVOn( TRUE );
			pMainFrame->m_wndImpPanelAll2.ShowCameraWindow();
		}
		else
			pMainFrame->m_wndImpPanelAll2.SPR_HVOn( FALSE );
		if( m_bHasNAV )
		{
			NAV_ShowWnd( pMainFrame->m_wndImpPanelAll2.m_hWnd, FALSE );
			NAV_SetLanguageID(CGlobalParamSetting::Instance().m_wLanguageID);
		}
		// 初始化关闭电源（导航灯板关）
		pMainFrame->m_wndImpPanelAll2.SPR_Fan(FALSE);
	}
	if( pMainFrame->m_MainPanelDebug.m_hWnd != NULL)
	{
		pMainFrame->m_MainPanelDebug.m_PageDebug2.MCI_Init();
		pMainFrame->m_MainPanelDebug.m_PageDebug2.VAC_Init();
		pMainFrame->m_MainPanelDebug.m_PageDebug2.MCI_SyncParams();
	}
	if( pMainFrame->m_dlgDebugR.m_hWnd != NULL)
	{
		pMainFrame->m_dlgDebugR.MCI_SyncParams();
	}

	pMainFrame->SetTimers( nSemAutosaveTimerID, TRUE );
	pMainFrame->SetTimers( nSemMessageTimerID, TRUE );

	//束流
	if( CControlLayer::Instance().m_nPAType > 0 )
		pMainFrame->D22_Init();

	///////////////////////////////////
	// MCB
	// 2. 直接在主线程ScanView中处理MCB消息
	// App::AutoInitSemCtrl之后再启动定时器
	if( pView != NULL )
	{
		pView->UpdatePhotoSn( m_nPhotoSerialNumber );
		//pView->nMCB_Timer = pView->SetTimer( nMCB_TimerID, nMCB_TimerElapse, NULL );
	}
	/* 2. End*/


	// 15.11 自动台
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )
	{
		// 15.10 XY自动台
		CStageXYManager::Instance().Stage_XY_FindIndex();
	}
}


void CXXX2App::SetupDFocus( double dMag )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	ClassID.lVal = SemControlPanelClass_DFocus;
	Flag.lVal = 1;

	CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
	if( dMag < 10000 )
	{
		// 10000倍以下时，默认关闭DF；如用户有需要，需自行打开DF功能
		// 暂定须由用户选定DF档位，之后在后台做
		CSemCoreAssemble::Instance().m_DFocus.SetParams( 14, 0 );
		CSemCoreAssemble::Instance().m_DFocus.SetParams( 15, 0 );
		CSemCoreAssemble::Instance().m_DFocus.GetRange( Xpos, Ypos );
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Xpos, Flag);
		CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 11, 0 );

		Flag.lVal			= 3;
		Xpos.lVal			= 0;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Flag );

		ClassID.lVal		= SemControlPanelClass_DFocusSwitch;
		Flag.lVal			= 3;
		Xpos.lVal			= 1;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Flag );

		if( (pMainFrame->m_MainPanelDebug.m_hWnd != NULL)
			&& (pMainFrame->m_MainPanelDebug.m_PageDebug2.m_hWnd != NULL) )
			pMainFrame->m_MainPanelDebug.m_PageDebug2.m_ctrlDFocus.ShowWindow( SW_SHOW );

		CControlLayer::Instance().m_bDFforFF = FALSE;
	}
	else
	{
		// 10000倍以上时，DF复用为FF，且初值为0x8000
		CSemCoreAssemble::Instance().m_DFocus.SetParams( 14, 3 );
		CSemCoreAssemble::Instance().m_DFocus.SetParams( 15, 1 );
		Xpos.lVal = 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Xpos, Flag);
		Sleep(100);
		// Dynamic Focus level boundary
		CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 11, 1 );
		Sleep(100);
		CSemCoreAssemble::Instance().m_DFocus.SetParams( SET_PARAMS_DFocus_End, 0x8100 );
		CSemCoreAssemble::Instance().m_DFocus.SetParams( SET_PARAMS_DFocus_Start, 0x8000 );

		Flag.lVal			= 3;
		Xpos.lVal			= 1;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Flag );

		ClassID.lVal		= SemControlPanelClass_DFocusSwitch;
		Flag.lVal			= 3;
		Xpos.lVal			= 0;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Flag );

		if( pMainFrame->m_MainPanelDebug.m_hWnd != NULL )
			pMainFrame->m_MainPanelDebug.m_PageDebug2.m_ctrlDFocus.ShowWindow( SW_HIDE );
		CControlLayer::Instance().m_bDFforFF	= TRUE;
	}
}

void CXXX2App::VAC_SaveStates()
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	char		stabuff[20];
	char		staitembuff[20];
	int			nTotal	= 7;
	int			nCheck[7];

	nCheck[0]	= CVACDataManager::Instance().m_VacParamMain.bPower;
	nCheck[1]	= CVACDataManager::Instance().m_VacParamMain.bChamb;
	nCheck[2]	= CVACDataManager::Instance().m_VacParamMain.bGun;
	nCheck[3]	= CVACDataManager::Instance().m_VacParamMain.bVf;
	nCheck[4]	= CVACDataManager::Instance().m_VacParamMain.bVc;
	nCheck[5]	= CVACDataManager::Instance().m_VacParamMain.bTMP;
	nCheck[6]	= CVACDataManager::Instance().m_VacParamMain.bVa;

	sprintf(stabuff, "VACParam");
	sprintf(staitembuff, "VStates");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, INTSTYLE, nTotal, nCheck );
}


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	void	Localize(void);

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnRegister();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_6K_BUTTON_Register, OnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutDlg::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDC_6K_STATIC_Copyright)->SetWindowText(GetResString(IDS_6K_STATIC_Copyright)); 
	GetDlgItem(IDC_6K_STATIC_Time)->SetWindowText(GetResString(IDS_6K_STATIC_Time)); 
	GetDlgItem(IDC_6K_BUTTON_Register)->SetWindowText(GetResString(IDS_6K_BUTTON_Register)); 

	CString str, str2;
	if( !(CProductVersionManager::Instance().m_bTrial_NormalUse) || CProductVersionManager::Instance().m_nTrialTime > 0 )
		str.Format( GetResString(IDF_SD_PROMPT_Trial_Prompt1), CProductVersionManager::Instance().m_nTrialTime /(24*60*60) );
	else 
		str = GetResString(IDS_SD_PROMPT_Trial_Prompt2);
	GetDlgItem(IDC_6K_STATIC_TrialTime)->SetWindowText(str); 

	// 版本
	str = GetResString(IDS_6K_STATIC_Version);
	int nPos = str.Find(' ');
	str2 = str.Right( str.GetLength() -nPos );
	str.Format( "%s%s", theApp.m_strLogoDisp, str2 );
	GetDlgItem(IDC_6K_STATIC_Version)->SetWindowText(str); 

	// 标题
	str = GetResString(IDS_6K_ABOUTBOX);
	nPos = str.Find(' ');
	str2 = str.Left( nPos +1);
	str.Format( "%s%s", str2, theApp.m_strLogoDisp );
	SetWindowText(str); 
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnRegister() 
{
	CDlgInput dlg;
	dlg.m_nTitleIndex = IDS_SD_PROMPT_Input_Trial;
	if( dlg.DoModal() == IDOK )
	{
		BOOL bLegal = TRUE;
		char ch, cKey[255];
		sprintf( cKey, "%s", dlg.m_strInput );
		// 长度
		int i, nLength = strlen(cKey);
		if( nLength > 23 )
			bLegal = FALSE;
		if( bLegal )
		{
			// 格式1：必须是大写字母和数字，每5个字符中间用‘-’连接
			for( i=0; i<nLength; i++ )
			{
				ch = cKey[i];
				if( ch >= '0' && ch <= '9' )
					continue;
				if( ch >= 'A' && ch <= 'Z' )
					continue;
				if( ch == '-' )
					continue;
				bLegal = FALSE;
				break;
			}
		}
		if( bLegal )
		{
			// 格式2：指定位置的特殊字符
			if( cKey[5] != '-' || cKey[11] != '-' || cKey[17] != '-'
				|| cKey[2] != '5' || cKey[9] != '2' || cKey[13] != '6' || cKey[22] != '8' )
				bLegal = FALSE;
		}
		if( bLegal )
		{
			// 更新版本信息到文件和注册表
			CProductVersionManager::Instance().Trial_SaveTime( TRUE );
		}
		if( !bLegal )
		{
			CString strPrompt = GetResString( IDS_SD_PROMPT_Trial_Prompt5 );
			MessageBox( strPrompt, "Warning", MB_OK|MB_ICONWARNING );
		}
	}
}

// App command to run the dialog
void CXXX2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

#include <Windows.h>



void CXXX2App::WriteMenuFile(CMenu* pmenu, LPCTSTR lpSection, LPCTSTR lpFileName)
{
	TCHAR szBufSection[20], szBufKey[20];
	sprintf(szBufSection, "%s", lpSection);
	int nMenuCount = pmenu->GetMenuItemCount();
	CString str;
	str.Format( "%d", nMenuCount );
	WritePrivateProfileString(szBufSection, "Count", str, lpFileName);
	UINT nID = 0;
	for( int i=0; i<nMenuCount; i++ )
	{
		sprintf(szBufKey, "menu%d", i);
		nID = pmenu->GetMenuItemID( i );
		if( nID == -1 )		// has sub menu
		{
			str.Format( "%s_Sub_%d", lpSection, i );
			WritePrivateProfileString(szBufSection, szBufKey, str, lpFileName);
			CMenu* menuSub = pmenu->GetSubMenu( i );
			WriteMenuFile( menuSub, (LPCTSTR)str, lpFileName );
		}
		else
		{
			str.Format( "%d", nID );
			WritePrivateProfileString(szBufSection, szBufKey, str, lpFileName);
		}
	}
}


// Determine if we are running the new shell (i.e. Windows 95 or Windows NT 4.0
// or later)
BOOL CXXX2App::HasNewShell()
{
   // Get the Windows Version
   DWORD dwVersion = GetVersion();

   // Windows NT?
   if(dwVersion < 0x80000000) 
   {
       // Yes, Windows NT 4.0 or greater?
       if (LOBYTE(LOWORD(dwVersion)) >= 4) 
         return TRUE; 
   } // Windows 95?
   else if(LOBYTE(LOWORD(dwVersion)) >= 4) 
         return TRUE;

   return FALSE;  // Win16, 32s or NT3.x.
}

// Determine if SEM program is already running by using a Mutex object -- returns
// TRUE if already running, FALSE otherwise.  Note that a mutex is created
// with the name of the application
BOOL CXXX2App::AlreadyRunning()
{
   BOOL bFound = FALSE;

   // Try to create a mutex with the app's name
   HANDLE hMutexOneInstance = ::CreateMutex(NULL,TRUE,_T(AfxGetAppName()));

   // Already there...means that we are already running an instance
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
      bFound = TRUE;

   // Release the mutex
   if(hMutexOneInstance)
      ::ReleaseMutex(hMutexOneInstance);

   return(bFound);
}

void CXXX2App::DisplayMsg(LPCTSTR lpszText)
{
	m_criticalMsgDisplay.Lock();

	DWORD	thisTickCount = timeGetTime();
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format(IDF_MessageTimeFormat);
	
	CString csMessage;
	csMessage.Format("[%010Ld]  ", thisTickCount);
	csMessage = csCurrentTime + csMessage + lpszText;

	CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
	pMainFrame->Message((LPCTSTR)csMessage);

	__int64 _i64Time = GetCurrentTimeStampMS();
	char szSQL[512] = {0};
	sprintf_s(szSQL,256,"insert into OperationLog(Time,Content) values(%I64d,'%s')",_i64Time,lpszText);
	string strSQL = szSQL;
	CDBDataManager::Instance().Exec(strSQL);

	m_criticalMsgDisplay.Unlock();
}

// prompt for file name - used for open and save as
// static function called from app
BOOL CXXX2App::PromptForFileName(CString& fileName, UINT nIDSTitle, 
	DWORD dwFlags, BOOL bOpenFileDialog, int* pType)
{
//	CImageSaveAsDlg dlgFile(bOpenFileDialog);
	CSEMFileDialog dlgFile(bOpenFileDialog,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,NULL,0,FALSE);

	CString title;
	if (bOpenFileDialog)
		title = GetResString( IDS_SD_DLGTITLE_IMAGEFILE1 );
	else
		title = GetResString( IDS_SD_DLGTITLE_IMAGEFILE2 );

	dlgFile.m_ofn.Flags |= dwFlags;

	int nDocType = (pType != NULL) ? *pType : CXIMAGE_FORMAT_BMP;
	if (nDocType==0) nDocType=1;

	if( bOpenFileDialog )	// 10.07.02
	{
		int nCount;
		GetFileExts( bOpenFileDialog, nCount );
		dlgFile.m_ofn.nFilterIndex = nCount +1;
	}
	else
	{
		int nIndex = GetIndexFromType(nDocType, bOpenFileDialog);
		if (nIndex == -1) nIndex = m_nPriorSaveType;
		dlgFile.m_ofn.nFilterIndex = nIndex +1;
	}

	// strDefExt is necessary to hold onto the memory from GetExtFromType
	CString strDefExt = GetExtFromType(nDocType).Mid(2,3);
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter = GetFileTypes(bOpenFileDialog);
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	// 10.07.02 根据目前的操作系统的版本信息显示不同对话框风格 
	DWORD dwVersion	= GetVersion(); 
	if(dwVersion < 0x80000000) 
		dlgFile.m_ofn.lStructSize = 88;	//2k,xp下显示新的版本
	else 
		dlgFile.m_ofn.lStructSize = 76;	//98下显示老版本
	// 10.07.02 根据目前的操作系统的版本信息显示不同对话框风格 

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	if (bRet){
		if (pType != NULL){
			int nIndex = (int)dlgFile.m_ofn.nFilterIndex - 1;
			ASSERT(nIndex >= 0);
			*pType = GetTypeFromIndex(nIndex, bOpenFileDialog);
		}
	}
	return bRet;
}

int CXXX2App::GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<SEM_IMAGE_FORMATS;i++){
		if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite){
			if (doctypes[i].nID == nDocType) return nCnt;
			nCnt++;
		}
	}
	return -1;
}

int CXXX2App::GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<SEM_IMAGE_FORMATS;i++){
		if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite){
			if (nCnt == nIndex)
//              return i; // PJO - Buglet ?
                return doctypes[i].nID;
			nCnt++;
		}
	}
//	ASSERT(FALSE);		// 10.07.02 delete, because now we have "All support files" item.
	return -1;
}

CString CXXX2App::GetExtFromType(int nDocType)
{
	for (int i=0;i<SEM_IMAGE_FORMATS;i++){
		if (doctypes[i].nID == nDocType)
			return doctypes[i].ext;
	}
	return CString("");
}

BOOL CXXX2App::GetWritableType(int nDocType){
	for (int i=0;i<SEM_IMAGE_FORMATS;i++){
		if (doctypes[i].nID == nDocType)
			return doctypes[i].bWrite;
	}
	return FALSE;
}

CString CXXX2App::GetFileTypes(BOOL bOpenFileDialog)
{
	CString str;
	for (int i=0;i<SEM_IMAGE_FORMATS;i++){
		if (bOpenFileDialog && doctypes[i].bRead){
			str += doctypes[i].description;
			str += (TCHAR)NULL;
			str += doctypes[i].ext;
			str += (TCHAR)NULL;
		} else if (!bOpenFileDialog && doctypes[i].bWrite) {
			str += doctypes[i].description;
			str += (TCHAR)NULL;
			str += doctypes[i].ext;
			str += (TCHAR)NULL;
		}
	}
	return str;
}

CString CXXX2App::GetFileExts(BOOL bOpenFileDialog, int& nCount)		// 10.07.02
{
	CString str;
	int n = 0;
	for (int i=0;i<SEM_IMAGE_FORMATS;i++){
		if (bOpenFileDialog && doctypes[i].bRead){
			n++;
			str += doctypes[i].ext;
			str += ";";
		} else if (!bOpenFileDialog && doctypes[i].bWrite) {
			n++;
			str += doctypes[i].ext;
			str += ";";
		}
	}
	str.Delete( str.GetLength() -1, 1 );	// delete the last ";"
	nCount = n;
	return str;
}


CString CXXX2App::FindExtension(const CString& name)
{
	int len = name.GetLength();
	for (int i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString("");
}


BOOL CXXX2App::PreTranslateMessage(MSG* pMsg) 
{
	return CWinApp::PreTranslateMessage(pMsg);
}

CView* CXXX2App::GetScanView()
{
	if( m_pSemScanDocTemplate == NULL )
		return NULL;

	ASSERT_KINDOF(CDocTemplate, m_pSemScanDocTemplate);
	POSITION pos = m_pSemScanDocTemplate->GetFirstDocPosition();
	if( pos )
	{
		CDocument* pDoc = m_pSemScanDocTemplate->GetNextDoc( pos );
		pos = pDoc->GetFirstViewPosition();
		if( pos )
		{
			CView* pView = pDoc->GetNextView( pos );
			if( pView )
				return pView;
		}
	}
	return NULL;
}

CWnd* CXXX2App::GetScanFrameWnd()
{
	if( m_pSemScanDocTemplate == NULL )		// 10.02.02添加保护
		return NULL;

	ASSERT(m_pSemScanDocTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, m_pSemScanDocTemplate);
	POSITION pos = m_pSemScanDocTemplate->GetFirstDocPosition();
	if( pos )
	{
		CDocument* pDoc = m_pSemScanDocTemplate->GetNextDoc( pos );
		pos = pDoc->GetFirstViewPosition();
		if( pos )
		{
			CView* pView = pDoc->GetNextView( pos );
			if( pView )
			{
				return (pView->GetParentFrame());
			}
		}
	}
	return NULL;
}

void CXXX2App::SetupSerialNumber()
{
	CString csCurrentDate, csRegDate;
	CTime	timeCurrent = CTime::GetCurrentTime();
	csCurrentDate = timeCurrent.Format(g_csRegDateFormat);
	
	csRegDate = GetProfileString( g_csSettingSection, g_csRegEntry_csCurrentDate, (LPCTSTR)g_csRegEntry_csCurrentDate );
	m_nSerialNumber = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nSerialNumber, 1 );

	if ( csRegDate != csCurrentDate )
	{
		m_csCurrentDate = csCurrentDate;
		WriteProfileString( g_csSettingSection, g_csRegEntry_csCurrentDate, m_csCurrentDate );

		m_nSerialNumber = 1;
	}
	else
	{
		m_csCurrentDate = csRegDate;
		m_nSerialNumber ++;
	}

	WriteProfileInt( g_csSettingSection, g_csRegEntry_nSerialNumber, m_nSerialNumber );

	m_nPhotoSerialNumber = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nPhotoSerialNumber, 0 );
	m_nDPhotoForeground = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nDPhotoForeground, 200 );
	m_nDPhotoBackground = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nDPhotoBackground, 100 );

	m_nOptimalBrightness = atol( GetProfileString( g_csSettingSection,g_csRegEntry_csOptimalBrightness, "0" ) );
	m_nOptimalContrast = atol( GetProfileString( g_csSettingSection,g_csRegEntry_csOptimalContrast, "0" ) );

	m_nPriorSaveType = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, g_csRegEntry_nPriorSaveType, 0 );
}

BOOL CXXX2App::OnIdle(LONG lCount) 
{
//	return CWinApp::OnIdle(lCount);

	if (CWinApp::OnIdle(lCount))
		return TRUE;

	CString str;
	BOOL bMore = TRUE;	// Next time give next task a chance
	switch (lCount)
	{
	case	1000:
//		PumpMCBSemMessage();
		break;

	case	1100:
		BYTE byteDB[2];
		CSemCtrl::Instance().USB_GetPortInfo( byteDB );
		if( (byteDB[0] == 2) && (byteDB[1] > 2) )
		{
			str.Format( "PORT D = 0x%02X, B = 0x%02X in idle", byteDB[0], byteDB[1] );
			DisplayMsg( str );
			ResponseUrgency( byteDB[1] );
		}
		break;
			
	case	1200:
			PumpSemError();
			break;
			
	case	1300:
//		SemStateMechanics();
		bMore = FALSE;	// cycle through the idle loop tasks again
		break;

	default:
		break;
	}

	return bMore;
}

BOOL CXXX2App::InquirerZ80B()
{
	BOOL	boolRet = FALSE;
	static	DWORD	lastTickCount = timeGetTime();
			DWORD	thisTickCount = timeGetTime();

	const DWORD dwInquirerTimer = 250;	// 250 ms
	
	if ( (thisTickCount - lastTickCount) > dwInquirerTimer )
	{
		lastTickCount = thisTickCount;

		CSemCoreAssemble::Instance().m_InquirerZ80B.Flush();
		boolRet = TRUE;
	}
		
	return boolRet;
}

BOOL CXXX2App::HVPower_IsReady()
{
	BOOL bReady = FALSE;
	switch( CHVPowerDataManager::Instance().m_dwHVtype )
	{
	case	0:
		bReady = CCPS3603tatusDetector::Instance().IsReady();
		break;
	case	1:
		bReady = CHVPowerDataManager::Instance().m_Spellmans.status.bReady;
		break;
	case	2:
	case	3:
		bReady = m_cfgWSM30KV.bReady;
		break;
	}
	return bReady;
}

void CXXX2App::SemStateMechanics()
{
	DisplayMsg( "Into sem state mechanics" );

	CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();

	pMainFrame->SetEnableClose( m_blReady_HV || CVACDataManager::Instance().m_VacParamMain.bVa );		// 11.07.15

	if( pMainFrame && pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
	{
		if( m_blOverhaulState )
		{
			// 维修状态下，若之前加的真高压，则主动关J14
			BOOL bReady =  HVPower_IsReady();
			pMainFrame->m_wndImpPanelAll2.SPR_En(FALSE);
			if( bReady )
				Sleep(1000);
			pMainFrame->m_wndImpPanelAll2.HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, TRUE );
			if( CHVPowerDataManager::Instance().m_dwHVtype > 0 )
			{
				pMainFrame->m_wndImpPanelAll2.HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, TRUE );
				pMainFrame->m_wndImpPanelAll2.HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, TRUE );
			}
		}
		else
		{
			pMainFrame->m_wndImpPanelAll2.HVPower_Reset();
			pMainFrame->m_wndImpPanelAll2.VAC_UpdateUIswitches();
		}
	}

	VARIANT	ClassID, SerialNumber, Enabled, Flag;
	ClassID.lVal		= SemControlPanelClass_CPS_HV;
	SerialNumber.lVal	= -1;
	Enabled.lVal		= m_blUnlock_HV;
	Flag.lVal			= 3;
	
	// Leo 2003.12.5 添加维修状态，此时高压控制不连锁。
	if ( m_blOverhaulState == TRUE )
	{
		Enabled.lVal	= TRUE;
	}

	// 05.06.22 使对比度使能与高压使能一致
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		ClassID.lVal		= SemControlPanelClass_Contrast;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	}
	else
	{
		ClassID.lVal		= SemControlPanelClass_SP_PMT;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	}
}

void CXXX2App::SystemReady( BOOL bVacuum, BOOL bHV, BOOL bFilament )
{
	m_blReady_Vacuum	= bVacuum;
	m_blReady_HV		= bHV;
	m_blReady_Filament	= bFilament;
}

BOOL CXXX2App::PumpSemError()
{
	static MicrocomputerID Z80ID = SEM_Z80ID_Z80A;
	CString csError;
	
	if ( Z80ID == SEM_Z80ID_Z80A )	Z80ID = SEM_Z80ID_Z80B;
	else							Z80ID = SEM_Z80ID_Z80A;

	if ( CSemCtrl::Instance().GetLastCommError(Z80ID, csError) )
	{
		if ( CAppLogManager::Instance().m_boolLogMsgPcMc == TRUE )
			DisplayMsg( csError );
	}


	if ( CSemVirtualMCICard::Instance().IsShouldReset() )
	{
		CScanView* pView = (CScanView*)GetScanView();
		if( pView )
			pView->ResetImageCard();


		csError = _T("Reset VGD!");
		DisplayMsg( csError );
	}


	if ( CSemCtrl::Instance().m_SemScan.IsAutoStop() )
	{
		CScanView* pView = (CScanView*)GetScanView();
		if( pView )
			pView->StopPhoto();

		csError = _T("Photo Finished!");
		DisplayMsg( csError );
	}

	return TRUE;
}

BOOL CXXX2App::PumpSemMessage()
{
	return TRUE;
}

BOOL CXXX2App::ProcessSemMessage( MicrocomputerID Z80ID, SEM_ControlMessage& mcMSG )
{
	CString csTemp;

	static int nLoop = 0;
	static BOOL blUnlock_HV_Old = TRUE;

	static BYTE bGunIOld = 0;

	VARIANT	ClassID, SerialNumber, Xpos, Ypos, Flag;

	const BYTE bCtrlParam	= (BYTE)(mcMSG.bData[1]);
	const long lObjectiveLens	= (long)bCtrlParam | ((long)mcMSG.bData[2] << 8);

	const long lStigmatorCTLIndexMin	= (long)-128;
	const long lStigmatorX	= (long)bCtrlParam		+ lStigmatorCTLIndexMin;
	const long lStigmatorY	= (long)mcMSG.bData[2]	+ lStigmatorCTLIndexMin;

	switch ( mcMSG.bMessageID )
	{
	case H_Z80A_Reset:	// H_Z80A_Reset H_Z80B_Reset
		if ( Z80ID == SEM_Z80ID_Z80A )
		{
			csTemp = GetResString( IDS_HD_RESET_PC1 );

			// 2005-3-7
			PC1ResetingProcess();	// 2005-3-7_PC1复位处理程序
		}
		else
		{
			csTemp = GetResString( IDS_HD_RESET_PC9 );

			// 2004-8-3
			AutoInitSemCtrl( TRUE );// 2004-6-2_自动保存并更新控制量
									// 05.06.22 加入参数标明是PC9复位
		}
		DisplayMsg( csTemp );
		break;
		
	case H_Z80B_Flag:
		m_blReady_Vacuum	= ( ( bCtrlParam & H_Z80B_Flag_Vacuum ) == H_Z80B_Flag_Vacuum );
		m_blReady_HV		= ( ( bCtrlParam & H_Z80B_Flag_HV ) == H_Z80B_Flag_HV );
		
		m_blUnlock_HV		= ( ( bCtrlParam & H_Z80B_Enable_HV ) == H_Z80B_Enable_HV ) && m_blReady_Vacuum;
		
		//======2004-8-23_控制量自动清零==================
		if ( (m_blUnlock_HV == FALSE) && (blUnlock_HV_Old == TRUE) )
		{
			// HV
			ClassID.lVal = SemControlPanelClass_CPS_HV;
			SerialNumber.lVal = -1;
			Flag.lVal = 1;
			Xpos.lVal = 0;
		
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);

			// Contrast 05.06.22 一旦检测到高压为零，对比度也降为零
			if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			{
				ClassID.lVal = SemControlPanelClass_Contrast;
				Flag.lVal = 1;
				Xpos.lVal = 0;
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);
			}
			else
			{
				ClassID.lVal = SemControlPanelClass_SP_PMT;
				Flag.lVal = 1;
				Xpos.lVal = 0;
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);
				if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
					m_WSM30KV2.SetParam( 4, Xpos.lVal );
				else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					m_WSM30KV.SetParam( 4, Xpos.lVal );
				else
					CSpellmanStatusDetector::Instance().SetParam( 35, Xpos.lVal );
			}
		}

		else if ( (m_blUnlock_HV == TRUE) && (blUnlock_HV_Old == FALSE) )
		{
			CSemCoreAssemble::Instance().m_SignalProcesser.Flush();
		}

		blUnlock_HV_Old = m_blUnlock_HV;

		m_blReady_Filament	= ( ( bCtrlParam & H_Z80B_Flag_Filament ) == H_Z80B_Flag_Filament );  

		SemStateMechanics();

		if( CAppLogManager::Instance().m_boolMsgFromMicroPCLog )
		{
			csTemp.Format("H_Z80B_Flag, ID=%d, param=0x%02X", mcMSG.bMessageID, bCtrlParam);
			DisplayMsg( csTemp );
		}
		break;

	case H_Z80B_FilamentHeatingCurrent:
		if( CAppLogManager::Instance().m_boolMsgFromMicroPCLog )
		{
			csTemp.Format("H_Z80B_FilamentHeatingCurrent, ID=%d, param=0x%02X", mcMSG.bMessageID, bCtrlParam);
			DisplayMsg( csTemp );
		}

		break;

	case H_Z80B_GunI:
		ClassID.lVal		= SemControlPanelClass_GunCurrent;
		SerialNumber.lVal	= -1;
		Flag.lVal			= 1;
		Xpos.lVal			= bCtrlParam;
		Ypos				= Xpos;
		
		if ( bGunIOld != bCtrlParam )
		{
			bGunIOld = bCtrlParam;
			CAppLogManager::Instance().m_boolGunCurrentUpdate = TRUE;
		}
		else
		{
			CAppLogManager::Instance().m_boolGunCurrentUpdate = FALSE;
		}

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);

		if( CAppLogManager::Instance().m_boolMsgFromMicroPCLog )
		{
			csTemp.Format("H_Z80B_GunI, ID=%d, param=0x%02X", mcMSG.bMessageID, bCtrlParam);
			DisplayMsg( csTemp );
		}

		break;

	case H_Z80_CheckedErrMsg:
		if ( Z80ID == SEM_Z80ID_Z80A )
			csTemp.Format( GetResString(IDF_HD_CheckedErr_SemMessage_MP1),
				mcMSG.bData[1], mcMSG.bData[2], mcMSG.bData[3], 
				mcMSG.bData[4], mcMSG.bData[5], mcMSG.bData[6] );
		else
			csTemp.Format( GetResString(IDF_HD_CheckedErr_SemMessage_MP9),
				mcMSG.bData[1], mcMSG.bData[2], mcMSG.bData[3], 
				mcMSG.bData[4], mcMSG.bData[5], mcMSG.bData[6] );
		DisplayMsg( csTemp );

		break;

	case H_Z80_UnrecognizedMsg:

		if ( Z80ID == SEM_Z80ID_Z80A )
			csTemp.Format(GetResString(IDF_HD_Unrecognized_SemMessage_MP1), mcMSG.bData[1], mcMSG.bData[2] );
		else
			csTemp.Format(GetResString(IDF_HD_Unrecognized_SemMessage_MP9), mcMSG.bData[1], mcMSG.bData[2] );
		DisplayMsg( csTemp );

		break;

	default:

		if ( Z80ID == SEM_Z80ID_Z80A )
			csTemp.Format(GetResString(IDF_HD_Unrecognized_MpMessage_MP1),
				mcMSG.bCtlMSGLength, mcMSG.bMessageID, 
				mcMSG.bData[1], mcMSG.bData[2], mcMSG.bData[3], mcMSG.bData[4] );
		else
			csTemp.Format(GetResString(IDF_HD_Unrecognized_MpMessage_MP9),
				mcMSG.bCtlMSGLength, mcMSG.bMessageID, 
				mcMSG.bData[1], mcMSG.bData[2], mcMSG.bData[3], mcMSG.bData[4] );

		DisplayMsg( csTemp );
		break;
	}

	return TRUE;
}

BOOL CXXX2App::PumpUSBSemMessage()
{
	USB_SEM_ControlMessage	mcMSG, mcMSGNext;

	if ( !CSemCtrl::Instance().USB_GetMSG( SEM_MCI, mcMSG ) )
		return FALSE;

	if ( !CSemCtrl::Instance().USB_GetMSG( SEM_MCI, mcMSGNext ) )
	{
		ProcessUSBSemMessage( SEM_MCI, mcMSG );
		return TRUE;
	}

	if ( mcMSG.bMessageID == mcMSGNext.bMessageID )
	{
		// 如果是中断消息，有可能有不同的中断号
		if ( mcMSG.bMessageID == USB_TTTT_INT 
			&& mcMSG.bData[1] == mcMSGNext.bData[1] )
			mcMSG = mcMSGNext;
		else
		{
			ProcessUSBSemMessage( SEM_MCI, mcMSG );
			ProcessUSBSemMessage( SEM_MCI, mcMSGNext );
			return TRUE;
		}
	}
	else
	{
		ProcessUSBSemMessage( SEM_MCI, mcMSG );
		ProcessUSBSemMessage( SEM_MCI, mcMSGNext );
		return TRUE;
	}

	while ( CSemCtrl::Instance().USB_GetMSG( SEM_MCI, mcMSGNext ) )
	{
		if ( mcMSG.bMessageID == mcMSGNext.bMessageID )
		{
			// 如果是中断消息，有可能有不同的中断号
			if ( mcMSG.bMessageID == USB_TTTT_INT 
				&& mcMSG.bData[1] == mcMSGNext.bData[1] )
				mcMSG = mcMSGNext;
			else
				break;
		}
		else
			break;
	}

	ProcessUSBSemMessage( SEM_MCI, mcMSG );
	if ( mcMSG.bMessageID != mcMSGNext.bMessageID )
		ProcessUSBSemMessage( SEM_MCI, mcMSGNext );
	return TRUE;
}

BOOL CXXX2App::ProcessUSBSemMessage( MicrocomputerID Z80ID, USB_SEM_ControlMessage& mcMSG )
{
	const BYTE bCtrlParam	= (BYTE)(mcMSG.bData[1]);
	CString str;

	switch ( mcMSG.bMessageID )
	{
	case USB_TTTT_INT:		// 需要响应下位机的紧急情况
		ResponseUrgency( bCtrlParam );
		break;
	case USB_TTTT_Mon_Err:	// 监测值有异常
		str.Format( "Monitor data errors: nModID = %d; nIndex = %d", mcMSG.bData[1], mcMSG.bData[2] );
		DisplayMsg( str );
		break;
	default:
		break;
	}

	return TRUE;
}

// 响应紧急情况
void CXXX2App::ResponseUrgency( BYTE bData )
{
	// 先读端口，看看是否系统已经响应了
	CString str;
	BYTE byteDB[2];
	unsigned short readData[512] = {0};
	for(int i=0; i<5; i++ )
	{
		CSemCtrl::Instance().USB_GetPortInfo( byteDB );
		str.Format( "PORT D = 0x%02X, B = 0x%02X in Response, %d times", byteDB[0], byteDB[1], i+1 );
		DisplayMsg( str );
		if( byteDB[0] != 0x02 )
			return;
		Sleep( 1 );
	}

	CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();

	if( bData == 0xFF )
	{
		// 从Z值最后一位判断出有中断的时候，从端口B得到中断号
		bData = byteDB[1];
		str.Format( "Receive MSG from VirtualMCICard: 0x%02X", bData );
		DisplayMsg( str );
	}

	if( bData == 0x04 )			// cB = 0100
	{
		str.Format( "INT 4 from SPR board: 0x%02X", bData );
		DisplayMsg( str );

		ZeroMemory(&readData, sizeof(readData));
		if( CSemCtrl::Instance().Serial_ReadbackData( 4, USB_CID_SPR_INT_Source_Register, &(readData[0]) ) )
		{
			unsigned short nData1 = readData[0];
			unsigned short nData2 = readData[1];
			if( nData1 == 0x0001 )		// V1 open
			{
				// 空扫时V1开，只需更新VACStateMachine V1状态
				Sleep( 1 );	// 等待VACStateMachine自动更新状态
			}
			else if( nData1 == 0x0002 )	// V1 close
			{
				// V1关，若正在看实际像关闭高压等；
				if( pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
					pMainFrame->m_wndImpPanelAll2.SPR_En( FALSE );
				if( pMainFrame->m_MainPanelDebug.m_hWnd != NULL )
					pMainFrame->m_MainPanelDebug.m_PageDebug2.SPR_En( FALSE );
				Sleep( 1 );	// 等待VACStateMachine自动更新状态
				DisplayMsg( "Vacuum turns BAD, shutdown HV power supply!");
				AfxMessageBox( "Vacuum turns BAD, shutdown HV power supply!\n\nYou should CLOSE V1 valve now!" );
			}
			// Clear port B & D
			CSemCtrl::Instance().USB_ClearPortDB();
		}
	}
	else if( bData == 0x03 )	// cB = 0011
	{
		str.Format( GetResString(IDF_SD_PROMPT_INT_01), bData );
		DisplayMsg( str );

		ZeroMemory(&readData, sizeof(readData));
		if( CSemCtrl::Instance().Serial_ReadbackData( 3, USB_CID_VAC_INT_Source_Register, &(readData[0]) ) )
		{
			unsigned short nData1 = readData[0];
			unsigned short nData2 = readData[1];
			str.Format( GetResString(IDF_SD_PROMPT_INT_02), nData1, nData2 );
			DisplayMsg( str );
			if( nData1 == 0x10 || nData1 == 0x20 || nData1 == 0x30 )
			{
				// 15.08.03 修改中断响应，重复一定次数收到中断后才响应，避免误报
				if( m_nINT_Counts < 1 )
				{
					// 第一次收到中断，开始计时并更新次数
					m_nINT_Ticks = GetTickCount();
					m_nINT_Ticks++;
				}
				else
				{
					DWORD nTicks = GetTickCount();
					if( nTicks -m_nINT_Ticks < 1000 )
					{
						// 是1秒内再次收到中断，更新次数
						m_nINT_Counts++;
						if( m_nINT_Counts > 2 )
						{
							// 1秒内3次收到中断，进行中断响应处理
							if( CVACDataManager::Instance().m_VacParamMain.bV1 )
							{
								if( pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
									pMainFrame->m_wndImpPanelAll2.SPR_En( FALSE );
								if( pMainFrame->m_MainPanelDebug.m_hWnd != NULL )
									pMainFrame->m_MainPanelDebug.m_PageDebug2.SPR_En( FALSE );
								DisplayMsg( GetResString(IDS_SD_PROMPT_INT_03) );
								str.Format( "%s\n\n%s", GetResString(IDS_SD_PROMPT_INT_03), GetResString(IDS_SD_PROMPT_VAC_01) );
								AfxMessageBox( str );
							}
							m_nINT_Counts = 0;
						}
					}
					else
					{
						// 1秒以上再次收到中断，重新开始计时并更新次数
						m_nINT_Ticks	= GetTickCount();
						m_nINT_Counts	= 1;
					}
				}
				Sleep( 1 );	// 等待VACStateMachine自动更新状态
			}
			// Clear port B & D
			CSemCtrl::Instance().USB_ClearPortDB();
			DisplayMsg( "Clear Port D & B" );
		}
		else
			DisplayMsg( "Can't read VAC INT Source Register" );
	}
	else if( bData == 0x02 )	// cB = 0010
	{
		str.Format( "INT 2 from LNS board: 0x%02X", bData );
		DisplayMsg( str );
	}
	else						// cB = 0001
	{
		str.Format( "INT 1 from SCN board: 0x%02X", bData );
		DisplayMsg( str );
	}
}

void CXXX2App::UpdateReadbackData( unsigned short* pnData, unsigned short& nData1, unsigned short& nData2 )
{
	CString str, strMsg;
	str.Format( "Data Received: %ld words, UpdateReadbackData", pnData[0] );

	strMsg = str;
	strMsg += "\r\n";
	unsigned long lCount = pnData[0];
	BOOL bFound = FALSE;
	nData1 = nData2 = 0;
	for( unsigned long i=1; i<=lCount; i++ )
	{
		str.Format( "%04X, ", pnData[i] );
		strMsg += str;
		if( pnData[i] == 0xD22D )
			bFound = TRUE;
		if( !bFound )
			continue;
		nData1 = pnData[i+1];
		if( i == lCount -1 )
			nData2 = pnData[i+1];
		else
			nData2 = pnData[i+2];
	}
	DisplayMsg( strMsg );
}

// Leo 2003.12.5 添加维修状态，此时高压控制不连锁。
void CXXX2App::OnCTRLOverhaul() 
{
	BOOL bVisible = m_blOverhaulState;
	
	if (bVisible == FALSE)
	{
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x02;
		dlg.m_strMsg = GetResString( IDS_SD_PROMPT_OVERHAUL_STATE );
		dlg.m_strTitle = GetResString( IDS_SD_PROMPT_OVERHAUL_TITLE );
		if( dlg.DoModal() == IDOK )
			bVisible = TRUE;
	}
	else
	{
		bVisible = FALSE;
	}

	m_blOverhaulState = bVisible;
	SemStateMechanics();	
}

// Leo 2003.12.5 添加维修状态，此时高压控制不连锁。
void CXXX2App::OnUpdateCTRLOverhaul(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_blOverhaulState;
	pCmdUI->SetCheck(bVisible);	
}

// 09.12.10 增加调试状态功能
// 10.01.22 增加调试状态功能参数：最佳亮度、最佳对比度值
void CXXX2App::SetupDebugParam( BOOL bDebug, int nBrightness, int nContrast )
{
	// 维修状态高压应用
	if( m_blOverhaulState != bDebug )
	{
		m_blOverhaulState = bDebug;
		SemStateMechanics();
	}

	if( bDebug )
		return;
	// 最佳亮度应用
	if( nBrightness < 1000 )
	{
		if( m_nOptimalBrightness != nBrightness ) 
		{
			m_nOptimalBrightness = nBrightness;

			CString str;
			str.Format( "%ld", nBrightness );
			WriteProfileString( g_csSettingSection,
				g_csRegEntry_csOptimalBrightness, str );
		}	
		if(	CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal != nBrightness )
		{
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal = SemControlPanelClass_Brightness;
			Flag.lVal = 1;
			Xpos.lVal = m_nOptimalBrightness;
		
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);
		}
	}

	// 最佳对比度应用
	if( nContrast > -1 )
	{
		if( m_nOptimalContrast != nContrast )
		{
			m_nOptimalContrast = nContrast;

			CString str;
			str.Format( "%ld", nContrast );
			WriteProfileString( g_csSettingSection,g_csRegEntry_csOptimalContrast, str );
		}
		if( CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal != nContrast )
		{
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal = SemControlPanelClass_Contrast;
			Flag.lVal = 1;
			Xpos.lVal = m_nOptimalContrast;
		
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag);
		}
	}
}

// 09.01.15 S.A.C.P.功能默认状态为不启用
void CXXX2App::OnCtrlSACP() 
{
	m_blSACPState = !m_blSACPState;
}

// 09.01.15 S.A.C.P.功能默认状态为不启用
void CXXX2App::OnUpdateCtrlSACP(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_blSACPState );
}

void CXXX2App::_InitDlgFlag( CSemCtrlDlgRenewSelect &dlg ) 
{
	dlg.m_boolHV						=	CAppLogManager::Instance().m_boolHVLog;
	dlg.m_boolScanShift					=	CAppLogManager::Instance().m_boolScanShiftLog;
	dlg.m_boolBeemAlignment				=	CAppLogManager::Instance().m_boolBeemAlignmentLog;
	dlg.m_boolBrightness				=	CAppLogManager::Instance().m_boolBrightnessLog;
	dlg.m_boolContrast					=	CAppLogManager::Instance().m_boolContrastLog;
	dlg.m_boolStigmator					=	CAppLogManager::Instance().m_boolStigmatorLog;
	dlg.m_boolObjectiveLens				=	CAppLogManager::Instance().m_boolObjectiveLensLog;
	dlg.m_boolCondenserLens				=	CAppLogManager::Instance().m_boolCondenserLensLog;
	dlg.m_boolFilamentCurrent			=	CAppLogManager::Instance().m_boolFilamentCurrentLog;
	dlg.m_boolMagnifier					=	CAppLogManager::Instance().m_boolMagnifierLog;
	dlg.m_boolWorkDistance				=	CAppLogManager::Instance().m_boolWorkDistanceLog;
	dlg.m_boolObjectiveLensReverse		=	CAppLogManager::Instance().m_boolObjectiveLensReverseLog;
	dlg.m_boolCondenserLensReverse		=	CAppLogManager::Instance().m_boolCondenserLensReverseLog;
	dlg.m_boolAuxiliaryStigmation		=	CAppLogManager::Instance().m_boolAuxiliaryStigmationLog;
	dlg.m_boolDCondenserLens			=	CAppLogManager::Instance().m_boolDCondenserLensLog;
	dlg.m_boolScanRotate				=	CAppLogManager::Instance().m_boolScanRotateLog;
	dlg.m_boolSACP						=	CAppLogManager::Instance().m_boolSACPLog;
	dlg.m_boolDFocus					=	CAppLogManager::Instance().m_boolDFocusLog;
	dlg.m_boolDFocusSwitch				=	CAppLogManager::Instance().m_boolDFocusSwitchLog;
	dlg.m_boolDObjectiveLens			=	CAppLogManager::Instance().m_boolDObjectiveLensLog;
	dlg.m_boolDObjectiveLensSwitch		=	CAppLogManager::Instance().m_boolDObjectiveLensSwitchLog;
	dlg.m_boolRobinsonDetectorSwitch	=	CAppLogManager::Instance().m_boolRobinsonDetectorSwitchLog;
	dlg.m_boolSignal2Brightness			=	CAppLogManager::Instance().m_boolSignal2BrightnessLog;
	dlg.m_boolSignal2Contrast			=	CAppLogManager::Instance().m_boolSignal2ContrastLog;
	dlg.m_boolYAdjust					=	CAppLogManager::Instance().m_boolYAdjustLog;
	dlg.m_boolImageReverse				=	CAppLogManager::Instance().m_boolImageReverseLog;
	dlg.m_boolMsgFromMicroPC			=	CAppLogManager::Instance().m_boolMsgFromMicroPCLog;
}


void CXXX2App::_SaveDlgFlag( CSemCtrlDlgRenewSelect &dlg ) 
{
	CAppLogManager::Instance().m_boolHVLog						=	dlg.m_boolHV;
	CAppLogManager::Instance().m_boolScanShiftLog				=	dlg.m_boolScanShift;
	CAppLogManager::Instance().m_boolBeemAlignmentLog			=	dlg.m_boolBeemAlignment;
	CAppLogManager::Instance().m_boolBrightnessLog				=	dlg.m_boolBrightness;
	CAppLogManager::Instance().m_boolContrastLog				=	dlg.m_boolContrast;
	CAppLogManager::Instance().m_boolStigmatorLog				=	dlg.m_boolStigmator;
	CAppLogManager::Instance().m_boolObjectiveLensLog			=	dlg.m_boolObjectiveLens;
	CAppLogManager::Instance().m_boolCondenserLensLog			=	dlg.m_boolCondenserLens;
	CAppLogManager::Instance().m_boolFilamentCurrentLog		=	dlg.m_boolFilamentCurrent;
	CAppLogManager::Instance().m_boolMagnifierLog				=	dlg.m_boolMagnifier;
	CAppLogManager::Instance().m_boolWorkDistanceLog			=	dlg.m_boolWorkDistance;
	CAppLogManager::Instance().m_boolObjectiveLensReverseLog	=	dlg.m_boolObjectiveLensReverse;
	CAppLogManager::Instance().m_boolCondenserLensReverseLog	=	dlg.m_boolCondenserLensReverse;
	CAppLogManager::Instance().m_boolAuxiliaryStigmationLog	=	dlg.m_boolAuxiliaryStigmation;
	CAppLogManager::Instance().m_boolDCondenserLensLog			=	dlg.m_boolDCondenserLens;
	CAppLogManager::Instance().m_boolScanRotateLog				=	dlg.m_boolScanRotate;
	CAppLogManager::Instance().m_boolSACPLog					=	dlg.m_boolSACP;
	CAppLogManager::Instance().m_boolDFocusLog					=	dlg.m_boolDFocus;
	CAppLogManager::Instance().m_boolDFocusSwitchLog			=	dlg.m_boolDFocusSwitch;
	CAppLogManager::Instance().m_boolDObjectiveLensLog			=	dlg.m_boolDObjectiveLens;
	CAppLogManager::Instance().m_boolDObjectiveLensSwitchLog	=	dlg.m_boolDObjectiveLensSwitch;
	CAppLogManager::Instance().m_boolRobinsonDetectorSwitchLog	=	dlg.m_boolRobinsonDetectorSwitch;
	CAppLogManager::Instance().m_boolSignal2BrightnessLog		=	dlg.m_boolSignal2Brightness;
	CAppLogManager::Instance().m_boolSignal2ContrastLog		=	dlg.m_boolSignal2Contrast;
	CAppLogManager::Instance().m_boolYAdjustLog				=	dlg.m_boolYAdjust;
	CAppLogManager::Instance().m_boolImageReverseLog			=	dlg.m_boolImageReverse;
	CAppLogManager::Instance().m_boolMsgFromMicroPCLog			=	dlg.m_boolMsgFromMicroPC;
}

void CXXX2App::OnLogSelect() 
{
	// TODO: Add your command handler code here
	CSemCtrlDlgRenewSelect	dlg;
	_InitDlgFlag(dlg);

	dlg.m_boolIsRecord = TRUE;
	if ( dlg.DoModal() != IDOK )
		return;

	_SaveDlgFlag(dlg);
}

void CXXX2App::OnLogMsg() 
{
	// TODO: Add your command handler code here
	CAppLogManager::Instance().m_boolLogMsgPcMc = !CAppLogManager::Instance().m_boolLogMsgPcMc;
}

void CXXX2App::OnUpdateLogMsg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( CAppLogManager::Instance().m_boolLogMsgPcMc );
}

void CXXX2App::OnFileNew() 
{
	// TODO: Add your command handler code here
	static BOOL blStart = FALSE;

	if ( !blStart )
	{
		blStart = TRUE;
		ASSERT(m_pSemScanDocTemplate != NULL);
		ASSERT_KINDOF(CDocTemplate, m_pSemScanDocTemplate);
		m_pSemScanDocTemplate->OpenDocumentFile(NULL);
	}
	else
	{
		ASSERT(m_pSemImageDocTemplate != NULL);
		ASSERT_KINDOF(CDocTemplate, m_pSemImageDocTemplate);
		m_pSemImageDocTemplate->OpenDocumentFile(NULL);
	}
}

CDocument* CXXX2App::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class
	return m_pSemImageDocTemplate->OpenDocumentFile(lpszFileName);

//	return CWinApp::OpenDocumentFile(lpszFileName);
}

void CXXX2App::OnFileOpen()
{
	// prompt the user (with all document templates)
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

	OpenDocumentFile(newName);
	// if returns NULL, the user has already been alerted
	return;

	// 读取图像
	CString filename(newName);
	CString ext = filename.Right(3);
	ext.MakeLower();
	if (ext == "") return;

	BeginWaitCursor();
	int nType = CxImage::GetTypeIdFromName(ext);
	if (nType == CXIMAGE_FORMAT_UNKNOWN)
		nType = SEMIMAGE_FORMAT_SEM;

	CThumbImageItem* pImageItem = new CThumbImageItem();
	if (nType < SEMIMAGE_FORMAT_SEM)
		pImageItem->m_pImageSrc = new CxImage(filename, nType);
	else
	{
		// 读取sem文件并生成图像
		CFile file;
		if( !file.Open(filename, CFile::modeRead) )
			return;

		DWORD dwImageFileSize;										//图像文件的大小
		DWORD dwImageSize;											//图像的大小

		BITMAPFILEHEADER	bmpFileHeader;
		file.Read( &dwImageFileSize, sizeof( DWORD ) );

		file.Read( &bmpFileHeader, sizeof( BITMAPFILEHEADER ) );
		dwImageSize = dwImageFileSize - sizeof(BITMAPFILEHEADER);

		if( dwImageSize > 0 )
		{
			HANDLE hDIB = (HANDLE)GlobalAlloc(GHND, dwImageSize);
			if( hDIB == NULL )
			{
				file.Close();
				return;
			}
			
			BYTE* pGreyImageBuffer = (BYTE *)::GlobalLock( hDIB );
			file.Read( pGreyImageBuffer, dwImageSize );
			GlobalUnlock( hDIB );

			CxImage *newImage = new CxImage();
			newImage->CreateFromHANDLE( hDIB );
			pImageItem->m_pImageSrc = newImage;

			GlobalFree( hDIB );
		}

		file.Close();
	}

	if( pImageItem->GetImage() != NULL )
		((CMainFrame *)GetMainWnd())->m_wndImpPanelThumb.AddItem( pImageItem );

	EndWaitCursor();
}

AFX_STATIC void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

BOOL CXXX2App::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	CSEMFileDialog dlgFile(bOpenFileDialog,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,NULL,0,FALSE);

	CString title;
	VERIFY(title = GetResString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	// 10.07.02
	CString strAllSupport;
	int nCount = 0;
	// 10.07.02
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		strFilter = GetFileTypes(bOpenFileDialog);
		// 10.07.02
		strAllSupport = GetFileExts( bOpenFileDialog, nCount );

		CString strTemp;
		strTemp.Empty();
     }

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	// 10.07.02 根据目前的操作系统的版本信息显示不同对话框风格 
	DWORD dwVersion	= GetVersion(); 
	if(dwVersion < 0x80000000) 
		dlgFile.m_ofn.lStructSize = 88;	//2k,xp下显示新的版本
	else 
		dlgFile.m_ofn.lStructSize = 76;	//98下显示老版本
	// 10.07.02 根据目前的操作系统的版本信息显示不同对话框风格 

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}

void CXXX2App::OnHelp() 
{
	// TODO: Add your command handler code here
	WORD lid = MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
	if( CGlobalParamSetting::Instance().m_wLanguageID != lid )
		return;
	TCHAR path[255];
	::GetModuleFileName( NULL, path, 255);
//	::PathRemoveFileSpec( path );
	TCHAR *ptemp = strrchr( path, '\\');
	ptemp[1] = 0;
	ShellExecute(AfxGetMainWnd()->m_hWnd,NULL,_T("hh.exe"),_T("KYKY-SEM.chm"),path,1);
}


// Language
void CXXX2App::Localize(void)
{
	CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
	pMainFrame->Localize();
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 )
		StageTMCM_SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 )
		StageMC6600_SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
		StageMC600_SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
	if(  CControlLayer::Instance().m_bHasBSD)
		BSD_SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
	if( m_bHasCCD )
		CCD_SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
	if( m_bHasNAV )
		NAV_SetLanguageID(CGlobalParamSetting::Instance().m_wLanguageID);
}

void CXXX2App::OnOptionsSetupLanguageChs() 
{
	// TODO: Add your command handler code here
	WORD lid = MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
	if( CGlobalParamSetting::Instance().m_wLanguageID != lid )
	{
		CGlobalParamSetting::Instance().m_wLanguageID = lid;
//		::SetThreadLocale( MAKELCID( m_wLanguageID, SORT_DEFAULT ) );
		m_LanguageManagerCtrl.SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
		Localize();
	}
}

void CXXX2App::OnOptionsSetupLanguageEn() 
{
	// TODO: Add your command handler code here
	WORD lid = MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
	if( CGlobalParamSetting::Instance().m_wLanguageID != lid )
	{
		CGlobalParamSetting::Instance().m_wLanguageID = lid;
		m_LanguageManagerCtrl.SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
		Localize();
	}
}

void CXXX2App::MyAppendMenu(CMenu* pmenu, UINT nRootMenuID, LPCTSTR lpFileName)
{
	char szBufSection[20], szBufKey[20], szBuf[32];
	sprintf(szBufSection, "Menu_%d", nRootMenuID);
	int nMenuCount = 0;
	UINT nID = 0;
	if(GetPrivateProfileString( szBufSection, "Count", "0", szBuf, 255, lpFileName) != 0)
	{
		// 获取其中一个子菜单的数目
		nMenuCount = atoi(szBuf);
		for( int i=0; i<nMenuCount; i++ )
		{
			sprintf(szBufKey, "menu%d", i);
			if(GetPrivateProfileString(szBufSection, szBufKey, "0", szBuf, 255, lpFileName) != 0)
			{
				nID = atoi(szBuf);
				if( nID == ID_APP_ABOUT )		// about
				{
					// 关于
					CString str, str2, str3;
					str = GetResString(IDS_MENUITEM_APP_ABOUT_6K);
					int nPos = str.Find(' ');
					str2 = str.Left(nPos +1);
					WORD lid = MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
					if( CGlobalParamSetting::Instance().m_wLanguageID != lid )
						nPos = str.Find('.');
					else
						nPos = str.Find('(');
					str3 = str.Right( str.GetLength() -nPos );
					str.Format( "%s%s%s", str2, theApp.m_strLogoDisp, str3 );
//					pmenu->AppendMenu( MF_STRING, nID, (LPCTSTR)GetResString( IDS_MENUITEM_APP_ABOUT_6K ) );
					pmenu->AppendMenu( MF_STRING, nID, (LPCTSTR)str );
				}
				else if( nID == 0 )				// SEPARATOR
					pmenu->AppendMenu(MF_SEPARATOR);
				else if( nID >= ID_FILE_NEW )	// system menu(0xE100 --> 0xE810)
					pmenu->AppendMenu( MF_STRING, nID, (LPCTSTR)GetResString( nID -ID_FILE_NEW +IDS_MENUITEM_FILE_NEW ) );
				else if( nID > IDS_MENUITEM_ROOT_SCAN_SETUP )
				{
					// from 42070 --> 42099, has sub menu
					CMenu menuSub;
					menuSub.CreateMenu();
					MyAppendMenu( &menuSub, nID, lpFileName );
					pmenu->AppendMenu(MF_POPUP, (UINT) menuSub.m_hMenu, GetResString(nID));
					menuSub.Detach();
				}
				else	// from 40001 --> 40999
					pmenu->AppendMenu(MF_STRING, nID, (LPCTSTR)GetResString( nID -ID_CXIMAGE_ALPHAPALETTETOGGLE +IDS_MENUITEM_CXIMAGE_ALPHAPALETTETOGGLE ) );
			}
		}
// 		if( (nRootMenuID == IDS_MENUITEM_ROOT_SCAN_IMAGE) && (CStageXYManager::Instance().m_Stage.nHasAutoStage > 0) )
// 			pmenu->AppendMenu(MF_STRING, ID_Stage_Coordinate_origin, (LPCTSTR)GetResString( ID_Stage_Coordinate_origin -ID_CXIMAGE_ALPHAPALETTETOGGLE +IDS_MENUITEM_CXIMAGE_ALPHAPALETTETOGGLE ) );
	}
}


void CXXX2App::CFGfile_InitParams()
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	CHVPowerDataManager::Instance().CFGFile_InitCPS3603s( path );
	// 2023.03 临时
	m_cfgWSM30KV.nPort	= 1;
	if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		m_cfgWSM30KV.nPort = CHVPowerDataManager::Instance().m_CPS3603s.nPort +1;

	CCFGFileManager::Instance().CFGfile_InitChangingParams( path,CControlLayer::Instance().m_DL );
	CCFGFileManager::Instance().CFGfile_InitSelectedADC( path,CControlLayer::Instance().m_DL );

	double*	testData = NULL;
	CCFGFileManager::Instance().CFGfile_InitLoggingParams( path, CControlLayer::Instance().m_DL, testData );
	if( testData != NULL )
		delete [] testData;

	CFGfile_InitHVParams( path );
	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
	{
		CFGfile_InitVACParams( path );
	}
	else
	{
		CFGfile_InitVACParamsNew( path );
	}
	CFGfile_InitSCNParams( path );
	CCFGFileManager::Instance().CFGfile_InitLNSParams( path,CMCBControler::Instance().m_nDFforFFBase );
	CCFGFileManager::Instance().CFGfile_InitMCIParams( path );

	CCFGFileManager::Instance().CFGfile_InitAutoTestParams( path,CControlLayer::Instance().m_AT );
	CFGfile_InitScanParams( path );

	CCFGFileManager::Instance().CFGfile_InitFastParams( path,CControlLayer::Instance().m_Fast, CHVPowerDataManager::Instance().m_dwHVtype );

	CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
	if( pMainFrame != NULL && pMainFrame->m_dlgAutotest.m_hWnd != NULL )
		pMainFrame->m_dlgAutotest.InitParams();
	if( pMainFrame != NULL && pMainFrame->m_dlgCali.m_hWnd != NULL )
		pMainFrame->m_dlgCali.InitParams();

	// 其它参数
	CSemVirtualMCICard::Instance().CFGfile_InitImageCardParams(path);

	CScanView* pView = (CScanView*)GetScanView();
/*	// 读ini中的聚光镜快速档位参数组
	if( pView != NULL )
	{
		long* plCLfast = new long[4];
		int nCount = CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "CLFast", LONGSTYLE, 4, plCLfast);
		for( int i=0; i<nCount; i++ )
			pView->m_lCLfast[i]	= plCLfast[i];
		delete [] plCLfast;
	}
*/
	CCFGFileManager::Instance().CFGFile_InitSemCoreParams(path,m_strLogo);

	// 读ini中的扫描旋转度数校准值和限制值
	int nRotate = GetPrivateProfileInt( "Calibrate_Offset", "RotateLimit", 10, path );
	if( pView != NULL )
		pView->m_nCommScanRotAngInit = nRotate;

	CCFGFileManager::Instance().CFGFile_InitNetParams(path,CControlLayer::Instance().m_Net);

	if( pMainFrame->m_dlgDebug.m_hWnd != NULL )
	{
		if( pMainFrame->m_dlgDebug.m_pPageAdvance->m_hWnd != NULL )
		{
			pMainFrame->m_dlgDebug.m_pPageAdvance->m_ipAddress.SetAddress( CControlLayer::Instance().m_Net.LocalAddr.sin_addr.S_un.S_addr );
			CString str;
			str.Format( "%d", CControlLayer::Instance().m_Net.LocalAddr.sin_port );
			pMainFrame->m_dlgDebug.m_pPageAdvance->GetDlgItem( IDC_IP_Debug_Port )->SetWindowText( str );
		}
	}

	if( pView != NULL )
	{
		// 2024.07 远程参数
		pView->m_nCommTimerMode = GetPrivateProfileInt( "Remote", "TimerMode", 0, path );
		pView->m_nCommTimerInteval = GetPrivateProfileInt( "Remote", "Interval", 1000, path );

		pView->ListenStart();
	}
}


void CXXX2App::CFGfile_InitHVParams( char* path )
{
	CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
	if (NULL == pMainFrame->m_dlgDebug.m_hWnd || NULL == pMainFrame->m_dlgDebug.m_pPageAdvance->m_hWnd)
	{
		return;
	}

	CComboBox* pEMI = ((CComboBox*)(pMainFrame->m_dlgDebug.m_pPageAdvance->GetDlgItem(IDC_COMBO_Debug_EMI)));
	if( pEMI != NULL )
	{
		char szBuf[20];
		if(GetPrivateProfileString("Auto", "EMI", "", szBuf, 15, path) != 0)
		{
			int nEMI = atoi(szBuf);
			switch( nEMI )
			{
			case	100:
				pEMI->SetCurSel(0);
				break;
			case	150:
				pEMI->SetCurSel(1);
				break;
			case	200:
				pEMI->SetCurSel(2);
				break;
			case	250:
				pEMI->SetCurSel(3);
				break;
			}
		}
	}
}

void CXXX2App::CFGfile_InitVACParams( char* path )
{
	CVACDataManager::Instance().CFGfile_InitVacParams( path );
	CVACDataManager::Instance().CFGfile_InitCCGParams( path );

	CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
	if( pMainFrame && (pMainFrame->m_dlgDebugVIP.m_hWnd != NULL))
	{
		((CButton*)(pMainFrame->m_dlgDebugVIP.GetDlgItem( IDC_CHECK_Debug_CCGvalueLock)))->SetCheck( CVACDataManager::Instance().m_cfgVacCCG.bInInterlock );
		// 16.07 V1阀控制方式
		if( CVACDataManager::Instance().m_dwV1CtrlMode< 1 )
			pMainFrame->m_dlgDebugVIP.GetDlgItem( IDC_CHECK_Debug_CCGvacuum )->ShowWindow(SW_HIDE);
		else
		{
			if( CSemCtrl::Instance().m_nVACBoardType < 1 )
				pMainFrame->m_dlgDebugVIP.GetDlgItem( IDC_CHECK_Debug_CCGvacuum )->ShowWindow(SW_SHOW);
			else
				pMainFrame->m_dlgDebugVIP.GetDlgItem( IDC_CHECK_Debug_CCGvacuum )->ShowWindow(SW_HIDE);
		}
	}
}

void CXXX2App::CFGfile_InitVACParamsNew( char* path )
{
	char		szBuf[20], szBuf1[20], stabuff[20];
	sprintf(stabuff, "BoardParam");

	// CCG阈值
	CVACDataManager::Instance().m_thresholdsVAC.fCCG1 = 5.5e-5;	// Torr
	CVACDataManager::Instance().m_thresholdsVAC.fCCG2 = 5.0e-5;	// Torr
	if(GetPrivateProfileString(stabuff, "Thresholds", "", szBuf, 18, path) != 0)
	{
		strcpy(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf1, strrchr(szBuf, ',')+1);	// szBuf1中是阈值2
			*strchr(szBuf, ',') = '\0';				// szBuf中是阈值1
			CVACDataManager::Instance().m_thresholdsVAC.fCCG2 = atof( szBuf );	// Torr
			CVACDataManager::Instance().m_thresholdsVAC.fCCG1 = atof( szBuf1 );
		}
	}
	CString str;
	str.Format( "Thresholds of CCG = %.1e, %.1e", CVACDataManager::Instance().m_thresholdsVAC.fCCG1, CVACDataManager::Instance().m_thresholdsVAC.fCCG2 );
	DisplayMsg(str);
//	m_cfgVacCCG.bInInterlock = (BOOL)(GetPrivateProfileInt( stabuff, "InInterlock", 0, path ));

	// TC Amp的最大值
	if(GetPrivateProfileString(stabuff, "Max", "", szBuf, 15, path) != 0)
	{
		strcpy(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf1, strrchr(szBuf, ',')+1);
			*strchr(szBuf, ',') = '\0';
			CVACDataManager::Instance().m_thresholdsVAC.fTC3Max	= atof( szBuf );
			CVACDataManager::Instance().m_thresholdsVAC.fTC4Max	= atof( szBuf1 );
		}
	}
	// TC Amp的最小值
	if(GetPrivateProfileString(stabuff, "Min", "", szBuf, 15, path) != 0)
	{
		strcpy(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf1, strrchr(szBuf, ',')+1);
			*strchr(szBuf, ',') = '\0';
			CVACDataManager::Instance().m_thresholdsVAC.fTC3Min	= atof( szBuf );
			CVACDataManager::Instance().m_thresholdsVAC.fTC4Min	= atof( szBuf1 );
		}
	}

	// 幂函数系数
	CVACDataManager::Instance().m_thresholdsVAC.fTC3 = 0.35f;	// V, 2.5e-1 Torr; 0.24V 大约对应4.0e-1 Torr
	CVACDataManager::Instance().m_thresholdsVAC.fTC4 = 0.35f;	// V, 2.5e-1 Torr; 0.24V 大约对应4.0e-1 Torr
	int nTotal = 4;
	double* pFactors = new double[nTotal];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "Factors", DOUBLESTYLE, nTotal, pFactors) == nTotal )
	{
		// 电压阈值（即为真空度阈值）(TC3/4)
		nTotal = 3;
		double* pThresholds	= new double[nTotal];
		if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "Thresholds2", DOUBLESTYLE, nTotal, pThresholds) == nTotal )
		{
			// Thresholds_AmpVoltage = A * (Vacuum ^ B)
			CVACDataManager::Instance().m_thresholdsVAC.fTC3 = pFactors[0] * pow(pThresholds[0], pFactors[1]);
			CVACDataManager::Instance().m_thresholdsVAC.fTC4 = pFactors[2] * pow(pThresholds[2], pFactors[3]);
		}
		delete [] pThresholds;
	}
	delete [] pFactors;

	// 真空计类型：ini中0为INFICON，1为WPC
	CVACDataManager::Instance().m_cfgVacCCG.nType = 3;
	if(GetPrivateProfileString(stabuff, "GaugeType", "", szBuf, 10, path) != 0)
		CVACDataManager::Instance().m_cfgVacCCG.nType = 3 +atoi(szBuf);	// 3为INFICON，4为WPC
}

void CXXX2App::CFGfile_InitSCNParams( char* path )
{
	// 15.06.19 使用原来闲置的xover功能来调整最低放大倍数档的偏心问题
	//BYTE byteHi, byteLo;
	// 从配置文件中读取参数值
	int nPosX = GetPrivateProfileInt( "Calibrate_Offset", "LowAmpX", 0, path) +2048;
	int nPosY = GetPrivateProfileInt( "Calibrate_Offset", "LowAmpY", 0, path) +2048;
	CCFGFileManager::Instance().CFGfile_InitSEMCtrlParams(nPosX, nPosY);

	// 若有面板，要同步面板显示值
		CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
		if( (pMainFrame->m_dlgCali.m_hWnd != NULL))
			pMainFrame->m_dlgCali.SCN_SyncXOverPos( nPosX, nPosY );
}

void CXXX2App::CFGfile_InitScanParams( char* path )
{
	CScanParameterManager::Instance().ReadFromINI(path);
	
	CScanView* pView = (CScanView*)GetScanView();
	if( pView != NULL )
	{
		pView->CheckCCDSwitch();
		pView->NotifyScanResolution( CScanParameterManager::Instance().m_nusbScanResoIndex, TRUE );
	}

	CImageDataManager::Instance().ReadFromINI(path);
}

void CXXX2App::_InitDocTemplateList()
{
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_XXX2TYPE,
		RUNTIME_CLASS(CXXX2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CXXX2View));
	pDocTemplate->SetContainerInfo(IDR_XXX2TYPE_CNTR_IP);
	pDocTemplate->SetServerInfo(
		IDR_XXX2TYPE_SRVR_EMB, IDR_XXX2TYPE_SRVR_IP,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
		IDR_IMGTYPE,
		RUNTIME_CLASS(CImageDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CImageView));
	AddDocTemplate(pDocTemplate);
	m_pSemImageDocTemplate		= pDocTemplate;

	/*	pDocTemplate = new CMultiDocTemplate(
	IDR_SEMTYPE,
	RUNTIME_CLASS(CSemControlRecordDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CSemControlRecordView));
	AddDocTemplate(pDocTemplate);

	/*	pDocTemplate = new CMultiDocTemplate(
	IDR_SEMTYPE,
	RUNTIME_CLASS(CSemControlDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CSemControlView));
	AddDocTemplate(pDocTemplate);	//2005.01.24原来为屏蔽掉的，程序执行后又内存泄漏，改后无
	m_pSemControlDocTemplate	= pDocTemplate;

	/*	pDocTemplate = new CMultiDocTemplate(
	IDR_LOGTYPE,
	RUNTIME_CLASS(CMessageDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CMessageView));
	AddDocTemplate(pDocTemplate);
	m_pSemLogDocTemplate		= pDocTemplate;
	*/
	pDocTemplate = new CMultiDocTemplate(
		IDR_SCANTYPE,
		RUNTIME_CLASS(CScanDoc),
		RUNTIME_CLASS(CScanChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CScanView));
	AddDocTemplate(pDocTemplate);
	m_pSemScanDocTemplate		= pDocTemplate;
}

void CXXX2App::_ReadFromRegistry()
{
	CString strTitle, strSubKey;
	strTitle.LoadString( AFX_IDS_APP_TITLE );
	strSubKey.Format( "Software\\KYKY\\%s\\%s", strTitle, g_csSettingSection );
	HKEY hk;
	if( RegOpenKeyEx( HKEY_CURRENT_USER, (LPCTSTR)strSubKey,0, KEY_ALL_ACCESS,&hk ) != ERROR_SUCCESS )
	{
		return;
	}
	
	DWORD dwValue = 0;
	DWORD dwSize = sizeof(DWORD);
	if( RegQueryValueEx( hk, (LPCTSTR)g_csRegEntry_nLanguage,NULL,NULL,(LPBYTE)&dwValue,&dwSize) == ERROR_SUCCESS )
	{
		CGlobalParamSetting::Instance().m_wLanguageID	= (WORD)dwValue;
	}

	//从注册表中读取自动台参数
	dwValue = 0;
	if( RegQueryValueEx( hk, (LPCTSTR)g_csRegEntry_nAutoStage,NULL,NULL,(LPBYTE)&dwValue,&dwSize) == ERROR_SUCCESS )
	{
		CStageXYManager::Instance().m_Stage.nHasAutoStage	= dwValue;
	}

	//从注册表中读取CCD参数
	dwValue = 0;
	if( RegQueryValueEx( hk, (LPCTSTR)g_csRegEntry_nCCD,NULL,NULL,(LPBYTE)&dwValue,&dwSize) == ERROR_SUCCESS )
	{
		m_bHasCCD	= dwValue;
	}

	//从注册表中读取BSD参数
	dwValue = 0;
	if( RegQueryValueEx( hk, (LPCTSTR)g_csRegEntry_nBSD,NULL,NULL,(LPBYTE)&dwValue,&dwSize) == ERROR_SUCCESS )
	{
		CControlLayer::Instance().m_bHasBSD	= dwValue;
	}

	//从注册表中读取EDS参数
	dwValue = 0;
	if( RegQueryValueEx( hk, (LPCTSTR)g_csRegEntry_nEDS,NULL,NULL,(LPBYTE)&dwValue,&dwSize) == ERROR_SUCCESS )
	{
		m_bHasEDS	= dwValue;
	}

	//从注册表中读取NAV参数
	dwValue = 0;
	if( RegQueryValueEx( hk, (LPCTSTR)g_csRegEntry_nNAV,NULL,NULL,(LPBYTE)&dwValue,&dwSize) == ERROR_SUCCESS )
	{
		m_bHasNAV	= dwValue;
	}

	//从注册表中读取真空控制板类型参数
	dwValue = 0;
	if( RegQueryValueEx( hk, (LPCTSTR)g_csRegEntry_nBoardType,NULL,NULL,(LPBYTE)&dwValue,&dwSize) == ERROR_SUCCESS )
	{
		CSemCtrl::Instance().m_nVACBoardType	= dwValue;
	}

	//从注册表中读取PA计参数
	dwValue = 0;
	if( RegQueryValueEx( hk, (LPCTSTR)g_csRegEntry_nPA,NULL,NULL,(LPBYTE)&dwValue,&dwSize) == ERROR_SUCCESS )
	{
		CControlLayer::Instance().m_nPAType	= dwValue;
	}

	RegCloseKey(hk);
}
