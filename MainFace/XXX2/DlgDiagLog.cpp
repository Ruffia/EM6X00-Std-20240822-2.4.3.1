// DlgDiagLog.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgDiagLog.h"
#include "CommonFunctionManager.h"
#include "CFGFileManager.h"
#include "ControlLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgDiagLog dialog
CDlgDiagLog::CDlgDiagLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDiagLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDiagLog)
	m_nAveCounts = 0;
	//}}AFX_DATA_INIT
}


void CDlgDiagLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDiagLog)
	DDX_Text(pDX, IDC_EDIT_Ave_Counts, m_nAveCounts);
	DDV_MinMaxInt(pDX, m_nAveCounts, 0, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDiagLog, CDialog)
	//{{AFX_MSG_MAP(CDlgDiagLog)
	ON_BN_CLICKED(IDC_BUTTON_Diag_ClearAll, OnClearAll)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_CHECK_SCNADC_Ch0, IDC_CHECK_SCNADC_Ch7, OnCHECKSCNADCCh)
END_MESSAGE_MAP()

// CDlgDiagLog message handlers

BOOL CDlgDiagLog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitSetChangingParams();
	InitReadADCsParams();

	CString str;
	str.Format( "%ld", CControlLayer::Instance().m_DL.other.nElapseChanging );
	GetDlgItem( IDC_EDIT_ChangingParam_nElapse )->SetWindowText( str );
	str.Format( "%d", CControlLayer::Instance().m_DL.other.nElapseLogging );
	GetDlgItem( IDC_EDIT_LoggingParam_nElapse )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_Elapse )->SetWindowText( str );
	str.Format( "%d", CControlLayer::Instance().m_DL.adc.nAveCounts );
	GetDlgItem( IDC_EDIT_Ave_Counts )->SetWindowText( str );
	((CButton*)(GetDlgItem( IDC_CHECK_GaugeCompr)))->SetCheck( CControlLayer::Instance().m_DL.other.bGaugeCompr );
	((CButton*)(GetDlgItem( IDC_CHECK_Graph)))->SetCheck( CControlLayer::Instance().m_DL.other.bGraph );
	((CButton*)(GetDlgItem( IDC_CHECK_Debug)))->SetCheck( CControlLayer::Instance().m_DL.other.bDebug );

