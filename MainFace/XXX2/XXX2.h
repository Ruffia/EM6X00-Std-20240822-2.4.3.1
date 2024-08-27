// XXX2.h : main header file for the XXX2 application
//
// Last Modify : 2011.07.19

/////////////////////////////////////////////////////////////////////////////
// 09.12.10 增加调试状态功能
// 10.07.02 增加优先存储格式
// 11.07.19 加入隐含项，全选时可以记录下位机发送的当前未直接记录的消息
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_XXX2_H__EB90150D_9CBE_43BD_8993_17EA400B457D__INCLUDED_)
#define AFX_XXX2_H__EB90150D_9CBE_43BD_8993_17EA400B457D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"       // main symbols
#include "SemCtrl.h"
#include "SemCtrlPanelManager.h"
#include "SemVirtualMCICard.h"
//#include "CriticalResource.h"
#include <afxmt.h>
//#include <WINSOCK2.H>

#include "LanguageManagerCtrl.h"

#include "ThumbnailListCtrlEx.h"

#include "MCBCtrl.h"

#include <USB_Functions.h>

///////////////////////////////////////
#include <Comm3603.h>
#include <CommSpellman.h>
#include <CommWSM30KV.h>
#include <CommWSM30KV2.h>
#include <PMSKV.h>
///////////////////////////////////////

///////////////////////////////////////
#include <SMvac.h>
#include <VACobtain.h>
#include <CommVAC.h>

#include <PLCvacBrd.h>
#include <PLCvacBoard.h>
///////////////////////////////////////

///////////////////////////////////////
// 15.10 XY自动台
#include "dmcwin.h"
// 20.01 XYZR自动台
#include <Comm_MC600.h>
#include <ExportStageMC600Func.h>
// 22.12 新5轴台
#include <ExportStageMC6600Func.h>
#include <ExportStageTMCMFunc.h>
#include "SemCtrlInitializer.h"
///////////////////////////////////////
#include <ExportCameraFunc.h>
// 23.06 导航
#include <Comm_NAV.h>
#include <ExportNAVFunc.h>

const	CString	g_csSemControlDocMark						= _T("KYKY6X00-SemControlRecord");

const	CString	g_csRegistryKey								= _T("KYKY");
const	CString	g_csSettingSection							= _T("Settings");
const	CString	g_csRegDateFormat							= _T("%Y%m%d");
const	CString	g_csRegEntry_csCurrentDate					= _T("current date");
const	CString	g_csRegEntry_nSerialNumber					= _T("serial number");
const	CString	g_csRegEntry_nImageSerialNumber				= _T("image serial number");
const	CString	g_csRegEntry_nDPhotoForeground				= _T("DPhoto Fore ground");
const	CString	g_csRegEntry_nDPhotoBackground				= _T("DPhoto Back ground");
const	CString	g_csRegEntry_nPhotoSerialNumber				= _T("Photo serial number");
const	CString	g_csRegEntry_nLanguage						= _T("Language");
const	CString	g_csRegEntry_csOptimalBrightness			= _T("Optimal Brightness");	// 10.01.22
const	CString	g_csRegEntry_csOptimalContrast				= _T("Optimal Contrast");	// 10.01.22
const	CString g_csRegEntry_nPriorSaveType					= _T("Prior Save Type");	// 10.07.02
const	CString g_csRegEntry_nLogAll						= _T("Record All Logs");	// 11.07.19

//======2004-6-2_自动保存并更新控制量=========== Begin ==========
const	CString	g_csRegEntry_nBeemAlignmentX				= _T("BeemAlignmentX");
const	CString	g_csRegEntry_nBeemAlignmentY				= _T("BeemAlignmentY");
const	CString	g_csRegEntry_nStigmatorX					= _T("StigmatorX");
const	CString	g_csRegEntry_nStigmatorY					= _T("StigmatorY");
const	CString	g_csRegEntry_nCondenserLens					= _T("CondenserLens");
const	CString	g_csRegEntry_nObjectiveLens					= _T("ObjectiveLens");
const	CString	g_csRegEntry_nMagnifier						= _T("Magnifier");
const	CString	g_csRegEntry_nBrightness					= _T("Brightness");
//======2004-6-2_自动保存并更新控制量============ End ===========

