#if !defined(AFX_PANELSTAGE_H__51C9CBE3_D28E_4E90_82A0_D0D5F254F4C5__INCLUDED_)
#define AFX_PANELSTAGE_H__51C9CBE3_D28E_4E90_82A0_D0D5F254F4C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelStage.h : header file
//

#include "CommMC6600.h"
#include "Comm_MC6600.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelStage dialog

class CPanelStage : public CDialog
{
	HICON		m_hIconStage1;
	HICON		m_hIconStage2;
	HICON		m_hIconStage3;
	HICON		m_hIconStage4;
	HICON		m_hIconStage5;
	HICON		m_hIconStage6;
	HICON		m_hIconStage7;
	HICON		m_hIconStage8;
	HICON		m_hIconStage9;
	HICON		m_hIconStage10;

// Construction
public:
	CPanelStage(CWnd* pParent = NULL);   // standard constructor
	void	Message(LPCTSTR lpszMessage);
	CString	DisplayOutput(short anxi, float fValue, float fGoal);

	BOOL	m_bNum[5], m_bToHome[5];

	int		m_nPort;		// 通讯端口号
	char	m_nAxial;		// 轴向
	int		m_nAddress;		// 从站地址显示
	BOOL	m_bIsMC6600Ready;				// 通讯端口是否已打开连接
	BOOL	m_bEnable[5];					// 是否使能
	BOOL	m_bIsMoving[5];					// 是否正在移动
	BOOL	m_bDirection[5];				// 驱动方向
//	BOOL	m_bWD;							// Z向是否显示工作距离
//	int		m_nStageType;
	BOOL	m_bRelative;					// 是否为相对运动
	BOOL	m_bBL[5];						// 是否有间隙
	float	m_fBLCount[5], m_fSLength[2];	// 含间隙量目标值    
	BOOL	m_bBlClick, m_bBlTrack;			// 双击和跟踪时是否消间隙
	int		m_nVentSel;						// 放气前的选项
	double	m_dEquivalent[5];				// 码数与物理量mm的换算系数
	float	m_fCurStep[5], m_fDis[5];		// 当前位置
	float	m_fCali[5], m_fGoal[5];			// 扫描中心位置mm，目标位置mm
	UINT	m_nTimer[10];					// 定时器
	CRect	m_rcWindow, m_rcReport;			// 窗口尺寸
	float	m_fDeadline[5];					// 死区精度
	float	m_fCalibrationZ, m_fCalibrationR, m_fCalibrationT;
	int		m_nProcess, m_nMoveSteps;		// 移动进程类型，某一移动进程中的具体步骤
	BOOL	m_bEqual[5], m_bOrigion[5];
	BOOL	m_bXdir, m_bYdir;
	BOOL	m_bHighPrecCount;				// 高精度计数显示
	float	m_fMaxSpeed[5], m_fMinSpeed[5];
	BOOL	m_bSoftLimit;
	BOOL	m_bAxisInitia[5], m_bMotorFault[5], m_bCommunicationFault[5];// 调试用：哪个轴不正常，就不用哪个轴
	BOOL	m_bMoveXY;
	int     m_nPhaseInit[5];				// 初始化阶段，初始化 0-右限位 1-左限位 2-机械原点
	BOOL	m_bDbClick;						// 双击

	MC6600_Params_Speed			m_paramsSpeed;
	MC6600_Params_Current		m_paramsCurrent;
	MC6600_Params_Bl			m_paramBl;			// 间隙参数相关
	MC6600_Params_ZRes			m_paramZRes;		// Z向约束参数相关
	MC6600_Params_AxialAddr		m_paramAddr;		// 从站地址
	MC6600_Params_LimitDir		m_paramLimitDir;	// 极限
	MC6600_Params_SetupWizard	m_paramWizard;		// 设置向导
	MC6600_Params_PID			m_paramDefaultPID, m_paramCurPID;	// 默认PID，当前PID
	MC6600_Params_Style			m_paramStyle;		// 是否有老控制器
	MC6600_Params_DriveDir		m_paramDriveDir;	// 驱动方向
	MC6600_Params_Fault			m_paramFault;		// 通讯故障