//	theApp.m_dADCchanModes[/*0*16*2 +*/i *2 +0] = 1;	// If value = 1 then S.E. / If value = 2 then diff mode
	if( CControlLayer::Instance().m_DL.adc.chn[0].bDiff )
		((CButton*)(GetDlgItem( IDC_CHECK_SCNADC_SE )))->SetCheck( 0 );
	else
		((CButton*)(GetDlgItem( IDC_CHECK_SCNADC_SE )))->SetCheck( 1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDiagLog::OnCHECKSCNADCCh(UINT nID)
{
	// SCN板ADC的0-7通道是差分信号，需两两回读
	int nChn	= nID - IDC_CHECK_SCNADC_Ch0;
	int nCheck	= ((CButton*)(GetDlgItem(nID)))->GetCheck();
	switch( nChn )
	{
	case	0:
	case	2:
	case	4:
	case	6:
		((CButton*)(GetDlgItem(nID +1)))->SetCheck( nCheck );
		break;
	case	1:
	case	3:
	case	5:
	case	7:
		((CButton*)(GetDlgItem(nID -1)))->SetCheck( nCheck );
		break;
	default:
		break;
	}
}

void CDlgDiagLog::InitReadADCsParams()
{
	// 设置各ADC回读通道参数
	int i,  nTotal = 16;

	// SCN ADC (only one ADC on SCN board):
	for( i=0; i<nTotal; i++ )
		GetDlgItem( IDC_CHECK_SCNADC_Ch0 +i)->SetWindowText( CControlLayer::Instance().m_DL.adc.chn[0*16+i].strLabel );

	// LNS ADC (one LTC1859 8 channel ADC):
	nTotal = 8;
	for( i=0; i<nTotal; i++ )
		GetDlgItem( IDC_CHECK_LNSADC_Ch0 +i)->SetWindowText(CControlLayer::Instance().m_DL.adc.chn[1*16+i].strLabel );

	// VAC ADC#0:
	nTotal = 16;
	for( i=0; i<nTotal; i++ )
		GetDlgItem( IDC_CHECK_VACADC0_Ch0 +i)->SetWindowText( CControlLayer::Instance().m_DL.adc.chn[2*16+i].strLabel );

	// VAC ADC#1:
	for( i=0; i<nTotal; i++ )
		GetDlgItem( IDC_CHECK_VACADC1_Ch0 +i)->SetWindowText( CControlLayer::Instance().m_DL.adc.chn[3*16+i].strLabel );

	// SPR ADC#0:
	for( i=0; i<nTotal; i++ )
		GetDlgItem( IDC_CHECK_SPRADC0_Ch0 +i)->SetWindowText( CControlLayer::Instance().m_DL.adc.chn[4*16+i].strLabel );

	// SPR ADC#1:
	for( i=0; i<nTotal; i++ )
		GetDlgItem( IDC_CHECK_SPRADC1_Ch0 +i)->SetWindowText( CControlLayer::Instance().m_DL.adc.chn[5*16+i].strLabel );
}

void CDlgDiagLog::InitSetChangingParams()
{
	InitChangingParams();
	InitLoggingParams();
}

void CDlgDiagLog::InitChangingParams()
{
	int			nTotal	= 11;
	int			i, j=0;
	CString		str;

	// SCN & LNS
	nTotal = 11;
	for( i=0; i<nTotal; i++,j++ )
	{
		((CButton*)(GetDlgItem(IDC_CHECK_Stig_X +i)))->SetCheck( CControlLayer::Instance().m_DL.diag[ j ].nCheck );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nMin );
		GetDlgItem( IDC_EDIT_XstigDAC +i )->SetWindowText( str );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nMax );
		GetDlgItem( IDC_EDIT_XstigDAC2 +i )->SetWindowText( str );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nStep );
		GetDlgItem( IDC_EDIT_XstigDAC3 +i )->SetWindowText( str );

		str.Format( "%d", CControlLayer::Instance().m_DL.diag[ j ].nIntv );
		GetDlgItem( IDC_EDIT_XstigDAC4 +i )->SetWindowText( str );

		GetDlgItem(IDC_CHECK_Stig_X +i)->GetWindowText( str );
		CControlLayer::Instance().m_DL.diag[ j ].strLabel = str;
	}

	// VAC
	nTotal = 12;
	for( i=0; i<nTotal; i++,j++ )
	{
		((CButton*)(GetDlgItem(IDC_CHECK_CalI_TC0 +i)))->SetCheck( CControlLayer::Instance().m_DL.diag[ j ].nCheck );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nMin );
		GetDlgItem( IDC_EDIT_VACDAC_U44_CHA +i )->SetWindowText( str );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nMax );
		GetDlgItem( IDC_EDIT_VACDAC_U44_CHA2 +i )->SetWindowText( str );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nStep );
		GetDlgItem( IDC_EDIT_VACDAC_U44_CHA3 +i )->SetWindowText( str );

		str.Format( "%d", CControlLayer::Instance().m_DL.diag[ j ].nIntv );
		GetDlgItem( IDC_EDIT_VACDAC_U44_CHA4 +i )->SetWindowText( str );

		GetDlgItem(IDC_CHECK_CalI_TC0 +i)->GetWindowText( str );
		CControlLayer::Instance().m_DL.diag[ j ].strLabel = str;
	}

	// SPR
	nTotal = 4;
	for( i=0; i<nTotal; i++,j++ )
	{
		((CButton*)(GetDlgItem(IDC_CHECK_SPR_DAC_CHA +i)))->SetCheck( CControlLayer::Instance().m_DL.diag[ j ].nCheck );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nMin );
		GetDlgItem( IDC_EDIT_SPRDAC_CHA +i )->SetWindowText( str );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nMax );
		GetDlgItem( IDC_EDIT_SPRDAC_CHA2 +i )->SetWindowText( str );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nStep );
		GetDlgItem( IDC_EDIT_SPRDAC_CHA3 +i )->SetWindowText( str );

		str.Format( "%d", CControlLayer::Instance().m_DL.diag[ j ].nIntv );
		GetDlgItem( IDC_EDIT_SPRDAC_CHA4 +i )->SetWindowText( str );

		GetDlgItem(IDC_CHECK_SPR_DAC_CHA +i)->GetWindowText( str );
		CControlLayer::Instance().m_DL.diag[ j ].strLabel = str;
	}

	// MCI
	nTotal = 10;
	for( i=0; i<nTotal; i++,j++ )
	{
		((CButton*)(GetDlgItem(IDC_CHECK_Offset_0 +i)))->SetCheck( CControlLayer::Instance().m_DL.diag[ j ].nCheck );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nMin );
		GetDlgItem( IDC_EDIT_Offset_0 +i )->SetWindowText( str );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nMax );
		GetDlgItem( IDC_EDIT_Offset_02 +i )->SetWindowText( str );

		str.Format( "%4X", CControlLayer::Instance().m_DL.diag[ j ].nStep );
		GetDlgItem( IDC_EDIT_Offset_03 +i )->SetWindowText( str );

		str.Format( "%d", CControlLayer::Instance().m_DL.diag[ j ].nIntv );
		GetDlgItem( IDC_EDIT_Offset_04 +i )->SetWindowText( str );

		GetDlgItem(IDC_CHECK_Offset_0 +i)->GetWindowText( str );
		CControlLayer::Instance().m_DL.diag[ j ].strLabel = str;
	}

	str.Format( "%d", CControlLayer::Instance().m_DL.other.nElapseChanging );
