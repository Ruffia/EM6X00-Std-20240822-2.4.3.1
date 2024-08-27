// DlgImpPanelDebug.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgImpPanelDebug.h"

#include "ScanView.h"
#include "MainFrm.h"
#include "DlgPrompt.h"
#include "StageXYManager.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "HVPowerDataManager.h"
#include "ScanParameterManager.h"
#include "PLCvacBrdStatusDetector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebug dialog


CDlgImpPanelDebug::CDlgImpPanelDebug(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImpPanelDebug::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImpPanelDebug)
	//}}AFX_DATA_INIT
}

CDlgImpPanelDebug::~CDlgImpPanelDebug()
{
}

void CDlgImpPanelDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImpPanelDebug)
	DDX_Control(pDX, IDC_CHECK_TestLEDs, m_btnTestLEDs);
	DDX_Control(pDX, IDC_BUTTON_Relay1EnCmd, m_btnSPRRelay1En);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImpPanelDebug, CDialog)
	//{{AFX_MSG_MAP(CDlgImpPanelDebug)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_TestLEDs, OnCHECKTestLEDs)
	ON_BN_CLICKED(IDC_BUTTON_Relay1EnCmd, OnSPRRelay1En)
	ON_BN_CLICKED(IDC_BUTTON_ReadIDs, OnReadBoardIDs)
	ON_BN_CLICKED(IDC_BUTTON_Reset_SubBoards, OnBUTTONResetSubBoards)
	ON_BN_CLICKED(IDC_BUTTON_Reset_FT232, OnBUTTONResetFT232)
	ON_BN_CLICKED(IDC_BUTTON_Reset_DI16A, OnBUTTONResetDI16A)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_SetLength, OnBUTTONSetupStatus)
	ON_BN_CLICKED(IDC_CHECK_Debug_V1, OnCHECKDebugV1)
	ON_BN_CLICKED(IDC_CHECK_Debug_V2, OnCHECKDebugV2)
	ON_BN_CLICKED(IDC_CHECK_Debug_V3, OnCHECKDebugV3)
	ON_BN_CLICKED(IDC_CHECK_Debug_V4, OnCHECKDebugV4)
	ON_BN_CLICKED(IDC_CHECK_Debug_TMP, OnCHECKDebugTMP)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_TCf, OnCHECKVacuumTCf)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_TCc, OnCHECKVacuumTCc)
	ON_BN_CLICKED(IDC_CHECK_Vacuum_PauseRead, OnCHECKVacuumPauseRead)
	ON_BN_CLICKED(IDC_CHECK_Debug_CCGvalueLock, OnCHECKDebugCCGvalueLock)
	ON_BN_CLICKED(IDC_CHECK_Debug_CCGvacuum, OnCHECKDebugCCGvacuum)
	ON_BN_CLICKED(IDC_CHECK_OFF, OnCHECKOff)
	ON_BN_CLICKED(IDC_CHECK_Debug_DrawCircle, OnCHECKDrawCircle)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_EditText_Size, OnSelchangeCCD)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_EditText_Name, OnSelchangeStage)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_LabelText_Name, OnSelchangeEDS)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_UScaleText_Name, OnSelchangeBSD)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_LabelText_Size, OnSelchangeHVType)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_UScaleText_Size, OnSelchangeVacType)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_SEratio, OnSelchangeGaugeType)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_Chn1, OnSelchangeNAV)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_Chn2, OnSelchangeBeam)
	ON_BN_CLICKED(IDC_BUTTON_PLCvacBrd_Thresholds_Set, OnBUTTONPLCvacBrdThresholdsSet)
	ON_BN_CLICKED(IDC_BUTTON_PLCvacBrd_Thresholds_Get, OnBUTTONPLCvacBrdThresholdsGet)
// 	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebug message handlers

void CDlgImpPanelDebug::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_CHECK_Debug_HV)->SetWindowText(GetResString( IDS_CHECK_Debug_HV )); 
	GetDlgItem(IDC_CHECK_Debug_Vacuum)->SetWindowText(GetResString( IDS_CHECK_Debug_Vacuum )); 
	GetDlgItem(IDC_CHECK_DEBUG_AutoStage)->SetWindowText(GetResString( IDS_CHECK_DEBUG_AutoStage )); 
	GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_SetLength)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_SetLength ));
	GetDlgItem(IDC_CHECK_Debug_CCGvalueLock)->SetWindowText(GetResString( IDS_CHECK_Debug_CCGvalueLock ));
}

