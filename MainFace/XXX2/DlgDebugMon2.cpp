// DlgDebugMon2.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgDebugMon2.h"
#include "MainFrm.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "CCGStatusDetector.h"
#include "SpellmanStatusDetector.h"
#include "HVPowerDataManager.h"
#include "PLCvacBrdStatusDetector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMon2 dialog


CDlgDebugMon2::CDlgDebugMon2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebugMon2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebugMon2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bmpON.LoadBitmap(IDB_BITMAP_LED_RED_H);
	m_bmpOFF.LoadBitmap(IDB_BITMAP_LED_RED_D);
	m_bmpERR.LoadBitmap(IDB_BITMAP_LED_ERR);
}

CDlgDebugMon2::~CDlgDebugMon2()
{
	m_bmpON.DeleteObject();
	m_bmpOFF.DeleteObject();
	m_bmpERR.DeleteObject();
	m_bmpPneumaticDiagram.DeleteObject();
}


void CDlgDebugMon2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebugMon2)
	DDX_Control(pDX, IDC_STATIC_PneumaticDiagram, m_staticPneumaticDiagram);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebugMon2, CDialog)
	//{{AFX_MSG_MAP(CDlgDebugMon2)
	ON_BN_CLICKED(IDC_BUTTON_ReLink_CCG, OnReLinkCCG)
	ON_BN_CLICKED(IDC_BUTTON_ReLink_HVpower, OnReLinkHVpower)
	ON_BN_CLICKED(IDC_BUTTON_DEBUG_Reset_HVPower, OnReset771)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_VAC_MSG, OnSMvacChanged)
	ON_MESSAGE(WM_USER_BoardVAC_UpdateStatus, OnBoardVACUpdateStatus)
	ON_MESSAGE(WM_USER_SPELLMAN_Status, OnSpellmanStatusChanged)
	ON_MESSAGE(WM_USER_SPELLMAN_Param, OnSpellmanParamChanged)
	ON_MESSAGE(WM_USER_SPELLMAN_Param2, OnSpellmanParam2Changed)
	ON_MESSAGE(WM_USER_SPELLMAN_Param3, OnSpellmanParam3Changed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMon2 message handlers

void CDlgDebugMon2::Localize(void)
{
	if( m_hWnd == NULL )
		return;
	GetDlgItem(IDC_STATIC_STATUS_Vacuum_Label)->SetWindowText(GetResString(IDS_STATIC_STATUS_Vacuum_Label)); 
	GetDlgItem(IDC_STATIC_STATUS_HV_Label)->SetWindowText(GetResString(IDS_STATIC_STATUS_HV_Label)); 
	GetDlgItem(IDC_BUTTON_ReLink_HVpower)->SetWindowText(GetResString(IDS_BUTTON_ReLink_HVpower)); 
//	GetDlgItem(IDC_BUTTON_DEBUG_Reset_HVPower)->SetWindowText(GetResString(IDS_BUTTON_DEBUG_771_Reset)); 
}

void CDlgDebugMon2::AdjustCtrlsPos()
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

void CDlgDebugMon2::AdjustCtrlsPos_ValveLamp( int nID, int nLeftPD, int nTopPD, int nWidthPD, int nHeightPD )
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

CRect CDlgDebugMon2::GetControlRect( int nID )
{
	CRect rc;
	CWnd* pWnd = GetDlgItem( nID );
	pWnd->GetWindowRect( (LPRECT)rc );
	ScreenToClient( (LPRECT)rc );
	return rc;
}

BOOL CDlgDebugMon2::OnInitDialog() 
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

	AdjustCtrlsPos();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDebugMon2::VAC_ShowBrdCtrls()
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

void CDlgDebugMon2::VAC_ShowPLCvacBrdCtrls()
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
LRESULT CDlgDebugMon2::OnSMvacChanged(WPARAM wParam, LPARAM lParam)
{
	VAC_UpdatePneumaticDiagram();

	return 0;
}

void CDlgDebugMon2::VAC_UpdateSMvacTCAmpUpdate(int nType, double dValue)
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

void CDlgDebugMon2::VAC_UpdateTCAmpUpdate(int nType, double dValue)
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

void CDlgDebugMon2::UpdateStatus_BoardStatus()
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

void CDlgDebugMon2::UpdateStatus_BoardTCs(WPARAM wParam, LPARAM lParam)
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

LRESULT CDlgDebugMon2::OnBoardVACUpdateStatus(WPARAM wParam, LPARAM lParam)
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

BOOL CDlgDebugMon2::VAC_UpdatePneumaticDiagram()
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

void CDlgDebugMon2::VAC_UpdatePneumaticDiagram_Old()
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


void CDlgDebugMon2::VAC_UpdatePneumaticDiagram_New()
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

void CDlgDebugMon2::VAC_UpdatePneumaticDiagram_PLC()
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

void CDlgDebugMon2::PLC_UpdateStatusVac()
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

void CDlgDebugMon2::PLC_UpdateStatusValve()
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
void CDlgDebugMon2::PLC_UpdateThresholds()
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
void CDlgDebugMon2::PLC_UpdateStatus( BYTE bUpdate )
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

void CDlgDebugMon2::OnReLinkCCG() 
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

void CDlgDebugMon2::OnReLinkHVpower() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press HV Power: relink");
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
		pfrm->m_wndImpPanelAll2.HVPower_Init( TRUE );
}

