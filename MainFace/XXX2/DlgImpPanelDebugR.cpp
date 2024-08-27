// DlgImpPanelDebugR.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgImpPanelDebugR.h"
#include "ScanView.h"
#include "DlgInput.h"
#include "ControlLayer.h"
#include "ScaleManager.h"
#include "ScanParameterManager.h"
#include "BoardComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double g_dDwellTimeMin = 0.125 *4;

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebugR dialog


CDlgImpPanelDebugR::CDlgImpPanelDebugR(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImpPanelDebugR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImpPanelDebugR)
	m_nReso = -1;
	m_nPixClk = -1;
	//}}AFX_DATA_INIT
	m_nusbPixRate	= 1;
	m_bTrialParam1	= FALSE;
	m_bTrialParam2	= FALSE;
	m_bTrialParam3	= FALSE;
}


void CDlgImpPanelDebugR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImpPanelDebugR)
	DDX_Control(pDX, IDC_SCROLLBAR_SpotY, m_scrollSpotY);
	DDX_Control(pDX, IDC_SCROLLBAR_SpotX, m_scrollSpotX);
	DDX_Control(pDX, IDC_CHECK_MainsLock, m_btnMainsLock);
	DDX_Control(pDX, IDC_SCROLLBAR_BW_sel, m_scrollBWsel);
	DDX_Control(pDX, IDC_SCROLLBAR_DetGain, m_scrollDetGain);
	DDX_Control(pDX, IDC_SCROLLBAR_DetGain2, m_scrollDetGain2);
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
	DDX_Radio(pDX, IDC_RADIO_Resolution1, m_nReso);
	DDX_Radio(pDX, IDC_RADIO_PixClkEq_1, m_nPixClk);
	DDX_Control(pDX, IDC_SCROLLBAR_SPRDACsValSB, m_scrollSPRDACsVal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImpPanelDebugR, CDialog)
	//{{AFX_MSG_MAP(CDlgImpPanelDebugR)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_Set_Pixel_Rate, OnSetPixelRate)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Start, OnScanStart)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Stop, OnScanStop)
	ON_BN_CLICKED(IDC_BUTTON_UpdateX, OnUpdateX)
	ON_BN_CLICKED(IDC_BUTTON_UpdateY, OnUpdateY)
	ON_BN_CLICKED(IDC_CHECK_MainsLock, OnCHECKMainsLock)
	ON_BN_CLICKED(IDC_BUTTON_RampSeg, OnUpdateRamp)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreezeX, OnSpotSetX)
	ON_BN_CLICKED(IDC_BUTTON_cmdFreezeY, OnSpotSetY)
	ON_BN_CLICKED(IDC_CHECK_Det2nd, OnCHECKDet2nd)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Scan, OnCHECKDebugScan)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Z, OnCHECKDebugZ)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Osc, OnCHECKDebugOsc)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Osc_90, OnCHECKDebugModeOsc90)
	ON_BN_CLICKED(IDC_CHECK_NeedMon, OnCHECKNeedMon)
	ON_BN_CLICKED(IDC_6K_BUTTON_Register, OnGenerateKey)
	ON_BN_CLICKED(IDC_BUTTON_SPRDACupdateCmd, OnSPRDACupdate)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_Resolution1, IDC_RADIO_Resolution7, OnResolutionChanged)
	ON_COMMAND_RANGE(IDC_RADIO_PixClkEq_1, IDC_RADIO_PixClkEq_10, OnPixClkChanged)
	ON_COMMAND_RANGE(IDC_RADIO_DetPair_Sel_01, IDC_RADIO_DetPair_Sel_45, OnDetPairSelect)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Debug_VideoGraph, OnDeltaposSPINVideoGraph)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Debug_VideoGraph_Offset, OnDeltaposSPINVideoGraphOffset)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebugR message handlers