const	CString g_csRegEntry_nFilaLifeTime					= _T("FilaLifeTime");

const	UINT nSemMessageTimerID			= 100;
const	UINT nSemMessageTimerElapse		= 100;

const	UINT nSemAutosaveTimerID		= 101;
const	UINT nSemAutosaveTimerElapse	= 1000;

///////////////////////////////////////////////////////////
// MCI
const	UINT nMCI_ReadStatusTimerID		= 110;
const	UINT nMCI_ReadStatusTimerElapse	= 2000;
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// X-Y自动台 15.10
const	CString g_csRegEntry_nAutoStage						= _T("hasAutoStage");

const UINT nAutoStageXY_FI11	= 131;
const UINT nAutoStageXY_FI12	= 132;
const UINT nAutoStageXY_FI21	= 133;
const UINT nAutoStageXY_FI22	= 134;
const UINT nAutoStageXY_FI31	= 135;
const UINT nAutoStageXY_FI32	= 136;
const UINT nAutoStageXY_FI4		= 137;
const UINT nAutoStageTrack		= 138;
const UINT nAutoStageClearXY	= 150;
const UINT nAutoStageXY_FIElapse= 300;
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// CCD 17.02
const	CString g_csRegEntry_nCCD							= _T("hasCCD");
// BSD 20.11
const	CString g_csRegEntry_nBSD							= _T("hasBSD");
// EDS 17.10
const	CString g_csRegEntry_nEDS							= _T("hasEDS");
// NAV 23.06
const	CString g_csRegEntry_nNAV							= _T("hasNAV");
// pA计 23.09
const	CString g_csRegEntry_nPA							= _T("PAType");

// VAC Board Type 20.11
const	CString g_csRegEntry_nBoardType						= _T("VACBoardType");

///////////////////////////////////////////////////////////
#define	ID_TIMER_VACUUM_waitTC2toReady	11
#define	ID_TIMER_VACUUM_waitDPtoReady	12
#define	ID_TIMER_VACUUM_waitTC3toReady	13
#define	ID_TIMER_VACUUM_waitTC4toReady	14
#define	ID_TIMER_VACUUM_waitSYStoReady	15
#define	ID_TIMER_VACUUM_waitTOopenV6	16
#define	ID_TIMER_VACUUM_waitTOcloseV2	17
///////////////////////////////////////////////////////////

const	CString	g_csMenuFileName							= _T("6000.mu");
const	int		g_intSTAND1CTRLHeight						= 50;

typedef struct tagPageAssamblePanel{
	CDialog*	pPageDlg;
	UINT		nIDD;
	UINT		nStringID;
	UINT		nMsgID;
} PageAssamblePanel;

//调节器类型
#define	SORT_BEAM_ALIGNMENT		0	//电对中
#define	SORT_SCAN_SHIFT			1	//电位移
#define	SORT_BEAM_ROTATE		2	//电旋转
#define	SORT_CONDENSER_LENS		3	//聚光镜
#define	SORT_OBJECTIVE_LENS		4	//物镜
#define	SORT_BRIGHT				5	//亮度
#define	SORT_CONTRAST			6	//对比度
#define	SORT_STIGMATOR			7	//消像散

#define UPDATEVIEW_CHANGEADJUSTOR	1

#include "ximage.h"

#define SEMIMAGE_SUPPORT_SEM	1
const int SEMIMAGE_FORMAT_SEM	= CMAX_IMAGE_FORMATS + 1;
const int SEM_IMAGE_FORMATS		= CMAX_IMAGE_FORMATS + 2;

class CSemCtrlDlgRenewSelect;