void CDlgDebugMon2::OnReset771()
{
	theApp.DisplayMsg( "Press HV Power 771 reset" );
	CSpellmanStatusDetector::Instance().SetParam( 56, 0 );
}

LRESULT CDlgDebugMon2::OnSpellmanStatusChanged(WPARAM wParam, LPARAM lParam)
{
	if( wParam == 0x01 )
	{
		CString str;
		str.Format( "%s / %s", CHVPowerDataManager::Instance().m_Spellmans.status.strID, CHVPowerDataManager::Instance().m_Spellmans.status.strVer );
		GetDlgItem( IDC_STATIC_DEBUG_771_Type_Ver )->SetWindowText( str );
	}
	else if( wParam == 0x02 )
	{
		CString str;
		str.Format( "Gun supply ID is: %s", CHVPowerDataManager::Instance().m_Spellmans.status.strID );
		theApp.DisplayMsg( str );
		str.Format( "%s / %s", CHVPowerDataManager::Instance().m_Spellmans.status.strID, CHVPowerDataManager::Instance().m_Spellmans.status.strVer );
		GetDlgItem( IDC_STATIC_DEBUG_771_Type_Ver )->SetWindowText( str );
	}
	else if( wParam == 0x03 )
	{
		CString str;
		str.Format( "Software Version ID is: %s", CHVPowerDataManager::Instance().m_Spellmans.status.strVer );
		theApp.DisplayMsg( str );
		str.Format( "%s / %s", CHVPowerDataManager::Instance().m_Spellmans.status.strID, CHVPowerDataManager::Instance().m_Spellmans.status.strVer );
		GetDlgItem( IDC_STATIC_DEBUG_771_Type_Ver )->SetWindowText( str );
	}
	else if( wParam == 0x04 )
	{
		CString str;
		str.Format( "On/Off Status is 0x%02X, register is 0x%05X", CHVPowerDataManager::Instance().m_Spellmans.status.bstatusSwitch, CHVPowerDataManager::Instance().m_Spellmans.status.dwstatusRegister );
		theApp.DisplayMsg( str );
 		HV_UpdateStatusSwitches();
 		HV_UpdateStatusRegister();
	}

	return 0;
}

void CDlgDebugMon2::HV_UpdateStatusSwitches()
{
	BYTE bS = CHVPowerDataManager::Instance().m_Spellmans.status.bstatusSwitch;
	if( bS & 0x01 )
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_Clt )->SetWindowText( "1" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bCollector	= TRUE;
	}
	else
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_Clt )->SetWindowText( "0" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bCollector	= FALSE;
	}
	if( bS & 0x02 )
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_Scint )->SetWindowText( "1" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bScint		= TRUE;
	}
	else
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_Scint )->SetWindowText( "0" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bScint		= FALSE;
	}
	if( bS & 0x04 )
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_EHT )->SetWindowText( "1" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bEHT		= TRUE;
	}
	else
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_EHT )->SetWindowText( "0" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bEHT		= FALSE;
	}
	if( bS & 0x08 )
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_Heater )->SetWindowText( "1" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bHeater		= TRUE;
	}
	else
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_Heater )->SetWindowText( "0" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bHeater		= FALSE;
	}
	if( bS & 0x10 )
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_PMT )->SetWindowText( "1" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bPMT		= TRUE;
	}
	else
	{
		GetDlgItem( IDC_STATIC_DEBUG_771_Switch_PMT )->SetWindowText( "0" );
		CHVPowerDataManager::Instance().m_Spellmans.status.bPMT		= FALSE;
	}
}

