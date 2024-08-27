// PageVS.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "PageVS.h"
#include "TabDialog.h"
#include "Insample0Dlg.h"

#include "TimeStatistic.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int nTimes = 0;

/////////////////////////////////////////////////////////////////////////////
// CPageVS dialog


CPageVS::CPageVS(CWnd* pParent /*=NULL*/)
	: CDialog(CPageVS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageVS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bmpON.LoadBitmap(IDB_BITMAP_LED_BLUE_H);
	m_bmpOFF.LoadBitmap(IDB_BITMAP_LED_BLUE_D);
	m_bGraph = m_bRecord = FALSE;
}


void CPageVS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageVS)
	DDX_Control(pDX, IDC_BUTTON_Relay4EnCmd, m_btnSPRRelay4En);
	DDX_Control(pDX, IDC_BUTTON_Relay3EnCmd, m_btnSPRRelay3En);
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


BEGIN_MESSAGE_MAP(CPageVS, CDialog)
	//{{AFX_MSG_MAP(CPageVS)
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
	ON_BN_CLICKED(IDC_BUTTON_StartScanCmd, OnStartScan)
	ON_BN_CLICKED(IDC_BUTTON_StopScanCmd, OnStopScan)
	ON_BN_CLICKED(IDC_BUTTON_SetPxlClkto2Cmd, OnSetPxlClkto2)
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
	ON_BN_CLICKED(IDC_BUTTON_WDT_SelfResetToggleCmd, OnWDTSelfResetToggle)
	ON_BN_CLICKED(IDC_BUTTON_SPRDACupdateCmd, OnSPRDACupdate)
	ON_BN_CLICKED(IDC_CHECK_ReadVACmprsAuto, OnCHECKReadVACmprsAuto)
	ON_BN_CLICKED(IDC_BUTTON_BoardCommRxCmd, OnBoardCommRx)
	ON_BN_CLICKED(IDC_CHECK_Switch_Analog_Power_Supplies, OnSwitchAnalogPowerSupplies)
	ON_BN_CLICKED(IDC_BUTTON_Relay2EnCmd, OnSPRRelay2En)
	ON_BN_CLICKED(IDC_BUTTON_VACreadBoardID, OnVACreadBoardID)
	ON_BN_CLICKED(IDC_BUTTON_SPRreadBoardID, OnSPRreadBoardID)
	ON_BN_CLICKED(IDC_CHECK_SoftEnable_Vacuum, OnCHECKSoftEnableVacuum)
	ON_BN_CLICKED(IDC_CHECK_ReadSelectedADCAuto, OnCHECKReadSelectedADCAuto)
	ON_BN_CLICKED(IDC_BUTTON_OnlySendReadSelectedADCcmd, OnBUTTONOnlySendReadSelectedADCcmd)
	ON_BN_CLICKED(IDC_BUTTON_Relay3EnCmd, OnSPRRelay3En)
	ON_BN_CLICKED(IDC_BUTTON_Relay4EnCmd, OnSPRRelay4En)
	ON_BN_CLICKED(IDC_CHECK_SwitchTest1, OnHVOnAutoTest)
	ON_BN_CLICKED(IDC_CHECK_SwitchTest2, OnSPRRelay1EnAutoTest)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_VACDACgroupOption0, IDC_RADIO_VACDACgroupOption2, OnVACDACgroupOptionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_VACDACselOption0, IDC_RADIO_VACDACselOption3, OnVACDACselOptionChanged)
	ON_COMMAND_RANGE(IDC_CHECK_FPGArelayEn0, IDC_CHECK_FPGArelayEn11, OnFPGArelayEnChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCselectOB0, IDC_RADIO_ADCselectOB5, OnADCselectOBChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCsChSelect0, IDC_RADIO_ADCsChSelect15, OnADCsChSelectChanged)
	ON_COMMAND_RANGE(IDC_RADIO_SPRDACselOption0, IDC_RADIO_SPRDACselOption3, OnSPRDACselOptionChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageVS message handlers

BOOL CPageVS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	/////////////////////////////////////////////////////////////////
	for( int i=0; i<12; i++ )
		m_nDACTCvalue[i] = 2048;

	m_nVACDACselected	= 0;
	m_nVACDACchannel	= 0;
	m_byteVACDACselBits	= 0x10;

	m_nSPRDACchannel = m_nSPRDACnewVal = 0;
	m_byteSPRDACselBits = 0;
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
	m_bSPRRelay1En = m_bSPRRelay2En = m_bSPRRelay3En = m_bSPRRelay4En = m_bHVOn = FALSE;
	m_bInh20V = m_bInh13V = m_bInh8V = TRUE;

	CString str;
	m_btnVACtestLED4.SetColor( g_crOFF );
//	m_btnVACtestLED4.SetWindowText( "Test LED is now OFF" );
	m_btnFansOnFPGA.SetColor( g_crON_Test );
	str.LoadString( IDS_SPR_Fans_On_FPGA_ON );
	m_btnFansOnFPGA.SetWindowText( str );
	m_btnFansOnPLD.SetColor( g_crON_Test );
	str.LoadString( IDS_SPR_Fans_On_PLD_ON );
	m_btnFansOnPLD.SetWindowText( str );
	m_btnSPRRelay1En.SetColor( g_crOFF );
	str.LoadString( IDS_SPR_Relay1_En_OFF );
	m_btnSPRRelay1En.SetWindowText( str );
	m_btnSPRRelay2En.SetColor( g_crOFF );
	str.LoadString( IDS_SPR_Relay2_En_OFF );
	m_btnSPRRelay2En.SetWindowText( str );
	m_btnSPRRelay3En.SetColor( g_crOFF );
	str.LoadString( IDS_SPR_Relay3_En_OFF );
	m_btnSPRRelay3En.SetWindowText( str );
	m_btnSPRRelay4En.SetColor( g_crOFF );
	str.LoadString( IDS_SPR_Relay4_En_OFF );
	m_btnSPRRelay4En.SetWindowText( str );
	m_btnHVOn.SetColor( g_crOFF );
	str.LoadString( IDS_SPR_HV_OFF );
	m_btnHVOn.SetWindowText( str );
	m_btnSPRtestLED4.SetColor( g_crOFF );
//	m_btnSPRtestLED4.SetWindowText( "Test LED is now OFF" );
	m_btnInh20V.SetColor( g_crScanEmphasis );
	str.LoadString( IDS_SPR_Inh_20V_ON );
	m_btnInh20V.SetWindowText( str );
	m_btnInh13V.SetColor( g_crScanEmphasis );
	str.LoadString( IDS_SPR_Inh_13V_ON );
	m_btnInh13V.SetWindowText( str );
	m_btnInh8V.SetColor( g_crScanEmphasis );
	str.LoadString( IDS_SPR_Inh_8V_ON );
	m_btnInh8V.SetWindowText( str );
	m_btnWDTSelfReset.SetColor( g_crOFF );
//	m_btnWDTSelfReset.SetWindowText( "Comm Inactivity Self-Init is OFF" );
	m_btnVACsoftReset.SetColor( g_crScanEmphasis );
	m_btnVACreadBoardID.SetColor( g_crSYS );
	m_btnSPRsoftReset.SetColor( g_crScanEmphasis );
	m_btnSPRreadBoardID.SetColor( g_crSYS );

	GetDlgItem( IDC_EDIT_Elapse )->SetWindowText( "100" );
	/////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageVS::Init()
{
	m_btnAnalogPS.SetCheck( 1 );
	SwitchAnalogPowerSupplies( 1 );

	m_scrollVACDACsVal.SetScrollPos( 2048 );
	VACDACvalUpdate( 0 );
	m_scrollIOexp.SetScrollPos( 5 );
	IOXguide( 5 );
	m_scrollLegHV.SetScrollPos( 0 );
	LegHVUpdate( 0 );

	// 初始化时将光电倍增管PMT的电压设置为0V, pos=0xFFFF
	m_scrollSPRDACsVal.SetScrollPos( 0xFFFF );
	SPRDACsValUpdae( 0xFFFF );
	SPRDACupdate();

	m_btnSoftEnVac.SetCheck( 1 );
	CheckSoftEnableVacuum( 1 );

	ReadComprs();

	//18.03.06 让HVon B始终为开
// 	wData		= 0x8C;		// Strobe IOX output HVonB high
// 	nWsize		= 8;
// 	nModID		= USB_MID_SPR;
// 	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
//	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	theApp.BoardWrite0816( 8, USB_MID_SPR, USB_CID_SPR_ExpanderChip, 0x8C );
	theApp.BoardWrite0816( 8, USB_MID_SPR, USB_CID_SPR_ExpanderChip, 0x8C );
	theApp.BoardWrite0816( 8, USB_MID_SPR, USB_CID_SPR_ExpanderChip, 0x8C );
}

void CPageVS::OnOK() 
{
}

void CPageVS::OnCancel() 
{
}

BOOL CPageVS::PreTranslateMessage(MSG* pMsg) 
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

void CPageVS::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

long CPageVS::BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata )
{
	CString strMsgCounts, strMsgData;

	long lRet = theApp.BoardRead0816( nRsize, nModID, bCktAddr, wWdata, strMsgCounts, strMsgData, TRUE );
	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( strMsgCounts );
	GetDlgItem( IDC_EDIT_ADCreadingTB )->SetWindowText( strMsgData );

	return lRet;
}

