// PageMCI.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "PageMCI.h"
#include "TabDialog.h"
#include "Insample0Dlg.h"

#include <afxtempl.h>		// MFC support for Template
#include <Mmsystem.h>
#include <Math.h>

#define READ_CLOCK 0.0
#define WRITE_CLOCK 0.0
#define NUM_SCANS 10
//Each scan is [NUM_READINGS] transfers
#define NUM_READINGS 2048
#define	numreadings NUM_READINGS+512

unsigned short readings2D[NUM_SCANS][numreadings];
unsigned long readingsCounts[NUM_SCANS];
unsigned long bytes_transferred, total_readings;
unsigned long status, Calls;


CListBox * m_g_ReportList;
CPageMCI* m_g_Dialog;
unsigned int readings[NUM_READINGS];

DWORD	g_TickCount;
DWORD	g_thisTickCount, g_lastTickCount;

UINT __cdecl threadfunc (LPVOID pParam);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageMCI dialog


CPageMCI::CPageMCI(CWnd* pParent /*=NULL*/)
	: CDialog(CPageMCI::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageMCI)
	m_strPixelRateHB = _T("00");
	m_strPixelRateLB = _T("08");
	m_strSegNum = _T("2B");
	m_strSegHB = _T("00");
	m_strSegLB = _T("00");
	m_strLineHB = _T("00");
	m_strLineLB = _T("00");
	m_nReso = -1;
	m_nPixClk = -1;
	m_nDetGainNo2 = 0;
	m_nDetPair2 = 0;
	m_nDispType = -1;
	m_nBoardID = 2;
	m_nPwrChn = 2;
	//}}AFX_DATA_INIT

	m_Speed			= USB_SS_500ns;
	m_bVideoMode	= FALSE;
	m_bDetMode		= FALSE;
	m_bDispMode		= TRUE;
	m_bRotate		= FALSE;

	m_nDet03Chn2	= 0;
	m_nDet45Chn2	= 2;

	m_pBuffUSB		= NULL;
}

CPageMCI::~CPageMCI()
{
	if ( m_pBuffUSB != NULL )
	{
		delete [] m_pBuffUSB;
		m_pBuffUSB	= NULL;
	}
}


void CPageMCI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMCI)
	DDX_Control(pDX, IDC_BUTTON_cmdSpotMode, m_btnModeSpot);
	DDX_Control(pDX, IDC_BUTTON_cmdLineMode_Vertical, m_btnModeLineVertical);
	DDX_Control(pDX, IDC_BUTTON_cmdLineMode_Horizontal, m_btnModeLineHorizontal);
	DDX_Control(pDX, IDC_BUTTON_cmdNormalMode, m_btnModeNormal);
	DDX_Control(pDX, IDC_CHECK_Video, m_btnVideo);
	DDX_Control(pDX, IDC_CHECK_MainsLock, m_btnMainsLock);
	DDX_Control(pDX, IDC_RADIO_Display_Serial, m_btnDispSerial);
	DDX_Control(pDX, IDC_RADIO_Display_Delay, m_btnDispDelay);
	DDX_Control(pDX, IDC_RADIO_Resolution1, m_radioResolution1);
	DDX_Control(pDX, IDC_RADIO_Resolution2, m_radioResolution2);
	DDX_Control(pDX, IDC_RADIO_Resolution3, m_radioResolution3);
	DDX_Control(pDX, IDC_RADIO_Resolution4, m_radioResolution4);
	DDX_Control(pDX, IDC_RADIO_Resolution5, m_radioResolution5);
	DDX_Control(pDX, IDC_RADIO_Resolution6, m_radioResolution6);
	DDX_Control(pDX, IDC_RADIO_Resolution7, m_radioResolution7);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_1, m_radioPixClkEq1);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_2, m_radioPixClkEq2);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_3, m_radioPixClkEq3);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_4, m_radioPixClkEq4);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_5, m_radioPixClkEq5);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_6, m_radioPixClkEq6);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_7, m_radioPixClkEq7);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_8, m_radioPixClkEq8);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_9, m_radioPixClkEq9);
	DDX_Control(pDX, IDC_RADIO_PixClkEq_10, m_radioPixClkEq10);
	DDX_Control(pDX, IDC_BUTTON_cmdRotation, m_btnRotate);
	DDX_Control(pDX, IDC_SCROLLBAR_BW_sel, m_scrollBWsel);
	DDX_Control(pDX, IDC_SCROLLBAR_DetGain, m_scrollDetGain);
	DDX_Control(pDX, IDC_SCROLLBAR_Det45, m_scrollDet45);
	DDX_Control(pDX, IDC_SCROLLBAR_Det03, m_scrollDet03);
	DDX_Control(pDX, IDC_RADIO_DetSingle, m_btnDetSingle);
	DDX_Control(pDX, IDC_RADIO_DetDuel, m_btnDetDual);
	DDX_Control(pDX, IDC_RADIO_acqRaster, m_btnVideoRaster);
	DDX_Control(pDX, IDC_RADIO_acqXYZ, m_btnVideoXYZ);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_BUTTON_Receive, m_btnReceive);
	DDX_Control(pDX, IDC_BUTTON_Demo_Image, m_btnDemoImage);
	DDX_Control(pDX, IDC_BUTTON_DefaultRamps, m_btnDefaultRamp);
	DDX_Control(pDX, IDC_BUTTON_UpdateY, m_btnUpdateY);
	DDX_Control(pDX, IDC_BUTTON_UpdateX, m_btnUpdateX);
	DDX_Control(pDX, IDC_BUTTON_Scan_Stop, m_btnScanStop);
	DDX_Control(pDX, IDC_BUTTON_Scan_Start, m_btnScanStart);
	DDX_Text(pDX, IDC_EDIT_PixelRate_HB, m_strPixelRateHB);
	DDX_Text(pDX, IDC_EDIT_PixelRate_LB, m_strPixelRateLB);
	DDX_Text(pDX, IDC_EDIT_SegNum, m_strSegNum);
	DDX_Text(pDX, IDC_EDIT_valHB, m_strSegHB);
	DDX_Text(pDX, IDC_EDIT_valLB, m_strSegLB);
	DDX_Text(pDX, IDC_EDIT_lineHB, m_strLineHB);
	DDX_Text(pDX, IDC_EDIT_lineLB, m_strLineLB);
	DDX_Radio(pDX, IDC_RADIO_Resolution1, m_nReso);
	DDX_Radio(pDX, IDC_RADIO_PixClkEq_1, m_nPixClk);
	DDX_Text(pDX, IDC_EDIT_DetGain_no2, m_nDetGainNo2);
	DDV_MinMaxUInt(pDX, m_nDetGainNo2, 0, 5);
	DDX_Text(pDX, IDC_EDIT_DetPair2, m_nDetPair2);
	DDV_MinMaxUInt(pDX, m_nDetPair2, 0, 2);
	DDX_Radio(pDX, IDC_RADIO_Display_Serial, m_nDispType);
	DDX_Radio(pDX, IDC_RADIO_Board_SCN, m_nBoardID);
	DDX_Text(pDX, IDC_EDIT_Pwr_Chn, m_nPwrChn);
	DDV_MinMaxUInt(pDX, m_nPwrChn, 0, 7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageMCI, CDialog)
	//{{AFX_MSG_MAP(CPageMCI)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_Read_PortA, OnReadPortA)
	ON_BN_CLICKED(IDC_BUTTON_Write_PortA, OnWritePortA)
	ON_BN_CLICKED(IDC_BUTTON_Read_PortB, OnReadPortB)
	ON_BN_CLICKED(IDC_BUTTON_Write_PortB, OnWritePortB)
	ON_BN_CLICKED(IDC_BUTTON_Read_PortC, OnReadPortC)
	ON_BN_CLICKED(IDC_BUTTON_Write_PortC, OnWritePortC)
	ON_BN_CLICKED(IDC_BUTTON_Read_PortD, OnReadPortD)
	ON_BN_CLICKED(IDC_BUTTON_Write_PortD, OnWritePortD)
	ON_BN_CLICKED(IDC_BUTTON_ReadAll, OnReadAll)
	ON_BN_CLICKED(IDC_BUTTON_WriteAll, OnWriteAll)
	ON_BN_CLICKED(IDC_BUTTON_Set_Pixel_Rate, OnSetPixelRate)
	ON_BN_CLICKED(IDC_BUTTON_PixClkEq250KHz, OnSetPixClkEq250KHz)
	ON_BN_CLICKED(IDC_BUTTON_PixClkEq1MHz, OnSetPixClkEq1MHz)
	ON_BN_CLICKED(IDC_BUTTON_XrampSeg, OnSetXrampSeg)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Start, OnScanStart)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Stop, OnScanStop)
	ON_BN_CLICKED(IDC_BUTTON_Receive, OnReceive)
	ON_BN_CLICKED(IDC_CHECK_InHexCheck, OnInHexCheck)
	ON_BN_CLICKED(IDC_BUTTON_UpdateX, OnUpdateX)
	ON_BN_CLICKED(IDC_BUTTON_UpdateY, OnUpdateY)
	ON_BN_CLICKED(IDC_BUTTON_DefaultRamps, OnUpdateDefaultRamps)
	ON_BN_CLICKED(IDC_BUTTON_RampSeg, OnSetRampSegment)
	ON_BN_CLICKED(IDC_BUTTON_Det03_Load, OnDet03Load)
	ON_BN_CLICKED(IDC_BUTTON_Det45_Load, OnDet45Load)
	ON_BN_CLICKED(IDC_BUTTON_BoardComm, OnBoardComm)
	ON_BN_CLICKED(IDC_CHECK_StartImage, OnStartImage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Demo_Image, OnDemoImage)
	ON_BN_CLICKED(IDC_BUTTON_cmdNormalMode, OnScanModeNormal)
	ON_BN_CLICKED(IDC_BUTTON_cmdLineMode_Horizontal, OnScanModeLineHorizontal)
	ON_BN_CLICKED(IDC_BUTTON_cmdLineMode_Vertical, OnScanModeLineVertical)
	ON_BN_CLICKED(IDC_BUTTON_cmdSpotMode, OnScanModeSpot)
	ON_BN_CLICKED(IDC_BUTTON_SaveListResult, OnSaveListResult)
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Det03, OnDeltaposDet03)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Det45, OnDeltaposDet45)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DetGain, OnDeltaposDetGain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DetPair, OnDeltaposDetPair)
	ON_BN_CLICKED(IDC_BUTTON_cmdRotation, OnScanRotation)
	ON_BN_CLICKED(IDC_BUTTON_cmdRotation_Update, OnScanRotationUpdate)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_Read_Version, OnReadVersion)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Start_With_Params, OnScanStartWithParams)
	ON_BN_CLICKED(IDC_BUTTON_Clear_DB, OnINT_ClearDB)
	ON_BN_CLICKED(IDC_BUTTON_Set_IMR, OnINT_SetIMR)
	ON_BN_CLICKED(IDC_BUTTON_INT_test, OnINT_INTtest)
	ON_BN_CLICKED(IDC_CHECK_MainsLock, OnCHECKMainsLock)
	ON_BN_CLICKED(IDC_BUTTON_Fake_Mainslock, OnFakeMainslock)
	ON_BN_CLICKED(IDC_CHECK_Video, OnCHECKVideo)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreeze, OnFreeze)
	ON_BN_CLICKED(IDC_BUTTON_Reset_Communicate, OnResetCommunicate)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreezeX, OnFreezeX)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreezeY, OnFreezeY)
	ON_BN_CLICKED(IDC_BUTTON_Read_INT_Source_Register, OnReadINTSourceRegister)
	ON_BN_CLICKED(IDC_BUTTON_Write_INT_Enable_Register, OnWriteINTEnableRegister)
	ON_BN_CLICKED(IDC_BUTTON_DetPair_Select, OnDetPairSelect)
	ON_BN_CLICKED(IDC_BUTTON_ReadMCIPower, OnReadMCIPower)
	ON_BN_CLICKED(IDC_BUTTON_ReadMCIPower_All, OnReadMCIPowerAll)
	ON_BN_CLICKED(IDC_BUTTON_FT232Test_Send, OnBUTTONFT232TestSend)
	ON_BN_CLICKED(IDC_BUTTON_FT232Test_Stop, OnBUTTONFT232TestStop)
	ON_BN_CLICKED(IDC_BUTTON_FT232Test_Recv, OnBUTTONFT232TestRecv)
	ON_BN_CLICKED(IDC_CHECK_FT232Test_Loop, OnCHECKFT232TestLoop)
	ON_BN_CLICKED(IDC_BUTTON_SendInOneTime, OnBUTTONSendInOneTime)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Pwr_Chn, OnDeltaposSPINPwrChn)
	ON_BN_CLICKED(IDC_BUTTON_ReadBoardIDs, OnReadBoardIDs)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_Resolution1, IDC_RADIO_Resolution7, OnResolutionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_PixClkEq_1, IDC_RADIO_PixClkEq_10, OnPixClkChanged)
	ON_COMMAND_RANGE(IDC_RADIO_Display_Serial, IDC_RADIO_Display_Delay, OnDisplayModeChanged)
	ON_COMMAND_RANGE(IDC_RADIO_acqXYZ, IDC_RADIO_acqRaster, OnVideoModeChanged)
	ON_COMMAND_RANGE(IDC_RADIO_DetSingle, IDC_RADIO_DetDuel, OnDetectorModeChanged)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, memberFxn )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMCI message handlers

//在线提示
BOOL CPageMCI::memberFxn( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{

    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            pTTT->lpszText = MAKEINTRESOURCE(nID);
            pTTT->hinst = AfxGetResourceHandle();
            return(TRUE);
        }
    }
	return FALSE;
}