void CDlgDebugMon2::HV_UpdateStatusRegister()
{
	DWORD dwR = CHVPowerDataManager::Instance().m_Spellmans.status.dwstatusRegister;
	if( dwR & 0x01 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_0 )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_0 )->SetWindowText( "0" );
	if( dwR & 0x02 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_Vac )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_Vac )->SetWindowText( "0" );
	if( dwR & 0x04 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_EHT )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_EHT )->SetWindowText( "0" );
	if( dwR & 0x08 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_3 )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_3 )->SetWindowText( "0" );
	if( dwR & 0x10 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_4 )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_4 )->SetWindowText( "0" );
	if( dwR & 0x20 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_5 )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_5 )->SetWindowText( "0" );
	if( dwR & 0x40 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_6 )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_6 )->SetWindowText( "0" );
	if( dwR & 0x80 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_PMT )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_PMT )->SetWindowText( "0" );
	if( dwR & 0x100 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_8 )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_8 )->SetWindowText( "0" );
	if( dwR & 0x200 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_9 )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Reg_9 )->SetWindowText( "0" );
	if( dwR & 0x400 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_Clt )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_Clt )->SetWindowText( "0" );
	if( dwR & 0x800 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_Scint )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Int_Scint )->SetWindowText( "0" );
	if( dwR & 0x1000 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Rail_Fault )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Rail_Fault )->SetWindowText( "0" );
	if( dwR & 0x2000 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_EHT )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_EHT )->SetWindowText( "0" );
	if( dwR & 0x4000 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_Clt )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_Clt )->SetWindowText( "0" );
	if( dwR & 0x8000 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_Scint )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_Scint )->SetWindowText( "0" );
	if( dwR & 0x10000 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_PMT )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_PMT )->SetWindowText( "0" );
	if( dwR & 0x20000 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_Fila )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Trip_Fila )->SetWindowText( "0" );
	if( dwR & 0x40000 )
		GetDlgItem( IDC_STATIC_DEBUG_771_Fila_Timeout )->SetWindowText( "1" );
	else
		GetDlgItem( IDC_STATIC_DEBUG_771_Fila_Timeout )->SetWindowText( "0" );
}

LRESULT CDlgDebugMon2::OnSpellmanParamChanged(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str.Format( "%.2f", CHVPowerDataManager::Instance().m_Spellmans.pM.dII );
	GetDlgItem( IDC_STATIC_DEBUG_771_Mon_EMI )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM.dVP );
	GetDlgItem( IDC_STATIC_DEBUG_771_Mon_PMT )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM.dIH );
	GetDlgItem( IDC_STATIC_DEBUG_771_Mon_Heater )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM.dVE );
	GetDlgItem( IDC_STATIC_DEBUG_771_Mon_EHT )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM.dIB );
	GetDlgItem( IDC_STATIC_DEBUG_771_Mon_Beam )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM.dVC );
	GetDlgItem( IDC_STATIC_DEBUG_771_Mon_Clt )->SetWindowText( str );

	if( wParam > 0 )
	{
/*		// 同步滚动条位置
		CString str;
		int nPos;
		// Collector
		m_Spellmans.pS.dVC = m_Spellmans.pM.dVC;
 		nPos = (int)(m_Spellmans.pM.dVC *5);
 		m_scrollCollector.SetScrollPos( nPos );
		str.Format( "%.1f", m_Spellmans.pM.dVC );
		GetDlgItem( IDC_EDIT_Set_Collector )->SetWindowText( str );
		// EHT
		m_Spellmans.pS.dVE = m_Spellmans.pM.dVE;
		nPos = (int)(m_Spellmans.pM.dVE /10);
		m_scrollEHT.SetScrollPos( nPos );
		str.Format( "%.1f", m_Spellmans.pM.dVE );
		GetDlgItem( IDC_EDIT_Set_EHT )->SetWindowText( str );
		// Beam
		m_Spellmans.pS.dIB = m_Spellmans.pM.dIB;
		nPos = (int)(m_Spellmans.pM.dIB *10);
		m_scrollBeam.SetScrollPos( nPos );
		str.Format( "%.1f", m_Spellmans.pM.dIB );
		GetDlgItem( IDC_EDIT_Set_Beam )->SetWindowText( str );
		// Heater
		m_Spellmans.pS.dIH = m_Spellmans.pM.dIH;
		nPos = (int)(m_Spellmans.pM.dIH *1000);
		m_scrollHeater.SetScrollPos( nPos );
		str.Format( "%.3f", m_Spellmans.pM.dIH );
		GetDlgItem( IDC_EDIT_Set_Heater )->SetWindowText( str );
		// PMT
		m_Spellmans.pS.dVP = m_Spellmans.pM.dVP;
		nPos = (int)(m_Spellmans.pM.dVP /0.33);
		m_scrollPMT.SetScrollPos( nPos );
		str.Format( "%.1f", m_Spellmans.pM.dVP );
		GetDlgItem( IDC_EDIT_Set_PMT )->SetWindowText( str );
*/	}
	return 0;
}

