// DlgDebugAdvance.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgDebugAdvance.h"

#include "ScanView.h"
#include "MainFrm.h"
#include "DlgDiagLog.h"
#include "ControlLayer.h"
#include "CCPS3603tatusDetector.h"
#include "ScanParameterManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugAdvance dialog


CDlgDebugAdvance::CDlgDebugAdvance(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebugAdvance::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebugAdvance)
	m_bLEDs			= FALSE;
	//}}AFX_DATA_INIT
	m_bLEDsbak		= FALSE;
}


void CDlgDebugAdvance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebugAdvance)
	DDX_Control(pDX, IDC_IP_Debug_Address, m_ipAddress);
	DDX_Check(pDX, IDC_CHECK_TestLEDs, m_bLEDs);
	DDX_Control(pDX, IDC_SCROLLBAR_WobDAC, m_scrollWobDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_WobFreq, m_scrollWobFreq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebugAdvance, CDialog)
	//{{AFX_MSG_MAP(CDlgDebugAdvance)
	ON_BN_CLICKED(IDC_BUTTON_ReadIDs, OnReadBoardIDs)
	ON_BN_CLICKED(IDC_CHECK_TestLEDs, OnCHECKTestLEDs)
	ON_BN_CLICKED(IDC_BUTTON_Reset_SubBoards, OnBUTTONResetSubBoards)
	ON_BN_CLICKED(IDC_BUTTON_Reset_FT232, OnBUTTONResetFT232)
	ON_BN_CLICKED(IDC_BUTTON_Reset_DI16A, OnBUTTONResetDI16A)
	ON_BN_CLICKED(IDC_BUTTON_Diagnose, OnBUTTONDiagnose)
	ON_BN_CLICKED(IDC_BUTTON_Diagnose_Setup, OnBUTTONDiagnoseSetup)
	ON_BN_CLICKED(IDC_CHECK_L2CompDisable, OnL2CompDisable)
	ON_BN_CLICKED(IDC_BUTTON_SetWobMinCmd, OnSetWobMinCmd)
	ON_BN_CLICKED(IDC_BUTTON_SetAutoWobbleCmd, OnSetAutoWobbleCmd)
	ON_BN_CLICKED(IDC_BUTTON_SetWobMaxCmd, OnSetWobMaxCmd)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugAdvance message handlers

void CDlgDebugAdvance::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL));

	GetDlgItem(IDC_CHECK_TestLEDs)->SetWindowText(GetResString( IDS_CHECK_TestLEDs ));
	GetDlgItem(IDC_BUTTON_ReadIDs)->SetWindowText(GetResString( IDS_BUTTON_ReadIDs ));

	GetDlgItem(IDS_EDIT_Debug_Scale)->SetWindowText(GetResString( IDS_EDIT_Debug_Scale )); 
	GetDlgItem(IDS_COMBO_Debug_EMI)->SetWindowText(GetResString( IDS_COMBO_Debug_EMI ));
}

BOOL CDlgDebugAdvance::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox* pEMI = ((CComboBox*)(GetDlgItem(IDC_COMBO_Debug_EMI)));
	if( pEMI != NULL )
	{
		pEMI->AddString("100");
		pEMI->AddString("150");
		pEMI->AddString("200");
		pEMI->AddString("250");
	}

	m_scrollWobDAC.SetScrollRange( 0x7A0, 0x840 );
	m_scrollWobDAC.SetScrollPos( 2048 );
	m_scrollWobFreq.SetScrollRange( 1, 2000 );
	m_scrollWobFreq.SetScrollPos( 20 );
	m_nAutoWobblerMin = 0x7F8;
	m_nAutoWobblerMax = 0x808;
	m_nAutoWobblerStep = 1;
