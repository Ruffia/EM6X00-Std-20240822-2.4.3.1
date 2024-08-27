// DlgImpPanelDebug2.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgImpPanelDebug2.h"
#include "DlgPrompt.h"
#include "DlgProgress.h"
#include "MainFrm.h"
#include "ScanView.h"
#include "ScanParameterManager.h"
#include "DlgInput.h"
#include "AppLogManager.h"
#include "CommonFunctionManager.h"
#include "CFGFileManager.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "BoardComm.h"
#include "ScaleManager.h"
#include "CCGStatusDetector.h"
#include "CCPS3603tatusDetector.h"
#include "VACDataManager.h"
#include "HVPowerDataManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "AutoFocusThread.h"
#include "Util.h"
#include "SEMLog/AlarmManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebug2 dialog


CDlgImpPanelDebug2::CDlgImpPanelDebug2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImpPanelDebug2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImpPanelDebug2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bVacuumSoftAuto		= TRUE;
	m_pAutoProgressDlg		= NULL;

	m_bmpON.LoadBitmap(IDB_BITMAP_LED_RED_H);
	m_bmpOFF.LoadBitmap(IDB_BITMAP_LED_RED_D);
	m_hIcon1				= NULL;
	m_hIcon2				= NULL;
	m_hIcon3				= NULL;
	m_hIcon4				= NULL;

	m_nApproxMag	= -1;
	m_nPosB	=	m_nPosC		= 0;
	m_nVACMsgType	= -1;
}

CDlgImpPanelDebug2::~CDlgImpPanelDebug2()
{
	m_brushVacNotGood.DeleteObject();
	m_brushVacGood.DeleteObject();
}


void CDlgImpPanelDebug2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImpPanelDebug2)
	DDX_Control(pDX, IDC_BUTTON_Fake_Mainslock, m_btnFakeMains);
	DDX_Control(pDX, IDC_BUTTON_INT_test, m_btnINTtest);
	DDX_Control(pDX, IDC_CHECK_Video, m_btnVideo);
	DDX_Control(pDX, IDC_CHECK_MainsLock, m_btnMainsLock);
	DDX_Control(pDX, IDC_CHECK_Vacuum_Soft_Auto, m_btnSoftAuto);
	DDX_Control(pDX, IDC_CHECK_Vacuum_CHAMBER_VENT, m_btnChamberVent);
	DDX_Control(pDX, IDC_CHECK_Vacuum_GUN_VENT, m_btnGunVent);
	DDX_Control(pDX, IDC_CHECK_Vacuum_VACUUM_SWITCH, m_btnVacuumSwitch);
	DDX_Control(pDX, IDC_CHECK_Vacuum_VACUUM_POWER, m_btnVacuumPower);
	DDX_Control(pDX, IDC_CHECK_V1status, m_btnV1status);
	DDX_Control(pDX, IDC_BUTTON_Reset_Communicate, m_btnResetComm);
	DDX_Control(pDX, IDC_CHECK_TestLEDs, m_btnTestLEDs);
	DDX_Control(pDX, IDC_BUTTON_HVOnCmd, m_btnHVOn);
	DDX_Control(pDX, IDC_BUTTON_Relay1EnCmd, m_btnSPRRelay1En);
	DDX_Control(pDX, IDC_SCROLLBAR_DetGain, m_scrollDetGain);
	DDX_Control(pDX, IDC_SCROLLBAR_DetGain2, m_scrollDetGain2);
	DDX_Control(pDX, IDC_SCROLLBAR_Det45, m_scrollDet45);
	DDX_Control(pDX, IDC_SCROLLBAR_Det45Y, m_scrollDet45Y);
	DDX_Control(pDX, IDC_SCROLLBAR_BW_sel, m_scrollBWsel);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_HV, m_ctrlHV);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTANDACTRL_WD, m_ctrlWD);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND9CTRL_Magnifier, m_ctrlMagnifier);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND6CTRL_CondenserLens, m_ctrlCondenserLens);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens, m_ctrlObjectiveLens);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_Brightness, m_ctrlBrightness);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_Contrast, m_ctrlContrast);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND2CTRL_BeamAlign, m_ctrlBeemAlignment);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND2CTRL_ScanShift, m_ctrlScanShift);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND3CTRL_ScanRotate, m_ctrlScanRotate);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND2CTRL_Stig, m_ctrlStigmation);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_AuxStig, m_ctrlAuxiliaryStigmation);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_DFocus, m_ctrlDFocus);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_DFocus, m_ctrlDFocusSwitch);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV, m_ctrlCPS_HV);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias, m_ctrlCPS_Bias);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent, m_ctrlCPS_FilamentCurrent);
	DDX_Control(pDX, IDC_CHECK_IMP_PANEL_HV, m_btnHV);
	DDX_Control(pDX, IDC_CHECK_IMP_PANEL_Bias, m_btnBias);
	DDX_Control(pDX, IDC_CHECK_IMP_PANEL_Filament, m_btnFilament);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness, m_ctrlSignal2Brightness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImpPanelDebug2, CDialog)
	//{{AFX_MSG_MAP(CDlgImpPanelDebug2)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_HVOnCmd, OnHVOn)
	ON_BN_CLICKED(IDC_BUTTON_Relay1EnCmd, OnSPRRelay1En)
	ON_BN_CLICKED(IDC_BUTTON_ReadComprsCmd, OnReadComprsCmd)
	ON_BN_CLICKED(IDC_CHECK_ReadVACmprsAuto, OnCHECKReadVACmprsAuto)
	ON_BN_CLICKED(IDC_BUTTON_Reset_Communicate, OnResetCommunicate)
	ON_BN_CLICKED(IDC_CHECK_TestLEDs, OnCHECKTestLEDs)
	ON_BN_CLICKED(IDC_BUTTON_Clear_All_Channels, OnClearAllChannels)
	ON_BN_CLICKED(IDC_CHECK_V1status, OnCHECKV1status)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_GUN_VENT, OnVacuumGUNVENT)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_CHAMBER_VENT, OnVacuumCHAMBERVENT)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_Soft_Auto, OnVacuumSoftAuto)
	ON_BN_CLICKED(IDC_CHECK_MainsLock, OnCHECKMainsLock)
	ON_BN_CLICKED(IDC_CHECK_Video, OnCHECKVideo)
	ON_BN_CLICKED(IDC_CHECK_Stig_Ctrl, OnCHECKStigCtrl)
	ON_BN_CLICKED(IDC_BUTTON_ReadSelectedADCcmd, OnReadSelectedADC)
	ON_BN_CLICKED(IDC_CHECK_ReadSelectedADCAuto, OnCHECKReadSelectedADCAuto)
	ON_BN_CLICKED(IDC_CHECK_NeedMon, OnCHECKNeedMon)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Z, OnCHECKDebugZ)
	ON_BN_CLICKED(IDC_BUTTON_WriteRateX, OnWriteRateX)
	ON_BN_CLICKED(IDC_BUTTON_WriteRateY, OnWriteRateY)
	ON_BN_CLICKED(IDC_BUTTON_INT_test, OnBUTTONINTtest)
	ON_BN_CLICKED(IDC_BUTTON_Fake_Mainslock, OnBUTTONFakeMainslock)
	ON_BN_CLICKED(IDC_BUTTON_MainsLock_LineSync, OnBUTTONMainsLockLineSync)
	ON_BN_CLICKED(IDC_BUTTON_MainsLock_FrameSync, OnBUTTONMainsLockFrameSync)
	ON_BN_CLICKED(IDC_CHECK_FPGArelayEn11, OnCHECKFPGArelayEn11)
	ON_BN_CLICKED(IDC_CHECK_Comp_Cond, OnCHECKCompCond)
	ON_BN_CLICKED(IDC_CHECK_Comp_Obj, OnCHECKCompObj)
	ON_BN_CLICKED(IDC_CHECK_Comp_AlignX, OnCHECKCompAlignX)
	ON_BN_CLICKED(IDC_CHECK_Comp_AlignY, OnCHECKCompAlignY)
	ON_BN_CLICKED(IDC_CHECK_Det2nd, OnCHECKDet2nd)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Osc, OnCHECKDebugOsc)
	ON_BN_CLICKED(IDC_BUTTON_WriteOLctrl, OnWriteOLctrl)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_VACUUM_POWER, OnVacuumVACUUMPOWER)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_VACUUM_SWITCH, OnVacuumVACUUMSWITCH)
	ON_BN_CLICKED(IDC_CHECK_FPGArelayEn1, OnCHECKFPGArelayEn1)
	ON_BN_CLICKED(IDC_CHECK_FPGArelayEn2, OnCHECKFPGArelayEn2)
	ON_BN_CLICKED(IDC_CHECK_FPGArelayEn5, OnCHECKFPGArelayEn56)
	ON_BN_CLICKED(IDC_CHECK_FPGArelayEn4, OnCHECKFPGArelayEn4)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_PauseRead, OnCHECKVacuumPauseRead)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Coadjust, OnCHECKDebugModeCoadjust)
	ON_BN_CLICKED(IDC_CHECK_IMP_PANEL_HV, OnCheckHV)
	ON_BN_CLICKED(IDC_CHECK_IMP_PANEL_Bias, OnCheckBias)
	ON_BN_CLICKED(IDC_CHECK_IMP_PANEL_Filament, OnCheckFilament)
	ON_BN_CLICKED(IDC_CHECK_OFF, OnCheckOff)
	ON_BN_CLICKED(IDC_BUTTON_ReLink_HVpower, OnReLinkHVpower)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_WriteOneKey, OnWriteOneKey)
	ON_BN_CLICKED(IDC_BUTTON_ReLink_CCG, OnReLinkCCG)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_TCf, OnCHECKVacuumTCf)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_TCc, OnCHECKVacuumTCc)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_Approx_Mag_10, IDC_RADIO_Approx_Mag_10000, OnApproxMagChanged)
	ON_COMMAND_RANGE(ID_ADJUSTOR_BEAM_ALIGNMENT, ID_ADJUSTOR_STIGMATOR, OnAdjustorSortChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCselectOB0, IDC_RADIO_ADCselectOB5, OnADCselectOBChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCsChSelect0, IDC_RADIO_ADCsChSelect15, OnADCsChSelectChanged)
	ON_COMMAND_RANGE(IDC_RADIO_DetPair_Sel_01, IDC_RADIO_DetPair_Sel_45, OnDetPairSelect)
	ON_MESSAGE(WM_USER_VAC_MSG, OnSMvacChanged)
	ON_MESSAGE(WM_USER_VAC_TCAmp, OnSMvacTCAmpUpdate)
	ON_MESSAGE(WM_USER_VAC_UpdateStatus, OnVACUpdateStatus)
	ON_MESSAGE(WM_USER_VAC_UpdateCCGValue, OnVACUpdateCCGValue)
	ON_MESSAGE(WM_USER_CPS_Status, OnCPS3603statusChanged)
	ON_MESSAGE(WM_USER_CPS_MSG, OnCPS3603msgChanged)
	ON_MESSAGE(WM_USER_CPS_Param, OnCPS3603paramChanged)
	ON_MESSAGE(WM_USER_CPS_AutoParam, OnCPS3603AutoParamChanged)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgImpPanelDebug2, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgImpPanelDebug2)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND1CTRL_HV, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTANDACTRL_WD, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND9CTRL_Magnifier, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND6CTRL_CondenserLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND1CTRL_Brightness, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND1CTRL_Contrast, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND2CTRL_BeamAlign, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND2CTRL_ScanShift, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND3CTRL_ScanRotate, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND2CTRL_Stig, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND7CTRL_AuxStig, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND1CTRL_DFocus, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND7CTRL_DFocus, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebug2, IDC_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebug2 message handlers

void CDlgImpPanelDebug2::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	// Column
	VARIANT	ResourceID;
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE;
	m_ctrlCondenserLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Coarse)); 
	m_ctrlObjectiveLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Coarse)); 
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE;
	m_ctrlCondenserLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Fine)); 
	m_ctrlObjectiveLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Fine)); 
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY;
	m_ctrlCondenserLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Tiny)); 
	m_ctrlObjectiveLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Tiny)); 

	m_ctrlHV.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_HV)); 
	m_ctrlCondenserLens.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_CondenserLens)); 
	m_ctrlObjectiveLens.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens)); 

	// Detector
	m_ctrlBrightness.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Brightness)); 
	m_ctrlContrast.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Contrast)); 
	m_ctrlSignal2Brightness.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness)); 

	// Image
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_COARSE;
	m_ctrlScanRotate.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND3CTRL_Coarse)); 
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_FINE;
	m_ctrlScanRotate.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND3CTRL_Fine)); 
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_TINY;
	m_ctrlScanRotate.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND3CTRL_Tiny)); 

	m_ctrlMagnifier.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND9CTRL_Magnifier)); 
	m_ctrlWD.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTANDACTRL_WD)); 
	m_ctrlBeemAlignment.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND2CTRL_BeamAlign)); 
	m_ctrlScanShift.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND2CTRL_ScanShift)); 
	m_ctrlStigmation.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND2CTRL_Stig)); 
	m_ctrlScanRotate.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND3CTRL_ScanRotate)); 
	m_ctrlAuxiliaryStigmation.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND7CTRL_AuxStig)); 

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	Flag.lVal = 1;
	ClassID = m_ctrlAuxiliaryStigmation.GetPanelClassID();
	SerialNumber = m_ctrlAuxiliaryStigmation.GetPanelSerialNumber();
	CSemCtrlPanelManager::Instance().SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );

	// DF
//	m_ctrlDFocus.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_DFocus)); 
//	m_ctrlDFocusSwitch.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND7CTRL_DFocus)); 
	ClassID = m_ctrlDFocusSwitch.GetPanelClassID();
	SerialNumber = m_ctrlDFocusSwitch.GetPanelSerialNumber();
	CSemCtrlPanelManager::Instance().SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );

	// CPS 3601
	GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_HV)); 
	GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias)); 
	GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent)); 

	GetDlgItem(IDC_CHECK_Vacuum_VACUUM_POWER)->SetWindowText(GetResString(IDS_CHECK_Vacuum_VACUUM_POWER)); 
	GetDlgItem(IDC_CHECK_Vacuum_VACUUM_SWITCH)->SetWindowText(GetResString(IDS_CHECK_Vacuum_VACUUM_SWITCH)); 
	GetDlgItem(IDC_CHECK_Vacuum_GUN_VENT)->SetWindowText(GetResString(IDS_CHECK_Vacuum_GUN_VENT)); 
	GetDlgItem(IDC_CHECK_Vacuum_CHAMBER_VENT)->SetWindowText(GetResString(IDS_CHECK_Vacuum_CHAMBER_VENT)); 
	GetDlgItem(IDC_CHECK_OFF)->SetWindowText(GetResString(IDS_CHECK_OFF)); 
	GetDlgItem(IDC_BUTTON_ReLink_HVpower)->SetWindowText(GetResString(IDS_BUTTON_ReLink_HVpower)); 
	GetDlgItem(IDC_STATIC_STATUS_Vacuum)->SetWindowText(GetResString(IDS_STATIC_STATUS_Vacuum)); 
}

