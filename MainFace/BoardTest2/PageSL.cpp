// PageSL.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "PageSL.h"
#include "TabDialog.h"
#include "Insample0Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSL dialog


CPageSL::CPageSL(CWnd* pParent /*=NULL*/)
	: CDialog(CPageSL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSL)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_byteXYgainFBbits	= 0;
	m_byteXYlowFBbits	= 0;
	m_byteXYlowMuxbits	= 0;
	m_byteAmuxAddrX		= 0;
	m_byteAmuxAddrY		= 0;
	m_bSCNtestLED4		= FALSE;
	m_bLNStestLED4		= FALSE;

	m_nOldAMPselOption	= -1;
}


void CPageSL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSL)
	DDX_Control(pDX, IDC_SCROLLBAR_DFDACsteps, m_scrollDFSteps);
	DDX_Control(pDX, IDC_SCROLLBAR_DFEnd, m_scrollDFEnd);
	DDX_Control(pDX, IDC_SCROLLBAR_DFStart, m_scrollDFStart);
	DDX_Control(pDX, IDC_BUTTON_cmdSpotMode, m_btnModeSpot);
	DDX_Control(pDX, IDC_BUTTON_cmdLineMode_Vertical, m_btnModeLineVertical);
	DDX_Control(pDX, IDC_BUTTON_cmdLineMode_Horizontal, m_btnModeLineHorizontal);
	DDX_Control(pDX, IDC_BUTTON_cmdNormalMode, m_btnModeNormal);
	DDX_Control(pDX, IDC_CHECK_Video, m_btnVideo);
	DDX_Control(pDX, IDC_CHECK_MainsLock, m_btnMainsLock);
	DDX_Control(pDX, IDC_BUTTON_LNSreadBoardID, m_btnLNSreadBoardID);
	DDX_Control(pDX, IDC_BUTTON_SCNreadBoardID, m_btnSCNreadBoardID);
	DDX_Control(pDX, IDC_BUTTON_SCNtestLED4Cmd, m_btnSCNtestLED4);
	DDX_Control(pDX, IDC_BUTTON_SCNsoftReset, m_btnSCNsoftReset);
	DDX_Control(pDX, IDC_SCROLLBAR_IOexp, m_scrollIOexp);
	DDX_Control(pDX, IDC_SCROLLBAR_YxoverDACSB, m_scrollYxoverDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_XxoverDACSB, m_scrollXxoverDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_YshiftDACSB, m_scrollYshiftDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_XshiftDACSB, m_scrollXshiftDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_YstigDACSB, m_scrollYstigDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_XstigDACSB, m_scrollXstigDAC);
	DDX_Control(pDX, IDC_BUTTON_LNStestLED4Cmd, m_btnLNStestLED4);
	DDX_Control(pDX, IDC_BUTTON_SetAutoWobbleCmd, m_btnAutoWob);
	DDX_Control(pDX, IDC_BUTTON_LNSsoftReset, m_btnLNSsoftReset);
	DDX_Control(pDX, IDC_SCROLLBAR_WobDAC, m_scrollWobDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_YalDAC, m_scrollYalDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_XalDAC, m_scrollXalDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_L2SDAC, m_scrollL2SDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_L1DAC, m_scrollL1DAC);
	DDX_Control(pDX, IDC_CHECK_YalCompDisable, m_btnYalCompDisable);
	DDX_Control(pDX, IDC_CHECK_XalCompDisable, m_btnXalCompDisable);
	DDX_Control(pDX, IDC_CHECK_L2CompDisable, m_btnL2CompDisable);
	DDX_Control(pDX, IDC_CHECK_L1CompDisable, m_btnL1CompDisable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSL, CDialog)
	//{{AFX_MSG_MAP(CPageSL)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_WrMuxCtl, OnWrMuxCtl)
	ON_BN_CLICKED(IDC_BUTTON_IOXstbHi, OnIOXstbHi)
	ON_BN_CLICKED(IDC_BUTTON_IOXstbLo, OnIOXstbLo)
	ON_BN_CLICKED(IDC_BUTTON_SCNsoftReset, OnSCNsoftReset)
	ON_BN_CLICKED(IDC_BUTTON_SCNtestLED4Cmd, OnSCNtestLED4Cmd)
	ON_BN_CLICKED(IDC_BUTTON_TestW8cmd, OnTestW8)
	ON_BN_CLICKED(IDC_BUTTON_TestW16cmd, OnTestW16)
	ON_BN_CLICKED(IDC_BUTTON_TestW24cmd, OnTestW24)
	ON_BN_CLICKED(IDC_BUTTON_TestW32cmd, OnTestW32)
	ON_BN_CLICKED(IDC_CHECK_L1CompDisable, OnL1CompDisable)
	ON_BN_CLICKED(IDC_CHECK_L2CompDisable, OnL2CompDisable)
	ON_BN_CLICKED(IDC_CHECK_XalCompDisable, OnXalCompDisable)
	ON_BN_CLICKED(IDC_CHECK_YalCompDisable, OnYalCompDisable)
	ON_BN_CLICKED(IDC_BUTTON_LNSsoftReset, OnLNSsoftReset)
	ON_BN_CLICKED(IDC_BUTTON_ReadADC, OnReadADC)
	ON_BN_CLICKED(IDC_BUTTON_SetWobMinCmd, OnSetWobMinCmd)
	ON_BN_CLICKED(IDC_BUTTON_SetAutoWobbleCmd, OnSetAutoWobbleCmd)
	ON_BN_CLICKED(IDC_BUTTON_SetWobMaxCmd, OnSetWobMaxCmd)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LNStestLED4Cmd, OnLNStestLED4)
	ON_BN_CLICKED(IDC_BUTTON_ReadSelectedADCcmd, OnReadSelectedADC)
	ON_BN_CLICKED(IDC_BUTTON_SCNreadBoardID, OnSCNreadBoardID)
	ON_BN_CLICKED(IDC_BUTTON_LNSreadBoardID, OnLNSreadBoardID)
	ON_BN_CLICKED(IDC_CHECK_Stig_Ctrl, OnCHECKStigCtrl)
	ON_BN_CLICKED(IDC_BUTTON_WrMuxClear, OnWrMuxClear)
	ON_BN_CLICKED(IDC_BUTTON_SCN_ExpChip_ClearCurrent, OnSCNExpChipClearCurrent)
	ON_BN_CLICKED(IDC_BUTTON_SCN_ExpChip_ClearAll, OnSCNExpChipClearAll)
	ON_BN_CLICKED(IDC_CHECK_ReadSelectedADCAuto, OnCHECKReadSelectedADCAuto)
	ON_BN_CLICKED(IDC_BUTTON_cmdNormalMode, OnScanModeNormal)
	ON_BN_CLICKED(IDC_BUTTON_cmdLineMode_Horizontal, OnScanModeLineHorizontal)
	ON_BN_CLICKED(IDC_BUTTON_cmdLineMode_Vertical, OnScanModeLineVertical)
	ON_BN_CLICKED(IDC_BUTTON_cmdSpotMode, OnScanModeSpot)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreeze, OnFreeze)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreezeX, OnFreezeX)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreezeY, OnFreezeY)
	ON_BN_CLICKED(IDC_CHECK_MainsLock, OnCHECKMainsLock)
	ON_BN_CLICKED(IDC_CHECK_Video, OnCHECKVideo)
	ON_BN_CLICKED(IDC_BUTTON_StartScanCmd, OnStartScan)
	ON_BN_CLICKED(IDC_BUTTON_StopScanCmd, OnStopScan)
	ON_BN_CLICKED(IDC_BUTTON_SetPxlClkto2Cmd, OnSetPxlClkto2)
	ON_BN_CLICKED(IDC_BUTTON_DFInit, OnDFInit)
	ON_BN_CLICKED(IDC_CHECK_DFNegative, OnCHECKDFNegative)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_AmuxSelOption0, IDC_RADIO_AmuxSelOption2, OnAmuxSelOptionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCselectOB0, IDC_RADIO_ADCselectOB1, OnADCselectOBChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCsChSelect0, IDC_RADIO_ADCsChSelect15, OnADCsChSelectChanged)
	ON_COMMAND_RANGE(IDC_RADIO_Amp_Main, IDC_RADIO_Amp_Low, OnAMPselOptionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_DFModeOB0, IDC_RADIO_DFModeOB2, OnDFselectOBChanged)
	ON_COMMAND_RANGE(IDC_RADIO_Mag_1, IDC_RADIO_Mag_5, OnMagOBChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSL message handlers

BOOL CPageSL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/////////////////////////////////////////////////////////////////
	// SCN
	m_scrollXstigDAC.SetScrollRange( -2048, 2047 );
	m_scrollYstigDAC.SetScrollRange( -2048, 2047 );
	m_scrollXshiftDAC.SetScrollRange( -2048, 2047 );
	m_scrollYshiftDAC.SetScrollRange( -2048, 2047 );
	m_scrollXxoverDAC.SetScrollRange( -2048, 2047 );
	m_scrollYxoverDAC.SetScrollRange( -2048, 2047 );
	m_scrollIOexp.SetScrollRange( 0, 31 );

	m_btnSCNsoftReset.SetColor( g_crScanEmphasis );
	m_btnSCNtestLED4.SetColor( g_crOFF );
//	m_btnSCNtestLED4.SetWindowText( "Test LED is now OFF" );
	m_btnSCNreadBoardID.SetColor( g_crSYS );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// LSN
	m_scrollL1DAC.SetScrollRange( -32768, 32767 );//-16384, 4096 );//
	m_scrollL2SDAC.SetScrollRange( -32768, 32767 );//-16384, 4096 );//
	m_scrollXalDAC.SetScrollRange( -32768, 32767 );//-16384, 4096 );//
	m_scrollYalDAC.SetScrollRange( -32768, 32767 );//-16384, 4096 );//
	m_scrollWobDAC.SetScrollRange( -2048, 2047 );
	m_scrollDFStart.SetScrollRange( -32768, 32767 );
	m_scrollDFEnd.SetScrollRange( -32768, 32767 );
	m_scrollDFSteps.SetScrollRange( 0, 0x400 );
//	((CButton*)(GetDlgItem(IDC_RADIO_DFModeOB0)))->SetCheck( 1 );
	GetDlgItem(IDC_EDIT_DFModeBits)->SetWindowText( "DF mode 4: 0000 0100" );
	m_scrollDFEnd.SetScrollPos( 0xFC00 -32768 );

	m_btnL1CompDisable.SetColor( g_crOFF );
	m_btnL2CompDisable.SetColor( g_crOFF );
	m_btnXalCompDisable.SetColor( g_crOFF );
	m_btnYalCompDisable.SetColor( g_crOFF );

//	((CButton*)(GetDlgItem( IDC_RADIO_ADCselOption2 )))->SetCheck(1);
	m_btnLNSsoftReset.SetColor( g_crScanEmphasis );
	m_btnLNStestLED4.SetColor( g_crOFF );
//	m_btnLNStestLED4.SetWindowText( "Test LED is now OFF" );
	m_btnLNSreadBoardID.SetColor( g_crSYS );

	m_bAutoWobblerOn = FALSE;
	m_btnAutoWob.SetColor( ::GetSysColor(COLOR_3DFACE) );
//	m_btnAutoWob.SetWindowText( "Turn off Auto Wobble" );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// MCI
	m_btnModeNormal.SetColor( g_crEmphasis1 );
	m_btnModeLineHorizontal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeSpot.SetColor( ::GetSysColor(COLOR_3DFACE) );
	GetDlgItem( IDC_EDIT_spotX_HB )->SetWindowText( "00" );
	GetDlgItem( IDC_EDIT_spotX_LB )->SetWindowText( "00" );
	GetDlgItem( IDC_EDIT_spotY_HB )->SetWindowText( "00" );
	GetDlgItem( IDC_EDIT_spotY_LB )->SetWindowText( "00" );

	CString str;
	m_btnMainsLock.SetColor( ::GetSysColor(COLOR_3DFACE) );
	str.LoadString( IDS_MCI_Mainslock_ON );
	m_btnMainsLock.SetWindowText( str );
	m_btnVideo.SetColor( g_crEmphasis2 );
	str.LoadString( IDS_MCI_Video_ON );
	m_btnVideo.SetWindowText( str );
	m_btnVideo.SetCheck( 1 );
	/////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageSL::Init()
{
	m_scrollXstigDAC.SetScrollPos( 0 );
	XstigDACSB_Change( 0 );
	m_scrollYstigDAC.SetScrollPos( 0 );
	YstigDACSB_Change( 0 );
	m_scrollXshiftDAC.SetScrollPos( 0 );
	XshiftDACSB_Change( 0 );
	m_scrollYshiftDAC.SetScrollPos( 0 );
	YshiftDACSB_Change( 0 );
	m_scrollXxoverDAC.SetScrollPos( 0 );
	XxoverDACSB_Change( 0 );
	m_scrollYxoverDAC.SetScrollPos( 0 );
	YxoverDACSB_Change( 0 );
	m_scrollIOexp.SetScrollPos( 0 );
/*
	m_scrollL1DAC.SetScrollPos( 0x8000 -32768 );
	L1DACSB_Change( 0x8000 -32768 );
	m_scrollL2SDAC.SetScrollPos( 0x8000 -32768 );
	L2SDACSB_Change( 0x8000 - 32768 );
	m_scrollXalDAC.SetScrollPos( 0 );
	XalDACSB_Change( 0 );
	m_scrollYalDAC.SetScrollPos( 0 );
	YalDACSB_Change( 0 );
	m_scrollWobDAC.SetScrollPos( 0 );
	WobDACSB_Change( 0 );

	((CButton*)(GetDlgItem(IDC_RADIO_DFModeOB1)))->SetCheck( 1 );
	ChangeDFselectOB( 1 );
	m_scrollDFStart.SetScrollPos( 0 );
	DFStartSB_Change( 0 );
	m_scrollDFEnd.SetScrollPos( 0xFC00 -32768 );
	DFEndSB_Change( 0xFC00 -32768 );
	m_scrollDFSteps.SetScrollPos( 0 );
	DFStepsSB_Change( 0 );
*/
	// 2015.09.11 实验，只送一路X对中值
//	m_scrollXalDAC.SetScrollPos( 0 );
//	XalDACSB_Change( 0 );
}

void CPageSL::OnOK() 
{
}

void CPageSL::OnCancel() 
{
}

BOOL CPageSL::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_XstigDAC:
		case	IDC_EDIT_YstigDAC:
		case	IDC_EDIT_XshiftDAC:
		case	IDC_EDIT_YshiftDAC:
		case	IDC_EDIT_XxoverDAC:
		case	IDC_EDIT_YxoverDAC:
		case	IDC_EDIT_L1DAC:
		case	IDC_EDIT_L2SDAC:
		case	IDC_EDIT_L2DDAC:
		case	IDC_EDIT_XalDAC:
		case	IDC_EDIT_YalDAC:
		case	IDC_EDIT_WobDAC:
		case	IDC_EDIT_DFStart:
		case	IDC_EDIT_DFEnd:
		case	IDC_EDIT_DFDACsteps:
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
		case	IDC_EDIT_XstigDAC:
			m_scrollXstigDAC.SetScrollPos( nPos -2048 );
			XstigDACSB_Change( nPos -2048 );
			break;
		case	IDC_EDIT_YstigDAC:
			m_scrollYstigDAC.SetScrollPos( nPos -2048 );
			YstigDACSB_Change( nPos -2048 );
			break;
		case	IDC_EDIT_XshiftDAC:
			m_scrollXshiftDAC.SetScrollPos( nPos -2048 );
			XshiftDACSB_Change( nPos -2048 );
			break;
		case	IDC_EDIT_YshiftDAC:
			m_scrollYshiftDAC.SetScrollPos( nPos -2048 );
			YshiftDACSB_Change( nPos -2048 );
			break;
		case	IDC_EDIT_XxoverDAC:
			m_scrollXxoverDAC.SetScrollPos( nPos -2048 );
			XxoverDACSB_Change( nPos -2048 );
			break;
		case	IDC_EDIT_YxoverDAC:
			m_scrollYxoverDAC.SetScrollPos( nPos -2048 );
			YxoverDACSB_Change( nPos -2048 );
			break;
		case	IDC_EDIT_L1DAC:
			m_scrollL1DAC.SetScrollPos( nPos -32768 );
			L1DACSB_Change( nPos -32768 );
			break;
		case	IDC_EDIT_L2SDAC:
			m_scrollL2SDAC.SetScrollPos( nPos -32768 );
			L2SDACSB_Change( nPos -32768 );
			break;
		case	IDC_EDIT_XalDAC:
			m_scrollXalDAC.SetScrollPos( nPos -32768 );
			XalDACSB_Change( nPos -32768 );
			break;
		case	IDC_EDIT_YalDAC:
			m_scrollYalDAC.SetScrollPos( nPos -32768 );
			YalDACSB_Change( nPos -32768 );
			break;
		case	IDC_EDIT_WobDAC:
			m_scrollWobDAC.SetScrollPos( nPos -2048 );
			WobDACSB_Change( nPos -2048 );
			break;
		case	IDC_EDIT_DFStart:
			m_scrollDFStart.SetScrollPos( nPos -32768 );
			DFStartSB_Change( nPos -32768 );
			break;
		case	IDC_EDIT_DFEnd:
			m_scrollDFEnd.SetScrollPos( nPos -32768 );
			DFEndSB_Change( nPos -32768 );
			break;
		case	IDC_EDIT_DFDACsteps:
			m_scrollDFSteps.SetScrollPos( nPos );
			DFStepsSB_Change( nPos );
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPageSL::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	case IDC_SCROLLBAR_XstigDACSB:
		XstigDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_YstigDACSB:
		YstigDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_XshiftDACSB:
		XshiftDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_YshiftDACSB:
		YshiftDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_XxoverDACSB:
		XxoverDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_YxoverDACSB:
		YxoverDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_IOexp:
		IOexpSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_L1DAC:
		L1DACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_L2SDAC:
		L2SDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_XalDAC:
		XalDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_YalDAC:
		YalDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_WobDAC:
		WobDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_DFStart:
		DFStartSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_DFEnd:
		DFEndSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_DFDACsteps:
		DFStepsSB_Change( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPageSL::XstigDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollXstigDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 2048;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_XstigDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_Stig_X].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if Y), MSB = 0 if X.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Stigmation,		// Serial Comm operand low byte (X/Y Stig DAC circuit address)
							USB_TM_SCN_Write_16,		// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::YstigDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollYstigDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 2048;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_YstigDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_Stig_Y].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if Y), MSB = 0 if X.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Stigmation,		// Serial Comm operand low byte (X/Y Stig DAC circuit address)
							USB_TM_SCN_Write_16,		// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::XshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollXshiftDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 2048;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_XshiftDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_Shift_X].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
							USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::YshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollYshiftDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 2048;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_YshiftDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_Shift_Y].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
							USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::XxoverDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollXxoverDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 2048;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_XxoverDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_xover_Y].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
							USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::YxoverDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollYxoverDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 2048;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_YxoverDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_xover_X].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
							USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::OnAmuxSelOptionChanged(UINT nID)
{
	CString str;
	int nOption = nID - IDC_RADIO_AmuxSelOption0;
	BOOL bCheck;

	// Globals m_byteXYgainFBbits, m_byteXYlowFBbits and m_byteXYlowMuxbits keep track of user bit settings.
	switch( nOption )
	{
	case	0:		// Main Gain and FB control (2 bits ea.)
// 		( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) )->SetWindowText( _T("FB Sel 2") );
// 		( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) )->SetWindowText( _T("FB Sel 1") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) )->ShowWindow( SW_HIDE );
		( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) )->ShowWindow( SW_HIDE );
		( GetDlgItem( IDC_CHECK_XYamux_Bit1 ) )->SetWindowText( _T("Gain 2") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit0 ) )->SetWindowText( _T("Gain 1") );
		m_byteAmuxAddrX	= USB_CID_SCN_XMainGain_FeedbackSetting;
		m_byteAmuxAddrY	= USB_CID_SCN_YMainGain_FeedbackSetting;

		// Load existing bit values into checkboxes (all 0's if not specified yet):
		bCheck = FLAGBIT( m_byteXYgainFBbits, USB_Flag_Bit3 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYgainFBbits, USB_Flag_Bit2 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYgainFBbits, USB_Flag_Bit1 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit1 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYgainFBbits, USB_Flag_Bit0 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit0 ) ) )->SetCheck( bCheck );

		str = "80";	// ADC Channel 0, S.E., Uni/Gain = 00, NAP/Sleep = 00		??????????
		break;
	case	1:		// Low range FB control (4 bits)