/* 	m_scrollWobDAC.SetScrollRange( 1, 8 );
 	m_scrollWobDAC.SetScrollPos( 2 );
	m_scrollWobFreq.SetScrollRange( 1, 8 );
	m_scrollWobFreq.SetScrollPos( 3 );
*/	m_bAutoWobblerOn = FALSE;
	m_nTimerWob = 0;
	m_bL2Comp = TRUE;	// 补偿开关始终开
	GetDlgItem(IDC_EDIT_WobDAC)->SetWindowText("800");
	GetDlgItem(IDC_STATIC_Wob_Status)->SetWindowText("Wobbler: from [7F8] to [808], 01, @ 20 ms");

	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDebugAdvance::OnOK() 
{
//	CDialog::OnOK();
	///////////////////////////////////////////////////////
	// 自定义标尺长度
	CScanView* pView = (CScanView*)(theApp.GetScanView());
	if( pView )
	{
		CString str;
		GetDlgItem(IDC_EDIT_Debug_Scale)->GetWindowText( str );
		pView->SetUScaleParamByDefine( atof(str) );
	}
	///////////////////////////////////////////////////////

	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	CString str;
	int nPort;

	///////////////////////////////////////////////////////
	// EMI
	CComboBox* pEMI = ((CComboBox*)(GetDlgItem(IDC_COMBO_Debug_EMI)));
	if( pEMI != NULL )
	{
		int nIndex = pEMI->GetCurSel();
		int nEMI = 0;
		switch( nIndex )
		{
		case	0:
			nEMI	= 100;
			break;
		case	1:
			nEMI	= 150;
			break;
		case	2:
			nEMI	= 200;
			break;
		case	3:
			nEMI	= 250;
			break;
		default:
			nEMI	= 150;
			break;
		}
		CCPS3603tatusDetector::Instance().SetParam( 31, nEMI );
		str.Format( "%d", nEMI );
		WritePrivateProfileString( "Auto", "EMI", str, path );
	}


	DWORD dwIP;
	m_ipAddress.GetAddress( dwIP );
	if( CControlLayer::Instance().m_Net.LocalAddr.sin_addr.S_un.S_addr != dwIP )
	{
		CControlLayer::Instance().m_Net.LocalAddr.sin_addr.S_un.S_addr = dwIP;
		str.Format( "%ld", dwIP );
		WritePrivateProfileString( "Third", "Address", str, path );
	}
	GetDlgItem( IDC_IP_Debug_Port )->GetWindowText( str );
	nPort = atoi( str );
	if( CControlLayer::Instance().m_Net.LocalAddr.sin_port != nPort )
	{
		CControlLayer::Instance().m_Net.LocalAddr.sin_port = nPort;
		str.Format( "%d", nPort );
		WritePrivateProfileString( "Third", "Port", str, path );
	}
}

void CDlgDebugAdvance::OnCHECKTestLEDs() 
{
	UpdateData();
	if( m_bLEDs != m_bLEDsbak )
	{
		m_bLEDsbak = m_bLEDs;
		CString str1, str2;
		str1 = GetResString( IDS_CHECK_TestLEDs );
		if( m_bLEDs )
			str2 = str1 + ": ON";
		else
			str2 = str1 + ": OFF";
		GetDlgItem(IDC_CHECK_TestLEDs)->SetWindowText( str2 );
		int nCheck = (int)m_bLEDs;
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
}

void CDlgDebugAdvance::OnReadBoardIDs()
{
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
}


void CDlgDebugAdvance::OnBUTTONResetSubBoards() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
	theApp.DisplayMsg("Press reset SubBoards");
}

void CDlgDebugAdvance::OnBUTTONResetFT232() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press reset FT232");
	CScanView* pView = (CScanView*)(theApp.GetScanView());
	ASSERT(pView);
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

void CDlgDebugAdvance::OnBUTTONResetDI16A() 
{
	// TODO: Add your control notification handler code here
	theApp.DisplayMsg("Press reset DI16A");
	CScanView* pView = (CScanView*)(theApp.GetScanView());
	ASSERT(pView);

	// 保存现场
	USB_ScanType nScanType = CScanParameterManager::Instance().m_nusbScanType;
	// 停止扫描，包括后台扫描
	pView->USB_ChangeScan( USB_ST_Stop );
	CSemCtrl::Instance().USB_WriteToBoard( USB_StopScan, FALSE, 0 );
	// 重新寻找并连接DI-16A接口
	CSemCtrl::Instance().USB_ReinitImagePort();
	// 恢复现场
	pView->USB_ChangeScan( nScanType );
	theApp.DisplayMsg("Press reset DI16A");
}

void CDlgDebugAdvance::OnBUTTONDiagnose() 
{
	// TODO: Add your control notification handler code here
	theApp.m_bDiagLogging = ((CButton*)(GetDlgItem(IDC_BUTTON_Diagnose)))->GetCheck();
	CScanView* pView = (CScanView*)(theApp.GetScanView());
	if( pView )
	{
		((CMainFrame *)theApp.GetMainWnd())->m_dlgGraph.ShowWindow( SW_SHOW );
		((CMainFrame *)theApp.GetMainWnd())->m_dlgDiffGraph.ShowWindow( SW_SHOW );
		pView->Diagnose( theApp.m_bDiagLogging );
	}
	GetDlgItem(IDC_BUTTON_Diagnose_Setup)->EnableWindow( !theApp.m_bDiagLogging );
}

