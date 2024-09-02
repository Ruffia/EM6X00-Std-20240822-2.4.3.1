// DlgImpPanelAll2.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgImpPanelAll2.h"
#include "DlgProgress.h"
#include "ScanView.h"
#include "MainFrm.h"
#include "SEMCTRL_Label.h"
#include "DlgPrompt.h"

#include "GlobalParamSetting.h"
#include "StageXYManager.h"
#include "AppLogManager.h"
#include "CFGFileManager.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "BoardComm.h"
#include "ScaleManager.h"
#include "CCGStatusDetector.h"
#include "WPCStatusDetector.h"
#include "IPGStatusDetector.h"
#include "IPCStatusDetector.h"
#include "PLCvacBrdStatusDetector.h"
#include "CCPS3603tatusDetector.h"
#include "SpellmanStatusDetector.h"
#include "HVPowerDataManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "CommonFunctionManager.h"
#include "ScanParameterManager.h"
#include "AutoFocusThread.h"
#include "Util.h"
#include "SEMPanel/PanelDataProcessorMgr.h"
#include "SEMPanel/PanelDataProcessor.h"
#include "SEMLog/AlarmManager.h"
#include "TrackStatusDetector.h"
#include "DlgSetImageData.h"
#include "CommBSD.h"
#include "DiatomDetection/AutoImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelAll2 dialog


CDlgImpPanelAll2::CDlgImpPanelAll2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImpPanelAll2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImpPanelAll2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pAutoProgressDlg		= NULL;
	m_nCPSTimer				= 0;
	m_bVacuumSoftAuto		= FALSE;

	m_bmpON.LoadBitmap(IDB_BITMAP_LED_RED_H);
	m_bmpOFF.LoadBitmap(IDB_BITMAP_LED_RED_D);
	m_bmpERR.LoadBitmap(IDB_BITMAP_LED_ERR);
	m_hIcon1				= NULL;
	m_hIcon2				= NULL;
	m_hIcon3				= NULL;
	m_hIcon4				= NULL;
	m_hIcon5				= NULL;

	m_nPosB	=	m_nPosC		= 0;
	m_nVACMsgType	= -1;
	m_nCCG_CountsBak = 0;
	m_nCCG_Counts = 0;
}

CDlgImpPanelAll2::~CDlgImpPanelAll2()
{
	m_bmpON.DeleteObject();
	m_bmpOFF.DeleteObject();
	m_bmpERR.DeleteObject();
	m_bmpPneumaticDiagram.DeleteObject();
	m_brushVacNotGood.DeleteObject();
	m_brushVacGood.DeleteObject();
	m_brushVacErr.DeleteObject();
}

void CDlgImpPanelAll2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImpPanelAll2)
	DDX_Control(pDX, IDC_STATIC_PneumaticDiagram, m_staticPneumaticDiagram);
	DDX_Control(pDX, IDC_CHECK_Vacuum_Soft_Auto, m_btnSoftAuto);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTANDACTRL_WD, m_ctrlWD);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND6CTRL_CondenserLens, m_ctrlCondenserLens);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens, m_ctrlObjectiveLens);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_Brightness, m_ctrlBrightness);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_Contrast, m_ctrlContrast);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND9CTRL_Magnifier, m_ctrlMagnifier);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND2CTRL_BeamAlign, m_ctrlBeemAlignment);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND2CTRL_ScanShift, m_ctrlScanShift);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND3CTRL_ScanRotate, m_ctrlScanRotate);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND2CTRL_Stig, m_ctrlStigmation);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_AuxStig, m_ctrlAuxiliaryStigmation);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV, m_ctrlHV);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias, m_ctrlBias);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent, m_ctrlHeater);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness, m_ctrlSignal2Brightness);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias, m_ctrlWSMBias);
	DDX_Control(pDX, IDC_CHECK_Vacuum_CHAMBER_VENT, m_btnChamberVent);
	DDX_Control(pDX, IDC_CHECK_Vacuum_GUN_VENT, m_btnGunVent);
	DDX_Control(pDX, IDC_CHECK_Vacuum_VACUUM_SWITCH, m_btnVacuumSwitch);
	DDX_Control(pDX, IDC_CHECK_Vacuum_VACUUM_POWER, m_btnVacuumPower);
	DDX_Control(pDX, IDC_CHECK_Vacuum_VACUUM_STANDBY, m_btnVacuumStandby);
	DDX_Control(pDX, IDC_CHECK_Debug_V1, m_btnV1);
	DDX_Control(pDX, IDC_CHECK_IMP_PANEL_HV, m_btnHV);
	DDX_Control(pDX, IDC_CHECK_IMP_PANEL_Bias, m_btnBias);
	DDX_Control(pDX, IDC_CHECK_IMP_PANEL_Filament, m_btnFilament);
	DDX_Control(pDX, IDC_BUTTON_ShowCameraImage, m_btnCCD);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImpPanelAll2, CDialog)
	//{{AFX_MSG_MAP(CDlgImpPanelAll2)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_Vacuum_VACUUM_POWER, OnVacuumVACUUMPOWER)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_VACUUM_SWITCH, OnVacuumVACUUMSWITCH)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_GUN_VENT, OnVacuumGUNVENT)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_CHAMBER_VENT, OnVacuumCHAMBERVENT)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_VACUUM_STANDBY, OnVacuumVACUUMSTANDBY)
	ON_BN_CLICKED(IDC_CHECK_IMP_PANEL_HV, OnCheckHV)
	ON_BN_CLICKED(IDC_CHECK_IMP_PANEL_Bias, OnCheckBias)
	ON_BN_CLICKED(IDC_CHECK_IMP_PANEL_Filament, OnCheckFilament)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_Vacuum_Soft_Auto, OnVacuumSoftAuto)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_OFF, OnCheckOff)
	ON_BN_CLICKED(IDC_BUTTON_Debug_Tab_Mon, OnBUTTONMon)
	ON_BN_CLICKED(IDC_CHECK_Debug_V1, OnCHECKV1)
	ON_BN_CLICKED(IDC_BUTTON_ShowCameraImage, OnBUTTONShowCameraImage)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_SetLength, OnRotateWrite)
	ON_BN_CLICKED(IDC_CHECK_Debug_HV, OnRotateDebug)
	ON_BN_CLICKED(IDC_BUTTON_IMP_PANEL_Navigate, OnBUTTONNavigate)
	ON_BN_CLICKED(IDC_BUTTON_SetData, OnBnClickedBtnSetData)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_ADJUSTOR_BEAM_ALIGNMENT, ID_ADJUSTOR_STIGMATOR, OnAdjustorSortChanged)
	ON_MESSAGE(WM_USER_VAC_MSG, OnSMvacChanged)
	ON_MESSAGE(WM_USER_VAC_TCAmp, OnSMvacTCAmpUpdate)
	ON_MESSAGE(WM_USER_VAC_UpdateStatus, OnVACUpdateStatus)
	ON_MESSAGE(WM_USER_VAC_UpdateCCGValue, OnVACUpdateCCGValue)
	ON_MESSAGE(WM_USER_VAC_UpdateIPGValue, OnVACUpdateIPGValue)
	ON_MESSAGE(WM_USER_BoardVAC_UpdateStatus, OnBoardVACUpdateStatus)
	ON_MESSAGE(WM_USER_PLCvacBrd_UpdateStatus, OnUpdatePLCvacBrdStatus)
	ON_MESSAGE(WM_USER_CPS_Status, OnCPS3603statusChanged)
	ON_MESSAGE(WM_USER_CPS_MSG, OnCPS3603msgChanged)
	ON_MESSAGE(WM_USER_CPS_Param, OnCPS3603paramChanged)
	ON_MESSAGE(WM_USER_CPS_AutoParam, OnCPS3603AutoParamChanged)
	ON_MESSAGE(WM_USER_SPELLMAN_Status, OnSpellmanStatusChanged)
	ON_MESSAGE(WM_USER_SPELLMAN_Param, OnSpellmanParamChanged)
	ON_MESSAGE(WM_USER_SPELLMAN_Param2, OnSpellmanParam2Changed)
	ON_MESSAGE(WM_USER_SPELLMAN_Param3, OnSpellmanParam3Changed)
	ON_MESSAGE(WM_USER_SPELLMAN_AutoParam, OnSpellmanAutoParamChanged)
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_SETPARAM, OnWSM30KVSetParam)
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_UpdateMon, OnWSM30KVUpdateMon)
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_UpdateStatus, OnWSM30KVUpdateStatus)
	ON_MESSAGE(WM_USER_ControlPanel_MSG, OnControlPanelDataRecieved)
	ON_MESSAGE(WM_USER_CCD_MSG, OnCCDmsgChanged)
	ON_MESSAGE(WM_USER_BSD_Update, OnBSDmsgChanged)
	ON_MESSAGE(WM_USER_StagePod_Update, OnUpdateStagePod)
	ON_MESSAGE(WM_USER_NAV_Update, OnNAVUpdate)
	ON_BN_CLICKED(IDC_BUTTON_DiatomDetect, &CDlgImpPanelAll2::OnBnClickedButtonDiatomdetect)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgImpPanelAll2, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgImpPanelAll2)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTANDACTRL_WD, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND6CTRL_CondenserLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_Brightness, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_Contrast, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND9CTRL_Magnifier, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND2CTRL_BeamAlign, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND2CTRL_ScanShift, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND3CTRL_ScanRotate, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND2CTRL_Stig, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND7CTRL_AuxStig, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND6CTRL_CondenserLens, 2 /* ReportAction */, OnReportAction, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens, 2 /* ReportAction */, OnReportAction, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_Brightness, 2 /* ReportAction */, OnReportAction, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_Contrast, 2 /* ReportAction */, OnReportAction, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND9CTRL_Magnifier, 2 /* ReportAction */, OnReportAction, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV, 2 /* ReportAction */, OnReportAction, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias, 2 /* ReportAction */, OnReportAction, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAll2, IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent, 2 /* ReportAction */, OnReportAction, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelAll2 message handlers

void CDlgImpPanelAll2::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	// Vacuum
	GetDlgItem(IDC_CHECK_Vacuum_VACUUM_POWER)->SetWindowText(GetResString(IDS_CHECK_Vacuum_VACUUM_POWER));
	GetDlgItem(IDC_CHECK_Vacuum_VACUUM_SWITCH)->SetWindowText(GetResString(IDS_CHECK_Vacuum_VACUUM_SWITCH));
	GetDlgItem(IDC_CHECK_Vacuum_GUN_VENT)->SetWindowText(GetResString(IDS_CHECK_Vacuum_GUN_VENT));
	GetDlgItem(IDC_CHECK_Vacuum_CHAMBER_VENT)->SetWindowText(GetResString(IDS_CHECK_Vacuum_CHAMBER_VENT));
	GetDlgItem(IDC_CHECK_Vacuum_VACUUM_STANDBY)->SetWindowText(GetResString(IDC_CHECK_Vacuum_VACUUM_STANDBY));
	GetDlgItem(IDC_CHECK_OFF)->SetWindowText(GetResString(IDS_CHECK_OFF));
	GetDlgItem(IDC_BUTTON_Debug_Tab_Mon)->SetWindowText(GetResString( IDS_BUTTON_Debug_Tab_Mon ));

	GetDlgItem(IDC_STATIC_IMP_PANEL_Group_Vacuum)->SetWindowText(GetResString(IDS_STATIC_IMP_PANEL_Group_Vacuum));

/*	// HV module
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_HV));
		GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias));
		GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent));
	}
	else
	{
		GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_HV));
		GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Bias));
		GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Heater));
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias));
	}
	GetDlgItem(IDC_CHECK_IMP_PANEL_HV)->SetWindowText(GetResString(IDS_CHECK_IMP_PANEL_HV));
	GetDlgItem(IDC_CHECK_IMP_PANEL_Bias)->SetWindowText(GetResString(IDS_CHECK_IMP_PANEL_Bias));
	GetDlgItem(IDC_CHECK_IMP_PANEL_Filament)->SetWindowText(GetResString(IDS_CHECK_IMP_PANEL_Filament));

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
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_CHECK_Rev;
	m_ctrlCondenserLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Rev));
	m_ctrlObjectiveLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Rev));
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_BUTTON_Auto;
	m_ctrlCondenserLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Auto));
	m_ctrlObjectiveLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Auto));

//	m_ctrlHV.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_HV));
	m_ctrlWD.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTANDACTRL_WD));
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
*/
	CString str;
	VARIANT	ResourceID, ResourceString;
	ResourceID.intVal = 0;
	// HV module
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_HV);
		ResourceString.bstrVal = str.AllocSysString();
		((CSemCoreCtrlPanel_Stand1*)(GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV)))->SyncString(ResourceID, ResourceString);
		str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias);
		ResourceString.bstrVal = str.AllocSysString();
		((CSemCoreCtrlPanel_Stand1*)(GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias)))->SyncString(ResourceID, ResourceString);
		str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent);
		ResourceString.bstrVal = str.AllocSysString();
		((CSemCoreCtrlPanel_Stand1*)(GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent)))->SyncString(ResourceID, ResourceString);
	}
	else
	{
		str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_HV);
		ResourceString.bstrVal = str.AllocSysString();
		((CSemCoreCtrlPanel_Stand1*)(GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_HV)))->SyncString(ResourceID, ResourceString);
		str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Bias);
		ResourceString.bstrVal = str.AllocSysString();
		((CSemCoreCtrlPanel_Stand1*)(GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias)))->SyncString(ResourceID, ResourceString);
		str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Heater);
		ResourceString.bstrVal = str.AllocSysString();
		((CSemCoreCtrlPanel_Stand1*)(GetDlgItem(IDC_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent)))->SyncString(ResourceID, ResourceString);
	}

	GetDlgItem(IDC_CHECK_IMP_PANEL_HV)->SetWindowText(GetResString(IDS_CHECK_IMP_PANEL_HV));
	GetDlgItem(IDC_CHECK_IMP_PANEL_Bias)->SetWindowText(GetResString(IDS_CHECK_IMP_PANEL_Bias));
	GetDlgItem(IDC_CHECK_IMP_PANEL_Filament)->SetWindowText(GetResString(IDS_CHECK_IMP_PANEL_Filament));

	// Column
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE;
	str = GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Coarse);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlCondenserLens.SyncString(ResourceID, ResourceString);
	m_ctrlObjectiveLens.SyncString(ResourceID, ResourceString);
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE;
	str = GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Fine);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlCondenserLens.SyncString(ResourceID, ResourceString);
	m_ctrlObjectiveLens.SyncString(ResourceID, ResourceString);
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY;
	str = GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Tiny);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlCondenserLens.SyncString(ResourceID, ResourceString);
	m_ctrlObjectiveLens.SyncString(ResourceID, ResourceString);
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_CHECK_Rev;
	str = GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Rev);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlCondenserLens.SyncString(ResourceID, ResourceString);
	m_ctrlObjectiveLens.SyncString(ResourceID, ResourceString);
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_BUTTON_Auto;
	str = GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Auto);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlCondenserLens.SyncString(ResourceID, ResourceString);
	m_ctrlObjectiveLens.SyncString(ResourceID, ResourceString);
	ResourceID.intVal = 0;
	str = GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_CondenserLens);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlCondenserLens.SyncString(ResourceID, ResourceString);
	str = GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlObjectiveLens.SyncString(ResourceID, ResourceString);

	m_ctrlWD.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTANDACTRL_WD));

	// Detector
	ResourceID.intVal = 0;
	str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Brightness);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlBrightness.SyncString(ResourceID, ResourceString);
	str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Contrast);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlContrast.SyncString(ResourceID, ResourceString);
	str = GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlSignal2Brightness.SyncString(ResourceID, ResourceString);

	str = GetResString(IDS_SEMCORECTRLPANELSTAND2CTRL_BeamAlign);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlBeemAlignment.SyncString(ResourceID, ResourceString);
	str = GetResString(IDS_SEMCORECTRLPANELSTAND2CTRL_ScanShift);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlScanShift.SyncString(ResourceID, ResourceString);
	str = GetResString(IDS_SEMCORECTRLPANELSTAND2CTRL_Stig);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlStigmation.SyncString(ResourceID, ResourceString);
	str = GetResString(IDS_SEMCORECTRLPANELSTAND3CTRL_ScanRotate);
	ResourceString.bstrVal = str.AllocSysString();
	m_ctrlScanRotate.SyncString(ResourceID, ResourceString);
	str = GetResString(IDS_SEMCORECTRLPANELSTAND7CTRL_AuxStig);
	ResourceString.bstrVal = str.AllocSysString();
//	m_ctrlAuxiliaryStigmation.SyncString(ResourceID, ResourceString);
	// 使用完成后，需要释放
	SysFreeString( ResourceString.bstrVal );

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	Flag.lVal = 1;
	ClassID = m_ctrlAuxiliaryStigmation.GetPanelClassID();
	SerialNumber = m_ctrlAuxiliaryStigmation.GetPanelSerialNumber();
	CSemCtrlPanelManager::Instance().SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );

	// Status
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		GetDlgItem(IDC_STATIC_STATUS_HV_Label)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_HV));
		GetDlgItem(IDC_STATIC_STATUS_Pressure_Label)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_Bias));
		GetDlgItem(IDC_STATIC_STATUS_Pressure_Gun_Label)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_CPS_FilamentCurrent));
		GetDlgItem(IDC_STATIC_STATUS_EMI_Label)->SetWindowText(GetResString(IDS_STATIC_STATUS_EMI_Label));
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS_HV_Label)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_HV));
		GetDlgItem(IDC_STATIC_STATUS_Pressure_Label)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Bias));
		GetDlgItem(IDC_STATIC_STATUS_Pressure_Gun_Label)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Heater));
		GetDlgItem(IDC_STATIC_STATUS_EMI_Label)->SetWindowText(GetResString(IDS_STATIC_STATUS_EMI_Label));
	}

	//GetDlgItem(IDC_STATIC_STATUS_Vacuum)->SetWindowText(GetResString(IDS_STATIC_STATUS_Vacuum));
	GetDlgItem(IDC_STATIC_STATUS_EMI)->SetWindowText(GetResString(IDS_STATIC_STATUS_EMI));

	GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->SetWindowText(GetResString(IDS_STATIC_STATUS_Vacuum_Unit));
	GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->Invalidate(TRUE);

	GetDlgItem(IDC_BUTTON_IMP_PANEL_Navigate)->SetWindowTextA(GetResString(IDC_BUTTON_IMP_PANEL_Navigate));
	GetDlgItem(IDC_BUTTON_SetData)->SetWindowTextA(GetResString(IDC_BUTTON_SetData));
}

BOOL CDlgImpPanelAll2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RegisterCtrl();

	m_btnHV.EnableWindow( FALSE );
	m_btnBias.EnableWindow( FALSE );
	m_btnFilament.EnableWindow( FALSE );

	m_bSPRRelay1En = FALSE;
	m_bHVOn = FALSE;
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	m_btnSoftAuto.SetCheck( 1 );
	m_btnSoftAuto.SetColor( RGB(0,0,255) );
	m_btnSoftAuto.SetCheck(1);
	VAC_VacuumReady( FALSE );
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// 16.07 V1阀控制方式
	if( CVACDataManager::Instance().m_dwV1CtrlMode< 1 )
		m_btnV1.ShowWindow(SW_HIDE);
	else
		m_btnV1.ShowWindow(SW_SHOW);
	m_nHIstate = 0;
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// 17.02 CCD
	if( theApp.m_bHasCCD )
		GetDlgItem( IDC_BUTTON_ShowCameraImage)->ShowWindow(SW_SHOW);
	else
		GetDlgItem( IDC_BUTTON_ShowCameraImage)->ShowWindow(SW_HIDE);
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	m_hIcon1 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_BEAM_ALIGN));
	m_hIcon2 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_MOVE));
	m_hIcon3 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_ROTATE));
	m_hIcon4 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_STIG));
	m_hIcon5 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SORT_STIG_AUTO));
	((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_ALIGNMENT ) ))->SetIcon( m_hIcon1 );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_SHIFT ) ))->SetIcon( m_hIcon2 );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_SHIFT ) ))->SetCheck( TRUE );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_BEAM_ROTATE ) ))->SetIcon( m_hIcon3 );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_STIGMATOR ) ))->SetIcon( m_hIcon4 );
	((CButton*)( GetDlgItem( ID_ADJUSTOR_STIGMATOR ) ))->SetCheck( TRUE );
	((CButton*)( GetDlgItem( ID_AUTO_STIGMATOR ) ))->SetIcon( m_hIcon5 );
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log) );
	if( peditLog != NULL )
		peditLog->SetLimitText( 4294967294 );
	
	CEdit* peditLog2 = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log2) );
	if( peditLog2 != NULL )
		peditLog2->SetLimitText( 4294967294 );
	///////////////////////////////////////////////////////////////////////////

	Localize();

	m_brushVacNotGood.CreateSolidBrush(RGB(128,128,64));
	m_brushVacGood.CreateSolidBrush(RGB(0,128,0));
	m_brushVacErr.CreateSolidBrush(RGB(255,0,0));

	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	22:	// PLC真空板（有V3，有V5、CCG）
	case	21:	// PLC真空板（有V3，有V5、CCG）
		{
			m_bmpPneumaticDiagram.LoadBitmap(IDB_BITMAP_PneumaticDiagramNew);
			GetDlgItem( IDC_STATIC_Lamp_Vacuum_CCG )->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_PROGRESS_Vacuum_TCf)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PROGRESS_Vacuum_TCc)->ShowWindow(SW_HIDE);
			if( CSemCtrl::Instance().m_nVACBoardType > 21 )
			{
				GetDlgItem( IDC_CHECK_Vacuum_VACUUM_STANDBY )->ShowWindow(SW_SHOW);
				GetDlgItem( IDC_STATIC_Lamp_Vacuum_MP )->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem( IDC_CHECK_Vacuum_VACUUM_STANDBY )->ShowWindow(SW_HIDE);
				GetDlgItem( IDC_STATIC_Lamp_Vacuum_MP )->ShowWindow(SW_HIDE);
			}
		}
		break;
	case	2:	// 统一真空板（无V3，有V5、CCG），使用LaB6灯丝+IPG
		{
			m_bmpPneumaticDiagram.LoadBitmap(IDB_BITMAP_PneumaticDiagramNew2);
			GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vc )->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PROGRESS_Vacuum_TCc)->ShowWindow(SW_HIDE);
		}
		break;
	case	1:	// 统一真空板（有V3、V5、CCG）
		m_bmpPneumaticDiagram.LoadBitmap(IDB_BITMAP_PneumaticDiagramNew);
		break;
	default:	// VAC子卡（有V3，无V5、CCG）
		{
			m_bmpPneumaticDiagram.LoadBitmap(IDB_BITMAP_PneumaticDiagramOld);
			GetDlgItem( IDC_STATIC_Lamp_Vacuum_V5 )->ShowWindow(SW_HIDE);
			GetDlgItem( IDC_STATIC_Lamp_Vacuum_CCG )->ShowWindow(SW_HIDE);
		}
		break;
	}
//	((CStatic*)(GetDlgItem(IDC_STATIC_PneumaticDiagram)))->SetBitmap((HBITMAP)m_bmpPneumaticDiagram);
	m_staticPneumaticDiagram.SetBitmap((HBITMAP)m_bmpPneumaticDiagram);

	// 推拉杆
	StagePodInit();
// 	CTrackStatusDetector::Instance().Init(m_hWnd);
// 	CTrackStatusDetector::Instance().Start();

	// 导航
	m_bNavPowerOn = TRUE;
	m_nNavProcess = 0;
	ZeroMemory( &(m_navPosStart), sizeof(NAV_Position) );
	if( theApp.m_bHasNAV )
		GetDlgItem(IDC_BUTTON_IMP_PANEL_Navigate)->ShowWindow( SW_SHOW );
	else
		GetDlgItem(IDC_BUTTON_IMP_PANEL_Navigate)->ShowWindow( SW_HIDE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImpPanelAll2::OnDestroy() 
{
	theApp.DisplayMsg("Into all2 destroy");
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

	HVPower_Clear();
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		CCPS3603tatusDetector::Instance().Release();
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			// 关闭所有使能
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			{
				theApp.m_WSM30KV2.SetParam( 18, 0 );
				theApp.m_WSM30KV2.Close();
			}
			else
			{
				theApp.m_WSM30KV.SetParam( 17, 0 );
				theApp.m_WSM30KV.Close();
			}
		}
		else
		{
			// 关闭所有使能
			CSpellmanStatusDetector::Instance().SetParam( 61, 0 );
			CSpellmanStatusDetector::Instance().Release();
		}
	}
	///////////////////////////////////////////////////////////////////////////
	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	22:
	case	21:
		CPLCvacBrdStatusDetector::Instance().Release();
		break;
	case	0:
		CSMvac::Instance().Close();
		break;
	default:
		CVACDataManager::Instance().m_BoardVAC.Close(0);
		break;
	}
	SPR_HVOn(FALSE);
	SPR_Fan(FALSE);
	CCCGStatusDetector::Instance().Release();
	CWPCStatusDetector::Instance().Release();
	CIPGStatusDetector::Instance().Release();
	CIPCStatusDetector::Instance().Release();

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
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

	// 高压模块
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlHV );
	CPanelDataProcessorMgr::Instance().Release();

	if ( m_nCPSTimer != 0 )
	{
		KillTimer( m_nCPSTimer );
		m_nCPSTimer = 0;
	}

	if( m_hIcon1 != NULL )
		::DestroyIcon( m_hIcon1 );
	if( m_hIcon2 != NULL )
		::DestroyIcon( m_hIcon2 );
	if( m_hIcon3 != NULL )
		::DestroyIcon( m_hIcon3 );
	if( m_hIcon4 != NULL )
		::DestroyIcon( m_hIcon4 );
	if( m_hIcon5 != NULL )
		::DestroyIcon( m_hIcon5 );
}

void CDlgImpPanelAll2::AdjustCtrlsPos()
{
	//根据气路图位置动态调整各阀灯控件的布局
	CRect rcPD;
	int nLeftPD, nTopPD, nWidthPD, nHeightPD;
	// 气路图控件的参数: PD( Pneumatic Diagram )
	rcPD = GetControlRect( IDC_STATIC_PneumaticDiagram );
	nLeftPD		= rcPD.left;
	nTopPD		= rcPD.top;
	nWidthPD	= rcPD.Width();
	nHeightPD	= rcPD.Height();

	AdjustCtrlsPos_ValveLamp(IDC_STATIC_Lamp_Vacuum_V1, nLeftPD, nTopPD, nWidthPD, nHeightPD);
	AdjustCtrlsPos_ValveLamp(IDC_STATIC_Lamp_Vacuum_Vf, nLeftPD, nTopPD, nWidthPD, nHeightPD);
	AdjustCtrlsPos_ValveLamp(IDC_STATIC_Lamp_Vacuum_Vc, nLeftPD, nTopPD, nWidthPD, nHeightPD);
	AdjustCtrlsPos_ValveLamp(IDC_STATIC_Lamp_Vacuum_V4, nLeftPD, nTopPD, nWidthPD, nHeightPD);
	AdjustCtrlsPos_ValveLamp(IDC_STATIC_Lamp_Vacuum_V5, nLeftPD, nTopPD, nWidthPD, nHeightPD);
	AdjustCtrlsPos_ValveLamp(IDC_STATIC_Lamp_Vacuum_MP, nLeftPD, nTopPD, nWidthPD, nHeightPD);
	AdjustCtrlsPos_ValveLamp(IDC_STATIC_Lamp_Vacuum_TMP, nLeftPD, nTopPD, nWidthPD, nHeightPD);
	AdjustCtrlsPos_ValveLamp(IDC_STATIC_Lamp_Vacuum_CCG, nLeftPD, nTopPD, nWidthPD, nHeightPD);

	return;	// 2023.09 即使是威思曼的新电源，也不用调整了

	static BOOL bFirst = TRUE;
	if( m_hWnd == NULL )
		return;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 3 )
	{
		GetDlgItem( IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias )->ShowWindow( SW_HIDE );
		return;
	}
	if( !bFirst )
		return;

	bFirst = FALSE;	// 只调整一次
	CRect rc, rcItem;
	int nTopOffset;
	GetDlgItem( IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias )->ShowWindow( SW_SHOW );
//	rc = GetControlRect( IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias );
	GetDlgItem( IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias )->GetWindowRect( (LPRECT)rc );
	ScreenToClient( (LPRECT)rc );
	nTopOffset = rc.Height() +1;

	GetDlgItem( IDC_STATIC_IMP_PANEL_Group_Vacuum )->GetWindowRect( (LPRECT)rcItem );
	ScreenToClient( (LPRECT)rcItem );
	GetDlgItem( IDC_STATIC_IMP_PANEL_Group_Vacuum )->MoveWindow( rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height() +nTopOffset );

	CWnd *pw = GetWindow(GW_CHILD);
	while(pw != NULL)
	{
		pw->GetWindowRect( (LPRECT)rcItem );
		ScreenToClient( (LPRECT)rcItem );
		if( rcItem.top > rc.top )
			pw->MoveWindow( rcItem.left, rcItem.top +nTopOffset, rcItem.Width(), rcItem.Height() );
		pw = pw->GetWindow(GW_HWNDNEXT);
	};
}

