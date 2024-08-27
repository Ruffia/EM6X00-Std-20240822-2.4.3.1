// PanelSL.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "PanelSL.h"
#include "ControlLayer.h"
#include "BoardComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FLAGBIT( flag, flag_bit )	( ( (flag) & (flag_bit) ) == (flag_bit) )

/////////////////////////////////////////////////////////////////////////////
// CPanelSL dialog


CPanelSL::CPanelSL(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelSL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelSL)
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


void CPanelSL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelSL)
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
	DDX_Control(pDX, IDC_SCROLLBAR_L2DDAC, m_scrollL2DDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_L2SDAC, m_scrollL2SDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_L1DAC, m_scrollL1DAC);
	DDX_Control(pDX, IDC_CHECK_YalCompDisable, m_btnYalCompDisable);
	DDX_Control(pDX, IDC_CHECK_XalCompDisable, m_btnXalCompDisable);
	DDX_Control(pDX, IDC_CHECK_L2CompDisable, m_btnL2CompDisable);
	DDX_Control(pDX, IDC_CHECK_L1CompDisable, m_btnL1CompDisable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelSL, CDialog)
	//{{AFX_MSG_MAP(CPanelSL)
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
	ON_BN_CLICKED(IDC_CHECK_ReadLNSadcAuto, OnCHECKReadLNSadcAuto)
	ON_BN_CLICKED(IDC_BUTTON_ReadSelectedADCcmd, OnReadSelectedADC)
	ON_BN_CLICKED(IDC_BUTTON_SCNreadBoardID, OnSCNreadBoardID)
	ON_BN_CLICKED(IDC_BUTTON_LNSreadBoardID, OnLNSreadBoardID)
	ON_BN_CLICKED(IDC_CHECK_Stig_Ctrl, OnCHECKStigCtrl)
	ON_BN_CLICKED(IDC_BUTTON_WrMuxClear, OnWrMuxClear)
	ON_BN_CLICKED(IDC_BUTTON_SCN_ExpChip_ClearCurrent, OnSCNExpChipClearCurrent)
	ON_BN_CLICKED(IDC_BUTTON_SCN_ExpChip_ClearAll, OnSCNExpChipClearAll)
	ON_BN_CLICKED(IDC_BUTTON_WriteRateX, OnWriteRateX)
	ON_BN_CLICKED(IDC_BUTTON_WriteRateY, OnWriteRateY)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_AmuxSelOption0, IDC_RADIO_AmuxSelOption2, OnAmuxSelOptionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_ADCselOption0, IDC_RADIO_ADCselOption7, OnADCselOptionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_Amp_Main, IDC_RADIO_Amp_Low, OnAMPselOptionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_Wobbler_None, IDC_RADIO_Wobbler_ALY, OnWobblerselOptionChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelSL message handlers

BOOL CPanelSL::OnInitDialog() 
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

	m_btnSCNsoftReset.SetColor( RGB(0,255,255) );
	m_btnSCNtestLED4.SetColor( g_crOFF );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// LSN
	m_scrollL1DAC.SetScrollRange( -32768, 32767 );
	m_scrollL2SDAC.SetScrollRange( -32768, 32767 );
	m_scrollL2DDAC.SetScrollRange( -32768, 32767 );
	m_scrollXalDAC.SetScrollRange( -32768, 32767 );
	m_scrollYalDAC.SetScrollRange( -32768, 32767 );
	m_scrollWobDAC.SetScrollRange( -2048, 2047 );

	m_btnL1CompDisable.SetColor( g_crOFF );
	m_btnL2CompDisable.SetColor( g_crOFF );
	m_btnXalCompDisable.SetColor( g_crOFF );
	m_btnYalCompDisable.SetColor( g_crOFF );

//	((CButton*)(GetDlgItem( IDC_RADIO_ADCselOption2 )))->SetCheck(1);
	m_btnLNSsoftReset.SetColor( RGB(128,255,128) );
	m_btnLNStestLED4.SetColor( g_crOFF );

	m_bAutoWobblerOn = FALSE;
	m_btnAutoWob.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnAutoWob.SetWindowText( "Turn off Auto Wobble" );
	/////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPanelSL::Init()
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

	m_scrollL1DAC.SetScrollPos( 49152 -32768 );	// 0x4B42
	L1DACSB_Change( 49152 -32768 );
	m_scrollL2SDAC.SetScrollPos( 49152 -32768 );// 0x5310
	L2SDACSB_Change( 49152 -32768 );
	m_scrollXalDAC.SetScrollPos( 0 );	//0x88B6 -32768 );
	XalDACSB_Change( 0 );
	m_scrollYalDAC.SetScrollPos( 0 );	//0x6515 -32768 );
	YalDACSB_Change( 0 );
	m_scrollWobDAC.SetScrollPos( 0 );
	WobDACSB_Change( 0 );
}

