// Last Modify : 2009.12.10

/////////////////////////////////////////////////////////////////////////////
// 09.11.18 ���ӵ�����������GraphHitTestNULL
// 09.12.10 ���ӵ���״̬����

// 12.02.15 ���ӵ�˫̽������ʶ
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
#include "XXX2.h"
#include "ScanParameterSaver.h"
/*
// 1 inch = 25.4 mm
const double mm_TO_inch				= (double)0.0393700787401574803149606299212598;
const double SEM_dfStandImageWidth	= (double)127;		// mm
const double SEM_dfStandImageHeight	= (double)89;		// mm
*/
///////////////////////////////////////
// 15.08 Զ�̹���
#include "TCPServer.h"
#include "Message.h"
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
		GraphHitTest_C		= 11,	// ����
		GraphHitTest_L		= 12,	// ��ע
		// 09.11.18 ���ӵ�����������GraphHitTestNULL
		GraphHitTest_NULL	= 13,
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
	BOOL	m_bFOV;
	BOOL	m_bPixelSize;
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
	
	BOOL	m_bStopPhoto;	// 2004-5-12_�������Զ�����

	void	USB_ChangeScan( USB_ScanType nusbScanType = USB_ST_Max );
	UINT	USB_GraphHitTest( const CPoint ptGraphPos );
	void	USB_SetScreenMouseCtrl( CPoint point );

//LMH
protected:
	CPoint		m_ptPosSave;
	int			m_nGraphHitTestSave;
	BOOL		m_bCatch;
	
	CRect		m_rcDragRectLast;
	//CSize		m_szDragSizeLast;

	int			m_nMobileUScalePixelNum;
	CPoint		m_ptMobileUScale;
	CPoint		m_ptMobileUScale2;		// 07.07---07.08�������ⷽ����
	BOOL		m_bUScaleHitTest;

	// 16.06.12 ���ƶ����ʵ��
	int			m_nMobileUScalePixelNumArray[MobileUScaleCounts];
	CPoint		m_ptMobileUScaleArray[MobileUScaleCounts];
	CPoint		m_ptMobileUScaleArray2[MobileUScaleCounts];
	BOOL		m_bUScaleHitTestArray[MobileUScaleCounts];
	void		SetUScaleParamArray(int nIndex, CPoint point);

	int			m_nMobileUScaleCounts;	// �����ӵ��ƶ������Ŀ
	// 16.06.12 ���ƶ����ʵ��

	// 2023.07 �ǶȲ���
	BOOL		m_bAngle, m_bAddAngle, m_bAngleHitTest;
	int			m_nAngleCounts, m_nAngleIndex;
	//	AngleMeasure	m_AM[AngleMeasureCounts], m_AMbak[AngleMeasureCounts];	// Angle Measure
	ListAngle	*m_plistAngle;
	AngleOption	m_AngleOption;

	void		AngleAddNew();
	double		GetAngleOfTwoVector( CPoint ptS, CPoint ptE, CPoint ptC );
	CPoint		GetCoordinates( CPoint ptCenter, double dAngle, int nR );
	void		GetAngleMeasureInfo( AnglePosition& AP );
	void		GetAnglePoints( AngleMeasure* pAM, CPoint ptPos );
	void		ResetAngleList();
	// 2023.07 �ǶȲ���

	CPoint		m_ptOscillograph;
	BOOL		m_bOscillographHitTest;

	int			m_nMapMode;
	CSize		m_szTotalLog;			// total size in logical units (no rounding)
	CSize		m_szTotalDev;			// total size in device units

	BOOL		m_bCenter;             // Center output if larger than total size
	BOOL		m_bInsideUpdate;       // internal state for OnSize callback

	CBrush		m_brHatch;
//LMH
	CBitmap		m_bmpBackground;

	//07.04.26��ʾͼ��С��
public:
	CImageItem*				m_pSelItem;
	ListImageItem*			m_plistImageItem;
	BOOL					m_bCtrlDown;		//Ctrl�����±�־08.06.27
	BOOL					m_bIsMultiSel;		//���ļ�ѡ���־08.06.27

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

	BOOL		m_bDebugAccel;		// 09.12.10 ���ӵ���״̬����
	UINT		nMCB_Timer;

	///////////////////////////////////////////////////////
	void		Diagnose( BOOL bCheck );
	CString		Diag_VAC();
	CString		Diag_SPR();
	CString		Diag_MCI();
	CString		Diag_Logging();
	CString		Diag_Logging2();
	///////////////////////////////////////////////////////
	void		SetLineScanParamManually( BOOL bHORZ, int nPos );
	void		SetUScaleParamByDefine( double dScale );

	// 15.10 XY�Զ�̨
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

	// 15.11 У׼crossoverʱʹ��Ļ������ʾ�ο��㣬�ұ�׼�����Բο���Ϊ���ĵ�������
	BOOL		m_bDebugCoadjust;