BOOL CDlgImpPanelDebug2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	///////////////////////////////////////////////////////////////////////////
	//注册控件
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlHV, SemControlPanelClass_HV );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlWD, SemControlPanelClass_WorkDistance );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlCondenserLens, SemControlPanelClass_CondenserLens );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlObjectiveLens, SemControlPanelClass_ObjectiveLens );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlBrightness, SemControlPanelClass_Brightness );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlContrast, SemControlPanelClass_Contrast );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlSignal2Brightness, SemControlPanelClass_Signal2Brightness );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlMagnifier, SemControlPanelClass_Magnifier );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlBeemAlignment, SemControlPanelClass_BeemAlignment );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlScanShift, SemControlPanelClass_ScanShift );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlStigmation, SemControlPanelClass_Stigmator );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlScanRotate, SemControlPanelClass_ScanRotate );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlAuxiliaryStigmation, SemControlPanelClass_AuxiliaryStigmation );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDFocus, SemControlPanelClass_DFocus );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDFocusSwitch, SemControlPanelClass_DFocusSwitch );
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	m_btnResetComm.SetColor( g_crON_Test );
	// HV
	m_bV1status = m_bSPRRelay1En = m_bHVOn = FALSE;
	GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_HVOnCmd )->EnableWindow( FALSE );
	m_btnSPRRelay1En.SetColor( g_crOFF );
	m_btnHVOn.SetColor( g_crOFF );

	VARIANT Xmin, Xmax, Ymin, Ymax, Xpos, Ypos, Enabled, Flag, ClassID, SerialNumber;
	ClassID.lVal		= SemControlPanelClass_HV;
	SerialNumber.lVal	= -1;
	Enabled.lVal		= 0;
	Flag.lVal			= 3;
	CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	ClassID.lVal		= SemControlPanelClass_Contrast;
	CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	// HV

	// Vacuum
	if( m_bVacuumSoftAuto )
	{
		m_btnSoftAuto.SetColor( RGB(0,0,255) );
		m_btnSoftAuto.SetWindowText( "Soft Auto" );
	}
	else
	{
		m_btnSoftAuto.SetColor( g_crOFF );
//		m_btnSoftAuto.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnSoftAuto.SetWindowText( "Soft Man" );
	}
	m_btnSoftAuto.EnableWindow( FALSE );
	m_btnVacuumSwitch.EnableWindow( m_bVacuumSoftAuto );
	m_btnGunVent.EnableWindow( m_bVacuumSoftAuto );
	m_btnChamberVent.EnableWindow( m_bVacuumSoftAuto );
	for( int i=0; i<6; i++ )
		GetDlgItem( IDC_CHECK_FPGArelayEn0 +i )->EnableWindow(  !m_bVacuumSoftAuto );
	GetDlgItem( IDC_CHECK_Vacuum_TCf )->EnableWindow( !m_bVacuumSoftAuto );
	GetDlgItem( IDC_CHECK_Vacuum_TCc )->EnableWindow( !m_bVacuumSoftAuto );
	///////////////////////////////////////////////////////
	// Vacuum

	// MCI
	CString str;
	switch( CControlLayer::Instance().m_BrdMCI.nDetPair )
	{
	case	0:
		((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->SetCheck(1);
		break;
	case	1:
		((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_23 )))->SetCheck(1);
		break;
	case	2:
		((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_45 )))->SetCheck(1);
		break;
	}
	m_scrollDet45.SetScrollRange( 0, 4095 );
	m_scrollDet45.SetScrollPos( 2047 );
	GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( "7FF" );
	m_scrollDet45Y.SetScrollRange( 0, 4095 );
	m_scrollDet45Y.SetScrollPos( 2047 );
	GetDlgItem( IDC_EDIT_Det45Y )->SetWindowText( "7FF" );

	m_scrollDetGain.SetScrollRange( 0, 7 );
	m_scrollDetGain.SetScrollPos( 0 );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( "0" );
	m_scrollDetGain2.SetScrollRange( 0, 7 );
	m_scrollDetGain2.SetScrollPos( 0 );
	GetDlgItem( IDC_EDIT_DetGain2 )->SetWindowText( "0" );
	m_scrollBWsel.SetScrollRange( 0, 7 );
	m_scrollBWsel.SetScrollPos( 0 );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( "0" );

	m_btnMainsLock.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnMainsLock.SetWindowText( "MainsLock OFF" );
	m_btnVideo.SetColor( g_crEmphasis2 );
	m_btnVideo.SetWindowText( "Video ON" );
	m_btnVideo.SetCheck( 1 );

	m_hIcon1 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_BEAM_ALIGN));
	m_hIcon2 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_MOVE));
	m_hIcon3 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_ROTATE));
	m_hIcon4 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_STIG));
	((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_ALIGNMENT ) ))->SetIcon( m_hIcon1 );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_SHIFT ) ))->SetIcon( m_hIcon2 );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_SHIFT ) ))->SetCheck( TRUE );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_ROTATE ) ))->SetIcon( m_hIcon3 );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_STIGMATOR ) ))->SetIcon( m_hIcon4 );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_STIGMATOR ) ))->SetCheck( TRUE );
	// MCI

	((CButton*)(GetDlgItem( IDC_CHECK_NeedMon )))->SetCheck(1);

	// CPS 3601
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlCPS_HV, SemControlPanelClass_CPS_HV );

	CString strX, strY;
	Xpos.lVal	= 0;
	Flag.ulVal	= (DWORD)0x01;

	// Bias
	Xmin.lVal	= 0;
	Xmax.lVal	= 500;		// 500V
	Ymin		= Xmin;
	Ymax		= Xmax;
	m_ctrlCPS_Bias.SetRange( Xmin, Xmax, Ymin, Ymax );
	m_ctrlCPS_Bias.SyncPos( Xpos, Ypos, Flag );
	strX.Format( "%.1f V", Xpos.lVal );
	m_ctrlCPS_Bias.SyncText( strX, strY, Flag );
	ClassID.lVal		= SemControlPanelClass_CPS_Bias;
	SerialNumber.lVal	= 201;
	m_ctrlCPS_Bias.Bind( ClassID, SerialNumber );
	// Filamnet
	Xmin.lVal	= 0;
	Xmax.lVal	= 300;		// 3A
	Ymin		= Xmin;
	Ymax		= Xmax;
	m_ctrlCPS_FilamentCurrent.SetRange( Xmin, Xmax, Ymin, Ymax );
	m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );
	strX.Format( "%.3f A", Xpos.lVal );
	m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );
	ClassID.lVal		= SemControlPanelClass_CPS_FilamentCurrent;
	SerialNumber.lVal	= 202;
	m_ctrlCPS_FilamentCurrent.Bind( ClassID, SerialNumber );

	VARIANT Enable;
	Flag.lVal	= 3;
	Enable.lVal	= 0;
	m_ctrlCPS_HV.Enable(Enable, Flag);
	Enable.lVal	= 0;
	m_ctrlCPS_Bias.Enable(Enable, Flag);
	Enable.lVal	= 0;
	m_ctrlCPS_FilamentCurrent.Enable(Enable, Flag);
	m_btnHV.EnableWindow( FALSE );
	m_btnBias.EnableWindow( FALSE );
	m_btnFilament.EnableWindow( FALSE );

	// CPS 3601
	VAC_VacuumReady( FALSE );
	///////////////////////////////////////////////////////////////////////////

	Localize();

	m_brushVacNotGood.CreateSolidBrush(RGB(128,128,64));
	m_brushVacGood.CreateSolidBrush(RGB(0,128,0));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImpPanelDebug2::MCI_Init()
{
	int nPos = CControlLayer::Instance().m_BrdMCI.nGain[ 2* CControlLayer::Instance().m_BrdMCI.nDetPair ];
	m_scrollDetGain.SetScrollPos( nPos );
	MCI_DetGainSB_Change( nPos );
	nPos = CControlLayer::Instance().m_BrdMCI.nGain[ 2* CControlLayer::Instance().m_BrdMCI.nDetPair +1 ];
	m_scrollDetGain2.SetScrollPos( nPos );
	MCI_DetGain2SB_Change( nPos );
	nPos = CControlLayer::Instance().m_BrdMCI.nFilter[ CControlLayer::Instance().m_BrdMCI.nDetPair ];
	m_scrollBWsel.SetScrollPos( nPos );
	MCI_BWselSB_Change( nPos );

	SetNeedMon( 1 );
}

void CDlgImpPanelDebug2::OnDestroy() 
{
	theApp.DisplayMsg("Into debug2 destroy");
	///////////////////////////////////////////////////////////////////////////
	// 以防用户在看像时直接单击程序关闭按钮
	KillTimer(11);
	KillTimer(12);
	KillTimer(13);
	CCPS3603tatusDetector::Instance().SetParam(21,AS_NULL);
	// 停止扫描
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
		pView->USB_ChangeScan( USB_ST_Stop );

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	SerialNumber.lVal	= -1;
	Xpos.lVal			= 0;
	Flag.lVal			= 1;
	// 1. 自动退灯丝至0
	m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );
	strX = "0.0 V";
	m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );
	CCPS3603tatusDetector::Instance().SetParam( 12, 0 );
	// 2. 自动退偏压至0
	m_ctrlCPS_Bias.SyncPos( Xpos, Ypos, Flag );
	strX = "0.0 V";
	m_ctrlCPS_Bias.SyncText( strX, strY, Flag );
	CCPS3603tatusDetector::Instance().SetParam( 11, 0 );
	// 3. 自动退高压至0
	CCPS3603tatusDetector::Instance().SetParam( 10, 0 );
	ClassID.lVal	= SemControlPanelClass_CPS_HV;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	if( pView != NULL && (CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop) )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
	// 4. 自动退对比度至0
	// Contrast
	{
		ClassID.lVal = SemControlPanelClass_Contrast;
		Flag.lVal = 1;
		Xpos.lVal = 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
	///////////////////////////////////////////////////////////////////////////
	CCPS3603tatusDetector::Instance().Release();
	CSMvac::Instance().Close();

	CDialog::OnDestroy();
	
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlHV );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlWD );

	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlCondenserLens );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlObjectiveLens );

	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlBrightness );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlContrast );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlSignal2Brightness );

	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlMagnifier );

	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlBeemAlignment );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlScanShift );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlScanRotate );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlStigmation );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlAuxiliaryStigmation );

	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDFocus );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDFocusSwitch );

	if( m_hIcon1 != NULL )
		::DestroyIcon( m_hIcon1 );
	if( m_hIcon2 != NULL )
		::DestroyIcon( m_hIcon2 );
	if( m_hIcon3 != NULL )
		::DestroyIcon( m_hIcon3 );
	if( m_hIcon4 != NULL )
		::DestroyIcon( m_hIcon4 );

	// CPS 3601
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlCPS_HV );
}

void CDlgImpPanelDebug2::OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your control notification handler code here
	if( ClassID.lVal < SemControlPanelClass_CPS_HV )
	{
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		BOOL bEnable = (BOOL)(Xpos.lVal);
		CScanView* pView = (CScanView*)theApp.GetScanView();
		VARIANT XposNew;
		CString str;
		if( pView )
		{
			switch( ClassID.lVal )
			{
			case	SemControlPanelClass_DFocusSwitch:
				pView->SyncDynFocus( bEnable );
				EnableCtrl( ClassID.lVal, bEnable );	// 09.01.15
				break;
			case	SemControlPanelClass_DFocus:
				XposNew = CSemCoreAssemble::Instance().m_DFocus.GetPos();
				m_ctrlDFocus.SyncPos( XposNew, Ypos, Flag );
				XposNew = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
				m_ctrlObjectiveLens.SyncPos( XposNew, Ypos, Flag );
				str = CSemCoreAssemble::Instance().m_ObjectiveLens.FormatText();
				m_ctrlObjectiveLens.SyncText( str, str, Flag );
				break;
			}
		}
//		UpdateManualAdjustor( ClassID.lVal, Flag.lVal );
	}
	else
	{
		CString strX, strY;
		if( ClassID.lVal == SemControlPanelClass_CPS_HV )
		{
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			double dHV = 0;
			if( Xpos.lVal < 100 )
			{
				strX.Format( "%.1f KV", Xpos.lVal /10.0 );
				CCPS3603tatusDetector::Instance().SetParam( 10, Xpos.lVal *100 );
				dHV = Xpos.lVal /10.0;
			}
			else
			{
				int nPos = Xpos.lVal -100 +10;
				strX.Format( "%d KV", nPos);
				CCPS3603tatusDetector::Instance().SetParam( 10, nPos *1000 );
				dHV = nPos;
			}
			m_ctrlCPS_HV.SyncText( strX, strY, Flag );
			CScanView* pView = (CScanView*)theApp.GetScanView();
			if( pView != NULL && CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
				CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( dHV );
		}
		else if( ClassID.lVal == SemControlPanelClass_CPS_Bias )
		{
			strX.Format( "%.1f V", Xpos.lVal *1.0 );
			m_ctrlCPS_Bias.SyncText( strX, strY, Flag );
			CCPS3603tatusDetector::Instance().SetParam( 11, Xpos.lVal *10 );
			if( CAppLogManager::Instance().m_boolHVLog )
			{
				strY.Format( GetResString(IDF_SD_SemControlPanelClassID_HVAccelerate), ClassID.lVal);
				strX = strY + _T(" ") + strX;
				theApp.DisplayMsg( strX );
			}
		}
		else if( ClassID.lVal == SemControlPanelClass_CPS_FilamentCurrent )
		{
			strX.Format( "%.3f A", Xpos.lVal /100.0 );
			m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );
			CCPS3603tatusDetector::Instance().SetParam( 12, Xpos.lVal *10 );
			if( CAppLogManager::Instance().m_boolFilamentCurrentLog )
			{
				strY.Format( GetResString(IDF_SD_SemControlPanelClassID_FilamentCurrent), ClassID.lVal);
				strX = strY + _T(" ") + strX;
				theApp.DisplayMsg( strX );
			}
		}
	}
}

void CDlgImpPanelDebug2::EnableCtrl( long lClassID, BOOL bEnable )
{
	VARIANT ClassID, SerialNumber, Enabled, Flag, varPos;
	SerialNumber.lVal	= -1;
	Flag.lVal			= 3;
	Enabled.lVal		= (long)bEnable;
	switch( lClassID )
	{
	case	SemControlPanelClass_DFocusSwitch:
		ClassID.lVal = SemControlPanelClass_DFocus;
		varPos = CSemCoreAssemble::Instance().m_DFocus.GetSwitch();
		// 同步使能
		if( varPos.lVal != 0 )
			Enabled.lVal = 1;
		else
			Enabled.lVal = 0;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		// 关闭状态时，将步进量恢复为最小值0
		if( varPos.lVal == 0 )
		{
			CSemCoreAssemble::Instance().m_DFocus.GetRange( varPos, Enabled );
			Flag.lVal = 1;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, varPos, varPos, Flag );
		}
		break;
	}
}

//调节器类型改变
void CDlgImpPanelDebug2::OnAdjustorSortChanged(UINT nID)
{
	UpdateData();
	int nAdjustorSort = nID - ID_ADJUSTOR_BEAM_ALIGNMENT;
	UINT nFlag = 0;
	switch( nAdjustorSort )
	{
	case	0:
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_BeamAlign ) )->ShowWindow( TRUE );
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_ScanShift ) )->ShowWindow( FALSE );
		nID	= SemControlPanelClass_BeemAlignment;
		if( CControlLayer::Instance().m_nAdjustorManualSort == SORT_MANUAL_BEAM_ALIGNMENT_Y )
			nFlag	= 2;
		else
			nFlag	= 1;
		break;
	case	1:
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_BeamAlign ) )->ShowWindow( FALSE );
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_ScanShift ) )->ShowWindow( TRUE );
		nID	= SemControlPanelClass_ScanShift;
		if( CControlLayer::Instance().m_nAdjustorManualSort == SORT_MANUAL_BEAM_DISPLACEMENT_Y )
			nFlag	= 2;
		else
			nFlag	= 1;
		break;
	case	2:
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND3CTRL_ScanRotate ) )->ShowWindow( TRUE );
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_Stig ) )->ShowWindow( FALSE );
		nID	= SemControlPanelClass_ScanRotate;
		break;
	case	7:
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND3CTRL_ScanRotate ) )->ShowWindow( FALSE );
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_Stig ) )->ShowWindow( TRUE );
		nID	= SemControlPanelClass_Stigmator;
		if( CControlLayer::Instance().m_nAdjustorManualSort == SORT_MANUAL_STIGMATOR_Y )
			nFlag	= 2;
		else
			nFlag	= 1;
		break;
	}

//	UpdateManualAdjustor( nID, nFlag );
	UpdateData( FALSE );
}

