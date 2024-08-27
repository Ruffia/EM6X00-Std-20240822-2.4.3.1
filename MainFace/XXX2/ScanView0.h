// Last Modify : 2009.12.10

/////////////////////////////////////////////////////////////////////////////
// 09.11.18 增加单击测试类型GraphHitTestNULL
// 09.12.10 增加调试状态功能

// 12.02.15 增加单双探测器标识
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANVIEW_H__AB607E47_147C_45D3_9F18_032220C2EF5D__INCLUDED_)
#define AFX_SCANVIEW_H__AB607E47_147C_45D3_9F18_032220C2EF5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanView.h : header file
//

#include "ImageItem.h"
#include "ScanDoc.h"

// 1 inch = 25.4 mm
const double mm_TO_inch				= (double)0.0393700787401574803149606299212598;
const double SEM_dfStandImageWidth	= (double)127;		// mm
const double SEM_dfStandImageHeight	= (double)89;		// mm

///////////////////////////////////////
// 15.08 远程功能
#include "TCPServer.h"
#include <CommFunctions.h>
///////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CScanView view

class CScanView : public CScrollView
{
	enum {
		GraphHitTest_Normal	= 0,
		GraphHitTest_In		= 1,
		GraphHitTest_E		= 2,
		GraphHitTest_W		= 3,
		GraphHitTest_N		= 4,
		GraphHitTest_S		= 5,
		GraphHitTest_NE		= 6,
		GraphHitTest_SE		= 7,
		GraphHitTest_NW		= 8,
		GraphHitTest_SW		= 9,
		GraphHitTest_NS		= 10,
		// 09.11.18 增加单击测试类型GraphHitTestNULL
		GraphHitTest_NULL	= 11,
	};

protected:
	CScanView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScanView)

// Attributes
public:
	unsigned short	m_nAdjustorSort;
	unsigned short	m_nCondenserSort;
	unsigned short	m_nObjectiveSort;

	BOOL	m_bAbc;
	BOOL	m_bLabel;
	BOOL	m_bUScale;
	BOOL	m_bDate;
	BOOL	m_bNote;
	BOOL	m_bWobbler;
	BOOL	m_bLean;
	BOOL	m_bDynFocus;
	BOOL	m_bSacp;
	BOOL	m_bRobinson;
	BOOL	m_bOscillograph;
	BOOL	m_bFrameRate;

	BOOL	m_bCondRev;
	BOOL	m_bObjectiveRev;
	BOOL	m_bAuxStig;

	BOOL	m_bPreview;
	BOOL	m_bLive;
	BOOL	m_bSplit;
	BOOL	m_bSplitZoom;
	BOOL	m_bOnlyDet2;

	CRect	m_rcScan;
	BOOL	m_bZoomFit;

///////////////////////////////////////////////////////////
	BOOL	m_boolScreenFull;
	
	long				m_nLineScanParam;
	CPoint				m_ptPointScanParam;
	CRect				m_rcAreaScanParam;
	CRect				m_rcStaticAreaScanParam;
	CRect				m_rcAutoBCAreaScanParam;
	CRect				m_rcAutoFocusAreaScanParam;
	CRect				m_rcAutoAstigmatAreaScanParam;
	CRect				m_rcDualMagParam;

	BOOL		m_bStopPhoto;	// 2004-5-12_照相编号自动增加
	BOOL		m_bStopScan;	// 2005-1-17_0.0KV问题

	///////////////////////////////////////////////////////
	// USB
	unsigned short	m_nusbScanResoIndex;
	CSize			m_sizeReso;
	unsigned short	m_nusbPixRateIndex;
	unsigned short	m_nusbPhotoPixRateIndex;
	unsigned short	m_nusbScanType;
	unsigned short	m_nusbScanTypeSaved;
	unsigned short	m_nusbScanStoreMode;
	unsigned short	m_nusbScanDetMode;		// 12.02.15 探测器模式
	PublicParam_Scan	m_scanParamLive;
	PublicParam_Scan	m_scanParamBak;

	UINT	m_nPixRateIndex[9];// = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };//40, 80, 128 };
	int		m_nResos[RESO_COUNT];
	int		m_nResoX[8];
	int		m_nResoY[8];
	double	m_dResoRate;

 	UINT	m_nCLgrade;