///////////////////////////////////////////////////////
// Debug OSC
typedef struct tagDebug_OSC
{
	int		nOscillIndex;		// 13.04.03 Debug调试时示波器Y向档位
	double	dOffset;			// 13.08.05 Debug调试时示波器Y向偏移量
}DebugOSC;
///////////////////////////////////////////////////////

///////////////////////////////////////
//typedef struct graphdata
//{
//	double dValue[4];
//}Element;


/////////////////////////////////////////////////////////////////////////////
// CXXX2App:
// See XXX2.cpp for the implementation of this class
//

class CXXX2App : public CWinApp
{
// Private operations
private:
	// Returns TRUE if running the new Win95 shell (Win95 or WinNT 4.0 or greater)
	BOOL	HasNewShell();

	// Returns TRUE if SEM program is already running
	BOOL	AlreadyRunning();

	void	SetupSerialNumber();

//	CriticalResource	m_criticalMsgDisplay;
	static	CCriticalSection	m_criticalMsgDisplay;

	///////////////////////////////////////////////////////////
	BOOL	m_blInquireZ80A;
	BOOL	m_blInquireZ80B;
	BOOL	m_blPauseAll;

	BOOL	InquirerZ80B();
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	BOOL	PumpSemError();
	BOOL	ProcessSemMessage( MicrocomputerID Z80ID, SEM_ControlMessage& mcMSG );

	BOOL	ProcessUSBSemMessage( MicrocomputerID Z80ID, USB_SEM_ControlMessage& mcMSG );

	void   _InitDlgFlag( CSemCtrlDlgRenewSelect &dlg );
	void   _SaveDlgFlag( CSemCtrlDlgRenewSelect &dlg ); 
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	BOOL	m_blReady_Vacuum;
	BOOL	m_blReady_HV;
	BOOL	m_blReady_Filament;

	BOOL	m_blUnlock_Filament;

	void	SemStateMechanics();
	void   _InitDocTemplateList();
	void   _ReadFromRegistry();

public:
	BOOL	m_blUnlock_HV;
	///////////////////////////////////////////////////////////
	BOOL	m_boolHV;

	double	m_dfMouseSpeed;			// 2004-5-14_高倍像操作组合控制

	///////////////////////////////////////////////////////////
	BOOL	m_boolShowUScaleXY;		// 2005-1-17_放大倍数XY联合校正

	// 2005-1-17_0.0KV问题
	BOOL	IsStopScan();

	//MJY///////////////////////////////////////////////////////
	CWnd*	GetScanFrameWnd();		// 05.01.28 获取扫描框架类指针
	CView*	GetScanView();			// 12.03.01 获取视图类指针
	void	UpdateScanStatusBar( const VARIANT FAR& ClassID, const CString str );

	BOOL	m_blSACPState;			// 09.01.15 S.A.C.P.功能默认状态为不启用
	void	SetupDebugParam( BOOL bDebug, int nBrightness, int nContrast );	// 10.01.22 增加调试状态功能参数 v 2.2.2.2
	int		m_nOptimalBrightness;	// 10.01.22 增加调试状态功能参数（最佳亮度值）
	int		m_nOptimalContrast;		// 10.01.22 增加调试状态功能参数（最佳对比度值）
	int		m_nPriorSaveType;		// 10.07.02 增加优先存储格式

	// 09.02.20 本地化
	CLanguageManagerCtrl m_LanguageManagerCtrl;// CPreferences thePrefs;
	void	Localize(void);
	
	void	MyAppendMenu(CMenu* pmenu, UINT nRootMenuID, LPCTSTR lpFileName);
	void	WriteMenuFile(CMenu* pmenu, LPCTSTR lpSection, LPCTSTR lpFileName);

	// 11.04.20 连接常规图像处理程序KYKYImage
	void	LinkImageProcess( CThumbImageItem* pitem );

	float	m_optJpegQuality;
	int		m_optJpegOptions;
	int		m_optRawOptions;

#ifdef VATI_EXTENSIONS
	// for text placement
	CxImage::CXTEXTINFO	m_text;
#endif