long CPageVS::BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata )
{
	CString strMsgCounts, strMsgData;

	long lRet = theApp.BoardRead2432( nRsize, nModID, bCktAddr, dwWdata, strMsgCounts, strMsgData, TRUE );
	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( strMsgCounts );
	GetDlgItem( IDC_EDIT_ADCreadingTB )->SetWindowText( strMsgData );

	return lRet;
}

void CPageVS::ADCvconv()
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str;
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dMin;
	double dRangeMax = theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dMax;
	double dEquivCurrent = 0.0;
	if( theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].bType )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)theApp.m_nBoardReadDataBuf[2*2]<<16) | (DWORD)theApp.m_nBoardReadDataBuf[3*2];
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
			str.Format( "%.5f", dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			str.Format( "%.5f", dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 +theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCOMoffset );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );
			dEquivCurrent = (dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 +theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCOMoffset) * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCampsPerVolt;
		}
		else
		{
			str.Format( "-%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "-%.5f", dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			str.Format( "%.5f", theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCOMoffset - dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );
			dEquivCurrent = (theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCOMoffset - dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2) * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(theApp.m_nBoardReadDataBuf[2*2]);
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
			str.Format( "%.5f", dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );	// Same - no adj. needed
			dEquivCurrent = dADCreadVrefFrac * 5 * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCampsPerVolt;
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "%.5f", dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );	// Same - no adj. needed
			dEquivCurrent = dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCampsPerVolt;
		}
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * theApp.m_dADCvref / 2 * theApp.m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dFactor > 0 )
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

void CPageVS::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

void CPageVS::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	case IDC_SCROLLBAR_VACDACsValSB:
		VACDACvalUpdate( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_SPRDACsValSB:
		SPRDACsValUpdae( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPageVS::OnVACDACgroupOptionChanged( UINT nID )
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

void CPageVS::VACDACgroupOptionChanged()
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
	m_scrollVACDACsVal.SetScrollPos( m_nDACTCvalue[m_nVACDACselected * 4 + m_nVACDACchannel] );
	for( int i=0; i<3; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_VACDACgroupOption0 +i )))->SetCheck( 0 );
	((CButton*)(GetDlgItem( IDC_RADIO_VACDACgroupOption0 +m_nVACDACselected )))->SetCheck( 1 );
}

void CPageVS::OnVACDACselOptionChanged( UINT nID )
{
	m_nVACDACchannel = nID -IDC_RADIO_VACDACselOption0;
	VACDACselOptionChanged();
}

void CPageVS::VACDACselOptionChanged()
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
	m_scrollVACDACsVal.SetScrollPos( m_nDACTCvalue[m_nVACDACselected * 4 + m_nVACDACchannel] );
	for( int i=0; i<4; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_VACDACselOption0 +i )))->SetCheck( 0 );
	((CButton*)(GetDlgItem( IDC_RADIO_VACDACselOption0 +m_nVACDACchannel )))->SetCheck( 1 );
}

void CPageVS::VACDACvalUpdate( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_VACDACsValSBTB )->SetWindowText( str );
	str.Format( "%.3f V", 1.0 *nPos /4095 * 2.5 );	// Range @ DAC output is 0 to +2.5V
	GetDlgItem( IDC_EDIT_DACvalTB )->SetWindowText( str );
	m_nDACTCvalue[m_nVACDACselected * 4 + m_nVACDACchannel] = nPos;	// Keep history of user setting
	theApp.m_DL.diag[Diag_CalI_TC0 + m_nVACDACselected * 4 + m_nVACDACchannel].nCurPos = nPos;
	// Call VACDACupdateCmd_Click 'Commented out to prevent slider from updating DAC's
}

void CPageVS::OnVACDACupdateCmd() 
{
	// TODO: Add your control notification handler code here
	int nVACDACvalue = m_scrollVACDACsVal.GetScrollPos();
	VACDACupdate( nVACDACvalue );
}

void CPageVS::VACDACupdate( int nVACDACvalue )
{
	BYTE byteVACDAChibyte, byteVACDAClobyte, byteVACDACfirst24;

	byteVACDAChibyte = (BYTE)(( nVACDACvalue & 0xFF0 ) /16 );	// Shift down 4 bits to properly byte-align
	byteVACDAClobyte = (BYTE)(( nVACDACvalue & 0x0F ) *16 );	// Need to shift up by 4 bits for DAC7565 for proper alignment
	byteVACDACfirst24	= 0x10 | ( m_nVACDACchannel *2 );		// Selects immediate load, channel select shifted up 1 bit//m_byteVACDACselBits;//

	// Write via MCI to VAC board.  Cluster 1: select Xfer type, module ID and circuit ID
/*	theApp.CommToBoards(	m_byteVACDACaddr,		// VAC board circuit address for selected DAC (0x12,22 or 32) for OP1
							USB_TM_VAC_Write_24 );	// Serial Comm operand high byte (24 bit write to VAC board) for OP2
	// Write via MCI to VAC board.  Cluster 2: Write DAC upper byte value in OP3, Select DAC channel, etc. in OP4:
	theApp.CommToBoards(	byteVACDAChibyte,		// Upper 8 of 12 bits of DAC data for OP3
							byteVACDACfirst24 );	// OP4: Bit 4 selects immediate DAC value update in H/W. Bits 2,1 select DAC channel
	// Write via MCI to VAC board.  Cluster 3: Write 0 to OP5, Write low DAC value nibble in bits 7..4 of OP6
	theApp.CommToBoards(	0x0,					// Send 0 for OP5
							byteVACDAClobyte );		// DAC low 4 bits placed in upper 4 bits of OP6
*/
//	CommToBoards3( m_byteVACDACaddr, USB_TM_VAC_Write_24, byteVACDAChibyte, byteVACDACfirst24, 0x0, byteVACDAClobyte );
	DWORD dwWdata = (DWORD)(byteVACDACfirst24 *65536 + byteVACDAChibyte *256 +byteVACDAClobyte);
	theApp.BoardWrite24(24, USB_MID_VAC, m_byteVACDACaddr, dwWdata);

	CString str;
//	str.Format( "Sent: 0x%02X 0x%02X 0x%02X", byteVACDACfirst24, byteVACDAChibyte, byteVACDAClobyte );
	str.Format( "Sent: 0x%X", byteVACDACfirst24 *65536 + byteVACDAChibyte *256 +byteVACDAClobyte );
	GetDlgItem( IDC_EDIT_VACDACsentTB )->SetWindowText( str );
}

void CPageVS::OnReadComprsCmd() 
{
	ReadComprs();
}

void CPageVS::ReadComprs()
{
	// TODO: Add your control notification handler code here
//	SetPxlClkto2();
	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_VAC_ReadThermoCCGComp, USB_TM_VAC_Read_16, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );

	if( dwRet > 0 )
	{
		// 4th readback word (7th and 8th ASCII characters) contains Comparator data
		WORD wData, wRBit, wMskBit;
		wData = (WORD)wData1;
		for( wRBit=0; wRBit<14; wRBit += 2 )
		{
			wMskBit = (WORD)(pow(2.0, wRBit) );	// Odd numbered bits are Upper trip point comparator states
			if( wData & wMskBit )
				GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "1" );
			else
				GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "0" );
			
			wMskBit *= 2;						// Shift up one bit to check Lower trip point comparator state
			if( wData & wMskBit )
				GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "1" );
			else
				GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "0" );
		}
	}
}