void CPanelSL::OnOK() 
{
}

void CPanelSL::OnCancel() 
{
}

BOOL CPanelSL::PreTranslateMessage(MSG* pMsg) 
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
		case	IDC_EDIT_L2DDAC:
			m_scrollL2DDAC.SetScrollPos( nPos -32768 );
			L2DDACSB_Change( nPos -32768 );
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
			WobDACSB_Change( nPos -32768 );
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPanelSL::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	case IDC_SCROLLBAR_L2DDAC:
		L2DDACSB_Change( pScrollBar->GetScrollPos() );
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
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPanelSL::UpdateReadbackData( unsigned short* pnData )
{
	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", pnData[0] );
//	Message( str );
//	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = str;
	strMsg += "\r\n";
	unsigned long lCount = pnData[0];
	for( unsigned long i=1; i<=lCount; i++ )
	{
		str.Format( "%04X, ", pnData[i] );
//		strMsg += str;
		strList.Format( "@ %d: 0x%04X", i, pnData[i] );
		strMsg += str;
//		Message( strList );
	}
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( strMsg );
}

void CPanelSL::XstigDACSB_Change( int nPos )
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
	CControlLayer::Instance().m_DL.diag[Diag_Stig_X].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if Y), MSB = 0 if X.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Stigmation,		// Serial Comm operand low byte (X/Y Stig DAC circuit address)
										USB_TM_SCN_Write_16,		// Serial Comm operand high byte (16 bit write to SCN board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::YstigDACSB_Change( int nPos )
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
	CControlLayer::Instance().m_DL.diag[Diag_Stig_Y].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if Y), MSB = 0 if X.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Stigmation,		// Serial Comm operand low byte (X/Y Stig DAC circuit address)
										USB_TM_SCN_Write_16,		// Serial Comm operand high byte (16 bit write to SCN board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::XshiftDACSB_Change( int nPos )
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
	CControlLayer::Instance().m_DL.diag[Diag_Shift_X].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
										USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::YshiftDACSB_Change( int nPos )
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
	CControlLayer::Instance().m_DL.diag[Diag_Shift_Y].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
										USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::XxoverDACSB_Change( int nPos )
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
	CControlLayer::Instance().m_DL.diag[Diag_xover_X].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
										USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::YxoverDACSB_Change( int nPos )
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
	CControlLayer::Instance().m_DL.diag[Diag_xover_Y].nCurPos = nPos;

	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
										USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::SyncXOverPos( int nPosX, int nPosY )
{
	m_scrollXxoverDAC.SetScrollPos( nPosX -2048 );
	m_scrollYxoverDAC.SetScrollPos( nPosY -2048 );

	CString str;
	str.Format( "%X", nPosX );
	GetDlgItem( IDC_EDIT_XxoverDAC )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_xover_X].nCurPos = nPosX -2048;
	str.Format( "%X", nPosY );
	GetDlgItem( IDC_EDIT_YxoverDAC )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_xover_Y].nCurPos = nPosY -2048;
}

void CPanelSL::OnAmuxSelOptionChanged(UINT nID)
{
	CString str;
	int nOption = nID - IDC_RADIO_AmuxSelOption0;
	BOOL bCheck;

	// Globals m_byteXYgainFBbits, m_byteXYlowFBbits and m_byteXYlowMuxbits keep track of user bit settings.
	switch( nOption )
	{
	case	0:		// Main Gain and FB control (2 bits ea.)
		( GetDlgItem( IDC_CHECK_XYamux_Bit3 ) )->SetWindowText( _T("FB Sel 2") );
		( GetDlgItem( IDC_CHECK_XYamux_Bit2 ) )->SetWindowText( _T("FB Sel 1") );
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

int CPanelSL::GetAmuxSelOption()
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

void CPanelSL::OnWrMuxCtl() 
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
		m_byteXYlowFBbits		= byteBitValues;
		break;
	case	USB_CID_SCN_XLowRangeAnalogMultiplexerSel:
		m_byteXYlowMuxbits	= byteBitValues;
		break;
	}

	str.Format( "Written: 0x%X to\r\n0x%X and 0x%X", byteBitValues, m_byteAmuxAddrX, m_byteAmuxAddrY );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( str );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x11 (8 bit write to Board 1), low byte = 0x24 (X gain/FB ctl.)
	// First write X values:
	CSemCtrl::Instance().USB_CommToBoards2(	m_byteAmuxAddrX,		// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
										USB_TM_SCN_Write_08,	// Serial Comm operand high byte (8 bit write to SCN board)
										byteBitValues,			// Serial Comm operand low byte
										byteBitValues );		// Serial Comm operand high byte

	// Now write Y values:
	CSemCtrl::Instance().USB_CommToBoards2(	m_byteAmuxAddrY,		// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
										USB_TM_SCN_Write_08,	// Serial Comm operand high byte (8 bit write to SCN board)
										byteBitValues,			// Serial Comm operand low byte
										byteBitValues );		// Serial Comm operand high byte
}