BOOL CDlgImpPanelDebug::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnTestLEDs.SetColor( ::GetSysColor(COLOR_3DFACE) );

	GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( FALSE );
	m_btnSPRRelay1En.SetColor( g_crOFF );
	m_bSPRRelay1En = FALSE;

	UpdateData( FALSE );
	Localize();

	if( CStageXYManager::Instance().m_Stage.nHasAutoStage != 1 )
		GetDlgItem(IDC_CHECK_DEBUG_AutoStage)->EnableWindow( FALSE );

	// CCD : IDC_PAGE_SYS_Font_ComboBox_EditText_Size
	CComboBox* pCCD = (CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_EditText_Size));
	pCCD->AddString( "No" );
	pCCD->AddString( "Yes" );
	pCCD->SetCurSel( theApp.m_bHasCCD );
	// Stage : IDC_PAGE_SYS_Font_ComboBox_EditText_Name
	CComboBox* pStage = (CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_EditText_Name));
	pStage->AddString( "TMCM" );
	pStage->AddString( "MC6600" );
	pStage->AddString( "MC600+V" );
	pStage->AddString( "MC600" );
	pStage->AddString( "XY" );
	pStage->AddString( "None" );
	pStage->SetCurSel( 5 -CStageXYManager::Instance().m_Stage.nHasAutoStage );
	// EDS : IDC_PAGE_SYS_Font_ComboBox_LabelText_Name
	CComboBox* pEDS = (CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_LabelText_Name));
	pEDS->AddString( "No" );
	pEDS->AddString( "Yes" );
	pEDS->SetCurSel( theApp.m_bHasEDS );
	// BSD : IDC_PAGE_SYS_Font_ComboBox_UScaleText_Name
	CComboBox* pBSD = (CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_UScaleText_Name));
	pBSD->AddString( "No" );
	pBSD->AddString( "Yes" );
	pBSD->SetCurSel(  CControlLayer::Instance().m_bHasBSD );
	// HV type : IDC_PAGE_SYS_Font_ComboBox_LabelText_Size
	CComboBox* pHVType = (CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_LabelText_Size));
	pHVType->AddString( "CPS" );
	pHVType->AddString( "Spellman" );
	pHVType->AddString( "Wisman" );
	pHVType->AddString( "Wisman2" );
	pHVType->SetCurSel( CHVPowerDataManager::Instance().m_dwHVtype );
	// Vac type : IDC_PAGE_SYS_Font_ComboBox_UScaleText_Size
	CComboBox* pVacType = (CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_UScaleText_Size));
	pVacType->AddString( "Board" );
	pVacType->AddString( "PLC" );
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
		pVacType->SetCurSel( 1 );
	else
		pVacType->SetCurSel( 0 );
	// Gauge type : IDC_PAGE_SCAN_COMBO_SEratio
	CComboBox* pGaugeType = (CComboBox*)(GetDlgItem(IDC_PAGE_SCAN_COMBO_SEratio));
	pGaugeType->AddString( "INFICON" );
	pGaugeType->AddString( "WPC" );
	if( CVACDataManager::Instance().m_cfgVacCCG.nType > 3 )
		pGaugeType->SetCurSel( 1 );
	else
		pGaugeType->SetCurSel( 0 );
	// NAV : IDC_PAGE_SCAN_COMBO_Chn1
	CComboBox* pNAV = (CComboBox*)(GetDlgItem(IDC_PAGE_SCAN_COMBO_Chn1));
	pNAV->AddString( "No" );
	pNAV->AddString( "Yes" );
	pNAV->SetCurSel(  theApp.m_bHasNAV );
	// Beam Detection : IDC_PAGE_SCAN_COMBO_Chn2
	CComboBox* pBeam = (CComboBox*)(GetDlgItem(IDC_PAGE_SCAN_COMBO_Chn2));
	pBeam->AddString( "No" );
	pBeam->AddString( "D22" );
	pBeam->SetCurSel( CControlLayer::Instance().m_nPAType );

	UINT nLifetime = CSemCtrlInitializer::Instance().ReadSettingFromRegistry( g_csSettingSection, 
		g_csRegEntry_nFilaLifeTime, 0 );
	CString str;
	str.Format( "%.1f", nLifetime /60.0 );
	GetDlgItem( IDC_PANEL_LOG_EDIT_Log )->SetWindowText( str );

	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		GetDlgItem(IDC_CHECK_Vacuum_TCf)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_Vacuum_TCc)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_Debug_CCGvalueLock)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_Debug_CCGvacuum)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_Vacuum_PauseRead)->ShowWindow(SW_HIDE);
	}
	else if( CSemCtrl::Instance().m_nVACBoardType > 0 )
	{
		GetDlgItem(IDC_CHECK_Debug_Vacuum)->ShowWindow(SW_HIDE);
		CRect rc, rcRef;
		GetDlgItem(IDC_STATIC_Debug)->GetWindowRect(&rcRef);
		CWnd *pWnd = GetWindow(GW_CHILD);
		while(pWnd != NULL)
		{
			pWnd->GetWindowRect(&rc);
			if( PtInRect( rcRef, rc.CenterPoint() ) )
				pWnd->ShowWindow(SW_HIDE);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		};
	}
	if( CSemCtrl::Instance().m_nVACBoardType < 20 )
	{
		GetDlgItem(IDC_STATIC_PLCvacBrd_Group_Thresholds)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PLCvacBrd_Thresholds_VacPre)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PLCvacBrd_Thresholds_VacOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PLCvacBrd_Thresholds_VacNotOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PLCvacBrd_Thresholds_VacPre)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PLCvacBrd_Thresholds_VacOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PLCvacBrd_Thresholds_VacNotOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_PLCvacBrd_Thresholds_Set)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_PLCvacBrd_Thresholds_Get)->ShowWindow(SW_HIDE);
	}
	if( CSemCtrl::Instance().m_nVACBoardType > 21 )
		GetDlgItem(IDC_CHECK_Debug_MP)->ShowWindow(TRUE);
	else
		GetDlgItem(IDC_CHECK_Debug_MP)->ShowWindow(FALSE);

	// 将对话框位置设置到客户区左上角
	CRect rc;
	GetClientRect( &rc );
	ClientToScreen( &rc );
	SetWindowPos(&CWnd::wndTop,
		10,//GetSystemMetrics(SM_CXSCREEN) - rc.Width()-10,
		32*3,//GetSystemMetrics(SM_CYSCREEN) /2 -rc.Height() /2,
		rc.Width(), rc.Height(), SWP_NOSIZE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgImpPanelDebug::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDlgImpPanelDebug::OnOK() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgImpPanelDebug::OnCancel() 
{
	// TODO: Add your control notification handler code here
	((CButton*)(GetDlgItem(IDC_CHECK_Debug_HV)))->SetCheck(0);
	((CButton*)(GetDlgItem(IDC_CHECK_Debug_Vacuum)))->SetCheck(0);
	((CButton*)(GetDlgItem(IDC_CHECK_DEBUG_AutoStage)))->SetCheck(0);
	SetupDebugParam( FALSE );
	((CMainFrame *)theApp.GetMainWnd())->DebugClose();

	CDialog::OnCancel();
}



/////////////////////////////////////////////////////////////////////
// 通讯测试
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebug::OnCHECKTestLEDs() 
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
		m_btnTestLEDs.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnTestLEDs.SetWindowText( "Test LEDs: now OFF" );
		theApp.DisplayMsg("Press test LEDs: OFF");
	}

	///////////////////////////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Test_LED,		// Low byte = ckt addr. of Test LED 16
										USB_TM_SCN_Write_08,		// Serial Comm operand high byte (8 bit write to SCN board)
										0x0,						// Low byte for 8 bit write needs to be in hi byte operand location (for now)
										nCheck );					// Hi byte data is actually low byte of 8 bit write
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_Test_LED,		// Low byte = ckt addr. of Test LED 16
										USB_TM_LNS_Write_08,		// Serial Comm operand high byte (8 bit write to LNS board)
										0x0,						// Low byte for 8 bit write needs to be in hi byte operand location (for now)
										nCheck );					// Hi byte data is actually low byte of 8 bit write
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_Test_LED,		// Low byte = ckt addr. of Test LED 16
										USB_TM_VAC_Write_08,		// Serial Comm operand high byte (8 bit write to VAC board)
										0x0,						// OP3 = 0 for 8 bit write
										nCheck );					// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_Test_LED,		// Low byte = ckt addr. of Test LED 16
										USB_TM_SPR_Write_08,		// Serial Comm operand high byte (8 bit write to SPR board)
										0x0,						// OP3 = 0 for 8 bit write
										nCheck );					// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CDlgImpPanelDebug::OnReadBoardIDs()
{
	theApp.DisplayMsg("Press ReadBoadIDs");
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( "" );
	unsigned short nRegister1 = 0;
	unsigned short nRegister2 = 0;
	unsigned short nRegister3 = 0;
	unsigned short nRegister4 = 0;
	BOOL bOld = TRUE;
	if( CSemCtrl::Instance().m_nVACBoardType > 0 )
		bOld = FALSE;
	CSemCtrl::Instance().Serial_ReadbackData( 1, USB_CID_SCN_ReadBoardID, &nRegister1 );
	CSemCtrl::Instance().Serial_ReadbackData( 2, USB_CID_LNS_ReadBoardID, &nRegister2 );
	if( bOld )
	   CSemCtrl::Instance().Serial_ReadbackData( 3, USB_CID_VAC_ReadBoardID, &nRegister3 );
	CSemCtrl::Instance().Serial_ReadbackData( 4, USB_CID_SPR_ReadBoardID, &nRegister4 );

	CString str;
	if( (nRegister1 == 0xA1) && (nRegister2 == 0xA2) && (nRegister4 == 0xA4)
		&& ((bOld && nRegister3 == 0xA3) || !bOld) )
		str = GetResString( IDS_SD_PROMPT_AllBoardsOK );
	else
	{
		CString strTemp;
		if( nRegister1 != 0xA1 )
		{
			strTemp.Format( "%s %s\r\n", GetResString(IDS_SD_PROMPT_VAC_31), GetResString(IDS_SD_PROMPT_VAC_39) );
			str += strTemp;
		}
		if( nRegister2 != 0xA2 )
		{
			strTemp.Format( "%s %s\r\n", GetResString(IDS_SD_PROMPT_VAC_32), GetResString(IDS_SD_PROMPT_VAC_39) );
			str += strTemp;
		}
		if( bOld && nRegister3 != 0xA3 )
		{
			strTemp.Format( "%s %s\r\n", GetResString(IDS_SD_PROMPT_VAC_33), GetResString(IDS_SD_PROMPT_VAC_39) );
			str += strTemp;
		}
		if( nRegister4 != 0xA4 )
		{
			strTemp.Format( "%s %s\r\n", GetResString(IDS_SD_PROMPT_VAC_34), GetResString(IDS_SD_PROMPT_VAC_39) );
			str += strTemp;
		}
	}
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( str );
	theApp.DisplayMsg(str);
}