CString CPageVS::ReadComprs_Logging()
{
	// TODO: Add your control notification handler code here
	CString str, strRes;
	unsigned short nData = 0;
	long lRet = 1;
	while( (lRet > 0) || (nData == 0) )
		lRet = theApp.ReadComprs( &nData, str );

	// 4th readback word (7th and 8th ASCII characters) contains Comparator data
	WORD wData, wRBit, wMskBit;
	wData = (WORD)nData;
	for( wRBit=0; wRBit<14; wRBit += 2 )
	{
		wMskBit = (WORD)(pow(2.0, wRBit) );	// Odd numbered bits are Upper trip point comparator states
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "0" );

		wMskBit *= 2;						// Shift up one bit to check Lower trip point comparator state
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "0" );
	}

	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( str );
	strRes.Format( "%04X\t", nData );
	return strRes;
}

CString CPageVS::ReadComprs_Logging( int nData )
{
	// TODO: Add your control notification handler code here
	CString strRes = "";
	
	// 4th readback word (7th and 8th ASCII characters) contains Comparator data
	WORD wData, wRBit, wMskBit;
	wData = (WORD)nData;
	for( wRBit=0; wRBit<14; wRBit += 2 )
	{
		wMskBit = (WORD)(pow(2.0, wRBit) );	// Odd numbered bits are Upper trip point comparator states
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "0" );
		
		wMskBit *= 2;						// Shift up one bit to check Lower trip point comparator state
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "0" );
	}
	
	strRes.Format( "%04X\t", nData );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strRes );
	return strRes;
}

void CPageVS::OnCHECKReadVACmprsAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_ReadVACmprsAuto )))->GetCheck();
	if( nCheck )
	{
		nTimes = 0;
		SetTimer( 1, 2000, NULL );
	}
	else
		KillTimer( 1 );
}

void CPageVS::IOXguide( int nPos )
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

void CPageVS::OnIOXstbHiCmd() 
{
	// TODO: Add your control notification handler code here
	BYTE bytePos = m_scrollIOexp.GetScrollPos();
	bytePos |= 0x80;
	theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip,	// Low byte = ckt addr. of VAC I/O Expander
							USB_TM_VAC_Write_08,		// Serial Comm operand high byte (&H13 = 8 bit write to VAC board)
							0x0,						// OP3 = 0 for 8 bit write
							bytePos );					// Strobe selected line high
}

void CPageVS::OnIOXstbLoCmd() 
{
	// TODO: Add your control notification handler code here
	BYTE bytePos = m_scrollIOexp.GetScrollPos();
	bytePos |= 0x40;
	theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip,	// Low byte = ckt addr. of VAC I/O Expander
							USB_TM_VAC_Write_08,		// Serial Comm operand high byte (&H13 = 8 bit write to VAC board)
							0x0,						// OP3 = 0 for 8 bit write
							bytePos );					// Strobe selected line low
}

void CPageVS::OnCHECKSoftEnableVacuum() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_btnSoftEnVac.GetCheck();
	CheckSoftEnableVacuum( nCheck );
}

void CPageVS::CheckSoftEnableVacuum( int nCheck )
{
	CString str;
	if( nCheck )
	{
		m_btnSoftEnVac.SetColor( g_crON_Test );
		str.LoadString( IDS_VAC_Soft_Enable );
		m_btnSoftEnVac.SetWindowText( str );
	}
	else
	{
		m_btnSoftEnVac.SetColor( ::GetSysColor(COLOR_3DFACE) );
		str.LoadString( IDS_VAC_Soft_Disable );
		m_btnSoftEnVac.SetWindowText( str );
	}
	InitVacuumControls( (BOOL)nCheck );
}

void CPageVS::InitVacuumControls( BOOL bCtrlBySoft ) 
{
	if( bCtrlBySoft )
	{
		// "0 (Rly Tst A), 29 (Rly Tst B)" = High
		BYTE bytePos = 0x80 | 0x0;	// 0
		theApp.CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x1D;		// 29
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "Test Relays 0-7" = check on
		for( int i=1; i<9; i++ )
		{
			bytePos = 0x80 | (BYTE)i;
			theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		}
		// "En TC0,1,2,3" = High (enables TC1,2,3,4 on VAC diagram)
		bytePos = 0x80 | 0x13;		// 19 (En TC1 Ref.)
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x14;		// 20 (En TC2 Ref.)
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x15;		// 21 (En TC3 Ref.)
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x16;		// 22 (En TC4 Ref.)
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
	else
	{
		BYTE bytePos = 0x0;
		// "En TC0,1,2,3" = Low (enables TC1,2,3,4 on VAC diagram)
		bytePos = 0x40 | 0x13;		// 19 (En TC1 Ref.)
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x14;		// 20 (En TC2 Ref.)
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x15;		// 21 (En TC3 Ref.)
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x16;		// 22 (En TC4 Ref.)
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "Test Relays 0-7" = check off
		for( int i=1; i<9; i++ )
		{
			bytePos = 0x40 | (BYTE)i;
			theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		}
		// "0 (Rly Tst A), 29 (Rly Tst B)" = Low
		bytePos = 0x40 | 0x0;		// 0
		theApp.CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		theApp.CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
}

void CPageVS::OnFPGArelayEnChanged( UINT nID )
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

	theApp.CommToBoards2(	byteRelayEnAddr,		// OP1 = ckt addr. of VAC circuit address for corresponding relay enable
							USB_TM_VAC_Write_08,	// OP2 = 8 bit write to VAC board)
							0x0,					// OP3 = 0
							bCheck );				// OP4 = set bit 0 high to enable relay or low to disable relay
}

void CPageVS::OnReadVACextSwitchStates() 
{
	// Before each read, must first select which switch is to be read via the I/O Extender chip (addresses 0x19,0x1A, 0x1B)
	// Open and Closed ("Clsd") Results are sent to label array: VACextSwStatesLabel(index)

	// Example below: Select switch #4 by setting select bits to 1/0/0 (requires 3 back-to-back 8 bit writes to IOX)
	int		nWsize, nRsize, nModID;
	BYTE	bCktAddr;
	WORD	wWdata;	// arg's to pass to theApp.BoardWrite0816 sub
	BYTE	bSwSelect2, bSwSelect1, bSwSelect0;
	for( int nSwNum=0; nSwNum<1; nSwNum++ )	// 总共有8个外部switch，目前只读1个
	{
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
		theApp.BoardWrite0816(nWsize, nModID, bCktAddr, wWdata);

		if( bSwSelect1 )
			wWdata	= 0x1A | 0x80;	// Strobe select line 1@0x1A to a high state
		else
			wWdata	= 0x1A | 0x40;	// Strobe select line 1@0x1A to a low state
		theApp.BoardWrite0816(nWsize, nModID, bCktAddr, wWdata);

		if( bSwSelect0 )
			wWdata	= 0x1B | 0x80;	// Strobe select line 0@0x1B to a high state
		else
			wWdata	= 0x1B | 0x40;	// Strobe select line 0@0x1B to a low state
		theApp.BoardWrite0816(nWsize, nModID, bCktAddr, wWdata);

		bCktAddr	= 0x98;			// USB_CID_VAC_ReadSelExtSwitchState
		nRsize		= 16;
		wWdata		= 0;
		if( BoardRead0816( nRsize, nModID, bCktAddr, wWdata ) > 0 )
			theApp.WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );

		// BoardReadDatBuf(256, 2) '2nd element is type: "FFFF"=9, marker "D22D"=4, Board Readback Data=1, (0,0)=count
		int nNumWords = theApp.m_nBoardReadDataBuf[0];	// # of words read from board stored in Read Subroutine
		int nBoardData, nDataType;
		for( int i=1; i<=nNumWords; i++ )	// For I = 1 To NumWords
		{
			nBoardData	= theApp.m_nBoardReadDataBuf[i*2];
			nDataType	= theApp.m_nBoardReadDataBuf[i*2 +1];
			CString str;
			if( nDataType == 1 )	// Board readback data - use it
			{
				if( nBoardData & 1 )	// 通常返回值为0x0101
				{
					str.Format( "S %d:OPEN", nSwNum );
					GetDlgItem( IDC_STATIC_VACextSwStatesLabel0 +nSwNum )->SetWindowText( "Open" );
//					VACextSwStatesLabel(SwNum).BackColor = &H8000000F  'White
					((CStatic*)(GetDlgItem( IDC_STATIC_VACextSwStatesLED0 +nSwNum )))->SetBitmap((HBITMAP)(m_bmpON));
				}
				else					// 通常返回值为0x0000
				{
					str.Format( "S %d:CLSD", nSwNum );
					GetDlgItem( IDC_STATIC_VACextSwStatesLabel0 +nSwNum )->SetWindowText( "Close" );
//					VACextSwStatesLabel(SwNum).BackColor = &H8080FF 'Light red
					((CStatic*)(GetDlgItem( IDC_STATIC_VACextSwStatesLED0 +nSwNum )))->SetBitmap((HBITMAP)(m_bmpOFF));
				}
			}
		}
	}
}