//	GetDlgItem( IDC_EDIT_ChangingParam_nElapse )->SetWindowText( str );
	GetDlgItem( IDC_EDIT_Elapse )->SetWindowText( str );
}

void CDlgDiagLog::InitLoggingParams()
{
	int			nTotal	= 16;
	int			i, j=0;

	// SCN
	nTotal = 16;
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_SCNADC_Ch0 +i)))->SetCheck( CControlLayer::Instance().m_DL.log.bLog[j*16 +i] );

	// LNS
	j++;
	nTotal = 8;
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_LNSADC_Ch0 +i)))->SetCheck( CControlLayer::Instance().m_DL.log.bLog[j*16 +i] );
	for( i=0; i<9; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_PARAM_LNS_L1DAC +i)))->SetCheck( CControlLayer::Instance().m_DL.log.bLogParam[i] );

	// VAC0
	j++;
	nTotal = 16;
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_VACADC0_Ch0 +i)))->SetCheck( CControlLayer::Instance().m_DL.log.bLog[j*16 +i] );

	// VAC1
	j++;
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_VACADC1_Ch0 +i)))->SetCheck( CControlLayer::Instance().m_DL.log.bLog[j*16 +i] );

	// VAC0
	j++;
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_SPRADC0_Ch0 +i)))->SetCheck( CControlLayer::Instance().m_DL.log.bLog[j*16 +i] );

	// SPR1
	j++;
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_SPRADC1_Ch0 +i)))->SetCheck( CControlLayer::Instance().m_DL.log.bLog[j*16 +i] );

	CString str;
	str.Format( "%d", CControlLayer::Instance().m_DL.other.nElapseLogging );
	GetDlgItem( IDC_EDIT_Elapse )->SetWindowText( str );

	str.Format( "%d", CControlLayer::Instance().m_DL.adc.nAveCounts );
	GetDlgItem( IDC_EDIT_Ave_Counts )->SetWindowText( str );
}

void CDlgDiagLog::SaveSetChangingParams()
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	SaveChangingParams( path );
	SaveLoggingParams( path );
}