	void	SetStatus(BOOL bEnable);

	void	Connect();
	void	InitParam_SysParams();
	void	InitParam_Backlash( char* path );
	void	InitParam_Calibration( char* path );
	void	InitParam_SetWizard( char* path );
	void	InitParam_SetDefaultPID( char* path );
	void	SyncParamsPos();
	void	SaveToINI( int nIndex );

	void	GetCurStep( short anxi );				// 获取当前位置
	BOOL	QueryMoving();
	void	RestoreSpeed();
	void	Stop();
	void	Grounding( unsigned char ucAxis, short anxi );
	void	GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 );

	void	FindIndex();
	void	DiffCenter(short anxi);
	void	StartPos(short anxi);
	void	StartPos_LimitDir( short anxi, unsigned char ucAxis );

	void	Zero();
	void	ZeroAxis( short anxi );

	void	Home();
	void	HomeAxis( short anxi );

	float	RDriveConversion(float fRCurrent, float fRTarget);
	CString	RDriveConversion_Disp( float fJudge, float fFormat );

	void	MoveInput( short anxi );				// 手动输入走
	void	MoveMinusPlus( int nIndex );			// 单步走
	void	MoveContinues( int nIndex );			// 持续走
	void	MoveAxis( short anxi, float fStep );	// 单轴走
	void	MoveAxis_FromThird( short anxi, float fStep );	// 单轴走

	void	Move5Axis(float fX, float fY, float fZ, float fR, float fT );
	void	Move5Axis_JudgeDirMove( short anxi );
	void	Move5Axis_FromThird( float fX, float fY, float fZ, float fR, float fT );

	void	Move2Axis(float fX, float fY);
	void	Move2Axis_FromThird( float fX, float fY );

	void	MoveSteps();							// 移动进程类型走
	void	MoveSteps_Axis( short anxi );
	void	MoveSteps_JudgeDirMove( short anxi );

	void	SaveUserOrigin();	// 保存工作原点绝对位置
	void	SetUserOrigin();	// 回到原点
	void	OnVentExit();		// 放气和退出时的操作

	void	SafeLimit(unsigned char ucAxis, float fState);
	void	Safe_Axis( unsigned char ucAxis, float fState, int nType );

	void	Timer_Axis( short anxi );
	void	ToHome_XY( unsigned char ucAxis, short anxi );
	void	ToHome_ZRT( unsigned char ucAxis, short anxi );
	void	NonToHome( unsigned char ucAxis, short anxi );
	void	Timer_KillTimer( UINT nIDEvent );
	void	Timer_Tracklocus( UINT nIDEvent, short anxi );

	// 推拉杆相关
	BOOL	m_bTrackState[5];
	//float	m_fTrackStep, m_fTrackStepR, m_fTrackStepT;
	float	m_fTrackStepAxis,m_fRockerStep[5];
	MC6600_Params_Speed	m_paramsTrackSpeed;
	void	SetTracklocusAxis1( short anxi, int nSpeed );
	void	SetTracklocusAxis2( short anxi, int nSpeed );
	void	SetTracklocus1( int nSpeedX, int nSpeedY, int nSpeedR );
	void	SetTracklocus2( int nSpeedT, int nSpeedZ);
	void	SetTracklocusAxis( short anxi, BOOL bDirect );
	void	RestoreSpeedlocus();
	void	StopTracklocus();

	//拖曳相关
	void	SetTrackParamAxis( short anxi, int nSpeed );
	void	SetTrackParam( int nSpeedX, int nSpeedY );
	void	StopTrack();
	void	StopTrackAxis1( short anxi );
	void	StopTrackAxis2( short anxi );

	// Z对X/Y的约束
	float	m_factualLimitH[10],m_factualLimit[10];	// XY轴实际运动范围
	float	m_fSLimit[10];
	void	InitParam_ZRestraint( char* path );// Z向约束Restraint，数学模型
	void	CoherenceCheck_Z( float& fXExp, float& fYExp, float fZExp, BOOL bAfter );

	//放大倍数相关
	CSize	m_sizeReso;
	void	CalculateStageUScale( int nAxis,		// 是哪个轴
							 double	dScalePixels,	// 像素长度
							 float	&nScale );		// 实际移动步数
	double	m_dMag;					// 当前放大倍数
	double	m_dblPixelsPerMM_HORZ, m_dblPixelsPerMM_VERT;
	void	SetMag( double dMag );	// 主程序设置的放大倍数
	void	MagChange();			// 更新放大倍数相关参数
	void	CoherenceCheck_Mag();	// 检查放大倍数范围等参数
	void	SetResolution( CSize szReso );

	//位置存储有关
	void	List_SetHead( int nIndex, CString strName );	// 设置表头
	void	List_Init();									// 表格初始化
	void	List_Load();									// 表格装载
	void	List_ChangeName( int nItem, CString strName );	// 某一位置改名
	void	List_Update();									// 表格更新
	void	List_Goto( int nItem );							// 跳转到某一位置

	CString	m_strPosListHead[6];		// 表格每列标签
	int		m_nPosTotal;				// 存储位置的总数
	int		m_nPosColWidth[6];			// 表格每列宽度
	int		m_nPosEdit;					// 单击了哪一列
	ListMC6600Pos*	m_plistMC6600Pos;	// 位置结构列表

	// 本地化相关
	char	m_cPathLocalize[256];
	WORD	m_wLanguageID;				// 语言类型
	void	SetLanguageID( WORD wID );	// 主程序设置的语言类型
	void	Localize();					// 本地化
	CString	m_strMsg[20];	// char* m_szMsg[20];// 字符串数组（指向char类型的指针数组）

	//屏幕操作相关
	void	SetPixelsPerMM( double dX, double dY );
	void	MoveToCenter( int nCenterX, int nCenterY, int nX, int nY );

	//日志相关
	HANDLE	m_hFileLog;
	int		m_nLoopsLog;
	void	InitLog();

	//手动、自动相关
	BOOL	m_bMotoronoff[5],m_bMotoronoffAll;
	void	MotorOnOff(short anxi);
	void	Timer_CurPos(short anxi);
	void	JudgeMotorOff(short anxi,BOOL bMotoronoff);
	void	MantoAuto(short anxi);
	void	MantoAutoAll();