void CPageVS::OnVACtestLED4() 
{
	// Utilizes VACTestLED4State global, temporarily uses CL wobbler On ckt address (0x1c)
	CString str, strTemp;
	m_bVACtestLED4 = !m_bVACtestLED4;
	if( m_bVACtestLED4 )
	{
		m_btnVACtestLED4.SetColor( g_crON_Test );
		strTemp.LoadString( IDS_Test_LED_ON );
		str.Format( "VAC: %s", strTemp );
		m_btnVACtestLED4.SetWindowText( str );
	}
	else
	{
		m_btnVACtestLED4.SetColor( g_crOFF );
		strTemp.LoadString( IDS_Test_LED_OFF );
		str.Format( "VAC: %s", strTemp );
		m_btnVACtestLED4.SetWindowText( str );
	}

	///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_VAC_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_VAC_Write_08,		// Serial Comm operand high byte (8 bit write to VAC board)
							0x0,						// OP3 = 0 for 8 bit write
							(BYTE)m_bVACtestLED4 );		// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CPageVS::OnSPRtestLED4() 
{
	// Utilizes SPRtestLED4State global, temporarily uses CL wobbler On ckt address (0x1c)
	CString str, strTemp;
	m_bSPRtestLED4 = !m_bSPRtestLED4;
	if( m_bSPRtestLED4 )
	{
		m_btnSPRtestLED4.SetColor( g_crON_Test );
		strTemp.LoadString( IDS_Test_LED_ON );
		str.Format( "SPR: %s", strTemp );
		m_btnSPRtestLED4.SetWindowText( str );
	}
	else
	{
		m_btnSPRtestLED4.SetColor( g_crOFF );
		strTemp.LoadString( IDS_Test_LED_OFF );
		str.Format( "SPR: %s", strTemp );
		m_btnSPRtestLED4.SetWindowText( str );
	}

	///////////////////////////////////////////////////////////////////////////
//	theApp.BoardWrite0816(8, USB_MID_SPR, USB_CID_SPR_Test_LED, m_bSPRtestLED4);
	theApp.CommToBoards2(	USB_CID_SPR_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_SPR_Write_08,		// Serial Comm operand high byte (8 bit write to SPR board)
							0x0,						// OP3 = 0 for 8 bit write
							(BYTE)m_bSPRtestLED4 );		// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CPageVS::OnVACsoftReset() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	nWsize		= 8;
	nModID		= USB_MID_VAC;
	bCktAddr	= USB_CID_VAC_Soft_Reset;
	wData		= 0;
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	// Force DAC control sliders and resulting reports to 0:
}

void CPageVS::OnSPRsoftReset() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_Soft_Reset;
	wData		= 0;
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnVACreadBoardID() 
{
	// TODO: Add your control notification handler code here
	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_VAC_ReadBoardID, USB_TM_VAC_Read_08, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPageVS::OnSPRreadBoardID() 
{
	// TODO: Add your control notification handler code here
	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_SPR_ReadBoardID, USB_TM_SPR_Read_08, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPageVS::OnLocalLoopback() 
{
	// by DD  Jan 24, 2012:
	// Force MCI local loopback to assure alternative RB data from board comm will work.
	// Each time this button is clicked (presuming Scanning has stopped), board_comm block
	// will force 16 words of 0xABCD into To_USB FIFO to be sent to host.
	theApp.CommToBoards2(	0x5A,		// OP1 = Circuit ID for force 16 r/b words
							0xAD,		// OP2 = Transfer type A for module D(MCI)
							0x0,		// OP3
							0x0 );		// OP4
}

void CPageVS::OnStartScan() 
{
	StartScan();
}

void CPageVS::StartScan()
{
	// Start scan
	theApp.WriteToBoard( USB_StartScan, FALSE, 0 );

	// setup the signal
	theApp.m_bIsStop = FALSE;
}

void CPageVS::OnStopScan() 
{
	StopScan();
}

void CPageVS::StopScan()
{
	// Send binary 100 on Port C and 0x13 on Port A, then bin 111 on C, then 000 on C to start scan
	theApp.WriteToBoard( USB_StopScan, FALSE, 0 );

	// setup the signal
	theApp.m_bIsStop = TRUE;
}

void CPageVS::OnSetPxlClkto2() 
{
	SetPxlClkto2();
}

void CPageVS::SetPxlClkto2()
{
	theApp.WriteToBoard( USB_SetPixelRate, TRUE, 0x02 );
}

void CPageVS::OnFansOnFPGA() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue
	m_bFansOnFPGA = !m_bFansOnFPGA;
	CString str;
	if( m_bFansOnFPGA )
	{
		m_btnFansOnFPGA.SetColor( g_crON_Test );
		str.LoadString( IDS_SPR_Fans_On_FPGA_ON );
		m_btnFansOnFPGA.SetWindowText( str );
	}
	else
	{
		m_btnFansOnFPGA.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Fans_On_FPGA_OFF );
		m_btnFansOnFPGA.SetWindowText( str );
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_Fan_PWM_Control;
	wData		= (WORD)m_bFansOnFPGA;	// 1 or 0 for now - PWM speed control possibly in future
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnFansOnPLD() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bFansOnPLD = !m_bFansOnPLD;
	CString str;
	if( m_bFansOnPLD )
	{
		m_btnFansOnPLD.SetColor( g_crON_Test );
		str.LoadString( IDS_SPR_Fans_On_PLD_ON );
		m_btnFansOnPLD.SetWindowText( str );
		wData	= 0x87;		// Strobe IOX output @0A high
	}
	else
	{
		m_btnFansOnPLD.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Fans_On_PLD_OFF );
		m_btnFansOnPLD.SetWindowText( str );
		wData	= 0x47;		// Strobe IOX output @0A low
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnSPRRelay1En() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bSPRRelay1En= !m_bSPRRelay1En;
	CString str;
	if( m_bSPRRelay1En )
	{
		m_btnSPRRelay1En.SetColor( g_crEmphasis3 );
		str.LoadString( IDS_SPR_Relay1_En_ON );
		m_btnSPRRelay1En.SetWindowText( str );
		wData	= 0x82;		// Strobe IOX output @0A high
	}
	else
	{
		m_btnSPRRelay1En.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Relay1_En_OFF );
		m_btnSPRRelay1En.SetWindowText( str );
		wData	= 0x42;		// Strobe IOX output @0A low
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	if( m_bSPRRelay1En )
	{
		// J14开之后，光电倍增管电压才加上，对比度设置才有效
		// 初始化时将光电倍增管PMT的电压设置为0V, pos=0xFFFF
		m_scrollSPRDACsVal.SetScrollPos( 0xFFFF );
		SPRDACsValUpdae( 0xFFFF );
		SPRDACupdate();
	}
}

void CPageVS::OnSPRRelay1EnAutoTest() 
{
	// TODO: Add your control notification handler code here
	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());

	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_SwitchTest2 )))->GetCheck();
	if( nCheck )
	{
		/////////////////////////////////////////
		pParent->KillTimer( Diag_Total );
		pParent->KillTimer( tmrReadPort );
		pParent->KillTimer( tmrClear );
		pParent->GetDlgItem( IDC_BUTTON_Diagnose )->EnableWindow( FALSE );
		pParent->GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( FALSE );

		KillTimer( 1 );
		/////////////////////////////////////////

		/////////////////////////////////////////
		nTimes = 0;
		CString str;
		GetDlgItem( IDC_EDIT_Elapse )->GetWindowText( str );
		int nIntv = atoi( str );
		m_bRecord	= ((CButton*)(GetDlgItem(IDC_CHECK_Debug)))->GetCheck();

		if( m_bRecord )
		{
			// 准备记录文件
			char appfilepath[255];
			::GetModuleFileName( NULL, appfilepath, 255 );
			char *ptemp = strrchr( appfilepath, '\\');
			ptemp[1] = 0;

			CString str, strTemp, strParam, strFirst;
			CTime	timeCurrent = CTime::GetCurrentTime();
			strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
			str.Format( "%s\\Log\\zSwitchTest-%s.txt", appfilepath, strTemp );
			theApp.m_DL.other.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			theApp.m_DL.other.lTimes = 0;
		}
		/////////////////////////////////////////

		SetTimer( 4, nIntv, NULL );
	}
	else
	{
		KillTimer( 4 );
		pParent->GetDlgItem( IDC_BUTTON_Diagnose )->EnableWindow( TRUE );
		pParent->GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( TRUE );
		if( m_bRecord )
			CloseHandle(theApp.m_DL.other.hLogFile);
	}
}