// 	long	m_lCLgrade[3*9];	// 9个聚光镜快速档位，每个档位包括聚光镜、亮度、对比度各一个值
	long	m_lCLfast[4];		// 4个聚光镜快速设置值

	USB_ScanFunction		m_usbFunction;
	USB_ScanParam			m_usbParam;
	unsigned short			m_nusbPixRate;
	unsigned short			m_nusbPhotoPixRate;
	unsigned short			m_nusbPixRateSaved;

	void	USB_ChangeScan( int nusbScanType = -1 );
	UINT	USB_GraphHitTest( const CPoint ptGraphPos );
	void	USB_SetScreenMouseCtrl( CPoint point );
	void	USB_SetScanRectParam();
	void	USB_GetResoSize( int nScanResolution );
	///////////////////////////////////////////////////////


//LMH//////////////////////////////////
protected:
	CPoint		m_ptPosSave;
	int			m_nGraphHitTestSave;
	BOOL		m_bCatch;
	
	CRect		m_rcDragRectLast;
	//CSize		m_szDragSizeLast;

	int			m_nMobileUScalePixelNum;
	CPoint		m_ptMobileUScale;
	CPoint		m_ptMobileUScale2;		// 07.07---07.08加入任意方向标尺
	BOOL		m_bUScaleHitTest;

	// 16.06.12 多移动标尺实验
	int			m_nMobileUScalePixelNumArray[MobileUScaleCounts];
	CPoint		m_ptMobileUScaleArray[MobileUScaleCounts];
	CPoint		m_ptMobileUScaleArray2[MobileUScaleCounts];
	BOOL		m_bUScaleHitTestArray[MobileUScaleCounts];
	void		SetUScaleParamArray(int nIndex, CPoint point);

	int			m_nMobileUScaleCounts;	// 已增加的移动标尺数目
	// 16.06.12 多移动标尺实验

	CPoint		m_ptOscillograph;
	BOOL		m_bOscillographHitTest;

	int			m_nMapMode;
	CSize		m_szTotalLog;			// total size in logical units (no rounding)
	CSize		m_szTotalDev;			// total size in device units

	BOOL		m_bCenter;             // Center output if larger than total size
	BOOL		m_bInsideUpdate;       // internal state for OnSize callback

	CBrush		m_brHatch;
//LMH//////////////////////////////////
	CBitmap		m_bmpBackground;

///////////////////////////////////////////////////////////
	//07.04.26显示图像小样