	RGBQUAD	m_FloodColor;
	BYTE	m_FloodTolerance;
	BYTE	m_FloodOpacity;
	BYTE	m_FloodSelect;

	typedef struct FiltersParams_t {
		long	Kernel5x5[25];
		long	Kernel3x3[9];
		long	kSize;
		long	kBias;
		long	kDivisor;

		float	RotateAngle;
		long	RotateMethod;
		long	RotateOverflow;
		BYTE	RotateKeepsize;

		BYTE	ResampleSizemode;
		float	ResampleFactor;
		long	ResampleW;
		long	ResampleH;
		BYTE	ResampleKeepRatio;
		BYTE	ResampleMethod;

		long	SkewPivotX;
		long	SkewPivotY;
		float	SkewSlopeX;
		float	SkewSlopeY;
		float	SkewX;
		float	SkewY;
		BYTE	SkewInterp;

		BYTE	DitherMethod;

		BYTE	ThreshLevel;
		BYTE	ThreshPreserveColors;

		BYTE	ColorMode;
		RGBQUAD	ColorHSL;
		long	ColorRed;
		long	ColorGreen;
		long	ColorBlue;
		BYTE	ColorSolarLevel;
		BYTE	ColorSolarLink;

		float	GammaLevel;
		float	GammaR;
		float	GammaG;
		float	GammaB;
		BYTE	GammaLink;

		BYTE	IncBppBPP;
		BYTE	DecBppBPP;
		BYTE	DecBppPalMethod;
		BYTE	DecBppErrDiff;
		BYTE	DecBppLimitColors;
		long	DecBppMaxColors;

		BYTE	CanvasMode;
		long	CanvasW;
		long	CanvasH;
		long	CanvasLeft;
		long	CanvasRight;
		long	CanvasTop;
		long	CanvasBottom;
		BYTE	CanvasCenterH;
		BYTE	CanvasCenterV;
		BYTE	CanvasKeepRatio;
		BYTE	CanvasUseImageBkg;
		COLORREF CanvasBkg;

		long	ShadowX;
		long	ShadowY;
		BYTE	ShadowR;
		COLORREF ShadowColor;
		COLORREF ShadowBkg;
		BYTE	ShadowIntensity;
		BYTE	ShadowRelative;
	} FiltersParams;

	FiltersParams m_Filters;
	//MJY///////////////////////////////////////////////////////

	DWORD	m_dwOpr;		// 面板模式
	CString	m_strLogo, m_strLogoDisp;		// 产品型号

	void	CFGfile_InitParams();
	void	CFGfile_InitHVParams(char *path);
	void	CFGfile_InitVACParams(char *path);
	void	CFGfile_InitVACParamsNew(char *path);
	void	CFGfile_InitSCNParams(char *path);
	void	CFGfile_InitScanParams(char *path);

//	VAC_Param	m_BrdVAC;
	void	VAC_SaveStates();
	double	ScientificToDouble(CString strVal);

	///////////////////////////////////////////////////////
	BOOL		m_bDiagLogging;		// 13.01.05 诊断记录标志

	///////////////////////////////////////////////////////

	HANDLE	m_hTimeDelay;
	void	Delay( int msecond );

	// Leo 2003.12.5 添加维修状态，此时高压控制不连锁。
	BOOL	m_blOverhaulState;
	BOOL	m_blUnlockAll;

public:
	// 17.02 CCD
	BOOL	m_bHasCCD;
	// 17.10 EDS
	BOOL	m_bHasEDS;
	// 23.06 NAV
	BOOL	m_bHasNAV;

	// 2021.06 原始数据存储
	UINT	m_nSaveSrc;