void CDlgDiagLog::SaveChangingParams( char* path )
{
	char		stabuff[20];
	char		staitembuff[20];

	int			nTotal	= 11;
	int			i, j=0;
	CString		str;
	DWORD		output;
	int			nCheck[12], nMin[12], nMax[12], nStep[12], nIntv[12];

	// 变化量
	sprintf(stabuff, "ChangingParam");

	// SCN & LNS
	for( i=0; i<nTotal; i++,j++ )
	{
		nCheck[i] = ((CButton*)(GetDlgItem(IDC_CHECK_Stig_X +i)))->GetCheck();
		CControlLayer::Instance().m_DL.diag[ j ].nCheck = nCheck[i];

		GetDlgItem( IDC_EDIT_XstigDAC +i )->GetWindowText( str );
		char *bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nMin = (long)output;
		nMin[i] = (long)output;

		GetDlgItem( IDC_EDIT_XstigDAC2 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nMax = (long)output;
		nMax[i] = (long)output;

		GetDlgItem( IDC_EDIT_XstigDAC3 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nStep = (long)output;
		nStep[i] = (long)output;

		GetDlgItem( IDC_EDIT_XstigDAC4 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(10);
		sscanf(bufNum, "%d", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nIntv = (long)output;
		nIntv[i] = (long)output;
	}
	sprintf(staitembuff, "SCN-Check");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nCheck );
	sprintf(staitembuff, "SCN-Min");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nMin );
	sprintf(staitembuff, "SCN-Max");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nMax );
	sprintf(staitembuff, "SCN-Step");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nStep );
	sprintf(staitembuff, "SCN-Intv");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nIntv );

	// VAC
	nTotal = 12;
	for( i=0; i<nTotal; i++,j++ )
	{
		nCheck[i] = ((CButton*)(GetDlgItem(IDC_CHECK_CalI_TC0 +i)))->GetCheck();
		CControlLayer::Instance().m_DL.diag[ j ].nCheck = nCheck[i];

		GetDlgItem( IDC_EDIT_VACDAC_U44_CHA +i )->GetWindowText( str );
		char *bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nMin = (long)output;
		nMin[i] = (long)output;

		GetDlgItem( IDC_EDIT_VACDAC_U44_CHA2 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nMax = (long)output;
		nMax[i] = (long)output;

		GetDlgItem( IDC_EDIT_VACDAC_U44_CHA3 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nStep = (long)output;
		nStep[i] = (long)output;

		GetDlgItem( IDC_EDIT_VACDAC_U44_CHA4 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(10);
		sscanf(bufNum, "%d", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nIntv = (long)output;
		nIntv[i] = (long)output;
	}
	sprintf(staitembuff, "VAC-Check");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nCheck );
	sprintf(staitembuff, "VAC-Min");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nMin );
	sprintf(staitembuff, "VAC-Max");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nMax );
	sprintf(staitembuff, "VAC-Step");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nStep );
	sprintf(staitembuff, "VAC-Intv");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nIntv );

	// SPR
	nTotal = 4;
	for( i=0; i<nTotal; i++,j++ )
	{
		nCheck[i] = ((CButton*)(GetDlgItem(IDC_CHECK_SPR_DAC_CHA +i)))->GetCheck();
		CControlLayer::Instance().m_DL.diag[ j ].nCheck = nCheck[i];

		GetDlgItem( IDC_EDIT_SPRDAC_CHA +i )->GetWindowText( str );
		char *bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nMin = (long)output;
		nMin[i] = (long)output;

		GetDlgItem( IDC_EDIT_SPRDAC_CHA2 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nMax = (long)output;
		nMax[i] = (long)output;

		GetDlgItem( IDC_EDIT_SPRDAC_CHA3 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nStep = (long)output;
		nStep[i] = (long)output;

		GetDlgItem( IDC_EDIT_SPRDAC_CHA4 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(10);
		sscanf(bufNum, "%d", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nIntv = (long)output;
		nIntv[i] = (long)output;
	}
	sprintf(staitembuff, "SPR-Check");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nCheck );
	sprintf(staitembuff, "SPR-Min");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nMin );
	sprintf(staitembuff, "SPR-Max");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nMax );
	sprintf(staitembuff, "SPR-Step");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nStep );
	sprintf(staitembuff, "SPR-Intv");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nIntv );

	// MCI
	nTotal = 10;
	for( i=0; i<nTotal; i++,j++ )
	{
		nCheck[i] = ((CButton*)(GetDlgItem(IDC_CHECK_Offset_0 +i)))->GetCheck();
		CControlLayer::Instance().m_DL.diag[ j ].nCheck = nCheck[i];

		GetDlgItem( IDC_EDIT_Offset_0 +i )->GetWindowText( str );
		char *bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nMin = (long)output;
		nMin[i] = (long)output;

		GetDlgItem( IDC_EDIT_Offset_02 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nMax = (long)output;
		nMax[i] = (long)output;

		GetDlgItem( IDC_EDIT_Offset_03 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(5);
		sscanf(bufNum, "%4X", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nStep = (long)output;
		nStep[i] = (long)output;

		GetDlgItem( IDC_EDIT_Offset_04 +i )->GetWindowText( str );
		bufNum = str.GetBuffer(10);
		sscanf(bufNum, "%d", &output);
		CControlLayer::Instance().m_DL.diag[ j ].nIntv = (long)output;
		nIntv[i] = (long)output;
	}
	sprintf(staitembuff, "MCI-Check");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nCheck );
	sprintf(staitembuff, "MCI-Min");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nMin );
	sprintf(staitembuff, "MCI-Max");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nMax );
	sprintf(staitembuff, "MCI-Step");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nStep );
	sprintf(staitembuff, "MCI-Intv");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nIntv );

	char szBuf[20];
	sprintf(szBuf, "%d", CControlLayer::Instance().m_DL.other.nElapseChanging);
	sprintf(stabuff, "ChangingParam");
	sprintf(staitembuff, "TimeElapse");
	WritePrivateProfileString(stabuff, staitembuff, szBuf, path);
}

