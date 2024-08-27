// PanelMCI.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "PanelMCI.h"
#include "ScanView.h"
#include "ControlLayer.h"
#include "BoardComm.h"
#include "ScaleManager.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "ScanParameterManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double g_dDwellTimeMin = 0.125 *4;

/////////////////////////////////////////////////////////////////////////////
// CPanelMCI dialog


CPanelMCI::CPanelMCI(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelMCI::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelMCI)
	m_strPortA = _T("");
	m_strPortB = _T("");
	m_strPortC = _T("");
	m_strPortD = _T("");
	m_strBWsel = _T("0");
	m_nReso = -1;
	m_nPixClk = -1;
	m_nDet03Chn2 = 0;
	m_nDet45Chn2 = 0;
	m_nDetGainNo2 = 0;
	m_nDetPair2 = 0;
	m_nDispType = -1;
	m_nBoardID = 2;
	//}}AFX_DATA_INIT

	m_nusbPixRate	= 1;
	m_bVideoMode	= FALSE;
	m_bDetMode		= FALSE;
	m_bDispMode		= TRUE;
	m_bRotate		= FALSE;
}

CPanelMCI::~CPanelMCI()
{
}


void CPanelMCI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelMCI)
	DDX_Control(pDX, IDC_CHECK_TestLED16on, m_btnTestLED16State);
	DDX_Control(pDX, IDC_SCROLLBAR_SpotY, m_scrollSpotY);
	DDX_Control(pDX, IDC_SCROLLBAR_SpotX, m_scrollSpotX);
	DDX_Control(pDX, IDC_BUTTON_Fake_Mainslock, m_btnFakeMains);
	DDX_Control(pDX, IDC_BUTTON_INT_test, m_btnINTtest);
	DDX_Control(pDX, IDC_CHECK_Video, m_btnVideo);
	DDX_Control(pDX, IDC_CHECK_MainsLock, m_btnMainsLock);
	DDX_Control(pDX, IDC_BUTTON_cmdRotation, m_btnRotate);
	DDX_Control(pDX, IDC_SCROLLBAR_BW_sel, m_scrollBWsel);
	DDX_Control(pDX, IDC_SCROLLBAR_DetGain, m_scrollDetGain);
	DDX_Control(pDX, IDC_SCROLLBAR_Det45, m_scrollDet45);
	DDX_Control(pDX, IDC_SCROLLBAR_Det03, m_scrollDet03);
	DDX_Control(pDX, IDC_SCROLLBAR_PixelRate, m_scrollPixelRate);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_DShift_X, m_scrollSegDShiftX);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_DShift_Y, m_scrollSegDShiftY);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_Valley_X, m_scrollSegValleyX);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_Valley_Y, m_scrollSegValleyY);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_Rise_X, m_scrollSegRiseX);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_Rise_Y, m_scrollSegRiseY);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_DRise_X, m_scrollSegDRiseX);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_DRise_Y, m_scrollSegDRiseY);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_Peak_X, m_scrollSegPeakX);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_Peak_Y, m_scrollSegPeakY);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_Fall_X, m_scrollSegFallX);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_Fall_Y, m_scrollSegFallY);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_DFall_X, m_scrollSegDFallX);
	DDX_Control(pDX, IDC_SCROLLBAR_Seg_DFall_Y, m_scrollSegDFallY);
	DDX_Control(pDX, IDC_BUTTON_DefaultRamps, m_btnDefaultRamp);
	DDX_Text(pDX, IDC_EDIT_PortA, m_strPortA);
	DDV_MaxChars(pDX, m_strPortA, 2);
	DDX_Text(pDX, IDC_EDIT_PortB, m_strPortB);
	DDV_MaxChars(pDX, m_strPortB, 2);
	DDX_Text(pDX, IDC_EDIT_PortC, m_strPortC);
	DDV_MaxChars(pDX, m_strPortC, 2);
	DDX_Text(pDX, IDC_EDIT_PortD, m_strPortD);
	DDV_MaxChars(pDX, m_strPortD, 2);
	DDX_Text(pDX, IDC_EDIT_BW_sel, m_strBWsel);
	DDX_Radio(pDX, IDC_RADIO_Resolution1, m_nReso);
	DDX_Radio(pDX, IDC_RADIO_PixClkEq_1, m_nPixClk);
	DDX_Text(pDX, IDC_EDIT_Det03_Chn2, m_nDet03Chn2);
	DDV_MinMaxUInt(pDX, m_nDet03Chn2, 0, 3);
	DDX_Text(pDX, IDC_EDIT_Det45_Chn2, m_nDet45Chn2);
	DDV_MinMaxUInt(pDX, m_nDet45Chn2, 0, 3);
	DDX_Text(pDX, IDC_EDIT_DetGain_no2, m_nDetGainNo2);
	DDV_MinMaxUInt(pDX, m_nDetGainNo2, 0, 5);
	DDX_Text(pDX, IDC_EDIT_DetPair2, m_nDetPair2);
	DDV_MinMaxUInt(pDX, m_nDetPair2, 0, 2);
	DDX_Radio(pDX, IDC_RADIO_Board_SCN, m_nBoardID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelMCI, CDialog)
	//{{AFX_MSG_MAP(CPanelMCI)
	ON_BN_CLICKED(IDC_BUTTON_ReadAll, OnReadAll)
	ON_BN_CLICKED(IDC_BUTTON_Set_Pixel_Rate, OnSetPixelRate)
	ON_BN_CLICKED(IDC_BUTTON_PixClkEq250KHz, OnSetPixClkEq250KHz)
	ON_BN_CLICKED(IDC_BUTTON_PixClkEq1MHz, OnSetPixClkEq1MHz)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Start, OnScanStart)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Stop, OnScanStop)
	ON_BN_CLICKED(IDC_BUTTON_UpdateX, OnUpdateX)
	ON_BN_CLICKED(IDC_BUTTON_UpdateY, OnUpdateY)
	ON_BN_CLICKED(IDC_BUTTON_DefaultRamps, OnUpdateDefaultRamps)
	ON_BN_CLICKED(IDC_BUTTON_Det03_Load, OnDet03Load)
	ON_BN_CLICKED(IDC_BUTTON_Det45_Load, OnDet45Load)
	ON_BN_CLICKED(IDC_BUTTON_cmdNormalMode, OnScanModeNormal)
	ON_BN_CLICKED(IDC_BUTTON_cmdLineMode, OnScanModeLine)
	ON_BN_CLICKED(IDC_BUTTON_cmdSpotMode, OnScanModeSpot)
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Det03, OnDeltaposDet03)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Det45, OnDeltaposDet45)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DetGain, OnDeltaposDetGain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DetPair, OnDeltaposDetPair)
	ON_BN_CLICKED(IDC_BUTTON_cmdRotation, OnScanRotation)
	ON_BN_CLICKED(IDC_BUTTON_cmdRotation_Update, OnScanRotationUpdate)
	ON_BN_CLICKED(IDC_BUTTON_Read_Version, OnReadVersion)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Start_With_Params, OnScanStartWithParams)
	ON_BN_CLICKED(IDC_BUTTON_Clear_DB, OnINT_ClearDB)
	ON_BN_CLICKED(IDC_BUTTON_Set_IMR, OnINT_SetIMR)
	ON_BN_CLICKED(IDC_BUTTON_INT_test, OnINT_INTtest)
	ON_BN_CLICKED(IDC_CHECK_MainsLock, OnCHECKMainsLock)
	ON_BN_CLICKED(IDC_BUTTON_MainsLock_LineSync, OnBUTTONMainsLockLineSync)
	ON_BN_CLICKED(IDC_BUTTON_MainsLock_FrameSync, OnBUTTONMainsLockFrameSync)
	ON_BN_CLICKED(IDC_CHECK_Video, OnCHECKVideo)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreeze, OnFreeze)
	ON_BN_CLICKED(IDC_BUTTON_Reset_Communicate, OnResetCommunicate)
	ON_BN_CLICKED(IDC_BUTTON_Read_INT_Source_Register, OnReadINTSourceRegister)
	ON_BN_CLICKED(IDC_BUTTON_Write_INT_Enable_Register, OnWriteINTEnableRegister)
	ON_BN_CLICKED(IDC_BUTTON_Fake_Mainslock, OnBUTTONFakeMainslock)
	ON_BN_CLICKED(IDC_BUTTON_RampSeg, OnUpdateRamp)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreezeX, OnSpotSetX)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreezeY, OnSpotSetY)
	ON_BN_CLICKED(IDC_BUTTON_SetPxlClkto2Cmd, OnSetPxlClkto2Cmd)
	ON_BN_CLICKED(IDC_CHECK_Det2nd, OnCHECKDet2nd)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Scan, OnCHECKDebugScan)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Z, OnCHECKDebugZ)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Osc, OnCHECKDebugOsc)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Osc_90, OnCHECKDebugModeOsc90)
	ON_BN_CLICKED(IDC_CHECK_TestLED16on, OnTestLED16on)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_Resolution1, IDC_RADIO_Resolution7, OnResolutionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_PixClkEq_1, IDC_RADIO_PixClkEq_10, OnPixClkChanged)
	ON_COMMAND_RANGE(IDC_RADIO_DetPair_Sel_01, IDC_RADIO_DetPair_Sel_45, OnDetPairSelect)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, memberFxn )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelMCI message handlers