//		ADCchanSel = &HC0 'ADC Channel 1, S.E., Uni/Gain = 00, NAP/Sleep = 00
		( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) )->SetWindowText( _T("FB Sel 4") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) )->SetWindowText( _T("FB Sel 3") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit1 ) )->SetWindowText( _T("FB Sel 2") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit0 ) )->SetWindowText( _T("FB Sel 1") );
		m_byteAmuxAddrX	= USB_CID_SCN_XLowRangeFeedbackSelect;
		m_byteAmuxAddrY	= USB_CID_SCN_YLowRangeFeedbackSelect;

		// Load existing bit values into checkboxes (all 0's if not specified yet):
		bCheck = FLAGBIT( m_byteXYlowFBbits, USB_Flag_Bit3 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYlowFBbits, USB_Flag_Bit2 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYlowFBbits, USB_Flag_Bit1 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit1 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYlowFBbits, USB_Flag_Bit0 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit0 ) ) )->SetCheck( bCheck );

 		str = "C0";	// ADC Channel 1, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	case	2:		// Low range Mux control (4 bits)
//		ADCchanSel = &H90 'ADC Channel 2, S.E., Uni/Gain = 00, NAP/Sleep = 00
		( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) )->ShowWindow( SW_SHOW );
		( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) )->ShowWindow( SW_SHOW );
		( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) )->SetWindowText( _T("Mux Sel 4") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) )->SetWindowText( _T("Mux Sel 3") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit1 ) )->SetWindowText( _T("Mux Sel 2") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit0 ) )->SetWindowText( _T("Mux Sel 1") );
		m_byteAmuxAddrX	= USB_CID_SCN_XLowRangeAnalogMultiplexerSel;
		m_byteAmuxAddrY	= USB_CID_SCN_YLowRangeAnalogMultiplexerSel;

		// Load existing bit values into checkboxes (all 0's if not specified yet):
		bCheck = FLAGBIT( m_byteXYlowMuxbits, USB_Flag_Bit3 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYlowMuxbits, USB_Flag_Bit2 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYlowMuxbits, USB_Flag_Bit1 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit1 ) ) )->SetCheck( bCheck );
		bCheck = FLAGBIT( m_byteXYlowMuxbits, USB_Flag_Bit0 );
		( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit0 ) ) )->SetCheck( bCheck );

		str = "90";	// ADC Channel 2, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	}
	// 11.11.18与Peter & Dan 确认，这是SCN的功能，与LNS无关，所以去掉以下两句