BOOL CDlgImpPanelDebugR::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	m_scrollDetGain.SetScrollRange( 0, 7 );
	m_scrollDetGain2.SetScrollRange( 0, 7 );
	m_scrollBWsel.SetScrollRange( 0, 7 );
	m_scrollDetGain.SetScrollPos( 0 );
	m_scrollDetGain2.SetScrollPos( 0 );
	m_scrollBWsel.SetScrollPos( 0 );

	m_btnMainsLock.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnMainsLock.SetWindowText( "MainsLock OFF" );

	m_debugOSC.nOscillIndex	= 1;		// 13.04.03 Debug调试时示波器Y向档位，起始值为1，即范围为+/-2.5V
	m_debugOSC.dOffset		= 0.0;		// 13.08.05 Debug调试时示波器Y向偏移量
	GetDlgItem(IDC_EDIT_Debug_VideoGraph)->SetWindowText( "0.0" );

	m_nSPRDACnewVal = 0;
	m_scrollSPRDACsVal.SetScrollRange( -32768, 32767 );

	UpdateData( FALSE );
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgImpPanelDebugR::Init()
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


BOOL CDlgImpPanelDebugR::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_DetGain:
		case	IDC_EDIT_DetGain2:
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
		case	IDC_EDIT_Debug_VideoGraph:
		case	IDC_EDIT_SPRDACsValHexTB:
			GetDlgItem( nID )->GetWindowText( str );
			break;
		default:
			break;
		}
		if( str.IsEmpty() )
			return CDialog::PreTranslateMessage(pMsg);
		int nPos;
		double dPos;
		char *bufNum;
		switch( nID )
		{
		case	IDC_EDIT_Seg_DShift_X:
		case	IDC_EDIT_Seg_DShift_Y:
		case	IDC_EDIT_Seg_DFall_X:
		case	IDC_EDIT_Seg_DFall_Y:
		case	IDC_EDIT_SPRDACsValHexTB:
			bufNum = str.GetBuffer(5);
			sscanf(bufNum, "%04X", &nPos);
			break;
		case	IDC_EDIT_DetGain:
		case	IDC_EDIT_DetGain2:
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
		case	IDC_EDIT_Debug_VideoGraph:
			dPos = atof( str );
			break;
		default:
			break;
		}
		switch( nID )
		{
		case	IDC_EDIT_DetGain:
			m_scrollDetGain.SetScrollPos( nPos );
			DetGainSB_Change( nPos );
			break;
		case	IDC_EDIT_DetGain2:
			m_scrollDetGain2.SetScrollPos( nPos );
			DetGain2SB_Change( nPos );
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
		case	IDC_EDIT_Debug_VideoGraph:
			m_debugOSC.dOffset = dPos;
			CSemVirtualMCICard::Instance().SetParams( 9, (int)(dPos * 10000) );
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

void CDlgImpPanelDebugR::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	case IDC_SCROLLBAR_DetGain:
		DetGainSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_DetGain2:
		DetGain2SB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_BW_sel:
		BWselSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_SPRDACsValSB:
		SPRDACsValUpdae( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}




/////////////////////////////////////////////////////////////////////
// 扫描速度设置
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebugR::OnSetPixelRate() 
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

void CDlgImpPanelDebugR::OnPixClkChanged(UINT uID)
{
	UpdateData();
	PixClkChanged();
}

void CDlgImpPanelDebugR::PixClkChanged()
{
	COLORREF clr = ::GetSysColor(COLOR_3DFACE);

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

void CDlgImpPanelDebugR::PixelRateSB_Change( int nPos )
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




/////////////////////////////////////////////////////////////////////
// 线扫、点扫设置
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebugR::OnSpotSetX() 
{
	// TODO: Add your control notification handler code here
	int nPos = m_scrollSpotX.GetScrollPos();
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
	{
		pView->SetLineScanParamManually( FALSE, nPos );
		if ( (CScanParameterManager::Instance().m_nusbScanType) == USB_ST_LineScan_VERT )
			pView->USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
	}
//	CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeX, TRUE, (WORD)m_scrollSpotX.GetScrollPos() );
}

void CDlgImpPanelDebugR::UpdateSpotX( int nPos, BOOL bUpdate )
{
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_spotX_HB )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Pos_X].nCurPos = nPos;

	if( bUpdate )
		CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeX, TRUE, (WORD)nPos );
}

void CDlgImpPanelDebugR::OnSpotSetY() 
{
	// TODO: Add your control notification handler code here
	int nPos = m_scrollSpotY.GetScrollPos();
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
	{
		pView->SetLineScanParamManually( TRUE, nPos );
		if ( (CScanParameterManager::Instance().m_nusbScanType) == USB_ST_LineScan_HORZ )
			pView->USB_ChangeScan( CScanParameterManager::Instance().m_nusbScanType );
	}
//	CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeY, TRUE, (WORD)m_scrollSpotY.GetScrollPos() );
}