BOOL CPageMCI::OnInitDialog()
{
	CDialog::OnInitDialog();
    EnableToolTips(TRUE);		//允许在线提示

	// TODO: Add extra initialization here
	///////////////////////////////////////////////////////////////////////////

	CString str;
	str = "x21 - Xoffset,	x28 - Yoffset";
	str += "\r\n";
	str += "x22 - Xvalley,	x29 - Yvalley";
	str += "\r\n";
	str += "x23 - Xrise,	x2A - Yrise";
	str += "\r\n";
	str += "x24 - dXrise,	x2B - dYrise";
	str += "\r\n";
	str += "x25 - Xpeak,	x2C - Ypeak";
	str += "\r\n";
	str += "x26 - Xfall,	x2D - Yfall";
	str += "\r\n";
	str += "x27 - dXfall,	x2E - dYfall";
	str += "\r\n";
	( GetDlgItem( IDC_STATIC_txtSegNums ) )->SetWindowText( str );

	///////////////////////////////////////////////////////////////////////////

	m_scrollDet03.SetScrollRange( 0, 4095 );	// 0 --- 0xFFF
	m_scrollDet45.SetScrollRange( 0, 4095 );	// 0 --- 0xFFF
	m_scrollDetGain.SetScrollRange( 0, 7 );
    m_scrollDet03.SetScrollPos( 2048 );
    m_scrollDet45.SetScrollPos( 2048 );
    m_scrollDetGain.SetScrollPos( 5 );
	m_scrollBWsel.SetScrollRange( 0, 7 );

	m_btnDemoImage.SetColor( g_crImage );
	m_btnReceive.SetColor( g_crImage );
	m_btn1.SetColor( g_crImage );

	m_btnScanStart.SetColor( g_crScanEmphasis );
	m_btnScanStop.SetColor( g_crScanEmphasis );
	m_btnUpdateX.SetColor( g_crScanEmphasis );
	m_btnUpdateY.SetColor( g_crScanEmphasis );
	( GetDlgItem( IDC_EDIT_RecLen ) )->SetWindowText( "256" );
	( GetDlgItem( IDC_EDIT_RecTimes ) )->SetWindowText( "2" );
	( GetDlgItem( IDC_EDIT_RecCounts ) )->SetWindowText( "1024" );
	GetDlgItem( IDC_BUTTON1 )->SetWindowText( "collect 2x 1.00k data" );

	m_btnVideoXYZ.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnVideoRaster.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnDetSingle.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnDetDual.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnRotate.SetColor( ::GetSysColor(COLOR_3DFACE) );
	GetDlgItem( IDC_EDIT_Rdegree )->SetWindowText( "0" );
	m_btnDispSerial.SetColor( g_crEmphasis2 );
	m_btnDispDelay.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeNormal.SetColor( g_crEmphasis1 );
	m_btnModeLineHorizontal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeSpot.SetColor( ::GetSysColor(COLOR_3DFACE) );
	GetDlgItem( IDC_EDIT_spotX_HB )->SetWindowText( "00" );
	GetDlgItem( IDC_EDIT_spotX_LB )->SetWindowText( "00" );
	GetDlgItem( IDC_EDIT_spotY_HB )->SetWindowText( "00" );
	GetDlgItem( IDC_EDIT_spotY_LB )->SetWindowText( "00" );

	( GetDlgItem( IDC_EDIT_Interrupt_Mask_Register ) )->SetWindowText( "02" );
	m_btnMainsLock.SetColor( ::GetSysColor(COLOR_3DFACE) );
	str.LoadString( IDS_MCI_Mainslock_ON );
	m_btnMainsLock.SetWindowText( str );
	m_btnVideo.SetColor( g_crEmphasis2 );
	str.LoadString( IDS_MCI_Video_ON );
	m_btnVideo.SetWindowText( str );
	m_btnVideo.SetCheck( 1 );

	if (!m_dlgDisplay.Create( CDlgDisplay::IDD, this ))
	{
		TRACE0("Failed to create CDlgDisplay dlg\n");
	}
	m_dlgDisplay.ShowWindow(SW_HIDE);

	GetDlgItem( IDC_EDIT_FT232Test_Intv )->SetWindowText( "10" );
	GetDlgItem( IDC_EDIT_Board_Opr_LB )->SetWindowText("100");

	for( int i=0; i<8; i++ )
		m_dPwrReadback[i] = 0.0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPageMCI::Init( BOOL bReady )
{
	if( !bReady )
		return;

	// set mainslock off
	theApp.WriteToBoard( USB_MainsLock_OFF, FALSE, 0 );
	//	theApp.WriteToBoard( USB_INT_test, FALSE, 0 );
	// set video on
	theApp.WriteToBoard( USB_Video_ON, FALSE, 0 );
	// set detector pair
	theApp.WriteToBoard( USB_DetPairSelection, TRUE, 0x00 );
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->SetCheck(1);

	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
	m_g_ReportList	= &(pParent->m_listReport);

	// 4个通道全部设中间值
	m_nDet03Chn2 = 0;
	Det03SB_Change( 2048 );
	m_nDet03Chn2 = 1;
	Det03SB_Change( 2048 );
	m_nDet03Chn2 = 2;
	Det03SB_Change( 2048 );
	m_nDet03Chn2 = 3;
	Det03SB_Change( 2048 );
	m_nDet03Chn2 = 0;
	GetDlgItem( IDC_EDIT_Det03_Chn2 )->SetWindowText( "0" );

	// 4个通道全部设中间值
	m_nDet45Chn2 = 0;
	Det45SB_Change( 2048 );
	m_nDet45Chn2 = 1;
	Det45SB_Change( 2048 );
	m_nDet45Chn2 = 2;
	Det45SB_Change( 2048 );
	m_nDet45Chn2 = 3;
	Det45SB_Change( 2048 );
	m_nDet45Chn2 = 2;
	GetDlgItem( IDC_EDIT_Det45_Chn2 )->SetWindowText( "2" );

	// 第4通道的Gain
    DetGainSB_Change( 5 );
	// 滤波器
	BWselSB_Change( 0 );
}

void CPageMCI::OnOK() 
{
}

void CPageMCI::OnCancel() 
{
}

BOOL CPageMCI::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_Det03:
		case	IDC_EDIT_Det45:
		case	IDC_EDIT_DetGain:
		case	IDC_EDIT_BW_sel2:
		case	IDC_EDIT_Det03_Chn2:
		case	IDC_EDIT_Det45_Chn2:
		case	IDC_EDIT_DetGain_no2:
		case	IDC_EDIT_DetPair2:
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
		case	IDC_EDIT_Det03:
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
			break;
		case	IDC_EDIT_Det45:
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
			break;
		case	IDC_EDIT_DetGain:
			m_scrollDetGain.SetScrollPos( nPos );
			DetGainSB_Change( nPos );
			break;
		case	IDC_EDIT_BW_sel2:
			m_scrollBWsel.SetScrollPos( nPos );
			BWselSB_Change( nPos );
			break;
		case	IDC_EDIT_Det03_Chn2:
			m_nDet03Chn2 = nPos;
			if(m_nDet03Chn2 < 0)
				m_nDet03Chn2 = 0;
			else if( m_nDet03Chn2 > 3 )
				m_nDet03Chn2 = 3;
			UpdateData(FALSE);
			break;
		case	IDC_EDIT_Det45_Chn2:
			m_nDet45Chn2 = nPos;
			if(m_nDet45Chn2 < 0)
				m_nDet45Chn2 = 0;
			else if( m_nDet45Chn2 > 3 )
				m_nDet45Chn2 = 3;
			UpdateData(FALSE);
			break;
		case	IDC_EDIT_DetGain_no2:
			m_nDetGainNo2 = nPos;
			if(m_nDetGainNo2 < 0)
				m_nDetGainNo2 = 0;
			else if( m_nDetGainNo2 > 5 )
				m_nDetGainNo2 = 5;
			UpdateData(FALSE);
			break;
		case	IDC_EDIT_DetPair2:
			m_nDetPair2 = nPos;
			if(m_nDetPair2 < 0)
				m_nDetPair2 = 0;
			else if( m_nDetPair2 > 2 )
				m_nDetPair2 = 2;
			UpdateData(FALSE);
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPageMCI::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CPageMCI::OnSaveListResult() 
{
	// TODO: Add your control notification handler code here
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;

	unsigned long cbRet;
	char Buf[255];
	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
	str.Format( "%szListResult%s.log", appfilepath, strTemp );
	HANDLE hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

/*	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
	int nCounts = pParent->m_listReport.GetCount();
*/	int nCounts = m_g_ReportList->GetCount();
	int nLength;
	for( int i=0; i<nCounts; i++ )
	{
//		nLength = pParent->m_listReport.GetTextLen( i );
//		pParent->m_listReport.GetText( i, strTemp.GetBuffer(nLength) );
		nLength = m_g_ReportList->GetTextLen( i );
		m_g_ReportList->GetText( i, strTemp.GetBuffer(nLength) );
		if( (strTemp.Left(7) == "Writing")
			|| ( strTemp.Left(3) == "Got" ) )
			sprintf(Buf, "%s\r\n", strTemp );
		else
			sprintf(Buf, "%s", strTemp );
		WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
	}
	CloseHandle(hFil);
}

void CPageMCI::OnReadPortA() 
{
}

void CPageMCI::OnWritePortA() 
{
}

void CPageMCI::OnReadPortB() 
{
}

void CPageMCI::OnWritePortB() 
{
}

void CPageMCI::OnReadPortC() 
{
}

void CPageMCI::OnWritePortC() 
{
}

void CPageMCI::OnReadPortD() 
{
}

void CPageMCI::OnWritePortD() 
{
}

void CPageMCI::OnReadAll() 
{
	// TODO: Add your control notification handler code here
	theApp.WriteToBoard( USB_ReadVersion, FALSE, 0 );
	ReadBackdata();
}

void CPageMCI::OnWriteAll() 
{
}

void CPageMCI::OnReadVersion() 
{
	// send version and check returns of port B,D
//	theApp.WriteToBoard( USB_ReadVersion, FALSE, 0 );
//	ReadBackdata();
	unsigned char input[4];
	DIO_ReadAll(theApp.deviceIndex, &input);
	CString str;
	str.Format( "D = 0x%02X, B = 0x%02X", input[3], input[1] );

	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
	//	pParent->m_listReadback.ResetContent();
	pParent->UpdateListReadbackString( str );
}

void CPageMCI::OnReadMCIPower() 
{
	// TODO: Add your control notification handler code here
	WORD wData = 0xF0 | (BYTE)m_nPwrChn;
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, wData );
	ReadBackdata();
}

void CPageMCI::OnReadMCIPowerAll() 
{
	// TODO: Add your control notification handler code here
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, 0xF2 );
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, 0xF6 );
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, 0xF7 );
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, 0xF5 );
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, 0xF1 );
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, 0xF0 );
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, 0xF3 );
	theApp.WriteToBoard( USB_Read_Pwr, TRUE, 0xF4 );

	ReadBackdata();
}

void CPageMCI::OnReadBoardIDs() 
{
	// TODO: Add your control notification handler code here
	theApp.CommToBoards2( USB_CID_SCN_ReadBoardID, USB_TM_SCN_Read_08, 0x0, 0x0 );
	theApp.CommToBoards2( USB_CID_LNS_ReadBoardID, USB_TM_LNS_Read_08, 0x0, 0x0 );
	theApp.CommToBoards2( USB_CID_VAC_ReadBoardID, USB_TM_VAC_Read_08, 0x0, 0x0 );
	theApp.CommToBoards2( USB_CID_SPR_ReadBoardID, USB_TM_SPR_Read_08, 0x0, 0x0 );
	ReadBackdata( 16 );
}

void CPageMCI::OnResetCommunicate() 
{
	// Reset / Initialize Board Communication Circuit
	theApp.WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
}

void CPageMCI::ReadBackdata( int nBits )
{
	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
	pParent->m_listReadback.ResetContent();
	Sleep(30);

	if( nBits == 8 )
	{
		BYTE bData[1024];
		DWORD i, dwBytesReturned = 0;
		if( theApp.Serial_Read( bData, dwBytesReturned ) == 0 )
		{
			CString str, strMsg;
			str.Format( "Data Received: %ld bytes", dwBytesReturned );
			pParent->UpdateListReadbackString( str );
			
			if( dwBytesReturned > 0 )
			{
				strMsg = "";
				if( dwBytesReturned % frameLen2 == 0 )
				{
					for( i=0; i<dwBytesReturned; i++ )
					{
						str.Format( "0x%02X, ", bData[i] );
						strMsg += str;
						if( (i > 1) && ((i+1) % frameLen2 == 0) )
						{
							pParent->UpdateListReadbackString( strMsg );
							strMsg = "";
						}
					}
				}
				else
				{
					for( i=0; i<dwBytesReturned; i++ )
					{
						str.Format( "0x%02X, ", bData[i] );
						pParent->UpdateListReadbackString( str );
					}
				}
				pParent->UpdateListReadbackString( " " );
				if( dwBytesReturned > frameLen2 )
				{
					for( i=0; i<dwBytesReturned; i++ )
					{
						if( (i+4 < dwBytesReturned) && (bData[i] == byte_HeadRec) && (bData[i+1] == byte_LenSnd1) )
						{
							BYTE bHigh = bData[i+2] & 0x80;
							if( bHigh == 0x80 )
							{
								PwrVconV( bData[i+2], bData[i+3], pParent );
								i += 4;
							}
						}
					}
				}
				else
				{
					if( (bData[fHead] == byte_HeadRec) && (bData[fLen] == byte_LenSnd1) && (bData[frameLen2-1] == byte_TailRec) )
					{
						BYTE bHigh = bData[fLen +1] & 0x80;
						if( bHigh == 0x80 )
						{
							// 电源回读
							PwrVconV( bData[fLen+1], bData[fLen+2], pParent );
						}
						else
						{
							// B & D状态回读
							str.Format( "D = 0x%02X, B = 0x%02X", bData[fLen+1], bData[fLen+2] );
							pParent->UpdateListReadbackString( str );
						}
					}
				}
			}
		}
	}
	else
	{
		WORD bData[1024];
		DWORD i, dwBytesReturned = 0;
		if( theApp.Serial_Read( bData, dwBytesReturned ) == 0 )
		{
			CString str;
			str.Format( "Data Received: %ld words, %ld bytes", dwBytesReturned /2, dwBytesReturned );
			pParent->UpdateListReadbackString( str );
			
			if( dwBytesReturned > 0 )
			{
				for( i=0; i<dwBytesReturned/2; i++ )
				{
					str.Format( "0x%04X,", bData[i] );
					pParent->UpdateListReadbackString( str );
				}
				pParent->UpdateListReadbackString( " " );
			}
		}
	}
}

void CPageMCI::PwrVconV( BYTE bData1, BYTE bData2, CDialog* pParent )
{
	// 电源回读数字量转模拟量
	// CH2：+8V		模拟电源，	回读值 D /4095 *4.096 *3
	// CH6：+3.3V	模拟电源，	回读值 D /4095 *4.096 *2
	// CH7：+5V		数字电源，	回读值 D /4095 *4.096 *2
	// CH5：+5V		模拟电源，	回读值 D /4095 *4.096 *2
	// CH1：-8V		模拟电源，	回读值 D /4095 *4.096 *3.5 - 2.5 *(CH2)
	// CH0：-5V		模拟电源，	回读值 D /4095 *4.096 *3.5 - 2.5 *(CH5)
	// CH3：CH0-3	基准源，	回读值 D /4095 *4.096 *6/5
	// CH4：CH4-5	基准源，	回读值 D /4095 *4.096 *2

	WORD wData = ((bData1 & 0x0F) << 8) | bData2;
	int nChn = (bData1 & 0x70)	/16;
	double dValue = 0.0;

	CString strRef;
	switch( nChn )
	{
	case	7:
		dValue = 2 *g_dPwrbitsRange *wData /g_wPwrbitsRange;
		strRef = "+5V";
		break;
	case	4:
		dValue = 2 *g_dPwrbitsRange *wData /g_wPwrbitsRange;
		strRef = "";
		break;
	case	5:
		dValue = 2 *g_dPwrbitsRange *wData /g_wPwrbitsRange;
		strRef = "+5V";
		break;
	case	6:
		dValue = 2 *g_dPwrbitsRange *wData /g_wPwrbitsRange;
		strRef = "+3.3V";
		break;
	case	3:
		dValue = 6 *g_dPwrbitsRange /5 *wData /g_wPwrbitsRange;
		strRef = "";
		break;
	case	2:
		dValue = 3 *g_dPwrbitsRange *wData /g_wPwrbitsRange;
		strRef = "+8V";
		break;
	case	1:
		dValue = 3.5 *g_dPwrbitsRange *wData /g_wPwrbitsRange - 2.5 *m_dPwrReadback[2];
		strRef = "-8V";
		break;
	case	0:
		dValue = 3.5 *g_dPwrbitsRange *wData /g_wPwrbitsRange - 2.5 *m_dPwrReadback[5];
		strRef = "-5V";
		break;
	default:
		break;
	}
	m_dPwrReadback[nChn] = dValue;

	CString str;
	str.Format( "%d chn Pwr readbak: 0x%03X, %+.3f (Ref = %s)", nChn, wData, dValue, strRef );
	((CInsample0Dlg*)pParent)->UpdateListReadbackString( str );
}








void CPageMCI::OnSetPixelRate() 
{
	// Send binary 101(5) on Port C and 0x11 on Port A to begin setting pixel clock rate to 8 x 125nS (1 MHz),
	// followed by 001 on C + 8 on A, then 010 on C + 0 on A, then 111 on C, then 000 on C.
	UpdateData( TRUE );
	BOOL bIsStop = theApp.m_bIsStop;
	if( !bIsStop )
		ScanStop();

	BYTE bLow, bHigh;
	CString str;
	str = m_strPixelRateLB;
	char *bufLB = str.GetBuffer(3);
	sscanf(bufLB, "%x", &bLow);
	str = m_strPixelRateHB;
	char *bufHB = str.GetBuffer(3);
	sscanf(bufHB, "%x", &bHigh);

	WORD wData = bHigh *256 | bLow;
	theApp.WriteToBoard( USB_SetPixelRate, TRUE, wData );

	if( !bIsStop )
		ScanStart();
}

void CPageMCI::OnSetPixClkEq250KHz() 
{
	BOOL bIsStop = theApp.m_bIsStop;
	if( !bIsStop )
		ScanStop();

	theApp.WriteToBoard( USB_SetPixelRate, TRUE, 0x20 );

	if( !bIsStop )
		ScanStart();
}

void CPageMCI::OnSetPixClkEq1MHz() 
{
	BOOL bIsStop = theApp.m_bIsStop;
	if( !bIsStop )
		ScanStop();

	theApp.WriteToBoard( USB_SetPixelRate, TRUE, 0x08 );

	if( !bIsStop )
		ScanStart();
}

void CPageMCI::OnPixClkChanged( UINT nID )
{
	UpdateData();
	BOOL bIsStop = theApp.m_bIsStop;
	if( !bIsStop )
		ScanStop();

	COLORREF clr = ::GetSysColor(COLOR_3DFACE);
	m_radioPixClkEq1.SetColor( clr );
	m_radioPixClkEq2.SetColor( clr );
	m_radioPixClkEq3.SetColor( clr );
	m_radioPixClkEq4.SetColor( clr );
	m_radioPixClkEq5.SetColor( clr );
	m_radioPixClkEq6.SetColor( clr );
	m_radioPixClkEq7.SetColor( clr );
	m_radioPixClkEq8.SetColor( clr );
	m_radioPixClkEq9.SetColor( clr );
	m_radioPixClkEq10.SetColor( clr );
//	Invalidate(0);

	switch( m_nPixClk )
	{
	case	0:
		m_Speed	= USB_SS_125ns;
		m_radioPixClkEq1.SetColor( g_crEmphasis1 );
		break;
	case	1:
		m_Speed	= USB_SS_250ns;
		m_radioPixClkEq2.SetColor( g_crEmphasis1 );
		break;
	case	2:
		m_Speed	= USB_SS_500ns;
		m_radioPixClkEq3.SetColor( g_crEmphasis1 );
		break;
	case	3:
		m_Speed	= USB_SS_1us;
		m_radioPixClkEq4.SetColor( g_crEmphasis1 );
		break;
	case	4:
		m_Speed	= USB_SS_2us;
		m_radioPixClkEq5.SetColor( g_crEmphasis1 );
		break;
	case	5:
		m_Speed	= USB_SS_5us;
		m_radioPixClkEq6.SetColor( g_crEmphasis1 );
		break;
	case	6:
		m_Speed	= USB_SS_10us;
		m_radioPixClkEq7.SetColor( g_crEmphasis1 );
		break;
	case	7:
		m_Speed	= USB_SS_16us;
		m_radioPixClkEq8.SetColor( g_crEmphasis1 );
		break;
	case	8:
		m_Speed	= USB_SS_20us;
		m_radioPixClkEq9.SetColor( g_crEmphasis1 );
		break;
	case	9:
		m_Speed	= USB_SS_40us;
		m_radioPixClkEq10.SetColor( g_crEmphasis1 );
		break;
	}

	// 设置点逗留时间 pixel rate
	theApp.WriteToBoard( USB_SetPixelRate, TRUE, USB_PIXEL_RATES[m_Speed] );

	if( !bIsStop )
		ScanStart();
}

