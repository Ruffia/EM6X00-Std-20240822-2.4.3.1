// Last Modify : 2012.09.01

//{{AFX_INCLUDES()
#include "semcorectrlpanel_stand1.h"
#include "semcorectrlpanel_stand2.h"
#include "semcorectrlpanel_stand3.h"
#include "semcorectrlpanel_stand4.h"
#include "semcorectrlpanel_stand5.h"
#include "semcorectrlpanel_stand6.h"
#include "semcorectrlpanel_stand7.h"
#include "semcorectrlpanel_stand8.h"
#include "semcorectrlpanel_stand9.h"
#include "semcorectrlpanel_standA.h"
//}}AFX_INCLUDES

#if !defined(AFX_DLGIMPPANELDEBUG2_H__AE2E11A4_D109_44BD_A088_A7B692A95F10__INCLUDED_)
#define AFX_DLGIMPPANELDEBUG2_H__AE2E11A4_D109_44BD_A088_A7B692A95F10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImpPanelDebug2.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebug2 dialog

class CDlgImpPanelDebug2 : public CDialog
{
	CBitmap		m_bmpON;
	CBitmap		m_bmpOFF;
	HICON		m_hIcon1;
	HICON		m_hIcon2;
	HICON		m_hIcon3;
	HICON		m_hIcon4;
	CBrush		m_brushVacNotGood;
	CBrush		m_brushVacGood;

// Construction
public:
	CDlgImpPanelDebug2(CWnd* pParent = NULL);   // standard constructor
	~CDlgImpPanelDebug2();

	void	Localize(void);
	void	EnableCtrl( long lClassID, BOOL bEnable );
	void	UpdateAdjustor( UINT nAdjustorSort );
	void	UpdateManualAdjustor( UINT nID, UINT nFlag );

	void	UpdateReadbackData( unsigned short* pnData, DWORD& dwRegister );

	BOOL	m_bVacuumSoftAuto;
	CDialog*	m_pAutoProgressDlg;

	/////////////////////////////////////////////
	void	SPR_En( BOOL bEn );
	void	SPR_HVOn( BOOL bHVOn );

	void	HVPower_Init();
	void	HVPower_Reset();
	void	HVPower_SyncEnable( int nID, BOOL bEnable );
	void	HVPower_SetEnable( int nID, int nCheck );
	void	dlgprintf(int id, char *fmt, ... );

	int		m_nPosB, m_nPosC;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	BOOL	VAC_UpdatePneumaticDiagram();
	BOOL	VAC_UpdateUIswitches( BOOL bReset = FALSE );

	void	VAC_ReadComprs();
	void	VAC_Reset();
	BOOL	VAC_Init();
	void	VAC_SoftAuto( int nCheck );
	void	VAC_VacuumReady( BOOL bReady );
	void	VAC_VacuumReady2( BOOL bReady );
	void	VAC_SyncStates( int nIndex );
	int		m_nVACMsgType;

	void	SetupDebugParam( BOOL bDebugVac );

	/////////////////////////////////////////////
	BOOL	m_bV1status;
	BOOL	m_bSPRRelay1En, m_bHVOn;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	void	MCI_Init();
	void	MCI_Det45SB_Change( int nPos );
	void	MCI_Det45SBY_Change( int nPos );
	void	MCI_DetGainSB_Change( int nPos );
	void	MCI_DetGain2SB_Change( int nPos );
	void	MCI_BWselSB_Change( int nPos );
	void	MCI_SyncParams();
	void	MCI_SyncDet45( int nPosX, int nPosY );
	/////////////////////////////////////////////

	/////////////////////////////////////////////



	int		m_nApproxMag;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	void	ADCvconv();
	/////////////////////////////////////////////

	void	SetNeedMon( int nCheck );

