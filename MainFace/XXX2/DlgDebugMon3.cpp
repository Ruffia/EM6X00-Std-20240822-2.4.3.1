// DlgDebugMon3.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgDebugMon3.h"
#include "MainFrm.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "CCGStatusDetector.h"
#include "HVPowerDataManager.h"
#include "PLCvacBrdStatusDetector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMon3 dialog


CDlgDebugMon3::CDlgDebugMon3(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebugMon3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebugMon3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bmpON.LoadBitmap(IDB_BITMAP_LED_RED_H);
	m_bmpOFF.LoadBitmap(IDB_BITMAP_LED_RED_D);
	m_bmpERR.LoadBitmap(IDB_BITMAP_LED_ERR);
}

CDlgDebugMon3::~CDlgDebugMon3()
{
	m_bmpON.DeleteObject();
	m_bmpOFF.DeleteObject();
	m_bmpERR.DeleteObject();
	m_bmpPneumaticDiagram.DeleteObject();
	m_brushHLcyan.DeleteObject();
	m_brushHLgreen.DeleteObject();
	m_brushHLred.DeleteObject();
}


void CDlgDebugMon3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebugMon3)
	DDX_Control(pDX, IDC_STATIC_PneumaticDiagram, m_staticPneumaticDiagram);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebugMon3, CDialog)
	//{{AFX_MSG_MAP(CDlgDebugMon3)
	ON_BN_CLICKED(IDC_BUTTON_ReLink_CCG, OnReLinkCCG)
	ON_BN_CLICKED(IDC_BUTTON_ReLink_HVpower, OnReLinkHVpower)
	ON_BN_CLICKED(IDC_BUTTON_DEBUG_Reset_HVPower, OnResetHVPower)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_VAC_MSG, OnSMvacChanged)
	ON_MESSAGE(WM_USER_BoardVAC_UpdateStatus, OnBoardVACUpdateStatus)
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_UpdateMon, OnWSM30KVUpdateMon)
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_UpdateStatus, OnWSM30KVUpdateStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMon3 message handlers

void CDlgDebugMon3::Localize(void)
{
	if( m_hWnd == NULL )
		return;
	GetDlgItem(IDC_STATIC_STATUS_Vacuum_Label)->SetWindowText(GetResString(IDS_STATIC_STATUS_Vacuum_Label)); 
	GetDlgItem(IDC_STATIC_STATUS_HV_Label)->SetWindowText(GetResString(IDS_STATIC_STATUS_HV_Label)); 
	GetDlgItem(IDC_BUTTON_ReLink_HVpower)->SetWindowText(GetResString(IDS_BUTTON_ReLink_HVpower)); 
//	GetDlgItem(IDC_BUTTON_DEBUG_Reset_HVPower)->SetWindowText(GetResString(IDS_BUTTON_DEBUG_771_Reset));

}

void CDlgDebugMon3::AdjustCtrlsPos()
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
}

void CDlgDebugMon3::AdjustCtrlsPos_ValveLamp( int nID, int nLeftPD, int nTopPD, int nWidthPD, int nHeightPD )
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

CRect CDlgDebugMon3::GetControlRect( int nID )
{
	CRect rc;
	CWnd* pWnd = GetDlgItem( nID );
	pWnd->GetWindowRect( (LPRECT)rc );
	ScreenToClient( (LPRECT)rc );
	return rc;
}

BOOL CDlgDebugMon3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nVACBoardType = CSemCtrl::Instance().m_nVACBoardType;
	switch( nVACBoardType )
	{
	case	22:	// PLC真空板（无V3，有V5、CCG）
	case	21:	// PLC真空板（无V3，有V5、CCG）
		{
			m_bmpPneumaticDiagram.LoadBitmap(IDB_BITMAP_PneumaticDiagramNew);
			GetDlgItem( IDC_STATIC_Lamp_Vacuum_CCG )->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_PROGRESS_Vacuum_TCf)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PROGRESS_Vacuum_TCc)->ShowWindow(SW_HIDE);
			if( CSemCtrl::Instance().m_nVACBoardType > 21 )
				GetDlgItem( IDC_STATIC_Lamp_Vacuum_MP )->ShowWindow(SW_SHOW);
			else
				GetDlgItem( IDC_STATIC_Lamp_Vacuum_MP )->ShowWindow(SW_HIDE);
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

	if( nVACBoardType < 1 )
		GetDlgItem(IDC_BUTTON_ReLink_CCG)->SetWindowText( "CCG" );
	else
		GetDlgItem(IDC_BUTTON_ReLink_CCG)->SetWindowText( "Board" );

	VAC_ShowBrdCtrls();
	VAC_ShowPLCvacBrdCtrls();

	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
	{
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_ACL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_SCI)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_COL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_PMT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P3V325)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P5V25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P15V25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_N15V25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P24V25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_T25)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P3V325)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P5V25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P15V25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_N15V25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P24V25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_T25)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_FIL)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_BIAS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_BIAS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_BIAS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_BIASV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_BIASV)->ShowWindow(SW_HIDE);
	}

	m_brushHLcyan.CreateSolidBrush(RGB(153,217,234));
	m_brushHLred.CreateSolidBrush(RGB(255,0,0));
	m_brushHLgreen.CreateSolidBrush(RGB(181,230,29));

	AdjustCtrlsPos();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDebugMon3::VAC_ShowBrdCtrls()
{
	int nCmdShow = SW_HIDE;
	if( CSemCtrl::Instance().m_nVACBoardType > 0 && CSemCtrl::Instance().m_nVACBoardType < 10 )
		nCmdShow = SW_SHOW;

	GetDlgItem(IDC_EDIT_Status1_X)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_Status1_X)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_Status1_M)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_Status1_M)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_Value_TC3)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_Value_TC4)->ShowWindow(nCmdShow);
/*		CRect rc, rcRef;
		GetDlgItem(IDC_STATIC_Debug)->GetWindowRect(&rcRef);
		CWnd *pWnd = GetWindow(GW_CHILD);
		while(pWnd != NULL)
		{
			pWnd->GetWindowRect(&rc);
			if( PtInRect( rcRef, rc.CenterPoint() ) )
				pWnd->ShowWindow(SW_HIDE);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		};*/
}