void CPageVS::SPRRelay1EnAutoTest()
{
	// CCD电源开关实验：自动间隔时间发送J12开关命令，并读取V1回读值
	// 硬件上单独做了连线，可将J12开关与V1回读关联
	OnSPRRelay1En();
//	OnTestReadSPRswitches();
	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_SPR_ReadExternalSwitch, USB_TM_SPR_Read_08, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
	
	if( m_bRecord )
	{
		CString str;
		str.Format( "%04d\t", theApp.m_DL.other.lTimes++ );
		if( m_bRecord )
		{
			str += strMsg;
			unsigned long cbRet;
			char Buf[4096];
			sprintf( Buf, "%s\r\n", str );
			WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}

void CPageVS::OnSPRRelay2En() 
{
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bSPRRelay2En= !m_bSPRRelay2En;
	CString str;
	if( m_bSPRRelay2En )
	{
		m_btnSPRRelay2En.SetColor( g_crEmphasis3 );
		str.LoadString( IDS_SPR_Relay2_En_ON );
		m_btnSPRRelay2En.SetWindowText( str );
		wData	= 0x83;		// Strobe IOX output @0A high
	}
	else
	{
		m_btnSPRRelay2En.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Relay2_En_OFF );
		m_btnSPRRelay2En.SetWindowText( str );
		wData	= 0x43;		// Strobe IOX output @0A low
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnSPRRelay3En() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bSPRRelay3En= !m_bSPRRelay3En;
	CString str;
	if( m_bSPRRelay3En )
	{
		m_btnSPRRelay3En.SetColor( g_crEmphasis3 );
		str.LoadString( IDS_SPR_Relay3_En_ON );
		m_btnSPRRelay3En.SetWindowText( str );
		wData	= 0x84;		// Strobe IOX output @0A high
	}
	else
	{
		m_btnSPRRelay3En.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Relay3_En_OFF );
		m_btnSPRRelay3En.SetWindowText( str );
		wData	= 0x44;		// Strobe IOX output @0A low
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnSPRRelay4En() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bSPRRelay4En= !m_bSPRRelay4En;
	CString str;
	if( m_bSPRRelay4En )
	{
		m_btnSPRRelay4En.SetColor( g_crEmphasis3 );
		str.LoadString( IDS_SPR_Relay4_En_ON );
		m_btnSPRRelay4En.SetWindowText( str );
		wData	= 0x85;		// Strobe IOX output @0A high
	}
	else
	{
		m_btnSPRRelay4En.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Relay4_En_OFF );
		m_btnSPRRelay4En.SetWindowText( str );
		wData	= 0x45;		// Strobe IOX output @0A low
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnHVOn() 
{
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: FanControlValue (initially shared by FPGA and PLD controls)
	m_bHVOn = !m_bHVOn;
	CString str;
	if( m_bHVOn )
	{
		m_btnHVOn.SetColor( g_crEmphasis4 );
		str.LoadString( IDS_SPR_HV_ON );
		m_btnHVOn.SetWindowText( str );
		wData		= 0x89;		// Strobe IOX output HVonA high
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
// 		for(int i=0; i<1000000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
// 			double x = sqrt(4.);
// 		wData		= 0x8C;		// Strobe IOX output HVonB high
// 		theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	}
	else
	{
		m_btnHVOn.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_HV_OFF );
		m_btnHVOn.SetWindowText( str );
		wData		= 0x49;		// Strobe IOX output HVonA low
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
// 		for(int i=0; i<1000000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
// 			double x = sqrt(4.);
// 		wData		= 0x4C;		// Strobe IOX output HVonB low
// 		theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	}
}

void CPageVS::OnHVOnAutoTest() 
{
	// TODO: Add your control notification handler code here
	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());

	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_SwitchTest1 )))->GetCheck();
	if( nCheck )
	{
		/////////////////////////////////////////
		pParent->KillTimer( Diag_Total );
		pParent->KillTimer( tmrReadPort );
		pParent->KillTimer( tmrClear );
		pParent->GetDlgItem( IDC_BUTTON_Diagnose )->EnableWindow( FALSE );
		pParent->GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( FALSE );

		KillTimer( 1 );
		/////////////////////////////////////////

		/////////////////////////////////////////
		nTimes = 0;
		CString str;
		GetDlgItem( IDC_EDIT_Elapse )->GetWindowText( str );
		int nIntv = atoi( str );
		m_bRecord	= ((CButton*)(GetDlgItem(IDC_CHECK_Debug)))->GetCheck();

		if( m_bRecord )
		{
			// 准备记录文件
			char appfilepath[255];
			::GetModuleFileName( NULL, appfilepath, 255 );
			char *ptemp = strrchr( appfilepath, '\\');
			ptemp[1] = 0;
			
			CString str, strTemp, strParam, strFirst;
			CTime	timeCurrent = CTime::GetCurrentTime();
			strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
			str.Format( "%s\\Log\\zSwitchTest-%s.txt", appfilepath, strTemp );
			theApp.m_DL.other.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			theApp.m_DL.other.lTimes = 0;
		}
		/////////////////////////////////////////
		m_dwV1stat = 0x0;
		SetTimer( 3, nIntv, NULL );
	}
	else
	{
		KillTimer( 3 );
		pParent->GetDlgItem( IDC_BUTTON_Diagnose )->EnableWindow( TRUE );
		pParent->GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( TRUE );
		if( m_bRecord )
			CloseHandle(theApp.m_DL.other.hLogFile);
	}
}

void CPageVS::HVOnAutoTest()
{
	// CCD电源开关实验：自动间隔时间发送J12开关命令，并读取V1回读值
	// 硬件上单独做了连线，可将J12开关与V1回读关联
	OnHVOn();
//	OnTestReadSPRswitches();
	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_SPR_ReadExternalSwitch, USB_TM_SPR_Read_08, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );

	BOOL bError = FALSE;
	if( m_dwV1stat != wData1 )
		m_dwV1stat = wData1;
	else
		bError = TRUE;
	if( m_bRecord )
	{
		CString str;
		str.Format( "%04d\t", theApp.m_DL.other.lTimes++ );
		if( m_bRecord )
		{
			str += strMsg;
			unsigned long cbRet;
			char Buf[4096];
			sprintf( Buf, "%s\r\n", str );
			WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );

			if( bError )
			{
				str = "error!!!";
				sprintf( Buf, "%s\r\n", str );
				WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}
	if( bError )
	{
		KillTimer( 3 );
		CTabDialog* pTabDlg = (CTabDialog*)GetParent();
		CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
		pParent->GetDlgItem( IDC_BUTTON_Diagnose )->EnableWindow( TRUE );
		pParent->GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( TRUE );
		if( m_bRecord )
			CloseHandle(theApp.m_DL.other.hLogFile);
	}
}

