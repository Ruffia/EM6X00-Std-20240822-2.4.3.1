// PanelVS.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "PanelVS.h"
#include "MainFrm.h"
#include "ControlLayer.h"
#include "BoardComm.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelVS dialog


CPanelVS::CPanelVS(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelVS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelVS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPanelVS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelVS)
	DDX_Control(pDX, IDC_CHECK_SoftEnable_Vacuum, m_btnSoftEnVac);
	DDX_Control(pDX, IDC_BUTTON_SPRreadBoardID, m_btnSPRreadBoardID);
	DDX_Control(pDX, IDC_BUTTON_VACreadBoardID, m_btnVACreadBoardID);
	DDX_Control(pDX, IDC_BUTTON_SPRsoftResetCmd, m_btnSPRsoftReset);
	DDX_Control(pDX, IDC_BUTTON_VACsoftResetCmd, m_btnVACsoftReset);
	DDX_Control(pDX, IDC_BUTTON_HVOnCmd, m_btnHVOn);
	DDX_Control(pDX, IDC_BUTTON_Relay2EnCmd, m_btnSPRRelay2En);
	DDX_Control(pDX, IDC_CHECK_Switch_Analog_Power_Supplies, m_btnAnalogPS);
	DDX_Control(pDX, IDC_SCROLLBAR_LegHVSB, m_scrollLegHV);
	DDX_Control(pDX, IDC_SCROLLBAR_SPRDACsValSB, m_scrollSPRDACsVal);
	DDX_Control(pDX, IDC_BUTTON_WDT_SelfResetToggleCmd, m_btnWDTSelfReset);
	DDX_Control(pDX, IDC_BUTTON_SPRtestLED4Cmd, m_btnSPRtestLED4);
	DDX_Control(pDX, IDC_BUTTON_Inh8Vcmd, m_btnInh8V);
	DDX_Control(pDX, IDC_BUTTON_Inh13Vcmd, m_btnInh13V);
	DDX_Control(pDX, IDC_BUTTON_Inh20Vcmd, m_btnInh20V);
	DDX_Control(pDX, IDC_BUTTON_Relay1EnCmd, m_btnSPRRelay1En);
	DDX_Control(pDX, IDC_BUTTON_FansOnPLDcmd, m_btnFansOnPLD);
	DDX_Control(pDX, IDC_BUTTON_FansOnFPGAcmd, m_btnFansOnFPGA);
	DDX_Control(pDX, IDC_BUTTON_VACtestLED4Cmd, m_btnVACtestLED4);
	DDX_Control(pDX, IDC_SCROLLBAR_IOexpSB, m_scrollIOexp);
	DDX_Control(pDX, IDC_SCROLLBAR_VACDACsValSB, m_scrollVACDACsVal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelVS, CDialog)
	//{{AFX_MSG_MAP(CPanelVS)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_VACDACupdateCmd, OnVACDACupdateCmd)
	ON_BN_CLICKED(IDC_BUTTON_ReadComprsCmd, OnReadComprsCmd)
	ON_BN_CLICKED(IDC_BUTTON_IOXstbHiCmd, OnIOXstbHiCmd)
	ON_BN_CLICKED(IDC_BUTTON_IOXstbLoCmd, OnIOXstbLoCmd)
	ON_BN_CLICKED(IDC_BUTTON_ReadVACextSwitchStatesCmd, OnReadVACextSwitchStates)
	ON_BN_CLICKED(IDC_BUTTON_VACtestLED4Cmd, OnVACtestLED4)
	ON_BN_CLICKED(IDC_BUTTON_VACsoftResetCmd, OnVACsoftReset)
	ON_BN_CLICKED(IDC_BUTTON_LocalLoopbackCmd, OnLocalLoopback)
	ON_BN_CLICKED(IDC_BUTTON_FansOnFPGAcmd, OnFansOnFPGA)
	ON_BN_CLICKED(IDC_BUTTON_FansOnPLDcmd, OnFansOnPLD)
	ON_BN_CLICKED(IDC_BUTTON_SPRsoftResetCmd, OnSPRsoftReset)
	ON_BN_CLICKED(IDC_BUTTON_Relay1EnCmd, OnSPRRelay1En)
	ON_BN_CLICKED(IDC_BUTTON_HVOnCmd, OnHVOn)
	ON_BN_CLICKED(IDC_BUTTON_LegHVOnOffCmd, OnLegHVOnOff)
	ON_BN_CLICKED(IDC_BUTTON_Inh20Vcmd, OnInh20V)
	ON_BN_CLICKED(IDC_BUTTON_Inh13Vcmd, OnInh13V)
	ON_BN_CLICKED(IDC_BUTTON_Inh8Vcmd, OnInh8V)
	ON_BN_CLICKED(IDC_BUTTON_SPRtestLED4Cmd, OnSPRtestLED4)
	ON_BN_CLICKED(IDC_BUTTON_TestR8cmd, OnTestR8)
	ON_BN_CLICKED(IDC_BUTTON_TestReadSPRswitchesCmd, OnTestReadSPRswitches)
	ON_BN_CLICKED(IDC_BUTTON_TestR16cmd, OnTestR16)
	ON_BN_CLICKED(IDC_BUTTON_TestR32cmd, OnTestR32)
	ON_BN_CLICKED(IDC_BUTTON_ReadSPRADCcmd, OnReadSPRADC)
	ON_BN_CLICKED(IDC_BUTTON_ReadVACADCcmd, OnReadVACADC)
	ON_BN_CLICKED(IDC_BUTTON_ReadSelectedADCcmd, OnReadSelectedADC)
	ON_BN_CLICKED(IDC_CHECK_ReadSelectedADCAuto, OnCHECKReadSelectedADCAuto)
	ON_BN_CLICKED(IDC_BUTTON_WDT_SelfResetToggleCmd, OnWDTSelfResetToggle)
	ON_BN_CLICKED(IDC_BUTTON_SPRDACupdateCmd, OnSPRDACupdate)
	ON_BN_CLICKED(IDC_CHECK_ReadVACmprsAuto, OnCHECKReadVACmprsAuto)
	ON_BN_CLICKED(IDC_BUTTON_BoardCommRxCmd, OnBoardCommRx)
	ON_BN_CLICKED(IDC_BUTTON_ClearFIFO, OnClearFIFO)
	ON_BN_CLICKED(IDC_BUTTON_ClearList, OnClearList)
	ON_BN_CLICKED(IDC_CHECK_Switch_Analog_Power_Supplies, OnSwitchAnalogPowerSupplies)
	ON_BN_CLICKED(IDC_BUTTON_Relay2EnCmd, OnSPRRelay2En)
	ON_BN_CLICKED(IDC_BUTTON_VACreadBoardID, OnVACreadBoardID)
	ON_BN_CLICKED(IDC_BUTTON_SPRreadBoardID, OnSPRreadBoardID)
	ON_BN_CLICKED(IDC_CHECK_SoftEnable_Vacuum, OnCHECKSoftEnableVacuum)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_VACDACgroupOption0, IDC_RADIO_VACDACgroupOption2, OnVACDACgroupOptionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_VACDACselOption0, IDC_RADIO_VACDACselOption3, OnVACDACselOptionChanged)
	ON_COMMAND_RANGE(IDC_CHECK_FPGArelayEn0, IDC_CHECK_FPGArelayEn11, OnFPGArelayEnChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCselectOB0, IDC_RADIO_ADCselectOB5, OnADCselectOBChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCsChSelect0, IDC_RADIO_ADCsChSelect15, OnADCsChSelectChanged)
	ON_COMMAND_RANGE(IDC_RADIO_SPRDACselOption0, IDC_RADIO_SPRDACselOption3, OnSPRDACselOptionChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelVS message handlers

BOOL CPanelVS::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_nVACDACselected	= 0;
	m_nVACDACchannel	= 0;
	m_byteVACDACselBits	= 0x10;

	CBoardComm::Instance().ResetADC();

	m_nSPRDACchannel = m_nSPRDACnewVal = 0;
	((CButton*)(GetDlgItem(IDC_RADIO_SPRDACselOption0)))->SetCheck(1);

	m_nVACDACselected = m_nVACDACchannel = 0;
	m_byteVACDACaddr = m_byteVACDACselBits = 0;
	m_strVACDACRefDes = m_strVACDACchan = "";

	m_scrollVACDACsVal.SetScrollRange( 0, 4095 );
	m_scrollIOexp.SetScrollRange( 0, 31 );
	m_scrollLegHV.SetScrollRange( 0, 511 );
	m_scrollSPRDACsVal.SetScrollRange( -32768, 32767 );

	m_bVACtestLED4 = m_bSPRtestLED4 =  m_bWDTSelfReset = FALSE;
	m_bFansOnFPGA = m_bFansOnPLD = TRUE;
	m_bSPRRelay1En = m_bSPRRelay2En = m_bHVOn = FALSE;
	m_bInh20V = m_bInh13V = m_bInh8V = TRUE;

	m_btnVACtestLED4.SetColor( g_crOFF );
//	m_btnVACtestLED4.SetWindowText( "Test LED is now OFF" );
	m_btnFansOnFPGA.SetColor( g_crON_Test );
//	m_btnFansOnFPGA.SetWindowText( "F:Turn Fans OFF (Fans are now ON)" );
	m_btnFansOnPLD.SetColor( g_crON_Test );
//	m_btnFansOnPLD.SetWindowText( "P:Turn Fans OFF (Fans are now ON)" );
	m_btnSPRRelay1En.SetColor( g_crOFF );
//	m_btnSPRRelay1En.SetWindowText( "J14: Vacuum Interlock (Now OFF)" );
	m_btnSPRRelay2En.SetColor( g_crOFF );
//	m_btnSPRRelay2En.SetWindowText( "Turn SPR Relay 2 ON (Now OFF)" );
	m_btnHVOn.SetColor( g_crOFF );
//	m_btnHVOn.SetWindowText( "J12: HV Disable" );
	m_btnSPRtestLED4.SetColor( g_crOFF );
//	m_btnSPRtestLED4.SetWindowText( "Test LED is now OFF" );
	m_btnAnalogPS.SetColor( g_crScanEmphasis );
	m_btnAnalogPS.SetCheck( 0 );
	m_btnInh20V.SetColor( g_crScanEmphasis );
//	m_btnInh20V.SetWindowText( "Turn +/-20V Power OFF (Now ON)" );
	m_btnInh13V.SetColor( g_crScanEmphasis );
//	m_btnInh13V.SetWindowText( "Turn +/-13V Power OFF (Now ON)" );
	m_btnInh8V.SetColor( g_crScanEmphasis );
//	m_btnInh8V.SetWindowText( "Turn +/-8V Power OFF (Now ON)" );
	m_btnWDTSelfReset.SetColor( g_crOFF );
	m_btnWDTSelfReset.SetWindowText( "Comm Inactivity Self-Init is OFF" );
	/////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPanelVS::Init()
{
	m_scrollVACDACsVal.SetScrollPos( 2048 );
	VACDACvalUpdate( 0 );
	m_scrollIOexp.SetScrollPos( 5 );
	IOXguide( 5 );
	m_scrollLegHV.SetScrollPos( 0 );
	LegHVUpdate( 0 );

	// 初始化时将光电倍增管PMT的电压设置为0V, pos = 0xFFFF
	m_scrollSPRDACsVal.SetScrollPos( 32767 );
	SPRDACsValUpdae( 32767 );
	SPRDACupdate();

	m_btnAnalogPS.SetCheck( 1 );
	SwitchAnalogPowerSupplies( 1 );

//	m_btnSoftEnVac.SetCheck( 1 );
//	CheckSoftEnableVacuum( 1 );
}

void CPanelVS::OnOK() 
{
}

void CPanelVS::OnCancel() 
{
}

BOOL CPanelVS::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_VACDACsValSBTB:
		case	IDC_EDIT_LegHVSBvalueTB:
		case	IDC_EDIT_SPRDACsValHexTB:
			GetDlgItem( nID )->GetWindowText( str );
			break;
		default:
			break;
		}
		if( str.IsEmpty() )
			return CDialog::PreTranslateMessage(pMsg);
		int nPos;
		char *bufNum = str.GetBuffer(4);
		if( nID == IDC_EDIT_SPRDACsValHexTB )
			sscanf(bufNum, "%x", &nPos);
		else
			sscanf(bufNum, "%d", &nPos);
		switch( nID )
		{
		case	IDC_EDIT_VACDACsValSBTB:
			m_scrollVACDACsVal.SetScrollPos( nPos );
			VACDACvalUpdate( nPos );
			break;
		case	IDC_EDIT_LegHVSBvalueTB:
			m_scrollLegHV.SetScrollPos( nPos );
			LegHVUpdate( nPos );
			break;
		case	IDC_EDIT_SPRDACsValHexTB:
			m_scrollSPRDACsVal.SetScrollPos( nPos -32768 );
			SPRDACsValUpdae( nPos -32768 );
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPanelVS::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CPanelVS::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 2 )
	{
		// Read Selected ADC
		CString str;
		if( CBoardComm::Instance().m_bADCtype )
			str = CBoardComm::Instance().ReadSelectedADC_Procedure( 100, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log, FALSE );
		else
			str = CBoardComm::Instance().ReadSelectedADC_Procedure( 70,  CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log, FALSE );
		GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
		ADCvconv();
	}