void CDlgDebugMon3::VAC_ShowPLCvacBrdCtrls()
{
	int nCmdShow = SW_HIDE;
	if( CSemCtrl::Instance().m_nVACBoardType > 20 )
		nCmdShow = SW_SHOW;

	GetDlgItem(IDC_STATIC_PLCvacBrd_Group_TMP)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_PLCvacBrd_TMP_RotateSpeed)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_PLCvacBrd_TMP_Temperature)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_PLCvacBrd_TMP_Time)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_TMP_RotateSpeed)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_TMP_Temperature)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_TMP_Time)->ShowWindow(nCmdShow);

	GetDlgItem(IDC_EDIT_PLCvacBrd_VacStatus_XX)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_VacChambOK)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_VacErr)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_TMPErr)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_TMPFail)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_TMPLink)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_MPErr)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_CCGErr)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_Valve_CCD)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_Valve_Mode)->ShowWindow(nCmdShow);

	GetDlgItem(IDC_EDIT_PLCvacBrd_VacStatus_XX)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_XX)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_VacChambOK_Disp)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_VacErr_Disp)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_TMPErr_Disp)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_TMPFail_Disp)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_TMPLink_Disp)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_MPErr_Disp)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_VacStatus_CCGErr_Disp)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_Valve_CCD_Disp)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PLCvacBrd_Valve_Mode_Disp)->ShowWindow(nCmdShow);
}


//////////////////////////////////////////////////////////////////////////////
LRESULT CDlgDebugMon3::OnSMvacChanged(WPARAM wParam, LPARAM lParam)
{
	VAC_UpdatePneumaticDiagram();

	return 0;
}

void CDlgDebugMon3::VAC_UpdateSMvacTCAmpUpdate(int nType, double dValue)
{
	if( nType < 1 )
	{
		// 更新TCf进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCf)))->SetPos( (int)((dValue -CVACDataManager::Instance().m_VacTCAmp.dTCfMin) / (CVACDataManager::Instance().m_VacTCAmp.dTCfMax -CVACDataManager::Instance().m_VacTCAmp.dTCfMin) *100 ));
	}
	else
	{
		// 更新TCc进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCc)))->SetPos( (int)((dValue -CVACDataManager::Instance().m_VacTCAmp.dTCcMin) / (CVACDataManager::Instance().m_VacTCAmp.dTCcMax -CVACDataManager::Instance().m_VacTCAmp.dTCcMin) *100 ));
	}
}

void CDlgDebugMon3::VAC_UpdateTCAmpUpdate(int nType, double dValue)
{
	if( nType < 1 )
	{
		// 更新TCf进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCf)))->SetPos( (int)((dValue -CVACDataManager::Instance().m_thresholdsVAC.fTC3Min) / (CVACDataManager::Instance().m_thresholdsVAC.fTC3Max -CVACDataManager::Instance().m_thresholdsVAC.fTC3Min) *100 ));
	}
	else
	{
		// 更新TCc进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCc)))->SetPos( (int)((dValue -CVACDataManager::Instance().m_thresholdsVAC.fTC4Min) / (CVACDataManager::Instance().m_thresholdsVAC.fTC4Max -CVACDataManager::Instance().m_thresholdsVAC.fTC4Min) *100 ));
	}
}

void CDlgDebugMon3::UpdateStatus_BoardStatus()
{
	CString str;
	StatusVac6K status = CVACDataManager::Instance().m_statusBoard.Vac.status;

	/////////////////////////////////////////////////////
	// 分析状态
	//const	BYTE	byte_opcStatus	= 0x11;		// 返回真空状态
	// 参数1：0xXY
	//	X：	0 初始化状态；1 样品室大气状态；2 电子枪大气状态
	//		3 样品室抽真空状态；4 枪抽真空状态；
	//		5 样品室粗真空状态；6 枪粗真空状态；
	//		7 TMP运行状态；8 TMP运行错误状态
	//	Y：	0bMKQP
	//		M：1 样品室真空错误状态；0 样品室真空正常状态
	//		KQP 保留
	//	参数2：0x00

	// 分析参数1
	// 显示参数1状态
	str.Format( "%d", status.X );
	GetDlgItem(IDC_EDIT_Status1_X)->SetWindowText( str );
	if( status.X > -1 && status.X < 9 )
		str = GetResString(IDS_SD_PROMPT_NEWVAC_01 +status.X);
	else
		str = "Unknown";
	GetDlgItem(IDC_STATIC_Status1_X)->SetWindowText( str );
	if( status.YM )
	{
		GetDlgItem(IDC_EDIT_Status1_M)->SetWindowText( "1" );
		str = GetResString(IDS_SD_PROMPT_NEWVAC_10);
	}
	else
	{
		GetDlgItem(IDC_EDIT_Status1_M)->SetWindowText( "0" );
		str = GetResString(IDS_SD_PROMPT_NEWVAC_11);
	}
	GetDlgItem(IDC_STATIC_Status1_M)->SetWindowText( str );

	VAC_UpdatePneumaticDiagram();
}

void CDlgDebugMon3::UpdateStatus_BoardTCs(WPARAM wParam, LPARAM lParam)
{
	CVACDataManager::Instance().m_valueTC = *((VACvalue6K_TC*)lParam);
	CString str;
	if( wParam == byte_opcTC3 )
	{
		str.Format( "%.3f", CVACDataManager::Instance().m_valueTC.fTC3 );
		GetDlgItem(IDC_STATIC_Value_TC3)->SetWindowText(str);
		VAC_UpdateTCAmpUpdate(0, CVACDataManager::Instance().m_valueTC.fTC3);
	}
	else if( wParam == byte_opcTC4 )
	{
		str.Format( "%.3f", CVACDataManager::Instance().m_valueTC.fTC4 );
		GetDlgItem(IDC_STATIC_Value_TC4)->SetWindowText(str);
		VAC_UpdateTCAmpUpdate(1, CVACDataManager::Instance().m_valueTC.fTC4);
	}
}