void CPanelSL::OnWrMuxClear() 
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

	str.Format( "Written: 0x%X to\r\n0x%X and 0x%X", byteBitValues, m_byteAmuxAddrX, m_byteAmuxAddrY );
	GetDlgItem( IDC_EDIT_BoardDataTB )->SetWindowText( str );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x11 (8 bit write to Board 1), low byte = 0x24 (X gain/FB ctl.)
	// First write X values:
	CSemCtrl::Instance().USB_CommToBoards2(	m_byteAmuxAddrX, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );

	// Now write Y values:
	CSemCtrl::Instance().USB_CommToBoards2(	m_byteAmuxAddrY, USB_TM_SCN_Write_08, byteBitValues, byteBitValues );
}

void CPanelSL::IOexpSB_Change( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	switch( nPos )
	{
	case	0:
		str += " ( not used )";
		break;
	case	1:
		str += " ( X cros 1 )";
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
		str += " ( X Cros E/S Ctrl )";
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
		str += " ( Y Cros E/S Ctrl )";
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

void CPanelSL::OnIOXstbHi() 
{
	// TODO: Add your control notification handler code here
	BYTE bytePos = m_scrollIOexp.GetScrollPos();
	bytePos |= 0x80;
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip,	// Low byte = ckt addr. of SCN I/O Expander
										USB_TM_SCN_Write_08,		// Serial Comm operand high byte (&H11 = 8 bit write to SCN board)
										bytePos,	// OR in Strobe Hi bit, which will occur 125nS later
										bytePos );	// Hi byte data is actually low byte of 8 bit write
}

void CPanelSL::OnIOXstbLo() 
{
	// TODO: Add your control notification handler code here
	BYTE bytePos = m_scrollIOexp.GetScrollPos();
	bytePos |= 0x40;
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip,	// Low byte = ckt addr. of SCN I/O Expander
										USB_TM_SCN_Write_08,		// Serial Comm operand high byte (&H11 = 8 bit write to SCN board)
										bytePos,	// OR in Strobe Lo bit, which will occur 125nS later
										bytePos );	// Hi byte data is actually low byte of 8 bit write
}

void CPanelSL::OnSCNsoftReset() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Soft_Reset,	// Low byte = ckt addr. of Scan board soft reset
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

void CPanelSL::OnSCNtestLED4Cmd() 
{
	// TODO: Add your control notification handler code here
	m_bSCNtestLED4 = !m_bSCNtestLED4;
	if( m_bSCNtestLED4 )
	{
		m_btnSCNtestLED4.SetColor( g_crON_Test );
	}
	else
	{
		m_btnSCNtestLED4.SetColor( g_crOFF );
	}

	///////////////////////////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Test_LED,		// Low byte = ckt addr. of Test LED 16
										USB_TM_SCN_Write_08,		// Serial Comm operand high byte (8 bit write to SCN board)
										0x0,						// OP3 = 0 for 8 bit write
										(BYTE)m_bSCNtestLED4 );		// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CPanelSL::OnSCNreadBoardID() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ReadBoardID,
										USB_TM_SCN_Read_08,
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
//	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = str;
	strMsg += "\r\n";
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

void CPanelSL::OnTestW8() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	0xD0,						// Operand 1 = ckt addr. of Test LED 4
										0x14,						// Operand 2 = 0x14 (8 bit write to SPR board)???????
										0x0,						// Operand 3: Always set to 0!
										0x11 );						// Operand 4: ONLY valid data byte for 8 bit write
}

void CPanelSL::OnTestW16() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Test_LED,		// Operand 1 = ckt addr. of Test LED 4
										0x24,						// Operand 2 = 0x24 (16 bit write to SPR board)???????
										0x22,						// operand 3 = 2nd data byte of 2
										0x33 );						// operand 4 = 1st data byte of 2
}