//在线提示
BOOL CPanelMCI::memberFxn( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
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

BOOL CPanelMCI::OnInitDialog()
{
	CDialog::OnInitDialog();
    EnableToolTips(TRUE);		//允许在线提示

	// TODO: Add extra initialization here
	///////////////////////////////////////////////////////////////////////////

	CString str;
	str = "x21 - Xoffset,    x22 - Xvalley,	x23 - Xrise,       x24 - dXrise";
	str += "\r\n";
	str += "\r\n";
	str += "x25 - Xpeak,     x26 - Xfall,	x27 - dXfall";
	str += "\r\n";
	str += "\r\n";
	str += "x28 - Yoffset,    x29 - Yvalley,	x2A - Yrise,       x2B - dYrise";
	str += "\r\n";
	str += "\r\n";
	str += "x2C - Ypeak,     x2D - Yfall,	x2E - dYfall";
//	( GetDlgItem( IDC_STATIC_txtSegNums ) )->SetWindowText( str );

	///////////////////////////////////////////////////////////////////////////

	m_scrollPixelRate.SetScrollRange( 1, 511 );			// 1 --- 2K
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	m_scrollSegDShiftX.SetScrollRange( g_n16bitDAC_Min_Offset, -1*g_n16bitDAC_Min_Offset -1 );	// 0 --- 0x3FFF (14 bit)
	m_scrollSegDShiftY.SetScrollRange( g_n16bitDAC_Min_Offset, -1*g_n16bitDAC_Min_Offset -1 );
	m_scrollSegDFallX.SetScrollRange( g_n16bitDAC_Min_Offset +1, -1*g_n16bitDAC_Min_Offset -1 );
	m_scrollSegDFallY.SetScrollRange( g_n16bitDAC_Min_Offset +1, -1*g_n16bitDAC_Min_Offset -1 );
	m_scrollSegDRiseX.SetScrollRange( 1, 16 );	// 16384/1024 = 16
	m_scrollSegDRiseY.SetScrollRange( 1, 16 );
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192

	m_scrollSegValleyX.SetScrollRange( 1, 256 );
	m_scrollSegValleyY.SetScrollRange( 0, 256 );
	m_scrollSegRiseX.SetScrollRange( 16, 2048 );
	m_scrollSegRiseY.SetScrollRange( 16, 2048 );
	m_scrollSegPeakX.SetScrollRange( 0, 8 );
	m_scrollSegPeakY.SetScrollRange( 0, 8 );
	m_scrollSegFallX.SetScrollRange( 0, 8 );
	m_scrollSegFallY.SetScrollRange( 0, 8 );
	m_scrollSpotX.SetScrollRange( 0, 1023 );
	m_scrollSpotY.SetScrollRange( 0, 1023 );

	m_nPixClk = 3;
	((CButton*)(GetDlgItem( IDC_RADIO_PixClkEq_4 )))->SetCheck( 1 );
	m_nReso = 6;
	((CButton*)(GetDlgItem( IDC_RADIO_Resolution7 )))->SetCheck( 1 );

	m_scrollDet03.SetScrollRange( 0, 4095 );	// 0 --- 0xFFF
	m_scrollDet45.SetScrollRange( 0, 4095 );	// 0 --- 0xFFF
	m_scrollDetGain.SetScrollRange( 0, 7 );
	m_scrollBWsel.SetScrollRange( 0, 7 );
	switch( CControlLayer::Instance().m_BrdMCI.nDetPair )
	{
	case	0:
		((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->SetCheck(1);
		break;
	case	1:
		((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_23 )))->SetCheck(1);
		break;
	case	2:
		((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_45 )))->SetCheck(1);
		break;
	}
	if( CControlLayer::Instance().m_BrdMCI.bDet2nd )
		m_nDetGainNo2 = 2 * CControlLayer::Instance().m_BrdMCI.nDetPair +1;
	else
		m_nDetGainNo2 = 2 * CControlLayer::Instance().m_BrdMCI.nDetPair;
	str.Format( "%d", CControlLayer::Instance().m_BrdMCI.nGain[m_nDetGainNo2] );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );
	str.Format( "%d", CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair] );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );

	m_btnOK.SetColor( g_crSYS );
	m_btnCancel.SetColor( g_crSYS );

	m_btnRotate.SetColor( ::GetSysColor(COLOR_3DFACE) );
	GetDlgItem( IDC_EDIT_Rdegree )->SetWindowText( "0" );

	( GetDlgItem( IDC_EDIT_Interrupt_Mask_Register ) )->SetWindowText( "02" );
	m_btnMainsLock.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnMainsLock.SetWindowText( "MainsLock OFF" );
	m_btnVideo.SetColor( g_crEmphasis2 );
	m_btnVideo.SetWindowText( "Video ON" );
	m_btnVideo.SetCheck( 1 );
	m_btnTestLED16State.SetColor( g_crOFF );

	UpdateData( FALSE );
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPanelMCI::Init()
{
	m_scrollPixelRate.SetScrollPos( 8 );
	PixelRateSB_Change( 8 );

	m_scrollSegDShiftX.SetScrollPos( 0 +g_n16bitDAC_Min_Offset );
	SegDShiftXSB_Change( 0 +g_n16bitDAC_Min_Offset );
	m_scrollSegDShiftY.SetScrollPos( 0 +g_n16bitDAC_Min_Offset );
	SegDShiftYSB_Change( 0 +g_n16bitDAC_Min_Offset );

	m_scrollSegValleyX.SetScrollPos( 12 );
	SegValleyXSB_Change( 12 );
	m_scrollSegValleyY.SetScrollPos( 12 );
	SegValleyYSB_Change( 12 );
	m_scrollSegRiseX.SetScrollPos( 1024 );
	SegRiseXSB_Change( 1024 );
	m_scrollSegRiseY.SetScrollPos( 1024 );
	SegRiseYSB_Change( 1024 );

	m_scrollSegDRiseX.SetScrollPos( 16 );
	SegDRiseXSB_Change( 16 );
	m_scrollSegDRiseY.SetScrollPos( 16 );
	SegDRiseYSB_Change( 16 );

	m_scrollSegPeakX.SetScrollPos( 0 );
	SegPeakXSB_Change( 0 );
	m_scrollSegPeakY.SetScrollPos( 0 );
	SegPeakYSB_Change( 0 );
	m_scrollSegFallX.SetScrollPos( 0 );
	SegFallXSB_Change( 0 );
	m_scrollSegFallY.SetScrollPos( 0 );
	SegFallYSB_Change( 0 );
	m_scrollSegDFallX.SetScrollPos( 8191 );
	SegDFallXSB_Change( 8191 );
	m_scrollSegDFallY.SetScrollPos( 8191 );
	SegDFallYSB_Change( 8191 );
	UpdateLineRate();
	UpdateFrameRate();
	m_scrollSpotX.SetScrollPos( 511 );
	SpotXSB_Change( 511 );
	m_scrollSpotY.SetScrollPos( 511 );
	SpotYSB_Change( 511 );
}

void CPanelMCI::OnOK() 
{
}

void CPanelMCI::OnCancel() 
{
}