//	if( theApp.m_bIsStop )
	{
		VAC_ReadComprs();
	}

	CDialog::OnTimer(nIDEvent);
}

void CPanelVS::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
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
	case IDC_SCROLLBAR_IOexpSB:
		IOXguide( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_LegHVSB:
		LegHVUpdate( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPanelVS::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
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

	int id = pScrollBar->GetDlgCtrlID();					//获取滚动条ID
	switch( id )
	{
	case IDC_SCROLLBAR_VACDACsValSB:
		VACDACvalUpdate( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_SPRDACsValSB:
		SPRDACsValUpdae( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPanelVS::OnVACDACgroupOptionChanged( UINT nID )
{
	// Globals VACDACselected, VACDACaddr, VACDACchannel, VACDACselBits (DAC and channel select and mask bits for comm)
	//  DACTC0Cal, DACTC0Cal, DACTC0Cal, DACTC0Cal (previous values IF user has changed them)
	//  DACTC0Set, DACTC0Set, DACTC0Set, DACTC0Set
	//  DACTC4Cal, DACTC5Cal, DACTC4Set, DACTC5Set
	//  DACTCvalue(12) As Integer 'Current calibration and comparator adjust DAC settings as above in array form
	// Map of above array: Elements 0-3 = TC0-3 cal DAC settings @12, 4-7=TC0-3 comp adj @22, 8-11=TC4,5 cal, TC4,5 adj @32
	m_nVACDACselected = nID -IDC_RADIO_VACDACgroupOption0;
	switch( m_nVACDACselected )
	{
	case	0:	// TC1~4 Cal I
		GetDlgItem( IDC_RADIO_VACDACselOption0 )->SetWindowText( "TC0 Cal I" );
		GetDlgItem( IDC_RADIO_VACDACselOption1 )->SetWindowText( "TC1 Cal I" );
		GetDlgItem( IDC_RADIO_VACDACselOption2 )->SetWindowText( "TC2 Cal I" );
		GetDlgItem( IDC_RADIO_VACDACselOption3 )->SetWindowText( "TC3 Cal I" );
		break;
	case	1:	// TC1~4 setpoints
		GetDlgItem( IDC_RADIO_VACDACselOption0 )->SetWindowText( "TC0 Setpoint" );
		GetDlgItem( IDC_RADIO_VACDACselOption1 )->SetWindowText( "TC1 Setpoint" );
		GetDlgItem( IDC_RADIO_VACDACselOption2 )->SetWindowText( "TC2 Setpoint" );
		GetDlgItem( IDC_RADIO_VACDACselOption3 )->SetWindowText( "TC3 Setpoint" );
		break;
	case	2:	// TC5~6 Cal I and setpoints
		GetDlgItem( IDC_RADIO_VACDACselOption0 )->SetWindowText( "TC4 Cal I" );
		GetDlgItem( IDC_RADIO_VACDACselOption1 )->SetWindowText( "TC5 Cal I" );
		GetDlgItem( IDC_RADIO_VACDACselOption2 )->SetWindowText( "TC4 Setpoint" );
		GetDlgItem( IDC_RADIO_VACDACselOption3 )->SetWindowText( "TC5 Setpoint" );
		break;
	default:
		break;
	}
	VACDACgroupOptionChanged();
}

void CPanelVS::VACDACgroupOptionChanged()
{
	CString str;
	switch( m_nVACDACselected )
	{
	case	0:	// TC1~4 Cal I
		m_byteVACDACaddr	= 0x12;
		m_strVACDACRefDes	= "U44";
		str = m_strVACDACRefDes + m_strVACDACchan;
		GetDlgItem( IDC_EDIT_WhichDACTB )->SetWindowText( str );
		break;
	case	1:	// TC1~4 setpoints
		m_byteVACDACaddr	= 0x22;
		m_strVACDACRefDes	= "U45";
		str = m_strVACDACRefDes + m_strVACDACchan;
		GetDlgItem( IDC_EDIT_WhichDACTB )->SetWindowText( str );
		break;
	case	2:	// TC5~6 Cal I and setpoints
		m_byteVACDACaddr	= 0x32;
		m_strVACDACRefDes	= "U46";
		str = m_strVACDACRefDes + m_strVACDACchan;
		GetDlgItem( IDC_EDIT_WhichDACTB )->SetWindowText( str );
		break;
	default:
		m_byteVACDACaddr	= 0x0;	// nothing there, so can't accidentally a value incorrectly
		break;
	}
	// Force slider to last value (or initial value if not yet set)
	m_scrollVACDACsVal.SetScrollPos( CControlLayer::Instance().m_nDACTCvalue[m_nVACDACselected * 4 + m_nVACDACchannel] );
}

void CPanelVS::OnVACDACselOptionChanged( UINT nID )
{
	m_nVACDACchannel = nID -IDC_RADIO_VACDACselOption0;
	VACDACselOptionChanged();
}

void CPanelVS::VACDACselOptionChanged()
{
	CString str;
	switch( m_nVACDACchannel )
	{
	case	0:	// TC1~4 Cal I
		m_byteVACDACselBits	= 0x10;	// upper byte value for 24 bit write and load for DAC channel A
		m_strVACDACchan		= " Ch A";
		str = m_strVACDACRefDes + m_strVACDACchan;
		GetDlgItem( IDC_EDIT_WhichDACTB )->SetWindowText( str );
		break;
	case	1:	// TC1~4 setpoints
		m_byteVACDACselBits	= 0x14;	// upper byte value for 24 bit write and load for DAC channel B
		m_strVACDACchan		= " Ch B";
		str = m_strVACDACRefDes + m_strVACDACchan;
		GetDlgItem( IDC_EDIT_WhichDACTB )->SetWindowText( str );
		break;
	case	2:	// TC5~6 Cal I and setpoints
		m_byteVACDACselBits	= 0x18;	// upper byte value for 24 bit write and load for DAC channel C
		m_strVACDACchan		= " Ch C";
		str = m_strVACDACRefDes + m_strVACDACchan;
		GetDlgItem( IDC_EDIT_WhichDACTB )->SetWindowText( str );
		break;
	case	3:	// TC5~6 Cal I and setpoints
		m_byteVACDACselBits	= 0x1C;	// upper byte value for 24 bit write and load for DAC channel D
		m_strVACDACchan		= " Ch D";
		str = m_strVACDACRefDes + m_strVACDACchan;
		GetDlgItem( IDC_EDIT_WhichDACTB )->SetWindowText( str );
		break;
	default:
		break;
	}
	// Force slider to last value (or initial value if not yet set)
	m_scrollVACDACsVal.SetScrollPos( CControlLayer::Instance().m_nDACTCvalue[m_nVACDACselected * 4 + m_nVACDACchannel] );
}

void CPanelVS::VACDACvalUpdate( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_VACDACsValSBTB )->SetWindowText( str );
	str.Format( "%.3f V", 1.0 *nPos /4095 * 2.5 );	// Range @ DAC output is 0 to +2.5V
	GetDlgItem( IDC_EDIT_DACvalTB )->SetWindowText( str );
	CControlLayer::Instance().m_nDACTCvalue[m_nVACDACselected * 4 + m_nVACDACchannel] = nPos;	// Keep history of user setting
	CControlLayer::Instance().m_DL.diag[Diag_CalI_TC0 + m_nVACDACselected * 4 + m_nVACDACchannel].nCurPos = nPos;
	// Call VACDACupdateCmd_Click 'Commented out to prevent slider from updating DAC's
}

void CPanelVS::OnVACDACupdateCmd() 
{
	// TODO: Add your control notification handler code here
	int nVACDACvalue = m_scrollVACDACsVal.GetScrollPos();
	VACDACupdate( nVACDACvalue );
}

void CPanelVS::VACDACupdate( int nVACDACvalue )
{
	BYTE byteVACDAChibyte, byteVACDAClobyte, byteVACDACfirst24;

	byteVACDAChibyte = (BYTE)(( nVACDACvalue & 0xFF0 ) /16 );	// Shift down 4 bits to properly byte-align
	byteVACDAClobyte = (BYTE)(( nVACDACvalue & 0x0F ) *16 );	// Need to shift up by 4 bits for DAC7565 for proper alignment
	byteVACDACfirst24	= 0x10 | ( m_nVACDACchannel *2 );		// Selects immediate load, channel select shifted up 1 bit//m_byteVACDACselBits;//

	// Write via MCI to VAC board.  Cluster 1: select Xfer type, module ID and circuit ID
	// Write via MCI to VAC board.  Cluster 2: Write DAC upper byte value in OP3, Select DAC channel, etc. in OP4:
	// Write via MCI to VAC board.  Cluster 3: Write 0 to OP5, Write low DAC value nibble in bits 7..4 of OP6
/*	CSemCtrl::Instance().USB_CommToBoards3(	m_byteVACDACaddr,		// VAC board circuit address for selected DAC (0x12,22 or 32) for OP1
										USB_TM_VAC_Write_24,	// Serial Comm operand high byte (24 bit write to VAC board) for OP2
										byteVACDAChibyte,		// Upper 8 of 12 bits of DAC data for OP3
										byteVACDACfirst24,		// OP4: Bit 4 selects immediate DAC value update in H/W. Bits 2,1 select DAC channel
										0x0,					// Send 0 for OP5
										byteVACDAClobyte );		// DAC low 4 bits placed in upper 4 bits of OP6
*/
//	CommToBoards3( m_byteVACDACaddr, USB_TM_VAC_Write_24, byteVACDAChibyte, byteVACDACfirst24, 0x0, byteVACDAClobyte );
	DWORD dwWdata = (DWORD)(byteVACDACfirst24 *65536 + byteVACDAChibyte *256 +byteVACDAClobyte);
	CBoardComm::Instance().BoardWrite24(24, USB_MID_VAC, m_byteVACDACaddr, dwWdata);

	CString str;
//	str.Format( "Sent: 0x%02X 0x%02X 0x%02X", byteVACDACfirst24, byteVACDAChibyte, byteVACDAClobyte );
	str.Format( "Sent: 0x%X", byteVACDACfirst24 *65536 + byteVACDAChibyte *256 +byteVACDAClobyte );
	GetDlgItem( IDC_EDIT_VACDACsentTB )->SetWindowText( str );
}

void CPanelVS::OnReadComprsCmd() 
{
	VAC_ReadComprs();
}

void CPanelVS::VAC_ReadComprs()
{
	unsigned short data[2];
	if( CSemCtrl::Instance().Serial_ReadbackData( 3, USB_CID_VAC_ReadThermoCCGComp, data) )
	{
		CString str;
		str.Format( "%04X, %04X", data[0], data[1] );
		GetDlgItem(IDC_EDIT_BoardDataTB)->SetWindowText( str );
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
	else
		GetDlgItem(IDC_EDIT_BoardDataTB)->SetWindowText( "" );
}

void CPanelVS::OnCHECKReadVACmprsAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_ReadVACmprsAuto )))->GetCheck();
	if( nCheck )
	{
//		nTimes = 0;
		CMainFrame* frame = (CMainFrame *)theApp.GetMainWnd();
		frame->SetTimers( nMCI_ReadStatusTimerID, TRUE );
	}
	else
	{
		CMainFrame* frame = (CMainFrame *)theApp.GetMainWnd();
		frame->SetTimers( nMCI_ReadStatusTimerID, FALSE );
	}
}

void CPanelVS::IOXguide( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	switch( nPos )
	{
	case	0:
		str += " (Rly Tst A)";
		break;
	case	1:
		str += " (Test Rly 0)";
		break;
	case	2:
		str += " (Test Rly 1)";
		break;
	case	3:
		str += " (Test Rly 2)";
		break;
	case	4:
		str += " (Test Rly 3)";
		break;
	case	5:
		str += " (Test Rly 4)";
		break;
	case	6:
		str += " (Test Rly 5)";
		break;
	case	7:
		str += " (Test Rly 6)";
		break;
	case	8:
		str += " (Test Rly 7)";
		break;
	case	9:
		str += " (Test Rly 8)";
		break;
	case	10:
		str += " (Test Rly 9)";
		break;
	case	11:
		str += " (Test Rly 10)";
		break;
	case	12:
		str += " (Test Rly 11)";
		break;
	case	19:
		str += " (En TC1 Ref.)";
		break;
	case	20:
		str += " (En TC2 Ref.)";
		break;
	case	21:
		str += " (En TC3 Ref.)";
		break;
	case	22:
		str += " (En TC4 Ref.)";
		break;
	case	23:
		str += " (En TC5 Ref.)";
		break;
	case	24:
		str += " (En TC6 Ref.)";
		break;
	case	25:
		str += " (Ext Sw RB2)";
		break;
	case	26:
		str += " (Ext Sw RB1)";
		break;
	case	27:
		str += " (Ext Sw RB0)";
		break;
	case	29:
		str += " (Rly Tst B)";
		break;
	default:
		str += " (not used)";
		break;
	}
	GetDlgItem( IDC_EDIT_IOexpTB )->SetWindowText( str );
}

void CPanelVS::OnIOXstbHiCmd() 
{
	// TODO: Add your control notification handler code here
	BYTE bytePos = m_scrollIOexp.GetScrollPos();
	bytePos |= 0x80;
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip,	// Low byte = ckt addr. of VAC I/O Expander
										USB_TM_VAC_Write_08,		// Serial Comm operand high byte (&H13 = 8 bit write to VAC board)
										0x0,						// OP3 = 0 for 8 bit write
										bytePos );					// Strobe selected line high
}

