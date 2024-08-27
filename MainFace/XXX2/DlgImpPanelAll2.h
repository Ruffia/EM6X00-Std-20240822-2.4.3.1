// Last Modify : 2011.04.15

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

#if !defined(AFX_DLGIMPPANELALL2_H__990B9136_F3F7_4E5B_A8EC_B89E2AEE5FD9__INCLUDED_)
#define AFX_DLGIMPPANELALL2_H__990B9136_F3F7_4E5B_A8EC_B89E2AEE5FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImpPanelAll2.h : header file
//

#include "MacButtons.h"
#include "TransparentImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelAll2 dialog

class CDlgImpPanelAll2 : public CDialog
{
	CBitmap		m_bmpON, m_bmpOFF, m_bmpERR;
	CBitmap		m_bmpPneumaticDiagram;
	HICON		m_hIcon1, m_hIcon2, m_hIcon3, m_hIcon4, m_hIcon5;
	CBrush		m_brushVacNotGood, m_brushVacGood, m_brushVacErr;

// Construction
public:
	CDlgImpPanelAll2(CWnd* pParent = NULL);   // standard constructor
	~CDlgImpPanelAll2();

	void	AdjustCtrlsPos();
	void	AdjustCtrlsPos_ValveLamp( int nID, int nLeftPD, int nTopPD, int nWidthPD, int nHeightPD );
	CRect	GetControlRect( int nID );

	void	UpdateAdjustor( UINT nAdjustorSort );
	void	UpdateManualAdjustor( UINT nID, UINT nFlag );
	void	updateStatusV1(bool status);
	/////////////////////////////////////////////
	void	VAC_Reset();
	BOOL	VAC_Init();

	BOOL	VAC_UpdatePneumaticDiagram();
	void	VAC_UpdatePneumaticDiagram_Old();
	void	VAC_UpdatePneumaticDiagram_New();
	void	VAC_UpdatePneumaticDiagram_PLC();

	BOOL	VAC_UpdateUIswitches( BOOL bReset = FALSE );
	void	VAC_UpdateUIswitches_Old( BOOL bReset = FALSE );
	void	VAC_UpdateUIswitches_New();
	void	VAC_UpdateUIswitches_PLC();

	void	VAC_VacuumReady( BOOL bReady );
	void	VAC_VacuumReady2( BOOL bReady );
	void	VAC_Vent( BOOL bGun );
	void	VAC_CheckV1( int nCheck );

	int		m_nVACMsgType;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	void	SPR_En( BOOL bEn );
	void	SPR_HVOn( BOOL bHVOn );
	void	SPR_Fan( BOOL bFan );

	BOOL	m_bSPRRelay1En, m_bHVOn, m_bNavPowerOn;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	void	ShowCameraWindow();
	void	SyncCCDSwitch( BOOL bOn );
	/////////////////////////////////////////////

	void	HVPower_Shutdownbeam();
	void	HVPower_Clear();
	void	HVPower_Init( BOOL bForce = FALSE );
	void	HVPower_Reset();
	void	HVPower_UpdateStatus();
	void	HVPower_SyncEnable( int nID, BOOL bEnable );
	void	HVPower_SetEnable( int nID, int nCheck );
	void	dlgprintf(int id, char *fmt, ... );

	void	ChangeHighVoltage_3603(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bNotify, BOOL bSync, BOOL bSet);
	void	ChangeHighVoltage_771(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bNotify, BOOL bSync, BOOL bSet);
	void	ChangeFilament_3603(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet);
	void	ChangeFilament_771(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet);
	void	ChangeBeam_771(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet);
	void	ChangeBias_3603(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet);
	void	ChangePMT_771(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync);
	void	ChangeHighVoltage_WSM30KV(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bNotify, BOOL bSync, BOOL bSet);
	void	ChangeFilament_WSM30KV(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet);
	void	ChangeBeam_WSM30KV(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet);
	void	ChangePMT_WSM30KV(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync);
	void	ChangeBias_WSM30KV2(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet);
	void	ChangeBias_WSM30KV2_Max();
	void	ChangeBias_WSM30KV2_AutoAdjust( int nIDEvent );

	void	WSM30KV_SetParam( WPARAM wParam, LPARAM lParam );
	void	WSM30KV2_SetParam( WPARAM wParam, LPARAM lParam );

	void	OneKEY_LastStep();
	void	VacuumVACUUMSWITCH_HVCheck(int &nCheck);
	void	VacuumVACUUMSWITCH_LoadData();
	void	VacuumVACUUMSWITCH_Cancel();

	int		m_nPosB, m_nPosC;
	BOOL	m_bCCGBad;
	int		m_nCCG_Counts, m_nCCG_CountsBak;

	void	HV_UpdateStatusSwitches( BYTE bSSbak );
	
	void	UpdateStatus_BoardCCG();
	void	UpdateStatus_BoardTCs(WPARAM wParam, LPARAM lParam);
	
	int		m_nHIstate;