BOOL CPanelMCI::PreTranslateMessage(MSG* pMsg) 
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
		case	IDC_EDIT_spotX_HB:
		case	IDC_EDIT_spotY_HB:
		case	IDC_EDIT_Seg_DShift_X:
		case	IDC_EDIT_Seg_DShift_Y:
		case	IDC_EDIT_Seg_Rise_X:
		case	IDC_EDIT_Seg_Rise_Y:
		case	IDC_EDIT_Seg_DRise_X:
		case	IDC_EDIT_Seg_DRise_Y:
		case	IDC_EDIT_Seg_DFall_X:
		case	IDC_EDIT_Seg_DFall_Y:
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
		case	IDC_EDIT_Det03:
		case	IDC_EDIT_Det45:
		case	IDC_EDIT_Seg_DShift_X:
		case	IDC_EDIT_Seg_DShift_Y:
		case	IDC_EDIT_Seg_DFall_X:
		case	IDC_EDIT_Seg_DFall_Y:
			bufNum = str.GetBuffer(5);
			sscanf(bufNum, "%04X", &nPos);
			break;
		case	IDC_EDIT_DetGain:
		case	IDC_EDIT_BW_sel2:
		case	IDC_EDIT_spotX_HB:
		case	IDC_EDIT_spotY_HB:
		case	IDC_EDIT_Seg_Rise_X:
		case	IDC_EDIT_Seg_Rise_Y:
		case	IDC_EDIT_Seg_DRise_X:
		case	IDC_EDIT_Seg_DRise_Y:
			bufNum = str.GetBuffer(5);
			sscanf(bufNum, "%d", &nPos);
			break;
		default:
			break;
		}
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
		case	IDC_EDIT_spotX_HB:
			m_scrollSpotX.SetScrollPos( nPos );
			SpotXSB_Change( nPos );
			break;
		case	IDC_EDIT_spotY_HB:
			m_scrollSpotY.SetScrollPos( nPos );
			SpotYSB_Change( nPos );
			break;
		case	IDC_EDIT_Seg_DShift_X:
			m_scrollSegDShiftX.SetScrollPos( nPos +g_n16bitDAC_Min_Offset );
			SegDShiftXSB_Change( nPos +g_n16bitDAC_Min_Offset );
			break;
		case	IDC_EDIT_Seg_DShift_Y:
			m_scrollSegDShiftY.SetScrollPos( nPos +g_n16bitDAC_Min_Offset );
			SegDShiftYSB_Change( nPos +g_n16bitDAC_Min_Offset );
			break;
		case	IDC_EDIT_Seg_Rise_X:
			m_scrollSegRiseX.SetScrollPos( nPos );
			SegRiseXSB_Change( nPos );
			break;
		case	IDC_EDIT_Seg_Rise_Y:
			m_scrollSegRiseY.SetScrollPos( nPos );
			SegRiseYSB_Change( nPos );
			break;
		case	IDC_EDIT_Seg_DRise_X:
			m_scrollSegDRiseX.SetScrollPos( nPos );
			SegDRiseXSB_Change( nPos );
			break;
		case	IDC_EDIT_Seg_DRise_Y:
			m_scrollSegDRiseY.SetScrollPos( nPos );
			SegDRiseYSB_Change( nPos );
			break;
		case	IDC_EDIT_Seg_DFall_X:
			m_scrollSegDFallX.SetScrollPos( nPos );
			SegDFallXSB_Change( nPos );
			break;
		case	IDC_EDIT_Seg_DFall_Y:
			m_scrollSegDFallY.SetScrollPos( nPos );
			SegDFallYSB_Change( nPos );
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPanelMCI::OnReadAll() 
{
	CString str = "";
	GetDlgItem( IDC_EDIT_PortA )->SetWindowText(str);
	GetDlgItem( IDC_EDIT_PortB )->SetWindowText(str);
	GetDlgItem( IDC_EDIT_PortC )->SetWindowText(str);
	GetDlgItem( IDC_EDIT_PortD )->SetWindowText(str);
	if( CSemCtrl::Instance().Serial_ReadbackData(1,USB_CID_SCN_ReadBoardID, CBoardComm::Instance().m_pnReadCommonData) )
	{
		str.Format( "%02X", (BYTE)(CBoardComm::Instance().m_pnReadCommonData[0] & 0x00FF) );
		GetDlgItem( IDC_EDIT_PortA )->SetWindowText(str);
	}
	if( CSemCtrl::Instance().Serial_ReadbackData(2,USB_CID_LNS_ReadBoardID, CBoardComm::Instance().m_pnReadCommonData) )
	{
		str.Format( "%02X", (BYTE)(CBoardComm::Instance().m_pnReadCommonData[0] & 0x00FF) );
		GetDlgItem( IDC_EDIT_PortB )->SetWindowText(str);
	}
	if( CSemCtrl::Instance().Serial_ReadbackData(3,USB_CID_VAC_ReadBoardID, CBoardComm::Instance().m_pnReadCommonData) )
	{
		str.Format( "%02X", (BYTE)(CBoardComm::Instance().m_pnReadCommonData[0] & 0x00FF) );
		GetDlgItem( IDC_EDIT_PortC )->SetWindowText(str);
	}
	if( CSemCtrl::Instance().Serial_ReadbackData(4,USB_CID_SPR_ReadBoardID, CBoardComm::Instance().m_pnReadCommonData) )
	{
		str.Format( "%02X", (BYTE)(CBoardComm::Instance().m_pnReadCommonData[0] & 0x00FF) );
		GetDlgItem( IDC_EDIT_PortD )->SetWindowText(str);
	}
}
/*
void CPanelMCI::ReadBackdata( int nBits )
{
	Sleep(30);

	if( nBits == 8 )
	{
		BYTE bData[1024];
		DWORD i, dwBytesReturned = 0;
		if( CSemCtrl::Instance().Serial_Read( bData, dwBytesReturned ) == 0 )
		{
			CString str, strMsg;
			str.Format( "Data Received: %ld bytes", dwBytesReturned );
			pParent->UpdateListReadbackString( str );
			
			if( dwBytesReturned > 0 )
			{
				strMsg = "";
				for( i=0; i<dwBytesReturned; i++ )
				{
					str.Format( "0x%02X, ", bData[i] );
					pParent->UpdateListReadbackString( str );
					strMsg += str;
				}
				pParent->UpdateListReadbackString( " " );
				if( dwBytesReturned > frameLen2 )
					return;
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
					str.Format( "0x%04X", bData[i] );
					pParent->UpdateListReadbackString( str );
				}
				pParent->UpdateListReadbackString( " " );
			}
		}
	}
}

void CPanelMCI::PwrVconV( BYTE bData1, BYTE bData2, CDialog* pParent )
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

	switch( nChn )
	{
	case	4:
	case	5:
	case	6:
	case	7:
		dValue = 2 *g_dPwrbitsRange *wData /g_wPwrbitsRange; 
		break;
	case	3:
		dValue = 6 *g_dPwrbitsRange /5 *wData /g_wPwrbitsRange; 
		break;
	case	2:
		dValue = 3 *g_dPwrbitsRange *wData /g_wPwrbitsRange; 
		break;
	case	1:
		dValue = 3.5 *g_dPwrbitsRange *wData /g_wPwrbitsRange - 2.5 *m_dPwrReadback[1]; 
		break;
	case	0:
		dValue = 3.5 *g_dPwrbitsRange *wData /g_wPwrbitsRange - 2.5 *m_dPwrReadback[4]; 
		break;
	default:
		break;
	}
	m_dPwrReadback[nChn] = dValue;
}
*/
void CPanelMCI::OnReadVersion() 
{
	BYTE byteDB[2];
	CSemCtrl::Instance().USB_GetPortInfo( byteDB );
	CString str;
	str.Format( "%02X", byteDB[0] );
	GetDlgItem(IDC_EDIT_PortD)->SetWindowText(str);
	str.Format( "%02X", byteDB[1] );
	GetDlgItem(IDC_EDIT_PortB)->SetWindowText(str);
}

void CPanelMCI::OnResetCommunicate() 
{
	// Reset / Initialize Board Communication Circuit
	CSemCtrl::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
}









void CPanelMCI::OnSetPixelRate() 
{
	// Send binary 101(5) on Port C and 0x11 on Port A to begin setting pixel clock rate to 8 x 125nS (1 MHz),
	// followed by 001 on C + 8 on A, then 010 on C + 0 on A, then 111 on C, then 000 on C.
	// 设置点逗留时间 pixel rate
	int nPos = m_scrollPixelRate.GetScrollPos();
//	CSemCtrl::Instance().USB_WriteToBoard( USB_SetPixelRate, TRUE, (WORD)nPos );

	CSemCtrl::Instance().m_SemScan.USB_SetParams( 10, nPos );	// 调试模式下设置像素时钟N值

	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
	{
		if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
			pView->USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
	}
}

void CPanelMCI::OnSetPixClkEq250KHz() 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetPixelRate, TRUE, 0x20 );
}

void CPanelMCI::OnSetPixClkEq1MHz() 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetPixelRate, TRUE, 0x08 );
}

void CPanelMCI::OnPixClkChanged(UINT nID)
{
	UpdateData();
	PixClkChanged();
}

void CPanelMCI::PixClkChanged()
{
	COLORREF clr = ::GetSysColor(COLOR_3DFACE);
/*	m_radioPixClkEq1.SetColor( clr );
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
*/
	int nPos = m_scrollPixelRate.GetScrollPos();
	switch( m_nPixClk )
	{
	case	0:
//		m_radioPixClkEq1.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 1;
		break;
	case	1:
//		m_radioPixClkEq2.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 2;
		break;
	case	2:
//		m_radioPixClkEq3.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 4;
		break;
	case	3:
//		m_radioPixClkEq4.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 8;
		break;
	case	4:
//		m_radioPixClkEq5.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 16;
		break;
	case	5:
//		m_radioPixClkEq6.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 40;
		break;
	case	6:
//		m_radioPixClkEq7.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 80;
		break;
	case	7:
//		m_radioPixClkEq8.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 128;
		break;
	case	8:
//		m_radioPixClkEq9.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 160;
		break;
	case	9:
//		m_radioPixClkEq10.SetColor( g_crEmphasis1 );
		m_nusbPixRate = 320;
		break;
	}

	if( nPos != m_scrollPixelRate.GetScrollPos() )
	{
		m_scrollPixelRate.SetScrollPos( nPos );
		PixelRateSB_Change( nPos );
	}
}

void CPanelMCI::OnSetPxlClkto2Cmd() 
{
	// TODO: Add your control notification handler code here
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetPixelRate, TRUE, 2 );
}

void CPanelMCI::OnScanStart() 
{
	ScanStart();
}

void CPanelMCI::ScanStart()
{
	// Clear FIFO
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();//TIME_METHOD_WAIT_INPUT_ENABLE);
	// Start scan
	CSemCtrl::Instance().USB_WriteToBoard( USB_StartScan, FALSE, 0 );
}

void CPanelMCI::OnScanStartWithParams() 
{
	// TODO: Add your control notification handler code here
	
}

void CPanelMCI::OnScanStop() 
{
	ScanStop();
}

void CPanelMCI::ScanStop()
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_StopScan, FALSE, 0 );
}






void CPanelMCI::UpdateLineRate()
{
	CString str;
	double dValue, dDwellPixel;
	dDwellPixel = m_scrollPixelRate.GetScrollPos() * g_dDwellTimeMin;

	dValue = m_scrollSegValleyX.GetScrollPos() * dDwellPixel;
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Valley_TP_X )->SetWindowText( str );

	dValue = m_scrollSegRiseX.GetScrollPos() * dDwellPixel;
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Rise_TP_X )->SetWindowText( str );
	
	dValue = m_scrollSegPeakX.GetScrollPos() * dDwellPixel;
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Peak_TP_X )->SetWindowText( str );
	
	dValue = m_scrollSegFallX.GetScrollPos() * dDwellPixel;
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Fall_TP_X )->SetWindowText( str );

	dValue = ( m_scrollSegValleyX.GetScrollPos()
			+ m_scrollSegRiseX.GetScrollPos()
			+ m_scrollSegPeakX.GetScrollPos()
			+ m_scrollSegFallX.GetScrollPos() )
			* dDwellPixel;
	str.Format( "%.3f", dValue /1000 );
	GetDlgItem( IDC_EDIT_LineRate )->SetWindowText( str );
}

void CPanelMCI::UpdateFrameRate()
{
	CString str;
	double dValue, dDwellLine;
	GetDlgItem( IDC_EDIT_LineRate )->GetWindowText( str );
	dDwellLine = atof( str );

	dValue = m_scrollSegValleyY.GetScrollPos() * dDwellLine;
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Valley_TP_Y )->SetWindowText( str );

	dValue = m_scrollSegRiseY.GetScrollPos() * dDwellLine;
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Rise_TP_Y )->SetWindowText( str );

	dValue = m_scrollSegPeakY.GetScrollPos() * dDwellLine;
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Peak_TP_Y )->SetWindowText( str );

	dValue = m_scrollSegFallY.GetScrollPos() * dDwellLine;
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Fall_TP_Y )->SetWindowText( str );

	dValue = ( m_scrollSegValleyY.GetScrollPos()
			+ m_scrollSegRiseY.GetScrollPos()
			+ m_scrollSegPeakY.GetScrollPos()
			+ m_scrollSegFallY.GetScrollPos() )
			* dDwellLine;
	str.Format( "%.3f", dValue /1000 );
	GetDlgItem( IDC_EDIT_FrameRate )->SetWindowText( str );
}