public:
	CImageItem*				m_pSelItem;
	ListImageItem*			m_plistImageItem;
	BOOL					m_bCtrlDown;		//Ctrl键按下标志08.06.27
	BOOL					m_bIsMultiSel;		//多文件选择标志08.06.27

	CImageItem* GetSelImageItem( CPoint &point );
	void		DeleteItem( CString strTitle );
	void		ChangeItemName( CString strOldName, CString strNewName );
	void		UpdateItemRect( BOOL bPrompt = TRUE );

	// 09.03.05
	CScanDoc*	GetDocument();
	void		Localize(void);
	void		LocalizeMenu(void);
	WORD		m_wLanguageID;
	BOOL		m_bOpenImageAfterSnap;

	BOOL		m_bDebugAccel;		// 09.12.10 增加调试状态功能
	UINT		nMCB_Timer;

	int			ReadFooterSettings( char* path );
	void		ReadFooter_TimeFormat( char* buf );
	void		SetFooterSettingsDefault();
	///////////////////////////////////////////////////////
	void		Diagnose( BOOL bCheck );
	void		Diag_Init( int nID, BOOL bSet );
	CString		Diag_SCN_LNS();
	CString		Diag_VAC();
	CString		Diag_SPR();
	CString		Diag_MCI();
	CString		Diag_Logging();
	CString		Diag_Logging2();
	///////////////////////////////////////////////////////
	void		SetLineScanParamManually( BOOL bHORZ, int nPos );
	void		SetUScaleParamByDefine( double dScale );

	// 15.10 XY自动台
	void		ManuallyAdjustStage( UINT nChar );
	void		SetStagePointParam(CPoint point);
	void		SetStageTrackParam(CPoint point);

	BOOL		m_bStageOrigin;
	BOOL		m_bStageOriginHitTest;
	BOOL		m_bStageTrack;
	BOOL		m_bShiftDown;
	CRect		m_rcStageDragRectLast;
	CPoint		m_ptStagePointParam, m_ptStageTrackParam;
	int			m_nStageTrackStepX, m_nStageTrackStepY;
	DWORD		m_lastStage;

	void		AutoStageXY_FI_Init();
	void		AutoStageXY_FI_Process11();
	void		AutoStageXY_FI_Process12();
	void		AutoStageXY_FI_Process21();
	void		AutoStageXY_FI_Process22();
	void		AutoStageXY_FI_Process31();
	void		AutoStageXY_FI_Process32();
	void		AutoStageXY_FI2_Process11();
	void		AutoStageXY_FI2_Process12();
	void		AutoStageXY_FI2_Process21();
	void		AutoStageXY_FI2_Process22();
	void		AutoStageXY_FI2_Process31();
	void		AutoStageXY_FI2_Process32();
	void		AutoStageXY_FI_XQ();
	void		AutoStageXY_FI_Process4();
	void		AutoStageXY_FI_Finish( BOOL bAutoFinish );
	void		AutoStageXY_FI_TrackSecurity();
	BOOL		m_bStageFinishX, m_bStageFinishY;

	void		AutoStage_Clear( BOOL bZRT );
	void		AutoStage_Clear_XY();
	void		AutoStage_Clear_Finish( BOOL bStep, BOOL bAutoFinish );
	// Auto Stage

	// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
	BOOL		m_bDebugCoadjust;

public:
// Attribute
	int			m_nScanTypeSaved;
	UINT		m_nAutoFunctionTimer;
	DWORD		m_dwFramGrayHistogramBuff[256];

	LPBYTE		m_bImageBuff;
	int			m_nLoop;

	CDialog*	m_pAutoProgressDlg;

	int			m_nAutoFuction;

	WORD		m_nMagnifierSaved;
	double		m_dblMagnifierSaved;
	long		m_lObjectiveLensSaved;
	long		m_lLastlObjectiveLens;
//ZYX add 090407
	double		m_dbLastlMagnifierSaved;

	long		m_lStigmatorXSaved;
	long		m_lStigmatorYSaved;

	long		m_lBrightnessSaved;
	long		m_lContrastSaved;

	long		m_lFilaSaved;
	BOOL		m_bAutoFilaPause;

	// 16.04.26 为诊断功能添加参数保存
	long		m_lCondensorLensSaved;
	long		m_lAlignXSaved;
	long		m_lAlignYSaved;

	// 19.06.19 扫描区域显示像素
	int			m_nDispPixel;

// Operations
	void	AFTest();

	void	AutoFocusOK( long lObjectiveLens );
	void	AutoAstigmatOK( long lObjectiveLens, long lStigmatorX, long lStigmatorY );
	void	AutoBCOK( long lBrightness, long lContrast );
	void	AutoFilamentOK( long lFila );

	void	StartAutoFocus();
	void	AutoFocusProcess();
	void	StopAutoFocus();
	void	AutoFocus_Setup(long lObjectiveLens, double dblMagnifier = 200.0 );

	void	StartAutoAstigmat();
	void	AutoAstigmatProcess();
	void	StopAutoAstigmat();

	void	StartAutoBC();
	void	AutoBCProcess();
	void	StopAutoBC();

	// 11.01.28 改善自动亮度对比度功能
	void	StartAutoBC2();
	void	AutoBCProcess2();
	void	StopAutoBC2();
	// 11.01.28 改善自动亮度对比度功能
	
	void	StartAutoFilament();
	void	AutoFilamentProcess();
	void	StopAutoFilament();

	void	FuncAutoFilament();
	void	FuncAutoBrightContrast();
	void	FuncAutoFocus();
	void	FuncAutoStigmator();
//--Auto Functions---------------------------------------//