void CPageMCI::OnSetXrampSeg() 
{
	// Send binary 101 on Port C and 0x21 on Port A to for "Set Xramp segment" opcode for setting Xoffset=6,
	// followed by 010 on C and 0 on A, then 001 on C ans 0x06 on A, for high and low byte operand,
	// then 111 on C to load, and then 000 on C to finish.
	theApp.WriteToBoard( USB_SetXoffset, TRUE, 0x06 );
}

void CPageMCI::OnScanStart() 
{
	ScanStart();
}

void CPageMCI::ScanStart()
{
	// Send binary 100 on Port C and 0x12 on Port A, then bin 111 on C, then 000 on C to start scan
 	unsigned long DIOData = 0;
	CString str = "";
	CString strTemp = "";
	CString strTemp2 = "";

	// Clear FIFO
	unsigned long lRet = AIOUSB_ClearFIFO(theApp.deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	strTemp.Format( "ClearFIFO Ret = %ld", lRet );
	theApp.UpdateListString( strTemp );

	// Video mode and Detector mode
	CSize sz = GetSizes();
	if( m_bVideoMode )
		strTemp = "Video Raster";
	else
		strTemp = "Video XYZ";
	if( m_bDetMode )
		strTemp2 = "Duel Detector";
	else
		strTemp2 = "Single Detector";
	str.Format( "Reso = %d*%d, USB pixel rate = %.1f us (N = %d)",
				sz.cx, sz.cy, 1.*USB_SCANSPEED_TP[m_Speed]/1000, USB_PIXEL_RATES[m_Speed] );
	theApp.UpdateListString( str );
	str = "";
	str.Format( "%s\r\n, %s\r\n", strTemp, strTemp2 );
	theApp.UpdateListString( str );

	// Start scan
	theApp.WriteToBoard( USB_StartScan, FALSE, 0 );

	// setup the signal
	theApp.m_bIsStop = FALSE;
}

void CPageMCI::OnScanStartWithParams() 
{
	// TODO: Add your control notification handler code here
	
}

void CPageMCI::OnScanStop() 
{
	ScanStop();
}

void CPageMCI::ScanStop()
{
	// Send binary 100 on Port C and 0x13 on Port A, then bin 111 on C, then 000 on C to start scan
	theApp.WriteToBoard( USB_StopScan, FALSE, 0 );

	// setup the signal
	theApp.m_bIsStop = TRUE;
}

void CPageMCI::OnUpdateX() 
{
	BOOL bIsStop = theApp.m_bIsStop;
	if( !bIsStop )
		ScanStop();

	// Send binary 100 on Port C and 0x14 on Port A, then bin 111 on C, then 000 on C to start scan
	theApp.WriteToBoard( USB_UpdateXramp, FALSE, 0 );

	if( !bIsStop )
		ScanStart();
}

void CPageMCI::OnUpdateY() 
{
	BOOL bIsStop = theApp.m_bIsStop;
	if( !bIsStop )
		ScanStop();

	// Send binary 100 on Port C and 0x15 on Port A, then bin 111 on C, then 000 on C to start scan
	theApp.WriteToBoard( USB_UpdateYramp, FALSE, 0 );

	if( !bIsStop )
		ScanStart();
}

void CPageMCI::OnUpdateDefaultRamps() 
{
	BOOL bIsStop = theApp.m_bIsStop;
	if( !bIsStop )
		ScanStop();

	// Send binary 100 on Port C and 0x16 on Port A, then bin 111 on C, then 000 on C to start scan
	theApp.WriteToBoard( USB_SetDefaultRamps, FALSE, 0 );

	if( !bIsStop )
		ScanStart();
}

void CPageMCI::OnSetRampSegment() 
{
	// PSC Exp
	// Send binary 101 in Port C and txtSeg in Port A to for the segment,
	// followed by 010 on C and 0 on A, then xHB on Port A, for high byte operand, then 001 in C and xLB in A
	// then 111 on C to load, and then 000 on C to finish.
	UpdateData( TRUE );

	BYTE bNum, bLow, bHigh;
	CString str;
	str = m_strSegNum;
	char *bufNum = str.GetBuffer(3);
	sscanf(bufNum, "%x", &bNum);
	str = m_strSegLB;
	char *bufLB = str.GetBuffer(3);
	sscanf(bufLB, "%x", &bLow);
	str = m_strSegHB;
	char *bufHB = str.GetBuffer(3);
	sscanf(bufHB, "%x", &bHigh);

	WORD wData = bHigh *256 | bLow;
	theApp.WriteToBoard( bNum, TRUE, wData );
}

CSize CPageMCI::GetSizes()
{
	CSize sz(256,256);
	switch( m_nReso )
	{
	case	0:
		sz = CSize(16,16);
		break;
	case	1:
		sz = CSize(32,32);
		break;
	case	2:
		sz = CSize(64,64);
		break;
	case	3:
		sz = CSize(128,128);
		break;
	case	4:
		sz = CSize(256,256);
		break;
	case	5:
		sz = CSize(512,512);
		break;
	case	6:
		sz = CSize(1024,1024);
		break;
	}
	return sz;
}

void CPageMCI::OnResolutionChanged( UINT nID )
{
	UpdateData();
	BOOL bIsStop = theApp.m_bIsStop;
	if( !bIsStop )
		ScanStop();

	COLORREF clr = ::GetSysColor(COLOR_3DFACE);
	m_radioResolution1.SetColor( clr );
	m_radioResolution2.SetColor( clr );
	m_radioResolution3.SetColor( clr );
	m_radioResolution4.SetColor( clr );
	m_radioResolution5.SetColor( clr );
	m_radioResolution6.SetColor( clr );
	m_radioResolution7.SetColor( clr );
//	Invalidate(0);

	int i, nOffset = 0;
	switch( m_nReso )
	{
	case	0:
		m_radioResolution1.SetColor( g_crEmphasis1 );
		break;
	case	1:
		m_radioResolution2.SetColor( g_crEmphasis1 );
		nOffset = 50;
		break;
	case	2:
		m_radioResolution3.SetColor( g_crEmphasis1 );
		nOffset = 1000;
		break;
	case	3:
		m_radioResolution4.SetColor( g_crEmphasis1 );
		nOffset = 2000;
		break;
	case	4:
		m_radioResolution5.SetColor( g_crEmphasis1 );
		nOffset = 4000;
		break;
	case	5:
		m_radioResolution6.SetColor( g_crEmphasis1 );
		nOffset = 8000;
		break;
	case	6:
		m_radioResolution7.SetColor( g_crEmphasis1 );
		nOffset = 1600;
		break;
	}

	CSize sz = GetSizes();

	///////////////////////////////////////////////////////////////////////////
	USB_InitData	data;
	ZeroMemory( &data, sizeof(USB_InitData) );

	// 始终使用满量程2的14次方，步距根据当前分辨率设置
//	WORD	wFullRange	= (WORD)pow(2, 14);
	WORD	wFullRange	= 65535;	//这是目前试验时的最大值
	WORD	wStepX		= (WORD)(wFullRange / sz.cx);
	WORD	wStepY		= (WORD)(wFullRange / sz.cy);

	USB_InitScanRampParam usbParam;
	// X扫描波
	usbParam.wOffset		= 0;//(WORD)rcFrame.left;
	usbParam.wValley		= ValleyX;
	usbParam.wRise			= (WORD)sz.cx;//rcFrame.Width();
	usbParam.wDRise			= wStepX;
	usbParam.wPeak			= PeakX;
	usbParam.wFall			= FallX;
	usbParam.wDFall			= DFallX;//(WORD)(rcFrame.Width()/usbParam.wFall *wStepX);
//	usbParam.wDirection		= 0;
	data.ScanRampParamX		= usbParam;

	// Y扫描波
	usbParam.wOffset		= 0;//(WORD)rcFrame.top;
	usbParam.wValley		= ValleyY;
	usbParam.wRise			= (WORD)sz.cy;//rcFrame.Height();
	usbParam.wDRise			= wStepY;
	usbParam.wPeak			= PeakY;
	usbParam.wFall			= FallY;
	usbParam.wDFall			= DFallY;//(WORD)(rcFrame.Height()/usbParam.wFall *wStepX);
//	usbParam.wDirection		= 1;
	data.ScanRampParamY		= usbParam;

	data.Speed				= m_Speed;
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	unsigned char funcNo;
	USB_InitParam* pParam;
	pParam = (USB_InitParam *)(&data);

	// 设置点逗留时间 pixel rate
	theApp.WriteToBoard( USB_SetPixelRate, TRUE, USB_PIXEL_RATES[data.Speed] );

	// 设置X扫描波形7参数
	funcNo = USB_SetXoffset;
	for (i=0; i<7; i++)
	{
		if( i == 0 )
			theApp.WriteToBoard( funcNo +i, TRUE, pParam->wData[i] +nOffset );
		else
			theApp.WriteToBoard( funcNo +i, TRUE, pParam->wData[i] );
	}
	theApp.WriteToBoard( USB_UpdateXramp, FALSE, 0 );

	// 设置Y扫描波形7参数
	funcNo = USB_SetYoffset;
	for (i=0; i<7; i++)
		theApp.WriteToBoard( funcNo +i, TRUE, pParam->wData[i +7] );
	theApp.WriteToBoard( USB_UpdateYramp, FALSE, 0 );
	///////////////////////////////////////////////////////////////////////////

	if( !bIsStop )
		ScanStart();
}

void CPageMCI::OnDet03Load() 
{
	// Send binary 100 on Port C and 0x32 on Port A, then bin 111 on C, then 000 on C to start scan
	theApp.WriteToBoard( USB_Det03LoadDACs, FALSE, 0 );
}

void CPageMCI::OnDet45Load() 
{
	// Send binary 101(5) on Port C and opcode 0x34 on Port A
	// followed by 001 on C + 8 on A, then 010 on C + 0 on A, then 111 on C, then 000 on C.
	theApp.WriteToBoard( USB_Det45LoadDACs, FALSE, 0 );
}

void CPageMCI::OnDeltaposDet03(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nDet03Chn2 -= pNMUpDown->iDelta;
	if(m_nDet03Chn2 < 0)
		m_nDet03Chn2 = 0;
	else if(m_nDet03Chn2 > 3)
		m_nDet03Chn2 = 3;

	CString str;
	str.Format( "%d", m_nDet03Chn2 );
	GetDlgItem( IDC_EDIT_Det03_Chn2 )->SetWindowText( str );

	*pResult = 0;
}

void CPageMCI::OnDeltaposDet45(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nDet45Chn2 -= pNMUpDown->iDelta;
	if(m_nDet45Chn2 < 0)
		m_nDet45Chn2 = 0;
	else if(m_nDet45Chn2 > 3)
		m_nDet45Chn2 = 3;
/*
	if( m_nDet45Chn2 == 0 || m_nDet45Chn2 == 1 )
		m_scrollDet45.SetScrollRange( 0, 4095 );
	else
		m_scrollDet45.SetScrollRange( 0, 0x8FF );
*/	

	CString str;
	str.Format( "%d", m_nDet45Chn2 );
	GetDlgItem( IDC_EDIT_Det45_Chn2 )->SetWindowText( str );

	*pResult = 0;
}

void CPageMCI::OnDeltaposDetGain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nDetGainNo2 -= pNMUpDown->iDelta;
	if(m_nDetGainNo2 < 0)
		m_nDetGainNo2 = 0;
	else if(m_nDetGainNo2 > 5)
		m_nDetGainNo2 = 5;
	UpdateData( FALSE );

	*pResult = 0;
}

void CPageMCI::OnDeltaposDetPair(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nDetPair2 -= pNMUpDown->iDelta;
	if(m_nDetPair2 < 0)
		m_nDetPair2 = 0;
	else if(m_nDetPair2 > 2)
		m_nDetPair2 = 2;
	UpdateData( FALSE );

	*pResult = 0;
}

void CPageMCI::OnDeltaposSPINPwrChn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nPwrChn -= pNMUpDown->iDelta;
	if(m_nPwrChn < 0)
		m_nPwrChn = 0;
	else if(m_nPwrChn > 7)
		m_nPwrChn = 7;
	UpdateData( FALSE );

	*pResult = 0;
}

void CPageMCI::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	case IDC_SCROLLBAR_Det03:
		Det03SB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Det45:
		Det45SB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_DetGain:
		DetGainSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_BW_sel:
		BWselSB_Change( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPageMCI::Det03SB_Change( int nPos )
{
	UpdateData();
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_Det03 )->SetWindowText( str );
	m_nPosDet03[m_nDet03Chn2] = nPos;
	theApp.m_DL.diag[Diag_Offset_0 + m_nDet03Chn2].nCurPos = nPos;

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
	///////////////////////////////////////////////////////////////////////////

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (m_nDet03Chn2 << 6)) << 8);
	theApp.WriteToBoard( USB_Det03StoreDAC, TRUE, wValue );
}

void CPageMCI::Det45SB_Change( int nPos )
{
	UpdateData();
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );
	m_nPosDet45[m_nDet45Chn2] = nPos;
	theApp.m_DL.diag[Diag_Offset_4 + m_nDet45Chn2].nCurPos = nPos;

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (m_nDet45Chn2 << 6)) << 8);
	theApp.WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
}

void CPageMCI::DetGainSB_Change( int nPos )
{
	UpdateData();
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );
	///////////////////////////////////////////////////////////////////////////

	WORD wValue = (WORD)( (m_nDetGainNo2 << 8) | (BYTE)nValue );
	theApp.WriteToBoard( USB_SetDetGain, TRUE, wValue );
}

void CPageMCI::BWselSB_Change( int nPos )
{
	UpdateData();
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	CString str = "";

	///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );
	///////////////////////////////////////////////////////////////////////////

	// filter pin in operand low byte
	// detector number in operand high byte
	WORD wValue = (WORD)( (m_nDetPair2 << 8) | (BYTE)nValue );
	theApp.WriteToBoard( USB_SetDetFilter, TRUE, wValue );
}

/*
For detector pair selection:      
Opcode:		x55,
operand:	x00	-	Detector pair 0-1, default (Note, old default was pair 4-5)
			x01	-	Detector pair 2-3
			x02	-	Detector pair 4-5
			x03	-	spare
input connector:	J5A - pair0-1, J5B - pair2-3, J6A - pair4-5
*/

void CPageMCI::OnDetPairSelect() 
{
	// TODO: Add your control notification handler code here
	WORD wData = 0x0;
	int nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->GetCheck();
	if( nCheck )
		wData = 0x00;
	else
	{
		nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_23 )))->GetCheck();
		if( nCheck )
			wData = 0x01;
		else
		{
			nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_45 )))->GetCheck();
			if( nCheck )
				wData = 0x02;
		}
	}
	theApp.WriteToBoard( USB_DetPairSelection, TRUE, wData );
}

void CPageMCI::OnBoardComm() 
{
	// Send binary 101(5) on Port C and 0x11 on Port A,
	// followed by 001 on C and 8 on A, then 010 on C + 0 on A, then 111 on C, then 000 on C.
	// "Board_Opr_HB" text box contains high byte, "Board_Opr_LB" contains low byte.
/*	UpdateData( TRUE );

	BYTE bLow, bHigh;
	CString str;

	str = m_strBoardOprLB;
	char *bufLB = str.GetBuffer(3);
	sscanf(bufLB, "%x", &bLow);
	str = m_strBoardOprHB;
	char *bufHB = str.GetBuffer(3);
	sscanf(bufHB, "%x", &bHigh);

	theApp.CommToBoards( bLow, bHigh );*/

	// 用于测试新编带通讯线工作是否正常
	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
	pParent->m_listReadback.ResetContent();

	CString str;
	unsigned short nRegister;
	int nSCN, nLNS, nVAC, nSPR, nCounts;
	nSCN = nLNS = nVAC = nSPR = 0;
	GetDlgItem(IDC_EDIT_Board_Opr_LB)->GetWindowText(str);
	nCounts = atoi(str);
	for( int i=0; i<nCounts; i++ )
	{
		// SCN
		nRegister = 0;
		ReadBoardID( 1, &nRegister );
		if( nRegister == 0xA1 )
			str.Format( "%03d times read SCN boardID, Y", i+1 );
		else
		{
			str.Format( "%03d times read SCN boardID, ERR", i+1 );
			nSCN++;
		}
		pParent->UpdateListReadbackString( str );

		// LNS
		nRegister = 0;
		ReadBoardID( 2, &nRegister );
		if( nRegister == 0xA2 )
			str.Format( "%03d times read LNS boardID, Y", i+1 );
		else
		{
			str.Format( "%03d times read LNS boardID, ERR", i+1 );
			nLNS++;
		}
		pParent->UpdateListReadbackString( str );

		// VAC
		nRegister = 0;
		ReadBoardID( 3, &nRegister );
		if( nRegister == 0xA3 )
			str.Format( "%03d times read VAC boardID, Y", i+1 );
		else
		{
			str.Format( "%03d times read VAC boardID, ERR", i+1 );
			nVAC++;
		}
		pParent->UpdateListReadbackString( str );

		// SPR
		nRegister = 0;
		ReadBoardID( 4, &nRegister );
		if( nRegister == 0xA4 )
			str.Format( "%03d times read SPR boardID, Y", i+1 );
		else
		{
			str.Format( "%03d times read SPR boardID, ERR", i+1 );
			nSPR++;
		}
		pParent->UpdateListReadbackString( str );
	}
	pParent->UpdateListReadbackString( " " );
	str.Format( "SCN ERR%% = %.2f%%", nSCN*100.0 /nCounts );
	pParent->UpdateListReadbackString( str );
	str.Format( "LNS ERR%% = %.2f%%", nLNS*100.0 /nCounts );
	pParent->UpdateListReadbackString( str );
	str.Format( "VAC ERR%% = %.2f%%", nVAC*100.0 /nCounts );
	pParent->UpdateListReadbackString( str );
	str.Format( "SPR ERR%% = %.2f%%", nSPR*100.0 /nCounts );
	pParent->UpdateListReadbackString( str );
}