void CPanelMCI::OnUpdateX() 
{
	// TODO: Add your control notification handler code here
	USB_InitData	data;
	ZeroMemory( &data, sizeof(USB_InitData) );

	// 始终使用满量程2的14次方，步距根据当前分辨率设置
	WORD	wStepX		= (WORD)( (g_n16bitDAC_Max +1) / m_scrollSegRiseX.GetScrollPos() );

	USB_InitScanRampParam usbParam;
	CString str;
	// X扫描波
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	// 所以最终送给MCI的参数要左移2位，即变为16位的高14位
	// 涉及参数为Offset, dRise, dFall
	int nOffset = UpdateOffsetX();
	if( nOffset > g_n16bitDAC_Max )
		nOffset = g_n16bitDAC_Max;
	usbParam.wOffset		= (WORD)nOffset;

	usbParam.wValley		= m_scrollSegValleyX.GetScrollPos();

	usbParam.wRise			= m_scrollSegRiseX.GetScrollPos();

	GetDlgItem( IDC_EDIT_Seg_DRise_X )->GetWindowText( str );
	usbParam.wDRise			= (WORD)atoi(str);
	if( usbParam.wDRise > wStepX )
		usbParam.wDRise = wStepX;

	usbParam.wPeak			= m_scrollSegPeakX.GetScrollPos();

	usbParam.wFall			= m_scrollSegFallX.GetScrollPos();
	if( usbParam.wFall > 0 )
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
	else
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);

	usbParam.wOffset		= usbParam.wOffset << 2;
	usbParam.wDRise			= usbParam.wDRise << 2;
	usbParam.wDFall			= usbParam.wDFall << 2;

	data.ScanRampParamX		= usbParam;
	///////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////
	unsigned char funcNo;
	USB_InitParam* pParam;
	pParam = (USB_InitParam *)(&data);

	// 设置X扫描波形7参数
	funcNo = USB_SetXoffset;
	for (int i=0; i<7; i++)
//		CSemCtrl::Instance().USB_WriteToBoard( funcNo +i, TRUE, pParam->wData[i] );
		CSemCtrl::Instance().m_SemScan.USB_SetParams( 11 +i, pParam->wData[i] );
	///////////////////////////////////////////////////////////////////////////
}

void CPanelMCI::OnUpdateY() 
{
	// TODO: Add your control notification handler code here
	USB_InitData	data;
	ZeroMemory( &data, sizeof(USB_InitData) );

	// 始终使用满量程2的14次方，步距根据当前分辨率设置
	WORD	wStepY		= (WORD)((g_n16bitDAC_Max +1) / m_scrollSegRiseY.GetScrollPos());

	USB_InitScanRampParam usbParam;
	CString str;
	// Y扫描波
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	// 所以最终送给MCI的参数要左移2位，即变为16位的高14位
	// 涉及参数为Offset, dRise, dFall
	int nOffset = UpdateOffsetY();
	if( nOffset > g_n16bitDAC_Max )
		nOffset = g_n16bitDAC_Max;
	usbParam.wOffset		= (WORD)nOffset;

	usbParam.wValley		= m_scrollSegValleyY.GetScrollPos();

	usbParam.wRise			= m_scrollSegRiseY.GetScrollPos();

	GetDlgItem( IDC_EDIT_Seg_DRise_Y )->GetWindowText( str );
	usbParam.wDRise			= (WORD)atoi(str);
	if( usbParam.wDRise > wStepY )
		usbParam.wDRise = wStepY;

	usbParam.wPeak			= m_scrollSegPeakY.GetScrollPos();

	usbParam.wFall			= m_scrollSegFallY.GetScrollPos();
	if( usbParam.wFall > 0 )
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
	else
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);

	usbParam.wOffset		= usbParam.wOffset << 2;
	usbParam.wDRise			= usbParam.wDRise << 2;
	usbParam.wDFall			= usbParam.wDFall << 2;

	data.ScanRampParamX		= usbParam;
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	unsigned char funcNo;
	USB_InitParam* pParam;
	pParam = (USB_InitParam *)(&data);

	// 设置X扫描波形7参数
	funcNo = USB_SetYoffset;
	for (int i=0; i<7; i++)
//		CSemCtrl::Instance().USB_WriteToBoard( funcNo +i, TRUE, pParam->wData[i] );
		CSemCtrl::Instance().m_SemScan.USB_SetParams( 21 +i, pParam->wData[i] );
	///////////////////////////////////////////////////////////////////////////
}

void CPanelMCI::OnUpdateRamp() 
{
	// TODO: Add your control notification handler code here
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
	{
		if( CScanParameterManager::Instance().m_nusbScanResoIndex != m_nReso )
			pView->NotifyScanResolution( m_nReso );
		else
		{
			if ( CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
				pView->USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
		}
	}
}

void CPanelMCI::OnUpdateDefaultRamps() 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDefaultRamps, FALSE, 0 );
}

void CPanelMCI::OnSpotSetX() 
{
	// TODO: Add your control notification handler code here
	int nPos = m_scrollSpotX.GetScrollPos();
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
	{
		pView->SetLineScanParamManually( FALSE, nPos );
		if ( CScanParameterManager::Instance().m_nusbScanType == USB_ST_LineScan_VERT )
			pView->USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
	}
//	CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeX, TRUE, (WORD)m_scrollSpotX.GetScrollPos() );
}

void CPanelMCI::UpdateSpotX( int nPos, BOOL bUpdate )
{
	double dValue = 0.0;
	CString str = "";
	
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_spotX_HB )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Pos_X].nCurPos = nPos;

	if( bUpdate )
		CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeX, TRUE, (WORD)nPos );
}

void CPanelMCI::OnSpotSetY() 
{
	// TODO: Add your control notification handler code here
	int nPos = m_scrollSpotY.GetScrollPos();
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
	{
		pView->SetLineScanParamManually( TRUE, nPos );
		if ( CScanParameterManager::Instance().m_nusbScanType == USB_ST_LineScan_HORZ )
			pView->USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
	}
//	CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeY, TRUE, (WORD)m_scrollSpotY.GetScrollPos() );
}

void CPanelMCI::UpdateSpotY( int nPos, BOOL bUpdate )
{
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_spotY_HB )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Pos_Y].nCurPos = nPos;

	if( bUpdate )
		CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeY, TRUE, (WORD)nPos );
}

CSize CPanelMCI::GetSizes()
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

void CPanelMCI::OnResolutionChanged(UINT nID)
{
	UpdateData();

	COLORREF clr = ::GetSysColor(COLOR_3DFACE);
/*	m_radioResolution1.SetColor( clr );
	m_radioResolution2.SetColor( clr );
	m_radioResolution3.SetColor( clr );
	m_radioResolution4.SetColor( clr );
	m_radioResolution5.SetColor( clr );
	m_radioResolution6.SetColor( clr );
	m_radioResolution7.SetColor( clr );
//	Invalidate(0);

	switch( m_nReso )
	{
	case	0:
		m_radioResolution1.SetColor( g_crEmphasis1 );
		break;
	case	1:
		m_radioResolution2.SetColor( g_crEmphasis1 );
		break;
	case	2:
		m_radioResolution3.SetColor( g_crEmphasis1 );
		break;
	case	3:
		m_radioResolution4.SetColor( g_crEmphasis1 );
		break;
	case	4:
		m_radioResolution5.SetColor( g_crEmphasis1 );
		break;
	case	5:
		m_radioResolution6.SetColor( g_crEmphasis1 );
		break;
	case	6:
		m_radioResolution7.SetColor( g_crEmphasis1 );
		break;
	}
*/
	CSize sz = GetSizes();
	m_scrollSegRiseX.SetScrollPos( sz.cx );
	SegRiseXSB_Change( sz.cx );
	m_scrollSegRiseY.SetScrollPos( sz.cx );
	SegRiseYSB_Change( sz.cy );

	m_scrollSegDRiseX.SetScrollRange( 1, (g_n16bitDAC_Max +1) / sz.cx );
	m_scrollSegDRiseX.SetScrollPos( (g_n16bitDAC_Max +1) / sz.cx );
	SegDRiseXSB_Change( (g_n16bitDAC_Max +1) / sz.cx );
	m_scrollSegDRiseY.SetScrollRange( 1, (g_n16bitDAC_Max +1) / sz.cy );
	m_scrollSegDRiseY.SetScrollPos( (g_n16bitDAC_Max +1) / sz.cy );
	SegDRiseYSB_Change( (g_n16bitDAC_Max +1) / sz.cy );

	m_scrollSpotX.SetScrollRange(0, sz.cx-1);
	m_scrollSpotX.SetScrollPos(sz.cx-1);
	SpotXSB_Change(sz.cx-1);
	m_scrollSpotY.SetScrollRange(0, sz.cy-1);
	m_scrollSpotY.SetScrollPos(sz.cy-1);
	SpotYSB_Change(sz.cy-1);
	return;

	///////////////////////////////////////////////////////////////////////////
	USB_InitData	data;
	ZeroMemory( &data, sizeof(USB_InitData) );

	// 始终使用满量程2的14次方，步距根据当前分辨率设置
//	WORD	wFullRange	= (WORD)pow(2, 14);
	WORD	wFullRange	= 64000;	//这是目前试验时的最大值
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

	data.PixRate			= m_nusbPixRate;
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	unsigned char funcNo;
	USB_InitParam* pParam;
	pParam = (USB_InitParam *)(&data);

	// 设置点逗留时间 pixel rate
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetPixelRate, TRUE, data.PixRate );

	// 设置X扫描波形7参数，调试时只设置Rise和dRise
	funcNo = USB_SetXoffset;
	int i=2;
	for (i=2; i<4; i++)
		CSemCtrl::Instance().USB_WriteToBoard( funcNo +i, TRUE, pParam->wData[i] );
	CSemCtrl::Instance().USB_WriteToBoard( USB_UpdateXramp, FALSE, 0 );

	// 设置Y扫描波形7参数，调试时只设置Rise和dRise
	funcNo = USB_SetYoffset;
	for (i=2; i<4; i++)
		CSemCtrl::Instance().USB_WriteToBoard( funcNo +i, TRUE, pParam->wData[i +7] );
	CSemCtrl::Instance().USB_WriteToBoard( USB_UpdateYramp, FALSE, 0 );
	///////////////////////////////////////////////////////////////////////////
}