// Operations
public:
	void	NotifyPixRate( unsigned short nNewVal );
	void	NotifyScanResolution( UINT uNewVal, BOOL bForce = FALSE );
	void	NotifyScanSplit( BOOL bSplit, BOOL bSplitZoom );
	void	NotifyScanLive( BOOL bLive );

	void	SyncAdjustorSort( UINT nNewVal );
	void	SyncAdjustorManualSort( UINT nNewVal );

	void	SyncObjectiveLensReverse( BOOL bNewVal );
	void	SyncCondenserLensReverse( BOOL bNewVal );

	void	SyncLabel( BOOL bNewVal );
	void	SyncDate( BOOL bNewVal );

	void	SyncWobbler( BOOL bNewVal );
	void	NotifyWobbler( BOOL bNewVal );
	void	SyncLean( BOOL bNewVal );
	void	NotifyLean( BOOL bNewVal );
	void	SyncDynFocus( BOOL bNewVal );
	void	NotifyDynFocus( BOOL bNewVal );
	void	SyncSacp( BOOL bNewVal );
	void	NotifySacp( BOOL bNewVal );
	void	SyncRobinsonDetector( BOOL bNewVal );
	void	NotifyRobinsonDetector( BOOL bNewVal );
	void	SyncOscillograph( BOOL bNewVal );
	void	SyncFrameRate( BOOL bNewVal );
	void	SyncAuxiliaryStigmation( BOOL bNewVal );
	///////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
	BOOL	IsStopScan();
	BOOL	IsPhoto();
	void	ResetImageCard();
	void	StopPhoto();

	void	ScreenZoom();

//	void	ChangeScan( int nScanType = -1 );
	void	UpdatePhotoSn(int nSN);

	void	StandImage( int nDelt );

	void	UpdateWindowPos( BOOL bDual = FALSE);
	void	UpdateScale();

	// 17.09 CCD在看背散射像的时候不工作
	void	CheckCCDSwitch();

//LMH//////////////////////////////////
public:
	CPoint	ScreenToGraph( const CPoint point );
	// 07.07---07.08加入任意方向标尺
	void	GetFitScreenClientRect( CRect& rcScreenClientRect );
	void	AdjustPos( CPoint& ptPos );
	void	AdjustUScaleLength( CPoint& ptStart, CPoint& ptEnd, BOOL bTranslation = FALSE );
	// 07.07---07.08加入任意方向标尺

	// 2005-1-17_放大倍数XY联合校正
	CRect	m_rcUScaleAreaLast;
	BOOL	m_boolUScaleAreaIsDraw;
	void	UpdateUScaleXY();
	BOOL	m_bDrawCircle;

protected:
	void	GraphTextEdit( CPoint point );

	void	UpdateScanParam( CPoint point );

	void	SetPointScanParam(CPoint point);
	void	SetLineScanParam(CPoint point);
	void	SetActiveAreaParam(CPoint point);
	void	SetDualMagParam(CPoint point);
	void	SetUScaleParam(CPoint point);
	void	SetOscillographParam(CPoint point);

	void	CenterOnPoint(CPoint ptCenter);
	void	ScrollToDevicePosition(POINT ptDev); // explicit scrolling no checking

	void	UpdateBars();          // adjust scrollbars etc
	BOOL	GetTrueClientSize(CSize& size, CSize& sizeSb);
		// size with no bars
	void	GetScrollBarSizes(CSize& sizeSb);
	void	GetScrollBarState(CSize sizeClient, CSize& needSb,
			CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);

public:
	static	AFX_DATA const SIZE sizeDefault;
		// used to specify default calculated page and line sizes

	// in logical units - call one of the following Set routines
	void	SetScaleToFitSize(SIZE sizeTotal);

	CPoint	GetScrollPosition() const;       // upper corner of scrolling
	CSize	GetTotalSize() const
			{ ASSERT(this != NULL); return m_szTotalLog; };	// logical size

	// for device units
	CPoint	GetDeviceScrollPosition() const;
	void	GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal,
			SIZE& sizePage, SIZE& sizeLine) const;

	void	ScrollToPosition(POINT pt);    // set upper left position
	void	FillOutsideRect(CDC* pDC, CBrush* pBrush);
	void	ResizeParentToFit(BOOL bShrinkOnly = TRUE);