void CDlgImpPanelDebug2::UpdateAdjustor( UINT nAdjustorSort )
{
	switch( nAdjustorSort )
	{
	case	SORT_BEAM_ALIGNMENT:
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_BeamAlign ) )->ShowWindow( TRUE );
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_ScanShift ) )->ShowWindow( FALSE );
		((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_ALIGNMENT ) ))->SetCheck( TRUE );
		((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_SHIFT ) ))->SetCheck( FALSE );
		break;
	case	SORT_SCAN_SHIFT:
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_BeamAlign ) )->ShowWindow( FALSE );
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_ScanShift ) )->ShowWindow( TRUE );
		((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_ALIGNMENT ) ))->SetCheck( FALSE );
		((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_SHIFT ) ))->SetCheck( TRUE );
		break;
	case	SORT_STIGMATOR:
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND3CTRL_ScanRotate ) )->ShowWindow( FALSE );
		( GetDlgItem( IDC_SEMCORECTRLPANELSTAND2CTRL_Stig ) )->ShowWindow( TRUE );
		((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_ROTATE ) ))->SetCheck( FALSE );
		((CButton*)( GetDlgItem( ID_ADJUSTOR_STIGMATOR ) ))->SetCheck( TRUE );
		break;
	case	SORT_CONDENSER_LENS:
		if( (CControlLayer::Instance().m_nAdjustorManualSort == SORT_MANUAL_CONDENSER_CORSE)
			&& (m_ctrlCondenserLens.GetGradeState() != 0) )
			m_ctrlCondenserLens.SetGradeState( 0 );
		else if( (CControlLayer::Instance().m_nAdjustorManualSort == SORT_MANUAL_CONDENSER_FINE)
			&& (m_ctrlCondenserLens.GetGradeState() == 0) )
			m_ctrlCondenserLens.SetGradeState( 1 );
		break;
	case	SORT_OBJECTIVE_LENS:
		if( (CControlLayer::Instance().m_nAdjustorManualSort == SORT_MANUAL_OBJECTIVE_CORSE)
			&& (m_ctrlObjectiveLens.GetGradeState() !=0) )
			m_ctrlObjectiveLens.SetGradeState( 0 );
		else if( (CControlLayer::Instance().m_nAdjustorManualSort == SORT_MANUAL_OBJECTIVE_FINE)
			&& (m_ctrlObjectiveLens.GetGradeState() == 0) )
			m_ctrlObjectiveLens.SetGradeState( 1 );
		break;
	}
}

void CDlgImpPanelDebug2::UpdateManualAdjustor( UINT nID, UINT nFlag )
{
	int nSort = CControlLayer::Instance().m_nAdjustorManualSort;
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( (nID == SemControlPanelClass_Brightness)
		&& (nSort != SORT_MANUAL_BRIGHT) )
		pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_Brightness );
	else if( (nID == SemControlPanelClass_Contrast)
		&& (nSort != SORT_MANUAL_CONTRAST) )
		pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_Contrast );
	else if( nID == SemControlPanelClass_CondenserLens )
	{
		short nState = m_ctrlCondenserLens.GetGradeState();
		if( (nState == 0)		//聚光镜粗调
			&& (nSort != SORT_MANUAL_CONDENSER_CORSE) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_CondenserLens2 );
		else if( (nState != 0)
			&& (nSort != SORT_MANUAL_CONDENSER_FINE) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_CondenserLens );
	}
	else if( nID == SemControlPanelClass_ObjectiveLens )
	{
		short nState = m_ctrlObjectiveLens.GetGradeState();
		if( (nState == 0)		//物镜粗调
			&& (nSort != SORT_MANUAL_OBJECTIVE_CORSE) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_ObjectiveLens2 );
		else if( (nState != 0)
			&& (nSort != SORT_MANUAL_OBJECTIVE_FINE) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_ObjectiveLens );
	}
	else if( nID == SemControlPanelClass_ScanShift )
	{
		if( (nFlag == 1)	//电位移X方向
			&& (nSort != SORT_MANUAL_BEAM_DISPLACEMENT_X) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_ScanShiftX );
		else if( (nFlag == 2)
			&& (nSort != SORT_MANUAL_BEAM_DISPLACEMENT_Y) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_ScanShiftY );
		else
			if( (nSort != SORT_MANUAL_BEAM_DISPLACEMENT_X)
				&& (nSort != SORT_MANUAL_BEAM_DISPLACEMENT_Y) )
				pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_ScanShiftX );
	}
	else if( nID == SemControlPanelClass_BeemAlignment )
	{
		if( (nFlag == 1)	//电对中X方向
			&& (nSort != SORT_MANUAL_BEAM_ALIGNMENT_X) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_BeamAlignmentX );
		else if( (nFlag == 2 )
			&& (nSort != SORT_MANUAL_BEAM_ALIGNMENT_Y) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_BeamAlignmentY );
		else
			if( (nSort != SORT_MANUAL_BEAM_ALIGNMENT_X)
				&& (nSort != SORT_MANUAL_BEAM_ALIGNMENT_Y) )
				pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_BeamAlignmentX );
	}
	else if( nID == SemControlPanelClass_Stigmator )
	{
		if( (nFlag == 1)	//消像散X方向
			&& (nSort != SORT_MANUAL_STIGMATOR_X) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_StigmatorX );
		else if( (nFlag == 2)
			&& (nSort != SORT_MANUAL_STIGMATOR_Y) )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_StigmatorY );
		else
			if( (nSort != SORT_MANUAL_STIGMATOR_X)
				&& (nSort != SORT_MANUAL_STIGMATOR_Y) )
				pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_StigmatorX );
	}
	else if( nID == SemControlPanelClass_ScanRotate )
	{
		if( nSort != SORT_MANUAL_NULL )
			pView->SyncAdjustorManualSort( ID_ADJUSTOR_MANUAL_NULL );
	}
}