void CPanelSL::OnTestW24() 
{
	// TODO: Add your control notification handler code here
    // Following set is ONLY necessary for 24 bit or 32 bit writes:
	CSemCtrl::Instance().USB_CommToBoards3(	0x12,						// operand 1 = ckt addr. of Test LED 4
										0x33,						// operand 2 = 0x34 (24 bit write to VAC board)???????
										0xDD,						// operand 3 = 2nd byte of 3
										0x0,						// operand 4 = 1st byte of 3
										0x0,						// operand 5: ALWAYS set to 0!
										0x66 );						// operand 6 = 3rd byte of 3
}

void CPanelSL::OnTestW32() 
{
	// TODO: Add your control notification handler code here
    // Following set is ONLY necessary for 24 bit or 32 bit writes:
	CSemCtrl::Instance().USB_CommToBoards3(	0xD0,						// operand 1 = ckt addr. of Test LED 4
										0x44,						// operand 2 = 0x44 (32 bit write to SPR board)???????
										0x22,						// operand 3 = 2nd data byte of 4
										0x33,						// operand 4 = 1st data byte of 4
										0x44,						// operand 5 = 4th data byte of 4
										0x66 );						// operand 6 = 3rd data byte of 4
}







void CPanelSL::OnL1CompDisable() 
{
	// Disable/Enable control for drift compensation of L1 circuit
	// Dim L1CompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: L1CompOn keeps track of current software state of control of this function. All bets are off if out of sync.
	int nCheck = m_btnL1CompDisable.GetCheck();
	if( nCheck )
	{
		m_btnL1CompDisable.SetColor( g_crON_Comm );	// light RED (to simulate Comp Fn is ON)
//		m_btnL1CompDisable.SetWindowText( "Comp is ON" );
	}
	else
	{
		m_btnL1CompDisable.SetColor( g_crOFF );		// gray (to simulate Comp Fn is OFF)
//		m_btnL1CompDisable.SetWindowText( "Comp is OFF" );
	}

	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x1A as low byte (L1 compensation disable ctl.)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_CondenserLens_LoopTest,	// Serial Comm operand low byte (L1 compensation disable ctl.)
										USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
										nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
										nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CPanelSL::OnL2CompDisable() 
{
	// Disable/Enable control for drift compensation of L2 circuit
	// Dim L2CompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: L2CompOn keeps track of current software state of control of this function. All bets are off if out of sync.
	int nCheck = m_btnL2CompDisable.GetCheck();
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
	CControlLayer::Instance().m_DL.log.nCheckComp[Logging_Comp_OL -5] = nCheck;

	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x2A as low byte (L2 compensation disable ctl.)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ObjectiveLens_LoopTest,	// Serial Comm operand low byte (L2 compensation disable ctl.)
										USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
										nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
										nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CPanelSL::OnXalCompDisable() 
{
	// Disable/Enable control for drift compensation of L2 circuit
	// Dim XalCompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: XalCompOn keeps track of current software state of control of this function. All bets are off if out of sync.
	int nCheck = m_btnXalCompDisable.GetCheck();
	if( nCheck )
	{
		m_btnXalCompDisable.SetColor( g_crON_Comm );// light RED (to simulate Comp Fn is ON)
//		m_btnXalCompDisable.SetWindowText( "Comp is ON" );
	}
	else
	{
		m_btnXalCompDisable.SetColor( g_crOFF );	// gray (to simulate Comp Fn is OFF)
//		m_btnXalCompDisable.SetWindowText( "Comp is OFF" );
	}
	CControlLayer::Instance().m_DL.log.nCheckComp[Logging_Comp_Align_X -5] = nCheck;

	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x4A as low byte (Xalign compensation disable ctl.)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_XAlignment_LoopTest,// Serial Comm operand low byte (Xalign compensation disable ctl.)
										USB_TM_LNS_Write_08,			// Serial Comm operand high byte (8 bit write to LNS board)
										nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
										nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CPanelSL::OnYalCompDisable() 
{
	// Disable/Enable control for drift compensation of Yalign circuit
	// Dim XalCompCktDis As Single 'Set to 0 = enable comp; Set to 1 to disable comp. (Toggle)
	// Uses global: YalCompOn keeps track of current software state of control of this function. All bets are off if out of sync.
	int nCheck = m_btnYalCompDisable.GetCheck();
	if( nCheck )
	{
		m_btnYalCompDisable.SetColor( g_crON_Comm );// light RED (to simulate Comp Fn is ON)
//		m_btnYalCompDisable.SetWindowText( "Comp is ON" );
	}
	else
	{
		m_btnYalCompDisable.SetColor( g_crOFF );	// gray (to simulate Comp Fn is OFF)
//		m_btnYalCompDisable.SetWindowText( "Comp is OFF" );
	}
	CControlLayer::Instance().m_DL.log.nCheckComp[Logging_Comp_Align_Y -5] = nCheck;

	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2), 0x5A as low byte (Yalign compensation disable ctl.)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_YAlignment_LoopTest,// Serial Comm operand low byte (Yalign compensation disable ctl.)
										USB_TM_LNS_Write_08,			// Serial Comm operand high byte (8 bit write to LNS board)
										nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
										nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CPanelSL::L1DACSB_Change( int nPos )
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
	str.Format( "%04X", nValue );
	GetDlgItem( IDC_EDIT_L1DAC )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_CL].nCurPos = nPos;

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
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_CondenserLens,	// Serial Comm operand low byte (Condenser Lens DAC circuit address)
										USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::L2SDACSB_Change( int nPos )
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
	str.Format( "%04X", nValue );
	GetDlgItem( IDC_EDIT_L2SDAC )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_OL].nCurPos = nPos;

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
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ObjectiveLens_Static,// Serial Comm operand low byte (Obj Lens Static DAC circuit address)
										USB_TM_LNS_Write_16,			// Serial Comm operand high byte (16 bit write to LNS board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::L2DDACSB_Change( int nPos )
{
}

void CPanelSL::XalDACSB_Change( int nPos )
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
	str.Format( "%04X", nValue );
	GetDlgItem( IDC_EDIT_XalDAC )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Align_X].nCurPos = nPos;

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
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_XAlignment,		// Serial Comm operand low byte (Obj Lens Static DAC circuit address)
										USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::YalDACSB_Change( int nPos )
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
	str.Format( "%04X", nValue );
	GetDlgItem( IDC_EDIT_YalDAC )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Align_Y].nCurPos = nPos;

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
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_YAlignment,	// Serial Comm operand low byte (Y alignment DAC circuit address)
										USB_TM_LNS_Write_16,	// Serial Comm operand high byte (16 bit write to LNS board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::WobDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;
	double dVolts;

	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos + 2048;
	str.Format( "%03X", nValue );
	GetDlgItem( IDC_EDIT_WobDAC )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Wob].nCurPos = nPos;

	dVolts = nValue / 4095.0 * 5 - 2.5;
	str.Format( "%.3f", dVolts );
	GetDlgItem( IDC_EDIT_WobDacV )->SetWindowText( str );

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_WobblerSetting,	// Serial Comm operand low byte (Y alignment DAC circuit address)
										USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CPanelSL::OnLNSsoftReset() 
{
	// Write via MCI to LNS board.  Instruction 1: write 0x12 as high byte (16 bit write to Board 2),
	// 0xEE as low byte (Low byte = ckt addr. of Lens board soft reset)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_Soft_Reset,	// Low byte = ckt addr. of Lens board soft reset
										USB_TM_LNS_Write_08,	// Serial Comm operand high byte (8 bit write to LNS board)
										0,			// Low byte is data-less
										0 );		// High byte is data-less