void CDlgDiagLog::SaveLoggingParams( char* path )
{
	char	stabuff[20];
	char	staitembuff[20];
	char szBuf[20];

	int		nTotal	= 16;
	int		i, j=0, k=0;
	int		nLog[16];

	sprintf(stabuff, "LoggingParam");
	CControlLayer::Instance().m_DL.log2.nCounts = 0;
	ZeroMemory( &(CControlLayer::Instance().m_DL.log2.logParams), sizeof(logParam)*6*16 );

	for( i=0; i<4; i++ )
	{
		CControlLayer::Instance().m_DL.graph[i].bGraph = FALSE;
		CControlLayer::Instance().m_DL.graph[i].nIndex = 0;
	}
	// 记录量
	sprintf(szBuf, "%d", CControlLayer::Instance().m_DL.other.bGaugeCompr);
	sprintf(staitembuff, "GaugeCompr");
	WritePrivateProfileString(stabuff, staitembuff, szBuf, path);
	if( CControlLayer::Instance().m_DL.other.bGaugeCompr )
	{
		CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nType	= 2;
		CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nNum	= 0;
		CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nMID	= 0;
		CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nCID	= 0;
		CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nChn	= 0;
		CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bType	= 0;
		CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bDiff	= 0;
		CControlLayer::Instance().m_DL.log2.nCounts++;
	}

	// SCN
	for( i=0; i<nTotal; i++ )
	{
		nLog[i] = ((CButton*)(GetDlgItem(IDC_CHECK_SCNADC_Ch0 +i)))->GetCheck();
		CControlLayer::Instance().m_DL.log.bLog[j*16 +i] = nLog[i];
		if( nLog[i] )
		{
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nNum	= 0;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nMID	= USB_MID_SCN;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nCID	= USB_CID_SCN_ADC_Read;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nChn	= i;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bDiff	= CControlLayer::Instance().m_DL.adc.chn[j*16 +i].bDiff;
			CControlLayer::Instance().m_DL.log2.nCounts++;
		}
		if( CControlLayer::Instance().m_DL.other.bGraph && nLog[i] && k<GraphDataBuffGroups )
		{
			CControlLayer::Instance().m_DL.graph[k].bGraph = TRUE;
			CControlLayer::Instance().m_DL.graph[k].nIndex = j*16 +i;
			k++;
		}
	}
	sprintf(staitembuff, "SCN");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nLog );

	j++;
	nTotal = 8;
	// LNS
	for( i=0; i<nTotal; i++ )
	{
		nLog[i] = ((CButton*)(GetDlgItem(IDC_CHECK_LNSADC_Ch0 +i)))->GetCheck();
		CControlLayer::Instance().m_DL.log.bLog[j*16 +i] = nLog[i];
		if( nLog[i] )
		{
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nNum	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nMID	= USB_MID_LNS;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nCID	= USB_CID_LNS_ADC_Read;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nChn	= i;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bType	= 0;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bDiff	= CControlLayer::Instance().m_DL.adc.chn[j*16 +i].bDiff;
			CControlLayer::Instance().m_DL.log2.nCounts++;
		}
		if( CControlLayer::Instance().m_DL.other.bGraph && nLog[i] && k<GraphDataBuffGroups )
		{
			CControlLayer::Instance().m_DL.graph[k].bGraph = TRUE;
			CControlLayer::Instance().m_DL.graph[k].nIndex = j*16 +i;
			k++;
		}
	}
	sprintf(staitembuff, "LNS");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nLog );

	for( i=0; i<9; i++ )
	{
		nLog[i] = ((CButton*)(GetDlgItem(IDC_CHECK_PARAM_LNS_L1DAC +i)))->GetCheck();
		CControlLayer::Instance().m_DL.log.bLogParam[i] = nLog[i];
	}
	sprintf(staitembuff, "LNS-Param");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, 9, nLog );

	j++;
	nTotal = 16;
	// VAC0
	for( i=0; i<nTotal; i++ )
	{
		nLog[i] = ((CButton*)(GetDlgItem(IDC_CHECK_VACADC0_Ch0 +i)))->GetCheck();
		CControlLayer::Instance().m_DL.log.bLog[j*16 +i] = nLog[i];
		if( nLog[i] )
		{
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nNum	= 2;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nMID	= USB_MID_VAC;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nCID	= USB_CID_VAC_ADC0_Read;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nChn	= i;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bDiff	= CControlLayer::Instance().m_DL.adc.chn[j*16 +i].bDiff;
			CControlLayer::Instance().m_DL.log2.nCounts++;
		}
		if( CControlLayer::Instance().m_DL.other.bGraph && nLog[i] && k<GraphDataBuffGroups )
		{
			CControlLayer::Instance().m_DL.graph[k].bGraph = TRUE;
			CControlLayer::Instance().m_DL.graph[k].nIndex = j*16 +i;
			k++;
		}
	}
	sprintf(staitembuff, "VAC0");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nLog );

	j++;
	// VAC1
	for( i=0; i<nTotal; i++ )
	{
		nLog[i] = ((CButton*)(GetDlgItem(IDC_CHECK_VACADC1_Ch0 +i)))->GetCheck();
		CControlLayer::Instance().m_DL.log.bLog[j*16 +i] = nLog[i];
		if( nLog[i] )
		{
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nNum	= 3;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nMID	= USB_MID_VAC;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nCID	= USB_CID_VAC_ADC1_Read;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nChn	= i;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bDiff	= CControlLayer::Instance().m_DL.adc.chn[j*16 +i].bDiff;
			CControlLayer::Instance().m_DL.log2.nCounts++;
		}
		if( CControlLayer::Instance().m_DL.other.bGraph && nLog[i] && k<GraphDataBuffGroups )
		{
			CControlLayer::Instance().m_DL.graph[k].bGraph = TRUE;
			CControlLayer::Instance().m_DL.graph[k].nIndex = j*16 +i;
			k++;
		}
	}
	sprintf(staitembuff, "VAC1");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nLog );

	j++;
	// SPR0
	for( i=0; i<nTotal; i++ )
	{
		nLog[i] = ((CButton*)(GetDlgItem(IDC_CHECK_SPRADC0_Ch0 +i)))->GetCheck();
		CControlLayer::Instance().m_DL.log.bLog[j*16 +i] = nLog[i];
		if( nLog[i] )
		{
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nNum	= 4;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nMID	= USB_MID_SPR;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nCID	= USB_CID_SPR_ADC0_Read;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nChn	= i;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bDiff	= CControlLayer::Instance().m_DL.adc.chn[j*16 +i].bDiff;
			CControlLayer::Instance().m_DL.log2.nCounts++;
		}
		if( CControlLayer::Instance().m_DL.other.bGraph && nLog[i] && k<GraphDataBuffGroups )
		{
			CControlLayer::Instance().m_DL.graph[k].bGraph = TRUE;
			CControlLayer::Instance().m_DL.graph[k].nIndex = j*16 +i;
			k++;
		}
	}
	sprintf(staitembuff, "SPR0");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nLog );

	j++;
	// SPR1
	for( i=0; i<nTotal; i++ )
	{
		nLog[i] = ((CButton*)(GetDlgItem(IDC_CHECK_SPRADC1_Ch0 +i)))->GetCheck();
		CControlLayer::Instance().m_DL.log.bLog[j*16 +i] = nLog[i];
		if( nLog[i] )
		{
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nNum	= 5;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nMID	= USB_MID_SPR;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nCID	= USB_CID_SPR_ADC0_Read;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].nChn	= i;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bType	= 1;
			CControlLayer::Instance().m_DL.log2.logParams[CControlLayer::Instance().m_DL.log2.nCounts].bDiff	= CControlLayer::Instance().m_DL.adc.chn[j*16 +i].bDiff;
			CControlLayer::Instance().m_DL.log2.nCounts++;
		}
		if( CControlLayer::Instance().m_DL.other.bGraph && nLog[i] && k<GraphDataBuffGroups )
		{
			CControlLayer::Instance().m_DL.graph[k].bGraph = TRUE;
			CControlLayer::Instance().m_DL.graph[k].nIndex = j*16 +i;
			k++;
		}
	}
	sprintf(staitembuff, "SPR1");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, staitembuff, LONGSTYLE, nTotal, nLog );

	sprintf(szBuf, "%d", CControlLayer::Instance().m_DL.other.nElapseLogging);
	sprintf(staitembuff, "TimeElapse");
	WritePrivateProfileString(stabuff, staitembuff, szBuf, path);
	sprintf(szBuf, "%d", CControlLayer::Instance().m_DL.adc.nAveCounts);
	sprintf(staitembuff, "AverageCounts");
	WritePrivateProfileString(stabuff, staitembuff, szBuf, path);
	sprintf(szBuf, "%d", CControlLayer::Instance().m_DL.other.bGraph);
	sprintf(staitembuff, "Graph");
	WritePrivateProfileString(stabuff, staitembuff, szBuf, path);
	sprintf(szBuf, "%d", CControlLayer::Instance().m_DL.other.bDebug);
	sprintf(staitembuff, "Debug");
	WritePrivateProfileString(stabuff, staitembuff, szBuf, path);
}