BOOL CPageMCI::ReadBoardID( int nMID, unsigned short* pBoardID )
{
	BYTE bOp1 = 0x0;
	BYTE bOp2 = 0x0;
	switch( nMID )
	{
	case	1:
		bOp1	= USB_CID_SCN_ReadBoardID;
		bOp2	= USB_TM_SCN_Read_08;
		break;
	case	2:
		bOp1	= USB_CID_LNS_ReadBoardID;
		bOp2	= USB_TM_LNS_Read_08;
		break;
	case	3:
		bOp1	= USB_CID_VAC_ReadBoardID;
		bOp2	= USB_TM_VAC_Read_08;
		break;
	case	4:
		bOp1	= USB_CID_SPR_ReadBoardID;
		bOp2	= USB_TM_SPR_Read_08;
		break;
	default:
		break;
	}
	if( bOp1 == 0x0 )
		return FALSE;

	theApp.CommToBoards2( bOp1, bOp2, 0, 0 );

	Sleep( 30 );
	BOOL bRet = FALSE;
	unsigned short nData[256];
	ZeroMemory(nData, sizeof(nData));
	DWORD dwBytesReturned;
	int nRet = theApp.Serial_Read( nData, dwBytesReturned );
	if( (nRet == 0) && (dwBytesReturned > 0) )
	{
		for( DWORD i=0; i<dwBytesReturned/2; i++ )
		{
			if( nData[i] == 0xD22D )
			{
				pBoardID[0] = nData[i+1];
				bRet = TRUE;
			}
		}
	}
	return bRet;
}
/*
void CPageMCI::OnTestLED16on() 
{
	// Utilizes TestLED16State global, temporarily uses CL wobbler On ckt address (0x1c)
 	unsigned long DIOData = 0;
	CString str = "";
	CString strTemp = "";

	int nCheck = m_btnTestLED16State.GetCheck();
	if( nCheck )
	{
		m_btnTestLED16State.SetColor( g_crON_Test );
		m_btnTestLED16State.SetWindowText( "Test LED is now ON" );
	}
	else
	{
		m_btnTestLED16State.SetColor( g_crOFF );
		m_btnTestLED16State.SetWindowText( "Test LED is now OFF" );
	}

    ///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_SCN_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_SCN_Write_08,		// Serial Comm operand high byte (8 bit write to SCN board)
							0x0,						// Low byte for 8 bit write needs to be in hi byte operand location (for now)
							nCheck );					// Hi byte data is actually low byte of 8 bit write
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_LNS_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_LNS_Write_08,		// Serial Comm operand high byte (8 bit write to LNS board)
							0x0,						// Low byte for 8 bit write needs to be in hi byte operand location (for now)
							nCheck );					// Hi byte data is actually low byte of 8 bit write
    ///////////////////////////////////////////////////////////////////////////
}
*/









#define	tmrGray		1

void CPageMCI::OnStartImage() 
{
	// TODO: Add your control notification handler code here
	CButton* pBtn = (CButton*)(GetDlgItem( IDC_CHECK_StartImage ));
	int nCheck = pBtn->GetCheck();
	if( nCheck )
	{
		pBtn->SetWindowText( "Stop Image" );
		SetTimer( tmrGray, 1000, NULL );
	}
	else
	{
		pBtn->SetWindowText( "Start Image" );
		KillTimer( tmrGray );
		Invalidate(0);
	}
}

void CPageMCI::DrawGrayScale(int nXDim, int nYDim)
{
/*	int m_nX, m_nY, nPix, nIncr, nFrame;
	CString strMsg;

//	CPaintDC dc(this); // device context for painting
	CDC* pDC;

	CPen pen, *poldpen;
	nIncr = 0;
	for( nFrame=0; nFrame<5; nFrame++ )
	{
		for( m_nY=0; m_nY<nYDim; m_nY++ )
		{
			for( m_nX=0; m_nX<nXDim; m_nX++ )
			{
				nPix = m_nX / 2 + nIncr;

				pen.CreatePen( PS_SOLID, 1, RGB(nPix, nPix, nPix) );
				poldpen = pDC->SelectObject( &pen );
				pDC->MoveTo( m_nX, m_nY );
				pDC->LineTo( m_nX +1, m_nY );
				pDC->SelectObject( poldpen );
				pen.DeleteObject();
//				PSet (ix, iy), RGB(iPix, iPix, iPix)
			}
		}
		nIncr += 8;
		strMsg.Format( "Done %d frames", nFrame );
	}*/
}

void CPageMCI::OnButton1() 
{
/*	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
	pParent->m_listReport.ResetContent();
	m_g_ReportList	= &(pParent->m_listReport);
*/	m_g_Dialog		= this;

	status = DIO_StreamOpen (theApp.deviceIndex, 1); //the 1 means it is a read. Send 0 for a write

	if (status != ERROR_SUCCESS) //this really should never happen.
	{
		theApp.UpdateListString("Failed to open DIO stream.");
	}
	else
	{
		theApp.UpdateListString("Opened DIO stream...");

		InitData();

//		ScanStart();
		g_lastTickCount = GetTickCount();
		AfxBeginThread(threadfunc, NULL);
	}
}

BOOL CPageMCI::InitData()
{
	CString str, strTemp;

	( GetDlgItem( IDC_EDIT_RecCounts ) )->GetWindowText( str );
	if( str.IsEmpty() )
		return FALSE;
	m_lReadSize = atol( str );	// data length of each time
	( GetDlgItem( IDC_EDIT_RecTimes ) )->GetWindowText( str );
	if( str.IsEmpty() )
		m_lLoopCounts = 1;		// how many times
	else
		m_lLoopCounts = atol( str );
	strTemp.LoadString( NULL, IDS_MCI_Collect_Scan_Data );
	str.Format( strTemp, m_lLoopCounts, 1.0 * m_lReadSize / 1024 );
	GetDlgItem( IDC_BUTTON1 )->SetWindowText( str );

	long lReadSizeBuff = m_lReadSize +512;
	// Allocate on the heap
	if ( m_pBuffUSB != NULL )
	{
		delete [] m_pBuffUSB;
		m_pBuffUSB	= NULL;
	}
	m_pBuffUSB = new unsigned short[m_lLoopCounts*lReadSizeBuff];
	ZeroMemory(m_pBuffUSB, sizeof(m_pBuffUSB));

	int iScan = 0;
	for ( iScan = 0; iScan < m_lLoopCounts; ++iScan )
	{
		unsigned long lOffset = iScan*lReadSizeBuff;
		for ( int iReading = 0; iReading < lReadSizeBuff; ++iReading )
		{
			readings2D[iScan][iReading] = 0xCCCC;
			m_pBuffUSB[lOffset+iReading] = 0xCCCC;
		}
	}
	for ( iScan = 0; iScan < 10; iScan++ )
		readingsCounts[iScan] = 0;

	m_dwX			= 0xFFFF;
	m_dwY			= 0xFFFF;
	m_dwZa			= 0xFFFF;
	m_nIndexX		= 0;		// 有效的数据的X序号
	m_nIndexY		= 0;		// 有效的数据的Y序号
	m_nX			= 0;
	m_nY			= 0;
	m_bPeakX		= FALSE;
	m_bPeakY		= FALSE;
	m_nDisX			= 0;
	m_nDisY			= 0;

	if( !m_bDispMode )	// 延迟显示时打开专用窗口显示图像
	{
		m_dlgDisplay.ShowWindow(SW_SHOW);
		CDC* pDC = m_dlgDisplay.GetDC();
		CRect rc;
		m_dlgDisplay.GetClientRect( &rc );
		rc.right = rc.bottom;
		// 将显示区域设置一个特殊的背景
		pDC->FillRect( rc, &CBrush(g_crImageBkg) );
	}

	// 清空一下数据记录区
	CEdit* peditLog = (CEdit*)(m_dlgDisplay.GetDlgItem( IDC_EDIT_List) );
	if( peditLog != NULL )
	{
		CString csMessage = _T("");
		int len = peditLog->GetWindowTextLength();
		peditLog->SetSel(0,len);
		peditLog->ReplaceSel( (LPCTSTR)csMessage );
	}

	return TRUE;
}

UINT __cdecl threadfunc (LPVOID pParam)
{
	CString str, strTemp;
	g_thisTickCount = GetTickCount();
	g_TickCount = g_thisTickCount - g_lastTickCount;
	str.Format( "CreateThread time = %ld.\r\n", g_TickCount );
	m_g_ReportList->AddString(str);

	m_g_Dialog->ScanStart();
//	g_lastTickCount = GetTickCount();
	long lLoopCounts = m_g_Dialog->m_lLoopCounts;
	unsigned long lReadSize = m_g_Dialog->m_lReadSize;
	unsigned short* pReadings = m_g_Dialog->m_pBuffUSB;
	int iScan = 0;
	for ( iScan = 0; iScan < lLoopCounts; ++iScan )
	{
		total_readings = 0;
		Calls = 0;

		//m_g_ReportList->AddString("Getting scan data...");
		do
		{
			//If making a "remainder" request, round the remainder up to the next 256-sample packet.
			unsigned long MasterCt = lReadSize - total_readings;
			unsigned long RoundCt = MasterCt & 0xFFFFFF00;
			if ( MasterCt & 0xFF )
				RoundCt += 0x100;
//			status = DIO_StreamFrame(theApp.deviceIndex, RoundCt, &(readings2D[iScan][total_readings]), &bytes_transferred);
			status = DIO_StreamFrame(theApp.deviceIndex, RoundCt, &(pReadings[iScan*(lReadSize+512) +total_readings +1]), &bytes_transferred);
			total_readings += bytes_transferred / 2UL;
			++Calls;
/*			if ( status )
			{
				char Buf[128];
				sprintf(Buf, "Error code %ld...", status);
				m_g_ReportList->AddString(Buf);
			}
			else if ( bytes_transferred >= 2 )
			{
				char Buf[128];
				sprintf(Buf, "Got %ld bytes...", bytes_transferred);
				m_g_ReportList->AddString(Buf);
			}
			else
			{
				m_g_ReportList->AddString("Success, but no data...");
			}*/
		} while((total_readings < lReadSize) && (Calls < 10));
		//once we are out of this loop the readings array will contain the requested number of readings.
		{
/*			char Buf[128];
			sprintf(Buf, "Got scan, %ld calls...", Calls);
			m_g_ReportList->AddString(Buf);
*/			readingsCounts[iScan] = total_readings;
			pReadings[iScan*(lReadSize+512)] = total_readings;
		}
	}
	m_g_ReportList->AddString("Done getting data...");
	g_thisTickCount = GetTickCount();
	g_TickCount = g_thisTickCount - g_lastTickCount;
	str.Format( "Read time for collect %dx %.2fk data = %ld.\r\n", lLoopCounts, 1.0 * lReadSize / 1024, g_TickCount );
	m_g_ReportList->AddString(str);

	DIO_StreamClose(theApp.deviceIndex);

	m_g_ReportList->AddString("Closed DIO stream...");
/*	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
	str.Format( "zcollect%s.bin", strTemp );
	HANDLE hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

	if ( hFil )
	{
		m_g_ReportList->AddString("Opened log file...");
	}
	else
	{
		m_g_ReportList->AddString("Failed to open log file...");
	}
*/

	str = "";
//	unsigned long cbRet;
//	char Buf[32];
	g_lastTickCount = GetTickCount();
	long lReadSizeBuff = lReadSize +512;
	for ( iScan = 0; iScan < lLoopCounts; ++iScan )
	{
		strTemp.Format( "Writing scan data..., times %d", iScan +1);
		m_g_ReportList->AddString(strTemp);
/*		sprintf(Buf, "/BEGIN SCAN\\\r\n");
		WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
		str = "";
//		str = "BEGIN SCAN";
//		str += "\r\n";
*/
		strTemp.Format( "nLoop = %ld", readingsCounts[iScan]);
		m_g_ReportList->AddString(strTemp);

		unsigned long lOffset = iScan*lReadSizeBuff;
		for ( int iReading = 0; iReading < lReadSizeBuff; iReading++ )
		{
//			sprintf(Buf, "%ld\r\n", readings2D[iScan][iReading]);
//			WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);

//			strTemp.Format( "%04X, ", pReadings[iScan][iReading]);
			strTemp.Format( "%04X, ", pReadings[lOffset+iReading]);
			str += strTemp;
			if( ( iReading +1 ) % 8 == 0 )
			{
				m_g_ReportList->AddString(str);
				str = "";
//				str += "\r\n";
			}
		}
		m_g_ReportList->AddString(str);
		str = "";
		unsigned long lSize = 0;
		lSize = pReadings[lOffset];
		DWORD dwCounts = lSize;
		DWORD* pdwPixX = new DWORD[ dwCounts ];
		DWORD* pdwPixY = new DWORD[ dwCounts ];
		DWORD* pdwPixZ = new DWORD[ dwCounts ];

		if( m_g_Dialog->m_bVideoMode )
		{
			// 因为为提高MCI效率，现在数据格式总是
			// XYZ模式:(X, Y, Za, Zb), Raster模式:(Za, Zb)
			// 所以，Raster模式下分析数据的函数都调用双探测器的
			// Raster mode
			if( m_g_Dialog->m_bDetMode )
			{
				// Duel
//				int nCounts = m_g_Dialog->PixConvertZZZ( &(readings2D[iScan][0]), readingsCounts[iScan]*2, pdwPixX, pdwPixY, pdwPixZ );
				int nCounts = m_g_Dialog->PixConvertZZZ2( &(pReadings[lOffset]), lSize*2, pdwPixX, pdwPixY, pdwPixZ );
				m_g_Dialog->PixDisplayByReceiveZZZ( nCounts, pdwPixX, pdwPixY, pdwPixZ );
			}
			else
			{
				// Single
//				int nCounts = m_g_Dialog->PixConvertZZZ( &(readings2D[iScan][0]), readingsCounts[iScan]*2, pdwPixX, pdwPixY, pdwPixZ );
				int nCounts = m_g_Dialog->PixConvertZZZ( &(pReadings[lOffset]), lSize*2, pdwPixX, pdwPixY, pdwPixZ );
				m_g_Dialog->PixDisplayByReceiveZZZ( nCounts, pdwPixX, pdwPixY, pdwPixZ );
			}
		}
		else
		{
			// XYZ mode
			if( m_g_Dialog->m_bDetMode )
			{
				// Duel
//				int nCounts = m_g_Dialog->PixConvert2( &(readings2D[iScan][0]), readingsCounts[iScan]*2, pdwPixX, pdwPixY, pdwPixZ );
				int nCounts = m_g_Dialog->PixConvert2( &(pReadings[lOffset]), lSize*2, pdwPixX, pdwPixY, pdwPixZ );
				m_g_Dialog->PixDisplayByReceive2( nCounts, pdwPixX, pdwPixY, pdwPixZ );
			}
			else
			{
				// Single
//				int nCounts = m_g_Dialog->PixConvert( &(readings2D[iScan][0]), readingsCounts[iScan]*2, pdwPixX, pdwPixY, pdwPixZ );
				int nCounts = m_g_Dialog->PixConvert( &(pReadings[lOffset]), lSize*2, pdwPixX, pdwPixY, pdwPixZ );
				if( m_g_Dialog->m_bDispMode )
					m_g_Dialog->PixDisplayByReceiveSerial( nCounts, pdwPixX, pdwPixY, pdwPixZ );
				else
					m_g_Dialog->PixDisplayByReceiveDelay( nCounts, pdwPixX, pdwPixY, pdwPixZ );
			}
/*
			unsigned long nReadingCount = readingsCounts[iScan] *2;
			for( unsigned long i=0; i<nReadingCount; i+=4096 )
			{
				DWORD* pdwPixX = new DWORD[ 4096 ];//readingsCounts[iScan]*2 ];
				DWORD* pdwPixY = new DWORD[ 4096 ];//readingsCounts[iScan]*2 ];
				DWORD* pdwPixZ = new DWORD[ 4096 ];//readingsCounts[iScan]*2 ];

				int nCounts = m_g_Dialog->PixConvert( &(readings2D[iScan][i*4096]), 4096, pdwPixX, pdwPixY, pdwPixZ );
				m_g_Dialog->PixDisplayByReceive( nCounts, pdwPixX, pdwPixY, pdwPixZ );

				// Use the delete operator as follows:
				delete [] pdwPixX;
				delete [] pdwPixY;
				delete [] pdwPixZ;
			}
*/		}
/*
		sprintf(Buf, "\\END SCAN/\r\n\r\n");
//		WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
//		str += "\r\n";
//		str += "END SCAN";
//		str += "\r\n";
*/			// Use the delete operator as follows:
			delete [] pdwPixX;
			delete [] pdwPixY;
			delete [] pdwPixZ;
	}
	g_thisTickCount = GetTickCount();
	g_TickCount = g_thisTickCount - g_lastTickCount;
	str.Format( "Analyze and display time for collect %dx %.2fk data = %ld.\r\n", NUM_SCANS, 1.0 * NUM_READINGS / 1024, g_TickCount );
	m_g_ReportList->AddString(str);

//	CloseHandle(hFil);

	m_g_ReportList->AddString("Done.");

	return 0;
}