//	CSemCtrl::Instance().USB_CommToBoards(	USB_CID_LNS_Soft_Reset,	// Low byte = ckt addr. of Lens board soft reset
//										USB_TM_LNS_Write_16 );	// Serial Comm operand high byte (16 bit write to LNS board)

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
}

void CPanelSL::OnLNStestLED4() 
{
	// TODO: Add your control notification handler code here
	m_bLNStestLED4 = !m_bLNStestLED4;
	if( m_bLNStestLED4 )
	{
		m_btnLNStestLED4.SetColor( g_crON_Test );
	}
	else
	{
		m_btnLNStestLED4.SetColor( g_crOFF );
	}

	///////////////////////////////////////////////////////////////////////////
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_Test_LED,		// Low byte = ckt addr. of Test LED 16
										USB_TM_LNS_Write_08,		// Serial Comm operand high byte (8 bit write to SCN board)
										0x0,						// OP3 = 0 for 8 bit write
										(BYTE)m_bLNStestLED4 );		// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CPanelSL::OnLNSreadBoardID() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ReadBoardID,
										USB_TM_LNS_Read_08,
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
//	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = str;
	strMsg += "\r\n";
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

void CPanelSL::OnSetWobMaxCmd() 
{
	// Utilizes global: AutoWobblerMax, used by Auto Wobbler timer
	m_nAutoWobblerMax = m_scrollWobDAC.GetScrollPos();	// Use actual slider value, not variable value
}

void CPanelSL::OnSetWobMinCmd() 
{
	// Utilizes global: AutoWobblerMin, used by Auto Wobbler timer
	m_nAutoWobblerMin = m_scrollWobDAC.GetScrollPos();	// Use actual slider value, not variable value
}