void CDlgDebugAdvance::OnBUTTONDiagnoseSetup() 
{
	// TODO: Add your control notification handler code here
	CDlgDiagLog dlg;
	dlg.DoModal();
}




/////////////////////////////////////////////////////////////////////
// Wobbler
/////////////////////////////////////////////////////////////////////

BOOL CDlgDebugAdvance::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_WobDAC:
		case	IDC_EDIT_WobFreq:
			GetDlgItem( nID )->GetWindowText( str );
			break;
		default:
			break;
		}
		if( str.IsEmpty() )
			return CDialog::PreTranslateMessage(pMsg);
		int nPos;
		char *bufNum;
		switch( nID )
		{
		case	IDC_EDIT_WobDAC:
			bufNum = str.GetBuffer(4);
			sscanf(bufNum, "%03X", &nPos);
			break;
		case	IDC_EDIT_WobFreq:
			bufNum = str.GetBuffer(5);
			sscanf(bufNum, "%d", &nPos);
			break;
		default:
			break;
		}
		switch( nID )
		{
		case	IDC_EDIT_WobDAC:
			m_scrollWobDAC.SetScrollPos( nPos );
			WobDACSB_Change( nPos, TRUE );
			break;
		case	IDC_EDIT_WobFreq:
			m_scrollWobFreq.SetScrollPos( nPos );
			WobFreqSB_Change( nPos );
		}
		
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDebugAdvance::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	case IDC_SCROLLBAR_WobDAC:
		WobDACSB_Change( pScrollBar->GetScrollPos(), m_bL2Comp );
		break;
	case	IDC_SCROLLBAR_WobFreq:
		WobFreqSB_Change( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgDebugAdvance::OnL2CompDisable() 
{
	// Disable/Enable control for drift compensation of L2 circuit
	// Dim L2CompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: L2CompOn keeps track of current software state of control of this function. All bets are off if out of sync.
/*	int nCheck = m_btnL2CompDisable.GetCheck();
	if( nCheck )
	{
		m_btnL2CompDisable.SetColor( g_crON_Comm );	// light RED (to simulate Comp Fn is ON)
//		m_btnL2CompDisable.SetWindowText( "Comp is ON" );
	}
	else
	{
		m_btnL2CompDisable.SetColor( g_crOFF );		// gray (to simulate Comp Fn is OFF)
//		m_btnL2CompDisable.SetWindowText( "Comp is OFF" );
	}
	theApp.m_DL.log.nCheckComp[Logging_Comp_OL -5] = nCheck;
*/
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_L2CompDisable)))->GetCheck();
	if( nCheck )
	{
		m_bL2Comp = TRUE;
		theApp.DisplayMsg( "compensation ON" );
	}
	else
	{
		m_bL2Comp = FALSE;
		theApp.DisplayMsg( "compensation OFF" );
	}
	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x2A as low byte (L2 compensation disable ctl.)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ObjectiveLens_Wobbler,	// Serial Comm operand low byte (L2 compensation disable ctl.)
										USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
										nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
										nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CDlgDebugAdvance::OnSetWobMaxCmd() 
{
	// Utilizes global: AutoWobblerMax, used by Auto Wobbler timer
	m_nAutoWobblerMax = m_scrollWobDAC.GetScrollPos();	// Use actual slider value, not variable value
	CalcWobStep();
}

void CDlgDebugAdvance::OnSetWobMinCmd() 
{
	// Utilizes global: AutoWobblerMin, used by Auto Wobbler timer
	m_nAutoWobblerMin = m_scrollWobDAC.GetScrollPos();	// Use actual slider value, not variable value
	CalcWobStep();
}