void CPanelVS::OnIOXstbLoCmd() 
{
	// TODO: Add your control notification handler code here
	BYTE bytePos = m_scrollIOexp.GetScrollPos();
	bytePos |= 0x40;
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip,	// Low byte = ckt addr. of VAC I/O Expander
										USB_TM_VAC_Write_08,		// Serial Comm operand high byte (&H13 = 8 bit write to VAC board)
										0x0,						// OP3 = 0 for 8 bit write
										bytePos );					// Strobe selected line low
}

void CPanelVS::OnCHECKSoftEnableVacuum() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_btnSoftEnVac.GetCheck();
	CheckSoftEnableVacuum( nCheck );
}

void CPanelVS::CheckSoftEnableVacuum( int nCheck )
{
	if( nCheck )
	{
		m_btnSoftEnVac.SetColor( g_crON_Test );
		m_btnSoftEnVac.SetWindowText( "Soft Enable" );
	}
	else
	{
		m_btnSoftEnVac.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnSoftEnVac.SetWindowText( "Soft Disable" );
	}
	InitVacuumControls( (BOOL)nCheck );
}

void CPanelVS::InitVacuumControls( BOOL bCtrlBySoft ) 
{
	if( bCtrlBySoft )
	{
		// "0 (Rly Tst A), 29 (Rly Tst B)" = High
		BYTE bytePos = 0x80 | 0x0;	// 0
		CSemCtrl::Instance().USB_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x1D;		// 29
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "Test Relays 0,1,2,3,5,6" = check on
		bytePos = 0x80 | 0x01;		// 1 (Test Rly 0)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x02;		// 2 (Test Rly 1)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x03;		// 3 (Test Rly 2)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x04;		// 4 (Test Rly 3)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x06;		// 6 (Test Rly 5)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x07;		// 7 (Test Rly 6)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "En TC0,1,2,3" = High (enables TC1,2,3,4 on VAC diagram)
		bytePos = 0x80 | 0x13;		// 19 (En TC1 Ref.)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x14;		// 20 (En TC2 Ref.)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x15;		// 21 (En TC3 Ref.)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x16;		// 22 (En TC4 Ref.)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
	else
	{
		// "En TC0,1,2,3" = Low (enables TC1,2,3,4 on VAC diagram)
		BYTE bytePos = 0x40 | 0x13;	// 19 (En TC1 Ref.)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x14;		// 20 (En TC2 Ref.)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x15;		// 21 (En TC3 Ref.)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x16;		// 22 (En TC4 Ref.)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "Test Relays 0,1,2,3,5,6" = check off
		bytePos = 0x40 | 0x01;		// 1 (Test Rly 0)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x02;		// 2 (Test Rly 1)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x03;		// 3 (Test Rly 2)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x04;		// 4 (Test Rly 3)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x06;		// 6 (Test Rly 5)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x07;		// 7 (Test Rly 6)
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "0 (Rly Tst A), 29 (Rly Tst B)" = Low
		bytePos = 0x40 | 0x0;		// 0
		CSemCtrl::Instance().USB_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
}

void CPanelVS::OnFPGArelayEnChanged( UINT nID )
{
	// Checkboxes used to apply one of two require on controls for relays 0~11
	// Note: Other on/off control is normally supplied by logic, but can be overridden using expander addresses 0 and 29,
	// and if BOTH of those test enable lines are set true, the respective expander I/O line can be activated to test each relay.
	int nIndex = nID -IDC_CHECK_FPGArelayEn0;
	BYTE byteRelayEnAddr = (BYTE)(0x40 + nIndex);	// Address range of 12 relays is &H40~&H4B
	BOOL bCheck = ((CButton*)(GetDlgItem( nID )))->GetCheck();
	CString str;
	if( bCheck )
	{
//		FPGArelayEn(Index).BackColor = &HFF00& 'Set BG color to GREEN to indicate control line has been turned ON
		str.Format( "Sent ON to 0x%02X", byteRelayEnAddr );
	}
	else
	{
//		FPGArelayEn(Index).BackColor = &H8000000B  'Set BG color to GRAY to indicate control line has been turned OFF
		str.Format( "Sent OFF to 0x%02X", byteRelayEnAddr );
	}
	GetDlgItem( IDC_EDIT_FPGArelayEnTB )->SetWindowText( str );

	CSemCtrl::Instance().USB_CommToBoards2(	byteRelayEnAddr,		// OP1 = ckt addr. of VAC circuit address for corresponding relay enable
										USB_TM_VAC_Write_08,	// OP2 = 8 bit write to VAC board)
										0x0,					// OP3 = 0
										bCheck );				// OP4 = set bit 0 high to enable relay or low to disable relay
}

void CPanelVS::OnReadVACextSwitchStates() 
{
	// Before each read, must first select which switch is to be read via the I/O Extender chip (addresses 0x19,0x1A, 0x1B)
	// Open and Closed ("Clsd") Results are sent to label array: VACextSwStatesLabel(index)

	// Example below: Select switch #4 by setting select bits to 1/0/0 (requires 3 back-to-back 8 bit writes to IOX)
	int nWsize, nRsize, nModID;
	BYTE bCktAddr;
	WORD wWdata;	// arg's to pass to CBoardComm::Instance().BoardWrite0816 sub
	BYTE bSwSelect2, bSwSelect1, bSwSelect0;
	for( int nSwNum=0; nSwNum<8; nSwNum++ )
	{
		GetDlgItem( IDC_STATIC_VACextSwStatesLabel0 +nSwNum )->SetWindowText( "unknown" );

		bSwSelect2	= nSwNum & 0x4;
		bSwSelect1	= nSwNum & 0x2;
		bSwSelect0	= nSwNum & 0x1;

		nWsize		= 8;
		nModID		= USB_MID_VAC;
		bCktAddr	= USB_CID_VAC_ExpanderChip;	// Circuit address of I/O Expander 0x72

		if( bSwSelect2 )
			wWdata	= 0x19 | 0x80;	// Strobe select line 2@0x19 to a high state
		else
			wWdata	= 0x19 | 0x40;	// Strobe select line 2@0x19 to a low state
		CBoardComm::Instance().BoardWrite0816(nWsize, nModID, bCktAddr, wWdata);

		if( bSwSelect1 )
			wWdata	= 0x1A | 0x80;	// Strobe select line 1@0x1A to a high state
		else
			wWdata	= 0x1A | 0x40;	// Strobe select line 1@0x1A to a low state
		CBoardComm::Instance().BoardWrite0816(nWsize, nModID, bCktAddr, wWdata);

		if( bSwSelect0 )
			wWdata	= 0x1B | 0x80;	// Strobe select line 0@0x1B to a high state
		else
			wWdata	= 0x1B | 0x40;	// Strobe select line 0@0x1B to a low state
		CBoardComm::Instance().BoardWrite0816(nWsize, nModID, bCktAddr, wWdata);

		bCktAddr	= 0x98;			// USB_CID_VAC_ReadSelExtSwitchState
		nRsize		= 16;
		wWdata		= 0;

		CString strMsgCounts, strMsgData;
		CBoardComm::Instance().BoardRead0816( nRsize, nModID, bCktAddr, wWdata, strMsgCounts, strMsgData, TRUE );
		GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsgData );

		// BoardReadDatBuf(256, 2) '2nd element is type: "FFFF"=9, marker "D22D"=4, Board Readback Data=1, (0,0)=count
		int nNumWords = CBoardComm::Instance().m_nBoardReadDataBuf[0];	// # of words read from board stored in Read Subroutine
		int nBoardData, nDataType;
		for( int i=1; i<=nNumWords; i++ )	// For I = 1 To NumWords
		{
			nBoardData	= CBoardComm::Instance().m_nBoardReadDataBuf[i*2];
			nDataType	= CBoardComm::Instance().m_nBoardReadDataBuf[i*2+1];
			CString str;
			if( nDataType == 1 )	// Board readback data - use it
			{
				if( nBoardData & 1 )
				{
					str.Format( "S %d:OPEN", nSwNum );
					GetDlgItem( IDC_STATIC_VACextSwStatesLabel0 +nSwNum )->SetWindowText( str );
//					VACextSwStatesLabel(SwNum).BackColor = &H8000000F  'White
				}
				else
				{
					str.Format( "S %d:CLSD", nSwNum );
					GetDlgItem( IDC_STATIC_VACextSwStatesLabel0 +nSwNum )->SetWindowText( str );
//					VACextSwStatesLabel(SwNum).BackColor = &H8080FF 'Light red
				}
			}
		}
	}
/*
Dim BoardDat, I, NumWords As Integer
Dim BoardData$ '2 or 4 char string rep of above
Dim SwNum, DatType As Integer 'Keeps track of which switch is being fed into readback circuit
Dim SwSelect2, SwSelect1, SwSelect0 As Integer 'Individual mux select values (varies per switch selected)

For SwNum = 0 To 7

      
 'BoardReadDatBuf(256, 2) '2nd element is type: "FFFF"=9, marker "D22D"=4, Board Readback Data=1, (0,0)=count
  NumWords = BoardReadDatBuf(0, 0) '# of words read from board stored in Read Subroutine

   For I = 1 To NumWords
     BoardDat = BoardReadDatBuf(I, 1)
     BoardData$ = BoardRBdatBuf$(I, 1)
     DatType = BoardReadDatBuf(I, 2)
     If DatType = 1 Then 'Board readback data - use it
       If (BoardDat And 1) Then 'Check state of LSB only
         VACextSwStatesLabel(SwNum).Caption = "S" + Str$(SwNum) + ":" + "OPEN"
         VACextSwStatesLabel(SwNum).BackColor = &H8000000F  'White
       Else
         VACextSwStatesLabel(SwNum).Caption = "S" + Str$(SwNum) + ":" + "CLSD"
         VACextSwStatesLabel(SwNum).BackColor = &H8080FF 'Light red
       End If
     End If
              
   Next I
 Next SwNum*/
}

void CPanelVS::OnVACtestLED4() 
{
	// Utilizes VACTestLED4State global, temporarily uses CL wobbler On ckt address (0x1c)
	m_bVACtestLED4 = !m_bVACtestLED4;
	if( m_bVACtestLED4 )
	{
		m_btnVACtestLED4.SetColor( g_crON_Test );
//		m_btnVACtestLED4.SetWindowText( "Test LED is now ON" );
	}
	else
	{
		m_btnVACtestLED4.SetColor( g_crOFF );
//		m_btnVACtestLED4.SetWindowText( "Test LED is now OFF" );
	}

	///////////////////////////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_Test_LED,		// Low byte = ckt addr. of Test LED 16
										USB_TM_VAC_Write_08,		// Serial Comm operand high byte (8 bit write to VAC board)
										0x0,						// OP3 = 0 for 8 bit write
										(BYTE)m_bVACtestLED4 );		// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CPanelVS::OnSPRtestLED4() 
{
	// Utilizes SPRtestLED4State global, temporarily uses CL wobbler On ckt address (0x1c)
	m_bSPRtestLED4 = !m_bSPRtestLED4;
	if( m_bSPRtestLED4 )
	{
		m_btnSPRtestLED4.SetColor( g_crON_Test );
//		m_btnSPRtestLED4.SetWindowText( "Test LED is now ON" );
	}
	else
	{
		m_btnSPRtestLED4.SetColor( g_crOFF );
//		m_btnSPRtestLED4.SetWindowText( "Test LED is now OFF" );
	}

	///////////////////////////////////////////////////////////////////////////
//	CBoardComm::Instance().BoardWrite0816(8, USB_MID_SPR, USB_CID_SPR_Test_LED, m_bSPRtestLED4);
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_Test_LED,		// Low byte = ckt addr. of Test LED 16
										USB_TM_SPR_Write_08,		// Serial Comm operand high byte (8 bit write to SPR board)
										0x0,						// OP3 = 0 for 8 bit write
										(BYTE)m_bSPRtestLED4 );		// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CPanelVS::OnVACsoftReset() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	nWsize		= 8;
	nModID		= USB_MID_VAC;
	bCktAddr	= USB_CID_VAC_Soft_Reset;
	wData		= 0;
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	// Force DAC control sliders and resulting reports to 0:
}