void CPanelMCI::OnDet03Load() 
{
	// Send binary 100 on Port C and 0x32 on Port A, then bin 111 on C, then 000 on C to start scan
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det03LoadDACs, FALSE, 0 );
}

void CPanelMCI::OnDet45Load() 
{
	// Send binary 101(5) on Port C and opcode 0x34 on Port A
	// followed by 001 on C + 8 on A, then 010 on C + 0 on A, then 111 on C, then 000 on C.
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det45LoadDACs, FALSE, 0 );
}

void CPanelMCI::OnDeltaposDet03(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
/*	if (pNMUpDown->iDelta >= 1)
	{
		lXPos = GetNewPos( lXPos, 0 );
		SetCtrlPos( lXPos );
	}
	if (pNMUpDown->iDelta <= -1)
	{
		lXPos = GetNewPos( lXPos, 2 );
		SetCtrlPos( lXPos );
	}
*/
	m_nDet03Chn2 -= pNMUpDown->iDelta;
	if(m_nDet03Chn2 < 0)
		m_nDet03Chn2 = 0;
	else if(m_nDet03Chn2 > 3)
		m_nDet03Chn2 = 3;
	UpdateData( FALSE );

	int nPos = 2048- CControlLayer::Instance().m_BrdMCI.nOffset[m_nDet03Chn2];
	m_scrollDet03.SetScrollPos( nPos );
	CString str;
	str.Format( "%03X", nPos );
	GetDlgItem( IDC_EDIT_Det03 )->SetWindowText( str );

	*pResult = 0;
}

void CPanelMCI::OnDeltaposDet45(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nDet45Chn2 -= pNMUpDown->iDelta;
	if(m_nDet45Chn2 < 0)
		m_nDet45Chn2 = 0;
	else if(m_nDet45Chn2 > 3)
		m_nDet45Chn2 = 3;

/*	if( m_nDet45Chn2 == 0 || m_nDet45Chn2 == 1 )
		m_scrollDet45.SetScrollRange( 0, 4095 );
	else
		m_scrollDet45.SetScrollRange( 511, 2047 );
*/	UpdateData( FALSE );

	if( m_nDet45Chn2 < 2 )
	{
		int nPos = 2048 -CControlLayer::Instance().m_BrdMCI.nOffset[m_nDet45Chn2 +4];
		m_scrollDet45.SetScrollPos( nPos );
		CString str;
		str.Format( "%03X", nPos );
		GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );
	}

	*pResult = 0;
}

void CPanelMCI::OnDeltaposDetGain(NMHDR* pNMHDR, LRESULT* pResult) 
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

	int nPos = CControlLayer::Instance().m_BrdMCI.nGain[m_nDetGainNo2];
	m_scrollDetGain.SetScrollPos( nPos );
	CString str;
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );

	*pResult = 0;
}

void CPanelMCI::OnDeltaposDetPair(NMHDR* pNMHDR, LRESULT* pResult) 
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

	int nPos = CControlLayer::Instance().m_BrdMCI.nFilter[m_nDetPair2];
	m_scrollBWsel.SetScrollPos( nPos );
	CString str;
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );

	*pResult = 0;
}

void CPanelMCI::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	case IDC_SCROLLBAR_PixelRate:
		PixelRateSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_DShift_X:
		SegDShiftXSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_DShift_Y:
		SegDShiftYSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_Valley_X:
		SegValleyXSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_Valley_Y:
		SegValleyYSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_Rise_X:
		SegRiseXSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_Rise_Y:
		SegRiseYSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_DRise_X:
		SegDRiseXSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_DRise_Y:
		SegDRiseYSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_Peak_X:
		SegPeakXSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_Peak_Y:
		SegPeakYSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_Fall_X:
		SegFallXSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_Fall_Y:
		SegFallYSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_DFall_X:
		SegDFallXSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Seg_DFall_Y:
		SegDFallYSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_SpotX:
		SpotXSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_SpotY:
		SpotYSB_Change( pScrollBar->GetScrollPos() );
		break;
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

void CPanelMCI::PixelRateSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_DioData )->SetWindowText( str );
	dValue = nPos * g_dDwellTimeMin;	// 点逗留时间多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_PixelRate )->SetWindowText( str );

	int nValleyPos = (int)(50000 / (nPos * g_dDwellTimeMin*1000 ));
	if( nValleyPos < 1 )
		nValleyPos = 1;
	m_scrollSegValleyX.SetScrollPos( nValleyPos );
	SegValleyXSB_Change( nValleyPos );
	m_scrollSegValleyY.SetScrollPos( nValleyPos );
	SegValleyYSB_Change( nValleyPos );

	UpdateLineRate();
	UpdateFrameRate();
	int nPixClk;
	switch( nPos )
	{
	case	1:
		nPixClk = 0;	// 125
		break;
	case	2:
		nPixClk = 1;	// 250
		break;
	case	4:
		nPixClk = 2;	// 500
		break;
	case	8:
		nPixClk = 3;	// 1u
		break;
	case	16:
		nPixClk = 4;	// 2u
		break;
	case	40:
		nPixClk = 5;	// 5u
		break;
	case	80:
		nPixClk = 6;	// 10u
		break;
	case	128:
		nPixClk = 7;	// 16u
		break;
	case	160:
		nPixClk = 8;	// 20u
		break;
	case	320:
		nPixClk = 9;	// 40u
		break;
	default:
		nPixClk = -1;
		break;
	}
	if( nPixClk != m_nPixClk )
	{
		m_nPixClk = nPixClk;
		UpdateData( FALSE );
		PixClkChanged();
	}
}

void CPanelMCI::SegDShiftXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	// 目前使用14bit DAC
	int nMin, nMax;
	m_scrollSegDShiftX.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%04X", nPos -nMin );
	GetDlgItem( IDC_EDIT_Seg_DShift_X )->SetWindowText( str );
	UpdateOffsetX();
}

int CPanelMCI::UpdateOffsetX()
{
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	// Offset	= 8192 +DShift - Rise * dRise /2
	// Center	= 8192 +DShift;
	// Max		= 8192 +DShift + Rise * dRise /2
	int nMin, nMax;
	m_scrollSegDShiftX.GetScrollRange( &nMin, &nMax );
	int nDShift	= m_scrollSegDShiftX.GetScrollPos() -nMin;
	int nRise	= m_scrollSegRiseX.GetScrollPos();
	int nDRise	= m_scrollSegDRiseX.GetScrollPos();
	CString str;
	int nOffset	= g_n16bitDAC_Mid + nDShift - nRise *nDRise /2;
	str.Format( "%04X,%04X", nOffset, nOffset << 2 );
	GetDlgItem( IDC_EDIT_Seg_Offset_X )->SetWindowText( str );
	str.Format( "%04X", g_n16bitDAC_Mid + nDShift );
	GetDlgItem( IDC_EDIT_Seg_CenterX )->SetWindowText( str );
	str.Format( "%04X", g_n16bitDAC_Mid + nDShift + nRise *nDRise /2 -1 );
	GetDlgItem( IDC_EDIT_Seg_MaxX )->SetWindowText( str );
	return nOffset;
}

void CPanelMCI::SegDShiftYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	// 目前使用14bit DAC
	int nMin, nMax;
	m_scrollSegDShiftX.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%04X", nPos -nMin );
	GetDlgItem( IDC_EDIT_Seg_DShift_Y )->SetWindowText( str );
	UpdateOffsetY();
}

int CPanelMCI::UpdateOffsetY()
{
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	// Offset	= 8192 +DShift - Rise * dRise /2
	// Center	= 8192 +DShift;
	// Max		= 8192 +DShift + Rise * dRise /2
	int nMin, nMax;
	m_scrollSegDShiftY.GetScrollRange( &nMin, &nMax );
	int nDShift	= m_scrollSegDShiftY.GetScrollPos() -nMin;
	int nRise	= m_scrollSegRiseY.GetScrollPos();
	int nDRise	= m_scrollSegDRiseY.GetScrollPos();
	CString str;
	int nOffset	= g_n16bitDAC_Mid + nDShift - nRise *nDRise /2;
	str.Format( "%04X,%04X", nOffset, nOffset << 2 );
	GetDlgItem( IDC_EDIT_Seg_Offset_Y )->SetWindowText( str );
	str.Format( "%04X", g_n16bitDAC_Mid + nDShift );
	GetDlgItem( IDC_EDIT_Seg_CenterY )->SetWindowText( str );
	str.Format( "%04X", g_n16bitDAC_Mid + nDShift + nRise *nDRise /2 -1 );
	GetDlgItem( IDC_EDIT_Seg_MaxY )->SetWindowText( str );
	return nOffset;
}

void CPanelMCI::SegValleyXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_Valley_X )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_PixelRate )->GetWindowText( str );
	dValue = nPos * atof( str );	// 多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Valley_TP_X )->SetWindowText( str );

	UpdateLineRate();
	UpdateFrameRate();
}