LRESULT CDlgDebugMon3::OnBoardVACUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	CString str;
	if( wParam > 15 && wParam < 150 )
	{
		if( wParam == byte_opcStatus1 )
		{
			// 返回真空状态
			UpdateStatus_BoardStatus();
		}
		else if( wParam == byte_opcStatus2 )
		{
			// 返回阀门状态
			VAC_UpdatePneumaticDiagram();
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
				str = GetResString(IDS_SD_PROMPT_NEWVAC_12);
				break;
			case	0x01:
				str = GetResString(IDS_SD_PROMPT_NEWVAC_13);
				break;
			}
//			Message( str );
		}
	}
	return 0;
}


BOOL CDlgDebugMon3::VAC_UpdatePneumaticDiagram()
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

void CDlgDebugMon3::VAC_UpdatePneumaticDiagram_Old()
{
	// VAC子卡
	CStatic* pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vf );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_VacParamMain.bVf )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vc );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_VacParamMain.bVc )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V4 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_VacParamMain.bVa )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_TMP );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_VacParamMain.bTMP )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}

	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V1 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_VacParamMain.bV1 )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}
}


void CDlgDebugMon3::VAC_UpdatePneumaticDiagram_New()
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

void CDlgDebugMon3::VAC_UpdatePneumaticDiagram_PLC()
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
		else if ( CVACDataManager::Instance().m_cfgPLCvacBrd.bReady )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
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


void CDlgDebugMon3::PLC_UpdateStatusVac()
{
	CString str, strTemp;
	// 参数1：真空状态
	int nX = CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.XX;
	switch( nX )
	{
	case	0:
		strTemp = "Manual Mode";			// 手动状态
		break;
	case	1:
		strTemp = "Auto Stop";				// 自动停止状态
		break;
	case	2:
		strTemp = "Rough pumping";			// 粗抽状态
		break;
	case	3:
		strTemp = "Rough Vacuum";			// 粗真空状态
		break;
	case	4:
		strTemp = "Low Vacuum";				// 低真空状态
		break;

	case	5:
		strTemp = "TMP run start";			// TMP运行启动状态
		break;
	case	6:
		strTemp = "TMP reach normal speed";	// TMP达正常转速状态
		break;
	case	7:
		strTemp = "TMP running VacOK";		// TMP运行VacOK状态
		break;

	case	10:
		strTemp = "Start to vent";			// 准备放气状态
		break;
	case	11:
		strTemp = "Vent slowly";			// 慢速放气状态
		break;
	case	12:
		strTemp = "Vent fast";				// 快速放气状态
		break;
	case	19:
		strTemp = "Standby";				// 待机状态
		break;
	case	20:
		strTemp = "Vacuum Error";			// 真空运行错误状态
		break;
	}
	str.Format( "%d", nX );
	GetDlgItem( IDC_EDIT_PLCvacBrd_VacStatus_XX )->SetWindowText( str );
	GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_XX )->SetWindowText( strTemp );

	// 参数2
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.CCGErr )
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_CCGErr )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_CCGErr )->SetWindowText("0");
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.MPErr )
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_MPErr )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_MPErr )->SetWindowText("0");
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.TMPLink )
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_TMPLink )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_TMPLink )->SetWindowText("0");
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.TMPFail )
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_TMPFail )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_TMPFail )->SetWindowText("0");
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.TMPErr )
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_TMPErr )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_TMPErr )->SetWindowText("0");
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.VacErr )
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_VacErr )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_VacErr )->SetWindowText("0");
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusVac.status.VacChambOK )
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_VacChambOK )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_VacStatus_VacChambOK )->SetWindowText("0");
}

void CDlgDebugMon3::PLC_UpdateStatusValve()
{
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.CCD )
		GetDlgItem( IDC_STATIC_PLCvacBrd_Valve_CCD )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_Valve_CCD )->SetWindowText("0");
	if( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.mode )
		GetDlgItem( IDC_STATIC_PLCvacBrd_Valve_Mode )->SetWindowText("1");
	else
		GetDlgItem( IDC_STATIC_PLCvacBrd_Valve_Mode )->SetWindowText("0");
}
/*
void CDlgDebugMon3::PLC_UpdateThresholds()
{
	char szBuf[50] = {0};
	sprintf_s( szBuf, "%.1e", m_cfgPLCvacBrd.dThreshold_PreVac );
	GetDlgItem( IDC_EDIT_PLCvacBrd_Thresholds_VacPre_Read )->SetWindowText(szBuf);
	sprintf_s( szBuf, "%.1e", m_cfgPLCvacBrd.dThreshold_VacOK );
	GetDlgItem( IDC_EDIT_PLCvacBrd_Thresholds_VacOK_Read )->SetWindowText(szBuf);
	sprintf_s( szBuf, "%.1e", m_cfgPLCvacBrd.dThreshold_VacNotOK );
	GetDlgItem( IDC_EDIT_PLCvacBrd_Thresholds_VacNotOK_Read )->SetWindowText(szBuf);
	sprintf_s( szBuf, "Get Thresholds: %.1e, %.1e, %.1e", m_cfgPLCvacBrd.dThreshold_PreVac,
					m_cfgPLCvacBrd.dThreshold_VacOK, m_cfgPLCvacBrd.dThreshold_VacNotOK );
	Message(szBuf);
}
*/
void CDlgDebugMon3::PLC_UpdateStatus( BYTE bUpdate )
{
	if( (bUpdate & 0x01) == 0x01 )
		PLC_UpdateStatusVac();
	if( (bUpdate & 0x10) == 0x10 )
	{
		PLC_UpdateStatusValve();
		VAC_UpdatePneumaticDiagram_PLC();
	}

	char szBuf[50] = {0};
	CVACDataManager::Instance().m_StatusPLCvacBrd.tmp = CVACDataManager::Instance().m_StatusPLCvacBrdBak.tmp;
	CVACDataManager::Instance().m_StatusPLCvacBrd.dPccg = CVACDataManager::Instance().m_StatusPLCvacBrdBak.dPccg;
	sprintf_s( szBuf, "%ld", CVACDataManager::Instance().m_StatusPLCvacBrd.tmp.wR );
	GetDlgItem(IDC_EDIT_PLCvacBrd_TMP_RotateSpeed)->SetWindowText(szBuf);
	sprintf_s( szBuf, "%ld", CVACDataManager::Instance().m_StatusPLCvacBrd.tmp.wD );
	GetDlgItem(IDC_EDIT_PLCvacBrd_TMP_Temperature)->SetWindowText(szBuf);
	sprintf_s( szBuf, "%ld", CVACDataManager::Instance().m_StatusPLCvacBrd.tmp.wT );
	GetDlgItem(IDC_EDIT_PLCvacBrd_TMP_Time)->SetWindowText(szBuf);
// 	sprintf_s( szBuf, "%.2e", CVACDataManager::Instance().m_StatusPLCvacBrd.dPccg );
// 	GetDlgItem(IDC_EDIT_PLCvacBrd_CCG)->SetWindowText(szBuf);
}