void CPageVS::OnLegHVOnOff() 
{
}

void CPageVS::LegHVUpdate( int nPos )
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

	theApp.BoardWrite0816( 16, USB_MID_SPR, USB_CID_SPR_Legacy_HV_Power, wData );
}

void CPageVS::OnSwitchAnalogPowerSupplies() 
{
	int nCheck = m_btnAnalogPS.GetCheck();
	SwitchAnalogPowerSupplies( nCheck );
}

void CPageVS::SwitchAnalogPowerSupplies( int nCheck )
{
	// TODO: Add your control notification handler code here
	CString str;
	if( nCheck )
	{
		m_btnAnalogPS.SetColor( g_crON_Test );
		str.LoadString( IDS_SPR_Analog_Power_Supply_ON );
		m_btnAnalogPS.SetWindowText( str );
	}
	else
	{
		m_btnAnalogPS.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Analog_Power_Supply_OFF );
		m_btnAnalogPS.SetWindowText( str );
	}

	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_AnalogPower_Enable;
	wData		= (WORD)nCheck;
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnInh20V() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: Inh20V (located at dual I/O Expander addresses 0x19 and 0x20) IOX is @ 0x72
	m_bInh20V= !m_bInh20V;
	CString str;
	if( m_bInh20V )
	{
		m_btnInh20V.SetColor( g_crScanEmphasis );
		str.LoadString( IDS_SPR_Inh_20V_ON );
		m_btnInh20V.SetWindowText( str );
		wData	= 0x99;		// Strobe IOX output @25 high (bit 7 plus address = 1001 1001)
	}
	else
	{
		m_btnInh20V.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Inh_20V_OFF );
		m_btnInh20V.SetWindowText( str );
		wData	= 0x59;		// Strobe IOX output @25 low (bit 6 plus address = 0101 1001)
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );

	wData		+= 1;		// Also strobe next address @ 26 (requires dual writes to inhibit)
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnInh13V() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: Inh13V (located at dual I/O Expander addresses 22,23: 0x16 and 0x17) IOX is @ 0x72
	m_bInh13V= !m_bInh13V;
	CString str;
	if( m_bInh13V )
	{
		m_btnInh13V.SetColor( g_crScanEmphasis );
		str.LoadString( IDS_SPR_Inh_13V_ON );
		m_btnInh13V.SetWindowText( str );
		wData	= 0x96;		// Strobe IOX output @16 high (bit 7 plus address = 1001 0110)
	}
	else
	{
		m_btnInh13V.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Inh_13V_OFF );
		m_btnInh13V.SetWindowText( str );
		wData	= 0x56;		// Strobe IOX output @16 low (bit 6 plus address = 0101 0110)
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );

	wData		+= 1;		// Also strobe next address @ 23 (requires dual writes to inhibit)
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnInh8V() 
{
	// TODO: Add your control notification handler code here
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	// Utilizes global: Inh8V (located at dual I/O Expander addresses 0x13 and 0x14) IOX is @ 0x72
	m_bInh8V= !m_bInh8V;
	CString str;
	if( m_bInh8V )
	{
		m_btnInh8V.SetColor( g_crScanEmphasis );
		str.LoadString( IDS_SPR_Inh_8V_ON );
		m_btnInh8V.SetWindowText( str );
		wData	= 0x93;		// Strobe IOX output @13 high (bit 7 plus address = 1001 0011)
	}
	else
	{
		m_btnInh8V.SetColor( g_crOFF );
		str.LoadString( IDS_SPR_Inh_8V_OFF );
		m_btnInh8V.SetWindowText( str );
		wData	= 0x53;		// Strobe IOX output @13 low (bit 6 plus address = 0101 0011)
	}
	nWsize		= 8;
	nModID		= USB_MID_SPR;
	bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );

	wData		+= 1;		// Also strobe next address @ 1A (requires dual writes to inhibit)
	theApp.BoardWrite0816( nWsize, nModID, bCktAddr, wData );
}

void CPageVS::OnTestR8() 
{
	// TODO: Add your control notification handler code here
// 	theApp.CommToBoards2(	USB_CID_SPR_PowerStatus_Readback,	// OP1 = ckt addr. of SPR Power supply status word (bits 2-0 only)
// 							USB_TM_SPR_Read_08,					// OP2 = 0x94 (8 bit READ from SPR board)
// 							0x0,								// OP3: n/a, default to 0
// 							0x0 );								// OP4: n/a, default to 0
	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_SPR_PowerStatus_Readback, USB_TM_SPR_Read_08, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPageVS::OnTestReadSPRswitches() 
{
	// TODO: Add your control notification handler code here
	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_SPR_ReadExternalSwitch, USB_TM_SPR_Read_08, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPageVS::OnTestR16() 
{
	// TODO: Add your control notification handler code here
	theApp.CommToBoards2(	USB_CID_LNS_ADC_Read,				// OP1 = ckt addr. of ADC0
							USB_TM_LNS_Read_16,					// OP2 = 0xA2 (16 bit read from LNS board)
							0x0,								// OP3 = 0
							0xA4 );								// OP4: 2nd byte of 2 to ADC. 0xA4=Select Ch 4 (SE),+/-10V dyn. range
}

void CPageVS::OnTestR32() 
{
	// TODO: Add your control notification handler code here
	theApp.CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
							USB_TM_SPR_Read_32,					// operand 2 = 0xC4 (32 bit READ from SPR board)
							0x60,								// OP3: 2nd byte of 2 to ADC: 0x60 = select OSR level 6, TWOX=false
							0xBA );								// OP4: 1st byte of 2 to ADC: 0xBA = Select +3.3V monitor (ADC1 Ch. 5)
	theApp.CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
							USB_TM_SPR_Read_32,					// operand 2 = 0xC4 (32 bit READ from SPR board)
							0x60,								// OP3: 2nd byte of 2 to ADC: 0x60 = select OSR level 6, TWOX=false
							0x9A );								// OP4: 1st byte of 2 to ADC, 2nd time without changing parameters

//	theApp.CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
//							USB_TM_SPR_Read_32,					// operand 2 = 0xC4 (32 bit READ from SPR board)
//							0x60,								// OP3: 2nd byte of 2 to ADC: 0x60 = select OSR level 6, TWOX=false
//							0xBA );								// OP4: 1st byte of 2 to ADC: 0xBA = Select +3.3V monitor (ADC1 Ch. 5)
}

void CPageVS::OnReadSPRADC() 
{
	// As of early Feb 2012, An EVEN number of reads has to be handed off to USB interface in PC's circuit, so the ADC read
	// is executed 2x.  This also provides the add'l benefit of the ADC's returning the results of the prior conversion.
	theApp.CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
							USB_TM_SPR_Read_32,					// operand 2 = 0xC4 (32 bit READ from SPR board)
							0x40,								// OP3: 2nd byte of 2 to ADC: 0x30 = select OSR level 4 (880Hz/1.18mS), TWOX=false
							0xB0 );								// OP4: 1st byte of 2 to ADC: 0xB0 = Ext analog Ch 1 (P5 input), EN change settings

	// Insert wait here?