//	CInsample0Dlg* pParent = (CInsample0Dlg*)GetParentOwner();
//	pParent->GetDlgItem( IDC_EDIT_ADCchanSel )->SetWindowText( str );
}

int CPageSL::GetAmuxSelOption()
{
	int i;
	for( i=0; i<3; i++ )
	{
		if( ((CButton*)(GetDlgItem( IDC_RADIO_AmuxSelOption0 + i )))->GetCheck() )
			break;
	}

	if( i==3 )
		i = -1;

	return i;
}

void CPageSL::OnWrMuxCtl() 
{
	// TODO: Add your control notification handler code here
	CString str = "";
	BYTE byteBitValues = 0;

    ///////////////////////////////////////////////////////////////////////////
	if( ((CButton*)(GetDlgItem( IDC_CHECK_XYamux_Bit3 )))->GetCheck() )
		byteBitValues |= USB_Flag_Bit3;
	if( ((CButton*)(GetDlgItem( IDC_CHECK_XYamux_Bit2 )))->GetCheck() )
		byteBitValues |= USB_Flag_Bit2;
	if( ((CButton*)(GetDlgItem( IDC_CHECK_XYamux_Bit1 )))->GetCheck() )
		byteBitValues |= USB_Flag_Bit1;
	if( ((CButton*)(GetDlgItem( IDC_CHECK_XYamux_Bit0 )))->GetCheck() )
		byteBitValues |= USB_Flag_Bit0;

	switch( m_byteAmuxAddrX )
	{
	case	USB_CID_SCN_XMainGain_FeedbackSetting:
		m_byteXYgainFBbits	= byteBitValues;
		break;
	case	USB_CID_SCN_XLowRangeFeedbackSelect:
		m_byteXYlowFBbits	= byteBitValues;
		break;
	case	USB_CID_SCN_XLowRangeAnalogMultiplexerSel:
		m_byteXYlowMuxbits	= byteBitValues;
		break;
	}

	str.Format( "Written: 0x%X to 0x%X and 0x%X", byteBitValues, m_byteAmuxAddrX, m_byteAmuxAddrY );
	GetDlgItem( IDC_EDIT_WrMuxCtl )->SetWindowText( str );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x11 (8 bit write to Board 1), low byte = 0x24 (X gain/FB ctl.)
	// First write X values:
	theApp.CommToBoards2(	m_byteAmuxAddrX,		// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
							USB_TM_SCN_Write_08,	// Serial Comm operand high byte (8 bit write to SCN board)
							byteBitValues,			// Serial Comm operand low byte
							byteBitValues );		// Serial Comm operand high byte

	// Now write Y values:
	theApp.CommToBoards2(	m_byteAmuxAddrY,		// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
							USB_TM_SCN_Write_08,	// Serial Comm operand high byte (8 bit write to SCN board)
							byteBitValues,			// Serial Comm operand low byte
							byteBitValues );		// Serial Comm operand high byte
}

void CPageSL::OnWrMuxClear() 
{
	// TODO: Add your control notification handler code here
	CString str = "";
	BYTE byteBitValues = 0;

	switch( m_byteAmuxAddrX )
	{
	case	USB_CID_SCN_XMainGain_FeedbackSetting:
		m_byteXYgainFBbits	= byteBitValues;
		break;
	case	USB_CID_SCN_XLowRangeFeedbackSelect:
		m_byteXYlowFBbits	= byteBitValues;
		break;
	case	USB_CID_SCN_XLowRangeAnalogMultiplexerSel:
		m_byteXYlowMuxbits	= byteBitValues;
		break;
	}
	( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) ) )->SetCheck( 0 );
	( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) ) )->SetCheck( 0 );
	( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit1 ) ) )->SetCheck( 0 );
	( (CButton*)( GetDlgItem( IDC_CHECK_XYamux_Bit0 ) ) )->SetCheck( 0 );

	str.Format( "Written: 0x%X to 0x%X and 0x%X", byteBitValues, m_byteAmuxAddrX, m_byteAmuxAddrY );
	GetDlgItem( IDC_EDIT_WrMuxCtl )->SetWindowText( str );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x11 (8 bit write to Board 1), low byte = 0x24 (X gain/FB ctl.)
	// First write X values:
	theApp.CommToBoards2(	m_byteAmuxAddrX,		// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
							USB_TM_SCN_Write_08,	// Serial Comm operand high byte (8 bit write to SCN board)
							byteBitValues,			// Serial Comm operand low byte
							byteBitValues );		// Serial Comm operand high byte

	// Now write Y values:
	theApp.CommToBoards2(	m_byteAmuxAddrY,		// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
							USB_TM_SCN_Write_08,	// Serial Comm operand high byte (8 bit write to SCN board)
							byteBitValues,			// Serial Comm operand low byte
							byteBitValues );		// Serial Comm operand high byte
}

void CPageSL::IOexpSB_Change( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	switch( nPos )
	{
	case	0:
		str += " ( not used )";
		break;
	case	1:
		str += " ( Y gain rly ctrl )";//" ( X cros 1 )";
		break;
	case	2:
		str += " ( X cros 2 )";
		break;
	case	3:
		str += " ( X cros 3 )";
		break;
	case	4:
		str += " ( X cros 4 )";
		break;
	case	5:
		str += " ( X cros FB sel 1 )";
		break;
	case	6:
		str += " ( X cros FB sel 2 )";
		break;
	case	7:
		str += " ( X cros FB sel 3 )";
		break;
	case	8:
		str += " ( X cros FB sel 4 )";
		break;
	case	9:
		str += " ( Y cros 1 )";
		break;
	case	10:
		str += " ( Y cros 2 )";
		break;
	case	11:
		str += " ( Y cros 3 )";
		break;
	case	12:
		str += " ( Y cros 4 )";
		break;
	case	13:
		str += " ( Y cros FB sel 1 )";
		break;
	case	14:
		str += " ( Y cros FB sel 2 )";
		break;
	case	15:
		str += " ( Y cros FB sel 3 )";
		break;
	case	16:
		str += " ( Y cros FB sel 4 )";
		break;
	case	17:
		str += " ( X gain Rly Ctrl )";
		break;
	case	18:
		str += " ( X Short Rly Ctrl )";
		break;
	case	19:
		str += " ( X Amp Stig E/S Ctrl )";
		break;
	case	20:
		str += " ( X Main Rly Ctrl )";
		break;
	case	21:
		str += " ( X Main Cath Ctrl )";
		break;
	case	22:
		str += " ( X Main Amp E/S Ctrl )";
		break;
	case	23:
		str += " ( X Shift E/S Ctrl )";	// " ( X Cros E/S Ctrl )";
		break;
	case	24:
		str += " ( X Low Amp E/S Ctrl )";
		break;
	case	25:
		str += " ( Y Short Rly Ctrl )";
		break;
	case	26:
		str += " ( Y Amp Stig E/S Ctrl )";
		break;
	case	27:
		str += " ( Y Main Rly Ctrl )";
		break;
	case	28:
		str += " ( Y Main Cath Ctrl )";
		break;
	case	29:
		str += " ( Y Main Amp E/S Ctrl )";
		break;
	case	30:
		str += " ( Y Shift E/S Ctrl )";	// " ( Y Cros E/S Ctrl )";
		break;
	case	31:
		str += " ( Y Low Amp E/S Ctrl )";
		break;
	default:
		str += " (not used)";
		break;
	}
	GetDlgItem( IDC_EDIT_IOexp )->SetWindowText( str );
}

