// DlgDebugMon.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgDebugMon.h"
#include "MainFrm.h"
#include "DlgPrompt.h"
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
// CDlgDebugMon dialog


CDlgDebugMon::CDlgDebugMon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebugMon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebugMon)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bmpON.LoadBitmap(IDB_BITMAP_LED_RED_H);
	m_bmpOFF.LoadBitmap(IDB_BITMAP_LED_RED_D);
	m_wVer_a = m_wVer_e = m_wVer_f = 0;

	m_hIconON = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_R));
	m_hIconOFF = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_GRAY));
	m_brushVacNotGood.CreateSolidBrush(RGB(128,128,64));
	m_brushVacGood.CreateSolidBrush(RGB(0,128,0));
}

CDlgDebugMon::~CDlgDebugMon()
{
	m_bmpON.DeleteObject();
	m_bmpOFF.DeleteObject();
	m_bmpPneumaticDiagram.DeleteObject();
	m_brushVacNotGood.DeleteObject();
	m_brushVacGood.DeleteObject();
	if( m_hIconON != NULL )
		::DestroyIcon( m_hIconON );
	if( m_hIconOFF != NULL )
		::DestroyIcon( m_hIconOFF );
}

void CDlgDebugMon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebugMon)
	DDX_Control(pDX, IDC_STATIC_PneumaticDiagram, m_staticPneumaticDiagram);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebugMon, CDialog)
	//{{AFX_MSG_MAP(CDlgDebugMon)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ReLink_CCG, OnReLinkCCG)
	ON_BN_CLICKED(IDC_BUTTON_ReLink_HVpower, OnReLinkHVpower)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_VAC_MSG, OnSMvacChanged)
	ON_MESSAGE(WM_USER_BoardVAC_UpdateStatus, OnBoardVACUpdateStatus)
	ON_MESSAGE(WM_USER_CPS_Status, OnCPS3603statusChanged)
	ON_MESSAGE(WM_USER_CPS_MSG, OnCPS3603msgChanged)
	ON_MESSAGE(WM_USER_CPS_Param, OnCPS3603paramChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMon message handlers

void CDlgDebugMon::Localize(void)
{
	if( m_hWnd == NULL )
		return;
	GetDlgItem(IDC_STATIC_STATUS_Vacuum_Label)->SetWindowText(GetResString(IDS_STATIC_STATUS_Vacuum_Label)); 
	GetDlgItem(IDC_STATIC_STATUS_HV_Label)->SetWindowText(GetResString(IDS_STATIC_STATUS_HV_Label)); 
	GetDlgItem(IDC_BUTTON_ReLink_HVpower)->SetWindowText(GetResString(IDS_BUTTON_ReLink_HVpower)); 
}

void CDlgDebugMon::AdjustCtrlsPos()
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

void CDlgDebugMon::AdjustCtrlsPos_ValveLamp( int nID, int nLeftPD, int nTopPD, int nWidthPD, int nHeightPD )
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

CRect CDlgDebugMon::GetControlRect( int nID )
{
	CRect rc;
	CWnd* pWnd = GetDlgItem( nID );
	pWnd->GetWindowRect( (LPRECT)rc );
	ScreenToClient( (LPRECT)rc );
	return rc;
}

BOOL CDlgDebugMon::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Debug_3603_Corona)))->SetRange(0,1000);

	switch( CSemCtrl::Instance().m_nVACBoardType )
	{
	case	22:	// PLC真空板（有V3，有V5、CCG）
	case	21:	// PLC真空板（有V3，有V5、CCG）
		{
			m_bmpPneumaticDiagram.LoadBitmap(IDB_BITMAP_PneumaticDiagramNew);
			GetDlgItem(IDC_PROGRESS_Vacuum_TCf)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PROGRESS_Vacuum_TCc)->ShowWindow(SW_HIDE);
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

	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
	{
		GetDlgItem(IDC_BUTTON_ReLink_CCG)->SetWindowText( "CCG" );
		
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
	else
		GetDlgItem(IDC_BUTTON_ReLink_CCG)->SetWindowText( "Board" );

	AdjustCtrlsPos();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////////
LRESULT CDlgDebugMon::OnSMvacChanged(WPARAM wParam, LPARAM lParam)
{
	VAC_UpdatePneumaticDiagram();

	return 0;
}

void CDlgDebugMon::VAC_UpdateSMvacTCAmpUpdate(int nType, double dValue)
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

void CDlgDebugMon::VAC_UpdateTCAmpUpdate(int nType, double dValue)
{
	if( nType < 1 )
	{
		// 更新TCf进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCf)))->SetPos( (int)((dValue - CVACDataManager::Instance().m_thresholdsVAC.fTC3Min) / (CVACDataManager::Instance().m_thresholdsVAC.fTC3Max - CVACDataManager::Instance().m_thresholdsVAC.fTC3Min) *100 ));
	}
	else
	{
		// 更新TCc进度条
		((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Vacuum_TCc)))->SetPos( (int)((dValue - CVACDataManager::Instance().m_thresholdsVAC.fTC4Min) / (CVACDataManager::Instance().m_thresholdsVAC.fTC4Max - CVACDataManager::Instance().m_thresholdsVAC.fTC4Min) *100 ));
	}
}

void CDlgDebugMon::UpdateStatus_BoardStatus()
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

void CDlgDebugMon::UpdateStatus_BoardTCs(WPARAM wParam, LPARAM lParam)
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

LRESULT CDlgDebugMon::OnBoardVACUpdateStatus(WPARAM wParam, LPARAM lParam)
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

BOOL CDlgDebugMon::VAC_UpdatePneumaticDiagram()
{
	// 更新气路图的各阀灯
	switch(  CSemCtrl::Instance().m_nVACBoardType )
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

void CDlgDebugMon::VAC_UpdatePneumaticDiagram_Old()
{
	// VAC子卡
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
}

void CDlgDebugMon::VAC_UpdatePneumaticDiagram_New()
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

void CDlgDebugMon::VAC_UpdatePneumaticDiagram_PLC()
{
/*	CStatic* pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_Vf );
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
*/
/*	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_CCD );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.CCD )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}
*/
/*	pLamp = (CStatic*)GetDlgItem( IDC_STATIC_Lamp_Vacuum_V1 );
	if( pLamp != NULL )
	{
		if ( CVACDataManager::Instance().m_StatusPLCvacBrd.statusValve.valve.V1_SEM )
			pLamp->SetBitmap((HBITMAP)(m_bmpON));
		else
			pLamp->SetBitmap((HBITMAP)(m_bmpOFF));
	}*/
}


LRESULT CDlgDebugMon::OnCPS3603statusChanged(WPARAM wParam, LPARAM lParam)
{
	if( wParam == 0x01 )
	{
		CString str = "no comm";
		dlgprintf(IDC_STATIC_Debug_3603_Conect, "%s : %s : %s", str, str, str);
	}
	else if( wParam == 0x02 )
	{
		CString strA, strE, strF;
		if( CHVPowerDataManager::Instance().m_CPS3603s.nConnectA )
			strA = "Connected";
		else
			strA = "no comm";
		if( CHVPowerDataManager::Instance().m_CPS3603s.nConnectE )
			strE = "Connected";
		else
			strE = "no comm";
		if( CHVPowerDataManager::Instance().m_CPS3603s.nConnectF )
			strF = "Connected";
		else
			strF = "no comm";
		dlgprintf(IDC_STATIC_Debug_3603_Conect, "%s : %s : %s", strA, strE, strF);
		dlgprintf(IDC_STATIC_Debug_3603_Ver, "%04x:%04x:%04x", CHVPowerDataManager::Instance().m_CPS3603s.nVerA, CHVPowerDataManager::Instance().m_CPS3603s.nVerE, CHVPowerDataManager::Instance().m_CPS3603s.nVerF);
	}
	return 0;
}

LRESULT CDlgDebugMon::OnCPS3603msgChanged(WPARAM wParam, LPARAM lParam)
{
	char *err, errbuf[50];
	switch( lParam )
	{
	case 0: err = "none";
		break;
	case ERR_INVALID_PORT:
		err = "Incorrect COM port number";
		break;
	case ERR_INVALID_VALUE:
		err = "Incorrect value of parameter";
		break;
	case ERR_WRITE_FAIL:
		err = "Cannot write to port";
		break;
	case ERR_READ_FAIL:
		err = "Cannot read from port";
		break;
	case ERR_READ_SUM:
		err = "Incorrect checksum";
		break;
	case ERR_INTERNAL:
		err = "Internal error";
		break;
	case ERR_ADC_OVERFLOW:
		err = "Meter overflow";
		break;
	case ERR_ADC_NEGATIVE:
		err = "Negative value from meter";
		break;
	default:
		sprintf(errbuf, "%d", (int)lParam);
		err = errbuf;
		break;
    }
//	SetDlgItemText(IDC_ERROR, err);
	return 0;
}

LRESULT CDlgDebugMon::OnCPS3603paramChanged(WPARAM wParam, LPARAM lParam)
{
	dlgprintf(IDC_STATIC_Debug_3603_Vout_E, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dVoutE);
	dlgprintf(IDC_STATIC_Debug_3603_Iout_E, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dIoutE * 1e6);

	dlgprintf(IDC_STATIC_Debug_3603_Vout_A, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dVoutA);
	dlgprintf(IDC_STATIC_Debug_3603_Iout_A, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dIoutA * 1e6);
	((CProgressCtrl*)(GetDlgItem(IDC_PROGRESS_Debug_3603_Corona)))->SetPos( (int)(CHVPowerDataManager::Instance().m_CPS3603.dCoronalevel) );

	dlgprintf(IDC_STATIC_Debug_3603_Vout_I, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dVoutF);
	dlgprintf(IDC_STATIC_Debug_3603_Iout_I, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dIoutF);
	dlgprintf(IDC_STATIC_Debug_3603_P_I, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dPf);
	dlgprintf(IDC_STATIC_Debug_3603_R_I, "%9.3lf", CHVPowerDataManager::Instance().m_CPS3603.dRf);
	dlgprintf(IDC_STATIC_Debug_3603_T_I, "%9.0lf", CHVPowerDataManager::Instance().m_CPS3603.dTf);
 
//	dlgprintf(IDC_T_CORR, "%4.1lf : %4.1lf", CHVPowerDataManager::Instance().m_CPS3603s.dTa, CHVPowerDataManager::Instance().m_CPS3603s.dThv);

	return 0;
}

// Printf to a dialog item
void CDlgDebugMon::dlgprintf( int id, char *fmt, ... )
{
	va_list argptr;
	static char str[256];
	
	va_start(argptr, fmt);
	vsprintf(str, fmt, argptr);
	SetDlgItemText(id, str);
	va_end(argptr);
}

HBRUSH CDlgDebugMon::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
/*
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		int nID = pWnd->GetDlgCtrlID();
		if( nID == IDC_STATIC_Mon_Gun )
		{
			if( CVACDataManager::Instance().m_VacTCAmp.nState < 2 )
			{
				pDC->SetTextColor(RGB(255,255,255));	// 设置文本颜色  
				pDC->SetBkMode(TRANSPARENT);			// 设置背景透明
				hbr = (HBRUSH)m_brushVacNotGood;
			}
			else
			{
				pDC->SetTextColor(RGB(255,255,255));  
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_brushVacGood;
			}
		}
	}
*/
	return hbr;
}

void CDlgDebugMon::OnReLinkCCG() 
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
		case	0:
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

void CDlgDebugMon::OnReLinkHVpower() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press HV Power: relink");
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
		pfrm->m_wndImpPanelAll2.HVPower_Init( TRUE );
}