void CDlgImpPanelDebugR::UpdateSpotY( int nPos, BOOL bUpdate )
{
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_spotY_HB )->SetWindowText( str );
	CControlLayer::Instance().m_DL.diag[Diag_Pos_Y].nCurPos = nPos;

	if( bUpdate )
		CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeY, TRUE, (WORD)nPos );
}




/////////////////////////////////////////////////////////////////////
// 快速分辨率设置
/////////////////////////////////////////////////////////////////////

CSize CDlgImpPanelDebugR::GetSizes()
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

void CDlgImpPanelDebugR::OnResolutionChanged(UINT uID)
{
	UpdateData();

	COLORREF clr = ::GetSysColor(COLOR_3DFACE);

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
	int i=0;
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




/////////////////////////////////////////////////////////////////////
// 扫描波形设置
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebugR::OnScanStart() 
{
	ScanStart();
}

void CDlgImpPanelDebugR::ScanStart()
{
	// Clear FIFO
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();//TIME_METHOD_WAIT_INPUT_ENABLE);
	// Start scan
	CSemCtrl::Instance().USB_WriteToBoard( USB_StartScan, FALSE, 0 );
}

void CDlgImpPanelDebugR::OnScanStop() 
{
	ScanStop();
}

void CDlgImpPanelDebugR::ScanStop()
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_StopScan, FALSE, 0 );
}

void CDlgImpPanelDebugR::UpdateLineRate()
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

void CDlgImpPanelDebugR::UpdateFrameRate()
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

void CDlgImpPanelDebugR::OnUpdateX() 
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

// 	usbParam.wOffset		= usbParam.wOffset << 2;
// 	usbParam.wDRise			= usbParam.wDRise << 2;
// 	usbParam.wDFall			= usbParam.wDFall << 2;

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

void CDlgImpPanelDebugR::OnUpdateY() 
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

// 	usbParam.wOffset		= usbParam.wOffset << 2;
// 	usbParam.wDRise			= usbParam.wDRise << 2;
// 	usbParam.wDFall			= usbParam.wDFall << 2;

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

void CDlgImpPanelDebugR::OnUpdateRamp() 
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

void CDlgImpPanelDebugR::SegDShiftXSB_Change( int nPos )
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

int CDlgImpPanelDebugR::UpdateOffsetX()
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

void CDlgImpPanelDebugR::SegDShiftYSB_Change( int nPos )
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

int CDlgImpPanelDebugR::UpdateOffsetY()
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

void CDlgImpPanelDebugR::SegValleyXSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegValleyYSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegRiseXSB_Change( int nPos )
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

	UpdateOffsetX();
	UpdateLineRate();
	UpdateFrameRate();
}

void CDlgImpPanelDebugR::SegRiseYSB_Change( int nPos )
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

	UpdateOffsetY();
	UpdateFrameRate();
}

void CDlgImpPanelDebugR::SegDRiseXSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegDRiseYSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegPeakXSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegPeakYSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegFallXSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegFallYSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegDFallXSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SegDFallYSB_Change( int nPos )
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

void CDlgImpPanelDebugR::SpotXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_spotX_HB )->SetWindowText( str );
}

void CDlgImpPanelDebugR::SpotYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	double dValue = 0.0;
	CString str = "";

	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_spotY_HB )->SetWindowText( str );
}



/////////////////////////////////////////////////////////////////////
// MCI其它功能
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebugR::DetGainSB_Change( int nPos )
{
	int nValue;
	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );
	CControlLayer::Instance().m_BrdMCI.nGain[ 2*CControlLayer::Instance().m_BrdMCI.nDetPair ] = nPos;
    ///////////////////////////////////////////////////////////////////////////

	WORD wValue = (BYTE)nValue;
	if( CControlLayer::Instance().m_BrdMCI.nDetPair != 0 )
		wValue |= ( (WORD)(pow((double)2, CControlLayer::Instance().m_BrdMCI.nDetPair)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetGain, TRUE, wValue );

	str.Format( "Press select gain: %X", nValue );
	theApp.DisplayMsg(str);
	MCI_UpdateCfg( 1 );
}