void CPanelMCI::SegValleyYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_Valley_Y )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_PixelRate )->GetWindowText( str );
	dValue = nPos * atof( str );	// 多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Valley_TP_Y )->SetWindowText( str );
	UpdateFrameRate();
}

void CPanelMCI::SegRiseXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nMin, nMax;
	m_scrollSegRiseX.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_Rise_X )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_PixelRate )->GetWindowText( str );
	dValue = nPos * atof( str );	// 多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Rise_TP_X )->SetWindowText( str );

	// 更新DRise范围，保证Rise * dRise <= Max
	nPos = (g_n16bitDAC_Max+1) / nPos;
	m_scrollSegDRiseX.GetScrollRange( &nMin, &nMax );
	if( nPos != nMax -nMin +1 )
	{
		nMax = nPos;
		nPos = m_scrollSegDRiseX.GetScrollPos();
		m_scrollSegDRiseX.SetScrollRange( 1, nMax );
		if( nPos > nMax )
		{
			nPos = nMax;
			m_scrollSegDRiseX.SetScrollPos( nPos );
			SegDRiseXSB_Change( nPos );
		}
	}
	// 更新DFall范围
	nPos = m_scrollSegFallX.GetScrollPos();
	SegFallXSB_Change( nPos );
	// 更新DShift范围
	// The permitted range of DShift:  - 8K +(dRise*Rise)/2 <= DShift < 8K –(dRise*Rise)/2
	int nRise	= m_scrollSegRiseX.GetScrollPos();
	int nDRise	= m_scrollSegDRiseX.GetScrollPos();
	nPos		= m_scrollSegDShiftX.GetScrollPos();
	if( -g_n16bitDAC_Mid +(nRise *nDRise) /2 < 0 )
		nMin = -g_n16bitDAC_Mid;
	else
		nMin = -g_n16bitDAC_Mid +(nRise *nDRise) /2;
	if( g_n16bitDAC_Mid -(nRise *nDRise) /2 > g_n16bitDAC_Max -1 )
		nMax = g_n16bitDAC_Max -1;
	else
		nMax = g_n16bitDAC_Mid -(nRise *nDRise) /2;
	m_scrollSegDShiftX.SetScrollRange( nMin, nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;
	m_scrollSegDShiftX.SetScrollPos( nPos );
	SegDShiftXSB_Change( nPos );
	if( nMin == nMax )
	{
		m_scrollSegDShiftX.EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Seg_DShift_X )->EnableWindow( FALSE );
	}
	else
	{
		m_scrollSegDShiftX.EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_Seg_DShift_X )->EnableWindow( TRUE );
	}
/*	if( nMax -nMin + nRise *nDRise /2 > g_n16bitDAC_Max )
	{
		// 扫描波的Offset超出范围，需要调整
		nMin = 0 -g_n16bitDAC_Mid;
		nMax = g_n16bitDAC_Max -nRise *nDRise - g_n16bitDAC_Mid;
		nPos = m_scrollSegDShiftX.GetScrollPos();
		m_scrollSegDShiftX.SetScrollRange( nMin, nMax );
		if( nPos > nMax )
		{
			// 为保证同心，还有待计算
			nPos = nMax;
			m_scrollSegDShiftX.SetScrollPos( nPos );
		}
	}*/

	UpdateOffsetX();
	UpdateLineRate();
	UpdateFrameRate();
}

void CPanelMCI::SegRiseYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nMin, nMax;
	m_scrollSegRiseY.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_Rise_Y )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_PixelRate )->GetWindowText( str );
	dValue = nPos * atof( str );	// 多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Rise_TP_Y )->SetWindowText( str );

	// 更新DRise范围，保证Rise * dRise <= Max
	nPos = (g_n16bitDAC_Max+1) / nPos;
	m_scrollSegDRiseY.GetScrollRange( &nMin, &nMax );
	if( nPos != nMax -nMin +1 )
	{
		nMax = nPos;
		nPos = m_scrollSegDRiseY.GetScrollPos();
		m_scrollSegDRiseY.SetScrollRange( 1, nMax );
		if( nPos > nMax )
		{
			nPos = nMax;
			m_scrollSegDRiseY.SetScrollPos( nPos );
			SegDRiseYSB_Change( nPos );
		}
	}
	// 更新DFall范围
	nPos = m_scrollSegFallY.GetScrollPos();
	SegFallYSB_Change( nPos );
	// 更新DShift范围
	// The permitted range of DShift:  - 8K +(dRise*Rise)/2 <= DShift < 8K –(dRise*Rise)/2
	int nRise	= m_scrollSegRiseY.GetScrollPos();
	int nDRise	= m_scrollSegDRiseY.GetScrollPos();
	nPos		= m_scrollSegDShiftY.GetScrollPos();
	if( -g_n16bitDAC_Mid +(nRise *nDRise) /2 < 0 )
		nMin = -g_n16bitDAC_Mid;
	else
		nMin = -g_n16bitDAC_Mid +(nRise *nDRise) /2;
	if( g_n16bitDAC_Mid -(nRise *nDRise) /2 > g_n16bitDAC_Max -1 )
		nMax = g_n16bitDAC_Max -1;
	else
		nMax = g_n16bitDAC_Mid -(nRise *nDRise) /2;
	m_scrollSegDShiftY.SetScrollRange( nMin, nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;
	m_scrollSegDShiftY.SetScrollPos( nPos );
	SegDShiftYSB_Change( nPos );
	if( nMin == nMax )
	{
		m_scrollSegDShiftY.EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Seg_DShift_Y )->EnableWindow( FALSE );
	}
	else
	{
		m_scrollSegDShiftY.EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_Seg_DShift_Y )->EnableWindow( TRUE );
	}
/*	m_scrollSegDShiftY.GetScrollRange( &nMin, &nMax );
	int nRise	= m_scrollSegRiseY.GetScrollPos();
	int nDRise	= m_scrollSegDRiseY.GetScrollPos();
	if( nMax -nMin + nRise *nDRise /2 > g_n16bitDAC_Max )
	{
		// 扫描波的Offset超出范围，需要调整
		nMin = 0 -g_n16bitDAC_Mid;
		nMax = g_n16bitDAC_Max -nRise *nDRise - g_n16bitDAC_Mid;
		nPos = m_scrollSegDShiftY.GetScrollPos();
		m_scrollSegDShiftY.SetScrollRange( nMin, nMax );
		if( nPos > nMax )
		{
			// 为保证同心，还有待计算
			nPos = nMax;
			m_scrollSegDShiftY.SetScrollPos( nPos );
		}
	}
*/
	UpdateOffsetY();
	UpdateFrameRate();
}

void CPanelMCI::SegDRiseXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	int nMin, nMax;
	m_scrollSegDRiseX.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_DRise_X )->SetWindowText( str );
	str.Format( "%04X", nPos << 2 );
	GetDlgItem( IDC_EDIT_Seg_DRise_TP_X )->SetWindowText( str );

	// 更新DFall范围
	nPos = m_scrollSegFallX.GetScrollPos();
	SegFallXSB_Change( nPos );

	// 更新DShift范围
	// The permitted range of DShift:  - 8K +(dRise*Rise)/2 <= DShift < 8K –(dRise*Rise)/2
	int nRise	= m_scrollSegRiseX.GetScrollPos();
	int nDRise	= m_scrollSegDRiseX.GetScrollPos();
	nPos		= m_scrollSegDShiftX.GetScrollPos();
	if( -g_n16bitDAC_Mid +(nRise *nDRise) /2 < 0 )
		nMin = 0;
	else
		nMin = -g_n16bitDAC_Mid +(nRise *nDRise) /2;
	if( g_n16bitDAC_Mid -(nRise *nDRise) /2 > g_n16bitDAC_Max -1 )
		nMax = g_n16bitDAC_Max -1;
	else
		nMax = g_n16bitDAC_Mid -(nRise *nDRise) /2;
	m_scrollSegDShiftX.SetScrollRange( nMin, nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;
	m_scrollSegDShiftX.SetScrollPos( nPos );
	SegDShiftXSB_Change( nPos );
	if( nMin == nMax )
	{
		m_scrollSegDShiftX.EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Seg_DShift_X )->EnableWindow( FALSE );
	}
	else
	{
		m_scrollSegDShiftX.EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_Seg_DShift_X )->EnableWindow( TRUE );
	}

	UpdateOffsetX();
}

void CPanelMCI::SegDRiseYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	int nMin, nMax;
	m_scrollSegDRiseY.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_DRise_Y )->SetWindowText( str );
	str.Format( "%04X", nPos << 2 );
	GetDlgItem( IDC_EDIT_Seg_DRise_TP_Y )->SetWindowText( str );

	// 更新DFall范围
	nPos = m_scrollSegFallY.GetScrollPos();
	SegFallYSB_Change( nPos );

	// 更新DShift范围
	// The permitted range of DShift:  - 8K +(dRise*Rise)/2 <= DShift < 8K –(dRise*Rise)/2
	int nRise	= m_scrollSegRiseY.GetScrollPos();
	int nDRise	= m_scrollSegDRiseY.GetScrollPos();
	nPos		= m_scrollSegDShiftY.GetScrollPos();
	if( -g_n16bitDAC_Mid +(nRise *nDRise) /2 < 0 )
		nMin = 0;
	else
		nMin = -g_n16bitDAC_Mid +(nRise *nDRise) /2;
	if( g_n16bitDAC_Mid -(nRise *nDRise) /2 > g_n16bitDAC_Max -1 )
		nMax = g_n16bitDAC_Max -1;
	else
		nMax = g_n16bitDAC_Mid -(nRise *nDRise) /2;
	m_scrollSegDShiftY.SetScrollRange( nMin, nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;
	m_scrollSegDShiftY.SetScrollPos( nPos );
	SegDShiftYSB_Change( nPos );
	if( nMin == nMax )
	{
		m_scrollSegDShiftY.EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Seg_DShift_Y )->EnableWindow( FALSE );
	}
	else
	{
		m_scrollSegDShiftY.EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_Seg_DShift_Y )->EnableWindow( TRUE );
	}

	UpdateOffsetY();
}