void CPanelVS::OnSPRsoftReset() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_Soft_Reset;
	wData		= 0;
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnVACreadBoardID() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ReadBoardID,
										USB_TM_VAC_Read_08,
										0x0,
										0x0 );

	// **************** READ PORTION ******************** //
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
//	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
//	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	///////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_ReadBoardData( ReadBack_DataSize, CBoardComm::Instance().m_pnReadCommonData, &lBytesTransferred );
	UpdateReadbackData( CBoardComm::Instance().m_pnReadCommonData );
	///////////////////////////////////////////////////////
/*
	CSemCtrl::Instance().USB_StreamOpen( TRUE );	// TRUE for read; FALSE for write.
	CSemCtrl::Instance().USB_StreamFrame(lReadSize, pnFrameData, &lBytesTransferred, -1 );
	CSemCtrl::Instance().USB_StreamClose();
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();//TIME_METHOD_WAIT_INPUT_ENABLE);

	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", pnFrameData[0] );
	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = "";
//	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
//	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
//	pParent->m_listReadback.ResetContent();
	BOOL bFound = FALSE;
	for( unsigned long i=1; i<=pnFrameData[0]; i++ )
	{
		str.Format( "%04X, ", pnFrameData[i] );
		strMsg += str;
		strList.Format( "@ %d: 0x%04X", i, pnFrameData[i] );
//		pParent->m_listReadback.AddString( strList );
	}
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
*/
//	delete [] pnFrameData;
}