void CDlgImpPanelDebugR::DetGain2SB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_DetGain2 )->SetWindowText( str );
	CControlLayer::Instance().m_BrdMCI.nGain[ 2*CControlLayer::Instance().m_BrdMCI.nDetPair +1 ] = nPos;
	///////////////////////////////////////////////////////////////////////////

	WORD wValue = (BYTE)nValue;
	if( CControlLayer::Instance().m_BrdMCI.nDetPair == 0 )
		wValue |= ( (WORD)(pow((double)2, CControlLayer::Instance().m_BrdMCI.nDetPair)) << 8);
	else
		wValue |= ( (WORD)(pow((double)2, CControlLayer::Instance().m_BrdMCI.nDetPair) +1) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetGain, TRUE, wValue );

	str.Format( "Press select gain 2: %X", nValue );
	theApp.DisplayMsg(str);
	MCI_UpdateCfg( 2 );
}

void CDlgImpPanelDebugR::BWselSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	unsigned long DIOData = 0;
	CString str = "";

    ///////////////////////////////////////////////////////////////////////////
	nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );
	CControlLayer::Instance().m_BrdMCI.nFilter[ CControlLayer::Instance().m_BrdMCI.nDetPair ] = nPos;
    ///////////////////////////////////////////////////////////////////////////

	WORD wValue = (BYTE)nValue;				// filter pin in operand low byte
	wValue |= (((BYTE)CControlLayer::Instance().m_BrdMCI.nDetPair) << 8 );	// detector number in operand high byte
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );

	str.Format( "Press select filter: %X", nValue );
	theApp.DisplayMsg(str);
	MCI_UpdateCfg( 3 );
}

void CDlgImpPanelDebugR::OnDetPairSelect(UINT uID) 
{
	// TODO: Add your control notification handler code here
	WORD wData = 0x0;
	int nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->GetCheck();
	if( nCheck )
	{
		wData = 0x00;
		theApp.DisplayMsg("Press det pair: select 01");
	}
	else
	{
		nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_23 )))->GetCheck();
		if( nCheck )
		{
			wData = 0x01;
			theApp.DisplayMsg("Press det pair: select 23");
		}
		else
		{
			nCheck = ((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_45 )))->GetCheck();
			if( nCheck )
			{
				wData = 0x02;
				theApp.DisplayMsg("Press det pair: select 45");
			}
		}
	}
	CSemCtrl::Instance().USB_WriteToBoard( USB_DetPairSelection, TRUE, wData );
	CControlLayer::Instance().m_BrdMCI.nDetPair = (int)wData;
	MCI_SyncParams();
	MCI_UpdateCfg( 0 );
}

void CDlgImpPanelDebugR::OnCHECKDet2nd() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Det2nd)))->GetCheck();
	CControlLayer::Instance().CheckDet2nd( nCheck );
	if( nCheck )
		theApp.DisplayMsg("Press 2nd: select 1,3,5 chn");
	else
		theApp.DisplayMsg("Press 2nd: select 0,2,4 chn");
	MCI_UpdateCfg( 0 );
}

void CDlgImpPanelDebugR::MCI_SyncParams()
{
	// Det Pair
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_23 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_45 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_DetPair_Sel_01 + CControlLayer::Instance().m_BrdMCI.nDetPair )))->SetCheck(1);

	// Det 2nd
	((CButton*)(GetDlgItem(IDC_CHECK_Det2nd)))->SetCheck( CControlLayer::Instance().m_BrdMCI.bDet2nd );

	CString str;
	int nPos;
	// Gain
	nPos = CControlLayer::Instance().m_BrdMCI.nGain[ 2 * CControlLayer::Instance().m_BrdMCI.nDetPair ];
	m_scrollDetGain.SetScrollPos( nPos );
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_DetGain )->SetWindowText( str );
	// Gain2
	nPos = CControlLayer::Instance().m_BrdMCI.nGain[ 2 * CControlLayer::Instance().m_BrdMCI.nDetPair +1 ];
	m_scrollDetGain2.SetScrollPos( nPos );
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_DetGain2 )->SetWindowText( str );
	// Filter
	nPos = CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair];
	m_scrollBWsel.SetScrollPos( nPos );
	str.Format( "%d", nPos );
	GetDlgItem( IDC_EDIT_BW_sel2 )->SetWindowText( str );
}

