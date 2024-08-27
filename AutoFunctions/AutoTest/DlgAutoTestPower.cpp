// DlgAutoTestPower.cpp : implementation file
//

#include "stdafx.h"
#include "AutoTest.h"
#include "DlgAutoTestPower.h"
#include "MsWord9.h"
#include "SMVac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//int g_AutoTestPowerSPR0_order[16] = {8,0,9,1,10,2,11,3,12,4,13,5,14,6,15,7};
int g_AutoTestPowerSPR0_order[14] = {8,0,9,1,10,2,11,3,12,4,13,5,14,6};
/////////////////////////////////////////////////////////////////////////////
// MCI Power readback
const	WORD	g_wPwrbitsRange	= 0x0FFF;	// 4095
const	double	g_dPwrbitsRange	= 4.096;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestPower dialog


CDlgAutoTestPower::CDlgAutoTestPower(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTestPower::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTestPower)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgAutoTestPower::~CDlgAutoTestPower()
{
	m_brushError.DeleteObject();
}

void CDlgAutoTestPower::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTestPower)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTestPower, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTestPower)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Power_Start, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Power_Clear, OnClear)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Power_Report, OnReport)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestPower message handlers

HBRUSH CDlgAutoTestPower::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		int nID = pWnd->GetDlgCtrlID();
		int nIndex = nID - IDC_STATIC_AT_Cali_Power_SCN10;
		if( nID == IDC_STATIC_AT_Cali_Power_SCN10 +nIndex
			&& nIndex >=0 && nIndex <= 35
			&& theApp.m_AT.pPower_OutofRange[nIndex] )
		{
			pDC->SetTextColor(RGB(255,255,255));	// 设置文本颜色  
			pDC->SetBkMode(TRANSPARENT);			// 设置背景透明
			hbr = (HBRUSH)m_brushError;
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));			// 设置文本颜色  
			pDC->SetBkMode(TRANSPARENT);			// 设置背景透明
		}
	}

	return hbr;
}