//LMH//////////////////////////////////

	/////////////////////////////////////////////
	// Remote
	CWinThread*		m_commCmdThd;		// 客户端接收和发送消息的线程
	CWinThread*		m_commSendImageThd;	// 发送图像数据的线程
	CTCPServer*		m_pcommServer;		// 发送控制命令
	CTCPServer*		m_pcommSendImgData;	// 发送图像数据
	KYKYCommMsg_ImgData m_commmsgSendData;// 发送的图像数据的结构体
	volatile int	m_nCommRectLen;		// 批量发送的行数
	BOOL	m_bCommListenStarted;
	BOOL	m_bCommSendImgData;
	double	m_dCommMag, m_dCommWD, m_dCommBrt, m_dCommBrt2, m_dCommCst, m_dCommHV, m_dCommFila, m_dCommBias, m_dCommBeamCurrent;
	int		m_nCommScanRotAngInit, m_nCommScanRotAng;
	int		m_nCommSEMType, m_nCommStatusVac, m_nCommStatusGun, m_nCommHVType;
	CSize	m_szCommStigmator;
	BYTE	m_btCommStageStatus;
	CSize	m_szCommStageGrating, m_szCommStageRange, m_szCommStagePosition;
	KYKYCommMsg_Data2f	m_2fStageRange, m_2fStagePos;
	KYKYCommMsg_Data5f	m_5fStageRange5, m_5fStagePos5;

	void	ListenStart();

	void	KYKYComm_EndConnectThread();
	void	KYKYComm_Snap();
	void	KYKYComm_SimulImage();
	void	KYKYComm_SendSEMType();
	void	KYKYComm_SendMag();
	void	KYKYComm_SendSRotAngInit();
	void	KYKYComm_SendSRotAng();
	void	KYKYComm_SendWD();
	void	KYKYComm_SendBrt();
	void	KYKYComm_SendBrt2();
	void	KYKYComm_SendCst();
	void	KYKYComm_SendStigmator();
	void	KYKYComm_SendVacStatus();
	void	KYKYComm_SendGunStatus();
	void	KYKYComm_SendHVType();
	void	KYKYComm_SendHV();
	void	KYKYComm_SendFilament();
	void	KYKYComm_SendBias();
	void	KYKYComm_SendBeamCurrent();
	void	KYKYComm_SendReso();
	void	KYKYComm_SetSendRow(int nCount = 8);

	void	KYKYComm_StageSendStatus();
	void	KYKYComm_StageSendGrating();
	void	KYKYComm_StageSendRange();
	void	KYKYComm_StageSendPosition();

	static DWORD WINAPI	KYKYComm_BatchSendImageData(LPVOID lparam);
	/////////////////////////////////////////////
private:
	volatile BOOL	m_bCommBatchSendEnd;
	/////////////////////////////////////////////