void CDlgDiagLog::OnOK() 
{
	// TODO: Add extra validation here
	CString str;

	GetDlgItem( IDC_EDIT_Elapse )->GetWindowText( str );
	CControlLayer::Instance().m_DL.other.nElapseChanging	= atoi( str );
	CControlLayer::Instance().m_DL.other.nElapseLogging	= atoi( str );

	GetDlgItem( IDC_EDIT_Ave_Counts )->GetWindowText( str );
	CControlLayer::Instance().m_DL.adc.nAveCounts = atoi( str );

	CControlLayer::Instance().m_DL.other.bGaugeCompr		= ((CButton*)(GetDlgItem( IDC_CHECK_GaugeCompr )))->GetCheck();
	CControlLayer::Instance().m_DL.other.bGraph			= ((CButton*)(GetDlgItem( IDC_CHECK_Graph )))->GetCheck();
	CControlLayer::Instance().m_DL.other.bDebug			= ((CButton*)(GetDlgItem( IDC_CHECK_Debug )))->GetCheck();

	SaveSetChangingParams();

	int i, nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_SCNADC_SE )))->GetCheck();
	if( nCheck )
	{
		// S.E. measurement for SCN 0---7 chn
		for( i=0; i<8; i++ )
			CControlLayer::Instance().m_DL.adc.chn[i].bDiff = FALSE;	// If value = 1 then S.E. / If value = 2 then diff mode
	}
	else
	{
		// Diff mode measurement for SCN 0---7 chn
		for( i=0; i<8; i++ )
			CControlLayer::Instance().m_DL.adc.chn[i].bDiff = TRUE;	// If value = 1 then S.E. / If value = 2 then diff mode
	}

	CDialog::OnOK();
}