void CPanelMCI::SegPeakXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_Peak_X )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_PixelRate )->GetWindowText( str );
	dValue = nPos * atof( str );	// 多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Peak_TP_X )->SetWindowText( str );
	UpdateLineRate();
	UpdateFrameRate();
}

void CPanelMCI::SegPeakYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_Peak_Y )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_PixelRate )->GetWindowText( str );
	dValue = nPos * atof( str );	// 多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Peak_TP_Y )->SetWindowText( str );
	UpdateFrameRate();
}

void CPanelMCI::SegFallXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nMin, nMax;
	m_scrollSegFallX.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_Fall_X )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_PixelRate )->GetWindowText( str );
	dValue = nPos * atof( str );	// 多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Fall_TP_X )->SetWindowText( str );

	// 更新DFall范围，保证Fall * dFall <= Rise * dRise
	int nRise	= m_scrollSegRiseX.GetScrollPos();
	int nDRise	= m_scrollSegDRiseX.GetScrollPos();
	m_scrollSegDFallX.GetScrollRange( &nMin, &nMax );
	if( nPos * (nMax-nMin+1) != nRise *nDRise )
	{
		if( nPos > 0 )
			nMax = nRise *nDRise / nPos /2;
		else
			nMax = nRise *nDRise /2;
		m_scrollSegDFallX.SetScrollRange( -1*nMax +1, nMax -1 );
		m_scrollSegDFallX.SetScrollPos( nMax -1 );
		SegDFallXSB_Change( nMax -1 );
	}

	UpdateLineRate();
	UpdateFrameRate();
}

void CPanelMCI::SegFallYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nMin, nMax;
	m_scrollSegFallY.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_Seg_Fall_Y )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_PixelRate )->GetWindowText( str );
	dValue = nPos * atof( str );	// 多少us
	str.Format( "%.1f", dValue );
	GetDlgItem( IDC_EDIT_Seg_Fall_TP_Y )->SetWindowText( str );

	// 更新DFall范围，保证Fall * dFall <= Rise * dRise
	int nRise	= m_scrollSegRiseY.GetScrollPos();
	int nDRise	= m_scrollSegDRiseY.GetScrollPos();
	m_scrollSegDFallY.GetScrollRange( &nMin, &nMax );
	if( nPos * (nMax-nMin+1) != nRise *nDRise )
	{
		if( nPos > 0 )
			nMax = nRise *nDRise / nPos /2;
		else
			nMax = nRise *nDRise /2;
		m_scrollSegDFallY.SetScrollRange( -1*nMax +1, nMax -1 );
		m_scrollSegDFallY.SetScrollPos( nMax -1 );
		SegDFallYSB_Change( nMax -1 );
	}

	UpdateFrameRate();
}

void CPanelMCI::SegDFallXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	int nMin, nMax;
	m_scrollSegDFallX.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%04X", nPos -nMin +1 );
	GetDlgItem( IDC_EDIT_Seg_DFall_X )->SetWindowText( str );
	str.Format( "%04X", (nPos -nMin +1) << 2 );
	GetDlgItem( IDC_EDIT_Seg_DFall_TP_X )->SetWindowText( str );
}

void CPanelMCI::SegDFallYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	int nMin, nMax;
	m_scrollSegDFallY.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%04X", nPos -nMin +1 );
	GetDlgItem( IDC_EDIT_Seg_DFall_Y )->SetWindowText( str );
	str.Format( "%04X", (nPos -nMin +1) << 2 );
	GetDlgItem( IDC_EDIT_Seg_DFall_TP_Y )->SetWindowText( str );
}

void CPanelMCI::SpotXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_spotX_HB )->SetWindowText( str );
}

void CPanelMCI::SpotYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_spotY_HB )->SetWindowText( str );
}

void CPanelMCI::Det03SB_Change( int nPos )
{
	UpdateData();
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

 	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%03X", nValue );
	GetDlgItem( IDC_EDIT_Det03 )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Offset_0 + m_nDet03Chn2].nCurPos = nPos;
	CControlLayer::Instance().m_BrdMCI.nOffset[m_nDet03Chn2] = 2048 -nPos;

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (m_nDet03Chn2 << 6)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det03StoreDAC, TRUE, wValue );
}

void CPanelMCI::Det45SB_Change( int nPos )
{
	UpdateData();
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	unsigned long DIOData = 0;
//	unsigned char output;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%03X", nValue );
	GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Offset_4 + m_nDet45Chn2].nCurPos = nPos;
	if( m_nDet45Chn2 < 2 )
		CControlLayer::Instance().m_BrdMCI.nOffset[m_nDet45Chn2 +4] = 2048 -nPos;

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );
    ///////////////////////////////////////////////////////////////////////////

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (m_nDet45Chn2 << 6)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
}

void CPanelMCI::DetGainSB_Change( int nPos )
{
	UpdateData();
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;

	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%d", nValue );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );
	CControlLayer::Instance().m_BrdMCI.nGain[m_nDetGainNo2] = nPos;
    ///////////////////////////////////////////////////////////////////////////

	WORD wValue = (WORD)( (m_nDetGainNo2 << 8) | (BYTE)nValue );
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetGain, TRUE, wValue );
}

void CPanelMCI::BWselSB_Change( int nPos )
{
	UpdateData();
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;

	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%d", nValue );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );
	CControlLayer::Instance().m_BrdMCI.nFilter[m_nDetPair2] = nPos;
    ///////////////////////////////////////////////////////////////////////////

	// filter pin in operand low byte
	// detector number in operand high byte
	WORD wValue = (WORD)( (m_nDetPair2 << 8) | (BYTE)nValue );
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
}

void CPanelMCI::OnDetPairSelect(UINT nID) 
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
	CSemCtrl::Instance().USB_WriteToBoard( USB_DetPairSelection, TRUE, wData );
	CControlLayer::Instance().m_BrdMCI.nDetPair = (int)wData;
	MCI_SyncParams();
}

void CPanelMCI::OnCHECKDet2nd() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Det2nd)))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams(4, nCheck);
	CSemVirtualMCICard::Instance().SetParams(SET_PARAMS_DetectorMode, nCheck);
	//	CSemCtrl::Instance().m_SemScan.USB_SetParams(SET_PARAMS_DetectorMode, nCheck);
	
	CControlLayer::Instance().m_BrdMCI.bDet2nd = nCheck;
	MCI_SyncParams();
}

void CPanelMCI::MCI_SyncParams()
{
	// Det Pair
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_23 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_45 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 + CControlLayer::Instance().m_BrdMCI.nDetPair )))->SetCheck(1);
	
	// Det 2nd
	((CButton*)(GetDlgItem(IDC_CHECK_Det2nd)))->SetCheck( CControlLayer::Instance().m_BrdMCI.bDet2nd );
	
	CString str;
	int nPos = 0;
	int nChn = 2 * CControlLayer::Instance().m_BrdMCI.nDetPair + CControlLayer::Instance().m_BrdMCI.bDet2nd;
	// Brightness
	// 1. scrollbar
	if( nChn < 4 )
	{
		m_nDet03Chn2 = nChn;
		nPos = 2048 -CControlLayer::Instance().m_BrdMCI.nOffset[ nChn ];
		m_scrollDet03.SetScrollPos( nPos );
		str.Format( "%03X", nPos );
		GetDlgItem( IDC_EDIT_Det03 )->SetWindowText( str );

		if( m_nDet45Chn2 < 2 )
		{
			nPos = 2048 -CControlLayer::Instance().m_BrdMCI.nOffset[ m_nDet45Chn2 +4 ];
			m_scrollDet45.SetScrollPos( nPos );
			str.Format( "%03X", nPos );
			GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );
		}
	}
	else
	{
		m_nDet45Chn2 = nChn -4;
		nPos = 2048 -CControlLayer::Instance().m_BrdMCI.nOffset[ nChn ];
		m_scrollDet45.SetScrollPos( nPos );
		str.Format( "%03X", nPos );
		GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );

		nPos = 2048 -CControlLayer::Instance().m_BrdMCI.nOffset[ m_nDet03Chn2 ];
		m_scrollDet03.SetScrollPos( nPos );
		str.Format( "%03X", nPos );
		GetDlgItem( IDC_EDIT_Det03 )->SetWindowText( str );
	}
	// 2. ctrl OCX
	CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_DetPair, CControlLayer::Instance().m_BrdMCI.nDetPair );
	CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_Det2nd, CControlLayer::Instance().m_BrdMCI.bDet2nd );
	CSemCoreAssemble::Instance().m_Signal2Brightness.SetParams( SET_PARAMS_DetPair, CControlLayer::Instance().m_BrdMCI.nDetPair );
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	Flag.lVal = 1;
	ClassID.lVal = SemControlPanelClass_Brightness;
	Xpos.lVal = CControlLayer::Instance().m_BrdMCI.nOffset[ nChn ];
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	ClassID.lVal = SemControlPanelClass_Signal2Brightness;
	Xpos.lVal = CControlLayer::Instance().m_BrdMCI.nOffset[ nChn +1 ];
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	// Gain
	m_nDetGainNo2 = nChn;
	nPos = CControlLayer::Instance().m_BrdMCI.nGain[ nChn ];
	m_scrollDetGain.SetScrollPos( nPos );
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );
	// Filter
	m_nDetPair2 = CControlLayer::Instance().m_BrdMCI.nDetPair;
	nPos = CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair];
	m_scrollBWsel.SetScrollPos( nPos );
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );

	UpdateData( FALSE );
}

void CPanelMCI::OnCHECKDebugZ()
{
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Z )))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams( 5, nCheck );
}