void CDlgImpPanelDebugR::MCI_UpdateCfg( int nIndex )
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	int nValue;
	CString str;
	switch( nIndex )
	{
	case	0:	// Det Pair (Channel)
		nValue	= CControlLayer::Instance().m_BrdMCI.nDetPair * 2 + (int)(CControlLayer::Instance().m_BrdMCI.bDet2nd);
		str.Format( "%d", nValue );
		WritePrivateProfileString( "Options", "Channel", str, path );
		CSemCtrl::Instance().m_SemScan.USB_SetParams( 32, CControlLayer::Instance().m_BrdMCI.nDetPair );
		break;
	case	1:	// Gain
	case	2:	// Gain2
		str.Format( "%d,%d", CControlLayer::Instance().m_BrdMCI.nGain[ 2 * CControlLayer::Instance().m_BrdMCI.nDetPair ],
							CControlLayer::Instance().m_BrdMCI.nGain[ 2 * CControlLayer::Instance().m_BrdMCI.nDetPair +1 ]);
		WritePrivateProfileString( "Options", "Gain", str, path );
		break;
	case	3:	// Filter
		str.Format( "%d", CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair] );
		WritePrivateProfileString( "Options", "Filter", str, path );
		CSemCtrl::Instance().m_SemScan.USB_SetParams( 33 +CControlLayer::Instance().m_BrdMCI.nDetPair, CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair] );
		break;
	}
}

void CDlgImpPanelDebugR::OnCHECKMainsLock() 
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




/////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebugR::OnCHECKDebugZ()
{
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Z )))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams( 5, nCheck );
}

void CDlgImpPanelDebugR::OnCHECKDebugOsc() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Osc )))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams( 6, nCheck );
}

void CDlgImpPanelDebugR::OnCHECKDebugModeOsc90() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Osc_90 )))->GetCheck();
	CSemVirtualMCICard::Instance().SetParams( 8, nCheck );
}

void CDlgImpPanelDebugR::OnCHECKDebugScan() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_DebugMode_Scan)))->GetCheck();
	CSemCtrl::Instance().m_SemScan.USB_SetParams( 5, nCheck );
}

void CDlgImpPanelDebugR::OnCHECKNeedMon() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_NeedMon )))->GetCheck();
	CSemCtrl::Instance().m_SemScan.USB_SetParams( SET_PARAMS_NeedMon, nCheck );
}





/////////////////////////////////////////////////////////////////////
// Oscillograph
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebugR::OnDeltaposSPINVideoGraph(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		m_debugOSC.nOscillIndex++;
	else
		m_debugOSC.nOscillIndex--;
	if( m_debugOSC.nOscillIndex > 9 )
		m_debugOSC.nOscillIndex = 9;
	if( m_debugOSC.nOscillIndex < 1 )
		m_debugOSC.nOscillIndex = 1;

	CSemVirtualMCICard::Instance().SetParams( 7, m_debugOSC.nOscillIndex );
	*pResult = 0;
}

void CDlgImpPanelDebugR::OnDeltaposSPINVideoGraphOffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		m_debugOSC.dOffset -= 5;
	else
		m_debugOSC.dOffset += 5;

	CString str;
	str.Format( "%.4f", m_debugOSC.dOffset );
	GetDlgItem(IDC_EDIT_Debug_VideoGraph)->SetWindowText( str );
	CSemVirtualMCICard::Instance().SetParams( 9, (int)(m_debugOSC.dOffset * 10000) );
	*pResult = 0;
}



/////////////////////////////////////////////////////////////////////
// Trial
/////////////////////////////////////////////////////////////////////
void CDlgImpPanelDebugR::SetupDebugParam( BOOL bDebugHV, BOOL bDebugVac, BOOL bDebugStage )
{
	m_bTrialParam1	= bDebugHV;
	m_bTrialParam2	= bDebugVac;
	m_bTrialParam3	= bDebugStage;
	ShowGenerate();
}