void CDlgImpPanelAll2::AdjustCtrlsPos_ValveLamp( int nID, int nLeftPD, int nTopPD, int nWidthPD, int nHeightPD )
{
	CWnd* pWnd = GetDlgItem( nID );
	CRect rc;
	pWnd->GetClientRect( (LPRECT)rc );
	int nWidth	= rc.Width();
	int nHeight	= rc.Height();
	switch( nID )
	{
	case	IDC_STATIC_Lamp_Vacuum_V1:
		rc.left	= nLeftPD +nWidthPD *31 /96;
		rc.top	= nTopPD +nHeightPD *20 /96;
		break;
	case	IDC_STATIC_Lamp_Vacuum_Vf:
		rc.left	= nLeftPD +nWidthPD *49 /96;
		rc.top	= nTopPD +nHeightPD *77 /96;
		break;
	case	IDC_STATIC_Lamp_Vacuum_Vc:
		rc.left	= nLeftPD +nWidthPD *4 /96;
		rc.top	= nTopPD +nHeightPD *51 /96;
		break;
	case	IDC_STATIC_Lamp_Vacuum_V4:
		rc.left	= nLeftPD +nWidthPD *80 /96;
		rc.top	= nTopPD +nHeightPD *38 /96;
		break;
	case	IDC_STATIC_Lamp_Vacuum_V5:
		rc.left	= nLeftPD +nWidthPD *80 /96;
		rc.top	= nTopPD +nHeightPD *52 /96;
		break;
	case	IDC_STATIC_Lamp_Vacuum_MP:
		rc.left	= nLeftPD +nWidthPD *68 /96;
		rc.top	= nTopPD +nHeightPD *77 /96;
		break;
	case	IDC_STATIC_Lamp_Vacuum_TMP:
		rc.left	= nLeftPD +nWidthPD *31 /96;
		rc.top	= nTopPD +nHeightPD *66 /96;
		break;
	case	IDC_STATIC_Lamp_Vacuum_CCG:
		rc.left	= nLeftPD +nWidthPD *58 /96;
		rc.top	= nTopPD +nHeightPD *53 /96;
		break;
	}
	pWnd->MoveWindow( rc.left, rc.top, nWidth, nHeight, TRUE );
}

CRect CDlgImpPanelAll2::GetControlRect( int nID )
{
	CRect rc;
	CWnd* pWnd = GetDlgItem( nID );
	pWnd->GetWindowRect( (LPRECT)rc );
	ScreenToClient( (LPRECT)rc );
	return rc;
}

void CDlgImpPanelAll2::ChangeHighVoltage_3603(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bNotify, BOOL bSync, BOOL bSet)
{
	// bNotify: 通知所有控制面板
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CScanView* pView = (CScanView*)theApp.GetScanView();
	ASSERT(pView);
	USB_ScanType eScanType =  CScanParameterManager::Instance().m_nusbScanType;
	
	CString str;
	if( Xpos.lVal < 100 )
	{
		str.Format( "%.1f KV", Xpos.lVal /10.0 );
	}
	else
	{
		int nPos = Xpos.lVal -100 +10;
		str.Format( "%d KV", nPos);
	}
	m_ctrlHV.SyncText( str, str, Flag );
	if( bSync )
		m_ctrlHV.SyncPos( Xpos, Ypos, Flag );

	CHVPowerDataManager::Instance().ChangeHighVoltage_3603(bNotify, ClassID, SerialNumber, Xpos, Ypos, Flag, bSet, eScanType);
}

void CDlgImpPanelAll2::ChangeHighVoltage_771(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bNotify, BOOL bSync, BOOL bSet)
{
	// bNotify: 通知所有控制面板
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	double dHV = 0;
	CString str;
	if( Xpos.lVal < 100 )
	{
		str.Format( "%.1f KV", Xpos.lVal /10.0 );
	}
	else
	{
		int nPos = Xpos.lVal -100 +10;
		str.Format( "%d KV", nPos);
	}

	m_ctrlHV.SyncText( str, str, Flag );
	if( bSync )
		m_ctrlHV.SyncPos( Xpos, Ypos, Flag );

	CScanView* pView = (CScanView*)theApp.GetScanView();
	ASSERT(pView);
	USB_ScanType eScanType =  CScanParameterManager::Instance().m_nusbScanType;
	CHVPowerDataManager::Instance().ChangeHighVoltage_771(bNotify, ClassID, SerialNumber, Xpos, Ypos, Flag, bSet, bSync, eScanType);
}

// 2023.03 临时
void CDlgImpPanelAll2::ChangeHighVoltage_WSM30KV(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bNotify, BOOL bSync, BOOL bSet)
{
	if( bSet && CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		ChangeBias_WSM30KV2_Max();

	// bNotify: 通知所有控制面板
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	if( bNotify )
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	double dHV = 0;
	CString str;
	if( Xpos.lVal < 100 )
	{
		str.Format( "%.1f KV", Xpos.lVal /10.0 );
		if( bSet )
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
				theApp.m_WSM30KV2.SetParam( 1, Xpos.lVal *100 );
			else
				theApp.m_WSM30KV.SetParam( 1, Xpos.lVal *100 );
		}
		dHV = Xpos.lVal /10.0;
	}
	else
	{
		int nPos = Xpos.lVal -100 +10;
		str.Format( "%d KV", nPos);
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			theApp.m_WSM30KV2.SetParam( 1, nPos *1000 );
		else
			theApp.m_WSM30KV.SetParam( 1, nPos *1000 );
		dHV = nPos;
	}
	m_ctrlHV.SyncText( str, str, Flag );
	if( bSync )
		m_ctrlHV.SyncPos( Xpos, Ypos, Flag );
	theApp.m_setWSM30KV.fACLv	= (float)dHV;
	theApp.m_setWSM30KV2.fACLv	= (float)dHV;
	CGlobalParamSetting::Instance().g_setWSM30KV.fACLv	= (float)dHV;
	CGlobalParamSetting::Instance().g_WSM30KV2set.fACLv	= (float)dHV * 1000;
	CGlobalParamSetting::Instance().g_dHV		= dHV;
	CHVPowerDataManager::Instance().m_dHV		= dHV;
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL && (CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop) )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( CHVPowerDataManager::Instance().m_dHV );
	CControlLayer::Instance().SyncFineMag();

	SetTimer( 47, 1000, NULL );
}

void CDlgImpPanelAll2::ChangeFilament_3603(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet)
{
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CString strX, strY;
	strX.Format( "%.2f A", Xpos.lVal *0.01 );
	m_ctrlHeater.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
	if( bSet )
		CCPS3603tatusDetector::Instance().SetParam( 12, Xpos.lVal *10 );
	if ( CAppLogManager::Instance().m_boolFilamentCurrentLog )
	{
		strY.Format( GetResString(IDF_SD_SemControlPanelClassID_FilamentCurrent), ClassID.lVal);
		strX = strY + _T(" ") + strX;
		theApp.DisplayMsg( strX );
	}
	CGlobalParamSetting::Instance().g_dHV3 = Xpos.lVal /100.0;
	if( CGlobalParamSetting::Instance().g_dHV3 > 2.3 )
	{
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
			pfrm->LoggingFilamentLifetime_Start();
	}
	else
	{
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
			pfrm->LoggingFilamentLifetime_End();
	}

	if( bSync )
	{
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
}

void CDlgImpPanelAll2::ChangeFilament_771(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet)
{
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CString strX, strY;
	CHVPowerDataManager::Instance().m_Spellmans.pS.dIH	= Xpos.lVal *0.01;
	CGlobalParamSetting::Instance().g_Spellmans.pS.dIH	= Xpos.lVal *0.01;
	strX.Format( "%.2f A", Xpos.lVal *0.01 );
	m_ctrlHeater.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
	if( bSet )
		CSpellmanStatusDetector::Instance().SetParam( 34, Xpos.lVal *10 );
	if ( CAppLogManager::Instance().m_boolFilamentCurrentLog )
	{
		strY.Format( GetResString(IDF_SD_SemControlPanelClassID_FilamentCurrent), ClassID.lVal);
		strX = strY + _T(" ") + strX;
		theApp.DisplayMsg( strX );
	}
	if( CHVPowerDataManager::Instance().m_Spellmans.pS.dIH > 2.3 )
	{
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
			pfrm->LoggingFilamentLifetime_Start();
	}
	else
	{
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
			pfrm->LoggingFilamentLifetime_End();
	}

	if( bSync )
	{
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
}

// 2023.03 临时
void CDlgImpPanelAll2::ChangeFilament_WSM30KV(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet)
{
	if( bSet && CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		ChangeBias_WSM30KV2_Max();

	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CString strX, strY;
	theApp.m_setWSM30KV.fFILi	= (float)(Xpos.lVal *0.01);
	theApp.m_setWSM30KV2.fFILi	= (float)(Xpos.lVal *0.01);
	CGlobalParamSetting::Instance().g_Spellmans.pS.dIH	= Xpos.lVal *0.01;
	CGlobalParamSetting::Instance().g_setWSM30KV.fFILi	= (float)(Xpos.lVal *0.01);
	CGlobalParamSetting::Instance().g_WSM30KV2set.fFILi = (float)(Xpos.lVal *0.01);
	strX.Format( "%.2f A", Xpos.lVal *0.01 );
	m_ctrlHeater.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
	if( bSet )
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			theApp.m_WSM30KV2.SetParam( 3, Xpos.lVal *10 );
		else
			theApp.m_WSM30KV.SetParam( 3, Xpos.lVal *10 );
	}
	if ( CAppLogManager::Instance().m_boolFilamentCurrentLog )
	{
		strY.Format( GetResString(IDF_SD_SemControlPanelClassID_FilamentCurrent), ClassID.lVal);
		strX = strY + _T(" ") + strX;
		theApp.DisplayMsg( strX );
	}
	if( theApp.m_setWSM30KV.fFILi > 2.1 || theApp.m_setWSM30KV2.fFILi > 2.1 )
	{
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
			pfrm->LoggingFilamentLifetime_Start();
	}
	else
	{
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
			pfrm->LoggingFilamentLifetime_End();
	}
	SetTimer( 48, 1000, NULL );
}

void CDlgImpPanelAll2::ChangeBeam_771(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet)
{
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CString strX, strY;
	CHVPowerDataManager::Instance().m_Spellmans.pS.dIB	= Xpos.lVal;
	CGlobalParamSetting::Instance().g_Spellmans.pS.dIB	= Xpos.lVal;
	strX.Format( "%ld uA", Xpos.lVal );
	m_ctrlBias.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
	if( bSet )
		CSpellmanStatusDetector::Instance().SetParam( 33, Xpos.lVal *10 );
	if ( CAppLogManager::Instance().m_boolHVLog )
	{
		strY.Format( GetResString(IDF_SD_SemControlPanelClassID_GunCurrentAccelerate), ClassID.lVal);
		strX = strY + _T(" ") + strX;
		theApp.DisplayMsg( strX );
	}

	if( bSync )
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
}

// 2023.03 临时
void CDlgImpPanelAll2::ChangeBeam_WSM30KV(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet)
{
	if( bSet && CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		ChangeBias_WSM30KV2_Max();

	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CString strX, strY;
	theApp.m_setWSM30KV.fACLi	= (float)(Xpos.lVal);
	theApp.m_setWSM30KV2.fACLi	= (float)(Xpos.lVal);
	CGlobalParamSetting::Instance().g_setWSM30KV.fACLi	= (float)(Xpos.lVal);
	CGlobalParamSetting::Instance().g_WSM30KV2set.fACLi	= (float)(Xpos.lVal);
	CGlobalParamSetting::Instance().g_Spellmans.pS.dIB	= Xpos.lVal *1.0;
	strX.Format( "%ld uA", Xpos.lVal );
	m_ctrlBias.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
	if( bSet )
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			theApp.m_WSM30KV2.SetParam( 2, Xpos.lVal );
		else
			theApp.m_WSM30KV.SetParam( 2, Xpos.lVal );
	}
	if ( CAppLogManager::Instance().m_boolHVLog )
	{
		strY.Format( GetResString(IDF_SD_SemControlPanelClassID_GunCurrentAccelerate), ClassID.lVal);
		strX = strY + _T(" ") + strX;
		theApp.DisplayMsg( strX );
	}

	SetTimer( 46, 1000, NULL );
}

void CDlgImpPanelAll2::ChangeBias_3603(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet)
{
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CString strX, strY;
	strX.Format( "%.1f V", Xpos.lVal *1.0 );
	m_ctrlBias.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
	if( bSet )
		CCPS3603tatusDetector::Instance().SetParam( 11, Xpos.lVal *10 );
	if ( CAppLogManager::Instance().m_boolHVLog )
	{
		strY.Format( GetResString(IDF_SD_SemControlPanelClassID_HVAccelerate), ClassID.lVal);
		strX = strY + _T(" ") + strX;
		theApp.DisplayMsg( strX );
	}
	CGlobalParamSetting::Instance().g_dHV2 = Xpos.lVal *1.0;

	if( bSync )
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
}

// 2023.03 临时
void CDlgImpPanelAll2::ChangeBias_WSM30KV2_AutoAdjust( int nIDEvent )
{
	// 在自动找灯丝饱和点流程中的时候不处理
	if( CScanParameterSaver::Instance().m_nAutoFuction == AutoFilament )
		return;
	// 2023.08 对于威思曼新电源，当调节量在误差范围内，认为已调整完毕
	// 此时，将偏压值设置为回读值+40V
	double dDiff1 = abs(theApp.m_monWSM30KV2.monFB.fACLi -theApp.m_setWSM30KV2.fACLi);		
	double dDiff2 = abs(theApp.m_monWSM30KV2.monFB.fACLv -theApp.m_setWSM30KV2.fACLv *1000);
	if( (dDiff1 < 10.0) && (dDiff2 < 30.0) )	// 束流值误差+-10uA内，加速电压值误差+-30V内
	{
		KillTimer(nIDEvent);
		switch( nIDEvent )
		{
		case	46:
			theApp.DisplayMsg("WSM30KV2: Beam has been in place, adjust BIAS");
			break;
		case	47:
			theApp.DisplayMsg("WSM30KV2: Acc has been in place, adjust BIAS");
			break;
		case	48:
			theApp.DisplayMsg("WSM30KV2: Filament has been in place, adjust BIAS");
			break;
		}
		VARIANT Bias_ClassID, Bias_SerialNumber, Bias_Xpos, Bias_Ypos, Bias_Flag;
		Bias_SerialNumber.lVal	= -1;
		Bias_Xpos.lVal			= (int)(theApp.m_monWSM30KV2.monFB.fBIASv +theApp.m_setWSM30KV2.fBIASv);
		Bias_Ypos				= Bias_Xpos;
		Bias_Flag.lVal			= 1;
		Bias_ClassID.lVal = SemControlPanelClass_CPS_Bias;
		ChangeBias_WSM30KV2( Bias_ClassID, Bias_SerialNumber, Bias_Xpos, Bias_Ypos, Bias_Flag, TRUE, TRUE );
	}
}

void CDlgImpPanelAll2::ChangeBias_WSM30KV2_Max()
{
	// 2023.08 对于威思曼新电源，改变高压前先将偏压设置为最大值
	VARIANT Bias_ClassID, Bias_SerialNumber, Bias_Xpos, Bias_Ypos, Bias_Flag;
	Bias_SerialNumber.lVal	= -1;
	Bias_Xpos.lVal			= (int)g_fWSM30KV2FS_BIAS_mon_V;
	Bias_Ypos.lVal			= (int)g_fWSM30KV2FS_BIAS_mon_V;
	Bias_Flag.lVal			= 1;
	Bias_ClassID.lVal = SemControlPanelClass_CPS_Bias;
	ChangeBias_WSM30KV2( Bias_ClassID, Bias_SerialNumber, Bias_Xpos, Bias_Ypos, Bias_Flag, TRUE, TRUE );
}

void CDlgImpPanelAll2::ChangeBias_WSM30KV2(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync, BOOL bSet)
{
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	if( Xpos.lVal > (int)g_fWSM30KV2FS_BIAS_mon_V )
		Xpos.lVal = (int)g_fWSM30KV2FS_BIAS_mon_V;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	CString strX, strY;
	strX.Format( "%ld V", Xpos.lVal );
	m_ctrlWSMBias.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlWSMBias.SyncPos( Xpos, Ypos, Flag );
	if( bSet )
		theApp.m_WSM30KV2.SetParam( 7, Xpos.lVal );
	if ( CAppLogManager::Instance().m_boolHVLog )
	{
		strY.Format( GetResString(IDF_SD_SemControlPanelClassID_HVAccelerate), ClassID.lVal);
		strX = strY + _T(" ") + strX;
		theApp.DisplayMsg( strX );
	}
}

void CDlgImpPanelAll2::ChangePMT_771(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync)
{
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CString strX, strY;
	CHVPowerDataManager::Instance().m_Spellmans.pS.dVP	= Xpos.lVal;
	CGlobalParamSetting::Instance().g_Spellmans.pS.dVP	= Xpos.lVal *10 *1350.0 /4095;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	strX = CSemCoreAssemble::Instance().m_SP_PMT.FormatText();
	m_ctrlContrast.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlContrast.SyncPos( Xpos, Ypos, Flag );
	CSpellmanStatusDetector::Instance().SetParam( 35, Xpos.lVal *10 );
}

// 2023.03 临时
void CDlgImpPanelAll2::ChangePMT_WSM30KV(VARIANT ClassID, VARIANT SerialNumber, VARIANT Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSync)
{
	// bSync: 同步控件位置
	// bSet: 设置电源模块参数
	CString strX, strY;
	CHVPowerDataManager::Instance().m_Spellmans.pS.dVP	= Xpos.lVal;
	CGlobalParamSetting::Instance().g_Spellmans.pS.dVP	= Xpos.lVal *1300.0 /400;
	theApp.m_setWSM30KV.fPMTv	= (float)(Xpos.lVal *1300.0 /400);
	theApp.m_setWSM30KV2.fPMTv	= (float)(Xpos.lVal *1300.0 /400);
	CGlobalParamSetting::Instance().g_WSM30KV2set.fPMTv = (float)(Xpos.lVal *1300.0 /400);
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	strX = CSemCoreAssemble::Instance().m_SP_PMT.FormatText();
	m_ctrlContrast.SyncText( strX, strY, Flag );
	if( bSync )
		m_ctrlContrast.SyncPos( Xpos, Ypos, Flag );
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		theApp.m_WSM30KV2.SetParam( 4, (int)(Xpos.lVal *1300.0 /400) );
	else
		theApp.m_WSM30KV.SetParam( 4, (int)(Xpos.lVal *1300.0 /400) );
}

void CDlgImpPanelAll2::OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your control notification handler code here
	if( ClassID.lVal < SemControlPanelClass_CPS_HV )
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	else
	{
		CString strX, strY;
		switch( ClassID.lVal )
		{
		case	SemControlPanelClass_CPS_HV:
			{
				ChangeHighVoltage_3603( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, FALSE, TRUE );
			}
			break;
		case	SemControlPanelClass_CPS_Bias:
			{
				if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
					ChangeBias_WSM30KV2( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE, TRUE );
				else
					ChangeBias_3603( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, TRUE );
			}
			break;
		case	SemControlPanelClass_CPS_FilamentCurrent:
			{
				ChangeFilament_3603( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, TRUE );
			}
			break;
		case	SemControlPanelClass_SP_HV:
			{
				if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					ChangeHighVoltage_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, FALSE, TRUE );
				else
					ChangeHighVoltage_771( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, TRUE, TRUE );
			}
			break;
		case	SemControlPanelClass_SP_Bias:
			{
				if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					ChangeBeam_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE, TRUE );
				else
					ChangeBeam_771( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, TRUE );
			}
			break;
		case	SemControlPanelClass_SP_Heater:
			{
				if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					ChangeFilament_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE, TRUE );
				else
					ChangeFilament_771( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, TRUE );
			}
			break;
		case	SemControlPanelClass_SP_PMT:
			{
				if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
				else
					ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
			}
			break;
		}
	}
}

void CDlgImpPanelAll2::OnReportAction(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your control notification handler code here
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView == NULL )
		return;
	if( ClassID.lVal == SemControlPanelClass_CondenserLens )
	{
		if( Xpos.lVal < 8 )
		{
			// fast
			int nIndex = Xpos.lVal;
			VARIANT XNewPos, NewSerialNumber;
			NewSerialNumber.lVal = -1;
			XNewPos.lVal = CControlLayer::Instance().m_Fast.cl.nFast[nIndex];
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, NewSerialNumber, XNewPos, Ypos, Flag  );
		}
		else if( Xpos.lVal < 11 )
			pView->SyncCondenserLensReverse( Ypos.lVal );
	}
	else if( ClassID.lVal == SemControlPanelClass_ObjectiveLens )
	{
		if( Xpos.lVal < 8 )
		{
			// fast
		}
		else if( Xpos.lVal < 11 )
			pView->SyncObjectiveLensReverse( Ypos.lVal );
		else if( Xpos.lVal > 10 )
			pView->FuncAutoFocus();
	}
	else if( ClassID.lVal == SemControlPanelClass_CPS_HV )
	{
		if( Ypos.lVal > 0 )
			return;
		int nIndex = Xpos.lVal;
		CString strX, strY;
		VARIANT XNewPos;
		XNewPos.lVal = CControlLayer::Instance().m_Fast.hv.nFast[nIndex];
		ChangeHighVoltage_3603( ClassID, SerialNumber, XNewPos, Ypos, Flag, TRUE, TRUE, TRUE );
	}
	else if( ClassID.lVal == SemControlPanelClass_CPS_FilamentCurrent )
	{
		if( Ypos.lVal < 1 )
		{
			int nIndex = Xpos.lVal;
			CString strX, strY;
			VARIANT XNewPos;
			XNewPos.lVal = CControlLayer::Instance().m_Fast.fila.nFast[nIndex];
			ChangeFilament_3603( ClassID, SerialNumber, XNewPos, Ypos, Flag, TRUE, TRUE );
		}
		else
			pView->FuncAutoFilament();
	}
	else if( ClassID.lVal == SemControlPanelClass_SP_HV )
	{
		if( Ypos.lVal > 0 )
			return;
		int nIndex = Xpos.lVal;
		CString strX, strY;
		VARIANT XNewPos;
		XNewPos.lVal = CControlLayer::Instance().m_Fast.hv.nFast[nIndex];
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			ChangeHighVoltage_WSM30KV( ClassID, SerialNumber, XNewPos, Ypos, Flag, TRUE, TRUE, TRUE);
		else
			ChangeHighVoltage_771( ClassID, SerialNumber, XNewPos, Ypos, Flag, TRUE, TRUE, TRUE);
	}
	else if( ClassID.lVal == SemControlPanelClass_SP_Bias )
	{
		if( Ypos.lVal > 0 )
			return;
		int nIndex = Xpos.lVal;
		CString strX, strY;
		VARIANT XNewPos;
		XNewPos.lVal = CControlLayer::Instance().m_Fast.beam.nFast[nIndex];
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			ChangeBeam_WSM30KV( ClassID, SerialNumber, XNewPos, Ypos, Flag, TRUE, TRUE );
		else
			ChangeBeam_771( ClassID, SerialNumber, XNewPos, Ypos, Flag, TRUE, TRUE );
	}
	else if( ClassID.lVal == SemControlPanelClass_SP_Heater )
	{
		if( Ypos.lVal < 1 )
		{
			int nIndex = Xpos.lVal;
			CString strX, strY;
			VARIANT XNewPos;
			XNewPos.lVal = CControlLayer::Instance().m_Fast.fila.nFast[nIndex];
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				ChangeFilament_WSM30KV( ClassID, SerialNumber, XNewPos, Ypos, Flag, TRUE, TRUE );
			else
				ChangeFilament_771( ClassID, SerialNumber, XNewPos, Ypos, Flag, TRUE, TRUE );
		}
		else
			pView->FuncAutoFilament();
	}
	else if( ClassID.lVal == SemControlPanelClass_Brightness )
	{
		if( Ypos.lVal > 0 )
			pView->FuncAutoBrightContrast();
	}
	else if( ClassID.lVal == SemControlPanelClass_Contrast )
	{
		if( Ypos.lVal < 1 )
		{
			int nIndex = Xpos.lVal;
			VARIANT XNewPos, NewSerialNumber;
			NewSerialNumber.lVal = -1;
			XNewPos.lVal = CControlLayer::Instance().m_Fast.cst.nFast[nIndex];
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, NewSerialNumber, XNewPos, Ypos, Flag  );
		}
		else
			pView->FuncAutoBrightContrast();
	}
	else if( ClassID.lVal == SemControlPanelClass_SP_PMT )
	{
		if( Ypos.lVal < 1 )
		{
			int nIndex = Xpos.lVal;
			CString strX, strY;
			VARIANT XNewPos, NewSerialNumber;
			NewSerialNumber.lVal = -1;
			XNewPos.lVal = CControlLayer::Instance().m_Fast.cst.nFast[nIndex];
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				ChangePMT_WSM30KV( ClassID, NewSerialNumber, XNewPos, Ypos, Flag, TRUE );
			else
				ChangePMT_771( ClassID, NewSerialNumber, XNewPos, Ypos, Flag, TRUE );
		}
		else
			pView->FuncAutoBrightContrast();
	}
	else if( ClassID.lVal == SemControlPanelClass_Magnifier )
	{
		int nIndex = Xpos.lVal;
		VARIANT XNewPos, NewSerialNumber;
		NewSerialNumber.lVal = -1;
		XNewPos.dblVal = CControlLayer::Instance().m_Fast.mag.nFast[nIndex];
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, NewSerialNumber, XNewPos, Ypos, Flag  );
	}
}

#include "AlgorithmManager.h"
const int g_nCalibrateRatoteDataMaxCounts	= 20;

void CDlgImpPanelAll2::OnRotateDebug() 
{
	// TODO: Add your control notification handler code here
	// 2024.06 统调实验
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_HV)))->GetCheck();
	if( nCheck > 0 )
	{
		// 更新物镜统调扫描旋转的模式标志
		CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 22, nCheck );
		GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_SetLength)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_SetLength)->EnableWindow(FALSE);
		// 1. 从文档读取统调数据
		CFileDialog dlg(TRUE, "csv", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "csv文件 (*.csv)|*.csv||", NULL);
		if( dlg.DoModal() != IDOK )
			return;

		CString csFullPath = dlg.GetPathName();
		FILE *stream = fopen( (LPCSTR)csFullPath, "r" );
		if( stream == NULL )
			return;

		TCHAR	cBuff[256];
		/* Set pointer to beginning of file: */
		fseek( stream, 0L, SEEK_SET );
		/* Read data back from file: */
		//fscanf( stream, "%s", cBuff );
		int i, nCounts = 0;
		double dHV[g_nCalibrateRatoteDataMaxCounts], dOLdisp[g_nCalibrateRatoteDataMaxCounts], dOLtotal[g_nCalibrateRatoteDataMaxCounts], dWD[g_nCalibrateRatoteDataMaxCounts];
		long lOLindex[g_nCalibrateRatoteDataMaxCounts], lOLsent[g_nCalibrateRatoteDataMaxCounts];
		double dRotate[g_nCalibrateRatoteDataMaxCounts];
		for( i=0; i<g_nCalibrateRatoteDataMaxCounts; i++ )
		{
			dRotate[i] = -1.0;
			//csMessage.Format( "[%s], %.1f, %ld, %ld, %04X, %.6f, %.6f, %.6f, %ld\r\n", strTime, dHV, lOLindex, lOLsent, lOLsent, dOLdisp, dOLtotal, dWD, lRotate );
			fscanf( stream, "%lf,%ld,%ld,%lf,%lf,%lf,%lf", &dHV[i], &lOLindex[i], &lOLsent[i], &dOLdisp[i], &dOLtotal[i], &dWD[i], &dRotate[i] );
			if( feof( stream ) )
				break;
		}
		fclose( stream );
		for( i=0; i<g_nCalibrateRatoteDataMaxCounts; i++ )
		{
			if( dRotate[i] < 0.0 )
				break;
			// 让角度都在270 ~ 360+90范围内
			if( dRotate[i] < 180.0 )
				dRotate[i] += 360.0;
		}
		nCounts = i;
		char szBuf[255] = {0};
		sprintf( szBuf, "Read coadjust ScanRotate file, there are %d group data", nCounts );
		theApp.DisplayMsg(szBuf);

		// 2. 得到系数K和B
		// 用最小二乘法拟合直线方程
		double* pdX	= new double[g_nCalibrateRatoteDataMaxCounts];
		double* pdY	= new double[g_nCalibrateRatoteDataMaxCounts];
		for(int i=0; i<nCounts; i++ )
		{
			pdX[i]	= lOLindex[i];
			pdY[i]	= dRotate[i];
		}
		double dKa, dKb;
		CAlgorithmManager::Instance().LeastSquares_LinearEquation2( nCounts, pdX, pdY, dKa, dKb );
		delete [] pdX;
		delete [] pdY;

		sprintf( szBuf, "Rotate compAngle = %.10f *OL + %.10f", dKa, dKb );
		theApp.DisplayMsg(szBuf);

		// 3. 将系数K和B写入ini
		// 获取工作路径
		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");
		sprintf( szBuf, "%.10f, %.10f", dKa, dKb );
		WritePrivateProfileString( "Coadjust", "FactorsRotate", szBuf, path );

		// 4. 更新物镜统调扫描旋转的模式标志及系数
		CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 22, nCheck );
		// 5. 将扫描旋转角度归零
		VARIANT ClassID, SerialNumber, Flag, Rotate;
		ClassID.lVal		= SemControlPanelClass_ScanRotate;
		SerialNumber.lVal	= -1;
		Flag.lVal			= 1;
		Rotate.lVal			= 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Rotate, Rotate, Flag );
	}
	return;