LRESULT CDlgDebugMon2::OnSpellmanParam2Changed(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str.Format( "%5.1f", CHVPowerDataManager::Instance().m_Spellmans.pM2.dVC );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_Clt )->SetWindowText( str );
	str.Format( "%4d", (int)(CHVPowerDataManager::Instance().m_Spellmans.pM2.dVE) );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_EHT )->SetWindowText( str );
	str.Format( "%4.1f", CHVPowerDataManager::Instance().m_Spellmans.pM2.dIB );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_Beam )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM2.dIH );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_Heater )->SetWindowText( str );
	str.Format( "%6.2f", CHVPowerDataManager::Instance().m_Spellmans.pM2.dVP );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_PMT )->SetWindowText( str );

	if( wParam > 0 )
	{
/*		// 同步滚动条位置
		CString str;
		int nPos;
		// Collector
		nPos = (int)(m_Spellmans.pM2.dVC *5);
		m_scrollCollector2.SetScrollPos( nPos );
		str.Format( "%05.1f", m_Spellmans.pM2.dVC );
		GetDlgItem( IDC_EDIT_Set_Collector2 )->SetWindowText( str );
		// EHT
		nPos = (int)(m_Spellmans.pM2.dVE /10);
		m_scrollEHT2.SetScrollPos( nPos );
		str.Format( "%04d", (int)(m_Spellmans.pM2.dVE) );
		GetDlgItem( IDC_EDIT_Set_EHT2 )->SetWindowText( str );
		// Beam
		nPos = (int)(m_Spellmans.pM2.dIB *10);
		m_scrollBeam2.SetScrollPos( nPos );
		str.Format( "%04.1f", m_Spellmans.pM2.dIB );
		GetDlgItem( IDC_EDIT_Set_Beam2 )->SetWindowText( str );
		// Heater
		nPos = (int)(m_Spellmans.pM2.dIH *1000);
		m_scrollHeater2.SetScrollPos( nPos );
		str.Format( "%.3f", m_Spellmans.pM2.dIH );
		GetDlgItem( IDC_EDIT_Set_Heater2 )->SetWindowText( str );
		// PMT
		nPos = (int)(m_Spellmans.pM2.dVP /0.33);
		m_scrollPMT2.SetScrollPos( nPos );
		str.Format( "%06.2f", m_Spellmans.pM2.dVP );
		GetDlgItem( IDC_EDIT_Set_PMT2 )->SetWindowText( str );
*/	}
	return 0;
}

LRESULT CDlgDebugMon2::OnSpellmanParam3Changed(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM3.dVC );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_Clt )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM3.dVE );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_EHT )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM3.dIB );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_Beam )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM3.dIH );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_Heater )->SetWindowText( str );
	str.Format( "%.3f", CHVPowerDataManager::Instance().m_Spellmans.pM3.dVP );
	GetDlgItem( IDC_STATIC_DEBUG_771_Rates_PMT )->SetWindowText( str );

	if( wParam > 0 )
	{
/*		// 同步滚动条位置
		CString str;
		int nPos;
		// Collector
		nPos = (int)(m_Spellmans.pM2.dVC *5);
		m_scrollCollector2.SetScrollPos( nPos );
		str.Format( "%05.1f", m_Spellmans.pM2.dVC );
		GetDlgItem( IDC_EDIT_Set_Collector2 )->SetWindowText( str );
		// EHT
		nPos = (int)(m_Spellmans.pM2.dVE /10);
		m_scrollEHT2.SetScrollPos( nPos );
		str.Format( "%04d", (int)(m_Spellmans.pM2.dVE) );
		GetDlgItem( IDC_EDIT_Set_EHT2 )->SetWindowText( str );
		// Beam
		nPos = (int)(m_Spellmans.pM2.dIB *10);
		m_scrollBeam2.SetScrollPos( nPos );
		str.Format( "%04.1f", m_Spellmans.pM2.dIB );
		GetDlgItem( IDC_EDIT_Set_Beam2 )->SetWindowText( str );
		// Heater
		nPos = (int)(m_Spellmans.pM2.dIH *1000);
		m_scrollHeater2.SetScrollPos( nPos );
		str.Format( "%.3f", m_Spellmans.pM2.dIH );
		GetDlgItem( IDC_EDIT_Set_Heater2 )->SetWindowText( str );
		// PMT
		nPos = (int)(m_Spellmans.pM2.dVP /0.33);
		m_scrollPMT2.SetScrollPos( nPos );
		str.Format( "%06.2f", m_Spellmans.pM2.dVP );
		GetDlgItem( IDC_EDIT_Set_PMT2 )->SetWindowText( str );
*/	}
	return 0;
}/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
