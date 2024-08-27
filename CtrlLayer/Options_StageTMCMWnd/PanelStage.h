#if !defined(AFX_PANELSTAGE_H__51C9CBE3_D28E_4E90_82A0_D0D5F254F205__INCLUDED_)
#define AFX_PANELSTAGE_H__51C9CBE3_D28E_4E90_82A0_D0D5F254F205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelStage.h : header file
//

#include "CommTMCM.h"
#include "Comm_TMCM.h"
// CPanelStage �Ի���

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

	BOOL   m_bNum[5], m_bToHome[5];

	int		m_nPort;						// ͨѶ�˿ں�
	int		m_nBaudrate;
	BOOL	m_bIsTMCMReady;					// ͨѶ�˿��Ƿ��Ѵ�����
	BOOL    m_bEnable[5];					// �Ƿ�ʹ��
	BOOL    m_bIsMoving[5], m_bIsMove;		// �Ƿ������ƶ�
	BOOL	m_bDirection[5];				// ��������
//	BOOL	m_bWD;							// Z���Ƿ���ʾ��������
//	int     m_nStageType;
	BOOL	m_bRelative;					// �Ƿ�Ϊ����˶�
	BOOL    m_bBL[5];						// �Ƿ��м�϶
	float   m_fBLCount[5], m_fSLength[2];	// ����϶��Ŀ��ֵ    
	BOOL	m_bBlClick, m_bBlTrack;			// ˫���͸���ʱ�Ƿ�����϶
	int		m_nVentSel;						// ����ǰ��ѡ��
	double	m_dEquivalent[5];				// ������pps��������mm�Ļ���ϵ��
	float	m_fCurStep[5], m_fDis[5];		// ��ǰλ��
	float	m_fCali[5], m_fGoal[5];			// ɨ������λ��mm��Ŀ��λ��mm
	UINT	m_nTimer[10];					// ��ʱ��
	CRect	m_rcWindow, m_rcReport;			// ���ڳߴ�
	float   m_fDeadline[5];					// ��������
	float   m_fCalibrationZ, m_fCalibrationR, m_fCalibrationT;
	int     m_nProcess, m_nMoveSteps;		// �ƶ��������ͣ�ĳһ�ƶ������еľ��岽��
	BOOL    m_bEqual[5], m_bOrigion[5];
	BOOL	m_bXdir, m_bYdir;
	BOOL    m_bHighPrecCount;				// �߾��ȼ�����ʾ
	float   m_fMaxCurSpeed[5], m_fMinCurSpeed[5];
	float   m_fMaxStandbySpeed[5],m_fMinStandbySpeed[5];
	float   m_fMaxPospeed[5], m_fMinPospeed[5];
	float   m_fMaxAccspeed[5],m_fMinAccspeed[5];
	float   m_fMaxDecspeed[5],m_fMinDecspeed[5];
	float   m_fMaxTrackSpeed[5],m_fMinTrackSpeed[5];
	BOOL    m_bSoftLimit;
	float   m_fPreCurpos;
	BOOL	m_bAxisInitia[5], m_bMotorFault[5], m_bCommunicationFault[5];	// ��ʼ���ã��ĸ��᲻�������Ͳ����ĸ���
	//float   m_fZHeight;
	BOOL	m_bMoveXY;
	int     m_nPhaseInit[5];				// ��ʼ���׶Σ���ʼ�� 0-����λ 1-����λ 2-��еԭ��
	BOOL	m_bDbClick;						// ˫��

	TMCM_Params_Speed			m_paramSpeed;
	TMCM_Params_Current			m_paramCurrent;
	TMCM_Params_Bl				m_paramBl;			// ��϶�������
	TMCM_Params_ZRes			m_paramZRes;		// Z��Լ���������
//	TMCM_Params_AxialAddr		m_paramAddr;		// ��վ��ַ
	TMCM_Params_LimitStatus		m_paramLimitStatus;	// ����,�ӵء���������
	TMCM_Params_Sys				m_paramSys;
	TMCM_Params_Elec			m_paramAxis;
	TMCM_Params_Style			m_paramStyle;		// �Ƿ����Ͽ�����
	TMCM_Params_DriveDir		m_paramDriveDir;    // ��������
	TMCM_Params_LimitParam		m_paramLimitParam;  // ��λ��������
	TMCM_Params_Fault			m_paramFault;		// ͨѶ����
	
	void    SetStatus(BOOL bEnable);
	CString DisplayOutput(short motor, float fValue, float fGoal);

	void    Connect();
	void	InitParam_SysParams();
	void    InitParam_Backlash( char* path );
	void    InitParam_Calibration( char* path );
	void	SyncParamsPos();
	void	SaveToINI( int nIndex );

	void	GetCurStep( short motor );				// ��ȡ��ǰλ��
	BOOL    QueryMoving();
	void    Stop();
	void	Grounding();
	void	GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 );

	void	FindIndex();
	void    DiffCenter(short motor);
	void    StartPos(short motor);
	void	StartPos_LimitDir( short motor);

	void	Zero();
	void	ZeroAxis( short motor );

	void    Home();
	void	HomeAxis( short motor );

	float	RDriveConversion(float fRCurrent, float fRTarget);

	void	MoveInput( short motor );				// �ֶ�������
	void	MoveMinusPlus( int nIndex );			// ������
	void	MoveContinues( int nIndex );			// ������
	void	MoveAxis( short motor, float fStep );	// ������
	void	MoveAxis_FromThird( short motor, float fStep );
	void    Move5Axis_FromThird( float fX, float fY, float fZ, float fR, float fT );
	void	Move5Axis(float fX, float fY, float fZ, float fR, float fT );
	void	Move5Axis_JudgeDirMove( short motor );
	void	Move2Axis(float fX, float fY);
	void	Move2Axis_FromThird( float fX, float fY );

	void	MoveSteps();							// �ƶ�����������
	void	MoveSteps_Axis( short motor );
	void	MoveSteps_JudgeDirMove( short motor );

	void	SaveUserOrigin();	// ���湤��ԭ�����λ��
	void	SetUserOrigin();	// �ص�ԭ��
	void	OnVentExit();		// �������˳�ʱ�Ĳ���

	void    SafeLimit(unsigned char ucMotor, int fState);
	void	Safe_Axis( unsigned char ucMotor, int fState, int nType );

	void	Timer_Axis( short motor );
	void	Timer_Stop( short motor );
	void	ToHome_XY( unsigned char ucMotor, short motor );
	void	ToHome_ZRT( unsigned char ucMotor, short motor );
	//void	ToHome_T( unsigned char ucMotor, short motor );
	void	NonToHome( unsigned char ucMotor, short motor );
	void	Timer_KillTimer( UINT nIDEvent );
	void	Timer_Tracklocus( UINT nIDEvent, short anxi );

	// ���������
	BOOL    m_bTrackState[5];
	//float   m_fTrackStep, m_fTrackStepR, m_fTrackStepT;
	float	m_fTrackStepAxis,m_fRockerStep[5];
	TMCM_Params_Speed   m_paramsTrackSpeed;
	void	SetTracklocusAxis1( short motor, int nSpeed );
	void	SetTracklocusAxis2( short motor, int nSpeed );
	void    SetTracklocus1( int nSpeedX, int nSpeedY, int nSpeedR );
	void    SetTracklocus2( int nSpeedT, int nSpeedZ);
	void	SetTracklocusAxis( short motor, BOOL bDirect );
	void    RestoreSpeedlocus();
	void    StopTracklocus();

	//��ҷ���
	void	SetTrackParamAxis( short motor, int nSpeed );
	void	SetTrackParam( int nSpeedX, int nSpeedY );
	void	StopTrack();
	void	StopTrackAxis1( short motor );
	void	StopTrackAxis2( short motor );
	void	RestoreSpeed();

	// Z��X/Y��Լ��
	float	m_factualLimitH[10],m_factualLimit[10];	// XY��ʵ���˶���Χ
	float   m_fSLimit[10];
	void	InitParam_ZRestraint( char* path );// Z��Լ��Restraint����ѧģ��
	void    CoherenceCheck_Z( float& fXExp, float& fYExp, float fZExp, BOOL bAfter );

	//�Ŵ������
	CSize	m_sizeReso;
	void	CalculateStageUScale( int nAxis,		// ���ĸ���
							 double	dScalePixels,	// ���س���
							 float	&nScale );		// ʵ���ƶ�����
	double	m_dMag;					// ��ǰ�Ŵ���
	double	m_dblPixelsPerMM_HORZ, m_dblPixelsPerMM_VERT;
	void	SetMag( double dMag );	// ���������õķŴ���
	void	MagChange();			// ���·Ŵ�����ز���
	void	CoherenceCheck_Mag();	// ���Ŵ�����Χ�Ȳ���
	void	SetResolution( CSize szReso );

	//λ�ô洢�й�
	void	List_SetHead( int nIndex, CString strName );	// ���ñ�ͷ
	void	List_Init();									// ����ʼ��
	void	List_Load();									// ���װ��
	void	List_ChangeName( int nItem, CString strName );	// ĳһλ�ø���
	void	List_Update();									// ������
	void	List_Goto( int nItem );							// ��ת��ĳһλ��

	CString	m_strPosListHead[6];		// ���ÿ�б�ǩ
	int		m_nPosTotal;				// �洢λ�õ�����
	int		m_nPosColWidth[6];			// ���ÿ�п��
	int		m_nPosEdit;					// ��������һ��
	ListTMCMPos*	m_plistTMCMPos;		// λ�ýṹ�б�

	// ���ػ����
	char	m_cPathLocalize[256];
	WORD	m_wLanguageID;				// ��������
	void	SetLanguageID( WORD wID );	// ���������õ���������
	void	Localize();					// ���ػ�
	CString	m_strMsg[20];	// char* m_szMsg[20];// �ַ������飨ָ��char���͵�ָ�����飩

	//��Ļ�������
	void	SetPixelsPerMM( double dX, double dY );
	void	MoveToCenter( int nCenterX, int nCenterY, int nX, int nY );

	//��־���
	HANDLE	m_hFileLog;
	int		m_nLoopsLog;
	void	InitLog();

	//��Χ�˶�
	void	AutoMove( float fstep, short motor );
	void	Timer_AutoMove(short motor);

// Dialog Data
	//{{AFX_DATA(CPanelStage)
	enum { IDD = IDD_DIALOG_Stage };
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
	afx_msg void OnBnClickedButtonListadd();
	afx_msg void OnBnClickedButtonListdel();
	afx_msg void OnBnClickedButtonListchange();
	afx_msg void OnBnClickedButtonSetuserorigin();
	afx_msg void OnBnClickedButtonSetup();
	afx_msg void OnBnClickedButtonLimitparam();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
	afx_msg void OnHomeChanged(UINT nID);
	afx_msg void OnZeroChanged(UINT nID);
	afx_msg void OnMoveMinusChanged(UINT nID);
	afx_msg void OnMovePlusChanged(UINT nID);
	afx_msg void OnMoveMinus2Changed(UINT nID);
	afx_msg void OnMovePlus2Changed(UINT nID);
	afx_msg LRESULT OnUpdateTMCMValue(WPARAM wParam, LPARAM lParam);
public:
	
};

#endif