//	theApp.CommToBoards2(	USB_CID_SPR_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on SPR board
//							USB_TM_SPR_Read_32,					// operand 2 = 0xC4 (32 bit READ from SPR board)
//							0x40,								// OP3: 2nd byte of 2 to ADC: 0x40 = select OSR level 4 (880Hz/1.18mS), TWOX=false
//							0x90 );								// OP4: 1st byte of 2 to ADC: 0xB0 = = Ext analog Ch 1 (P5 input), no change to settings

	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_SPR_ADC1_Read, USB_TM_SPR_Read_32, 0x40, 0xB0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPageVS::OnReadVACADC() 
{
	// read instruction must be executed twice. The first one sets up the ADC channel and starts the conversion, and the 2nd
	// one retrieves the desired data.  If the time between these two read instructions is too short, then checking of the ADC
	// busy line is required before valid read data is available.
	theApp.CommToBoards2(	USB_CID_VAC_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on VACx board
							USB_TM_VAC_Read_32,					// operand 2 = 0xC3 (32 bit READ from VAC board)
							0x40,								// OP3: 2nd byte of 2 to ADC: 0x30 = select OSR level 4 (880Hz/1.18mS), TWOX=false
							0xBE );								// OP4: 1st byte of 2 to ADC: 0xBE = Ch 14, EN change settings

	// 2nd pass...
//	theApp.CommToBoards2(	USB_CID_VAC_ADC1_Read,				// operand 1 = ckt addr. of ADC1 on VACx board
//							USB_TM_VAC_Read_32,					// operand 2 = 0xC3 (32 bit READ from VAC board)
//							0x40,								// OP3: 2nd byte of 2 to ADC: 0x40 = select OSR level 4 (880Hz/1.18mS), TWOX=false
//							0x9E );								// OP4: 1st byte of 2 to ADC: 0x9E = Ch 14, no change to settings

	WORD wData1, wData2;
	CString strMsg = "";
	DWORD dwRet = theApp.BoardReadback( USB_CID_VAC_ADC1_Read, USB_TM_VAC_Read_32, 0x40, 0xBE, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPageVS::OnADCselectOBChanged( UINT nID )
{
	int nIndex = nID -IDC_RADIO_ADCselectOB0;
	// show assignments for each of the channels (16 for all except for LNS board, which as 8)
	CString str;
	for( int i=0; i<16; i++ )
	{
		str.Format( "Ch %d: %s", i, theApp.m_DL.adc.chn[nIndex*16+i].strLabel );
		GetDlgItem( IDC_RADIO_ADCsChSelect0 +i )->SetWindowText( str );
	}

	str = theApp.ChangedADCselectOB( nIndex );
	GetDlgItem( IDC_STATIC_PctVrefLabel )->SetWindowText( str );
	str.Format( "%.1f V", theApp.m_DL.adc.chn[nIndex*16].dVref );
	GetDlgItem( IDC_EDIT_ADCvrefTB )->SetWindowText( str );
}

void CPageVS::OnADCsChSelectChanged( UINT nID )
{
	theApp.ChangedADCsChSelect( nID -IDC_RADIO_ADCsChSelect0 );

	// show assignments for each of the channels (16 for all except for LNS board, which as 8)
	CString str;
	str.Format( "%d", nID -IDC_RADIO_ADCsChSelect0 );
//	GetDlgItem( IDC_EDIT_ADCchanSelTB )->SetWindowText( str );
}

void CPageVS::OnCHECKReadSelectedADCAuto() 
{
	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());

	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_ReadSelectedADCAuto )))->GetCheck();
	if( nCheck )
	{
		/////////////////////////////////////////
		pParent->KillTimer( Diag_Total );
		pParent->KillTimer( tmrReadPort );
		pParent->KillTimer( tmrClear );
		pParent->GetDlgItem( IDC_BUTTON_Diagnose )->EnableWindow( FALSE );
		pParent->GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( FALSE );

		KillTimer( 1 );
		/////////////////////////////////////////

		/////////////////////////////////////////
		nTimes = 0;
		CString str;
		GetDlgItem( IDC_EDIT_Elapse )->GetWindowText( str );
		int nIntv = atoi( str );
		m_bGraph	= ((CButton*)(GetDlgItem(IDC_CHECK_Graph)))->GetCheck();
		m_bRecord	= ((CButton*)(GetDlgItem(IDC_CHECK_Debug)))->GetCheck();
		if( m_bGraph )
		{
			ZeroMemory( theApp.m_pGraphDataBuff[0], sizeof(double)*GraphDataBuffCounts );
			theApp.m_DL.other.lTimes	= 0;
			theApp.m_DL.other.nElapseLogging = nIntv;
			theApp.m_DL.graph[0].bGraph = TRUE;
			theApp.m_DL.graph[0].nIndex = theApp.m_nADCnum*16+theApp.m_nADCchn;
			pParent->m_dlgGraph.m_nIntv = nIntv;
			pParent->m_dlgGraph.ShowWindow( SW_SHOW );
		}
		else
		{
			theApp.m_DL.graph[0].bGraph = FALSE;
			// 获取工作路径
			char path[256];
			::GetModuleFileName(0, path, 255);
			char *exd = strrchr(path, (int)'.');
			if(exd != NULL)
				*exd = '\0';
			strcat(path, ".ini");
			char szBuf[20];
			if(GetPrivateProfileString("LoggingParam", "TimeElapse", "", szBuf, 255, path) != 0)
			{
				CString str;
				str.Format( "%s", szBuf );
				theApp.m_DL.other.nElapseLogging = atoi( str );
			}
		}
		if( m_bRecord )
		{
			// 准备记录文件
			char appfilepath[255];
			::GetModuleFileName( NULL, appfilepath, 255 );
			char *ptemp = strrchr( appfilepath, '\\');
			ptemp[1] = 0;

			CString str, strTemp, strParam, strFirst;
			CTime	timeCurrent = CTime::GetCurrentTime();
			strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
			str.Format( "%szLog%s.txt", appfilepath, strTemp );
			theApp.m_DL.other.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			theApp.m_DL.other.lTimes = 0;
		}
		/////////////////////////////////////////

		SetTimer( 2, nIntv, NULL );
		pParent->SetTimer( tmrClear, 120000, NULL );
	}
	else
	{
		KillTimer( 2 );
		pParent->KillTimer( tmrClear );
		pParent->GetDlgItem( IDC_BUTTON_Diagnose )->EnableWindow( TRUE );
		pParent->GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( TRUE );
		if( m_bRecord )
			CloseHandle(theApp.m_DL.other.hLogFile);
	}
}

void CPageVS::OnReadSelectedADC() 
{
	theApp.ReadSelectedADC();	// start ADC change
	ReadSelectedADC_Finish();
}

void CPageVS::ReadSelectedADC_Finish(BOOL bSingle)
{
	CString str;
	if( theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].bType )
	{
		// Two back-to-back 16 bit transfers to receive RB data
		str.Format( "%04X %04X", theApp.m_nBoardReadDataBuf[2*2], theApp.m_nBoardReadDataBuf[3*2] );
	}
	else
	{
		// Only 16 bits max of readback data
		str.Format( "%04X", theApp.m_nBoardReadDataBuf[2*2] );
	}
	GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
	if( bSingle )
	{
		ADCvconv();
		return;
	}

	if( m_bGraph || m_bRecord )
	{
		double dValue = 0.0;
		CString str, strTemp;
		str.Format( "%04d\t", theApp.m_DL.other.lTimes++ );
		strTemp = theApp.ADCvconv_Logging( dValue, m_bRecord );
		if( m_bGraph )
		{
			theApp.m_pGraphDataBuff[0][theApp.m_DL.other.lTimes-1] = dValue;
			CTabDialog* pTabDlg = (CTabDialog*)GetParent();
			CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
			pParent->m_dlgGraph.UpdateGraphData( theApp.m_DL.other.lTimes );
		}
		if( m_bRecord )
		{
			str += strTemp;
			unsigned long cbRet;
			char Buf[4096];
			sprintf( Buf, "%s\n", str );
			WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
	else
		ADCvconv();
}

void CPageVS::OnWDTSelfResetToggle() 
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

	theApp.BoardWrite0816( 8, USB_MID_MCI2, 0x0, (BYTE)m_bWDTSelfReset );
}

void CPageVS::OnSPRDACselOptionChanged( UINT nID )
{
	m_nSPRDACchannel = nID -IDC_RADIO_SPRDACselOption0;
	SPRDACselOptionChanged();
}

void CPageVS::SPRDACselOptionChanged()
{
	CString str = "";
	switch( m_nSPRDACchannel )
	{
	case	0:							// DAC Channel A
		m_byteSPRDACselBits	= 0x10;		// upper byte value for 24 bit write and load for DAC channel A
		str					= "Ch A";
		break;
	case	1:							// DAC Channel B
		m_byteSPRDACselBits	= 0x14;		// upper byte value for 24 bit write and load for DAC channel B
		str					= "Ch B";
		break;
	case	2:							// DAC Channel C
		m_byteSPRDACselBits	= 0x18;		// upper byte value for 24 bit write and load for DAC channel C
		str					= "Ch C";
		break;
	case	3:							// DAC Channel D
		m_byteSPRDACselBits	= 0x1C;		// upper byte value for 24 bit write and load for DAC channel D
		str					= "Ch D";
		break;
	default:
		break;
	}
	GetDlgItem( IDC_EDIT_WhichSPRDACTB )->SetWindowText( str );
	// Force slider to last value (or initial value if not yet set)
	m_scrollSPRDACsVal.SetScrollPos( m_nSPRDACvalues[m_nSPRDACchannel] );
	SPRDACsValUpdae( m_nSPRDACvalues[m_nSPRDACchannel] );
}