void CDlgDebugMon3::OnReLinkCCG() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
	{
		switch( CSemCtrl::Instance().m_nVACBoardType )
		{
		case	22:	// PLC
		case	21:	// PLC
			{
				theApp.DisplayMsg("Press PLC Board: relink");
				CPLCvacBrdStatusDetector::Instance().Release();
				Sleep(1000);
				ZeroMemory( &(CVACDataManager::Instance().m_cfgPLCvacBrd), sizeof(Cfg_PLCvacBrd) );
				ZeroMemory( &(CVACDataManager::Instance().m_StatusPLCvacBrd), sizeof(Status_PLCvacBrd) );
				CPLCvacBrdStatusDetector::Instance().Init( pfrm->m_wndImpPanelAll2.m_hWnd, CVACDataManager::Instance().m_cfgVacCCG.nType -3 );
				CPLCvacBrdStatusDetector::Instance().Start();
				pfrm->m_wndImpPanelAll2.GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
			}
			break;
		case	0:	// Old
			{
				theApp.DisplayMsg("Press CCG: relink");
				int nPort = CVACDataManager::Instance().m_cfgVacCCG.nPort;
				CCCGStatusDetector::Instance().Release();

				CVACDataManager::Instance().m_nCCGconnects = 0;
				CVACDataManager::Instance().m_VacTCAmp.nState = 0;
				Sleep(1000);
				pfrm->m_wndImpPanelAll2.GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
				CVACDataManager::Instance().m_cfgVacCCG.nPort = nPort;

				CCCGStatusDetector::Instance().Start();
			}
			break;
		default:
			{
				theApp.DisplayMsg("Press Board: relink");
				int nPort = CVACDataManager::Instance().m_portBoard.nPort;
				CVACDataManager::Instance().m_BoardVAC.Close(0);
				Sleep(1000);
				pfrm->m_wndImpPanelAll2.GetDlgItem( IDC_STATIC_STATUS_Vacuum_Unit )->SetWindowText( "" );
				CVACDataManager::Instance().m_portBoard.nPort = nPort;
				CVACDataManager::Instance().m_BoardVAC.SetParam( 4, CVACDataManager::Instance().m_portBoard.nPort );
				CVACDataManager::Instance().m_BoardVAC.InitHWND( pfrm->m_wndImpPanelAll2.m_hWnd );
				CVACDataManager::Instance().m_BoardVAC.Init(0);
			}
			break;
		}
	}
}

void CDlgDebugMon3::OnReLinkHVpower() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press HV Power: relink");
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
		pfrm->m_wndImpPanelAll2.HVPower_Init( TRUE );
}

void CDlgDebugMon3::OnResetHVPower()
{
	theApp.DisplayMsg( "Press Clear HV Power WSM" );
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		theApp.m_WSM30KV2.SetParam( 38, 1 );
	else
		theApp.m_WSM30KV.SetParam( 37, 1 );
}

