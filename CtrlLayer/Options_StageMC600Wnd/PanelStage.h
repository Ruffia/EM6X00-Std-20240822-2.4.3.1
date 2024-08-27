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
	MC600_Params_Bl		m_paramBl;			// ��϶�������
	MC600_Params_ZRes	m_paramZRes;		// Z��Լ���������
	float				m_actualLimit[6];	// XYZ��ʵ���˶���Χ

	long	m_lSpeed[5], m_lSpeed2[5], m_lSpeed3[5];	// �ٶȣ����ٶȣ���ʼ�ٶ�
	double	m_dEquivalent[5];				// ������pp��������mm�Ļ���ϵ��
	long	m_lCurStep[5];					// ��ǰλ��
	UINT	m_nTimer[5], m_nTimerCounts[5];	// ��ʱ��
	float	m_fCali[5], m_fGoal[5];			// ɨ������λ��mm��Ŀ��λ��mm
	float	m_fZ0Wd;						// Z��������Ӧ�Ĺ�������ֵmm

	int		m_nPort, m_nPort2;	// ͨѶ�˿ں�
	BOOL	m_bIsMC600Ready;	// ͨѶ�˿��Ƿ��Ѵ�����
	BOOL	m_bIsMoving;		// �Ƿ������ƶ�
	BOOL	m_bRelative, m_bRelativeBak;	// �Ƿ�Ϊ����˶�
	BOOL	m_bWD;				// Z���Ƿ���ʾ��������
	int		m_nProcess, m_nMoveSteps;		// �ƶ��������ͣ�ĳһ�ƶ������еľ��岽��
	BOOL	m_bBlClick, m_bBlTrack;			// ˫���͸���ʱ�Ƿ�����϶
	int		m_nVentSel;						// ����ǰ��ѡ��
	BOOL	m_bOptR;						// R���Ƿ�ʹ�ù�翪��

	CRect	m_rcWindow, m_rcReport;	// ���ڳߴ�
	char	m_cPathLocalize[256];

	///////////////////////////////////
	// Z��X/Y��Լ����أ���Z > 10mm�Ľϴ�Zֵ��������XY������
	float	m_fLimit[255][5];		// 0ΪZ��1ΪX����2ΪX����3ΪY����4ΪY��
	int		m_nZLimitTotal;
	void	InitParam_ZRestraint( char* path );	// Z��Լ��Restraint����ѯ��
	void	InitParam_ZRestraint_Fixed( char* path );	// Z��Լ��Restraint����ѧģ�ͣ��̶��Ĳ���
	void	InitParam_ZRestraint_User( char* path );	// Z��Լ��Restraint����ѧģ�ͣ��û������õĲ���
	void	CoherenceCheck_Z();		// ���Z��X/Y��Լ������
	void	CoherenceCheck_Z2( float& fXExp, float& fYExp, float fZExp, BOOL bAfter );	// ���Z��X/Y��Լ������
	void	SetZLimit();
	///////////////////////////////////

	///////////////////////////////////
	// ��־���
	HANDLE	m_hFileLog;
	int		m_nLoopsLog;
	void	InitLog();
	///////////////////////////////////

	///////////////////////////////////
	// ��δʹ�õ�ԭMC300�ı���
	long	m_lStepBase;	// �������
	double	m_dStepRatio;	// ����ϵ������
	int		m_nSpeedRatio;	// �ٶ�ϵ������
	int		m_nHomeState;	// ���㲽��
	int		m_nSpeed[5];	// �ٶȵ�λ0,1,2,3,4,5,6,7
	///////////////////////////////////

	///////////////////////////////////
	// �Ŵ������
	CSize	m_sizeReso;
	void	CalculateStageUScale( int nAxis,		// ���ĸ���
							 double	dScalePixels,	// ���س���
							 int	&nScale );		// ʵ���ƶ�����
	void	CoherenceCheck_Mag();	// ���Ŵ�����Χ�Ȳ���
	void	MagChange();			// ���·Ŵ�����ز���
	void	SetMag( double dMag );	// ���������õķŴ���
	void	SetResolution( CSize szReso );
	double	m_dMag;					// ��ǰ�Ŵ���
	double	m_dblPixelsPerMM_HORZ, m_dblPixelsPerMM_VERT;
	///////////////////////////////////

	///////////////////////////////////
	// ��Ļ�������
	void	SetPixelsPerMM( double dX, double dY );
	void	MoveToCenter( int nCenterX, int nCenterY, int nX, int nY );
	///////////////////////////////////

	///////////////////////////////////
	// λ�ô洢���
	void	List_SetHead( int nIndex, CString strName );	// ���ñ�ͷ
	void	List_Init();									// ����ʼ��
	void	List_Load();									// ���װ��
	void	List_ChangeName( int nItem, CString strName );	// ĳһλ�ø���
	void	List_Update();									// ������
	void	List_Goto( int nItem );							// ��ת��ĳһλ��

	CString	m_strPosListHead[6];		// ���ÿ�б�ǩ
	int		m_nPosColWidth[6];			// ���ÿ�п��
	int		m_nPosEdit;					// ��������һ��
	int		m_nPosTotal;				// �洢λ�õ�����
	ListMC600Pos*	m_plistMC600Pos;	// λ�ýṹ�б�
	///////////////////////////////////

	///////////////////////////////////
	// ���ػ����
	WORD	m_wLanguageID;				// ��������
	void	SetLanguageID( WORD wID );	// ���������õ���������
	void	Localize();					// ���ػ�
	CString	m_strMsg[10];	// char* m_szMsg[10];// �ַ������飨ָ��char���͵�ָ�����飩
	///////////////////////////////////

	void	SetParam( int nIndex, double dParam );
	void	GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 );
	BOOL	m_bPEswitch;

	///////////////////////////////////
	// T ��
	CCommVince	m_Vince;
	Vince_Param_Status	GetStatus( short anxi );
	BOOL	m_bIsVinceReady;	// ͨѶ�˿��Ƿ��Ѵ�����
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