void CPageVS::SPRDACsValUpdae( int nPos )
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
	GetDlgItem( IDC_EDIT_SPRDACsValTB )->SetWindowText( str );
	str.Format( "%.6f V", 2.5 *m_nSPRDACnewVal /65536 );
	GetDlgItem( IDC_EDIT_SPRDACvoltsTB )->SetWindowText( str );
	str.Format( "%04X", m_nSPRDACnewVal );
	GetDlgItem( IDC_EDIT_SPRDACsValHexTB )->SetWindowText( str );

	m_scrollSPRDACsVal.SetScrollPos( nPos );
	m_nSPRDACvalues[m_nSPRDACchannel] = nPos;
	theApp.m_DL.diag[Diag_SPRDAC_ChA + m_nSPRDACchannel].nCurPos = m_nSPRDACnewVal;

	SPRDACupdate();
}

void CPageVS::OnSPRDACupdate() 
{
	SPRDACupdate();
}

void CPageVS::SPRDACupdate()
{
	BYTE bSPRDAChibyte, bSPRDAClobyte, bSPRDACfirst24;
	DWORD dwWdata;

	bSPRDAChibyte	= (BYTE)((m_nSPRDACnewVal & 0xFF00) / 256);	// Shift down 8 bits to properly byte-align
	bSPRDAClobyte	= (BYTE)(m_nSPRDACnewVal & 0xFF);
	bSPRDACfirst24	= (BYTE)( 0x10 | (m_nSPRDACchannel * 2) );	// Selects immediate load, channel select shifted up 1 bit

	dwWdata = (DWORD)(bSPRDACfirst24 * 65536 + m_nSPRDACnewVal);

	CString str;
	str.Format( "Sent: 0x%X", (DWORD)(bSPRDACfirst24 *65536 + bSPRDAChibyte *256 + bSPRDAClobyte) );
	GetDlgItem( IDC_EDIT_SPRDACsentTB )->SetWindowText( str );
 
	theApp.BoardWrite24(24, USB_MID_SPR, USB_CID_SPR_General_Purpose_DACs, dwWdata);
}

void CPageVS::OnBoardCommRx() 
{
	unsigned short nData[96*3*3];
	DWORD	dwBytesReturned = 0;
	if( theApp.Serial_Read( nData, dwBytesReturned ) == 0 )
	{
		CString str, strMsg;
		strMsg.Format( "Data Received: %ld words. ", dwBytesReturned /2 );
		strMsg += "\r\n";
		if( dwBytesReturned /2 > 0 )
		{
			CTabDialog* pTabDlg = (CTabDialog*)GetParent();
			CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
			pParent->m_listReadback.ResetContent();
			for( DWORD i=0; i<dwBytesReturned/2; i++ )
			{
				str.Format( "@ %ld: %ld [ %04X ] ", i, nData[i], nData[i] );
				pParent->UpdateListReadbackString( str );
				str.Format( "%04X, ", nData[i] );
				strMsg += str;
			}
			pParent->UpdateListReadbackString( " " );
		}
		GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
	}
}

void CPageVS::OnBUTTONOnlySendReadSelectedADCcmd() 
{
	logParam lP;
	lP.nMID		= theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].nMID;
	lP.nCID		= theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].nCID;
	lP.nChn		= theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].nChn;
	lP.bType	= theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].bType;
	lP.bDiff	= theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].bDiff;
	theApp.OnlySendReadSelectedADCcmd( lP );
}

void CPageVS::InitDiagnose( BOOL bEnd )
{
	KillTimer( 1 );
	KillTimer( 2 );

	int i;
	for( i=0; i<16; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_ADCsChSelect0 +i )))->SetCheck( 0 );
	for( i=0; i<6; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_ADCselectOB0 +i )))->SetCheck( 0 );

	SetPxlClkto2();

	GetDlgItem( IDC_BUTTON_ReadSelectedADCcmd )->EnableWindow( bEnd );
	GetDlgItem( IDC_CHECK_ReadSelectedADCAuto )->EnableWindow( bEnd );
}

void CPageVS::SetTimers( UINT nIDEvent, BOOL bSet )
{
	if( nIDEvent == Diag_Total )
	{
		// Logging Param
		if( bSet )
			SetTimer( Diag_Total +100, theApp.m_DL.other.nElapseLogging, NULL );
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
			VACDACvalUpdate( theApp.m_DL.diag[nIDEvent].nMin );
			m_scrollVACDACsVal.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			VACDACupdate( theApp.m_DL.diag[nIDEvent].nMin );
		}
		else
		{
			// SPR board
			m_nSPRDACchannel = nIDEvent -Diag_SPRDAC_ChA;
			SPRDACselOptionChanged();
			SPRDACsValUpdae( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			SPRDACupdate();
		}
//		SetTimer( nIDEvent +100, theApp.m_DL.diag[nIDEvent].nIntv, NULL );
	}
	else
		KillTimer( nIDEvent +100 );
}

void CPageVS::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1 )
	{
		if( !theApp.m_bIsStop )
		{
			theApp.WriteToBoard( USB_Video_OFF, FALSE, 0 );
			unsigned long lRet = AIOUSB_ClearFIFO(theApp.deviceIndex, 0);
		}

		CString str;
		str.Format( "%d times.\r\n", nTimes );
		nTimes++;

		theApp.UpdateListString( str );
		TRACE( str );

		ReadComprs();

		if( !theApp.m_bIsStop )
		{
			theApp.WriteToBoard( USB_SetPixelRate, TRUE, USB_PIXEL_RATES[USB_SS_1us] );
			theApp.WriteToBoard( USB_Video_ON, FALSE, 0 );
		}
	}
	else if( nIDEvent == 2 )
	{
/*		if( theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].bType )		// 2448
		{
			theApp.ReadSelectedADC( FALSE );	// 残留值
			theApp.ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
			theApp.Delay( 80 );					// 等待建立及转换
			{
				theApp.ReadSelectedADC( TRUE );	// 真正的回读值，只读一次
				theApp.Delay( 80 );
				ADCvconv();
			}
		}
		else						// 1859
		{
			theApp.ReadSelectedADC( FALSE );	// 残留值
			theApp.ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
			theApp.Delay( 5 );					// 等待转换完成
			{
				theApp.ReadSelectedADC( TRUE );	// 真正的回读值，只读一次
				ADCvconv();
			}
		}*/
		theApp.ReadSelectedADC();	// start ADC change
		ReadSelectedADC_Finish( FALSE );
	}
	else if( nIDEvent == 3 )
	{
		HVOnAutoTest();
	}
	else if( nIDEvent == 4 )
	{
		SPRRelay1EnAutoTest();
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	else if( (nIDEvent >= Diag_CalI_TC0 +100) && (nIDEvent <=Diag_SetPoint_TC5 +100) )
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
		int nPos = m_nDACTCvalue[m_nVACDACselected * 4 + m_nVACDACchannel];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			VACDACvalUpdate( nPos );
			m_scrollVACDACsVal.SetScrollPos( nPos );
			VACDACupdate( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			VACDACvalUpdate( nPos );
			m_scrollVACDACsVal.SetScrollPos( nPos );
			VACDACupdate( nPos );
		}
	}
	else if( (nIDEvent >= Diag_SPRDAC_ChA +100) && (nIDEvent <= Diag_SPRDAC_ChD +100) )
	{
		// SPR board
		m_nSPRDACchannel = nIDEvent -Diag_SPRDAC_ChA -100;
		SPRDACselOptionChanged();
		int nPos = m_nSPRDACvalues[m_nSPRDACchannel];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -32768;
			SPRDACsValUpdae( nPos );
			SPRDACupdate();
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			SPRDACsValUpdae( nPos );
			SPRDACupdate();
		}
	}

	CDialog::OnTimer(nIDEvent);
}