HBRUSH CDlgDebugMon3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		int nID = pWnd->GetDlgCtrlID();
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		{
			Display_Status_INT2( pDC, hbr, nID );
			if( nID > IDC_STATIC_DEBUG_WSM30KV_EN_SCI -1 && nID < IDC_STATIC_DEBUG_WSM30KV_EN_BIAS +1 )
				Display_Status_Output2( pDC, hbr, nID );
			else if( nID > IDC_STATIC_DEBUG_WSM30KV_OB_P3V301 -1 && nID < IDC_STATIC_DEBUG_WSM30KV_OB_T01 +1 )
				Display_Status_Protect2( pDC, hbr, nID );
			else if( nID > IDC_STATIC_DEBUG_WSM30KV_OV_SCI -1 && nID < IDC_STATIC_DEBUG_WSM30KV_OV_BIAS +1 )
				Display_Status_ProtectOV2( pDC, hbr, nID );
			else if( nID > IDC_STATIC_DEBUG_WSM30KV_OC_SCI -1 && nID < IDC_STATIC_DEBUG_WSM30KV_OC_PMT +1 )
				Display_Status_ProtectOC2( pDC, hbr, nID );
			else if( nID == IDC_STATIC_DEBUG_WSM30KV_OA_ACL )
			{
				if( theApp.m_statusWSM30KV2.statusA.status.ACL )
				{
					pDC->SetTextColor(RGB(255,255,255));
					hbr = (HBRUSH)m_brushHLred;
				}
				else
					pDC->SetTextColor(RGB(0,0,0));
			}
		}
		else
		{
			if( nID > IDC_STATIC_DEBUG_WSM30KV_INT_SCI -1 && nID < IDC_STATIC_DEBUG_WSM30KV_INT_PMT +1 )
				Display_Status_INT( pDC, hbr, nID );
			else if( nID > IDC_STATIC_DEBUG_WSM30KV_EN_SCI -1 && nID < IDC_STATIC_DEBUG_WSM30KV_EN_PMT +1 )
				Display_Status_Output( pDC, hbr, nID );
			else if( nID > IDC_STATIC_DEBUG_WSM30KV_OB_P3V301 -1 && nID < IDC_STATIC_DEBUG_WSM30KV_OB_T01 +1 )
				Display_Status_Protect01( pDC, hbr, nID );
			else if( nID > IDC_STATIC_DEBUG_WSM30KV_OB_P3V325 -1 && nID < IDC_STATIC_DEBUG_WSM30KV_OB_T25 +1 )
				Display_Status_Protect25( pDC, hbr, nID );
			else if( nID > IDC_STATIC_DEBUG_WSM30KV_OV_SCI -1 && nID < IDC_STATIC_DEBUG_WSM30KV_OV_PMT +1 )
				Display_Status_ProtectOV( pDC, hbr, nID );
			else if( nID > IDC_STATIC_DEBUG_WSM30KV_OC_SCI -1 && nID < IDC_STATIC_DEBUG_WSM30KV_OC_PMT +1 )
				Display_Status_ProtectOC( pDC, hbr, nID );
			else if( nID == IDC_STATIC_DEBUG_WSM30KV_OA_ACL )
			{
				if( theApp.m_statusWSM30KV.statusA25.status.ACL )
				{
					pDC->SetTextColor(RGB(255,255,255));
					hbr = (HBRUSH)m_brushHLred;
				}
				else
					pDC->SetTextColor(RGB(0,0,0));
			}
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgDebugMon3::Display_Status_INT( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_INT_SCI:
		{
			if( theApp.m_statusWSM30KV.statusINT01.status.SCI )
				hbr = (HBRUSH)m_brushHLcyan;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_INT_ACL:
		{
			if( theApp.m_statusWSM30KV.statusINT01.status.ACL_FIL )
				hbr = (HBRUSH)m_brushHLcyan;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_INT_COL:
		{
			if( theApp.m_statusWSM30KV.statusINT01.status.COL )
				hbr = (HBRUSH)m_brushHLcyan;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_INT_PMT:
		{
			if( theApp.m_statusWSM30KV.statusINT01.status.PMT )
				hbr = (HBRUSH)m_brushHLcyan;
		}
		break;
	}
}

void CDlgDebugMon3::Display_Status_Output( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_EN_SCI:
		{
			if( theApp.m_statusWSM30KV.statusO01.status.SCI )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_ACL:
		{
			if( theApp.m_statusWSM30KV.statusO01.status.ACL_FIL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_EMS:
		{
			if( theApp.m_statusWSM30KV.statusO01.status.EMS )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_FIL:
		{
			if( theApp.m_statusWSM30KV.statusO25.status.ACL_FIL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_COL:
		{
			if( theApp.m_statusWSM30KV.statusO01.status.COL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_PMT:
		{
			if( theApp.m_statusWSM30KV.statusO01.status.PMT )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	}
}
void CDlgDebugMon3::Display_Status_Protect01( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P3V301:
		{
			if( theApp.m_statusWSM30KV.statusB01.status.P3V3 )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P5V01:
		{
			if( theApp.m_statusWSM30KV.statusB01.status.P5V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P15V01:
		{
			if( theApp.m_statusWSM30KV.statusB01.status.P15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_N15V01:
		{
			if( theApp.m_statusWSM30KV.statusB01.status.N15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P24V01:
		{
			if( theApp.m_statusWSM30KV.statusB01.status.P24V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_T01:
		{
			if( theApp.m_statusWSM30KV.statusB01.status.Temp )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CDlgDebugMon3::Display_Status_Protect25( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P3V325:
		{
			if( theApp.m_statusWSM30KV.statusB25.status.P3V3 )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P5V25:
		{
			if( theApp.m_statusWSM30KV.statusB25.status.P5V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P15V25:
		{
			if( theApp.m_statusWSM30KV.statusB25.status.P15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_N15V25:
		{
			if( theApp.m_statusWSM30KV.statusB25.status.N15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P24V25:
		{
			if( theApp.m_statusWSM30KV.statusB25.status.P24V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_T25:
		{
			if( theApp.m_statusWSM30KV.statusB25.status.Temp )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CDlgDebugMon3::Display_Status_ProtectOV( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_OV_SCI:
		{
			if( theApp.m_statusWSM30KV.statusV01.status.SCI )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OV_ACL:
		{
			if( theApp.m_statusWSM30KV.statusV01.status.ACL_FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OV_FIL:
		{
			if( theApp.m_statusWSM30KV.statusV25.status.ACL_FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OV_COL:
		{
			if( theApp.m_statusWSM30KV.statusV01.status.COL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OV_PMT:
		{
			if( theApp.m_statusWSM30KV.statusV01.status.PMT )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CDlgDebugMon3::Display_Status_ProtectOC( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_OC_SCI:
		{
			if( theApp.m_statusWSM30KV.statusI01.status.SCI )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OC_ACL:
		{
			if( theApp.m_statusWSM30KV.statusI01.status.ACL_FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OC_FIL:
		{
			if( theApp.m_statusWSM30KV.statusI25.status.ACL_FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OC_COL:
		{
			if( theApp.m_statusWSM30KV.statusI01.status.COL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OC_PMT:
		{
			if( theApp.m_statusWSM30KV.statusI01.status.PMT )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

LRESULT CDlgDebugMon3::OnWSM30KVUpdateMon(WPARAM wParam, LPARAM lParam)
{
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
	{
		switch( wParam )
		{
		case	AddrWSM30KV2R_FB_P3V3:
			UpdateMon_VT2( wParam, lParam );
			break;
		case	AddrWSM30KV2R_FB_BIAS_V:
			UpdateMon_FB2( wParam, lParam );
			break;
		case	AddrWSM30KV2R_Given_BIAS_V:
			UpdateMon_Given2( wParam, lParam );
			break;
		}
	}
	else
	{
/*		if( wParam > AddrWSM30KVR01_FB_SCI_V -1 && wParam < AddrWSM30KVR01_FB_P3V3 )
		{
			UpdateMon_FB( wParam, lParam );
		}
		else if( wParam > AddrWSM30KVR01_Given_SCI_V -1 && wParam < AddrWSM30KVR01_InquireStatus1 )
		{
			UpdateMon_Given( wParam, lParam );
		}*/
		switch( wParam )
		{
		case	AddrWSM30KVR01_FB_P3V3:
			UpdateMon_VT( wParam, lParam );
			break;
		case	AddrWSM30KVR01_FB_SCI_V:
			UpdateMon_FB( wParam, lParam );
			break;
		case	AddrWSM30KVR01_Given_SCI_V:
			UpdateMon_Given( wParam, lParam );
			break;
		}
	}
	return 0;
}

void CDlgDebugMon3::UpdateMon_VT( WPARAM wParam, LPARAM lParam )
{
	CString str;
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fP3V3[0] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P3V301)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fP5V[0] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P5V01)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fP15V[0] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P15V01)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fN15V[0] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_N15V01)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fP24V[0] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P24V01)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fTemp[0] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_T01)->SetWindowText(str);

	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fP3V3[1] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P3V325)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fP5V[1] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P5V25)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fP15V[1] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P15V25)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fN15V[1] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_N15V25)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fP24V[1] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P24V25)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monVT.fTemp[1] );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_T25)->SetWindowText(str);
}

void CDlgDebugMon3::UpdateMon_FB( WPARAM wParam , LPARAM lParam )
{
	CString str;
	str.Format( "%.3f", theApp.m_monWSM30KV.monFB.fSCIv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_SCI)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monFB.fCOLv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_COL)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monFB.fACLv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_ACLV)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monFB.fACLi );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_ACLI)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monFB.fPMTv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_PMT)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monFB.fFILi );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_FIL)->SetWindowText(str);
}

void CDlgDebugMon3::UpdateMon_Given( WPARAM wParam, LPARAM lParam )
{
	CString str;
	str.Format( "%.3f", theApp.m_monWSM30KV.monOP.fSCIv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_SCI)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monOP.fCOLv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_COL)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monOP.fACLv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_ACLV)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monOP.fACLi );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_ACLI)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monOP.fPMTv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_PMT)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV.monOP.fFILi );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_FIL)->SetWindowText(str);
}

LRESULT CDlgDebugMon3::OnWSM30KVUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
	{
		switch( wParam )
		{
		case	AddrWSM30KV2R_OutputStatus1:
		case	AddrWSM30KV2R_ProtectStatusB:
		case	AddrWSM30KV2R_ProtectStatusV:
		case	AddrWSM30KV2R_ProtectStatusI:
		case	AddrWSM30KV2R_ProtectStatusA:
			UpdateStatus2( wParam, lParam );
			break;
		}
	}
	else
	{
		switch( wParam )
		{
		case	AddrWSM30KVR01_INTStatus:
		case	AddrWSM30KVR01_OutputStatus1:
		case	AddrWSM30KVR01_ProtectStatusB:
		case	AddrWSM30KVR01_ProtectStatusI:
		case	AddrWSM30KVR01_ProtectStatusV:
			UpdateStatus_01( wParam, lParam );
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_INTStatus:
		case	byteWSM30KV_Offset +AddrWSM30KVR25_OutputStatus1:
		case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusB:
		case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusI:
		case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusV:
		case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusArc:
			UpdateStatus_25( wParam, lParam );
			break;
/*		case	AddrWSM30KVR01_InquireStatus1:
		case	AddrWSM30KVR01_InquireStatus2:
		case	AddrWSM30KVR01_InquireStatus3:
			UpdateInquire_01( wParam, lParam );
			break;
		case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus1:
		case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus2:
		case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus3:
			UpdateInquire_25( wParam, lParam );
			break;
*/		}
	}
	return 0;
}

void CDlgDebugMon3::UpdateStatus_01( WPARAM wParam, LPARAM lParam )
{
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR01_INTStatus:
		if( theApp.m_statusWSM30KV.statusINT01.status.SCI )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_SCI)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_SCI)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusINT01.status.ACL_FIL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_ACL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_ACL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusINT01.status.COL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_COL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_COL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusINT01.status.PMT )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_PMT)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT_PMT)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_OutputStatus1:
		if( theApp.m_statusWSM30KV.statusO01.status.SCI )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_SCI)->SetWindowText("1");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_SCI)))->SetCheck(1);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_SCI)->SetWindowText("0");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_SCI)))->SetCheck(0);
		}
		if( theApp.m_statusWSM30KV.statusO01.status.ACL_FIL )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_ACL)->SetWindowText("1");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->SetCheck(1);
//			m_scrollACLv.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_ACL)->SetWindowText("0");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->SetCheck(0);
//			m_scrollACLv.EnableWindow(FALSE);
		}
		if( theApp.m_statusWSM30KV.statusO01.status.EMS )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_EMS)->SetWindowText("1");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_EMS)))->SetCheck(1);
//			m_scrollACLi.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_EMS)->SetWindowText("0");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_EMS)))->SetCheck(0);
//			m_scrollACLi.EnableWindow(FALSE);
		}
		if( theApp.m_statusWSM30KV.statusO01.status.COL )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_COL)->SetWindowText("1");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_COL)))->SetCheck(1);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_COL)->SetWindowText("0");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_COL)))->SetCheck(0);
		}
		if( theApp.m_statusWSM30KV.statusO01.status.PMT )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_PMT)->SetWindowText("1");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_PMT)))->SetCheck(1);