void CDlgImpPanelDebug::OnBUTTONResetSubBoards() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
	theApp.DisplayMsg("Press reset SubBoards");
}

void CDlgImpPanelDebug::OnBUTTONResetFT232() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press reset FT232");
	CScanView* pView = (CScanView*)(theApp.GetScanView());
	if( pView != NULL )
	{
		// 保存现场
		USB_ScanType nScanType = CScanParameterManager::Instance().m_nusbScanType;
		// 停止扫描，包括后台扫描
		pView->USB_ChangeScan( USB_ST_Stop );
		CSemCtrl::Instance().USB_WriteToBoard( USB_StopScan, FALSE, 0 );
		// 暂停真空状态机
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		// 重新寻找并连接FT232接口
		CSemCtrl::Instance().USB_ReinitCommPort();
		// 重新开启真空状态机
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
		// 恢复现场
		pView->USB_ChangeScan( nScanType );
	}
}

void CDlgImpPanelDebug::OnBUTTONResetDI16A() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press reset DI16A");
	CScanView* pView = (CScanView*)(theApp.GetScanView());
	if( pView != NULL )
	{
		// 保存现场
		USB_ScanType nScanType = CScanParameterManager::Instance().m_nusbScanType;
		// 停止扫描，包括后台扫描
		pView->USB_ChangeScan( USB_ST_Stop );
		CSemCtrl::Instance().USB_WriteToBoard( USB_StopScan, FALSE, 0 );
		// 重新寻找并连接DI-16A接口
		CSemCtrl::Instance().USB_ReinitImagePort();
		// 恢复现场
		pView->USB_ChangeScan( nScanType );
	}
}