public:
// Attribute
	UINT		m_nAutoFunctionTimer;
	DWORD		m_dwFramGrayHistogramBuff[256];

	LPBYTE		m_bImageBuff;
	int			m_nLoop;

	CDialog*	m_pAutoProgressDlg;

// Operations
	void	AFTest();
	void	AutoFocusOK( long lObjectiveLens );
	void	AutoAstigmatOK( long lObjectiveLens, long lStigmatorX, long lStigmatorY );
	void	AutoBCOK( long lBrightness, long lContrast );
	void	AutoFilamentOK( long lFila );

	void	StartAutoFocus();
	void	AutoFocusProcess();
	void	StopAutoFocus();
	void	AutoFocus_Setup(const long lObjectiveLens, const double dblMagnifier = 200.0 );

	void	AFTest1();
	void	AutoFocusOK1( long lObjectiveLens );
	void	StartAutoFocus1();
	void	AutoFocusProcess1();
	void	StopAutoFocus1();
	void	AutoFocus_Setup1(const long lObjectiveLens, const double dblMagnifier = 200.0 );

	void	StartAutoAstigmat();
	void	AutoAstigmatProcess();
	void	StopAutoAstigmat();

	void	StartAutoBC();
	void	AutoBCProcess();
	void	StopAutoBC();

	// 11.01.28 �����Զ����ȶԱȶȹ���
	void	StartAutoBC2();
	void	AutoBCProcess2();
	void	StopAutoBC2();
	// 11.01.28 �����Զ����ȶԱȶȹ���
	
	void	StartAutoFilament();
	void	AutoFilamentProcess();
	void	StopAutoFilament();

	void	FuncAutoFilament();
	void	FuncAutoBrightContrast();
	void	FuncAutoFocus();
	void	FuncAutoFocus1();
	void	FuncAutoStigmator();

	void	FuncAutoWaitHV( BOOL bAutoFinish );

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

	BOOL	IsPhoto();
	void	ResetImageCard();
	void	StopPhoto();

	void	ScreenZoom();

//	void	ChangeScan( int nScanType = -1 );
	void	UpdatePhotoSn(int nSN);

	void	StandImage( int nDelt );

	void	UpdateWindowPos( BOOL bDual = FALSE);
	void	UpdateScale();

	// 17.09 CCD�ڿ���ɢ�����ʱ�򲻹���
	void	CheckCCDSwitch();

	// 2024.06 ����û�������ɨ����϶���ɨ������֮����޷���Ч������
	void	SetActiveAreaParam_FromMainFrm(CPoint point);

//LMH//////////////////////////////////
public:
	CPoint	ScreenToGraph( const CPoint point );
	// 07.07---07.08�������ⷽ����
	void	GetFitScreenClientRect( CRect& rcScreenClientRect );
	void	AdjustPos( CPoint& ptPos );
	void	AdjustUScaleLength( CPoint& ptStart, CPoint& ptEnd, BOOL bTranslation = FALSE );
	// 07.07---07.08�������ⷽ����

	// 2005-1-17_�Ŵ���XY����У��
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
	void    _GetScanTypeChangePromptMsg(CString &str);
	void    _DisplayMsgOnViewCreate(const int nReady); 
	void    _InitLogoDisp(char * path);
	
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
	CWinThread*		m_commCmdThd;		// �ͻ��˽��պͷ�����Ϣ���߳�
	CWinThread*		m_commSendImageThd;	// ����ͼ�����ݵ��߳�
	CTCPServer*		m_pcommServer;		// ���Ϳ�������
//	CTCPServer*		m_pcommSendImgData;	// ����ͼ������
	KYKYCommMsg_ImgFrameData m_commmsgSendData;// ���͵�ͼ�����ݵĽṹ��
	volatile int	m_nCommRectLen;		// �������͵�����
	BOOL	m_bCommListenStarted;
	BOOL	m_bCommSendImgData;
	BOOL	m_bCommStopScan;
	double	m_dCommMag, m_dCommWD, m_dCommBrt, m_dCommBrt2, m_dCommCst, m_dCommHV, m_dCommFila, m_dCommBias, m_dCommBeamCurrent, m_dCommEMI;
	int		m_nCommScanRotAngInit, m_nCommScanRotAng;
	int		m_nCommSEMType, m_nCommStatusVac, m_nCommStatusGun, m_nCommHVType;
	CSize	m_szCommStigmator;
	BYTE	m_btCommStageStatus;
	CSize	m_szCommStageGrating, m_szCommStageRange, m_szCommStagePosition;
	KYKYCommMsg_Data2f	m_2fStagePos, m_2fMagRange;
	KYKYCommMsg_Data5f	m_5fStagePos5, m_5fFieldOfView;
	KYKYCommMsg_Data2fGroup	m_2fStageRange;
	KYKYCommMsg_Data5fGroup	m_5fStageRange5;
	int		m_nCommTimerMode, m_nCommTimerInteval;

	void	ListenStart();

	void	KYKYComm_EndConnectThread();
	void	KYKYComm_SendSEMType();
	void	KYKYComm_SendMagRange();
	void	KYKYComm_SendMag();
	void	KYKYComm_SendFOV();
	void	KYKYComm_SendSRotAngInit();
	void	KYKYComm_SendSRotAng();
	void	KYKYComm_SendWD();
	void	KYKYComm_SendBrt();
	void	KYKYComm_SendBrt2();
	void	KYKYComm_SendCst();
	void	KYKYComm_SendStigX();
	void	KYKYComm_SendStigY();
	void	KYKYComm_SendVacStatus();
	void	KYKYComm_SendGunStatus();
	void	KYKYComm_SendHVType();
	void	KYKYComm_SendHV();
	void	KYKYComm_SendFilament();
	void	KYKYComm_SendBias();
	void	KYKYComm_SendBeamCurrent();
	void	KYKYComm_SendEMI();
	void	KYKYComm_SendReso();
	void	KYKYComm_SendSpeed();
	void	KYKYComm_SetSendRow(int nCount = 8);

	void	KYKYComm_StageSendStatus();
	void	KYKYComm_StageSendGrating();
	void	KYKYComm_StageSendRange();
	void	KYKYComm_StageSendPosition();

	void	KYKYComm_SendIsStopScan();
	void	KYKYComm_Snap();
	void	KYKYComm_ImageFrame();
	void	KYKYComm_ImageSync();
	static DWORD WINAPI	KYKYComm_BatchSendImageData(LPVOID lparam);

private:
	volatile BOOL	m_bCommBatchSendEnd;


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
public:
	afx_msg void OnPhotoPreview();
	afx_msg void OnScanTypeChanged(UINT nID);

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
	afx_msg void OnAutoBrightContrast();
	afx_msg void OnAutoFocus();
	afx_msg void OnAutoContrast();
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
	afx_msg void OnFOV();
	afx_msg void OnUpdateFOV(CCmdUI* pCmdUI);
	afx_msg void OnPixelSize();
	afx_msg void OnUpdatePixelSize(CCmdUI* pCmdUI);
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
	afx_msg void OnAngle();
	afx_msg void OnUpdateAngle(CCmdUI* pCmdUI);
	afx_msg void OnAngleAdd();
	afx_msg void OnUpdateAngleAdd(CCmdUI* pCmdUI);
	afx_msg void OnAngleOptions();
	afx_msg void OnAngleDelete();
	//}}AFX_MSG
	afx_msg void OnAdjustorSortChanged(UINT nID);
	afx_msg void OnUpdateAdjustorSortChanged(CCmdUI* pCmdUI);
	afx_msg void OnAdjustorManualSortChanged(UINT nID);
	afx_msg void OnUpdateAdjustorManualSortChanged(CCmdUI* pCmdUI);
	afx_msg void OnUpdateScanTypeChanged(CCmdUI* pCmdUI);
	afx_msg void OnScanSpeedChanged(UINT nID);
	afx_msg void OnUpdateScanSpeedChanged(CCmdUI* pCmdUI);
	afx_msg void OnScanResolutionChanged(UINT nID);
	afx_msg void OnUpdateScanResolutionChanged(CCmdUI* pCmdUI);
	afx_msg void OnScanStoreModeChanged(UINT nID);
	afx_msg void OnUpdateScanStoreModeChanged(CCmdUI* pCmdUI);
	afx_msg LRESULT OnParamChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSMvacChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKYKYComm_ReplyAsk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKYKYComm_Connect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKYKYComm_ConnectThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStartAutoFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStopAutoFocus(WPARAM wParam, LPARAM lParam);
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