//			m_scrollPMTv.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_PMT)->SetWindowText("0");
//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_PMT)))->SetCheck(0);
//			m_scrollPMTv.EnableWindow(FALSE);
		}
		break;
	case	AddrWSM30KVR01_ProtectStatusB:
		if( theApp.m_statusWSM30KV.statusB01.status.P3V3 )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P3V301)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P3V301)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB01.status.P5V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P5V01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P5V01)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB01.status.P15V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P15V01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P15V01)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB01.status.N15V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_N15V01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_N15V01)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB01.status.P24V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P24V01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P24V01)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB01.status.Temp )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_T01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_T01)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_ProtectStatusV:
		if( theApp.m_statusWSM30KV.statusV01.status.SCI )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_SCI)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_SCI)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusV01.status.ACL_FIL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_ACL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_ACL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusV01.status.COL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_COL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_COL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusV01.status.PMT )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_PMT)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_PMT)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_ProtectStatusI:
		if( theApp.m_statusWSM30KV.statusI01.status.SCI )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_SCI)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_SCI)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusI01.status.ACL_FIL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_ACL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_ACL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusI01.status.COL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_COL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_COL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusI01.status.PMT )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_PMT)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_PMT)->SetWindowText("0");
		break;
	}
}

void CDlgDebugMon3::UpdateStatus_25( WPARAM wParam, LPARAM lParam )
{
	wParam -= byteWSM30KV_Offset;
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR25_OutputStatus1:
		if( theApp.m_statusWSM30KV.statusO25.status.ACL_FIL )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_FIL)->SetWindowText("1");