///////////////////////////////////////////////////////////////////////////
// 接收测试程序
// VB中接收数据存储区是字符串形式的，但VC不行，必须是pnFrameData的动态数组
//
// 另外，如果USB-DI16A的FIFO中的数据量 小于 需要接收的数据量，则DIO_StreamFrame执行时不能返回
// 表现的现象是计算机死机，必须强制关机后再重新启动
//
// 所以运行这部分程序时，最好先要start scan，然后马上stop scan；
// 保证FIFO中有数据，然后再单击Receive按钮；且连续单击Receive按钮次数最好小于5次
//
// 如果start scan后一直不停，此时连续单击Receive按钮，不会死机，但好像数据会乱跳（不确定，还需多次测试）
void CPageMCI::OnReceive() 
{
	if( m_g_ReportList == NULL )
		return;
	CString strBuf, strText, strMsg;
	long lReadSize;
	unsigned long lBytesTransferred = 0;

	( GetDlgItem( IDC_EDIT_RecLen ) )->GetWindowText( strText );
	if( strText.IsEmpty() )
		return;

	// Read uses an external clock(from the writing board), so we don't change the clocks here.

	lReadSize = atol( strText );
//	LPTSTR pBuf = strBuf.GetBufferSetLength( lReadSize *2 );

	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize +256];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

/*	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
	pParent->m_listReport.ResetContent();
*/	m_g_ReportList->ResetContent();
//	ScanStop();
//	ScanStart();
	DIO_StreamOpen( theApp.deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	DIO_StreamFrame( theApp.deviceIndex, lReadSize, &pnFrameData[1], &lBytesTransferred );
//	DIO_StreamFrame( theApp.deviceIndex, lReadSize, pnFrameData, &lBytesTransferred );
	DIO_StreamClose( theApp.deviceIndex );
	pnFrameData[0] = (unsigned short)(lBytesTransferred /2);
//	ScanStop();

	// Rather than displaying the received data in a memo, you could save it, process it further, etc.

	strText.Format( "Data Received: %ld Words", lBytesTransferred /2 );
//	(GetDlgItem( IDC_STATIC_DataMemo ) )->SetWindowText( strText );
	theApp.UpdateListString( strText );

	strMsg = "";
	for( unsigned long i=1; i<lBytesTransferred/2 +1; i++ )
	{
		strText.Format( "%04X, ", pnFrameData[i] );
		strMsg += strText;
		if( (i+1) % 8 == 0 )
		{
			theApp.UpdateListString( strMsg );
			strMsg = "";
		}
	}
	theApp.UpdateListString( strMsg );
//	(GetDlgItem( IDC_EDIT_DataMemo ) )->SetWindowText( strMsg );
	DWORD* pdwPixX = new DWORD[ lBytesTransferred/2 ];
	DWORD* pdwPixY = new DWORD[ lBytesTransferred/2 ];
	DWORD* pdwPixZ = new DWORD[ lBytesTransferred/2 ];

//	int nCounts = PixConvert( strMsg, pdwPixX, pdwPixY, pdwPixZ );
	int nCounts = PixConvert( pnFrameData, lBytesTransferred, pdwPixX, pdwPixY, pdwPixZ );
	if( m_bDispMode )
		PixDisplayByReceiveSerial( nCounts, pdwPixX, pdwPixY, pdwPixZ );
	else
		PixDisplayByReceiveDelay( nCounts, pdwPixX, pdwPixY, pdwPixZ );

	// Use the delete operator as follows:
	delete [] pdwPixX;
	delete [] pdwPixY;
	delete [] pdwPixZ;
	delete [] pnFrameData;
}

void CPageMCI::OnInHexCheck() 
{
	// TODO: Add your control notification handler code here
	
}

int nReceiveIndex = 0;
void CPageMCI::OnDemoImage() 
{
	// TODO: Add your control notification handler code here
	CString strBuf;
	ScanStart();

	// Allocate on the heap
	DWORD* pdwPixX = new DWORD[ 64 ];
	DWORD* pdwPixY = new DWORD[ 64 ];
	DWORD* pdwPixZ = new DWORD[ 64 ];
//	DWORD dwPixData[64][64][64];
//	DWORD dwPixX[64], dwPixY[64], dwPixZ[64];

	nReceiveIndex = 1;
	strBuf = PixReceive(102);
//	PixConvert( strBuf, &dwPixData );
	int nCounts = PixConvert( strBuf, pdwPixX, pdwPixY, pdwPixZ );
	PixDisplayByDemoImage( nCounts, pdwPixX, pdwPixY, pdwPixZ );

	nReceiveIndex = 2;
	strBuf = PixReceive(102);
	nCounts = PixConvert( strBuf, pdwPixX, pdwPixY, pdwPixZ );
	PixDisplayByDemoImage( nCounts, pdwPixX, pdwPixY, pdwPixZ );

	// Use the delete operator as follows:
	delete [] pdwPixX;
	delete [] pdwPixY;
	delete [] pdwPixZ;
}

CString CPageMCI::PixReceive( long lDataWords )
{
	CString strBuf, strText;
	long lReadSize;
	unsigned long lBytesTransferred = 0;

	strBuf = "";
	lReadSize = lDataWords *2;	// ReadSize in bytes

	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];

	DIO_StreamOpen( theApp.deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	DIO_StreamFrame( theApp.deviceIndex, lReadSize, pnFrameData, &lBytesTransferred );
	DIO_StreamClose( theApp.deviceIndex );

//	strBuf = strBuf.Left( lBytesTransferred );
	// 用于演示的图像数据
	strBuf = "0000ffffabcd345634563456"
			"811041700010811741700020" 
			"812041700030812741700040"
			"813041700050813741700060"
			"814041700070814741700080"
			"8150417000908157417000a0"
			"8160417000b08167417000c0"
			"8170417000d08177417000e0"
			"8180417000f0818741700100"
			"811041800010811741800020"
			"812041800030812741800040"
			"813041800050813741800060"
			"814041800070814741800080"
			"8150418000908157418000a0"
			"8160418000b08167418000c0"
			"8170418000d08177418000e0"
			"8180418000f0818741800100"
			"789a789a789affff0000ffff";


	(GetDlgItem( IDC_EDIT_DataMemo ) )->SetWindowText( strBuf );
//	strText.Format( "Data Received: %ld Words", lBytesTransferred /2 );
//	(GetDlgItem( IDC_STATIC_DataMemo ) )->SetWindowText( strText );

	// Use the delete operator as follows:
	delete [] pnFrameData;
	return strBuf;
}


///////////////////////////////////////////////////////////////////////////
// 像素数据转换：
// 输入:	strBuf: 字符串存储区
//			dwPixX: 用来存储X值的数组
//			dwPixY: 用来存储Y值的数组
//			dwPixZ: 用来存储Z值的数组
// 输出:	实际得到的有效数据的个数
int CPageMCI::PixConvert( const CString strBuf, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
	CString str = strBuf;
	CString strText, strTemp;

    ///////////////////////////////////////////////////////////////////////////
	// 形成输出字符串，开始出添加Start，每六个数据为一行，结尾处添加END
//	strTemp = str.Left( 24 );
//	if( strTemp != "0000ffffabcd345634563456" )
//		return;

	strText = "Start: ";
	strText += "\r\n";
	int i, nLoop = str.GetLength() /4;	// 四字符
	for( i=0; i<nLoop; i++ )
	{
		strTemp = str.Mid( i*4, 4 );
		strText += strTemp;
		strText += ", ";		// 每个数据后加入", "，以示区分
		if( (i+1) % 6 == 0 )	// 每六个数据为一行
			strText += "\r\n";
	}
	strText += "END";
	(GetDlgItem( IDC_EDIT_DataMemo ) )->SetWindowText( strText );
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
	BOOL bLastIsX, bLastIsY, bIsX, bIsY, bIsZ;
	DWORD dwData	= 0;
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwBitsOn, dwX, dwY;
	int nIndex		= 0;		// 有效的数据个数
	bLastIsX	= FALSE;		// 上一个数据是X
	bLastIsY	= FALSE;		// 上一个数据是Y
	bIsX		= FALSE;		// 当前数据是X
	bIsY		= FALSE;		// 当前数据是Y
	bIsZ		= FALSE;		// 当前数据是Z

	for( i=0; i<nLoop; i++ )
	{
		strTemp = str.Mid( i*4, 4 );
		char *bufData = strTemp.GetBuffer(5);
		sscanf(bufData, "%x", &dwData);		// 得到16进制的数据
		dwBitsOn = dwData & 0xC000;			// 得到所需的掩码位

		bIsX = dwBitsOn == dwMaskX;
		if( bIsX )							// 进一步确定是否当前数据是X值
			dwX = dwData = 0xFFF;
		if( bLastIsX )						// 上一个数据是X
		{
			bIsY = dwBitsOn == dwMaskY;
			if( bIsY )						// 进一步确定是否当前数据是Y值
				dwY = dwData & 0xFFF;
		}
		if( bLastIsY )
			bIsZ = (dwBitsOn == dwMaskZ);	//(dwBitsOn == dwMaskZ) ? TRUE : FALSE;// 进一步确定是否当前数据是Z值
		if( bIsZ )
		{
			// 填充数组
			pdwPixX[nIndex] = dwX;
			pdwPixY[nIndex] = dwY;
//			if( dwData > 255 )				// 将大于255的灰度值设置为255
//				dwData = 255;
			pdwPixZ[nIndex] = dwData & 0xFF;	// 将Z的高字节舍去，灰度值只需一个字节
			nIndex++;
		}

		// 刷新标志位
		bLastIsX	= bIsX;
		bLastIsY	= bIsY;
		bIsX		= FALSE;
		bIsY		= FALSE;
		bIsZ		= FALSE;
	}
    ///////////////////////////////////////////////////////////////////////////
	return nIndex;
}

///////////////////////////////////////////////////////////////////////////
// 像素数据转换：
// 输入:	FrameData: 数据存储区
//			dwPixX: 用来存储X值的数组
//			dwPixY: 用来存储Y值的数组
//			dwPixZ: 用来存储Z值的数组
// 输出:	实际得到的有效数据的个数
int CPageMCI::PixConvert( unsigned short* pnFrameData, long lBytesTransferred, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
	CString strText, strTextHex, strTemp;
	int i, nLoop = lBytesTransferred/2 +1;

    ///////////////////////////////////////////////////////////////////////////
	// 形成输出字符串，开始出添加Start，每六个数据为一行，结尾处添加END
//	strTemp = str.Left( 24 );
//	if( strTemp != "0000ffffabcd345634563456" )
//		return;

	strText = "Start: ";
	strText += "\r\n";
	strTextHex = "";
	for( i=1; i<nLoop; i++ )
	{
		strTemp.Format( "%ld, ", pnFrameData[i] );
		strText += strTemp;
		strTemp.Format( "%04X, ", pnFrameData[i] );
		strTextHex += strTemp;
//		if( (i+1) % 6 == 0 )
//			strText += "\r\n";
	}
//	strText += "\r\n";
//	strText += strTextHex;
//	strText += "END";
//	(GetDlgItem( IDC_EDIT_DataMemo ) )->SetWindowText( strText );
	strTextHex += "\r\n";
	strTextHex += strText;
	strTextHex += "END";
	(GetDlgItem( IDC_EDIT_DataMemo ) )->SetWindowText( strTextHex );
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
	BOOL bLastIsX, bLastIsY, bIsX, bIsY, bIsZ;
	int nPeakYCounts = 0;
	unsigned short dwData	= 0;
	CSize sz = GetSizes();
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwBitsOn;
	int nIndex		= 0;		// 有效的数据个数

	bLastIsY	= (m_dwY != 0xFFFF);
	if( bLastIsY )
		bLastIsX= FALSE;		// 上一个数据是Y
	else
		bLastIsX= (m_dwX != 0xFFFF);// 上一个数据不是Y
	bIsX		= FALSE;		// 当前数据是X
	bIsY		= FALSE;		// 当前数据是Y
	bIsZ		= FALSE;		// 当前数据是Z
	strTemp.Format( "In: m_dwX = %04X, m_dwY = %04X, data = %04X.  bLastIsX=%d, bLastIsY=%d\n", m_dwX, m_dwY, pnFrameData[0], bLastIsX, bLastIsY );
	TRACE( strTemp );

	for( i=0; i<nLoop; i++ )
	{
		dwData = pnFrameData[i];
		// 得到所需的掩码位
		// EOL & EOF所在的点都不应为有效数据，所以由原来的取0xC000改为取0xF000
		// 目前所用的判断X、Y、Z的方式具有互斥性，即是X的绝不符合Y或Z的条件，是Y的...，是Z的...
		dwBitsOn = dwData & 0xF000;

		if( bLastIsX )
		{
			bIsY = (dwBitsOn == dwMaskY);
			if( bIsY )						// 确定是否当前数据是Y值
				m_dwY = dwData & 0xFFF;
		}
		if( bLastIsY )
			bIsZ = (dwBitsOn == dwMaskZ);	// 确定是否当前数据是Z值
		if( bIsZ )
		{
			// 硬件坐标是从1开始，软件要从0开始，所以需要调整
			if( m_dwX > 0 && m_dwY > 0 )
			{
				if( !m_bPeakX && !m_bPeakY )
				{
					// 填充数组
					pdwPixX[nIndex] = m_dwX -1;
					pdwPixY[nIndex] = m_dwY -1;
					pdwPixZ[nIndex] = (dwData & 0x0FFF ) /16; 
					nIndex++;

					if( m_dwX == sz.cx && m_dwY < sz.cy )
						m_bPeakX = TRUE;
					else if( m_dwX == sz.cx && m_dwY == sz.cy )
						m_bPeakY = TRUE;

					m_dwX = 0xFFFF;
					m_dwY = 0xFFFF;
				} //if( !m_bPeakX && !m_bPeakY )
				else //if( !m_bPeakX && !m_bPeakY )
				{
					// 当在X/Y的非有效数据段(peak & fall)，要重新赋值
					m_dwX = 0xFFFF;
					m_dwY = 0xFFFF;
				} //if( !m_bPeakX && !m_bPeakY )
			} //if( m_dwX > 0 && m_dwY > 0 )
			else //if( m_dwX > 0 && m_dwY > 0 )
			{
				if( m_dwX == 0 )
					m_bPeakX = FALSE;
				if( m_dwY == 0 )
					m_bPeakY = FALSE;

				// 当在X/Y的非有效数据段(valley)，要重新赋值
				m_dwX = 0xFFFF;
				m_dwY = 0xFFFF;
			} //if( m_dwX > 0 && m_dwY > 0 )
		} //if( bIsZ )

		bIsX = (dwBitsOn == dwMaskX);
		if( bIsX )							// 确定是否当前数据是X值
			m_dwX = dwData & 0xFFF;

		// 刷新标志位
		bLastIsX	= bIsX;
		bLastIsY	= bIsY;
		bIsX		= FALSE;
		bIsY		= FALSE;
		bIsZ		= FALSE;
	} //for( i=0; i<nLoop; i++ )
    ///////////////////////////////////////////////////////////////////////////

	strTemp.Format( "return: m_dwX = %04X, m_dwY = %04X, data = %04X.  bPeakX=%d, bPeakY=%d\n", m_dwX, m_dwY, dwData, m_bPeakX, m_bPeakY );
	TRACE( strTemp );
	strTemp.Format( " \n" );
	TRACE( strTemp );
	return nIndex;
}