void CDlgAutoTestPower::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1 )
	{
		KillTimer(1);
		Start2();
	}
	else if( nIDEvent == 2 )
	{
		KillTimer(2);
		Start3();
	}
	else if( nIDEvent == 3 )
	{
		KillTimer(3);
		StartSCN();
	}
	else if( nIDEvent == 4 )
	{
		KillTimer(4);
		StartLNS();
	}
	else if( nIDEvent == 5 )
	{
		KillTimer(5);
		StartVAC();
	}
	else if( nIDEvent == 6 )
	{
		KillTimer(6);
		StartSPR01();
	}
	else if( nIDEvent == 7 )
	{
		KillTimer(7);
		StartSPR1();
	}
	else if( nIDEvent == 8 )
	{
		KillTimer(8);
		StartSPR02();
	}
	else if( nIDEvent == 9 )
	{
		KillTimer(9);
		StartSPR1();
	}
	else if( nIDEvent == 10 )
	{
		KillTimer(10);
		StartMCI();
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgAutoTestPower::InitParams()
{
	// 设置各ADC上电源回读通道参数
	int i;
	/////////////////////////////////////////////////////////////////
	// SCN ADC:
	for( i=10; i<16; i++ )
		GetDlgItem( IDC_STATIC_AT_Cali_Power_SCN10 +i -10)->SetWindowText( theApp.m_DL.adc.chn[0*16+i].strLabel );
	/////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////
	// LNS ADC:
	for( i=4; i<8; i++ )
		GetDlgItem( IDC_STATIC_AT_Cali_Power_LNS4 +i -4)->SetWindowText(theApp.m_DL.adc.chn[1*16+i].strLabel );
	/////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////
	// VAC ADC#1:
	for( i=10; i<16; i++ )
		GetDlgItem( IDC_STATIC_AT_Cali_Power_VAC10 +i -10)->SetWindowText( theApp.m_DL.adc.chn[3*16+i].strLabel );
	/////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////
	// SPR ADC#0:
	for( i=0; i<15; i++ )
		GetDlgItem( IDC_STATIC_AT_Cali_Power_SPR0 +i)->SetWindowText( theApp.m_DL.adc.chn[4*16+i].strLabel );
	/////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////
	// SPR ADC#1:
	for( i=5; i<8; i++ )
		GetDlgItem( IDC_STATIC_AT_Cali_Power_SPR16 +i -5)->SetWindowText( theApp.m_DL.adc.chn[5*16+i].strLabel );
	/////////////////////////////////////////////////////////////////

	m_brushError.CreateSolidBrush(RGB(255,0,0));

	CString str;
	int l = 0;//6+4+6 +8;
	for( i=0; i<33; i++ )
	{
		if( (i == 6+4+6 +7) || (i == 6+4+6+14) )
			l++;
		str.Format( "%.3f", theApp.m_AT.pAutoTestPowerExp[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc2 + l )->SetWindowText( str );
		l++;
	}

	/////////////////////////////////////////////////////////////////
	// MCI
	l = 0;
	for( i=0; i<8; i++ )
	{
		if( (i == 3) || (i == 4) )
			continue;
		str.Format( "MCI Chn %d", i );
		GetDlgItem( IDC_STATIC_AT_Cali_Power_MCI0 +l )->SetWindowText( str );
		str.Format( "%.3f", theApp.m_AT.pAutoTestPowerExp[33+i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_MCI0 +l )->SetWindowText( str );
		l++;
	}
}

void CDlgAutoTestPower::OnStart() 
{
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "In Progress");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( FALSE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( FALSE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( FALSE );
	OnClear();
	// 暂停真空状态机的工作
	if( CSMvac::Instance().isReady() )
		CSMvac::Instance().SetParams( 22, 1 );
	::SetCursor(LoadCursor(NULL, IDC_WAIT));
	SetTimer(2, 100, NULL);
}

void CDlgAutoTestPower::Start1()
{
	// TODO: Add your control notification handler code here
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "In Progress");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( FALSE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( FALSE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( FALSE );
	OnClear();
	::SetCursor(LoadCursor(NULL, IDC_WAIT));

	/////////////////////////////////////////////////////////////////
	// 读回ADC，并显示回读值
	/////////////////////////////////////////////////////////////////
	CString str, strRes;
	double dValue = 0.0;
	int i, j=0, k=0, nIndex;
	MSG msg;
	/////////////////////////////////////////////
	// SCN
	theApp.ChangedADCselectOB( 0 );
	for( i=10; i<16; i++ )
	{
		dValue = 0.0;
		theApp.ChangedADCsChSelect( i );
		str = theApp.ReadSelectedADC_Logging2( 100, dValue );
		strRes += str;
		theApp.m_AT.pAutoTestPower[k] = dValue;
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc + k )->SetWindowText( str );
		k++;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// LNS
	theApp.ChangedADCselectOB( 1 );
	for( i=4; i<8; i++ )
	{
		dValue = 0.0;
		theApp.ChangedADCsChSelect( i );
		str = theApp.ReadSelectedADC_Logging2( 5, dValue );
		strRes += str;
		theApp.m_AT.pAutoTestPower[k] = dValue;
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc + k )->SetWindowText( str );
		k++;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// VAC1
	theApp.ChangedADCselectOB( 3 );
	for( i=10; i<16; i++ )
	{
		dValue = 0.0;
		theApp.ChangedADCsChSelect( i );
		str = theApp.ReadSelectedADC_Logging2( 100, dValue );
		strRes += str;
		theApp.m_AT.pAutoTestPower[k] = dValue;
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc + k )->SetWindowText( str );
		k++;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// SPR0
	j = k;
	theApp.ChangedADCselectOB( 4 );
	theApp.ChangedADCsChSelect( 0 );
	theApp.ReadSelectedADC( FALSE );
	theApp.ReadSelectedADC( FALSE );
	theApp.Delay(120);
	theApp.ReadSelectedADC( FALSE );
	theApp.Delay(120);
	for( i=0; i<16; i++ )
	{
		dValue = 0.0;
		nIndex = g_AutoTestPowerSPR0_order[i];
		theApp.ChangedADCsChSelect( nIndex );
		theApp.Delay(10);
		str = theApp.ReadSelectedADC_Logging2( 120, dValue );
		strRes += str;
		theApp.m_AT.pAutoTestPower[nIndex +j] = dValue;
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR0_Vadc + nIndex )->SetWindowText( str );
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	k = j+i;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// SPR1
	theApp.ChangedADCselectOB( 5 );
	for( i=5; i<8; i++ )
	{
		dValue = 0.0;
		theApp.ChangedADCsChSelect( i );
		str = theApp.ReadSelectedADC_Logging2( 100, dValue );
		strRes += str;
		theApp.m_AT.pAutoTestPower[k] = dValue;
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc + k )->SetWindowText( str );
		k++;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	/////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 显示ADC计算分析值
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////
	// 计算值
	j = 0;
	k = 0;
	// SCN
	for( i=10; i<16; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +k )->SetWindowText( str );
		k++;
	}

	j++;
	// LNS
	for( i=4; i<8; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +k )->SetWindowText( str );
		k++;
	}

	j += 2;
	// VAC1
	for( i=10; i<16; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +k )->SetWindowText( str );
		k++;
	}

	j++;
	// SPR0
	for( i=0; i<16; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +k )->SetWindowText( str );
		k++;
	}

	j++;
	// SPR1
	for( i=5; i<8; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +k )->SetWindowText( str );
		k++;
	}
	/////////////////////////////////////////////////////////////////

	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "Start");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( TRUE );
}