// Dialog Data
	//{{AFX_DATA(CPanelStage)
	enum { IDD = IDD_PANEL_STAGE };
	CListCtrl m_listPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelStage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelStage)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDblclkListPosition(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckRelative();
	afx_msg void OnBnClickedCheckAdvance();
	afx_msg void OnBnClickedCheckZWd();
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnBnClickedButtonZero();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonMoveto();
	afx_msg void OnBnClickedButtonListChange();
	afx_msg void OnBnClickedButtonListadd();
	afx_msg void OnBnClickedButtonListdel();
	afx_msg void OnBnClickedButtonSetuserorigin();
	afx_msg void OnBnClickedButtonSetup();
	afx_msg void OnBnClickedButtonDriverInit();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheckAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
	afx_msg void OnHomeChanged(UINT nID);
	afx_msg void OnZeroChanged(UINT nID);
	afx_msg void OnMoveMinusChanged(UINT nID);
	afx_msg void OnMovePlusChanged(UINT nID);
	afx_msg void OnMoveMinus2Changed(UINT nID);
	afx_msg void OnMovePlus2Changed(UINT nID);
	afx_msg void OnMotorChanged(UINT nID);
	afx_msg LRESULT OnUpdateMC6600Value(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELSTAGE_H__51C9CBE3_D28E_4E90_82A0_D0D5F254F4C5__INCLUDED_)
