#if !defined(AFX_PANELSTAGE_H__8E93DBBF_F909_4CFF_8245_F950A252B135__INCLUDED_)
#define AFX_PANELSTAGE_H__8E93DBBF_F909_4CFF_8245_F950A252B135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelStage.h : header file
//

#include "CommMC600.h"
#include "Comm_MC600.h"

#include "CommVince.h"
#include "Comm_Vince.h"

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
	void	Connect();
	void	FindIndex();
	void	SetStatus(BOOL bEnable);
	void	Message(LPCTSTR lpszMessage);

	void	GetCurStep( short anxi );
	void	GetSpeed( unsigned char ucAxis );
	void	GetSpeedAcc( unsigned char ucAxis );
	void	GetSpeedInit( unsigned char ucAxis );

	void	SetTrackParam( int nSpeedX, int nSpeedY );
	void	StopTrack();
	void	RestoreSpeed();

	void	Stop();
	void	Move( short anxi );
	void	Move( short anxi, float fPos );
	void	MoveZ( float fZ, BOOL bRelative );
	void	Move4Axis( int nProcess, float fX, float fY, float fZ, float fR );
	void	Move5Axis( int nProcess, float fX, float fY, float fZ, float fR, float fT );
	void	MoveSteps();
	BOOL	QueryMoving();
	void	Home( int nProcess );
	void	Zero();
	void	OnVentExit();
	void	SaveUserOrigin();
	void	SetUserOrigin();

	void	SyncParamsSys();
	void	SyncParamsSpeed();
	void	SyncParamsPos();
	void	InitParam_SysParams();
	void	InitParam_Backlash( char* path );

	int		m_nStageType;

	CCommMC600	m_MC600;
	MC600_Params_Sys	m_paramsSys;
	MC600_Params_Speed	m_paramsSpeed;
	MC600_Params_Pos	m_paramsPos;
	MC600_Params_Bl		m_paramBl;			// 间隙参数相关
	MC600_Params_ZRes	m_paramZRes;		// Z向约束参数相关
	float				m_actualLimit[6];	// XYZ轴实际运动范围

	long	m_lSpeed[5], m_lSpeed2[5], m_lSpeed3[5];	// 速度，加速度，初始速度
	double	m_dEquivalent[5];				// 计数量pp与物理量mm的换算系数
	long	m_lCurStep[5];					// 当前位置
	UINT	m_nTimer[5], m_nTimerCounts[5];	// 定时器
	float	m_fCali[5], m_fGoal[5];			// 扫描中心位置mm，目标位置mm
	float	m_fZ0Wd;						// Z向光电零点对应的工作距离值mm

	int		m_nPort, m_nPort2;	// 通讯端口号
	BOOL	m_bIsMC600Ready;	// 通讯端口是否已打开连接
	BOOL	m_bIsMoving;		// 是否正在移动
	BOOL	m_bRelative, m_bRelativeBak;	// 是否为相对运动
	BOOL	m_bWD;				// Z向是否显示工作距离
	int		m_nProcess, m_nMoveSteps;		// 移动进程类型，某一移动进程中的具体步骤
	BOOL	m_bBlClick, m_bBlTrack;			// 双击和跟踪时是否消间隙
	int		m_nVentSel;						// 放气前的选项
	BOOL	m_bOptR;						// R向是否使用光电开关

	CRect	m_rcWindow, m_rcReport;	// 窗口尺寸
	char	m_cPathLocalize[256];

	///////////////////////////////////
	// Z对X/Y的约束相关：对Z > 10mm的较大Z值考虑限制XY正负向
	float	m_fLimit[255][5];		// 0为Z，1为X负，2为X正，3为Y负，4为Y正
	int		m_nZLimitTotal;
	void	InitParam_ZRestraint( char* path );	// Z向约束Restraint，查询表
	void	InitParam_ZRestraint_Fixed( char* path );	// Z向约束Restraint，数学模型，固定的参数
	void	InitParam_ZRestraint_User( char* path );	// Z向约束Restraint，数学模型，用户可设置的参数
	void	CoherenceCheck_Z();		// 检查Z对X/Y的约束参数
	void	CoherenceCheck_Z2( float& fXExp, float& fYExp, float fZExp, BOOL bAfter );	// 检查Z对X/Y的约束参数
	void	SetZLimit();
	///////////////////////////////////

	///////////////////////////////////
	// 日志相关
	HANDLE	m_hFileLog;
	int		m_nLoopsLog;
	void	InitLog();
	///////////////////////////////////

	///////////////////////////////////
	// 暂未使用的原MC300的变量
	long	m_lStepBase;	// 步距基数
	double	m_dStepRatio;	// 步距系数比例
	int		m_nSpeedRatio;	// 速度系数比例
	int		m_nHomeState;	// 归零步骤
	int		m_nSpeed[5];	// 速度档位0,1,2,3,4,5,6,7
	///////////////////////////////////

	///////////////////////////////////
	// 放大倍数相关
	CSize	m_sizeReso;
	void	CalculateStageUScale( int nAxis,		// 是哪个轴
							 double	dScalePixels,	// 像素长度
							 int	&nScale );		// 实际移动步数
	void	CoherenceCheck_Mag();	// 检查放大倍数范围等参数
	void	MagChange();			// 更新放大倍数相关参数
	void	SetMag( double dMag );	// 主程序设置的放大倍数
	void	SetResolution( CSize szReso );
	double	m_dMag;					// 当前放大倍数
	double	m_dblPixelsPerMM_HORZ, m_dblPixelsPerMM_VERT;
	///////////////////////////////////

	///////////////////////////////////
	// 屏幕操作相关
	void	SetPixelsPerMM( double dX, double dY );
	void	MoveToCenter( int nCenterX, int nCenterY, int nX, int nY );
	///////////////////////////////////

	///////////////////////////////////
	// 位置存储相关
	void	List_SetHead( int nIndex, CString strName );	// 设置表头
	void	List_Init();									// 表格初始化
	void	List_Load();									// 表格装载
	void	List_ChangeName( int nItem, CString strName );	// 某一位置改名
	void	List_Update();									// 表格更新
	void	List_Goto( int nItem );							// 跳转到某一位置

	CString	m_strPosListHead[6];		// 表格每列标签
	int		m_nPosColWidth[6];			// 表格每列宽度
	int		m_nPosEdit;					// 单击了哪一列
	int		m_nPosTotal;				// 存储位置的总数
	ListMC600Pos*	m_plistMC600Pos;	// 位置结构列表
	///////////////////////////////////

	///////////////////////////////////
	// 本地化相关
	WORD	m_wLanguageID;				// 语言类型
	void	SetLanguageID( WORD wID );	// 主程序设置的语言类型
	void	Localize();					// 本地化
	CString	m_strMsg[10];	// char* m_szMsg[10];// 字符串数组（指向char类型的指针数组）
	///////////////////////////////////

	void	SetParam( int nIndex, double dParam );
	void	GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 );
	BOOL	m_bPEswitch;

	///////////////////////////////////
	// T 轴
	CCommVince	m_Vince;
	Vince_Param_Status	GetStatus( short anxi );
	BOOL	m_bIsVinceReady;	// 通讯端口是否已打开连接
	void	SetStatus2( BOOL bEnable, BOOL bShow = FALSE );
	///////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CPanelStage)
	enum { IDD = IDD_PANEL_Stage };
	CListCtrl	m_listPos;
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
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBUTTONXMoveMinus();
	afx_msg void OnBUTTONXMovePlus();
	afx_msg void OnBUTTONYMoveMinus();
	afx_msg void OnBUTTONYMovePlus();
	afx_msg void OnBUTTONZMoveMinus();
	afx_msg void OnBUTTONZMovePlus();
	afx_msg void OnBUTTONRMoveMinus();
	afx_msg void OnBUTTONRMovePlus();
	afx_msg void OnBUTTONTMoveMinus();
	afx_msg void OnBUTTONTMovePlus();
	afx_msg void OnBUTTONXMoveMinus2();
	afx_msg void OnBUTTONXMovePlus2();
	afx_msg void OnBUTTONYMoveMinus2();
	afx_msg void OnBUTTONYMovePlus2();
	afx_msg void OnBUTTONZMoveMinus2();
	afx_msg void OnBUTTONZMovePlus2();
	afx_msg void OnBUTTONRMoveMinus2();
	afx_msg void OnBUTTONRMovePlus2();
	afx_msg void OnBUTTONTMoveMinus2();
	afx_msg void OnBUTTONTMovePlus2();
	afx_msg void OnBUTTONXMoveTo();
	afx_msg void OnBUTTONYMoveTo();
	afx_msg void OnBUTTONZMoveTo();
	afx_msg void OnBUTTONRMoveTo();
	afx_msg void OnBUTTONTMoveTo();
	afx_msg void OnBUTTONMoveTo();
	afx_msg void OnBUTTONStop();
	afx_msg void OnBUTTONZero();
	afx_msg void OnBUTTONZeroX();
	afx_msg void OnBUTTONZeroY();
	afx_msg void OnBUTTONZeroZ();
	afx_msg void OnBUTTONZeroR();
	afx_msg void OnBUTTONZeroT();
	afx_msg void OnBUTTONHome();
	afx_msg void OnBUTTONHomeX();
	afx_msg void OnBUTTONHomeY();
	afx_msg void OnBUTTONHomeZ();
	afx_msg void OnBUTTONHomeR();
	afx_msg void OnBUTTONHomeT();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDeltaposStageMagAdj(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCHECKRelative();
	afx_msg void OnCHECKAdvance();
	afx_msg void OnBUTTONSetup();
	afx_msg void OnBUTTONBacklash();
	afx_msg void OnBUTTONListChange();
	afx_msg void OnBUTTONListAdd();
	afx_msg void OnBUTTONListDel();
	afx_msg void OnDblclkLISTPosition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBUTTONSetUserOrigin();
	afx_msg void OnCheckZWd();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateMC600Value(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateVinceValue(WPARAM wParam, LPARAM lParam);
//	static   LRESULT   CALLBACK   GetMsgHookProc(int   nCode,   WPARAM   wParam,   LPARAM   lParam); 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELSTAGE_H__8E93DBBF_F909_4CFF_8245_F950A252B135__INCLUDED_)