//	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_HV)))->GetCheck();
	// 调试模式下任何放大倍数不禁用扫描旋转
	CSemCoreAssemble::Instance().m_Magnifier.SetParams( 50, nCheck );
	if( nCheck )
	{
		theApp.DisplayMsg( "ScanRotate debug mode" );
		// 调试模式下扫描旋转校准角度在线置0，但不改ini
		CSemCoreAssemble::Instance().m_ScanRotate.SetParams( 1, 0 );
	}
	else
	{
		theApp.DisplayMsg( "ScanRotate normal mode" );
		// 获取工作路径
		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");
		int nRotate	= GetPrivateProfileInt( "Calibrate_Offset", "Rotate", 10, path );
		CSemCoreAssemble::Instance().m_ScanRotate.SetParams( 1, nRotate );
	}
	CSemCoreAssemble::Instance().m_ScanRotate.Flush();
}

void CDlgImpPanelAll2::OnRotateWrite() 
{
	// TODO: Add your control notification handler code here
	// 2024.06 统调实验
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_HV)))->GetCheck();
	if( nCheck < 1 )
		return;
	// 记录当前高压值、物镜控件索引值、物镜下送值、物镜显示值、物镜总显示值、工作距离值、扫描旋转角度值到文件
	double dHV = CHVPowerDataManager::Instance().m_dHV;
	double dValue = 0.0;
	CSemCoreAssemble::Instance().m_ObjectiveLens.GetParams( 0, dValue );
	long lOLindex = (long)dValue;
	CSemCoreAssemble::Instance().m_ObjectiveLens.GetParams( 1, dValue );
	long lOLsent = (long)dValue;
	CSemCoreAssemble::Instance().m_ObjectiveLens.GetParams( 2, dValue );
	double dOLdisp = dValue;
	CSemCoreAssemble::Instance().m_ObjectiveLens.GetParams( 3, dValue );
	double dOLtotal = dValue;
	double dWD = CSemCoreAssemble::Instance().m_WD.GetPos().dblVal;
	double dRotate = CSemCoreAssemble::Instance().m_ScanRotate.GetPos().lVal /10.0;

	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strCoadjustFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d");
	strCoadjustFileName.Format( "%s\\Config\\Rotate-Cali-%s.csv", appfilepath, strTime );
	CFile fileTest;
	fileTest.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite );

	if( fileTest.m_hFile != CFile::hFileNull )
	{
		strTime = timeCurrent.Format(IDF_MessageTimeFormat);
		CString csMessage;
		csMessage.Format( "%.1f, %ld, %ld, %.6f, %.6f, %.6f, %.1f\r\n", dHV, lOLindex, lOLsent, dOLdisp, dOLtotal, dWD, dRotate );
		fileTest.SeekToEnd();
		fileTest.Write( csMessage, csMessage.GetLength() );
	}
	fileTest.Close();
	return;

	int nPos = CSemCoreAssemble::Instance().m_ScanRotate.GetPos().lVal;
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	int nRotateLimit = GetPrivateProfileInt( "Calibrate_Offset", "RotateLimit", 0, path );
	if( nPos > nRotateLimit && nPos < 360-nRotateLimit )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "The angle is too big!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	CString str;
	str.Format( "%d", nPos );
	WritePrivateProfileString( "Calibrate_Offset", "Rotate", str, path );
	str.Format( "Press write rotate on control panel, angle = %d", nPos );
	theApp.DisplayMsg( str );
}

//调节器类型改变
void CDlgImpPanelAll2::OnAdjustorSortChanged(UINT nID)
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

void CDlgImpPanelAll2::UpdateAdjustor( UINT nAdjustorSort )
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

void CDlgImpPanelAll2::UpdateManualAdjustor( UINT nID, UINT nFlag )
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

void CDlgImpPanelAll2::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent )
	{
	case	1:
		break;
	case	2:
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
		break;
	case	3:
		{
			KillTimer( 3 );
			if ( m_pAutoProgressDlg != NULL )
				m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
		}
		break;
	case	4:
		{
			if( (CVACDataManager::Instance().m_VacParamMain.bVf) && !(CVACDataManager::Instance().m_cfgVacCCG.bReady) )
			{
				CVACDataManager::Instance().m_nCCGconnects++;
				if( CVACDataManager::Instance().m_nCCGconnects > 5 )
					KillTimer( 4 );
				else
				{
					if( CVACDataManager::Instance().m_cfgVacCCG.nType > 3 )	// WPC
					{
						CWPCStatusDetector::Instance().Init( this->m_hWnd,CVACDataManager::Instance().m_cfgVacCCG.nPort );
						CWPCStatusDetector::Instance().Start();	// 只连接CCG
					}
					else
					{
						//CCCGStatusDetector::Instance().Init( this->m_hWnd , CVACDataManager::Instance().m_cfgVacCCG.nPort);
						//CCCGStatusDetector::Instance().Start();// 只连接CCG
					}
				}
			}
			else if( CVACDataManager::Instance().m_cfgVacCCG.bReady )
				KillTimer( 4 );
		}
		break;
	case	5:
		{
			KillTimer(nIDEvent);
			CCCGStatusDetector::Instance().Start();
		}
		break;
	case	11:
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
					CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
					KillTimer( 11 );
					SetTimer( 12, 150, NULL );	// 启动一键看像第二步：加对比度至经验值
				}
				else
					CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			}
		}
		break;
	case	12:
		{
			// 一键看像第二步：加对比度至经验值
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal	= -1;
			// Contrast
			if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
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
					CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
					KillTimer( 12 );
					// 启动一键看像第二步：加3603
					// 自动加高压至设定值
					// 自动加偏压至设定值
					// 自动加灯丝至设定值
					CCPS3603tatusDetector::Instance().SetParam( 21, AS_Rise_Acc1 );
				}
				else
					CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			}
			else
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
				ClassID.lVal = SemControlPanelClass_SP_PMT;
				Flag.lVal = 1;
				Xpos.lVal = CSemCoreAssemble::Instance().m_SP_PMT.GetPos().lVal +10;
				CString strX;
				if( Xpos.lVal > m_nPosC )
				{
					Xpos.lVal = m_nPosC;
					if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
						ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
					else
						ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );

					KillTimer( 12 );
					// 启动一键看像第二步：加771
					// 自动加高压至设定值
					// 自动加偏压至设定值
					// 自动加灯丝至设定值
					if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
						theApp.m_WSM30KV2.SetParam( 49, WSM30KV2AS_Rise_FIL );
					else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
						theApp.m_WSM30KV.SetParam( 49, WSM30KVAS_Rise_ACL1 );
					else
						CSpellmanStatusDetector::Instance().SetParam( 16, AS2_Rise_EHT1 );
				}
				else
				{
					if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
						ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
					else
						ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
				}
			}
		}
		break;
	case	13:
		{
			CScanView* pView = (CScanView*)theApp.GetScanView();
			if( CScanParameterSaver::Instance().m_nAutoFuction == AutoNULL )
			{
				KillTimer( 13 );
				// 自动加灯丝（找饱和点）完成
				// 启动2000倍自动聚焦
				Sleep(100);
//				pView->FuncAutoFocus();
				// 启动自动亮度对比度
//				pView->FuncAutoBrightContrast();
//				Sleep(1);
//				SetTimer( 14, 100, NULL );
			}
		}
		break;
	case	21:
		{
			KillTimer(21);
			CSpellmanStatusDetector::Instance().SetParam( 25, 1 );	// PMT
			theApp.DisplayMsg("Set Enables of PMT in OnTimer");
		}
		break;
	case	22:
		{
			KillTimer(22);
			CSpellmanStatusDetector::Instance().SetParam( 21, 1 );	// Collector
			theApp.DisplayMsg("Set Enables of Collector in OnTimer");
		}
		break;
	case	23:
		{
			KillTimer(23);
			theApp.DisplayMsg("Set Enables of Scint in OnTimer");
			CSpellmanStatusDetector::Instance().SetParam( 22, 1 );	// Scint
			theApp.DisplayMsg("OnTimer: set Scint init value 10000");
			Sleep(5);
			SetTimer(26, 1000, NULL);
		}
		break;
	case	24:
		{
			KillTimer(24);
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
			ClassID.lVal		= SemControlPanelClass_SP_PMT;
			SerialNumber.lVal	= -1;
			Enabled.lVal		= 1;
			Flag.lVal			= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		
			SerialNumber.lVal	= -1;
			ClassID.lVal		= SemControlPanelClass_SP_PMT;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			CSpellmanStatusDetector::Instance().SetParam( 35, Xpos.lVal );	// PMT设置为0
			theApp.DisplayMsg("OnTimer: set PMT init value 0");
		}
		break;
	case	25:
		{
			KillTimer(25);
			CSpellmanStatusDetector::Instance().SetParam( 31, 300*5 );
			CHVPowerDataManager::Instance().m_Spellmans.pS.dVC	= 300;
			CGlobalParamSetting::Instance().g_Spellmans.pS.dVC	= 300;
			theApp.DisplayMsg("OnTimer: set Collector init value 300");
		}
		break;
	case    26:
		{
			KillTimer(26);
			int nCheck = 1;
			VacuumVACUUMSWITCH_HVCheck(nCheck);
			VacuumVACUUMSWITCH_LoadData();
		}
		break;

	case	31:
		{
			BOOL bIsMoving = TRUE;
			if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
				bIsMoving = StageTMCM_IsMoving();
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
				bIsMoving = StageMC6600_IsMoving();
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
				bIsMoving = StageMC600_IsMoving();
			if( !bIsMoving )
			{
				KillTimer(31);
				int nCheck = m_btnGunVent.GetCheck();
				if( nCheck )
					VAC_Vent( TRUE );
				else
					VAC_Vent( FALSE );
			}
		}
		break;
	case	32:
		{
			BOOL bIsMoving = TRUE;
			if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
				bIsMoving = StageTMCM_IsMoving();
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
				bIsMoving = StageMC6600_IsMoving();
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
				bIsMoving = StageMC600_IsMoving();
			if( !bIsMoving )
			{
				KillTimer(32);
				NAV_UpdateStatus( 1, FALSE );	// 台子已到位
			}
		}
		break;
	case	41:
		{
			KillTimer(41);
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
				theApp.m_WSM30KV2.SetParam( 17, 1 );
			else
				theApp.m_WSM30KV.SetParam( 15, 1 );
			theApp.DisplayMsg("WSM30KV: Set Enables of PMT in OnTimer");
		}
		break;
	case	42:
		{
			KillTimer(42);
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
				theApp.m_WSM30KV2.SetParam( 16, 1 );
			else
				theApp.m_WSM30KV.SetParam( 14, 1 );
			theApp.DisplayMsg("WSM30KV: Set Enables of COL in OnTimer");
		}
		break;
	case	43:
		{
			KillTimer(43);
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
				theApp.m_WSM30KV2.SetParam( 14, 1 );
			else
				theApp.m_WSM30KV.SetParam( 11, 1 );
			theApp.DisplayMsg("WSM30KV: Set Enables of SCI in OnTimer");
		}
		break;

	case	46:
	case	47:
	case	48:
		ChangeBias_WSM30KV2_AutoAdjust( nIDEvent );
		break;
	case	50:
		{
			// 清零：设置偏压为最大值2000后，延迟一会儿再清零，防止触发OV/OC保护
			KillTimer(nIDEvent);
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal	= -1;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;
			Ypos.lVal			= 0;
			ClassID.lVal		= SemControlPanelClass_SP_Bias;
			ChangeBeam_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, TRUE );
			ClassID.lVal		= SemControlPanelClass_SP_Heater;
			ChangeFilament_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, TRUE );
			ClassID.lVal		= SemControlPanelClass_SP_HV;
			ChangeHighVoltage_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, TRUE, TRUE, TRUE );
			SetTimer( 49, 1000, NULL );
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgImpPanelAll2::OnOK() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgImpPanelAll2::OnCancel() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CDlgImpPanelAll2::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);
	BOOL bCheck = HIWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	switch( nID )
	{
	case	IDC_CHECK_Vacuum_VACUUM_POWER:
		if( bCheck )
			m_btnVacuumPower.SetColor( RGB(0,192,0) );
		else
			m_btnVacuumPower.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_CHECK_Vacuum_VACUUM_SWITCH:
		if( bCheck )
			m_btnVacuumSwitch.SetColor( RGB(255,0,0) );
		else
			m_btnVacuumSwitch.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_CHECK_Vacuum_GUN_VENT:
		if( bCheck )
			m_btnGunVent.SetColor( RGB(192,192,0) );
		else
			m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_CHECK_Vacuum_CHAMBER_VENT:
		if( bCheck )
			m_btnChamberVent.SetColor( RGB(192,192,0) );
		else
			m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_CHECK_Debug_V1:
		if( bCheck )
			m_btnV1.SetColor( RGB(0,192,0) );
		else
			m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_CHECK_Vacuum_Soft_Auto:
		if( bCheck )
			m_btnSoftAuto.SetColor( RGB(0,0,255) );
		else
			m_btnSoftAuto.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_CHECK_IMP_PANEL_HV:
		if( bCheck )
			m_btnHV.SetColor( RGB(0,192,0) );
		else
			m_btnHV.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_CHECK_IMP_PANEL_Bias:
		if( bCheck )
			m_btnBias.SetColor( RGB(0,192,0) );
		else
			m_btnBias.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_CHECK_IMP_PANEL_Filament:
		if( bCheck )
			m_btnFilament.SetColor( RGB(0,192,0) );
		else
			m_btnFilament.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	case	IDC_BUTTON_ShowCameraImage:
		if( bCheck )
			m_btnCCD.SetColor( RGB(0,128,255) );
		else
			m_btnCCD.SetColor( ::GetSysColor(COLOR_3DFACE) );
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CDlgImpPanelAll2::OnBUTTONMon() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if(!pfrm) return;
	if(NULL == pfrm->m_dlgDebug.m_hWnd) return;

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		pfrm->m_dlgDebug.m_pPageMon->VAC_UpdatePneumaticDiagram();
		if( CSemCtrl::Instance().m_nVACBoardType < 1 )
		{
			pfrm->m_dlgDebug.m_pPageMon->VAC_UpdateSMvacTCAmpUpdate( 0, CVACDataManager::Instance().m_VacTCAmp.dTCfAmp );
			pfrm->m_dlgDebug.m_pPageMon->VAC_UpdateSMvacTCAmpUpdate( 1, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
		}
		else if( CSemCtrl::Instance().m_nVACBoardType > 0 && CSemCtrl::Instance().m_nVACBoardType < 10 )
		{
			pfrm->m_dlgDebug.m_pPageMon->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
			pfrm->m_dlgDebug.m_pPageMon->VAC_UpdateTCAmpUpdate( 1, CVACDataManager::Instance().m_valueTC.fTC4 );
		}
		pfrm->m_dlgDebug.ShowWindow(SW_SHOW);
	}
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			pfrm->m_dlgDebug.m_pPageMon3->VAC_UpdatePneumaticDiagram();
			if( CSemCtrl::Instance().m_nVACBoardType < 1 )
			{
				pfrm->m_dlgDebug.m_pPageMon3->VAC_UpdateSMvacTCAmpUpdate( 0, CVACDataManager::Instance().m_VacTCAmp.dTCfAmp );
				pfrm->m_dlgDebug.m_pPageMon3->VAC_UpdateSMvacTCAmpUpdate( 1, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
			}
			else if( CSemCtrl::Instance().m_nVACBoardType > 0 && CSemCtrl::Instance().m_nVACBoardType < 10 )
			{
				pfrm->m_dlgDebug.m_pPageMon3->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
				pfrm->m_dlgDebug.m_pPageMon3->VAC_UpdateTCAmpUpdate( 1, CVACDataManager::Instance().m_valueTC.fTC4 );
			}
		}
		else
		{
			pfrm->m_dlgDebug.m_pPageMon2->VAC_UpdatePneumaticDiagram();
			if( CSemCtrl::Instance().m_nVACBoardType < 1 )
			{
				pfrm->m_dlgDebug.m_pPageMon2->VAC_UpdateSMvacTCAmpUpdate( 0, CVACDataManager::Instance().m_VacTCAmp.dTCfAmp );
				pfrm->m_dlgDebug.m_pPageMon2->VAC_UpdateSMvacTCAmpUpdate( 1, CVACDataManager::Instance().m_VacTCAmp.dTCcAmp );
			}
			else if( CSemCtrl::Instance().m_nVACBoardType > 0 && CSemCtrl::Instance().m_nVACBoardType < 10 )
			{
				pfrm->m_dlgDebug.m_pPageMon2->VAC_UpdateTCAmpUpdate( 0, CVACDataManager::Instance().m_valueTC.fTC3 );
				pfrm->m_dlgDebug.m_pPageMon2->VAC_UpdateTCAmpUpdate( 1, CVACDataManager::Instance().m_valueTC.fTC4 );
			}
		}
		pfrm->m_dlgDebug.ShowWindow(SW_SHOW);
	}
}

void CDlgImpPanelAll2::OnBUTTONShowCameraImage() 
{
	// TODO: Add your control notification handler code here
	char sz[512] = {0};
	sprintf_s(sz,512, "%s","ShowCameraImage\r\n" );
	OutputDebugString(sz);

	int nCheck = m_btnCCD.GetCheck();
	if( nCheck )
	{
		if( theApp.m_bHasEDS )
		{
			CDlgPrompt dlg;
			dlg.m_byteType = (BYTE)0x04;
			dlg.m_strMsg = GetResString(IDS_SD_PROMPT_Open_CCD);
			if( dlg.DoModal() != IDYES )
			{
				m_btnCCD.SetCheck(0);
				m_btnCCD.SetColor( ::GetSysColor(COLOR_3DFACE) );
				return;
			}
		}
		m_btnCCD.SetCheck(1);
		m_btnCCD.SetColor( RGB(0,128,255) );
		SPR_HVOn( TRUE );
		ShowCameraWnd(this->m_hWnd);	//AfxGetMainWnd()->m_hWnd);
	}
	else
	{
		m_btnCCD.SetCheck(0);
		m_btnCCD.SetColor( ::GetSysColor(COLOR_3DFACE) );
		SPR_HVOn( FALSE );
	}
}

void CDlgImpPanelAll2::ShowCameraWindow()
{
	ShowCameraWnd(this->m_hWnd);	//AfxGetMainWnd()->m_hWnd);
}

LRESULT CDlgImpPanelAll2::OnCCDmsgChanged(WPARAM wParam, LPARAM lParam)
{
	SPR_HVOn( FALSE );
	m_btnCCD.SetCheck(0);
	m_btnCCD.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnCCD.Invalidate(FALSE);
	return 0;
}

void CDlgImpPanelAll2::SyncCCDSwitch( BOOL bOn )
{
	if( bOn )
	{
		m_btnCCD.EnableWindow(TRUE);
		m_ctrlSignal2Brightness.ShowWindow( SW_HIDE );
		if(  CControlLayer::Instance().m_bHasBSD)
			//BSD_ShowWnd(((CMainFrame *)theApp.GetMainWnd())->m_hWnd, FALSE);
			BSD_ShowWnd(this->m_hWnd, FALSE);
	}
	else
	{
		ShowCameraWnd(this->m_hWnd, FALSE);
		SPR_HVOn( FALSE );
		m_btnCCD.SetCheck(0);
		m_btnCCD.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnCCD.EnableWindow(FALSE);
		m_ctrlSignal2Brightness.ShowWindow( SW_SHOW );
		if(  CControlLayer::Instance().m_bHasBSD)
			//BSD_ShowWnd(((CMainFrame *)theApp.GetMainWnd())->m_hWnd, TRUE);
			BSD_ShowWnd(this->m_hWnd, TRUE);
	}
}

LRESULT CDlgImpPanelAll2::OnBSDmsgChanged(WPARAM wParam, LPARAM lParam)
{
	if( CSemCtrl::Instance().m_nVACBoardType < 20 )
		return 0;
	if( BSD_IsReady() )
	{
		// 让BSE移入或移出
		BYTE bOpr = (BYTE)lParam;
		CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_BSE, bOpr );
	}
	return 0;
}


void CDlgImpPanelAll2::OnVacuumSoftAuto() 
{
	m_bVacuumSoftAuto = m_btnSoftAuto.GetCheck();
	if( m_bVacuumSoftAuto )
		m_btnSoftAuto.SetColor( RGB(0,0,255) );
	else
	{
		m_btnSoftAuto.SetColor( ::GetSysColor(COLOR_3DFACE) );
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
	m_btnHV.EnableWindow( !m_bVacuumSoftAuto );
	m_btnBias.EnableWindow( !m_bVacuumSoftAuto );
	m_btnFilament.EnableWindow( !m_bVacuumSoftAuto );
}

void CDlgImpPanelAll2::OnVacuumVACUUMPOWER() 
{
	int nCheck = m_btnVacuumPower.GetCheck();
	if( nCheck )
	{
		m_btnVacuumPower.SetColor( RGB(255,0,0) );
		
		if( CSemCtrl::Instance().m_nVACBoardType < 1 )
		{
			CSMvac::Instance().SetParams( VACIndex_Power, 1 );
			CSMvac::Instance().SetParams( VACIndex_MP, 1 );
		}
		theApp.DisplayMsg("Press vacuum POWER: start");
	}
	else
	{
		m_btnVacuumPower.SetColor( ::GetSysColor(COLOR_3DFACE) );
		if( CSemCtrl::Instance().m_nVACBoardType < 1 )
			CSMvac::Instance().SetParams( VACIndex_Power, 0 );
		theApp.DisplayMsg("Press vacuum POWER: close");
	}
}

void CDlgImpPanelAll2::OnVacuumVACUUMSWITCH() 
{
	int nCheck = m_btnVacuumSwitch.GetCheck();
	if( nCheck )
		theApp.DisplayMsg("Press ONEKEY: start");
	else
		theApp.DisplayMsg("Press ONEKEY: end");

	if( nCheck )
	{
		VacuumVACUUMSWITCH_HVCheck( nCheck );
	}

	m_btnVacuumSwitch.SetCheck( nCheck );
	if( nCheck )
		m_btnVacuumSwitch.SetColor( RGB(0,192,0) );
	else
		m_btnVacuumSwitch.SetColor( ::GetSysColor(COLOR_3DFACE) );

	if( nCheck )
	{
		VacuumVACUUMSWITCH_LoadData();
	}
	else
	{
		VacuumVACUUMSWITCH_Cancel();
		CScanView* pView = (CScanView*)theApp.GetScanView();
		// 进度条
		if( pView != NULL )
			pView->FuncAutoWaitHV( TRUE );
	}
}

void CDlgImpPanelAll2::VacuumVACUUMSWITCH_HVCheck( int &nCheck )
{
	// 什么时候可以加高压
	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	22:
	case	21:
		{
			if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM
				//&& (CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.XX == 7)		// TMP运行VacOK状态
				&& CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.VacChambOK )		// 样品室真空正常状态
			{
				if( !m_bSPRRelay1En )
					SPR_En( TRUE );
				if( m_bSPRRelay1En && theApp.HVPower_IsReady() )
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
					//theApp.m_HVPower.CloseDevice();
					m_btnHV.SetCheck(0);
					m_btnBias.SetCheck(0);
					m_btnFilament.SetCheck(0);
					SPR_En( FALSE );
				}
				AfxMessageBox( "Vacuum NOT ready or V1 NOT open!" );
				nCheck = 0;
			}
		}
		break;
	case	0:
		{
			if(CVACDataManager::Instance().CheckVACAndCCG())
			{
				if( !m_bSPRRelay1En )
					SPR_En( TRUE );
				if( m_bSPRRelay1En && ! CCPS3603tatusDetector::Instance().IsReady() )
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
		break;
	default:
		{
			if( CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec
				&& CVACDataManager::Instance().m_statusBoard.Vac.status.X == 7	// TMP运行状态
				&& !(CVACDataManager::Instance().m_statusBoard.Vac.status.YM) )	// 样品室真空正常状态
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
					//theApp.m_HVPower.CloseDevice();
					m_btnHV.SetCheck(0);
					m_btnBias.SetCheck(0);
					m_btnFilament.SetCheck(0);
					SPR_En( FALSE );
				}
				AfxMessageBox( "Vacuum NOT ready or V1 NOT open!" );
				nCheck = 0;
			}
		}
		break;
	}
}

void CDlgImpPanelAll2::VacuumVACUUMSWITCH_LoadData()
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
	int			nTotal	= SetImageData_Counts;
	int*		pCheck	= new int[SetImageData_Counts];
	CString		str;
	sprintf(stabuff, "OneKEY");
	sprintf(staitembuff, "param2");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck) < 2 )
	{
		// 如果ini中没有Align和Stig参数，就按原一键看像参数读取
		sprintf(staitembuff, "param");
		CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
		// Align和Stig参数，就按当前值
		pCheck[8]	= CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos().lVal;
		pCheck[9]	= CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos().lVal;
		pCheck[10]	= CSemCoreAssemble::Instance().m_StigmatorX.GetPos().lVal;
		pCheck[11]	= CSemCoreAssemble::Instance().m_StigmatorY.GetPos().lVal;
	}

	int nHV		= pCheck[0];
	int nBias	= pCheck[1];
	int nFila	= pCheck[2];
	m_nPosB		= pCheck[3];
	m_nPosC		= pCheck[4];
	int	nPosCL	= pCheck[5];
	int	nPosOL	= pCheck[6];
	int	nPosMag	= pCheck[7];
	int nAlignX	= pCheck[8];
	int nAlignY	= pCheck[9];
	int nStigX	= pCheck[10];
	int nStigY	= pCheck[11];
	// 高压步距参数
	nTotal = 4;
	sprintf(staitembuff, "HV");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	int nStepAcc1	= pCheck[0];
	int nStepAcc2	= pCheck[1];
	int nStepBeam	= pCheck[2];
	int nStepFil	= pCheck[3];
	delete [] pCheck;
	///////////////////////////////////////////////////

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	SerialNumber.lVal	= -1;
	Flag.lVal = 3;
	//BeamAlign
	{
		ClassID.lVal	= SemControlPanelClass_BeemAlignment;
		Xpos.lVal		= nAlignX;
		Ypos.lVal		= nAlignY;
		Flag.lVal		= 3;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	//Stigmator
	{
		ClassID.lVal	= SemControlPanelClass_Stigmator;
		Xpos.lVal		= nStigX;
		Ypos.lVal		= nStigY;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}

	Flag.lVal			= 1;
	// CondenserLens
	{
		ClassID.lVal	= SemControlPanelClass_CondenserLens;
		Xpos.lVal		= nPosCL;	// 550.00 [C4C0]
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	// ObjectiveLens
	{
		ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
		Xpos.dblVal		= nPosOL;	// 550.00 [C4C0]
		Xpos.lVal		= nPosOL;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	// Magnifier
	{
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		Xpos.dblVal		= nPosMag;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		CCPS3603tatusDetector::Instance().SetParam( 25, nHV );
		CCPS3603tatusDetector::Instance().SetParam( 26, nBias );
		CCPS3603tatusDetector::Instance().SetParam( 27, nFila );
	}
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		{
			theApp.m_WSM30KV2.SetParam( 41, nHV );
			theApp.m_WSM30KV2.SetParam( 42, nBias );
			theApp.m_WSM30KV2.SetParam( 43, nFila );
			theApp.m_WSM30KV2.SetParam( 44, nStepAcc1 );
			theApp.m_WSM30KV2.SetParam( 45, nStepAcc2 );
			theApp.m_WSM30KV2.SetParam( 46, nStepBeam );
			theApp.m_WSM30KV2.SetParam( 47, nStepFil );
			ChangeBias_WSM30KV2_Max();
		}
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			theApp.m_WSM30KV.SetParam( 41, nHV );
			theApp.m_WSM30KV.SetParam( 42, nBias );
			theApp.m_WSM30KV.SetParam( 43, nFila );
		}
		else
		{
			CSpellmanStatusDetector::Instance().SetParam( 17, nHV );
			CSpellmanStatusDetector::Instance().SetParam( 18, nBias );
			CSpellmanStatusDetector::Instance().SetParam( 19, nFila );
			CSpellmanStatusDetector::Instance().SetParam( 46, nStepAcc1 );
			CSpellmanStatusDetector::Instance().SetParam( 47, nStepAcc2 );
			CSpellmanStatusDetector::Instance().SetParam( 48, nStepBeam );
			CSpellmanStatusDetector::Instance().SetParam( 49, nStepFil );
		}
	}
	// 1. 加亮度及对比度至经验值
	// 2. 自动加高压至设定值
	// 3. 自动加偏压至设定值
	// 41.自动加灯丝至设定值
	// 显示进度条等待加载参数到位
	SetTimer( 11, 150, NULL );
	if( pView != NULL )
		pView->FuncAutoWaitHV( FALSE );
	// 42.启动自动加灯丝程序，从1.8A至第二饱和点
}