void CDlgDebugAdvance::CalcWobStep()
{
	int nDiff = m_nAutoWobblerMax -m_nAutoWobblerMin;
	if( nDiff < 0x10+1 )
		m_nAutoWobblerStep = 1;
	else if( nDiff < 0x20 +1 )
		m_nAutoWobblerStep = 2;
	else if( nDiff < 0x40 +1 )
		m_nAutoWobblerStep = 4;
	else if( nDiff < 0x80 +1 )
		m_nAutoWobblerStep = 8;
	else
		m_nAutoWobblerStep = nDiff /0x10;

	int nPosFreq = m_scrollWobFreq.GetScrollPos();
	CString str;
	str.Format( "Wobbler: from [%03X] to [%03X], %02X, @ %d ms",
		m_nAutoWobblerMin, m_nAutoWobblerMax, m_nAutoWobblerStep, nPosFreq );
	GetDlgItem(IDC_STATIC_Wob_Status)->SetWindowText(str);
	str.Format("%d", nPosFreq);
	GetDlgItem(IDC_EDIT_WobFreq)->SetWindowText(str);
}

void CDlgDebugAdvance::OnSetAutoWobbleCmd() 
{
	// Utilizes global: AutoWobblerOn
	m_bAutoWobblerOn = !m_bAutoWobblerOn;
/*	if( m_bAutoWobblerOn )
	{
		// 开启补偿开关
		((CButton*)(GetDlgItem(IDC_CHECK_L2CompDisable)))->SetCheck(1);
		OnL2CompDisable();

		// 得到最新范围值
		int nPos = m_scrollWobDAC.GetScrollPos();
		WobDACSB_Change2( nPos );
		m_bAutoWobblerDir	= TRUE;
		m_nAutoWobblerLast	= 0x800;	// m_nAutoWobblerMin;
		// 得到频率
		nPos = m_scrollWobFreq.GetScrollPos();
		m_nTimerWob = SetTimer( 1, (nPos +1) *250 /34, NULL );

		GetDlgItem(IDC_BUTTON_SetAutoWobbleCmd)->SetWindowText( "Wobbler ON" );
		CString str;
		str.Format( "Start auto wobbler @ %.2f s, from [%03X] to [%03X]",
			(nPos-1)*0.25 +0.5, m_nAutoWobblerMin, m_nAutoWobblerMax );
		theApp.DisplayMsg( str );
	}
	else
	{
		// 关闭补偿开关
		((CButton*)(GetDlgItem(IDC_CHECK_L2CompDisable)))->SetCheck(0);
		OnL2CompDisable();
		KillTimer( 1 );
		m_nTimerWob = 0;
		m_bAutoWobblerDir = FALSE;
		GetDlgItem(IDC_BUTTON_SetAutoWobbleCmd)->SetWindowText( "Wobbler OFF" );
		theApp.DisplayMsg( "Stop auto wobbler" );
	}
*/
	if( m_bAutoWobblerOn )
	{
		CalcWobStep();
		GetDlgItem(IDC_BUTTON_SetAutoWobbleCmd)->SetWindowText( "Wobbler ON" );
		m_bAutoWobblerDir	= TRUE;
		m_nAutoWobblerLast	= m_nAutoWobblerMin;
		int nPosFreq = m_scrollWobFreq.GetScrollPos();
		m_nTimerWob = SetTimer( 1, nPosFreq, NULL );

		CString str;
		str.Format( "Start auto wobbler: from [%03X] to [%03X], %02X, @ %d ms",
			m_nAutoWobblerMin, m_nAutoWobblerMax, m_nAutoWobblerStep, nPosFreq );
		theApp.DisplayMsg( str );
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SetAutoWobbleCmd)->SetWindowText( "Wobbler OFF" );
		KillTimer( 1 );
		m_nTimerWob = 0;
		m_bAutoWobblerDir = FALSE;
		theApp.DisplayMsg( "Stop auto wobbler" );
	}
}