void CPanelVS::OnSPRreadBoardID() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_ReadBoardID,
										USB_TM_SPR_Read_08,
										0x0,
										0x0 );

	// **************** READ PORTION ******************** //
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
//	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
//	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	///////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_ReadBoardData( ReadBack_DataSize, CBoardComm::Instance().m_pnReadCommonData, &lBytesTransferred );
	UpdateReadbackData( CBoardComm::Instance().m_pnReadCommonData );
	///////////////////////////////////////////////////////
/*
	CSemCtrl::Instance().USB_StreamOpen( TRUE );	// TRUE for read; FALSE for write.
	CSemCtrl::Instance().USB_StreamFrame(lReadSize, pnFrameData, &lBytesTransferred, -1 );
	CSemCtrl::Instance().USB_StreamClose();
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();//TIME_METHOD_WAIT_INPUT_ENABLE);

	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", pnFrameData[0] );
	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = "";
//	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
//	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
//	pParent->m_listReadback.ResetContent();
	BOOL bFound = FALSE;
	for( unsigned long i=1; i<=pnFrameData[0]; i++ )
	{
		str.Format( "%04X, ", pnFrameData[i] );
		strMsg += str;
		strList.Format( "@ %d: 0x%04X", i, pnFrameData[i] );
//		pParent->m_listReadback.AddString( strList );
	}
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
*/
//	delete [] pnFrameData;
}

void CPanelVS::OnLocalLoopback() 
{
	unsigned short data[256];
	if( CSemCtrl::Instance().Serial_ReadbackData( -1, 0x0, data) )
		UpdateReadbackData( data );
}