void CDlgImpPanelAll2::VacuumVACUUMSWITCH_Cancel()
{
	KillTimer(11);
	KillTimer(12);
	KillTimer(13);
	///////////////////////////////////////////////////
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		CCPS3603tatusDetector::Instance().SetParam( 21, AS_NULL );
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			theApp.m_WSM30KV2.SetParam( 49, WSM30KV2AS_NULL );
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			theApp.m_WSM30KV.SetParam( 49, WSM30KVAS_NULL );
		else
			CSpellmanStatusDetector::Instance().SetParam( 16, AS2_NULL );
	}
	// 高压统调扫描参数
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL && (CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop) )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( CHVPowerDataManager::Instance().m_dHV );
}

//参数设置
void CDlgImpPanelAll2::OnBnClickedBtnSetData()
{
	CDlgSetImageData dlgSetImageData;
	dlgSetImageData.DoModal();
}

void CDlgImpPanelAll2::OnCheckOff() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press clear OFF");
	HVPower_Shutdownbeam();
}

void CDlgImpPanelAll2::HVPower_Shutdownbeam()
{
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
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm )
		pfrm->LoggingFilamentLifetime_End();
	// 停止扫描
//	CScanView* pView = (CScanView*)theApp.GetScanView();
//	pView->USB_ChangeScan( USB_ST_Stop );
	HVPower_Clear();
}

void CDlgImpPanelAll2::HVPower_Clear()
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	SerialNumber.lVal	= -1;
	Xpos.lVal			= 0;
	Flag.lVal			= 1;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		// 1. 自动退灯丝至0
		CCPS3603tatusDetector::Instance().SetParam( 12, 0 );
		ClassID.lVal	= SemControlPanelClass_CPS_FilamentCurrent;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		// 2. 自动退偏压至0
		CCPS3603tatusDetector::Instance().SetParam( 11, 0 );
		ClassID.lVal	= SemControlPanelClass_CPS_Bias;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		// 3. 自动退高压至0
		CCPS3603tatusDetector::Instance().SetParam( 10, 0 );
		ClassID.lVal	= SemControlPanelClass_CPS_HV;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		// 4. 自动退对比度至0
		// Contrast
		ClassID.lVal = SemControlPanelClass_Contrast;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		Sleep(100);
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		CCPS3603tatusDetector::Instance().Release();
	}
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			// 1. 自动退灯丝至0
			m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
			strX = "0.00 A";
			m_ctrlHeater.SyncText( strX, strY, Flag );
			// 2. 自动退偏压至0
			m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
			strX = "0.0 V";
			m_ctrlBias.SyncText( strX, strY, Flag );
			// 3. 自动退高压至0
			ClassID.lVal	= SemControlPanelClass_SP_HV;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			// 4. 自动退对比度至0
			ClassID.lVal = SemControlPanelClass_SP_PMT;
			ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );

			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			{
				ChangeBias_WSM30KV2_Max();
				SetTimer( 50, 500, NULL );

				theApp.m_WSM30KV2.SetParam( 3, 0 );
				theApp.m_WSM30KV2.SetParam( 2, 0 );
				theApp.m_WSM30KV2.SetParam( 1, 0 );
			}
			else
			{
				theApp.m_WSM30KV.SetParam( 3, 0 );
				theApp.m_WSM30KV.SetParam( 2, 0 );
				theApp.m_WSM30KV.SetParam( 1, 0 );
			}
		}
		else
		{
			// 1. 自动退灯丝至0
			m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
			strX = "0.00 A";
			m_ctrlHeater.SyncText( strX, strY, Flag );
			CSpellmanStatusDetector::Instance().SetParam( 34, 0 );
			// 2. 自动退偏压至0
			m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
			strX = "0.0 V";
			m_ctrlBias.SyncText( strX, strY, Flag );
			CSpellmanStatusDetector::Instance().SetParam( 33, 0 );
			// 3. 自动退高压至0
			ClassID.lVal	= SemControlPanelClass_SP_HV;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			CSpellmanStatusDetector::Instance().SetParam( 32, 0 );
			// 4. 自动退对比度至0
			ClassID.lVal = SemControlPanelClass_SP_PMT;
			ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
		}
	}
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL && (CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop) )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
	CHVPowerDataManager::Instance().m_dHV = 0.0;
	CGlobalParamSetting::Instance().g_dHV = CGlobalParamSetting::Instance().g_dHV2 = CGlobalParamSetting::Instance().g_dHV3 = 0.0;
}

void CDlgImpPanelAll2::VAC_Vent( BOOL bGun )
{
	// 真空操作
	if( bGun )
	{
		// 电子枪放气
		m_btnGunVent.SetColor( RGB(192,192,0) );
		theApp.VAC_SaveStates();

		// 关高压、灯丝，对比度退0
		VAC_Reset();
		switch( CSemCtrl::Instance().m_nVACBoardType )
		{
		case	22:
		case	21:
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvSwitch );
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvVentGun );
			break;
		case	0:
			CSMvac::Instance().SetParams( VACIndex_Gun, 1 );
			break;
		default:
			CVACDataManager::Instance().m_BoardVAC.SetParam( 100 +byte_opcVentGun, byte_opcVentGun );
			break;
		}
		m_btnChamberVent.EnableWindow( FALSE );
		theApp.DisplayMsg("Press GUN: start vent");
	}
	else
	{
		// 样品室放气
		m_btnChamberVent.SetColor( RGB(192,192,0) );
		theApp.VAC_SaveStates();

		// 关高压、灯丝，对比度退0
		VAC_Reset();
		switch( CSemCtrl::Instance().m_nVACBoardType )
		{
		case	22:
		case	21:
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvSwitch );
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvVentChamb );
			break;
		case	0:
			CSMvac::Instance().SetParams( VACIndex_Chamb, 1 );
			break;
		default:
			CVACDataManager::Instance().m_BoardVAC.SetParam( 100 +byte_opcVentChamb, byte_opcVentChamb );
			break;
		}
		m_btnGunVent.EnableWindow( FALSE );
		theApp.DisplayMsg("Press CHAMB: start vent");
	}
	VAC_VentPump_StageNAV( TRUE );
}

void CDlgImpPanelAll2::VAC_VentPump_StageNAV( BOOL bVent )
{
	if( theApp.m_bHasNAV )
	{
		switch( m_nNavProcess )
		{
		case	0:
			// 摄像头安装在样品室门的边缘：通常为2024.07之前的方式，或此后改了机械设计后的中型样品室
			// 摄像头安装无角度，即样品台移动到导航原点仅为XYZ有变化
			// 安装传感器；模式选择触发模式，触发源选择LINE0，下降沿触发
			// 放气时回导航原点，且开灯板电源、摄像头准备。通常情况下，放气完成前准备工作均可完成
			// 点击导航面板上的“开始采集”，拉开门时，拍照一次；换样；推进门时拍照一次（ImageTick=2），作为导航图片
			// 弹出是否回扫描原点对话框，用户选是，表示认可导航图片，样品台回扫描原点
			//							用户选否，可再次单击“开始采集”按钮，重新拍导航图片
			if( bVent )
				VAC_VentPump_StageNAV_Prepare();
			break;
		case	1:
			// 摄像头安装在样品室内部：2024.07改了机械设计之后的方案
			// 通常为大样品室，摄像头安装无角度，即样品台移动到导航原点仅为XYZ有变化
			// 不安装传感器；模式选择触发模式，触发源选择SOFTWARE，拍照时需发软件触发命令
			// 放气时不做动作
			// 抽气时回导航原点，且开灯板电源、摄像头准备。
			// 准备工作完成后，自动拍照一次，作为导航图片
			// 弹出是否回扫描原点进度条（按钮为“取消”），10秒钟进度条走完；没有操作则XY回中心、Z向自动回到用户换完样品后的位置
			//							用户点击了“取消”，可再次单击导航面板上的“开始采集”按钮，重新拍导航图片
			if( !bVent )
				VAC_VentPump_StageNAV_Prepare();
			break;
		case	2:
//			VAC_VentPump_StageNAV_Prepare();
			break;
		}
	}
	else
	{
		if( bVent )
		{
			// 如无导航，则按自动台的“放气/退出”设置来移动台子
			if( (CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
				|| ( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
				|| ( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() ) )
			{
				CScanView* pView = (CScanView*)theApp.GetScanView();
				if( pView != NULL )
					pView->AutoStage_Clear(FALSE);
//				SetTimer(31, 250, NULL);
			}
		}
	}
}

void CDlgImpPanelAll2::VAC_VentPump_StageNAV_Prepare()
{
	// 导航灯板先开启，可以让导航相机先初始化（找到相机、打开相机及设置参数等，大约需15-20秒）
	SPR_Fan(TRUE);
	NAV_UpdateStatus( 0, TRUE );
	// 获取并保存自动台当前位置（只有当抽放气时才保存当前位置）
	if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
		StageTMCM_GetParam( 2, m_navPosOrigin.fPosX, m_navPosOrigin.fPosY, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR, m_navPosOrigin.fPosT );
	else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
		StageMC6600_GetParam( 2, m_navPosOrigin.fPosX, m_navPosOrigin.fPosY, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR, m_navPosOrigin.fPosT );
	else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
		StageMC600_GetParam( 2, m_navPosOrigin.fPosX, m_navPosOrigin.fPosY, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR, m_navPosOrigin.fPosT );
	char szBuf[255] = {0};
	sprintf_s( szBuf, "NAV: origin positions: [%.3f, %.3f, %.2f, %.1f, %.1f]", m_navPosOrigin.fPosX, m_navPosOrigin.fPosY, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR, m_navPosOrigin.fPosT );
	theApp.DisplayMsg( szBuf );
	// 回导航原点：R使用当前值，T设置为0（无倾斜）
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
	{
		StageTMCM_Move5Axis(m_navPosStart.fPosX, m_navPosStart.fPosY, m_navPosStart.fPosZ, m_navPosOrigin.fPosR, 0.0);
		SetTimer( 32, 1000, NULL );	// 启动定时器，查询自动台是否到位
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
	{
		StageMC6600_Move5Axis(m_navPosStart.fPosX, m_navPosStart.fPosY, m_navPosStart.fPosZ, m_navPosOrigin.fPosR, 0.0);
		SetTimer( 32, 1000, NULL );	// 启动定时器，查询自动台是否到位
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 && StageMC600_IsReady() )
	{
		StageMC600_Move5Axis(m_navPosStart.fPosX, m_navPosStart.fPosY, m_navPosStart.fPosZ, m_navPosOrigin.fPosR, 0.0);
		SetTimer( 32, 1000, NULL );	// 启动定时器，查询自动台是否到位
	}
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
	{
		StageMC600_Move4Axis(m_navPosStart.fPosX, m_navPosStart.fPosY, m_navPosStart.fPosZ, m_navPosOrigin.fPosR);
		SetTimer( 32, 1000, NULL );	// 启动定时器，查询自动台是否到位
	}

	sprintf_s( szBuf, "NAV: Move Stage to OpenMove position [%.2f, %.2f, %.2f, %.1f, %.1f]",
		m_navPosStart.fPosX, m_navPosStart.fPosY, m_navPosStart.fPosZ, m_navPosOrigin.fPosR, 0.0 );
	theApp.DisplayMsg( szBuf );
	NAV_UpdateStatus(2,TRUE);
	OnBUTTONNavigate();
}

void CDlgImpPanelAll2::OnVacuumGUNVENT() 
{
	// 电子枪放气
	int nCheck = m_btnGunVent.GetCheck();
	if( nCheck )
	{
/*		if( (CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
			|| ( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
			|| ( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() ) )
		{
			CScanView* pView = (CScanView*)theApp.GetScanView();
			if( pView != NULL )
				pView->AutoStage_Clear(FALSE);
			SetTimer(31, 250, NULL);
		}
		else
*/		{
			VAC_Vent( TRUE );

			CVACDataManager::Instance().m_nCCGconnects = 0;
			CVACDataManager::Instance().m_VacTCAmp.nState = 0;
			GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
			GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->Invalidate(TRUE);
		}
	}
	else
	{
		m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		theApp.VAC_SaveStates();
		switch( CSemCtrl::Instance().m_nVACBoardType )
		{
		case	22:
		case	21:
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvSwitch );
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvPump );
			break;
		case	0:
			CSMvac::Instance().SetParams( VACIndex_Gun, 0 );
			break;
		default:
			CVACDataManager::Instance().m_BoardVAC.SetParam( 100 +byte_opcPumpGun, byte_opcPumpGun );
			break;
		}
		m_btnChamberVent.EnableWindow( TRUE );
		theApp.DisplayMsg("Press GUN: start pump");
		VAC_VentPump_StageNAV( FALSE );
	}
}

void CDlgImpPanelAll2::OnVacuumCHAMBERVENT() 
{
	// 样品室放气
	int nCheck = m_btnChamberVent.GetCheck();
	if( nCheck )
	{
/*		if( (CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
			|| ( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
			|| ( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() ) )
		{
			CScanView* pView = (CScanView*)theApp.GetScanView();
			if( pView != NULL )
				pView->AutoStage_Clear(FALSE);
			SetTimer(31, 250, NULL);
		}
		else
*/		{
			VAC_Vent( FALSE );

			CVACDataManager::Instance().m_nCCGconnects = 0;
			CVACDataManager::Instance().m_VacTCAmp.nState = 0;
			GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
			GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->Invalidate(TRUE);
		}
	}
	else
	{
		m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
		theApp.VAC_SaveStates();

		switch( CSemCtrl::Instance().m_nVACBoardType )
		{
		case	22:
		case	21:
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvSwitch );
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvPump );
			break;
		case	0:
			CSMvac::Instance().SetParams( VACIndex_Chamb, 0 );
			break;
		default:
			CVACDataManager::Instance().m_BoardVAC.SetParam( 100 +byte_opcPumpChamb, byte_opcPumpChamb );
			break;
		}
		m_btnGunVent.EnableWindow( TRUE );
		theApp.DisplayMsg("Press CHAMB: start pump");
		VAC_VentPump_StageNAV( FALSE );
	}
}

void CDlgImpPanelAll2::OnVacuumVACUUMSTANDBY() 
{
	// 待机
	if( CSemCtrl::Instance().m_nVACBoardType < 21 )
		return;
	int nCheck = m_btnVacuumStandby.GetCheck();
	if( nCheck )
	{
		m_btnVacuumStandby.SetColor( RGB(0,0,255) );
		CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvStandby );
		theApp.DisplayMsg("Press STANDBY: start standby");
	}
	else
	{
		m_btnVacuumStandby.SetColor( ::GetSysColor(COLOR_3DFACE) );
		CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvPump );
		theApp.DisplayMsg("Press STANDBY: start pump");
	}
}

void CDlgImpPanelAll2::OnCHECKV1() 
{
	// TODO: Add your control notification handler code here
	if(m_bNavPowerOn)
	{
		CString str = "请先停止采集。";	// GetResString(IDS_SD_PROMPT_INT_03);
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += str;
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnV1.SetCheck(0);
		return;
	}
	// 气动V1阀控制
	int nCheck = m_btnV1.GetCheck();
	VAC_CheckV1( nCheck );
}

void CDlgImpPanelAll2::VAC_CheckV1( int nCheck )
{
	if( nCheck )
	{
		m_btnV1.SetColor( RGB(0,192,0) );
		switch( CSemCtrl::Instance().m_nVACBoardType )
		{
		case	22:
		case	21:
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_V1, byte_PLCvacBRDopr_Open );
			break;
		case	0:
			CSMvac::Instance().SetParams( 24, 1 );
			break;
		default:
			CVACDataManager::Instance().m_BoardVAC.SetParam( 100 +byte_opcV1, 0x01 );
			break;
		}
		theApp.DisplayMsg("Press V1: open V1");
	}
	else
	{
		m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
		switch( CSemCtrl::Instance().m_nVACBoardType )
		{
		case	22:
		case	21:
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_V1, byte_PLCvacBRDopr_Close );
			break;
		case	0:
			CSMvac::Instance().SetParams( 24, 0 );
			break;
		default:
			CVACDataManager::Instance().m_BoardVAC.SetParam( 100 +byte_opcV1, 0x02 );
			break;
		}
		theApp.DisplayMsg("Press V1: close V1");
	}
}

BOOL CDlgImpPanelAll2::VAC_Init()
{
	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"VAC_Init\r\n" );
	OutputDebugString(sz);

	BOOL bRet = FALSE;
	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	22:
	case	21:
		{
			VAC_VacuumReady2( FALSE );
			CVACDataManager::Instance().m_VacParamMain.bHVInt	= TRUE;	// 使用PLC真空板，默认标志位为1
			///////////////////////////////////////////////////////
			// 初始化
			ZeroMemory( &(CVACDataManager::Instance().m_cfgPLCvacBrd), sizeof(Cfg_PLCvacBrd) );
			ZeroMemory( &(CVACDataManager::Instance().m_StatusPLCvacBrd), sizeof(Status_PLCvacBrd) );
			CPLCvacBrdStatusDetector::Instance().Init( this->m_hWnd, CVACDataManager::Instance().m_cfgVacCCG.nType -3 );
			CPLCvacBrdStatusDetector::Instance().Start();
			GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
			bRet = TRUE;
			///////////////////////////////////////////////////////
		}
		break;
	case	0:
	    {
			if( CVACDataManager::Instance().m_cfgVacCCG.nType > 3 )	// WPC
			{
				char szBuf[10], szBuf1[10];
				sprintf( szBuf, "%.1e", CVACDataManager::Instance().m_cfgVacCCG.dThreshold1 );
				strcpy(szBuf1, strrchr(szBuf, 'e')+1);
				*strchr(szBuf, 'e') = '\0';
				int nValue = (int)(atof(szBuf) *100 -atoi(szBuf1) +0.5);
				CWPCStatusDetector::Instance().Init( this->m_hWnd,CVACDataManager::Instance().m_cfgVacCCG.nPort );
 				CWPCStatusDetector::Instance().InitThreshold(nValue,nValue);
				CWPCStatusDetector::Instance().Start();
			}
			else
			{
				CCCGStatusDetector::Instance().Init( this->m_hWnd,CVACDataManager::Instance().m_cfgVacCCG.nPort );
 				CCCGStatusDetector::Instance().InitThreshold(CVACDataManager::Instance().m_cfgVacCCG.dThreshold1,CVACDataManager::Instance().m_cfgVacCCG.dThreshold2);
				CCCGStatusDetector::Instance().Start();
			}

			SetTimer( 4, 5000, NULL );

			///////////////////////////////////////////////////////
			// 初始化状态机
			VAC_VacuumReady2( TRUE );

			bRet = CVACDataManager::Instance().CheckVACConnection(this);
			if( !bRet )
			{
					// 调试模式/未连下位机
				CSMvac::Instance().SetParams(20, 1);
				CSMvac::Instance().SetParams(23, CVACDataManager::Instance().m_dwV1CtrlMode > 0);
				VAC_VacuumReady2( FALSE );
				return bRet;
			}
		
			///////////////////////////////////////////////////////
			// 通过记录看是否上次退出时真空电源是打开的
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


			CVACDataManager::Instance().SetParam_Power();

			if(CVACDataManager::Instance().m_VacParamMain.bChamb )
			{
				m_btnChamberVent.SetColor( RGB(192,192,0) );
				m_btnGunVent.EnableWindow( FALSE );
			}
			else
			{
				m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
				m_btnGunVent.EnableWindow( TRUE );
			}

			CVACDataManager::Instance().SetParam_Chamb();

			if(CVACDataManager::Instance().m_VacParamMain.bGun )
			{
				m_btnGunVent.SetColor( RGB(192,192,0) );
				m_btnChamberVent.EnableWindow( FALSE );
			}
			else
			{
				m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
				m_btnChamberVent.EnableWindow( TRUE );
			}
			CVACDataManager::Instance().SetParam_Gun();
		
			// 幂函数系数
			// 获取工作路径
			char path[256];
			::GetModuleFileName(0, path, 255);
			char *exd = strrchr(path, (int)'\\');
			if(exd != NULL)
				*exd = '\0';
			strcat(path, "\\KYKYcfg.ini");

			CVACDataManager::Instance().LoadVACFactor(path);
		
			VAC_UpdateUIswitches( TRUE );

			ZeroMemory(sz,512);
			sprintf_s(sz,512,"VAC_Init(): VAC_UpdateUIswitches( TRUE )\r\n" );
			OutputDebugString(sz);
		}
		break;
	default:
		{
			VAC_VacuumReady2( FALSE );
			CVACDataManager::Instance().m_VacParamMain.bHVInt	= TRUE;	// 统一真空板默认为TRUE
			///////////////////////////////////////////////////////
			// 初始化
			CVACDataManager::Instance().m_BoardVAC.SetParam( 21, CVACDataManager::Instance().m_cfgVacCCG.nType );
			CVACDataManager::Instance().m_BoardVAC.SetParam( 4, CVACDataManager::Instance().m_portBoard.nPort );
			CVACDataManager::Instance().m_BoardVAC.InitHWND( this->m_hWnd );
			CVACDataManager::Instance().m_BoardVAC.Init(0);	// 连接真空控制板Board
			bRet = TRUE;
		}
		break;
	}

	// IPG
	if( CSemCtrl::Instance().m_nVACBoardType > 1 && CSemCtrl::Instance().m_nVACBoardType < 10 )
	{
		// 要连接IPG
		GetDlgItem(IDC_STATIC_STATUS_Vacuum)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_STATUS_Vacuum)->SetWindowText("");
		// 获取工作路径
		char path[256], szBuf[20];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");
		// IPG
		if(GetPrivateProfileString("FEGParam", "IPGport", "", szBuf, 10, path) != 0 )
		{
			CVACDataManager::Instance().m_cfgVacIPG.nPort = atoi( szBuf );
			if(GetPrivateProfileString("FEGParam", "IPtype", "", szBuf, 10, path) != 0 )
			{
				int nType = atoi( szBuf );
				if(GetPrivateProfileString("FEGParam", "IPGchn", "", szBuf, 10, path) != 0 )
				{
					int nChn = atoi( szBuf );	
					BOOL bIsDual = CVACDataManager::Instance().m_cfgVacIPG.nPort == CVACDataManager::Instance().m_cfgVacIPC.nPort;
					CIPGStatusDetector::Instance().Init(this->m_hWnd ,CVACDataManager::Instance().m_cfgVacIPG.nPort,nType,bIsDual);
					CIPGStatusDetector::Instance().SetChannel(nChn,CIPCStatusDetector::Instance().m_strWinIPC);
					CIPGStatusDetector::Instance().Start();  // 连接IPG
				}
			}
		}
	}

	return bRet;
}

void CDlgImpPanelAll2::VAC_Reset()
{
	theApp.DisplayMsg( "VAC_Reset: set HV/Fila/Contrast to 0");
	// 关高压、灯丝，对比度退0
	// 对比度
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
	}
	else
	{
		VARIANT Flag;
		Flag.lVal = 1;
		CString strX = CSemCoreAssemble::Instance().m_SP_PMT.FormatText();
		m_ctrlContrast.SyncText( strX, strX, Flag );
	}

	CScanView* pView = (CScanView*)theApp.GetScanView();
	ASSERT(pView);
	CHVPowerDataManager::Instance().VAC_Reset_ByHVType(CScanParameterManager::Instance().m_nusbScanType);

}

void CDlgImpPanelAll2::VAC_VacuumReady( BOOL bReady )
{
	m_btnHV.EnableWindow( bReady );
	m_btnBias.EnableWindow( bReady );
	m_btnFilament.EnableWindow( bReady );
	m_btnVacuumSwitch.EnableWindow( bReady );
	GetDlgItem( IDC_CHECK_OFF )->EnableWindow( bReady );
}

void CDlgImpPanelAll2::VAC_VacuumReady2( BOOL bReady )
{
	m_btnVacuumPower.EnableWindow( bReady );
	m_btnChamberVent.EnableWindow( bReady );
	m_btnGunVent.EnableWindow( bReady );
	m_btnVacuumStandby.EnableWindow( bReady );
	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	0:
		m_btnV1.EnableWindow( bReady && (CVACDataManager::Instance().m_VacTCAmp.nState == 2) );
		break;
	default:
		m_btnV1.EnableWindow( bReady && (m_nHIstate == 2) );
		break;
	}
}

BOOL CDlgImpPanelAll2::VAC_UpdatePneumaticDiagram()
{
	// 更新气路图的各阀灯
	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	0:
		VAC_UpdatePneumaticDiagram_Old();
		break;
	case	21:
	case	22:
		VAC_UpdatePneumaticDiagram_PLC();
		break;
	default:
		VAC_UpdatePneumaticDiagram_New();
		break;
	}
	return TRUE;
}

void CDlgImpPanelAll2::VAC_UpdatePneumaticDiagram_Old()
{
	// VAC子卡
	CStatic* pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vf );
	if( pLamp != NULL )
	{
		if (CVACDataManager::Instance().m_VacParamMain.bVf )
		{
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		}
		else
		{
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
		}
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

	if( CVACDataManager::Instance().m_dwV1CtrlMode> 0 )
	{
		if(CVACDataManager::Instance().m_VacParamMain.bV1 )
		{
			m_btnV1.SetCheck( 1 );
			m_btnV1.SetColor( RGB(0,192,0) );
			theApp.DisplayMsg("Update V1: open V1");
			m_btnV1.Invalidate();
		}
		else
		{
			m_btnV1.SetCheck( 0 );
			m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
			theApp.DisplayMsg("Update V1: close V1");
			m_btnV1.Invalidate();
		}
		NAV_SetV1Status(CVACDataManager::Instance().m_VacParamMain.bV1);
	}
}

void CDlgImpPanelAll2::VAC_UpdatePneumaticDiagram_New()
{
	CStatic* pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vf );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_statusBoard.Valve.status.V2 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	if( CSemCtrl::Instance().m_nVACBoardType < 2 )
	{
		pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vc );
		if( pLamp != NULL )
		{
			if ( CVACDataManager::Instance().m_statusBoard.Valve.status.V3 )
				pLamp->SetBitmap((HBITMAP)(m_bmpON));
			else
				pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
		}
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V4 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_statusBoard.Valve.status.V4 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V5 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_statusBoard.Valve.status.V5 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_TMP );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_statusBoard.Valve.status.TMP )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_CCG );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_statusBoard.Valve.status.CCG )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V1 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}
}

void CDlgImpPanelAll2::VAC_UpdatePneumaticDiagram_PLC()
{
	CStatic* pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vf );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V2 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vc );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V3 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V4 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V4 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V5 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V5 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_TMP );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.TMPErr
			|| CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.TMPFail
			|| CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.TMPLink )
			pLamp->SetBitmap((HBITMAP)(m_bmpERR));
		else if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.TMP )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_CCG );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.CCGErr )
			pLamp->SetBitmap((HBITMAP)(m_bmpERR));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_MP );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.MPErr )
			pLamp->SetBitmap((HBITMAP)(m_bmpERR));
		else if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.MP )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

/*	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_CCD );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.CCD )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}
*/
	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V1 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}
}

BOOL CDlgImpPanelAll2::VAC_UpdateUIswitches( BOOL bReset )
{
	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	22:
	case	21:
		VAC_UpdateUIswitches_PLC();
		break;
	case	0:
		VAC_UpdateUIswitches_Old( bReset );
		break;
	default:
		VAC_UpdateUIswitches_New();
		break;
	}

	VAC_UpdatePneumaticDiagram();
	return TRUE;
}

void CDlgImpPanelAll2::VAC_UpdateUIswitches_Old( BOOL bReset )
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

	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"VAC_UpdateUIswitches: bReset = %d, CheckVACAndCCG() = %d\r\n",
		bReset,CVACDataManager::Instance().CheckVACAndCCG() );
	OutputDebugString(sz);

	// 什么时候可以加高压
	if( CVACDataManager::Instance().CheckVACAndCCG() )
	{
		CVACDataManager::Instance().m_VacParamMain.bHVInt	= TRUE;
		SPR_En( TRUE );
		// 发起扫描
//		CScanView* pView = (CScanView*)theApp.GetScanView();
//		pView->USB_ChangeScan( USB_SF_RasterScan );
	}
	else
	{
		CVACDataManager::Instance().m_VacParamMain.bHVInt	= FALSE;
		SPR_En( FALSE );
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
}