void CDlgImpPanelDebug::OnCHECKDrawCircle() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_DrawCircle)))->GetCheck();
	CScanView* pView = (CScanView*)(theApp.GetScanView());
	if( pView != NULL )
	{
		pView->m_bDrawCircle = (BOOL)nCheck;
		if ( theApp.m_boolShowUScaleXY )
			CSemVirtualMCICard::Instance().GraphDrawRectEx( TRUE, pView->m_rcUScaleAreaLast );
	}
}


/////////////////////////////////////////////////////////////////////
// 调试状态开关
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebug::OnBUTTONSetupStatus() 
{
	// TODO: Add your control notification handler code here
	BOOL bDebugHV		= (BOOL)(((CButton*)(GetDlgItem(IDC_CHECK_Debug_HV)))->GetCheck());
	BOOL bDebugVac;
	if( GetDlgItem(IDC_CHECK_Debug_Vacuum)->IsWindowVisible() )
		bDebugVac		= (BOOL)(((CButton*)(GetDlgItem(IDC_CHECK_Debug_Vacuum)))->GetCheck());
	else
		bDebugVac		= TRUE;
	BOOL bDebugStage	= (BOOL)(((CButton*)(GetDlgItem(IDC_CHECK_DEBUG_AutoStage)))->GetCheck());
	((CMainFrame *)theApp.GetMainWnd())->SetupDebugParam( bDebugHV, TRUE, bDebugVac, bDebugStage, 1000, -1 );
	SetupDebugParam( bDebugVac );
}