///////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScanView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnAdjustorCondenserLensReverse();
	afx_msg void OnUpdateAdjustorCondenserLensReverse(CCmdUI* pCmdUI);
	afx_msg void OnAdjustorObjectiveLensReverse();
	afx_msg void OnUpdateAdjustorObjectiveLensReverse(CCmdUI* pCmdUI);
	afx_msg void OnStigmatorAssistant();
	afx_msg void OnUpdateStigmatorAssistant(CCmdUI* pCmdUI);
	afx_msg void OnPhotoSnapshot();
	afx_msg void OnPhotoPreview();
	afx_msg void OnAutoBrightContrast();
	afx_msg void OnAutoFocus();
	afx_msg void OnAutoStigmator();
	afx_msg void OnAutoFilament();
	afx_msg void OnAbc();
	afx_msg void OnUpdateAbc(CCmdUI* pCmdUI);
	afx_msg void OnLabel();
	afx_msg void OnUpdateLabel(CCmdUI* pCmdUI);
	afx_msg void OnRuler();
	afx_msg void OnUpdateRuler(CCmdUI* pCmdUI);
	afx_msg void OnDate();
	afx_msg void OnUpdateDate(CCmdUI* pCmdUI);
	afx_msg void OnNote();
	afx_msg void OnUpdateNote(CCmdUI* pCmdUI);
	afx_msg void OnVideoPro();
	afx_msg void OnRobinsonDetector();
	afx_msg void OnUpdateRobinsonDetector(CCmdUI* pCmdUI);
	afx_msg void OnWobbler();
	afx_msg void OnUpdateWobbler(CCmdUI* pCmdUI);
	afx_msg void OnSACP();
	afx_msg void OnUpdateSACP(CCmdUI* pCmdUI);
	afx_msg void OnLeanCompensate();
	afx_msg void OnUpdateLeanCompensate(CCmdUI* pCmdUI);
	afx_msg void OnDynamicFocus();
	afx_msg void OnUpdateDynamicFocus(CCmdUI* pCmdUI);
	afx_msg void OnOscillograph();
	afx_msg void OnUpdateOscillograph(CCmdUI* pCmdUI);
	afx_msg void OnStandImage();
	afx_msg void OnOptionsSetup();
	afx_msg void OnViewZoomIn();
	afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomOut();
	afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnViewActualSize();
	afx_msg void OnUpdateViewActualSize(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomFit();
	afx_msg void OnUpdateViewZoomFit(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdatePhotoPreview(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFrameRate();
	afx_msg void OnUpdateFrameRate(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChangestateDelete();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBUTTONDiagnose();
	afx_msg void OnUpdateBUTTONDiagnose(CCmdUI* pCmdUI);
	afx_msg void OnBUTTONDiagnoseSetup();
	afx_msg void OnUpdateBUTTONDiagnoseSetup(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnScanTypeSplit();
	afx_msg void OnUpdateScanTypeSplit(CCmdUI* pCmdUI);
	afx_msg void OnScanTypeSplitZoom();
	afx_msg void OnUpdateScanTypeSplitZoom(CCmdUI* pCmdUI);
	afx_msg void OnScanTypeDet2();
	afx_msg void OnUpdateScanTypeDet2(CCmdUI* pCmdUI);
	afx_msg void OnStageCoordinateorigin();
	afx_msg void OnUpdateStageCoordinateorigin(CCmdUI* pCmdUI);
	afx_msg void OnOptionsListenStart();
	afx_msg void OnUpdateOptionsListenStart(CCmdUI* pCmdUI);
	afx_msg void OnOptionsListenStop();
	afx_msg void OnUpdateOptionsListenStop(CCmdUI* pCmdUI);
	afx_msg void OnScanTypeLive();
	afx_msg void OnUpdateScanTypeLive(CCmdUI* pCmdUI);
	afx_msg void OnRulerAdd();
	afx_msg void OnUpdateRulerAdd(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnAdjustorSortChanged(UINT nID);
	afx_msg void OnUpdateAdjustorSortChanged(CCmdUI* pCmdUI);
	afx_msg void OnAdjustorManualSortChanged(UINT nID);
	afx_msg void OnUpdateAdjustorManualSortChanged(CCmdUI* pCmdUI);
	afx_msg void OnScanTypeChanged(UINT nID);
	afx_msg void OnUpdateScanTypeChanged(CCmdUI* pCmdUI);
	afx_msg void OnScanSpeedChanged(UINT nID);
	afx_msg void OnUpdateScanSpeedChanged(CCmdUI* pCmdUI);
	afx_msg void OnScanResolutionChanged(UINT nID);
	afx_msg void OnUpdateScanResolutionChanged(CCmdUI* pCmdUI);
	afx_msg void OnScanStoreModeChanged(UINT nID);
	afx_msg void OnUpdateScanStoreModeChanged(CCmdUI* pCmdUI);
	afx_msg void OnCondenserFastChanged(UINT nID);
	afx_msg void OnUpdateCondenserFastChanged(CCmdUI* pCmdUI);
	afx_msg LRESULT OnParamChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSMvacChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKYKYComm_ReplyAsk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKYKYComm_Connect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKYKYComm_ConnectThread(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TempView.cpp
inline CScanDoc* CScanView::GetDocument()
   { return (CScanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANVIEW_H__AB607E47_147C_45D3_9F18_032220C2EF5D__INCLUDED_)