void CDlgImpPanelAll2::VAC_UpdateUIswitches_New()
{
	BOOL bUpdate = FALSE;
	StatusVac6K	status = CVACDataManager::Instance().m_statusBoard.Vac.status;
	// 只有“枪真空正常状态”下，才可以开V1
	// 如果枪真空状态前后不同
	if( CVACDataManager::Instance().m_statusBoardBak.Vac.status.YM != status.YM )
	{
		bUpdate = TRUE;
		if( status.YM )
			theApp.DisplayMsg("枪真空错误");
		else
			theApp.DisplayMsg("枪真空正常");
		if( status.YM )	// 枪真空错误
		{
			// 如果V1已开启，要将V1关闭
			if( CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec )
			{
				CVACDataManager::Instance().m_BoardVAC.SetParam(105, 2);
				theApp.DisplayMsg("Vacuum is error, close V1 now");
			}
			if( m_bSPRRelay1En )
			{
				theApp.DisplayMsg("Gun Vacuum Error! Shut down SPR En");
				SPR_En(FALSE);
			}
			m_btnV1.SetCheck(0);
			m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
			m_btnV1.EnableWindow( FALSE );
		}
		else
			m_btnV1.EnableWindow( TRUE );
	}
	if( CVACDataManager::Instance().m_statusBoardBak.Valve.status.V1Rec != CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec )
	{
		if( CVACDataManager::Instance().m_dwV1CtrlMode > 0 )
		{
			if( CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec )
			{
				m_btnV1.SetCheck( 1 );
				m_btnV1.SetColor( RGB(0,192,0) );
				theApp.DisplayMsg("Update V1: open V1");
				m_btnV1.Invalidate();
				m_btnChamberVent.EnableWindow( FALSE );
				m_btnGunVent.EnableWindow( FALSE );
			}
			else
			{
				m_btnV1.SetCheck( 0 );
				m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
				theApp.DisplayMsg("Update V1: close V1");
				m_btnV1.Invalidate();
				m_btnChamberVent.EnableWindow( TRUE );
				m_btnGunVent.EnableWindow( TRUE );
			}
			NAV_SetV1Status(CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec);
		}
	}
	if( CVACDataManager::Instance().m_statusBoardBak.Valve.status.V4 != CVACDataManager::Instance().m_statusBoard.Valve.status.V4 )
	{
		if( CVACDataManager::Instance().m_statusBoard.Valve.status.V4 )
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
			// 放气之后不显示真空度
			CVACDataManager::Instance().m_valueCCG.dVac = 0.0;
			GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
			GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->Invalidate(TRUE);
		}
		else
		{
			// 只要真空电源打开且V1关时，就可以放气
			if( CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec )
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
	}

	// 什么时候可以加高压
	if( CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec
		&& CVACDataManager::Instance().m_statusBoard.Vac.status.X == 7	// TMP运行状态
		&& !(CVACDataManager::Instance().m_statusBoard.Vac.status.YM) )	// 样品室真空正常状态
		SPR_En( TRUE );
	else
		SPR_En( FALSE );

	if( CVACDataManager::Instance().m_statusBoardBak.Vac.status.X != CVACDataManager::Instance().m_statusBoard.Vac.status.X )
	{
		bUpdate = TRUE;
		if( CVACDataManager::Instance().m_statusBoard.Vac.status.X == 2 )
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
		if( CVACDataManager::Instance().m_statusBoard.Vac.status.X == 1 )
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
	}
}

void CDlgImpPanelAll2::VAC_UpdateUIswitches_PLC()
{
	BOOL bUpdate = FALSE;
	// 只有“样品室真空达到阈值”下，才可以开V1
	// 如果枪真空状态前后不同
	if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.status.VacChambOK != CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.VacChambOK )
	{
		bUpdate = TRUE;
		if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.status.VacChambOK )
			theApp.DisplayMsg("PLC: Chamb vacuum OK now");
		else
			theApp.DisplayMsg("PLC: Chamb vacuum NOT OK now");
		if( !(CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.status.VacChambOK) )	// 未到阈值
		{
			// 如果V1已开启，要将V1关闭
			if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V1_SEM )
			{
				CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_V1, byte_PLCvacBRDopr_Close );
				theApp.DisplayMsg("PLC: Vacuum is error, close V1 now");
			}
			if( m_bSPRRelay1En )
			{
				theApp.DisplayMsg("Gun Vacuum Error! Shut down SPR En");
				SPR_En(FALSE);
			}
			m_btnV1.SetCheck(0);
			m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
			m_btnV1.EnableWindow( FALSE );
		}
		else
			m_btnV1.EnableWindow( TRUE );
	}
	if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.mode != CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.mode )
	{
		bUpdate = TRUE;
		if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.mode )
			VAC_VacuumReady2( TRUE );
		else
			VAC_VacuumReady2( FALSE );
	}
	if( !(CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.mode) )
		return;
	if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.wValve != CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.wValve )
	{
		bUpdate = TRUE;
		// 阀门状态有不同
		if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V1_SEM != CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM )
		{
			if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.mode )
			{
				if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V1_SEM )
				{
					m_btnV1.SetCheck( 1 );
					m_btnV1.SetColor( RGB(0,192,0) );
					theApp.DisplayMsg("Update V1: open V1");
					m_btnV1.Invalidate();
					m_btnChamberVent.EnableWindow( FALSE );
					m_btnGunVent.EnableWindow( FALSE );
				}
				else
				{
					m_btnV1.SetCheck( 0 );
					m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
					theApp.DisplayMsg("Update V1: close V1");
					m_btnV1.Invalidate();
					m_btnChamberVent.EnableWindow( TRUE );
					m_btnGunVent.EnableWindow( TRUE );
				}
			}
		}
		if( (CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V4 != CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V4)
			|| (CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V5 != CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V5)
			|| (CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V3 != CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V3) )
		{
			// 仅关注V4、V5均关闭的非放气情况，V4和/或V5已开的情况，必定XX状态不同（可适用下面XX不同的判断）
			if( !(CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V4) && !(CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V5) )
			{
				// 只要真空电源打开且V1关时，就可以放气
				if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V1_SEM )
				{
					m_btnChamberVent.EnableWindow( FALSE );
					m_btnGunVent.EnableWindow( FALSE );
				}
				else
				{
					m_btnChamberVent.EnableWindow( TRUE );
					// 当V3关时，不允许电子枪放气
					if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V3 )
						m_btnGunVent.EnableWindow( TRUE );
					else
						m_btnGunVent.EnableWindow( FALSE );
				}
			}
		}
	}

	if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.status.XX != CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.XX )
	{
		bUpdate = TRUE;
		if( (CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.status.XX > 9 && CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.status.XX < 13)
			|| (CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.status.XX == 1) )
		{
			// 处于放气状态
			// 放气之后不显示真空度
			CVACDataManager::Instance().m_valueCCG.dVac = 0.0;
			GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
			if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V3 )
			{
				// 电子枪放气状态
				m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
				m_btnChamberVent.SetCheck(0);
				m_btnChamberVent.EnableWindow( FALSE );
				m_btnGunVent.SetColor( RGB(192,192,0) );
				m_btnGunVent.SetCheck(1);
			}
			else
			{
				// 样品室放气状态
				m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
				m_btnGunVent.SetCheck(0);
				m_btnGunVent.EnableWindow( FALSE );
				m_btnChamberVent.SetColor( RGB(192,192,0) );
				m_btnChamberVent.SetCheck(1);
			}
		}
		if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.status.XX == 19 )
		{
			m_btnVacuumStandby.SetColor( RGB(0,0,255) );
			m_btnVacuumStandby.SetCheck(1);
			// 待机时，因V3已关，故不允许电子枪放气
			m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
			m_btnGunVent.SetCheck(0);
			m_btnGunVent.EnableWindow( FALSE );
			m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
			m_btnChamberVent.SetCheck(0);
//			m_btnChamberVent.EnableWindow( FALSE );
		}
		else
		{
			m_btnVacuumStandby.SetColor( ::GetSysColor(COLOR_3DFACE) );
			m_btnVacuumStandby.SetCheck(0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
LRESULT CDlgImpPanelAll2::OnSMvacChanged(WPARAM wParam, LPARAM lParam)
{
	BOOL bVf =CVACDataManager::Instance().m_VacParamMain.bVf;
	if( lParam < 50 )
	{
		CVACDataManager::Instance().m_VacParamMain = *((SMvac_Param*)wParam);
		CGlobalParamSetting::Instance().g_VacParamMain = *((SMvac_Param*)wParam);
		theApp.VAC_SaveStates();
		VAC_UpdatePneumaticDiagram();

		char sz[512] = {0};
		sprintf_s(sz,512,"OnSMvacChanged, lParam = %d, VAC_UpdateUIswitches()\r\n",lParam );
		OutputDebugString(sz);

		VAC_UpdateUIswitches();
		if( !bVf &&CVACDataManager::Instance().m_VacParamMain.bVf )//&& CVACDataManager::Instance().m_cfgVacCCG.bReady )
		{
			CString str = GetResString(IDS_SD_PROMPT_VAC_27);
			theApp.DisplayMsg( str );

			//for(int i=0; i<500000000; i++)		// 1000000次开平方运算16ms，10000次开平方运算16us
			//	double x = sqrt(4.);			// 延迟8秒后再重新建立连接

			//Sleep(5000);
			//CCCGStatusDetector::Instance().Start();
			SetTimer( 5, 5000, NULL );

		}
		else if( bVf && !(CVACDataManager::Instance().m_VacParamMain.bVf) )//&& CVACDataManager::Instance().m_cfgVacCCG.bReady )
		{
			if( CVACDataManager::Instance().m_cfgVacCCG.nType > 3 )	// WPC
			{
				if (CWPCStatusDetector::Instance().m_cfgWPC.bReady)
				{
					CWPCStatusDetector::Instance().Release();
				}
				else
				{
					CWPCStatusDetector::Instance().Start();
				}		
			}
			else
			{
				//if (CCCGStatusDetector::Instance().m_cfgCCG.bReady)
				//{
				//	CCCGStatusDetector::Instance().Release();
				//}
				//else
				//{
				//	CCCGStatusDetector::Instance().Start();
				//}			
			}

			CVACDataManager::Instance().m_VacTCAmp.nState = 0;
			Sleep(1000);
			GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
			CString str = GetResString(IDS_SD_PROMPT_VAC_28);
			theApp.DisplayMsg( str );
			CVACDataManager::Instance().m_cfgVacCCG.dVac = 0;
			CGlobalParamSetting::Instance().g_cfgVacCCG.dVac = 0;
			Invalidate(FALSE);
		}

		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon->m_hWnd )
				pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_VAC_MSG, wParam, lParam);
		}
		else
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			{
				if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon3->m_hWnd )
					pfrm->m_dlgDebug.m_pPageMon3->PostMessage(WM_USER_VAC_MSG, wParam, lParam);
			}
			else
			{
				if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon2->m_hWnd )
					pfrm->m_dlgDebug.m_pPageMon2->PostMessage(WM_USER_VAC_MSG, wParam, lParam);
			}
		}
	}
	
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
		case	51:
			str = "Reset sub boards from VACSM";
			theApp.DisplayMsg( str );
			break;
		}
		m_nVACMsgType	= lParam;
	}

	return 0;
}

LRESULT CDlgImpPanelAll2::OnSMvacTCAmpUpdate(WPARAM wParam, LPARAM lParam)
{
	SMvac_TCAmp tcAmp = *((SMvac_TCAmp*)wParam);

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( lParam < 1 )
	{
		CVACDataManager::Instance().m_VacTCAmp.dTCfAmp = tcAmp.dTCfAmp;
		CGlobalParamSetting::Instance().g_VacTCAmp.dTCfAmp = tcAmp.dTCfAmp;
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			pfrm->m_dlgDebug.m_pPageMon->VAC_UpdateSMvacTCAmpUpdate( lParam, tcAmp.dTCfAmp );
		else
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				pfrm->m_dlgDebug.m_pPageMon3->VAC_UpdateSMvacTCAmpUpdate( lParam, tcAmp.dTCfAmp );
			else
				pfrm->m_dlgDebug.m_pPageMon2->VAC_UpdateSMvacTCAmpUpdate( lParam, tcAmp.dTCfAmp );
		}
		// 更新TCf进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCf)))->SetPos( (int)((tcAmp.dTCfAmp -CVACDataManager::Instance().m_VacTCAmp.dTCfMin) / (CVACDataManager::Instance().m_VacTCAmp.dTCfMax -CVACDataManager::Instance().m_VacTCAmp.dTCfMin) *100 ));
	}
	else
	{
		CVACDataManager::Instance().m_VacTCAmp.dTCcAmp = tcAmp.dTCcAmp;
		CGlobalParamSetting::Instance().g_VacTCAmp.dTCcAmp = tcAmp.dTCcAmp;
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			pfrm->m_dlgDebug.m_pPageMon->VAC_UpdateSMvacTCAmpUpdate( lParam, tcAmp.dTCcAmp );
		else
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				pfrm->m_dlgDebug.m_pPageMon3->VAC_UpdateSMvacTCAmpUpdate( lParam, tcAmp.dTCcAmp );
			else
				pfrm->m_dlgDebug.m_pPageMon2->VAC_UpdateSMvacTCAmpUpdate( lParam, tcAmp.dTCcAmp );
		}
		// 更新TCc进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCc)))->SetPos( (int)((tcAmp.dTCcAmp -CVACDataManager::Instance().m_VacTCAmp.dTCcMin) / (CVACDataManager::Instance().m_VacTCAmp.dTCcMax -CVACDataManager::Instance().m_VacTCAmp.dTCcMin) *100 ));
	}
	return 0;
}


LRESULT CDlgImpPanelAll2::OnVACUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	if( wParam < 5 )
	{
		CString strDevice, strStatus, str;
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
		switch( wParam )
		{
		case	1:
			strDevice = "IPG";
			str.Format( "%s %s", strDevice, strStatus );
			break;
		case	3:
			strDevice = GetResString(IDS_SD_PROMPT_VAC_35);
			str.Format( GetResString(IDF_SD_PROMPT_VAC_40), CVACDataManager::Instance().m_nCCGconnects+1, strDevice, strStatus );
			break;
		default:
			strDevice = GetResString(IDS_SD_PROMPT_VAC_36);
			break;
		}
		theApp.DisplayMsg( str );

		switch( wParam )
		{
		case	3:
			CVACDataManager::Instance().m_cfgVacCCG.bReady	= lParam;
			if( CVACDataManager::Instance().m_cfgVacCCG.bReady )
			{
				CVACDataManager::Instance().m_nCCGconnects = 0;
				m_nCCG_Counts	= 0;
				m_bCCGBad		= FALSE;
			}
			break;
		}
	}
	else
	{
		CString str;
		switch( wParam )
		{
		case	6:
			break;
		case	7:
			break;
		case	8:
			CVACDataManager::Instance().m_cfgVacCCG = *((CfgVAC_IP_CCG*)lParam);
			str.Format( "Read from device: Threshold Upper = %.2e, Threshold Lower = %.2e", CVACDataManager::Instance().m_cfgVacCCG.dThreshold1, CVACDataManager::Instance().m_cfgVacCCG.dThreshold2 );
			theApp.DisplayMsg( str );
			break;
		}
	}
	return 0;
}

LRESULT CDlgImpPanelAll2::OnVACUpdateCCGValue(WPARAM wParam, LPARAM lParam)
{
	CfgVAC_IP_CCG VacCCG = *((CfgVAC_IP_CCG*)lParam);
	
	CString str;
	str.Format( "%.2e", VacCCG.dVac );
	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
		CSMvac::Instance().SetParams2( 0, VacCCG.dVac );
	if( str.Left(3) == "0.0" )
	{
		GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
		return 0;
	}

	GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( str );
	GetDlgItem(IDC_STATIC_STATUS_Vacuum_Unit)->Invalidate(TRUE);

	double dVac = atof( str );	//theApp.ScientificToDouble( str );

	int nState = CVACDataManager::Instance().m_VacTCAmp.nState;
	if( dVac > CVACDataManager::Instance().m_cfgVacCCG.dThreshold2 )
		CVACDataManager::Instance().m_VacTCAmp.nState = 0;
	else if( dVac < CVACDataManager::Instance().m_cfgVacCCG.dThreshold1 )
		CVACDataManager::Instance().m_VacTCAmp.nState = 2;
	else
		CVACDataManager::Instance().m_VacTCAmp.nState = 1;
	// 16.07 V1阀控制方式
	if( (CVACDataManager::Instance().m_dwV1CtrlMode> 0) && (CVACDataManager::Instance().m_VacTCAmp.nState == 2) )
	{
		m_btnV1.EnableWindow( TRUE );
		m_btnV1.Invalidate(TRUE);
	}

	m_nCCG_Counts++;
	if( m_nCCG_Counts > 100000 )	// 60*60*24 =86400
		m_nCCG_Counts = 0;
	if( dVac > CVACDataManager::Instance().m_cfgVacCCG.dThreshold2 )								// 本次真空度低于低点
	{
		if( !m_bCCGBad														// 第一次
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
			char sz[512] = {0};
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"OnVACUpdateCCGValue, m_bCCGBad = %d, m_bSPRRelay1En = %d ,CCG value BAD, twice \r\n", m_bCCGBad, m_bSPRRelay1En);
			OutputDebugString(sz);

			// 连续第二次监测到真空变坏
			m_bCCGBad = FALSE;
			theApp.DisplayMsg("CCG value BAD, twice");
			if( m_bSPRRelay1En )
			{
				theApp.DisplayMsg("CCG value BAD, twice.  Shut down SPR En");
				SPR_En(FALSE);
			}
			else
			{
				m_btnV1.SetCheck(0);
				m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
				m_btnV1.EnableWindow( FALSE );
				m_btnV1.Invalidate(TRUE);
				if( CSemCtrl::Instance().m_nVACBoardType < 1 )
					CSMvac::Instance().SetParams(24, 0);
			}
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
	CGlobalParamSetting::Instance().g_cfgVacCCG.dVac = VacCCG.dVac;
//	CVACDataManager::Instance().m_cfgVacCCG.dVacBak = CVACDataManager::Instance().m_cfgVacCCG.dVac;
	if( nState != CVACDataManager::Instance().m_VacTCAmp.nState )
		Invalidate( FALSE );
	return 0;
}



HBRUSH CDlgImpPanelAll2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		int nID = pWnd->GetDlgCtrlID();
		if( nID == IDC_STATIC_STATUS_Vacuum_Unit )
		{
			if( CSemCtrl::Instance().m_nVACBoardType < 1 )
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
			else
			{
				if( m_nHIstate < 2 )
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
				if( CSemCtrl::Instance().m_nVACBoardType > 20 )
				{
					if( CVACDataManager::Instance().m_cfgPLCvacBrd.bReady
						&& (CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.CCGErr
							|| (CVACDataManager::Instance().m_StatusPLCvacBrd.dPccg < 1.0E-8) ) )
					{
						pDC->SetTextColor(RGB(255,255,255));
						pDC->SetBkMode(TRANSPARENT);
						hbr = (HBRUSH)m_brushVacErr;
					}
				}
			}
		}
	}

	return hbr;
}


LRESULT CDlgImpPanelAll2::OnVACUpdateIPGValue(WPARAM wParam, LPARAM lParam)
{
	CVACDataManager::Instance().m_cfgVacIPG = *((CfgVAC_IP_CCG*)lParam);
	CString str;
	str.Format( "%.2e", CVACDataManager::Instance().m_cfgVacIPG.dVac );
	GetDlgItem( IDC_STATIC_STATUS_Vacuum )->SetWindowText( str );
	return 0;
}
//////////////////////////////////////////////////////////////////////////////

void CDlgImpPanelAll2::UpdateStatus_BoardCCG()
{
	CString str;
	str.Format( "%.2e", CVACDataManager::Instance().m_valueCCG.dVac );
	if( str.Left(3) == "0.0" )
		return;
	if( CVACDataManager::Instance().m_statusBoard.Valve.status.CCG )
		GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( str );
	else
		GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
	GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->Invalidate(TRUE);
	double dVac = CVACDataManager::Instance().m_valueCCG.dVac;
	
	int nState = m_nHIstate;
	if( dVac > CVACDataManager::Instance().m_thresholdsVAC.fCCG1 )		// 差于差值
		m_nHIstate = 0;
	else if( dVac < CVACDataManager::Instance().m_thresholdsVAC.fCCG2 )	// 优于优值
		m_nHIstate = 2;
	else
		m_nHIstate = 1;
	if( nState != m_nHIstate )
		Invalidate( FALSE );
}

void CDlgImpPanelAll2::UpdateStatus_BoardTCs(WPARAM wParam, LPARAM lParam)
{
	CVACDataManager::Instance().m_valueTC = *((VACvalue6K_TC*)lParam);
	if( wParam == byte_opcTC3 )
	{
		// 更新TCf进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCf)))->SetPos( (int)((CVACDataManager::Instance().m_valueTC.fTC3 -CVACDataManager::Instance().m_thresholdsVAC.fTC3Min) / (CVACDataManager::Instance().m_thresholdsVAC.fTC3Max -CVACDataManager::Instance().m_thresholdsVAC.fTC3Min) *100 ));
	}
	else if( wParam == byte_opcTC4 )
	{
		// 更新TCc进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCc)))->SetPos( (int)((CVACDataManager::Instance().m_valueTC.fTC4 -CVACDataManager::Instance().m_thresholdsVAC.fTC4Min) / (CVACDataManager::Instance().m_thresholdsVAC.fTC4Max -CVACDataManager::Instance().m_thresholdsVAC.fTC4Min) *100 ));
	}
}


LRESULT CDlgImpPanelAll2::OnBoardVACUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	CString str;
	if( wParam < 5 )
	{
		CString strDevice, strStatus;
		switch( wParam )
		{
		case	4:
			strDevice = GetResString(IDS_SD_PROMPT_VAC_41);
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
		str.Format( "%s %s", strDevice, strStatus );
		theApp.DisplayMsg( str );
		if( wParam == 4 && lParam == 1 )
		{
			VAC_VacuumReady2( (BOOL)lParam );
			StatusVac6K status1	= CVACDataManager::Instance().m_statusBoard.Vac.status;
			StatusValve6K status2 = CVACDataManager::Instance().m_statusBoard.Valve.status;
			if( status1.X == 1 )	// 样品室大气状态
			{
				m_btnChamberVent.SetColor( RGB(192,192,0) );
				m_btnChamberVent.SetCheck(1);
			}
			else
			{
				m_btnChamberVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
				m_btnChamberVent.SetCheck(0);
			}
			if( status1.X == 2 )	// 电子枪大气状态
			{
				m_btnGunVent.SetColor( RGB(192,192,0) );
				m_btnGunVent.SetCheck(1);
			}
			else
			{
				m_btnGunVent.SetColor( ::GetSysColor(COLOR_3DFACE) );
				m_btnGunVent.SetCheck(0);
			}
			if( status1.YM )	// 枪真空错误
			{
				// 如果V1已开启，要将V1关闭
				if( status2.V1Rec )
				{
					CVACDataManager::Instance().m_BoardVAC.SetParam(105, 2);
					theApp.DisplayMsg("Vacuum is error, close V1 now");
				}
				if( m_bSPRRelay1En )
				{
					theApp.DisplayMsg("Gun Vacuum Error! Shut down SPR En");
					SPR_En(FALSE);
				}
				m_btnV1.SetCheck(0);
				m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
				m_btnV1.EnableWindow( FALSE );
			}
			else
				m_btnV1.EnableWindow( TRUE );
		}
	}
	else if( wParam < 150 )
	{
		if( wParam == byte_opcStatus1 )
		{
			// 返回真空状态
			CVACDataManager::Instance().m_statusBoardBak = CVACDataManager::Instance().m_statusBoard;
			CVACDataManager::Instance().m_statusBoard.Vac = *((StatusParam16K*)lParam);
			VAC_UpdateUIswitches();
		}
		else if( wParam == byte_opcStatus2 )
		{
			// 返回阀门状态
			CVACDataManager::Instance().m_statusBoardBak = CVACDataManager::Instance().m_statusBoard;
			CVACDataManager::Instance().m_statusBoard.Valve = *((StatusParam26K*)lParam);
			VAC_UpdateUIswitches();
		}
		else if( wParam == byte_opcCCG )
		{
			if( !(CVACDataManager::Instance().m_statusBoard.Valve.status.CCG) )
				return 0;
			CVACDataManager::Instance().m_valueCCG = *((VACvalue6K_CCG*)lParam);
			UpdateStatus_BoardCCG();
		}
		else if( wParam > byte_opcCCG && wParam < byte_opcWarning )
		{
			UpdateStatus_BoardTCs( wParam, lParam );
		}
		else if( wParam == byte_opcWarning )
		{
			// 分析警告
			BYTE bType = (BYTE)lParam;
			switch( bType )
			{
			case	0x00:
				{
					str = "TMP运行报警";
					CDlgPrompt dlg;
					dlg.m_byteType = 0x01;
					dlg.m_strMsg = "\r\n";
					dlg.m_strMsg += "\r\n";
					dlg.m_strMsg += str;
					dlg.m_strMsg += "\r\n";
					dlg.m_strMsg += "\r\n";
					dlg.DoModal();
				}
				break;
			case	0x01:
				{
					if( CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec )
					{
						str = "V1开启时，样品室真空错误报警";
						theApp.DisplayMsg( str );
						SPR_En( FALSE );
						m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
						CVACDataManager::Instance().m_BoardVAC.SetParam(105, 2);
						theApp.DisplayMsg("Chamb Vacuum ERROR: close V1");
						str = GetResString(IDS_SD_PROMPT_INT_03);
						CDlgPrompt dlg;
						dlg.m_byteType = 0x01;
						dlg.m_strMsg = "\r\n";
						dlg.m_strMsg += "\r\n";
						dlg.m_strMsg += str;
						dlg.m_strMsg += "\r\n";
						dlg.m_strMsg += "\r\n";
						dlg.DoModal();
					}
					else
						str = "V1未开启时，样品室真空错误报警";
				}
				break;
			default:
				str = "收到未知报警";
				break;
			}
			theApp.DisplayMsg( str );
		}
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon->m_hWnd )
				pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_BoardVAC_UpdateStatus, wParam, lParam);
		}
		else
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			{
				if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon3->m_hWnd )
					pfrm->m_dlgDebug.m_pPageMon3->PostMessage(WM_USER_BoardVAC_UpdateStatus, wParam, lParam);
			}
			else
			{
				if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon2->m_hWnd )
					pfrm->m_dlgDebug.m_pPageMon2->PostMessage(WM_USER_BoardVAC_UpdateStatus, wParam, lParam);
			}
		}
	}
	else
	{
		int nSend = (int)lParam;
		if( wParam == 150 +byte_opcVentChamb )
			str.Format( "样品室放气：发送 %d 个字节", nSend );
		else if( wParam == 150 +byte_opcVentGun )
			str.Format( "电子枪放气：发送 %d 个字节", nSend );
		else if( wParam == 150 +byte_opcPumpChamb )
			str.Format( "样品室抽真空：发送 %d 个字节", nSend );
		else if( wParam == 150 +byte_opcPumpGun )
			str.Format( "电子枪抽真空：发送 %d 个字节", nSend );
		Message( str );
	}
	return 0;
}


LRESULT CDlgImpPanelAll2::OnUpdatePLCvacBrdStatus(WPARAM wParam, LPARAM lParam)
{
	CString str;
	if( wParam < byte_PLCvacBoard_Offset )
	{
		switch( wParam )
		{
		case	0:
			{
				switch( lParam )
				{
				case	1:
				case	2:
					theApp.DisplayMsg("PLCvacBrd: socket connect succeeded!");
					break;
				case	11:
					theApp.DisplayMsg("PLCvacBrd: socket function failed with error!");
					break;
				case	12:
					theApp.DisplayMsg("PLCvacBrd: socket ioctlsocket failed!");
					break;
				case	13:
					theApp.DisplayMsg("PLCvacBrd: socket connect failed with error!");
					break;
				case	14:
					theApp.DisplayMsg("PLCvacBrd: socket connect timeout!");
					break;
				case	15:
					theApp.DisplayMsg("PLCvacBrd: socket connect failed with error!");
					break;
				case	16:
					theApp.DisplayMsg("PLCvacBrd: socket CreateThread failed!");
					break;
				}
			}
			break;
		case	1:
			{
				BOOL bReadyBak = CVACDataManager::Instance().m_cfgPLCvacBrd.bReady;
				CVACDataManager::Instance().m_cfgPLCvacBrd.bReady = (BOOL)lParam;
				if( bReadyBak != CVACDataManager::Instance().m_cfgPLCvacBrd.bReady )
				{
					if( CVACDataManager::Instance().m_cfgPLCvacBrd.bReady )
					{
						VAC_VacuumReady2( TRUE );
						theApp.DisplayMsg("PLCvacBrd: Device is ready!");
					}
					else
					{
						VAC_VacuumReady2( FALSE );
						ZeroMemory( &(CVACDataManager::Instance().m_cfgPLCvacBrd), sizeof(Cfg_PLCvacBrd) );
						ZeroMemory( &(CVACDataManager::Instance().m_StatusPLCvacBrd), sizeof(Status_PLCvacBrd) );
						theApp.DisplayMsg("PLCvacBrd: Device is NOT ready!");
					}
				}
			}
			break;
		}
	}
	else
	{
		if( !(CVACDataManager::Instance().m_cfgPLCvacBrd.bReady) )
			return 0;
		wParam -= byte_PLCvacBoard_Offset;
		switch( wParam )
		{
/*		case	byte_PLCvacBRDopc_InquireVACstatus:
			m_StatusPLCvacBrd.statusVac = *((Param_PLCvacBrd_vac*)lParam);
			PLC_UpdateStatusVac();
			break;
			
		case	byte_PLCvacBRDopc_InquireVACvalve:
			m_StatusPLCvacBrd.statusValve = *((Param_PLCvacBrd_valve*)lParam);
			PLC_UpdateStatusValve();
			if( m_StatusPLCvacBrd.statusValve.valve.mode )
				SetModeEnable( TRUE );
			else
				SetModeEnable( FALSE );
			break;
*/
		case	byte_PLCvacBRDopc_InquireVACthreshold:
			{
				CVACDataManager::Instance().m_cfgPLCvacBrd = *((Cfg_PLCvacBrd*)lParam);
//				PLC_UpdateThresholds();
				CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
				if( pfrm && pfrm->m_dlgDebugVIP.m_hWnd )
				{
					char szBuf[50] = {0};
					sprintf( szBuf, "%.1e", CVACDataManager::Instance().m_cfgPLCvacBrd.dThreshold_PreVac );
					pfrm->m_dlgDebugVIP.GetDlgItem( IDC_EDIT_PLCvacBrd_Thresholds_VacPre )->SetWindowText(szBuf);
					sprintf( szBuf, "%.1e", CVACDataManager::Instance().m_cfgPLCvacBrd.dThreshold_VacOK );
					pfrm->m_dlgDebugVIP.GetDlgItem( IDC_EDIT_PLCvacBrd_Thresholds_VacOK )->SetWindowText(szBuf);
					sprintf( szBuf, "%.1e", CVACDataManager::Instance().m_cfgPLCvacBrd.dThreshold_VacNotOK );
					pfrm->m_dlgDebugVIP.GetDlgItem( IDC_EDIT_PLCvacBrd_Thresholds_VacNotOK )->SetWindowText(szBuf);
					sprintf( szBuf, "PLC: Get Thresholds: %.1e, %.1e, %.1e", CVACDataManager::Instance().m_cfgPLCvacBrd.dThreshold_PreVac,
							CVACDataManager::Instance().m_cfgPLCvacBrd.dThreshold_VacOK, CVACDataManager::Instance().m_cfgPLCvacBrd.dThreshold_VacNotOK );
					theApp.DisplayMsg(szBuf);
				}
			}
			break;

		case	byte_PLCvacBRDopc_InquireStatus:
			{
				CVACDataManager::Instance().m_StatusPLCvacBrdBak = *((Status_PLCvacBrd*)lParam);
				VAC_UpdateUIswitches();
				BYTE bUpdate = PLC_UpdateStatus();
				{
					CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
					switch( CHVPowerDataManager::Instance().m_dwHVtype )
					{
					case	0:
// 						if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon->m_hWnd )
// 							pfrm->m_dlgDebug.m_pPageMon->PLC_UpdateStatus();
						break;
					case	1:
						if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon2->m_hWnd )
							pfrm->m_dlgDebug.m_pPageMon2->PLC_UpdateStatus( bUpdate );
						break;
					case	2:
					case	3:
						if( pfrm && pfrm->m_dlgDebug.m_hWnd && pfrm->m_dlgDebug.m_pPageMon3->m_hWnd )
							pfrm->m_dlgDebug.m_pPageMon3->PLC_UpdateStatus( bUpdate );
						break;
					}
					if( pfrm && pfrm->m_dlgDebugVIP.m_hWnd )
						pfrm->m_dlgDebugVIP.PLC_UpdateStatusValve();
				}
			}
			break;
		}
	}
	return 0;
}