// 			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(1);
// 			m_scrollFILi.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_FIL)->SetWindowText("0");
// 			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(0);
// 			m_scrollFILi.EnableWindow(FALSE);
		}
		break;
	case	AddrWSM30KVR25_ProtectStatusB:
		if( theApp.m_statusWSM30KV.statusB25.status.P3V3 )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P3V325)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P3V325)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB25.status.P5V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P5V25)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P5V25)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB25.status.P15V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P15V25)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P15V25)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB25.status.N15V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_N15V25)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_N15V25)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB25.status.P24V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P24V25)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P24V25)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.statusB25.status.Temp )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_T25)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_T25)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_ProtectStatusV:
		if( theApp.m_statusWSM30KV.statusV25.status.ACL_FIL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_FIL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_FIL)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_ProtectStatusI:
		if( theApp.m_statusWSM30KV.statusI25.status.ACL_FIL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_FIL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_FIL)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_ProtectStatusArc:
		if( theApp.m_statusWSM30KV.statusA25.status.ACL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OA_ACL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OA_ACL)->SetWindowText("0");
		break;
	}
}
/*
void CDlgDebugMon3::UpdateInquire_01( WPARAM wParam, LPARAM lParam )
{
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR01_InquireStatus1:
		if( theApp.m_statusWSM30KV.status101.status.SCI )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit0)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.status101.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit1)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.status101.status.EMS )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit2)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit2)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.status101.status.COL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit4)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit4)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.status101.status.PMT )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit5)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit5)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_InquireStatus2:
		str.Format( "%04X", theApp.m_statusWSM30KV.status201.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_01)->SetWindowText(str);
		if( theApp.m_statusWSM30KV.status201.status.SCI )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit0)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.status201.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit1)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.status201.status.EMS )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit2)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit2)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.status201.status.COL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit4)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit4)->SetWindowText("0");
		if( theApp.m_statusWSM30KV.status201.status.PMT )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit5)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit5)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_InquireStatus3:
		str.Format( "%04X", theApp.m_statusWSM30KV.status301.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_01)->SetWindowText(str);
		if( theApp.m_statusWSM30KV.status301.status.Reset )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_Bit1)->SetWindowText("0");
		break;
	}
}

void CDlgDebugMon3::UpdateInquire_25( WPARAM wParam, LPARAM lParam )
{
	wParam -= byteWSM30KV_Offset;
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR25_InquireStatus1:
		str.Format( "%04X", theApp.m_statusWSM30KV.status125.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_25)->SetWindowText(str);
		if( theApp.m_statusWSM30KV.status125.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit3)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_InquireStatus2:
		str.Format( "%04X", theApp.m_statusWSM30KV.status225.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_25)->SetWindowText(str);
		if( theApp.m_statusWSM30KV.status225.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit3)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_InquireStatus3:
		str.Format( "%04X", theApp.m_statusWSM30KV.status325.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_25)->SetWindowText(str);
		if( theApp.m_statusWSM30KV.status325.status.Reset )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_Bit0)->SetWindowText("0");
		break;
	}
}
*/

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void CDlgDebugMon3::Display_Status_INT2( CDC* pDC, HBRUSH& hbr, int nID )
{
	if( nID == IDC_STATIC_DEBUG_WSM30KV_INT )
	{
		if( theApp.m_statusWSM30KV2.statusO.status.INTLK )
			hbr = (HBRUSH)m_brushHLcyan;
	}
}