void CPanelVS::OnFansOnFPGA() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue
	m_bFansOnFPGA = !m_bFansOnFPGA;
	if( m_bFansOnFPGA )
	{
		m_btnFansOnFPGA.SetColor( g_crON_Test );
//		m_btnFansOnFPGA.SetWindowText( "F:Turn Fans OFF (Fans are now ON)" );
	}
	else
	{
		m_btnFansOnFPGA.SetColor( g_crOFF );
//		m_btnFansOnFPGA.SetWindowText( "F:Turn Fans ON (Fans are now OFF)" );
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_Fan_PWM_Control;
	wData		= (WORD)m_bFansOnFPGA;	// 1 or 0 for now - PWM speed control possibly in future
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnFansOnPLD() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bFansOnPLD = !m_bFansOnPLD;
	if( m_bFansOnPLD )
	{
		m_btnFansOnPLD.SetColor( g_crON_Test );
//		m_btnFansOnPLD.SetWindowText( "P:Turn Fans OFF (Fans are now ON)" );
		wData	= 0x87;		// Strobe IOX output @0A high
	}
	else
	{
		m_btnFansOnPLD.SetColor( g_crOFF );
//		m_btnFansOnPLD.SetWindowText( "P:Turn Fans ON (Fans are now OFF)" );
		wData	= 0x47;		// Strobe IOX output @0A low
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnSPRRelay1En() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bSPRRelay1En= !m_bSPRRelay1En;
	if( m_bSPRRelay1En )
	{
		m_btnSPRRelay1En.SetColor( g_crEmphasis3 );
//		m_btnSPRRelay1En.SetWindowText( "Turn SPR Relay 1 OFF (Now ON)" );
		wData	= 0x82;		// Strobe IOX output @0A high
	}
	else
	{
		m_btnSPRRelay1En.SetColor( g_crOFF );
//		m_btnSPRRelay1En.SetWindowText( "Turn SPR Relay 1 ON (Now OFF)" );
		wData	= 0x42;		// Strobe IOX output @0A low
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnSPRRelay2En() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bSPRRelay2En= !m_bSPRRelay2En;
	if( m_bSPRRelay2En )
	{
		m_btnSPRRelay2En.SetColor( g_crEmphasis3 );
//		m_btnSPRRelay2En.SetWindowText( "Turn SPR Relay 2 OFF (Now ON)" );
		wData	= 0x83;		// Strobe IOX output @0A high
	}
	else
	{
		m_btnSPRRelay2En.SetColor( g_crOFF );
//		m_btnSPRRelay2En.SetWindowText( "Turn SPR Relay 2 ON (Now OFF)" );
		wData	= 0x43;		// Strobe IOX output @0A low
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnHVOn() 
{
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bHVOn= !m_bHVOn;
	if( m_bHVOn )
	{
		m_btnHVOn.SetColor( g_crEmphasis4 );
//		m_btnHVOn.SetWindowText( "J12: HV Enable" );
		wData		= 0x89;		// Strobe IOX output HVonA high
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		wData		= 0x8C;		// Strobe IOX output HVonB high
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	}
	else
	{
		m_btnHVOn.SetColor( g_crOFF );
//		m_btnHVOn.SetWindowText( "J12: HV Disable" );
		wData		= 0x49;		// Strobe IOX output HVonA low
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		wData		= 0x4C;		// Strobe IOX output HVonB low
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	}
}

void CPanelVS::OnLegHVOnOff() 
{
}

void CPanelVS::LegHVUpdate( int nPos )
{
	// Uses globals: LegHVctlValue, LegHVapproxVoltage
	WORD wData = (WORD)nPos;
	if( wData == m_wposHV )
		return;
	m_wposHV = wData;
	CString str;
	str.Format( "%ld", nPos );
	GetDlgItem( IDC_EDIT_LegHVSBvalueTB )->SetWindowText( str );
	str.Format( "%.3f", 1.0 *nPos /511 *30000 );
	GetDlgItem( IDC_EDIT_LegHVprogValueTB )->SetWindowText( str );

	CBoardComm::Instance().BoardWrite0816( 16, USB_MID_SPR, USB_CID_SPR_Legacy_HV_Power, wData );
}

void CPanelVS::OnSwitchAnalogPowerSupplies() 
{
	int nCheck = m_btnAnalogPS.GetCheck();
	SwitchAnalogPowerSupplies( nCheck );
}

void CPanelVS::SwitchAnalogPowerSupplies( int nCheck )
{
	// TODO: Add your control notification handler code here
	if( nCheck )
	{
		m_btnAnalogPS.SetColor( g_crON_Test );
//		m_btnAnalogPS.SetWindowText( "Shutdown Analog Power Supplies (Now ON)" );
	}
	else
	{
		m_btnAnalogPS.SetColor( g_crScanEmphasis );
//		m_btnAnalogPS.SetWindowText( "Startup Analog Power Supplies (Now OFF)" );
	}

	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_AnalogPower_Enable;
	wData		= (WORD)nCheck;
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnInh20V() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: Inh20V (located at dual I/O Expander addresses 0x19 and 0x20) IOX is @ 0x72
	m_bInh20V= !m_bInh20V;
	if( m_bInh20V )
	{
		m_btnInh20V.SetColor( g_crScanEmphasis );
//		m_btnInh20V.SetWindowText( "Turn +/-20V Power OFF (Now ON)" );
		wData	= 0x99;		// Strobe IOX output @25 high (bit 7 plus address = 1001 1001)
	}
	else
	{
		m_btnInh20V.SetColor( g_crOFF );
//		m_btnInh20V.SetWindowText( "Turn +/-20V Power ON (Now OFF)" );
		wData	= 0x59;		// Strobe IOX output @25 low (bit 6 plus address = 0101 1001)
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

	wData		+= 1;		// Also strobe next address @ 26 (requires dual writes to inhibit)
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnInh13V() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: Inh13V (located at dual I/O Expander addresses 22,23: 0x16 and 0x17) IOX is @ 0x72
	m_bInh13V= !m_bInh13V;
	if( m_bInh13V )
	{
		m_btnInh13V.SetColor( g_crScanEmphasis );
//		m_btnInh13V.SetWindowText( "Turn +/-13V Power OFF (Now ON)" );
		wData	= 0x96;		// Strobe IOX output @16 high (bit 7 plus address = 1001 0110)
	}
	else
	{
		m_btnInh13V.SetColor( g_crOFF );
//		m_btnInh13V.SetWindowText( "Turn +/-13V Power ON (Now OFF)" );
		wData	= 0x56;		// Strobe IOX output @16 low (bit 6 plus address = 0101 0110)
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

	wData		+= 1;		// Also strobe next address @ 23 (requires dual writes to inhibit)
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnInh8V() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: Inh8V (located at dual I/O Expander addresses 0x13 and 0x14) IOX is @ 0x72
	m_bInh8V= !m_bInh8V;
	if( m_bInh8V )
	{
		m_btnInh8V.SetColor( g_crScanEmphasis );
//		m_btnInh8V.SetWindowText( "Turn +/-8V Power OFF (Now ON)" );
		wData	= 0x93;		// Strobe IOX output @13 high (bit 7 plus address = 1001 0011)
	}
	else
	{
		m_btnInh8V.SetColor( g_crOFF );
//		m_btnInh8V.SetWindowText( "Turn +/-8V Power ON (Now OFF)" );
		wData	= 0x53;		// Strobe IOX output @13 low (bit 6 plus address = 0101 0011)
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

	wData		+= 1;		// Also strobe next address @ 1A (requires dual writes to inhibit)
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPanelVS::OnTestR8() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_PowerStatus_Readback,	// OP1 = ckt addr. of SPR Power supply status word (bits 2-0 only)
										USB_TM_SPR_Read_08,				// OP2 = 0x94 (8 bit READ from SPR board)
										0x0,								// OP3: n/a, default to 0
										0x0 );								// OP4: n/a, default to 0
}

void CPanelVS::OnTestReadSPRswitches() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_ReadExternalSwitch,		// OP1 = ckt addr. of SPR External switch closure sensors (bits 3-0 only)
										USB_TM_SPR_Read_08,				// OP2 = 0x94 (8 bit READ from SPR board)
										0x0,								// OP3: n/a, default to 0
										0x0 );								// OP4: n/a, default to 0
}

void CPanelVS::OnTestR16() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ADC_Read,				// OP1 = ckt addr. of ADC0
										USB_TM_LNS_Read_16,				// OP2 = 0xA2 (16 bit read from LNS board)
										0x0,								// OP3 = 0
										0xA4 );								// OP4: 2nd byte of 2 to ADC. 0xA4=Select Ch 4 (SE),+/-10V dyn. range
}

void CPanelVS::OnTestR32() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
										USB_TM_SPR_Read_32,				// operand 2 = 0xC4 (32 bit READ from SPR board)
										0x60,								// OP3: 2nd byte of 2 to ADC: 0x60 = select OSR level 6, TWOX=false
										0xBA );								// OP4: 1st byte of 2 to ADC: 0xBA = Select +3.3V monitor (ADC1 Ch. 5)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
										USB_TM_SPR_Read_32,				// operand 2 = 0xC4 (32 bit READ from SPR board)
										0x60,								// OP3: 2nd byte of 2 to ADC: 0x60 = select OSR level 6, TWOX=false
										0x9A );								// OP4: 1st byte of 2 to ADC, 2nd time without changing parameters