BYTE CDlgImpPanelAll2::PLC_UpdateStatus()
{
	BYTE bUpdate = 0x00;
	if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.wStatus != CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.wStatus )
	{
		CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.wStatus = CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusVac.wStatus;
//		PLC_UpdateStatusVac();
		bUpdate |= 0x01;
	}
	if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.wValve != CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.wValve )
	{
		if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V1_SEM != CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM )
			NAV_SetV1Status(CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.V1_SEM);
		CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.wValve = CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.wValve;
//		PLC_UpdateStatusValve();
		bUpdate |= 0x10;

		// 什么时候可以加高压
		if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM
//			&& (CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.XX == 7)			// TMP运行VacOK状态
			&& CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.VacChambOK )		// 样品室真空正常状态
		{
			if( !m_bSPRRelay1En )
				SPR_En( TRUE );
		}
		else
		{
			if( m_bSPRRelay1En )
				SPR_En( FALSE );
		}
	}

	char szBuf[50] = {0};
	if( CVACDataManager::Instance().m_StatusPLCvacBrdBak.nType != CVACDataManager::Instance().m_StatusPLCvacBrd.nType )
	{
		CVACDataManager::Instance().m_StatusPLCvacBrd.nType = CVACDataManager::Instance().m_StatusPLCvacBrdBak.nType;
		if( CVACDataManager::Instance().m_StatusPLCvacBrd.nType > 1 )
			sprintf( szBuf, "PLC: Type = %d, FeSEM", CVACDataManager::Instance().m_StatusPLCvacBrd.nType );
		else
			sprintf( szBuf, "PLC: Type = %d, SEM", CVACDataManager::Instance().m_StatusPLCvacBrd.nType );
		theApp.DisplayMsg(szBuf);
	}

	CVACDataManager::Instance().m_StatusPLCvacBrd.dPccg = CVACDataManager::Instance().m_StatusPLCvacBrdBak.dPccg;
	PLC_UpdateCCGValue();
	return bUpdate;
}

void CDlgImpPanelAll2::PLC_UpdateCCGValue()
{
	CString str;
	str.Format( "%.2e", CVACDataManager::Instance().m_StatusPLCvacBrd.dPccg );
	if( str.Left(3) == "0.0" )
		return;
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V2 )
		GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( str );
	else
		GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
	double dVacNew = CVACDataManager::Instance().m_StatusPLCvacBrd.dPccg;
	double dVacOld = CVACDataManager::Instance().m_StatusPLCvacBrdBak.dPccg;

	int nState = m_nHIstate;
	if( dVacNew > CVACDataManager::Instance().m_cfgPLCvacBrd.dThreshold_VacNotOK )
		m_nHIstate = 0;
	else if( dVacNew < CVACDataManager::Instance().m_cfgPLCvacBrd.dThreshold_VacOK )
		m_nHIstate = 2;
	else
		m_nHIstate = 1;
	if( nState != m_nHIstate )
	{
		Invalidate( FALSE );
//		VAC_VacuumReady2( TRUE );
		m_btnV1.EnableWindow( CVACDataManager::Instance().m_StatusPLCvacBrdBak.statusValve.valve.mode && (m_nHIstate == 2) );
	}
}



void CDlgImpPanelAll2::SPR_HVOn( BOOL bHVOn )
{
	if( m_bHVOn == bHVOn )
		return;
	m_bHVOn = bHVOn;

	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	22:
	case	21:
		if( m_bHVOn )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_CCD, byte_PLCvacBRDopr_Open );
			theApp.DisplayMsg( "PLC: CCD Power Enable, CCD work" );
		}
		else
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_CCD, byte_PLCvacBRDopr_Close );
			theApp.DisplayMsg( "PLC: CCD Power Disable, CCD NOT work" );
		}
		break;
	case	0:
		{
			int nWsize, nModID;
			BYTE bCktAddr;
			WORD wData;
			// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
			if( m_bHVOn )
			{
				theApp.DisplayMsg( "SPR_HVOn: J12 HV Enable, CCD work" );
				// J12: HV Enable.
				// 现用作CCD所连继电器的开关的控制，J12开时CCD可工作
				wData		= 0x89;		// Strobe IOX output HVonA high
				nWsize		= 8;
				nModID		= USB_MID_SPR;
				bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
				CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
// 				for(int i=0; i<1000000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
// 					double x = sqrt(4.);
// 				wData		= 0x8C;		// Strobe IOX output HVonB high
// 				CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
			}
			else
			{
				theApp.DisplayMsg( "SPR_HVOn: J12 HV Disable, CCD NOT work" );
				// J12: HV Disable
				// 现用作CCD所连继电器的开关的控制，J12关时CCD不工作
				wData		= 0x49;		// Strobe IOX output HVonA low
				nWsize		= 8;
				nModID		= USB_MID_SPR;
				bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
				CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
// 				for(int i=0; i<1000000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
// 					double x = sqrt(4.);
// 				wData		= 0x4C;		// Strobe IOX output HVonB low
// 				CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
			}
		}
		break;
	default:
		{
			if( m_bHVOn )
			{
				CVACDataManager::Instance().m_BoardVAC.SetParam( 100 +byte_opcCCD, 0x01 );
				theApp.DisplayMsg( "CCD Power Enable, CCD work" );
			}
			else
			{
				CVACDataManager::Instance().m_BoardVAC.SetParam( 100 +byte_opcCCD, 0x02 );
				theApp.DisplayMsg( "CCD Power Disable, CCD NOT work" );
			}
		}
	}
}

void CDlgImpPanelAll2::SPR_Fan( BOOL bFan )
{
	if( m_bNavPowerOn == bFan )
		return;
	m_bNavPowerOn = bFan;

	/////////////////////////////////////////////
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		if( m_bNavPowerOn )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_NAV, byte_PLCvacBRDopr_Open );
			theApp.DisplayMsg( "NAV Power Enable, NAV work" );
		}
		else
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_NAV, byte_PLCvacBRDopr_Close );
			theApp.DisplayMsg( "NAV Power Disable, NAV NOT work" );
		}
	}
	else
	{
		int nWsize, nModID;
		BYTE bCktAddr;
		WORD wData;
		// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
		if( m_bNavPowerOn )
		{
			theApp.DisplayMsg( "SPR_Fan: J18 HV Enable, NAV work" );
			// J18: HV Enable.
			// 现用作导航灯板所连继电器的开关的控制，J18开时导航灯板可工作
			wData		= 0x87;		// Strobe IOX output HVonA high
			nWsize		= 8;
			nModID		= USB_MID_SPR;
			bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
			CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		}
		else
		{
			theApp.DisplayMsg( "SPR_Fan: J18 HV Disable, NAV NOT work" );
			// J18: HV Disable
			// 现用作导航灯板所连继电器的开关的控制，J18关时导航灯板不工作
			wData		= 0x47;		// Strobe IOX output HVonA low
			nWsize		= 8;
			nModID		= USB_MID_SPR;
			bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
			CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		}
	}
}

void CDlgImpPanelAll2::SPR_En( BOOL bEn )
{
	char sz[512] = {0};
	sprintf_s(sz,512,"SPR_En( BOOL bEn = %d ),m_bSPRRelay1En = %d  \r\n",bEn, m_bSPRRelay1En );
	OutputDebugString(sz);

	if( m_bSPRRelay1En == bEn )
		return;

	CString str;
	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
	str.Format( "MP=%d, V1=%d, TCf=%d, TCc=%d, Vf=%d, Vc=%d, TMP=%d",
		CVACDataManager::Instance().m_VacParamMain.bMP,CVACDataManager::Instance().m_VacParamMain.bV1,
		CVACDataManager::Instance().m_VacParamMain.bTCf,CVACDataManager::Instance().m_VacParamMain.bTCc,
		CVACDataManager::Instance().m_VacParamMain.bVf,CVACDataManager::Instance().m_VacParamMain.bVc,CVACDataManager::Instance().m_VacParamMain.bTMP);
	else
		str.Format( "V1=%d, Vf=%d, Vc=%d, TMP=%d",
			CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec,
			CVACDataManager::Instance().m_statusBoard.Valve.status.V2, CVACDataManager::Instance().m_statusBoard.Valve.status.V3, CVACDataManager::Instance().m_statusBoard.Valve.status.TMP);
	theApp.DisplayMsg( str );

	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bSPRRelay1En = bEn;
	theApp.m_blUnlock_HV = bEn;
	if( m_bSPRRelay1En )
	{
		theApp.DisplayMsg( "SPR_En: J14 vacuum interlock ON, cst & HV to init 0");
		// J14: Vacuum Interlock ON
		wData		= 0x82;		// Strobe IOX output @0A high
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

		// J14开时，允许加对比度，且设置初始值为0
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			ClassID.lVal		= SemControlPanelClass_Contrast;
			SerialNumber.lVal	= -1;
			Enabled.lVal		= 1;
			Flag.lVal			= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
			ClassID.lVal		= SemControlPanelClass_CPS_HV;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

			SerialNumber.lVal	= -1;
			// Contrast to 0
			ClassID.lVal		= SemControlPanelClass_Contrast;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			Sleep(100);
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			ClassID.lVal		= SemControlPanelClass_HV;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );

			ZeroMemory(sz,0);
			sprintf_s(sz,512,"SPR_En( BOOL bEn = %d )-----HVtype = %d,  HVPower_Init(); \r\n HVPower_UpdateStatus();\r\n",
				bEn, CHVPowerDataManager::Instance().m_dwHVtype);
			OutputDebugString(sz);

			Sleep(1000);
			HVPower_Init();
			HVPower_UpdateStatus();
		}
		else
		{
			ZeroMemory(sz,0);
			sprintf_s(sz,512,"SPR_En( BOOL bEn = %d )-----HVtype = %d,  HVPower_Init(); \r\n HVPower_UpdateStatus();\r\n",
				bEn, CHVPowerDataManager::Instance().m_dwHVtype);
			OutputDebugString(sz);

//			Sleep(1000);
			HVPower_Init();
			for(int i=0; i<2000000; i++)		// 1000000次开平方运算16ms，10000次开平方运算16us
				double x = sqrt(4.);			// 延迟2秒后再重新建立连接
			HVPower_UpdateStatus();
		}
		CScanView* pView = (CScanView*)theApp.GetScanView();
		if( pView != NULL && CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
			CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
		CHVPowerDataManager::Instance().m_dHV = CGlobalParamSetting::Instance().g_dHV = CGlobalParamSetting::Instance().g_dHV2 = CGlobalParamSetting::Instance().g_dHV3 = 0.0;
	}
	else
	{
		theApp.DisplayMsg( "SPR_En: J14 vacuum interlock OFF, cst & HV to 0 and disable");
		// J14: Vacuum Interlock OFF
		// J14关时，不允许加对比度，且恢复对比度值为0
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			SerialNumber.lVal	= -1;
			// Contrast to 0
			ClassID.lVal		= SemControlPanelClass_Contrast;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			Sleep(100);
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			ClassID.lVal		= SemControlPanelClass_CPS_HV;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );

			ClassID.lVal		= SemControlPanelClass_Contrast;
			SerialNumber.lVal	= -1;
			Enabled.lVal		= 0;
			Flag.lVal			= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
			ClassID.lVal		= SemControlPanelClass_CPS_HV;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		}
		else
		{
			SerialNumber.lVal	= -1;
			// Contrast to 0
			ClassID.lVal		= SemControlPanelClass_SP_PMT;
			Flag.lVal			= 1;
			Xpos.lVal			= 0;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
			ClassID.lVal		= SemControlPanelClass_SP_HV;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );

			ClassID.lVal		= SemControlPanelClass_SP_PMT;
			SerialNumber.lVal	= -1;
			Enabled.lVal		= 0;
			Flag.lVal			= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
			ClassID.lVal		= SemControlPanelClass_SP_HV;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			{
				theApp.m_WSM30KV2.SetParam( 4, Xpos.lVal );		// PMT
				theApp.m_WSM30KV2.SetParam( 6, Xpos.lVal );		// COL
				theApp.m_WSM30KV2.SetParam( 5, Xpos.lVal );		// SCI
			}
			else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			{
				theApp.m_WSM30KV.SetParam( 4, Xpos.lVal );		// PMT
				theApp.m_WSM30KV.SetParam( 6, Xpos.lVal );		// COL
				theApp.m_WSM30KV.SetParam( 5, Xpos.lVal );		// SCI
			}
			else
			{
				CSpellmanStatusDetector::Instance().SetParam( 35, Xpos.lVal );	// PMT
				CSpellmanStatusDetector::Instance().SetParam( 31, Xpos.lVal );	// Collector
			}
		}
		CScanView* pView = (CScanView*)theApp.GetScanView();
		if( pView != NULL && CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
			CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( 0.0 );
		CHVPowerDataManager::Instance().m_dHV = CGlobalParamSetting::Instance().g_dHV = CGlobalParamSetting::Instance().g_dHV2 = CGlobalParamSetting::Instance().g_dHV3 = 0.0;
		CGlobalParamSetting::Instance().ResetCPS3603Param();
		CGlobalParamSetting::Instance().ResetSpellmanDebugParam();

		wData		= 0x42;		// Strobe IOX output @0A low
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

		// 16.07 V1阀控制方式
		if( CVACDataManager::Instance().m_dwV1CtrlMode> 0 )
		{
			m_btnV1.SetCheck(0);
			m_btnV1.SetColor( ::GetSysColor(COLOR_3DFACE) );
			switch( CSemCtrl::Instance().m_nVACBoardType )
			{
			case	22:
			case	21:
				m_btnV1.EnableWindow( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.mode && (m_nHIstate == 2) );
				break;
			case	0:
				m_btnV1.EnableWindow( CVACDataManager::Instance().m_VacTCAmp.nState == 2 );
				CSMvac::Instance().SetParams(24, 0);
				break;
			default:
				m_btnV1.EnableWindow( m_nHIstate == 2 );
				break;
			}
			theApp.DisplayMsg("Close V1 in SPR_En");
		}

		{
			HVPower_SetEnable( 7, FALSE );
			HVPower_SetEnable( 6, FALSE );
			HVPower_SetEnable( 5, FALSE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, FALSE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, FALSE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, FALSE );
			if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
			{
				CCPS3603tatusDetector::Instance().Release();
			}
			else
			{
				if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
					theApp.m_WSM30KV2.SetParam( 18, 0 );// ALL DISABLE
				else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					theApp.m_WSM30KV.SetParam( 17, 0 );	// ALL DISABLE
				else
					CSpellmanStatusDetector::Instance().SetParam( 61, 0 );
//				Sleep(2000);
//				CSpellmanStatusDetector::Instance().Close();
			}
			
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
void CDlgImpPanelAll2::OnCheckHV() 
{
	int nCheck = m_btnHV.GetCheck();
	HVPower_SetEnable( 5, nCheck );
}

void CDlgImpPanelAll2::OnCheckBias() 
{
	int nCheck = m_btnBias.GetCheck();
	HVPower_SetEnable( 6, nCheck );
}

void CDlgImpPanelAll2::OnCheckFilament() 
{
	int nCheck = m_btnFilament.GetCheck();
	HVPower_SetEnable( 7, nCheck );
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CDlgImpPanelAll2::HVPower_Init( BOOL bForce )
{
	char sz[512] = {0};
	sprintf_s(sz,512,"HVPower_Init: bForce = %d\r\n",bForce );
	OutputDebugString(sz);

	ZeroMemory(sz,0);
	sprintf_s(sz,512,"HVPower_Init: m_dwHVtype = %d, m_Spellmans.status.bReady = %d\r\n",
		CHVPowerDataManager::Instance().m_dwHVtype, CHVPowerDataManager::Instance().m_Spellmans.status.bReady );
	OutputDebugString(sz);

	// 如果强制初始化，已连接的设备要先断开连接并关闭设备，再重新连接并初始化
	if( !bForce )
	{
		// 如果不是强制初始化，就看设备是否已经连通
		// 若已连通，则直接返回
		if( CHVPowerDataManager::Instance().m_dwHVtype == 1 && CHVPowerDataManager::Instance().m_Spellmans.status.bReady )
		{
			ZeroMemory(sz,0);
			sprintf_s(sz,512,"HVPower_Init: RETURN! m_dwHVtype = %d, m_Spellmans.status.bReady = %d, bForce = %d\r\n",
				CHVPowerDataManager::Instance().m_dwHVtype, CHVPowerDataManager::Instance().m_Spellmans.status.bReady, bForce);
			OutputDebugString(sz);
			return;
		}
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 && theApp.m_cfgWSM30KV.bReady )
		{
			ZeroMemory(sz,0);
			sprintf_s(sz,512,"HVPower_Init: RETURN! m_dwHVtype = %d, m_cfgWSM30KV.bReady = %d, bForce = %d\r\n",
				CHVPowerDataManager::Instance().m_dwHVtype, theApp.m_cfgWSM30KV.bReady, bForce);
			OutputDebugString(sz);
			return;
		}
	}

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

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		CCPS3603tatusDetector::Instance().Release();
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			theApp.m_WSM30KV2.Close();
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			theApp.m_WSM30KV.Close();
		else
			CSpellmanStatusDetector::Instance().Release();
	}

	// 2023.03 临时
	if( CHVPowerDataManager::Instance().m_dwHVtype < 2 )
		CHVPowerDataManager::Instance().HVPower_Init_ByHVType(this);
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		{
			theApp.m_cfgWSM30KV.bReady = FALSE;
			ZeroMemory( &(theApp.m_statusWSM30KV2), sizeof(Status_WSM30KV2) );
			ZeroMemory( &(theApp.m_monWSM30KV2), sizeof(Mon_WSM30KV2) );
			ZeroMemory( &(theApp.m_setWSM30KV2), sizeof(Setting_WSM30KV2) );

			// WSM新电源的Bias，在参数达标后，自动变为当前值+Offset（默认为40V）
			char path[256];
			::GetModuleFileName(0, path, 255);
			char *exd = strrchr(path, (int)'\\');
			if(exd != NULL)
				*exd = '\0';
			strcat(path, "\\KYKYcfg.ini");
			theApp.m_setWSM30KV2.fBIASv = (float)(GetPrivateProfileInt("Auto", "WSMbiasOffset", 40, path));

			theApp.m_WSM30KV2.InitHWND( this->m_hWnd );
			theApp.m_WSM30KV2.SetParam(0, theApp.m_cfgWSM30KV.nPort );
			theApp.m_WSM30KV2.Init();
		}
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			theApp.m_cfgWSM30KV.bReady = FALSE;
			ZeroMemory( &(theApp.m_statusWSM30KV), sizeof(Status_WSM30KV) );
			ZeroMemory( &(theApp.m_monWSM30KV), sizeof(Mon_WSM30KV) );
			ZeroMemory( &(theApp.m_setWSM30KV), sizeof(Setting_WSM30KV) );
			theApp.m_WSM30KV.InitHWND( this->m_hWnd );
			theApp.m_WSM30KV.SetParam(0, theApp.m_cfgWSM30KV.nPort );
			theApp.m_WSM30KV.Init();
		}
	}
}

void CDlgImpPanelAll2::HVPower_Reset()
{
	char sz[512] = {0};
	sprintf_s(sz,512,"HVPower_Reset\r\n");
	OutputDebugString(sz);

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

void CDlgImpPanelAll2::HVPower_UpdateStatus()
{
	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"HVPower_UpdateStatus: m_dwHVtype = %d, m_VacParamMain.bHVInt = %d, m_bSPRRelay1En = %d, m_blOverhaulState = %d \r\n",
		CHVPowerDataManager::Instance().m_dwHVtype, CVACDataManager::Instance().m_VacParamMain.bHVInt, m_bSPRRelay1En, theApp.m_blOverhaulState);
	OutputDebugString(sz);

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
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
	else
	{
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"HVPower_UpdateStatus: m_dwHVtype = %d, m_VacParamMain.bHVInt = %d,  m_bSPRRelay1En = %d, m_Spellmans.status.bReady = %d\r\n",
			CHVPowerDataManager::Instance().m_dwHVtype, CVACDataManager::Instance().m_VacParamMain.bHVInt, m_bSPRRelay1En, CHVPowerDataManager::Instance().m_Spellmans.status.bReady);
		OutputDebugString(sz);

		BOOL bCheck;
		if( theApp.m_blOverhaulState )
		{
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, TRUE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, TRUE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, TRUE );
		}
		else
		{
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				bCheck = CVACDataManager::Instance().m_VacParamMain.bHVInt && m_bSPRRelay1En && theApp.m_cfgWSM30KV.bReady;
			else
				bCheck = CVACDataManager::Instance().m_VacParamMain.bHVInt && m_bSPRRelay1En && CHVPowerDataManager::Instance().m_Spellmans.status.bReady;
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, bCheck );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, bCheck );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, bCheck );
		}
		if( ( (CSemCtrl::Instance().m_nVACBoardType < 1 && CVACDataManager::Instance().m_VacParamMain.bV1)
			|| (CSemCtrl::Instance().m_nVACBoardType > 20 && CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM)
			|| (CSemCtrl::Instance().m_nVACBoardType > 0 && CVACDataManager::Instance().m_statusBoard.Valve.status.V1) )
			&& m_bSPRRelay1En && ((CHVPowerDataManager::Instance().m_dwHVtype > 1 && theApp.m_cfgWSM30KV.bReady) ||
			(CHVPowerDataManager::Instance().m_dwHVtype < 2 && CHVPowerDataManager::Instance().m_Spellmans.status.bReady) ) )
		{
			m_btnVacuumSwitch.EnableWindow( TRUE );
			VAC_VacuumReady( TRUE );
		}
	}
}

LRESULT CDlgImpPanelAll2::OnCPS3603statusChanged(WPARAM wParam, LPARAM lParam)
{
	if( wParam == 0x01 )
	{
		if( !theApp.m_blOverhaulState )
		{
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_HV, FALSE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Bias, FALSE );
			HVPower_SyncEnable( IDC_CHECK_IMP_PANEL_Filament, FALSE );
		}
	}
	else if( wParam == 0x02 )
	{
		CHVPowerDataManager::Instance().m_CPS3603s = *((CPS3603_Status*)lParam);
		char sz[512] = {0};
		sprintf_s(sz,512,"OnCPS3603statusChanged, wParam == 0x02 \r\n");
		OutputDebugString(sz);

		HVPower_UpdateStatus();
	}

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_CPS_Status, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelAll2::OnCPS3603msgChanged(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_CPS_MSG, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelAll2::OnCPS3603paramChanged(WPARAM wParam, LPARAM lParam)
{
	// 通讯一次，读一次各路回读值。将所有回读值放在一个数据结构体中上传给主程序
	CHVPowerDataManager::Instance().m_CPS3603 = *((CPS3603_Param*)wParam);
	dlgprintf(IDC_STATIC_STATUS_EMI, "%5.3lf", CHVPowerDataManager::Instance().m_CPS3603.dIoutA * 1e6);//CHVPowerDataManager::Instance().m_CPS3603s.dIoutF);
	dlgprintf(IDC_STATIC_STATUS_HV, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dVoutA);
	dlgprintf(IDC_STATIC_STATUS_Pressure, "%7.3lf", CHVPowerDataManager::Instance().m_CPS3603.dVoutE);
	dlgprintf(IDC_STATIC_STATUS_Pressure_Gun, "%5.3lf", CHVPowerDataManager::Instance().m_CPS3603.dIoutF);
	CGlobalParamSetting::Instance().g_CPS3603 = CHVPowerDataManager::Instance().m_CPS3603;

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon->PostMessage(WM_USER_CPS_Param, wParam, lParam);

	// 若灯丝电流设置值高于监测值过多，就暂时不允许手动加灯丝电流
	VARIANT Enable, Flag;
	Flag.lVal	= 3;
	if( m_btnFilament.GetCheck() < 1 )
		Enable.lVal	= 0;
	else if( CHVPowerDataManager::Instance().m_CPS3603.dIoutF > 1.9
		&& (CGlobalParamSetting::Instance().g_dHV3 -CHVPowerDataManager::Instance().m_CPS3603.dIoutF) / CHVPowerDataManager::Instance().m_CPS3603.dIoutF > 0.04 )
		Enable.lVal	= 0;
	else
		Enable.lVal	= 1;
	m_ctrlHeater.Enable(Enable, Flag);

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
		OnCHECKV1();
	}
	return 0;
}

// Printf to a dialog item
void CDlgImpPanelAll2::dlgprintf( int id, char *fmt, ... )
{
	va_list argptr;
	static char str[256];
	
	va_start(argptr, fmt);
	vsprintf(str, fmt, argptr);
	SetDlgItemText(id, str);
	va_end(argptr);
}

LRESULT CDlgImpPanelAll2::OnCPS3603AutoParamChanged(WPARAM wParam, LPARAM lParam)
{
	CPS3603_AutoState AutoState	= (CPS3603_AutoState)wParam;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	SerialNumber.lVal	= -1;
	Ypos.lVal = -1;
	Flag.ulVal	= (DWORD)0x01;
	if( AutoState < AS_Rise_Bias )
	{
		// 同步高压控件位置及显示值
		ClassID.lVal		= SemControlPanelClass_CPS_HV;
		Flag.lVal			= 1;
		if( lParam < 10000 )
			Xpos.lVal	= (LONG)(lParam /100);
		else
			Xpos.lVal	= (LONG)(lParam /1000 -10 +100);
		ChangeHighVoltage_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, FALSE );
	}
	else if( AutoState < AS_Rise_Fila )
	{
		// 同步偏压控件位置及显示值
		ClassID.lVal	= SemControlPanelClass_CPS_Bias;
		Xpos.lVal		= (LONG)lParam;			// 500V = 500 steps
		ChangeBias_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, FALSE );
	}
	else if( AutoState < AS_NULL )
	{
		// 同步灯丝控件位置及显示值
		ClassID.lVal	= SemControlPanelClass_CPS_FilamentCurrent;
		Xpos.lVal		= (LONG)(lParam /10);	// 3000mA = 300 steps
		ChangeFilament_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, FALSE );
	}
	else
	{
		OneKEY_LastStep();
	}
	return 0;
}

void CDlgImpPanelAll2::HVPower_SyncEnable( int nID, BOOL bEnable )
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
			m_ctrlHV.Enable(Enable, Flag);
		}
		break;
	case	IDC_CHECK_IMP_PANEL_Bias:
		m_btnBias.EnableWindow( bEnable );
		if( !(theApp.m_blOverhaulState) )
		{
			nCheck = m_btnBias.GetCheck();
			Enable.lVal	= bEnable & nCheck;
			m_ctrlBias.Enable(Enable, Flag);
		}
		break;
	case	IDC_CHECK_IMP_PANEL_Filament:
		m_btnFilament.EnableWindow( bEnable );
		if( !(theApp.m_blOverhaulState) )
		{
			nCheck = m_btnFilament.GetCheck();
			Enable.lVal	= bEnable & nCheck;
			m_ctrlHeater.Enable(Enable, Flag);
		}
		break;
	}
}