void CDlgImpPanelDebug2::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == nMCI_ReadStatusTimerID )
	{
//		VAC_ReadComprs();	// 读TC当前状态
//		VAC_JudgeLogic();	// 根据TC更新阀状态
		return;
	}
	else if( nIDEvent == 2 )
	{
		// Read Selected ADC
		CString str;
		if( CBoardComm::Instance().m_bADCtype )
			str = CBoardComm::Instance().ReadSelectedADC_Procedure( 100, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log, FALSE );
		else
			str = CBoardComm::Instance().ReadSelectedADC_Procedure( 70, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log, FALSE );
		GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
		ADCvconv();
	}
	else if( nIDEvent == 4 )
	{

	}
	else if( nIDEvent == 11 )
	{
		// 一键看像第一步：加亮度至经验值
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
		SerialNumber.lVal	= -1;
		// Brightness
		{
			//	Disp		Ctrl		pos
			//	0.0			B00			0
			//	-1.56		B10			-16
			//	-3.13		B20			-32
			ClassID.lVal = SemControlPanelClass_Brightness;
			Flag.lVal = 1;
			Xpos.lVal = CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal -100;
			if( Xpos.lVal < m_nPosB )
			{
				Xpos.lVal	= m_nPosB;
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
				KillTimer( 11 );
				SetTimer( 12, 150, NULL );	// 启动一键看像第二步：加对比度至经验值
			}
			else
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
	}
	else if( nIDEvent == 12 )
	{
		// 一键看像第二步：加对比度至经验值
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
		SerialNumber.lVal	= -1;
		// Contrast
		{
			//	Disp		Ctrl		Pos,	Disp		Ctrl		Pos
			//	65.882		5700		168,	66.275		5600		169
			//	66.667		5500		170,	67.059		5400		171
			//	67.451		5300		172,	67.843		5200		173
			//	68.235		5100		174,	68.627		5000		175
			//	69.020		4F00		176,	69.412		4E00		177
			//	69.804		4D00		178,	70.196		4C00		179
			//	70.588		4B00		180,	70.980		4A00		181
			//	71.373		4900		182,	71.765		4800		183
			ClassID.lVal = SemControlPanelClass_Contrast;
			Flag.lVal = 1;
			Xpos.lVal = CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal +10;
			if( Xpos.lVal > m_nPosC )
			{
				Xpos.lVal = m_nPosC;
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
				KillTimer( 12 );
				// 启动一键看像第二步：加3603
				// 自动加高压至30KV
				// 自动加偏压至100V
				// 自动加灯丝至1.9A
				CCPS3603tatusDetector::Instance().SetParam( 21, AS_Rise_Acc1 );
			}
			else
				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
	}
	else if( nIDEvent == 13 )
	{
		CScanView* pView = (CScanView*)theApp.GetScanView();
		if( CScanParameterSaver::Instance().m_nAutoFuction == AutoNULL )
		{
			KillTimer( 13 );
			// 自动加灯丝（找饱和点）完成
			// 启动2000倍自动聚焦
			Sleep(100);
			pView->FuncAutoFocus();
			// 启动自动亮度对比度
//			pView->FuncAutoBrightContrast();
//			Sleep(1);
//			SetTimer( 14, 100, NULL );
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgImpPanelDebug2::OnResetCommunicate() 
{
	// Reset / Initialize Board Communication Circuit
	CSemCtrl::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
	theApp.DisplayMsg("Press reset communicate");
}

void CDlgImpPanelDebug2::OnCHECKTestLEDs() 
{
	// Utilizes TestLED16State global, temporarily uses CL wobbler On ckt address (0x1c)
 	unsigned long DIOData = 0;
	CString str = "";
	CString strTemp = "";

	int nCheck = m_btnTestLEDs.GetCheck();
	if( nCheck )
	{
		m_btnTestLEDs.SetColor( g_crON_Test );
		m_btnTestLEDs.SetWindowText( "Test LEDs: now ON" );
		theApp.DisplayMsg("Press test LEDs: ON");
	}
	else
	{
		m_btnTestLEDs.SetColor( g_crOFF );
		m_btnTestLEDs.SetWindowText( "Test LEDs: now OFF" );
		theApp.DisplayMsg("Press test LEDs: OFF");
	}

	CControlLayer::Instance().CHECKTestLEDs(nCheck);
}

void CDlgImpPanelDebug2::SetupDebugParam( BOOL bDebugVac )
{
	VAC_SoftAuto( !bDebugVac );
}

void CDlgImpPanelDebug2::OnVacuumSoftAuto() 
{
	VAC_SoftAuto( !m_bVacuumSoftAuto );
}

void CDlgImpPanelDebug2::VAC_SoftAuto( int nCheck )
{
	m_bVacuumSoftAuto = nCheck;
	if( m_bVacuumSoftAuto )
	{
		m_btnSoftAuto.SetColor( RGB(0,0,255) );
		m_btnSoftAuto.SetWindowText( "Soft Auto" );
	}
	else
	{
		m_btnSoftAuto.SetColor( g_crOFF );
//		m_btnSoftAuto.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnSoftAuto.SetWindowText( "Soft Man" );
		m_btnVacuumSwitch.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnVacuumSwitch.SetCheck(0);
		m_btnGunVent.SetCheck(0);
		m_btnChamberVent.SetCheck(0);
	}
	m_btnVacuumSwitch.EnableWindow( m_bVacuumSoftAuto );
	m_btnGunVent.EnableWindow( m_bVacuumSoftAuto );
	m_btnChamberVent.EnableWindow( m_bVacuumSoftAuto );
	m_btnV1status.EnableWindow( !m_bVacuumSoftAuto );
	m_btnSPRRelay1En.EnableWindow( !m_bVacuumSoftAuto );
	for( int i=0; i<6; i++ )
		GetDlgItem( IDC_CHECK_FPGArelayEn0 +i )->EnableWindow( !m_bVacuumSoftAuto );
	GetDlgItem( IDC_CHECK_Vacuum_TCf )->EnableWindow( !m_bVacuumSoftAuto );
	GetDlgItem( IDC_CHECK_Vacuum_TCc )->EnableWindow( !m_bVacuumSoftAuto );
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
		CSMvac::Instance().SetParams( 20, !m_bVacuumSoftAuto );
	else
		CVACDataManager::Instance().m_VacParamMain.bMP = TRUE;
}

void CDlgImpPanelDebug2::VAC_SyncStates( int nIndex )
{
	BOOL bUpdate = TRUE;
	switch( nIndex )
	{
	case	VACIndex_Vf:
		((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn1)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bVf);
		break;
	case	VACIndex_Vc:
		((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn2)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bVc);
		break;
	case	VACIndex_TMP:
		((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn5)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bTMP);
		break;
	case	VACIndex_Va:
		((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn4)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bVa);
		break;
	case	VACIndex_TCf:
		((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCf)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bTCf);
		break;
	case	VACIndex_TCc:
		((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCc)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bTCc);
		break;
	case	VACIndex_V1:
		if(CVACDataManager::Instance().m_VacParamMain.bV1 )
		{
			m_btnV1status.SetColor( g_crEmphasis1 );
			m_btnV1status.SetWindowText( "V1: Open" );//"V1 Close (Now Open)" );
			GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( TRUE );
			GetDlgItem( IDC_BUTTON_HVOnCmd )->EnableWindow( TRUE );
		}
		else
		{
			m_btnV1status.SetColor( g_crOFF );
			m_btnV1status.SetWindowText( "V1: Close" );//"V1 Open (Now Close)" );
			GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( FALSE );
			GetDlgItem( IDC_BUTTON_HVOnCmd )->EnableWindow( FALSE );
		}
		break;
	default:
		bUpdate = FALSE;
		break;
	}
	if( bUpdate )
	{
		VAC_UpdateUIswitches();
		VAC_UpdatePneumaticDiagram();
	}
}

void CDlgImpPanelDebug2::OnVacuumVACUUMPOWER() 
{
	int nCheck = m_btnVacuumPower.GetCheck();
	if( nCheck )
	{
		m_btnVacuumPower.SetColor( RGB(255,0,0) );

		CSMvac::Instance().SetParams( VACIndex_Power, 1 );
		CSMvac::Instance().SetParams( VACIndex_MP, 1 );
		theApp.DisplayMsg("Press vacuum POWER: start");
	}
	else
	{
		m_btnVacuumPower.SetColor( ::GetSysColor(COLOR_3DFACE) );
		CSMvac::Instance().SetParams( VACIndex_Power, 0 );
		theApp.DisplayMsg("Press vacuum POWER: close");
	}
}

void CDlgImpPanelDebug2::OnVacuumVACUUMSWITCH() 
{
	int nCheck = m_btnVacuumSwitch.GetCheck();
	if( nCheck )
		theApp.DisplayMsg("Press ONEKEY: start");
	else
		theApp.DisplayMsg("Press ONEKEY: end");
	if( nCheck )
	{
		// 什么时候可以加高压
		if( CVACDataManager::Instance().CheckVAC() )
		{
			if( !m_bSPRRelay1En )
				SPR_En( TRUE );
			if( m_bSPRRelay1En && !CCPS3603tatusDetector::Instance().IsReady() )
			{
				HVPower_SetEnable( 5, TRUE );
				HVPower_SetEnable( 6, TRUE );
				HVPower_SetEnable( 7, TRUE );
			}
		}
		else
		{
			if( m_bSPRRelay1En )
			{
				HVPower_SetEnable( 7, FALSE );
				HVPower_SetEnable( 6, FALSE );
				HVPower_SetEnable( 5, FALSE );
				m_btnHV.SetCheck(0);
				m_btnBias.SetCheck(0);
				m_btnFilament.SetCheck(0);
				SPR_En( FALSE );
			}
			AfxMessageBox( "Vacuum NOT ready or V1 NOT open!" );
			nCheck = 0;
		}
	}
	m_btnVacuumSwitch.SetCheck( nCheck );
	if( nCheck )
		m_btnVacuumSwitch.SetColor( RGB(0,192,0) );
	else
		m_btnVacuumSwitch.SetColor( ::GetSysColor(COLOR_3DFACE) );

	if( nCheck )
	{
		// 发起扫描
		CScanView* pView = (CScanView*)theApp.GetScanView();
		pView->USB_ChangeScan( USB_ST_RasterScan );

		// CPS3603三路使能
		m_btnHV.SetCheck( 1 );
		HVPower_SetEnable( 5, 1 );
		m_btnBias.SetCheck( 1 );
		HVPower_SetEnable( 6, 1 );
		m_btnFilament.SetCheck( 1 );
		HVPower_SetEnable( 7, 1 );

		// 0. 设置放大倍数、聚光镜、物镜至经验值
		///////////////////////////////////////////////////
		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");
		char		stabuff[20];
		char		staitembuff[20];
		int			nTotal	= 8;
		int*		pCheck	= new int[8];
		CString		str;
		sprintf(stabuff, "OneKEY");
		sprintf(staitembuff, "param");
		CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
		int nHV		= pCheck[0];
		int nBias	= pCheck[1];
		int nFila	= pCheck[2];
		m_nPosB		= pCheck[3];
		m_nPosC		= pCheck[4];
		int	nPosCL	= pCheck[5];
		int	nPosOL	= pCheck[6];
		int	nPosMag	= pCheck[7];
		delete [] pCheck;
		///////////////////////////////////////////////////

		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
		CString strX, strY;
		SerialNumber.lVal	= -1;
		Flag.lVal			= 1;
		// CondenserLens
		{
			ClassID.lVal	= SemControlPanelClass_CondenserLens;
			Xpos.lVal		= nPosCL;	// 550.00 [C4C0]
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		// ObjectiveLens
		{
			ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
			Xpos.dblVal		= nPosOL;	// 550.00 [C4C0]
			Xpos.lVal		= nPosOL;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		// Magnifier
		{
			ClassID.lVal	= SemControlPanelClass_Magnifier;
			Xpos.dblVal		= nPosMag;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		CCPS3603tatusDetector::Instance().SetParam( 25, nHV );
		CCPS3603tatusDetector::Instance().SetParam( 26, nBias );
		CCPS3603tatusDetector::Instance().SetParam( 27, nFila );
		// 1. 加亮度及对比度至经验值
		// 2. 自动加高压至设定值
		// 3. 自动加偏压至设定值
		// 41.自动加灯丝至设定值
		SetTimer( 11, 150, NULL );
		// 42.启动自动加灯丝程序，从1.8A至第二饱和点
	}
	else
	{
		KillTimer(11);
		KillTimer(12);
		KillTimer(13);
		CCPS3603tatusDetector::Instance().SetParam(21,AS_NULL);
		// 停止扫描
//		CScanView* pView = (CScanView*)theApp.GetScanView();
//		pView->USB_ChangeScan( USB_ST_Stop );
	}
}

void CDlgImpPanelDebug2::OnCheckOff() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press clear OFF");
	// 0. 先停止一键看像正在进行的步骤
	int nCheck = m_btnVacuumSwitch.GetCheck();
	if( nCheck )
	{
		KillTimer(11);
		KillTimer(12);
		KillTimer(13);
		CCPS3603tatusDetector::Instance().SetParam(21,AS_NULL);
		m_btnVacuumSwitch.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnVacuumSwitch.SetCheck(0);
	}
	// 停止扫描
//	CScanView* pView = (CScanView*)theApp.GetScanView();
//	pView->USB_ChangeScan( USB_ST_Stop );

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	SerialNumber.lVal	= -1;
	Xpos.lVal			= 0;
	Flag.lVal			= 1;
	// 1. 自动退灯丝至0
	m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );
	strX = "0.000 A";
	m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );
	CCPS3603tatusDetector::Instance().SetParam( 12, 0 );
	// 2. 自动退偏压至0
	m_ctrlCPS_Bias.SyncPos( Xpos, Ypos, Flag );
	strX = "0.0 V";
	m_ctrlCPS_Bias.SyncText( strX, strY, Flag );
	CCPS3603tatusDetector::Instance().SetParam( 11, 0 );
	// 3. 自动退高压至0
	ClassID.lVal	= SemControlPanelClass_CPS_HV;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	CCPS3603tatusDetector::Instance().SetParam( 10, 0 );
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL && CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
	// 4. 自动退对比度至0
	// Contrast
	{
		ClassID.lVal = SemControlPanelClass_Contrast;
		Flag.lVal = 1;
		Xpos.lVal = 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
}

void CDlgImpPanelDebug2::OnVacuumGUNVENT() 
{
	// 电子枪放气
	int nCheck = m_btnGunVent.GetCheck();
	if( nCheck )
	{
		m_btnGunVent.SetColor( RGB(192,192,0) );
		theApp.VAC_SaveStates();

		// 关高压、灯丝，对比度退0
		VAC_Reset();
		CSMvac::Instance().SetParams( VACIndex_Gun, 1 );
		m_btnChamberVent.EnableWindow( FALSE );
		theApp.DisplayMsg("Press GUN: start vent");
	}
	else
	{
		m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		theApp.VAC_SaveStates();
		CSMvac::Instance().SetParams( VACIndex_Gun, 0 );
		m_btnChamberVent.EnableWindow( TRUE );
		theApp.DisplayMsg("Press GUN: start pump");
	}
}

void CDlgImpPanelDebug2::OnVacuumCHAMBERVENT() 
{
	// 样品室放气
	int nCheck = m_btnChamberVent.GetCheck();
	if( nCheck )
	{
		m_btnChamberVent.SetColor( RGB(192,192,0) );
		theApp.VAC_SaveStates();

		// 关高压、灯丝，对比度退0
		VAC_Reset();
		CSMvac::Instance().SetParams( VACIndex_Chamb, 1 );
		m_btnGunVent.EnableWindow( FALSE );
		theApp.DisplayMsg("Press CHAMB: start vent");
	}
	else
	{
		m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		theApp.VAC_SaveStates();

		CSMvac::Instance().SetParams( VACIndex_Chamb, 0 );
		m_btnGunVent.EnableWindow( TRUE );
		theApp.DisplayMsg("Press CHAMB: start pump");
	}
}

BOOL CDlgImpPanelDebug2::VAC_Init()
{
	BOOL bRet = FALSE;

	CCCGStatusDetector::Instance().Init( this->m_hWnd, CVACDataManager::Instance().m_cfgVacCCG.nPort );
	CCCGStatusDetector::Instance().InitThreshold(CVACDataManager::Instance().m_cfgVacCCG.dThreshold1,CVACDataManager::Instance().m_cfgVacCCG.dThreshold2);
	CCCGStatusDetector::Instance().Start();

	///////////////////////////////////////////////////////
	// 初始化状态机
	VAC_VacuumReady2( TRUE );
	// 真正连接下位机时，V1的状态要靠回读
	BOOL bUSBReady = CSemCtrl::Instance().USB_isReady();
	if( bUSBReady )
	{
		// 使用Read Board ID功能来判断VAC板是否联通
		unsigned short nRegister = 0;
		int nCount = 0;
		CString str;
		do
		{
			CSemCtrl::Instance().Serial_ReadbackData( 3, USB_CID_VAC_ReadBoardID, &nRegister );
			nCount++;
			if( nRegister == 0x0 )
				CSemCtrl::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		} while( nRegister == 0x0 && nCount < 50 );
		str.Format( GetResString(IDF_SD_PROMPT_VAC_29), nCount, nRegister );
		theApp.DisplayMsg( str );
		if( nRegister == 0xA3 )
		{
			// 真实情况，非调试模式
			CSMvac::Instance().Reset( this );
			CSMvac::Instance().SetParams(20, 0);
			CSMvac::Instance().SetParams(22, 0);
			bRet = TRUE;
		}
	}
	if( !bRet )
	{
		// 调试模式/未连下位机
		CSMvac::Instance().SetParams(20, 1);
		VAC_VacuumReady2( FALSE );
		return bRet;
	}

	///////////////////////////////////////////////////////
	// 通过记录看是否上次退出时真空电源是打开的
	if(CVACDataManager::Instance().m_VacParamMain.bPower )
	{
		m_btnVacuumPower.SetColor( RGB(255,0,0) );
		m_btnVacuumPower.SetCheck(1);
		
		CSMvac::Instance().SetParams( VACIndex_Power, 1 );
		CSMvac::Instance().SetParams( VACIndex_MP, 1 );
	}
	else
	{
		m_btnVacuumPower.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnVacuumPower.SetCheck(0);
		CSMvac::Instance().SetParams( VACIndex_Power, 0 );
	}
	if(CVACDataManager::Instance().m_VacParamMain.bChamb )
	{
		m_btnChamberVent.SetColor( RGB(192,192,0) );
		CSMvac::Instance().SetParams( VACIndex_Chamb, 1 );
		m_btnGunVent.EnableWindow( FALSE );
	}
	else
	{
		m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		CSMvac::Instance().SetParams( VACIndex_Chamb, 0 );
		m_btnGunVent.EnableWindow( TRUE );
	}
	if(CVACDataManager::Instance().m_VacParamMain.bGun )
	{
		m_btnGunVent.SetColor( RGB(192,192,0) );
		CSMvac::Instance().SetParams( VACIndex_Gun, 1 );
		m_btnChamberVent.EnableWindow( FALSE );
	}
	else
	{
		m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		CSMvac::Instance().SetParams( VACIndex_Gun, 0 );
		m_btnChamberVent.EnableWindow( TRUE );
	}
	VAC_UpdateUIswitches( TRUE );

//	((CButton*)(GetDlgItem(IDC_CHECK_Stig_Ctrl)))->SetCheck(1);
	return bRet;
}

void CDlgImpPanelDebug2::VAC_Reset()
{
	theApp.DisplayMsg( "VAC_Reset: set HV/Fila/Contrast to 0");
	CScanView* pView = (CScanView*)theApp.GetScanView();
	CVACDataManager::Instance().VAC_Reset(CScanParameterManager::Instance().m_nusbScanType);
}

void CDlgImpPanelDebug2::VAC_VacuumReady( BOOL bReady )
{
	m_btnHV.EnableWindow( bReady );
	m_btnBias.EnableWindow( bReady );
	m_btnFilament.EnableWindow( bReady );
	m_btnVacuumSwitch.EnableWindow( bReady );
	GetDlgItem( IDC_CHECK_OFF )->EnableWindow( bReady );
}

void CDlgImpPanelDebug2::VAC_VacuumReady2( BOOL bReady )
{
	m_btnVacuumPower.EnableWindow( bReady );
	m_btnChamberVent.EnableWindow( bReady );
	m_btnGunVent.EnableWindow( bReady );
}

BOOL CDlgImpPanelDebug2::VAC_UpdatePneumaticDiagram()
{
	// 更新气路图的各阀灯
	CStatic* pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vf );
	if( pLamp != NULL )
	{
		if (CVACDataManager::Instance().m_VacParamMain.bVf )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vc );
	if( pLamp != NULL )
	{
		if (CVACDataManager::Instance().m_VacParamMain.bVc )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V4 );
	if( pLamp != NULL )
	{
		if (CVACDataManager::Instance().m_VacParamMain.bVa )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_TMP );
	if( pLamp != NULL )
	{
		if (CVACDataManager::Instance().m_VacParamMain.bTMP )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V1 );
	if( pLamp != NULL )
	{
		if (CVACDataManager::Instance().m_VacParamMain.bV1 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	return TRUE;
}

BOOL CDlgImpPanelDebug2::VAC_UpdateUIswitches( BOOL bReset )
{
	if(CVACDataManager::Instance().m_VacParamMain.bVa )
	{
		int nCheck = m_btnChamberVent.GetCheck();
		if( nCheck )
		{
			m_btnChamberVent.EnableWindow( TRUE );
			m_btnGunVent.EnableWindow( FALSE );
		}
		else
		{
			nCheck = m_btnGunVent.GetCheck();
			if( nCheck )
			{
				m_btnGunVent.EnableWindow( TRUE );
				m_btnChamberVent.EnableWindow( FALSE );
			}
		}
	}
	else
	{
		// 只要真空电源打开且V1关时，就可以放气
		if( !CVACDataManager::Instance().m_VacParamMain.bPower ||CVACDataManager::Instance().m_VacParamMain.bV1 )
		{
			m_btnChamberVent.EnableWindow( FALSE );
			m_btnGunVent.EnableWindow( FALSE );
		}
		else
		{
			m_btnChamberVent.EnableWindow( TRUE );
			m_btnGunVent.EnableWindow( TRUE );
		}
	}

	// 什么时候可以加高压
	if( theApp.m_blOverhaulState )
	{
		CVACDataManager::Instance().m_VacParamMain.bHVInt	= TRUE;
		GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_HVOnCmd )->EnableWindow( TRUE );
		SPR_En(TRUE);
	}
	else if( CVACDataManager::Instance().CheckVAC() )
	{
		CVACDataManager::Instance().m_VacParamMain.bHVInt	= TRUE;
		GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_HVOnCmd )->EnableWindow( TRUE );
		SPR_En(TRUE);
	}
	else
	{
		CVACDataManager::Instance().m_VacParamMain.bHVInt	= FALSE;
		GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_HVOnCmd )->EnableWindow( FALSE );
		SPR_En(FALSE);
	}

	if(CVACDataManager::Instance().m_VacParamMain.bV1 )
	{
		m_btnV1status.SetColor( g_crEmphasis1 );
		m_btnV1status.SetWindowText( "V1: Open" );//"V1 Close (Now Open)" );
	}
	else
	{
		m_btnV1status.SetColor( g_crOFF );
		m_btnV1status.SetWindowText( "V1: Close" );//"V1 Open (Now Close)" );
	}
	if(CVACDataManager::Instance().m_VacParamMain.bPower )
	{
		m_btnVacuumPower.SetColor( RGB(255,0,0) );
		m_btnVacuumPower.SetCheck(1);
	}
	else
	{
		m_btnVacuumPower.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnVacuumPower.SetCheck(0);
	}
	if(CVACDataManager::Instance().m_VacParamMain.bGun )
	{
		m_btnGunVent.SetColor( RGB(192,192,0) );
		m_btnGunVent.SetCheck(1);
		m_btnChamberVent.EnableWindow( FALSE );
	}
	else
	{
		m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnGunVent.SetCheck(0);
	}
	if(CVACDataManager::Instance().m_VacParamMain.bChamb )
	{
		m_btnChamberVent.SetColor( RGB(192,192,0) );
		m_btnChamberVent.SetCheck(1);
		m_btnGunVent.EnableWindow( FALSE );
	}
	else
	{
		m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnChamberVent.SetCheck(0);
	}

	if( bReset )
	{
		((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_VACUUM_POWER)))->SetCheck( FALSE );
	}

	((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn1)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bVf );
	((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn2)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bVc );
	((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn4)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bVa );
	((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn5)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bTMP );
	((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCf)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bTCf );
	((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCc)))->SetCheck(CVACDataManager::Instance().m_VacParamMain.bTCc );
	// TC gauge compr: 4th readback word (7th and 8th ASCII characters) contains Comparator data
	WORD wData, wRBit, wMskBit;
	wData = (WORD)CVACDataManager::Instance().m_VacParamMain.dwTCs;
	for( wRBit=0; wRBit<14; wRBit += 2 )
	{
		wMskBit = (WORD)(pow((double)2, wRBit) );	// Odd numbered bits are Upper trip point comparator states
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "0" );
		
		wMskBit *= 2;					// Shift up one bit to check Lower trip point comparator state
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "0" );
	}
	// ExtSwitch
	if(CVACDataManager::Instance().m_VacParamMain.bV1 )
		GetDlgItem(IDC_STATIC_VACextSwStatesLabel0)->SetWindowText( "OPEN" );
	else
		GetDlgItem(IDC_STATIC_VACextSwStatesLabel0)->SetWindowText( "CLOSE" );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