void CDlgDebugAdvance::WobDACSB_Change( int nPos, BOOL bUpdate )
{
	BYTE byteHi, byteLo;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	str.Format( "%03X", nPos );
	GetDlgItem( IDC_EDIT_WobDAC )->SetWindowText( str );

	byteLo	= (BYTE)( nPos & 0x00FF );
	byteHi	= (BYTE)( nPos >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	if( bUpdate )
		// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_WobblerSetting,	// Serial Comm operand low byte (Y alignment DAC circuit address)
		USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
}

void CDlgDebugAdvance::WobDACSB_Change2( int nPos )
{
	int nBakTimer = m_nTimerWob;
	if( nBakTimer > 0 )
	{
		KillTimer( 1 );
		m_nTimerWob = 0;
	}

//	// 1: 7F0 - 810,	2: 7E0 - 820,	3: 7D0 - 830,	4: 7C0 - 840
//	// 5: 7B0 - 850,	6: 7A0 - 860,	7: 790 - 870,	8: 780 - 880
	// 1: 7F8 - 808,	2: 7F0 - 810,	3: 7E8 - 818,	4: 7E0 - 820
	// 5: 7D8 - 828,	6: 7D0 - 830,	7: 7C8 - 838,	8: 7C0 - 840
	m_nAutoWobblerMin = 0x800 - 0x10 *nPos;
	m_nAutoWobblerMax = 0x800 + 0x10 *nPos;
	if( m_nAutoWobblerMin < 0 )
		m_nAutoWobblerMin = 0;
	if( m_nAutoWobblerMax > 0xFFF )
		m_nAutoWobblerMax = 0xFFF;

	int nFreqPos = m_scrollWobFreq.GetScrollPos();
	CString str;
	str.Format( "Wobbler Status: from [%03X] to [%03X], @ %.2f s",
		m_nAutoWobblerMin, m_nAutoWobblerMax, (nFreqPos-1)*0.25 +0.5 );
	GetDlgItem(IDC_STATIC_Wob_Status)->SetWindowText(str);

	if( nBakTimer > 0 )
	{
		int nFreqPos = m_scrollWobFreq.GetScrollPos();
		m_nTimerWob = SetTimer( 1, (nFreqPos +1) *250 /34, NULL );
		str.Format( "Change auto wobbler @ %.2f s, from [%03X] to [%03X]",
			(nFreqPos-1)*0.25 +0.5, m_nAutoWobblerMin, m_nAutoWobblerMax );
		theApp.DisplayMsg( str );
	}
}

void CDlgDebugAdvance::WobDAC_Output( int nValue )
{
	CString str;
	str.Format( "%03X", nValue );
	GetDlgItem(IDC_EDIT_WobDAC)->SetWindowText( str );

	BYTE byteHi, byteLo;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_WobblerSetting,	// Serial Comm operand low byte (Y alignment DAC circuit address)
		USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
}

void CDlgDebugAdvance::WobFreqSB_Change( int nPos )
{
	CalcWobStep();
	if( m_nTimerWob > 0 )
	{
		KillTimer( 1 );
		m_nTimerWob = 0;
		m_nTimerWob = SetTimer( 1, nPos, NULL );
		CString str;
		str.Format( "Change auto wobbler freq to @ %d ms", nPos );
		theApp.DisplayMsg( str );
	}
}

void CDlgDebugAdvance::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	// Utilizes globals: AutoWobblerOn, AutoWobblerMax and AutoWobblerMin, AutoWobblerLast, AutoWobblerDir
	int nAutoWobblerNew;
	// this size step should result in one full cycle per sec.
//	nAutoWobblerStep = (m_nAutoWobblerMax - m_nAutoWobblerMin) /10;
/*	
	if( m_bAutoWobblerOn )
	{
		if( m_bAutoWobblerDir )	// heading up
		{
			nAutoWobblerNew = m_nAutoWobblerLast + m_nAutoWobblerStep;
			if( nAutoWobblerNew > m_nAutoWobblerMax )
			{
				nAutoWobblerNew		= m_nAutoWobblerMax;
				m_bAutoWobblerDir	= FALSE;	// Hit max, change direction to DOWN
			}
		}
		else
		{
			nAutoWobblerNew = m_nAutoWobblerLast - m_nAutoWobblerStep;
			if( nAutoWobblerNew < m_nAutoWobblerMin )
			{
				nAutoWobblerNew		= m_nAutoWobblerMin;
				m_bAutoWobblerDir	= TRUE;		// Hit min, change direction to UP
			}
		}
		m_nAutoWobblerLast = nAutoWobblerNew;
		// force slider to new value, which triggers an update communication to hardware
		m_scrollWobDAC.SetScrollPos( nAutoWobblerNew );
		WobDACSB_Change( nAutoWobblerNew, TRUE );
//		WobDAC_Output( nAutoWobblerNew );
	}
*/
	if( m_bAutoWobblerDir )
		nAutoWobblerNew = m_nAutoWobblerMax;
	else
		nAutoWobblerNew = m_nAutoWobblerMin;
	m_bAutoWobblerDir = !m_bAutoWobblerDir;
	m_scrollWobDAC.SetScrollPos( nAutoWobblerNew );
	WobDACSB_Change( nAutoWobblerNew, TRUE );
}