void CDlgDiagLog::OnClearAll() 
{
	int i, j=0;
	int nTotal = 11;

	// SCN & LNS
	for( i=0; i<nTotal; i++,j++ )
		((CButton*)(GetDlgItem(IDC_CHECK_Stig_X +i)))->SetCheck( 0 );

	// VAC
	nTotal = 12;
	for( i=0; i<nTotal; i++,j++ )
		((CButton*)(GetDlgItem(IDC_CHECK_CalI_TC0 +i)))->SetCheck( 0 );

	// SPR
	nTotal = 4;
	for( i=0; i<nTotal; i++,j++ )
		((CButton*)(GetDlgItem(IDC_CHECK_SPR_DAC_CHA +i)))->SetCheck( 0 );

	// MCI
	nTotal = 10;
	for( i=0; i<nTotal; i++,j++ )
		((CButton*)(GetDlgItem(IDC_CHECK_Offset_0 +i)))->SetCheck( 0 );

	j=0;
	nTotal = 16;
	// SCN
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_SCNADC_Ch0 +i)))->SetCheck( 0 );

	j++;
	nTotal = 8;
	// LNS
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_LNSADC_Ch0 +i)))->SetCheck( 0 );
	for( i=0; i<9; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_PARAM_LNS_L1DAC +i)))->SetCheck( 0 );

	j++;
	nTotal = 16;
	// VAC0
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_VACADC0_Ch0 +i)))->SetCheck( 0 );

	j++;
	// VAC1
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_VACADC1_Ch0 +i)))->SetCheck( 0 );

	j++;
	// SPR0
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_SPRADC0_Ch0 +i)))->SetCheck( 0 );

	j++;
	// SPR1
	for( i=0; i<nTotal; i++ )
		((CButton*)(GetDlgItem(IDC_CHECK_SPRADC1_Ch0 +i)))->SetCheck( 0 );
}
