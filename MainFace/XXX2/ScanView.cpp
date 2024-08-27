// ScanView.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2011.07.19

/////////////////////////////////////////////////////////////////////////////
// 07.07---07.08 加入任意方向标尺
// 09.11.18 左键已按下时再按下右键，消像散值会使物镜值错乱。OnRButtonDown
// 09.11.18 增加单击测试类型GraphHitTestNULL，
//			将整个扫描区域内外的单击测试类型区别开。GraphHitTest
// 09.11.18 若当前没有被选中的单个项目，且并非处于多文件选择状态，
//			则遍历项目列表中的每一个项目，将所有item都设为非选中显示
// 09.11.18 对OnLButtonDown,OnMouseMove,OnRButtonDown中的
//			m_rcDragRectLast,SetCapture(),SemCtrlMouse_Init的执行条件
//			及m_bCatch的设定做了修正
// 09.11.19 将所有LoadString处均改为GetResString
// 09.12.02 修改OnDraw，确保图像区域以外的部分能够FillRect
// 09.12.04 整理任意方向移动标尺代码
// 09.12.10 增加调试状态功能
// 10.07.02 增加优先存储格式
// 11.07.19 按不同机型设置不同的IoRead循环次数

// 12.02.15 增加单双探测器标识
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "XXX2.h"
#include "ScanView.h"
#include "ScanChildFrm.h"
#include "MainFrm.h"
#include "SheetSetup.h"

#include "ImageDoc.h"
#include "FilmPhotoData.h"
#include "ImageStandStyle.h"
#include "SEMCTRL_Label.h"
#include "SemCtrl_Mouse.h"
#include "SemCtrlDlgEdit.h"
#include "DlgPrompt.h"
#include "DlgInput.h"

#include "DlgProgress.h"
#include "Algorithm_AutoBC.h"
#include "Algorithm_AutoFocus.h"
#include "Algorithm_AutoAstigmat.h"
#include "Algorithm_AutoFilament.h"
#include <mmsystem.h>		// system timer support

#include "LineTracker.h"	// 07.07---07.08加入任意方向标尺
#include "ProductVersionManager.h"
#include "StageXYManager.h"
#include "Preparation4Scan.h"
using namespace Prepare4Scan;
#include "Factory.h"
#include "CommonFunctionManager.h"
#include "CFGFileManager.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "GlobalParamSetting.h"
#include "AppOption.h"
#include "MCBControler.h"
#include "GraphBuffManager.h"
#include "ViewMCBControler.h"
#include "ScaleManager.h"
#include "CCPS3603tatusDetector.h"
#include "SpellmanStatusDetector.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "HVPowerDataManager.h"
#include "ScanParameterManager.h"
#include "MCICardCommonSetting.h"
#include "AutoFocusThread.h"
#include "ImageDataManager.h"
#include "TestParameterManager.h"
#include "BoardComm.h"
#include "Util.h"
#include "DlgAngleOptions.h"
#include "AutoContrastDlg.h"
#include "AppLogManager.h"
#include "../SEMPanel/PanelModeMgr.h"
#include "MPOData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//速度指示
#define	STATUS_SCAN_SPEED		0	//扫描速度
#define	STATUS_PHOTO_SPEED		1	//照相速度
//照相相关
#define	CAMERA_SNAP				1	//快照
#define	CAMERA_SIMULATE			2	//模拟照相
#define	CAMERA_DIGITAL			3	//数字照相
#define	CAMERA_PREVIEW			4	//照相预览
#define	CAMERA_SERNUM			5	//照相编号

#define CX_BORDER   1
#define CY_BORDER   1

extern BYTE g_cFilmPhotoData[];

// 2005-1-17_液晶屏幕XY比例问题
const COLORREF gc_crScreenColor = RGB(0, 0, 0);

const long HandleAreaW = 16;
const long HandleAreaH = 12;
const long HandleAreaW2 = 20;
const long HandleAreaH2 = 20;

// 09.11.18增加单击测试类型GraphHitTestNULL，总类型由11项变为12项
// 2023.07 增加角度测量，测试类型增加2项，总类型变为14项
HCURSOR g_hCursors[14];
extern BYTE g_cFilmPhotoData[];

// Special mapping modes just for CScanView implementation
#define MM_NONE             0
#define MM_SCALETOFIT       (-1)
// standard GDI mapping modes are > 0

//--Auto Functions---------------------------------------//
const UINT nAutoFunctionTimerID			= 100;
const UINT nAutoBCTimerElapse			= 500;

const UINT nAutoFocusTimerElapse		= 700; // ms
const UINT nAFTotalTime					= 45; // s

const UINT nAutoAstigmatTimerElapse		= 500; // ms
const UINT nASTotalTime					= 45;  // s
//--Auto Functions---------------------------------------//
//090327 ADD ZYX
#define DEBUG_Info_Sharpness
#define DEBUG_Info_ABC

const UINT nAutoTimerDate				= 110;

CSize GetImageSize()
{
	CSize szImage = CSemVirtualMCICard::Instance().GetResolution();
	return szImage;
}


// CScanView
IMPLEMENT_DYNCREATE(CScanView, CScrollView)

CScanView::CScanView()
{
//LMH//////////////////////////////////
	// Init everything to zero
	AFX_ZERO_INIT_OBJECT(CWnd);
	m_nMapMode = MM_NONE;

//	SetScrollSizes(MM_TEXT, CSize(0, 0));
	// 2005-1-17_液晶屏幕XY比例问题
	VERIFY(m_brHatch.CreateHatchBrush(HS_DIAGCROSS, RGB(191, 191, 191)));	// 09.12.02改回diagcross
	m_bmpBackground.LoadBitmap( MAKEINTRESOURCE(IDB_6K_BITMAP_Wallpaper) );

	g_hCursors[0] = LoadCursor(NULL, IDC_ARROW);
	g_hCursors[1] = LoadCursor(NULL, IDC_SIZEALL);
	g_hCursors[2] = LoadCursor(NULL, IDC_SIZEWE);
	g_hCursors[3] = LoadCursor(NULL, IDC_SIZEWE);
	g_hCursors[4] = LoadCursor(NULL, IDC_SIZENS);
	g_hCursors[5] = LoadCursor(NULL, IDC_SIZENS);
	g_hCursors[6] = LoadCursor(NULL, IDC_SIZENESW);
	g_hCursors[7] = LoadCursor(NULL, IDC_SIZENWSE);
	g_hCursors[8] = LoadCursor(NULL, IDC_SIZENESW);
	g_hCursors[9] = LoadCursor(NULL, IDC_SIZENWSE);
	g_hCursors[10] = LoadCursor(NULL, IDC_SIZENS);
	g_hCursors[11] = LoadCursor(NULL, IDC_SIZEALL);
	g_hCursors[12] = LoadCursor(NULL, IDC_SIZEALL);
	// 09.11.18增加单击测试类型GraphHitTestNULL
	g_hCursors[13] = LoadCursor(NULL, IDC_ARROW);

	m_bCatch = FALSE;

	m_bUScaleHitTest		= FALSE;
	m_bOscillographHitTest	= FALSE;
//LMH//////////////////////////////////

	m_nAdjustorSort			= SORT_OBJECTIVE_LENS;	//调节器类型为对比度调节
	m_nCondenserSort		= 0;					//聚光镜调节类型为粗调
	m_nObjectiveSort		= 0;					//物镜调节类型为粗调

	m_bAbc					= FALSE;	//不进行字符编辑
	m_bLabel				= TRUE;		//显示底标
	m_bUScale				= FALSE;	//隐藏活动标尺
	m_bDate					= FALSE;	//隐藏日期
	m_bNote					= FALSE;	//隐藏备注
	m_bFOV					= TRUE;
	m_bPixelSize			= TRUE;
	m_bWobbler				= FALSE;	//物镜摇摆关
	m_bLean					= FALSE;	//倾斜补偿关
	m_bDynFocus				= FALSE;	//动态聚焦关
	m_bSacp					= FALSE;	//通道花样关
	m_bRobinson				= FALSE;	//Robinson探头关
	m_bOscillograph			= FALSE;	//隐藏示波器
	m_bFrameRate			= FALSE;	//隐藏标准框

	m_bCondRev				= FALSE;	//聚光镜反转关
	m_bObjectiveRev			= FALSE;	//物镜反转关
	m_bAuxStig				= FALSE;	//辅助消像散关

	m_bPreview				= FALSE;	//非照相预览
	m_bZoomFit				= FALSE;
	m_bLive					= FALSE;	//快速扫描
	m_bSplit				= FALSE;	//非分割像
	m_bSplitZoom			= FALSE;	//分割像不放大显示
	m_bOnlyDet2				= FALSE;	//显示第一探测器图像

	m_bOpenImageAfterSnap	= FALSE;	//快照后不打开图像窗口

	m_boolScreenFull		= FALSE;

	m_bStopPhoto			= FALSE;	// 2004-5-12_照相编号自动增加

//--Auto Functions---------------------------------------//
	m_nAutoFunctionTimer	= 0;
	m_pAutoProgressDlg		= NULL;
	m_bImageBuff			= new BYTE[256*256];//bAutoBUFF;
//--Auto Functions---------------------------------------//

//	gpScanView = this;

	m_bShiftDown		= FALSE;
	m_bCtrlDown			= FALSE;
	// 07.04.26显示图像小样
	m_pSelItem			= NULL;
	m_plistImageItem	= new ListImageItem;
	m_bIsMultiSel		= FALSE;				//设置多文件选择标志08.06.27

	// 07.07---07.08加入任意方向标尺
	m_ptMobileUScale	= CPoint(100,400);
	m_ptMobileUScale2	= CPoint(400,400);

	// 16.06.12 多移动标尺实验
	for( int i=0; i<MobileUScaleCounts; i++ )
	{
//		m_nMobileUScalePixelNumArray[i]	= ;
		m_ptMobileUScaleArray[i]		= CPoint( -1, -1 );
		m_ptMobileUScaleArray2[i]		= CPoint( -1, -1 );
		m_bUScaleHitTestArray[i]		= FALSE;
	}
	m_nMobileUScaleCounts	= 0;
	// 16.06.12 多移动标尺实验

	// 2023.07 角度测量
	m_nAngleCounts		= 0;
	m_nAngleIndex		= 0;
	m_bAddAngle			= FALSE;
	m_bAngleHitTest		= FALSE;
	m_plistAngle		= new ListAngle;
	// 2023.07 角度测量

	m_bDebugAccel		= FALSE;				// 09.12.10 增加调试状态功能

	// 12.06.21 自动台
	m_bStageOrigin			= FALSE;
	m_bStageOriginHitTest	= FALSE;
	m_bStageTrack			= FALSE;
	m_ptStagePointParam.x	= CMCICardCommonSetting::Instance().m_sizeReso.cx / 2;
	m_ptStagePointParam.y	= CMCICardCommonSetting::Instance().m_sizeReso.cy / 2;
	m_ptStageTrackParam.x	= CMCICardCommonSetting::Instance().m_sizeReso.cx / 2;
	m_ptStageTrackParam.y	= CMCICardCommonSetting::Instance().m_sizeReso.cy / 2;

	// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
	m_bDebugCoadjust		= FALSE;
	m_bDrawCircle			= FALSE;

	// 15.09 远程功能
	m_commCmdThd		= NULL;
	m_commSendImageThd	= NULL;
	m_pcommServer		= NULL;
	m_bCommListenStarted= FALSE;
	m_bCommSendImgData	= FALSE;
}

CScanView::~CScanView()
{
//LMH//////////////////////////////////
	m_brHatch.DeleteObject();	// 2005-1-17_液晶屏幕XY比例问题
//LMH//////////////////////////////////

	delete [] m_bImageBuff;

	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer	= 0;
	}

	//07.04.26显示图像小样
	POSITION pos;
	while( (pos=m_plistImageItem->GetHeadPosition()) )
	{
		CImageItem* pItem = m_plistImageItem->GetAt( pos );
		m_plistImageItem->RemoveHead();
		delete pItem;
	}
	delete m_plistImageItem;

	// 2023.07 角度测量
	while( (pos = m_plistAngle->GetHeadPosition()) )
	{
		AngleMeasure* pItem = m_plistAngle->GetAt( pos );
		m_plistAngle->RemoveHead();
		delete pItem;
	}
	delete m_plistAngle;
}


BEGIN_MESSAGE_MAP(CScanView, CScrollView)
	//{{AFX_MSG_MAP(CScanView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_ADJUSTOR_CONDENSER_LENS_REVERSE, OnAdjustorCondenserLensReverse)
	ON_UPDATE_COMMAND_UI(ID_ADJUSTOR_CONDENSER_LENS_REVERSE, OnUpdateAdjustorCondenserLensReverse)
	ON_COMMAND(ID_ADJUSTOR_OBJECTIVE_LENS_REVERSE, OnAdjustorObjectiveLensReverse)
	ON_UPDATE_COMMAND_UI(ID_ADJUSTOR_OBJECTIVE_LENS_REVERSE, OnUpdateAdjustorObjectiveLensReverse)
	ON_COMMAND(ID_ADJUSTOR_STIGMATOR_ASSISTANT, OnStigmatorAssistant)
	ON_UPDATE_COMMAND_UI(ID_ADJUSTOR_STIGMATOR_ASSISTANT, OnUpdateStigmatorAssistant)
	ON_COMMAND(ID_PHOTO_SNAPSHOT, OnPhotoSnapshot)
	ON_COMMAND(ID_PHOTO_PREVIEW, OnPhotoPreview)
	ON_COMMAND(ID_AUTO_BRIGHT_CONTRAST, OnAutoBrightContrast)
	ON_COMMAND(ID_AUTO_FOCUS, OnAutoFocus)
	ON_COMMAND(ID_AUTO_Contrast, OnAutoContrast)  
	ON_COMMAND(ID_AUTO_STIGMATOR, OnAutoStigmator)
	ON_COMMAND(ID_AUTO_FILAMENT, OnAutoFilament)
	ON_COMMAND(ID_OTHERS_ABC, OnAbc)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_ABC, OnUpdateAbc)
	ON_COMMAND(ID_OTHERS_LABEL, OnLabel)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_LABEL, OnUpdateLabel)
	ON_COMMAND(ID_OTHERS_RULER, OnRuler)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_RULER, OnUpdateRuler)
	ON_COMMAND(ID_OTHERS_DATE, OnDate)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_DATE, OnUpdateDate)
	ON_COMMAND(ID_OTHERS_NOTE, OnNote)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_NOTE, OnUpdateNote)
	ON_COMMAND(ID_OTHERS_FOV, OnFOV)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_FOV, OnUpdateFOV)
	ON_COMMAND(ID_OTHERS_PixelSize, OnPixelSize)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_PixelSize, OnUpdatePixelSize)
	ON_COMMAND(ID_ACCESSORIES_VIDEO_PRO, OnVideoPro)
	ON_COMMAND(ID_ACCESSORIES_BSE_DET, OnRobinsonDetector)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_BSE_DET, OnUpdateRobinsonDetector)
	ON_COMMAND(ID_ACCESSORIES_WOBBLER, OnWobbler)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_WOBBLER, OnUpdateWobbler)
	ON_COMMAND(ID_ACCESSORIES_SACP, OnSACP)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_SACP, OnUpdateSACP)
	ON_COMMAND(ID_ACCESSORIES_LEAN_COMPENSATE, OnLeanCompensate)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_LEAN_COMPENSATE, OnUpdateLeanCompensate)
	ON_COMMAND(ID_ACCESSORIES_DYNAMIC_FOCUS, OnDynamicFocus)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_DYNAMIC_FOCUS, OnUpdateDynamicFocus)
	ON_COMMAND(ID_ACCESSORIES_OSCILLOGRAPH, OnOscillograph)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_OSCILLOGRAPH, OnUpdateOscillograph)
	ON_COMMAND(ID_ACCESSORIES_STAND_IMAGE, OnStandImage)
	ON_COMMAND(ID_OPTIONS_SETUP, OnOptionsSetup)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, OnUpdateViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, OnUpdateViewZoomOut)
	ON_COMMAND(ID_VIEW_ACTUAL_SIZE_SCAN, OnViewActualSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ACTUAL_SIZE_SCAN, OnUpdateViewActualSize)
	ON_COMMAND(ID_VIEW_ZOOM_FIT, OnViewZoomFit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_FIT, OnUpdateViewZoomFit)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_PHOTO_PREVIEW, OnUpdatePhotoPreview)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_ACCESSORIES_FRAME_RATE, OnFrameRate)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_FRAME_RATE, OnUpdateFrameRate)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_CHANGESTATE_DELETE, OnChangestateDelete)
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDC_BUTTON_Diagnose, OnBUTTONDiagnose)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_Diagnose, OnUpdateBUTTONDiagnose)
	ON_COMMAND(IDC_BUTTON_Diagnose_Setup, OnBUTTONDiagnoseSetup)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_Diagnose_Setup, OnUpdateBUTTONDiagnoseSetup)
	ON_WM_SETFOCUS()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_SCAN_TYPE_SPLIT, OnScanTypeSplit)
	ON_UPDATE_COMMAND_UI(ID_SCAN_TYPE_SPLIT, OnUpdateScanTypeSplit)
	ON_COMMAND(ID_SCAN_TYPE_SPLIT_ZOOM, OnScanTypeSplitZoom)
	ON_UPDATE_COMMAND_UI(ID_SCAN_TYPE_SPLIT_ZOOM, OnUpdateScanTypeSplitZoom)
	ON_COMMAND(ID_SCAN_TYPE_DET2, OnScanTypeDet2)
	ON_UPDATE_COMMAND_UI(ID_SCAN_TYPE_DET2, OnUpdateScanTypeDet2)
	ON_COMMAND(ID_Stage_Coordinate_origin, OnStageCoordinateorigin)
	ON_UPDATE_COMMAND_UI(ID_Stage_Coordinate_origin, OnUpdateStageCoordinateorigin)
	ON_COMMAND(ID_OPTIONS_ListenStart, OnOptionsListenStart)
	ON_COMMAND(ID_OPTIONS_ListenStop, OnOptionsListenStop)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ListenStart, OnUpdateOptionsListenStart)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ListenStop, OnUpdateOptionsListenStop)
	ON_COMMAND(ID_SCAN_TYPE_LIVE, OnScanTypeLive)
	ON_UPDATE_COMMAND_UI(ID_SCAN_TYPE_LIVE, OnUpdateScanTypeLive)
	ON_COMMAND(ID_OTHERS_RULER_Add, OnRulerAdd)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_RULER_Add, OnUpdateRulerAdd)
	ON_COMMAND(ID_OTHERS_ANGLE, OnAngle)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_ANGLE, OnUpdateAngle)
	ON_COMMAND(ID_OTHERS_ANGLE_Add, OnAngleAdd)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_ANGLE_Add, OnUpdateAngleAdd)
	ON_COMMAND(ID_OTHERS_ANGLE_Options, OnAngleOptions)
	ON_COMMAND(ID_OTHERS_ANGLE_Delete, OnAngleDelete)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_ADJUSTOR_BEAM_ALIGNMENT, ID_ADJUSTOR_STIGMATOR, OnAdjustorSortChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ADJUSTOR_BEAM_ALIGNMENT, ID_ADJUSTOR_STIGMATOR, OnUpdateAdjustorSortChanged)
	ON_COMMAND_RANGE(ID_ADJUSTOR_MANUAL_ObjectiveLens, ID_ADJUSTOR_MANUAL_NULL, OnAdjustorManualSortChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ADJUSTOR_MANUAL_ObjectiveLens, ID_ADJUSTOR_MANUAL_NULL, OnUpdateAdjustorManualSortChanged)
	ON_COMMAND_RANGE(ID_SCAN_TYPE_FREEZE, ID_SCAN_TYPE_CAMERA_SIMULATE, OnScanTypeChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCAN_TYPE_FREEZE, ID_SCAN_TYPE_CAMERA_SIMULATE, OnUpdateScanTypeChanged)
	ON_COMMAND_RANGE(ID_SCAN_SPEED_0, ID_SCAN_SPEED_9, OnScanSpeedChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCAN_SPEED_0, ID_SCAN_SPEED_9, OnUpdateScanSpeedChanged)
	ON_COMMAND_RANGE(ID_SCAN_RESOLUTION_1, ID_SCAN_RESOLUTION_9, OnScanResolutionChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCAN_RESOLUTION_1, ID_SCAN_RESOLUTION_9, OnUpdateScanResolutionChanged)
	ON_COMMAND_RANGE(ID_SCAN_STORE_ZZZ, ID_SCAN_STORE_XYZ, OnScanStoreModeChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCAN_STORE_ZZZ, ID_SCAN_STORE_XYZ, OnUpdateScanStoreModeChanged)

	ON_MESSAGE(WM_PARAM_CHANGED, OnParamChanged)
	ON_MESSAGE(WM_USER_VAC_MSG, OnSMvacChanged)
	ON_MESSAGE(WM_USER_KYKYComm_Recv, OnKYKYComm_ReplyAsk)
	ON_MESSAGE(WM_USER_KYKYComm_Thread_Send, OnKYKYComm_Connect)
	ON_MESSAGE(WM_USER_KYKYComm_Thread_Send_Connect, OnKYKYComm_ConnectThread)
	ON_MESSAGE(WM_USER_Stop_AutoFocus, OnStopAutoFocus)
	ON_MESSAGE(WM_USER_Start_AutoFocus, OnStartAutoFocus)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanView drawing

void CScanView::OnDraw(CDC* pDC)
{
	// 本次修改后代码
	//-------------------------------------------------
	CRect rcBounds;
	GetClientRect( (LPRECT)rcBounds );
}

void CScanView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 )
	{
		CPoint pt1 = m_ptStagePointParam;
		CPoint pt2 = ScreenToGraph( point );
		CPoint pt3 = ScreenToGraph( pt1 );
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
		{
			StageTMCM_MoveToCenter( pt1.x, pt1.y, pt2.x, pt2.y );
		}
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3  && StageMC6600_IsReady() )
		{
			StageMC6600_MoveToCenter( pt1.x, pt1.y, pt2.x, pt2.y );
		}
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1  && StageMC600_IsReady() )
		{
			StageMC600_MoveToCenter( pt1.x, pt1.y, pt2.x, pt2.y );
		}
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )	// && m_bStageOrigin )
		{
			// 像素距离
			int nLengthX = abs( pt1.x - pt2.x );
			int nLengthY = abs( pt1.y - pt2.y );
			// 实际步数
			int nScaleX = 0;
			int nScaleY = 0;
			if( CStageXYManager::Instance().m_Stage.xy.bRev )
			{
				CalculateStageUScale( FALSE, (pt1.x<pt2.x) ? TRUE : FALSE, nLengthX, nScaleX );
				CalculateStageUScale( TRUE, (pt1.y<pt2.y) ? TRUE : FALSE, nLengthY, nScaleY );
			}
			else
			{
				CalculateStageUScale( TRUE, (pt1.x<pt2.x) ? TRUE : FALSE, nLengthX, nScaleX );
				CalculateStageUScale( FALSE, (pt1.y<pt2.y) ? TRUE : FALSE, nLengthY, nScaleY );
			}
			
			char szBuffer[64];
			CString str;
			str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
			CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
//			str.Format( "SHXY;PR %ld,%ld;BGXY", (pt1.y<pt2.y) ? -nScaleY : nScaleY,
//												(pt1.x<pt2.x) ? nScaleX : -nScaleX );
//			CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			CStageXYManager::Instance().Stage_XY_Command2( "PR", nScaleX, nScaleY, (pt1.x<pt2.x) ? FALSE : TRUE, (pt1.y<pt2.y) ? FALSE : TRUE, TRUE );
			CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
			if( pMainFrame && (pMainFrame->m_dlgStage.m_hWnd != NULL))
				pMainFrame->m_dlgStage.SetTimer( 1, nAutoStageXY_FIElapse, NULL );
		}

		return;
	}
	//获得被选中的项目指针
	m_pSelItem = GetSelImageItem( point );
	if( m_pSelItem )
	{
		CString strName = m_pSelItem->m_strFileName;
		int nPos = strName.Find( '\\' );
		while( nPos != -1 )
		{
			strName = strName.Right( strName.GetLength() - nPos - 1 );
			nPos = strName.Find( '\\' );
		}
//		((CMainFrame *)theApp.GetMainWnd())->ActivateMDIChild( strName );
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CScanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	char szBuf[255];
	sprintf( szBuf, "OnKeyDown: nChar = %d, nRepCnt = %d, nFlags = %d\n", nChar, nRepCnt, nFlags );
//	TRACE( szBuf );
	switch( nChar )
	{
	case	VK_SHIFT:			// 按下Shift键，加四方向键，可移动自动台
		{
			m_bShiftDown = TRUE;
			char szBuffer[64];
			CString str;
			str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
			CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		}
		break;
	case	VK_CONTROL:			// 是否按下了Ctrl键，用于多个items的选择08.06.27
		m_bCtrlDown = TRUE;
		break;
	case	VK_MENU:			// 说明按下了Ctrl+Alt组合键，如：Ctrl+Alt+C, Ctrl+Alt+D, Ctrl+Alt+T, Ctrl+Alt+R
		m_bCtrlDown = FALSE;
		TRACE( "OnKeyDown: char = VK_MENU\n" );
		break;
	case	VK_ESCAPE:			// 按下ESC，停止自动台移动
		{
			AutoStageXY_FI_Finish( FALSE );
			CStageXYManager::Instance().m_DMCWin.Command( "ST" );
		}
		break;
	}

	if( m_bShiftDown && CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )
	{
		// 12.06.21 自动台：Shift+四个方向键
		ManuallyAdjustStage( nChar );
	}
	else if( m_bDebugAccel )	// 09.12.10 增加调试状态功能
	{
		// 调试状态下，四个方向键用于消像散的调节
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
		VARIANT varMinX, varMaxX, varMinY, varMaxY;

		CSemCoreAssemble::Instance().m_StigmatorX.GetRange( varMinX, varMaxX );
		CSemCoreAssemble::Instance().m_StigmatorY.GetRange( varMinY, varMaxY );
		ClassID.lVal		= SemControlPanelClass_Stigmator;
		SerialNumber.lVal	= -1;

		int nKey = -1;
		switch( nChar )
		{
		case	VK_LEFT:
			Xpos.lVal = CSemCoreAssemble::Instance().m_StigmatorX.GetPos().lVal;
			Xpos.lVal -= 1;
			Flag.lVal = 1;
			nKey = 0;
			break;
		case	VK_RIGHT:
			Xpos.lVal = CSemCoreAssemble::Instance().m_StigmatorX.GetPos().lVal;
			Xpos.lVal += 1;
			Flag.lVal = 1;
			nKey = 1;
			break;
		case	VK_UP:
			Ypos.lVal = CSemCoreAssemble::Instance().m_StigmatorY.GetPos().lVal;
			Ypos.lVal += 1;
			Flag.lVal = 2;
			nKey = 2;
			break;
		case	VK_DOWN:
			Ypos.lVal = CSemCoreAssemble::Instance().m_StigmatorY.GetPos().lVal;
			Ypos.lVal -= 1;
			Flag.lVal = 2;
			nKey = 3;
			break;
		case	192:
			nKey = 10;
			break;
		}

		if( nKey < 10 && nKey > 0 )
		{
			if( Xpos.lVal < varMinX.lVal )
				Xpos.lVal = varMinX.lVal;
			if( Xpos.lVal > varMaxX.lVal )
				Xpos.lVal = varMaxX.lVal;
			if( Ypos.lVal < varMinY.lVal )
				Ypos.lVal = varMinY.lVal;
			if( Ypos.lVal > varMaxY.lVal )
				Ypos.lVal = varMaxY.lVal;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		else if( nKey > 9 )
			SyncCondenserLensReverse( !m_bCondRev );
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CScanView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch( nChar )
	{
	case	VK_SHIFT:
		m_bShiftDown = FALSE;
		break;
	case	VK_CONTROL:		// 是否抬起了Ctrl键，用于多个items的选择08.06.27
		m_bCtrlDown = FALSE;
		break;
	}

	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CScanView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( bActivate )
		UpdateItemRect( FALSE );
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//关闭当前被选择的图像文件08.06.27
void CScanView::OnChangestateDelete() 
{
	// TODO: Add your command handler code here
	//获得项目列表头位置
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		//获取项目指针
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		//若只有一个项目被选中，且就是该项目
		if( m_pSelItem && !m_bIsMultiSel && (m_pSelItem == pitem) )
		{
			CString strName = pitem->m_strFileName;
			int nPos = strName.Find( '\\' );
			while( nPos != -1 )
			{
				strName = strName.Right( strName.GetLength() - nPos - 1 );
				nPos = strName.Find( '\\' );
			}
//			((CMainFrame *)theApp.GetMainWnd())->DestroyMDIChild( strName );
			//设置被选中项目指针为空
			m_pSelItem = NULL;
			//还原多文件选择标志
			m_bIsMultiSel = FALSE;
			Invalidate( FALSE );
			return;
		}
		else if( m_bIsMultiSel && pitem->m_bSel )
		{//若选择了多个项目，关闭每一个项目
			CString strName = pitem->m_strFileName;
			int nPos = strName.Find( '\\' );
			while( nPos != -1 )
			{
				strName = strName.Right( strName.GetLength() - nPos - 1 );
				nPos = strName.Find( '\\' );
			}
		}
	}
	//还原多文件选择标志
	m_bIsMultiSel = FALSE;
	Invalidate( FALSE );
}

void CScanView::UpdateItemRect( BOOL bPrompt )
{
	POSITION pos = m_plistImageItem->GetHeadPosition();
	if( !pos )
		return;

	//获取客户窗口大小
	CRect rect, rcClient;
	GetClientRect( &rcClient );
//	CPaintDC dc(this); // device context for painting
	if( m_szTotalLog.cx == 0 )
		rect.left = rcClient.Height()/3*4 + 10;
	else
		rect.left	= m_szTotalLog.cx + 4;
	rect.right	= rcClient.Width() - GetSystemMetrics( SM_CXVSCROLL );
	rect.top	= rcClient.top;
	rect.bottom	= rcClient.bottom;
	if( rect.Width() <= 0 || rect.Height() <= 0 )
	{
		while( pos )
		{
			CImageItem* pitem = m_plistImageItem->GetNext( pos );
			CRect rc = CRect( 0, 0, 0, 0 );
			pitem->m_rect = rc;
		}
		return;
	}
	
	CSize sz;
	int count = m_plistImageItem->GetCount();
	if( count <= 9 )
		sz.cx = 2;
	else if( count>9 && count<=20 )
		sz.cx = 3;
	else if( count>20 && count<=30 )
		sz.cx = 4;
	else if( count>30 && count<=42 )
		sz.cx = 5;
	sz.cy = ((double)count/sz.cx > count/sz.cx) ? count/sz.cx+1 : count/sz.cx;
	if( sz.cy < 4 )
		sz.cy = 4;

	int nWidth = rect.Width() / sz.cx;
	int nHeight = rect.Height() / sz.cy;
	if( nWidth/4*3 < nHeight )
		nHeight = nWidth/4*3;
	else if( nWidth/4*3 > nHeight )
		nWidth = nHeight/3*4;

	int x = rect.left;
	int y = rect.top;
	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		//调整画图区域
		if( x+nWidth > rect.right )
		{
			x = rect.left;
			y += nHeight;//(nHeight+4);
		}
		CRect rc = CRect( x, y, x+nWidth-4, y+nHeight-4 );//y+nHeight-4 );
		pitem->m_rect = rc;
		x += nWidth;
	}
	InvalidateRect( NULL );
	if( bPrompt && count > 16 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = GetResString( IDS_SD_PROMPT_TOOMANY_IMAGE );
		dlg.DoModal();
	}
}

//获得被选中的项目指针
CImageItem* CScanView::GetSelImageItem(CPoint &point)
{
	//获得单击点的实际位置
	CPoint offpoint = point;
	offpoint.Offset( GetScrollPos( SB_HORZ ), GetScrollPos( SB_VERT ));

	CImageItem* pSelItem = NULL;
	//遍历项目列表中的每一个项目，若单击点在某一项目的矩形区内，返回该项目；
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		if( pitem->m_rect.PtInRect( offpoint ) )
		{
//			pitem->m_bSel = TRUE;
			pSelItem = pitem;//return pitem;
		}
	}
	//若无项目被选中，返回空指针
	return pSelItem;
}

void CScanView::DeleteItem( CString strTitle )
{
	//获得项目列表头位置
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		//保存老位置
		POSITION oldpos = pos;
		//获取项目指针
		CImageItem* pitem = m_plistImageItem->GetNext( pos );

		CString strName = pitem->m_strFileName;
		int nPos = strName.Find( '\\' );
		while( nPos != -1 )
		{
			strName = strName.Right( strName.GetLength() - nPos - 1 );
			nPos = strName.Find( '\\' );
		}

		if( strName == strTitle )
		{
			//如果有同名临时文件，删除之
			pitem->DeleteTempFile( strTitle );
			//从项目列表中删除该项目
			m_plistImageItem->RemoveAt( oldpos );
			delete pitem;
			//设置被选中项目指针为空
			m_pSelItem = NULL;
			UpdateItemRect( FALSE );
			Invalidate( FALSE );
			return;
		}
	}
}

void CScanView::ChangeItemName( CString strOldName, CString strNewName )
{
	//遍历项目列表中的每一个项目，若单击点在某一项目的矩形区内，返回该项目；
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );

		CString strName = pitem->m_strFileName;
		int nPos = strName.Find( '\\' );
		while( nPos != -1 )
		{
			strName = strName.Right( strName.GetLength() - nPos - 1 );
			nPos = strName.Find( '\\' );
		}

		if( strName == strOldName )
		{
			pitem->m_strFileName = strNewName;
			return;
		}
	}
}

// CScanView diagnostics

#ifdef _DEBUG
void CScanView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CScanView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CScanDoc* CScanView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanDoc)));
	return (CScanDoc*)m_pDocument;
}
#endif //_DEBUG


// CScanView message handlers

BOOL CScanView::PreCreateWindow(CREATESTRUCT& cs) 
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

	return CScrollView::PreCreateWindow(cs);
}

int CScanView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Init SEM Control
//	if ( !CSemCtrl::Instance().Init(&(theApp.m_ImageCard)) )
	int nReady = CSemCtrl::Instance().USB_Init();
	if( nReady > -1 )
	{
		_DisplayMsgOnViewCreate(nReady);
	}

	CSemCtrl::Instance().m_SemScan.Bind( g_cFilmPhotoData );

	CMainFrame* frame = (CMainFrame *)theApp.GetMainWnd();
	///////////////////////////////////
	// MCB
	/* 1.使用单独线程处理MCB消息，主程序空闲时不断查询+定时查询
//	BOOL bMCBReady = CSemCtrl::Instance().MCB_Init();
//	frame->SetTimers( nMCB_SemMessageTimerID, bMCBReady );
	/* 1. End*/

	// 2. 直接在主线程ScanView中处理MCB消息
	// App::AutoInitSemCtrl之后再启动定时器
	//CViewMCBControler::Instance().g_strMCB_DeviceName = CViewMCBControler::Instance().MCB_OpenHIDDevice();
	//CViewMCBControler::Instance().MCB_InitHIDDevice( CViewMCBControler::Instance().g_strMCB_DeviceName );
	//nMCB_Timer				= 0;
//	nMCB_SemMessageTimer = SetTimer( nMCB_TimerID, nMCB_TimerElapse, NULL );
	/* 2. End*/

	/* 3. 在主程序中使用单独线程处理MCB消息*/
	// 见App::AutoInitSemCtrl，所有参数初始化后，启动专门线程
	/* 3. End*/
	///////////////////////////////////

	// Init ZOOM
//	SetScrollSizes( MM_TEXT, g_sizeUSB_RESO_2048_2048 );
//	m_szTotalLog = g_sizeUSB_RESO_2048_2048;
	SetScrollSizes( MM_TEXT, CMCICardCommonSetting::Instance().m_sizeReso );
	m_szTotalLog = CMCICardCommonSetting::Instance().m_sizeReso;

	// 读取各参数
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\KYKYcfg.ini");

	// 读颜色值
	CCFGFileManager::Instance().InitializeColor(path);
	CCFGFileManager::Instance().InitializeLineWidth(path);

	// 读字体参数
	CCFGFileManager::Instance().InitializeFontName(path);
	CCFGFileManager::Instance().InitializeFontSize(path);

	CCFGFileManager::Instance().InitializeShowParameter(path);

	m_bDate = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showDate];
	m_bNote = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showNote];
	m_bFOV = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showFOV];
	m_bPixelSize = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showPixeSize];
	m_AngleOption.clrLine	= CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleLine];
	m_AngleOption.clrArc	= CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleArc];
	m_AngleOption.clrText	= CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleText];
	m_AngleOption.nSize		= 12;

	_InitLogoDisp(path);

	CCFGFileManager::Instance().InitializeFooter(path);

	// 19.06.19 扫描区域显示像素
	char szBuf[20] = {0};
	if(GetPrivateProfileString( "Hardware", "DispP", "1024", szBuf, 10, path) != 0 )
		CScanParameterSaver::Instance().m_nDispPixel = atoi(szBuf);

	CScanParameterManager::Instance().InitScanParam();

	CSemVirtualMCICard::Instance().InitializeImageCard(CMCICardCommonSetting::Instance().m_sizeReso, this, m_bLabel,theApp.m_strLogoDisp);
	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
	{
		// VAC State Machine
		//CSemCtrl::Instance().VAC_Init();
	}

	int nMaxPix = 0;
	int nDiv = 0;
	ZeroMemory(szBuf,20);
	if(GetPrivateProfileString( "Hardware", "nMaxPix", "2559", szBuf, 20, path) != 0 )
		nMaxPix = atoi(szBuf);
	CSemVirtualMCICard::Instance().SetParams(15,nMaxPix);

	ZeroMemory(szBuf,20);
	if(GetPrivateProfileString( "Hardware", "nDiv", "10", szBuf, 20, path) != 0 )
		nDiv = atoi(szBuf);
	CSemVirtualMCICard::Instance().SetParams(16,nDiv);

	CSemCtrl::Instance().m_SemScan.USB_SetParams( SET_PARAMS_NeedMon, 0 );

	CGraphBuffMgr::Instance().AllocateBuff();
	return 0;
}

void CScanView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: calculate the total size of this view
	m_szTotalLog.cx = m_szTotalLog.cy = 100;
	SetScrollSizes(MM_TEXT, m_szTotalLog);

	SemLabel_Init( CMCICardCommonSetting::Instance().m_sizeReso.cx );
//	USB_ChangeScan( USB_ST_RasterScan );
	UpdateWindowPos();
//	OnViewZoomFit();
}

void CScanView::Localize(void)
{
	LocalizeMenu();
}

void CScanView::LocalizeMenu(void)
{
	m_wLanguageID = CGlobalParamSetting::Instance().m_wLanguageID;

	// 获取menu.mu的路径
	TCHAR path[255];
	::GetModuleFileName(0, path, 255);
	LPTSTR pszFileName = _tcsrchr(path, '\\') + 1;
	*pszFileName = '\0';
	strcat_s( path, g_csMenuFileName );

	TCHAR szBuf[20];

	// 获取根菜单数目
	TCHAR szBufRootSection[20];
	int nMenuRootCount = 0;
	sprintf_s(szBufRootSection, "Menu_Scan_Root");

	if(GetPrivateProfileString(szBufRootSection, "Count", "0", szBuf, 255, path) != 0)
		nMenuRootCount = atoi(szBuf);
	else
		return;
	if( nMenuRootCount == 0 )
		return;

	// 分配菜单资源
	CMenu menuNew;
	menuNew.CreateMenu();
	CMenu* pmenu = new CMenu[nMenuRootCount];
	for(int i=0; i<nMenuRootCount; i++ )
		VERIFY( pmenu[i].CreatePopupMenu() );

	// 获取根菜单IDS
	CStringArray strRoot;
	strRoot.SetSize( nMenuRootCount, 20 );
	TCHAR szBufRootKey[20] = {0};
	for(int i=0; i<nMenuRootCount; i++ )
	{
		sprintf_s(szBufRootKey, "menu%d", i);
		if(GetPrivateProfileString(szBufRootSection, szBufRootKey, "0", szBuf, 255, path) != 0)
			strRoot[i].Format( "%s", szBuf );
	}

	// 依次获取子菜单
	int nMenuSubCount = 0;
	UINT nID = 0;
	for(int i=0; i<nMenuRootCount; i++ )
		theApp.MyAppendMenu( &(pmenu[i]), atoi(strRoot[i]), path );

	CString str;
	for(int i=0; i<nMenuRootCount; i++ )
	{
		nID = atoi( strRoot[i] );
		menuNew.AppendMenu(MF_POPUP, (UINT) pmenu[i].m_hMenu, GetResString(nID));
	}

	CScanDoc* pdoc = GetDocument();
	pdoc->m_DefaultMenu = menuNew.GetSafeHmenu();
	if (pdoc->m_DefaultMenu == NULL)
		return;
/*	CMDIFrameWnd* frame = ((CMDIChildWnd *) GetParent())->GetMDIFrame();
	CMenu* wmenu = CMenu::FromHandle(frame->GetWindowMenuPopup((CMenu::FromHandle(pdoc->m_DefaultMenu))->GetSafeHmenu()));
	frame->MDISetMenu(CMenu::FromHandle(pdoc->m_DefaultMenu), wmenu);
	frame->DrawMenuBar();
*/
	CMainFrame* frame = (CMainFrame *)theApp.GetMainWnd();
	frame->SetMenu( CMenu::FromHandle(pdoc->m_DefaultMenu) );
	frame->DrawMenuBar();

	// delete and detach
	for(int i=0; i<nMenuRootCount; i++ )
		pmenu[i].Detach();
	delete [] pmenu;
	menuNew.Detach();
}


BOOL CScanView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect( &rc );
	pDC->FillRect( rc, &CBrush( RGB(0,0,0) ) );
	return TRUE;//CScrollView::OnEraseBkgnd(pDC);
}

void CScanView::OnDestroy() 
{
	// 15.09 远程功能
	if( m_bCommListenStarted )
		OnOptionsListenStop();
	///////////////////////////////////

	USB_ChangeScan( USB_ST_Stop );
	theApp.DisplayMsg("ImageCard Close");
	CSemVirtualMCICard::Instance().Close();

	// MCB
	CViewMCBControler::Instance().MCB_CloseHIDDevice();
	if( nMCB_Timer != 0 )
	{
		KillTimer( nMCB_Timer );
		nMCB_Timer = 0;
	}

	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
//	CSemCtrl::Instance().Close();	// 退出时重复调用，屏蔽
}

void CScanView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptPosSave				= ScreenToGraph( point );

	//鼠标在ScanView 上任意地方点击时，恢复原有的聚焦模式
	CPanelModeMgr::Instance().RestoreFocusMode();

	// 2023.07 角度测量
	if( m_bAngle )
	{
		int nIndex = 0;
		POSITION pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->nAngleStatus == 1 )
			{
				// 有新生成的
				GetAnglePoints( pAngle, m_ptPosSave );
				CSemVirtualMCICard::Instance().SetAngleParam( pAngle->nIndex, pAngle );
				break;
			}
			nIndex++;
			m_plistAngle->GetNext( pos );
		}
		if( nIndex != m_nAngleCounts )
			return;

		pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->bAngleHitTest )
			{
				// 有被选中的已有角度
				int nGraphHitTest = USB_GraphHitTest( m_ptPosSave );
				switch( nGraphHitTest )
				{
				case	GraphHitTest_W:
					pAngle->bStart = FALSE;
					break;
				case	GraphHitTest_E:
					pAngle->bEnd = FALSE;
					break;
				case	GraphHitTest_C:
					pAngle->bCenter = FALSE;
					break;
				case	GraphHitTest_L:
					pAngle->ptOriginal = m_ptPosSave;
					break;
				case	GraphHitTest_In:
					pAngle->ptOriginal = m_ptPosSave;
					break;
				}
				if( nGraphHitTest != GraphHitTest_NULL )
				{
					pAngle->nAngleStatus = 2;		// 修改
					m_nGraphHitTestSave = nGraphHitTest;
					m_bCatch = TRUE;
				}
				if( nGraphHitTest == GraphHitTest_L )
					pAngle->nAngleStatus = 4;		// 标注
				else if( nGraphHitTest == GraphHitTest_In )
					pAngle->nAngleStatus = 3;		// 整体移位
				break;
			}
			m_plistAngle->GetNext( pos );
		}
		if( m_bAngleHitTest )
			return;
	}
	// 2023.07 角度测量

	m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );

	if ( m_bAbc != TRUE )
	{
		if ( m_nGraphHitTestSave == GraphHitTest_Normal )
		{
			switch( m_nAdjustorSort )				//根据调节器类型设置光标
			{
			case	SORT_OBJECTIVE_LENS:
				SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_OBJECTIVE_LENS ) ) );
				break;
			case	SORT_STIGMATOR:
				SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_STIGMATOR ) ) );
				break;
			default:
				SetCursor( g_hCursors[0] );//AfxGetApp()->LoadCursor( IDC_ARROW ) );
				break;
			}
			m_rcDragRectLast = CRect(0,0,0,0);
			SetCapture();

			SemCtrlMouse_Init( m_ptPosSave, m_nAdjustorSort );
		}
		if( m_nGraphHitTestSave != GraphHitTest_NULL )
			m_bCatch = TRUE;
	}

/****************************************************************************
*	08.12.18  对于一般情况(单击鼠标)，或按下Ctrl键后单击，来选择文件的情况  *
*****************************************************************************/
	//获得被选中的项目指针
	m_pSelItem = GetSelImageItem( point );
	//若当前没有被选中的单个项目，且并非处于多文件选择状态，则返回
	if( !m_pSelItem && !m_bIsMultiSel )
	{
		//09.11.18遍历项目列表中的每一个项目，将所有item都设为非选中显示
		CDC* pDC = GetDC();
		POSITION pos = m_plistImageItem->GetHeadPosition();
		while( pos )
		{
			CImageItem* pitem = m_plistImageItem->GetNext( pos );
			if( pitem->m_bSel == TRUE )
			{
				pitem->m_bSel = FALSE;
				pitem->Draw( pDC, pitem->m_rect, pitem->m_bSel );
			}
		}
		ReleaseDC( pDC );

		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	CDC* pDC = GetDC();	//08.12.18
	//获得单击点的实际位置
	CPoint offpoint = point;
	offpoint.Offset( GetScrollPos( SB_HORZ ), GetScrollPos( SB_VERT ));
	//还原被选中项目指针和多文件选择标志
	m_pSelItem = NULL;
	m_bIsMultiSel = FALSE;
	//遍历项目列表中的每一个项目
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		if( pitem->m_rect.PtInRect( offpoint ) )
		{
			//对于一般情况，设置项目的被选中标志、文档中的被选中项目指针08.06.27
			if( !m_bCtrlDown )
			{
				if( !pitem->m_bSel )
				{
					pitem->m_bSel = TRUE;
					pitem->Draw( pDC, pitem->m_rect, pitem->m_bSel );//08.12.18
				}
				m_pSelItem = pitem;
			}
			//对于按下Ctrl的情况
			else
			{
				pitem->m_bSel = !(pitem->m_bSel);//可在选中与非选中之间切换
				pitem->Draw( pDC, pitem->m_rect, pitem->m_bSel );//08.12.18
			}
			//若项目被选中，设置多文件选择标志08.06.27
			if( pitem->m_bSel && m_bCtrlDown )
				m_bIsMultiSel = TRUE;
		}
		else//08.12.18
		{
			//对于一般情况
			if( !m_bCtrlDown && pitem->m_bSel )
			{
				pitem->m_bSel = FALSE;
				pitem->Draw( pDC, pitem->m_rect, pitem->m_bSel );
			}
		}
	}
	ReleaseDC( pDC );//08.12.18

	CScrollView::OnLButtonDown(nFlags, point);
}

void CScanView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptPosSave				= ScreenToGraph( point );
	m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );

	int i, j;
	// 16.06.12 多移动标尺实验
	// 右击则删除选中测量线
	for( i=0; i<MobileUScaleCounts; i++ )
	{
		if( m_bUScaleHitTestArray[i] )
			break;
	}
	if( i != MobileUScaleCounts )
	{
		m_bUScaleHitTestArray[i]	= FALSE;
		CString csMobileUScaleText;
		// 移动数组
		for( j=i; j<MobileUScaleCounts -1; j++ )
		{
			m_ptMobileUScaleArray[j]		= m_ptMobileUScaleArray[j+1];
			m_ptMobileUScaleArray2[j]		= m_ptMobileUScaleArray2[j+1];
			m_nMobileUScalePixelNumArray[j]	= m_nMobileUScalePixelNumArray[j+1];
			CSemVirtualMCICard::Instance().SetMobileUScalePosArray( j, m_ptMobileUScaleArray[j], m_ptMobileUScaleArray2[j], RGB(255, 0, 0) );
			csMobileUScaleText = CSemVirtualMCICard::Instance().CalculateMobileUScale( m_nMobileUScalePixelNumArray[j] );
			CSemVirtualMCICard::Instance().UpdateUScaleArray(j, m_nMobileUScalePixelNumArray[j], csMobileUScaleText );
		}
		// 数组最后一个值复位
		m_ptMobileUScaleArray[MobileUScaleCounts -1]		= CPoint(-1, -1);
		m_ptMobileUScaleArray2[MobileUScaleCounts -1]		= CPoint(-1, -1);
		m_nMobileUScalePixelNumArray[MobileUScaleCounts -1]	= 0;
		CSemVirtualMCICard::Instance().SetMobileUScalePosArray( MobileUScaleCounts -1, m_ptMobileUScaleArray[MobileUScaleCounts -1], m_ptMobileUScaleArray2[MobileUScaleCounts -1], RGB(255, 0, 0) );
		csMobileUScaleText = CSemVirtualMCICard::Instance().CalculateMobileUScale( m_nMobileUScalePixelNumArray[MobileUScaleCounts -1] );
		CSemVirtualMCICard::Instance().UpdateUScaleArray(MobileUScaleCounts -1, m_nMobileUScalePixelNumArray[MobileUScaleCounts -1], csMobileUScaleText );
		// 更新数量
		m_nMobileUScaleCounts--;
		if( m_nMobileUScaleCounts < 0 )
			m_nMobileUScaleCounts = 0;
	}
	// 16.06.12 多移动标尺实验

	// 2023.07 角度测量
	if( m_bAngleHitTest )
	{
		POSITION pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->bAngleHitTest )
			{
				CMenu menuManual;
				// Create a new menu for the application window.
				VERIFY(menuManual.CreatePopupMenu());
				menuManual.AppendMenu(MF_STRING, ID_OTHERS_ANGLE_Options, (LPCTSTR)GetResString(IDS_MENUITEM_OTHERS_ANGLE_Options));
				menuManual.AppendMenu(MF_STRING, ID_OTHERS_ANGLE_Delete, (LPCTSTR)GetResString(IDS_MENUITEM_OTHERS_ANGLE_Delete));

				CPoint temppoint = point;
				ClientToScreen( &temppoint );
				menuManual.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
					temppoint.x, temppoint.y, this, NULL);
				return;
			}
			m_plistAngle->GetNext( pos );
		}
		m_nAngleCounts = m_plistAngle->GetCount();
	}
	// 2023.07 角度测量

	//======2004-5-13_屏幕鼠标控制电镜=====================
	//SemCtrlMouse_Init
	if( m_bAbc != TRUE )
	{
		if( m_nGraphHitTestSave == GraphHitTest_Normal )
		{
			SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_STIGMATOR ) ) );
			m_rcDragRectLast = CRect(0,0,0,0);
			SetCapture();

			SemCtrlMouse_Init( m_ptPosSave, SORT_STIGMATOR );
		}
		if( m_nGraphHitTestSave != GraphHitTest_NULL )
			m_bCatch = TRUE;
	}
}

CPoint CScanView::ScreenToGraph( const CPoint point )
{
	CPoint ptPos = point;

	// Get Logic Pos (Screen)
	ptPos.x += GetScrollPos( SB_HORZ );
	ptPos.y += GetScrollPos( SB_VERT );

	// ZOOM to Graph (Graph)
	double dScale = CSemVirtualMCICard::Instance().GetZoom();
	if ( dScale > 0 )
	{
		ptPos.x = (int)(ptPos.x / dScale + 0.5);
		ptPos.y = (int)(ptPos.y / dScale + 0.5);
	}
	else
	{
		ptPos.x = (int)(ptPos.x / CScaleManager::Instance().GetCurScale() + 0.5);
		ptPos.y = (int)(ptPos.y / CScaleManager::Instance().GetCurScale() + 0.5);
	}

	return ptPos;
}

UINT CScanView::USB_GraphHitTest( const CPoint ptGraphPos )
{
	CRect	rcFrame;
	CRect	rcFrame2;
	CRect	rcFrame3;

	BOOL	boolLeft	= FALSE;
	BOOL	boolRight	= FALSE;
	BOOL	boolUp		= FALSE;
	BOOL	boolDown	= FALSE;

	if ( m_bOscillograph )
	{
		CSemVirtualMCICard::Instance().GetOscillographParam( m_ptOscillograph );

		rcFrame3.left	= m_ptOscillograph.x;
		rcFrame3.right	= m_ptOscillograph.x + g_szUSB_FrameOscillograph.cx;
		rcFrame3.top	= m_ptOscillograph.y;
		rcFrame3.bottom	= m_ptOscillograph.y + g_szUSB_FrameOscillograph.cy;

		if ( rcFrame3.PtInRect( ptGraphPos ) )
		{
			m_bOscillographHitTest = TRUE;
			return GraphHitTest_In;
		}
	}
	m_bOscillographHitTest = FALSE;

	if ( m_bUScale )
	{
		CSemVirtualMCICard::Instance().GetMobileUScaleParamEx(m_nMobileUScalePixelNum, m_ptMobileUScale, m_ptMobileUScale2 );
		CRect trackerRect( m_ptMobileUScale, m_ptMobileUScale2 );
		CLineTracker tracker( &trackerRect, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
		int nHitTest = tracker.HitTest( ptGraphPos );
		switch( nHitTest )
		{
		case	0:
			m_bUScaleHitTest = TRUE;
			return GraphHitTest_W;
		case	2:
			m_bUScaleHitTest = TRUE;
			return GraphHitTest_E;
		case	8:
			m_bUScaleHitTest = TRUE;
			return GraphHitTest_In;
		}

		// 16.06.12 多移动标尺实验
		for(int i=0; i<MobileUScaleCounts; i++ )
			m_bUScaleHitTestArray[i] = FALSE;
		int nPixelNum;
		CPoint pt, pt2;
		for(int i=0; i<MobileUScaleCounts; i++ )
		{
			CSemVirtualMCICard::Instance().GetMobileUScaleParamArray( i, nPixelNum, pt, pt2 );
			CRect trackerRc( pt, pt2 );
			CLineTracker tracker2( &trackerRc, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
			nHitTest = tracker2.HitTest( ptGraphPos );
			switch( nHitTest )
			{
			case	0:
				m_bUScaleHitTestArray[i] = TRUE;
				return GraphHitTest_W;
			case	2:
				m_bUScaleHitTestArray[i] = TRUE;
				return GraphHitTest_E;
			case	8:
				m_bUScaleHitTestArray[i] = TRUE;
				return GraphHitTest_In;
			}
		}
		// 16.06.12 多移动标尺实验
	}
	m_bUScaleHitTest = FALSE;

	// 2023.07 角度测量
	if ( m_bAngle )
	{
		POSITION pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			pAngle->bAngleHitTest = FALSE;
			m_plistAngle->GetNext( pos );
		}

		pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->bStart && pAngle->bCenter && pAngle->bEnd )
			{
				// 角的一条射线
				CRect trackerRect1( pAngle->AP.ptS, pAngle->AP.ptC );
				CLineTracker tracker1( &trackerRect1, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
				int nHitTest1 = tracker1.HitTest( ptGraphPos );
				if( nHitTest1 == 0 )
				{
					pAngle->bAngleHitTest = TRUE;
					m_bAngleHitTest = TRUE;
					return GraphHitTest_W;
				}

				// 角的另一条射线
				CRect trackerRect2( pAngle->AP.ptE, pAngle->AP.ptC );
				CLineTracker tracker2( &trackerRect2, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
				int nHitTest2 = tracker2.HitTest( ptGraphPos );
				if( nHitTest2 == 0 )
				{
					pAngle->bAngleHitTest = TRUE;
					m_bAngleHitTest = TRUE;
					return GraphHitTest_E;
				}
				if( nHitTest1 == 2 || nHitTest2 == 2 )
				{
					pAngle->bAngleHitTest = TRUE;
					m_bAngleHitTest = TRUE;
					return GraphHitTest_C;
				}

				// 标注所在大致区域
				CString str;
				str.Format( "%.1f", pAngle->AP.dAngle );
				CSize sz;
				CClientDC dc( this );
				sz = dc.GetTextExtent(str);
				CPoint ptPos1 = pAngle->AP.ptLabel;
				ptPos1.x -= sz.cx/2;
				ptPos1.y -= sz.cy/2;
				CPoint ptPos2 = CPoint( ptPos1.x +sz.cx, ptPos1.y +sz.cy );
				CRect trackerRect3( ptPos1, ptPos2 );
				if( trackerRect3.PtInRect( ptGraphPos ) )
				{
					pAngle->bAngleHitTest = TRUE;
					m_bAngleHitTest = TRUE;
					return GraphHitTest_L;	// 标注
				}

				// 弧所在大致区域
				if( pAngle->AP.rcBound.PtInRect( ptGraphPos ) )
				{
					pAngle->bAngleHitTest = TRUE;
					m_bAngleHitTest = TRUE;
					return GraphHitTest_In;	// 整体
				}
			}
			m_plistAngle->GetNext( pos );
		}
	}
	m_bAngleHitTest = FALSE;

	m_bStageOriginHitTest = FALSE;
/*	if( m_bStageOrigin )
	{
		rcFrame = m_rcStageDragRectLast;
		if( ptGraphPos.x < 1024 && ptGraphPos.y < 768 )
		{
			if ( ( abs(rcFrame.top +20 - ptGraphPos.y) < HandleAreaH2 )
				&& ( abs(rcFrame.left +20 - ptGraphPos.x) < HandleAreaW2 ) )
			{
				m_bStageOriginHitTest = TRUE;
				return GraphHitTest_In;
			}
		}
	}
*/
	int nusbScanType = CScanParameterManager::Instance().m_nusbScanType;
	int	nWidth		= CMCICardCommonSetting::Instance().m_sizeReso.cx;
	int nHeight		= CMCICardCommonSetting::Instance().m_sizeReso.cy;

	switch ( nusbScanType )
	{
	case USB_ST_SpotScan:			// 点扫描
	case USB_ST_LineScan_HORZ:		// 水平线扫描
	case USB_ST_LineScan_VERT:		// 垂直线扫描
	case USB_ST_AreaScan:			// 选区扫描
		rcFrame.left		= CScanParameterManager::Instance().m_usbParam.RasterScan.dwStartX;
		rcFrame.right		= CScanParameterManager::Instance().m_usbParam.RasterScan.dwStopX;
		rcFrame.top			= CScanParameterManager::Instance().m_usbParam.RasterScan.dwStartY;
		rcFrame.bottom		= CScanParameterManager::Instance().m_usbParam.RasterScan.dwStopY;
		break;

	case USB_ST_Stop:				// 冻结图象扫描
	case USB_ST_RasterScan:			// 常规扫描
	case USB_ST_AutoBCArea:			// 固定选区扫描	AutoBC
	case USB_ST_AutoFocusArea:		// 固定选区扫描 AutoFocus
	case USB_ST_AutoAstigmatArea:	// 固定选区扫描 AutoAstigmat
	case USB_ST_AnalogPhoto:		// 模拟照相
	default:
		// 09.11.18将整个扫描区域内外的单击测试类型区别开
		// 本次修改前代码副本
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
			return GraphHitTest_Normal;
		else
			return GraphHitTest_NULL;
	}

	if ( nusbScanType == USB_ST_SpotScan )
	{
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
		{
			if ( ( abs(rcFrame.top - ptGraphPos.y) < HandleAreaH )
				&& ( abs(rcFrame.left - ptGraphPos.x) < HandleAreaW ) )
				return GraphHitTest_In;
			else
				return GraphHitTest_Normal;
		}
		else
			return GraphHitTest_NULL;
	}


	if ( nusbScanType == USB_ST_LineScan_HORZ )
	{
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
		{
			if( abs(rcFrame.top - ptGraphPos.y) < HandleAreaH)
				return GraphHitTest_NS;
			else
				return GraphHitTest_Normal;
		}
		else
			return GraphHitTest_NULL;
	}

	if ( nusbScanType == USB_ST_LineScan_VERT )
	{
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
		{
			if( abs(rcFrame.left - ptGraphPos.x) < HandleAreaH)
				return GraphHitTest_E;
			else
				return GraphHitTest_Normal;
		}
		else
			return GraphHitTest_NULL;
	}

//	if ( rcFrame.PtInRect( ptGraphPos ) )
	CRect rcIn = rcFrame;
	rcIn.DeflateRect(HandleAreaW, HandleAreaH);
	if ( rcIn.PtInRect( ptGraphPos ) )
		return GraphHitTest_In;

	boolLeft	= FALSE;
	boolRight	= FALSE;
	boolUp		= FALSE;
	boolDown	= FALSE;

	// 本次修改后代码
	//-----------------------------------------------------
	if ( abs(rcFrame.left - ptGraphPos.x)<HandleAreaW
		&& (ptGraphPos.y >= rcFrame.top)
		&& (ptGraphPos.y <= rcFrame.bottom) )
		boolLeft = TRUE;

	if ( abs(ptGraphPos.x - rcFrame.right)<HandleAreaW
		&& (ptGraphPos.y >= rcFrame.top)
		&& (ptGraphPos.y <= rcFrame.bottom) )
		boolRight = TRUE;

	if ( abs(rcFrame.top - ptGraphPos.y)<HandleAreaH
		&& (ptGraphPos.x >= rcFrame.left)
		&& (ptGraphPos.x <= rcFrame.right) )
		boolUp = TRUE;

	if ( abs(ptGraphPos.y - rcFrame.bottom)<HandleAreaH
		&& (ptGraphPos.x >= rcFrame.left)
		&& (ptGraphPos.x <= rcFrame.right) )
		boolDown = TRUE;

/*	if ( nusbScanType == USB_ST_DualMagnification )
	{
		// 09.11.18将整个扫描区域内外的单击测试类型区别开
		if(boolRight)
			return GraphHitTest_E;
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
			return GraphHitTest_Normal;
		return GraphHitTest_NULL;
	}*/

	if(boolLeft && boolUp)
		return GraphHitTest_SW;

	if(boolLeft && boolDown)
		return GraphHitTest_NW;

	if(boolRight && boolUp)
		return GraphHitTest_NE;

	if(boolRight && boolDown)
		return GraphHitTest_SE;

	if(boolLeft)
		return GraphHitTest_W;

	if(boolRight)
		return GraphHitTest_E;

	if(boolUp)
		return GraphHitTest_N;
	
	if(boolDown)
		return GraphHitTest_S;

	if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
		return GraphHitTest_Normal;
	return GraphHitTest_NULL;
}

void CScanView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if ( m_bCatch )
	{
		//=====2004-5-13_屏幕鼠标控制电镜=================
		//SemCtrlMouse_Update
		if( (nFlags & MK_LBUTTON) == MK_LBUTTON )		//09.03.20
		{
			if ( m_nGraphHitTestSave == GraphHitTest_Normal )
			{
				switch( m_nAdjustorSort )				//根据调节器类型设置光标
				{
				case	SORT_BEAM_ALIGNMENT:
				case	SORT_SCAN_SHIFT:
				case	SORT_BEAM_ROTATE:
				case	SORT_CONDENSER_LENS:
				case	SORT_OBJECTIVE_LENS:
				case	SORT_BRIGHT:
				case	SORT_CONTRAST:
				case	SORT_STIGMATOR:
					SemCtrlMouse_Update( ScreenToGraph( point ), m_nAdjustorSort );
					return;
					break;
				}
			}
		}
		else if( (nFlags & MK_RBUTTON) == MK_RBUTTON )	//09.03.20
		{
			if ( m_nGraphHitTestSave == GraphHitTest_Normal )
			{
				SemCtrlMouse_Update( ScreenToGraph( point ), SORT_STIGMATOR );
				return;
			}
		}
		else if( (nFlags & MK_MBUTTON) == MK_MBUTTON )
		{
			// 按下中键且拖动，在样品台跟踪模式下，调整跟踪参数
			if( m_bStageTrack && 
				( (CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
				|| (CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
				|| (CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
				|| (CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady) ) )
				SetStageTrackParam( point );
			return;
		}
		USB_SetScreenMouseCtrl( point );
	}
	else
	{
		// 2023.07 角度测量
		if( m_bAngle )
		{
			POSITION pos = m_plistAngle->GetHeadPosition();
			while( pos )
			{
				AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
				if( pAngle->bStart && pAngle->bCenter && pAngle->bEnd )
				{
					m_plistAngle->GetNext( pos );
					continue;
				}
				if( pAngle->bCenter && pAngle->bStart )
				{
					pAngle->APbak = pAngle->AP;
					CPoint ptPosSave	= ScreenToGraph( point );
					pAngle->AP.ptE		= ptPosSave;
				}
				else if( pAngle->bStart )
				{
					pAngle->APbak = pAngle->AP;
					CPoint ptPosSave	= ScreenToGraph( point );
					pAngle->AP.ptC		= ptPosSave;
				}
				else
				{
					pAngle->APbak = pAngle->AP;
					CPoint ptPosSave	= ScreenToGraph( point );
					pAngle->AP.ptS		= ptPosSave;
				}
				GetAngleMeasureInfo( pAngle->AP );
				CSemVirtualMCICard::Instance().SetAngleParam( pAngle->nIndex, pAngle );
				break;
			}
		}
		else if ( (nFlags & MK_LBUTTON) == MK_LBUTTON )
		{
			m_ptPosSave				= ScreenToGraph( point );
			m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );

			// 本次修改后代码
			//-----------------------------------------------------
			//SemCtrlMouse_Init
			if ( m_bAbc != TRUE )
			{
				if ( m_nGraphHitTestSave == GraphHitTest_Normal )
				{
					SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_OBJECTIVE_LENS ) ) );
					m_rcDragRectLast = CRect(0,0,0,0);
					SetCapture();

					SemCtrlMouse_Init( m_ptPosSave, m_nAdjustorSort );
				}
				if ( m_nGraphHitTestSave != GraphHitTest_NULL )
					m_bCatch = TRUE;
			}

		}
		else if( (nFlags & MK_RBUTTON) == MK_RBUTTON )	//09.03.20
		{
			m_ptPosSave				= ScreenToGraph( point );
			m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );
			//SemCtrlMouse_Init
			if ( m_bAbc != TRUE )
			{
				if ( m_nGraphHitTestSave == GraphHitTest_Normal )
				{
					SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_STIGMATOR ) ) );
					m_rcDragRectLast = CRect(0,0,0,0);
					SetCapture();

					SemCtrlMouse_Init( m_ptPosSave, SORT_STIGMATOR );
				}
				if ( m_nGraphHitTestSave != GraphHitTest_NULL )
					m_bCatch = TRUE;
			}
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CScanView::USB_SetScreenMouseCtrl( CPoint point )
{
	if ( m_bOscillographHitTest )
	{
		SetOscillographParam(point);
		return;
	}

	if ( m_bUScaleHitTest )
	{
		SetUScaleParam(point);
		return;
	}
	
	if( m_bStageOriginHitTest )
	{
		SetStagePointParam( point );
		return;
	}

	// 16.06.12 多移动标尺实验
	for(int i=0; i<MobileUScaleCounts; i++ )
	{
		if( m_bUScaleHitTestArray[i] )
			SetUScaleParamArray( i, point );
	}
	// 16.06.12 多移动标尺实验

	// 2023.07 角度测量
	if ( m_bAngleHitTest )
	{
		CPoint ptPosSave	= ScreenToGraph( point );
		int nIndex = 0;
		POSITION pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->nAngleStatus > 2 )
			{
				int nOffsetX	= ptPosSave.x -pAngle->ptOriginal.x;
				int nOffsetY	= ptPosSave.y -pAngle->ptOriginal.y;
				if( pAngle->nAngleStatus > 3 )
				{
					// 标注
					pAngle->APbak = pAngle->AP;
					pAngle->AP.ptLabel.Offset(nOffsetX, nOffsetY);
				}
				else
				{
					// 整体移位
					pAngle->APbak = pAngle->AP;
					pAngle->AP.ptS.Offset(nOffsetX, nOffsetY);
					pAngle->AP.ptC.Offset(nOffsetX, nOffsetY);
					pAngle->AP.ptE.Offset(nOffsetX, nOffsetY);
					GetAngleMeasureInfo( pAngle->AP );
				}
				pAngle->ptOriginal = ptPosSave;
				CSemVirtualMCICard::Instance().SetAngleParam( pAngle->nIndex, pAngle );
				break;
			}
			nIndex++;
			m_plistAngle->GetNext( pos );
		}
		if( nIndex != m_nAngleCounts )
			return;

		pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->bStart && pAngle->bCenter && pAngle->bEnd )
			{
				m_plistAngle->GetNext( pos );
				continue;
			}
			if( pAngle->bCenter && pAngle->bStart )
			{
				pAngle->APbak = pAngle->AP;
				pAngle->AP.ptE		= ptPosSave;
			}
			else if( pAngle->bStart )
			{
				pAngle->APbak = pAngle->AP;
				pAngle->AP.ptC		= ptPosSave;
			}
			else
			{
				pAngle->APbak = pAngle->AP;
				pAngle->AP.ptS		= ptPosSave;
			}
			GetAngleMeasureInfo( pAngle->AP );
			CSemVirtualMCICard::Instance().SetAngleParam( pAngle->nIndex, pAngle );
			break;
		}
		return;
	}
	// 2023.07 角度测量

	int nScanType = CScanParameterManager::Instance().m_nusbScanType;
	if ( nScanType == USB_ST_SpotScan )
	{
		SetPointScanParam(point);
	}
	else if ( nScanType == USB_ST_LineScan_HORZ
		|| nScanType == USB_ST_LineScan_VERT )
	{
		SetLineScanParam(point);
	}
	else if ( nScanType == USB_ST_AreaScan )
	{
		SetActiveAreaParam(point);
	}
/*	else if ( nScanType == USB_ST_DualMagnification )
	{
		SetDualMagParam(point);		
	}*/
}


// 2009.03.20 鼠标滚轮消息响应
BOOL CScanView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag, varData;
	ClassID.lVal = SemControlPanelClass_Magnifier;
	SerialNumber.lVal = -1;
	Flag.lVal = 1;
	double sblStep;

	varData = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	sblStep = varData.dblVal;
	if( zDelta > 0 )		// Zoom In
	{
		sblStep = GetThirdSignificandWeight( varData.dblVal, 10 );
		varData.dblVal += sblStep;
	}
	else if( zDelta < 0 )	// Zoom Out
	{
		sblStep = GetThirdSignificandWeight( varData.dblVal, 10, FALSE );
		varData.dblVal -= sblStep;
	}
	Xpos = varData;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	theApp.DisplayMsg("Mouse wheel");
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CScanView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// 2023.07 角度测量
	if( m_bAngle )
	{
		CPoint ptPosSave = ScreenToGraph( point );
		int nIndex = 0;
		POSITION pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->nAngleStatus == 2 )
			{
				// 有要修改的
				GetAnglePoints( pAngle, ptPosSave );
				CSemVirtualMCICard::Instance().SetAngleParam( pAngle->nIndex, pAngle );
				break;
			}
			nIndex++;
			m_plistAngle->GetNext( pos );
		}
		if( nIndex != m_nAngleCounts )
		{
			if( m_bAngleHitTest )
			{
				if( m_bCatch )
				{
					m_bCatch = FALSE;
					ReleaseCapture();
				}
			}
			return;
		}

		pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->nAngleStatus > 2 )
			{
				if( pAngle->nAngleStatus > 3 )
				{
					int nOffsetX	= ptPosSave.x -pAngle->ptOriginal.x;
					int nOffsetY	= ptPosSave.y -pAngle->ptOriginal.y;
					pAngle->AP.ptLabel.Offset(nOffsetX, nOffsetY);
				}
				pAngle->bAngleHitTest = FALSE;
				pAngle->nAngleStatus = 0;
				CSemVirtualMCICard::Instance().SetAngleParam( pAngle->nIndex, pAngle );
				break;
			}
			m_plistAngle->GetNext( pos );
		}
		if( m_bAngleHitTest )
		{
			if( m_bCatch )
			{
				m_bCatch = FALSE;
				ReleaseCapture();
			}
			return;
		}
	}
	// 2023.07 角度测量

	if ( m_bCatch )
	{
		m_bCatch = FALSE;
		ReleaseCapture();
		UpdateScanParam( point );
		return;
	}
	
	GraphTextEdit( point );

	CScrollView::OnLButtonUp(nFlags, point);
}

//09.03.20
void CScanView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bCatch )
	{
		m_bCatch = FALSE;
		ReleaseCapture();
		UpdateScanParam( point );
		return;
	}

	CScrollView::OnRButtonUp(nFlags, point);
}

void CScanView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage < 1 
		|| ( !(CStageXYManager::Instance().m_Stage.xy.bReady)
			&& (!StageMC600_IsReady())
			&& (!StageMC6600_IsReady())
			&& (!StageTMCM_IsReady()) ) )	// 几个台子都未准备好
	{
		CMenu menuManual;
		// Create a new menu for the application window.
		VERIFY(menuManual.CreatePopupMenu());

		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_CondenserLens, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_CondenserLens));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_CondenserLens2, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_CondenserLens2));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_StigmatorX, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_StigmatorX));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_StigmatorX2, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_StigmatorX2));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_StigmatorY, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_StigmatorY));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_StigmatorY2, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_StigmatorY2));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_ScanShiftX, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_ScanShiftX));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_ScanShiftY, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_ScanShiftY));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_BeamAlignmentX, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_BeamAlignmentX));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_BeamAlignmentY, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_BeamAlignmentY));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_Brightness, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_Brightness));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_Contrast, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_Contrast));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_NULL, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_NULL));

		menuManual.CheckMenuItem( ID_ADJUSTOR_MANUAL_ObjectiveLens + CControlLayer::Instance().m_nAdjustorManualSort,
			MF_BYCOMMAND | MF_CHECKED );

		CPoint temppoint = point;
		ClientToScreen( &temppoint );
		menuManual.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			temppoint.x, temppoint.y, this, NULL);
		CScrollView::OnMButtonDown(nFlags, point);
		return;
	}

	m_bStageTrack	= TRUE;
	m_bCatch		= TRUE;
	SetCapture();

	m_ptPosSave				= ScreenToGraph( point );
	m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );
	m_ptStageTrackParam		= m_ptPosSave;

	// 画一个标识，准备进入跟踪模式
	CPoint ptPoint	= m_ptStageTrackParam;
	if( ptPoint.x < nSTAGE_TRACE_POINT_RADIUS )
		ptPoint.x = nSTAGE_TRACE_POINT_RADIUS;
	if( ptPoint.x > CMCICardCommonSetting::Instance().m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS )
		ptPoint.x = CMCICardCommonSetting::Instance().m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS;
	if( ptPoint.y < nSTAGE_TRACE_POINT_RADIUS )
		ptPoint.y = nSTAGE_TRACE_POINT_RADIUS;
	if( ptPoint.y > CMCICardCommonSetting::Instance().m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS )
		ptPoint.y = CMCICardCommonSetting::Instance().m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS;
	CSemVirtualMCICard::Instance().GraphDrawCircle( ptPoint, m_ptPosSave, FALSE, nSTAGE_TRACE_POINT_RADIUS, RGB(0, 255, 0) );

	// 设置初始跟踪步距
	double dMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		// 4/5轴台
		if( dMag < 1000 )
		{
			m_nStageTrackStepX = 50;
			m_nStageTrackStepY = 50;
		}
		else if( dMag < 5000 )
		{
			m_nStageTrackStepX = 20;
			m_nStageTrackStepY = 20;
		}
		else
		{
			m_nStageTrackStepX = 10;
			m_nStageTrackStepY = 10;
		}
		m_nStageTrackStepX = 0;
		m_nStageTrackStepY = 0;
		if(StageTMCM_IsReady())
			StageTMCM_SetTrackParam( m_nStageTrackStepX, m_nStageTrackStepY );
		else if(StageMC6600_IsReady())
			StageMC6600_SetTrackParam( m_nStageTrackStepX, m_nStageTrackStepY );
		else if( StageMC600_IsReady() )
			StageMC600_SetTrackParam( m_nStageTrackStepX, m_nStageTrackStepY );
	}
	else if( CStageXYManager::Instance().m_Stage.xy.bReady )
	{
		if( dMag < 1000 )
		{
			m_nStageTrackStepX = 50;
			m_nStageTrackStepY = 50;
		}
		else if( dMag < 5000 )
		{
			m_nStageTrackStepX = 20;
			m_nStageTrackStepY = 20;
		}
		else
		{
			m_nStageTrackStepX = 10;
			m_nStageTrackStepY = 10;
		}

		char szBuffer[64];
		CString str;
		// 跟踪
		str.Format( "JG %ld,%ld;BGXY", m_nStageTrackStepX, m_nStageTrackStepY );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		// 开启跟踪保护
		SetTimer( nAutoStageTrack, nAutoStageXY_FIElapse, NULL );
		CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
		if( pMainFrame && (pMainFrame->m_dlgStage.m_hWnd != NULL))
			pMainFrame->m_dlgStage.SetTimer( 1, nAutoStageXY_FIElapse, NULL );
	}

	CScrollView::OnMButtonDown(nFlags, point);
}

void CScanView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bCatch )
	{
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
		{
			StageTMCM_StopTrack();
			theApp.DisplayMsg( "Stop in OnMButtonUp" );
		}
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
		{
			StageMC6600_StopTrack();
			theApp.DisplayMsg( "Stop in OnMButtonUp" );
		}
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
		{
			StageMC600_StopTrack();
			theApp.DisplayMsg( "Stop in OnMButtonUp" );
		}
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )
		{
			// 停止样品台移动
			CStageXYManager::Instance().m_DMCWin.Command( "ST" );
			theApp.DisplayMsg( "ST in OnMButtonUp" );
			KillTimer( nAutoStageTrack );
		}
		m_bCatch = FALSE;
		ReleaseCapture();
		m_bStageTrack = FALSE;

		// 擦除标识，退出跟踪模式
		CPoint ptPoint	= m_ptStageTrackParam;
		if( ptPoint.x < nSTAGE_TRACE_POINT_RADIUS )
			ptPoint.x = nSTAGE_TRACE_POINT_RADIUS;
		if( ptPoint.x > CMCICardCommonSetting::Instance().m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS )
			ptPoint.x = CMCICardCommonSetting::Instance().m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS;
		if( ptPoint.y < nSTAGE_TRACE_POINT_RADIUS )
			ptPoint.y = nSTAGE_TRACE_POINT_RADIUS;
		if( ptPoint.y > CMCICardCommonSetting::Instance().m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS )
			ptPoint.y = CMCICardCommonSetting::Instance().m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS;
		CRect rect(ptPoint.x-nSTAGE_TRACE_POINT_RADIUS,ptPoint.y-nSTAGE_TRACE_POINT_RADIUS,
			ptPoint.x+nSTAGE_TRACE_POINT_RADIUS,ptPoint.y+nSTAGE_TRACE_POINT_RADIUS);
		CSemVirtualMCICard::Instance().GraphDrawCircle( ptPoint, m_ptPosSave );
		CSemVirtualMCICard::Instance().GraphDrawCircle( ptPoint, m_ptPosSave, FALSE );
		CSemVirtualMCICard::Instance().GraphClear( rect, RGB(0,0,0) );
		return;
	}
	CScrollView::OnMButtonUp(nFlags, point);
}

void CScanView::UpdateScanParam( CPoint point )
{
	if ( m_bOscillographHitTest )
		return;

	if ( m_bUScaleHitTest )
		return;

	if ( m_bStageOriginHitTest )
	{
		m_ptStagePointParam = m_rcStageDragRectLast.TopLeft();
		m_ptStagePointParam.Offset(20,20);
		return;
	}

	if ( m_rcDragRectLast == CRect(0,0,0,0) )
	{
		return;
	}

	CString str;
	USB_ScanType nScanType = CScanParameterManager::Instance().m_nusbScanType;	
	if ( nScanType == USB_ST_SpotScan )
	{
		CScanParameterManager::Instance().m_ptPointScanParam = m_rcDragRectLast.TopLeft();
		CScanParameterManager::Instance().m_ptPointScanParam.Offset(6,6);
		str.Format( "Press select scan type to Spot, (%d, %d), speed = %d us/s", CScanParameterManager::Instance().m_ptPointScanParam.x,
					CScanParameterManager::Instance().m_ptPointScanParam.y, CScanParameterManager::Instance().m_nusbPixRate /2 );
		theApp.DisplayMsg(str);
	}
	else if ( nScanType == USB_ST_LineScan_HORZ )
	{
		CScanParameterManager::Instance().m_nLineScanParam = m_rcDragRectLast.top;
		str.Format( "Press select scan type to LineScan HORZ, (%d), speed = %d us/s",
					CScanParameterManager::Instance().m_nLineScanParam, CScanParameterManager::Instance().m_nusbPixRate /2 );
		theApp.DisplayMsg(str);
	}
	else if ( nScanType == USB_ST_LineScan_VERT )
	{
		CScanParameterManager::Instance().m_nLineScanParam = m_rcDragRectLast.left;
		str.Format( "Press select scan type to LineScan VERT, (%d), speed = %d us/s",
					CScanParameterManager::Instance().m_nLineScanParam, CScanParameterManager::Instance().m_nusbPixRate /2 );
		theApp.DisplayMsg(str);
	}
	else if ( nScanType == USB_ST_AreaScan )
	{
		CScanParameterManager::Instance().m_rcAreaScanParam = m_rcDragRectLast;
		str.Format( "Press select scan type to Area Scan, (%d,%d,%d,%d), speed = %d us/s",
					CScanParameterManager::Instance().m_rcAreaScanParam.left, CScanParameterManager::Instance().m_rcAreaScanParam.top,
					CScanParameterManager::Instance().m_rcAreaScanParam.right, CScanParameterManager::Instance().m_rcAreaScanParam.bottom,
					CScanParameterManager::Instance().m_nusbPixRate /2 );
		theApp.DisplayMsg(str);
	}
/*	else if ( nScanType == USB_ST_DualMagnification )
	{
		m_rcDualMagParam = m_rcDragRectLast;
	}*/
	USB_ChangeScan( nScanType );
}

void CScanView::SetPointScanParam(CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );

	CPoint ptPoint	= CScanParameterManager::Instance().m_ptPointScanParam;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	nOffsetX = nOffsetX / 4 * 4;
	nOffsetY = nOffsetY / 4 * 4;

	ptPoint.x += nOffsetX;
	ptPoint.y += nOffsetY;

	// 本次修改后代码
	//-----------------------------------------------------
	CRect rect(ptPoint.x-6,ptPoint.y-6,ptPoint.x+8,ptPoint.y+8);
	CSemVirtualMCICard::Instance().GraphClear( m_rcDragRectLast );
	CSemVirtualMCICard::Instance().GraphClear( rect, RGB(0,0,0) );

	ptPos0.x = m_rcDragRectLast.left+6;
	ptPos0.y = m_rcDragRectLast.top+6;
	ptPos1 = ptPos0;
	ptPos0.x -= 6;
	ptPos1.x += 8;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1 );

	ptPos0.x = m_rcDragRectLast.left+6;
	ptPos0.y = m_rcDragRectLast.top+6;
	ptPos1 = ptPos0;
	ptPos0.y -= 6;
	ptPos1.y += 8;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1 );

	ptPos0.x = rect.left+6;
	ptPos0.y = rect.top+6;
	ptPos1 = ptPos0;
	ptPos0.x -= 6;
	ptPos1.x += 8;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );

	ptPos0.y += 1;
	ptPos1.y += 1;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );

	ptPos0.x = rect.left+6;
	ptPos0.y = rect.top+6;
	ptPos1 = ptPos0;

	ptPos0.y -= 6;
	ptPos1.y += 8;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );

	ptPos0.x += 1;
	ptPos1.x += 1;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );
	//======2006.09.04_点扫描点的画法=================

	if( theApp.m_dwOpr > 0 && ((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL )
	{
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelMCI.UpdateSpotX( ptPoint.x, FALSE );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelMCI.UpdateSpotY( ptPoint.y, FALSE );
	}
	m_rcDragRectLast = rect;
}

void CScanView::SetLineScanParam(CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );

	CRect rect;
	int nScanType = CScanParameterManager::Instance().m_nusbScanType;
	if ( nScanType == USB_ST_LineScan_HORZ )
	{
		long nLineScanY		= CScanParameterManager::Instance().m_nLineScanParam;
		long nLineOffset	= ptPos1.y - ptPos0.y;
		long nLineScanY_New = nLineScanY + nLineOffset;
		rect = CRect(0,nLineScanY_New,CMCICardCommonSetting::Instance().m_sizeReso.cx,nLineScanY_New+1);
		if( theApp.m_dwOpr > 0 && ((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL )
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelMCI.UpdateSpotY( nLineScanY_New, FALSE );
	}
	else
	{
		long nLineScanX		= CScanParameterManager::Instance().m_nLineScanParam;
		long nLineOffset	= ptPos1.x - ptPos0.x;
		long nLineScanX_New = nLineScanX + nLineOffset;
		rect = CRect(nLineScanX_New, 0, nLineScanX_New+1, CMCICardCommonSetting::Instance().m_sizeReso.cy);
		if( theApp.m_dwOpr > 0 && ((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL )
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelMCI.UpdateSpotX( nLineScanX_New, FALSE );
	}

	CSemVirtualMCICard::Instance().GraphClear( m_rcDragRectLast );
	CSemVirtualMCICard::Instance().GraphClear( rect, RGB(255,0,0) );
	m_rcDragRectLast = rect;
}

void CScanView::SetLineScanParamManually( BOOL bHORZ, int nPos )
{
	CRect rect;
	if ( bHORZ )
		rect = CRect(0, nPos, CMCICardCommonSetting::Instance().m_sizeReso.cx, nPos+1);
	else
		rect = CRect(nPos, 0, nPos+1, CMCICardCommonSetting::Instance().m_sizeReso.cy);

	CSemVirtualMCICard::Instance().GraphClear( m_rcDragRectLast );
	CSemVirtualMCICard::Instance().GraphClear( rect, RGB(255,0,0) );
	m_rcDragRectLast = rect;
	CScanParameterManager::Instance().m_nLineScanParam = nPos;
}

void CScanView::SetActiveAreaParam_FromMainFrm(CPoint point)
{
	if( !m_bCatch )
		return;
	CRect viewRect;
	GetWindowRect(&viewRect); // 获取子框架窗口的客户区大小
	// 假设视图的大小固定且与子框架窗口一致，这里直接用客户区大小代替视图大小
	// 如果视图大小与子框架不同，需要根据实际情况调整 viewRect
	if (!viewRect.PtInRect(point) && USB_ST_AreaScan == CScanParameterManager::Instance().m_nusbScanType )
	{
		// 点击在子框架客户区但不在视图区域内
		// 在这里处理点击事件
		SetActiveAreaParam(point);
		m_bCatch = FALSE;
		ReleaseCapture();
		UpdateScanParam( point );
	}
}

void CScanView::SetActiveAreaParam(CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );

	const long AreaMinW = 96;
	const long AreaMinH = 96;
	CRect rcArea	= CScanParameterManager::Instance().m_rcAreaScanParam;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	nOffsetX = nOffsetX / 4 * 4;
	nOffsetY = nOffsetY / 4 * 4;

	CRect rcAreaNew = rcArea;
	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		if ( (rcAreaNew.left + nOffsetX) < 0 )
			nOffsetX = - rcAreaNew.left;
		if ( (rcAreaNew.right + nOffsetX) > CMCICardCommonSetting::Instance().m_sizeReso.cx )
			nOffsetX = CMCICardCommonSetting::Instance().m_sizeReso.cx - rcAreaNew.right;

		if ( (rcAreaNew.top + nOffsetY) < 0 )
			nOffsetY = - rcAreaNew.top;
		if ( (rcAreaNew.bottom + nOffsetY) > CMCICardCommonSetting::Instance().m_sizeReso.cy )
			nOffsetY = CMCICardCommonSetting::Instance().m_sizeReso.cy - rcAreaNew.bottom;

		rcAreaNew.OffsetRect(nOffsetX, nOffsetY);
		break;

	case GraphHitTest_E:
		rcAreaNew.right += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.right = rcAreaNew.left + AreaMinW;
		}
		break;

	case GraphHitTest_W:
		rcAreaNew.left += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.left = rcAreaNew.right - AreaMinW;
		}
		break;

	case GraphHitTest_N:
		rcAreaNew.top += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.top = rcAreaNew.bottom - AreaMinH;
		}
		break;

	case GraphHitTest_S:
		rcAreaNew.bottom += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.bottom = rcAreaNew.top + AreaMinH;
		}
		break;

	case GraphHitTest_NE:
		rcAreaNew.top += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.top = rcAreaNew.bottom - AreaMinH;
		}
		rcAreaNew.right += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.right = rcAreaNew.left + AreaMinW;
		}
		break;

	case GraphHitTest_SE:
		rcAreaNew.bottom += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.bottom = rcAreaNew.top + AreaMinH;
		}
		rcAreaNew.right += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.right = rcAreaNew.left + AreaMinW;
		}
		break;

	case GraphHitTest_SW:
		rcAreaNew.top += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.top = rcAreaNew.bottom - AreaMinH;
		}
		rcAreaNew.left += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.left = rcAreaNew.right - AreaMinW;
		}
		break;

	case GraphHitTest_NW:
		rcAreaNew.bottom += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.bottom = rcAreaNew.top + AreaMinH;
		}
		rcAreaNew.left += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.left = rcAreaNew.right - AreaMinW;
		}
		break;

	default:
		break;
	}
	rcAreaNew.left = rcAreaNew.left < 0 ? 0 : rcAreaNew.left;
	rcAreaNew.top = rcAreaNew.top < 0 ? 0 : rcAreaNew.top;
	rcAreaNew.right = rcAreaNew.right > CMCICardCommonSetting::Instance().m_sizeReso.cx ? CMCICardCommonSetting::Instance().m_sizeReso.cx : rcAreaNew.right;
	rcAreaNew.bottom = rcAreaNew.bottom > CMCICardCommonSetting::Instance().m_sizeReso.cy ? CMCICardCommonSetting::Instance().m_sizeReso.cy : rcAreaNew.bottom;

	CSemVirtualMCICard::Instance().GraphDrawRect( m_rcDragRectLast );
	CSemVirtualMCICard::Instance().GraphDrawRect( rcAreaNew, RGB(255,0,0) );
	m_rcDragRectLast = rcAreaNew;
}

void CScanView::SetDualMagParam(CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );

	long AreaMinW	= CMCICardCommonSetting::Instance().m_sizeReso.cx /8;
	long AreaMinH	= CMCICardCommonSetting::Instance().m_sizeReso.cx /8;
	CRect rcArea	= CScanParameterManager::Instance().m_rcDualMagParam;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;
	int	nWidth		= CMCICardCommonSetting::Instance().m_sizeReso.cx /2;
	int nHeight		= CMCICardCommonSetting::Instance().m_sizeReso.cy;

	nOffsetX = nOffsetX / 4 * 4;
	nOffsetY = nOffsetY / 4 * 4;

	CRect rcAreaNew = rcArea;
	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		if ( (rcAreaNew.left + nOffsetX) < 0 )
			nOffsetX = - rcAreaNew.left;
		if ( (rcAreaNew.right + nOffsetX) > nWidth )
			nOffsetX = nWidth - rcAreaNew.right;

		if ( (rcAreaNew.top + nOffsetY) < 0 )
			nOffsetY = - rcAreaNew.top;
		if ( (rcAreaNew.bottom + nOffsetY) > nHeight )
			nOffsetY = nHeight - rcAreaNew.bottom;

		rcAreaNew.OffsetRect(nOffsetX, nOffsetY);
		break;

	case GraphHitTest_E:
		if ( (rcAreaNew.right + nOffsetX) > nWidth )
			nOffsetX = nWidth - rcAreaNew.right;

		rcAreaNew.right += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.right = rcAreaNew.left + AreaMinW;
		}

		rcAreaNew.bottom = rcAreaNew.top + rcAreaNew.Width() *2;
		if ( rcAreaNew.bottom > nHeight )
		{
			rcAreaNew.bottom = nHeight;
			rcAreaNew.right = rcAreaNew.left + rcAreaNew.Height() / 2;
		}
		break;

	default:
		break;
	}

	CSemVirtualMCICard::Instance().GraphDrawRect( m_rcDragRectLast );
	CSemVirtualMCICard::Instance().GraphDrawRect( rcAreaNew, RGB(255,0,0) );
	m_rcDragRectLast = rcAreaNew;
}

void CScanView::SetUScaleParam(CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );	

	// 本次修改后代码
	//-------------------------------------------------
	int	nLength		= 0;//sqrt( pow( (m_ptMobileUScale.x-m_ptMobileUScale2.x), 2) + pow( (m_ptMobileUScale.y-m_ptMobileUScale2.y), 2 ) ) +0.5;

	AdjustPos( ptPos1 );

	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	CPoint pt1 = m_ptMobileUScale;
	CPoint pt2 = m_ptMobileUScale2;
	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		pt1.x += nOffsetX;
		pt1.y += nOffsetY;
		pt2.x += nOffsetX;
		pt2.y += nOffsetY;
		AdjustUScaleLength( pt1, pt2, TRUE );
		break;

	case GraphHitTest_W:
		pt1 = ptPos1;
		if( m_bShiftDown )
			pt1.y = pt2.y;
		else if( m_bCtrlDown )
			pt1.x = pt2.x;
		AdjustUScaleLength( pt1, pt2 );
		break;

	case GraphHitTest_E:
		pt2 = ptPos1;
		if( m_bShiftDown )
			pt2.y = pt1.y;
		else if( m_bCtrlDown )
			pt2.x = pt1.x;
		AdjustUScaleLength( pt2, pt1 );
		break;

	default:
		break;
	}
	AdjustPos( pt1 );
	AdjustPos( pt2 );

	nLength = (int)(sqrt( pow((double)(pt1.x - pt2.x), 2) + pow( (double)(pt1.y - pt2.y), 2 ) ) +0.5);
	if( /*m_bStageOrigin && */m_bDebugCoadjust )
	{
		// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
		// 根据标尺长度，将标尺移到以参考点为中心、以标尺长度为边长的正方形的下底边
		pt1.x	= CMCICardCommonSetting::Instance().m_sizeReso.cx /2 -nLength /2;
		pt2.x	= pt1.x +nLength;
		pt1.y	= CMCICardCommonSetting::Instance().m_sizeReso.cy /2 +nLength /2;
		pt2.y	= pt1.y;
	}
	m_ptMobileUScale = pt1;
	m_ptMobileUScale2 = pt2;

	CSemVirtualMCICard::Instance().SetMobileUScalePosEx( m_ptMobileUScale, m_ptMobileUScale2, RGB(255, 0, 0) );
	//05.08.16移动标尺的刷新
	CString m_csMobileUScaleText = CSemVirtualMCICard::Instance().CalculateMobileUScale( (int)(nLength));// *1024.0 /m_sizeReso.cx) );
	CSemVirtualMCICard::Instance().UpdateUScale(nLength, m_csMobileUScaleText);
	m_ptPosSave = ptPos1;

	// 2005-1-17_放大倍数XY联合校正
	CPoint ptPosNew = m_ptMobileUScale;
	CRect rcAreaNew;
	rcAreaNew.left		= ptPosNew.x;
	rcAreaNew.top		= ptPosNew.y - nLength - 1;
	rcAreaNew.right		= ptPosNew.x + nLength + 1;
	rcAreaNew.bottom	= ptPosNew.y;

	if ( theApp.m_boolShowUScaleXY )
	{
		CSemVirtualMCICard::Instance().GraphDrawRectEx( m_bDrawCircle, m_rcUScaleAreaLast );
		CSemVirtualMCICard::Instance().GraphDrawRectEx( m_bDrawCircle, rcAreaNew, RGB(0,255,0) );
		m_boolUScaleAreaIsDraw = TRUE;
	}
	else if ( m_boolUScaleAreaIsDraw )
	{
		CSemVirtualMCICard::Instance().GraphDrawRect( m_rcUScaleAreaLast );
		m_boolUScaleAreaIsDraw = FALSE;
	}
	m_rcUScaleAreaLast = rcAreaNew;
}

// 16.06.12 多移动标尺实验
void CScanView::SetUScaleParamArray(int nIndex, CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	
	AdjustPos( ptPos1 );

	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	CPoint pt1 = m_ptMobileUScaleArray[nIndex];
	CPoint pt2 = m_ptMobileUScaleArray2[nIndex];
	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		pt1.x += nOffsetX;
		pt1.y += nOffsetY;
		pt2.x += nOffsetX;
		pt2.y += nOffsetY;
		AdjustUScaleLength( pt1, pt2, TRUE );
		break;

	case GraphHitTest_W:
		pt1 = ptPos1;
		if( m_bShiftDown )
			pt1.y = pt2.y;
		else if( m_bCtrlDown )
			pt1.x = pt2.x;
		AdjustUScaleLength( pt1, pt2 );
		break;

	case GraphHitTest_E:
		pt2 = ptPos1;
		if( m_bShiftDown )
			pt2.y = pt1.y;
		else if( m_bCtrlDown )
			pt2.x = pt1.x;
		AdjustUScaleLength( pt2, pt1 );
		break;

	default:
		break;
	}
	AdjustPos( pt1 );
	AdjustPos( pt2 );

	m_nMobileUScalePixelNumArray[nIndex] = (int)(sqrt( pow( (double)(pt1.x - pt2.x), 2) + pow( (double)(pt1.y - pt2.y), 2 ) ) +0.5);
	m_ptMobileUScaleArray[nIndex] = pt1;
	m_ptMobileUScaleArray2[nIndex] = pt2;

	CSemVirtualMCICard::Instance().SetMobileUScalePosArray( nIndex, m_ptMobileUScaleArray[nIndex], m_ptMobileUScaleArray2[nIndex], RGB(255, 0, 0) );
	CString csMobileUScaleText = CSemVirtualMCICard::Instance().CalculateMobileUScale( m_nMobileUScalePixelNumArray[nIndex] );
	CSemVirtualMCICard::Instance().UpdateUScaleArray(nIndex, m_nMobileUScalePixelNumArray[nIndex], csMobileUScaleText);
	m_ptPosSave = ptPos1;
//	m_rcUScaleAreaLast = rcAreaNew;
}
// 16.06.12 多移动标尺实验

void CScanView::SetUScaleParamByDefine( double dScale )
{
	// 由欲标注的尺寸得到标尺像素长度
	int nScalePixels = (int)( CSemVirtualMCICard::Instance().CalculateMobileUScale( dScale ) * CMCICardCommonSetting::Instance().m_sizeReso.cx /1024.0 );
	if( nScalePixels < 16 )
		return;
	if( m_ptMobileUScale.x + nScalePixels > CMCICardCommonSetting::Instance().m_sizeReso.cx )
		return;
	// 以m_ptMobileUScale为起点的向右的水平线,得到m_ptMobileUScale2的坐标
	m_ptMobileUScale2.x = m_ptMobileUScale.x + nScalePixels;
	m_ptMobileUScale2.y = m_ptMobileUScale.y;
	// 反送到显示层刷新标尺
	CSemVirtualMCICard::Instance().SetMobileUScalePosEx( m_ptMobileUScale, m_ptMobileUScale2, RGB(255, 0, 0) );
	CString m_csMobileUScaleText = CSemVirtualMCICard::Instance().CalculateMobileUScale( (int)(nScalePixels *1024.0 / CMCICardCommonSetting::Instance().m_sizeReso.cx) );
	CSemVirtualMCICard::Instance().UpdateUScale(nScalePixels, m_csMobileUScaleText);
}

void CScanView::AdjustPos( CPoint& ptPos )
{
	if( ptPos.x < 0 )
		ptPos.x = 0;
	if( ptPos.y < 0 )
		ptPos.y = 0;
	if( ptPos.x > CMCICardCommonSetting::Instance().m_sizeReso.cx )
		ptPos.x = CMCICardCommonSetting::Instance().m_sizeReso.cx;
	if( ptPos.y > CMCICardCommonSetting::Instance().m_sizeReso.cy )
		ptPos.y = CMCICardCommonSetting::Instance().m_sizeReso.cy;
}

void CScanView::AdjustUScaleLength( CPoint& ptStart, CPoint& ptEnd, BOOL bTranslation )
{
	CPoint pt1 = ptStart;	//动的点
	CPoint pt2 = ptEnd;		//不动的点

	double k, b;//斜率与截距
	int nLength = 0;

	const long UScaleMin = 16;
	int	nWidth		= CMCICardCommonSetting::Instance().m_sizeReso.cx;
	int nHeight		= CMCICardCommonSetting::Instance().m_sizeReso.cy;

	AdjustPos( pt1 );
	AdjustPos( pt2 );

	if( pt1.x == pt2.x )		//垂直线，无斜率
	{
		nLength = abs( pt1.y - pt2.y );
		if( nLength < UScaleMin )
		{
			if( bTranslation )	//平移时
			{
				if( pt1.y < pt2.y && pt1.y == 0 )
					pt2.y = pt1.y + UScaleMin;
				else if( pt1.y < pt2.y && pt2.y == nHeight )
					pt1.y = pt2.y - UScaleMin;
				else if( pt1.y >= pt2.y && pt2.y == 0 )
					pt1.y = pt2.y + UScaleMin;
				else if( pt1.y >= pt2.y && pt1.y == nHeight )
					pt2.y = pt1.y - UScaleMin;
			}
			else
			{
				if( pt1.y > pt2.y )
					pt1.y = pt2.y + UScaleMin;
				else
					pt1.y = pt2.y - UScaleMin;
			}
		}
		else if( nLength > nHeight )
		{
			if( pt1.y > pt2.y )
				pt1.y = nHeight;
			else
				pt1.y = 0;
		}
	}
	else if( pt1.y == pt2.y )	//水平线
	{
		nLength = abs( pt1.x - pt2.x );
		if( nLength < UScaleMin )
		{
			if( bTranslation )	//平移时
			{
				if( pt1.x < pt2.x && pt1.x == 0 )
					pt2.x = pt1.x + UScaleMin;
				else if( pt1.x < pt2.x && pt2.x == nWidth )
					pt1.x = pt2.x - UScaleMin;
				else if( pt1.x >= pt2.x && pt2.x == 0 )
					pt1.x = pt2.x + UScaleMin;
				else if( pt1.x >= pt2.x && pt1.x == nWidth )
					pt2.x = pt1.x - UScaleMin;
/*				if( pt1.x > pt2.x )
					pt2.x = pt1.x - UScaleMin;
				else
					pt2.x = pt1.x + UScaleMin;
*/			}
			else
			{
				if( pt1.x > pt2.x )
					pt1.x = pt2.x + UScaleMin;
				else
					pt1.x = pt2.x - UScaleMin;
			}
		}
		else if( nLength > nWidth )
		{
			if( pt1.x > pt2.x )
				pt1.x = nWidth;
			else
				pt1.x = 0;
		}
	}
	else
	{
		nLength = (int)(sqrt( pow( (double)(pt1.x-pt2.x), 2) + pow( (double)(pt1.y-pt2.y), 2 ) ) +0.5);
		if( nLength < UScaleMin )
		{
			k = 1. * (pt1.y - pt2.y) / (pt1.x - pt2.x);
			b = pt1.y - k * pt1.x;
			if( (pt1.x > pt2.x)  && (pt1.y > pt2.y) && (k < 1) )
			{
				pt1.x = pt2.x + UScaleMin;
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x > pt2.x) && (pt1.y > pt2.y) && (k == 1) )
			{
				pt1.x = (long)(pt2.x + UScaleMin / sqrt((double)2));
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x > pt2.x) && (pt1.y > pt2.y) && (k > 1) )
			{
				pt1.y = pt2.y + UScaleMin;
				pt1.x = (long)((pt1.y - b) / k);
			}
			else if( (pt1.x > pt2.x) && (pt1.y < pt2.y) && (k < -1) )
			{
				pt1.y = pt2.y - UScaleMin;
				pt1.x = (long)((pt1.y - b) / k);
			}
			else if( (pt1.x > pt2.x) && (pt1.y < pt2.y) && (k == -1) )
			{
				pt1.x = (long)(pt2.x + UScaleMin / sqrt((double)2));
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x > pt2.x) && (pt1.y < pt2.y) && (k > -1) )
			{
				pt1.x = pt2.x + UScaleMin;
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x < pt2.x) && (pt1.y > pt2.y) && (k < -1) )
			{
				pt1.y = pt2.y + UScaleMin;
				pt1.x = (long)((pt1.y - b) / k);
			}
			else if( (pt1.x < pt2.x) && (pt1.y > pt2.y) && (k == -1) )
			{
				pt1.x = (long)(pt2.x - UScaleMin / sqrt((double)2));
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x < pt2.x) && (pt1.y > pt2.y) && (k > -1) )
			{
				pt1.x = pt2.x - UScaleMin;
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x < pt2.x) && (pt1.y < pt2.y) && (k < 1) )
			{
				pt1.x = pt2.x - UScaleMin;
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x < pt2.x) && (pt1.y < pt2.y) && (k == 1) )
			{
				pt1.x = (long)(pt2.x - UScaleMin / sqrt((double)2));
				pt1.y = (long)(k * pt1.x + b);
			}
			else
			{
				pt1.y = pt2.y - UScaleMin;
				pt1.x = (long)((pt1.y - b) / k);
			}
		}
	}
	ptStart = pt1;
	ptEnd	= pt2;
}

// 2023.07 角度测量
// 得到两射线之间的夹角
double CScanView::GetAngleOfTwoVector( CPoint ptS, CPoint ptE, CPoint ptC )
{
	double angle = atan2( ptS.x -ptC.x, ptS.y -ptC.y) - atan2( ptE.x -ptC.x, ptE.y -ptC.y );
	if( angle > PI )
		angle -= 2*PI;
	if( angle < -PI )
		angle += 2*PI;
	angle = angle *180.0 /PI;	// 角度变弧度
	return angle;
}

// 得到坐标
CPoint CScanView::GetCoordinates( CPoint ptCenter, double dAngle, int nR )
{
	CPoint pt;
	if( dAngle <= 0 )
		dAngle = -dAngle;
	else
	{
		double a = 180 -dAngle;
		dAngle = 180 +a;
	}
	pt = CPoint( ptCenter.x + nR*cos(PI /180 *dAngle), ptCenter.y + nR*sin(PI /180 *dAngle) );
	return pt;
}

// 得到测量信息
void CScanView::GetAngleMeasureInfo( AnglePosition& AP )
{
	AP.dAngle = GetAngleOfTwoVector( AP.ptS, AP.ptE, AP.ptC );
	if( AP.dAngle < 0 )
		AP.dAngle = 360 +AP.dAngle;
	// 弧所在圆的半径
	double dX1 = AP.ptS.x - AP.ptC.x;
	double dY1 = AP.ptS.y - AP.ptC.y;
	double dX2 = AP.ptE.x - AP.ptC.x;
	double dY2 = AP.ptE.y - AP.ptC.y;
	int nLen1 = (int)(sqrt( dX1*dX1 +dY1*dY1 ));
	int nLen2 = (int)(sqrt( dX2*dX2 +dY2*dY2 ));
	int nR = nLen1 > nLen2 ? nLen2/3 : nLen1 /3;	// 短的线段的1/3
	// 弧所在圆的外接矩形
	AP.rcBound.left		= AP.ptC.x -nR;
	AP.rcBound.top		= AP.ptC.y -nR;
	AP.rcBound.right	= AP.rcBound.left + nR*2;
	AP.rcBound.bottom	= AP.rcBound.top + nR*2;
	// 弧的起始角度
	CPoint ptEnd;	// 中心点的3点钟方向上的任意一点
	ptEnd.x = AP.ptC.x +100;
	ptEnd.y = AP.ptC.y;
	double dAngleStart = GetAngleOfTwoVector(AP.ptS, ptEnd, AP.ptC);
	// 两线夹角的一半
	double dAngleLabel = dAngleStart -AP.dAngle /2;
	// 弧的终止角度
	double dAngleEnd = GetAngleOfTwoVector(AP.ptE, ptEnd, AP.ptC);

	// 弧的起始和终止位置
	AP.ptArcStart = GetCoordinates( AP.ptC, dAngleStart, nR );
	AP.ptArcEnd = GetCoordinates( AP.ptC, dAngleEnd, nR );
	// 标注点位置
	AP.ptLabel = GetCoordinates( AP.ptC, dAngleLabel, nR );
}

void CScanView::GetAnglePoints( AngleMeasure* pAM, CPoint ptPos )
{
	if( !(pAM->bStart) )
	{
		pAM->AP.ptS	= ptPos;	// 第一个为起始点
		pAM->bStart	= TRUE;
	}
	else if( !(pAM->bCenter) )
	{
		pAM->AP.ptC	= ptPos;	// 第二个点为中心点
		pAM->bCenter= TRUE;
	}
	else if( !(pAM->bEnd) )
	{
		pAM->AP.ptE	= ptPos;	// 第三个为终点
		pAM->bEnd	= TRUE;
	}
	if( pAM->bStart && pAM->bCenter && pAM->bEnd )
	{
		pAM->nAngleStatus	= 0;		// 添加角度完成，单击点不再记录
		if( pAM->bAngleHitTest )
			pAM->bAngleHitTest = FALSE;
		// 3个点都有了，计算角度
		GetAngleMeasureInfo( pAM->AP );
		if( m_bAddAngle )
			AngleAddNew();
	}
	Invalidate(FALSE);
}
// 2023.07 角度测量

// 2005-1-17_放大倍数XY联合校正
void CScanView::UpdateUScaleXY()
{
	if ( m_bUScale )
	{
		if ( theApp.m_boolShowUScaleXY )
		{
			if( m_rcUScaleAreaLast.IsRectEmpty() )
				SetUScaleParam( CPoint(0, 400) );
			CSemVirtualMCICard::Instance().GraphDrawRectEx( m_bDrawCircle, m_rcUScaleAreaLast, RGB(0,255,0) );
			m_boolUScaleAreaIsDraw = TRUE;
		}
		else
		{
			CSemVirtualMCICard::Instance().GraphDrawRectEx( m_bDrawCircle, m_rcUScaleAreaLast );
			m_boolUScaleAreaIsDraw = FALSE;
		}
	}
	else if ( m_boolUScaleAreaIsDraw )
	{
		CSemVirtualMCICard::Instance().GraphDrawRect( m_rcUScaleAreaLast );
		m_boolUScaleAreaIsDraw = FALSE;
	}
}

void CScanView::SetOscillographParam(CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	
	long nOffsetY	= ptPos1.y - ptPos0.y;

	CPoint ptPosNew = m_ptOscillograph;

	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		ptPosNew.y += nOffsetY;
		break;

	default:
		break;
	}
	//08.04.24保证示波器不出扫描区域
	if( ptPosNew.y < 0 )
		ptPosNew.y = 0;
	if( ptPosNew.y > CMCICardCommonSetting::Instance().m_sizeReso.cy-g_szUSB_FrameOscillograph.cy )
		ptPosNew.y = CMCICardCommonSetting::Instance().m_sizeReso.cy-g_szUSB_FrameOscillograph.cy;
	//08.04.24保证示波器不出扫描区域

	CSemVirtualMCICard::Instance().SetOscillographPos( ptPosNew );
}

void CScanView::GraphTextEdit( CPoint point )
{
	static BOOL boolBusy = FALSE;
	static CSemCtrlDlgEdit	dlg;

	if ( m_bAbc )
	{
		point.y -= 8;
		CPoint ptPos = ScreenToGraph( point );
		// Grid Pos (Screen)

		ptPos.x = (ptPos.x / 16) * 16;
		ptPos.y = (ptPos.y / 32) * 32;

		if( (ptPos.x > CMCICardCommonSetting::Instance().m_sizeReso.cx *3/4) && (ptPos.y > CMCICardCommonSetting::Instance().m_sizeReso.cy -64) )
		{
			CDlgInput input;
			input.m_nTitleIndex = IDS_SD_PROMPT_Input_Note;
			if( input.DoModal() == IDOK )
				CSemVirtualMCICard::Instance().UpdateNote( input.m_strInput );
		}
		else
		{
			dlg.m_ptPos = ptPos;
			if ( boolBusy )
			{
				dlg.ClearTextBuff();
			}
			else
			{
				boolBusy = TRUE;

				dlg.m_csText.Empty();
				dlg.DoModal();

				// Modify 2003-6-27 Leo
				m_bAbc = FALSE;
//				m_pScanDlg->UpdateData( FALSE );

				boolBusy = FALSE;
			}
		}
	}
}

BOOL CScanView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	static HCURSOR hCursor_IBEAM = LoadCursor(NULL, IDC_IBEAM);

	if ( m_bCatch )
	{
		return TRUE;
	}

	if ( nHitTest != HTCLIENT )
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	POINT point;
	GetCursorPos( &point );
	ScreenToClient( &point );

	CPoint ptPos = ScreenToGraph( point );
	int nGraphHitTest = USB_GraphHitTest( ptPos );

	if ( m_bAbc )
	{
		if ( nGraphHitTest == GraphHitTest_Normal )
		{
			if ( GetCursor() != hCursor_IBEAM )
				SetCursor( hCursor_IBEAM );

			return TRUE;
		}
	}

	if ( GetCursor() != g_hCursors[nGraphHitTest] )
		SetCursor( g_hCursors[nGraphHitTest] );

	return TRUE;
}

//扫描速度档改变
void CScanView::OnScanSpeedChanged(UINT nID)
{
	CScanParameterManager::Instance().m_nusbPixRateIndex = nID - ID_SCAN_SPEED_0;
	NotifyPixRate( CScanParameterManager::Instance().m_nPixRateIndex[CScanParameterManager::Instance().m_nusbPixRateIndex] );
}

void CScanView::OnUpdateScanSpeedChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( CScanParameterManager::Instance().m_nusbPixRateIndex == pCmdUI->m_nID -ID_SCAN_SPEED_0 );
	pCmdUI->Enable( !m_bLive );
}

//通报扫描速度改变
void CScanView::NotifyPixRate( unsigned short nNewVal )
{
	CScanParameterManager::Instance().m_nusbPixRate = nNewVal;

	// 扫描速度改变时，要通知DFocus重新计算Valley个数
	CSemCoreAssemble::Instance().m_DFocus.SetParams( 16, CScanParameterManager::Instance().m_nusbPixRate );

	CString str;
	str.Format( "Press select speed to %d us/s", CScanParameterManager::Instance().m_nusbPixRate /2 );
	theApp.DisplayMsg(str);

	if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
	if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_AnalogPhoto )
		USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
}

void CScanView::OnScanResolutionChanged(UINT nID)
{
	NotifyScanResolution( nID - ID_SCAN_RESOLUTION_1 );
}

void CScanView::OnUpdateScanResolutionChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( CScanParameterManager::Instance().m_nusbScanResoIndex == pCmdUI->m_nID -ID_SCAN_RESOLUTION_1 );
	pCmdUI->Enable( !m_bLive );
}

void CScanView::NotifyScanResolution( UINT uNewVal, BOOL bForce )
{
	if( !bForce && CScanParameterManager::Instance().m_nusbScanResoIndex == uNewVal )
		return;

	if ( CScanParameterManager::Instance().m_nusbScanType == USB_ST_AnalogPhoto )
		return;

	// 先停止采集
	USB_ScanType nusbScanType = CScanParameterManager::Instance().m_nusbScanType;
	USB_ChangeScan( USB_ST_Stop );

	CScanParameterManager::Instance().m_nusbScanResoIndex = uNewVal;
	CScanParameterManager::Instance().USB_GetResoSize( CScanParameterManager::Instance().m_nusbScanResoIndex );

	UpdateWindowPos(m_bSplit);
//	UpdateScale();

	// 16.06.12 多移动标尺实验
	m_nMobileUScaleCounts = 0;

	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
		StageTMCM_SetResolution( CScanParameterManager::Instance().m_sizeReso );
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
		StageMC6600_SetResolution( CScanParameterManager::Instance().m_sizeReso );
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
		StageMC600_SetResolution( CScanParameterManager::Instance().m_sizeReso );
	CSemVirtualMCICard::Instance().SetResolution( CScanParameterManager::Instance().m_sizeReso );
	CSemVirtualMCICard::Instance().SetSplit(m_bSplit);
	CSemVirtualMCICard::Instance().SetLiveMode( m_bLive );
	CSemVirtualMCICard::Instance().SetParams(17,CScanParameterManager::Instance().m_nSEratio);
	CSemVirtualMCICard::Instance().Reset( this );
	CSemVirtualMCICard::Instance().SetProductLogo( theApp.m_strLogoDisp );
	CSemVirtualMCICard::Instance().ShowLabel( m_bLabel );
	CSemVirtualMCICard::Instance().ShowHeader( m_bLabel );
	CSemVirtualMCICard::Instance().ShowTime( m_bDate );
	SyncDate( m_bDate );
	KillTimer( nAutoTimerDate );
	if( m_bDate )
		SetTimer( nAutoTimerDate, 1000*30, NULL );
	CSemVirtualMCICard::Instance().ShowNote( m_bNote );
	CSemVirtualMCICard::Instance().ShowHeader( m_bFOV );
	CSemVirtualMCICard::Instance().ShowPixelSize( m_bPixelSize );
	CSemVirtualMCICard::Instance().ShowMobileUScale( m_bUScale );
	CSemVirtualMCICard::Instance().ShowOscillograph( m_bOscillograph );
	CSemVirtualMCICard::Instance().UpdateHV( CHVPowerDataManager::Instance().HVPower_UpdateHVdisp() );

	UpdatePhotoSn(theApp.m_nPhotoSerialNumber);

	UpdateScale();
	OnViewZoomFit();

	// 设置扫描区域参数
	CScanParameterManager::Instance().USB_SetScanRectParam();
	CString str;
	str.Format( "Press select resolution to %d x %d", CMCICardCommonSetting::Instance().m_sizeReso.cx, CMCICardCommonSetting::Instance().m_sizeReso.cy );
	theApp.DisplayMsg(str);
	// 按照当前的方式重新采集
	USB_ChangeScan( nusbScanType );
	// 更新一下放大倍数相关参数
	VARIANT ClassID, SerialNumber, Flag, Xpos, Ypos; 
	ClassID.lVal	= SemControlPanelClass_Magnifier;
	SerialNumber.lVal = -1;
	Flag.lVal		= 1;
	Xpos.dblVal		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag );

	// 自动台移动参考点
	m_ptStagePointParam.x	= CMCICardCommonSetting::Instance().m_sizeReso.cx /2;
	m_ptStagePointParam.y	= CMCICardCommonSetting::Instance().m_sizeReso.cy /2;
	m_ptPosSave = m_ptStagePointParam;	// 屏幕显示区域始终是1024*768大小，所以中心固定不变
	SetStagePointParam( CPoint(512,384) );

	// 15.09 远程功能
	if (m_bCommSendImgData)
	{
//		KYKYComm_SimulImage();
		KYKYComm_ImageFrame();
	}
}

void CScanView::UpdateWindowPos( BOOL bDual )
{
	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	int nScreenX = GetSystemMetrics( SM_CXSCREEN );
	int nLeft, nTop;
	if( bDual )
	{
		if( m_bSplitZoom )
		{
			int nWidth = (nScreenX -20) /32 *32;
			nLeft	= ( nScreenX -nWidth ) /2;
			if( nLeft < 0 )
				nLeft = 10;
			nTop	= 10;
			pfrm->SetWindowPos(NULL, nLeft, nTop, nWidth +4, (int)(1.0* CMCICardCommonSetting::Instance().m_sizeReso.cy /CMCICardCommonSetting::Instance().m_sizeReso.cx *(nWidth/2)) +4, SWP_NOZORDER );
		}
		else
		{
			{
				nLeft	= (nScreenX -450 -1440) /2;
				if( nLeft < 0 )
					nLeft = 10;
				nTop	= 10;
				pfrm->SetWindowPos(NULL, nLeft, nTop, 1440 +4, (int)(1.0* CMCICardCommonSetting::Instance().m_sizeReso.cy /CMCICardCommonSetting::Instance().m_sizeReso.cx *720) +4, SWP_NOZORDER );
			}
		}
	}
	else
	{
		{
			int nX = CMCICardCommonSetting::Instance().m_sizeReso.cx;
				nX = CScanParameterSaver::Instance().m_nDispPixel;	// 1024;
			nLeft	= (nScreenX -450 -nX) /2;
			if( nLeft < 0 )
				nLeft = 10;
			nTop	= 10;
//			if( m_sizeReso.cx > 1024 || m_sizeReso.cy > 800 )
				pfrm->SetWindowPos(NULL, nLeft, nTop, nX +4, (int)(1.0* CMCICardCommonSetting::Instance().m_sizeReso.cy /CMCICardCommonSetting::Instance().m_sizeReso.cx *nX) +4, SWP_NOZORDER );
		}
	}
}

void CScanView::OnScanStoreModeChanged(UINT nID)
{
	if( CScanParameterManager::Instance().m_nusbScanStoreMode == nID - ID_SCAN_STORE_ZZZ )
		return;
	
	// 先停止采集
	USB_ScanType nusbScanType = CScanParameterManager::Instance().m_nusbScanType;
	USB_ChangeScan( USB_ST_Stop );
	
	CScanParameterManager::Instance().m_nusbScanStoreMode = nID - ID_SCAN_STORE_ZZZ;
	
	// 按照当前的方式重新采集
	USB_ChangeScan( nusbScanType );
}

void CScanView::OnUpdateScanStoreModeChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( CScanParameterManager::Instance().m_nusbScanStoreMode == pCmdUI->m_nID -ID_SCAN_STORE_ZZZ );
}

void CScanView::ResetImageCard()
{
	// 先停止采集
	USB_ScanType nusbScanType = CScanParameterManager::Instance().m_nusbScanType;
	USB_ChangeScan( USB_ST_Stop );
	Sleep(1000);

	CSemVirtualMCICard::Instance().SetParams(17,CScanParameterManager::Instance().m_nSEratio);
	CSemVirtualMCICard::Instance().Reset( this );
	CSemVirtualMCICard::Instance().SetProductLogo( theApp.m_strLogoDisp );
	CSemVirtualMCICard::Instance().ShowLabel( m_bLabel );
	CSemVirtualMCICard::Instance().ShowHeader( m_bLabel );
	CSemVirtualMCICard::Instance().ShowMobileUScale( m_bUScale );
	CSemVirtualMCICard::Instance().ShowOscillograph( m_bOscillograph );
	CSemVirtualMCICard::Instance().UpdateHV( CHVPowerDataManager::Instance().HVPower_UpdateHVdisp() );

	// 按照当前的方式重新采集
	USB_ChangeScan( nusbScanType );
	UpdatePhotoSn(theApp.m_nPhotoSerialNumber);
	UpdateData(FALSE);
}

void CScanView::StopPhoto()
{
	USB_ChangeScan( USB_ST_Stop );
	m_bStopPhoto = TRUE;

	OnPhotoSnapshot();


	// 照相后重新启动日期刷新定时器05.08.12
	if( m_bDate )
		SetTimer( nAutoTimerDate, 1000*30, NULL );
	// 停止照相后，将当前通道组的Filter恢复
	WORD wValue = (WORD)( (CControlLayer::Instance().m_BrdMCI.nDetPair << 8) | (BYTE)(CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair]) );

	// 判断一下是否要MCI复位通讯
	unsigned short nRegister1 = 0;
	unsigned short nRegister2 = 0;
	unsigned short nRegister3 = 0;
	unsigned short nRegister4 = 0;
	CSemCtrl::Instance().Serial_ReadbackData( 1, USB_CID_SCN_ReadBoardID, &nRegister1 );
	CSemCtrl::Instance().Serial_ReadbackData( 2, USB_CID_LNS_ReadBoardID, &nRegister2 );
	CSemCtrl::Instance().Serial_ReadbackData( 3, USB_CID_VAC_ReadBoardID, &nRegister3 );
	CSemCtrl::Instance().Serial_ReadbackData( 4, USB_CID_SPR_ReadBoardID, &nRegister4 );

	if( (nRegister1 != 0xA1) || (nRegister2 != 0xA2) || (nRegister3 != 0xA3) || (nRegister4 != 0xA4) )
	{
		CString str, strTemp;
		if( nRegister1 != 0xA1 )
			str += "1/ ";
		if( nRegister2 != 0xA2 )
			str += "2/ ";
		if( nRegister3 != 0xA3 )
			str += "3/ ";
		if( nRegister4 != 0xA4 )
			str += "4 ";
		str += "have errors! ";
		str += "Reset SubBoards!";
		theApp.DisplayMsg(str);
		CSemCtrl::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
	}
}

void CScanView::USB_ChangeScan( USB_ScanType nusbScanType )
{
	static int nusbScanTypeSave = USB_ST_RasterScan;

	CSemVirtualMCICard::Instance().SetStopScan(nusbScanType == USB_ST_Stop);

	USB_ScanFunction		Function	= USB_SF_RasterScan;	// 工作方式
	unsigned short			nPixRate	= CScanParameterManager::Instance().m_nusbPixRate;

	USB_ScanParam		Param;				// 扫描参数
	Param.RasterScan.dwStartX	= 0;
	Param.RasterScan.dwStartY	= 0;
	Param.RasterScan.dwStopX	= CMCICardCommonSetting::Instance().m_sizeReso.cx;
	Param.RasterScan.dwStopY	= CMCICardCommonSetting::Instance().m_sizeReso.cy;

	// ResetImageCard()
	if ( nusbScanType == -1 )
	{
		nusbScanType	= CScanParameterManager::Instance().m_nusbScanType;

		Function	= CScanParameterManager::Instance().m_usbFunction;		// 工作方式
		Param		= CScanParameterManager::Instance().m_usbParam;			// 扫描参数
	}

	IPrepare4Scan* pPreparation = Factory<IPrepare4Scan,USB_ScanType>::Instance().BuildProduct((USB_ScanType)nusbScanType);
	if (pPreparation)
	{
		pPreparation->Prepare(this,Param,Function,nPixRate);
		delete pPreparation;
	}
	else
	{
		nusbScanType = USB_ST_Stop;		// 默认冻结图象扫描
		CSemCtrl::Instance().m_SemScan.USB_ScanStop();
	}


	if( /*nusbScanType == USB_ST_AnalogPhoto ||*/ m_bPreview )
		// 恢复实际放大倍数
		CSemVirtualMCICard::Instance().UpdateScreenMag( CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal, TRUE );
	else
		CSemVirtualMCICard::Instance().UpdateScreenMag( CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal );
	if ( USB_ST_Stop < nusbScanType && nusbScanType <= USB_ST_Max )
	{
		CSemCtrl::Instance().m_SemScan.USB_SetScanFunctionEx(
			Function,			// 工作方式
			CMCICardCommonSetting::Instance().m_sizeReso,			// 分辨率
			nPixRate,			// 像素率 = 点停留时间/4，必须为整数
			Param,				// 扫描参数
			CHVPowerDataManager::Instance().m_dHV );

/*		// 扫描方式改变，动态聚焦参数也需改变
		varVal = CSemCoreAssemble::Instance().m_DFocus.GetSwitch();
		if( varVal.lVal == 1 || varVal.lVal == 2 )	// 正在使用动态聚焦
		{
			if( nusbScanType == USB_SF_RasterScan || nusbScanType == USB_ST_AreaScan )	// 只在常规扫描，区域扫描时使用
				CSemCoreAssemble::Instance().m_DFocus.SetParams( 13, Param.RasterScan.dwStopY - Param.RasterScan.dwStartY );
		}
*/	}

	// 保存参数 for ResetImageCard()
	nusbScanTypeSave	= nusbScanType;
	CScanParameterManager::Instance().m_usbFunction		= Function;			// 工作方式
	CScanParameterManager::Instance().m_usbParam		= Param;			// 扫描参数

	if ( CScanParameterManager::Instance().m_nusbScanType != nusbScanType )
		CScanParameterManager::Instance().m_nusbScanType = nusbScanType;

	if( m_bStageOrigin && nusbScanType != USB_ST_AnalogPhoto )	// 非同步扫描时恢复移动参考点
	{
		m_ptPosSave = CPoint(0,0);
		SetStagePointParam( CPoint(0,0) );
	}
}

BOOL CScanView::IsPhoto()
{
	if ( CScanParameterManager::Instance().m_nusbScanType == USB_ST_AnalogPhoto)
		return TRUE;
	
	return FALSE;
}

//-----------------------------------------------------
//======修改日期：2004年5月12日星期三==================


//扫描模式改变
void CScanView::OnScanTypeChanged(UINT nID)
{
	// TODO: Add your control notification handler code here
	unsigned short nusbScanTypeOld = CScanParameterManager::Instance().m_nusbScanType;

	BeginWaitCursor(); // display the hourglass cursor
	CScanParameterManager::Instance().m_nusbScanType = USB_ScanType(nID - ID_SCAN_TYPE_FREEZE);
	//======2004-5-12_照相编号自动增加====================
	if ( m_bStopPhoto == TRUE )
	{
		m_bStopPhoto = FALSE;
		UpdatePhotoSn(theApp.m_nPhotoSerialNumber + 1);
	}

	CString str = "";
	_GetScanTypeChangePromptMsg(str);
	theApp.DisplayMsg(str);

	if( CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
	{
		if( m_bDate )
			SetTimer( nAutoTimerDate, 1000*30, NULL );
		CSemVirtualMCICard::Instance().UpdateScreenMag( CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal );
	}

	if( nusbScanTypeOld == USB_ST_AnalogPhoto && CScanParameterManager::Instance().m_nusbScanType != USB_ST_AnalogPhoto )
	{
		// 将当前通道组的Filter恢复
		WORD wValue = (WORD)( (CControlLayer::Instance().m_BrdMCI.nDetPair << 8) | (BYTE)(CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair]) );
	}

	USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
	EndWaitCursor(); // remove the hourglass cursor
}

void CScanView::OnUpdateScanTypeChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( CScanParameterManager::Instance().m_nusbScanType == pCmdUI->m_nID -ID_SCAN_TYPE_FREEZE );
	pCmdUI->Enable( !m_bLive );
}

void CScanView::OnScanTypeLive() 
{
	// TODO: Add your command handler code here
	NotifyScanLive( !m_bLive );
}

void CScanView::OnUpdateScanTypeLive(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bLive );
}

void CScanView::NotifyScanLive( BOOL bLive )
{
	if( m_bLive == bLive )
		return;
	m_bLive = bLive;
	// 图像分辨率变为512*384，但扫描区域大小为1024*768，即放大2倍显示
	// 扫描速度设置为500ns

	if( m_bLive )
	{
		// 先备份扫描参数
//		m_scanParamBak = theApp.m_Param.scan;
		CScanParameterManager::Instance().m_scanParamBak.nScan_Type	= CScanParameterManager::Instance().m_nusbScanType;
		CScanParameterManager::Instance().m_scanParamBak.nScan_Speed	= CScanParameterManager::Instance().m_nusbPixRate;
		CScanParameterManager::Instance().m_scanParamBak.nSync_Speed	= CScanParameterManager::Instance().m_nusbPhotoPixRate;
		CScanParameterManager::Instance().m_scanParamBak.nScan_ResoX	= CMCICardCommonSetting::Instance().m_sizeReso.cx;
		CScanParameterManager::Instance().m_scanParamBak.nScan_ResoY	= CMCICardCommonSetting::Instance().m_sizeReso.cy;
		CScanParameterManager::Instance().m_scanParamBak.nSplit		= m_bSplit;
		CScanParameterManager::Instance().m_scanParamBak.nDet2		= m_bOnlyDet2;
		// 停止采集
		USB_ChangeScan( USB_ST_Stop );
		// 扫描参数设置为快速扫描参数
// 		theApp.m_Param.scan	= m_scanParamLive;
// 		theApp.m_Param.scan.nSplit	= m_bSplit;
// 		theApp.m_Param.scan.nDet2	= m_bOnlyDet2;
		CScanParameterManager::Instance().m_nusbScanType		= CScanParameterManager::Instance().m_scanParamLive.nScan_Type;
		CScanParameterManager::Instance().m_nusbPixRate		= CScanParameterManager::Instance().m_scanParamLive.nScan_Speed;
		CScanParameterManager::Instance().m_nusbPhotoPixRate	= CScanParameterManager::Instance().m_scanParamLive.nSync_Speed;
		CMCICardCommonSetting::Instance().m_sizeReso.cx		= CScanParameterManager::Instance().m_scanParamLive.nScan_ResoX;
		CMCICardCommonSetting::Instance().m_sizeReso.cy		= CScanParameterManager::Instance().m_scanParamLive.nScan_ResoY;
		// 改变扫描分辨率
//		UpdateWindowPos(m_bSplit);
		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
		int nScreenX = GetSystemMetrics( SM_CXSCREEN );
		int nLeft, nTop;
		if( m_bSplit )
		{
			if( m_bSplitZoom )
			{
				int nWidth = (nScreenX -20) /32 *32;
				nLeft	= ( nScreenX -nWidth ) /2;
				if( nLeft < 0 )
					nLeft = 10;
				nTop	= 10;
				pfrm->SetWindowPos(NULL, nLeft, nTop, nWidth +4, (int)(1.0* CMCICardCommonSetting::Instance().m_sizeReso.cy /CMCICardCommonSetting::Instance().m_sizeReso.cx *(nWidth/2)) +4, SWP_NOZORDER );
			}
			else
			{
				nLeft	= (nScreenX -450 -1440) /2;
				if( nLeft < 0 )
					nLeft = 10;
				nTop	= 10;
				pfrm->SetWindowPos(NULL, nLeft, nTop, 1440 +4, (int)(1.0* CMCICardCommonSetting::Instance().m_sizeReso.cy / CMCICardCommonSetting::Instance().m_sizeReso.cx *720) +4, SWP_NOZORDER );
			}
		}
		else
		{
			int nX = CMCICardCommonSetting::Instance().m_sizeReso.cx *2;
			nLeft	= (nScreenX -450 -nX) /2;
			if( nLeft < 0 )
				nLeft = 10;
			nTop	= 10;
			pfrm->SetWindowPos(NULL, nLeft, nTop, CMCICardCommonSetting::Instance().m_sizeReso.cx*2 +4, CMCICardCommonSetting::Instance().m_sizeReso.cy*2 +4, SWP_NOZORDER );
		}

		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
			StageTMCM_SetResolution( CScanParameterManager::Instance().m_sizeReso );
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
			StageMC6600_SetResolution( CScanParameterManager::Instance().m_sizeReso );
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
			StageMC600_SetResolution( CScanParameterManager::Instance().m_sizeReso );
		CSemVirtualMCICard::Instance().SetResolution( CMCICardCommonSetting::Instance().m_sizeReso );
		CSemVirtualMCICard::Instance().SetSplit(m_bSplit);
		CSemVirtualMCICard::Instance().SetLiveMode( m_bLive );
		CSemVirtualMCICard::Instance().SetParams(17,CScanParameterManager::Instance().m_nSEratio);
		CSemVirtualMCICard::Instance().Reset( this );
		CSemVirtualMCICard::Instance().SetProductLogo( theApp.m_strLogoDisp );
		CSemVirtualMCICard::Instance().ShowLabel( m_bLabel );
		CSemVirtualMCICard::Instance().ShowHeader( m_bLabel );
		CSemVirtualMCICard::Instance().ShowMobileUScale( m_bUScale );
		CSemVirtualMCICard::Instance().ShowOscillograph( m_bOscillograph );
		CSemVirtualMCICard::Instance().UpdateHV( CHVPowerDataManager::Instance().HVPower_UpdateHVdisp() );

		CString strPhotoSN;
		strPhotoSN.Format("SN:%04d", theApp.m_nPhotoSerialNumber);
		CSemVirtualMCICard::Instance().UpdatePhotoSN( strPhotoSN );

		UpdateScale();
		OnViewZoomFit();

		// 设置扫描区域参数
		CScanParameterManager::Instance().USB_SetScanRectParam();
		CString str;
		str.Format( "Press select resolution to %d x %d, Live mode", CMCICardCommonSetting::Instance().m_sizeReso.cx, CMCICardCommonSetting::Instance().m_sizeReso.cy );
		theApp.DisplayMsg(str);
		// 自动台移动参考点
		m_ptStagePointParam.x	= CMCICardCommonSetting::Instance().m_sizeReso.cx /2;
		m_ptStagePointParam.y	= CMCICardCommonSetting::Instance().m_sizeReso.cy /2;
		m_ptPosSave = m_ptStagePointParam;	// 屏幕显示区域始终是1024*768大小，所以中心固定不变
		SetStagePointParam( CPoint(512,384) );
		// 发起采集
		USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
		// 更新一下放大倍数相关参数
		VARIANT ClassID, SerialNumber, Flag, Xpos, Ypos; 
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		SerialNumber.lVal = -1;
		Flag.lVal		= 1;
		Xpos.dblVal		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
	else
	{
		USB_ChangeScan( USB_ST_Stop );
//		theApp.m_Param.scan	= m_scanParamBak;
		CScanParameterManager::Instance().m_nusbScanType		= CScanParameterManager::Instance().m_scanParamBak.nScan_Type;
		CScanParameterManager::Instance().m_nusbPixRate		= CScanParameterManager::Instance().m_scanParamBak.nScan_Speed;
		CScanParameterManager::Instance().m_nusbPhotoPixRate	= CScanParameterManager::Instance().m_scanParamBak.nSync_Speed;
		CMCICardCommonSetting::Instance().m_sizeReso.cx		= CScanParameterManager::Instance().m_scanParamBak.nScan_ResoX;
		CMCICardCommonSetting::Instance().m_sizeReso.cy		= CScanParameterManager::Instance().m_scanParamBak.nScan_ResoY;
		NotifyScanResolution( CScanParameterManager::Instance().m_nusbScanResoIndex, TRUE );
		USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
		// 更新一下放大倍数相关参数
		VARIANT ClassID, SerialNumber, Flag, Xpos, Ypos; 
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		SerialNumber.lVal = -1;
		Flag.lVal		= 1;
		Xpos.dblVal		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
}

void CScanView::OnScanTypeSplit() 
{
	// TODO: Add your command handler code here
	NotifyScanSplit( !m_bSplit, m_bSplitZoom );
}

void CScanView::NotifyScanSplit( BOOL bSplit, BOOL bSplitZoom )
{
	if( (m_bSplit == bSplit) && (m_bSplitZoom == bSplitZoom) )
		return;

	m_bSplit = bSplit;
	m_bSplitZoom = bSplitZoom;
	if( !m_bSplit )
		m_bSplitZoom = FALSE;

	USB_ScanType nScanType = CScanParameterManager::Instance().m_nusbScanType;
	USB_ChangeScan( USB_ST_Stop );
	Sleep(5);
	// 设置窗口大小
	UpdateWindowPos( m_bSplit );
	if( m_bSplit )
	{
		CSemVirtualMCICard::Instance().SetFunction( USB_ST_Max );	// Split
		if( m_bSplitZoom )
			theApp.DisplayMsg("Press split: split & zoom");
		else
			theApp.DisplayMsg("Press split: split");
		/////////////////////////////////////////////
		CControlLayer::Instance().CheckDet2nd( FALSE );
		theApp.DisplayMsg("force scan type det 2: select 0,2,4 chn");
		/////////////////////////////////////////////
	}
	else
	{
		CSemVirtualMCICard::Instance().SetFunction( CScanParameterManager::Instance().m_nusbScanType );
		theApp.DisplayMsg("Press split: normal");
		/////////////////////////////////////////////
		CControlLayer::Instance().CheckDet2nd( m_bOnlyDet2 );
		if( m_bOnlyDet2 )
			theApp.DisplayMsg("recover scan type det 2: select 1,3,5 chn");
		else
			theApp.DisplayMsg("recover scan type det 2: select 0,2,4 chn");
		/////////////////////////////////////////////
	}
	CSemVirtualMCICard::Instance().SetSplit( m_bSplit );
	CSemVirtualMCICard::Instance().SetParams(17,CScanParameterManager::Instance().m_nSEratio);
	// Reset 图像卡
	CSemVirtualMCICard::Instance().Reset( this );
	CSemVirtualMCICard::Instance().SetProductLogo( theApp.m_strLogoDisp );
	CSemVirtualMCICard::Instance().ShowLabel( m_bLabel );
	CSemVirtualMCICard::Instance().ShowHeader( m_bLabel );
	CSemVirtualMCICard::Instance().ShowTime( m_bDate );
	SyncDate( m_bDate );
	KillTimer( nAutoTimerDate );
	if( m_bDate )
		SetTimer( nAutoTimerDate, 1000*30, NULL );
	CSemVirtualMCICard::Instance().ShowNote( m_bNote );
	CSemVirtualMCICard::Instance().ShowHeader( m_bFOV );
	CSemVirtualMCICard::Instance().ShowPixelSize( m_bPixelSize );
	CSemVirtualMCICard::Instance().ShowMobileUScale( m_bUScale );
	CSemVirtualMCICard::Instance().ShowOscillograph( m_bOscillograph );
	CSemVirtualMCICard::Instance().UpdateHV( CHVPowerDataManager::Instance().HVPower_UpdateHVdisp() );

	UpdateScale();
	CSemCtrl::Instance().m_SemScan.USB_SetParams( 31, m_bSplit );
	CheckCCDSwitch();
	USB_ChangeScan( nScanType );

    CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if(NULL == pfrm) return;
	if(NULL == pfrm->m_wndImpPanelAll2.m_hWnd) return;

	if( m_bSplit )
	{
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_ctrlSignal2Brightness.ShowWindow( SW_SHOW );
		if( CControlLayer::Instance().m_bHasBSD)
			BSD_ShowWnd(pfrm->m_hWnd, TRUE);
	}
	else
	{
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_ctrlSignal2Brightness.ShowWindow( SW_HIDE );
		if( CControlLayer::Instance().m_bHasBSD)
			BSD_ShowWnd(pfrm->m_hWnd, FALSE);
	}

	if( m_bSplitZoom )
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ShowWindow(SW_HIDE);
	else
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ShowWindow(SW_SHOW);

}

void CScanView::CheckCCDSwitch()
{
	if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd == NULL) return;

	// 17.09 CCD在看背散射像的时候不工作（J12要关）
	if( m_bSplit || m_bSplitZoom )
	{
		if( CControlLayer::Instance().m_BrdMCI.nChnIndex[0] > 0 || CControlLayer::Instance().m_BrdMCI.nChnIndex[1] > 0 )
		{
			// 分割像
			((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( FALSE );
		}
		//			else
		//				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( TRUE );
	}
	else
	{
		// 仅看其中一通道像
		if( m_bOnlyDet2 )
		{
			// 仅看第二通道，且第二通道显示的是背散射像或混合像
			if( CControlLayer::Instance().m_BrdMCI.nChnIndex[1] > 0 )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( FALSE );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( TRUE );
		}
		else
		{
			// 普通看像（看第一通道），且第一通道显示的是背散射像或混合像
			if( CControlLayer::Instance().m_BrdMCI.nChnIndex[0] > 0 )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( FALSE );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( TRUE );
		}
	}
}

void CScanView::OnUpdateScanTypeSplit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bSplit );
}

void CScanView::OnScanTypeSplitZoom() 
{
	if( !m_bSplitZoom )
		NotifyScanSplit( TRUE, !m_bSplitZoom );
	else
		NotifyScanSplit( m_bSplit, !m_bSplitZoom );
}

void CScanView::OnUpdateScanTypeSplitZoom(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bSplitZoom );
}

void CScanView::OnScanTypeDet2() 
{
	m_bOnlyDet2 = !m_bOnlyDet2;
	CControlLayer::Instance().CheckDet2nd( m_bOnlyDet2 );
	CheckCCDSwitch();
	if( m_bOnlyDet2 )
		theApp.DisplayMsg("Press scan type det 2: select 1,3,5 chn");
	else
		theApp.DisplayMsg("Press scan type det 2: select 0,2,4 chn");
}

void CScanView::OnUpdateScanTypeDet2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bOnlyDet2 );
	pCmdUI->Enable( !(m_bSplit || m_bSplitZoom) );
}

void CScanView::UpdatePhotoSn(int nSN)
{
	if ( nSN > 9999 ) nSN = 0;

	if( theApp.m_nPhotoSerialNumber != nSN )
	{
		theApp.m_nPhotoSerialNumber = nSN;
	
		theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nPhotoSerialNumber, nSN );
	}

	CString m_csPhotoSN;
	m_csPhotoSN.Format("SN:%04d", nSN);
	CSemVirtualMCICard::Instance().UpdatePhotoSN( m_csPhotoSN );
}

//调节器类型改变
void CScanView::OnAdjustorSortChanged(UINT nID)
{
	SyncAdjustorSort( nID );
}

void CScanView::OnUpdateAdjustorSortChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_nAdjustorSort == pCmdUI->m_nID -ID_ADJUSTOR_BEAM_ALIGNMENT );
}

//同步调节器类型改变
void CScanView::SyncAdjustorSort( UINT nNewVal )
{
	m_nAdjustorSort = nNewVal - ID_ADJUSTOR_BEAM_ALIGNMENT;
	if( (((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL)
		&& (((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PageDebug2.m_hWnd != NULL) )
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PageDebug2.UpdateAdjustor( m_nAdjustorSort );
	if( (((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL) )
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.UpdateAdjustor( m_nAdjustorSort );
}

//手动调节器类型改变
void CScanView::OnAdjustorManualSortChanged(UINT nID)
{
	SyncAdjustorManualSort( nID );
}

void CScanView::OnUpdateAdjustorManualSortChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( CControlLayer::Instance().m_nAdjustorManualSort == pCmdUI->m_nID -ID_ADJUSTOR_MANUAL_ObjectiveLens );
}

//同步手动调节器类型改变
void CScanView::SyncAdjustorManualSort( UINT nNewVal )
{
	CControlLayer::Instance().m_nAdjustorManualSort = nNewVal -ID_ADJUSTOR_MANUAL_ObjectiveLens;
	return;
	UINT nID = 10000;
	switch( CControlLayer::Instance().m_nAdjustorManualSort )
	{
	case	SORT_MANUAL_OBJECTIVE_FINE:
	case	SORT_MANUAL_OBJECTIVE_CORSE:
		nID = ID_ADJUSTOR_OBJECTIVE_LENS;
		break;
	case	SORT_MANUAL_CONDENSER_FINE:
	case	SORT_MANUAL_CONDENSER_CORSE:
		nID = ID_ADJUSTOR_CONDENSER_LENS;
		break;
	case	SORT_MANUAL_STIGMATOR_X:
	case	SORT_MANUAL_STIGMATOR_Y:
	case	SORT_MANUAL_STIGMATOR_X2:
	case	SORT_MANUAL_STIGMATOR_Y2:
		nID = ID_ADJUSTOR_STIGMATOR;
		break;
	case	SORT_MANUAL_BEAM_DISPLACEMENT_X:
	case	SORT_MANUAL_BEAM_DISPLACEMENT_Y:
		nID = ID_ADJUSTOR_BEAM_SHIFT;
		break;
	case	SORT_MANUAL_BEAM_ALIGNMENT_X:
	case	SORT_MANUAL_BEAM_ALIGNMENT_Y:
		nID = ID_ADJUSTOR_BEAM_ALIGNMENT;
		break;
	case	SORT_MANUAL_BRIGHT:
		nID = ID_ADJUSTOR_BRIGHTNESS;
		break;
	case	SORT_MANUAL_CONTRAST:
		nID = ID_ADJUSTOR_CONTRASTNESS;
		break;
	case	SORT_MANUAL_NULL:
		nID = ID_ADJUSTOR_STIGMATOR_ASSISTANT;	// Temp
		break;
	}
	SyncAdjustorSort( nID );
}

//聚光镜反转开关改变
void CScanView::OnAdjustorCondenserLensReverse() 
{
	SyncCondenserLensReverse( !m_bCondRev );
}

void CScanView::OnUpdateAdjustorCondenserLensReverse(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bCondRev );
}

void CScanView::SyncCondenserLensReverse( BOOL bNewVal )
{
	m_bCondRev = bNewVal;

	CSemCoreAssemble::Instance().m_CondenserLens.SetParams( SET_PARAMS_Reverse, m_bCondRev );	// 反转

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_CondenserLens;
	SerialNumber.lVal = -1;
	Xpos = CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
	Flag.lVal = 1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	if( m_bCondRev )
		theApp.DisplayMsg("Press CL reverse: ON");
	else
		theApp.DisplayMsg("Press CL reverse: OFF");
}

//物镜反转开关改变
void CScanView::OnAdjustorObjectiveLensReverse() 
{
	SyncObjectiveLensReverse( !m_bObjectiveRev );
}

void CScanView::OnUpdateAdjustorObjectiveLensReverse(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bObjectiveRev );
}

void CScanView::SyncObjectiveLensReverse( BOOL bNewVal )
{
	m_bObjectiveRev = bNewVal;

	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( SET_PARAMS_Reverse, m_bObjectiveRev );	// 反转

	if( m_bObjectiveRev )
		theApp.DisplayMsg("Press OL reverse: ON");
	else
		theApp.DisplayMsg("Press OL reverse: OFF");
}

//辅助消像散开关改变
void CScanView::OnStigmatorAssistant() 
{
	SyncAuxiliaryStigmation( !m_bAuxStig );
}

void CScanView::OnUpdateStigmatorAssistant(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bAuxStig );
}

void CScanView::SyncAuxiliaryStigmation( BOOL bNewVal )
{
	m_bAuxStig = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_AuxiliaryStigmation;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)m_bAuxStig;
	Flag.lVal = 1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//四舍五入函数
extern int RoundFunc( double dNum );

//快照
void CScanView::OnPhotoSnapshot() 
{
	BeginWaitCursor();
	CThumbImageItem* pImageItem = new CThumbImageItem();

	double dfMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	double dblHV = CSemCoreAssemble::Instance().m_HVAccelerate.GetPos().dblVal;

	pImageItem->m_nMag	= RoundFunc( dfMag );	//09.06.05
	pImageItem->m_nHV	= RoundFunc( dblHV );	//09.06.05

	const double dblDPI = 25.4 * CMCICardCommonSetting::Instance().m_sizeReso.cx / 127;

	HANDLE hImageLay = NULL;
	hImageLay = CSemVirtualMCICard::Instance().GetDib();
	if (hImageLay != NULL)
	{
		CxImage *newima = new CxImage();
		newima->CreateFromHANDLE(hImageLay);
		newima->SetXDPI((long)dblDPI);
		newima->SetYDPI((long)dblDPI);
		pImageItem->m_pImageLayer = newima;

		if( hImageLay != NULL )
		{
			GlobalUnlock( (HGLOBAL)hImageLay );
			GlobalFree( (HGLOBAL)hImageLay );
			hImageLay = NULL;
		}
	}

	CBitmap bmpScreen;
	if ( CSemVirtualMCICard::Instance().CopyScreenBmp(bmpScreen) )
	{
		CxImage *newima = new CxImage();
		newima->CreateFromHBITMAP( (HBITMAP)bmpScreen );
		newima->SetXDPI((long)dblDPI);
		newima->SetYDPI((long)dblDPI);
		pImageItem->m_pImageSrc = newima;

		bmpScreen.DeleteObject();
	}

	if( pImageItem->GetImage() != NULL )
	{
		// 2021.06 原始数据存储
		CSize& m_sizeReso = CMCICardCommonSetting::Instance().m_sizeReso;
		pImageItem->m_pPixZsrc	= new WORD[m_sizeReso.cx *m_sizeReso.cy];
		ZeroMemory(pImageItem->m_pPixZsrc,	m_sizeReso.cx *m_sizeReso.cy * sizeof(WORD));
		if( m_bSplit )
		{
			pImageItem->m_pPixZsrc2	= new WORD[m_sizeReso.cx *m_sizeReso.cy];
			ZeroMemory(pImageItem->m_pPixZsrc,	m_sizeReso.cx *m_sizeReso.cy * sizeof(WORD));
		}
		CSemVirtualMCICard::Instance().GetImageDataSrc( 0, pImageItem->m_pPixZsrc );		// 第一通道数据
		if( m_bSplit )
			CSemVirtualMCICard::Instance().GetImageDataSrc( 1, pImageItem->m_pPixZsrc2 );	// 第二通道数据

		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.AddItem( pImageItem );
	}

	EndWaitCursor();
	theApp.DisplayMsg("Press photo snapshot");
}

//照相预览
void CScanView::OnPhotoPreview() 
{
	m_bPreview = !m_bPreview;
	if( m_bPreview )
	{
		CSemVirtualMCICard::Instance().UpdateScreenMag( CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal, TRUE );
		m_bZoomFit = TRUE;
		ScreenZoom();
		CSemVirtualMCICard::Instance().SetPhotoPreview( TRUE );
		theApp.DisplayMsg("Press photo preview: ON");
	}
	else
	{
		ScreenZoom();
		CSemVirtualMCICard::Instance().UpdateScreenMag( CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal );
		CSemVirtualMCICard::Instance().SetPhotoPreview( FALSE );
		theApp.DisplayMsg("Press photo preview: OFF");
	}
}

void CScanView::OnUpdatePhotoPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bPreview );
}

//设置字符编辑开关
void CScanView::OnAbc() 
{
	m_bAbc = !m_bAbc;
	if( m_bAbc )
		theApp.DisplayMsg("Press ABC: ON");
	else
		theApp.DisplayMsg("Press ABC: OFF");
}

void CScanView::OnUpdateAbc(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bAbc );
}

//设置底标显示开关
void CScanView::OnLabel() 
{
	SyncLabel( !m_bLabel );
}

void CScanView::OnUpdateLabel(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bLabel );
}

void CScanView::SyncLabel( BOOL bNewVal )
{
	if( m_bLabel == bNewVal )
		return;
	m_bLabel = bNewVal;
	CSemVirtualMCICard::Instance().ShowLabel( bNewVal );
	CSemVirtualMCICard::Instance().ShowHeader( bNewVal );

	if( m_bLabel )
		theApp.DisplayMsg("Press Label: ON");
	else
		theApp.DisplayMsg("Press Label: OFF");
}

//设置移动标尺显示开关
void CScanView::OnRuler() 
{
	m_bUScale = !m_bUScale;
	
	CSemVirtualMCICard::Instance().ShowMobileUScale( m_bUScale );

	if( m_bUScale )
		theApp.DisplayMsg("Press ruler: ON");
	else
		theApp.DisplayMsg("Press ruler: OFF");
	// 2005-1-17_放大倍数XY联合校正
	UpdateUScaleXY();
	InvalidateRect(NULL);
}

void CScanView::OnUpdateRuler(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bUScale );
}

// 16.06.12 多移动标尺实验
void CScanView::OnRulerAdd() 
{
	// TODO: Add your command handler code here
	m_ptMobileUScaleArray[m_nMobileUScaleCounts]		= CPoint(0,50);
	m_ptMobileUScaleArray2[m_nMobileUScaleCounts]		= CPoint(50,50);
	m_nMobileUScalePixelNumArray[m_nMobileUScaleCounts]	= 50;
	CSemVirtualMCICard::Instance().SetMobileUScalePosArray( m_nMobileUScaleCounts, m_ptMobileUScaleArray[m_nMobileUScaleCounts], m_ptMobileUScaleArray2[m_nMobileUScaleCounts], RGB(255, 0, 0) );
	CString csMobileUScaleText = CSemVirtualMCICard::Instance().CalculateMobileUScale(50);
	CSemVirtualMCICard::Instance().UpdateUScaleArray(m_nMobileUScaleCounts, 50, csMobileUScaleText);
	m_nMobileUScaleCounts++;
}

void CScanView::OnUpdateRulerAdd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_nMobileUScaleCounts < MobileUScaleCounts );
}
// 16.06.12 多移动标尺实验

// 2023.07 角度测量
void CScanView::OnAngle() 
{
	m_bAngle	= !m_bAngle;

	CSemVirtualMCICard::Instance().ShowAngle( m_bAngle );

	if( m_bAngle )
		theApp.DisplayMsg("Press angle: ON");
	else
		theApp.DisplayMsg("Press angle: OFF");
	InvalidateRect(NULL);
}

void CScanView::OnUpdateAngle(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bAngle );
}

void CScanView::OnAngleAdd() 
{
	m_bAddAngle	= !m_bAddAngle;
	if( m_bAddAngle )
		AngleAddNew();
	else
	{
		POSITION pos = m_plistAngle->GetTailPosition();
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		CSemVirtualMCICard::Instance().ChangeAngle( pAngle->nIndex, FALSE );
		m_plistAngle->RemoveAt( pos );
		delete pAngle;
		m_nAngleIndex--;
		m_nAngleCounts = m_plistAngle->GetCount();
	}
}

void CScanView::OnUpdateAngleAdd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bAddAngle );
	pCmdUI->Enable( m_bAngle && m_plistAngle->GetCount() <= AngleMeasureCounts );
}

void CScanView::AngleAddNew()
{
	if( m_plistAngle->GetCount() > AngleMeasureCounts -1 )
	{
		m_bAddAngle = FALSE;
		return;
	}
	AngleMeasure* pAngle = new AngleMeasure();
	ZeroMemory( pAngle, sizeof(AngleMeasure) );
	pAngle->AO				= m_AngleOption;
	pAngle->nAngleStatus	= 1;	// 生成
	pAngle->nIndex			= m_nAngleIndex;
	m_plistAngle->AddTail( pAngle );
	CSemVirtualMCICard::Instance().ChangeAngle( m_nAngleIndex, TRUE );
	m_nAngleCounts = m_plistAngle->GetCount();
	m_nAngleIndex++;

	if( m_nAngleIndex > 1000 )
	{
		// Re-arrange angle list
		int nIndex = 0;
		POSITION pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			pAngle->nIndex = nIndex++;
			m_plistAngle->GetNext( pos );
		}
		m_nAngleCounts = m_plistAngle->GetCount();
		CSemVirtualMCICard::Instance().ReArrangeAngles();
		m_nAngleIndex = nIndex;
	}
}

void CScanView::OnAngleOptions()
{
	BOOL bUpdateAll = FALSE;
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->bAngleHitTest )
		{
			CDlgAngleOptions dlgAO;
			dlgAO.m_AO = pAngle->AO;
			if( dlgAO.DoModal() == IDOK )
			{
				if( pAngle->AO.clrLine != dlgAO.m_AO.clrLine )
					pAngle->AO.clrLine = dlgAO.m_AO.clrLine;
				if( pAngle->AO.clrArc != dlgAO.m_AO.clrArc )
					pAngle->AO.clrArc = dlgAO.m_AO.clrArc;
				if( pAngle->AO.clrText != dlgAO.m_AO.clrText )
					pAngle->AO.clrText = dlgAO.m_AO.clrText;
				if( pAngle->AO.nSize != dlgAO.m_AO.nSize )
					pAngle->AO.nSize = dlgAO.m_AO.nSize;
				CSemVirtualMCICard::Instance().SetAngleParam( pAngle->nIndex, pAngle );
				if( dlgAO.m_bSetDefault )
				{
					bUpdateAll = TRUE;
					m_AngleOption = dlgAO.m_AO;
				}
			}
			pAngle->bAngleHitTest = FALSE;
			break;
		}
		m_plistAngle->GetNext( pos );
	}
	// 设置为默认属性
	if( bUpdateAll )
	{
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleLine]	= m_AngleOption.clrLine;
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleArc]	= m_AngleOption.clrArc;
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleText]	= m_AngleOption.clrText;
		CSemVirtualMCICard::Instance().SetColor( SysOpt_clrAngleLine, m_AngleOption.clrLine );
		CSemVirtualMCICard::Instance().SetColor( SysOpt_clrAngleArc, m_AngleOption.clrArc );
		CSemVirtualMCICard::Instance().SetColor( SysOpt_clrAngleText, m_AngleOption.clrText );

		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");
		CCommonFunctionManager::Instance().SaveInOneLine( path, "Options", "Colors", LONGSTYLE, 13, CAppOption::Instance().m_sysOpt.color );

		pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			pAngle->AO = m_AngleOption;
			CSemVirtualMCICard::Instance().SetAngleParam( pAngle->nIndex, pAngle );
			m_plistAngle->GetNext( pos );
		}
	}
}

void CScanView::OnAngleDelete()
{
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->bAngleHitTest )
		{
			// 有要删除的
			CSemVirtualMCICard::Instance().ChangeAngle( pAngle->nIndex, FALSE );
			m_plistAngle->RemoveAt( pos );
			delete pAngle;
			break;
		}
		m_plistAngle->GetNext( pos );
	}
	m_nAngleCounts = m_plistAngle->GetCount();
}

void CScanView::ResetAngleList()
{
	POSITION pos;
	while( (pos = m_plistAngle->GetHeadPosition()) )
	{
		AngleMeasure* pItem = m_plistAngle->GetAt( pos );
		m_plistAngle->RemoveHead();
		delete pItem;
	}
	m_nAngleCounts	= 0;
	m_nAngleIndex	= 0;
	m_bAddAngle		= FALSE;
}

//设置日期显示开关
void CScanView::OnDate() 
{
	SyncDate( !m_bDate );
	if( m_bDate )
	{
		theApp.DisplayMsg("Press date: ON");
		SetTimer( nAutoTimerDate, 1000*30, NULL );
	}
	else
	{
		KillTimer( nAutoTimerDate );
		theApp.DisplayMsg("Press date: OFF");
	}
}

void CScanView::OnUpdateDate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bDate );
}

void CScanView::SyncDate( BOOL bNewVal )
{
	m_bDate = bNewVal;
	CControlLayer::Instance().SyncDate(bNewVal);
}

//设置备注显示开关
void CScanView::OnNote() 
{
//	SyncDate( !m_bDate );
	m_bNote = !m_bNote;
	if( m_bNote )
	{
		theApp.DisplayMsg("Press note: ON");
	}
	else
	{
		theApp.DisplayMsg("Press note: OFF");
	}
	CSemVirtualMCICard::Instance().ShowNote( m_bNote );
}

void CScanView::OnUpdateNote(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bNote );
}

void CScanView::OnFOV() 
{
	m_bFOV = !m_bFOV;
	if( m_bFOV )
		theApp.DisplayMsg("Press FOV: ON");
	else
		theApp.DisplayMsg("Press FOV: OFF");
	CSemVirtualMCICard::Instance().ShowHeader( m_bFOV );
}

void CScanView::OnUpdateFOV(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bFOV );
}

void CScanView::OnPixelSize() 
{
	m_bPixelSize = !m_bPixelSize;
	if( m_bPixelSize )
		theApp.DisplayMsg("Press PixelSize: ON");
	else
		theApp.DisplayMsg("Press PixelSize: OFF");
	CSemVirtualMCICard::Instance().ShowPixelSize( m_bPixelSize );
}

void CScanView::OnUpdatePixelSize(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bPixelSize );
}


void CScanView::OnVideoPro() 
{
	//打开视频处理器对话框
//	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	pfrm->m_MainPanel.SelchangePage( 1 );
}

//设置物镜摇摆开关
void CScanView::OnWobbler() 
{
	SyncWobbler( !m_bWobbler );
	NotifyWobbler( m_bWobbler );
}

void CScanView::OnUpdateWobbler(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bWobbler );
}

void CScanView::NotifyWobbler( BOOL bNewVal )
{
	VARIANT ClassID, SerialNumber, Xpos, Flag;
	ClassID.lVal = SemControlPanelClass_DObjectiveLensSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
}

void CScanView::SyncWobbler( BOOL bNewVal )
{
	m_bWobbler = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_DObjectiveLensSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//设置倾斜补偿开关
void CScanView::OnLeanCompensate() 
{
	SyncLean( !m_bLean );
	NotifyLean( m_bLean );
}

void CScanView::OnUpdateLeanCompensate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bLean );
}

void CScanView::NotifyLean( BOOL bNewVal )
{
	VARIANT ClassID, SerialNumber, Xpos, Flag;
	ClassID.lVal = SemControlPanelClass_YAdjust;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
}

void CScanView::SyncLean( BOOL bNewVal )
{
	m_bLean = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_YAdjust;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//设置动态聚焦开关
void CScanView::OnDynamicFocus() 
{
	SyncDynFocus( !m_bDynFocus );
	NotifyDynFocus( m_bDynFocus );
}

void CScanView::OnUpdateDynamicFocus(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bDynFocus );
}

void CScanView::NotifyDynFocus( BOOL bNewVal )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_DFocusSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

void CScanView::SyncDynFocus( BOOL bNewVal )
{
	m_bDynFocus = bNewVal;
}

//设置通道花样开关
void CScanView::OnSACP() 
{
	if( theApp.m_blSACPState )				// 09.01.15 S.A.C.P.功能默认状态为不启用
	{
		SyncSacp( !m_bSacp );
		NotifySacp( m_bSacp );
	}
}

void CScanView::OnUpdateSACP(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( theApp.m_blSACPState );	// 09.01.15 S.A.C.P.功能默认状态为不启用
	pCmdUI->SetCheck( m_bSacp );
}

void CScanView::NotifySacp( BOOL bNewVal )
{
	bNewVal;
}

void CScanView::SyncSacp( BOOL bNewVal )
{
	m_bSacp = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_SACP;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//Robinson探头
void CScanView::OnRobinsonDetector() 
{
	SyncRobinsonDetector( !m_bRobinson );
	NotifyRobinsonDetector( m_bRobinson );
}

void CScanView::OnUpdateRobinsonDetector(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bRobinson );
}

void CScanView::NotifyRobinsonDetector( BOOL bNewVal )
{
	VARIANT ClassID, SerialNumber, Xpos, Flag;
	ClassID.lVal = SemControlPanelClass_RobinsonDetectorSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
}

void CScanView::SyncRobinsonDetector( BOOL bNewVal )
{
	m_bRobinson = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_RobinsonDetectorSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//设置示波器开关
void CScanView::OnOscillograph() 
{
	SyncOscillograph( !m_bOscillograph );
}

void CScanView::OnUpdateOscillograph(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bOscillograph );
}

void CScanView::SyncOscillograph( BOOL bNewVal )
{
	m_bOscillograph = bNewVal;

	CSemVirtualMCICard::Instance().ShowOscillograph( m_bOscillograph );

	if( m_bOscillograph )
		theApp.DisplayMsg("Press OSC: ON");
	else
		theApp.DisplayMsg("Press OSC: OFF");
}

//标准框显示开关
void CScanView::OnFrameRate() 
{
	// TODO: Add your command handler code here
	SyncFrameRate( !m_bFrameRate );
}

void CScanView::OnUpdateFrameRate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bFrameRate );
}

void CScanView::SyncFrameRate( BOOL bNewVal )
{
	m_bFrameRate = bNewVal;
//	CSemVirtualMCICard::Instance().ShowFrameRate( m_bFrameRate );

	// 2005-1-17_放大倍数XY联合校正
	theApp.m_boolShowUScaleXY = m_bFrameRate; 
	UpdateUScaleXY();

	if( m_bFrameRate )
		theApp.DisplayMsg("Press frame: ON");
	else
		theApp.DisplayMsg("Press frame: OFF");
}

//标准图样
void CScanView::OnStandImage() 
{
	StandImage( 1 );
}

void CScanView::StandImage( int nDelt )
{
	// TODO: Add your command handler code here
	static int		nStyle = 0;

	nStyle += nDelt;
	switch ( nStyle )
	{
	case 0:
		ImageStandStyle0();
		break;

	case 1:
		ImageStandStyle1();
		break;
	
	case 2:
		ImageStandStyle2();
		break;
	
	case 3:
		ImageStandStyle3();
		break;

	case 4:
		ImageStandStyle4();
		break;

	case 5:
		ImageStandStyle5();
		break;

	case 6:
		ImageStandStyle6();
		break;

	case 7:
		ImageStandStyle7();
		break;

	case 8:
		ImageStandStyle8();
		break;

	case 9:
		ImageStandStyle9();
		break;

	case 10:
		ImageStandStyle10();
		break;

	case 11:
		ImageStandStyle11();
		break;

	case 12:
		ImageStandStyle12();
		break;

	case 13:
		ImageStandStyle13();
		break;

	default:
		if ( nStyle < 0 )
		{
			nStyle = 13;
			ImageStandStyle13();
		}
		else
		{
			nStyle = 0;
			ImageStandStyle0();
		}
		break;
	}
}

void CScanView::OnOptionsSetup() 
{
	// 打开设置对话框，包括聚光镜、物镜步距设置、反转开关设置；
	//					  鼠标速度调节设置；
	//					  扫描档数对应实际扫描速度设置等
	// 所有不太常用的设置均可在该对话框中添加
	// 扫描速度档
	CSheetSetup sheetSetup( IDS_SD_TITLE_PARAM_SETUP );
	CPageScanPhoto* pPageScanPhoto = (CPageScanPhoto*)sheetSetup.GetPage(0);
	pPageScanPhoto->m_nPixRateIndex1Bak = CScanParameterManager::Instance().m_nPixRateIndex[1] /2;
	pPageScanPhoto->m_nPixRateIndex2Bak = CScanParameterManager::Instance().m_nPixRateIndex[2] /2;
	pPageScanPhoto->m_nPixRateIndex3Bak = CScanParameterManager::Instance().m_nPixRateIndex[3] /2;
	pPageScanPhoto->m_nPixRateIndex4Bak = CScanParameterManager::Instance().m_nPixRateIndex[4] /2;
	// 照相速度
	switch( CScanParameterManager::Instance().m_nusbPhotoPixRate )
	{
	case	64:		// 32us
		pPageScanPhoto->m_nCameraPixRateIndexbak = 0;
		break;
	case	128:	// 64us
		pPageScanPhoto->m_nCameraPixRateIndexbak = 1;
		break;
	case	256:	// 128us
		pPageScanPhoto->m_nCameraPixRateIndexbak = 2;
		break;
	case	512:	// 256us
		pPageScanPhoto->m_nCameraPixRateIndexbak = 3;
		break;
	}
	pPageScanPhoto->m_nChn1 = CControlLayer::Instance().m_BrdMCI.nChnIndex[0];
	pPageScanPhoto->m_nChn2 = CControlLayer::Instance().m_BrdMCI.nChnIndex[1];
	pPageScanPhoto->m_nSEratio = CScanParameterManager::Instance().m_nSEratio -1;

	// 其它
	CPageLangOther* pPageLangOther = (CPageLangOther*)sheetSetup.GetPage(1);
	if( m_wLanguageID == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED) )
		pPageLangOther->m_nLanguage = 0;
	else if( m_wLanguageID == MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT) )
		pPageLangOther->m_nLanguage = 1;
	else
		pPageLangOther->m_nLanguage = 2;
	// 打开图像窗口标志
	pPageLangOther->m_bOpenImageAfterSnap = m_bOpenImageAfterSnap;
	// 开机自动存储所有日志
	pPageLangOther->m_bLogAll = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, 
								g_csRegEntry_nLogAll, 1 );
	// 显示屏幕放大倍数
	pPageLangOther->m_bShowScreenMag = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showScrMag];
	// 显示日期
	pPageLangOther->m_bShowDate = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showDate];
	// 显示备注
	pPageLangOther->m_bShowNote = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showNote];
	// 显示CCD Camera窗口
	pPageLangOther->m_bShowCCD = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showCCD];
	// 优先存储格式10.07.02
	pPageLangOther->m_nPriorSaveType = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, 
								g_csRegEntry_nPriorSaveType, 2 );

	sheetSetup.DoModal();
}

//////////////////////////////////////////////////////////////////////////////
LRESULT CScanView::OnParamChanged(WPARAM wParam, LPARAM lParam)
{
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\KYKYcfg.ini");

	CString str;
	if( lParam == 0 )
	{
		CPageScanPhoto* pPageScanPhoto = (CPageScanPhoto*)wParam;

		// 将扫描速度写入配置文件
		CScanParameterManager::Instance().SavePixRate2INI(path,pPageScanPhoto->m_nPixRateIndex1,pPageScanPhoto->m_nPixRateIndex2,
			pPageScanPhoto->m_nPixRateIndex3,pPageScanPhoto->m_nPixRateIndex4);
	
		// 强制刷新
		bool bNotifyPixRate = CScanParameterManager::Instance().NotifyPixRate();
		if (bNotifyPixRate)
		{
			CString str;
			str.Format( "Press select speed to %d us/s", CScanParameterManager::Instance().m_nusbPixRate /2 );
			theApp.DisplayMsg(str);

			if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
				if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_AnalogPhoto )
					USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
		}

		// 照相速度
		switch( pPageScanPhoto->m_nCameraPixRateIndex )
		{
		case	0:
			CScanParameterManager::Instance().m_nusbPhotoPixRate = 64;	// 32us
			break;
		case	1:
			CScanParameterManager::Instance().m_nusbPhotoPixRate = 128;	// 64us
			break;
		case	2:
			CScanParameterManager::Instance().m_nusbPhotoPixRate = 256;	// 128us
			break;
		case	3:
			CScanParameterManager::Instance().m_nusbPhotoPixRate = 512;	// 256us
			break;
		}
		// 将照相速度写入配置文件
		CString str;
		str.Format( "%d", CScanParameterManager::Instance().m_nusbPhotoPixRate );
		WritePrivateProfileString( "Options", "PhotoPixRate", str, path );

		// 分辨率
		CScanParameterManager::Instance().m_nResoX[4] = pPageScanPhoto->m_nResoX1 + CScanParameterManager::Instance().m_nResosIndex[0];
		CScanParameterManager::Instance().m_nResoX[5] = pPageScanPhoto->m_nResoX2 + CScanParameterManager::Instance().m_nResosIndex[0];
		CScanParameterManager::Instance().m_nResoX[6] = pPageScanPhoto->m_nResoX3 + CScanParameterManager::Instance().m_nResosIndex[0];
		CScanParameterManager::Instance().m_nResoX[7] = pPageScanPhoto->m_nResoX4 + CScanParameterManager::Instance().m_nResosIndex[0];
		// 将分辨率写入配置文件
		CCommonFunctionManager::Instance().SaveInOneLine( path, "Options", "ResoX", INTSTYLE, 8, CScanParameterManager::Instance().m_nResoX );
 		// 强制刷新
		if( CScanParameterManager::Instance().m_dResoRate < 0 )
		{
			if( CMCICardCommonSetting::Instance().m_sizeReso.cx != CScanParameterManager::Instance().m_nResos[CScanParameterManager::Instance().m_nResoX[CScanParameterManager::Instance().m_nusbScanResoIndex]]
			|| CMCICardCommonSetting::Instance().m_sizeReso.cy != CScanParameterManager::Instance().m_nResos[CScanParameterManager::Instance().m_nResoY[CScanParameterManager::Instance().m_nusbScanResoIndex]] )
			{
				NotifyScanResolution( CScanParameterManager::Instance().m_nusbScanResoIndex, TRUE );
			}
		}
		else
		{
			if( CMCICardCommonSetting::Instance().m_sizeReso.cx != CScanParameterManager::Instance().m_nResos[CScanParameterManager::Instance().m_nResoX[CScanParameterManager::Instance().m_nusbScanResoIndex]]
			|| CMCICardCommonSetting::Instance().m_sizeReso.cy != CScanParameterManager::Instance().m_nResos[CScanParameterManager::Instance().m_nResoX[CScanParameterManager::Instance().m_nusbScanResoIndex]] *CScanParameterManager::Instance().m_dResoRate )
			{
 				NotifyScanResolution( CScanParameterManager::Instance().m_nusbScanResoIndex, TRUE );
			}
		}

		// 通道参数
		if( (CControlLayer::Instance().m_BrdMCI.nChnIndex[0] != pPageScanPhoto->m_nChn1)
			|| (CControlLayer::Instance().m_BrdMCI.nChnIndex[1] != pPageScanPhoto->m_nChn2) )
		{
			CControlLayer::Instance().m_BrdMCI.nChnIndex[0]	= pPageScanPhoto->m_nChn1;
			CControlLayer::Instance().m_BrdMCI.nChnIndex[1]	= pPageScanPhoto->m_nChn2;
			// 先停止扫描，更新完参数后再重新发起扫描
			if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
			{
				USB_ScanType nScanType = CScanParameterManager::Instance().m_nusbScanType;
				USB_ChangeScan( USB_ST_Stop );
				CSemVirtualMCICard::Instance().SetParams( 11, CControlLayer::Instance().m_BrdMCI.nChnIndex[0] );
				CSemVirtualMCICard::Instance().SetParams( 12, CControlLayer::Instance().m_BrdMCI.nChnIndex[1] );
				CheckCCDSwitch();
				USB_ChangeScan( nScanType );
			}
			else
			{
				CSemVirtualMCICard::Instance().SetParams( 11, CControlLayer::Instance().m_BrdMCI.nChnIndex[0] );
				CSemVirtualMCICard::Instance().SetParams( 12, CControlLayer::Instance().m_BrdMCI.nChnIndex[1] );
				CheckCCDSwitch();
			}
		}
		if( CScanParameterManager::Instance().m_nSEratio != pPageScanPhoto->m_nSEratio +1 )
		{
			CScanParameterManager::Instance().m_nSEratio = pPageScanPhoto->m_nSEratio +1;
			CSemVirtualMCICard::Instance().SetParams( 17, CScanParameterManager::Instance().m_nSEratio );
		}
//		str.Format( "%s,%s", CControlLayer::Instance().m_BrdMCI.strChnName[0], CControlLayer::Instance().m_BrdMCI.strChnName[1] );
		str.Format( "%s,%s,%s,%s,%s,%s", CControlLayer::Instance().m_BrdMCI.strChnName[0], CControlLayer::Instance().m_BrdMCI.strChnName[1],
									CControlLayer::Instance().m_BrdMCI.strChnName[0], CControlLayer::Instance().m_BrdMCI.strChnName[3],
									CControlLayer::Instance().m_BrdMCI.strChnName[0], CControlLayer::Instance().m_BrdMCI.strChnName[1] );
		WritePrivateProfileString( "Options", "ChnNames", str, path );
		str.Format( "%d,%d", CControlLayer::Instance().m_BrdMCI.nChnIndex[0], CControlLayer::Instance().m_BrdMCI.nChnIndex[1] );
		WritePrivateProfileString( "Options", "ChnIndex", str, path );
		str.Format( "%d", CScanParameterManager::Instance().m_nSEratio );
		WritePrivateProfileString( "Options", "SEratio", str, path );
	}
	else if( lParam == 1 )
	{
		CPageLangOther* pPageLangOther = (CPageLangOther*)wParam;
		// 语言
		WORD lid = MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
		if( pPageLangOther->m_nLanguage == 0 )
			lid = MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
		else if( pPageLangOther->m_nLanguage == 1 )
			lid = MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
		else if( pPageLangOther->m_nLanguage == 2 )
			lid = MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT);
		if( CGlobalParamSetting::Instance().m_wLanguageID != lid )
		{
			CGlobalParamSetting::Instance().m_wLanguageID = lid;
//			::SetThreadLocale( MAKELCID( theApp.m_wLanguageID, SORT_DEFAULT ) );
			theApp.m_LanguageManagerCtrl.SetLanguageID( CGlobalParamSetting::Instance().m_wLanguageID );
			theApp.WriteProfileInt( g_csSettingSection, 
							g_csRegEntry_nLanguage, CGlobalParamSetting::Instance().m_wLanguageID );
			// 即时生效
			theApp.Localize();
		}
		// 打开图像窗口标志
		m_bOpenImageAfterSnap = pPageLangOther->m_bOpenImageAfterSnap;
		// 开机自动存储所有日志
		theApp.WriteProfileInt( g_csSettingSection,
								g_csRegEntry_nLogAll, pPageLangOther->m_bLogAll );
		// 显示屏幕放大倍数
		int nShowScreenMag = CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showScrMag];
		CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showScrMag] = pPageLangOther->m_bShowScreenMag;
		if( nShowScreenMag != CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showScrMag] )
			CSemVirtualMCICard::Instance().UpdateScreenMag( CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal );
		// 显示日期
		CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showDate] = pPageLangOther->m_bShowDate;
		if( CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showDate] != m_bDate )
			OnDate();
		// 显示备注
		CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showNote] = pPageLangOther->m_bShowNote;
		if( CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showNote] != m_bNote )
			OnNote();
		// 显示CCD Camera窗口
		CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showCCD] = pPageLangOther->m_bShowCCD;
		str.Format( "%d,%d,%d,%d,%d,%d,%d", CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showScrMag],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showDate],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showNote],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showCCD],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showFOV],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showPixeSize] );
		WritePrivateProfileString( "Options", "ShowParams", str, path );
		// 优先存储格式10.07.02
		theApp.m_nPriorSaveType = pPageLangOther->m_nPriorSaveType;
		theApp.WriteProfileInt( g_csSettingSection,
								g_csRegEntry_nPriorSaveType, theApp.m_nPriorSaveType );
		// 端口设置
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			CHVPowerDataManager::Instance().m_CPS3603s.nPort = pPageLangOther->m_nPortHV;
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			theApp.m_cfgWSM30KV.nPort = pPageLangOther->m_nPortHV +1;
		if( CSemCtrl::Instance().m_nVACBoardType < 1 )
		{
			CVACDataManager::Instance().m_cfgVacCCG.nPort = pPageLangOther->m_nPortCCG +1;
			str.Format( "%d,%d", pPageLangOther->m_nPortHV +1, CVACDataManager::Instance().m_cfgVacCCG.nPort );
		}
		else
		{
			CVACDataManager::Instance().m_portBoard.nPort = pPageLangOther->m_nPortCCG +1;
			str.Format( "%d,%d", pPageLangOther->m_nPortHV +1, CVACDataManager::Instance().m_portBoard.nPort );
		}
		WritePrivateProfileString( "Options", "CommPorts", str, path );
		if( CSemCtrl::Instance().m_nVACBoardType > 1 )
		{
			CVACDataManager::Instance().m_cfgVacIPG.nPort = pPageLangOther->m_nPortIPG +1;
			str.Format( "%d", CVACDataManager::Instance().m_cfgVacIPG.nPort );
			WritePrivateProfileString( "FEGParam", "IPGport", str, path );
		}
	}
	else if( lParam == 2 )
	{
		CPageSys* pPageSys = (CPageSys*)wParam;
		// 颜色
		for(int i=0; i<10; i++ )
		{
			CAppOption::Instance().m_sysOpt.color[i]	= pPageSys->m_sysOpt.color[i];
			CSemVirtualMCICard::Instance().SetColor( i, pPageSys->m_sysOpt.color[i] );
		}
		CCommonFunctionManager::Instance().SaveInOneLine( path, "Options", "Colors", LONGSTYLE, 13, CAppOption::Instance().m_sysOpt.color );
		// 背景透明
		CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans]	= pPageSys->m_sysOpt.nShowParams[SysOpt_showLabelTrans];
		CSemVirtualMCICard::Instance().SetTransparent( CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans] );
		str.Format( "%d,%d,%d,%d,%d,%d,%d", CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showScrMag],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showDate],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showNote],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showCCD],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showFOV],
										CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showPixeSize] );
		WritePrivateProfileString( "Options", "ShowParams", str, path );
		// 线宽
		CAppOption::Instance().m_sysOpt.nLineWidth		= pPageSys->m_sysOpt.nLineWidth;
		CSemVirtualMCICard::Instance().SetParams( 10, CAppOption::Instance().m_sysOpt.nLineWidth );
		str.Format( "%d", CAppOption::Instance().m_sysOpt.nLineWidth );
		WritePrivateProfileString( "Options", "LineWidth", str, path );
/*		// 字体
		for(int i=0; i<3; i++ )
		{
			CAppOption::Instance().m_sysOpt.fName[i]	= pPageSys->m_sysOpt.fName[i];
			CSemVirtualMCICard::Instance().SetFontName( i, pPageSys->m_sysOpt.fName[i] );
		}
		str.Format( "%s,%s,%s", CAppOption::Instance().m_sysOpt.fName[0], CAppOption::Instance().m_sysOpt.fName[1], CAppOption::Instance().m_sysOpt.fName[2] );
		WritePrivateProfileString( "Options", "FontNames", str, path );
		// 字体大小
		for(int i=0; i<3; i++ )
		{
			CAppOption::Instance().m_sysOpt.fSize[i]	= pPageSys->m_sysOpt.fSize[i];
			CSemVirtualMCICard::Instance().SetFontSize( i, pPageSys->m_sysOpt.fSize[i] );
		}
		CCommonFunctionManager::Instance().SaveInOneLine( path, "Options", "FontSizes", INTSTYLE, 3, CAppOption::Instance().m_sysOpt.fSize );
*/	}
	return 0;
}

void CScanView::OnOptionsListenStart() 
{
	// TODO: Add your command handler code here
	ListenStart();
}

void CScanView::ListenStart()
{
	m_pcommServer = new CTCPServer(this->m_hWnd);
/*	m_pcommSendImgData = new CTCPServer(this->m_hWnd);
	m_pcommServer->SetPort(KYKYComm_ListenPort1);
	m_pcommSendImgData->SetPort(KYKYComm_ListenPort2);

	if (m_pcommServer->StartListen())
	{
		if (m_pcommSendImgData->StartListen())
		{
			m_bCommListenStarted = TRUE;
		}
	}
*/
	if( m_pcommServer->StartListen(TRUE) )		// cmd thread
	{
		if( m_pcommServer->StartListen(FALSE) )	// img thread
			m_bCommListenStarted = TRUE;
	}
}

void CScanView::OnUpdateOptionsListenStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( !m_bCommListenStarted );
	pCmdUI->SetCheck( m_bCommListenStarted );
}

void CScanView::OnOptionsListenStop() 
{
	// TODO: Add your command handler code here
	KYKYComm_EndConnectThread();
	if( m_pcommServer != NULL )
	{
		delete m_pcommServer;
		m_pcommServer = NULL;
	}
/*	if (m_pcommSendImgData != NULL)
	{
		delete m_pcommSendImgData;
		m_pcommSendImgData = NULL;
	}
*/	m_bCommListenStarted = FALSE;
}

void CScanView::OnUpdateOptionsListenStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_bCommListenStarted );
}

LRESULT CScanView::OnKYKYComm_ConnectThread(WPARAM wParam, LPARAM lParam)
{
	CWinThread* CmdThdBak = m_commCmdThd;
	if( wParam < 1 )
	{
		m_commCmdThd	= (CWinThread*)lParam;		// 命令线程建立
		theApp.DisplayMsg( "connect thread cmd" );
	}
	else
	{
		m_commSendImageThd = (CWinThread*)lParam;	// 图像数据线程建立		
		theApp.DisplayMsg( "connect thread img" );
	}
	if( CmdThdBak == NULL && m_commCmdThd != NULL )
	{
		KYKYComm_SendSEMType();
		KYKYComm_SendHVType();
		KYKYComm_SendBrt();
		KYKYComm_SendBrt2();
		KYKYComm_SendCst();
		KYKYComm_SendWD();
		KYKYComm_SendMag();
		KYKYComm_SendFOV();
		KYKYComm_SendMagRange();
		KYKYComm_SendStigX();
		KYKYComm_SendStigY();
		KYKYComm_SendSRotAngInit();
		KYKYComm_SendSRotAng();
		KYKYComm_SendHV();
		KYKYComm_SendBias();
		KYKYComm_SendFilament();
		KYKYComm_SendBeamCurrent();
		KYKYComm_SendEMI();
		KYKYComm_SendVacStatus();
		KYKYComm_SendGunStatus();
		KYKYComm_StageSendStatus();
		KYKYComm_StageSendRange();
		KYKYComm_StageSendPosition();
//		if( m_nCommTimerMode > 0 )
			SetTimer( 50, m_nCommTimerInteval, NULL );
	}
	return 0;	
}


LRESULT CScanView::OnKYKYComm_Connect(WPARAM wParam, LPARAM lParam)
{
	if( !m_commCmdThd )
		return 0;
	BYTE brecvCommand = BYTE(wParam);
	switch (brecvCommand)
	{
	case	KYKYComm_CMD_Sys_Connect_Success:
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )
		{
			// 因光栅尺参数是计算位置参数的基础，所以先传递
//			KYKYComm_StageSendGrating();
			m_szCommStageGrating.cx = CStageXYManager::Instance().m_Stage.xy.nGratingX;
			m_szCommStageGrating.cy = CStageXYManager::Instance().m_Stage.xy.nGratingY;
		}
		break;
	}
	return 0;	
}

LRESULT CScanView::OnKYKYComm_ReplyAsk(WPARAM wParam,LPARAM lParam)
{
	if( !m_commCmdThd )
		return 1;

	BYTE brecvCommand = BYTE(wParam);
	switch (brecvCommand)
	{
	case	KYKYComm_CMD_Sys_Disconnect:
		KYKYComm_EndConnectThread();
		break;

	case	KYKYComm_CMD_Func_AutoBC:
		FuncAutoBrightContrast();
		theApp.DisplayMsg( "Func AutoBC from third" );
		break;
	case	KYKYComm_CMD_Func_ShutdownBeam:
		{
			if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
			{
				theApp.DisplayMsg( "Shutdown Beam from third" );
				// Clear OFF and close V1
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.HVPower_Shutdownbeam();
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_btnV1.SetCheck(0);
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.VAC_CheckV1(0);
			}
		}
		break;

	case	KYKYComm_CMD_ScanCtrl_Freeze:
	case	KYKYComm_CMD_ScanCtrl_Normal:
	case	KYKYComm_CMD_ScanCtrl_Sync:
		{
			int nIndex = brecvCommand -KYKYComm_CMD_ScanCtrl_Freeze;
			int nCommand = ID_SCAN_TYPE_FREEZE;
			nCommand += nIndex;
			theApp.DisplayMsg( "Change Scan Type from third" );
			OnScanTypeChanged(nCommand);
		}
		break;

	case	KYKYComm_CMD_ScanCtrl_ActiveArea:
		{
			CScanParameterManager::Instance().m_rcAreaScanParam = *((CRect*)lParam);
			theApp.DisplayMsg( "Change Scan Type from third" );
			OnScanTypeChanged(ID_SCAN_TYPE_FREEZE +USB_ST_AreaScan);
		}
		break;
	case	KYKYComm_CMD_ScanCtrl_Line_HORZ:
		{
			CScanParameterManager::Instance().m_nLineScanParam = (long)lParam;
			theApp.DisplayMsg( "Change Scan Type from third" );
			OnScanTypeChanged(ID_SCAN_TYPE_FREEZE +USB_ST_LineScan_HORZ);
		}
		break;
	case	KYKYComm_CMD_ScanCtrl_Line_VERT:
		{
			CScanParameterManager::Instance().m_nLineScanParam = (long)lParam;
			theApp.DisplayMsg( "Change Scan Type from third" );
			OnScanTypeChanged(ID_SCAN_TYPE_FREEZE +USB_ST_LineScan_VERT);
		}
		break;
	case	KYKYComm_CMD_ScanCtrl_Spot:
		{
			CScanParameterManager::Instance().m_ptPointScanParam = *((CPoint*)lParam);
			theApp.DisplayMsg( "Change Scan Type from third" );
			OnScanTypeChanged(ID_SCAN_TYPE_FREEZE +USB_ST_SpotScan);
		}
		break;
	case	KYKYComm_CMD_ScanCtrl_IsStopScan:
		KYKYComm_SendIsStopScan();
		break;

	case	KYKYComm_CMD_Get_ScanCtrl_Speed:
		KYKYComm_SendSpeed();
		break;
	case	KYKYComm_CMD_Get_ScanCtrl_Reso:
		KYKYComm_SendReso();
		break;
	case	KYKYComm_CMD_Set_ScanCtrl_Speed:	// 设置速度档位
		{
			int nCommand = ID_SCAN_SPEED_1 +(int)lParam;
			theApp.DisplayMsg( "Change Scan Speed from third" );
			OnScanSpeedChanged(nCommand);
		}
		break;

	case	KYKYComm_CMD_Set_ScanCtrl_Reso:		// 设置分辨率档位
		{
			int nCommand = ID_SCAN_RESOLUTION_5 +(int)lParam;
			theApp.DisplayMsg( "Change Scan Reso from third" );
			OnScanResolutionChanged(nCommand);
		}
		break;

	case	KYKYComm_CMD_Sys_GetVersion:
		{
//			DWORD versionNumber;
//			versionNumber = MAKEWORD(1,1);   //(low,high) (sofware,hardware)
			WORD versionNumber;
			versionNumber = CProductVersionManager::Instance().GetVersionInfo();
			m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, wParam, versionNumber);
		}
		break;

	case	KYKYComm_CMD_Sys_GetSEMType:
		KYKYComm_SendSEMType();
		break;

	case	KYKYComm_CMD_Get_MagRange:
		KYKYComm_SendMagRange();
		break;
	case	KYKYComm_CMD_Get_Magnification:
		KYKYComm_SendMag();
		break;
	case	KYKYComm_CMD_Get_FieldOfView:
		KYKYComm_SendFOV();
		break;

	case	KYKYComm_CMD_Set_Magnification:
		{
			theApp.DisplayMsg( "Change Mag from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_Magnifier;
			Flag.lVal		= 1;
			Xpos.dblVal		= msg->fValue;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;

	case	KYKYComm_CMD_Get_ScanRotAngleInit:
		KYKYComm_SendSRotAngInit();
		break;
	case	KYKYComm_CMD_Get_ScanRotAngle:
		KYKYComm_SendSRotAng();
		break;
	case	KYKYComm_CMD_Set_ScanRotAngle:
		{
			theApp.DisplayMsg( "Change RotAngle from third" );
			KYKYCommMsg_SendDataC1i* msg = (KYKYCommMsg_SendDataC1i*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_ScanRotate;
			Flag.lVal		= 1;
			Xpos.lVal		= msg->nValue -m_nCommScanRotAngInit;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;

	case	KYKYComm_CMD_Get_WorkDistance:
		KYKYComm_SendWD();
		break;

	case	KYKYComm_CMD_Set_WorkDistance:
		{
			theApp.DisplayMsg( "Change WD from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			double dValue = msg->fValue;	// dWD
			CSemCoreAssemble::Instance().m_WD.GetParams( 0, dValue );	// dOL
			long lOL = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPosByValue( dValue );	// 由dOL得到lOL
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
			Flag.lVal		= 1;
			Xpos.lVal		= lOL;
			Ypos			= Xpos;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;

	case	KYKYComm_CMD_Get_Brightness:
		KYKYComm_SendBrt();
		break;

	case	KYKYComm_CMD_Get_Brightness2:
		KYKYComm_SendBrt2();
		break;

	case	KYKYComm_CMD_Set_Brightness:
		{
			theApp.DisplayMsg( "Change brt from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_Brightness;
			Flag.lVal		= 1;
			Xpos.lVal		= (int)(msg->fValue *2048 /100);
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;

	case	KYKYComm_CMD_Set_Brightness2:
		{
			theApp.DisplayMsg( "Change brt2 from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_Signal2Brightness;
			Flag.lVal		= 1;
			Xpos.lVal		= (int)(msg->fValue *2048 /100);
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;
		
	case	KYKYComm_CMD_Get_Contrast:
		KYKYComm_SendCst();
		break;

	case	KYKYComm_CMD_Set_Contrast:
		{
			theApp.DisplayMsg( "Change cst from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			Flag.lVal		= 1;
			if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			{
				ClassID.lVal	= SemControlPanelClass_Contrast;
				Xpos.lVal		= (int)(msg->fValue *255 /100);
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			}
			else
			{
				ClassID.lVal = SemControlPanelClass_SP_PMT;
				Xpos.lVal = (int)(msg->fValue *400 /100);
				Ypos = Xpos;
				if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
				{
					if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
						((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
					else
						((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
				}
			}
		}
		break;

	case	KYKYComm_CMD_Get_StigX:
		KYKYComm_SendStigX();
		break;
	case	KYKYComm_CMD_Get_StigY:
		KYKYComm_SendStigY();
		break;
	case	KYKYComm_CMD_Set_StigX:
		{
			theApp.DisplayMsg( "Change StigX from third" );
			KYKYCommMsg_SendDataC1i* msg = (KYKYCommMsg_SendDataC1i*)lParam;
			if( msg->nValue < 2048 && msg->nValue > -2048 )
			{
				VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
				SerialNumber.lVal = -1;
				ClassID.lVal	= SemControlPanelClass_Stigmator;
				Flag.lVal		= 1;
				Xpos.lVal		= msg->nValue;
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			}
		}
		break;
	case	KYKYComm_CMD_Set_StigY:
		{
			theApp.DisplayMsg( "Change StigY from third" );
			KYKYCommMsg_SendDataC1i* msg = (KYKYCommMsg_SendDataC1i*)lParam;
			if( msg->nValue < 2048 && msg->nValue > -2048 )
			{
				VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
				SerialNumber.lVal = -1;
				ClassID.lVal	= SemControlPanelClass_Stigmator;
				Flag.lVal		= 2;
				Ypos.lVal		= msg->nValue;
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			}
		}
		break;

	case	KYKYComm_CMD_Get_VacStatus:
		KYKYComm_SendVacStatus();
		break;
	case	KYKYComm_CMD_Get_GunStatus:
		KYKYComm_SendGunStatus();
		break;

	case	KYKYComm_CMD_Sys_GetHighVoltageType:
		KYKYComm_SendHVType();
		break;
	case	KYKYComm_CMD_Get_HighVoltage:
		KYKYComm_SendHV();
		break;
	case	KYKYComm_CMD_Set_HighVoltage:
		{
			theApp.DisplayMsg( "Change Acc from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
			{
				// 得到是V
				VARIANT ClassID, SerialNumber, Xpos, Flag;
				SerialNumber.lVal	= -1;
				Flag.lVal			= -1;
				if( msg->fValue < 10000 )
					Xpos.lVal	= (LONG)(msg->fValue /100);
				else
					Xpos.lVal	= (LONG)(msg->fValue /1000 -10 +100);
				if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
				{
					ClassID.lVal	= SemControlPanelClass_CPS_HV;
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeHighVoltage_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, TRUE );
				}
				else
				{
					ClassID.lVal	= SemControlPanelClass_SP_HV;
					if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
						((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeHighVoltage_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, TRUE );
					else
						((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeHighVoltage_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, TRUE );
				}
				theApp.DisplayMsg( "Set HV from third" );
			}
		}
		break;

	case	KYKYComm_CMD_Get_Fila:
		KYKYComm_SendFilament();
		break;
	case	KYKYComm_CMD_Set_Fila:
		{
			theApp.DisplayMsg( "Change Fil from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
			{
				VARIANT ClassID, SerialNumber, Xpos, Flag;
				SerialNumber.lVal	= -1;
				Flag.lVal			= -1;
				Xpos.lVal			= (LONG)(msg->fValue *100);
				if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
				{
					ClassID.lVal	= SemControlPanelClass_CPS_FilamentCurrent;
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeFilament_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
				}
				else
				{
					ClassID.lVal	= SemControlPanelClass_SP_Heater;
					if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
						((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeFilament_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
					else
						((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeFilament_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
				}
			}
		}
		break;

	case	KYKYComm_CMD_Get_Bias:
		KYKYComm_SendBias();
		break;
	case	KYKYComm_CMD_Set_Bias:
		{
			theApp.DisplayMsg( "Change Bias from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			{
				VARIANT ClassID, SerialNumber, Xpos, Flag;
				SerialNumber.lVal	= -1;
				Flag.lVal			= -1;
				Xpos.lVal			= (LONG)(msg->fValue);
				ClassID.lVal		= SemControlPanelClass_CPS_Bias;
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeBias_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
			}
		}
		break;

	case	KYKYComm_CMD_Set_BeamCurrent:
		{
			theApp.DisplayMsg( "Change Beam from third" );
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			if( CHVPowerDataManager::Instance().m_dwHVtype > 0 )
			{
				VARIANT ClassID, SerialNumber, Xpos, Flag;
				SerialNumber.lVal	= -1;
				Flag.lVal			= -1;
				Xpos.lVal			= (LONG)(msg->fValue);
				ClassID.lVal		= SemControlPanelClass_SP_Bias;
				if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeBeam_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
				else
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeBeam_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
			}
		}
		break;
	case	KYKYComm_CMD_Get_BeamCurrent:
		KYKYComm_SendBeamCurrent();
		break;
	case	KYKYComm_CMD_Get_EMI:
		KYKYComm_SendEMI();
		break;

	case	KYKYComm_CMD_Stage_GetStatus:
		KYKYComm_StageSendStatus();
		break;

// 	case	KYKYComm_CMD_Stage_GetGrating:
// 		KYKYComm_StageSendGrating();
// 		break;

	case	KYKYComm_CMD_Stage_GetRange:
		KYKYComm_StageSendRange();
		break;

	case	KYKYComm_CMD_Stage_GetPos:
		KYKYComm_StageSendPosition();
		break;

	case	KYKYComm_CMD_Stage_SetPos:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			CString str;
			if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
			{
				char cAxis;
				if(StageTMCM_IsReady())
					StageTMCM_Move( msg->bCommand, msg->fValue );
				else if(StageMC6600_IsReady())
					StageMC6600_Move( msg->bCommand, msg->fValue );
				else if(StageMC600_IsReady())
					StageMC600_Move( msg->bCommand, msg->fValue );
				switch( msg->bCommand )
				{
				case	0:
					cAxis = 'x';
					break;
				case	1:
					cAxis = 'y';
					break;
				case	2:
					cAxis = 'z';
					break;
				case	3:
					cAxis = 'r';
					break;
				case	4:
					cAxis = 't';
					break;
				}
				str.Format( "Set Position from third, pos[ %c ] = %.3f", cAxis, msg->fValue );
			}
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )
			{
				CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
				if( pfrm != NULL && pfrm->m_dlgStage.m_hWnd != NULL )
				{
					switch( msg->bCommand )
					{
					case	0:
						pfrm->m_dlgStage.StagePosGotoX( msg->fValue );
						str.Format( "Set Position from third, posX = %.3f mm", msg->fValue );
						break;
					case	1:
						pfrm->m_dlgStage.StagePosGotoY( msg->fValue );
						str.Format( "Set Position from third, posY = %.3f mm", msg->fValue );
						break;
					}
				}
			}
			else
				str.Format( "Set Position from third. Has NO auto-stage or Stage is NOT ready" );
			theApp.DisplayMsg( str );
		}
		break;

	case	KYKYComm_CMD_Stage_SetPos5:
		{
			KYKYCommMsg_SendDataC5f* msg = (KYKYCommMsg_SendDataC5f*)lParam;
			CString str;
			if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
			{
				if( StageTMCM_IsReady() )
				{
					StageTMCM_Move5Axis( msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4, msg->Values.fValue5 );
					str.Format( "Set Position from third, pos = %.3f, %.3f, %.3f, %.1f, %.1f", msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4, msg->Values.fValue5 );
				}
				else if( StageMC6600_IsReady() )
				{
					StageMC6600_Move5Axis( msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4, msg->Values.fValue5 );
					str.Format( "Set Position from third, pos = %.3f, %.3f, %.3f, %.1f, %.1f", msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4, msg->Values.fValue5 );
				}
				else if( StageMC600_IsReady() )
				{
					if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 )
					{
						StageMC600_Move5Axis( msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4, msg->Values.fValue5 );
						str.Format( "Set Position from third, pos = %.3f, %.3f, %.3f, %.1f, %.1f", msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4, msg->Values.fValue5 );
					}
					else
					{
						StageMC600_Move4Axis( msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4 );
						str.Format( "Set Position from third, pos = %.3f, %.3f, %.3f, %.1f", msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4 );
					}
				}
			}
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )
			{
				CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
				if( pfrm != NULL && pfrm->m_dlgStage.m_hWnd != NULL )
				{
					pfrm->m_dlgStage.StagePosGotoX( msg->Values.fValue1 );
					pfrm->m_dlgStage.StagePosGotoY( msg->Values.fValue2 );
					str.Format( "Set Position from third, pos = %.3f, %.3f", msg->Values.fValue1, msg->Values.fValue2 );
				}
			}
			else
				str.Format( "Set Position from third. Has NO auto-stage or Stage is NOT ready" );
			theApp.DisplayMsg( str );
		}
		break;

	case	KYKYComm_CMD_Stage_Stop:
		{
			if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4  && StageTMCM_IsReady() )
				StageTMCM_StopMove();
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3  && StageMC6600_IsReady() )
				StageMC6600_StopMove();
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1  && StageMC600_IsReady() )
				StageMC600_StopMove();
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )
			{
				CStageXYManager::Instance().m_DMCWin.Command( "ST" );
				Sleep( 100 );
				CStageXYManager::Instance().m_DMCWin.Command( "MO" );
			}
			theApp.DisplayMsg( "Stop stage from third" );
		}
		break;

	case	KYKYComm_CMD_Stage_GoHome:
		{
			if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4  && StageTMCM_IsReady() )
				StageTMCM_GoHome();
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3  && StageMC6600_IsReady() )
				StageMC6600_GoHome();
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1  && StageMC600_IsReady() )
				StageMC600_GoHome();
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady )
				CStageXYManager::Instance().Stage_XY_FindIndex();
			theApp.DisplayMsg( "Stage FindIndex/GoHome from third" );
		}
		break;
	}

	if ( !m_commSendImageThd )
		return 1;

	switch (brecvCommand )
	{
	case	KYKYComm_CMD_Image_GetFrameData:
		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
			theApp.DisplayMsg( "recv KYKYComm_CMD_Image_GetFrameData" );
		m_bCommSendImgData = TRUE;
		KYKYComm_ImageFrame();
		break;
	case	KYKYComm_CMD_Image_Sync:
		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
			theApp.DisplayMsg( "recv KYKYComm_CMD_Image_Sync" );
		m_bCommSendImgData = TRUE;
//		KYKYComm_ImageSync();
		break;
	case	KYKYComm_CMD_Image_Stop:
		m_bCommBatchSendEnd = TRUE;
		m_bCommSendImgData = FALSE;
		break;
	}

	return 0;
}

void CScanView::KYKYComm_EndConnectThread()
{
	m_bCommBatchSendEnd = TRUE;
	if (m_commCmdThd != NULL)
	{
		m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_Disconnect, 0);
		m_commCmdThd->PostThreadMessage(WM_QUIT, 0, 0);
		m_commCmdThd = NULL;	
		theApp.DisplayMsg( "Disconnect thread cmd" );
	}
	if (m_commSendImageThd != NULL)
	{
		m_commSendImageThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_Disconnect, 0);
		m_commSendImageThd->PostThreadMessage(WM_QUIT, 0, 0);
		m_commSendImageThd = NULL;
		theApp.DisplayMsg( "Disconnect thread img" );
	}
	KillTimer( 50 );
}

void CScanView::KYKYComm_SendReso()
{
	if (m_commCmdThd == NULL)
		return;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Get_ScanCtrl_Reso, (LPARAM)(&CMCICardCommonSetting::Instance().m_sizeReso));
	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send imageReso = (%d, %d)", CMCICardCommonSetting::Instance().m_sizeReso.cx, CMCICardCommonSetting::Instance().m_sizeReso.cy );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendSpeed()
{
	if (m_commCmdThd == NULL)
		return;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Get_ScanCtrl_Speed, (LPARAM)(&CScanParameterManager::Instance().m_nusbPixRate));
	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send Speed = %.1f", 1.0*CScanParameterManager::Instance().m_nusbPixRate /2 );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendSEMType()
{
	if (m_commCmdThd == NULL)
		return;
	int nPos = theApp.m_strLogo.Find( "6" );
	CString strType = theApp.m_strLogo.Right( theApp.m_strLogo.GetLength() - nPos );
	m_nCommSEMType = atoi(strType);
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_GetSEMType, (LPARAM)(&m_nCommSEMType));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{	
		CString str;
		str.Format( "Send SEM type = %s", strType );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendMagRange()
{
	if (m_commCmdThd == NULL)
		return;
	VARIANT varMin, varMax;
	CSemCoreAssemble::Instance().m_Magnifier.GetRange( varMin, varMax );
	m_2fMagRange.fValue1 = varMin.dblVal;
	m_2fMagRange.fValue2 = varMax.dblVal;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Get_MagRange, (LPARAM)(&m_2fMagRange));
	
	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send Mag range = (%.2f, %.2f)", m_2fMagRange.fValue1, m_2fMagRange.fValue2);
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendMag()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Magnification, (LPARAM)(&m_dCommMag));
	CMPOData::Instance().m_dMag = m_dCommMag;

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Server update magnification = %.1f, %ld", m_dCommMag, GetTickCount() );
		theApp.DisplayMsg( str );
	}
}

// 2022.03.23 得到视场大小
void CScanView::KYKYComm_SendFOV()
{
	if (m_commCmdThd == NULL)
		return;

	CString strH = CSemVirtualMCICard::Instance().CalculateMobileUScale( CMCICardCommonSetting::Instance().m_sizeReso.cx );
	CString strV = CSemVirtualMCICard::Instance().CalculateMobileUScale( CMCICardCommonSetting::Instance().m_sizeReso.cy );
	CString str;
	str = strH.Left( strH.GetLength() -2 );
	m_5fFieldOfView.fValue1 = atof( str );
	str = strV.Left( strV.GetLength() -2 );
	m_5fFieldOfView.fValue2 = atof( str );
	if( CGlobalParamSetting::Instance().g_csUScaleUnit == "cm" )
		m_5fFieldOfView.fValue3 = 0.0f;
	else if( CGlobalParamSetting::Instance().g_csUScaleUnit == "mm" )
		m_5fFieldOfView.fValue3 = 1.0f;
	else if( CGlobalParamSetting::Instance().g_csUScaleUnit == "nm" )
		m_5fFieldOfView.fValue3 = 3.0f;
	else
		m_5fFieldOfView.fValue3 = 2.0f;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_FieldOfView, (LPARAM)(&m_5fFieldOfView));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		str.Format( "Send FOV = [%s, %s]", strH, strV );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendSRotAngInit()
{
	if (m_commCmdThd == NULL)
		return;
	double dParam = 0.0;
	CSemCoreAssemble::Instance().m_ScanRotate.GetParams( 1, dParam );
	m_nCommScanRotAngInit = (int)dParam;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_ScanRotAngleInit, (LPARAM)(&m_nCommScanRotAngInit));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send scan rotate angle init = %d", m_nCommScanRotAngInit );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendSRotAng()
{
	if (m_commCmdThd == NULL)
		return;
	double dParam = 0.0;
	CSemCoreAssemble::Instance().m_ScanRotate.GetParams( 2, dParam );
	m_nCommScanRotAng = (int)dParam;
	//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_ScanRotAngle, (LPARAM)(&m_nCommScanRotAng));
	CMPOData::Instance().m_nScanRotAngle = m_nCommScanRotAng;

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Server update scan rotate angle actual = %d", m_nCommScanRotAng );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendWD()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommWD = CSemCoreAssemble::Instance().m_WD.GetPos().dblVal;
	//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_WorkDistance, (LPARAM)(&m_dCommWD));
	CMPOData::Instance().m_dWD = m_dCommWD;

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Server update WD = %.1f", m_dCommWD );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendBrt()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommBrt = CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal *100.0 /2048;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Brightness, (LPARAM)(&m_dCommBrt));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send brightness = %.2f", m_dCommBrt );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendBrt2()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommBrt2 = CSemCoreAssemble::Instance().m_Signal2Brightness.GetPos().lVal *100.0 /2048;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Brightness2, (LPARAM)(&m_dCommBrt2));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send brightness 2 = %.2f", m_dCommBrt2 );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendCst()
{
	if (m_commCmdThd == NULL)
		return;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		m_dCommCst = CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal *100.0 /255;
	else
		m_dCommCst = CSemCoreAssemble::Instance().m_SP_PMT.GetPos().lVal *100.0 /400;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Contrast, (LPARAM)(&m_dCommCst));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send contrast = %.3f", m_dCommCst );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendStigX()
{
	if (m_commCmdThd == NULL)
		return;
	m_szCommStigmator.cx = CSemCoreAssemble::Instance().m_StigmatorX.GetPos().lVal;
//	m_szCommStigmator.cy = theApp.m_SemCtrl.m_SemCore.m_StigmatorY.GetPos().lVal;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_StigX, (LPARAM)(&m_szCommStigmator.cx));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send StigX = %d", m_szCommStigmator.cx );
		theApp.DisplayMsg( str );
	}
}
void CScanView::KYKYComm_SendStigY()
{
	if (m_commCmdThd == NULL)
		return;

	m_szCommStigmator.cy = CSemCoreAssemble::Instance().m_StigmatorY.GetPos().lVal;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_StigY, (LPARAM)(&m_szCommStigmator.cy));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send StigY = %d", m_szCommStigmator.cy );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendVacStatus()
{
	if (m_commCmdThd == NULL)
		return;
	m_nCommStatusVac = 0;
	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
	{
		if( CVACDataManager::Instance().m_VacTCAmp.nState == 2 )	// 真空已好，允许开V1阀
			m_nCommStatusVac = 1;
	}
	else if( CSemCtrl::Instance().m_nVACBoardType > 0 && CSemCtrl::Instance().m_nVACBoardType < 10 )
	{
		if( CVACDataManager::Instance().m_statusBoard.Vac.status.X == 6 && !(CVACDataManager::Instance().m_statusBoard.Vac.status.YM) )	// 真空已好，允许开V1阀
			m_nCommStatusVac = 1;
	}
	else if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.VacChambOK )		// 样品室真空正常状态)
			m_nCommStatusVac = 1;
	}
	//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_VacStatus, (LPARAM)(&m_nCommStatusVac));
	CMPOData::Instance().m_nVacStatus = m_nCommStatusVac;

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Server update Vac status = %d", m_nCommStatusVac );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendGunStatus()
{
	if (m_commCmdThd == NULL)
		return;
	m_nCommStatusGun = 0;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )	// 3603
	{
		if( theApp.m_blUnlock_HV )
		{
			int nCheck1 = ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_btnHV.GetCheck();
// 			int nCheck2 = ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_btnBias.GetCheck();
// 			int nCheck3 = ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_btnFilament.GetCheck();
//			if( nCheck1 && nCheck2 && nCheck3 )
			if( nCheck1 )
				m_nCommStatusGun = 1;
		}
	}
	else
	{
		if( theApp.m_blUnlock_HV )
		{
			if( CHVPowerDataManager::Instance().m_Spellmans.status.bReady && CHVPowerDataManager::Instance().m_Spellmans.status.bstatusSwitch == 0x1F )
				m_nCommStatusGun = 1;
			else if( CHVPowerDataManager::Instance().m_dwHVtype > 2 && theApp.m_cfgWSM30KV.bReady 
				&& ((theApp.m_statusWSM30KV2.status1.wStatus & 0x007F) == 0x007F) )
				m_nCommStatusGun = 1;
			else if( theApp.m_cfgWSM30KV.bReady 
				&& ((theApp.m_statusWSM30KV.status101.wStatus & 0x0037) == 0x0037)
				&& ((theApp.m_statusWSM30KV.status125.wStatus & 0x0002) == 0x0002) )
				m_nCommStatusGun = 1;
		}
	}
	//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_GunStatus, (LPARAM)(&m_nCommStatusGun));
	CMPOData::Instance().m_nGunStatus = m_nCommStatusGun;

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Server update Gun status = %d", m_nCommStatusGun );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendHVType()
{
	if (m_commCmdThd == NULL)
		return;
	m_nCommHVType = CHVPowerDataManager::Instance().m_dwHVtype;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_GetHighVoltageType, (LPARAM)(&m_nCommHVType));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send HV type = %d", m_nCommHVType );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendHV()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommHV = CHVPowerDataManager::Instance().m_dHV *1000;
	//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_HighVoltage, (LPARAM)(&m_dCommHV));
	CMPOData::Instance().m_dAcc = m_dCommHV /1000.0;

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Server update HV = %.1f KV", m_dCommHV /1000.0 );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendFilament()
{
	if (m_commCmdThd == NULL)
		return;
// 	if( theApp.m_dwHVtype < 1 )
// 		m_dCommFila = theApp.m_CPS3603.dIoutF;
// 	else
// 		m_dCommFila = theApp.m_Spellmans.pM3.dIH;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		m_dCommFila = CGlobalParamSetting::Instance().g_dHV3;
	else
		m_dCommFila = CHVPowerDataManager::Instance().m_Spellmans.pS.dIH;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Fila, (LPARAM)(&m_dCommFila));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send Fila = %.2f A", m_dCommFila );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendBias()
{
	if (m_commCmdThd == NULL)
		return;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )	// 3603
	{
		m_dCommBias = CGlobalParamSetting::Instance().g_dHV2;
		m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Bias, (LPARAM)(&m_dCommBias));
		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
		{
			CString str;
			str.Format( "Send Bias = %.1f V", m_dCommBias );
			theApp.DisplayMsg( str );
		}
	}
}

void CScanView::KYKYComm_SendBeamCurrent()
{
	if (m_commCmdThd == NULL)
		return;
	if( CHVPowerDataManager::Instance().m_dwHVtype > 0 )	// 771
	{
		m_dCommBeamCurrent = CHVPowerDataManager::Instance().m_Spellmans.pS.dIB;
		//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_BeamCurrent, (LPARAM)(&m_dCommBeamCurrent));
		CMPOData::Instance().m_dEmi = m_dCommBeamCurrent;

		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
		{
			CString str;
			str.Format( "Server update Beam current = %.2f uA", m_dCommBeamCurrent /1000.0 );
			theApp.DisplayMsg( str );
		}
	}
}

void CScanView::KYKYComm_SendEMI()
{
	if (m_commCmdThd == NULL)
		return;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		m_dCommEMI = CHVPowerDataManager::Instance().m_CPS3603.dIoutA * 1e6;
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			m_dCommEMI = theApp.m_monWSM30KV2.monFB.fACLi;
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			m_dCommEMI = theApp.m_monWSM30KV.monFB.fACLi;
		else
			m_dCommEMI = CHVPowerDataManager::Instance().m_Spellmans.pM.dII;
	}
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_EMI, (LPARAM)(&m_dCommEMI));
	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send Iem = %.3f uA", m_dCommEMI );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_StageSendStatus()
{
	if (m_commCmdThd == NULL)
		return;

	m_btCommStageStatus = 0x00;
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 )
	{
		// TMCM-5轴
		m_btCommStageStatus |= 0x03;			// 5轴
		BOOL bReady = StageTMCM_IsReady();		// 自动台连接正常
		if( bReady )
			m_btCommStageStatus |= 0x04;
		BOOL bIsMoving = StageTMCM_IsMoving();	// 自动台正在运动中
		if( bIsMoving )
			m_btCommStageStatus |= 0x08;
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 )
	{
		// KYMAT-5轴
		m_btCommStageStatus |= 0x03;			// 5轴
		BOOL bReady = StageMC6600_IsReady();	// 自动台连接正常
		if( bReady )
			m_btCommStageStatus |= 0x04;
		BOOL bIsMoving = StageMC6600_IsMoving();// 自动台正在运动中
		if( bIsMoving )
			m_btCommStageStatus |= 0x08;
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 )
	{
		// MC600-5轴
		m_btCommStageStatus |= 0x03;			// 5轴
		BOOL bReady = StageMC600_IsReady();		// 自动台连接正常
		if( bReady )
			m_btCommStageStatus |= 0x04;
		BOOL bIsMoving = StageMC600_IsMoving();	// 自动台正在运动中
		if( bIsMoving )
			m_btCommStageStatus |= 0x08;
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		// MC600-4轴
		m_btCommStageStatus |= 0x02;			// 4轴
		BOOL bReady = StageMC600_IsReady();		// 自动台连接正常
		if( bReady )
			m_btCommStageStatus |= 0x04;
		BOOL bIsMoving = StageMC600_IsMoving();	// 自动台正在运动中
		if( bIsMoving )
			m_btCommStageStatus |= 0x08;
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 )
	{
		// 2轴
		m_szCommStageGrating.cx = CStageXYManager::Instance().m_Stage.xy.nGratingX;
		m_szCommStageGrating.cy = CStageXYManager::Instance().m_Stage.xy.nGratingY;
		m_btCommStageStatus |= 0x01;			// 2轴
		char szBuffer[64];
		CString str;
		str.Format( "MG_BGX" );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		int nIsXMoving = atoi(szBuffer);
		str.Format( "MG_BGY" );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		int nIsYMoving = atoi(szBuffer);
			
		if( CStageXYManager::Instance().m_Stage.xy.bReady )			// XY自动台连接正常
			m_btCommStageStatus |= 0x04;
		if( nIsXMoving > 0 || nIsYMoving > 0 )	// XY自动台正在运动中
			m_btCommStageStatus |= 0x08;
// 		if( CStageXYManager::Instance().m_Stage.xy.bRev )			// XY对调
// 			m_btCommStageStatus |= 0x10;
// 		if( CStageXYManager::Instance().m_Stage.xy.sXLD == "+" )		// X反向：向左为+
// 			m_btCommStageStatus |= 0x20;
// 		if( CStageXYManager::Instance().m_Stage.xy.sYLD == "+" )		// Y反向：向上为+
// 			m_btCommStageStatus |= 0x40;
	}
	//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetStatus, (LPARAM)(&m_btCommStageStatus));
	CMPOData::Instance().m_nStageStatus = m_btCommStageStatus;

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Server update Stage status = 0x%02X", m_btCommStageStatus);
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_StageSendGrating()
{
	if (m_commCmdThd == NULL)
		return;
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage == 1 )		// 有XY自动台
	{
		m_szCommStageGrating.cx = CStageXYManager::Instance().m_Stage.xy.nGratingX;
		m_szCommStageGrating.cy = CStageXYManager::Instance().m_Stage.xy.nGratingY;
		//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetGrating, (LPARAM)(&m_szCommStageGrating));

		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
		{
			CString str;
			str.Format( "Send Stage grating = (%d, %d)", m_szCommStageGrating.cx, m_szCommStageGrating.cy);
			theApp.DisplayMsg( str );
		}
	}
}

void CScanView::KYKYComm_StageSendRange()
{
	if (m_commCmdThd == NULL)
		return;
	CString str;
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		// 4/5轴
		float fRange[10] = {0.f};
		if( StageTMCM_IsReady() )
		{
			StageTMCM_GetParam( 0, fRange[0], fRange[1], fRange[2], fRange[3], fRange[4] );		// pos
			StageTMCM_GetParam( 1, fRange[5], fRange[6], fRange[7], fRange[8], fRange[9] );		// neg
		}
		else if( StageMC6600_IsReady() )
		{
			StageMC6600_GetParam( 0, fRange[0], fRange[1], fRange[2], fRange[3], fRange[4] );	// pos
			StageMC6600_GetParam( 1, fRange[5], fRange[6], fRange[7], fRange[8], fRange[9] );	// neg
		}
		else if( StageMC600_IsReady() )
		{
			StageMC600_GetParam( 0, fRange[0], fRange[1], fRange[2], fRange[3], fRange[4] );	// pos
			StageMC600_GetParam( 1, fRange[5], fRange[6], fRange[7], fRange[8], fRange[9] );	// neg
		}
		m_5fStageRange5.fValueX[0] = fRange[0];
		m_5fStageRange5.fValueX[1] = fRange[0 +5];
		m_5fStageRange5.fValueY[0] = fRange[1];
		m_5fStageRange5.fValueY[1] = fRange[1 +5];
		m_5fStageRange5.fValueZ[0] = fRange[2];
		m_5fStageRange5.fValueZ[1] = fRange[2 +5];
		m_5fStageRange5.fValueR[0] = fRange[3];
		m_5fStageRange5.fValueR[1] = fRange[3 +5];
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 )	// 5轴
		{
			m_5fStageRange5.fValueT[0] = fRange[4];
			m_5fStageRange5.fValueT[1] = fRange[4 +5];
		}
		else
		{
			m_5fStageRange5.fValueT[0] = 0.0f;
			m_5fStageRange5.fValueT[1] = 0.0f;
		}
		m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetRange, (LPARAM)(&m_5fStageRange5));
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 )
		{
			str.Format( "Send Stage range = [(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.1f, %.1f), (%.1f, %.1f)]",
				m_5fStageRange5.fValueX[1], m_5fStageRange5.fValueX[0], m_5fStageRange5.fValueY[1], m_5fStageRange5.fValueY[0],
				m_5fStageRange5.fValueZ[1], m_5fStageRange5.fValueZ[0], m_5fStageRange5.fValueR[1], m_5fStageRange5.fValueR[0],
				m_5fStageRange5.fValueT[1], m_5fStageRange5.fValueT[0] );
		}
		else
		{
			str.Format( "Send Stage range = [(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.1f, %.1f)]",
				m_5fStageRange5.fValueX[1], m_5fStageRange5.fValueX[0],	m_5fStageRange5.fValueY[1], m_5fStageRange5.fValueY[0],
				m_5fStageRange5.fValueZ[1], m_5fStageRange5.fValueZ[0],	m_5fStageRange5.fValueR[1], m_5fStageRange5.fValueR[0] );
		}
		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
			theApp.DisplayMsg( str );
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 )		// 有自动台
	{
		// 2轴
// 		m_szCommStageRange.cx = theApp.m_Stage.xy.nRangeX;
// 		m_szCommStageRange.cy = theApp.m_Stage.xy.nRangeY;
//		m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetRange, (LPARAM)(&m_szCommStageRange));
//		str.Format( "Send Stage range = (%d, %d)", m_szCommStageRange.cx, m_szCommStageRange.cy);
		m_2fStageRange.fValueX[0] = 0.5 * CStageXYManager::Instance().m_Stage.xy.nRangeX;
		m_2fStageRange.fValueX[1] = -0.5 * CStageXYManager::Instance().m_Stage.xy.nRangeX;
		m_2fStageRange.fValueY[0] = 0.5 * CStageXYManager::Instance().m_Stage.xy.nRangeY;
		m_2fStageRange.fValueY[1] = -0.5 * CStageXYManager::Instance().m_Stage.xy.nRangeY;
		m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetRange, (LPARAM)(&m_2fStageRange));
		str.Format( "Send Stage range = [(%.2f, %.2f); (%.2f, %.2f)]",
			m_2fStageRange.fValueX[1], m_2fStageRange.fValueX[0], m_2fStageRange.fValueY[1], m_2fStageRange.fValueY[0] );
		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
			theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_StageSendPosition()
{
	if (m_commCmdThd == NULL)
		return;
	CString str;
	m_5fStagePos5.fValue1 = m_5fStagePos5.fValue2 = m_5fStagePos5.fValue3 = m_5fStagePos5.fValue4 = m_5fStagePos5.fValue5 = 0.0f;
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		// 4/5轴
		float fPos[5] = {0.f};
		if( StageTMCM_IsReady() )
			StageTMCM_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );	// position
		else if( StageMC6600_IsReady() )
			StageMC6600_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );	// position
		else if( StageMC600_IsReady() )
			StageMC600_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );	// position
		m_5fStagePos5.fValue1 = fPos[0];
		m_5fStagePos5.fValue2 = fPos[1];
		m_5fStagePos5.fValue3 = fPos[2];
		m_5fStagePos5.fValue4 = fPos[3];
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 )	// 5轴
			m_5fStagePos5.fValue5 = fPos[4];
		else
			m_5fStagePos5.fValue5 = 0.0f;
		//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetPos, (LPARAM)(&m_5fStagePos5));
		str.Format( "Server update Stage position = (%.3f, %.3f, %.3f, %.1f, %.1f)", m_5fStagePos5.fValue1, m_5fStagePos5.fValue2, m_5fStagePos5.fValue3, m_5fStagePos5.fValue4, m_5fStagePos5.fValue5 );
		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
			theApp.DisplayMsg( str );
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 )
	{
		// 2轴
		// 得到当前位置
 		CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm != NULL && pfrm->m_dlgStage.m_hWnd != NULL )
			pfrm->m_dlgStage.StagePosUpdate();
		m_5fStagePos5.fValue1 = CStageXYManager::Instance().m_Stage.xy.fDispPos[0];
		m_5fStagePos5.fValue2 = CStageXYManager::Instance().m_Stage.xy.fDispPos[1];
		m_5fStagePos5.fValue1 = 1.0 * CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x *m_szCommStageGrating.cx /1000 /1000;
		m_5fStagePos5.fValue2 = 1.0 * CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y *m_szCommStageGrating.cy /1000 /1000;
		//m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetPos, (LPARAM)(&m_5fStagePos5));
		str.Format( "Server update Stage position = (%.3f, %.3f)", m_5fStagePos5.fValue1, m_5fStagePos5.fValue2);
		if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
			theApp.DisplayMsg( str );
	}
	CMPOData::Instance().m_dStagePos[0]	= m_5fStagePos5.fValue1;
	CMPOData::Instance().m_dStagePos[1]	= m_5fStagePos5.fValue2;
	CMPOData::Instance().m_dStagePos[2]	= m_5fStagePos5.fValue3;
	CMPOData::Instance().m_dStagePos[3]	= m_5fStagePos5.fValue4;
	CMPOData::Instance().m_dStagePos[4]	= m_5fStagePos5.fValue5;
}

void CScanView::KYKYComm_SendIsStopScan()
{
	m_bCommStopScan = FALSE;
	if( CScanParameterManager::Instance().m_nusbScanType == USB_ST_Stop )
		m_bCommStopScan = TRUE;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_ScanCtrl_IsStopScan, (LPARAM)(&((int)m_bCommStopScan)));
	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Is Stop Scan = %d", (int)m_bCommStopScan );
		theApp.DisplayMsg(str);
	}
}

void CScanView::KYKYComm_Snap()
{
}

void CScanView::KYKYComm_ImageFrame()
{
	m_commmsgSendData.rcDest = CRect(0, 0, CMCICardCommonSetting::Instance().m_sizeReso.cx, CMCICardCommonSetting::Instance().m_sizeReso.cy);
	long lBuffSize = m_commmsgSendData.rcDest.Width() * m_commmsgSendData.rcDest.Height();
	BOOL bRet = CSemVirtualMCICard::Instance().Read( (LPVOID)(m_commmsgSendData.bImageBuff), lBuffSize, m_commmsgSendData.rcDest );
	m_commSendImageThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send_Img, (WPARAM)KYKYComm_CMD_Image_GetFrameData, (LPARAM)(&m_commmsgSendData));

	if( CAppLogManager::Instance().m_boolLogMsgRemoteGet )
	{
		CString str;
		str.Format( "Send Image Data: [%d, %d, %d, %d]", m_commmsgSendData.rcDest.left, m_commmsgSendData.rcDest.top, m_commmsgSendData.rcDest.right, m_commmsgSendData.rcDest.bottom );
		theApp.DisplayMsg(str);
	}
}

void CScanView::KYKYComm_ImageSync()
{
	m_bCommBatchSendEnd = TRUE;

	KYKYComm_SendReso();
	Sleep(5);
	m_bCommBatchSendEnd = FALSE;

	DWORD id;
	HANDLE h = CreateThread(NULL, 0, KYKYComm_BatchSendImageData, this, 0, &id);
	CloseHandle(h);
}

void CScanView::KYKYComm_SetSendRow(int nCount)
{
	m_nCommRectLen = nCount;
}

DWORD WINAPI CScanView::KYKYComm_BatchSendImageData(LPVOID lparam)
{
	CScanView * pScan = (CScanView*)lparam;	
	int nDestRowStart, nDestRowEnd;
	int nRectLen = 4;	//pScan->m_nCommRectLen;     // 每次发送的行数
	CSemVirtualMCICard::Instance().GetCurrentY(nDestRowStart, nDestRowEnd);
	if (nDestRowStart <= nRectLen)
	{
		pScan->m_commmsgSendData.rcDest.bottom = 0;
	}
	else
	{
		int nbottom = nDestRowStart - nDestRowStart % nRectLen;
		pScan->m_commmsgSendData.rcDest.bottom = nbottom;
	}

	while (!pScan->m_bCommBatchSendEnd)
	{
		if (pScan->m_commmsgSendData.rcDest.bottom == CMCICardCommonSetting::Instance().m_sizeReso.cy)
			pScan->m_commmsgSendData.rcDest.bottom = 0;
// 		TRACE("m_commmsgSendData.rcDest = %d,%d,%d,%d\r\n",m_commmsgSendData.rcDest.left,m_commmsgSendData.rcDest.top,m_commmsgSendData.rcDest.right,m_commmsgSendData.rcDest.bottom);

		int nLastBottom = pScan->m_commmsgSendData.rcDest.bottom;
		CString s2;
		CSemVirtualMCICard::Instance().GetCurrentY(nDestRowStart, nDestRowEnd);
		s2.Format("nDestRowStart=%d, nDestRowEnd=%d\r\n",nDestRowStart,nDestRowEnd);

		int nGap = nDestRowEnd - nDestRowStart;
		if (nGap > 3)   //4,5
		{
			nRectLen = 16;
		}
		else if (nGap > 1)    //2,3
		{
			nRectLen = 8;
		}
		else if (nGap >0)     //1
		{
			nRectLen = 4;
		}
		// if nGap = 0;,nRectLen not change
//		TRACE("nRectLen = %d\r\n",nRectLen);
		//如果当前最新扫描开始行小于等于上次发送的最后一行，则等待
		if ((nDestRowStart - nLastBottom  <= nRectLen) && (nLastBottom <= nDestRowStart))
		{
			continue;
		}
		TRACE(s2);

		//在远程传送图片过程中，不允许扫描方式或者区域改变

		//每次发送 nRectLen 行数据
		pScan->m_commmsgSendData.rcDest = CRect(0, nLastBottom, CMCICardCommonSetting::Instance().m_sizeReso.cx, nLastBottom +nRectLen);
		//监测是否是低端数据
		if (pScan->m_commmsgSendData.rcDest.bottom >= CMCICardCommonSetting::Instance().m_sizeReso.cy)
		{
			pScan->m_commmsgSendData.rcDest.bottom = CMCICardCommonSetting::Instance().m_sizeReso.cy;
			TRACE("发送低端数据:");
		}

		TRACE("m_commmsgSendData.rcDest = %d,%d,%d,%d\r\n", pScan->m_commmsgSendData.rcDest.left, pScan->m_commmsgSendData.rcDest.top, pScan->m_commmsgSendData.rcDest.right, pScan->m_commmsgSendData.rcDest.bottom);

		int nSize = pScan->m_commmsgSendData.rcDest.Width() * pScan->m_commmsgSendData.rcDest.Height();

		BOOL bRet = CSemVirtualMCICard::Instance().Read( (LPVOID)pScan->m_commmsgSendData.bImageBuff, nSize, pScan->m_commmsgSendData.rcDest );

		LPARAM LParam;
		LParam = (LPARAM)&(pScan->m_commmsgSendData);

		pScan->m_commSendImageThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Image_Sync, LParam);
		Sleep(5);   //CSocket.send的时间  同一台电脑时设置为5，不同电脑设置为10
	}

	return 0;	
}


LRESULT CScanView::OnSMvacChanged(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str.Format( "VAC state: 0x%04X, 0x%04X", wParam, lParam );
	theApp.DisplayMsg( str );
	return 0;
}


void CScanView::OnViewZoomIn() 
{
	CScaleManager::Instance().OnViewZoomIn(m_bZoomFit);

//	m_bPreview = FALSE;
	CSemVirtualMCICard::Instance().SetPhotoPreview( FALSE );

	ScreenZoom();	// 2005-1-17_液晶屏幕XY比例问题
	theApp.DisplayMsg("Press view zoom in");
}

void CScanView::OnUpdateViewZoomIn(CCmdUI* pCmdUI) 
{
	if( CScaleManager::Instance().ISMaxIndex() )//&& !m_bZoomFit )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( !m_bPreview );
}

void CScanView::OnViewZoomOut() 
{
	CScaleManager::Instance().OnViewZoomOut(m_bZoomFit);

//	m_bPreview = FALSE;
	CSemVirtualMCICard::Instance().SetPhotoPreview( FALSE );

	ScreenZoom();	// 2005-1-17_液晶屏幕XY比例问题
	theApp.DisplayMsg("Press view zoom out");
}

void CScanView::OnUpdateViewZoomOut(CCmdUI* pCmdUI) 
{
	if( CScaleManager::Instance().ISMinIndex() && !m_bZoomFit )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( !m_bPreview );
}

void CScanView::OnViewActualSize() 
{
	theApp.DisplayMsg("Press view actual size");

	CScaleManager::Instance().OnViewActualSize(m_bZoomFit);

	m_bPreview = FALSE;
	CSemVirtualMCICard::Instance().SetPhotoPreview( FALSE );

//	ScreenZoom();	// 2005-1-17_液晶屏幕XY比例问题
	CSemVirtualMCICard::Instance().SetZoom( 1.0 );
	CSize szImage = GetImageSize();
	SetScrollSizes( MM_TEXT, szImage );
	m_szTotalLog = szImage;
	UpdateItemRect( FALSE );
//	InvalidateRect(NULL);
	CSemVirtualMCICard::Instance().UpdateScreenMag( CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal );
}

void CScanView::OnUpdateViewActualSize(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CSemVirtualMCICard::Instance().GetZoom() == 1.0 );
	pCmdUI->Enable( !m_bPreview );
}

void CScanView::OnViewZoomFit() 
{
//	UpdateScale();
	CScaleManager::Instance().OnViewZoomFit();
	m_bZoomFit = TRUE;
//	m_bPreview = FALSE;
	CSemVirtualMCICard::Instance().SetPhotoPreview( FALSE );
	ScreenZoom();	// 2005-1-17_液晶屏幕XY比例问题
	theApp.DisplayMsg("Press view zoom fit");
}

void CScanView::OnUpdateViewZoomFit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bZoomFit );
	pCmdUI->Enable( !m_bPreview );
}

// 2005-1-17_液晶屏幕XY比例问题
void CScanView::ScreenZoom() 
{
	CSize szImage, szClient;
	double dScale;
	CString csTemp;
	CRect	rcClient;

	if (( m_boolScreenFull == TRUE ) && ( m_bZoomFit == TRUE ))
	{
		szImage = CSize(0, 0);
		SetScrollSizes( MM_TEXT, szImage );
		GetClientRect( &rcClient );
		szClient = rcClient.Size();

		szImage = GetImageSize();
		if ( szClient.cy == 0 )
		{
			dScale = 0;
		}
		else if ( szClient.cx * 3 <  szClient.cy * 4 )
		{
			dScale = (double)szClient.cx / szImage.cx;
		}
		else
		{
			dScale = (double)szClient.cy / szImage.cy;
		}

		szImage.cx = (long)(szImage.cx *dScale);
		szImage.cy = (long)(szImage.cy *dScale);
		CSemVirtualMCICard::Instance().SetZoom( dScale );
		SetScrollSizes( MM_TEXT, szImage );
//		InvalidateRect(NULL);

		CScaleManager::Instance().ScreenZoom(dScale);
	}
	else if ( m_bZoomFit == TRUE )
	{
		szImage = CSize(0,0);
		
		CSemVirtualMCICard::Instance().SetZoom( 0 );
		SetScrollSizes( MM_TEXT, szImage );
//		InvalidateRect(NULL);
	}
	else
	{
		szImage = GetImageSize();
		if( m_bSplit )
			szImage.cx *= 2;
		dScale = CScaleManager::Instance().GetCurScale();
		CSemVirtualMCICard::Instance().SetZoom( dScale );

		szImage.cx = (long)(szImage.cx *dScale);
		szImage.cy = (long)(szImage.cy *dScale);
		SetScrollSizes( MM_TEXT, szImage );
//		InvalidateRect(NULL);
	}
//	pfrm->m_wndScan.GetClientRect( &m_rcScan );
	m_szTotalLog = szImage;
	UpdateItemRect();

	InvalidateRect(NULL);

//	SetDlgItemText(IDC_STATIC_ZOOM, csTemp);
//	UpdateData(FALSE);
	CSemVirtualMCICard::Instance().UpdateScreenMag( CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal );
}



// Auto Functions

void CScanView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static BOOL	boolBusy = FALSE;

	if ( !boolBusy )
		boolBusy = TRUE;
	else
		return;

	switch( nIDEvent )
	{
	case	nAutoTimerDate:
		SyncDate( m_bDate );
		break;
	case	31:
		{
			BOOL bIsMoving = TRUE;
			if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
				bIsMoving = StageTMCM_IsMoving();
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
				bIsMoving = StageMC6600_IsMoving();
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
				bIsMoving = StageMC600_IsMoving();
			if( !bIsMoving )
			{
				KillTimer(31);
				AutoStage_Clear_Finish( TRUE, TRUE );
			}
		}
		break;
	case	Diag_Total:
		{
			CString str, strTemp;
			str.Format( "%04d\t", CControlLayer::Instance().m_DL.other.lTimes++ );

			strTemp = Diag_Logging2();
			str += strTemp;
			strTemp = CControlLayer::Instance().Diag_SCN_LNS();
			str += strTemp;
			strTemp = Diag_VAC();
			str += strTemp;
			strTemp = Diag_SPR();
			str += strTemp;
			strTemp = Diag_MCI();
			str += strTemp;

			unsigned long cbRet;
			char Buf[4096];
			sprintf_s( Buf, "%s\n", str );
			WriteFile( CControlLayer::Instance().m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		}
		break;
	///////////////////////////////////////
	case	50:
		{
			KYKYComm_SendWD();
			KYKYComm_SendMag();
			KYKYComm_SendSRotAng();
			KYKYComm_SendHV();
			KYKYComm_SendBeamCurrent();
			KYKYComm_SendVacStatus();
			KYKYComm_SendGunStatus();
			KYKYComm_StageSendStatus();
			KYKYComm_StageSendPosition();
		}
		break;
	///////////////////////////////////////
	case	nAutoFunctionTimerID:
		{
			switch (CScanParameterSaver::Instance().m_nAutoFuction)
			{
			case	AutoBC:
				AutoBCProcess2();
				break;

			case	AutoFocus:
				AutoFocusProcess();
				break;
			case	AutoFocus1:
				AutoFocusProcess1();
				break;	

			case	AutoAstigmat:
				AutoAstigmatProcess();
				break;

			case	AutoFilament:
				AutoFilamentProcess();
				break;
			}
		}
		break;
	case	nAutoStageXY_FI11:
		{
			if( CStageXYManager::Instance().m_Stage.xy.nMtd < 1 )
				AutoStageXY_FI_XQ();
			else if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
				AutoStageXY_FI_Process11();
			else
				AutoStageXY_FI2_Process11();
		}
		break;
	case	nAutoStageXY_FI12:
		{
			if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
				AutoStageXY_FI_Process12();
			else
				AutoStageXY_FI2_Process12();
		}
		break;
	case	nAutoStageXY_FI21:
		{
			if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
				AutoStageXY_FI_Process21();
			else
				AutoStageXY_FI2_Process21();
		}
		break;
	case	nAutoStageXY_FI22:
		{
			if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
				AutoStageXY_FI_Process22();
			else
				AutoStageXY_FI2_Process22();
		}
		break;
	case	nAutoStageXY_FI31:
		{
			if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
				AutoStageXY_FI_Process31();
			else
				AutoStageXY_FI2_Process31();
		}
		break;
	case	nAutoStageXY_FI32:
		{
			if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
				AutoStageXY_FI_Process32();
			else
				AutoStageXY_FI2_Process32();
		}
		break;
	case	nAutoStageXY_FI4:
		AutoStageXY_FI_Process4();
		break;
	case	nAutoStageTrack:
		AutoStageXY_FI_TrackSecurity();
		break;
	case	nAutoStageClearXY:
		AutoStage_Clear_XY();
		break;
	///////////////////////////////////////
	case	nMCB_TimerID:
		{
			CViewMCBControler::Instance().HandleMCBTimerMessage(theApp.m_dwOpr,theApp.m_blUnlock_HV,theApp.m_blOverhaulState,boolBusy);
		}
		break;
	}

	boolBusy = FALSE;

//	CScrollView::OnTimer(nIDEvent);
}

//进行自动灯丝调节
void CScanView::OnAutoFilament() 
{
	FuncAutoFilament();
}

void CScanView::FuncAutoFilament()
{
	CDlgProgress	dlg;

	dlg.m_csTitle	= GetResString( IDS_SD_TITLE_AUTO_Filament );
	dlg.m_iRangeTo	= 100;//50;		// 200 s
	dlg.m_csMessage = GetResString( IDS_STRING_AUTO_FILAMENT2 );

	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
	{
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.ChangeBias_WSM30KV2_Max();
	}
	StartAutoFilament();

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	StopAutoFilament();
}

void AutoFilament_Setup(long lFila)
{
	VARIANT Xpos, Ypos, Flag;
	CString strX, strY;

	Flag.ulVal	= (DWORD)0x01;
	Xpos.lVal = lFila;

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.m_PageDebug2.m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );

		strX.Format( "%.2f A", double(Xpos.lVal *0.01) );
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.m_ctrlHeater.SyncText( strX, strY, Flag );
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.m_PageDebug2.m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );

		CCPS3603tatusDetector::Instance().SetParam( 12, Xpos.lVal *10 );
	}
	else
	{
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.m_PageDebug2.m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );

		strX.Format( "%.2f A", double(Xpos.lVal *0.01) );
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.m_ctrlHeater.SyncText( strX, strY, Flag );
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.m_PageDebug2.m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );

		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			theApp.m_WSM30KV2.SetParam( 3, Xpos.lVal *10 );
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			theApp.m_WSM30KV.SetParam( 3, Xpos.lVal *10 );
		else
			CSpellmanStatusDetector::Instance().SetParam( 34, Xpos.lVal *10 );
	}
}

void CScanView::StartAutoFilament()
{
	// 保存现场
	CScanParameterManager::Instance().m_nusbScanTypeSaved	= CScanParameterManager::Instance().m_nusbScanType;
	CScanParameterManager::Instance().m_nusbPixRateSaved	= CScanParameterManager::Instance().m_nusbPixRate;

	// 读取参数值
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\KYKYcfg.ini");
	char		stabuff[20];
	char		staitembuff[20];
	int			nTotal	= 4;
	int*		pCheck	= new int[4];
	CString		str;
	sprintf_s(stabuff, "Auto");
	sprintf_s(staitembuff, "Fila");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	int nMin	= pCheck[0];
	int nMax	= pCheck[1];
	int	nStep	= pCheck[2];
	int	nDebug	= pCheck[3];
	delete [] pCheck;

	// 设定灯丝初始尝试值
	AutoFila_Initialize( nMin, nMax, nStep, nDebug, CScanParameterSaver::Instance().m_lFilaSaved );
	AutoFilament_Setup(CScanParameterSaver::Instance().m_lFilaSaved);
	theApp.DisplayMsg( "AL-Start" );

	// 设定当前扫描
	CScanParameterManager::Instance().m_nusbPixRate	= 16;	// USB_SS_16us
	USB_ChangeScan( USB_ST_AutoBCArea );

	// 进入自动灯丝过程
	m_nLoop = 0;
	CScanParameterSaver::Instance().m_nAutoFuction		= AutoFilament;
	CScanParameterSaver::Instance().m_bAutoFilaPause	= FALSE;
	CSemVirtualMCICard::Instance().FramGrayHistogramReset();
	CSemVirtualMCICard::Instance().FramGrayHistogramStart();
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		CCPS3603tatusDetector::Instance().SetParam( 35, TRUE );	// 设置寻找饱和点标志位
		m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, 700/*nAutoBCTimerElapse*/, NULL );
	}
	else
		m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, 700/*nAutoBCTimerElapse*/, NULL );
}

void CScanView::StopAutoFilament()
{
	// 恢复灯丝
//	AutoFilament_Setup(m_lFilaSaved);

	// 停止图象分析
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	CSemVirtualMCICard::Instance().FramGrayHistogramStop();
	CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		CCPS3603tatusDetector::Instance().SetParam( 35, FALSE );	// 设置寻找饱和点标志位

	// 恢复扫描
	CScanParameterManager::Instance().m_nusbPixRate = CScanParameterManager::Instance().m_nusbPixRateSaved;
	USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanTypeSaved );
}

void CScanView::AutoFilamentOK( long lFila )
{
	CScanParameterSaver::Instance().m_lFilaSaved = lFila;
	AutoFilament_Setup(CScanParameterSaver::Instance().m_lFilaSaved);

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoFilamentProcess()
{
	if ( !CSemVirtualMCICard::Instance().IsReadyFramGrayHistogram() )
		return;

	DWORD	dwTotalPixel;
	dwTotalPixel = CSemVirtualMCICard::Instance().CopyFramGrayHistogramBuff( m_dwFramGrayHistogramBuff );
	CSemVirtualMCICard::Instance().FramGrayHistogramReset();
	CSemVirtualMCICard::Instance().FramGrayHistogramStart();

	// 分析灰度直方图，尝试新的亮度、对比度
	long lFilaSaved = CScanParameterSaver::Instance().m_lFilaSaved;
	UINT nRet = AutoFila_Analyze_GrayHistogram( CScanParameterSaver::Instance().m_bAutoFilaPause, dwTotalPixel,
		m_dwFramGrayHistogramBuff, CScanParameterSaver::Instance().m_lFilaSaved );

	if( theApp.m_dwOpr > 0 )
	{
		// Test 调试信息
		CString		csMsg;

		csMsg.Format("%02d F=%3d FS=%3d P=%6d L=%5.1f Min=%5.1f",
			m_nLoop++, lFilaSaved, CScanParameterSaver::Instance().m_lFilaSaved,
			dwTotalPixel, dwTotalPixel/256.0, dwTotalPixel * dblPeakMinParam);
		theApp.DisplayMsg( csMsg );
	}
	
	if ( nRet == AutoFila_Analyze_Continue )
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 0 )
		{
			if( theApp.m_dwOpr > 0 )
			{
				CString str;
				if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
					str.Format( "In AutoFilamentProcess, set ems = %.3f, mon ems = %.3f", theApp.m_monWSM30KV2.monOP.fACLi, theApp.m_monWSM30KV2.monFB.fACLi );
				else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					str.Format( "In AutoFilamentProcess, set ems = %.3f, mon ems = %.3f", theApp.m_monWSM30KV.monOP.fACLi, theApp.m_monWSM30KV.monFB.fACLi );
				else
					str.Format( "In AutoFilamentProcess, S.dIB = %.3f, M.dII = %.3f", CHVPowerDataManager::Instance().m_Spellmans.pS.dIB, CHVPowerDataManager::Instance().m_Spellmans.pM.dII );
				theApp.DisplayMsg( str );
			}
			if( (CHVPowerDataManager::Instance().m_dwHVtype > 2 && theApp.m_monWSM30KV2.monOP.fACLi > 0.1
				&& (theApp.m_monWSM30KV2.monFB.fACLi -theApp.m_monWSM30KV2.monOP.fACLi) / theApp.m_monWSM30KV2.monOP.fACLi < 0.3)
				|| (CHVPowerDataManager::Instance().m_dwHVtype > 1 && theApp.m_monWSM30KV.monOP.fACLi > 0.1
				&& (theApp.m_monWSM30KV.monFB.fACLi -theApp.m_monWSM30KV.monOP.fACLi) / theApp.m_monWSM30KV.monOP.fACLi < 0.3)
				|| (CHVPowerDataManager::Instance().m_dwHVtype < 2 && CHVPowerDataManager::Instance().m_Spellmans.pS.dIB > 0.001
				&& (CHVPowerDataManager::Instance().m_Spellmans.pM.dII -CHVPowerDataManager::Instance().m_Spellmans.pS.dIB) / CHVPowerDataManager::Instance().m_Spellmans.pS.dIB < 0.3) )
			{
				AutoFilament_Setup(CScanParameterSaver::Instance().m_lFilaSaved);
				CScanParameterSaver::Instance().m_bAutoFilaPause = FALSE;
			}
			else
				CScanParameterSaver::Instance().m_bAutoFilaPause = TRUE;
		}
		else
			AutoFilament_Setup(CScanParameterSaver::Instance().m_lFilaSaved);
	}
	else
	{
		// 退出自动灯丝
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		CSemVirtualMCICard::Instance().FramGrayHistogramStop();
		CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			CCPS3603tatusDetector::Instance().SetParam( 35, FALSE );	// 设置寻找饱和点标志位

		if ( nRet == AutoFila_Analyze_Completed )
		{
			AutoFilamentOK( CScanParameterSaver::Instance().m_lFilaSaved );
			theApp.DisplayMsg( "AL-Complete" );
		}
		else
		{
			AutoFilamentOK( lFilaSaved );
			theApp.DisplayMsg( "AL-Finish" );
		}
	}
}

//进行自动亮度/对比度调节
void CScanView::OnAutoBrightContrast() 
{
	FuncAutoBrightContrast();
}

void CScanView::FuncAutoBrightContrast()
{
	CDlgProgress	dlg;

	dlg.m_csTitle = GetResString( IDS_SD_TITLE_AUTO_BC );
	dlg.m_iRangeTo		= 100;//50;		// 200 s
	dlg.m_csMessage = GetResString( IDS_STRING_AUTO_BC );

	// 11.01.28 改善自动亮度对比度功能
//	StartAutoBC();
	StartAutoBC2();
	// 11.01.28 改善自动亮度对比度功能

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	// 11.01.28 改善自动亮度对比度功能
//	StopAutoBC();
	StopAutoBC2();
	// 11.01.28 改善自动亮度对比度功能
}

void AutoBC_Setup(long lBrightness, long lContrast)
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	ClassID.lVal = SemControlPanelClass_Brightness;
	Flag.lVal = 1;
	Xpos.lVal = lBrightness;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		ClassID.lVal = SemControlPanelClass_Contrast;
		Flag.lVal = 1;
		Xpos.lVal = lContrast;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
	else
	{
		ClassID.lVal = SemControlPanelClass_SP_PMT;
		Flag.lVal = 1;
		Xpos.lVal = lContrast;

		if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE );
		}
	}

}

void CScanView::StartAutoBC()
{
	// 保存现场
	CScanParameterManager::Instance().m_nusbScanTypeSaved	= CScanParameterManager::Instance().m_nusbScanType;
	CScanParameterManager::Instance().m_nusbPixRateSaved	= CScanParameterManager::Instance().m_nusbPixRate;

	CScanParameterSaver::Instance().m_lBrightnessSaved	= CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal;
	CScanParameterSaver::Instance().m_lContrastSaved	= CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal;

	// 设定亮度、对比度初始尝试值
	long	lBrightness;
	long	lContrast;
	AutoBC_Initialize( lBrightness, lContrast );

	AutoBC_Setup(lBrightness, lContrast);

	// 设定当前扫描
	CScanParameterManager::Instance().m_nusbPixRate	= 16;	// USB_SS_5us
	USB_ChangeScan( USB_ST_AutoBCArea );

	// 进入自动亮度、对比度过程
	m_nLoop = 0;
	CScanParameterSaver::Instance().m_nAutoFuction = AutoBC;
	CSemVirtualMCICard::Instance().FramGrayHistogramReset();
	CSemVirtualMCICard::Instance().FramGrayHistogramStart();
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, 100/*nAutoBCTimerElapse*/, NULL );	
}

void CScanView::StopAutoBC()
{
	// 恢复亮度、对比度
	AutoBC_Setup(CScanParameterSaver::Instance().m_lBrightnessSaved, CScanParameterSaver::Instance().m_lContrastSaved);

	// 停止图象分析
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	CSemVirtualMCICard::Instance().FramGrayHistogramStop();
	CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

	// 恢复扫描
	CScanParameterManager::Instance().m_nusbPixRate = CScanParameterManager::Instance().m_nusbPixRateSaved;
	USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanTypeSaved );
}

void CScanView::AutoBCOK( long lBrightness, long lContrast )
{
	CScanParameterSaver::Instance().m_lBrightnessSaved = lBrightness;
	CScanParameterSaver::Instance().m_lContrastSaved = lContrast;

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoBCProcess()
{
	if ( !CSemVirtualMCICard::Instance().IsReadyFramGrayHistogram() )
		return;

	DWORD	dwTotalPixel;
	dwTotalPixel = CSemVirtualMCICard::Instance().CopyFramGrayHistogramBuff( m_dwFramGrayHistogramBuff );
	CSemVirtualMCICard::Instance().FramGrayHistogramReset();
	CSemVirtualMCICard::Instance().FramGrayHistogramStart();

	// 分析灰度直方图，尝试新的亮度、对比度
	long	lContrast;
	long	lBrightness;
	UINT nRet = AutoBC_Analyze_GrayHistogram( dwTotalPixel, m_dwFramGrayHistogramBuff, lBrightness, lContrast );

#ifdef DEBUG_Info_ABC
	// Test 调试信息
	CString		csMsg;

	csMsg.Format("%d B0=%3d B=%3d C=%3d P=%6d L=%5.1f PeakMin=%5.1f",
		m_nLoop++, CScanParameterSaver::Instance().m_lBrightnessSaved, lBrightness, lContrast,
		dwTotalPixel, dwTotalPixel/256.0, dwTotalPixel * dblPeakMinParam);
//	((CMainFrame *)theApp.GetMainWnd())->
//		m_MainConsole.m_ctrlMsg.SetWindowText( csMsg );
		theApp.DisplayMsg( csMsg );
#endif //DEBUG_Info_ABC
	
	if ( nRet == AutoBC_Analyze_Continue )
	{
		AutoBC_Setup(lBrightness, lContrast);
	}
	else
	{
		// 退出自动亮度、对比度
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		CSemVirtualMCICard::Instance().FramGrayHistogramStop();
		CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

		if ( nRet == AutoBC_Analyze_Completed )
			AutoBCOK( lBrightness, lContrast );
		else
			AutoBCOK( CScanParameterSaver::Instance().m_lBrightnessSaved, CScanParameterSaver::Instance().m_lContrastSaved );
	}
}


// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理

void CScanView::StartAutoBC2()
{
	// 保存现场
	CScanParameterManager::Instance().m_nusbScanTypeSaved	= CScanParameterManager::Instance().m_nusbScanType;
	CScanParameterManager::Instance().m_nusbPixRateSaved	= CScanParameterManager::Instance().m_nusbPixRate;

	CScanParameterSaver::Instance().m_lBrightnessSaved		= CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal;
	if (CHVPowerDataManager::Instance().m_dwHVtype < 1)
	{
		CScanParameterSaver::Instance().m_lContrastSaved		= CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal;
	}
	else
	{
		CScanParameterSaver::Instance().m_lContrastSaved		= CSemCoreAssemble::Instance().m_SP_PMT.GetPos().lVal;
	}
	
	long lCondenserLens		= CSemCoreAssemble::Instance().m_CondenserLens.GetPos().lVal;
	CScanParameterSaver::Instance().m_lObjectiveLensSaved	= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;

	CString		csMsg;
	csMsg.Format("AutoBC2 Start! ScanType=%3d ScanSpeed=%3d B=%6.2d, C=%6.2d, CL=[0x%04X] %5.2f, OL=[0x%04X] %5.2f ",
		CScanParameterManager::Instance().m_nusbScanTypeSaved,CScanParameterManager::Instance().m_nusbPixRateSaved,CScanParameterSaver::Instance().m_lBrightnessSaved, CScanParameterSaver::Instance().m_lContrastSaved, lCondenserLens,lCondenserLens/OL_TO_LED*2, CScanParameterSaver::Instance().m_lObjectiveLensSaved,CScanParameterSaver::Instance().m_lObjectiveLensSaved/OL_TO_LED*2);
	theApp.DisplayMsg( csMsg );

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	if( ( lCondenserLens / OL_TO_LED *2 > 900 )
		|| ( lCondenserLens / OL_TO_LED *2 < 200 ) )
	{
		// 设置聚光镜聚焦
		ClassID.lVal	= SemControlPanelClass_CondenserLens;
		Flag.lVal		= 1;
		Xpos.lVal		= (long)(500*OL_TO_LED /2);

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
// 		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
// 		if( pfrm->m_bIsDebugCreated )
		{
			CString csMsg;
			csMsg.Format("Current CondenserLens(%5.2f) out of [200,900], turn CondenserLens to 500.00 ",
				lCondenserLens/OL_TO_LED*2);
			theApp.DisplayMsg( csMsg );
		}
	}
	if( (CScanParameterSaver::Instance().m_lObjectiveLensSaved / OL_TO_LED *2 > 900 )
		|| ( CScanParameterSaver::Instance().m_lObjectiveLensSaved / OL_TO_LED *2 < 200 ) )//根据统调表
	{
		// 设置物镜聚焦
		ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
		Flag.lVal		= 1;
		Xpos.dblVal		= 500*OL_TO_LED/2;
		Xpos.lVal		= (long)(Xpos.dblVal);

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

// 		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
// 		if( pfrm->m_bIsDebugCreated )
		{
			CString csMsg;
			csMsg.Format("Current ObjectiveLens(%5.2f) out of [200,900], turn ObjectiveLens to 500.00 ",
				CScanParameterSaver::Instance().m_lObjectiveLensSaved/OL_TO_LED*2);
			theApp.DisplayMsg( csMsg );
		}
	}

	// 设定亮度、对比度初始尝试值
	// B: -12.41(0xB7F, GetPos().lVal = -127)
	// C: 62.745(0x5F00, GetPos().lVal = 160)
	long	lBrightness	= -360;
	long	lContrast	= 120;
	// 读取参数值
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\KYKYcfg.ini");
	char		stabuff[20];
	char		staitembuff[20];
	int			nTotal	= 8;
	int*		pCheck	= new int[8];
	CString		str;
	sprintf_s(stabuff, "OneKEY");
	sprintf_s(staitembuff, "param");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	lBrightness	= pCheck[3] -10;
	lContrast	= pCheck[4] -20;
	delete [] pCheck;
	AutoBC_Initialize3( lBrightness, lContrast );

	AutoBC_Setup(lBrightness, lContrast);

	// 设定当前扫描
	CScanParameterManager::Instance().m_nusbPixRate	= 8;	// USB_SS_4us
	USB_ChangeScan( USB_ST_AutoBCArea );

	// 进入自动亮度、对比度BEST过程
	m_nLoop				= 0;
	CScanParameterSaver::Instance().m_nAutoFuction		= AutoBC;
	CSemVirtualMCICard::Instance().FramGrayHistogramReset();
	CSemVirtualMCICard::Instance().FramGrayHistogramStart();
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, nAutoBCTimerElapse, NULL );	
}

void CScanView::AutoBCProcess2()
{
	if ( !CSemVirtualMCICard::Instance().IsReadyFramGrayHistogram() )
		return;

	DWORD	dwTotalPixel;
	dwTotalPixel = CSemVirtualMCICard::Instance().CopyFramGrayHistogramBuff( m_dwFramGrayHistogramBuff );
	CSemVirtualMCICard::Instance().FramGrayHistogramReset();
	CSemVirtualMCICard::Instance().FramGrayHistogramStart();

	// 分析灰度直方图，尝试新的亮度、对比度
	long	lContrast;
	long	lBrightness;
	UINT nRet = AutoBC_Analyze_Image2( dwTotalPixel,
		m_dwFramGrayHistogramBuff, lBrightness, lContrast );

	if ( nRet == AutoBC_Analyze_Continue )
	{
		AutoBC_Setup(lBrightness, lContrast);

// 		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
// 		if( pfrm->m_bIsDebugCreated )
		{
			CString		csMsg;
			csMsg.Format("ABC-%02d, B0=%03d, B=%03d, C=%03d, P=%06d, L=%05.1f, PeakMin=%05.1f",
				m_nLoop++, CScanParameterSaver::Instance().m_lBrightnessSaved, lBrightness, lContrast,
				dwTotalPixel, dwTotalPixel/256.0, dwTotalPixel * dblPeakMinParam);
			theApp.DisplayMsg( csMsg );
		}
	}
	else
	{
		// 退出自动亮度、对比度
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		CSemVirtualMCICard::Instance().FramGrayHistogramStop();
		CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

		// 恢复扫描
		CScanParameterManager::Instance().m_nusbPixRate = CScanParameterManager::Instance().m_nusbPixRateSaved;
		USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanTypeSaved );

		CString		csMsg;
		if ( nRet == AutoBC_Analyze_Completed )
		{
			AutoBCOK( lBrightness, lContrast );
			csMsg.Format("ABC-Complete! Current B=%5d, C=%5d",CScanParameterSaver::Instance().m_lBrightnessSaved, CScanParameterSaver::Instance().m_lContrastSaved);
		}
		else
		{
			AutoBCOK( CScanParameterSaver::Instance().m_lBrightnessSaved, CScanParameterSaver::Instance().m_lContrastSaved );
			csMsg.Format("ABC-Finish! Saved B=%5d, C=%5d",CScanParameterSaver::Instance().m_lBrightnessSaved, CScanParameterSaver::Instance().m_lContrastSaved);
		}
		theApp.DisplayMsg( csMsg );
	}
}

void CScanView::StopAutoBC2()
{
	// 恢复亮度、对比度
	AutoBC_Setup(CScanParameterSaver::Instance().m_lBrightnessSaved, CScanParameterSaver::Instance().m_lContrastSaved);

	// 停止图象分析
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	CSemVirtualMCICard::Instance().FramGrayHistogramStop();
	CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

	// 恢复扫描
	CScanParameterManager::Instance().m_nusbPixRate = CScanParameterManager::Instance().m_nusbPixRateSaved;
	USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanTypeSaved );
}
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理



//进行自动聚焦调节
void CScanView::OnAutoFocus() 
{
	FuncAutoFocus1();
}

void CScanView::FuncAutoFocus()
{
	CDlgProgress	dlg;

	dlg.m_csTitle	= GetResString( IDS_SD_TITLE_AUTO_Focus );
	dlg.m_iRangeTo	= nAFTotalTime;		// 100 s
	dlg.m_csMessage = GetResString( IDS_STRING_AUTO_FOCUS );

	StartAutoFocus();

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	StopAutoFocus();

	//CAutoFocusThread::Instance().Release();
	//CAutoFocusThread* pThread = new CAutoFocusThread;
	//pThread->Initialize(m_hWnd,theApp.m_dwOpr);
	//pThread->SetSleep(CTestParameterManager::Instance().m_dwSleep_AutoFocus);
	//pThread->Start();
}

void CScanView::AutoFocus_Setup(const long lObjectiveLens, const double dblMagnifier )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	CScanParameterSaver::Instance().m_lLastlObjectiveLens	= lObjectiveLens;

	CScanParameterSaver::Instance().m_dbLastlMagnifierSaved =dblMagnifier;

	// 设置物镜聚焦
	ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
	Flag.lVal		= 1;
	Xpos.dblVal		= lObjectiveLens;
	Xpos.lVal		= lObjectiveLens;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	// 设置放大倍数
	ClassID.lVal	= SemControlPanelClass_Magnifier;
	Flag.lVal		= 1;
	Xpos.dblVal		= dblMagnifier;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag  );
}

void CScanView::StartAutoFocus()
{
	// 保存现场
	CScanParameterManager::Instance().m_nusbScanTypeSaved	= CScanParameterManager::Instance().m_nusbScanType;
	CScanParameterManager::Instance().m_nusbPixRateSaved	= CScanParameterManager::Instance().m_nusbPixRate;

	CScanParameterSaver::Instance().m_lObjectiveLensSaved	= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
	CScanParameterSaver::Instance().m_dblMagnifierSaved		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;

	if( theApp.m_dwOpr > 0 )
	{
		// Test 调试信息
		CString		csMsg;
		csMsg.Format("AF-Start, F =[0x%04X] %5.1f, M = %5.1f",
			CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_lObjectiveLensSaved/OL_TO_LED*2, CScanParameterSaver::Instance().m_dblMagnifierSaved );
		theApp.DisplayMsg( csMsg );
	}
	else
		theApp.DisplayMsg( "AF-Start" );

	// 设定初始尝试值
	long 		lObjectiveLens	= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
	double		dblMagnifier	= CScanParameterSaver::Instance().m_dblMagnifierSaved;
	AutoFocus_Initialize2( lObjectiveLens, dblMagnifier );

	AutoFocus_Setup( lObjectiveLens, dblMagnifier );

	// 设定当前扫描
	CScanParameterManager::Instance().m_nusbPixRate = 16;	// USB_SS_8us
	USB_ChangeScan( USB_ST_AutoFocusArea );

	// 进入自动聚焦过程
	m_nLoop = 0;
	CScanParameterSaver::Instance().m_nAutoFuction = AutoFocus;
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, nAutoFocusTimerElapse, NULL );
}

void CScanView::StopAutoFocus()
{
	// 恢复物镜、放大器
	AutoFocus_Setup( CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_dblMagnifierSaved );

	// 停止图象分析
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

	// 恢复扫描
	CScanParameterManager::Instance().m_nusbPixRate = CScanParameterManager::Instance().m_nusbPixRateSaved;
	USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanTypeSaved );

	// 结束自动功能
}

void CScanView::AutoFocusOK( long lObjectiveLens )
{
	CScanParameterSaver::Instance().m_lObjectiveLensSaved = lObjectiveLens;

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoFocusProcess()
{
	DWORD lastTickCount = timeGetTime();
	// 复制图象
	CRect	rcDest = CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	DWORD dwTotalPixel = CSemVirtualMCICard::Instance().Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );
	DWORD thisTickCount = timeGetTime();
	DWORD ReadTickCount = thisTickCount - lastTickCount;

	lastTickCount = timeGetTime();
	// 分析图象，尝试新的聚焦值、放大倍数
	long 		lObjectiveLens_Old	= CScanParameterSaver::Instance().m_lLastlObjectiveLens;
	long 		lObjectiveLens		= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
	double		dblMagnifier		= CScanParameterSaver::Instance().m_dblMagnifierSaved;
	//zyx add 090407
	double		dblMagnifier_Old	= CScanParameterSaver::Instance().m_dbLastlMagnifierSaved;
	double		dfSharpness = -1;
	UINT nRet = AutoFocus_Analyze_Image2((LPVOID)m_bImageBuff, rcDest.Size(),dfSharpness, lObjectiveLens, dblMagnifier );

	thisTickCount = timeGetTime();
	DWORD AnalyzeTickCount = thisTickCount - lastTickCount;

	// Setup lObjectiveLens, dfMagnifier
	if ( nRet == AutoFocus_Analyze_Continue )
	{
		lastTickCount = timeGetTime();

		AutoFocus_Setup( lObjectiveLens, dblMagnifier );

		thisTickCount = timeGetTime();
		DWORD SetupTickCount = thisTickCount - lastTickCount;

		if( theApp.m_dwOpr > 0 )
		{
			// Test 调试信息
			CString		csMsg;
			csMsg.Format("AF-%2d,time[ R=%3d,A=%3d,S=%3d ],[%5.2f, %.6f, %5.1f]",
				 m_nLoop++, ReadTickCount, AnalyzeTickCount, SetupTickCount,
				lObjectiveLens_Old/OL_TO_LED*2, dfSharpness*1000, dblMagnifier_Old );
			theApp.DisplayMsg( csMsg );
		}
	}
	else
	{
		// 退出自动聚焦
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

		if ( nRet == AutoFocus_Analyze_Completed )
		{
			if( theApp.m_dwOpr > 0 )
			{
				// Test 调试信息
				CString		csMsg;
				csMsg.Format("AF-Complete, F = %5.2f,[%5.2f,%.6f, %5.1f]", lObjectiveLens/OL_TO_LED*2,lObjectiveLens_Old/OL_TO_LED*2,dfSharpness*1000, dblMagnifier_Old );
				theApp.DisplayMsg( csMsg );
				TRACE( csMsg );
			}
			else
				theApp.DisplayMsg( "AF-Complete" );
			AutoFocusOK( lObjectiveLens );
		}
		else
		{
			if( theApp.m_dwOpr > 0 )
			{
				// Test 调试信息
				CString		csMsg;
				csMsg.Format("AF-Finish, F = %5.1f", CScanParameterSaver::Instance().m_lObjectiveLensSaved/OL_TO_LED*2 );
				theApp.DisplayMsg( csMsg );
				TRACE( csMsg );
			}
			else
				theApp.DisplayMsg( "AF-Finish" );
			AutoFocusOK( CScanParameterSaver::Instance().m_lObjectiveLensSaved );
		}
	}
}

void CScanView::AFTest()
{
	// 复制图象
	DWORD	dwTotalPixel;
	CRect	rcDest = CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	dwTotalPixel = CSemVirtualMCICard::Instance().Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );

	// 分析图象
	double		dfSharpness = -1;
	double		dfParamX	= -1;
	double		dfParamY	= -1;
	double		dfTotal		= -1;
	Analyze_Image( (LPVOID)m_bImageBuff, rcDest.Size(), dfSharpness, dfTotal );

	// Test 调试信息
	CString		csMsg;
	csMsg.Format("AF, %5.1f, %5.1f, %5.1f, %5.1f, %5.1f",
		dfSharpness / dfTotal * 1000,
		dfSharpness, dfParamX, dfParamY, dfTotal );

//	((CMainFrame *)theApp.GetMainWnd())->
//		m_MainConsole.m_ctrlMsg.SetWindowText( csMsg );
	
	theApp.DisplayMsg( csMsg );
}

//2022.12.28 hy 进行自动聚焦调节

void CScanView::FuncAutoFocus1()
{
	CDlgInput  input;
	input.m_nTitleIndex = IDS_SD_PROMPT_Input_SampleHeight;
	if( input.DoModal() != IDOK )
		return;
	CScanParameterSaver::Instance().m_fSampleHeight = (float)(atof( input.m_strInput ));

	CDlgProgress	dlg;

	dlg.m_csTitle	= GetResString( IDS_SD_TITLE_AUTO_Focus );
	dlg.m_iRangeTo	= nAFTotalTime;		// 100 s
	dlg.m_csMessage = GetResString( IDS_STRING_AUTO_FOCUS );

	StartAutoFocus1();

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	StopAutoFocus1();

	//CAutoFocusThread::Instance().Release();
	//CAutoFocusThread* pThread = new CAutoFocusThread;
	//pThread->Initialize(m_hWnd,theApp.m_dwOpr);
	//pThread->SetSleep(CTestParameterManager::Instance().m_dwSleep_AutoFocus);
	//pThread->Start();
}

void CScanView::AutoFocus_Setup1(const long lObjectiveLens, const double dblMagnifier )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	CScanParameterSaver::Instance().m_lLastlObjectiveLens	= lObjectiveLens;
	CScanParameterSaver::Instance().m_dbLastlMagnifierSaved = dblMagnifier;

	// 设置物镜聚焦
	ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
	Flag.lVal		= 1;
	Xpos.dblVal		= lObjectiveLens;
	Xpos.lVal		= lObjectiveLens;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	// 设置放大倍数
	ClassID.lVal	= SemControlPanelClass_Magnifier;
	Flag.lVal		= 1;
	Xpos.dblVal		= dblMagnifier;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag  );
}

void CScanView::StartAutoFocus1()
{
	//读取工作距离 Z0WD - Z - SampleH
	//基于手动样品台测试，直接给定工作距离
	float fValue[5], fZ0WD, fZcur;
	fZ0WD = fZcur = 0.0f;
	double dValue = 0.0;
	for( int i=0; i<5; i++ )
		fValue[i] = 0.0f;
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 )
	{
		StageTMCM_GetParam(2, fValue[0], fValue[1], fValue[2], fValue[3], fValue[4] );
		fZcur = fValue[2];	// Z 当前值
		StageTMCM_GetParam(3, fValue[0], fValue[1], fValue[2], fValue[3], fValue[4] );
		fZ0WD = fValue[0];
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 )
	{
		StageMC6600_GetParam(2, fValue[0], fValue[1], fValue[2], fValue[3], fValue[4] );
		fZcur = fValue[2];	// Z 当前值
		StageMC6600_GetParam(3, fValue[0], fValue[1], fValue[2], fValue[3], fValue[4] );
		fZ0WD = fValue[0];
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		StageMC600_GetParam(2, fValue[0], fValue[1], fValue[2], fValue[3], fValue[4] );
		fZcur = fValue[2];	// Z 当前值
		StageMC600_GetParam(3, fValue[0], fValue[1], fValue[2], fValue[3], fValue[4] );
		fZ0WD = fValue[0];
	}

	dValue = fZ0WD - fZcur - CScanParameterSaver::Instance().m_fSampleHeight;	// WD
	//根据6900统调公式计算相对清晰物镜值  dol = sqrt(1.0/(C+dwd*(b+A*dwd))) 及放大倍数
	CSemCoreAssemble::Instance().m_WD.GetParams(0, dValue );	// 由dWD算出dOL
	long lObjectiveLens = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPosByValue( dValue );	// 由dOL得到lOL

	// 保存现场
	CScanParameterManager::Instance().m_nusbScanTypeSaved	= CScanParameterManager::Instance().m_nusbScanType;
	CScanParameterManager::Instance().m_nusbPixRateSaved	= CScanParameterManager::Instance().m_nusbPixRate;
	CScanParameterSaver::Instance().m_lObjectiveLensSaved	= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
	CScanParameterSaver::Instance().m_dblMagnifierSaved		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;

	if( theApp.m_dwOpr > 0 )
	{
		// Test 调试信息
		CString		csMsg;
		csMsg.Format("AF-Start, F =[0x%04X] %5.1f, M = %5.1f",
			CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_lObjectiveLensSaved/OL_TO_LED*2, CScanParameterSaver::Instance().m_dblMagnifierSaved );
		theApp.DisplayMsg( csMsg );
	}
	else
		theApp.DisplayMsg( "AF-Start" );

	// 设定初始尝试值
	double		dblMagnifier	= CScanParameterSaver::Instance().m_dblMagnifierSaved;
	AutoFocus_Initialize1( lObjectiveLens, dblMagnifier );
	AutoFocus_Setup1( lObjectiveLens, dblMagnifier );

	// 设定当前扫描
	CScanParameterManager::Instance().m_nusbPixRate = 16;	// USB_SS_8us
	USB_ChangeScan( USB_ST_AutoFocusArea );

	// 进入自动聚焦过程
	m_nLoop = 0;
	CScanParameterSaver::Instance().m_nAutoFuction = AutoFocus1;
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, nAutoFocusTimerElapse, NULL );

/*
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\KYKYcfg.ini");
	char szBuf[255], szBuf2[255];

	double dCoFactorA = 0.0;
	double dCoFactorB = 0.0;
	double dCoFactorC = 0.0;
	if(GetPrivateProfileString("Coadjust", "Factors", "", szBuf, 255, path) != 0)
	{
		strcpy_s(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy_s(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			dCoFactorA = (double)atof(szBuf2);
		}
		strcpy_s(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy_s(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			dCoFactorB = (double)atof(szBuf2);
		}
		dCoFactorC = (double)atof(szBuf);
	}
	//基于手动样品台测试，直接给定工作距离
	double dZ, dZ0WD = 0.0;
	if(CStageXYManager::Instance().m_Stage.nHasAutoStage < 4 )
	{
		if(exd != NULL)
			*exd = '\0';
		strcat_s(path, "\\Stage\\StageMC600.ini");
		dZ0WD = 0.0;
		if(GetPrivateProfileString("StageMC600Cfg", "Z0WD", "", szBuf, 255, path) != 0)
			dZ0WD = (double)(atof(szBuf));

		float fX1, fX2, fY1, fY2, fZ1, fZ2, fR1, fR2, fT1, fT2;
		StageMC600_GetParam(2,fX1, fY1, fZ1, fR1, fT1 );

		dZ = fZ1;
	}
	else
	{
		if(exd != NULL)
			*exd = '\0';
		strcat_s(path, "\\Stage\\StageMC6600.ini");
		dZ0WD = 0.0;
		if(GetPrivateProfileString("StageMC6600Cfg", "Z0wd", "", szBuf, 255, path) != 0)
			dZ0WD = (double)(atof(szBuf));

		float fX1, fX2, fY1, fY2, fZ1, fZ2, fR1, fR2, fT1, fT2;
		StageMC6600_GetParam(2,fX1, fY1, fZ1, fR1, fT1 );

		dZ = fZ1;
	}

	float fSampleHeight = CScanParameterSaver::Instance().m_fSampleHeight;

	double dWD = dZ0WD - dZ - fSampleHeight ;

	//m_dWD = 21.2;

	//根据6900统调公式计算相对清晰物镜值  dol = sqrt(1.0/(C+dwd*(b+A*dwd))) 及放大倍数

	long lObjectiveLensSet = (sqrt(1.0/(dCoFactorC + dWD * ( dCoFactorB +  dWD * dCoFactorA ))))*32;

	CScanParameterSaver::Instance().m_dblMagnifierSaved		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	m_dMagSet	= CScanParameterSaver::Instance().m_dblMagnifierSaved;

	//物镜值设置

	AutoFocus_Setup1( lObjectiveLensSet, m_dMagSet );
	Sleep(400);
	// 保存现场
	CScanParameterManager::Instance().m_nusbScanTypeSaved	= CScanParameterManager::Instance().m_nusbScanType;
	CScanParameterManager::Instance().m_nusbPixRateSaved	= CScanParameterManager::Instance().m_nusbPixRate;

	CScanParameterSaver::Instance().m_lObjectiveLensSaved	= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
	CScanParameterSaver::Instance().m_dblMagnifierSaved		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;

	if( theApp.m_dwOpr > 0 )
	{
		// Test 调试信息
		CString		csMsg;
		csMsg.Format("AF-Start, F =[0x%04X] %5.1f, M = %5.1f",
			CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_lObjectiveLensSaved/OL_TO_LED*2, CScanParameterSaver::Instance().m_dblMagnifierSaved );
		theApp.DisplayMsg( csMsg );
	}
	else
		theApp.DisplayMsg( "AF-Start" );

	// 设定初始尝试值
	long 		lObjectiveLens	= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
	double		dblMagnifier	= CScanParameterSaver::Instance().m_dblMagnifierSaved;

	double      dblWorkDis      =  CSemCoreAssemble::Instance().m_WD.GetPos().dblVal;

	AutoFocus_Initialize1( lObjectiveLens, dblMagnifier );

	AutoFocus_Setup1( lObjectiveLens, dblMagnifier );

	// 设定当前扫描
	CScanParameterManager::Instance().m_nusbPixRate = 16;	// USB_SS_8us
	USB_ChangeScan( USB_ST_AutoFocusArea );

	// 进入自动聚焦过程
	m_nLoop = 0;
	CScanParameterSaver::Instance().m_nAutoFuction = AutoFocus1;
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, nAutoFocusTimerElapse, NULL );*/
}

void CScanView::StopAutoFocus1()
{
	// 恢复物镜、放大器
	AutoFocus_Setup1( CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_dblMagnifierSaved );

	// 停止图象分析
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

	// 恢复扫描
	CScanParameterManager::Instance().m_nusbPixRate = CScanParameterManager::Instance().m_nusbPixRateSaved;
	USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanTypeSaved );

	// 结束自动功能
}

void CScanView::AutoFocusOK1( long lObjectiveLens )
{
	CScanParameterSaver::Instance().m_lObjectiveLensSaved = lObjectiveLens;

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoFocusProcess1()
{
	DWORD lastTickCount = timeGetTime();
	// 复制图象
	CRect	rcDest = CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	DWORD dwTotalPixel = CSemVirtualMCICard::Instance().Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );
	DWORD thisTickCount = timeGetTime();
	DWORD ReadTickCount = thisTickCount - lastTickCount;

	lastTickCount = timeGetTime();
	// 分析图象，尝试新的聚焦值、放大倍数
	long 		lObjectiveLens_Old	= CScanParameterSaver::Instance().m_lLastlObjectiveLens;
	long 		lObjectiveLens		= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
	double		dblMagnifier		= CScanParameterSaver::Instance().m_dblMagnifierSaved;
	//zyx add 090407
	double		dblMagnifier_Old	= CScanParameterSaver::Instance().m_dbLastlMagnifierSaved;
	double		dfSharpness = -1;
	
	UINT nRet = AutoFocus_Analyze_Image1((LPVOID)m_bImageBuff, rcDest.Size(),dfSharpness,lObjectiveLens_Old , dblMagnifier ,lObjectiveLens);

	thisTickCount = timeGetTime();
	DWORD AnalyzeTickCount = thisTickCount - lastTickCount;

	// Setup lObjectiveLens, dfMagnifier
	if ( nRet == AutoFocus_Analyze_Continue )
	{
		lastTickCount = timeGetTime();

		AutoFocus_Setup1( lObjectiveLens_Old, dblMagnifier );
		Sleep(200);
		thisTickCount = timeGetTime();
		DWORD SetupTickCount = thisTickCount - lastTickCount;

		if( theApp.m_dwOpr > 0 )
		{
			// Test 调试信息
			CString		csMsg;
			csMsg.Format("AF-%2d,time[ R=%3d,A=%3d,S=%3d ],[%5.2f, %.6f, %5.1f]",
				 m_nLoop++, ReadTickCount, AnalyzeTickCount, SetupTickCount,
				lObjectiveLens_Old/OL_TO_LED*2, dfSharpness*1000, dblMagnifier_Old );
			theApp.DisplayMsg( csMsg );
		}
	}
	else
	{
		// 退出自动聚焦
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

		char sz[512] = {0};
	    ZeroMemory(sz,512);
		sprintf_s(sz,512,"AutoFocus_Analyze_Finsh ----- lObjectiveLens = %ld \n", lObjectiveLens/32.);
		OutputDebugString(sz);

		if ( nRet == AutoFocus_Analyze_Completed )
		{
			if( theApp.m_dwOpr > 0 )
			{
				// Test 调试信息
				CString		csMsg;
				csMsg.Format("AF-Complete, F = %5.2f,[%5.2f,%.6f, %5.1f]", lObjectiveLens/OL_TO_LED*2,lObjectiveLens_Old/OL_TO_LED*2,dfSharpness*1000, dblMagnifier_Old );
				theApp.DisplayMsg( csMsg );
				TRACE( csMsg );
			}
			else
				theApp.DisplayMsg( "AF-Complete" );
			AutoFocusOK1( lObjectiveLens_Old );
		}
		else
		{
			if( theApp.m_dwOpr > 0 )
			{
				// Test 调试信息
				CString		csMsg;
				csMsg.Format("AF-Finish, F = %5.1f", CScanParameterSaver::Instance().m_lObjectiveLensSaved/OL_TO_LED*2 );
				theApp.DisplayMsg( csMsg );
				TRACE( csMsg );
			}
			else
				theApp.DisplayMsg( "AF-Finish" );
			AutoFocusOK1( CScanParameterSaver::Instance().m_lObjectiveLensSaved );
		}
		
	}
}

void CScanView::AFTest1()
{
	// 复制图象
	DWORD	dwTotalPixel;
	CRect	rcDest = CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	dwTotalPixel = CSemVirtualMCICard::Instance().Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );

	// 分析图象
	double		dfSharpness = -1;
	double		dfParamX	= -1;
	double		dfParamY	= -1;
	double		dfTotal		= -1;
	Analyze_Image1( (LPVOID)m_bImageBuff, rcDest.Size(), dfSharpness );

	// Test 调试信息
	CString		csMsg;
	csMsg.Format("AF, %5.1f, %5.1f, %5.1f, %5.1f, %5.1f",
		dfSharpness  * 1000,
		dfSharpness, dfParamX, dfParamY, dfTotal );

//	((CMainFrame *)theApp.GetMainWnd())->
//		m_MainConsole.m_ctrlMsg.SetWindowText( csMsg );
	
	theApp.DisplayMsg( csMsg );
}

// 进行自动衬度调节
void CScanView::OnAutoContrast()
{
	CAutoContrastDlg  dlgAutocontrast;
	dlgAutocontrast.m_nTitleIndex = IDS_MENUITEM_AUTO_Contrast;
	dlgAutocontrast.DoModal();
}

//进行自动消像散调节
void CScanView::OnAutoStigmator() 
{
	FuncAutoStigmator();
}

void CScanView::FuncAutoStigmator()
{
	CDlgProgress	dlg;

	dlg.m_csTitle	= GetResString( IDS_SD_TITLE_AUTO_Stigmator );
	dlg.m_iRangeTo	= nASTotalTime;		// 200 s
	dlg.m_csMessage	= GetResString( IDS_STRING_AUTO_STIGMATOR );

	StartAutoAstigmat();

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	StopAutoAstigmat();
}


void AutoAstigmat_Setup(long lObjectiveLens, double dblMagnifier, long lStigmatorX, long lStigmatorY )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	// 设置物镜聚焦
	ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
	Flag.lVal		= 1;
	Xpos.dblVal		= lObjectiveLens;
	Xpos.lVal		= lObjectiveLens;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	// 设置消像散器
	ClassID.lVal	= SemControlPanelClass_Stigmator;
	Flag.lVal		= 3;
	Xpos.lVal		= lStigmatorX;
	Ypos.lVal		= lStigmatorY;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

void CScanView::StartAutoAstigmat()
{
	// 保存现场
	CScanParameterManager::Instance().m_nusbScanTypeSaved	= CScanParameterManager::Instance().m_nusbScanType;
	CScanParameterManager::Instance().m_nusbPixRateSaved	= CScanParameterManager::Instance().m_nusbPixRate;

	CScanParameterSaver::Instance().m_lStigmatorXSaved		= CSemCoreAssemble::Instance().m_StigmatorX.GetPos().lVal;
	CScanParameterSaver::Instance().m_lStigmatorYSaved		= CSemCoreAssemble::Instance().m_StigmatorY.GetPos().lVal;
	CScanParameterSaver::Instance().m_lObjectiveLensSaved	= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
	CScanParameterSaver::Instance().m_dblMagnifierSaved		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;

	// 设定初始尝试值
	long	lObjectiveLens	= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
	double	dblMagnifier	= CScanParameterSaver::Instance().m_dblMagnifierSaved;
	long	lStigmatorX		= CScanParameterSaver::Instance().m_lStigmatorXSaved;
	long	lStigmatorY		= CScanParameterSaver::Instance().m_lStigmatorYSaved;

	AutoAstigmat_Initialize( lObjectiveLens, dblMagnifier, lStigmatorX, lStigmatorY );

	AutoAstigmat_Setup( lObjectiveLens, dblMagnifier, lStigmatorX, lStigmatorY );

	// 设定当前扫描
	CScanParameterManager::Instance().m_nusbPixRate = 8;	// USB_SS_4us
	USB_ChangeScan( USB_ST_AutoAstigmatArea );

	// 进入自动消像散过程
	m_nLoop = 0;
	CScanParameterSaver::Instance().m_nAutoFuction = AutoAstigmat;
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, nAutoAstigmatTimerElapse, NULL );
}

void CScanView::StopAutoAstigmat()
{
	// 恢复物镜、消像散器
	AutoAstigmat_Setup( CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_dblMagnifierSaved, CScanParameterSaver::Instance().m_lStigmatorXSaved, CScanParameterSaver::Instance().m_lStigmatorYSaved );

	// 停止图象分析
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

	// 恢复扫描
	CScanParameterManager::Instance().m_nusbPixRate = CScanParameterManager::Instance().m_nusbPixRateSaved;
	USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanTypeSaved );

	// 结束自动功能
}

void CScanView::AutoAstigmatOK( long lObjectiveLens, long lStigmatorX, long lStigmatorY )
{
	CScanParameterSaver::Instance().m_lObjectiveLensSaved	= lObjectiveLens;
	CScanParameterSaver::Instance().m_lStigmatorXSaved	= lStigmatorX;
	CScanParameterSaver::Instance().m_lStigmatorYSaved	= lStigmatorY;

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoAstigmatProcess()
{
	DWORD	ReadTickCount, AnalyzeTickCount;
	DWORD	thisTickCount, lastTickCount;


	lastTickCount = timeGetTime();

	// 复制图象
	DWORD	dwTotalPixel;
	CRect	rcDest = CScanParameterManager::Instance().m_rcAutoAstigmatAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	dwTotalPixel = CSemVirtualMCICard::Instance().Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );

	thisTickCount = timeGetTime();
	ReadTickCount = thisTickCount - lastTickCount;

	lastTickCount = timeGetTime();

	// 分析图象，尝试新控制值
	long	lObjectiveLens	= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
	double	dblMagnifier	= CScanParameterSaver::Instance().m_dblMagnifierSaved;
	long	lStigmatorX		= CScanParameterSaver::Instance().m_lStigmatorXSaved;
	long	lStigmatorY		= CScanParameterSaver::Instance().m_lStigmatorYSaved;
	double	dfSharpness		= -1;

	UINT nRet = AutoAstigmat_Analyze_Image(	
		(LPVOID)m_bImageBuff, rcDest.Size(), dfSharpness,
		lObjectiveLens, dblMagnifier, lStigmatorX, lStigmatorY );

	thisTickCount = timeGetTime();
	AnalyzeTickCount = thisTickCount - lastTickCount;

	if( theApp.m_dwOpr > 0 )
	{
		// Test 调试信息
		CString		csMsg;
		csMsg.Format("AS %2d, %5.1f, time[ R=%3d,A=%3d ]",
			 m_nLoop++, dfSharpness*1000, ReadTickCount, AnalyzeTickCount );
		theApp.DisplayMsg( csMsg );
	}

	// AutoAstigmat_Setup( lObjectiveLens, nMagCtrl, lStigmatorX, lStigmatorY )
	if ( nRet == AutoAstigmat_Analyze_Continue )
	{
		AutoAstigmat_Setup( lObjectiveLens, dblMagnifier, lStigmatorX, lStigmatorY );
	}
	else
	{
		// 退出自动消像散
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

		if ( nRet == AutoAstigmat_Analyze_Completed )
		{
			AutoAstigmatOK( lObjectiveLens, lStigmatorX, lStigmatorY );

			// Test 调试信息
			CString		csMsg;
			csMsg = _T("AS Complete");
			theApp.DisplayMsg( csMsg );
		}
		else
		{
			AutoAstigmatOK( CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_lStigmatorXSaved, CScanParameterSaver::Instance().m_lStigmatorYSaved );

			// Test 调试信息
			CString		csMsg;
			csMsg = _T("AS Finish");
			theApp.DisplayMsg( csMsg );
		}
	}
}


void CScanView::GetFitScreenClientRect( CRect& rcScreenClientRect )
{
	CRect rc;
	GetClientRect( (LPRECT)rc );
//	if( m_bSplit )
//		rc.right = rc.left +rc.Width() /2;
	rcScreenClientRect = rc;
}

void CScanView::UpdateScale()
{
	CRect rcScreenClientRect;
	GetFitScreenClientRect( rcScreenClientRect );

//	double dScale = (double)rcScreenClientRect.Width() / g_szFrameGraph.cx;
	double dScale = (double)rcScreenClientRect.Width() / CMCICardCommonSetting::Instance().m_sizeReso.cx;
	if( m_bSplit )
		dScale /= 2;
	if( dScale == 0.0 )
		return;

	CScaleManager::Instance().UpdateScale(dScale,m_bZoomFit);
}

void CScanView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	// 扫描区域设为正方形
/*	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm != NULL && pfrm->m_pwndSplitter->m_hWnd != NULL )
	{
		CRect rect;
		GetClientRect( &rect );
		pfrm->m_pwndSplitter->SetColumnInfo(0, rect.Height(), 100);
		pfrm->m_pwndSplitter->RecalcLayout();
	}
*/
	UpdateItemRect( FALSE );
	UpdateScale();
	
	// TODO: Add your message handler code here
/*	if (m_nMapMode == MM_SCALETOFIT)
	{
		// force recalculation of scale to fit parameters
		SetScaleToFitSize(m_szTotalLog);
	}
	else
	{
		// UpdateBars() handles locking out recursion
		UpdateBars();
	}*/
}


// Set mode and scaling/scrolling sizes

void CScanView::SetScaleToFitSize(SIZE sizeTotal)
{
	// Note: It is possible to set sizeTotal members to negative values to
	//  effectively invert either the X or Y axis.

	ASSERT(m_hWnd != NULL);
	m_nMapMode = MM_SCALETOFIT;     // special internal value
	m_szTotalLog = sizeTotal;

	// reset and turn any scroll bars off
	if (m_hWnd != NULL && (GetStyle() & (WS_HSCROLL|WS_VSCROLL)))
	{
		SetScrollPos(SB_HORZ, 0);
		SetScrollPos(SB_VERT, 0);
		EnableScrollBarCtrl(SB_BOTH, FALSE);
		ASSERT((GetStyle() & (WS_HSCROLL|WS_VSCROLL)) == 0);
	}

	CRect rectT;
	GetClientRect(rectT);
	m_szTotalDev = rectT.Size();

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate
		UpdateBars();
		Invalidate(TRUE);
	}
}

void CScanView::UpdateBars()
{
	// UpdateBars may cause window to be resized - ignore those resizings
	if (m_bInsideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT(m_szTotalDev.cx >= 0 && m_szTotalDev.cy >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	CSize sizeClient;
	CSize sizeSb;

	GetClientRect(&rectClient);

	if (bCalcClient)
	{
		// get client rect
		if (!GetTrueClientSize(sizeClient, sizeSb))
		{
			// no room for scroll bars (common for zero sized elements)
			CRect rect;
			GetClientRect(&rect);
			if (rect.right > 0 && rect.bottom > 0)
			{
				// if entire client area is not invisible, assume we have
				//  control over our scrollbars
				EnableScrollBarCtrl(SB_BOTH, FALSE);
			}
			m_bInsideUpdate = FALSE;
			return;
		}
	}
	else
	{
		// let parent window determine the "client" rect
		GetScrollBarSizes(sizeSb);
		sizeClient.cx = rectClient.right - rectClient.left;
		sizeClient.cy = rectClient.bottom - rectClient.top;
	}

	// enough room to add scrollbars
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// get the current scroll bar state given the true client area
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
	if (needSb.cx)
		sizeClient.cy -= sizeSb.cy;
	if (needSb.cy)
		sizeClient.cx -= sizeSb.cx;

	// first scroll the window as needed
	ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	if (needSb.cx)
	{
		info.nPage = sizeClient.cx;
		info.nMax = m_szTotalDev.cx-1;
		if (!SetScrollInfo(SB_HORZ, &info, TRUE))
			SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
	}
	EnableScrollBarCtrl(SB_VERT, needSb.cy);
	if (needSb.cy)
	{
		info.nPage = sizeClient.cy;
		info.nMax = m_szTotalDev.cy-1;
		if (!SetScrollInfo(SB_VERT, &info, TRUE))
			SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// remove recursion lockout
	m_bInsideUpdate = FALSE;
}

void CScanView::ScrollToDevicePosition(POINT ptDev)
{
	ASSERT(ptDev.x >= 0);
	ASSERT(ptDev.y >= 0);

	// Note: ScrollToDevicePosition can and is used to scroll out-of-range
	//  areas as far as CScanView is concerned -- specifically in
	//  the print-preview code.  Since OnScrollBy makes sure the range is
	//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos(SB_HORZ, ptDev.x);
	int yOrig = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, ptDev.y);
	ScrollWindow(xOrig - ptDev.x, yOrig - ptDev.y);
}

// helper to return the state of the scrollbars without actually changing
//  the state of the scrollbars
void CScanView::GetScrollBarState(CSize sizeClient, CSize& needSb,
	CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient)
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// enough room to add scrollbars
	sizeRange = m_szTotalDev - sizeClient;
		// > 0 => need to scroll
	ptMove = GetDeviceScrollPosition();
		// point to move to (start at current scroll pos)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// need a horizontal scrollbar after all
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
		ptMove.x = sizeRange.cx;
	if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
		ptMove.y = sizeRange.cy;

	// now update the bars as appropriate
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, and ptMove area now all updated
}

/////////////////////////////////////////////////////////////////////////////
// Tie to scrollbars and CWnd behaviour

void CScanView::GetScrollBarSizes(CSize& sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = GetStyle();

	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		sizeSb.cx = GetSystemMetrics(SM_CXVSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cx -= CX_BORDER;
	}
	if (GetScrollBarCtrl(SB_HORZ) == NULL)
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy = GetSystemMetrics(SM_CYHSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cy -= CY_BORDER;
	}
}

BOOL CScanView::GetTrueClientSize(CSize& size, CSize& sizeSb)
	// return TRUE if enough room to add scrollbars if needed
{
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

CPoint CScanView::GetDeviceScrollPosition() const
{
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);

	if (m_bCenter)
	{
		CRect rect;
		GetClientRect(&rect);

		// if client area is larger than total device size,
		// the scroll positions are overridden to place origin such that
		// output is centered in the window
		// GetDeviceScrollPosition() must reflect this

		if (m_szTotalDev.cx < rect.Width())
			pt.x = -((rect.Width() - m_szTotalDev.cx) / 2);
		if (m_szTotalDev.cy < rect.Height())
			pt.y = -((rect.Height() - m_szTotalDev.cy) / 2);
	}

	return pt;
}


void CScanView::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm )
	{
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.ShowWindow( SW_SHOW );
		if( pfrm->m_wndScanCtrlBar.m_hWnd != NULL )
			pfrm->ShowControlBar(&(pfrm->m_wndScanCtrlBar), TRUE, FALSE);
		if( pfrm->m_HistoBar.m_hWnd != NULL )
			pfrm->ShowControlBar(&(pfrm->m_HistoBar), FALSE, FALSE);
		pfrm->RecalcLayout();
	}
}


// 自动台相关操作
void CScanView::OnStageCoordinateorigin() 
{
	// TODO: Add your command handler code here
	m_bStageOrigin = !m_bStageOrigin;
	if( m_bStageOrigin )
	{
		m_ptPosSave = CPoint(0,0);
		SetStagePointParam( CPoint(0,0) );
	}
	else
		CSemVirtualMCICard::Instance().GraphClear( m_rcStageDragRectLast );
}

void CScanView::OnUpdateStageCoordinateorigin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
	// 所以不光是自动台移动参考点，还复用为校准时的屏幕中心参考点
//	pCmdUI->Enable( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.bAutoStageReady );
	pCmdUI->SetCheck( m_bStageOrigin );
}

void CScanView::SetStagePointParam(CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );

	CPoint ptPoint	= m_ptStagePointParam;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	nOffsetX = nOffsetX / 4 * 4;
	nOffsetY = nOffsetY / 4 * 4;

	ptPoint.x += nOffsetX;
	ptPoint.y += nOffsetY;

	if( ptPoint.x < 20 )
		ptPoint.x = 20;
	if( ptPoint.x > CMCICardCommonSetting::Instance().m_sizeReso.cx -20 )
		ptPoint.x = CMCICardCommonSetting::Instance().m_sizeReso.cx -20;
	if( ptPoint.y < 20 )
		ptPoint.y = 20;
	if( ptPoint.y > CMCICardCommonSetting::Instance().m_sizeReso.cy -20 )
		ptPoint.y = CMCICardCommonSetting::Instance().m_sizeReso.cy -20;
	CRect rect(ptPoint.x-20,ptPoint.y-20,ptPoint.x+20,ptPoint.y+20);
	CSemVirtualMCICard::Instance().GraphClear( m_rcStageDragRectLast );
	CSemVirtualMCICard::Instance().GraphClear( rect, RGB(0,0,0) );

	if( !m_bStageOrigin )
		return;

//	if( m_bStageOrigin && m_bDebugCoadjust )
	{
		// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
		// DebugCoadjust且显示参考点的情况下，参考点固定在中心不许移动
		m_ptStagePointParam.x	= CMCICardCommonSetting::Instance().m_sizeReso.cx /2;
		m_ptStagePointParam.y	= CMCICardCommonSetting::Instance().m_sizeReso.cy /2;
		rect.left	= m_ptStagePointParam.x -20;
		rect.right	= m_ptStagePointParam.x +20;
		rect.top	= m_ptStagePointParam.y -20;
		rect.bottom	= m_ptStagePointParam.y +20;
	}

	ptPos0.x = rect.left+20;
	ptPos0.y = rect.top+20;
	ptPos1 = ptPos0;
	ptPos0.x -= 20;
	ptPos1.x += 20;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,255,0) );

	ptPos0.y += 1;
	ptPos1.y += 1;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,255,0) );

	ptPos0.x = rect.left+20;
	ptPos0.y = rect.top+20;
	ptPos1 = ptPos0;

	ptPos0.y -= 20;
	ptPos1.y += 20;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,255,0) );

	ptPos0.x += 1;
	ptPos1.x += 1;
	CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,255,0) );
	//======2006.09.04_点扫描点的画法=================

	m_rcStageDragRectLast = rect;
}

void CScanView::SetStageTrackParam(CPoint point)
{
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	CPoint ptPoint	= m_ptStageTrackParam;

	// 画线
	BOOL bSameX, bSameY;
	bSameX = bSameY = FALSE;
	if( ptPos1.x == ptPoint.x )
		bSameX = TRUE;
	if( ptPos1.y == ptPoint.y )
		bSameY = TRUE;
	CSemVirtualMCICard::Instance().GraphDrawCircle( ptPoint, ptPos0 );
//	CSemVirtualMCICard::Instance().GraphClear( CRect(ptPos0.x-1, ptPos0.y-1, ptPos0.x+1, ptPos0.y+1) );
	CSemVirtualMCICard::Instance().GraphDrawCircle( ptPoint, ptPos1, TRUE, nSTAGE_TRACE_POINT_RADIUS, RGB(0, 255, 0) );

	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		m_nStageTrackStepX = abs(ptPos1.x - ptPoint.x) *2;
		m_nStageTrackStepY = abs(ptPos1.y - ptPoint.y) *2;
		// 保存位置
		m_ptPosSave = ptPos1;
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 )
			StageTMCM_SetTrackParam( (ptPos1.x<ptPoint.x) ? 0 -m_nStageTrackStepX : m_nStageTrackStepX,
									(ptPos1.y<ptPoint.y) ? 0 -m_nStageTrackStepY : m_nStageTrackStepY );
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 )
			StageMC6600_SetTrackParam( (ptPos1.x<ptPoint.x) ? 0 -m_nStageTrackStepX : m_nStageTrackStepX,
									(ptPos1.y<ptPoint.y) ? 0 -m_nStageTrackStepY : m_nStageTrackStepY );
		else
			StageMC600_SetTrackParam( (ptPos1.x<ptPoint.x) ? 0 -m_nStageTrackStepX : m_nStageTrackStepX,
									(ptPos1.y<ptPoint.y) ? 0 -m_nStageTrackStepY : m_nStageTrackStepY );
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 )
	{
		double dScaleX, dScaleY;
		// 调整X步距及方向
		if( (ptPos0.x == ptPoint.x) || (ptPos1.x == ptPoint.x) )
			dScaleX = 1.0;
		else
			dScaleX = 1.0 * abs(ptPos1.x - ptPoint.x) / abs(ptPos0.x - ptPoint.x);
		m_nStageTrackStepX = (int)(abs(m_nStageTrackStepX) *dScaleX);

		// 调整Y步距及方向
		if( (ptPos0.y == ptPoint.y) || (ptPos1.y == ptPoint.y) )
			dScaleY = 1.0;
		else
			dScaleY = 1.0 * abs(ptPos1.y - ptPoint.y) / abs(ptPos0.y - ptPoint.y);
		m_nStageTrackStepY = (int)(abs(m_nStageTrackStepY) *dScaleY);

		// 保存位置
		m_ptPosSave = ptPos1;

		// 设置跟踪参数
			CStageXYManager::Instance().Stage_XY_Command2( "JG", bSameX ? 0 : m_nStageTrackStepX, bSameY ? 0 : m_nStageTrackStepY,
									(ptPos1.x<ptPoint.x) ? FALSE : TRUE, (ptPos1.y<ptPoint.y) ? FALSE : TRUE, FALSE );
	}
}

void CScanView::ManuallyAdjustStage( UINT nChar )
{
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm == NULL
		|| pfrm->m_dlgStage.m_hWnd == NULL )
		return;

	int nLength, nScale;	// 像素距离, 实际步数
//	char szBuffer[64];
	CString str;

	switch( nChar )
	{
	case	VK_LEFT:
		nLength = CMCICardCommonSetting::Instance().m_sizeReso.cx /4;
		nScale = 0;
		if( CStageXYManager::Instance().m_Stage.xy.bRev)
			CalculateStageUScale( FALSE, FALSE, nLength, nScale );
		else
			CalculateStageUScale( TRUE, FALSE, nLength, nScale );
// 		CStageXYManager::Instance().Stage_XY_Command( "PR", nScale, TRUE, FALSE );
		pfrm->m_dlgStage.StageMoveX( -1*nScale, TRUE, FALSE );
		break;
	case	VK_RIGHT:
		nLength = CMCICardCommonSetting::Instance().m_sizeReso.cx /4;
		nScale = 0;
		if( CStageXYManager::Instance().m_Stage.xy.bRev )
			CalculateStageUScale( FALSE, TRUE, nLength, nScale );
		else
			CalculateStageUScale( TRUE, TRUE, nLength, nScale );
// 		CStageXYManager::Instance().Stage_XY_Command( "PR", nScale, TRUE, TRUE );
		pfrm->m_dlgStage.StageMoveX( nScale, TRUE, TRUE );
		break;
	case	VK_UP:
		nLength = CMCICardCommonSetting::Instance().m_sizeReso.cy /4;
		nScale = 0;
		if( CStageXYManager::Instance().m_Stage.xy.bRev)
			CalculateStageUScale( TRUE, FALSE, nLength, nScale );
		else
			CalculateStageUScale( FALSE, FALSE, nLength, nScale );
//		CStageXYManager::Instance().Stage_XY_Command( "PR", nScale, FALSE, FALSE );
		pfrm->m_dlgStage.StageMoveY( nScale, TRUE, FALSE );
		break;
	case	VK_DOWN:
		nLength = CMCICardCommonSetting::Instance().m_sizeReso.cy /4;
		nScale = 0;
		if( CStageXYManager::Instance().m_Stage.xy.bRev )
			CalculateStageUScale( TRUE, TRUE, nLength, nScale );
		else
			CalculateStageUScale( FALSE, TRUE, nLength, nScale );
// 		CStageXYManager::Instance().Stage_XY_Command( "PR", nScale, FALSE, TRUE );
		pfrm->m_dlgStage.StageMoveY( -1*nScale, TRUE, TRUE );
		break;
	}
}

void CScanView::AutoStageXY_FI_Init()
{
	CDlgProgress		dlg;
	dlg.m_csTitle		= GetResString( IDS_Stage_SD_TITLE_AUTO_FindIndex );
	dlg.m_iRangeTo		= 60;
	dlg.m_csMessage		= GetResString( IDS_Stage_STRING_AUTO_FindIndex );

	m_bStageFinishX		= FALSE;
	m_bStageFinishY		= FALSE;

	char szBuffer[64];
	CString str;
	if( CStageXYManager::Instance().m_Stage.xy.nMtd < 1 )
	{
		m_lastStage = GetTickCount();
		// 调用上电复位程序
		str.Format( "XQ #AUTO" );
	}
	else
	{
		// 找中心原点
		// 第一步是撞到行程开关，先走Y方向
		str.Format( "SHY;JG %s;BGY", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
	}
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	theApp.DisplayMsg( str );
	SetTimer( nAutoStageXY_FI11, nAutoStageXY_FIElapse, NULL );

	m_pAutoProgressDlg	= &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg	= NULL;

	AutoStageXY_FI_Finish( FALSE );
}

void CScanView::AutoStageXY_FI_XQ()
{
	// 得到当前位置
	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );
	if(  (abs(lPosY - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y) < 10)
		&& (abs(lPosX - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x) < 10) )
	{
		DWORD thisTickCount = GetTickCount();
		if( thisTickCount -m_lastStage > 5000 )	// 3s没有动
			AutoStageXY_FI_Finish( TRUE );
	}
	else
	{
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;
		m_lastStage = GetTickCount();
	}
}

void CScanView::AutoStageXY_FI_Process11()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)CStageXYManager::Instance().m_Stage.xy.sCmdLY), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosY - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI11 );	// 停止第一步Y方向撞行程开关
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 21" );
		Sleep( 1000 );

		// 第二步，Y反方向向中间走，走至离中心约2mm
		str.Format( "JG %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI21 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

		str.Format( "SHY;PR %s;BGY", CStageXYManager::Instance().m_Stage.xy.sCmdFI22 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI21, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process12()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)CStageXYManager::Instance().m_Stage.xy.sCmdLX), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosX - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;

	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI12 );	// 停止第一步撞行程开关
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 22" );
		Sleep( 1000 );

		// 第二步，反方向向中间走，走至离中心约2mm
		str.Format( "JG %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI21 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

		str.Format( "SHX;PR %s;BGX", CStageXYManager::Instance().m_Stage.xy.sCmdFI22 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI22, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process21()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI21 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishY && abs(lPosY - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishY )
	{
		// Y方向已到达离中心约2mm处
		KillTimer( nAutoStageXY_FI21 );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 31" );
		Sleep( 1000 );

		// 第三步，使用较低的速度寻找零点Index
		str.Format( "SHY;JG %s;FIY;BGY", CStageXYManager::Instance().m_Stage.xy.sCmdFI3 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI31, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process22()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI21 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );

	if( !m_bStageFinishX && abs(lPosX - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x = lPosX;

	if( m_bStageFinishX )
	{
		// X方向已到达离中心约2mm处
		KillTimer( nAutoStageXY_FI22 );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 32" );
		Sleep( 1000 );

		// 第三步，使用较低的速度寻找零点Index
		str.Format( "SHX;JG %s;FIX;BGX", CStageXYManager::Instance().m_Stage.xy.sCmdFI3 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI32, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process31()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI3 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );

	if( abs(lPosY) < 10 )
	{
		KillTimer( nAutoStageXY_FI31 );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 12" );
		Sleep( 1000 );

		// X方向撞行程开关
		str.Format( "SHX;JG %s;BGX", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI12, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process32()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI3 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );

	if( abs(lPosX) < 10 )
	{
		KillTimer( nAutoStageXY_FI32 );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the end of process 32" );
		Sleep( 1000 );
		Sleep( 1000 );

		// 第四步，校准
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
		{
			BOOL bVisible = FALSE;
			if( pfrm->m_dlgStage.m_hWnd != NULL )
				bVisible = ((pfrm->m_dlgStage.GetDlgItem(IDC_Stage_Calibrate)->GetStyle() & WS_VISIBLE) != 0);
			if( bVisible )
			{
				// 调试时，只找到Index就停止
				AutoStageXY_FI_Finish( TRUE );
				return;
			}
		}
		// 运行软件第一次找Index时，再走绝对距离为校准值
		// 获取工作路径
		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat_s(path, "\\KYKYcfg.ini");

		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x = 0;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y = 0;
		char	szBuf1[50], szBuf2[50], szBufRootSection[20];
		sprintf_s(szBufRootSection, "StagePos");
		if(GetPrivateProfileString(szBufRootSection, "Cali", "0", szBuf1, 255, path) != 0)
		{
			strcpy_s(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy_s(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x = atol(szBuf2);
				CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y = atol(szBuf1);
			}
		}
		if( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x != 0 || CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y != 0 )
		{
			str.Format( "SHXY;PA %ld,%ld;BGXY", CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
			CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			theApp.DisplayMsg( str );
			SetTimer( nAutoStageXY_FI4, nAutoStageXY_FIElapse, NULL );
		}
		else
			AutoStageXY_FI_Finish( TRUE );
	}
}

void CScanView::AutoStageXY_FI2_Process11()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)CStageXYManager::Instance().m_Stage.xy.sCmdLY), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosY - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI11 );	// 停止第一步Y方向撞行程开关
		Sleep( 100 );

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 21" );
		Sleep( 500 );
		CStageXYManager::Instance().m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 21: to edge" );
		Sleep( 500 );

		// 定义原点
		CStageXYManager::Instance().m_DMCWin.Command( "DP 0,0" );
		theApp.DisplayMsg( "DP 0,0" );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= 0;

		// 第二步，Y反方向撞行程开关
		str.Format( "SHY;JG %s;BGY", CStageXYManager::Instance().m_Stage.xy.sCmdFI21 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI21, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process12()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)CStageXYManager::Instance().m_Stage.xy.sCmdLX), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosX - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;

	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI12 );	// 停止第一步X方向撞行程开关
		Sleep( 100 );

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 22" );
		Sleep( 500 );
		CStageXYManager::Instance().m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 22: to edge" );
		Sleep( 500 );
		// 定义原点
		CStageXYManager::Instance().m_DMCWin.Command( "DP 0,0" );
		theApp.DisplayMsg( "DP 0,0" );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= 0;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		// 第二步，X反方向撞行程开关
		str.Format( "SHX;JG %s;BGX", CStageXYManager::Instance().m_Stage.xy.sCmdFI21 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI22, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process21()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI21 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)CStageXYManager::Instance().m_Stage.xy.sCmdLY2), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosY - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;
	
	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI21 );	// 停止第二步Y反方向撞行程开关
		Sleep( 100 );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 31" );
		Sleep( 500 );
		CStageXYManager::Instance().m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 31: return to center" );
		Sleep( 500 );

		// 第三步，向中心走到Y整个行程的一半
		str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHY;PAY=%ld;BGY", lPosY /2 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI31, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process22()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI21 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)CStageXYManager::Instance().m_Stage.xy.sCmdLX2), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosX - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
	
	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI22 );	// 停止第二步X反方向撞行程开关
		Sleep( 100 );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 32" );
		Sleep( 500 );
		CStageXYManager::Instance().m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 32: return to center" );
		Sleep( 500 );

		// 第三步，向中心走到X整个行程的一半
		str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHX;PAX=%ld;BGX", lPosX /2 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI32, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process31()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishY && abs(lPosY - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishY )
	{
		KillTimer( nAutoStageXY_FI31 );	// 已到达中心处
		Sleep( 500 );
		// 保存现在的位置
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x	= lPosX;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y	= lPosY;

		CStageXYManager::Instance().m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 12" );
		Sleep( 500 );
		CStageXYManager::Instance().m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 12: to edge" );
		Sleep( 500 );

		// X方向撞行程开关
		CString str;
		str.Format( "SHX;JG %s;BGX", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI12, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process32()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", CStageXYManager::Instance().m_Stage.xy.sCmdFI1 );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );

	if( !m_bStageFinishX && abs(lPosX - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x = lPosX;

	if( m_bStageFinishX )
	{
		KillTimer( nAutoStageXY_FI32 );	// 已到达中心处
		Sleep( 1000 );
		// 定义原点
		CStageXYManager::Instance().m_DMCWin.Command( "DP 0,0" );
		theApp.DisplayMsg( "DP 0,0" );

		// 第四步，校准
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
		{
			BOOL bVisible = FALSE;
			if( pfrm->m_dlgStage.m_hWnd != NULL )
				bVisible = ((pfrm->m_dlgStage.GetDlgItem(IDC_Stage_Calibrate)->GetStyle() & WS_VISIBLE) != 0);
			if( bVisible )
			{
				// 调试时，只找到Index就停止
				AutoStageXY_FI_Finish( TRUE );
				return;
			}
		}
		// 运行软件第一次找Index时，再走绝对距离为校准值
		// 获取工作路径
		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat_s(path, "\\KYKYcfg.ini");

		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x = 0;
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y = 0;
		char	szBuf1[50], szBuf2[50], szBufRootSection[20];
		sprintf_s(szBufRootSection, "StagePos");
		if(GetPrivateProfileString(szBufRootSection, "Cali", "0", szBuf1, 255, path) != 0)
		{
			strcpy_s(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy_s(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x = atol(szBuf2);
				CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y = atol(szBuf1);
			}
		}
		if( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x != 0 || CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y != 0 )
		{
			str.Format( "SHXY;PA %ld,%ld;BGXY", CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
			CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			theApp.DisplayMsg( str );
			SetTimer( nAutoStageXY_FI4, nAutoStageXY_FIElapse, NULL );
		}
		else
			AutoStageXY_FI_Finish( TRUE );
	}
}

void CScanView::AutoStageXY_FI_Process4()
{
	long nPosX, nPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( nPosX, nPosY );

	long nDiffX	= CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x - nPosX;
	long nDiffY	= CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y - nPosY;
	if( -10 < nDiffX && nDiffX < 10 && -10 < nDiffY && nDiffY < 10 )
	{
		// 定义原点
		CStageXYManager::Instance().m_DMCWin.Command( "DP 0,0" );
		theApp.DisplayMsg( "DP 0,0" );
		// 软件限位
		char szBuffer[64];
		CString str;
		str.Format( "%s", CStageXYManager::Instance().m_Stage.xy.sCmdLimitX );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		str.Format( "%s", CStageXYManager::Instance().m_Stage.xy.sCmdLimitY );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		AutoStageXY_FI_Finish( TRUE );
	}
}

void CScanView::AutoStageXY_FI_Finish( BOOL bAutoFinish )
{
	KillTimer( nAutoStageXY_FI11 );
	KillTimer( nAutoStageXY_FI12 );
	KillTimer( nAutoStageXY_FI21 );
	KillTimer( nAutoStageXY_FI22 );
	KillTimer( nAutoStageXY_FI31 );
	KillTimer( nAutoStageXY_FI32 );
	KillTimer( nAutoStageXY_FI4 );

	if( bAutoFinish )
	{
		// 自动找到原点，完成退出
		if ( m_pAutoProgressDlg != NULL )
			m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
	}

	CStageXYManager::Instance().m_DMCWin.Command( "ST" );
	theApp.DisplayMsg( "ST in finish" );
	Sleep( 100 );
	CStageXYManager::Instance().m_DMCWin.Command( "MO" );
	theApp.DisplayMsg( "MO in finish" );
	CStageXYManager::Instance().m_DMCWin.Command( "SHXY" );
	theApp.DisplayMsg( "SHXY in finish" );
	Sleep( 100 );
	CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
}

void CScanView::AutoStageXY_FI_TrackSecurity()
{
	// 获取自动台X/Y当前位置脉冲数
	CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
	// 得到目前的实际位置mm
	BOOL bExceed = FALSE;
	int nActualX, nActualY;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		nActualX = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y;
		// 绝对坐标
		if( nActualX > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY				// 1450000
			|| nActualX < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )			// -1450000
			bExceed = TRUE;
	}
	else
	{
		nActualX = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x;
		// 绝对坐标
		if( nActualX > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX				// 1450000
			|| nActualX < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )			// -1450000 )
			bExceed = TRUE;
	}
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		nActualY = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x;
		// 绝对坐标
		if( nActualY > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX				// 1450000
			|| nActualY < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )			// -1450000 )
			bExceed = TRUE;
	}
	else
	{
		nActualY = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y;
		// 绝对坐标
		if( nActualY > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY				// 1450000
			|| nActualY < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )			// -1450000
			bExceed = TRUE;
	}
	if( bExceed )
	{
		KillTimer( nAutoStageTrack );
		CStageXYManager::Instance().m_DMCWin.Command( "ST" );
		theApp.DisplayMsg( "ST in AutoStageXY_FI_TrackSecurity" );
		Sleep( 100 );
		CStageXYManager::Instance().m_DMCWin.Command( "MO" );
		theApp.DisplayMsg( "MO in AutoStageXY_FI_TrackSecurity" );
		CStageXYManager::Instance().m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY in AutoStageXY_FI_TrackSecurity" );
		Sleep( 100 );
		CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
		AfxMessageBox( "超出行程范围！" );
		if ( m_bCatch )
		{
			m_bCatch = FALSE;
			ReleaseCapture();
			m_bStageTrack = FALSE;

			// 擦除标识，退出跟踪模式
			CPoint ptPoint	= m_ptStageTrackParam;
			if( ptPoint.x < nSTAGE_TRACE_POINT_RADIUS )
				ptPoint.x = nSTAGE_TRACE_POINT_RADIUS;
			if( ptPoint.x > CMCICardCommonSetting::Instance().m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS )
				ptPoint.x = CMCICardCommonSetting::Instance().m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS;
			if( ptPoint.y < nSTAGE_TRACE_POINT_RADIUS )
				ptPoint.y = nSTAGE_TRACE_POINT_RADIUS;
			if( ptPoint.y > CMCICardCommonSetting::Instance().m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS )
				ptPoint.y = CMCICardCommonSetting::Instance().m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS;
			CRect rect(ptPoint.x-nSTAGE_TRACE_POINT_RADIUS,ptPoint.y-nSTAGE_TRACE_POINT_RADIUS,
				ptPoint.x+nSTAGE_TRACE_POINT_RADIUS,ptPoint.y+nSTAGE_TRACE_POINT_RADIUS);
			CSemVirtualMCICard::Instance().GraphDrawCircle( ptPoint, m_ptPosSave );
			CSemVirtualMCICard::Instance().GraphDrawCircle( ptPoint, m_ptPosSave, FALSE );
			CSemVirtualMCICard::Instance().GraphClear( rect, RGB(0,0,0) );
		}
	}
}

void CScanView::AutoStage_Clear( BOOL bZRT )
{
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
	{
		BOOL bReady = FALSE;
		if( StageTMCM_IsReady() || StageMC6600_IsReady() || StageMC600_IsReady() )
			bReady = TRUE;
		if( !bReady )
			return;

		CDlgProgress		dlg;
		dlg.m_csTitle		= "Axis move";
		dlg.m_iRangeTo		= 100;
		dlg.m_csMessage		= "Axis X/YZ/R is moving now, please wait...";

		if( StageMC6600_IsReady() )
			StageMC6600_OnVentExit();
		else if( StageTMCM_IsReady() )
			StageTMCM_OnVentExit();
		else if( StageMC600_IsReady() )
			StageMC600_OnVentExit();
		SetTimer(31, 250, NULL);

		m_pAutoProgressDlg	= &dlg;
		dlg.DoModal();
		m_pAutoProgressDlg	= NULL;

		AutoStage_Clear_Finish( FALSE, FALSE );
	}
	else if( !bZRT && CStageXYManager::Instance().m_Stage.nHasAutoStage == 1 && CStageXYManager::Instance().m_Stage.xy.bReady )
	{
		CDlgProgress		dlg;
		dlg.m_csTitle		= "Axis X/Y clearing";
		dlg.m_iRangeTo		= 20;
		dlg.m_csMessage		= "Axis X/Y is clearing now, please wait...";

		m_bStageFinishX		= FALSE;
		m_bStageFinishY		= FALSE;

		// 第一步先XY向归零
		char szBuffer[64];
		CString str;
		str.Format( "SP 2000,2000" );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHXY;PA 0,0;BGXY" );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageClearXY, nAutoStageXY_FIElapse, NULL );

		m_pAutoProgressDlg	= &dlg;
		dlg.DoModal();
		m_pAutoProgressDlg	= NULL;

		AutoStage_Clear_Finish( FALSE, FALSE );
	}
}

void CScanView::AutoStage_Clear_XY()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP 5000,5000" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	CStageXYManager::Instance().Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishX && abs(lPosX - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x = lPosX;

	if(  !m_bStageFinishY && abs(lPosY - CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishX && m_bStageFinishY )
	{
		// XY方向已归零
		KillTimer( nAutoStageClearXY );
		Sleep( 100 );
		AutoStage_Clear_Finish( FALSE, TRUE );
	}
}

void CScanView::AutoStage_Clear_Finish( BOOL bStep, BOOL bAutoFinish )
{
	if( !bStep )
		KillTimer( nAutoStageClearXY );
	if( bAutoFinish )
	{
		// XYT向完成清零
		if( m_pAutoProgressDlg != NULL )
			m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
	}
}

void CScanView::FuncAutoWaitHV( BOOL bAutoFinish )
{
	if( bAutoFinish )
	{
		if( m_pAutoProgressDlg != NULL )
			m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
	}
	else
	{
		CDlgProgress	dlg;
		dlg.m_csTitle	= GetResString( IDS_STRING_AUTO_ACC );
		dlg.m_iRangeTo	= 90;
		dlg.m_csMessage = GetResString( IDS_STRING_AUTO_WAITHV );
		m_pAutoProgressDlg = &dlg;
		dlg.DoModal();
		m_pAutoProgressDlg = NULL;
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.VacuumVACUUMSWITCH_Cancel();
	}
}

void CScanView::_GetScanTypeChangePromptMsg(CString &str) 
{
	switch( CScanParameterManager::Instance().m_nusbScanType )
	{
	case	USB_ST_Stop:
		str = "Press select scan type to Freeze";
		break;
	case	USB_ST_RasterScan:
		str.Format( "Press select scan type to Normal Scan, %dx%d, speed = %d us/s", CMCICardCommonSetting::Instance().m_sizeReso.cx, CMCICardCommonSetting::Instance().m_sizeReso.cy, CScanParameterManager::Instance().m_nusbPixRate /2 );
		break;
	case	USB_ST_AreaScan:
		str.Format( "Press select scan type to Area Scan, (%d,%d,%d,%d), speed = %d us/s",
			CScanParameterManager::Instance().m_rcAreaScanParam.left, CScanParameterManager::Instance().m_rcAreaScanParam.top, CScanParameterManager::Instance().m_rcAreaScanParam.right, CScanParameterManager::Instance().m_rcAreaScanParam.bottom, CScanParameterManager::Instance().m_nusbPixRate /2 );
		break;
	case	USB_ST_LineScan_HORZ:
		str.Format( "Press select scan type to LineScan HORZ, speed = %d us/s", CScanParameterManager::Instance().m_nusbPixRate /2 );
		break;
	case	USB_ST_LineScan_VERT:
		str.Format( "Press select scan type to LineScan VERT, speed = %d us/s", CScanParameterManager::Instance().m_nusbPixRate /2 );
		break;
	case	USB_ST_SpotScan:
		str.Format( "Press select scan type to Spot, speed = %d us/s", CScanParameterManager::Instance().m_nusbPixRate /2 );
		break;
	case	USB_ST_AnalogPhoto:
		str.Format( "Press select scan type to Sync, %dx%d, speed = %d us/s", CMCICardCommonSetting::Instance().m_sizeReso.cx, CMCICardCommonSetting::Instance().m_sizeReso.cy, CScanParameterManager::Instance().m_nusbPhotoPixRate /2 );
		break;
	}
}


void CScanView::_DisplayMsgOnViewCreate(const int nReady) 
{
	CString str;
	BOOL bErr = FALSE;
	if( (nReady & 0x01) == 0x01 )
	{
		str.Format( "%s %s", GetResString(IDS_SD_PROMPT_VAC_31), GetResString(IDS_SD_PROMPT_VAC_37) );
		theApp.DisplayMsg( str );
		bErr = TRUE;
	}
	if( (nReady & 0x02) == 0x02 )
	{
		str.Format( "%s %s", GetResString(IDS_SD_PROMPT_VAC_32), GetResString(IDS_SD_PROMPT_VAC_37) );
		theApp.DisplayMsg( str );
		bErr = TRUE;
	}
	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
	{
		if( (nReady & 0x04) == 0x04 )
		{
			str.Format( "%s %s", GetResString(IDS_SD_PROMPT_VAC_33), GetResString(IDS_SD_PROMPT_VAC_37) );
			theApp.DisplayMsg( str );
			bErr = TRUE;
		}
	}
	if( (nReady & 0x08) == 0x08 )
	{
		str.Format( "%s %s", GetResString(IDS_SD_PROMPT_VAC_34), GetResString(IDS_SD_PROMPT_VAC_37) );
		theApp.DisplayMsg( str );
		bErr = TRUE;
	}

	CBoardComm::Instance().m_bBoardsReady = !bErr;
	if( (nReady & 0x10) == 0x10 )
		theApp.DisplayMsg( "DI16A config error!" );
	if( (nReady & 0x20) == 0x20 )
		theApp.DisplayMsg( "DI16A power not on!" );
	if( (nReady & 0x40) == 0x40 )
		theApp.DisplayMsg( "DI16A set clocks error!" );
	if( (nReady & 0x80) == 0x80 )
		theApp.DisplayMsg( "DI16A could not open stream!" );
}


void CScanView::_InitLogoDisp(char * path) 
{
	// 其它系统参数
	char szBuf[20] = {0};
	CString strLo1, strLo2, strLo3, strLo4;
	if(GetPrivateProfileString( "Options2", "Lo1", "EM", szBuf, 10, path) != 0 )
		strLo1 = szBuf;
	if(GetPrivateProfileString( "Options2", "Lo2", "62", szBuf, 10, path) != 0 )
		strLo2 = szBuf;
	if(GetPrivateProfileString( "Options2", "Lo3", "00", szBuf, 10, path) != 0 )
		strLo3 = szBuf;
	if(GetPrivateProfileString( "Options2", "Lo4", "", szBuf, 10, path) != 0 )
		strLo4 = szBuf;
	theApp.m_strLogoDisp.Format( "KYKY-%s%s%s%s", strLo1, strLo2, strLo3, strLo4 );
}


LRESULT CScanView::OnStartAutoFocus(WPARAM wParam, LPARAM lParam)
{
	FuncAutoFocus();
	return 0L;
}

LRESULT CScanView::OnStopAutoFocus(WPARAM wParam, LPARAM lParam)
{
	// 恢复物镜、放大器
	AutoFocus_Setup( CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_dblMagnifierSaved );

	CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

	// 恢复扫描
	CScanParameterManager::Instance().m_nusbPixRate = CScanParameterManager::Instance().m_nusbPixRateSaved;
	USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanTypeSaved );

	return 0L;
}