/////////////////////////////////////////////////////////////////////
// Vacuum
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebug::SetupDebugParam( BOOL bDebugVac )
{
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		if( bDebugVac )
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_SwitchMode, byte_PLCvacBRDopr_ModeManual );
		else
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_SwitchMode, byte_PLCvacBRDopr_ModeAuto );
		GetDlgItem(IDC_CHECK_Debug_V1)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_V2)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_V3)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_V4)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_TMP)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_MP)->EnableWindow(bDebugVac);
		GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( bDebugVac );
		GetDlgItem( IDC_BUTTON_PLCvacBrd_Thresholds_Set )->EnableWindow( bDebugVac );
		GetDlgItem( IDC_BUTTON_PLCvacBrd_Thresholds_Get )->EnableWindow( bDebugVac );
	}
	else if( CSemCtrl::Instance().m_nVACBoardType < 1 )
	{
		GetDlgItem(IDC_CHECK_Debug_V1)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_V2)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_V3)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_V4)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Debug_TMP)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Vacuum_TCf)->EnableWindow(bDebugVac);
		GetDlgItem(IDC_CHECK_Vacuum_TCc)->EnableWindow(bDebugVac);
		GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( bDebugVac );
		// 调试模式/未连下位机
		CSMvac::Instance().SetParams(20, bDebugVac);
	}
}

void CDlgImpPanelDebug::OnCHECKDebugV2() 
{
	// 前级阀
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_V2)))->GetCheck();
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		if( nCheck )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_V2, byte_PLCvacBRDopr_Open );
			theApp.DisplayMsg("PLC: press manual V2 open");
		}
		else
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_V2, byte_PLCvacBRDopr_Close );
			theApp.DisplayMsg("PLC: press manual V2 close");
		}
	}
	else
	{
		if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
		{
			if( nCheck )
			{
				CSMvac::Instance().SetParams( VACIndex_MP, 1 );
				CSMvac::Instance().SetParams( VACIndex_Vf, 1 );
			}
			else
				CSMvac::Instance().SetParams( VACIndex_Vf, 0 );
		}
		else
		{
			CVACDataManager::Instance().m_VacParamMain.bMP = (BOOL)nCheck;
			CVACDataManager::Instance().m_VacParamMain.bVf = (BOOL)nCheck;
			VAC_SyncStates( VACIndex_Vf );
		}
	}
}

void CDlgImpPanelDebug::OnCHECKDebugV3() 
{
	// 枪阀
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_V3)))->GetCheck();
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		if( nCheck )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_V3, byte_PLCvacBRDopr_Open );
			theApp.DisplayMsg("PLC: press manual V3 open");
		}
		else
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_V3, byte_PLCvacBRDopr_Close );
			theApp.DisplayMsg("PLC: press manual V3 close");
		}
	}
	else
	{
		if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
		{
			if( nCheck )
				CSMvac::Instance().SetParams( VACIndex_Vc, 1 );
			else
				CSMvac::Instance().SetParams( VACIndex_Vc, 0 );
		}
		else
		{
			CVACDataManager::Instance().m_VacParamMain.bVc = (BOOL)nCheck;
			VAC_SyncStates( VACIndex_Vc );
		}
	}
}

void CDlgImpPanelDebug::OnCHECKDebugTMP() 
{
	// 分子泵
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_TMP)))->GetCheck();
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		if( nCheck )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_TMP, byte_PLCvacBRDopr_Open );
			theApp.DisplayMsg("PLC: press manual TMP open");
		}
		else
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_TMP, byte_PLCvacBRDopr_Close );
			theApp.DisplayMsg("PLC: press manual TMP close");
		}
	}
	else
	{
		if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
		{
			if( nCheck )
				CSMvac::Instance().SetParams( VACIndex_TMP, 1 );
			else
				CSMvac::Instance().SetParams( VACIndex_TMP, 0 );
		}
		else
		{
			CVACDataManager::Instance().m_VacParamMain.bTMP = (BOOL)nCheck;
			VAC_SyncStates( VACIndex_TMP );
		}
	}
}

void CDlgImpPanelDebug::OnCHECKDebugMP() 
{
	// 机械泵
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_MP)))->GetCheck();
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		if( nCheck )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_MP, byte_PLCvacBRDopr_Open );
			theApp.DisplayMsg("PLC: press manual MP open");
		}
		else
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_MP, byte_PLCvacBRDopr_Close );
			theApp.DisplayMsg("PLC: press manual MP close");
		}
	}
}