///////////////////////////////////////////////////////////////////////////
// 像素数据转换（专用于双探测器）
// 输入:	FrameData: 数据存储区
//			dwPixX: 用来存储X值的数组
//			dwPixY: 用来存储Y值的数组
//			dwPixZ: 用来存储Z值的数组
// 输出:	实际得到的有效数据的个数
int CPageMCI::PixConvert2( unsigned short* pnFrameData, long lBytesTransferred, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
	CString strText, strTextHex, strTemp;
	int i, nLoop = lBytesTransferred/2 +1;

    ///////////////////////////////////////////////////////////////////////////
	// 形成输出字符串，开始出添加Start，每六个数据为一行，结尾处添加END
//	strTemp = str.Left( 24 );
//	if( strTemp != "0000ffffabcd345634563456" )
//		return;

	strText = "Start: ";
	strText += "\r\n";
	strTextHex = "";
	for( i=1; i<nLoop; i++ )
	{
		strTemp.Format( "%ld, ", pnFrameData[i] );
		strText += strTemp;
		strTemp.Format( "%04X, ", pnFrameData[i] );
		strTextHex += strTemp;
//		if( (i+1) % 6 == 0 )
//			strText += "\r\n";
	}
	strText += "\r\n";
	strText += strTextHex;
	strText += "END";
	(GetDlgItem( IDC_EDIT_DataMemo ) )->SetWindowText( strText );
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
	BOOL bLastIsX, bLastIsY, bLastIsZa, bIsX, bIsY, bIsZa, bIsZb;
	unsigned short dwData	= 0;
	CSize sz = GetSizes();
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwBitsOn;
	int nOffset = sz.cx;
	int nIndex		= 0;		// 有效的数据个数

	bLastIsZa	= (m_dwZa != 0xFFFF);
	if( bLastIsZa )
	{
		bLastIsX = FALSE;
		bLastIsY = FALSE;
	}
	else	//if( bLastIsZa )
	{
		bLastIsY = (m_dwY != 0xFFFF);
		if( bLastIsY )
			bLastIsX = FALSE;
		else
			bLastIsX = (m_dwX != 0xFFFF);
	}
	bIsX		= FALSE;		// 当前数据是X
	bIsY		= FALSE;		// 当前数据是Y
	bIsZa		= FALSE;		// 当前数据是Za
	bIsZb		= FALSE;		// 当前数据是Zb
	strTemp.Format( "In: m_dwX = %04X, m_dwY = %04X, m_dwZa = %04X, data = %04X.  bLastIsX=%d, bLastIsY=%d\n",
					m_dwX, m_dwY, m_dwZa, pnFrameData[0], bLastIsX, bLastIsY );
	TRACE( strTemp );

	for( i=0; i<nLoop; i++ )
	{
		dwData = pnFrameData[i];
		// 得到所需的掩码位
		dwBitsOn = dwData & 0xF000;

		if( bLastIsX )						// 上一个数据是X
		{
			bIsY = (dwBitsOn == dwMaskY);
			if( bIsY )						// 进一步确定是否当前数据是Y值
				m_dwY = dwData & 0xFFF;
		}
		if( bLastIsY )
		{
			bIsZa = (dwBitsOn == dwMaskZ);	// 进一步确定是否当前数据是Z值
			if( bIsZa )
				m_dwZa = dwData & 0xFFF;
		}
		if( bLastIsZa )
			bIsZb = (dwBitsOn == dwMaskZ);

		if( bLastIsZa )
		{
			// 硬件坐标是从1开始，软件要从0开始，所以需要调整
			if( m_dwX > 0 && m_dwY > 0 )
			{
				if( !m_bPeakX && !m_bPeakY )
				{
					// X,Y,Za都找到了，填充数组
					// 首先是Za
					pdwPixX[nIndex] = m_dwX -1;
					pdwPixY[nIndex] = m_dwY -1;
					pdwPixZ[nIndex] = m_dwZa /16;
					nIndex++;
					// 如果Zb也找到了
					if( bIsZb )
					{
						pdwPixX[nIndex] = m_dwX -1 + nOffset;
						pdwPixY[nIndex] = m_dwY -1;
						pdwPixZ[nIndex] = (dwData & 0x0FFF) /16;
						nIndex++;
					}

					if( m_dwX == sz.cx && m_dwY < sz.cy )
						m_bPeakX = TRUE;
					else if( m_dwX == sz.cx && m_dwY == sz.cy )
						m_bPeakY = TRUE;

					m_dwX	= 0xFFFF;
					m_dwY	= 0xFFFF;
					m_dwZa	= 0xFFFF;
				} //if( !m_bPeakX && !m_bPeakY )
				else //if( !m_bPeakX && !m_bPeakY )
				{
					m_dwX	= 0xFFFF;
					m_dwY	= 0xFFFF;
					m_dwZa	= 0xFFFF;
					bIsZa	= FALSE;
				} //if( !m_bPeakX && !m_bPeakY )
			} //if( m_dwX > 0 && m_dwY > 0 )
			else //if( m_dwX > 0 && m_dwY > 0 )
			{
				if( m_dwX == 0 )
					m_bPeakX = FALSE;
				if( m_dwY == 0 )
					m_bPeakY = FALSE;

				m_dwX	= 0xFFFF;
				m_dwY	= 0xFFFF;
				m_dwZa	= 0xFFFF;
				bIsZa	= FALSE;
			} //if( m_dwX > 0 && m_dwY > 0 )
		} //if( bLastIsZa || bIsZb )

		bIsX = (dwBitsOn == dwMaskX);
		if( bIsX )							// 进一步确定是否当前数据是X值
			m_dwX = dwData & 0xFFF;

		// 刷新标志位
		bLastIsX	= bIsX;
		bLastIsY	= bIsY;
		bLastIsZa	= bIsZa;
		bIsX		= FALSE;
		bIsY		= FALSE;
		bIsZa		= FALSE;
		bIsZb		= FALSE;
	} //for( i=0; i<nLoop; i++ )
    ///////////////////////////////////////////////////////////////////////////

	strTemp.Format( "return: m_dwX = %04X, m_dwY = %04X, data = %04X.  bPeakX=%d, bPeakY=%d\n", m_dwX, m_dwY, dwData, m_bPeakX, m_bPeakY );
	strTemp.Format( "return: m_dwX = %04X, m_dwY = %04X, m_dwZa = %04X, data = %04X.  bPeakX=%d, bPeakY=%d\n",
					m_dwX, m_dwY, m_dwZa, dwData, m_bPeakX, m_bPeakY );
	TRACE( strTemp );
	strTemp.Format( " \n" );
	TRACE( strTemp );
	return nIndex;
}