	///////////////////////////////////////////////////////
	// 内存泄漏检测
#ifdef _DEBUG
protected:
	CMemoryState	m_msOld, m_msNew, m_msDiff;
#endif // _DEBUG
	///////////////////////////////////////////////////////

public:
	CXXX2App();

/*	CMultiDocTemplate*	m_pSemControlDocTemplate;
	CMultiDocTemplate*	m_pSemLogDocTemplate;
*/	CMultiDocTemplate*	m_pSemImageDocTemplate;
	CMultiDocTemplate*	m_pSemScanDocTemplate;

	int		m_nImageDocCount;		// number of image documents

	///////////////////////////////////////////////////////////
	BOOL	m_blExit;
	CString	m_csCurrentDate;
	int		m_nSerialNumber;
	int		m_nPhotoSerialNumber;
	int		m_nDPhotoForeground;
	int		m_nDPhotoBackground;
	BOOL	m_blSaveGraphLay;		// 2004-8-2_001:保存SEM格式图像时，保存图形层。
	///////////////////////////////////////////////////////////

	///////////////////////////////////
	CCommWSM30KV	m_WSM30KV;
	Cfg_WSM30KV		m_cfgWSM30KV;
	Status_WSM30KV	m_statusWSM30KV;
	Mon_WSM30KV		m_monWSM30KV;
	Setting_WSM30KV	m_setWSM30KV;

	CCommWSM30KV2		m_WSM30KV2;
	Status_WSM30KV2		m_statusWSM30KV2;
	Mon_WSM30KV2		m_monWSM30KV2;
	Setting_WSM30KV2	m_setWSM30KV2;
	///////////////////////////////////
	BOOL			HVPower_IsReady();
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	BOOL	PromptForFileName(CString& fileName, UINT nIDSTitle, 
								DWORD dwFlags, BOOL bOpenFileDialog, int* pType=NULL);
	BOOL	DoPromptFileName(CString& fileName, UINT nIDSTitle,
								DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	int		GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	int		GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
	CString	GetExtFromType(int nDocType);
	CString	GetFileTypes(BOOL bOpenFileDialog);
	CString	GetFileExts(BOOL bOpenFileDialog, int& nCount);		// 10.07.02
	BOOL	GetWritableType(int nDocType);
	CString	FindExtension(const CString& name);

	///////////////////////////////////////////////////////////
	void	AutoInitSemCtrl( BOOL bPC9Reset = FALSE );	// 2004-6-2_自动保存并更新控制量
	void    SetupDFocus( double dMag );

	void	PC1ResetingProcess();	// 2005-3-7_PC1复位处理程序
	void	SetupScanShift( double dblMag );

	BOOL	PumpSemMessage();
	void	DisplayMsg(LPCTSTR lpszText);
	///////////////////////////////////////////////////////////
	
	BOOL	PumpUSBSemMessage();
	void	UpdateReadbackData( unsigned short* pnData, unsigned short& nData1, unsigned short& nData2 );

	void	SystemReady( BOOL bVacuum, BOOL bHV, BOOL bFilament );

	void	ResponseUrgency( BYTE bData );
	DWORD	m_nINT_Ticks;
	int		m_nINT_Counts;
	ULONG_PTR m_nGdiplusToken;  //ChartLib 需要用到Gdiplus

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXXX2App)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CXXX2App)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnLogSelect();
	afx_msg void OnLogMsg();
	afx_msg void OnUpdateLogMsg(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	afx_msg void OnCtrlSACP();
	afx_msg void OnUpdateCtrlSACP(CCmdUI* pCmdUI);
	afx_msg void OnOptionsSetupLanguageChs();
	afx_msg void OnOptionsSetupLanguageEn();
	afx_msg void OnCTRLOverhaul();
	afx_msg void OnUpdateCTRLOverhaul(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CXXX2App theApp;
#define	GetResString(uStringID)		theApp.m_LanguageManagerCtrl.GetResString(uStringID)

struct DocType
{
public:
	int		nID;
	BOOL	bRead;
	BOOL	bWrite;
	const	TCHAR*	description;
	const	TCHAR*	ext;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XXX2_H__EB90150D_9CBE_43BD_8993_17EA400B457D__INCLUDED_)