void CDlgImpPanelDebug::OnCHECKDebugV4() 
{
	// 放气阀
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_V4)))->GetCheck();
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
	{
		if( nCheck )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_V4, byte_PLCvacBRDopr_Open );
			theApp.DisplayMsg("PLC: press manual V4 open");
		}
		else
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_V4, byte_PLCvacBRDopr_Close );
			theApp.DisplayMsg("PLC: press manual V4 close");
		}
	}
	else
	{
		if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
		{
			if( nCheck )
				CSMvac::Instance().SetParams( VACIndex_Va, 1 );
			else
				CSMvac::Instance().SetParams( VACIndex_Va, 0 );
		}
		else
		{
			CVACDataManager::Instance().m_VacParamMain.bVa = (BOOL)nCheck;
			VAC_SyncStates( VACIndex_Va );
		}
	}
}

void CDlgImpPanelDebug::OnCHECKDebugV1() 
{
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_V1)))->GetCheck();
	if( nCheck )
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
		if( CSemCtrl::Instance().m_nVACBoardType > 20 )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_V1, byte_PLCvacBRDopr_Open );
			theApp.DisplayMsg("PLC: press manual V1 open");
		}
		else
		{
			if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
				CSMvac::Instance().SetParams( VACIndex_V1, 1 );
			else
			{
				CVACDataManager::Instance().m_VacParamMain.bV1 = TRUE;
				VAC_SyncStates( VACIndex_V1 );
			}
		}
	}
	else
	{
		if( CSemCtrl::Instance().m_nVACBoardType > 20 )
		{
			CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_Manual_V1, byte_PLCvacBRDopr_Close );
			theApp.DisplayMsg("PLC: press manual V1 close");
		}
		else
		{
			if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
				CSMvac::Instance().SetParams( VACIndex_V1, 0 );
			else
			{
				CVACDataManager::Instance().m_VacParamMain.bV1 = FALSE;
				VAC_SyncStates( VACIndex_V1 );
			}
		}
	}
}

void CDlgImpPanelDebug::OnCHECKVacuumTCf() 
{
	// 前级真空
	int nCheckf = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCf)))->GetCheck();
	int nCheckc = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCc)))->GetCheck();
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
	{
		if( nCheckf && nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9FAF );
		else if( nCheckf && !nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9F6F );
		else if( !nCheckf && nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9F9F );
		else
			CSMvac::Instance().SetParams( 12, 0x9F5F );
	}
	else
	{
		CVACDataManager::Instance().m_VacParamMain.bTCf = (BOOL)nCheckf;
		VAC_SyncStates( VACIndex_TCf );
	}
}

void CDlgImpPanelDebug::OnCHECKVacuumTCc() 
{
	// 镜筒真空
	int nCheckf = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCf)))->GetCheck();
	int nCheckc = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_TCc)))->GetCheck();
	if( CSMvac::Instance().isReady() &&CVACDataManager::Instance().m_VacParamMain.bPower )
	{
		if( nCheckf && nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9FAF );
		else if( nCheckf && !nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9F6F );
		else if( !nCheckf && nCheckc )
			CSMvac::Instance().SetParams( 12, 0x9F9F );
		else
			CSMvac::Instance().SetParams( 12, 0x9F5F );
	}
	else
	{
		CVACDataManager::Instance().m_VacParamMain.bTCc = (BOOL)nCheckc;
		VAC_SyncStates( VACIndex_TCc );
	}
}

void CDlgImpPanelDebug::OnCHECKVacuumPauseRead() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Vacuum_PauseRead)))->GetCheck();
	CSMvac::Instance().SetParams( 22, nCheck );
}

void CDlgImpPanelDebug::VAC_SyncStates( int nIndex )
{
	BOOL bUpdate = TRUE;
	switch( nIndex )
	{
	case	VACIndex_Vf:
	case	VACIndex_Vc:
	case	VACIndex_TMP:
	case	VACIndex_Va:
	case	VACIndex_TCf:
	case	VACIndex_TCc:
		break;
	case	VACIndex_V1:
		if(CVACDataManager::Instance().m_VacParamMain.bV1 )
			GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( TRUE );
		else
			GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( FALSE );
		break;
	default:
		bUpdate = FALSE;
		break;
	}

	if( bUpdate )
	{
		// 什么时候可以加高压
		if( theApp.m_blOverhaulState )
		{
			GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( TRUE );
			SPR_En(TRUE);
		}
		else if(CVACDataManager::Instance().CheckVACAndCCG())
		{
			GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( TRUE );
			SPR_En(TRUE);
		}
		else
		{
			GetDlgItem( IDC_BUTTON_Relay1EnCmd )->EnableWindow( FALSE );
			SPR_En(FALSE);
		}

		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm && pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
		{
			pfrm->m_wndImpPanelAll2.VAC_UpdateUIswitches();
			pfrm->m_wndImpPanelAll2.VAC_UpdatePneumaticDiagram();
		}
	}
}