///////////////////////////////////////////////////////////////////////////
// 像素数据显示（专用于DemoImage）：
// 输入:	nCounts: 数据个数
//			dwPixX: 存储了X值的数组
//			dwPixY: 存储了Y值的数组
//			dwPixZ: 存储了Z值的数组
// 输出:	无
void CPageMCI::PixDisplayByDemoImage( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
/*    ///////////////////////////////////////////////////////////////////////////
	// 获取灰度Z值的最大值
	double dMaxValue = 0;
	for( int j=0; j<nCounts; j++ )
	{
		if( pdwPixZ[j] > dMaxValue )
			dMaxValue = pdwPixZ[j];
	}
	// 如果Z最大值大于255，则对Z进行压缩调整
	if( dMaxValue > 255 )
	{
		for( j=0; j<nCounts; j++ )
			pdwPixZ[j] = pdwPixZ[j] * 255 / dMaxValue;
	}
    ///////////////////////////////////////////////////////////////////////////
*/
    ///////////////////////////////////////////////////////////////////////////
	CDC* pDC = GetDC();

	CPen pen, *poldpen;
	for( int i=0; i<32; i++ )
	{
		pen.CreatePen( PS_SOLID, 5, RGB(pdwPixZ[i], pdwPixZ[i], pdwPixZ[i]) );
		poldpen = pDC->SelectObject( &pen );
		pDC->MoveTo( pdwPixX[i] +550,		nReceiveIndex *5 +pdwPixY[i] -220 );
		pDC->LineTo( pdwPixX[i] +550 +1,	nReceiveIndex *5 +pdwPixY[i] -220 );
		pDC->SelectObject( poldpen );
		pen.DeleteObject();
	}
    ///////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////
// 像素数据显示（专用于Receive，XYZ模式，连续显示）：
// 输入:	nCounts: 数据个数
//			dwPixX: 存储了X值的数组
//			dwPixY: 存储了Y值的数组
//			dwPixZ: 存储了Z值的数组
// 输出:	无
void CPageMCI::PixDisplayByReceiveSerial( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
    ///////////////////////////////////////////////////////////////////////////
	CDC* pDC = GetDC();

	CPen pen, *poldpen;
	for( int i=0; i<nCounts; i++ )
	{
		pen.CreatePen( PS_SOLID, 1, RGB(pdwPixZ[i], pdwPixZ[i], pdwPixZ[i]) );
		poldpen = pDC->SelectObject( &pen );
//			pDC->MoveTo( pdwPixX[i] *nWidth,			pdwPixY[i] *nWidth );
//			pDC->LineTo( pdwPixX[i] *nWidth +nLength,	pdwPixY[i] *nWidth );
		pDC->MoveTo( pdwPixX[i],	pdwPixY[i] );
		pDC->LineTo( pdwPixX[i] +1,	pdwPixY[i] );
		pDC->SelectObject( poldpen );
		pen.DeleteObject();
	}
    ///////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////
// 像素数据显示（专用于Receive，XYZ模式，延迟显示）：
// 输入:	nCounts: 数据个数
//			dwPixX: 存储了X值的数组
//			dwPixY: 存储了Y值的数组
//			dwPixZ: 存储了Z值的数组
// 输出:	无
void CPageMCI::PixDisplayByReceiveDelay( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
	if( nCounts == 0 )
		return;

    ///////////////////////////////////////////////////////////////////////////
	CDC* pDC = m_dlgDisplay.GetDC();

	CPen pen, *poldpen;
	CSize sz = GetSizes();
	int nWidth, nLength;
	GetPenParam( nWidth, nLength );

	int nIndex = 0;
	int nOneFrame = 0;
	CString str;
	int nIndexText = 0;

	CRect rc;
	m_dlgDisplay.GetClientRect( &rc );
	rc.right = rc.bottom;

	str.Format( "nCount = %d", nCounts );
	Message( (LPCTSTR)str );

	do
	{
		nIndex = 0;
		for( int i=nOneFrame; i<nCounts; i++ )
		{
			if( i== nOneFrame )
			{
				str.Format( "Fx = %02X, Fy = %02X; ", pdwPixX[i], pdwPixY[i] );
				Message( (LPCTSTR)str );
				nIndexText++;
			}
			if( (pdwPixX[i] != m_nDisX && pdwPixX[i] != m_nDisX+1)
				|| (pdwPixY[i] != m_nDisY && pdwPixY[i] != m_nDisY+1) )
			{
				// 坐标不符合预期增长，使人眼辨别
				Sleep(500);
				str.Format( "Lxyz = %02X,%02X,%02X", pdwPixX[i-1], pdwPixY[i-1], pdwPixZ[i-1] );
				Message( (LPCTSTR)str );
				str.Format( "xyz   = %02X,%02X,%02X ", pdwPixX[i], pdwPixY[i], pdwPixZ[i] );
				str += "\r\n";
				Message( (LPCTSTR)str );
				TRACE( "wrong X/Y\n" );
				// 刷新一次屏幕，以便能清楚以后的数据
				pDC->FillRect( rc, &CBrush(g_crImageBkg) );
			}
			m_nDisX = pdwPixX[i];
			m_nDisY = pdwPixY[i];
			if( m_nDisY +1 > sz.cy -1 && m_nDisX == sz.cx -1)
				m_nDisY = 0;
			if( m_nDisX +1 > sz.cx -1 )
				m_nDisX = 0;

			pen.CreatePen( PS_SOLID, nWidth, RGB(pdwPixZ[i], pdwPixZ[i], pdwPixZ[i]) );
			poldpen = pDC->SelectObject( &pen );
			pDC->MoveTo( pdwPixX[i] *nWidth,			pdwPixY[i] *nWidth );
			pDC->LineTo( pdwPixX[i] *nWidth +nLength,	pdwPixY[i] *nWidth );
			pDC->SelectObject( poldpen );
			pen.DeleteObject();

			if( pdwPixX[i] == sz.cx-1 && pdwPixY[i] != sz.cy-1 )
				Sleep(100);
			else if( pdwPixX[i] == sz.cx-1 && pdwPixY[i] == sz.cy-1 )
			{
				// 已经是最后一行最后一点
				nIndex		= i+1;
				nOneFrame	= i+1;
				str.Format( "Ex = %02X, Ey = %02X; ", pdwPixX[i], pdwPixY[i] );
				Message( (LPCTSTR)str );
				nIndexText++;
				// 每采集完一帧图像就延迟一下，使人眼辨别
				Sleep(500);
				// 刷新一次屏幕，以便能清楚看到第二帧的数据
				pDC->FillRect( rc, &CBrush(g_crImageBkg) );
				break;
			}
		}
	} while( nIndex != 0 );

 	str.Format( "Ty = %02X, Ty = %02X; ", pdwPixX[nCounts-1], pdwPixY[nCounts-1] );
	str += "\r\n";
	str += "\r\n";
	Message( (LPCTSTR)str );
   ///////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////
// 像素数据显示（专用于Receive，XYZ duel模式）：
// 输入:	nCounts: 数据个数
//			dwPixX: 存储了X值的数组
//			dwPixY: 存储了Y值的数组
//			dwPixZ: 存储了Z值的数组
// 输出:	无
void CPageMCI::PixDisplayByReceive2( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
	if( nCounts == 0 )
		return;

    ///////////////////////////////////////////////////////////////////////////
	CDC* pDC = m_dlgDisplay.GetDC();

	CPen pen, *poldpen;
	CSize sz = GetSizes();
	int nWidth, nLength;
	GetPenParam( nWidth, nLength );

	int nIndex = 0;
	int nOneFrame = 0;
	CString str;
	int nIndexText = 0;

	CRect rc;
	m_dlgDisplay.GetClientRect( &rc );
	rc.right = rc.bottom;

	str.Format( "nCount = %d", nCounts );
	Message( (LPCTSTR)str );

	do
	{
		nIndex = 0;
		for( int i=nOneFrame; i<nCounts; i++ )
		{
			if( i== nOneFrame )
			{
				str.Format( "Fx = %02X, Fy = %02X; ", pdwPixX[i], pdwPixY[i] );
				Message( (LPCTSTR)str );
				nIndexText++;
			}
//			if( (pdwPixX[i] != m_nDisX && pdwPixX[i] != m_nDisX+1)
//				|| (pdwPixY[i] != m_nDisY && pdwPixY[i] != m_nDisY+1) )
			if( pdwPixY[i] != m_nDisY && pdwPixY[i] != m_nDisY+1 )
			{
				// 坐标不符合预期增长，使人眼辨别
				Sleep(500);
				str.Format( "Lxyz = %02X,%02X,%02X", pdwPixX[i-1], pdwPixY[i-1], pdwPixZ[i-1] );
				Message( (LPCTSTR)str );
				str.Format( "xyz   = %02X,%02X,%02X ", pdwPixX[i], pdwPixY[i], pdwPixZ[i] );
				str += "\r\n";
				Message( (LPCTSTR)str );
				TRACE( "wrong X/Y\n" );
				// 刷新一次屏幕，以便能清楚以后的数据
				pDC->FillRect( rc, &CBrush(g_crImageBkg) );
			}
			m_nDisX = pdwPixX[i];
			m_nDisY = pdwPixY[i];
			if( m_nDisY +1 > sz.cy -1 && m_nDisX == sz.cx +sz.cx -1 )
				m_nDisY = 0;
//			if( m_nDisX +1 > sz.cx -1 )
//				m_nDisX = 0;

			pen.CreatePen( PS_SOLID, nWidth, RGB(pdwPixZ[i], pdwPixZ[i], pdwPixZ[i]) );
			poldpen = pDC->SelectObject( &pen );
			pDC->MoveTo( pdwPixX[i] *nWidth,			pdwPixY[i] *nWidth );
			pDC->LineTo( pdwPixX[i] *nWidth +nLength,	pdwPixY[i] *nWidth );
			pDC->SelectObject( poldpen );
			pen.DeleteObject();

			if( pdwPixX[i] == sz.cx +sz.cx-1 && pdwPixY[i] != sz.cy-1)
				Sleep(100);
			else if( pdwPixX[i] == sz.cx +sz.cx -1 && pdwPixY[i] == sz.cy-1 )
			{
				// 已经是最后一行最后一点
				nIndex		= i+1;
				nOneFrame	= i+1;
				str.Format( "Ex = %02X, Ey = %02X; ", pdwPixX[i], pdwPixY[i] );
				Message( (LPCTSTR)str );
				nIndexText++;
				// 每采集完一帧图像就延迟一下，使人眼辨别
				Sleep(500);
				// 刷新一次屏幕，以便能清楚看到第二帧的数据
				pDC->FillRect( rc, &CBrush(g_crImageBkg) );
				break;
			}
		}
	} while( nIndex != 0 );

 	str.Format( "Ty = %02X, Ty = %02X; ", pdwPixX[nCounts-1], pdwPixY[nCounts-1] );
	str += "\r\n";
	str += "\r\n";
	Message( (LPCTSTR)str );
   ///////////////////////////////////////////////////////////////////////////
}

void CPageMCI::Message(LPCTSTR lpszMessage)
{
	CEdit* pedit = (CEdit*)(m_dlgDisplay.GetDlgItem( IDC_EDIT_List) );
	if( pedit == NULL )
		return;

	CString csMessage = CString(lpszMessage);
	csMessage += "\r\n";
	int len = pedit->GetWindowTextLength();
	pedit->SetSel(len,len);
	pedit->ReplaceSel( (LPCTSTR)csMessage );
}

void CPageMCI::GetPenParam( int& nWidth, int& nLength )
{
	nWidth = 1;
	nLength = 1;

	CSize sz = GetSizes();
	switch( sz.cx )
	{
	case	16:
		nWidth	= 32;
		break;
	case	32:
		nWidth	= 16;
		break;
	case	64:
		nWidth	= 8;
		break;
	case	128:
		nWidth	= 4;
		break;
	case	256:
		nWidth	= 2;
		break;
	case	512:
	case	1024:
	default:
		nWidth	= 1;
		break;
	}
	if( m_bDetMode )
	{
		nWidth /= 2;
		if( nWidth < 1 )
			nWidth = 1;
	}
	if( nWidth < 4 )
		nLength = 1;
	else
		nLength = 2;
}

///////////////////////////////////////////////////////////////////////////
// 像素数据转换：ZZZ模式
// 输入:	FrameData: 数据存储区
//			dwPixX: 用来存储X值的数组
//			dwPixY: 用来存储Y值的数组
//			dwPixZ: 用来存储Z值的数组
// 输出:	实际得到的有效数据的个数
long CPageMCI::PixConvertZZZ( unsigned short* pnFrameData, long lBytesTransferred,
								  DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
	CString strText, strTextHex, strTemp;
	int i, nLoop = lBytesTransferred/2 +1;

    ///////////////////////////////////////////////////////////////////////////
	// 形成输出字符串，开始出添加Start，每六个数据为一行，结尾处添加END
//	strTemp = str.Left( 24 );
//	if( strTemp != "0000ffffabcd345634563456" )
//		return;

	strText = "Start: ";
	strText += "\r\n";
	strTextHex = "";
	for( i=1; i<nLoop; i++ )
	{
		strTemp.Format( "%ld, ", pnFrameData[i] );
		strText += strTemp;
		strTemp.Format( "%04X, ", pnFrameData[i] );
		strTextHex += strTemp;
//		if( (i+1) % 6 == 0 )
//			strText += "\r\n";
	}
	strText += "\r\n";
	strText += strTextHex;
	strText += "END";
	(GetDlgItem( IDC_EDIT_DataMemo ) )->SetWindowText( strText );
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
	BOOL bIsBOL, bIsBOF;
	unsigned short dwData	= 0;
	DWORD dwMaskBOL	= 0x1000;	// EOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// EOF 的掩码
	DWORD dwMask	= 0x2000;	// 最后一行所有数据的掩码
	DWORD dwBitsOn;
	CSize sz = GetSizes();
	int nLineCounts	= ValleyX +sz.cx +PeakX +FallX;
	int nFrameCounts= ValleyY +sz.cy +PeakY +FallY;
	bIsBOL		= FALSE;		// 当前数据是EOL
	bIsBOF		= FALSE;		// 当前数据是EOF
	int nIndex		= 0;		// 有效的数据个数

	for( i=0; i<nLoop; i++ )
	{
		dwData = pnFrameData[i];			// 得到16进制的数据
		if( dwData == 0x3456 )
			continue;

		dwBitsOn = dwData & 0x3000;			// 得到所需的掩码位

		bIsBOL = (dwBitsOn == dwMaskBOL);
		if( bIsBOL )
		{
			m_nX = 0;
			m_nY++;
			CheckIndexs( nLineCounts, nFrameCounts, m_nX, m_nY );
		}
		else
		{
			bIsBOF = (dwBitsOn == dwMaskBOF);
			if( bIsBOF )
			{
				m_nX		= 0;
				m_nY		= 0;
				m_nIndexX	= 0;
				m_nIndexY	= 0;
			}
			else
			{
				m_nX++;
//				CheckIndexs( nLineCounts, nFrameCounts, m_nX, m_nY );
			}
		}

//		if( m_nY > ValleyY && m_nY < ValleyY +sz.cy +1
//			&& m_nX > ValleyX && m_nX < ValleyX +sz.cx +1 )
		if( m_nY > 0 && m_nY < sz.cy +1
			&& m_nX > 0 && m_nX < sz.cx +1 )
		{
			// 填充数组
			pdwPixX[nIndex] = m_nIndexX;
			pdwPixY[nIndex] = m_nIndexY;
			pdwPixZ[nIndex] = ( dwData & 0x0FFF ) /16;
			nIndex++;
			m_nIndexX++;
			CheckIndexs( sz.cx, sz.cy, m_nIndexX, m_nIndexY );
		}
	}
    ///////////////////////////////////////////////////////////////////////////
	return nIndex;//(sz.cx * sz.cy);
}

///////////////////////////////////////////////////////////////////////////
// 像素数据转换：ZZZ模式
// 输入:	FrameData: 数据存储区
//			dwPixX: 用来存储X值的数组
//			dwPixY: 用来存储Y值的数组
//			dwPixZ: 用来存储Z值的数组
// 输出:	实际得到的有效数据的个数
long CPageMCI::PixConvertZZZ2( unsigned short* pnFrameData, long lBytesTransferred,
								  DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
	CString strText, strTextHex, strTemp;
	int i, nLoop = lBytesTransferred/2 +1;

    ///////////////////////////////////////////////////////////////////////////
	// 形成输出字符串，开始出添加Start，每六个数据为一行，结尾处添加END
//	strTemp = str.Left( 24 );
//	if( strTemp != "0000ffffabcd345634563456" )
//		return;

	strText = "Start: ";
	strText += "\r\n";
	strTextHex = "";
	for( i=1; i<nLoop; i++ )
	{
		strTemp.Format( "%ld, ", pnFrameData[i] );
		strText += strTemp;
		strTemp.Format( "%04X, ", pnFrameData[i] );
		strTextHex += strTemp;
//		if( (i+1) % 6 == 0 )
//			strText += "\r\n";
	}
	strText += "\r\n";
	strText += strTextHex;
	strText += "END";
	(GetDlgItem( IDC_EDIT_DataMemo ) )->SetWindowText( strText );
    ///////////////////////////////////////////////////////////////////////////
/*
    ///////////////////////////////////////////////////////////////////////////
	BOOL bIsEOL, bIsEOF;
	unsigned short dwData	= 0;
	DWORD dwMaskEOL	= 0x1000;	// EOL 的掩码
	DWORD dwMaskEOF	= 0x3000;	// EOF 的掩码
	DWORD dwMask	= 0x2000;	// 最后一行所有数据的掩码
	DWORD dwBitsOn;
	CSize sz = GetSizes();
	int nLineCounts	= ValleyX +sz.cx +PeakX +FallX;
	int nFrameCounts= ValleyY +sz.cy +PeakY +FallY;
	bIsEOL		= FALSE;		// 当前数据是EOL
	bIsEOF		= FALSE;		// 当前数据是EOF
	int nIndex		= 0;		// 有效的数据个数

	for( i=0; i<nLoop; i++ )
	{
		dwData = pnFrameData[i];			// 得到16进制的数据
		if( dwData == 0x3456 )
			continue;

		dwBitsOn = dwData & 0x3000;			// 得到所需的掩码位

		bIsEOL = (dwBitsOn == dwMaskEOL);
		if( bIsEOL )
		{
			if( m_nY > ValleyY -1 && m_nY < ValleyY +sz.cy
				&& m_nX > ValleyX -1 && m_nX < ValleyX +sz.cx )
			{
				m_nIndexX = 0;
				m_nIndexY++;
				if( m_nIndexY > sz.cy -1 )
					m_nY = 0;
			}
			m_nX = 0;
			m_nY++;
			if( m_nY > nFrameCounts -1 )
				m_nY = 0;
		}
		else
		{
			bIsEOF = (dwBitsOn == dwMaskEOF);
			if( bIsEOF )
			{
				m_nX		= 0;
				m_nY		= 0;
				m_nIndexX	= 0;
				m_nIndexY	= 0;
			}
			else
			{
				m_nX++;
				CheckIndexs( nLineCounts, nFrameCounts, m_nX, m_nY );
			}
		}

		if( m_nY > ValleyY && m_nY < ValleyY +sz.cy +1
			&& m_nX > ValleyX && m_nX < ValleyX +sz.cx +1 )
		{
			// 填充数组
			pdwPixX[nIndex] = m_nIndexX;
			pdwPixY[nIndex] = m_nIndexY;
			pdwPixZ[nIndex] = ( dwData & 0x0FF0 ) >> 4;
			nIndex++;

			i++;
			if( !m_bDetMode )
				continue;

			// Duel
			if( i == nLoop )
				break;
			dwData = pnFrameData[i];			// 得到16进制的数据
			pdwPixX[nIndex] = m_nIndexX +sz.cx;
			pdwPixY[nIndex] = m_nIndexY;
			pdwPixZ[nIndex] = ( dwData & 0x0FF0 ) >> 4;
			nIndex++;

			m_nIndexX++;
			CheckIndexs( sz.cx, sz.cy, m_nIndexX, m_nIndexY );
		}
	}
    ///////////////////////////////////////////////////////////////////////////
*/
    ///////////////////////////////////////////////////////////////////////////
	BOOL bIsEOL, bIsEOF;
	unsigned short dwData	= 0;
	DWORD dwMaskEOL	= 0x1000;	// EOL 的掩码
	DWORD dwMaskEOF	= 0x3000;	// EOF 的掩码
	DWORD dwMask	= 0x2000;	// 最后一行所有数据的掩码
	DWORD dwBitsOn;
	CSize sz = GetSizes();
	int nLineCounts	= ValleyX +sz.cx +PeakX +FallX;
	int nFrameCounts= ValleyY +sz.cy +PeakY +FallY;
	bIsEOL			= FALSE;	// 当前数据是EOL
	bIsEOF			= FALSE;	// 当前数据是EOF
	int nIndex		= 0;		// 有效的数据个数
	BOOL bFound		= FALSE;
	BOOL bFoundEOL	= FALSE;
	BOOL bFoundEOF	= FALSE;
	BOOL bLastIsEO	= FALSE;

	for( i=0; i<nLoop; i++ )
	{
		dwData = pnFrameData[i];			// 得到16进制的数据
		if( dwData == 0x3456 )
			continue;

		dwBitsOn = dwData & 0x3000;			// 得到所需的掩码位

		// 找到EOL或者EOF后，标记
		// 再找到紧跟着的一个既非EOL也非EOF的数（非标志数）
		// 则说明到了VALLEY, 则更新X & Y
		bIsEOL = (dwBitsOn == dwMaskEOL);
		bIsEOF = (dwBitsOn == dwMaskEOF);

		if( bIsEOL || bIsEOF )
		{ // 更新标志，继续
			bFoundEOL = bIsEOL;
			bFoundEOF = bIsEOF;
			continue;
		}

		// 这一次是一个非标志数
		// 则说明找到了VALLEY，要更新X & Y
		if( bFoundEOL )
		{
			// 之前找到的是EOL
			if( m_nY > ValleyY -1 && m_nY < ValleyY +sz.cy
				&& m_nX > ValleyX -1 && m_nX < ValleyX +sz.cx )
			{
				m_nIndexX = 0;
				m_nIndexY++;
				if( m_nIndexY > sz.cy -1 )
					m_nY = 0;
			}
			m_nX = 0;
			m_nY++;
			if( m_nY > nFrameCounts -1 )
				m_nY = 0;
			bFoundEOL = FALSE;
		}
		else
		{
//			bIsEOF = (dwBitsOn == dwMaskEOF);
			if( bFoundEOF )
			{
				// 之前找到的是EOF
				m_nX		= 0;
				m_nY		= 0;
				m_nIndexX	= 0;
				m_nIndexY	= 0;
				bFoundEOF = FALSE;
			}
			else
			{
				m_nX++;
				CheckIndexs( nLineCounts, nFrameCounts, m_nX, m_nY );
			}
		}

		if( m_nY > ValleyY && m_nY < ValleyY +sz.cy +1
			&& m_nX > ValleyX && m_nX < ValleyX +sz.cx +1 )
		{
			// 填充数组
			pdwPixX[nIndex] = m_nIndexX;
			pdwPixY[nIndex] = m_nIndexY;
			pdwPixZ[nIndex] = ( dwData & 0x0FF0 ) >> 4;
			nIndex++;

			i++;
			if( !m_bDetMode )
				continue;

			// Duel
			if( i == nLoop )
				break;
			dwData = pnFrameData[i];			// 得到16进制的数据
			pdwPixX[nIndex] = m_nIndexX +sz.cx;
			pdwPixY[nIndex] = m_nIndexY;
			pdwPixZ[nIndex] = ( dwData & 0x0FF0 ) >> 4;
			nIndex++;

			m_nIndexX++;
			CheckIndexs( sz.cx, sz.cy, m_nIndexX, m_nIndexY );
		}
	}
    ///////////////////////////////////////////////////////////////////////////
	return nIndex;//(sz.cx * sz.cy);
}

void CPageMCI::CheckIndexs( int nLineCounts, int nFrameCounts, int& nX, int& nY )
{
	if( nX > nLineCounts -1 )
	{
		nX = 0;
		nY++;
	}
	if( nY > nFrameCounts -1 )
	{
		nX = 0;
		nY = 0;
	}
}

///////////////////////////////////////////////////////////////////////////
// 像素数据显示（专用于Receive，ZZZ模式）：
// 输入:	nCounts: 数据个数
//			dwPixX: 存储了X值的数组
//			dwPixY: 存储了Y值的数组
//			dwPixZ: 存储了Z值的数组
// 输出:	无
void CPageMCI::PixDisplayByReceiveZZZ( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ )
{
	if( nCounts == 0 )
		return;

    ///////////////////////////////////////////////////////////////////////////
	CDC* pDC = m_dlgDisplay.GetDC();

	CPen pen, *poldpen;
	CSize sz = GetSizes();
	int nWidth, nLength;
	GetPenParam( nWidth, nLength );

	int nIndex = 0;
	int nOneFrame = 0;
	CString str;
	int nIndexText = 0;

	CRect rc;
	m_dlgDisplay.GetClientRect( &rc );
	rc.right = rc.bottom;

	str.Format( "nCount = %d", nCounts );
	Message( (LPCTSTR)str );

	do
	{
		nIndex = 0;
		for( int i=nOneFrame; i<nCounts; i++ )
		{
			if( i== nOneFrame )
			{
				str.Format( "Fx = %02X, Fy = %02X; ", pdwPixX[i], pdwPixY[i] );
				Message( (LPCTSTR)str );
				nIndexText++;
			}
			if( (pdwPixX[i] != m_nDisX && pdwPixX[i] != m_nDisX+1)
				|| (pdwPixY[i] != m_nDisY && pdwPixY[i] != m_nDisY+1) )
			{
				// 坐标不符合预期增长，使人眼辨别
				Sleep(500);
				str.Format( "Lxyz = %02X,%02X,%02X", pdwPixX[i-1], pdwPixY[i-1], pdwPixZ[i-1] );
				Message( (LPCTSTR)str );
				str.Format( "xyz   = %02X,%02X,%02X ", pdwPixX[i], pdwPixY[i], pdwPixZ[i] );
				str += "\r\n";
				Message( (LPCTSTR)str );
				TRACE( "wrong X/Y\n" );
				// 刷新一次屏幕，以便能清楚以后的数据
				pDC->FillRect( rc, &CBrush(g_crImageBkg) );
			}
			m_nDisX = pdwPixX[i];
			m_nDisY = pdwPixY[i];
			if( m_nDisY +1 > sz.cy -1 && m_nDisX == sz.cx -1)
				m_nDisY = 0;
			if( m_nDisX +1 > sz.cx -1 )
				m_nDisX = 0;

			pen.CreatePen( PS_SOLID, nWidth, RGB(pdwPixZ[i], pdwPixZ[i], pdwPixZ[i]) );
			poldpen = pDC->SelectObject( &pen );
			pDC->MoveTo( pdwPixX[i] *nWidth,			pdwPixY[i] *nWidth );
			pDC->LineTo( pdwPixX[i] *nWidth +nLength,	pdwPixY[i] *nWidth );
			pDC->SelectObject( poldpen );
			pen.DeleteObject();

			if( pdwPixX[i] == sz.cx-1 && pdwPixY[i] != sz.cy-1 )
				Sleep(100);
			else if( pdwPixX[i] == sz.cx-1 && pdwPixY[i] == sz.cy-1 )
			{
				// 已经是最后一行最后一点
				nIndex		= i+1;
				nOneFrame	= i+1;
				str.Format( "Ex = %02X, Ey = %02X; ", pdwPixX[i], pdwPixY[i] );
				Message( (LPCTSTR)str );
				nIndexText++;
				// 每采集完一帧图像就延迟一下，使人眼辨别
				Sleep(500);
				// 刷新一次屏幕，以便能清楚看到第二帧的数据
				pDC->FillRect( rc, &CBrush(g_crImageBkg) );
				break;
			}
		}
	} while( nIndex != 0 );

 	str.Format( "Ty = %02X, Ty = %02X; ", pdwPixX[nCounts-1], pdwPixY[nCounts-1] );
	str += "\r\n";
	str += "\r\n";
	Message( (LPCTSTR)str );
}









void CPageMCI::OnDetectorModeChanged( UINT nID )
{
	m_btnDetSingle.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnDetDual.SetColor( ::GetSysColor(COLOR_3DFACE) );
//	Invalidate( FALSE );

	m_bDetMode = ((nID - IDC_RADIO_DetSingle) == 1);
	if( !m_bDetMode )
	{
		m_btnDetSingle.SetColor( g_crEmphasis2 );
		theApp.WriteToBoard( USB_SingleDet, FALSE, 0 );
	}
	else
	{
		m_btnDetDual.SetColor( g_crEmphasis3 );
		theApp.WriteToBoard( USB_DuelDet, FALSE, 0 );
	}
}

void CPageMCI::OnVideoModeChanged( UINT nID )
{
	m_btnVideoXYZ.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnVideoRaster.SetColor( ::GetSysColor(COLOR_3DFACE) );
//	Invalidate( FALSE );

	m_bVideoMode = ((nID - IDC_RADIO_acqXYZ) == 1);
	if( !m_bVideoMode )
	{
		m_btnVideoXYZ.SetColor( g_crEmphasis2 );
		theApp.WriteToBoard( USB_VideoXYZ, FALSE, 0 );
	}
	else
	{
		m_btnVideoRaster.SetColor( g_crEmphasis3 );
		theApp.WriteToBoard( USB_VideoZZZ, FALSE, 0 );
	}
}

void CPageMCI::OnDisplayModeChanged( UINT nID )
{
	m_btnDispDelay.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnDispSerial.SetColor( ::GetSysColor(COLOR_3DFACE) );
//	Invalidate( FALSE );

	m_bDispMode = ((nID - IDC_RADIO_Display_Serial) == 0);
	if( m_bDispMode )
	{
		m_btnDispSerial.SetColor( g_crEmphasis2 );
	}
	else
	{
		m_btnDispDelay.SetColor( g_crEmphasis3 );
	}
}

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

void CPageMCI::OnScanModeNormal() 
{
	theApp.WriteToBoard( USB_ScanModeNormal, FALSE, 0 );
	m_btnModeNormal.SetColor( g_crEmphasis1 );
	m_btnModeLineHorizontal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineVertical.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeSpot.SetColor( ::GetSysColor(COLOR_3DFACE) );
	Invalidate( FALSE );
}

void CPageMCI::OnScanModeLineHorizontal() 
{
	theApp.WriteToBoard( USB_ScanModeLineHorizontal, FALSE, 0 );
	m_btnModeNormal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineHorizontal.SetColor( g_crEmphasis2 );
	m_btnModeLineVertical.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeSpot.SetColor( ::GetSysColor(COLOR_3DFACE) );
	Invalidate( FALSE );
}

void CPageMCI::OnScanModeLineVertical() 
{
	theApp.WriteToBoard( USB_ScanModeLineVertical, FALSE, 0 );
	m_btnModeNormal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineHorizontal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineVertical.SetColor( g_crEmphasis2 );
	m_btnModeSpot.SetColor( ::GetSysColor(COLOR_3DFACE) );
	Invalidate( FALSE );
}

void CPageMCI::OnScanModeSpot() 
{
//	theApp.WriteToBoard( USB_ScanModeLineHorizontal, FALSE, 0 );
	theApp.WriteToBoard( USB_ScanModeSpot, FALSE, 0 );
	m_btnModeNormal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineHorizontal.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeLineVertical.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnModeSpot.SetColor( g_crEmphasis3 );
	Invalidate( FALSE );
}

void CPageMCI::OnFreeze() 
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

void CPageMCI::OnFreezeX() 
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

void CPageMCI::OnFreezeY() 
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

void CPageMCI::OnScanRotation() 
{
	// TODO: Add your control notification handler code here
	m_bRotate = !m_bRotate;
	if( m_bRotate )
	{
		m_btnRotate.SetColor( g_crON_Test );
		m_btnRotate.SetWindowText( "开" );
		theApp.WriteToBoard( USB_RotationON, FALSE, 0 );
	}
	else
	{
		m_btnRotate.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnRotate.SetWindowText( "关" );
		theApp.WriteToBoard( USB_RotationOFF, FALSE, 0 );
	}
/*
	1. Set Cosine – 0x1A, with 16bit operand, a signed integer for Cosine(theta) * 32767 (or x7FFF).
					Theta can be anywhere between 0 to 360 degrees.
	2. Set Sine – 0x1B, with 16bit operand, a signed integer for Sine(theta) * 32767 (or x7FFF).
	3. Update Rotation – 0x17, no operand.
	4. Scan Rotation ON – 0x19, no operand.
	5. Scan Rotation OFF – 0x18.
*/
}

void CPageMCI::OnScanRotationUpdate() 
{
	CString str;
	( GetDlgItem( IDC_EDIT_Rdegree ) )->GetWindowText( str );
	double dRadian	= atof( str ) * PI / 180;	// degree to radian
	WORD wSin		= (WORD)( sin( dRadian ) * 0x7FFF );
	WORD wCos		= (WORD)( cos( dRadian ) * 0x7FFF );

	theApp.WriteToBoard( USB_RotationSetCos, TRUE, wCos );
	theApp.WriteToBoard( USB_RotationSetSin, TRUE, wSin );
	theApp.WriteToBoard( USB_UpdateRotation, FALSE, 0 );
}

void CPageMCI::OnReadINTSourceRegister() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	BYTE bOP1 = 0x0;
	BYTE bOP2 = 0x0;
	switch( m_nBoardID )
	{
	case	0:
	case	1:
		break;
	case	2:
		bOP1	= USB_CID_VAC_INT_Source_Register;
		bOP2	= USB_TM_VAC_Read_16;
		break;
	case	3:
		bOP1	= USB_CID_SPR_INT_Source_Register;
		bOP2	= USB_TM_SPR_Read_08;
		break;
	}
	if( bOP1 == 0x0 )
		return;

	WORD wData1, wData2;
	CString strMsg = "";
	theApp.BoardReadback( bOP1, bOP2, 0x0, 0x0, wData1, wData2, strMsg );
	GetDlgItem( IDC_EDIT_ISR_IER )->SetWindowText( strMsg );
}

void CPageMCI::OnWriteINTEnableRegister() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	// 15.04.03 如果设置IER后直接读VAC的ISR，则回读值为0x0F7C
	//			即除了未定义的位，其它所有位都有中断
	// 为避免这种情况，初始化时设置VAC的IER前先回读一次其ISR
	//			此时回读值会为0x0000，可以不做任何处理
//	Serial_ReadISR( 3, &nRegister );
	WORD wData1, wData2;
	CString strMsg = "";
	theApp.BoardReadback( USB_CID_VAC_INT_Source_Register, USB_TM_VAC_Read_16, 0x0, 0x0, wData1, wData2, strMsg );
	theApp.BoardReadback( USB_CID_SPR_INT_Source_Register, USB_TM_SPR_Read_08, 0x0, 0x0, wData1, wData2, strMsg );

	WORD output;
	CString str = "";
	GetDlgItem( IDC_EDIT_ISR_IER )->GetWindowText( str );
	char *bufNum = str.GetBuffer(3);
	sscanf(bufNum, "%x", &output);

	BYTE bOP1 = 0x0;
	BYTE bOP2 = 0x0;
	BYTE bOp3 = 0x0;
	BYTE bOp4 = 0x0;
	switch( m_nBoardID )
	{
	case	0:
	case	1:
		break;
	case	2:
		bOP1	= USB_CID_VAC_INT_Enable_Register;
		bOP2	= USB_TM_VAC_Write_16;
		bOp3	= (BYTE)( output & 0x00FF );
		bOp4	= (BYTE)( output >> 8 );
		break;
	case	3:
		bOP1	= USB_CID_SPR_INT_Enable_Register;
		bOP2	= USB_TM_SPR_Write_08;
		bOp3	= 0;
		bOp4	= (BYTE)output;
		break;
	}
	if( bOP1 == 0x0 )
		return;

	theApp.CommToBoards2(	bOP1,
							bOP2,
							bOp3,
							bOp4 );
}

void CPageMCI::OnINT_ClearDB() 
{
	// Clear port B & D
	theApp.WriteToBoard( USB_ClearDB, FALSE, 0 );
	// After clear DB, comm with any sub-board, first
	// otherwise, PC can not receive INT signal any more
	theApp.CommToBoards2(	USB_CID_SPR_Test_LED,
							USB_TM_SPR_Write_08,
							0x0,
							0x0 );
}

void CPageMCI::OnINT_SetIMR() 
{
	// Set interrupt mask register
	CString str;
	GetDlgItemText( IDC_EDIT_Interrupt_Mask_Register, str );

	unsigned char output;
	char *buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	theApp.WriteToBoard( USB_SetIMR, TRUE, output );
}

void CPageMCI::OnINT_INTtest() 
{
	// INT test
	int nCheck = ((CButton*)(GetDlgItem(IDC_BUTTON_INT_test)))->GetCheck();
	if( nCheck )
	{
		(GetDlgItem(IDC_BUTTON_INT_test))->SetWindowText( "INT test ON" );
		theApp.WriteToBoard( USB_INT_test_ON, FALSE, 0 );

		UpdateData();
		int nIndex = 3 -m_nBoardID;
		theApp.WriteToBoard( USB_INT_test_SPR +nIndex, FALSE, 0 );
	}
	else
	{
		(GetDlgItem(IDC_BUTTON_INT_test))->SetWindowText( "INT test OFF" );
		theApp.WriteToBoard( USB_INT_test_OFF, FALSE, 0 );
	}
}

void CPageMCI::OnCHECKMainsLock() 
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

void CPageMCI::OnFakeMainslock() 
{
//	theApp.WriteToBoard( USB_Fake_Mains, FALSE, 0 );
}

void CPageMCI::OnCHECKVideo() 
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

void CPageMCI::InitDiagnose( BOOL bEnd )
{
}

void CPageMCI::SetTimers( UINT nIDEvent, BOOL bSet )
{
	if( bSet )
	{
		switch( nIDEvent )
		{
		case	Diag_Offset_0:
			m_nDet03Chn2 = 0;
			m_scrollDet03.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			Det03SB_Change( theApp.m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_1:
			m_nDet03Chn2 = 1;
			m_scrollDet03.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			Det03SB_Change( theApp.m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_2:
			m_nDet03Chn2 = 2;
			m_scrollDet03.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			Det03SB_Change( theApp.m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_3:
			m_nDet03Chn2 = 3;
			m_scrollDet03.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			Det03SB_Change( theApp.m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_4:
			m_nDet45Chn2 = 0;
			m_scrollDet45.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			Det45SB_Change( theApp.m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_5:
			m_nDet45Chn2 = 1;
			m_scrollDet45.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			Det45SB_Change( theApp.m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_FineMag_X:
			m_nDet45Chn2 = 2;
			m_scrollDet45.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			Det45SB_Change( theApp.m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_FineMag_Y:
			m_nDet45Chn2 = 3;
			m_scrollDet45.SetScrollPos( theApp.m_DL.diag[nIDEvent].nMin );
			Det45SB_Change( theApp.m_DL.diag[nIDEvent].nMin );
			break;
		default:
			break;
		}
//		SetTimer( nIDEvent +100, theApp.m_DL.diag[nIDEvent].nIntv, NULL );
	}
	else
		KillTimer( nIDEvent +100 );
}

void CPageMCI::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	DrawGrayScale(512, 192);
	if( nIDEvent == Diag_Offset_0 +100 )
	{
		m_nDet03Chn2 = 0;
		int nPos = m_nPosDet03[m_nDet03Chn2];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Offset_1 +100 )
	{
		m_nDet03Chn2 = 1;
		int nPos = m_nPosDet03[m_nDet03Chn2];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Offset_2 +100 )
	{
		m_nDet03Chn2 = 2;
		int nPos = m_nPosDet03[m_nDet03Chn2];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Offset_3 +100 )
	{
		m_nDet03Chn2 = 3;
		int nPos = m_nPosDet03[m_nDet03Chn2];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollDet03.SetScrollPos( nPos );
			Det03SB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Offset_4 +100 )
	{
		m_nDet45Chn2 = 0;
		int nPos = m_nPosDet45[m_nDet45Chn2];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_Offset_5 +100 )
	{
		m_nDet45Chn2 = 1;
		int nPos = m_nPosDet45[m_nDet45Chn2];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_FineMag_X +100 )
	{
		m_nDet45Chn2 = 2;
		int nPos = m_nPosDet45[m_nDet45Chn2];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
		}
	}
	else if( nIDEvent == Diag_FineMag_Y +100 )
	{
		m_nDet45Chn2 = 3;
		int nPos = m_nPosDet45[m_nDet45Chn2];
		nPos += theApp.m_DL.diag[nIDEvent-100].nStep;
		if( nPos > theApp.m_DL.diag[nIDEvent-100].nMax )
		{
			nPos = theApp.m_DL.diag[nIDEvent-100].nMax;
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
			KillTimer( nIDEvent );
			theApp.m_DL.diag[nIDEvent-100].nFinish = 1;
		}
		else
		{
			m_scrollDet45.SetScrollPos( nPos );
			Det45SB_Change( nPos );
		}
	}
	else if( nIDEvent == 200 )
	{
		// FT232循环发送
		FT232Test_SendData();
	}
	else if( nIDEvent == 201 )
	{
		// FT232循环发送
		FT232Test_RecvData();
	}

	CDialog::OnTimer(nIDEvent);
}

void CPageMCI::OnBUTTONFT232TestSend() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_EDIT_FT232Test_Intv )->EnableWindow( FALSE );
	GetDlgItem( IDC_CHECK_FT232Test_Loop )->EnableWindow( FALSE );
	GetDlgItem( IDC_EDIT_FT232Test_SendData )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_FT232Test_Send )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_FT232Test_Recv )->EnableWindow( FALSE );

	unsigned char output;
	CString str, strTemp, strRight;
	int i;

	GetDlgItem( IDC_EDIT_FT232Test_SendData )->GetWindowText( str );
	BOOL bIsSendEmpty = str.IsEmpty();
	if( bIsSendEmpty )
		m_nFT232SendCount	= 0;
	else
	{
		for( i=0; i<64; i++ )
		{
			int nPos = str.Find( "," );
			if( nPos < 0 )
			{
				char *bufLB = str.GetBuffer(3);
				sscanf(bufLB, "%x", &output);
				m_byteFT232Send[i] = (BYTE)output;
				break;
			}
			strTemp = str.Left( nPos );
			str = str.Right( str.GetLength() - nPos -1 );
			char *bufLB = strTemp.GetBuffer(3);
			sscanf(bufLB, "%x", &output);
			m_byteFT232Send[i] = (BYTE)output;
		}
		m_nFT232SendCount	= i +1;
	}

	GetDlgItem( IDC_EDIT_FT232Test_Intv )->GetWindowText( str );
	int nIntv = atoi( str );
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_FT232Test_Loop)))->GetCheck();
	if( nCheck )
	{
		if( !bIsSendEmpty )
			SetTimer( 200, nIntv, NULL );
		SetTimer( 201, nIntv, NULL );
	}
	else
	{
		DWORD dwBytesWritten = 0;
		BOOL bRet = theApp.Serial_Write(m_byteFT232Send, m_nFT232SendCount, dwBytesWritten);
		str.Format( "BytesWritten = %ld", dwBytesWritten );
		theApp.UpdateListString( str );

		GetDlgItem( IDC_EDIT_FT232Test_Intv )->EnableWindow( TRUE );
		GetDlgItem( IDC_CHECK_FT232Test_Loop )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_FT232Test_SendData )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_FT232Test_Send )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_FT232Test_Recv )->EnableWindow( TRUE );
	}
}