void CPageSL::OnIOXstbHi() 
{
	// TODO: Add your control notification handler code here
	BYTE bytePos = m_scrollIOexp.GetScrollPos();
	bytePos |= 0x80;
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip,	// Low byte = ckt addr. of SCN I/O Expander
							USB_TM_SCN_Write_08,		// Serial Comm operand high byte (&H11 = 8 bit write to SCN board)
							bytePos,	// OR in Strobe Hi bit, which will occur 125nS later
							bytePos );	// Hi byte data is actually low byte of 8 bit write
}

void CPageSL::OnIOXstbLo() 
{
	// TODO: Add your control notification handler code here
	BYTE bytePos = m_scrollIOexp.GetScrollPos();
	bytePos |= 0x40;
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip,	// Low byte = ckt addr. of SCN I/O Expander
							USB_TM_SCN_Write_08,		// Serial Comm operand high byte (&H11 = 8 bit write to SCN board)
							bytePos,	// OR in Strobe Lo bit, which will occur 125nS later
							bytePos );	// Hi byte data is actually low byte of 8 bit write
}

void CPageSL::OnSCNsoftReset() 
{
	// TODO: Add your control notification handler code here
	theApp.CommToBoards2(	USB_CID_SCN_Soft_Reset,	// Low byte = ckt addr. of Scan board soft reset
							USB_TM_SCN_Write_08,	// Serial Comm operand high byte (8 bit write to SCN board)
							0,						// Low byte is data-less
							0 );					// Hi byte is data-less

	// Force DAC7822 DAC control sliders(and resulting values/reports) to 0 (center) @ soft reset:
    m_scrollXstigDAC.SetScrollPos( 0 );
	XstigDACSB_Change( 0 );
    m_scrollYstigDAC.SetScrollPos( 0 );
	YstigDACSB_Change( 0 );
    m_scrollXshiftDAC.SetScrollPos( 0 );
	XshiftDACSB_Change( 0 );
    m_scrollYshiftDAC.SetScrollPos( 0 );
	YshiftDACSB_Change( 0 );
    m_scrollXxoverDAC.SetScrollPos( 0 );
	XxoverDACSB_Change( 0 );
    m_scrollYxoverDAC.SetScrollPos( 0 );
	YxoverDACSB_Change( 0 );
	m_scrollIOexp.SetScrollPos( 0 );
}