	// 导航
	int		m_nNavProcess;
	NAV_Position	m_navPosStart;
	NAV_Position5	m_navPosOrigin;
	void	VAC_VentPump_StageNAV( BOOL bVent );
	void	VAC_VentPump_StageNAV_Prepare();

	// PLC真空控制板
	BYTE	PLC_UpdateStatus();
	void	PLC_UpdateCCGValue();

// Dialog Data
	//{{AFX_DATA(CDlgImpPanelAll2)
	enum { IDD = IDD_IMP_PANEL_ALL2 };
	CTransparentImage	m_staticPneumaticDiagram;
	CMacCheckBox	m_btnSoftAuto;
	CSemCoreCtrlPanel_StandA	m_ctrlWD;
	CSemCoreCtrlPanel_Stand6	m_ctrlCondenserLens;
	CSemCoreCtrlPanel_Stand6	m_ctrlObjectiveLens;
	CSemCoreCtrlPanel_Stand1	m_ctrlBrightness;
	CSemCoreCtrlPanel_Stand1	m_ctrlContrast;
	CSemCoreCtrlPanel_Stand9	m_ctrlMagnifier;
	CSemCoreCtrlPanel_Stand2	m_ctrlBeemAlignment;
	CSemCoreCtrlPanel_Stand2	m_ctrlScanShift;
	CSemCoreCtrlPanel_Stand3	m_ctrlScanRotate;
	CSemCoreCtrlPanel_Stand2	m_ctrlStigmation;
	CSemCoreCtrlPanel_Stand7	m_ctrlAuxiliaryStigmation;
	CSemCoreCtrlPanel_Stand1	m_ctrlHV;
	CSemCoreCtrlPanel_Stand1	m_ctrlBias;
	CSemCoreCtrlPanel_Stand1	m_ctrlHeater;
	CSemCoreCtrlPanel_Stand1	m_ctrlSignal2Brightness;
	CSemCoreCtrlPanel_Stand1	m_ctrlWSMBias;
	CMacCheckBox	m_btnVacuumStandby;
	CMacCheckBox	m_btnChamberVent;
	CMacCheckBox	m_btnGunVent;
	CMacCheckBox	m_btnVacuumSwitch;
	CMacCheckBox	m_btnVacuumPower;
	CMacCheckBox	m_btnV1;
	CMacCheckBox	m_btnHV;
	CMacCheckBox	m_btnBias;
	CMacCheckBox	m_btnFilament;
	CMacCheckBox	m_btnCCD;
	//}}AFX_DATA

// Attributes
public:
	CDialog*	m_pAutoProgressDlg;
// Operations
public:
//	CScanDoc* GetDocument();
	void	Message(LPCTSTR lpszMessage);
	void	Message2(LPCTSTR lpszMessage);
	void	Localize(void);

	int		m_nRBwdCnt;
	int		m_nBoardReadDataBuf[256];

	BOOL	m_bVacuumSoftAuto;

	// 推拉杆 2022.12
	unsigned short	m_nStagePodState[3];
	short	m_nStagePodSetTrack1[3], m_nStagePodSetTrack2[3];
	void	StagePodInit();
	void	StagePodTrack(short nIndex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImpPanelAll2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT		m_nCPSTimer;

	// Generated message map functions
	//{{AFX_MSG(CDlgImpPanelAll2)
	virtual BOOL OnInitDialog();

	void	RegisterCtrl();
	void	RegisterControls_CPS();
	void	RegisterControls_SPM();
	void	RegisterControls_WSM();

	afx_msg void OnDestroy();
	afx_msg void OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void OnReportAction(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnVacuumVACUUMPOWER();
	afx_msg void OnVacuumVACUUMSWITCH();
	afx_msg void OnVacuumGUNVENT();
	afx_msg void OnVacuumCHAMBERVENT();
	afx_msg void OnVacuumVACUUMSTANDBY();
	afx_msg void OnCheckHV();
	afx_msg void OnCheckBias();
	afx_msg void OnCheckFilament();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnVacuumSoftAuto();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckOff();
	afx_msg void OnBUTTONMon();
	afx_msg void OnCHECKV1();
	afx_msg void OnBUTTONShowCameraImage();
	afx_msg void OnRotateWrite();
	afx_msg void OnRotateDebug();
	afx_msg void OnBUTTONNavigate();
	afx_msg void OnBnClickedBtnSetData();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg void OnAdjustorSortChanged(UINT nID);
	afx_msg LRESULT OnSMvacChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSMvacTCAmpUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVACUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVACUpdateCCGValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVACUpdateIPGValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBoardVACUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePLCvacBrdStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603statusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603msgChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603paramChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603AutoParamChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanStatusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanParamChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanParam2Changed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanParam3Changed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanAutoParamChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWSM30KVSetParam(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWSM30KVUpdateMon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWSM30KVUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCCDmsgChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBSDmsgChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnControlPanelDataRecieved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateStagePod(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNAVUpdate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPPANELALL2_H__990B9136_F3F7_4E5B_A8EC_B89E2AEE5FD9__INCLUDED_)