void CDlgImpPanelAll2::HVPower_SetEnable( int nID, int nCheck )
{
	char sz[512] = {0};
	sprintf_s(sz,512,"HVPower_SetEnable: nID = %d, nCheck = %d\r\n",nID, nCheck );
	OutputDebugString(sz);

	VARIANT Xpos, Ypos, Enable, Flag, ClassID, SerialNumber;
	CString strX, strY;
	Xpos.lVal = 0;
	Enable.lVal	= nCheck;

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		// CPS3603
		switch( nID )
		{
		case	5:
			if( nCheck )
			{
				m_btnHV.SetCheck( 1 );
				m_btnHV.SetColor( RGB(192,0,0) );
			}
			else
			{
				m_btnHV.SetCheck( 0 );
				m_btnHV.SetColor( ::GetSysColor(COLOR_3DFACE) );
				Flag.ulVal	= (DWORD)0x01;
				m_ctrlHV.SyncPos( Xpos, Ypos, Flag );

				ClassID.lVal		= SemControlPanelClass_CPS_HV;
				SerialNumber.lVal	= -1;
				Flag.lVal			= 1;
				Xpos.lVal			= 0;
				strX = "0.0 KV";
				m_ctrlHV.SyncText( strX, strY, Flag );
			}
			Flag.lVal	= 3;
			m_ctrlHV.Enable(Enable, Flag);
			break;

		case	6:
			if( nCheck )
			{
				m_btnBias.SetCheck( 1 );
				m_btnBias.SetColor( RGB(0,192,0) );
			}
			else
			{
				m_btnBias.SetCheck( 0 );
				m_btnBias.SetColor( ::GetSysColor(COLOR_3DFACE) );
				Flag.ulVal	= (DWORD)0x01;
				strX = "0.0 V";
				m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
				m_ctrlBias.SyncText( strX, strY, Flag );
			}
			Flag.lVal	= 3;
			m_ctrlBias.Enable(Enable, Flag);
			break;

		case	7:
			if( nCheck )
			{
				m_btnFilament.SetCheck( 1 );
				m_btnFilament.SetColor( RGB(128,128,0) );
			}
			else
			{
				m_btnFilament.SetCheck( 0 );
				m_btnFilament.SetColor( ::GetSysColor(COLOR_3DFACE) );
				Flag.ulVal	= (DWORD)0x01;
				strX = "0.00 A";
				m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
				m_ctrlHeater.SyncText( strX, strY, Flag );
			}
			Flag.lVal	= 3;
			m_ctrlHeater.Enable(Enable, Flag);
			break;

		default:
			break;
		}
	}
	else
	{
		// Spellman EBM30N12/771 or WSM30KV
		switch( nID )
		{
		case	5:
			if( nCheck )
			{
				theApp.DisplayMsg( "Set HV Enable" );
				m_btnHV.SetCheck( 1 );
				m_btnHV.SetColor( RGB(192,0,0) );
			}
			else
			{
				theApp.DisplayMsg( "Set HV to 0 and disable" );
				m_btnHV.SetCheck( 0 );
				m_btnHV.SetColor( ::GetSysColor(COLOR_3DFACE) );
				if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
					theApp.m_WSM30KV2.SetParam( 1, 0 );
				else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					theApp.m_WSM30KV.SetParam( 1, 0 );
				Flag.ulVal	= (DWORD)0x01;
				m_ctrlHV.SyncPos( Xpos, Ypos, Flag );

				ClassID.lVal		= SemControlPanelClass_SP_HV;
				SerialNumber.lVal	= -1;
				Flag.lVal			= 1;
				Xpos.lVal			= 0;
				strX = "0.0 KV";
				m_ctrlHV.SyncText( strX, strY, Flag );
				theApp.m_setWSM30KV.fACLv = 0.0;
				theApp.m_setWSM30KV2.fACLv = 0.0;
			}
			Flag.lVal	= 3;
			m_ctrlHV.Enable(Enable, Flag);
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			{
				if( theApp.m_blOverhaulState )
					theApp.m_WSM30KV2.SetParam( 13, 0 );
				else
					theApp.m_WSM30KV2.SetParam( 13, nCheck );
			}
			else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			{
				if( theApp.m_blOverhaulState )
					theApp.m_WSM30KV.SetParam( 12, 0 );
				else
					theApp.m_WSM30KV.SetParam( 12, nCheck );
			}
			break;

		case	6:
			if( nCheck )
			{
				theApp.DisplayMsg( "Set Beam Enable" );
				m_btnBias.SetCheck( 1 );
				m_btnBias.SetColor( RGB(0,192,0) );
			}
			else
			{
				theApp.DisplayMsg( "Set Beam to 0" );
				m_btnBias.SetCheck( 0 );
				m_btnBias.SetColor( ::GetSysColor(COLOR_3DFACE) );
				if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
					theApp.m_WSM30KV2.SetParam( 2, 0 );
				else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					theApp.m_WSM30KV.SetParam( 2, 0 );
				Flag.ulVal	= (DWORD)0x01;
				strX = "0 uA";
				m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
				m_ctrlBias.SyncText( strX, strY, Flag );
			}
			Flag.lVal	= 3;
			m_ctrlBias.Enable(Enable, Flag);
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			{
				m_ctrlWSMBias.Enable(Enable, Flag);
				if( theApp.m_blOverhaulState )
				{
					theApp.m_WSM30KV2.SetParam( 15, 0 );
					theApp.m_WSM30KV2.SetParam( 11, 0 );
				}
				else
				{
					theApp.m_WSM30KV2.SetParam( 15, nCheck );
					theApp.m_WSM30KV2.SetParam( 11, nCheck );
				}
			}
			else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			{
				if( theApp.m_blOverhaulState )
					theApp.m_WSM30KV.SetParam( 13, 0 );
				else
					theApp.m_WSM30KV.SetParam( 13, nCheck );
			}
			break;

		case	7:
			if( nCheck )
			{
				theApp.DisplayMsg( "Set Fila Enable" );
				m_btnFilament.SetCheck( 1 );
				m_btnFilament.SetColor( RGB(128,128,0) );
			}
			else
			{
				theApp.DisplayMsg( "Set Fila to 0" );
				m_btnFilament.SetCheck( 0 );
				m_btnFilament.SetColor( ::GetSysColor(COLOR_3DFACE) );
				if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
					theApp.m_WSM30KV2.SetParam( 3, 0 );
				else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
					theApp.m_WSM30KV.SetParam( 3, 0 );
				Flag.ulVal	= (DWORD)0x01;
				strX = "0.00 A";
				m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
				m_ctrlHeater.SyncText( strX, strY, Flag );
			}
			Flag.lVal	= 3;
			m_ctrlHeater.Enable(Enable, Flag);
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			{
				if( theApp.m_blOverhaulState )
					theApp.m_WSM30KV2.SetParam( 12, 0 );
				else
					theApp.m_WSM30KV2.SetParam( 12, nCheck );
			}
			else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			{
				if( theApp.m_blOverhaulState )
					theApp.m_WSM30KV.SetParam( 16, 0 );
				else
					theApp.m_WSM30KV.SetParam( 16, nCheck );
			}
			break;

		default:
			break;
		}
	}

	CScanView* pView = (CScanView*)theApp.GetScanView();
	ASSERT(pView);
	// 2023.03 临时
	if( CHVPowerDataManager::Instance().m_dwHVtype < 2 )
		CHVPowerDataManager::Instance().HVPower_SetEnable_HVType(nCheck,nID,CScanParameterManager::Instance().m_nusbScanType,theApp.m_blOverhaulState);
}