void CDlgDebugMon3::Display_Status_Output2( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_EN_SCI:
		{
			if( theApp.m_statusWSM30KV2.statusO.status.SCI )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_ACL:
		{
			if( theApp.m_statusWSM30KV2.statusO.status.ACL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_EMS:
		{
			if( theApp.m_statusWSM30KV2.statusO.status.EMS )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_FIL:
		{
			if( theApp.m_statusWSM30KV2.statusO.status.FIL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_COL:
		{
			if( theApp.m_statusWSM30KV2.statusO.status.COL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_PMT:
		{
			if( theApp.m_statusWSM30KV2.statusO.status.PMT )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_EN_BIAS:
		{
			if( theApp.m_statusWSM30KV2.statusO.status.BIAS )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	}
}

void CDlgDebugMon3::Display_Status_Protect2( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P3V301:
		{
			if( theApp.m_statusWSM30KV2.statusB.status.P3V3 )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P5V01:
		{
			if( theApp.m_statusWSM30KV2.statusB.status.P5V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P15V01:
		{
			if( theApp.m_statusWSM30KV2.statusB.status.P15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_N15V01:
		{
			if( theApp.m_statusWSM30KV2.statusB.status.N15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_P24V01:
		{
			if( theApp.m_statusWSM30KV2.statusB.status.P24V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OB_T01:
		{
			if( theApp.m_statusWSM30KV2.statusB.status.Temp )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CDlgDebugMon3::Display_Status_ProtectOV2( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_OV_SCI:
		{
			if( theApp.m_statusWSM30KV2.statusV.status.SCI )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OV_ACL:
		{
			if( theApp.m_statusWSM30KV2.statusV.status.ACL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OV_COL:
		{
			if( theApp.m_statusWSM30KV2.statusV.status.COL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OV_PMT:
		{
			if( theApp.m_statusWSM30KV2.statusV.status.PMT )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OV_BIAS:
		{
			if( theApp.m_statusWSM30KV2.statusV.status.BIAS )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CDlgDebugMon3::Display_Status_ProtectOC2( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_STATIC_DEBUG_WSM30KV_OC_SCI:
		{
			if( theApp.m_statusWSM30KV2.statusI.status.SCI )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OC_ACL:
		{
			if( theApp.m_statusWSM30KV2.statusI.status.ACL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OC_FIL:
		{
			if( theApp.m_statusWSM30KV2.statusI.status.FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OC_COL:
		{
			if( theApp.m_statusWSM30KV2.statusI.status.COL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_STATIC_DEBUG_WSM30KV_OC_PMT:
		{
			if( theApp.m_statusWSM30KV2.statusI.status.PMT )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CDlgDebugMon3::UpdateMon_VT2( WPARAM wParam, LPARAM lParam )
{
	CString str;
	str.Format( "%.3f", theApp.m_monWSM30KV2.monVT.fP3V3 );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P3V301)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monVT.fP5V );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P5V01)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monVT.fP15V );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P15V01)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monVT.fN15V );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_N15V01)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monVT.fP24V );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_P24V01)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monVT.fTemp );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_T01)->SetWindowText(str);
}

void CDlgDebugMon3::UpdateMon_FB2( WPARAM wParam , LPARAM lParam )
{
	CString str;
	str.Format( "%.3f", theApp.m_monWSM30KV2.monFB.fSCIv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_SCI)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monFB.fCOLv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_COL)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monFB.fACLv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_ACLV)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monFB.fACLi );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_ACLI)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monFB.fPMTv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_PMT)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monFB.fFILi );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_FIL)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monFB.fBIASv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Mon_BIASV)->SetWindowText(str);
}

void CDlgDebugMon3::UpdateMon_Given2( WPARAM wParam, LPARAM lParam )
{
	CString str;
	str.Format( "%.3f", theApp.m_monWSM30KV2.monOP.fSCIv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_SCI)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monOP.fCOLv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_COL)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monOP.fACLv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_ACLV)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monOP.fACLi );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_ACLI)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monOP.fPMTv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_PMT)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monOP.fFILi );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_FIL)->SetWindowText(str);
	str.Format( "%.3f", theApp.m_monWSM30KV2.monOP.fBIASv );
	GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_Send_BIASV)->SetWindowText(str);
}

void CDlgDebugMon3::UpdateStatus2( WPARAM wParam, LPARAM lParam )
{
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR01_OutputStatus1:
		if( theApp.m_statusWSM30KV2.statusO.status.SCI )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_SCI)->SetWindowText("1");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_SCI)))->SetCheck(1);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_SCI)->SetWindowText("0");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_SCI)))->SetCheck(0);
		}
		if( theApp.m_statusWSM30KV2.statusO.status.ACL )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_ACL)->SetWindowText("1");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->SetCheck(1);
			//			m_scrollACLv.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_ACL)->SetWindowText("0");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->SetCheck(0);
			//			m_scrollACLv.EnableWindow(FALSE);
		}
		if( theApp.m_statusWSM30KV2.statusO.status.EMS )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_EMS)->SetWindowText("1");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_EMS)))->SetCheck(1);
			//			m_scrollACLi.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_EMS)->SetWindowText("0");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_EMS)))->SetCheck(0);
			//			m_scrollACLi.EnableWindow(FALSE);
		}
		if( theApp.m_statusWSM30KV2.statusO.status.COL )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_COL)->SetWindowText("1");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_COL)))->SetCheck(1);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_COL)->SetWindowText("0");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_COL)))->SetCheck(0);
		}
		if( theApp.m_statusWSM30KV2.statusO.status.PMT )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_PMT)->SetWindowText("1");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_PMT)))->SetCheck(1);
			//			m_scrollPMTv.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_PMT)->SetWindowText("0");
			//			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_PMT)))->SetCheck(0);
			//			m_scrollPMTv.EnableWindow(FALSE);
		}
		if( theApp.m_statusWSM30KV2.statusO.status.FIL )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_FIL)->SetWindowText("1");
			// 			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(1);
			// 			m_scrollFILi.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_FIL)->SetWindowText("0");
			// 			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(0);
			// 			m_scrollFILi.EnableWindow(FALSE);
		}
		if( theApp.m_statusWSM30KV2.statusO.status.BIAS )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_BIAS)->SetWindowText("1");
			// 			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(1);
			// 			m_scrollFILi.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_EN_BIAS)->SetWindowText("0");
			// 			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(0);
			// 			m_scrollFILi.EnableWindow(FALSE);
		}
		if( theApp.m_statusWSM30KV2.statusO.status.INTLK )
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT)->SetWindowText("1");
			// 			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(1);
			// 			m_scrollFILi.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_INT)->SetWindowText("0");
			// 			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(0);
			// 			m_scrollFILi.EnableWindow(FALSE);
		}
		break;
	case	AddrWSM30KVR01_ProtectStatusB:
		if( theApp.m_statusWSM30KV2.statusB.status.P3V3 )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P3V301)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P3V301)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusB.status.P5V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P5V01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P5V01)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusB.status.P15V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P15V01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P15V01)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusB.status.N15V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_N15V01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_N15V01)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusB.status.P24V )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P24V01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_P24V01)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusB.status.Temp )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_T01)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OB_T01)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_ProtectStatusV:
		if( theApp.m_statusWSM30KV2.statusV.status.SCI )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_SCI)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_SCI)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusV.status.ACL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_ACL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_ACL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusV.status.COL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_COL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_COL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusV.status.PMT )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_PMT)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_PMT)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusV.status.BIAS )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_BIAS)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OV_BIAS)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_ProtectStatusI:
		if( theApp.m_statusWSM30KV2.statusI.status.SCI )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_SCI)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_SCI)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusI.status.ACL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_ACL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_ACL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusI.status.FIL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_FIL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_FIL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusI.status.COL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_COL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_COL)->SetWindowText("0");
		if( theApp.m_statusWSM30KV2.statusI.status.PMT )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_PMT)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OC_PMT)->SetWindowText("0");
		break;
	case	AddrWSM30KV2R_ProtectStatusA:
		if( theApp.m_statusWSM30KV2.statusA.status.ACL )
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OA_ACL)->SetWindowText("1");
		else
			GetDlgItem(IDC_STATIC_DEBUG_WSM30KV_OA_ACL)->SetWindowText("0");
		break;
	}
}