void CDlgImpPanelDebug::OnSPRRelay1En() 
{
	// TODO: Add your control notification handler code here
	BOOL bEn = m_bSPRRelay1En;
	SPR_En( !bEn );
}

void CDlgImpPanelDebug::SPR_En( BOOL bEn )
{
	if( m_bSPRRelay1En == bEn )
		return;

	m_bSPRRelay1En = bEn;
	if( m_bSPRRelay1En )
	{
		m_btnSPRRelay1En.SetColor( g_crEmphasis3 );
		m_btnSPRRelay1En.SetWindowText( "J14: ON" );//"J14: Vacuum Interlock (Now ON)" );
	}
	else
	{
		m_btnSPRRelay1En.SetColor( g_crOFF );
		m_btnSPRRelay1En.SetWindowText( "J14: OFF" );//"J14: Vacuum Interlock (Now OFF)" );
	}
	
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
		pfrm->m_wndImpPanelAll2.SPR_En( m_bSPRRelay1En );
}

void CDlgImpPanelDebug::OnCHECKDebugCCGvalueLock() 
{
	// CCG是否参与高压真空联锁
	CVACDataManager::Instance().m_cfgVacCCG.bInInterlock = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_CCGvalueLock)))->GetCheck();

	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	CString str;
	str.Format( "%d", (int)(CVACDataManager::Instance().m_cfgVacCCG.bInInterlock) );
	WritePrivateProfileString( "VACParam", "InInterlock", str, path );
}

void CDlgImpPanelDebug::OnCHECKDebugCCGvacuum() 
{
	// CCG是否参与高压真空联锁
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_CCGvacuum)))->GetCheck();
// 	if( nCheck )
// 		CVACDataManager::Instance().m_cfgVacCCG.dVac = 1.0E-5;
// 	else
// 		CVACDataManager::Instance().m_cfgVacCCG.dVac = 1.0E-4;
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
	{
		CVACDataManager::Instance().m_cfgVacCCG.bReady = TRUE;
		if( nCheck == 1 )
		{
			CVACDataManager::Instance().m_cfgVacCCG.dVac = 1.0E-5;
//			pfrm->m_wndImpPanelAll2.PostMessage( WM_USER_VAC_UpdateCCGValue, 100, -5 );
			pfrm->m_wndImpPanelAll2.m_nCCG_Counts = 0;
		}
		else if( nCheck == 2 )
//			pfrm->m_wndImpPanelAll2.PostMessage( WM_USER_VAC_UpdateCCGValue, 700, -5 );
			CVACDataManager::Instance().m_cfgVacCCG.dVac	= 7.0E-5;
		else
//			pfrm->m_wndImpPanelAll2.PostMessage( WM_USER_VAC_UpdateCCGValue, 100, -4 );
			CVACDataManager::Instance().m_cfgVacCCG.dVac	= 1.0E-4;
		pfrm->m_wndImpPanelAll2.PostMessage( WM_USER_VAC_UpdateCCGValue, 0, (LPARAM)(&(CVACDataManager::Instance().m_cfgVacCCG)) );
	}
}

void CDlgImpPanelDebug::OnCHECKOff()
{
	GetDlgItem( IDC_PANEL_LOG_EDIT_Log )->SetWindowText( "0" );
	theApp.WriteProfileInt( g_csSettingSection, 
			g_csRegEntry_nFilaLifeTime, 0 );
}

void CDlgImpPanelDebug::OnSelchangeCCD() 
{
	// TODO: Add your control notification handler code here
/*	CComboBox* pProductType = (CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_EditText_Size));
	CString strType = _T("");
	pProductType->GetLBText( pProductType->GetCurSel(), strType );
	char szBuf[255];
	sprintf( szBuf, "%s", strType );
	
	CString strSubKey, strRegEntry = "SemType";
	strSubKey.Format( "Software\\KYKY" );
	HKEY hk;
   	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
		(LPCTSTR)strSubKey,
		0, 
		KEY_ALL_ACCESS,
		&hk ) == ERROR_SUCCESS )
	{
		RegSetValueEx( hk, 
			(LPCTSTR)strRegEntry,
			NULL,
			REG_SZ,
			(LPBYTE)szBuf,
			strlen(szBuf) + 1);
	}
	RegCloseKey(hk);
*/
	theApp.WriteProfileInt( g_csSettingSection, g_csRegEntry_nCCD,
			((CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_EditText_Size)))->GetCurSel() );
}

void CDlgImpPanelDebug::OnSelchangeStage() 
{
	// TODO: Add your control notification handler code here
	theApp.WriteProfileInt( g_csSettingSection, g_csRegEntry_nAutoStage,
		5 -((CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_EditText_Name)))->GetCurSel() );
}