void CPanelSL::OnSetAutoWobbleCmd() 
{
	// Utilizes global: AutoWobblerOn
	m_bAutoWobblerOn = !m_bAutoWobblerOn;
	if( m_bAutoWobblerOn )
	{
		m_btnAutoWob.SetColor( g_crON_Test );
		m_btnAutoWob.SetWindowText( "Auto Wobble @ 1 sec." );
		m_bAutoWobblerDir	= TRUE;
		m_nAutoWobblerLast	= m_nAutoWobblerMin;
		SetTimer( 1, 50, NULL );
	}
	else
	{
		m_btnAutoWob.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnAutoWob.SetWindowText( "Turn off Auto Wobble" );
		KillTimer( 1 );
		m_bAutoWobblerDir = FALSE;
	}
}

void CPanelSL::OnWobblerselOptionChanged(UINT nID)
{
	int nOption = nID - IDC_RADIO_Wobbler_None;

	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	nWsize		= 8;
	nModID		= USB_MID_LNS;

	// First turn off wobbler to ALL four functions, then turn on ONLY selected channel
	wData		= 0x0;
	bCktAddr	= USB_CID_LNS_CondenserLens_Wobbler;
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	bCktAddr	= USB_CID_LNS_ObjectiveLens_Wobbler;
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	bCktAddr	= USB_CID_LNS_XAlignment_Wobbler;
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	bCktAddr	= USB_CID_LNS_YAlignment_Wobbler;
	CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

	wData		= 0x1;
	switch( nOption )
	{
	case	0:
		break;
	case	1:
		bCktAddr	= USB_CID_LNS_CondenserLens_Wobbler;
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		break;
	case	2:
		bCktAddr	= USB_CID_LNS_ObjectiveLens_Wobbler;
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		break;
	case	3:
		bCktAddr	= USB_CID_LNS_XAlignment_Wobbler;
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		break;
	case	4:
		bCktAddr	= USB_CID_LNS_YAlignment_Wobbler;
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		break;
	}
}

void CPanelSL::OnAMPselOptionChanged(UINT nID)
{
	int nOption = nID - IDC_RADIO_Amp_Main;
//	if( nOption == m_nOldAMPselOption )
//		return;
	
	if( nOption == 0 )
	{
		// Main Amp: 24,31,17,1 off; 20,21,22,27,28,29 on
		BYTE bytePos = 0x40 | 0x18;	// 24
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x1F;		// 31
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x11;		// 17
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x40 | 0x01;		// 1
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x80 | 0x14;		// 20
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x80 | 0x15;		// 21
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x16;		// 22
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x1B;		// 27
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
			bytePos, bytePos );
		bytePos = 0x80 | 0x1C;		// 28
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x1D;		// 29
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
	}
	else if( nOption == 1 )
	{
		// Low Amp 1: 20,21,22,27,28,29 off; 24,31,17,1 on
		BYTE bytePos = 0x40 | 0x14;	// 20
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x15;		// 21
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x16;		// 22
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x1B;		// 27
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x1C;		// 28
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x18;		// 24
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x1F;		// 31
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x11;		// 17
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x01;		// 1
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
	}
	else
	{
		// Low Amp: 20,21,22,27,28,29 off; 17,1 off; 24,31 on
		BYTE bytePos = 0x40 | 0x14;	// 20
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x15;		// 21
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x16;		// 22
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x1B;		// 27
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x1C;		// 28
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x11;		// 17
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x40 | 0x01;		// 1
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x18;		// 24
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
		bytePos = 0x80 | 0x1F;		// 31
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08,
											bytePos, bytePos );
	}
	m_nOldAMPselOption = nOption;
}

void CPanelSL::OnCHECKStigCtrl() 
{
	// Stig Ctrl switch
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Stig_Ctrl)))->GetCheck();
	if( nCheck )
	{
		BYTE bytePos = 0x80 | 0x13;	// 19
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x80 | 0x1A;		// 26
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x80 | 0x17;		// 23
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x80 | 0x1E;		// 30
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x12;		// 18
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x19;		// 25
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	}
	else
	{
		BYTE bytePos = 0x40 | 0x13;	// 19
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x1A;		// 26
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x17;		// 23
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x1E;		// 30
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x12;		// 18
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x19;		// 25
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	}
}