LRESULT CDlgImpPanelAll2::OnSpellmanStatusChanged(WPARAM wParam, LPARAM lParam)
{
	if( wParam == 0x01 )
	{
		CHVPowerDataManager::Instance().m_Spellmans.status.bReady = (BOOL)lParam;
		char sz[512] = {0};
		sprintf_s(sz,512,"OnSpellmanStatusChanged, m_Spellmans.status.bReady = %d\r\n", (BOOL)lParam);
		OutputDebugString(sz);
		HVPower_UpdateStatus();
	}
	else if( wParam == 0x02 )
	{
		CHVPowerDataManager::Instance().m_Spellmans.status = *((Spellman_Status*)lParam);
		CGlobalParamSetting::Instance().g_Spellmans.status = *((Spellman_Status*)lParam);
		CString str;
		str.Format( "Gun supply ID is: %s", CHVPowerDataManager::Instance().m_Spellmans.status.strID );
		theApp.DisplayMsg( str );
	}
	else if( wParam == 0x03 )
	{
		CHVPowerDataManager::Instance().m_Spellmans.status = *((Spellman_Status*)lParam);
		CGlobalParamSetting::Instance().g_Spellmans.status = *((Spellman_Status*)lParam);
		CString str;
		str.Format( "Software Version ID is: %s", CHVPowerDataManager::Instance().m_Spellmans.status.strVer );
		theApp.DisplayMsg( str );
	}
	else if( wParam == 0x04 )
	{
		BOOL bSSbak = CHVPowerDataManager::Instance().m_Spellmans.status.bstatusSwitch;
		DWORD dwSRbak = CHVPowerDataManager::Instance().m_Spellmans.status.dwstatusRegister;
		CHVPowerDataManager::Instance().m_Spellmans.status = *((Spellman_Status*)lParam);
		CGlobalParamSetting::Instance().g_Spellmans.status = *((Spellman_Status*)lParam);
		CString str;
		str.Format( "On/Off Status is 0x%02X, register is 0x%05X", CHVPowerDataManager::Instance().m_Spellmans.status.bstatusSwitch, CHVPowerDataManager::Instance().m_Spellmans.status.dwstatusRegister );
		theApp.DisplayMsg( str );
 		HV_UpdateStatusSwitches( bSSbak );
// 		UpdateStatusRegister();

		if( ((dwSRbak & 0x00080) == 0x00080) && ((CHVPowerDataManager::Instance().m_Spellmans.status.dwstatusRegister & 0x00080) == 0x00000) )
		{
			// PMT Interlock已到位
			SetTimer(21, 500, NULL);
			theApp.DisplayMsg("SpellmanStatusChanged: PMT Interlock is ready, start the timer");
		}
		if( ((dwSRbak & 0x00400) == 0x00400) && ((CHVPowerDataManager::Instance().m_Spellmans.status.dwstatusRegister & 0x00400) == 0x00000) )
		{
			// Collector Interlock已到位
			SetTimer(22, 1000, NULL);
			theApp.DisplayMsg("SpellmanStatusChanged: Collector Interlock is ready, start the timer");
		}
		if( ((dwSRbak & 0x00800) == 0x00800) && ((CHVPowerDataManager::Instance().m_Spellmans.status.dwstatusRegister & 0x00800) == 0x00000) )
		{
			// Scintillator Interlock已到位
			SetTimer(23, 2000, NULL);
		}

		if( ((bSSbak & 0x10) == 0x00) && ((CHVPowerDataManager::Instance().m_Spellmans.status.bstatusSwitch & 0x10) == 0x10) )
		{
			// PMT Enable已到位
			SetTimer(24, 1000, NULL);
			theApp.DisplayMsg("SpellmanStatusChanged: PMT Enable is ready, start the timer");
		}
		if( ((bSSbak & 0x01) == 0x00) && ((CHVPowerDataManager::Instance().m_Spellmans.status.bstatusSwitch & 0x01) == 0x01) )
		{
			// Collector Enable已到位
			SetTimer(25, 1000, NULL);
			theApp.DisplayMsg("SpellmanStatusChanged: Collector Enable is ready, start the timer");
		}

		if( ((dwSRbak & 0x00008) == 0x00000) && ((CHVPowerDataManager::Instance().m_Spellmans.status.dwstatusRegister & 0x00008) == 0x00008) )
		{
			// 灯丝断了
//			CCPS3603tatusDetector::Instance().SetParam(21,AS_NULL);
			CString str;
			str.Format( "%s %s", GetResString(IDS_SD_PROMPT_FilaBroken), GetResString(IDS_SD_PROMPT_VAC_01) );
			AfxMessageBox( str );
			__int64 timestamp = GetCurrentTimeStampMS();
			CAlarmManager::Instance().AddAlarm(AlarmType::Alarm_FilaBroken,timestamp,string(str.GetBuffer()));
			OnCHECKV1();
		}
	}

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon2->PostMessage(WM_USER_SPELLMAN_Status, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelAll2::OnSpellmanParamChanged(WPARAM wParam, LPARAM lParam)
{
	CHVPowerDataManager::Instance().m_Spellmans.pM = *((Spellman_Param*)lParam);
	CGlobalParamSetting::Instance().g_Spellmans.pM = *((Spellman_Param*)lParam);
	dlgprintf(IDC_STATIC_STATUS_EMI, "%.1f", CHVPowerDataManager::Instance().m_Spellmans.pM.dII);
	dlgprintf(IDC_STATIC_STATUS_HV, "%.2f", CHVPowerDataManager::Instance().m_Spellmans.pM.dVE);
	dlgprintf(IDC_STATIC_STATUS_Pressure, "%.1f", CHVPowerDataManager::Instance().m_Spellmans.pM.dIB);
	dlgprintf(IDC_STATIC_STATUS_Pressure_Gun, "%.2f", CHVPowerDataManager::Instance().m_Spellmans.pM.dIH);

	// 若当前束流值高于束流值过多，就暂时不允许手动加灯丝电流
	VARIANT Enable, Flag;
	Flag.lVal	= 3;
	if( m_btnFilament.GetCheck() < 1 )
		Enable.lVal	= 0;
	else if( CHVPowerDataManager::Instance().m_Spellmans.pS.dIB > 0.001
			&& (CHVPowerDataManager::Instance().m_Spellmans.pM.dII -CHVPowerDataManager::Instance().m_Spellmans.pS.dIB) / CHVPowerDataManager::Instance().m_Spellmans.pS.dIB > 0.3 )
		Enable.lVal	= 0;
	else
		Enable.lVal	= 1;
	m_ctrlHeater.Enable(Enable, Flag);

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon2->PostMessage(WM_USER_SPELLMAN_Param, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelAll2::OnSpellmanParam2Changed(WPARAM wParam, LPARAM lParam)
{
	// 加速度
	CHVPowerDataManager::Instance().m_Spellmans.pM2 = *((Spellman_Param*)lParam);
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon2->PostMessage(WM_USER_SPELLMAN_Param2, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelAll2::OnSpellmanParam3Changed(WPARAM wParam, LPARAM lParam)
{
	// 监测值
	CHVPowerDataManager::Instance().m_Spellmans.pM3 = *((Spellman_Param*)lParam);
	CGlobalParamSetting::Instance().g_Spellmans.pM3 = *((Spellman_Param*)lParam);
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon2->PostMessage(WM_USER_SPELLMAN_Param3, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelAll2::OnSpellmanAutoParamChanged(WPARAM wParam, LPARAM lParam)
{
	Spellman_AutoState AutoState	= (Spellman_AutoState)wParam;
	
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	Flag.ulVal	= (DWORD)0x01;
	if( AutoState < AS2_Rise_Heater )
	{
		// 同步高压控件位置及显示值
		ClassID.lVal		= SemControlPanelClass_SP_HV;
		SerialNumber.lVal	= -1;
		double dValue		= (LONG)(lParam) /10.0;
		if( dValue < 10.0 )
			Xpos.lVal	= (LONG)(dValue *10);
		else
			Xpos.lVal	= (LONG)(dValue -10 +100);
		ChangeHighVoltage_771(ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, FALSE );
	}
	else if( AutoState < AS2_Rise_Beam )
	{
		// 同步灯丝控件位置及显示值
		ClassID.lVal = SemControlPanelClass_SP_Heater;
		SerialNumber.lVal = -1;
		Xpos.lVal = lParam /10;
		ChangeFilament_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, FALSE );
	}
	else if( AutoState < AS2_Rise_WaitEMI )
	{
		// 同步束流设置控件位置及显示值
		ClassID.lVal = SemControlPanelClass_SP_Bias;
		SerialNumber.lVal = -1;
		Xpos.lVal = (LONG)(lParam);
		ChangeBeam_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, FALSE );
	}
	else
	{
		if( AutoState == AS2_NULL)
			OneKEY_LastStep();
	}
	return 0;
}


LRESULT CDlgImpPanelAll2::OnControlPanelDataRecieved(WPARAM wParam, LPARAM lParam)
{
	BYTE* dataRecv = (BYTE*)wParam;
	BYTE data[CmdLen] = {0};
	memcpy(data,dataRecv,CmdLen);

	IPanelDataProcessor* Processor = CPanelDataProcessorMgr::Instance().GetProcessor(data[FunctionCode]);
	if (Processor)
	{
		Processor->Attach(theApp.GetMainWnd()->m_hWnd);
		Processor->SetCurrentData(data);
		Processor->Notify();
	}
	return 0L;
}

void CDlgImpPanelAll2::HV_UpdateStatusSwitches( BYTE bSSbak )
{
	BYTE bS = CHVPowerDataManager::Instance().m_Spellmans.status.bstatusSwitch;
	if( (bS & 0x04) != (bSSbak & 0x04) )
	{
		if( bS & 0x04 )
		{
			HVPower_SetEnable( 5, TRUE );
			HVPower_SetEnable( 6, TRUE );
		}
		else
		{
			HVPower_SetEnable( 5, FALSE );
			HVPower_SetEnable( 6, FALSE );
		}
	}

	if( (bS & 0x08) != (bSSbak & 0x08) )
	{
		if( bS & 0x08 )
			HVPower_SetEnable( 7, TRUE );
		else
			HVPower_SetEnable( 7, FALSE );
	}

	CHVPowerDataManager::Instance().HV_UpdateStatusSwitches(bSSbak);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CDlgImpPanelAll2::OnWSM30KVUpdateMon(WPARAM wParam, LPARAM lParam)
{
/*	if( wParam > AddrWSM30KVR01_FB_SCI_V -1 && wParam < AddrWSM30KVR01_FB_P3V3 )
	{
		UpdateMon_FB( wParam, lParam );
	}
	else if( wParam > AddrWSM30KVR01_Given_SCI_V -1 && wParam < AddrWSM30KVR01_InquireStatus1 )
	{
		UpdateMon_Given( wParam, lParam );
	}*/
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
	{
		switch( wParam )
		{
		case	AddrWSM30KV2R_FB_P3V3:
			theApp.m_monWSM30KV2.monVT = *((MonVT_WSM30KV2*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2mon.monVT = *((MonVT_WSM30KV2*)lParam);
			break;
		case	AddrWSM30KV2R_FB_BIAS_V:
			theApp.m_monWSM30KV2.monFB = *((MonFeedback_WSM30KV2*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2mon.monFB = *((MonFeedback_WSM30KV2*)lParam);
			// EMI = ACL I feedback
			dlgprintf(IDC_STATIC_STATUS_EMI, "%.1f", theApp.m_monWSM30KV2.monFB.fACLi);
			// HV = ACL V feedback
			dlgprintf(IDC_STATIC_STATUS_HV, "%.2f", theApp.m_monWSM30KV2.monFB.fACLv);
			// FIL = FIL I feedback
			dlgprintf(IDC_STATIC_STATUS_Pressure_Gun, "%.2f", theApp.m_monWSM30KV2.monFB.fFILi);
			break;
		case	AddrWSM30KV2R_Given_BIAS_V:
			theApp.m_monWSM30KV2.monOP = *((MonDACOutput_WSM30KV2*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2mon.monOP = *((MonDACOutput_WSM30KV2*)lParam);
			// BEAM = ACL I given (output to DAC)
			dlgprintf(IDC_STATIC_STATUS_Pressure, "%.1f", theApp.m_monWSM30KV2.monOP.fACLi);
			break;
		}
	}
	else
	{
		switch( wParam )
		{
		case	AddrWSM30KVR01_FB_P3V3:
			theApp.m_monWSM30KV.monVT = *((MonVT_WSM30KV*)lParam);
			break;
		case	AddrWSM30KVR01_FB_SCI_V:
			theApp.m_monWSM30KV.monFB = *((MonFeedback_WSM30KV*)lParam);
			// EMI = ACL I feedback
			dlgprintf(IDC_STATIC_STATUS_EMI, "%.1f", theApp.m_monWSM30KV.monFB.fACLi);
			// HV = ACL V feedback
			dlgprintf(IDC_STATIC_STATUS_HV, "%.2f", theApp.m_monWSM30KV.monFB.fACLv);
			// FIL = FIL I feedback
			dlgprintf(IDC_STATIC_STATUS_Pressure_Gun, "%.2f", theApp.m_monWSM30KV.monFB.fFILi);
			break;
		case	AddrWSM30KVR01_Given_SCI_V:
			theApp.m_monWSM30KV.monOP = *((MonDACOutput_WSM30KV*)lParam);
			// BEAM = ACL I given (output to DAC)
			dlgprintf(IDC_STATIC_STATUS_Pressure, "%.1f", theApp.m_monWSM30KV.monOP.fACLi);
			break;
		}
	}
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon3->PostMessage(WM_USER_SPELLMAN_PMSKV_UpdateMon, wParam, lParam);

	// 若灯丝电流设置值高于监测值过多，就暂时不允许手动加灯丝电流
	VARIANT Enable, Flag;
	Flag.lVal	= 3;
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
	{
		if( m_btnFilament.GetCheck() < 1 )
			Enable.lVal	= 0;
		else if( theApp.m_monWSM30KV2.monOP.fFILi > 1.9
			&& (theApp.m_monWSM30KV2.monFB.fFILi -theApp.m_monWSM30KV2.monOP.fFILi) / theApp.m_monWSM30KV2.monOP.fFILi > 0.04 )
			Enable.lVal	= 0;
		else
			Enable.lVal	= 1;
	}
	else
	{
		if( m_btnFilament.GetCheck() < 1 )
			Enable.lVal	= 0;
		else if( theApp.m_monWSM30KV.monOP.fFILi > 1.9
			&& (theApp.m_monWSM30KV.monFB.fFILi -theApp.m_monWSM30KV.monOP.fFILi) / theApp.m_monWSM30KV.monOP.fFILi > 0.04 )
			Enable.lVal	= 0;
		else
			Enable.lVal	= 1;
	}
	m_ctrlHeater.Enable(Enable, Flag);

	static DWORD dwWSM = 0;
	DWORD dwTicks = GetTickCount();
	if( (theApp.m_monWSM30KV.monFB.fFILv > 4 || theApp.m_monWSM30KV2.monFB.fFILi > 4)
		&& (dwTicks -dwWSM > 10000) )
	{
		dwWSM = dwTicks;
		// 灯丝断了
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			theApp.m_WSM30KV2.SetParam( 49, WSM30KV2AS_NULL );
		else
			theApp.m_WSM30KV.SetParam( 49, WSM30KVAS_NULL );
		CString str;
		str.Format( "%s %s", GetResString(IDS_SD_PROMPT_FilaBroken), GetResString(IDS_SD_PROMPT_VAC_01) );
		AfxMessageBox( str );
		OnCHECKV1();
	}
	return 0;
}

LRESULT CDlgImpPanelAll2::OnWSM30KVUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
	{
		switch( wParam )
		{
		case	1:
			theApp.m_cfgWSM30KV.bReady = (BOOL)lParam;
			if( theApp.m_cfgWSM30KV.bReady )
			{
//				m_checkACL.EnableWindow(FALSE);
// 				m_checkEMS.EnableWindow(FALSE);
// 				m_checkFIL.EnableWindow(FALSE);
// 				m_checkV1.EnableWindow(TRUE);
			}
			break;
		case	AddrWSM30KV2R_OutputStatus1:
			{
				StatusWSM30KV2_Output1Status s2bak = theApp.m_statusWSM30KV2.statusO;
				theApp.m_statusWSM30KV2.statusO = *((StatusWSM30KV2_Output1Status*)lParam);
				CGlobalParamSetting::Instance().g_WSM30KV2status.statusO = *((StatusWSM30KV2_Output1Status*)lParam);
				if( !(s2bak.status.INTLK) && theApp.m_statusWSM30KV2.statusO.status.INTLK )
				{
					// PMT Interlock已到位
					SetTimer(41, 1000, NULL);
					// COL Interlock已到位
					SetTimer(42, 1500, NULL);
					// SCI Interlock已到位
					SetTimer(43, 2000, NULL);
					theApp.DisplayMsg("WSM Status Changed: Interlock is ready, start the timer");
				}
				// Enable已到位，设置初始值
				if( !(s2bak.status.SCI) && theApp.m_statusWSM30KV2.statusO.status.SCI )
				{
					theApp.m_WSM30KV2.SetParam( 5, 1 );	// SCI 10KV
					theApp.DisplayMsg("WSM30KV: set SCI init value 10000");
					// 准备后台启动一键看像
					Sleep(10);
					SetTimer(26, 1000, NULL);
				}
				if( !(s2bak.status.COL) && theApp.m_statusWSM30KV2.statusO.status.COL )
				{
					theApp.m_WSM30KV2.SetParam( 6, 1 );	// COL 300V
					theApp.DisplayMsg("WSM30KV: set COL init value 300");
				}
				if( !(s2bak.status.PMT) && theApp.m_statusWSM30KV2.statusO.status.PMT )
				{
					VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
					ClassID.lVal		= SemControlPanelClass_SP_PMT;
					SerialNumber.lVal	= -1;
					Enabled.lVal		= 1;
					Flag.lVal			= 3;
					CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

					SerialNumber.lVal	= -1;
					ClassID.lVal		= SemControlPanelClass_SP_PMT;
					Flag.lVal			= 1;
					Xpos.lVal			= 0;
					CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
					theApp.m_WSM30KV2.SetParam( 4, 0 );	// PMT 0V
					theApp.DisplayMsg("WSM30KV: set PMT init value 0");
				}
				if( !(s2bak.status.BIAS) && theApp.m_statusWSM30KV2.statusO.status.BIAS )
					ChangeBias_WSM30KV2_Max();
			}
			break;
		case	AddrWSM30KV2R_ProtectStatusB:
			theApp.m_statusWSM30KV2.statusB = *((StatusWSM30KV2_ProtectStatusB*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2status.statusB = *((StatusWSM30KV2_ProtectStatusB*)lParam);
			break;
		case	AddrWSM30KV2R_ProtectStatusV:
			theApp.m_statusWSM30KV2.statusV = *((StatusWSM30KV2_ProtectStatusV*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2status.statusV = *((StatusWSM30KV2_ProtectStatusV*)lParam);
			break;
		case	AddrWSM30KV2R_ProtectStatusI:
			theApp.m_statusWSM30KV2.statusI = *((StatusWSM30KV2_ProtectStatusI*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2status.statusI = *((StatusWSM30KV2_ProtectStatusI*)lParam);
			break;
		case	AddrWSM30KV2R_ProtectStatusA:
			theApp.m_statusWSM30KV2.statusA = *((StatusWSM30KV2_ProtectStatusArc*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2status.statusA = *((StatusWSM30KV2_ProtectStatusArc*)lParam);
			break;
		case	AddrWSM30KV2R_InquireStatus1:
			theApp.m_statusWSM30KV2.status1 = *((StatusWSM30KV2_InquireStatus1*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2status.status1 = *((StatusWSM30KV2_InquireStatus1*)lParam);
			break;
		case	AddrWSM30KV2R_InquireStatus2:
			theApp.m_statusWSM30KV2.status2 = *((StatusWSM30KV2_InquireStatus2*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2status.status2 = *((StatusWSM30KV2_InquireStatus2*)lParam);
			break;
		case	AddrWSM30KV2R_InquireStatus3:
			theApp.m_statusWSM30KV2.status3 = *((StatusWSM30KV2_InquireStatus3*)lParam);
			CGlobalParamSetting::Instance().g_WSM30KV2status.status3 = *((StatusWSM30KV2_InquireStatus3*)lParam);
			break;
		}
	}
	else
	{
		switch( wParam )
		{
		case	1:
			theApp.m_cfgWSM30KV.bReady = (BOOL)lParam;
			if( theApp.m_cfgWSM30KV.bReady )
			{
// 				m_checkACL.EnableWindow(FALSE);
// 				m_checkEMS.EnableWindow(FALSE);
// 				m_checkFIL.EnableWindow(FALSE);
// 				m_checkV1.EnableWindow(TRUE);
				theApp.DisplayMsg( "WSM30KV is ready!" );
			}
			else
			{
				theApp.DisplayMsg( "WSM30KV is not Ready!" );
			}
			break;
		case	AddrWSM30KVR01_INTStatus:
			{
				StatusWSM30KV_INTStatus INTbak = theApp.m_statusWSM30KV.statusINT01;
				theApp.m_statusWSM30KV.statusINT01 = *((StatusWSM30KV_INTStatus*)lParam);
				if( !(INTbak.status.PMT) && theApp.m_statusWSM30KV.statusINT01.status.PMT )
				{
					// PMT Interlock已到位
					SetTimer(41, 500, NULL);
					theApp.DisplayMsg("WSM Status Changed: PMT Interlock is ready, start the timer");
				}
				if( !(INTbak.status.COL) && theApp.m_statusWSM30KV.statusINT01.status.COL )
				{
					// COL Interlock已到位
					SetTimer(42, 1000, NULL);
					theApp.DisplayMsg("WSM Status Changed: COL Interlock is ready, start the timer");
				}
				if( !(INTbak.status.SCI) && theApp.m_statusWSM30KV.statusINT01.status.SCI )
				{
					// SCI Interlock已到位
					SetTimer(43, 2000, NULL);
					theApp.DisplayMsg("WSM Status Changed: SCI Interlock is ready, start the timer");
				}
			}
			break;
		case	AddrWSM30KVR01_OutputStatus1:
			{
				StatusWSM30KV_Output1Status s01bak = theApp.m_statusWSM30KV.statusO01;
				theApp.m_statusWSM30KV.statusO01 = *((StatusWSM30KV_Output1Status*)lParam);
				// Enable已到位，设置初始值
				if( !(s01bak.status.SCI) && theApp.m_statusWSM30KV.statusO01.status.SCI )
				{
					theApp.m_WSM30KV.SetParam( 5, 1 );	// SCI 10KV
					theApp.DisplayMsg("WSM30KV: set SCI init value 10000");
					// 准备后台启动一键看像
					Sleep(10);
					SetTimer(26, 1000, NULL);
				}
				if( !(s01bak.status.COL) && theApp.m_statusWSM30KV.statusO01.status.COL )
				{
					theApp.m_WSM30KV.SetParam( 6, 1 );	// COL 300V
					theApp.DisplayMsg("WSM30KV: set COL init value 300");
				}
				if( !(s01bak.status.PMT) && theApp.m_statusWSM30KV.statusO01.status.PMT )
				{
					VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
					ClassID.lVal		= SemControlPanelClass_SP_PMT;
					SerialNumber.lVal	= -1;
					Enabled.lVal		= 1;
					Flag.lVal			= 3;
					CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

					SerialNumber.lVal	= -1;
					ClassID.lVal		= SemControlPanelClass_SP_PMT;
					Flag.lVal			= 1;
					Xpos.lVal			= 0;
					CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
					theApp.m_WSM30KV.SetParam( 4, 0 );	// PMT 0V
					theApp.DisplayMsg("WSM30KV: set PMT init value 0");
				}
			}
			break;
		case	AddrWSM30KVR01_ProtectStatusB:
			theApp.m_statusWSM30KV.statusB01 = *((StatusWSM30KV_ProtectStatusB*)lParam);
			break;
		case	AddrWSM30KVR01_ProtectStatusI:
			theApp.m_statusWSM30KV.statusI01 = *((StatusWSM30KV_ProtectStatusI*)lParam);
			break;
		case	AddrWSM30KVR01_ProtectStatusV:
			theApp.m_statusWSM30KV.statusV01 = *((StatusWSM30KV_ProtectStatusV*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_INTStatus:
			theApp.m_statusWSM30KV.statusINT25 = *((StatusWSM30KV_INTStatus*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_OutputStatus1:
			theApp.m_statusWSM30KV.statusO25 = *((StatusWSM30KV_Output1Status*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusB:
			theApp.m_statusWSM30KV.statusB25 = *((StatusWSM30KV_ProtectStatusB*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusI:
			theApp.m_statusWSM30KV.statusI25 = *((StatusWSM30KV_ProtectStatusI*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusV:
			theApp.m_statusWSM30KV.statusV25 = *((StatusWSM30KV_ProtectStatusV*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusArc:
			theApp.m_statusWSM30KV.statusA25 = *((StatusWSM30KV_ProtectStatusArc*)lParam);
			break;
		case	AddrWSM30KVR01_InquireStatus1:
			theApp.m_statusWSM30KV.status101 = *((StatusWSM30KV_InquireStatus1*)lParam);
			break;
		case	AddrWSM30KVR01_InquireStatus2:
			theApp.m_statusWSM30KV.status201 = *((StatusWSM30KV_InquireStatus2*)lParam);
			break;
		case	AddrWSM30KVR01_InquireStatus3:
			theApp.m_statusWSM30KV.status301 = *((StatusWSM30KV_InquireStatus3*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus1:
			theApp.m_statusWSM30KV.status125 = *((StatusWSM30KV_InquireStatus1*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus2:
			theApp.m_statusWSM30KV.status225 = *((StatusWSM30KV_InquireStatus2*)lParam);
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus3:
			theApp.m_statusWSM30KV.status325 = *((StatusWSM30KV_InquireStatus3*)lParam);
			break;
		}
	}
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	pfrm->m_dlgDebug.m_pPageMon3->PostMessage(WM_USER_SPELLMAN_PMSKV_UpdateStatus, wParam, lParam);
	return 0;
}

LRESULT CDlgImpPanelAll2::OnWSM30KVSetParam(WPARAM wParam, LPARAM lParam)
{
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		WSM30KV2_SetParam( wParam, lParam );
	else
		WSM30KV_SetParam( wParam, lParam );
	return 0;
}

void CDlgImpPanelAll2::WSM30KV2_SetParam( WPARAM wParam, LPARAM lParam )
{
	WSM30KV2_AutoState AutoState	= (WSM30KV2_AutoState)wParam;
	
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	Flag.ulVal	= (DWORD)0x01;
	if( AutoState < WSM30KV2AS_Rise_ACL1 )
	{
		// 同步灯丝控件位置及显示值
		ClassID.lVal = SemControlPanelClass_SP_Heater;
		SerialNumber.lVal = -1;
		Xpos.lVal = lParam /10;
		ChangeFilament_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, FALSE );
	}
	else if( AutoState < WSM30KV2AS_Rise_EMS )
	{
		// 同步高压控件位置及显示值
		ClassID.lVal		= SemControlPanelClass_SP_HV;
		SerialNumber.lVal	= -1;
		double dValue		= (LONG)(lParam) /1000.0;
		if( dValue < 10.0 )
			Xpos.lVal	= (LONG)(dValue *10);
		else
			Xpos.lVal	= (LONG)(dValue -10 +100);
		ChangeHighVoltage_WSM30KV(ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, FALSE );
	}
	else if( AutoState < WSM30KV2AS_NULL )
	{
		// 同步束流设置控件位置及显示值
		ClassID.lVal = SemControlPanelClass_SP_Bias;
		SerialNumber.lVal = -1;
		Xpos.lVal = (LONG)(lParam);
		ChangeBeam_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, FALSE );
	}
	else
	{
		OneKEY_LastStep();
	}
}

void CDlgImpPanelAll2::WSM30KV_SetParam( WPARAM wParam, LPARAM lParam )
{
	WSM30KV_AutoState AutoState	= (WSM30KV_AutoState)wParam;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	Flag.ulVal	= (DWORD)0x01;
	if( AutoState < WSM30KVAS_Rise_FIL )
	{
		// 同步高压控件位置及显示值
		ClassID.lVal		= SemControlPanelClass_SP_HV;
		SerialNumber.lVal	= -1;
		double dValue		= (LONG)(lParam) /1000.0;
		if( dValue < 10.0 )
			Xpos.lVal	= (LONG)(dValue *10);
		else
			Xpos.lVal	= (LONG)(dValue -10 +100);
		ChangeHighVoltage_WSM30KV(ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, FALSE );
	}
	else if( AutoState < WSM30KVAS_Rise_EMS )
	{
		// 同步灯丝控件位置及显示值
		ClassID.lVal = SemControlPanelClass_SP_Heater;
		SerialNumber.lVal = -1;
		Xpos.lVal = lParam /10;
		ChangeFilament_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, FALSE );
	}
	else if( AutoState < WSM30KVAS_NULL )
	{
		// 同步束流设置控件位置及显示值
		ClassID.lVal = SemControlPanelClass_SP_Bias;
		SerialNumber.lVal = -1;
		Xpos.lVal = (LONG)(lParam);
		ChangeBeam_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, FALSE );
	}
	else
	{
		OneKEY_LastStep();
	}
}

void CDlgImpPanelAll2::OneKEY_LastStep()
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	Flag.ulVal	= (DWORD)0x01;
	///////////////////////////////////////////////////
	// 做几次聚光镜反转
	ClassID.lVal = SemControlPanelClass_CondenserLens;
	SerialNumber.lVal = -1;
	BOOL bRev = TRUE;
	int i;
	for( i=0; i<6; i++ )
	{
		CSemCoreAssemble::Instance().m_CondenserLens.SetParams( SET_PARAMS_Reverse, bRev );	// 反转
		Xpos = CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		if( bRev )
			theApp.DisplayMsg("Press CL reverse: ON");
		else
			theApp.DisplayMsg("Press CL reverse: OFF");
		bRev = !bRev;
		Sleep(500);
	}
	// 做几次物镜反转
	ClassID.lVal = SemControlPanelClass_ObjectiveLens;
	for( i=0; i<6; i++ )
	{
		CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( SET_PARAMS_Reverse, bRev );	// 反转
		Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		if( bRev )
			theApp.DisplayMsg("Press OL reverse: ON");
		else
			theApp.DisplayMsg("Press OL reverse: OFF");
		bRev = !bRev;
		Sleep(500);
	}
	// 进度条
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
		pView->FuncAutoWaitHV( TRUE );
	///////////////////////////////////////////////////
/*	// 自动加灯丝至1.9A已完成，启动自动灯丝调节（找饱和点）
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		CHVPowerDataManager::Instance().m_dHV	= theApp.m_setWSM30KV2.fACLv;
	else
		CHVPowerDataManager::Instance().m_dHV	= theApp.m_setWSM30KV.fACLv;
	if( pView != NULL && CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( CHVPowerDataManager::Instance().m_dHV );
	if( CScanParameterSaver::Instance().m_nAutoFuction == AutoNULL )
	{
		pView->FuncAutoFilament();
		SetTimer(13, 500, NULL);
	}
*/	// 自动加灯丝已完成，启动自动聚焦
	if( pView != NULL && CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
		CSemCtrl::Instance().m_SemScan.USB_SetRampParam_CoadjustHV( CHVPowerDataManager::Instance().m_dHV );
	//if( CScanParameterSaver::Instance().m_nAutoFuction == AutoNULL )
	//	pView->FuncAutoFocus();
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CDlgImpPanelAll2::Message(LPCTSTR lpszMessage)
{
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log) );
	if( peditLog == NULL )
		return;

	CString csMessage = CString(lpszMessage) + _T("\r\n");
	int len = peditLog->GetWindowTextLength();
	peditLog->SetSel(len,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );
}

void CDlgImpPanelAll2::Message2(LPCTSTR lpszMessage)
{
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log2) );
	if( peditLog == NULL )
		return;

	CString csMessage = CString(lpszMessage) + _T("\r\n");
	int len = peditLog->GetWindowTextLength();
	peditLog->SetSel(len,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );
}

void CDlgImpPanelAll2::RegisterCtrl()
{
	//注册控件
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlWD, SemControlPanelClass_WorkDistance );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlCondenserLens, SemControlPanelClass_CondenserLens );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlObjectiveLens, SemControlPanelClass_ObjectiveLens );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlBrightness, SemControlPanelClass_Brightness );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlSignal2Brightness, SemControlPanelClass_Signal2Brightness );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlMagnifier, SemControlPanelClass_Magnifier );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlBeemAlignment, SemControlPanelClass_BeemAlignment );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlScanShift, SemControlPanelClass_ScanShift );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlStigmation, SemControlPanelClass_Stigmator );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlScanRotate, SemControlPanelClass_ScanRotate );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlAuxiliaryStigmation, SemControlPanelClass_AuxiliaryStigmation );

	///////////////////////////////////////////////////////////////////////////
	// 高压电源模块
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		RegisterControls_CPS();
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
			RegisterControls_WSM();
		else
			RegisterControls_SPM();
	}
}

void CDlgImpPanelAll2::RegisterControls_CPS()
{
	VARIANT Xmin, Xmax, Ymin, Ymax, Xpos, Ypos, Flag, ClassID, SerialNumber, Enable;
	CString strX, strY;
	Xpos.lVal	= 0;
	Flag.ulVal	= (DWORD)0x01;
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlContrast, SemControlPanelClass_Contrast );
	// CPS 3601
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlHV, SemControlPanelClass_CPS_HV );
	//CSemCtrlPanelManager::Instance().Regist( &m_ctrlBias, SemControlPanelClass_CPS_Bias);
	//CSemCtrlPanelManager::Instance().Regist( &m_ctrlHeater, SemControlPanelClass_CPS_FilamentCurrent);

	// Bias
	Xmin.lVal	= 0;
	Xmax.lVal	= 500;		// 500V
	Ymin		= Xmin;
	Ymax		= Xmax;
	m_ctrlBias.SetRange( Xmin, Xmax, Ymin, Ymax );
	m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
	//strX.Format( "%.1f V", Xpos.lVal );
	strX = "0.0 V";
	m_ctrlBias.SyncText( strX, strY, Flag );
	ClassID.lVal		= SemControlPanelClass_CPS_Bias;
	SerialNumber.lVal	= 201;
	m_ctrlBias.Bind( ClassID, SerialNumber );
	// Filamnet
	Xmin.lVal	= 0;
	Xmax.lVal	= 300;		// 3A
	Ymin		= Xmin;
	Ymax		= Xmax;
	Xpos.lVal   = 0;
	m_ctrlHeater.SetRange( Xmin, Xmax, Ymin, Ymax );
	m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
	//strX.Format( "%.2f A", Xpos.lVal );
	strX = "0.00 A";
	m_ctrlHeater.SyncText( strX, strY, Flag );
	ClassID.lVal		= SemControlPanelClass_CPS_FilamentCurrent;
	SerialNumber.lVal	= 202;
	m_ctrlHeater.Bind( ClassID, SerialNumber );

	Flag.lVal	= 3;
	Enable.lVal	= 0;
	m_ctrlHV.Enable(Enable, Flag);
	Enable.lVal	= 0;
	m_ctrlBias.Enable(Enable, Flag);
	Enable.lVal	= 0;
	m_ctrlHeater.Enable(Enable, Flag);
}

void CDlgImpPanelAll2::RegisterControls_SPM()
{
	VARIANT Xmin, Xmax, Ymin, Ymax, Xpos, Ypos, Flag, ClassID, SerialNumber, Enable;
	CString strX, strY;
	Xpos.lVal	= 0;
	Flag.ulVal	= (DWORD)0x01;

	// Spellman EBM30N12/771
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlHV, SemControlPanelClass_SP_HV );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlContrast, SemControlPanelClass_SP_PMT );
	//CSemCtrlPanelManager::Instance().Regist( &m_ctrlBias, SemControlPanelClass_SP_Bias);
	//CSemCtrlPanelManager::Instance().Regist( &m_ctrlHeater, SemControlPanelClass_SP_Heater);

	// Beam
	Xmin.lVal	= 0;
	Xmax.lVal	= 400;		// 400uA
	Ymin		= Xmin;
	Ymax		= Xmax;
	m_ctrlBias.SetRange( Xmin, Xmax, Ymin, Ymax );
	m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
	strX.Format( "%ld uA", Xpos.lVal );
	m_ctrlBias.SyncText( strX, strY, Flag );
	ClassID.lVal		= SemControlPanelClass_SP_Bias;
	SerialNumber.lVal	= 211;
	m_ctrlBias.Bind( ClassID, SerialNumber );
	// Heater
	Xmin.lVal	= 0;
	Xmax.lVal	= 300;		// 3A
	Ymin		= Xmin;
	Ymax		= Xmax;
	Xpos.lVal   = 0;
	m_ctrlHeater.SetRange( Xmin, Xmax, Ymin, Ymax );
	m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
	//strX.Format( "%.2f A", Xpos.lVal );
	strX = "0.00 A";
	m_ctrlHeater.SyncText( strX, strY, Flag );
	ClassID.lVal		= SemControlPanelClass_SP_Heater;
	SerialNumber.lVal	= 212;
	m_ctrlHeater.Bind( ClassID, SerialNumber );

	Flag.lVal	= 3;
	Enable.lVal	= 0;
	m_ctrlHV.Enable(Enable, Flag);
	Enable.lVal	= 0;
	m_ctrlBias.Enable(Enable, Flag);
	Enable.lVal	= 0;
	m_ctrlHeater.Enable(Enable, Flag);
}

void CDlgImpPanelAll2::RegisterControls_WSM()
{
	VARIANT Xmin, Xmax, Ymin, Ymax, Xpos, Ypos, Flag, ClassID, SerialNumber, Enable;
	CString strX, strY;
	Xpos.lVal	= 0;
	Flag.ulVal	= (DWORD)0x01;
	// WSM30KV
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlHV, SemControlPanelClass_SP_HV );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlContrast, SemControlPanelClass_SP_PMT );

	// Beam
	Xmin.lVal	= 0;
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		Xmax.lVal	= (int)g_fWSM30KV2FS_ACL_mon_I;	// 240uA
	else
		Xmax.lVal	= (int)g_fWSM30KVFS_ACL_mon_I;	// 400uA
	Ymin		= Xmin;
	Ymax		= Xmax;
	m_ctrlBias.SetRange( Xmin, Xmax, Ymin, Ymax );
	m_ctrlBias.SyncPos( Xpos, Ypos, Flag );
	strX.Format( "%ld uA", Xpos.lVal );
	m_ctrlBias.SyncText( strX, strY, Flag );
	ClassID.lVal		= SemControlPanelClass_SP_Bias;
	SerialNumber.lVal	= 211;
	m_ctrlBias.Bind( ClassID, SerialNumber );
	// Heater
	Xmin.lVal	= 0;
	Xmax.lVal	= 300;		// 3A
	Ymin		= Xmin;
	Ymax		= Xmax;
	m_ctrlHeater.SetRange( Xmin, Xmax, Ymin, Ymax );
	m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
	strX.Format( "%.2f A", Xpos.lVal*0.01 );
	m_ctrlHeater.SyncText( strX, strY, Flag );
	ClassID.lVal		= SemControlPanelClass_SP_Heater;
	SerialNumber.lVal	= 212;
	m_ctrlHeater.Bind( ClassID, SerialNumber );

	Flag.lVal	= 3;
	Enable.lVal	= 0;
	m_ctrlHV.Enable(Enable, Flag);
	Enable.lVal	= 0;
	m_ctrlBias.Enable(Enable, Flag);
	Enable.lVal	= 0;
	m_ctrlHeater.Enable(Enable, Flag);

	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
	{
//		GetDlgItem( IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias )->ShowWindow( SW_SHOW );
		// Bias
		Xmin.lVal	= 0;
		Xmax.lVal	= (int)g_fWSM30KV2FS_BIAS_mon_V;	// 2000V
		Ymin		= Xmin;
		Ymax		= Xmax;
		m_ctrlWSMBias.SetRange( Xmin, Xmax, Ymin, Ymax );
		m_ctrlWSMBias.SyncPos( Xpos, Ypos, Flag );
		strX.Format( "%ld V", Xpos.lVal );
		m_ctrlWSMBias.SyncText( strX, strY, Flag );
		ClassID.lVal		= SemControlPanelClass_CPS_Bias;
		SerialNumber.lVal	= 213;
		m_ctrlWSMBias.Bind( ClassID, SerialNumber );
	}
//	else
//		GetDlgItem( IDC_SEMCORECTRLPANELSTAND1CTRL_WSM_Bias )->ShowWindow( SW_HIDE );
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 导航

void CDlgImpPanelAll2::updateStatusV1(bool status)
{
	//GetDlgItem(IDC_CHECK_Vacuum_VACUUM_POWER)->EnableWindow(status);
	m_btnV1.EnableWindow(status);
}

void CDlgImpPanelAll2::OnBUTTONNavigate()
{	
	NAV_ShowWnd( this->m_hWnd );
	NAV_SetLanguageID(CGlobalParamSetting::Instance().m_wLanguageID);
	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	22:
	case	21:
		NAV_SetV1Status(CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM);
		break;
	case	0:
		NAV_SetV1Status(CVACDataManager::Instance().m_VacParamMain.bV1);
		break;
	default:
		NAV_SetV1Status(CVACDataManager::Instance().m_statusBoard.Valve.status.V1Rec);
		break;
	}
}	

LRESULT CDlgImpPanelAll2::OnNAVUpdate(WPARAM wParam, LPARAM lParam)
{
	char szBuf[255] = {0};
	switch( wParam )
	{
	case	11:	// 导航灯板开关控制
		{
			BOOL bNavPowerOn = (BOOL)lParam;
			SPR_Fan( bNavPowerOn );
		}
		break;

	case	12:	// 移动自动台X和Y
		{
			NAV_Position& navPos = *((NAV_Position*)lParam);
			sprintf_s( szBuf, "NAV: Recv positions: [%.2f, %.2f, %.2f]", navPos.fPosX, navPos.fPosY, navPos.fPosZ );
			theApp.DisplayMsg( szBuf );
			float fPos[5] = {0.f};
			// 获取当前位置值
			if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
				StageTMCM_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
				StageMC6600_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
				StageMC600_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
			//navPos.fPosXReceive = fPos[0];
			//navPos.fPosYReceive = fPos[1];
			// 导航目标位置更新
			BOOL bMultiAxis = FALSE;
			if( navPos.fPosZ > -999.0 )
			{
				// 约定：Z值有效，是要回导航原点；此时应让T归零
				bMultiAxis = TRUE;
				// 保存位置
				if( m_nNavProcess == 1 )
				{
					// In的安装方式，要保存当前位置
					m_navPosOrigin.fPosX = fPos[0];
					m_navPosOrigin.fPosY = fPos[1];
					m_navPosOrigin.fPosZ = fPos[2];
					m_navPosOrigin.fPosR = fPos[3];
					m_navPosOrigin.fPosT = fPos[4];
					sprintf_s( szBuf, "NAV: origin positions: [%.3f, %.3f, %.2f, %.1f, %.1f]", m_navPosOrigin.fPosX, m_navPosOrigin.fPosY, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR, m_navPosOrigin.fPosT );
					theApp.DisplayMsg( szBuf );
				}
				fPos[2] = navPos.fPosZ;
				fPos[4] = 0.0;
			}
			fPos[0] = navPos.fPosX;
			fPos[1] = navPos.fPosY;
			// 移动样品台
			if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
			{
				StageTMCM_Move5Axis( fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
				SetTimer( 32, 1000, NULL );	// 启动定时器，查询自动台是否到位
			}
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
			{
				if( bMultiAxis )
					StageMC6600_Move5Axis( fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
				else
					StageMC6600_Move2Axis( fPos[0], fPos[1] );
				SetTimer( 32, 1000, NULL );	// 启动定时器，查询自动台是否到位
			}
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 && StageMC600_IsReady())
			{
				StageMC600_Move5Axis( fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
				SetTimer( 32, 1000, NULL );	// 启动定时器，查询自动台是否到位
			}
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
			{
				StageMC600_Move4Axis( fPos[0], fPos[1], fPos[2], fPos[3] );
				SetTimer( 32, 1000, NULL );	// 启动定时器，查询自动台是否到位
			}
		}
		break;

	case	13:	// 获取当前位置值后，更新导航原点参数并写入文件
		{
			float fPos[5] = {0.f};
			// 获取当前位置值
			if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
				StageTMCM_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
				StageMC6600_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
				StageMC600_GetParam( 2, fPos[0], fPos[1], fPos[2], fPos[3], fPos[4] );
			// 更新XY
			NAV_UpdateXYPos( fPos[0], fPos[1] );
			// 更新Z
			NAV_UpdateXYPos( -201, fPos[2] );
			m_navPosStart.fPosX = fPos[0];
			m_navPosStart.fPosY = fPos[1];
			m_navPosStart.fPosZ = fPos[2];
			sprintf_s( szBuf, "NAV: Get Current pos (%.3f, %.3f, %.2f), update NAV, and save to ini", fPos[0], fPos[1], fPos[2] );
			theApp.DisplayMsg( szBuf );
		}
		break;
	case	14:
		//m_bV1CanOpen = (BOOL)lParam;
		//sprintf_s( szBuf, "NAV: Recv V1CanOpen (%ld)", lParam );
		//theApp.DisplayMsg( szBuf );
		break;
	case	15:
		{
			NAV_Settings_Move navMove = *((NAV_Settings_Move*)lParam);
			m_navPosStart = navMove.posStart;
			m_nNavProcess = navMove.nProcess;
			sprintf_s( szBuf, "NAV: Recv OpenMove Params: [%.2f, %.2f, %.2f], Process = %d",
						m_navPosStart.fPosX, m_navPosStart.fPosY, m_navPosStart.fPosZ, m_nNavProcess );
			theApp.DisplayMsg( szBuf );
		}
		break;
	case	16:	// 导航图片拍完之后，回到扫描原点
		{
			if( m_nNavProcess < 2 )
			{
				if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
					StageTMCM_GoOrigin();
				else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
					StageMC6600_GoOrigin();
				else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
					StageMC600_GoOrigin();
				theApp.DisplayMsg("NAV: finish nav image, go to origin position");
			}
/*			else if( m_nNavProcess < 2 )
			{
				// XY回原点，Z回之前的位置
				if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
					StageTMCM_Move5Axis( 0.0f, 0.0f, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR, 0.0f );
				else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
					StageMC6600_Move5Axis( 0.0f, 0.0f, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR, 0.0f );
				else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 && StageMC600_IsReady())
					StageMC600_Move5Axis( 0.0f, 0.0f, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR, 0.0f );
				else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
					StageMC600_Move4Axis( 0.0f, 0.0f, m_navPosOrigin.fPosZ, m_navPosOrigin.fPosR );
				theApp.DisplayMsg("NAV: finish nav image, go to origin position");
			}*/
		}
		break;
/*	case 18:
		{
			NAV_Position5& navPos = *((NAV_Position5*)lParam);
			if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
				StageTMCM_GetParam( 2, navPos.fPosX,navPos.fPosY,navPos.fPosZ,navPos.fPosR,navPos.fPosT );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
				StageMC6600_GetParam( 2, navPos.fPosX,navPos.fPosY,navPos.fPosZ,navPos.fPosR,navPos.fPosT );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
				StageMC600_GetParam( 2, navPos.fPosX,navPos.fPosY,navPos.fPosZ,navPos.fPosR,navPos.fPosT );
			break;
		}*/
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 推拉杆

void CDlgImpPanelAll2::StagePodInit()
{
	for(int i =0;i<3;i++)
	{
		m_nStagePodSetTrack1[i] =0;
		m_nStagePodSetTrack2[i] =0;
	}
}

void CDlgImpPanelAll2::StagePodTrack(short nIndex)
{
	switch( nIndex )
	{
	case	100:
		if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
			StageTMCM_StopTracklocus();
		else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
			StageMC6600_StopTracklocus();
		StagePodInit();
		break;

	case	1:
	case	2:
		{
			BOOL bNum[3];
			int i;
			for( i=0; i<3; i++ )
			{
				if(m_nStagePodSetTrack1[i] == 0)
				{
					if(m_nStagePodState[i] -StagePod_Stop > StagePod_Offset_Action )
						m_nStagePodSetTrack1[i] =  1;
					else if (m_nStagePodState[i] -StagePod_Stop < -StagePod_Offset_Action )
						m_nStagePodSetTrack1[i] =  -1;
					bNum[i] = TRUE;
				}
				else
				{
					if(m_nStagePodState[i] -StagePod_Stop > StagePod_Offset_Action  )
						m_nStagePodSetTrack2[i] =  1;
					else if (m_nStagePodState[i] -StagePod_Stop < -StagePod_Offset_Action )
						m_nStagePodSetTrack2[i] =  -1;
					bNum[i] = FALSE;
				}
			}

			if((m_nStagePodSetTrack1[0] == m_nStagePodSetTrack2[0])
				&& (m_nStagePodSetTrack1[1] == m_nStagePodSetTrack2[1])
				&& (m_nStagePodSetTrack1[2] == m_nStagePodSetTrack2[2]))
				break;	//return;

			for(int i=0; i<3; i++)
				if( !bNum[i] )
					m_nStagePodSetTrack1[i] = m_nStagePodSetTrack2[i];

			unsigned short nState[3];
			for( i=0; i<3; i++ )
			{
				// 如果变化量在偏移量范围内，就认为该轴不动，即下送0值
				if( m_nStagePodSetTrack1[i] != 0 )
					nState[i] = m_nStagePodState[i];
				else
					nState[i] = 0;
			}
			if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady() )
			{
				if( nIndex > 1 )
					StageTMCM_SetTracklocus2( nState[0], nState[1] );
				else
					StageTMCM_SetTracklocus1( nState[0], nState[1], nState[2] );
			}
			else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady() )
			{
				if( nIndex > 1 )
					StageMC6600_SetTracklocus2( nState[0], nState[1] );
				else
					StageMC6600_SetTracklocus1( nState[0], nState[1], nState[2] );
			}
		}
		break;
	} 
}

LRESULT CDlgImpPanelAll2::OnUpdateStagePod(WPARAM wParam, LPARAM lParam)
{
	CString str;
	switch( wParam )
	{
	case	0:
		{
			BOOL bReady = (BOOL)lParam;
			if( bReady )
				OutputDebugString( "Stage Pod is ready!\n");
			else
				OutputDebugString( "Stage Pod is NOT ready!\n");
		}
		break;

	case	1:
	case	2:
		{
			unsigned short* state = (unsigned short *)lParam;
			for( int i=0; i<3; i++ )
				m_nStagePodState[i] = state[i];
			StagePodTrack( (short)wParam );
		}
		break;

	case	100:
		{
			for( int i=0; i<3; i++ )
				m_nStagePodState[i] = 0;
			StagePodTrack( (short)wParam );
			theApp.DisplayMsg( "Stage Pod Track STOP!" );
		}
		break;
	}

	return 0;
}


void CDlgImpPanelAll2::OnBnClickedButtonDiatomdetect()
{
	// TODO: 在此添加控件通知处理程序代码
	CAutoImageDlg dlg;
	dlg.DoModal();
}