void CPageSL::OnSCNtestLED4Cmd() 
{
	// TODO: Add your control notification handler code here
	CString str, strTemp;
	m_bSCNtestLED4 = !m_bSCNtestLED4;
	if( m_bSCNtestLED4 )
	{
		m_btnSCNtestLED4.SetColor( g_crON_Test );
		strTemp.LoadString( IDS_Test_LED_ON );
		str.Format( "SCN: %s", strTemp );
		m_btnSCNtestLED4.SetWindowText( str );
	}
	else
	{
		m_btnSCNtestLED4.SetColor( g_crOFF );
		strTemp.LoadString( IDS_Test_LED_OFF );
		str.Format( "SCN: %s", strTemp );
		m_btnSCNtestLED4.SetWindowText( str );
	}

	///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_SCN_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_SCN_Write_08,		// Serial Comm operand high byte (8 bit write to SCN board)
							0x0,						// OP3 = 0 for 8 bit write
							(BYTE)m_bSCNtestLED4 );		// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CPageSL::OnSCNreadBoardID() 
{
	// TODO: Add your control notification handler code here
	WORD wData1, wData2;
	CString strMsg = "";
	theApp.BoardReadback( USB_CID_SCN_ReadBoardID, USB_TM_SCN_Read_08, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPageSL::OnTestW8() 
{
	// TODO: Add your control notification handler code here
	theApp.CommToBoards2(	0xD0,						// Operand 1 = ckt addr. of Test LED 4
							0x14,						// Operand 2 = 0x14 (8 bit write to SPR board)???????
							0x0,						// Operand 3: Always set to 0!
							0x11 );						// Operand 4: ONLY valid data byte for 8 bit write
}

void CPageSL::OnTestW16() 
{
	// TODO: Add your control notification handler code here
	theApp.CommToBoards2(	USB_CID_SCN_Test_LED,		// Operand 1 = ckt addr. of Test LED 4
							0x24,						// Operand 2 = 0x24 (16 bit write to SPR board)???????
							0x22,						// operand 3 = 2nd data byte of 2
							0x33 );						// operand 4 = 1st data byte of 2
}

void CPageSL::OnTestW24() 
{
	// TODO: Add your control notification handler code here
	theApp.CommToBoards3(	0x12,						// operand 1 = ckt addr. of Test LED 4
							0x33,						// operand 2 = 0x34 (24 bit write to VAC board)???????
							0xDD,						// operand 3 = 2nd byte of 3
							0x0,						// operand 4 = 1st byte of 3
    // Following set is ONLY necessary for 24 bit or 32 bit writes:
							0x0,						// operand 5: ALWAYS set to 0!
							0x66 );						// operand 6 = 3rd byte of 3
}

void CPageSL::OnTestW32() 
{
	// TODO: Add your control notification handler code here
	theApp.CommToBoards3(	0xD0,						// operand 1 = ckt addr. of Test LED 4
							0x44,						// operand 2 = 0x44 (32 bit write to SPR board)???????
							0x22,						// operand 3 = 2nd data byte of 4
							0x33,						// operand 4 = 1st data byte of 4
    // Following set is ONLY necessary for 24 bit or 32 bit writes:
							0x44,						// operand 5 = 4th data byte of 4
							0x66 );						// operand 6 = 3rd data byte of 4
}







void CPageSL::OnL1CompDisable() 
{
	// Disable/Enable control for drift compensation of L1 circuit
	// Dim L1CompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: L1CompOn keeps track of current software state of control of this function. All bets are off if out of sync.
	int nCheck = m_btnL1CompDisable.GetCheck();
	if( nCheck )
	{
		m_btnL1CompDisable.SetColor( g_crON_Comm );	// light RED (to simulate Comp Fn is ON)
		m_btnL1CompDisable.SetWindowText( "ON" );
	}
	else
	{
		m_btnL1CompDisable.SetColor( g_crOFF );		// gray (to simulate Comp Fn is OFF)
		m_btnL1CompDisable.SetWindowText( "OFF" );
	}
	theApp.m_DL.log.nCheckComp[Logging_Comp_CL -5] = nCheck;

	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x1A as low byte (L1 compensation disable ctl.)
	theApp.CommToBoards2(	USB_CID_LNS_CondenserLens_LoopTest,	// Serial Comm operand low byte (L1 compensation disable ctl.)
							USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
							nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
							nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CPageSL::OnL2CompDisable() 
{
	// Disable/Enable control for drift compensation of L2 circuit
	// Dim L2CompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: L2CompOn keeps track of current software state of control of this function. All bets are off if out of sync.
	int nCheck = m_btnL2CompDisable.GetCheck();
	if( nCheck )
	{
		m_btnL2CompDisable.SetColor( g_crON_Comm );	// light RED (to simulate Comp Fn is ON)
		m_btnL2CompDisable.SetWindowText( "ON" );
	}
	else
	{
		m_btnL2CompDisable.SetColor( g_crOFF );		// gray (to simulate Comp Fn is OFF)
		m_btnL2CompDisable.SetWindowText( "OFF" );
	}
	theApp.m_DL.log.nCheckComp[Logging_Comp_OL -5] = nCheck;

	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x2A as low byte (L2 compensation disable ctl.)
	theApp.CommToBoards2(	USB_CID_LNS_ObjectiveLens_LoopTest,	// Serial Comm operand low byte (L2 compensation disable ctl.)
							USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
							nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
							nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CPageSL::OnXalCompDisable() 
{
	// Disable/Enable control for drift compensation of L2 circuit
	// Dim XalCompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: XalCompOn keeps track of current software state of control of this function. All bets are off if out of sync.
	int nCheck = m_btnXalCompDisable.GetCheck();
	if( nCheck )
	{
		m_btnXalCompDisable.SetColor( g_crON_Comm );// light RED (to simulate Comp Fn is ON)
		m_btnXalCompDisable.SetWindowText( "ON" );
	}
	else
	{
		m_btnXalCompDisable.SetColor( g_crOFF );	// gray (to simulate Comp Fn is OFF)
		m_btnXalCompDisable.SetWindowText( "OFF" );
	}
	theApp.m_DL.log.nCheckComp[Logging_Comp_Align_X -5] = nCheck;

	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x4A as low byte (Xalign compensation disable ctl.)
	theApp.CommToBoards2(	USB_CID_LNS_XAlignment_LoopTest,// Serial Comm operand low byte (Xalign compensation disable ctl.)
							USB_TM_LNS_Write_08,			// Serial Comm operand high byte (8 bit write to LNS board)
							nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
							nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CPageSL::OnYalCompDisable() 
{
	// Disable/Enable control for drift compensation of Yalign circuit
	// Dim XalCompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: YalCompOn keeps track of current software state of control of this function. All bets are off if out of sync.
	int nCheck = m_btnYalCompDisable.GetCheck();
	if( nCheck )
	{
		m_btnYalCompDisable.SetColor( g_crON_Comm );// light RED (to simulate Comp Fn is ON)
		m_btnYalCompDisable.SetWindowText( "ON" );
	}
	else
	{
		m_btnYalCompDisable.SetColor( g_crOFF );	// gray (to simulate Comp Fn is OFF)
		m_btnYalCompDisable.SetWindowText( "OFF" );
	}
	theApp.m_DL.log.nCheckComp[Logging_Comp_Align_Y -5] = nCheck;

	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x5A as low byte (Yalign compensation disable ctl.)
	theApp.CommToBoards2(	USB_CID_LNS_YAlignment_LoopTest,// Serial Comm operand low byte (Yalign compensation disable ctl.)
							USB_TM_LNS_Write_08,			// Serial Comm operand high byte (8 bit write to LNS board)
							nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
							nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CPageSL::L1DACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;
	double dVolts, dAmps;

	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollL1DAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 32768;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_L1DAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_CL].nCurPos = nPos;

	dVolts = nValue / 65536.0 * 5 - 2.5;
	str.Format( "%.3f", dVolts );
	GetDlgItem( IDC_EDIT_L1dacV )->SetWindowText( str );

	dAmps = dVolts * 12.69 / 15.7;	// estimated DCR of C lens is ~15.7 Ohms
	str.Format( "%.3f", dAmps );
	GetDlgItem( IDC_EDIT_L1dacI )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x12 as low byte (L1DAC)
	theApp.CommToBoards2(	USB_CID_LNS_CondenserLens,	// Serial Comm operand low byte (Condenser Lens DAC circuit address)
							USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::L2SDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;
	double dVolts, dAmps;

	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollL2SDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 32768;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_L2SDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_OL].nCurPos = nPos;

	dVolts = nValue / 65536.0 * 5 - 2.5;
	str.Format( "%.3f", dVolts );
	GetDlgItem( IDC_EDIT_L2sdacV )->SetWindowText( str );

	dAmps = dVolts * 12.69 / 15.7;	// estimated DCR of C lens is ~15.7 Ohms
	str.Format( "%.3f", dAmps );
	GetDlgItem( IDC_EDIT_L2sdacI )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x22 as low byte (L2SDAC)
	theApp.CommToBoards2(	USB_CID_LNS_ObjectiveLens_Static,// Serial Comm operand low byte (Obj Lens Static DAC circuit address)
							USB_TM_LNS_Write_16,			// Serial Comm operand high byte (16 bit write to LNS board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
// 	theApp.CommToBoards2(	USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint,// Serial Comm operand low byte (Obj Lens Static DAC circuit address)
// 							USB_TM_LNS_Write_16,			// Serial Comm operand high byte (16 bit write to LNS board)
// 							0,		// Serial Comm operand low byte
// 							0x80 );	// Serial Comm operand high byte

	GetDlgItem( IDC_EDIT_DFStart )->GetWindowText( str );
	if( str.IsEmpty() )
		return;
	int nPosDF;
	char *bufNum = str.GetBuffer(4);
	sscanf(bufNum, "%x", &nPosDF);
	DFStartSB_Change( nPosDF -32768 );
}

void CPageSL::XalDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;
	double dVolts, dAmps;

	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollXalDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 32768;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_XalDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_Align_X].nCurPos = nPos;

	dVolts = nValue / 65536.0 * 5 - 2.5;
	str.Format( "%.3f", dVolts );
	GetDlgItem( IDC_EDIT_XaldacV )->SetWindowText( str );

	dAmps = dVolts * 5.333 / 85;	// estimated DCR of alignment coils is ~85 Ohms
	str.Format( "%.3f", dAmps );
	GetDlgItem( IDC_EDIT_XaldacI )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x42 as low byte (XalDAC)
	theApp.CommToBoards2(	USB_CID_LNS_XAlignment,		// Serial Comm operand low byte (Obj Lens Static DAC circuit address)
							USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::YalDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;
	double dVolts, dAmps;

	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollYalDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 32768;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_YalDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_Align_Y].nCurPos = nPos;

	dVolts = nValue / 65536.0 * 5 - 2.5;
	str.Format( "%.3f", dVolts );
	GetDlgItem( IDC_EDIT_YaldacV )->SetWindowText( str );

	dAmps = dVolts * 5.333 / 85;	// estimated DCR of alignment coils is ~85 Ohms
	str.Format( "%.3f", dAmps );
	GetDlgItem( IDC_EDIT_YaldacI )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x52 as low byte (YalDAC)
	theApp.CommToBoards2(	USB_CID_LNS_YAlignment,	// Serial Comm operand low byte (Y alignment DAC circuit address)
							USB_TM_LNS_Write_16,	// Serial Comm operand high byte (16 bit write to LNS board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::WobDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;
	double dVolts;

	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollWobDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 2048;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_WobDAC )->SetWindowText( str );
	theApp.m_DL.diag[Diag_Wob].nCurPos = nPos;

	dVolts = nValue / 4095.0 * 5 - 2.5;
	str.Format( "%.3f", dVolts );
	GetDlgItem( IDC_EDIT_WobDacV )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
	theApp.CommToBoards2(	USB_CID_LNS_WobblerSetting,	// Serial Comm operand low byte (Y alignment DAC circuit address)
							USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::DFStartSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollDFStart.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 32768;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_DFStart )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
	theApp.CommToBoards2(	USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint,	// Serial Comm operand low byte (Y alignment DAC circuit address)
							USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::DFEndSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollDFEnd.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos + 32768;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_DFEnd )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
	theApp.CommToBoards2(	USB_CID_LNS_ObjectiveLens_DnyFocusEndBound,	// Serial Comm operand low byte (Y alignment DAC circuit address)
							USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::DFStepsSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	int nMinpos, nMaxpos;
	m_scrollDFSteps.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_DFDACsteps )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
	theApp.CommToBoards2(	USB_CID_LNS_ObjectiveLens_DynFocusSteps,	// Serial Comm operand low byte (Y alignment DAC circuit address)
							USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CPageSL::OnDFselectOBChanged( UINT nID )
{
	ChangeDFselectOB( nID -IDC_RADIO_DFModeOB0 );
}

void CPageSL::ChangeDFselectOB( int nChan )
{
	// Sets and reads globals variables: DFonFull (bit 1), DFonAtten (bit 0), DFrampNeg (bit 2)
	BYTE byteDFAtten = 0x00;
	switch( nChan )
	{
	case	0:	// Off
		break;
	case	1:	// 10%
		byteDFAtten	= 0x01;
		break;
	case	2:	// Full
		byteDFAtten	= 0x02;
		break;
	}

	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_DFNegative)))->GetCheck();
	if( nCheck < 1 )
		byteDFAtten |= 0x04;	// bit2 = 1, pos
	CString str;
	str.Format( "DF mode %d: 0000 0%d%d%d", byteDFAtten, (byteDFAtten & 0x04)>>2, (byteDFAtten & 0x02)>>1, byteDFAtten & 0x01 );
	GetDlgItem( IDC_EDIT_DFModeBits )->SetWindowText( str );

	theApp.CommToBoards2(	USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten,
							USB_TM_LNS_Write_08,
							byteDFAtten,
							byteDFAtten );
}

void CPageSL::OnCHECKDFNegative() 
{
	// Sets and reads globals variables: DFonFull (bit 1), DFonAtten (bit 0), DFrampNeg (bit 2)
	BYTE byteDFAtten = 0x00;
	int nCheck = ((CButton*)(GetDlgItem(IDC_RADIO_DFModeOB0)))->GetCheck();
	if( nCheck )
		byteDFAtten	= 0x00;		// Off
	else
	{
		nCheck = ((CButton*)(GetDlgItem(IDC_RADIO_DFModeOB1)))->GetCheck();
		if( nCheck )
			byteDFAtten	= 0x01;	// 10%
		else
			byteDFAtten	= 0x02;	// Full
	}

	nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_DFNegative)))->GetCheck();
	if( nCheck < 1 )
		byteDFAtten |= 0x04;	// bit2 = 1, pos
	CString str;
	str.Format( "DF mode %d: 0000 0%d%d%d", byteDFAtten, (byteDFAtten & 0x04)>>2, (byteDFAtten & 0x02)>>1, byteDFAtten & 0x01 );
	GetDlgItem( IDC_EDIT_DFModeBits )->SetWindowText( str );

	theApp.CommToBoards2(	USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten,
							USB_TM_LNS_Write_08,
							byteDFAtten,
							byteDFAtten );
}

void CPageSL::OnDFInit() 
{
	m_scrollDFStart.SetScrollPos( 0 );
	DFStartSB_Change( 0 );
// 	m_scrollDFEnd.SetScrollPos( 0 );
// 	DFEndSB_Change( 0 );
	m_scrollDFEnd.SetScrollPos( 0xFC00 -32768 );
	DFEndSB_Change( 0xFC00 -32768 );
	m_scrollDFSteps.SetScrollPos( 0 );
	DFStepsSB_Change( 0 );

	((CButton*)(GetDlgItem(IDC_RADIO_DFModeOB1)))->SetCheck( 1 );
	ChangeDFselectOB( 1 );
}

void CPageSL::OnLNSsoftReset() 
{
	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2),
	// 0xEE as low byte (Low byte = ckt addr. of Lens board soft reset)
	theApp.CommToBoards2(	USB_CID_LNS_Soft_Reset,	// Low byte = ckt addr. of Lens board soft reset
							USB_TM_LNS_Write_08,	// Serial Comm operand high byte (8 bit write to LNS board)
//							USB_TM_LNS_Write_16 );	// Serial Comm operand high byte (16 bit write to LNS board)
							0,			// Low byte is data-less
							0 );		// High byte is data-less

	// Force DAC control sliders and resulting reports to 0:
    m_scrollL1DAC.SetScrollPos( 0 );
	L1DACSB_Change( 0 );
    m_scrollL2SDAC.SetScrollPos( 0 );
	L2SDACSB_Change( 0 );
    m_scrollXalDAC.SetScrollPos( 0 );
	XalDACSB_Change( 0 );
    m_scrollYalDAC.SetScrollPos( 0 );
	YalDACSB_Change( 0 );
    m_scrollWobDAC.SetScrollPos( 0 );
	WobDACSB_Change( 0 );
	m_scrollDFStart.SetScrollPos( 0 );
	DFStartSB_Change( 0 );
	m_scrollDFEnd.SetScrollPos( 0 );
	DFEndSB_Change( 0 );
	m_scrollDFSteps.SetScrollPos( 0 );
	DFStepsSB_Change( 0 );
}

void CPageSL::OnLNStestLED4() 
{
	// TODO: Add your control notification handler code here
	CString str, strTemp;
	m_bLNStestLED4 = !m_bLNStestLED4;
	if( m_bLNStestLED4 )
	{
		m_btnLNStestLED4.SetColor( g_crON_Test );
		strTemp.LoadString( IDS_Test_LED_ON );
		str.Format( "LNS: %s", strTemp );
		m_btnLNStestLED4.SetWindowText( str );
	}
	else
	{
		m_btnLNStestLED4.SetColor( g_crOFF );
		strTemp.LoadString( IDS_Test_LED_OFF );
		str.Format( "LNS: %s", strTemp );
		m_btnLNStestLED4.SetWindowText( str );
	}

	///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_LNS_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_LNS_Write_08,		// Serial Comm operand high byte (8 bit write to SCN board)
							0x0,						// OP3 = 0 for 8 bit write
							(BYTE)m_bLNStestLED4 );		// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CPageSL::OnLNSreadBoardID() 
{
	// TODO: Add your control notification handler code here
	WORD wData1, wData2;
	CString strMsg = "";
	theApp.BoardReadback( USB_CID_LNS_ReadBoardID, USB_TM_LNS_Read_08, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPageSL::OnSetWobMaxCmd() 
{
	// Utilizes global: AutoWobblerMax, used by Auto Wobbler timer
	m_nAutoWobblerMax = m_scrollWobDAC.GetScrollPos();	// Use actual slider value, not variable value
}

void CPageSL::OnSetWobMinCmd() 
{
	// Utilizes global: AutoWobblerMin, used by Auto Wobbler timer
	m_nAutoWobblerMin = m_scrollWobDAC.GetScrollPos();	// Use actual slider value, not variable value
}

void CPageSL::OnSetAutoWobbleCmd() 
{
	// Utilizes global: AutoWobblerOn
	CString str;
	m_bAutoWobblerOn = !m_bAutoWobblerOn;
	if( m_bAutoWobblerOn )
	{
		m_btnAutoWob.SetColor( g_crON_Test );
		str.LoadString( IDS_LNS_AutoWob_ON );
		m_btnAutoWob.SetWindowText( str );
		m_bAutoWobblerDir	= TRUE;
		m_nAutoWobblerLast	= 
		SetTimer( 1, 50, NULL );
	}
	else
	{
		m_btnAutoWob.SetColor( ::GetSysColor(COLOR_3DFACE) );
		str.LoadString( IDS_LNS_AutoWob_OFF );
		m_btnAutoWob.SetWindowText( str );
		KillTimer( 1 );
		m_bAutoWobblerDir = FALSE;
	}
}

void CPageSL::OnAMPselOptionChanged(UINT nID)
{
	int nOption = nID - IDC_RADIO_Amp_Main;
//	if( nOption == m_nOldAMPselOption )
//		return;

	if( nOption == 0 )
	{
		// Main Amp: 24,31,17,1 off; 20,21,22,27,28,29 on
		BYTE bytePos = 0x40 | 0x18;	// 24
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1F;		// 31
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x11;		// 17
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x40 | 0x01;		// 1
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x80 | 0x14;		// 20
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x80 | 0x15;		// 21
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x16;		// 22
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x1B;		// 27
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x1C;		// 28
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x80 | 0x1D;		// 29
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
	}
	else if( nOption == 1 )
	{
		// Low Amp: 20,21,22,27,28,29 off; 24,31,17,1 on
		BYTE bytePos = 0x40 | 0x14;	// 20
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x15;		// 21
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x16;		// 22
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1B;		// 27
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1C;		// 28
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x18;		// 24
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x1F;		// 31
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x11;		// 17
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x01;		// 1
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
	}
	else
	{
		// Low Amp: 20,21,22,27,28,29 off; 17,1 off; 24,31 on
		BYTE bytePos = 0x40 | 0x14;	// 20
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x15;		// 21
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x16;		// 22
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1B;		// 27
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1C;		// 28
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x11;		// 17
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x01;		// 1
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x18;		// 24
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x1F;		// 31
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
	}
	m_nOldAMPselOption = nOption;
}

void CPageSL::OnMagOBChanged(UINT nID)
{
	int nOption = nID - IDC_RADIO_Mag_1;
	switch( nOption )
	{
	case	0:
		UpdateStage15();
		break;
	case	1:
		UpdateStage150();
		break;
	case	2:
		UpdateStage1500();
		break;
	case	3:
		UpdateStage15000();
		break;
	case	4:
		UpdateStage150000();
		break;
	}
}

BOOL CPageSL::UpdateStrobe( BYTE bID, BOOL bHigh )
{
	BYTE bytePos = bID;
	if( bHigh )
		bytePos |= 0x80;
	else
		bytePos |= 0x40;

	theApp.CommToBoards2( USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	return TRUE;
}

BOOL CPageSL::ClearAnalogSwitches()
{
	BYTE byteBitValues = 0;
	/////////////////////////////////////////////////////////////////
	// MainGain 1 (X & Y)
	// MainFB 1 (X & Y)
	theApp.CommToBoards2( USB_CID_SCN_XMainGain_FeedbackSetting, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// XYLowRngMux
	theApp.CommToBoards2( USB_CID_SCN_XLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// XYLowRngFB
	theApp.CommToBoards2( USB_CID_SCN_XLowRangeFeedbackSelect, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	/////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CPageSL::UpdateStage15()
{
	// Approx mag: 15 to 150
	// Current range: >2.5A to 250mA

	/////////////////////////////////////////////////////////////////
	// X) 24 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, FALSE ) )
		return FALSE;
	// Y) 31 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, FALSE ) )
		return FALSE;

	// XGainRly 17 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, FALSE ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, FALSE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	ClearAnalogSwitches();

	/////////////////////////////////////////////////////////////////
	// X) 20, 21, 22 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, TRUE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, TRUE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, TRUE ) )
		return FALSE;

	// Y) 27, 28, 29 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, TRUE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, TRUE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, TRUE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////
/*
	/////////////////////////////////////////////////////////////////
	// MainGain 1 (X & Y)
	// MainFB 1 (X & Y)
	BYTE byteBitValues = 0;
//	byteBitValues |= USB_Flag_Bit2;	// FB
	byteBitValues |= USB_Flag_Bit0;	// Gain
	// Write via MCI to SCN board.  First write 0x11 (8 bit write to Board 1), low byte = 0x24 (X gain/FB ctl.)
	// First write X values:
	theApp.CommToBoards2( USB_CID_SCN_XMainGain_FeedbackSetting, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	// Now write Y values:
	theApp.CommToBoards2( USB_CID_SCN_YMainGain_FeedbackSetting, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	/////////////////////////////////////////////////////////////////
*/
	return TRUE;
}

BOOL CPageSL::UpdateStage150()
{
	// Approx mag: 150 to 1500
	// Current range: 250mA to 25mA

	/////////////////////////////////////////////////////////////////
	// X) 20, 21, 22 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, FALSE ) )
		return FALSE;

	// Y) 27, 28, 29 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, FALSE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	ClearAnalogSwitches();

	/////////////////////////////////////////////////////////////////
	// Xlow E/S) 24 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, TRUE ) )
		return FALSE;
	// Ylow E/S) 31 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, TRUE ) )
		return FALSE;

	// XGainRly 17 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, TRUE ) )
		return FALSE;
	// YGainRly 1 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, TRUE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	BYTE byteBitValues = 0;
	/////////////////////////////////////////////////////////////////
	// XYLowRngMux) 4
	byteBitValues |= USB_Flag_Bit3;
	// First write X values:
	theApp.CommToBoards2( USB_CID_SCN_XLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	// Now write Y values:
	theApp.CommToBoards2( USB_CID_SCN_YLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );

	return TRUE;
}

BOOL CPageSL::UpdateStage1500()
{
	// Approx mag: 1500 to 15K
	// Current range: 25mA to 2.5mA

	/////////////////////////////////////////////////////////////////
	// X) 20, 21, 22 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, FALSE ) )
		return FALSE;

	// Y) 27, 28, 29 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, FALSE ) )
		return FALSE;

	// XGainRly 17 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, FALSE ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, FALSE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	ClearAnalogSwitches();
	
	/////////////////////////////////////////////////////////////////
	// Xlow E/S) 24 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, TRUE ) )
		return FALSE;

	// Ylow E/S) 31 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, TRUE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	BYTE byteBitValues = 0;
	/////////////////////////////////////////////////////////////////
	// XYLowRngMux 3
	byteBitValues |= USB_Flag_Bit2;
	// First write X values:
	theApp.CommToBoards2( USB_CID_SCN_XLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	// Now write Y values:
	theApp.CommToBoards2( USB_CID_SCN_YLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );

	return TRUE;
}

BOOL CPageSL::UpdateStage15000()
{
	// Approx mag: 15K to 150K
	// Current range: 2.5mA to 250uA

	/////////////////////////////////////////////////////////////////
	// X) 20, 21, 22 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, FALSE ) )
		return FALSE;

	// Y) 27, 28, 29 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, FALSE ) )
		return FALSE;

	// XGainRly 17 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, FALSE ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, FALSE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	ClearAnalogSwitches();
	
	/////////////////////////////////////////////////////////////////
	// Xlow E/S) 24 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, TRUE ) )
		return FALSE;

	// Ylow E/S) 31 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, TRUE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	BYTE byteBitValues = 0;
	/////////////////////////////////////////////////////////////////
	// XYLowRngMux 2
	byteBitValues |= USB_Flag_Bit1;
	// First write X values:
	theApp.CommToBoards2( USB_CID_SCN_XLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	// Now write Y values:
	theApp.CommToBoards2( USB_CID_SCN_YLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );

	return TRUE;
}

BOOL CPageSL::UpdateStage150000()
{
	// Approx mag: >150K
	// Current range: <250uA

	/////////////////////////////////////////////////////////////////
	// X) 20, 21, 22 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, FALSE ) )
		return FALSE;

	// Y) 27, 28, 29 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, FALSE ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, FALSE ) )
		return FALSE;

	// XGainRly 17 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, FALSE ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, FALSE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	ClearAnalogSwitches();
	
	/////////////////////////////////////////////////////////////////
	// Xlow E/S) 24 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, TRUE ) )
		return FALSE;

	// Ylow E/S) 31 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, TRUE ) )
		return FALSE;
	/////////////////////////////////////////////////////////////////

	BYTE byteBitValues = 0;
	/////////////////////////////////////////////////////////////////
	// XYLowRngMux 1
	byteBitValues |= USB_Flag_Bit0;
	// First write X values:
	theApp.CommToBoards2( USB_CID_SCN_XLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
	// Now write Y values:
	theApp.CommToBoards2( USB_CID_SCN_YLowRangeAnalogMultiplexerSel, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );

	return TRUE;
}

void CPageSL::OnCHECKStigCtrl() 
{
	// Stig Ctrl switch
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Stig_Ctrl)))->GetCheck();
	if( nCheck )
	{
		BYTE bytePos = 0x80 | 0x13;	// 19
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x1A;		// 26
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x80 | 0x17;		// 23
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x80 | 0x1E;		// 30
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x40 | 0x12;		// 18
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x40 | 0x19;		// 25
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
	}
	else
	{
		BYTE bytePos = 0x40 | 0x13;	// 19
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1A;		// 26
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x17;		// 23
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x40 | 0x1E;		// 30
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x40 | 0x12;		// 18
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x40 | 0x19;		// 25
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
	}
}

void CPageSL::OnSCNExpChipClearCurrent() 
{
	if( m_nOldAMPselOption == 0 )
	{
		// Main Amp: 20,21,22,27,28,29 on, clear them
		BYTE bytePos = 0x40 | 0x14;	// 20
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x15;		// 21
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x16;		// 22
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1B;		// 27
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1C;		// 28
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
	}
	else if( m_nOldAMPselOption == 1 )
	{
		// Low Amp: 24,31,17,1 on, clear them
		BYTE bytePos = 0x40 | 0x18;	// 24
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1F;		// 31
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x11;		// 17
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x01;		// 1
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
	}
	else
	{
		// Low Amp: 24,31 on, clear them
		BYTE bytePos = 0x40 | 0x18;	// 24
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
		bytePos = 0x40 | 0x1F;		// 31
		theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
								bytePos, bytePos );
	}
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Main )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Low1 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Low )))->SetCheck(0);
	m_nOldAMPselOption = -1;
}

void CPageSL::OnSCNExpChipClearAll() 
{
	// Main Amp: 20,21,22,27,28,29
	BYTE bytePos = 0x40 | 0x14;	// 20
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x15;		// 21
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x16;		// 22
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x1B;		// 27
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x1C;		// 28
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x1D;		// 29
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	// Low Amp: 24,31
	bytePos = 0x40 | 0x18;		// 24
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x1F;		// 31
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x11;		// 17
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x01;		// 1
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	// Stig Ctrl switch
	bytePos = 0x40 | 0x13;		// 19
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x1A;		// 26
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	// ShortRly
	bytePos = 0x40 | 0x12;		// 18
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x19;		// 25
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	// Shift E/S
	bytePos = 0x40 | 0x17;		// 23
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );
	bytePos = 0x40 | 0x1E;		// 30
	theApp.CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
							bytePos, bytePos );

	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Main )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Low1 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Low )))->SetCheck(0);
	m_nOldAMPselOption = -1;
	((CButton*)(GetDlgItem(IDC_CHECK_Stig_Ctrl)))->SetCheck(0);
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void CPageSL::OnADCselectOBChanged( UINT nID )
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