void CPanelSL::OnSCNExpChipClearCurrent() 
{
	// TODO: Add your control notification handler code here
	if( m_nOldAMPselOption == 0 )
	{
		// Main Amp: 20,21,22,27,28,29 on, clear them
		BYTE bytePos = 0x40 | 0x14;	// 20
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x15;		// 21
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x16;		// 22
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x1B;		// 27
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x1C;		// 28
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	}
	else if( m_nOldAMPselOption == 1 )
	{
		// Low Amp: 24,31,17,1 on, clear them
		BYTE bytePos = 0x40 | 0x18;	// 24
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x1F;		// 31
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x11;		// 17
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x01;		// 1
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	}
	else
	{
		// Low Amp: 24,31,17,1 on, clear them
		BYTE bytePos = 0x40 | 0x18;	// 24
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		bytePos = 0x40 | 0x1F;		// 31
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	}
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Main )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Low1 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Low )))->SetCheck(0);
	m_nOldAMPselOption = -1;
}

void CPanelSL::OnSCNExpChipClearAll() 
{
	// Main Amp: 20,21,22,27,28,29
	BYTE bytePos = 0x40 | 0x14;	// 20
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x15;		// 21
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x16;		// 22
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1B;		// 27
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1C;		// 28
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1D;		// 29
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	// Low Amp: 24,31,17,1
	bytePos = 0x40 | 0x18;		// 24
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1F;		// 31
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x11;		// 17
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x01;		// 1
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	// Stig Ctrl switch
	bytePos = 0x40 | 0x13;		// 19
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1A;		// 26
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	// ShortRly
	bytePos = 0x40 | 0x12;		// 18
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x19;		// 25
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	// Shift E/S
	bytePos = 0x40 | 0x17;		// 23
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1E;		// 30
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Main )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Low1 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Amp_Low )))->SetCheck(0);
	m_nOldAMPselOption = -1;
	((CButton*)(GetDlgItem(IDC_CHECK_Stig_Ctrl)))->SetCheck(0);
}

void CPanelSL::OnADCselOptionChanged(UINT nID)
{
	CString str;
	int nOption = nID - IDC_RADIO_ADCselOption0;
	switch( nOption )
	{
	case	0:		// Condenser Lens current monitor
		str = "80";	// ADC Channel 0, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	case	1:		// Objective Lens current monitor
		str = "C0";	// ADC Channel 1, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	case	2:		// X alignment coil current monitor
		str = "90";	// ADC Channel 2, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	case	3:		// Y alignment coil current monitor
		str = "D0";	// ADC Channel 3, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	case	4:		// Amplifier positive power supplies voltages monitor
		str = "A0";	// ADC Channel 4, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	case	5:		// Amplifier positive power supplies voltages monitor
		str = "E0";	// ADC Channel 5, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	case	6:		// Thermistor input 1
		str = "B0";	// ADC Channel 6, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	case	7:		// Thermistor input 2
		str = "F0";	// ADC Channel 7, S.E., Uni/Gain = 00, NAP/Sleep = 00
		break;
	}
	GetDlgItem( IDC_EDIT_ADCchanSel )->SetWindowText( str );
}

int CPanelSL::GetADCselOption()
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

void CPanelSL::OnReadADC() 
{
	// Write via MCI to LNS board.  Instruction 1: Select ADC on LNS board
	// Write via MCI to LNS board.  Instruction 2: Write channel select to ADC
	int nADCchanSel = GetADCselOption();
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ADC_Write,					// LNS board circuit address for LNS board ADC = C2
										USB_TM_LNS_Write_16,					// Serial Comm operand high byte (16 bit write to LNS board)
										nADCchanSel,			// Serial Comm operand low byte (this one is ignored by ADC)
										nADCchanSel );			// Serial Comm operand high byte (this one matters)

	// Write via MCI to LNS board.  Instruction 3: Select ADC Start Conversion on LNS board (8 bit write)
	// Write via MCI to LNS board.  Instruction 4: Start conversion is data-less, but must complete comm cycle.
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ADC_HD_Start_Conversion,	// Circuit address for LNS board Start ADC conversion = C4
										USB_TM_LNS_Write_08,					// Serial Comm operand high byte (8 bit write to LNS board)
										0x00,					// data-less write
										0x00 );					// data-less write

	// Write via MCI to LNS board.  Instruction 5: Select ADC Read (ckt address C8, 16 bit read)
	// Write via MCI to LNS board.  Instruction 6: Read of ADC is data-less MCI operand, but must complete comm cycle.
	CSemCtrl::Instance().USB_CommToBoards2(	0xC6,					// Circuit address for LNS board Start ADC conversion = C6
										USB_TM_LNS_Read_16,	// Serial Comm operand high byte (0xA2 = 16 bit read from LNS board)
										0x00,					// data-less write
										0x00 );					// data-less write

	// HERE IS WHERE WE WILL NEED TO GRAB THE DATA THAT COMES BACK FROM ADC, AND SEND OVER USB TO HOST
}

void CPanelSL::OnReadSelectedADC() 
{
	// TODO: Add your control notification handler code here
	
}