void CDlgAutoTestPower::Start2()
{
	// TODO: Add your control notification handler code here
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "In Progress");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( FALSE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( FALSE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( FALSE );
	::SetCursor(LoadCursor(NULL, IDC_WAIT));

	CString str, strRes;
	int i, j, k, l;
	/////////////////////////////////////////////////////////////////
	// 发送所有回读命令
	/////////////////////////////////////////////////////////////////
	int	nTotalADCs = 0;
	int	nSendReadCmdCounts = 0;	// 为从FIFO回读所有参数，总共发送了多少次回读命令
	logParam lP[35];			// 总共35个电源相关的ADC需要回读
	/////////////////////////////////////////////
	// SCN
	for( i=10; i<16; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 0;
		lP[nTotalADCs].nMID		= USB_MID_SCN;
		lP[nTotalADCs].nCID		= USB_CID_SCN_ADC_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// LNS
	for( i=4; i<8; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 1;
		lP[nTotalADCs].nMID		= USB_MID_LNS;
		lP[nTotalADCs].nCID		= USB_CID_LNS_ADC_Read;
		lP[nTotalADCs].bType	= 0;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 5 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 5 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// VAC1
	for( i=10; i<16; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 3;
		lP[nTotalADCs].nMID		= USB_MID_VAC;
		lP[nTotalADCs].nCID		= USB_CID_VAC_ADC1_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// SPR0
	for( i=0; i<15; i++ )
	{
		if( i == 7 )	// 原来的Imon +24V，现在去掉了
			continue;
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 4;
		lP[nTotalADCs].nMID		= USB_MID_SPR;
		lP[nTotalADCs].nCID		= USB_CID_SPR_ADC0_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// SPR1
	for( i=5; i<8; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 5;
		lP[nTotalADCs].nMID		= USB_MID_SPR;
		lP[nTotalADCs].nCID		= USB_CID_SPR_ADC1_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	/////////////////////////////////////////////
	theApp.Delay( 50 );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 读一次FIFO
	long lReadSize = 35*3*10 +128;
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	DWORD lRet = CSemCtrl::Instance().USB_ReadBoardData( lReadSize, pnFrameData, &lBytesTransferred );
	if( (lRet != 0) || (lBytesTransferred < 1) )
	{
		delete [] pnFrameData;
		((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "Start");
		((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( TRUE );
		((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( TRUE );
		((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( TRUE );
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
		return;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 先看一看0xD22D的个数是否符合发出命令的个数
	/////////////////////////////////////////////////////////////////
	int nTotal = lBytesTransferred/2;
	int nRecvReadCounts = 0;	// 总共收到多少组回读值，每组6个数据，每个0xD22D后面跟着一组回读值
	for( i=0; i<nTotal; i++ )
	{
		if( pnFrameData[i] == 0xD22D )
			nRecvReadCounts++;
	}
	if( nRecvReadCounts != nSendReadCmdCounts )
	{
		delete [] pnFrameData;
		((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "Start");
		((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( TRUE );
		((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( TRUE );
		((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( TRUE );
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
		return;
	}

	/////////////////////////////////////////////////////////////////
	// 将有效的0xD22D后边的数据取出
	/////////////////////////////////////////////////////////////////
	// 每个有效数据由两个16位数据组成
	int nValidCount = 0;	// 总共要分析记录多少组数据
	int *pnValidData = new int[nTotalADCs *2];
	ZeroMemory(pnValidData, sizeof(pnValidData));
	int nD22Dcount	= 0;	// 非调试模式下，用于记录已忽略的无效数据的个数
	for( i=0; i<nTotal; i++ )
	{
		if( pnFrameData[i] == 0xD22D )
		{
			nD22Dcount++;
			if( lP[nValidCount].bType && nD22Dcount == 3 )
			{
				nD22Dcount = 0;
				pnValidData[nValidCount *2]		= pnFrameData[i+1];
				pnValidData[nValidCount *2 +1]	= pnFrameData[i+2];
				nValidCount++;
				i += 2;
			}
			else if( !(lP[nValidCount].bType) && nD22Dcount == 3 )
			{
				nD22Dcount = 0;
				pnValidData[nValidCount *2]		= pnFrameData[i+1];
				pnValidData[nValidCount *2 +1]	= pnFrameData[i+1];
				nValidCount++;
				i += 1;
			}
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 分析数据，并显示回读值
	/////////////////////////////////////////////////////////////////
	double dValue = 0.0;
	nTotal = nTotalADCs;
	k = 0;
	l = 0;
	for( i=0; i<nTotal; i++ )
	{
		if( (i == 6+4+6 +7) || (i == 6+4+6 +14) )
			l++;
		str = theApp.ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP[i], dValue );
		strRes += str;
		theApp.m_AT.pAutoTestPower[k] = dValue;
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc + l )->SetWindowText( str );
		k++;
		l++;
	}
	delete [] pnFrameData;
	delete [] pnValidData;
	/////////////////////////////////////////////////////////////////

	l = 0;//6+4+6 +8;
	for( i=0; i<nTotal; i++ )
	{
		if( (i == 6+4+6 +7) || (i == 6+4+6+14) )
			l++;
		str.Format( "%.3f", theApp.m_AT.pAutoTestPowerExp[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc2 + l )->SetWindowText( str );
		l++;
	}

	l = 0;//6+4+6 +8;
	double dErr = 0.0;
	for( i=0; i<nTotal; i++ )
	{
		if( (i == 6+4+6 +7) || (i == 6+4+6+14) )
			l++;
		if( (theApp.m_AT.pAutoTestPowerExp[i]) != 0 && (theApp.m_AT.pAutoTestPower[i] != 0) )
		{
			dErr = fabs(theApp.m_AT.pAutoTestPower[i] -theApp.m_AT.pAutoTestPowerExp[i]) / fabs(theApp.m_AT.pAutoTestPowerExp[i]);
			str.Format( "%.2f %%", dErr *100 );
			GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc4 + l )->SetWindowText( str );
			if( dErr *100 > theApp.m_AT.pAutoTestPowerErr[i] )
				theApp.m_AT.pPower_OutofRange[l] = TRUE;
		}
		l++;
	}

	/////////////////////////////////////////////////////////////////
	// 显示ADC计算分析值
	/////////////////////////////////////////////////////////////////
	// 计算值
	j = 0;
	k = 0;
	l = 0;
	// SCN
	for( i=10; i<16; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +l )->SetWindowText( str );
		k++;
		l++;
	}

	j++;
	// LNS
	for( i=4; i<8; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +l )->SetWindowText( str );
		k++;
		l++;
	}

	j += 2;
	// VAC1
	for( i=10; i<16; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +l )->SetWindowText( str );
		k++;
		l++;
	}

	j++;
	// SPR0
	for( i=0; i<15; i++ )
	{
		if( i == 7 )	// 原来的Imon +24V，现在去掉了
		{
			l++;
			continue;
		}
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +l )->SetWindowText( str );
		k++;
		l++;
	}
	l++;

	j++;
	// SPR1
	for( i=5; i<8; i++ )
	{
		dValue = theApp.GetPhysicsFromAdcs( j*16 +i, theApp.m_AT.pAutoTestPower[k] );
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +l )->SetWindowText( str );
		k++;
		l++;
	}
	/////////////////////////////////////////////////////////////////

	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "Start");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( TRUE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( TRUE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( TRUE );
	::SetCursor(LoadCursor(NULL, IDC_ARROW));

	Invalidate();
}

void CDlgAutoTestPower::Start3()
{
	// TODO: Add your control notification handler code here
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "In Progress");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( FALSE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( FALSE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( FALSE );
	::SetCursor(LoadCursor(NULL, IDC_WAIT));

// 	StartSCN();
// 	StartLNS();
// 	StartVAC();
// 	StartSPR();
	SetTimer(3, 100, NULL);
}

void CDlgAutoTestPower::StartSCN()
{
	/////////////////////////////////////////////////////////////////
	// 发送所有回读命令
	/////////////////////////////////////////////////////////////////
	int	nTotalADCs = 0;
	int	nSendReadCmdCounts = 0;	// 为从FIFO回读所有参数，总共发送了多少次回读命令
	logParam lP[6];				// 总共6个电源相关的ADC需要回读
	/////////////////////////////////////////////
	// SCN
	for( int i=10; i<16; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 0;
		lP[nTotalADCs].nMID		= USB_MID_SCN;
		lP[nTotalADCs].nCID		= USB_CID_SCN_ADC_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	StartReadAnalyze( USB_MID_SCN, lP );
	SetTimer(4, 100, NULL);
}

void CDlgAutoTestPower::StartLNS()
{
	/////////////////////////////////////////////////////////////////
	// 发送所有回读命令
	/////////////////////////////////////////////////////////////////
	int	nTotalADCs = 0;
	int	nSendReadCmdCounts = 0;	// 为从FIFO回读所有参数，总共发送了多少次回读命令
	logParam lP[4];				// 总共4个电源相关的ADC需要回读
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// LNS
	for( int i=4; i<8; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 1;
		lP[nTotalADCs].nMID		= USB_MID_LNS;
		lP[nTotalADCs].nCID		= USB_CID_LNS_ADC_Read;
		lP[nTotalADCs].bType	= 0;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 5 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 5 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	StartReadAnalyze( USB_MID_LNS, lP );
	SetTimer(5, 100, NULL);
}

void CDlgAutoTestPower::StartVAC()
{
	/////////////////////////////////////////////////////////////////
	// 发送所有回读命令
	/////////////////////////////////////////////////////////////////
	int	nTotalADCs = 0;
	int	nSendReadCmdCounts = 0;	// 为从FIFO回读所有参数，总共发送了多少次回读命令
	logParam lP[6];				// 总共6个电源相关的ADC需要回读
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// VAC1
	for( int i=10; i<16; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 3;
		lP[nTotalADCs].nMID		= USB_MID_VAC;
		lP[nTotalADCs].nCID		= USB_CID_VAC_ADC1_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	StartReadAnalyze( USB_MID_VAC, lP );
	SetTimer(6, 100, NULL);
}

void CDlgAutoTestPower::StartSPR()
{
	/////////////////////////////////////////////////////////////////
	// 发送所有回读命令
	/////////////////////////////////////////////////////////////////
	int	i, nTotalADCs = 0;
	int	nSendReadCmdCounts = 0;	// 为从FIFO回读所有参数，总共发送了多少次回读命令
	logParam lP[14];			// 总共14个电源相关的ADC需要回读
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// SPR0
	for( i=0; i<15; i++ )
	{
		if( i == 7 )	// 原来的Imon +24V，现在去掉了
			continue;
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 4;
		lP[nTotalADCs].nMID		= USB_MID_SPR;
		lP[nTotalADCs].nCID		= USB_CID_SPR_ADC0_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	StartReadAnalyze( USB_MID_SPR, lP );
	SetTimer(7, 100, NULL);
}

void CDlgAutoTestPower::StartSPR01()
{
	/////////////////////////////////////////////////////////////////
	// 发送所有回读命令: chn 8 - 14
	/////////////////////////////////////////////////////////////////
	int	i, nTotalADCs = 0;
	int	nSendReadCmdCounts = 0;	// 为从FIFO回读所有参数，总共发送了多少次回读命令
	logParam lP[7];				// 总共7个电源相关的ADC需要回读
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// SPR0
	for( i=8; i<15; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 4;
		lP[nTotalADCs].nMID		= USB_MID_SPR;
		lP[nTotalADCs].nCID		= USB_CID_SPR_ADC0_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	StartReadAnalyze( USB_MID_SPR+2, lP );
	SetTimer(8, 100, NULL);
}

void CDlgAutoTestPower::StartSPR02()
{
	/////////////////////////////////////////////////////////////////
	// 发送所有回读命令: chn 0 - 6
	/////////////////////////////////////////////////////////////////
	int	i, nTotalADCs = 0;
	int	nSendReadCmdCounts = 0;	// 为从FIFO回读所有参数，总共发送了多少次回读命令
	logParam lP[7];				// 总共7个电源相关的ADC需要回读
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// SPR0
	for( i=0; i<7; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 4;
		lP[nTotalADCs].nMID		= USB_MID_SPR;
		lP[nTotalADCs].nCID		= USB_CID_SPR_ADC0_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	StartReadAnalyze( USB_MID_SPR+3, lP );
	SetTimer(9, 100, NULL);
}

void CDlgAutoTestPower::StartSPR1()
{
	/////////////////////////////////////////////////////////////////
	// 发送所有回读命令
	/////////////////////////////////////////////////////////////////
	int	nTotalADCs = 0;
	int	nSendReadCmdCounts = 0;	// 为从FIFO回读所有参数，总共发送了多少次回读命令
	logParam lP[3];				// 总共3个电源相关的ADC需要回读
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// SPR1
	for( int i=5; i<8; i++ )
	{
		lP[nTotalADCs].nType	= 1;
		lP[nTotalADCs].nNum		= 5;
		lP[nTotalADCs].nMID		= USB_MID_SPR;
		lP[nTotalADCs].nCID		= USB_CID_SPR_ADC1_Read;
		lP[nTotalADCs].bType	= 1;
		lP[nTotalADCs].nChn		= i;
		lP[nTotalADCs].bDiff	= theApp.m_DL.adc.chn[lP[nTotalADCs].nNum*16 +i].bDiff;
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		theApp.OnlySendReadSelectedADCcmd( lP[nTotalADCs] );
		theApp.Delay( 100 );
		nTotalADCs++;
		nSendReadCmdCounts += 3;
	}
	StartReadAnalyze( USB_MID_SPR+1, lP );
	SetTimer(10, 100, NULL);

/*	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "Start");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( TRUE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( TRUE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( TRUE );
	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	// 恢复真空状态机的工作
	CSMvac::Instance().SetParams( 22, 0 );*/
}

BOOL CDlgAutoTestPower::StartReadAnalyze( int nMID, logParam* lP )
{
	CString str;
	/////////////////////////////////////////////
	theApp.Delay( 50 );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 读一次FIFO
	long lReadSize = 20*3+256;	//35*3*10 +128;
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	DWORD lRet = CSemCtrl::Instance().USB_ReadBoardData( lReadSize, pnFrameData, &lBytesTransferred );
	if( (lRet != 0) || (lBytesTransferred < 1) )
	{
		delete [] pnFrameData;
		str = "Auto test power: ReadBoardData Error!";
//		theApp.DisplayMsg(str);
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////

	int nSendReadCmdCounts = 0;
	int	nTotalADCs = 0;
	int nNum = lP[0].nNum;
	int nADCChnIndex	= 0;
	int nPwrArrayIndex	= 0;
	int nLabelIndex		= 0;
	switch( nMID )
	{
	case	1:
		nTotalADCs		= 6;	// SCN
		nADCChnIndex	= 10;
		nPwrArrayIndex	= 0;
		nLabelIndex		= 0;
		break;
	case	2:
		nTotalADCs		= 4;	// LNS
		nADCChnIndex	= 5;
		nPwrArrayIndex	= 6;
		nLabelIndex		= 6;
		break;
	case	3:
		nTotalADCs		= 6;	// VAC
		nADCChnIndex	= 10;
		nPwrArrayIndex	= 6+4;
		nLabelIndex		= 6+4;
		break;
	case	4:
		nTotalADCs		= 14;	// SPR (#0)
		nADCChnIndex	= 0;
		nPwrArrayIndex	= 6+4+6;
		nLabelIndex		= 6+4+6;
		break;
	case	5:
		nTotalADCs		= 3;	// SPR (#1)
		nADCChnIndex	= 5;
		nPwrArrayIndex	= 6+4+6+14;
		nLabelIndex		= 6+4+6+16;
		break;
	case	6:
		nTotalADCs		= 7;	// SPR (#0:chn8-15)
		nADCChnIndex	= 8;
		nPwrArrayIndex	= 6+4+6+7;
		nLabelIndex		= 6+4+6+8;
		break;
	case	7:
		nTotalADCs		= 7;	// SPR (#0:chn0-7)
		nADCChnIndex	= 0;
		nPwrArrayIndex	= 6+4+6;
		nLabelIndex		= 6+4+6;
		break;
	}
	nSendReadCmdCounts = nTotalADCs * 3;
	/////////////////////////////////////////////////////////////////
	// 先看一看0xD22D的个数是否符合发出命令的个数
	/////////////////////////////////////////////////////////////////
	int i, nTotal = lBytesTransferred/2;
	int nRecvReadCounts = 0;	// 总共收到多少组回读值，每组4个数据，每个0xD22D后面跟着一组回读值
	for( i=0; i<nTotal; i++ )
	{
		if( pnFrameData[i] == 0xD22D )
			nRecvReadCounts++;
	}
	if( nRecvReadCounts != nSendReadCmdCounts )
	{
		delete [] pnFrameData;
		str.Format( "Auto test power: SendCounts [%d] != RecvCounts [%d]", nSendReadCmdCounts, nRecvReadCounts );
//		theApp.DisplayMsg(str);
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////
	// 将有效的0xD22D后边的数据取出
	/////////////////////////////////////////////////////////////////
	// 每个有效数据由两个16位数据组成
	int nValidCount = 0;	// 总共要分析记录多少组数据
	int *pnValidData = new int[nTotalADCs *2];
	ZeroMemory(pnValidData, sizeof(pnValidData));
	int nD22Dcount	= 0;	// 非调试模式下，用于记录已忽略的无效数据的个数
	for( i=0; i<nTotal; i++ )
	{
		if( pnFrameData[i] == 0xD22D )
		{
			nD22Dcount++;
			if( lP[nValidCount].bType && nD22Dcount == 3 )
			{
				nD22Dcount = 0;
				pnValidData[nValidCount *2]		= pnFrameData[i+1];
				pnValidData[nValidCount *2 +1]	= pnFrameData[i+2];
				nValidCount++;
				i += 2;
			}
			else if( !(lP[nValidCount].bType) && nD22Dcount == 3 )
			{
				nD22Dcount = 0;
				pnValidData[nValidCount *2]		= pnFrameData[i+1];
				pnValidData[nValidCount *2 +1]	= pnFrameData[i+1];
				nValidCount++;
				i += 1;
			}
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 分析数据，并显示回读值
	/////////////////////////////////////////////////////////////////
	CString strRes;
	double dValue = 0.0;
	nTotal = nTotalADCs;
	int nKbak = nPwrArrayIndex;
	int nLbak = nLabelIndex;
	for( i=0; i<nTotal; i++ )
	{
		dValue = 0.0;
		if( (nMID == 4) && ((i == 7) || (i == 14)) )
			nLabelIndex++;
		str = theApp.ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP[i], dValue );
		strRes += str;
		theApp.m_AT.pAutoTestPower[nPwrArrayIndex] = dValue;
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc + nLabelIndex )->SetWindowText( str );
		nPwrArrayIndex++;
		nLabelIndex++;
	}
	delete [] pnFrameData;
	delete [] pnValidData;
	/////////////////////////////////////////////////////////////////

	nPwrArrayIndex	= nKbak;
	nLabelIndex		= nLbak;//6+4+6 +8;
	double dErr = 0.0;
	for( i=0; i<nTotal; i++ )
	{
		if( (nMID == 4) && ((i == 7) || (i == 14)) )
			nLabelIndex++;
		if( (theApp.m_AT.pAutoTestPowerExp[nPwrArrayIndex]) != 0 && (theApp.m_AT.pAutoTestPower[nPwrArrayIndex] != 0) )
		{
			dErr = fabs(theApp.m_AT.pAutoTestPower[nPwrArrayIndex] -theApp.m_AT.pAutoTestPowerExp[nPwrArrayIndex]) / fabs(theApp.m_AT.pAutoTestPowerExp[nPwrArrayIndex]);
			str.Format( "%.2f %%", dErr *100 );
			GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc4 + nLabelIndex )->SetWindowText( str );
			if( dErr *100 > theApp.m_AT.pAutoTestPowerErr[nPwrArrayIndex] )
				theApp.m_AT.pPower_OutofRange[nLabelIndex] = TRUE;
		}
		nPwrArrayIndex++;
		nLabelIndex++;
	}

	/////////////////////////////////////////////////////////////////
	// 显示ADC计算分析值
	/////////////////////////////////////////////////////////////////
	// 计算值
	nPwrArrayIndex	= nKbak;
	nLabelIndex		= nLbak;//6+4+6 +8;
	if( nMID == 4 )
	{
		// SPR0
		for( i=0; i<15; i++ )
		{
			if( i == 7 )	// 原来的Imon +24V，现在去掉了
			{
				nLabelIndex++;
				continue;
			}
			dValue = theApp.GetPhysicsFromAdcs( nNum*16 +i, theApp.m_AT.pAutoTestPower[nPwrArrayIndex] );
			str.Format( "%.6f", dValue );
			GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +nLabelIndex )->SetWindowText( str );
			nPwrArrayIndex++;
			nLabelIndex++;
		}
	}
	else
	{
		for( i=nADCChnIndex; i<nADCChnIndex +nTotalADCs; i++ )
		{
			dValue = theApp.GetPhysicsFromAdcs( nNum*16 +i, theApp.m_AT.pAutoTestPower[nPwrArrayIndex] );
			str.Format( "%.6f", dValue );
			GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 +nLabelIndex )->SetWindowText( str );
			nPwrArrayIndex++;
			nLabelIndex++;
		}
	}
	/////////////////////////////////////////////////////////////////

	Invalidate();
	theApp.Delay( 50 );
	return TRUE;
}

void CDlgAutoTestPower::StartMCI()
{
	unsigned short* pnFrameData = new unsigned short[ 8 ];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));
	if( CSemCtrl::Instance().Serial_ReadbackData( 0, 0, pnFrameData, 0, TRUE ) )
		StartReadAnalyze(pnFrameData);
	delete [] pnFrameData;
	
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->SetWindowText( "Start");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Start)))->EnableWindow( TRUE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Clear)))->EnableWindow( TRUE );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_Power_Report)))->EnableWindow( TRUE );
	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	// 恢复真空状态机的工作
	CSMvac::Instance().SetParams( 22, 0 );
}

BOOL CDlgAutoTestPower::StartReadAnalyze( unsigned short* pPwr )	// MCI Pwr
{
	/////////////////////////////////////////////////////////////////
	// 分析数据，并显示回读值
	/////////////////////////////////////////////////////////////////
	int i;
	for( i=0; i<8; i++ )
	{
		WORD wHigh = pPwr[i] & 0x8000;
		if( wHigh == 0x8000 )
		{
			PwrVconV( pPwr[i] );
		}
	}
	CString str, strRes;
	int nPwrArrayIndex	= 0;
	int nLabelIndex		= 0;
	for( i=0; i<8; i++ )
	{
		if( (i == 3) || (i == 4) )
			continue;
		nPwrArrayIndex	= 33+i;
		str.Format( "%.3f", theApp.m_AT.pAutoTestPower[33+i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_MCI02 +nLabelIndex )->SetWindowText( str );
		nLabelIndex++;
	}
	/////////////////////////////////////////////////////////////////

	nLabelIndex		= 0;
	double dErr = 0.0;
	for( i=0; i<8; i++ )
	{
		if( (i == 3) || (i == 4) )
			continue;
		nPwrArrayIndex	= 33+i;
		if( (theApp.m_AT.pAutoTestPowerExp[nPwrArrayIndex]) != 0 && (theApp.m_AT.pAutoTestPower[nPwrArrayIndex] != 0) )
		{
			dErr = fabs(theApp.m_AT.pAutoTestPower[nPwrArrayIndex] -theApp.m_AT.pAutoTestPowerExp[nPwrArrayIndex]) / fabs(theApp.m_AT.pAutoTestPowerExp[nPwrArrayIndex]);
			str.Format( "%.2f %%", dErr *100 );
			GetDlgItem( IDC_EDIT_AT_Cali_Power_MCI03 + nLabelIndex )->SetWindowText( str );
			if( dErr *100 > theApp.m_AT.pAutoTestPowerErr[nPwrArrayIndex] )
				theApp.m_AT.pPower_OutofRange[nLabelIndex] = TRUE;
		}
//		nPwrArrayIndex++;
		nLabelIndex++;
	}

	Invalidate();
	theApp.Delay( 50 );
	return TRUE;
}
	
void CDlgAutoTestPower::PwrVconV( unsigned short nPwr )
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

	WORD wData = nPwr & 0x0FFF;			// ((bData1 & 0x0F) << 8) | bData2;
	int nChn = (nPwr & 0x7000) / 4096;	// (bData1 & 0x70)	/16;
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
		dValue = 3.5 *g_dPwrbitsRange *wData /g_wPwrbitsRange - 2.5 *theApp.m_AT.pAutoTestPower[33+2];
		strRef = "-8V";
		break;
	case	0:
		dValue = 3.5 *g_dPwrbitsRange *wData /g_wPwrbitsRange - 2.5 *theApp.m_AT.pAutoTestPower[33+5];
		strRef = "-5V";
		break;
	default:
		break;
	}
	theApp.m_AT.pAutoTestPower[33+nChn] = dValue;
}

void CDlgAutoTestPower::OnClear() 
{
	// TODO: Add your control notification handler code here
	int i;
	for( i=0; i<35; i++ )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc + i )->SetWindowText( "" );
//		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc2 + i )->SetWindowText( "" );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 + i )->SetWindowText( "" );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc4 + i )->SetWindowText( "" );
	}
	for( i=0; i<6; i++ )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Power_MCI02 + i )->SetWindowText( "" );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_MCI03 + i )->SetWindowText( "" );
	}
	for( i=0; i<35+8; i++ )
	{
		theApp.m_AT.pAutoTestPower[i] = 0.0;
		theApp.m_AT.pPower_OutofRange[i] = FALSE;
	}
	Invalidate();
}

void CDlgAutoTestPower::OnReport() 
{
	// TODO: Add your control notification handler code here
/*	CFileDialog filedlg( FALSE, "doc", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "*.doc", this );
	if( filedlg.DoModal() != IDOK )
		return;
	CString filename = filedlg.GetPathName();

	BOOL result;
	COleException	e;
	_Application	app;
	_Document		doc;
	Documents		docs;
	Selection		sel;
	_Font			font;
	Paragraph		par;
	Tables			tables;
	Range			range;
	
	CString str;
	CString strUnit;

	//Define a VARIANT as optional parameter passed
	VARIANT varOptional;
	varOptional.vt = VT_ERROR;
	varOptional.scode = DISP_E_PARAMNOTFOUND;
	COleVariant varTrue((short)-1, VT_BOOL), varFalse((short)0, VT_BOOL);

	//Create a Application object of Word
	result = app.CreateDispatch( "Word.Application", &e );
	if( !result )
	{
		char szErrMsg[255];
		e.GetErrorMessage( szErrMsg, 255);
		AfxMessageBox( szErrMsg, MB_ICONSTOP );
		return;
	}

	::SetCursor(LoadCursor(NULL, IDC_WAIT));//02.08.30
	//Get the selection of documents
	docs.AttachDispatch( app.GetDocuments() );
	docs.Add( &varOptional, &varOptional, &varOptional, &varOptional );
	docs.ReleaseDispatch();
	//Get selection object
	sel.AttachDispatch( app.GetSelection() );
	//Alignment
	par.AttachDispatch( sel.GetParagraphFormat() );
	//Get font
	font.AttachDispatch( sel.GetFont() );
	str = "Arial";
	font.SetName( str );
	float fontsize = font.GetSize();
	font.SetSize( (float)(fontsize*1.5) );
	font.SetBold( 2 );
	
	par.SetAlignment(WORDALIGNMENT_CENTER);
	str = "Auto Test Report";
	sel.TypeText( str );
	sel.TypeParagraph();
	font.SetBold( 1 );
	str = "Power";
	sel.TypeText( str );
	sel.TypeParagraph();
	par.SetAlignment( WORDALIGNMENT_LEFT );
	sel.TypeParagraph();


	font.SetBold( 0 );
	font.SetSize( 8 );
	par.SetAlignment( WORDALIGNMENT_RIGHT );
	range.AttachDispatch( sel.GetRange() );
	tables.AttachDispatch( sel.GetTables() );
	tables.Add( range, 34, 5, varTrue, varTrue );
	tables.ReleaseDispatch();
	range.ReleaseDispatch();


	//存表头
	sel.SelectRow();
	font.SetBold( 1 );
	font.SetSize( 10 );
	str = "Label";//.LoadString( IDF_WORD_PARTICLE );
	par.SetAlignment( WORDALIGNMENT_LEFT );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "Vadc";//.LoadString( IDF_WORD_GEOMETRY_CENTER );
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "Vadc Expected";//.LoadString( IDF_WORD_AREA );
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "Calc.";//.LoadString( IDF_WORD_CIRCLE_DIAMETER );
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "Calc. Expected";//.LoadString( IDF_WORD_PERIMETER );
	sel.TypeText( str );
	
	//存表
	font.SetBold( 0 );
	font.SetSize( 8 );
	/////////////////////////////////////////////
	// SCN
	theApp.ChangedADCselectOB( 0 );
	for( int i=0; i<16; i++ )
	{
		GetDlgItem( IDC_STATIC_AT_Cali_Power_SCN10 + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		par.SetAlignment( WORDALIGNMENT_LEFT );

		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc2 + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc3 + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SCN10_Vadc4 + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
	}
	int nIndex = 0;
	for( i=0; i<14; i++ )	// SPR0
	{
		nIndex = g_AutoTestPowerSPR0_order[i];

		GetDlgItem( IDC_STATIC_AT_Cali_Power_SPR0 + nIndex )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		par.SetAlignment( WORDALIGNMENT_LEFT );

		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR0_Vadc + nIndex )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR0_Vadc2 + nIndex )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR0_Vadc3 + nIndex )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR0_Vadc4 + nIndex )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
	}
	for( i=0; i<3; i++ )	// SPR1
	{
		GetDlgItem( IDC_STATIC_AT_Cali_Power_SPR16 + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		par.SetAlignment( WORDALIGNMENT_LEFT );

		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR16_Vadc + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR16_Vadc2 + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR16_Vadc3 + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Power_SPR16_Vadc4 + i )->GetWindowText( str );
		sel.Move( &varOptional, &varOptional );
		sel.TypeText( str );
	}
	
	font.ReleaseDispatch();
	par.ReleaseDispatch();
	sel.ReleaseDispatch();
	
	COleVariant varFileName(filename), varPasswd("");
	COleVariant varFileFormat((short)0);
	//Get the active document object
	doc.AttachDispatch( app.GetActiveDocument() );
	//Save the document
	doc.SaveAs( varFileName, varFileFormat, varFalse, varPasswd, varTrue, varPasswd, varFalse, varFalse, varFalse, varFalse, varFalse);
	//Close the document
	doc.Close(&varOptional, &varOptional, &varOptional);
	doc.ReleaseDispatch();
	
	app.Quit(&varOptional, &varOptional, &varOptional);
	app.DetachDispatch();
	app.ReleaseDispatch();
	
	::SetCursor(LoadCursor(NULL, IDC_ARROW));//02.08.30*/
}