void CPageSL::OnADCsChSelectChanged( UINT nID )
{
	theApp.ChangedADCsChSelect( nID -IDC_RADIO_ADCsChSelect0 );

	// show assignments for each of the channels (16 for all except for LNS board, which as 8)
	CString str;
	str.Format( "%d", nID -IDC_RADIO_ADCsChSelect0 );
	GetDlgItem( IDC_EDIT_ADCchanSelTB )->SetWindowText( str );
}

void CPageSL::OnCHECKReadSelectedADCAuto() 
{
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_ReadSelectedADCAuto )))->GetCheck();
	if( nCheck )
		SetTimer( 2, 4000, NULL );
	else
		KillTimer( 2 );
}

int CPageSL::GetADCselOption()
{
	int i;
	for( i=0; i<8; i++ )
	{
		if( ((CButton*)(GetDlgItem( IDC_RADIO_ADCselOption0 + i )))->GetCheck() )
			break;
	}

	if( i==8 )
		i = -1;

	return i;
}

void CPageSL::OnReadADC() 
{
	int nADCchanSel = GetADCselOption();
	// Write via MCI to LNS board.  Instruction 1: Select ADC on LNS board
	theApp.CommToBoards2(	USB_CID_LNS_ADC_Write,					// LNS board circuit address for LNS board ADC = C2
							USB_TM_LNS_Write_16,					// Serial Comm operand high byte (16 bit write to LNS board)
	// Write via MCI to LNS board.  Instruction 2: Write channel select to ADC
							nADCchanSel,			// Serial Comm operand low byte (this one is ignored by ADC)
							nADCchanSel );			// Serial Comm operand high byte (this one matters)

	// Write via MCI to LNS board.  Instruction 3: Select ADC Start Conversion on LNS board (8 bit write)
	theApp.CommToBoards2(	USB_CID_LNS_ADC_HD_Start_Conversion,	// Circuit address for LNS board Start ADC conversion = C4
							USB_TM_LNS_Write_08,					// Serial Comm operand high byte (8 bit write to LNS board)
	// Write via MCI to LNS board.  Instruction 4: Start conversion is data-less, but must complete comm cycle.
							0x00,					// data-less write
							0x00 );					// data-less write

	// Write via MCI to LNS board.  Instruction 5: Select ADC Read (ckt address C8, 16 bit read)
	theApp.CommToBoards2(	0xC6,					// Circuit address for LNS board Start ADC conversion = C6
							USB_TM_LNS_Read_16,		// Serial Comm operand high byte (0xA2 = 16 bit read from LNS board)
	// Write via MCI to LNS board.  Instruction 6: Read of ADC is data-less MCI operand, but must complete comm cycle.
							0x00,					// data-less write
							0x00 );					// data-less write

	// HERE IS WHERE WE WILL NEED TO GRAB THE DATA THAT COMES BACK FROM ADC, AND SEND OVER USB TO HOST
}