void CPanelSL::OnCHECKReadLNSadcAuto() 
{
	// TODO: Add your control notification handler code here
	
}










void CPanelSL::InitDiagnose( BOOL bEnd )
{
	KillTimer( 1 );
	KillTimer( 2 );

/*	for( int i=0; i<16; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_ADCsChSelect0 +i )))->SetCheck( 0 );
	for( i=0; i<2; i++ )
		((CButton*)(GetDlgItem( IDC_RADIO_ADCselectOB0 +i )))->SetCheck( 0 );

	GetDlgItem( IDC_BUTTON_ReadSelectedADCcmd )->EnableWindow( bEnd );
	GetDlgItem( IDC_CHECK_ReadLNSadcAuto )->EnableWindow( bEnd );*/
}

void CPanelSL::SetTimers( UINT nIDEvent, BOOL bSet )
{
	if( bSet )
	{
		switch( nIDEvent )
		{
		case	Diag_Stig_X:
			m_scrollXstigDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			XstigDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_Stig_Y:
			m_scrollYstigDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			YstigDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_Shift_X:
			m_scrollXshiftDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			XshiftDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_Shift_Y:
			m_scrollYshiftDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			YshiftDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_xover_X:
			m_scrollXxoverDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			XxoverDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_xover_Y:
			m_scrollYxoverDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			YxoverDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -2048 );
			break;
		case	Diag_CL:
			m_scrollL1DAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			L1DACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			break;
		case	Diag_OL:
			m_scrollL2SDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			L2SDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			break;
		case	Diag_Align_X:
			m_scrollXalDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			XalDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			break;
		case	Diag_Align_Y:
			m_scrollYalDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			YalDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin -32768 );
			break;
		case	Diag_Wob:
			m_scrollWobDAC.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin  -2048);
			WobDACSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin  -2048);
			break;
		default:
			break;
		}
//		SetTimer( nIDEvent +100, CControlLayer::Instance().m_DL.diag[nIDEvent].nIntv, NULL );
	}
	else
		KillTimer( nIDEvent +100 );
}

void CPanelSL::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
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
	else if( nIDEvent == 2 )
	{
		// 读取ADC返回值
		CBoardComm::Instance().ReadSelectedADC( FALSE );
		theApp.Delay(80);//Sleep(80);
		CBoardComm::Instance().ReadSelectedADC( TRUE );
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	else if( nIDEvent == Diag_Stig_X +100 )
	{
		int nPos = m_scrollXstigDAC.GetScrollPos();
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollXstigDAC.SetScrollPos( nPos );
			XstigDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollYstigDAC.SetScrollPos( nPos );
			YstigDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollXshiftDAC.SetScrollPos( nPos );
			XshiftDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollYshiftDAC.SetScrollPos( nPos );
			YshiftDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollXxoverDAC.SetScrollPos( nPos );
			XxoverDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollYxoverDAC.SetScrollPos( nPos );
			YxoverDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -32768;
			m_scrollL1DAC.SetScrollPos( nPos );
			L1DACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -32768;
			m_scrollL2SDAC.SetScrollPos( nPos );
			L2SDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -32768;
			m_scrollXalDAC.SetScrollPos( nPos );
			XalDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -32768 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -32768;
			m_scrollYalDAC.SetScrollPos( nPos );
			YalDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
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
		nPos += CControlLayer::Instance().m_DL.diag[nIDEvent-100].nStep;
		if( nPos > CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048 )
		{
			nPos = CControlLayer::Instance().m_DL.diag[nIDEvent-100].nMax -2048;
			m_scrollWobDAC.SetScrollPos( nPos );
			WobDACSB_Change( nPos );
			KillTimer( nIDEvent );
			CControlLayer::Instance().m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollWobDAC.SetScrollPos( nPos );
			WobDACSB_Change( nPos );
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CPanelSL::OnWriteRateX() 
{
	// TODO: Add your control notification handler code here
	// 15.06.19 使用原来闲置的xover功能来调整最低放大倍数档的偏心问题
	int nPos = m_scrollXxoverDAC.GetScrollPos();
	CString str;
	str.Format( "%d", nPos );

	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	WritePrivateProfileString( "Calibrate_Offset", "LowAmpX", str, path );
}

void CPanelSL::OnWriteRateY() 
{
	// TODO: Add your control notification handler code here
	// 15.06.19 使用原来闲置的xover功能来调整最低放大倍数档的偏心问题
	int nPos = m_scrollYxoverDAC.GetScrollPos();
	CString str;
	str.Format( "%d", nPos );

	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	WritePrivateProfileString( "Calibrate_Offset", "LowAmpY", str, path );
}