//	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
//										USB_TM_SPR_Read_32,					// operand 2 = 0xC4 (32 bit READ from SPR board)
//										0x60,								// OP3: 2nd byte of 2 to ADC: 0x60 = select OSR level 6, TWOX=false
//										0xBA );								// OP4: 1st byte of 2 to ADC: 0xBA = Select +3.3V monitor (ADC1 Ch. 5)
}

void CPanelVS::OnReadSPRADC() 
{
	// As of early Feb 2012, An EVEN number of reads has to be handed off to USB interface in PC's circuit, so the ADC read
	// is executed 2x.  This also provides the add'l benefit of the ADC's returning the results of the prior conversion.
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
										USB_TM_SPR_Read_32,				// operand 2 = 0xC4 (32 bit READ from SPR board)
										0x40,								// OP3: 2nd byte of 2 to ADC: 0x30 = select OSR level 4 (880Hz/1.18mS), TWOX=false
										0xB0 );								// OP4: 1st byte of 2 to ADC: 0xB0 = Ext analog Ch 1 (P5 input), EN change settings

	// Insert wait here?
//	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
//										USB_TM_SPR_Read_32,					// operand 2 = 0xC4 (32 bit READ from SPR board)
//										0x40,								// OP3: 2nd byte of 2 to ADC: 0x40 = select OSR level 4 (880Hz/1.18mS), TWOX=false
//										0x90 );								// OP4: 1st byte of 2 to ADC: 0xB0 = = Ext analog Ch 1 (P5 input), no change to settings

	// **************** READ PORTION ******************** //
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
//	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
//	ZeroMemory(pnFrameData, sizeof(pnFrameData));
	
	///////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_ReadBoardData( ReadBack_DataSize, CBoardComm::Instance().m_pnReadADCData, &lBytesTransferred );
	UpdateReadbackData( CBoardComm::Instance().m_pnReadADCData );
	///////////////////////////////////////////////////////
/*
	CSemCtrl::Instance().USB_StreamOpen( TRUE );	// TRUE for read; FALSE for write.
	CSemCtrl::Instance().USB_StreamFrame( lReadSize, pnFrameData, &lBytesTransferred, -1 );
	CSemCtrl::Instance().USB_StreamClose();
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();//TIME_METHOD_WAIT_INPUT_ENABLE);

	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", lBytesTransferred /2 );
//	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = str;
	theApp.m_nRBwdCnt = 0;
	for( unsigned long i=0; i<lBytesTransferred/2; i++ )
	{
		str.Format( "%04X, ", pnFrameData[i] );
		strMsg += str;
		strList.Format( "@ %ld (hex %04X)", pnFrameData[i], pnFrameData[i] );
//		pParent->m_listReadback.AddString( strList );
	}
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
*/
//	delete [] pnFrameData;
}

void CPanelVS::OnReadVACADC() 
{
	// read instruction must be executed twice. The first one sets up the ADC channel and starts the conversion, and the 2nd
	// one retrieves the desired data.  If the time between these two read instructions is too short, then checking of the ADC
	// busy line is required before valid read data is available.
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on VACx board
										USB_TM_VAC_Read_32,					// operand 2 = 0xC3 (32 bit READ from VAC board)
										0x40,								// OP3: 2nd byte of 2 to ADC: 0x30 = select OSR level 4 (880Hz/1.18mS), TWOX=false
										0xBE );								// OP4: 1st byte of 2 to ADC: 0xBE = Ch 14, EN change settings

	// 2nd pass...
//	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on VACx board
//										USB_TM_VAC_Read_32,					// operand 2 = 0xC3 (32 bit READ from VAC board)
//										0x40,								// OP3: 2nd byte of 2 to ADC: 0x40 = select OSR level 4 (880Hz/1.18mS), TWOX=false
//										0x9E );								// OP4: 1st byte of 2 to ADC: 0x9E = Ch 14, no change to settings

	// **************** READ PORTION ******************** //
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
//	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
//	ZeroMemory(pnFrameData, sizeof(pnFrameData));
	
	///////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_ReadBoardData( ReadBack_DataSize, CBoardComm::Instance().m_pnReadADCData, &lBytesTransferred );
	UpdateReadbackData( CBoardComm::Instance().m_pnReadADCData );
	///////////////////////////////////////////////////////
/*
	CSemCtrl::Instance().USB_StreamOpen( TRUE );	// TRUE for read; FALSE for write.
	CSemCtrl::Instance().USB_StreamFrame( lReadSize, pnFrameData, &lBytesTransferred, -1 );
	CSemCtrl::Instance().USB_StreamClose();
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();//TIME_METHOD_WAIT_INPUT_ENABLE);

	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", lBytesTransferred /2 );
//	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = str;
	theApp.m_nRBwdCnt = 0;
	for( unsigned long i=0; i<lBytesTransferred/2; i++ )
	{
		str.Format( "%04X, ", pnFrameData[i] );
		strMsg += str;
		strList.Format( "@ %ld (hex %04X)", pnFrameData[i], pnFrameData[i] );
//		pParent->m_listReadback.AddString( strList );
	}
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
*/
//	delete [] pnFrameData;
}

void CPanelVS::OnADCselectOBChanged( UINT nID )
{
	int nIndex = nID -IDC_RADIO_ADCselectOB0;
	// show assignments for each of the channels (16 for all except for LNS board, which as 8)
	CString str;
	for( int i=0; i<16; i++ )
	{
		str.Format( "Ch %d: %s", i, CControlLayer::Instance().m_DL.adc.chn[nIndex*16+i].strLabel );
		GetDlgItem( IDC_RADIO_ADCsChSelect0 +i )->SetWindowText( str );
	}
	CBoardComm::Instance().ChangedADCselectOB( nIndex );

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
}

void CPanelVS::OnADCsChSelectChanged( UINT nID )
{
	int nIndex = nID -IDC_RADIO_ADCsChSelect0;
	CBoardComm::Instance().ChangedADCsChSelect( nIndex,CControlLayer::Instance().m_DL.adc.chn );

	CString str;
	str.Format( "Press select %02d chn", nIndex );
	theApp.DisplayMsg(str);
}

void CPanelVS::OnCHECKReadSelectedADCAuto() 
{
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_ReadSelectedADCAuto )))->GetCheck();
	if( nCheck )
		SetTimer( 2, 2000, NULL );
	else
		KillTimer( 2 );
}

void CPanelVS::OnReadSelectedADC() 
{
	theApp.DisplayMsg("Press read selected ADC");
	CString str;
	if( CBoardComm::Instance().m_bADCtype )
		str = CBoardComm::Instance().ReadSelectedADC_Procedure( 100, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log, FALSE );
	else
		str = CBoardComm::Instance().ReadSelectedADC_Procedure( 70, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log, FALSE );
	GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
	ADCvconv();
}

void CPanelVS::ADCvconv()
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

void CPanelVS::OnWDTSelfResetToggle() 
{
	// Utilizes MCIcommSelfResetEn global
	m_bWDTSelfReset= !m_bWDTSelfReset;
	if( m_bWDTSelfReset )
	{
		m_btnWDTSelfReset.SetColor( g_crON_Test );
		m_btnWDTSelfReset.SetWindowText( "Comm Inactivity Self-Init is ON" );
	}
	else
	{
		m_btnWDTSelfReset.SetColor( g_crOFF );
		m_btnWDTSelfReset.SetWindowText( "Comm Inactivity Self-Init is OFF" );
	}

	CBoardComm::Instance().BoardWrite0816( 8, USB_MID_MCI2, 0x0, (BYTE)m_bWDTSelfReset );
}

void CPanelVS::OnSPRDACselOptionChanged( UINT nID )
{
	m_nSPRDACchannel = nID -IDC_RADIO_SPRDACselOption0;
	SPRDACselOptionChanged();
}

void CPanelVS::SPRDACselOptionChanged()
{
	CString str = "";
	switch( m_nSPRDACchannel )
	{
	case	0:							// DAC Channel A
//		m_byteSPRDACselBits	= 0x10;		// upper byte value for 24 bit write and load for DAC channel A
		str					= "Ch A";
		break;
	case	1:							// DAC Channel B
//		m_byteSPRDACselBits	= 0x14;		// upper byte value for 24 bit write and load for DAC channel B
		str					= "Ch B";
		break;
	case	2:							// DAC Channel C
//		m_byteSPRDACselBits	= 0x18;		// upper byte value for 24 bit write and load for DAC channel C
		str					= "Ch C";
		break;
	case	3:							// DAC Channel D
//		m_byteSPRDACselBits	= 0x1C;		// upper byte value for 24 bit write and load for DAC channel D
		str					= "Ch D";
		break;
	default:
		break;
	}
	GetDlgItem( IDC_EDIT_WhichSPRDACTB )->SetWindowText( str );
	// Force slider to last value (or initial value if not yet set)
	m_scrollSPRDACsVal.SetScrollPos(CControlLayer::Instance().m_nSPRDACvalues[m_nSPRDACchannel] );
	SPRDACsValUpdae( CControlLayer::Instance().m_nSPRDACvalues[m_nSPRDACchannel] );
}