void CPageSL::OnReadSelectedADC() 
{
	// TODO: Add your control notification handler code here
	theApp.ReadSelectedADC();
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
	ADCvconv();
}

long CPageSL::BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata )
{
	CString strMsgCounts, strMsgData;

	long lRet = theApp.BoardRead0816( nRsize, nModID, bCktAddr, wWdata, strMsgCounts, strMsgData, TRUE );
	strMsgData = strMsgCounts + "\r\n" + strMsgData;
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsgData );

	return lRet;
}

long CPageSL::BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata )
{
	CString strMsgCounts, strMsgData;

	long lRet = theApp.BoardRead2432( nRsize, nModID, bCktAddr, dwWdata, strMsgCounts, strMsgData, TRUE );
	strMsgData = strMsgCounts + "\r\n" + strMsgData;
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsgData );

	return lRet;
}

void CPageSL::ADCvconv()
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str;
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = theApp.m_DL.adc.chn[theApp.m_nADCnum*16 +theApp.m_nADCchn].dMin;
	double dRangeMax = theApp.m_DL.adc.chn[theApp.m_nADCnum*16 +theApp.m_nADCchn].dMax;
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
		}
		else
		{
			str.Format( "-%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "-%.5f", dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			str.Format( "%.5f", theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCOMoffset - dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );
		}
		dADCreadVrefFrac = dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref /2;
		if( (dRangeMin != dRangeMax)
			&& (dADCreadVrefFrac < dRangeMin || dADCreadVrefFrac > dRangeMax) )
			GetDlgItem( IDC_EDIT_ADCreadingTB )->SetWindowText( "Exceed Range!!!" );
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
			str.Format( "%.5f", dADCreadVrefFrac * 5 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );	// Same - no adj. needed
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "%.5f", dADCreadVrefFrac * 5 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );	// Same - no adj. needed
		}
	}	// If LTC1859 ADC read

	double dEquivCurrent = dADCreadVrefFrac * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dVref / 2 * theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].dFactor > 0 )
	{
		GetDlgItem( IDC_STATIC_CurrentMeasLabel )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->ShowWindow( SW_SHOW );
		str.Format( "%.3f", dEquivCurrent );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->SetWindowText( str );
	}
	else
	{
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->SetWindowText( "n/a" );
		GetDlgItem( IDC_STATIC_CurrentMeasLabel )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->ShowWindow( SW_HIDE );
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// MCI
/*
Opcodes:	0x49 - freezeX, operand = pixel number
				MCI computes Xramp DAC value as Xoffset + pixel number * dXrise
			0x4A - freezeY, operand = line number
				MCI computes Yramp DAC value as Yoffset + line number * dYrise

			0x43 - normal scan mode, no operand
			0x44 - horizotal line mode, line number given by "freezeY"
			0x45 - vertical line mode, line number given by "freezeX"
			0x46 - spot mode, with spot at (freezeX, freezeY)
*/

void CPageSL::OnScanModeNormal() 
{
	theApp.WriteToBoard( USB_ScanModeNormal, FALSE, 0 );
	m_btnModeNormal.SetColor( g_crEmphasis1 );
	m_btnModeLineHorizontal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineVertical.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeSpot.SetColor( ::GetSysColor(COLOR_3DFACE) );
	Invalidate( FALSE );
}

void CPageSL::OnScanModeLineHorizontal() 
{
	theApp.WriteToBoard( USB_ScanModeLineHorizontal, FALSE, 0 );
	m_btnModeNormal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineHorizontal.SetColor( g_crEmphasis2 );
	m_btnModeLineVertical.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeSpot.SetColor( ::GetSysColor(COLOR_3DFACE) );
	Invalidate( FALSE );
}

void CPageSL::OnScanModeLineVertical() 
{
	theApp.WriteToBoard( USB_ScanModeLineVertical, FALSE, 0 );
	m_btnModeNormal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineHorizontal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineVertical.SetColor( g_crEmphasis2 );
	m_btnModeSpot.SetColor( ::GetSysColor(COLOR_3DFACE) );
	Invalidate( FALSE );
}

void CPageSL::OnScanModeSpot() 
{
//	theApp.WriteToBoard( USB_ScanModeLineHorizontal, FALSE, 0 );
	theApp.WriteToBoard( USB_ScanModeSpot, FALSE, 0 );
	m_btnModeNormal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineHorizontal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineVertical.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeSpot.SetColor( g_crEmphasis3 );
	Invalidate( FALSE );
}

void CPageSL::OnFreeze() 
{
	unsigned char output;
	WORD wData;
	CString str = "";
	char* buf;

	// Freaze X
	GetDlgItemText( IDC_EDIT_spotX_HB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData = (WORD)output << 8;

	GetDlgItemText( IDC_EDIT_spotX_LB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData |= output;
	theApp.WriteToBoard( USB_FreezeX, TRUE, wData );

	// Freeze Y
	GetDlgItemText( IDC_EDIT_spotY_HB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData = (WORD)output << 8;

	GetDlgItemText( IDC_EDIT_spotY_LB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData |= output;
	theApp.WriteToBoard( USB_FreezeY, TRUE, wData );
}

void CPageSL::OnFreezeX() 
{
	unsigned char output;
	WORD wData;
	CString str = "";
	char* buf;

	// Freaze X
	GetDlgItemText( IDC_EDIT_spotX_HB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData = (WORD)output << 8;

	GetDlgItemText( IDC_EDIT_spotX_LB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData |= output;
	theApp.WriteToBoard( USB_FreezeX, TRUE, wData );
}

void CPageSL::OnFreezeY() 
{
	unsigned char output;
	WORD wData;
	CString str = "";
	char* buf;

	// Freeze Y
	GetDlgItemText( IDC_EDIT_spotY_HB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData = (WORD)output << 8;

	GetDlgItemText( IDC_EDIT_spotY_LB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData |= output;
	theApp.WriteToBoard( USB_FreezeY, TRUE, wData );
}

void CPageSL::OnCHECKMainsLock() 
{
	// Toggle MainsLock
	CString str;
	int nCheck = m_btnMainsLock.GetCheck();
	if( nCheck )
	{
		m_btnMainsLock.SetColor( g_crEmphasis1 );
		str.LoadString( IDS_MCI_Mainslock_ON );
		m_btnMainsLock.SetWindowText( str );
		theApp.WriteToBoard( USB_MainsLock_ON, FALSE, 0 );
	}
	else
	{
		m_btnMainsLock.SetColor( ::GetSysColor(COLOR_3DFACE) );
		str.LoadString( IDS_MCI_Mainslock_OFF );
		m_btnMainsLock.SetWindowText( str );
		theApp.WriteToBoard( USB_MainsLock_OFF, FALSE, 0 );
	}
}

void CPageSL::OnCHECKVideo() 
{
	// Toggle Video
	CString str;
	int nCheck = m_btnVideo.GetCheck();
	if( nCheck )
	{
		m_btnVideo.SetColor( g_crEmphasis2 );
		str.LoadString( IDS_MCI_Video_ON );
		m_btnVideo.SetWindowText( str );
		theApp.WriteToBoard( USB_Video_ON, FALSE, 0 );
	}
	else
	{
		m_btnVideo.SetColor( ::GetSysColor(COLOR_3DFACE) );
		str.LoadString( IDS_MCI_Video_OFF );
		m_btnVideo.SetWindowText( str );
		theApp.WriteToBoard( USB_Video_OFF, FALSE, 0 );
	}
}

void CPageSL::OnStartScan() 
{
	StartScan();
}

void CPageSL::StartScan()
{
	// Start scan
	theApp.WriteToBoard( USB_StartScan, FALSE, 0 );

	// setup the signal
	theApp.m_bIsStop = FALSE;
}

void CPageSL::OnStopScan() 
{
	StopScan();
}

void CPageSL::StopScan()
{
	// Send binary 100 on Port C and 0x13 on Port A, then bin 111 on C, then 000 on C to start scan
	theApp.WriteToBoard( USB_StopScan, FALSE, 0 );

	// setup the signal
	theApp.m_bIsStop = TRUE;
}

void CPageSL::OnSetPxlClkto2() 
{
	SetPxlClkto2();
}

void CPageSL::SetPxlClkto2()
{
	theApp.WriteToBoard( USB_SetPixelRate, TRUE, 0x02 );
}
///////////////////////////////////////////////////////////////////////////////


void CPageSL::InitDiagnose( BOOL bEnd )
{
	KillTimer( 1 );
	KillTimer( 2 );

	int i;
	for( i=0; i<16; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_ADCsChSelect0 +i )))->SetCheck( 0 );
	for( i=0; i<2; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_ADCselectOB0 +i )))->SetCheck( 0 );

	GetDlgItem( IDC_BUTTON_ReadSelectedADCcmd )->EnableWindow( bEnd );
	GetDlgItem( IDC_CHECK_ReadSelectedADCAuto )->EnableWindow( bEnd );
}

void CPageSL::SetTimers( UINT nIDEvent, BOOL bSet )
{
	if( bSet )
	{
		switch( nIDEvent )
		{
		case	Diag_Stig_X:
			m_scrollXstigDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			XstigDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_Stig_Y:
			m_scrollYstigDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			YstigDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_Shift_X:
			m_scrollXshiftDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			XshiftDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_Shift_Y:
			m_scrollYshiftDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			YshiftDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_xover_X:
			m_scrollXxoverDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			XxoverDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_xover_Y:
			m_scrollYxoverDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			YxoverDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_CL:
			m_scrollL1DAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			L1DACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			break;
		case	Diag_OL:
			m_scrollL2SDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			L2SDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			break;
		case	Diag_Align_X:
			m_scrollXalDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			XalDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			break;
		case	Diag_Align_Y:
			m_scrollYalDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			YalDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin -32768 );
			break;
		case	Diag_Wob:
			m_scrollWobDAC.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin  -2048);
			WobDACSB_Change( theApp.m_DL.diag[nIDEvent].nMin  -2048);
			break;
		default:
			break;
		}
//		SetTimer( nIDEvent +100, theApp.m_DL.diag[nIDEvent].nIntv, NULL );
	}
	else
		KillTimer( nIDEvent +100 );
}

void CPageSL::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1 )
	{
		// 自动摇摆
		// Utilizes globals: AutoWobblerOn, AutoWobblerMax and AutoWobblerMin, AutoWobblerLast, AutoWobblerDir
		int nAutoWobblerStep, nAutoWobblerNew;
		// this size step should result in one full cycle per sec.
		nAutoWobblerStep = (m_nAutoWobblerMax - m_nAutoWobblerMin) /10;

		if( m_bAutoWobblerOn )
		{
			if( m_bAutoWobblerDir )	// heading up
			{
				nAutoWobblerNew = m_nAutoWobblerLast + nAutoWobblerStep;
				if( nAutoWobblerNew > m_nAutoWobblerMax )
				{
					nAutoWobblerNew		= m_nAutoWobblerMax;
					m_bAutoWobblerDir	= FALSE;	// Hit max, change direction to DOWN
				}
			}
			else
			{
				nAutoWobblerNew = m_nAutoWobblerLast - nAutoWobblerStep;
				if( nAutoWobblerNew < m_nAutoWobblerMin )
				{
					nAutoWobblerNew		= m_nAutoWobblerMin;
					m_bAutoWobblerDir	= TRUE;		// Hit min, change direction to UP
				}
			}
			m_nAutoWobblerLast = nAutoWobblerNew;
			// force slider to new value, which triggers an update communication to hardware
			m_scrollWobDAC.SetScrollPos( nAutoWobblerNew );
			WobDACSB_Change( nAutoWobblerNew );
		}
	}
	else if( nIDEvent == 2 )
	{
		// 读取ADC返回值
		if( theApp.m_DL.adc.chn[theApp.m_nADCnum*16+theApp.m_nADCchn].bType )		// 2448
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
		}
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	else if( nIDEvent == Diag_Stig_X +100 )
	{
		int nPos = m_scrollXstigDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollXstigDAC.SetScrollPos( nPos );
			XstigDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollXstigDAC.SetScrollPos( nPos );
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Stig_Y +100 )
	{
		int nPos = m_scrollYstigDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollYstigDAC.SetScrollPos( nPos );
			YstigDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollYstigDAC.SetScrollPos( nPos );
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Shift_X +100 )
	{
		int nPos = m_scrollXshiftDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollXshiftDAC.SetScrollPos( nPos );
			XshiftDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollXshiftDAC.SetScrollPos( nPos );
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Shift_Y +100 )
	{
		int nPos = m_scrollYshiftDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollYshiftDAC.SetScrollPos( nPos );
			YshiftDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollYshiftDAC.SetScrollPos( nPos );
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_xover_X +100 )
	{
		int nPos = m_scrollXxoverDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollXxoverDAC.SetScrollPos( nPos );
			XxoverDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollXxoverDAC.SetScrollPos( nPos );
			XxoverDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_xover_Y +100 )
	{
		int nPos = m_scrollYxoverDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollYxoverDAC.SetScrollPos( nPos );
			YxoverDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollYxoverDAC.SetScrollPos( nPos );
			YxoverDACSB_Change( nPos );
		}
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	else if( nIDEvent == Diag_CL +100 )
	{
		int nPos = m_scrollL1DAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -32768;
			m_scrollL1DAC.SetScrollPos( nPos );
			L1DACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollL1DAC.SetScrollPos( nPos );
			L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_OL +100 )
	{
		int nPos = m_scrollL2SDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -32768;
			m_scrollL2SDAC.SetScrollPos( nPos );
			L2SDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollL2SDAC.SetScrollPos( nPos );
			L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Align_X +100 )
	{
		int nPos = m_scrollXalDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -32768;
			m_scrollXalDAC.SetScrollPos( nPos );
			XalDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollXalDAC.SetScrollPos( nPos );
			XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Align_Y +100 )
	{
		int nPos = m_scrollYalDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -32768;
			m_scrollYalDAC.SetScrollPos( nPos );
			YalDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollYalDAC.SetScrollPos( nPos );
			YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Wob +100 )
	{
		int nPos = m_scrollWobDAC.GetScrollPos();
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollWobDAC.SetScrollPos( nPos );
			WobDACSB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollWobDAC.SetScrollPos( nPos );
			WobDACSB_Change( nPos );
		}
	}

	CDialog::OnTimer(nIDEvent);
}