LRESULT CDlgImpPanelDebug2::OnSMvacChanged(WPARAM wParam, LPARAM lParam)
{
	BOOL bVf =CVACDataManager::Instance().m_VacParamMain.bVf;
	if( lParam != 50 )
		CVACDataManager::Instance().m_VacParamMain = *((SMvac_Param*)wParam);
	theApp.VAC_SaveStates();
	VAC_UpdatePneumaticDiagram();
	VAC_UpdateUIswitches();
	if( !bVf &&CVACDataManager::Instance().m_VacParamMain.bVf && CVACDataManager::Instance().m_cfgVacCCG.bReady )
	{
		Sleep(2000);	// 延迟2秒后再重新建立连接
		CString str = GetResString(IDS_SD_PROMPT_VAC_27);
		theApp.DisplayMsg( str );
	}
	else if( bVf && !(CVACDataManager::Instance().m_VacParamMain.bVf) && CVACDataManager::Instance().m_cfgVacCCG.bReady )
	{
		CString str = GetResString(IDS_SD_PROMPT_VAC_28);
		theApp.DisplayMsg( str );
	}

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon->m_hWnd )
		pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_VAC_MSG, wParam, lParam);
	
	if( lParam > 0 && m_nVACMsgType != lParam )
	{
		CString str;
		str.Format( "MsgType = %d", (int)lParam );
		theApp.DisplayMsg(str);
		switch( lParam )
		{
		case	1:
			str.Format( "TCs=0x%04X, bTCf=%d, bTCc=%d, TCf = %.3f, TCc = %.3f",
						CVACDataManager::Instance().m_VacParamMain.dwTCs,CVACDataManager::Instance().m_VacParamMain.bTCf,CVACDataManager::Instance().m_VacParamMain.bTCc,
						CVACDataManager::Instance().m_VacTCAmp.dTCfAmp, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
			theApp.DisplayMsg(str);
			if (CVACDataManager::Instance().m_VacParamMain.bV1 )
			{
				str = GetResString(IDS_SD_PROMPT_VAC_01);
				theApp.DisplayMsg( str );
				AfxMessageBox( str );
			}
			break;
		case	2:
			str = GetResString(IDS_SD_PROMPT_VAC_02);
			theApp.DisplayMsg( str );
			if (CVACDataManager::Instance().m_VacParamMain.bV1 )
			{
				str = GetResString(IDS_SD_PROMPT_VAC_03);
				AfxMessageBox( str );
			}
			break;
		case	5:
			str = GetResString(IDS_SD_PROMPT_VAC_04);
			theApp.DisplayMsg( str );
			break;
		case	6:
			str = GetResString(IDS_SD_PROMPT_VAC_05);
			theApp.DisplayMsg( str );
			break;
		case	11:
			str = GetResString(IDS_SD_PROMPT_VAC_06);
			theApp.DisplayMsg( str );
			break;
		case	12:
			str = GetResString(IDS_SD_PROMPT_VAC_07);
			theApp.DisplayMsg( str );
			break;
		case	13:
			str = GetResString(IDS_SD_PROMPT_VAC_08);
			theApp.DisplayMsg( str );
			break;
		case	14:
			str = GetResString(IDS_SD_PROMPT_VAC_09);
			theApp.DisplayMsg( str );
			break;
		case	21:
			str = GetResString(IDS_SD_PROMPT_VAC_10);
			theApp.DisplayMsg( str );
			str.Format( "TCs=0x%04X, bTCf=%d, bTCc=%d, TCf = %.3f, TCc = %.3f",
						CVACDataManager::Instance().m_VacParamMain.dwTCs,CVACDataManager::Instance().m_VacParamMain.bTCf,CVACDataManager::Instance().m_VacParamMain.bTCc,
						CVACDataManager::Instance().m_VacTCAmp.dTCfAmp, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
			theApp.DisplayMsg(str);
			if (CVACDataManager::Instance().m_VacParamMain.bV1 )
			{
				str = GetResString(IDS_SD_PROMPT_VAC_01);
				theApp.DisplayMsg( str );
				AfxMessageBox( str );
			}
			break;
		case	22:
			str = GetResString(IDS_SD_PROMPT_VAC_11);
			theApp.DisplayMsg( str );
			str.Format( "TCs=0x%04X, bTCf=%d, bTCc=%d, TCf = %.3f, TCc = %.3f",
						CVACDataManager::Instance().m_VacParamMain.dwTCs,CVACDataManager::Instance().m_VacParamMain.bTCf,CVACDataManager::Instance().m_VacParamMain.bTCc,
						CVACDataManager::Instance().m_VacTCAmp.dTCfAmp, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
			theApp.DisplayMsg( str );
			if (CVACDataManager::Instance().m_VacParamMain.bV1 )
			{
				str = GetResString(IDS_SD_PROMPT_VAC_01);
				theApp.DisplayMsg( str );
				AfxMessageBox( str );
			}
			break;
		case	23:
			CVACDataManager::Instance().m_VacParamMain.bTCf	= TRUE;
			str = GetResString(IDS_SD_PROMPT_VAC_12);
			theApp.DisplayMsg( str );
			break;
		case	24:
			CVACDataManager::Instance().m_VacParamMain.bTCc	= TRUE;
			str = GetResString(IDS_SD_PROMPT_VAC_13);
			theApp.DisplayMsg( str );
			break;
		case	25:
			str = GetResString(IDS_SD_PROMPT_VAC_14);
			theApp.DisplayMsg( str );
			str.Format( "TCs=0x%04X, bTCf=%d, bTCc=%d, TCf = %.3f, TCc = %.3f",
						CVACDataManager::Instance().m_VacParamMain.dwTCs,CVACDataManager::Instance().m_VacParamMain.bTCf,CVACDataManager::Instance().m_VacParamMain.bTCc,
						CVACDataManager::Instance().m_VacTCAmp.dTCfAmp, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
			theApp.DisplayMsg( str );
			if (CVACDataManager::Instance().m_VacParamMain.bV1 )
			{
				str = GetResString(IDS_SD_PROMPT_VAC_01);
				theApp.DisplayMsg( str );
				AfxMessageBox( str );
			}
			break;
		case	26:
			str = GetResString(IDS_SD_PROMPT_VAC_15);
			theApp.DisplayMsg( str );
			break;
		case	27:
			str = GetResString(IDS_SD_PROMPT_VAC_16);
			theApp.DisplayMsg( str );
			break;
		case	28:
			str = GetResString(IDS_SD_PROMPT_VAC_17);
			theApp.DisplayMsg( str );
			break;
		case	29:
			str = GetResString(IDS_SD_PROMPT_VAC_18);
			theApp.DisplayMsg( str );
			str.Format( "TCs=0x%04X, bTCf=%d, bTCc=%d, TCf = %.3f, TCc = %.3f",
						CVACDataManager::Instance().m_VacParamMain.dwTCs,CVACDataManager::Instance().m_VacParamMain.bTCf,CVACDataManager::Instance().m_VacParamMain.bTCc,
						CVACDataManager::Instance().m_VacTCAmp.dTCfAmp, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
			theApp.DisplayMsg( str );
			if (CVACDataManager::Instance().m_VacParamMain.bV1 )
			{
				str = GetResString(IDS_SD_PROMPT_VAC_01);
				theApp.DisplayMsg( str );
				AfxMessageBox( str );
			}
			break;
		case	40:
			str = GetResString(IDS_SD_PROMPT_VAC_19);
			theApp.DisplayMsg( str );
			break;
		case	41:
			str = GetResString(IDS_SD_PROMPT_VAC_20);
			theApp.DisplayMsg( str );
			break;
		case	42:
			str = GetResString(IDS_SD_PROMPT_VAC_21);
			theApp.DisplayMsg( str );
			break;
		case	43:
			str = GetResString(IDS_SD_PROMPT_VAC_22);
			theApp.DisplayMsg( str );
			break;
		case	44:
			str = GetResString(IDS_SD_PROMPT_VAC_23);
			theApp.DisplayMsg( str );
			break;
		case	45:
			str = GetResString(IDS_SD_PROMPT_VAC_24);
			theApp.DisplayMsg( str );
			break;
		case	46:
			str = GetResString(IDS_SD_PROMPT_VAC_25);
			theApp.DisplayMsg( str );
			break;
		case	50:
			str = GetResString(IDS_SD_PROMPT_VAC_26);
			theApp.DisplayMsg( str );
			theApp.ResponseUrgency( 0xFF );
			break;
		}
		m_nVACMsgType	= lParam;
	}

	return 0;
}

LRESULT CDlgImpPanelDebug2::OnSMvacTCAmpUpdate(WPARAM wParam, LPARAM lParam)
{
	SMvac_TCAmp tcAmp = *((SMvac_TCAmp*)wParam);
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( lParam < 1 )
	{
		CVACDataManager::Instance().m_VacTCAmp.dTCfAmp = tcAmp.dTCfAmp;
		pfrm->m_dlgDebug.m_pPageMon->VAC_UpdateSMvacTCAmpUpdate( lParam, tcAmp.dTCfAmp );
		// 更新TCf进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCf)))->SetPos( (int)((tcAmp.dTCfAmp -CVACDataManager::Instance().m_VacTCAmp.dTCfMin) / (CVACDataManager::Instance().m_VacTCAmp.dTCfMax -CVACDataManager::Instance().m_VacTCAmp.dTCfMin) *100 ));
	}
	else
	{
		CVACDataManager::Instance().m_VacTCAmp.dTCcAmp = tcAmp.dTCcAmp;
		pfrm->m_dlgDebug.m_pPageMon->VAC_UpdateSMvacTCAmpUpdate( lParam, tcAmp.dTCcAmp );
		// 更新TCc进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCc)))->SetPos( (int)((tcAmp.dTCcAmp -CVACDataManager::Instance().m_VacTCAmp.dTCcMin) / (CVACDataManager::Instance().m_VacTCAmp.dTCcMax -CVACDataManager::Instance().m_VacTCAmp.dTCcMin) *100 ));
	}
/*
	double dR1 = CVACDataManager::Instance().m_VacTCAmp.dTCfAmp / CVACDataManager::Instance().m_VacTCAmp.dTCfMax;
	double dR2 = CVACDataManager::Instance().m_VacTCAmp.dTCcAmp / CVACDataManager::Instance().m_VacTCAmp.dTCcMax;
	if( dR1 > 0.95 && dR2 > 0.95 )
	{
		GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->SetWindowText( "GOOD" );
		CVACDataManager::Instance().m_VacTCAmp.nState = 2;
	}
	else if( dR1 < 0.5 || dR2 < 0.5 )
	{
		// 真空坏
		GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->SetWindowText( "BAD" );
		CVACDataManager::Instance().m_VacTCAmp.nState = 0;
	}
	else if( dR1 > 0.85 || dR2 > 0.85 )	// 1.1/1.31=0.83
	{
		// 有一个真空好了
		GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->SetWindowText( "NOT GOOD" );
		CVACDataManager::Instance().m_VacTCAmp.nState = 1;
	}
*/	return 0;
}

LRESULT CDlgImpPanelDebug2::OnVACUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	if( wParam < 10 )
	{
		CString strDevice, strStatus, str;
		switch( wParam )
		{
		case	3:
			strDevice = GetResString(IDS_SD_PROMPT_VAC_35);
			break;
		default:
			strDevice = GetResString(IDS_SD_PROMPT_VAC_36);
			break;
		}
		switch( lParam )
		{
		case	0:
			strStatus = GetResString(IDS_SD_PROMPT_VAC_37);
			break;
		case	1:
			strStatus = GetResString(IDS_SD_PROMPT_VAC_38);
			break;
		default:
			strStatus = GetResString(IDS_SD_PROMPT_VAC_39);
			break;
		}
		str.Format( GetResString(IDF_SD_PROMPT_VAC_40), CVACDataManager::Instance().m_nCCGconnects+1, strDevice, strStatus );
		theApp.DisplayMsg( str );

		switch( wParam )
		{
		case	3:
			CVACDataManager::Instance().m_cfgVacCCG.bReady	= lParam;
			if( CVACDataManager::Instance().m_cfgVacCCG.bReady )
			{
				KillTimer( 4 );
				CVACDataManager::Instance().m_nCCGconnects = 0;
				m_nCCG_Counts	= 0;
				m_bCCGBad		= FALSE;
			}
			break;
		}
	}
	return 0;
}

LRESULT CDlgImpPanelDebug2::OnVACUpdateCCGValue(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str.Format( "%.1fe%03d", 1.0*wParam/100, lParam );
	if( str.Left(3) == "0.0" )
		return 0;
	GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( str );
	double dVac = atof( str );	//theApp.ScientificToDouble( str );

//	if( (fabs(dVac -CVACDataManager::Instance().m_cfgVacCCG.dVac)) < EPSILON_E7 )
// 	{
// 		CVACDataManager::Instance().m_cfgVacCCG.dVac = dVac;
// 		return 0;
// 	}
	int nState = CVACDataManager::Instance().m_VacTCAmp.nState;
	if( dVac > CVACDataManager::Instance().m_cfgVacCCG.dThreshold2 )
		CVACDataManager::Instance().m_VacTCAmp.nState = 0;
	else if( dVac < CVACDataManager::Instance().m_cfgVacCCG.dThreshold1 )
		CVACDataManager::Instance().m_VacTCAmp.nState = 2;
	else
		CVACDataManager::Instance().m_VacTCAmp.nState = 1;

	m_nCCG_Counts++;
	if( m_nCCG_Counts > 100000 )	// 60*60*24 =86400
		m_nCCG_Counts = 0;
	if( dVac > CVACDataManager::Instance().m_cfgVacCCG.dThreshold2 )		// 本次真空度低于低点
	{
		if( !m_bCCGBad												// 第一次
			&& nState > 1													// 之前在真空好的状态
			&& CVACDataManager::Instance().m_cfgVacCCG.dVac < CVACDataManager::Instance().m_cfgVacCCG.dThreshold2 )	// 上次真空度高于低点
		{
			// 第一次监测到真空变坏
			m_bCCGBad = TRUE;
			theApp.DisplayMsg("CCG value BAD, once");
		}
		else if( m_bCCGBad
			&& ((m_nCCG_CountsBak+1 == m_nCCG_Counts) || (m_nCCG_CountsBak -m_nCCG_Counts > 99999)) )
		{
			// 连续第二次监测到真空变坏
			m_bCCGBad = FALSE;
			theApp.DisplayMsg("CCG value BAD, twice.  Shut down SPR En");
			SPR_En(FALSE);
		}
		else
			m_bCCGBad = FALSE;
	}
	else
	{
		if( m_bCCGBad )
			m_bCCGBad = FALSE;
	}
	m_nCCG_CountsBak	= m_nCCG_Counts;

	CVACDataManager::Instance().m_cfgVacCCG.dVac = dVac;
	if( nState != CVACDataManager::Instance().m_VacTCAmp.nState )
		Invalidate( FALSE );
	return 0;
}

void CDlgImpPanelDebug2::OnCHECKVacuumPauseRead() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_PauseRead)))->GetCheck();
	CSMvac::Instance().SetParams( 22, nCheck );
}

HBRUSH CDlgImpPanelDebug2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		int nID = pWnd->GetDlgCtrlID();
		if( nID == IDC_STATIC_STATUS_Vacuum_Unit )
		{
			if( CVACDataManager::Instance().m_VacTCAmp.nState < 2 )
			{
				pDC->SetTextColor(RGB(255,255,255));	// 设置文本颜色  
				pDC->SetBkMode(TRANSPARENT);			// 设置背景透明
				hbr = (HBRUSH)m_brushVacNotGood;
			}
			else
			{
				pDC->SetTextColor(RGB(255,255,255));	// 设置文本颜色  
				pDC->SetBkMode(TRANSPARENT);			// 设置背景透明
				hbr = (HBRUSH)m_brushVacGood;
			}
		}
	}

	return hbr;
}

void CDlgImpPanelDebug2::OnCHECKV1status() 
{
	// 模拟V1阀的开关状态，V1开时才允许开J14、J12
	m_bV1status = !m_bV1status;
	if( m_bV1status )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "You should be sure that";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "V1 has been openned!";
		if( dlg.DoModal() != IDOK )
			return;

		theApp.DisplayMsg("Press V1: OPEN");
		m_btnV1status.SetColor( g_crEmphasis1 );
		m_btnV1status.SetWindowText( "V1: Open" );//"V1 Close (Now Open)" );
		GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_HVOnCmd )->EnableWindow( TRUE );

		if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
		{
		}
		else
		{
			VAC_UpdateUIswitches();
			VAC_UpdatePneumaticDiagram();
		}
	}
	else
	{
		theApp.DisplayMsg("Press V1: CLOSE");
		m_btnV1status.SetColor( g_crOFF );
		m_btnV1status.SetWindowText( "V1: Close" );//"V1 Open (Now Close)" );
		GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_HVOnCmd )->EnableWindow( FALSE );

		if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
		{
		}
		else
		{
			VAC_UpdateUIswitches();
			VAC_UpdatePneumaticDiagram();
		}
	}

	CVACDataManager::Instance().CHECKV1status(m_bV1status);
}

void CDlgImpPanelDebug2::OnHVOn() 
{
	BOOL bHVOn = m_bHVOn;
	SPR_HVOn( !bHVOn );
}

void CDlgImpPanelDebug2::SPR_HVOn( BOOL bHVOn )
{
	if( m_bHVOn == bHVOn )
		return;

	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bHVOn= bHVOn;
	if( m_bHVOn )
	{
		m_btnHVOn.SetColor( g_crEmphasis4 );
		m_btnHVOn.SetWindowText( "J12: ON" );//"J12: HV Enable" );
	}
	else
	{
		m_btnHVOn.SetColor( g_crOFF );
		m_btnHVOn.SetWindowText( "J12: OFF" );//"J12: HV Disable" );
	}

	CControlLayer::Instance().SPR_HVOn(bHVOn);
}

void CDlgImpPanelDebug2::OnSPRRelay1En() 
{
	// TODO: Add your control notification handler code here
	BOOL bSPRRelay1En = m_bSPRRelay1En;
	SPR_En( !bSPRRelay1En );
}