void CPanelVS::SPRDACsValUpdae( int nPos )
{
	// Quad 16 bit general purpose DAC on SPR board.  Utilizes globals: SPRDACnewVal,SPRDAC1, SPRDAC2, SPRDAC3, SPRDAC4
	// Because of the configurability of the output buffers that include unipolar and bipolar options, RSTSEL
	// is set to low (resets to 0) which sets input coding to straight binary.

	int nMinpos, nMaxpos;
	m_scrollSPRDACsVal.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	m_nSPRDACnewVal = nPos + 32768;
	CString str;
	str.Format( "%d", m_nSPRDACnewVal );
//	GetDlgItem( IDC_EDIT_SPRDACsValTB )->SetWindowText( str );
	str.Format( "%.6f V", 2.5 *m_nSPRDACnewVal /65536 );
	GetDlgItem( IDC_EDIT_SPRDACvoltsTB )->SetWindowText( str );
	str.Format( "%04X", m_nSPRDACnewVal );
	GetDlgItem( IDC_EDIT_SPRDACsValHexTB )->SetWindowText( str );
	CControlLayer::Instance().m_nSPRDACvalues[m_nSPRDACchannel] = nPos;	// Keep history of user setting
	CControlLayer::Instance().m_DL.diag[Diag_SPRDAC_ChA + m_nSPRDACchannel].nCurPos = nPos;
}

void CPanelVS::OnSPRDACupdate() 
{
	SPRDACupdate();
}

void CPanelVS::SPRDACupdate()
{
	BYTE bSPRDAChibyte, bSPRDAClobyte, bSPRDACfirst24;
	DWORD dwWdata;

	bSPRDAChibyte	= (BYTE)((m_nSPRDACnewVal & 0xFF00) / 256);	// Shift down 8 bits to properly byte-align
	bSPRDAClobyte	= (BYTE)(m_nSPRDACnewVal & 0xFF);
	bSPRDACfirst24	= (BYTE)( 0x10 | (m_nSPRDACchannel * 2) );	// Selects immediate load, channel select shifted up 1 bit

	dwWdata = (DWORD)(bSPRDACfirst24 * 65536 + m_nSPRDACnewVal);

	CString str;
	str.Format( "0x%X", (DWORD)(bSPRDACfirst24 *65536 + bSPRDAChibyte *256 + bSPRDAClobyte) );
	GetDlgItem( IDC_EDIT_SPRDACsentTB )->SetWindowText( str );
 
	CBoardComm::Instance().BoardWrite24(24, USB_MID_SPR, USB_CID_SPR_General_Purpose_DACs, dwWdata);
}

void CPanelVS::OnBoardCommRx() 
{
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
//	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
//	ZeroMemory(pnFrameData, sizeof(pnFrameData));
	
	///////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_ReadBoardData( ReadBack_DataSize, CBoardComm::Instance().m_pnReadCommonData, &lBytesTransferred );
	UpdateReadbackData( CBoardComm::Instance().m_pnReadCommonData );
	///////////////////////////////////////////////////////

/*	CSemCtrl::Instance().USB_StreamOpen( TRUE );	// TRUE for read; FALSE for write.
	CSemCtrl::Instance().USB_StreamFrame( lReadSize, pnFrameData, &lBytesTransferred, -1 );
	CSemCtrl::Instance().USB_StreamClose();
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();//TIME_METHOD_WAIT_INPUT_ENABLE);

	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", lBytesTransferred /2 );
//	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = "Start:" + str;
	theApp.m_nRBwdCnt = 0;
	for( unsigned long i=0; i<lBytesTransferred/2; i++ )
	{
		str.Format( "%04X, ", pnFrameData[i] );
		strMsg += str;
		strList.Format( "@ %d: 0x%04X", i, pnFrameData[i] );
//		pParent->m_listReadback.AddString( strList );

//		theApp.m_nRBwdCnt++;
//		CBoardComm::Instance().m_nBoardReadDataBuf[ theApp.m_nRBwdCnt*2 ] = pnFrameData[i];	// Numeric value (0-FFFF range)
	}
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
*/
//	delete [] pnFrameData;
}

void CPanelVS::OnClearFIFO() 
{
	// TODO: Add your control notification handler code here
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();
}

void CPanelVS::OnClearList() 
{
	// TODO: Add your control notification handler code here
/*	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
	pParent->m_listReport.ResetContent();*/
}

void CPanelVS::UpdateReadbackData( unsigned short* pnData )
{
	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", pnData[0] );
	
	strMsg = str;
	unsigned long lCount = pnData[0];
	for( unsigned long i=1; i<=lCount; i++ )
	{
		str.Format( "%04X, ", pnData[i] );
		strList.Format( "@ %d: 0x%04X", i, pnData[i] );
		strMsg += strList;
		strMsg += "\r\n";
	}
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPanelVS::InitDiagnose( BOOL bEnd )
{
	KillTimer( 1 );
	KillTimer( 2 );
/*
	for( int i=0; i<16; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_ADCsChSelect0 +i )))->SetCheck( 0 );
	for( i=0; i<6; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_ADCselectOB0 +i )))->SetCheck( 0 );

	SetPxlClkto2();

	GetDlgItem( IDC_BUTTON_ReadSelectedADCcmd )->EnableWindow( bEnd );
	GetDlgItem( IDC_CHECK_ReadSelectedADCAuto )->EnableWindow( bEnd );*/
}

void CPanelVS::SetTimers( UINT nIDEvent, BOOL bSet )
{
	if( nIDEvent == Diag_Total )
	{
		// Logging Param
		if( bSet )
			SetTimer( Diag_Total +100, CControlLayer::Instance().m_DL.other.nElapseLogging, NULL );
		else
			KillTimer( Diag_Total +100 );
		return;
	}

	// Changing Param
	if( bSet )
	{
		if( (nIDEvent >= Diag_CalI_TC0) && (nIDEvent <= Diag_SetPoint_TC5) )
		{
			// VAC board
			switch( nIDEvent )
			{
			case	Diag_CalI_TC0:
				m_nVACDACselected	= 0;
				m_nVACDACchannel	= 0;
				break;
			case	Diag_CalI_TC1:
				m_nVACDACselected	= 0;
				m_nVACDACchannel	= 1;
				break;
			case	Diag_CalI_TC2:
				m_nVACDACselected	= 0;
				m_nVACDACchannel	= 2;
				break;
			case	Diag_CalI_TC3:
				m_nVACDACselected	= 0;
				m_nVACDACchannel	= 3;
				break;
			case	Diag_CalI_TC4:
				m_nVACDACselected	= 2;
				m_nVACDACchannel	= 0;
				break;
			case	Diag_CalI_TC5:
				m_nVACDACselected	= 2;
				m_nVACDACchannel	= 1;
				break;
			case	Diag_SetPoint_TC0:
				m_nVACDACselected	= 1;
				m_nVACDACchannel	= 0;
				break;
			case	Diag_SetPoint_TC1:
				m_nVACDACselected	= 1;
				m_nVACDACchannel	= 1;
				break;
			case	Diag_SetPoint_TC2:
				m_nVACDACselected	= 1;
				m_nVACDACchannel	= 2;
				break;
			case	Diag_SetPoint_TC3:
				m_nVACDACselected	= 1;
				m_nVACDACchannel	= 3;
				break;
			case	Diag_SetPoint_TC4:
				m_nVACDACselected	= 2;
				m_nVACDACchannel	= 2;
				break;
			case	Diag_SetPoint_TC5:
				m_nVACDACselected	= 2;
				m_nVACDACchannel	= 3;
				break;
			}
			VACDACgroupOptionChanged();
			VACDACselOptionChanged();
			VACDACvalUpdate( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			m_scrollVACDACsVal.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			VACDACupdate( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
		}
		else
		{
			// SPR board
			m_nSPRDACchannel = nIDEvent -Diag_SPRDAC_ChA;
			SPRDACselOptionChanged();
			SPRDACsValUpdae( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			SPRDACupdate();
		}
//		SetTimer( nIDEvent +100, CControlLayer::Instance().m_DL.diag[nIDEvent].nIntv, NULL );
	}
	else
		KillTimer( nIDEvent +100 );
}