void CPageMCI::OnBUTTONSendInOneTime() 
{
	// TODO: Add your control notification handler code here
	unsigned char output;
	CString str, strTemp, strRight;
	int i;

	GetDlgItem( IDC_EDIT_FT232Test_SendData )->GetWindowText( str );
	BOOL bIsSendEmpty = str.IsEmpty();
	if( bIsSendEmpty )
		m_nFT232SendCount	= 0;
	else
	{
		for( i=0; i<64; i++ )
		{
			int nPos = str.Find( "," );
			if( nPos < 0 )
			{
				char *bufLB = str.GetBuffer(3);
				sscanf(bufLB, "%x", &output);
				m_byteFT232Send[i] = (BYTE)output;
				break;
			}
			strTemp = str.Left( nPos );
			str = str.Right( str.GetLength() - nPos -1 );
			char *bufLB = strTemp.GetBuffer(3);
			sscanf(bufLB, "%x", &output);
			m_byteFT232Send[i] = (BYTE)output;
		}
		m_nFT232SendCount	= i +1;
	}

	DWORD dwBytesWritten = 0;
	BOOL bRet = theApp.Serial_Write(m_byteFT232Send, m_nFT232SendCount, dwBytesWritten);
	str.Format( "BytesWritten = %ld", dwBytesWritten );
	theApp.UpdateListString( str );
}

void CPageMCI::OnBUTTONFT232TestStop() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_EDIT_FT232Test_Intv )->EnableWindow( TRUE );
	GetDlgItem( IDC_CHECK_FT232Test_Loop )->EnableWindow( TRUE );
	GetDlgItem( IDC_EDIT_FT232Test_SendData )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON_FT232Test_Send )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON_FT232Test_Recv )->EnableWindow( TRUE );
	KillTimer( 200 );
	KillTimer( 201 );
}

void CPageMCI::FT232Test_SendData()
{
	DWORD dwBytesWritten = 0;
	BOOL bRet = theApp.Serial_Write(m_byteFT232Send, m_nFT232SendCount, dwBytesWritten);

	CString str;
	str.Format( "BytesWritten = %ld", dwBytesWritten );
	theApp.UpdateListString( str );
}

void CPageMCI::FT232Test_RecvData()
{
	DWORD	dwBytesReturned = 0;
	CString str, strMsg, strList;
	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());

	int nReadReturn = theApp.Serial_Read( m_rxBuf, dwBytesReturned );
	if( nReadReturn == 1 )
		str = "no data in buf";
	else if( nReadReturn == 2 )
		str = "recv data, but timeout";
	pParent->UpdateListReadbackString( str );

	if( nReadReturn == 0 || nReadReturn == 2 )
	{
		str.Format( "FT232 Data Received: %ld bytes", dwBytesReturned );
		pParent->UpdateListReadbackString( str );
		strMsg = "";
		BOOL bFound = FALSE;
		for( DWORD i=0; i<dwBytesReturned; i++ )
		{
			strList.Format( "@ %d: 0x%02X", i, m_rxBuf[i] );
			pParent->UpdateListReadbackString( strList );
			
//			// 直到找到0xD22D才开始计数，前边的数据忽略
//			if( rxBuf[i] == 0xD2 && rxBuf[i+1] == 0x2D )
//				bFound = TRUE;
			str.Format( "%02X, ", m_rxBuf[i] );
			strMsg += str;
		}
		GetDlgItem( IDC_EDIT_FT232Test_RecvData )->SetWindowText( strMsg );
	}
}

void CPageMCI::OnBUTTONFT232TestRecv() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_FT232Test_Intv )->GetWindowText( str );
	int nIntv = atoi( str );
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_FT232Test_Loop)))->GetCheck();
	if( nCheck )
	{
		GetDlgItem( IDC_EDIT_FT232Test_Intv )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHECK_FT232Test_Loop )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_FT232Test_SendData )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_FT232Test_Send )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_FT232Test_Recv )->EnableWindow( FALSE );
		SetTimer( 201, nIntv, NULL );
	}
	else
		FT232Test_RecvData();
}

void CPageMCI::OnCHECKFT232TestLoop() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_FT232Test_Loop)))->GetCheck();
	if( nCheck )
	{
		GetDlgItem(IDC_BUTTON_FT232Test_Send)->SetWindowText( "循环发送和接收" );
		GetDlgItem(IDC_BUTTON_FT232Test_Recv)->SetWindowText( "循环接收" );
	}
	else
	{
		GetDlgItem(IDC_BUTTON_FT232Test_Send)->SetWindowText( "单次发送" );
		GetDlgItem(IDC_BUTTON_FT232Test_Recv)->SetWindowText( "单次接收" );
	}
}