	BOOL	m_bCCGBad;
	int		m_nCCG_Counts, m_nCCG_CountsBak;

// Dialog Data
	//{{AFX_DATA(CDlgImpPanelDebug2)
	enum { IDD = IDD_IMP_PANEL_DEBUG2 };
	CMacCheckBox	m_btnFakeMains;
	CMacCheckBox	m_btnINTtest;
	CMacCheckBox	m_btnVideo;
	CMacCheckBox	m_btnMainsLock;
	CMacCheckBox	m_btnSoftAuto;
	CMacCheckBox	m_btnChamberVent;
	CMacCheckBox	m_btnGunVent;
	CMacCheckBox	m_btnVacuumSwitch;
	CMacCheckBox	m_btnVacuumPower;
	CMacCheckBox	m_btnV1status;
	CMacButton	m_btnResetComm;
	CMacCheckBox	m_btnTestLEDs;
	CMacButton	m_btnHVOn;
	CMacButton	m_btnSPRRelay1En;
	CScrollBar	m_scrollDetGain;
	CScrollBar	m_scrollDetGain2;
	CScrollBar	m_scrollDet45;
	CScrollBar	m_scrollDet45Y;
	CScrollBar	m_scrollBWsel;
	CSemCoreCtrlPanel_Stand1	m_ctrlHV;
	CSemCoreCtrlPanel_StandA	m_ctrlWD;
	CSemCoreCtrlPanel_Stand9	m_ctrlMagnifier;
	CSemCoreCtrlPanel_Stand6	m_ctrlCondenserLens;
	CSemCoreCtrlPanel_Stand6	m_ctrlObjectiveLens;
	CSemCoreCtrlPanel_Stand1	m_ctrlBrightness;
	CSemCoreCtrlPanel_Stand1	m_ctrlContrast;
	CSemCoreCtrlPanel_Stand2	m_ctrlBeemAlignment;
	CSemCoreCtrlPanel_Stand2	m_ctrlScanShift;
	CSemCoreCtrlPanel_Stand3	m_ctrlScanRotate;
	CSemCoreCtrlPanel_Stand2	m_ctrlStigmation;
	CSemCoreCtrlPanel_Stand7	m_ctrlAuxiliaryStigmation;
	CSemCoreCtrlPanel_Stand1	m_ctrlDFocus;
	CSemCoreCtrlPanel_Stand7	m_ctrlDFocusSwitch;
	CSemCoreCtrlPanel_Stand1	m_ctrlCPS_HV;
	CSemCoreCtrlPanel_Stand1	m_ctrlCPS_Bias;
	CSemCoreCtrlPanel_Stand1	m_ctrlCPS_FilamentCurrent;
	CSemCoreCtrlPanel_Stand1	m_ctrlSignal2Brightness;
	CMacCheckBox	m_btnHV;
	CMacCheckBox	m_btnBias;
	CMacCheckBox	m_btnFilament;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImpPanelDebug2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImpPanelDebug2)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHVOn();
	afx_msg void OnSPRRelay1En();
	afx_msg void OnReadComprsCmd();
	afx_msg void OnCHECKReadVACmprsAuto();
	afx_msg void OnResetCommunicate();
	afx_msg void OnCHECKTestLEDs();
	afx_msg void OnClearAllChannels();
	afx_msg void OnCHECKV1status();
	afx_msg void OnVacuumGUNVENT();
	afx_msg void OnVacuumCHAMBERVENT();
	afx_msg void OnVacuumSoftAuto();
	afx_msg void OnCHECKMainsLock();
	afx_msg void OnCHECKVideo();
	afx_msg void OnCHECKStigCtrl();
	afx_msg void OnReadSelectedADC();
	afx_msg void OnCHECKReadSelectedADCAuto();
	afx_msg void OnCHECKNeedMon();
	afx_msg void OnCHECKDebugZ();
	afx_msg void OnWriteRateX();
	afx_msg void OnWriteRateY();
	afx_msg void OnBUTTONINTtest();
	afx_msg void OnBUTTONFakeMainslock();
	afx_msg void OnBUTTONMainsLockLineSync();
	afx_msg void OnBUTTONMainsLockFrameSync();
	afx_msg void OnCHECKFPGArelayEn11();
	afx_msg void OnCHECKCompCond();
	afx_msg void OnCHECKCompObj();
	afx_msg void OnCHECKCompAlignX();
	afx_msg void OnCHECKCompAlignY();
	afx_msg void OnCHECKDet2nd();
	afx_msg void OnCHECKDebugOsc();
	afx_msg void OnWriteOLctrl();
	afx_msg void OnVacuumVACUUMPOWER();
	afx_msg void OnVacuumVACUUMSWITCH();
	afx_msg void OnCHECKFPGArelayEn1();
	afx_msg void OnCHECKFPGArelayEn2();
	afx_msg void OnCHECKFPGArelayEn56();
	afx_msg void OnCHECKFPGArelayEn4();
	afx_msg void OnCHECKVacuumPauseRead();
	afx_msg void OnCHECKDebugModeCoadjust();
	afx_msg void OnCheckHV();
	afx_msg void OnCheckBias();
	afx_msg void OnCheckFilament();
	afx_msg void OnCheckOff();
	afx_msg void OnReLinkHVpower();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnWriteOneKey();
	afx_msg void OnReLinkCCG();
	afx_msg void OnCHECKVacuumTCf();
	afx_msg void OnCHECKVacuumTCc();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg void OnApproxMagChanged( UINT nID );
	afx_msg void OnAdjustorSortChanged(UINT nID);
	afx_msg void OnADCselectOBChanged( UINT nID );
	afx_msg void OnADCsChSelectChanged( UINT nID );
	afx_msg void OnDetPairSelect(UINT nID);
	afx_msg LRESULT OnSMvacChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSMvacTCAmpUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVACUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVACUpdateCCGValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603statusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603msgChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603paramChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603AutoParamChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPPANELDEBUG2_H__AE2E11A4_D109_44BD_A088_A7B692A95F10__INCLUDED_)