void CDlgImpPanelDebugR::ShowGenerate()
{
	if( m_bTrialParam1 && m_bTrialParam2 && m_bTrialParam3 && theApp.m_dwOpr > 0 )
		GetDlgItem( IDC_6K_BUTTON_Register )->ShowWindow( TRUE );
	else
		GetDlgItem( IDC_6K_BUTTON_Register )->ShowWindow( FALSE );
}

void CDlgImpPanelDebugR::OnGenerateKey() 
{
	// 生成密钥
	/*生成纯字母随机字符串
	srand(time(NULL));
	char cKey[20+1];
	for( int i=0; i<20; i++ )
		cKey[i] = 'A' +rand() %26;
	cKey[i] = '\0';*/

	// 先生成一个20长度的包含大写字母和数字的随机字符串
	// 格式为XXXXX-XXXXX-XXXXX-XXXXX
	int flag, i;  
	char cKey[20 +3 +1];
    srand((unsigned) time(NULL ));  
    for( i=0; i<20 +3; i++ )  
    {
		if( (i != 0) && ((i+1) % 6 == 0) )
		{
			cKey[i] = '-';
			continue;
		}
        flag = rand() % 3;  
        switch (flag)  
        {  
		case 0:  
		case 1:  
			cKey[i] = 'A' + rand() % 26;  
			break;  
//		case 1:  
//			cKey[i] = 'a' + rand() % 26;  
//			break;  
		case 2:  
			cKey[i] = '0' + rand() % 10;  
			break;  
		default:  
			cKey[i] = 'X';  
			break;  
        }
	}
	cKey[i] = '\0';
	// 指定位置插入正式版本信息
	// 格式为XX5XX-XXX2X-X6XXX-XXXX8
	cKey[2]		= '5';
	cKey[9]		= '2';
	cKey[13]	= '6';
	cKey[22]	= '8';

	// 显示
	CString str;
	str.Format( "%s", cKey );
	CDlgInput dlg;
	dlg.m_strInput = str;
	dlg.DoModal();
}


/////////////////////////////////////////////////////////////////////
// Wobbler
/////////////////////////////////////////////////////////////////////

void CDlgImpPanelDebugR::SPRDACsValUpdae( int nPos )
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
	str.Format( "%5.3f, %5.3f", 2.5 *m_nSPRDACnewVal /65536, 10.0 *(65535-m_nSPRDACnewVal) /65535 );
	GetDlgItem( IDC_EDIT_SPRDACvoltsTB )->SetWindowText( str );
	str.Format( "%04X", m_nSPRDACnewVal );
	GetDlgItem( IDC_EDIT_SPRDACsValHexTB )->SetWindowText( str );
	CControlLayer::Instance().m_nSPRDACvalues[3] = nPos;	// Keep history of user setting
	CControlLayer::Instance().m_DL.diag[Diag_SPRDAC_ChA + 3].nCurPos = nPos;
}

void CDlgImpPanelDebugR::OnSPRDACupdate() 
{
	SPRDACupdate();
}

void CDlgImpPanelDebugR::SPRDACupdate()
{
	BYTE bSPRDAChibyte, bSPRDAClobyte, bSPRDACfirst24;
	DWORD dwWdata;

	bSPRDAChibyte	= (BYTE)((m_nSPRDACnewVal & 0xFF00) / 256);	// Shift down 8 bits to properly byte-align
	bSPRDAClobyte	= (BYTE)(m_nSPRDACnewVal & 0xFF);
	bSPRDACfirst24	= (BYTE)( 0x10 | (3 * 2) );	// Selects immediate load, channel select shifted up 1 bit

	dwWdata = (DWORD)(bSPRDACfirst24 * 65536 + m_nSPRDACnewVal);

	CString str;
	str.Format( "CCD Power: sent 0x%X, %.3f V, %.3f V", 
				(DWORD)(bSPRDACfirst24 *65536 + bSPRDAChibyte *256 + bSPRDAClobyte),
				2.5 *m_nSPRDACnewVal /65536, 10.0 *(65535-m_nSPRDACnewVal) /65535 );
//	GetDlgItem( IDC_EDIT_SPRDACsentTB )->SetWindowText( str );
	theApp.DisplayMsg(str);

	CBoardComm::Instance().BoardWrite24(24, USB_MID_SPR, USB_CID_SPR_General_Purpose_DACs, dwWdata);
}