void CDlgImpPanelDebug2::SPR_En( BOOL bEn )
{
	if( m_bSPRRelay1En == bEn )
		return;

	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bSPRRelay1En = bEn;
	theApp.m_blUnlock_HV = bEn;
	if( m_bSPRRelay1En )
	{
		theApp.DisplayMsg( "SPR_En: J14 vacuum interlock ON, cst & HV to init 0");
		m_btnSPRRelay1En.SetColor( g_crEmphasis3 );
		m_btnSPRRelay1En.SetWindowText( "J14: ON" );//"J14: Vacuum Interlock (Now ON)" );
		wData		= 0x82;		// Strobe IOX output @0A high
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

		// J14开时，允许加对比度，且设置初始值为0
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
		ClassID.lVal		= SemControlPanelClass_Contrast;
		SerialNumber.lVal	= -1;
		Enabled.lVal		= 1;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		ClassID.lVal		= SemControlPanelClass_HV;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

		SerialNumber.lVal	= -1;
		// Contrast to 0
		ClassID.lVal		= SemControlPanelClass_Contrast;
		Flag.lVal			= 1;
		Xpos.lVal			= 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		ClassID.lVal		= SemControlPanelClass_HV;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

		Sleep(1000);
		HVPower_Init();
	}
	else
	{
		theApp.DisplayMsg( "SPR_En: J14 vacuum interlock OFF, cst & HV to 0 and disable");
		m_btnSPRRelay1En.SetColor( g_crOFF );
		m_btnSPRRelay1En.SetWindowText( "J14: OFF" );//"J14: Vacuum Interlock (Now OFF)" );

		// J14关时，不允许加对比度，且恢复对比度值为0
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
		SerialNumber.lVal	= -1;
		// Contrast to 0
		ClassID.lVal		= SemControlPanelClass_Contrast;
		Flag.lVal			= 1;
		Xpos.lVal			= 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		ClassID.lVal		= SemControlPanelClass_HV;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

		ClassID.lVal		= SemControlPanelClass_Contrast;
		SerialNumber.lVal	= -1;
		Enabled.lVal		= 0;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		ClassID.lVal		= SemControlPanelClass_HV;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

		wData		= 0x42;		// Strobe IOX output @0A low
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

		{
			HVPower_SetEnable( 7, FALSE );
			HVPower_SetEnable( 6, FALSE );
			HVPower_SetEnable( 5, FALSE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, FALSE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, FALSE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, FALSE );
			CCPS3603tatusDetector::Instance().Release();
			
			m_btnHV.SetCheck(0);
			m_btnBias.SetCheck(0);
			m_btnFilament.SetCheck(0);
			m_btnVacuumSwitch.SetCheck(0);
			m_btnVacuumSwitch.SetColor( ::GetSysColor(COLOR_3DFACE) );
			VAC_VacuumReady( FALSE );
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CDlgImpPanelDebug2::OnCheckHV() 
{
	int nCheck = m_btnHV.GetCheck();
	HVPower_SetEnable( 5, nCheck );
}

void CDlgImpPanelDebug2::OnCheckBias() 
{
	int nCheck = m_btnBias.GetCheck();
	HVPower_SetEnable( 6, nCheck );
}

void CDlgImpPanelDebug2::OnCheckFilament() 
{
	int nCheck = m_btnFilament.GetCheck();
	HVPower_SetEnable( 7, nCheck );
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebug2::OnReLinkCCG() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press CCG: relink");
	if( CVACDataManager::Instance().m_cfgVacCCG.bReady )
	{
        CCCGStatusDetector::Instance().Release();
		Sleep(2000);
	}
	CVACDataManager::Instance().m_nCCGconnects = 0;
	CCCGStatusDetector::Instance().Start();
}

void CDlgImpPanelDebug2::OnReLinkHVpower() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press HV Power: relink");
	HVPower_Init();
}

void CDlgImpPanelDebug2::HVPower_Init()
{
	if( CCPS3603tatusDetector::Instance().IsReady() )
	{
		HVPower_SetEnable( 7, FALSE );
		HVPower_SetEnable( 6, FALSE );
		HVPower_SetEnable( 5, FALSE );
		HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, FALSE );
		HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, FALSE );
		HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, FALSE );

		m_btnHV.SetCheck(0);
		m_btnBias.SetCheck(0);
		m_btnFilament.SetCheck(0);
		m_btnVacuumSwitch.SetCheck(0);
		m_btnVacuumSwitch.SetColor( ::GetSysColor(COLOR_3DFACE) );
		VAC_VacuumReady( FALSE );
	}

	CHVPowerDataManager::Instance().HVPower_Init(this);
}

void CDlgImpPanelDebug2::HVPower_Reset()
{
	{
		HVPower_SetEnable( 7, FALSE );
		HVPower_SetEnable( 6, FALSE );
		HVPower_SetEnable( 5, FALSE );

		m_btnHV.SetCheck(0);
		m_btnBias.SetCheck(0);
		m_btnFilament.SetCheck(0);
		m_btnVacuumSwitch.SetCheck(0);
		m_btnVacuumSwitch.SetColor( ::GetSysColor(COLOR_3DFACE) );
		VAC_VacuumReady( FALSE );
	}
}

LRESULT CDlgImpPanelDebug2::OnCPS3603statusChanged(WPARAM wParam, LPARAM lParam)
{
	if( wParam == 0x01 )
	{
		HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, FALSE );
		HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, FALSE );
		HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, FALSE );
	}
	else if( wParam == 0x02 )
	{
		CHVPowerDataManager::Instance().m_CPS3603s = *((CPS3603_Status*)lParam);
		dlgprintf(IDC_STATIC_STATUS_Pressure_Gun, "%04x:%04x:%04x", CHVPowerDataManager::Instance().m_CPS3603s.nVerA, CHVPowerDataManager::Instance().m_CPS3603s.nVerE, CHVPowerDataManager::Instance().m_CPS3603s.nVerF);

		if( theApp.m_blOverhaulState )
		{
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, TRUE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, TRUE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, TRUE );
		}
		else
		{
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, (CVACDataManager::Instance().m_VacParamMain.bHVInt && m_bSPRRelay1En && CHVPowerDataManager::Instance().m_CPS3603s.nConnectA ) );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, (CVACDataManager::Instance().m_VacParamMain.bHVInt && m_bSPRRelay1En && CHVPowerDataManager::Instance().m_CPS3603s.nConnectE ) );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, (CVACDataManager::Instance().m_VacParamMain.bHVInt && m_bSPRRelay1En && CHVPowerDataManager::Instance().m_CPS3603s.nConnectF ) );
		}
		if(CVACDataManager::Instance().m_VacParamMain.bHVInt && m_bSPRRelay1En && CHVPowerDataManager::Instance().m_CPS3603s.nConnectA && CHVPowerDataManager::Instance().m_CPS3603s.nConnectE && CHVPowerDataManager::Instance().m_CPS3603s.nConnectF )
		{
			m_btnVacuumSwitch.EnableWindow( TRUE );
			VAC_VacuumReady( TRUE );
		}
	}

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_CPS_Status, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelDebug2::OnCPS3603msgChanged(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_CPS_MSG, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelDebug2::OnCPS3603paramChanged(WPARAM wParam, LPARAM lParam)
{
	// 通讯一次，读一次各路回读值。将所有回读值放在一个数据结构体中上传给主程序
	CHVPowerDataManager::Instance().m_CPS3603 = *((CPS3603_Param*)wParam);
	dlgprintf(IDC_STATIC_STATUS_EMI, "%5.3lf", CHVPowerDataManager::Instance().m_CPS3603.dIoutA * 1e6);//CHVPowerDataManager::Instance().m_CPS3603s.dIoutF);

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_CPS_Param, wParam, lParam);

	static DWORD dwCPS3603 = 0;
	DWORD dwTicks = GetTickCount();
	if( (CHVPowerDataManager::Instance().m_CPS3603.dIoutF < 0.5) && (CHVPowerDataManager::Instance().m_CPS3603.dVoutF > 5)
		&& (dwTicks -dwCPS3603 > 10000) )
	{
		dwCPS3603 = dwTicks;
		// 灯丝断了
		CCPS3603tatusDetector::Instance().SetParam(21,AS_NULL);
		CString str;
		str.Format( "%s %s", GetResString(IDS_SD_PROMPT_FilaBroken), GetResString(IDS_SD_PROMPT_VAC_01) );
		__int64 timestamp = GetCurrentTimeStampMS();
		CAlarmManager::Instance().AddAlarm(AlarmType::Alarm_FilaBroken,timestamp,string(str.GetBuffer()));
		AfxMessageBox( str );
	}

	return 0;
}

// Printf to a dialog item
void CDlgImpPanelDebug2::dlgprintf( int id, char *fmt, ... )
{
	va_list argptr;
	static char str[256];
	
	va_start(argptr, fmt);
	vsprintf(str, fmt, argptr);
	SetDlgItemText(id, str);
	va_end(argptr);
}

LRESULT CDlgImpPanelDebug2::OnCPS3603AutoParamChanged(WPARAM wParam, LPARAM lParam)
{
	CPS3603_AutoState AutoState	= (CPS3603_AutoState)wParam;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	Flag.ulVal	= (DWORD)0x01;
	if( AutoState < AS_Rise_Bias )
	{
		// 同步高压控件位置及显示值
		ClassID.lVal		= SemControlPanelClass_CPS_HV;
		SerialNumber.lVal	= -1;
		if( lParam < 10000 )
		{
			Xpos.lVal	= (LONG)(lParam /100);
			strX.Format( "%.1f KV", lParam /1000.0 );
		}
		else
		{
			Xpos.lVal	= (LONG)(lParam /1000 -10 +100);
			strX.Format( "%d KV", lParam /1000 );
		}
		m_ctrlCPS_HV.SyncPos( Xpos, Ypos, Flag );
		Flag.lVal	= 1;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		m_ctrlCPS_HV.SyncText( strX, strY, Flag );
	}
	else if( AutoState < AS_Rise_Fila )
	{
		// 同步偏压控件位置及显示值
		Xpos.lVal		= (LONG)lParam;			// 500V = 500 steps
		m_ctrlCPS_Bias.SyncPos( Xpos, Ypos, Flag );
		strX.Format( "%.1f V", Xpos.lVal *1.0 );
		m_ctrlCPS_Bias.SyncText( strX, strY, Flag );
		if( theApp.m_boolHV )
		{
			strY.Format( GetResString(IDF_SD_SemControlPanelClassID_HVAccelerate), SemControlPanelClass_CPS_Bias);
			strX = strY + _T(" ") + strX;
			theApp.DisplayMsg( strX );
		}
	}
	else if( AutoState < AS_NULL )
	{
		// 同步灯丝控件位置及显示值
		Xpos.lVal		= (LONG)(lParam /10);	// 3000mA = 300 steps
		m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );
		strX.Format( "%.3f A", Xpos.lVal /100.0 );
		m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );
		if( CAppLogManager::Instance().m_boolFilamentCurrentLog )
		{
			strY.Format( GetResString(IDF_SD_SemControlPanelClassID_FilamentCurrent), SemControlPanelClass_CPS_FilamentCurrent);
			strX = strY + _T(" ") + strX;
			theApp.DisplayMsg( strX );
		}
	}
	else
	{
		// 自动加灯丝至1.9A已完成，启动自动灯丝调节（找饱和点）
		CScanView* pView = (CScanView*)theApp.GetScanView();
		if( CScanParameterSaver::Instance().m_nAutoFuction == AutoNULL )
		{
			pView->FuncAutoFilament();
			SetTimer(13, 500, NULL);
		}
	}
	return 0;
}

void CDlgImpPanelDebug2::HVPower_SyncEnable( int nID, BOOL bEnable )
{
	if( m_hWnd == NULL )
		return;

	int nCheck = 0;
	VARIANT Enable, Flag;
	Flag.lVal	= 3;
	switch( nID )
	{
	case	IDC_CHECK_IMP_PANEL_HV:
		m_btnHV.EnableWindow( bEnable );
		if( !(theApp.m_blOverhaulState) )
		{
			nCheck = m_btnHV.GetCheck();
			Enable.lVal	= bEnable & nCheck;
			m_ctrlCPS_HV.Enable(Enable, Flag);
		}
		break;
	case	IDC_CHECK_IMP_PANEL_Bias:
		m_btnBias.EnableWindow( bEnable );
		if( !(theApp.m_blOverhaulState) )
		{
			nCheck = m_btnBias.GetCheck();
			Enable.lVal	= bEnable & nCheck;
			m_ctrlCPS_Bias.Enable(Enable, Flag);
		}
		break;
	case	IDC_CHECK_IMP_PANEL_Filament:
		m_btnFilament.EnableWindow( bEnable );
		if( !(theApp.m_blOverhaulState) )
		{
			nCheck = m_btnFilament.GetCheck();
			Enable.lVal	= bEnable & nCheck;
			m_ctrlCPS_FilamentCurrent.Enable(Enable, Flag);
		}
		break;
	}
}

void CDlgImpPanelDebug2::HVPower_SetEnable( int nID, int nCheck )
{
	VARIANT Xpos, Ypos, Enable, Flag, ClassID, SerialNumber;
	CString strX, strY;
	Xpos.lVal = 0;
	Enable.lVal	= nCheck;
	switch( nID )
	{
	case	5:
		if( nCheck )
		{
			m_btnHV.SetColor( RGB(192,0,0) );
		}
		else
		{ 
			m_btnHV.SetColor( ::GetSysColor(COLOR_3DFACE) );
			Flag.ulVal	= (DWORD)0x01;
			m_ctrlCPS_HV.SyncPos( Xpos, Ypos, Flag );

			ClassID.lVal		= SemControlPanelClass_CPS_HV;
			SerialNumber.lVal	= -1;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;

			strX = "0.0 KV";
			m_ctrlCPS_HV.SyncText( strX, strY, Flag );
		}
		Flag.lVal	= 3;
		m_ctrlCPS_HV.Enable(Enable, Flag);
		break;

	case	6:
		if( nCheck )
		{
			m_btnBias.SetColor( RGB(0,192,0) );
		}
		else
		{
			m_btnBias.SetColor( ::GetSysColor(COLOR_3DFACE) );
			Flag.ulVal	= (DWORD)0x01;
			strX = "0.0 V";
			m_ctrlCPS_Bias.SyncPos( Xpos, Ypos, Flag );
			m_ctrlCPS_Bias.SyncText( strX, strY, Flag );
		}
		Flag.lVal	= 3;
		m_ctrlCPS_Bias.Enable(Enable, Flag);
		break;

	case	7:
		if( nCheck )
		{
			m_btnFilament.SetColor( RGB(128,128,0) );
		}
		else
		{
			m_btnFilament.SetColor( ::GetSysColor(COLOR_3DFACE) );
			Flag.ulVal	= (DWORD)0x01;
			strX = "0.000 A";
			m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );
			m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );
		}
		Flag.lVal	= 3;
		m_ctrlCPS_FilamentCurrent.Enable(Enable, Flag);
		break;

	default:
		break;
	}

	CScanView* pView = (CScanView*)theApp.GetScanView();
	ASSERT(pView);
	CHVPowerDataManager::Instance().HVPower_SetEnable(nID, nCheck, CScanParameterManager::Instance().m_nusbScanType, theApp.m_blOverhaulState);
}


void CDlgImpPanelDebug2::OnCHECKReadVACmprsAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_ReadVACmprsAuto )))->GetCheck();
	if( nCheck )
		SetTimer( nMCI_ReadStatusTimerID, 2000, NULL );
	else
		KillTimer( nMCI_ReadStatusTimerID );
}

void CDlgImpPanelDebug2::OnReadComprsCmd() 
{
	VAC_ReadComprs();
}

void CDlgImpPanelDebug2::VAC_ReadComprs()
{
	unsigned short data[2];
	if( CSemCtrl::Instance().Serial_ReadbackData( 3, USB_CID_VAC_ReadThermoCCGComp, data) )
	{
		// 4th readback word (7th and 8th ASCII characters) contains Comparator data
		WORD wData, wRBit, wMskBit;
		wData = (WORD)data[0];	// 0xD22D后面的数就是需要的状态数据
		for( wRBit=0; wRBit<14; wRBit += 2 )
		{
			wMskBit = (WORD)(pow((double)2, wRBit) );	// Odd numbered bits are Upper trip point comparator states
			if( wData & wMskBit )
				GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "1" );
			else
				GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "0" );

			wMskBit *= 2;					// Shift up one bit to check Lower trip point comparator state
			if( wData & wMskBit )
				GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "1" );
			else
				GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "0" );
		}
	}
}