void CPanelMCI::OnCHECKDebugOsc() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Osc )))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams( 6, nCheck );
}

void CPanelMCI::OnCHECKDebugModeOsc90() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Osc_90 )))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams( 8, nCheck );
}

void CPanelMCI::OnCHECKDebugScan() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_DebugMode_Scan)))->GetCheck();
	CSemCtrl::Instance().m_SemScan.USB_SetParams( 5, nCheck );
}

void CPanelMCI::OnTestLED16on() 
{
	// Utilizes TestLED16State global, temporarily uses CL wobbler On ckt address (0x1c)
 	unsigned long DIOData = 0;
	CString str = "";
	CString strTemp = "";

	int nCheck = m_btnTestLED16State.GetCheck();
	if( nCheck )
	{
		m_btnTestLED16State.SetColor( g_crON_Test );
		m_btnTestLED16State.SetWindowText( "Test LEDs: now ON" );
	}
	else
	{
		m_btnTestLED16State.SetColor( g_crOFF );
		m_btnTestLED16State.SetWindowText( "Test LEDs: now OFF" );
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

void CPanelMCI::OnScanModeNormal() 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_ScanModeNormal, FALSE, 0 );
}

void CPanelMCI::OnScanModeLine() 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_ScanModeLineHorizontal, FALSE, 0 );
}

void CPanelMCI::OnScanModeSpot() 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_ScanModeSpot, FALSE, 0 );
}

void CPanelMCI::OnFreeze() 
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
	CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeX, TRUE, wData );

	// Freeze Y
	GetDlgItemText( IDC_EDIT_spotY_HB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData = (WORD)output << 8;

	GetDlgItemText( IDC_EDIT_spotY_LB, str );
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	wData |= output;
	CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeY, TRUE, wData );
}

void CPanelMCI::OnScanRotation() 
{
	// TODO: Add your control notification handler code here
	m_bRotate = !m_bRotate;
	if( m_bRotate )
	{
		m_btnRotate.SetColor( g_crON_Test );
		m_btnRotate.SetWindowText( "Scan Rotation ON!" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_RotationON, FALSE, 0 );
	}
	else
	{
		m_btnRotate.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnRotate.SetWindowText( "Scan Rotation OFF" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_RotationOFF, FALSE, 0 );
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

void CPanelMCI::OnScanRotationUpdate() 
{
	CString str;
	( GetDlgItem( IDC_EDIT_Rdegree ) )->GetWindowText( str );
	double dRadian	= atof( str ) * PI / 180;	// degree to radian
	WORD wSin		= (WORD)( sin( dRadian ) * 0x7FFF );
	WORD wCos		= (WORD)( cos( dRadian ) * 0x7FFF );

	CSemCtrl::Instance().USB_WriteToBoard( USB_RotationSetCos, TRUE, wCos );
	CSemCtrl::Instance().USB_WriteToBoard( USB_RotationSetSin, TRUE, wSin );
	CSemCtrl::Instance().USB_WriteToBoard( USB_UpdateRotation, FALSE, 0 );
}

void CPanelMCI::OnReadINTSourceRegister() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if( CSemCtrl::Instance().Serial_ReadbackData( m_nBoardID +1, USB_CID_VAC_INT_Source_Register, CBoardComm::Instance().m_pnReadCommonData ) )
	{
		CString str, strMsg;
		str.Format( "%04X, ", CBoardComm::Instance().m_pnReadCommonData[0] );
		strMsg += str;
		str.Format( "%04X", CBoardComm::Instance().m_pnReadCommonData[1] );
		strMsg += str;
		GetDlgItem( IDC_EDIT_ISR_IER )->SetWindowText( strMsg );
	}
	else
		GetDlgItem( IDC_EDIT_ISR_IER )->SetWindowText( "" );
}

void CPanelMCI::OnWriteINTEnableRegister() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	unsigned char output;
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

	CSemCtrl::Instance().USB_CommToBoards2(	bOP1,
										bOP2,
										bOp3,
										bOp4 );
}

void CPanelMCI::OnINT_ClearDB() 
{
	// Clear port B & D
	CSemCtrl::Instance().USB_WriteToBoard( USB_ClearDB, FALSE, 0 );
}

void CPanelMCI::OnINT_SetIMR() 
{
	// Set interrupt mask register
	CString str;
	GetDlgItemText( IDC_EDIT_Interrupt_Mask_Register, str );

	unsigned char output;
	char *buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetIMR, TRUE, output );
}

void CPanelMCI::OnINT_INTtest() 
{
	// INT test
	int nCheck = m_btnINTtest.GetCheck();
	if( nCheck )
	{
		m_btnINTtest.SetColor( g_crEmphasis3 );
		m_btnINTtest.SetWindowText( "INT test ON" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_INT_test_ON, FALSE, 0 );

		UpdateData();
		CSemCtrl::Instance().USB_WriteToBoard( USB_INT_test_SPR +(3-m_nBoardID), FALSE, 0 );
	}
	else
	{
		m_btnINTtest.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnINTtest.SetWindowText( "INT test OFF" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_INT_test_OFF, FALSE, 0 );
	}
}

void CPanelMCI::OnCHECKMainsLock() 
{
	// Toggle MainsLock
	int nCheck = m_btnMainsLock.GetCheck();
	if( nCheck )
	{
		m_btnMainsLock.SetColor( g_crEmphasis1 );
		m_btnMainsLock.SetWindowText( "MainsLock ON" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_MainsLock_ON, FALSE, 0 );
	}
	else
	{
		m_btnMainsLock.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnMainsLock.SetWindowText( "MainsLock OFF" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_MainsLock_OFF, FALSE, 0 );
	}
}

void CPanelMCI::OnBUTTONFakeMainslock() 
{
	// TODO: Add your control notification handler code here
	// Toggle MainsLock
	int nCheck = m_btnFakeMains.GetCheck();
	if( nCheck )
	{
		m_btnFakeMains.SetColor( g_crEmphasis1 );
		m_btnFakeMains.SetWindowText( "FakeMains ON" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_FakeMains_ON, FALSE, 0 );
	}
	else
	{
		m_btnFakeMains.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnFakeMains.SetWindowText( "FakeMains OFF" );
		CSemCtrl::Instance().USB_WriteToBoard( USB_FakeMains_OFF, FALSE, 0 );
	}
}

void CPanelMCI::OnBUTTONMainsLockLineSync() 
{
	// TODO: Add your control notification handler code here
//	int nCheck = m_btnFakeMains.GetCheck();
//	if( nCheck == 0 )
//		return;
	
	CSemCtrl::Instance().USB_WriteToBoard( USB_MainsLock_LineSync, FALSE, 0 );
}

void CPanelMCI::OnBUTTONMainsLockFrameSync() 
{
	// TODO: Add your control notification handler code here
//	int nCheck = m_btnFakeMains.GetCheck();
//	if( nCheck == 0 )
//		return;
	
	CSemCtrl::Instance().USB_WriteToBoard( USB_MainsLock_FrameSync, FALSE, 0 );
}

void CPanelMCI::OnCHECKVideo() 
{
	// Toggle Video
	int nCheck = m_btnVideo.GetCheck();
	if( nCheck )
	{
		m_btnVideo.SetColor( g_crEmphasis2 );
		m_btnVideo.SetWindowText( "Video ON" );

		CSemCtrl::Instance().USB_StreamOpen( TRUE );	// TRUE for read; FALSE for write.
		m_nusbPixRate	= 80;
//		CSemCtrl::Instance().USB_WriteToBoard( USB_SetPixelRate, TRUE, m_nusbPixRate );
//		CSemCtrl::Instance().USB_WriteToBoard( USB_Video_ON, FALSE, 0 );
		CSemCtrl::Instance().USB_VideoON();
	}
	else
	{
		m_btnVideo.SetColor( ::GetSysColor(COLOR_3DFACE) );
		m_btnVideo.SetWindowText( "Video OFF" );

		CSemCtrl::Instance().USB_StreamClose();
		m_nusbPixRate	= 2;
//		CSemCtrl::Instance().USB_WriteToBoard( USB_SetPixelRate, TRUE, m_nusbPixRate );
//		CSemCtrl::Instance().USB_WriteToBoard( USB_Video_OFF, FALSE, 0 );
		CSemCtrl::Instance().USB_VideoOFF();
	}
}





void CPanelMCI::InitDiagnose( BOOL bEnd )
{
}

void CPanelMCI::SetTimers( UINT nIDEvent, BOOL bSet )
{
	if( bSet )
	{
		switch( nIDEvent )
		{
		case	Diag_Offset_0:
			m_nDet03Chn2 = 0;
			m_scrollDet03.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			Det03SB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_1:
			m_nDet03Chn2 = 1;
			m_scrollDet03.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			Det03SB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_2:
			m_nDet03Chn2 = 2;
			m_scrollDet03.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			Det03SB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_3:
			m_nDet03Chn2 = 3;
			m_scrollDet03.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			Det03SB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_4:
			m_nDet45Chn2 = 0;
			m_scrollDet45.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			Det45SB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Offset_5:
			m_nDet45Chn2 = 1;
			m_scrollDet45.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			Det45SB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_FineMag_X:
			m_nDet45Chn2 = 2;
			m_scrollDet45.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			Det45SB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_FineMag_Y:
			m_nDet45Chn2 = 3;
			m_scrollDet45.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			Det45SB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Pos_X:
			m_scrollSpotX.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			SpotXSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		case	Diag_Pos_Y:
			m_scrollDet45.SetScrollPos( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			SpotYSB_Change( CControlLayer::Instance().m_DL.diag[nIDEvent].nMin );
			break;
		default:
			break;
		}
		//		SetTimer( nIDEvent +100, CControlLayer::Instance().m_DL.diag[nIDEvent].nIntv, NULL );
	}
	else
		KillTimer( nIDEvent +100 );
}