void CDlgImpPanelDebug::OnSelchangeEDS() 
{
	// TODO: Add your control notification handler code here
	theApp.WriteProfileInt( g_csSettingSection, g_csRegEntry_nEDS,
		((CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_LabelText_Name)))->GetCurSel() );
}

void CDlgImpPanelDebug::OnSelchangeBSD() 
{
	// TODO: Add your control notification handler code here
	theApp.WriteProfileInt( g_csSettingSection, g_csRegEntry_nBSD,
		((CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_UScaleText_Name)))->GetCurSel() );
}

void CDlgImpPanelDebug::OnSelchangeHVType() 
{
	// TODO: Add your control notification handler code here
	theApp.WriteProfileInt( g_csSettingSection, "HVtype",
		((CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_LabelText_Size)))->GetCurSel() );
}

void CDlgImpPanelDebug::OnSelchangeVacType() 
{
	// TODO: Add your control notification handler code here
	int nSel = ((CComboBox*)(GetDlgItem(IDC_PAGE_SYS_Font_ComboBox_UScaleText_Size)))->GetCurSel();
	if( nSel > 0 )
		theApp.WriteProfileInt( g_csSettingSection, "VACBoardType", 22 );
	else
		theApp.WriteProfileInt( g_csSettingSection, "VACBoardType", 0 );
}

void CDlgImpPanelDebug::OnSelchangeGaugeType() 
{
	// TODO: Add your control notification handler code here
	int nSel = ((CComboBox*)(GetDlgItem(IDC_PAGE_SCAN_COMBO_SEratio)))->GetCurSel();
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	CString str;
	str.Format( "%d", nSel );
	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
		WritePrivateProfileString( "BoardParam", "GaugeType", str, path );
	else
		WritePrivateProfileString( "VACParam", "GaugeType", str, path );
}

void CDlgImpPanelDebug::OnSelchangeNAV() 
{
	// TODO: Add your control notification handler code here
	theApp.WriteProfileInt( g_csSettingSection, g_csRegEntry_nNAV,
		((CComboBox*)(GetDlgItem(IDC_PAGE_SCAN_COMBO_Chn1)))->GetCurSel() );
}

void CDlgImpPanelDebug::OnSelchangeBeam() 
{
	// TODO: Add your control notification handler code here
	theApp.WriteProfileInt( g_csSettingSection, g_csRegEntry_nPA,
		((CComboBox*)(GetDlgItem(IDC_PAGE_SCAN_COMBO_Chn2)))->GetCurSel() );
}



void CDlgImpPanelDebug::PLC_UpdateStatusValve()
{
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM )
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_V1 )))->SetCheck(1);
	else
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_V1 )))->SetCheck(0);
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V2 )
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_V2 )))->SetCheck(1);
	else
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_V2 )))->SetCheck(0);
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V3 )
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_V3 )))->SetCheck(1);
	else
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_V3 )))->SetCheck(0);
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V4 )
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_V4 )))->SetCheck(1);
	else
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_V4 )))->SetCheck(0);
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.TMP )
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_TMP )))->SetCheck(1);
	else
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_TMP )))->SetCheck(0);
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.MP )
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_MP )))->SetCheck(1);
	else
		((CButton*)(GetDlgItem( IDC_CHECK_Debug_MP )))->SetCheck(0);
}

void CDlgImpPanelDebug::OnBUTTONPLCvacBrdThresholdsGet() 
{
	CPLCvacBrdStatusDetector::Instance().SetCmd( byte_PLCvacBRDopc_InquireVACthreshold, byte_PLCvacBRDopr_Default );
	theApp.DisplayMsg("PLC: press get thresholds");
}

void CDlgImpPanelDebug::OnBUTTONPLCvacBrdThresholdsSet() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_PLCvacBrd_Thresholds_VacPre)->GetWindowText(str);
	double dVacPre = atof(str);
	if( dVacPre < 0.1 || dVacPre > 1 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "the Pre value is not in range [0.1, 1]!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	GetDlgItem(IDC_EDIT_PLCvacBrd_Thresholds_VacOK)->GetWindowText(str);
	double dVacOK = atof(str);
	if( dVacOK < 1E-7 || dVacOK > 8E-5 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "the VacOK value is not in range [1E-7, 8E-5]!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	GetDlgItem(IDC_EDIT_PLCvacBrd_Thresholds_VacNotOK)->GetWindowText(str);
	double dVacNotOK = atof(str);
	if( dVacNotOK < 1E-7 || dVacNotOK > 1E-4 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "the VacNotOK value is not in range [1E-7, 1E-4]!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	CPLCvacBrdStatusDetector::Instance().SetThresholds( dVacPre, dVacOK, dVacNotOK );
	theApp.DisplayMsg("PLC: press set thresholds");
}