void CDlgImpPanelDebug2::UpdateReadbackData( unsigned short* pnData, DWORD& dwRegister )
{
	unsigned long lCount = pnData[0];
	for( unsigned long i=1; i<=lCount; i++ )
	{
		// 直到找到0xD22D才开始计数，前边的数据忽略
		if( pnData[i] == 0xD22D )
		{
			// 0xD22D后面的数就是需要的状态数据
			if( i+1 <= lCount )
			{
				dwRegister = pnData[i +1];
				return;
			}
		}
	}
}

void CDlgImpPanelDebug2::OnCHECKFPGArelayEn1() 
{
	// 前级阀
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn1)))->GetCheck();
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
	{

	}
	else
	{
		VAC_UpdateUIswitches();
		VAC_UpdatePneumaticDiagram();
	}

	CVACDataManager::Instance().CHECKFPGArelayEn1(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKFPGArelayEn2() 
{
	// 枪阀
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn2)))->GetCheck();
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
	{
	}
	else
	{
		VAC_UpdateUIswitches();
		VAC_UpdatePneumaticDiagram();
	}

	CVACDataManager::Instance().CHECKFPGArelayEn2(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKFPGArelayEn56() 
{
	// 分子泵
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn5)))->GetCheck();
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
	{
	}
	else
	{
		VAC_UpdateUIswitches();
		VAC_UpdatePneumaticDiagram();
	}

	CVACDataManager::Instance().CHECKFPGArelayEn56(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKFPGArelayEn4() 
{
	// 放气阀
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_FPGArelayEn4)))->GetCheck();
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
	{
	}
	else
	{
		VAC_UpdateUIswitches();
		VAC_UpdatePneumaticDiagram();
	}

	CVACDataManager::Instance().CHECKFPGArelayEn4(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKFPGArelayEn11() 
{
	// TODO: Add your control notification handler code here
	CControlLayer::Instance().CHECKFPGArelayEn11();
}

void CDlgImpPanelDebug2::OnCHECKVacuumTCf() 
{
	// 前级真空
	int nCheckf = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCf)))->GetCheck();
	int nCheckc = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCc)))->GetCheck();
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
	{
	}
	else
	{
		VAC_UpdateUIswitches();
		VAC_UpdatePneumaticDiagram();
	}

	CVACDataManager::Instance().CHECKVacuumTCf(nCheckf, nCheckc);
}

void CDlgImpPanelDebug2::OnCHECKVacuumTCc() 
{
	// 镜筒真空
	int nCheckf = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCf)))->GetCheck();
	int nCheckc = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCc)))->GetCheck();
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
	{
	}
	else
	{
		VAC_UpdateUIswitches();
		VAC_UpdatePneumaticDiagram();
	}

	CVACDataManager::Instance().CHECKVacuumTCc(nCheckf, nCheckc);
}


void CDlgImpPanelDebug2::OnOK() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgImpPanelDebug2::OnCancel() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CDlgImpPanelDebug2::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_Det45:
		case	IDC_EDIT_Det45Y:
		case	IDC_EDIT_DetGain:
		case	IDC_EDIT_DetGain2:
		case	IDC_EDIT_BW_sel2:
			GetDlgItem( nID )->GetWindowText( str );
			break;
		default:
			break;
		}
		if( str.IsEmpty() )
			return CDialog::PreTranslateMessage(pMsg);
		int nPos;
		char *bufNum = str.GetBuffer(4);
		sscanf(bufNum, "%x", &nPos);
		switch( nID )
		{
		case	IDC_EDIT_Det45:
			m_scrollDet45.SetScrollPos( nPos );
			MCI_Det45SB_Change( nPos );
			break;
		case	IDC_EDIT_Det45Y:
			m_scrollDet45Y.SetScrollPos( nPos );
			MCI_Det45SBY_Change( nPos );
			break;
		case	IDC_EDIT_DetGain:
			m_scrollDetGain.SetScrollPos( nPos );
			MCI_DetGainSB_Change( nPos );
			break;
		case	IDC_EDIT_DetGain2:
			m_scrollDetGain2.SetScrollPos( nPos );
			MCI_DetGain2SB_Change( nPos );
			break;
		case	IDC_EDIT_BW_sel2:
			m_scrollBWsel.SetScrollPos( nPos );
			MCI_BWselSB_Change( nPos );
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


// FineMag and Gain
void CDlgImpPanelDebug2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int nOldPos = pScrollBar->GetScrollPos();
	int nNewPos = pScrollBar->GetScrollPos();
	SCROLLINFO  scrollInfo;
	pScrollBar->GetScrollInfo( &scrollInfo, SIF_ALL );
	switch (nSBCode) 
	{
	 case SB_LINELEFT:
		nNewPos = nOldPos-1;								//步进量为1
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	 case SB_LINERIGHT:
		nNewPos = nOldPos+1;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	 case SB_LEFT:
		pScrollBar->SetScrollPos( scrollInfo.nMax );
		break;

	 case SB_RIGHT:
		pScrollBar->SetScrollPos( scrollInfo.nMin );
		break;

	 case SB_PAGELEFT:
		nNewPos = nOldPos-10;//scrollInfo.nPage;			//页进量为10
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	 case SB_PAGERIGHT:
		nNewPos = nOldPos+10;//scrollInfo.nPage;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	 case SB_ENDSCROLL:
		break;

	 case SB_THUMBPOSITION:
		pScrollBar->SetScrollPos( nPos );
		break;

	 case SB_THUMBTRACK:
		pScrollBar->SetScrollPos( scrollInfo.nTrackPos );
		break;
	}

	nNewPos = pScrollBar->GetScrollPos();
	if( nOldPos == nNewPos )
		return;

	int id = pScrollBar->GetDlgCtrlID();					//获取滚动条ID
	switch( id )
	{
	case IDC_SCROLLBAR_Det45:
		MCI_Det45SB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Det45Y:
		MCI_Det45SBY_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_DetGain:
		MCI_DetGainSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_DetGain2:
		MCI_DetGain2SB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_BW_sel:
		MCI_BWselSB_Change( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgImpPanelDebug2::OnWriteRateX() 
{
	theApp.DisplayMsg("Press write FineMagX");
	CString str = "";
	GetDlgItem( IDC_EDIT_Det45 )->GetWindowText( str );
	if( str.IsEmpty() )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "NO data to write!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	if( m_nApproxMag < 0 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "NO selected stage!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}

	double dMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	if( (dMag < 249.5 && m_nApproxMag != 0)
		|| (dMag > 249.5 && dMag < 2495 && m_nApproxMag != 1)
		|| (dMag > 2495 && dMag < 24950 && m_nApproxMag != 2)
		|| (dMag > 24950 && dMag < 59950 && m_nApproxMag != 3)
		|| (dMag > 59950 && m_nApproxMag != 4) )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "stage NOT MATCH mag!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	
	int nPos;
	char *bufNum = str.GetBuffer(4);
	sscanf(bufNum, "%x", &nPos);
	double dPos = 1.0*nPos;
	if( dMag < 249.5 )
		dPos = dMag / 15.0 * dPos;
	else if( dMag > 249.5 && dMag < 2495 )
		dPos = dMag / 250 * dPos;
	else if( dMag > 2495 && dMag < 24975 )
		dPos = dMag / 2500 * dPos;
	else if( dMag > 24975 && dMag < 59950 )
		dPos = dMag / 25000 *dPos;
	else
		dPos = dMag / 60000 *dPos;
	CCFGFileManager::Instance().WriteINI( INI_Ratio, INIIndex_MagK, &dPos, m_nApproxMag, 1);

	// 15.08.03 记录到文件
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strCoadjustFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d");
	strCoadjustFileName.Format( "%sCoadjustMag-%s.log", appfilepath, strTime );
	CFile file;
	file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite );

	if( file.m_hFile != CFile::hFileNull )
	{
		strTime = timeCurrent.Format(IDF_MessageTimeFormat);
		CString csMessage;
		csMessage.Format( "[%s] ApproxMag = %d, Mag = %.2f, MagX = 0x%s\r\n", strTime, m_nApproxMag, dMag, str );
		file.SeekToEnd();
		file.Write( csMessage, csMessage.GetLength() );
	}
	file.Close();
}

void CDlgImpPanelDebug2::OnWriteRateY() 
{
	theApp.DisplayMsg("Press write FineMagY");
	CString str = "";
	GetDlgItem( IDC_EDIT_Det45Y )->GetWindowText( str );
	if( str.IsEmpty() )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "NO data to write!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	if( m_nApproxMag < 0 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "NOT select stage!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	
	double dMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	if( (dMag < 249.5 && m_nApproxMag != 0)
		|| (dMag > 249.5 && dMag < 2495 && m_nApproxMag != 1)
		|| (dMag > 2495 && dMag < 24950 && m_nApproxMag != 2)
		|| (dMag > 24950 && dMag < 59950 && m_nApproxMag != 3)
		|| (dMag > 59950 && m_nApproxMag != 4) )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "stage NOT MATCH mag!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	
	int nPos;
	char *bufNum = str.GetBuffer(4);
	sscanf(bufNum, "%x", &nPos);
	double dPos = 1.0*nPos;
	if( dMag < 249.5 )
		dPos = dMag / 15.0 * dPos;
	else if( dMag > 249.5 && dMag < 2495 )
		dPos = dMag / 250 * dPos;
	else if( dMag > 2495 && dMag < 24975 )
		dPos = dMag / 2500 * dPos;
	else if( dMag > 24975 && dMag < 59950 )
		dPos = dMag / 25000 *dPos;
	else
		dPos = dMag / 60000 *dPos;
	CCFGFileManager::Instance().WriteINI( INI_Ratio, INIIndex_MagK, &dPos, m_nApproxMag, 2 );

	// 15.08.03 记录到文件
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strCoadjustFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d");
	strCoadjustFileName.Format( "%sCoadjustMag-%s.log", appfilepath, strTime );
	CFile file;
	file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite );

	if( file.m_hFile != CFile::hFileNull )
	{
		strTime = timeCurrent.Format(IDF_MessageTimeFormat);
		CString csMessage;
		csMessage.Format( "[%s] ApproxMag = %d, Mag = %.2f, MagY = 0x%s\r\n", strTime, m_nApproxMag, dMag, str );
		file.SeekToEnd();
		file.Write( csMessage, csMessage.GetLength() );
	}
	file.Close();
}

void CDlgImpPanelDebug2::OnWriteOLctrl() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press write OL ctrl");
	if( CControlLayer::Instance().m_Cali.nCaliCoCounts > 9 )
	{
		AfxMessageBox( "Counts Exceeded !" );
		return;
	}
	CDlgInput dlg;
	if( dlg.DoModal() == IDOK )
	{
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm != NULL && pfrm->m_dlgCali.m_hWnd != NULL )
		{
			// 获取工作路径
			char path[256];
			::GetModuleFileName(0, path, 255);
			char *exd = strrchr(path, (int)'\\');
			if(exd != NULL)
				*exd = '\0';
			strcat(path, "\\KYKYcfg.ini");

			/////////////////////////////////////////////////////////////
			// 15.05.14 韩勇说在拟合新统调公式时，3200的拟合效果偏差较大
			// 所以有可能6200还使用之前的统调公式
			// 故将m_nType做调整，原来为0代表6200,1代表6900
			// 现改为：	0代表6200且使用原统调公式
			//			1代表6200且使用新统调公式(用原3200统调表，用新公式拟合出固定系数)
			//			11代表6200且使用新标高样品动态拟合新公式的系数
			//			12代表6900(使用新标高样品动态拟合新公式的系数)
			// 判断电镜型号时除原有注册表项目，暂时使用ini中的[Debug]/Start来区分6200的新老统调公式类型
			// 该ini项目原来用于物镜显示值的6X00-->3X00的映射，早已不用
			/////////////////////////////////////////////////////////////
			char szBuf[20];
			int nType = 0;
			int nPos = theApp.m_strLogo.Find( "6" );
			CString strType = theApp.m_strLogo.Right( theApp.m_strLogo.GetLength() - nPos );
			if( strType == "6200" )
			{
				int nLeastSquares_power = 1;
				if(GetPrivateProfileString( "Coadjust", "power", "1", szBuf, 255, path) != 0)
					nLeastSquares_power = atoi( szBuf );
				if( nLeastSquares_power < 2 )	// 直线方程
					nType	= 0;
				else
				{
					if(GetPrivateProfileString("Debug", "Start", "", szBuf, 15, path) != 0)
					{
						int nStart = atoi(szBuf);
						if( nStart < 1 )
							nType = 1;	// 使用二次多项式拟合统调公式的6200 15.07.22
						else
							nType = 11;	// 使用二次多项式动态拟合统调公式的6200 15.07.22
					}
					else
						nType	= 1;
				}
			}
			else
				nType = 12;	// 使用二次多项式动态拟合统调公式的6900 15.07.22

			if( nType < 1 )
			{
				// 原来的6200
				VARIANT varPos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
				CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = varPos.lVal +32768;
			}
			else
			{
				// 6900和新6200
				double dLength = 3.04;
				if( GetPrivateProfileString("Coadjust", "Length", "", szBuf, 15, path) != 0 )
					dLength = atof( szBuf );
				// OL disp
				VARIANT varPos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
				CString strText = CSemCoreAssemble::Instance().m_ObjectiveLens.FormatText( varPos );
				CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = atof( strText );
				// Mag min = 标尺长度显示值D(uint:mm) * 放大倍数显示值Mag / 3
				CString strUScale = CSemVirtualMCICard::Instance().GetMobileUScaleText();
				CString strUnit = strUScale.Right(2);
				strText = strUScale.Left( strUScale.GetLength() -2 );
				double dUScale = atof( strText );
				if( strUnit == "μm" )
					dUScale /= 1000.0;
				else if( strUnit == "nm" )
					dUScale /= 1000000.0;
				else if( strUnit == "cm" )
					dUScale *= 10;
				varPos = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
				CControlLayer::Instance().m_Cali.dCaliCoY[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = dUScale * varPos.dblVal /dLength;
			}
			CControlLayer::Instance().m_Cali.dCaliCoWD[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = atof( dlg.m_strInput );
			CControlLayer::Instance().m_Cali.bCaliCoSel[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = TRUE;
			CControlLayer::Instance().m_Cali.nCaliCoCounts++;
			pfrm->m_dlgCali.UpdateCaliData();

			// 15.08.03 记录到文件
			char appfilepath[255];
			::GetModuleFileName( NULL, appfilepath, 255 );
			char *ptemp = strrchr( appfilepath, '\\');
			ptemp[1] = 0;
			CString strCoadjustFileName;
			CTime	timeCurrent = CTime::GetCurrentTime();
			CString strTime = timeCurrent.Format("%Y%m%d");
			strCoadjustFileName.Format( "%sCoadjustMag-%s.log", appfilepath, strTime );
			CFile file;
			file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite );

			if( file.m_hFile != CFile::hFileNull )
			{
				strTime = timeCurrent.Format(IDF_MessageTimeFormat);
				CString csMessage;
				double dX = CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ];
				if( nType < 1 )
					dX = (CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ] -32768) / 32.0;
				csMessage.Format( "[%s] WD = %.1f, CoX = %.6f, CoY = %.6f\r\n", strTime,
									CControlLayer::Instance().m_Cali.dCaliCoWD[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ],
									dX,
									CControlLayer::Instance().m_Cali.dCaliCoY[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ]);
				file.SeekToEnd();
				file.Write( csMessage, csMessage.GetLength() );
			}
			file.Close();
		}
	}
}

void CDlgImpPanelDebug2::OnWriteOneKey() 
{
	theApp.DisplayMsg("Press Write ONEKEY: save param as default");
	// 将当前参数值写入配置文件，作为一键看像时的默认值
	// [OneKEY]
	// HV,Bias,Fila,B,C,CL,OL,Mag
	// param=30000,0,190,-32,144,13568,17600,1000
	int	nB		= CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal;
	int	nC		= CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal;
	int	nCL		= CSemCoreAssemble::Instance().m_CondenserLens.GetPos().lVal;
	int	nOL		= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
	int	nMag	= (int)(CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal);
	int nHV, nBias, nFila, nFilaDefault = 160;
	CCPS3603tatusDetector::Instance().GetParam( 10, nHV );
	CCPS3603tatusDetector::Instance().GetParam( 11, nBias );
	CCPS3603tatusDetector::Instance().GetParam( 12, nFila );
	if( nFila < 160 )
		nFila = nFilaDefault;

	CString strParam;
	CDlgPrompt dlg;
	dlg.m_byteType = (BYTE)0x04;
	dlg.m_strMsg = "Save CL/OL/Mag parameter also?";
	int nRet = dlg.DoModal();
	switch( nRet )
	{
	case	IDYES:
		// 存所有当前参数
		strParam.Format( "%d,%d,%d,%d,%d,%d,%d,%d", nHV, nBias, nFila, nB, nC, nCL, nOL, nMag );
		break;
	case	IDNO:
		// 只存HV,Bias,Fila,B,C
		strParam.Format( "%d,%d,%d,%d,%d,13568,17600,1000", nHV, nBias, nFila, nB, nC );
		break;
	default:
		break;
	}
	if( strParam.IsEmpty() )
		return;

	// 获取工作路径
	TCHAR exeFullPath[255];
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	char path[255];
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	// 写入配置文件
	WritePrivateProfileString( "OneKEY", "param", strParam, path );

	char		stabuff[20];
	char		staitembuff[20];
	int			nTotal	= 4;
	int*		pCheck	= new int[4];
	sprintf(stabuff, "Auto");
	sprintf(staitembuff, "Fila");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	pCheck[0]	= nFila;
	CCommonFunctionManager::Instance().SaveInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	delete [] pCheck;
}

void CDlgImpPanelDebug2::MCI_Det45SB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	CString str = "";

	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );

	CControlLayer::Instance().MCI_Det45SB_Change(nPos);
}

void CDlgImpPanelDebug2::MCI_Det45SBY_Change( int nPos )
{
	int nValue = nPos;
	CString str = "";	
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_Det45Y )->SetWindowText( str );

	CControlLayer::Instance().MCI_Det45SBY_Change(nPos);
}

void CDlgImpPanelDebug2::MCI_SyncDet45( int nPosX, int nPosY )
{
	CString str;
	str.Format( "%X", nPosX );
	GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );
	m_scrollDet45.SetScrollPos( nPosX );
	str.Format( "%X", nPosY );
	GetDlgItem( IDC_EDIT_Det45Y )->SetWindowText( str );
	m_scrollDet45Y.SetScrollPos( nPosY );
}

void CDlgImpPanelDebug2::MCI_DetGainSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	unsigned long DIOData = 0;
	CString str = "";

	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );

	CControlLayer::Instance().MCI_DetGainSB_Change(nPos);
}

void CDlgImpPanelDebug2::MCI_DetGain2SB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	unsigned long DIOData = 0;
	CString str = "";

	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_DetGain2 )->SetWindowText( str );

	CControlLayer::Instance().MCI_DetGain2SB_Change(nPos);
}

void CDlgImpPanelDebug2::MCI_BWselSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	unsigned long DIOData = 0;
	CString str = "";

	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );

	CControlLayer::Instance().MCI_BWselSB_Change(nPos);
}

void CDlgImpPanelDebug2::OnCHECKDet2nd() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Det2nd)))->GetCheck();
	if( nCheck )
		theApp.DisplayMsg("Press 2nd: select 1,3,5 chn");
	else
		theApp.DisplayMsg("Press 2nd: select 0,2,4 chn");

	CControlLayer::Instance().CheckDet2nd( nCheck );
}

void CDlgImpPanelDebug2::OnDetPairSelect(UINT nID) 
{
	WORD wData = 0x0;
	int nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->GetCheck();
	if( nCheck )
	{
		wData = 0x00;
		theApp.DisplayMsg("Press det pair: select 01");
	}
	else
	{
		nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_23 )))->GetCheck();
		if( nCheck )
		{
			wData = 0x01;
			theApp.DisplayMsg("Press det pair: select 23");
		}
		else
		{
			nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_45 )))->GetCheck();
			if( nCheck )
			{
				wData = 0x02;
				theApp.DisplayMsg("Press det pair: select 45");
			}
		}
	}

	CControlLayer::Instance().SetDetPair(wData);
	MCI_SyncParams();
}

void CDlgImpPanelDebug2::MCI_SyncParams()
{
	// Det Pair
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_23 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_45 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 + CControlLayer::Instance().m_BrdMCI.nDetPair )))->SetCheck(1);

	// Det 2nd
	((CButton*)(GetDlgItem(IDC_CHECK_Det2nd)))->SetCheck( CControlLayer::Instance().m_BrdMCI.bDet2nd );

	CControlLayer::Instance().MCI_Sync_BrightnessParam();

	CString str;
	int nPos;
	// Gain
	nPos = CControlLayer::Instance().m_BrdMCI.nGain[ 2 * CControlLayer::Instance().m_BrdMCI.nDetPair ];
	m_scrollDetGain.SetScrollPos( nPos );
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );
	nPos = CControlLayer::Instance().m_BrdMCI.nGain[ 2 * CControlLayer::Instance().m_BrdMCI.nDetPair +1 ];
	m_scrollDetGain2.SetScrollPos( nPos );
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_DetGain2 )->SetWindowText( str );
	// Filter
	nPos = CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair];
	m_scrollBWsel.SetScrollPos( nPos );
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );
}

void CDlgImpPanelDebug2::OnCHECKMainsLock() 
{
	// Toggle MainsLock
	int nCheck = m_btnMainsLock.GetCheck();
	if( nCheck )
	{
		m_btnMainsLock.SetColor( g_crEmphasis1 );
		m_btnMainsLock.SetWindowText( "MainsLock ON" );
	}
	else
	{
		m_btnMainsLock.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnMainsLock.SetWindowText( "MainsLock OFF" );
	}

	CControlLayer::Instance().CHECKMainsLock(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKVideo() 
{
	// Toggle Video
	int nCheck = m_btnVideo.GetCheck();
	if( nCheck )
	{
		m_btnVideo.SetColor( g_crEmphasis2 );
		m_btnVideo.SetWindowText( "Video ON" );
	}
	else
	{
		m_btnVideo.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnVideo.SetWindowText( "Video OFF" );
	}

	CControlLayer::Instance().CHECKVideo(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKNeedMon() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_NeedMon )))->GetCheck();
	SetNeedMon( nCheck );
}

void CDlgImpPanelDebug2::SetNeedMon( int nCheck )
{
	CSemCtrl::Instance().m_SemScan.USB_SetParams( SET_PARAMS_NeedMon, !nCheck );
}

void CDlgImpPanelDebug2::OnCHECKDebugModeCoadjust() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Coadjust )))->GetCheck();
	if( nCheck )
	{
		// 可以使用Write按钮
		GetDlgItem( IDC_BUTTON_WriteRateX )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_BUTTON_WriteRateY )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_BUTTON_WriteOLctrl )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_BUTTON_WriteOneKey )->ShowWindow( SW_SHOW );
	}
	else
	{
		GetDlgItem( IDC_BUTTON_WriteRateX )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_BUTTON_WriteRateY )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_BUTTON_WriteOLctrl )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_BUTTON_WriteOneKey )->ShowWindow( SW_HIDE );
	}

	CControlLayer::Instance().CHECKDebugModeCoadjust(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKDebugZ()
{
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Z )))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams( 5, nCheck );
}

void CDlgImpPanelDebug2::OnCHECKDebugOsc() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Osc )))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams( 6, nCheck );
}


// SCN
// Mag

void CDlgImpPanelDebug2::OnApproxMagChanged( UINT nID )
{
	int nIndex = nID -IDC_RADIO_Approx_Mag_10;
	switch( nIndex )
	{
	case	0:
		CControlLayer::Instance().SCN_AmpMain();
		m_nApproxMag = 0;
		break;
	case	1:
		CControlLayer::Instance().SCN_AmpLow1();
		m_nApproxMag = 1;
		break;
	case	2:
		CControlLayer::Instance().SCN_AmpLow2();
		m_nApproxMag = 2;
		break;
	case	3:
		CControlLayer::Instance().SCN_AmpLow3();
		m_nApproxMag = 3;
		break;
	case	4:
		CControlLayer::Instance().SCN_AmpLow4();
		m_nApproxMag = 4;
		break;
	}
}


void CDlgImpPanelDebug2::OnClearAllChannels() 
{
	theApp.DisplayMsg("Press clear all channels of SCN board");
	CControlLayer::Instance().ClearAllChannels();
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_10 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_100_1 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_100_2 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_1000 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_10000 )))->SetCheck(0);
	m_nApproxMag = -1;
}

void CDlgImpPanelDebug2::OnCHECKStigCtrl() 
{
	// Stig Ctrl switch
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Stig_Ctrl)))->GetCheck();
	CControlLayer::Instance().CHECKStigCtrl(nCheck);

}

void CDlgImpPanelDebug2::OnADCselectOBChanged( UINT nID )
{
	int nIndex = nID -IDC_RADIO_ADCselectOB0;
	// show assignments for each of the channels (16 for all except for LNS board, which as 8)
	CString str;
	for( int i=0; i<16; i++ )
	{
		str.Format( "Ch %d: %s", i, CControlLayer::Instance().m_DL.adc.chn[nIndex*16+i].strLabel );
		GetDlgItem( IDC_RADIO_ADCsChSelect0 +i )->SetWindowText( str );
	}

	switch( nIndex )
	{
	case	0:
		str.Format( "Press select ADC on SCN");
		break;
	case	1:
		str.Format( "Press select ADC on LNS");
		break;
	case	2:
		str.Format( "Press select #0 ADC on VAC");
		break;
	case	3:
		str.Format( "Press select #1 ADC on VAC");
		break;
	case	4:
		str.Format( "Press select #0 ADC on SPR");
		break;
	case	5:
		str.Format( "Press select #1 ADC on SPR");
		break;
	}
	theApp.DisplayMsg(str);

	CBoardComm::Instance().ChangedADCselectOB( nIndex );
}

void CDlgImpPanelDebug2::OnADCsChSelectChanged( UINT nID )
{
	int nIndex = nID -IDC_RADIO_ADCsChSelect0;
	CString str;
	str.Format( "Press select %02d chn", nIndex );
	theApp.DisplayMsg(str);

	CBoardComm::Instance().ChangedADCsChSelect( nIndex,CControlLayer::Instance().m_DL.adc.chn );
}

void CDlgImpPanelDebug2::OnCHECKReadSelectedADCAuto() 
{
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_ReadSelectedADCAuto )))->GetCheck();
	if( nCheck )
		SetTimer( 2, 2000, NULL );
	else
		KillTimer( 2 );
}

void CDlgImpPanelDebug2::OnReadSelectedADC() 
{
	theApp.DisplayMsg("Press read selected ADC");
	CString str;
	if( CBoardComm::Instance().m_bADCtype )
		str = CBoardComm::Instance().ReadSelectedADC_Procedure( 100, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log,FALSE );
	else
		str = CBoardComm::Instance().ReadSelectedADC_Procedure( 70,  CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log, FALSE );
	GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
	ADCvconv();
}

void CDlgImpPanelDebug2::ADCvconv()
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str;
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = CBoardComm::Instance().GetChnRangeMin(CControlLayer::Instance().m_DL.adc.chn);
	double dRangeMax = CBoardComm::Instance().GetChnRangeMax(CControlLayer::Instance().m_DL.adc.chn);
	double dEquivCurrent = 0.0;
	if( CBoardComm::Instance().m_bADCtype )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = CBoardComm::Instance().GetADCReadRaw();
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		str.Format( "%06X", dwADCread24 );				// Convert to 6 character hex string
		GetDlgItem( IDC_EDIT_ADCreadVal24TB )->SetWindowText( str );

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
			dADCreadVrefFrac = 1.0 * dwADCread24 / 0x7FFFFF;	// Fractional value of full range of 0 to +Vref/2
		else
			dADCreadVrefFrac = 1.0 - 1.0*(dwADCread24 - 0x800000) / 0x7FFFFF;	// Fractional value of range of 0 to -Vref/2

		dADCreadVrefPct = dADCreadVrefFrac * 100;	// Percentage of full range of Vref(+) - Vref(-)
		if( bADCvalSignBit )
		{
			str.Format( "%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			// g_ADCvref varies per board design
			str.Format( "%.5f", dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			str.Format( "%.5f", dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2 +CBoardComm::Instance().m_dADCCOMoffset );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );
			dEquivCurrent = (dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2 +CBoardComm::Instance().m_dADCCOMoffset) * CBoardComm::Instance().m_dADCampsPerVolt;
		}
		else
		{
			str.Format( "-%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "-%.5f", dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			str.Format( "%.5f", CBoardComm::Instance().m_dADCCOMoffset - dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );
			dEquivCurrent = (CBoardComm::Instance().m_dADCCOMoffset - dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2) * CBoardComm::Instance().m_dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(CBoardComm::Instance().m_nBoardReadDataBuf[2*2]);
		dwADCreadRaw &= 0xFFFF;
		str.Format( "%04X", dwADCreadRaw );	// Identical value for 16 bit LTC1859
		GetDlgItem( IDC_EDIT_ADCreadVal24TB )->SetWindowText( str );

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "%.5f", dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );	// Same - no adj. needed
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "%.5f", dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );	// Same - no adj. needed
		}
		dEquivCurrent = dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref /2 * CBoardComm::Instance().m_dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * CBoardComm::Instance().m_dADCvref / 2 * CBoardComm::Instance().m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( CBoardComm::Instance().m_dADCcurrentMeas > 0 )
	{
		GetDlgItem( IDC_STATIC_CurrentMeasLabel )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->ShowWindow( SW_SHOW );
		str.Format( "%.6f", dEquivCurrent );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->SetWindowText( str );
	}
	else
	{
		GetDlgItem( IDC_STATIC_CurrentMeasLabel )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->SetWindowText( "n/a" );
	}
}


void CDlgImpPanelDebug2::OnBUTTONINTtest() 
{
	// INT test
	int nCheck = m_btnINTtest.GetCheck();
	if( nCheck )
	{
		m_btnINTtest.SetColor( g_crEmphasis2 );
		m_btnINTtest.SetWindowText( "INT test ON" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_INT_test_ON, FALSE, 0 );

		int i;
		for( i=0; i<4; i++ )
		{
			int nCheck = ((CButton*)(GetDlgItem(IDC_RADIO_Board_SCN +i)))->GetCheck();
			if( nCheck )
				break;
		}
		if( i == 4 )
			i = 2;	// VAC board
		CSemCtrl::Instance().USB_WriteToBoard( USB_INT_test_SPR +(3-i), FALSE, 0 );
	}
	else
	{
		m_btnINTtest.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnINTtest.SetWindowText( "INT test OFF" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_INT_test_OFF, FALSE, 0 );
	}
}

void CDlgImpPanelDebug2::OnBUTTONFakeMainslock() 
{
	// Fake MainsLock
	int nCheck = m_btnFakeMains.GetCheck();
	if( nCheck )
	{
		m_btnFakeMains.SetColor( g_crEmphasis1 );
		m_btnFakeMains.SetWindowText( "FakeMains ON" );
	}
	else
	{
		m_btnFakeMains.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnFakeMains.SetWindowText( "FakeMains OFF" );
	}

	CControlLayer::Instance().CHECKFakeMainslock(nCheck);
}

void CDlgImpPanelDebug2::OnBUTTONMainsLockLineSync() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_btnFakeMains.GetCheck();
	if( nCheck == 0 )
		return;

	CSemCtrl::Instance().USB_WriteToBoard( USB_MainsLock_LineSync, FALSE, 0 );
}

void CDlgImpPanelDebug2::OnBUTTONMainsLockFrameSync() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_btnFakeMains.GetCheck();
	if( nCheck == 0 )
		return;

	CControlLayer::Instance().MainsLockFrameSync();
}

void CDlgImpPanelDebug2::OnCHECKCompCond() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Comp_Cond)))->GetCheck();
	CControlLayer::Instance().CHECKCompCond(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKCompObj() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Comp_Obj)))->GetCheck();
	CControlLayer::Instance().CHECKCompObj(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKCompAlignX() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Comp_AlignX)))->GetCheck();
	CControlLayer::Instance().CHECKCompAlignX(nCheck);
}

void CDlgImpPanelDebug2::OnCHECKCompAlignY() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Comp_AlignY)))->GetCheck();
	CControlLayer::Instance().CHECKCompAlignY(nCheck);
}
