// DlgCaliCoadjust.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgCaliCoadjust.h"
#include "DlgDiagLogDiffGraph.h"

#include "DlgInput.h"
#include "MainFrm.h"
#include "ScanView.h"
#include "DlgPrompt.h"
#include "CFGFileManager.h"
#include "ControlLayer.h"
#include "AlgorithmManager.h"
#include "HVPowerDataManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "SEMLog/AlarmManager.h"
#include "Util.h"
#include "CommonFunctionManager.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const	long	CtrlDataSizeMax_WD		= 256;		// �����Ʋ���
const	UINT	nMag_MinStageTimerID	= 122;
const	UINT	nMag_MinStageTimerElapse= 30000;	// 30s

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliCoadjust dialog

CDlgCaliCoadjust::CDlgCaliCoadjust(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCaliCoadjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCaliCoadjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nType			= 12;
	m_nApproxMag	= -1;
	m_nMag_MinStageTimer		= 0;
	m_nMagMinStageTimeoutCounts = 0;
	m_nCoadjustMode	= 0;
}


void CDlgCaliCoadjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCaliCoadjust)
	DDX_Control(pDX, IDC_SCROLLBAR_Det45, m_scrollDet45);
	DDX_Control(pDX, IDC_SCROLLBAR_Det45Y, m_scrollDet45Y);
	DDX_Control(pDX, IDC_SCROLLBAR_XxoverDACSB, m_scrollXxoverDAC);
	DDX_Control(pDX, IDC_SCROLLBAR_YxoverDACSB, m_scrollYxoverDAC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCaliCoadjust, CDialog)
	//{{AFX_MSG_MAP(CDlgCaliCoadjust)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_1, OnBUTTONATCaliCoadjustApply)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_2, OnBUTTONATCaliCoadjustReset)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_3, OnBUTTONATCaliCoadjustRecord)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_4, OnBUTTONATCaliCoadjustLoad)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_5, OnBUTTONATCaliCoadjustAdd)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_SetLength, OnBUTTONATCaliCoadjustSetLength)
	ON_BN_CLICKED(IDC_CHECK_DebugMode_Coadjust, OnCHECKDebugModeCoadjust)
	ON_BN_CLICKED(IDC_BUTTON_Clear_All_Channels, OnClearAllChannels)
	ON_BN_CLICKED(IDC_BUTTON_WriteRateX, OnWriteRateX)
	ON_BN_CLICKED(IDC_BUTTON_WriteRateY, OnWriteRateY)
	ON_BN_CLICKED(IDC_BUTTON_WriteRateX2, OnWriteRateX2)
	ON_BN_CLICKED(IDC_BUTTON_WriteRateY2, OnWriteRateY2)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_STATIC_AT_Cali_Graph1, OnShowGraph1)
	ON_BN_CLICKED(IDC_STATIC_AT_Cali_Graph2, OnShowGraph2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_AT_Cali_Coadjust_Mode_OL, OnRadioAtCaliCoadjustModeOL)
	ON_BN_CLICKED(IDC_RADIO_AT_Cali_Coadjust_Mode_HV, OnRadioAtCaliCoadjustModeHV)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_CHECK_AT_Cali_Coadjust_1, IDC_CHECK_AT_Cali_Coadjust_11, OnCheckChanged)
	ON_COMMAND_RANGE(IDC_RADIO_Approx_Mag_10, IDC_RADIO_Approx_Mag_10000, OnApproxMagChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliCoadjust message handlers

void CDlgCaliCoadjust::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_1)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_1 ));
	GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_2)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_2 ));
	GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_3)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_3 ));
	GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_4)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_4 ));
	GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_5)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_5 ));

	GetDlgItem(IDC_STATIC_AT_Cali_Coadjust_Length)->SetWindowText(GetResString( IDS_STATIC_AT_Cali_Coadjust_Length ));
	GetDlgItem(IDC_BUTTON_AT_Cali_Coadjust_SetLength)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_SetLength ));

	GetDlgItem(IDC_BUTTON_WriteRateX)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_3 ));
	GetDlgItem(IDC_BUTTON_WriteRateY)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_3 ));
	GetDlgItem(IDC_BUTTON_WriteRateX2)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_3 ));
	GetDlgItem(IDC_BUTTON_WriteRateY2)->SetWindowText(GetResString( IDS_BUTTON_AT_Cali_Coadjust_3 ));

	// 2017.10.24
	// ���е�������Ϊ�ɱ�����洢�������ļ���
	double dRateMag[5];
	CControlLayer::Instance().GetMagnifierParam(dRateMag);

	CString str;
	for( int i=0; i<5; i++ )
	{
		str.Format( "x%d", (int)(dRateMag[i]) );
		GetDlgItem(IDC_RADIO_Approx_Mag_10 +i)->SetWindowText( str );
	}
}

BOOL CDlgCaliCoadjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// MCI
	m_scrollDet45.SetScrollRange( 0, 4095 );
	m_scrollDet45.SetScrollPos( 2047 );
	GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( "7FF" );
	m_scrollDet45Y.SetScrollRange( 0, 4095 );
	m_scrollDet45Y.SetScrollPos( 2047 );
	GetDlgItem( IDC_EDIT_Det45Y )->SetWindowText( "7FF" );
	// SCN
	m_scrollXxoverDAC.SetScrollRange( -2048, 2047 );
	m_scrollYxoverDAC.SetScrollRange( -2048, 2047 );
	m_scrollXxoverDAC.SetScrollPos( 0 );
	m_scrollYxoverDAC.SetScrollPos( 0 );
	GetDlgItem( IDC_EDIT_XxoverDAC )->SetWindowText( "0" );
	GetDlgItem( IDC_EDIT_YxoverDAC )->SetWindowText( "0" );
	// Mode
	((CButton*)(GetDlgItem(IDC_RADIO_AT_Cali_Coadjust_Mode_OL)))->SetCheck(1);
	((CButton*)(GetDlgItem(IDC_RADIO_AT_Cali_Coadjust_Mode_HV)))->SetCheck(0);
	CComboBox* pWD = (CComboBox*)(GetDlgItem(IDC_COMBO_AT_Cali_Coadjust_WD));
	pWD->AddString("5.0 mm");
	pWD->AddString("10.0 mm");
	pWD->AddString("15.0 mm");
	pWD->AddString("20.0 mm");
	pWD->AddString("25.0 mm");
	pWD->AddString("30.0 mm");
	pWD->SetCurSel(2);
	pWD->EnableWindow(FALSE);

	// 2021.11: ��С�Ŵ�������ʱ����
	InitMagMinLogFile();

	UpdateCaliData();
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCaliCoadjust::OnCancel() 
{
	// TODO: Add extra cleanup here
	theApp.m_dwOpr = 0;
	CSemCoreAssemble::Instance().m_Brightness.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_Contrast.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_CondenserLens.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_BeemAlignmentX.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_BeemAlignmentY.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_ScanShiftX.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_ScanShiftY.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_StigmatorX.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_StigmatorY.SetParams( 100, theApp.m_dwOpr );
	CSemCoreAssemble::Instance().m_SP_PMT.SetParams( 100, theApp.m_dwOpr );

	CDialog::OnCancel();
}

void CDlgCaliCoadjust::InitParams()
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	char szBuf[20];
	int nLeastSquares_power = 1;
	if(GetPrivateProfileString( "Coadjust", "power", "1", szBuf, 255, path) != 0)
		nLeastSquares_power = atoi( szBuf );

	/////////////////////////////////////////////////////////////
	// 15.05.14 ����˵�������ͳ����ʽʱ��3200�����Ч��ƫ��ϴ�
	// �����п���6200��ʹ��֮ǰ��ͳ����ʽ
	// �ʽ�m_nType��������ԭ��Ϊ0����6200,1����6900
	// �ָ�Ϊ��	0����6200��ʹ��ԭͳ����ʽ
	//			1����6200��ʹ����ͳ����ʽ(��ԭ3200ͳ�������¹�ʽ��ϳ��̶�ϵ��)
	//			11����6200��ʹ���±����Ʒ��̬����¹�ʽ��ϵ��
	//			12����6900(ʹ���±����Ʒ��̬����¹�ʽ��ϵ��)
	// �жϵ羵�ͺ�ʱ��ԭ��ע�����Ŀ����ʱʹ��ini�е�[Debug]/Start������6200������ͳ����ʽ����
	// ��ini��Ŀԭ�������ﾵ��ʾֵ��6X00-->3X00��ӳ�䣬���Ѳ���
	/////////////////////////////////////////////////////////////
	int nPos = theApp.m_strLogo.Find( "6" );
	CString strType = theApp.m_strLogo.Right( theApp.m_strLogo.GetLength() - nPos );
	if( strType == "6200" )
	{
		if( nLeastSquares_power < 2 )	// ֱ�߷���
			m_nType	= 0;
		else
		{
			if(GetPrivateProfileString("Debug", "Start", "", szBuf, 15, path) != 0)
			{
				int nStart = atoi(szBuf);
				if( nStart < 1 )
					m_nType = 1;	// ʹ�ö��ζ���ʽ���ͳ����ʽ��6200 15.07.22
				else
					m_nType = 11;	// ʹ�ö��ζ���ʽ��̬���ͳ����ʽ��6200 15.07.22
			}
			else
				m_nType	= 1;
		}
	}
	else
		m_nType = 12;	// ʹ�ö��ζ���ʽ��̬���ͳ����ʽ��6900 15.07.22
	if( m_nType < 10 )
	{
		for( int i=0; i<11; i++ )
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->ShowWindow( SW_HIDE );
//		GetDlgItem( IDC_STATIC_AT_Cali_Graph2 )->ShowWindow( SW_HIDE );
	}
	double dLength = 3.04;
	// ����ֱ��
	if( GetPrivateProfileString("Coadjust", "Length", "", szBuf, 15, path) != 0 )
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Length  )->SetWindowText( szBuf );
}

void CDlgCaliCoadjust::UpdateCaliData()
{
	CString str;
	str.Format( "%d", CControlLayer::Instance().m_Cali.nCaliCoCounts );
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Counts )->SetWindowText( str );

	if( m_nCoadjustMode < 1 )
		UpdateCaliData_OLWDMag();
	else
		UpdateCaliData_HV();
}

void CDlgCaliCoadjust::UpdateCaliData_OLWDMag()
{
	CString str;

	double dTotal = 0.0;
	for( int i=0; i<11; i++ )
	{
		if( m_nType < 1 )
		{
			// ԭ����6200
			if( CControlLayer::Instance().m_Cali.dCaliCoX[i] > 32767 )
				str.Format( "%.2f", (CControlLayer::Instance().m_Cali.dCaliCoX[i] -32768) /32.0 );
			else
				str = "0.0";
		}
		else
		{
			// 6900����6200
			if( CControlLayer::Instance().m_Cali.dCaliCoX[i] > 0.0 )
				str.Format( "%.6f", CControlLayer::Instance().m_Cali.dCaliCoX[i] );
			else
				str = "0.0";
		}
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->SetWindowText( str );

		str.Format( "%.1f", CControlLayer::Instance().m_Cali.dCaliCoWD[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->SetWindowText( str );

		if( m_nType > 10 )	// 6900 & 6200
		{
			if( CControlLayer::Instance().m_Cali.dCaliCoY[i] > 0.0 )
				str.Format( "%.6f", CControlLayer::Instance().m_Cali.dCaliCoY[i] );
			else
				str = "0.0";
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->SetWindowText( str );
		}
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
		{
			((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->SetCheck( 1 );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->EnableWindow( TRUE );
		}
		else
		{
			((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->SetCheck( 0 );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->EnableWindow( FALSE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->EnableWindow( FALSE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->EnableWindow( FALSE );
		}
	}
}

void CDlgCaliCoadjust::UpdateCaliData_HV()
{
	CString str;
	for( int i=0; i<11; i++ )
	{
		str.Format( "%.1f", CControlLayer::Instance().m_Cali.dCaliCoWD[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->SetWindowText( str );
		str.Format( "%.1f", CControlLayer::Instance().m_Cali.dCaliCoX[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->SetWindowText( str );
		str.Format( "%.1f", CControlLayer::Instance().m_Cali.dCaliCoY[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->SetWindowText( str );
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
		{
			((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->SetCheck( 1 );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->EnableWindow( TRUE );
		}
		else
		{
			((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->SetCheck( 0 );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->EnableWindow( FALSE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->EnableWindow( FALSE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->EnableWindow( FALSE );
		}
	}
}

bool LoadSemWDAdjustParam( const CString& filename, const WORD nLengthMax, WORD &nLength,
						  double dfObjectiveLensToWDIndex[], WORD wWDIndexToWD[], double	dfWDIndexToMAGmin[] )
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\" );
	strcat(path, filename);

	FILE *stream;
	stream = fopen( path, "r" );
	if( stream == NULL )
		return FALSE;

	long	i, nIndex, nFL2WDIndex=0, nWDIndex2WD=0;
	double	dfWDIndex2MAGmin=0.0, dfFL2WDIndex=0.0;
	char	cBuff[256];
	/* Set pointer to beginning of file: */
	fseek( stream, 0L, SEEK_SET );
	/* Read data back from file: */
	fscanf( stream, "%s", cBuff );
	fscanf( stream, "%s", cBuff );
	fscanf( stream, "%s", cBuff );

	/////////////////////////////////
	fscanf( stream, "%s", cBuff );
	fscanf( stream, "%ld",&nLength );
	fscanf( stream, "%s", cBuff );
	if ( nLength > nLengthMax )
		nLength = nLengthMax;
	/////////////////////////////////

	if( filename == "Coadjust_WD_3900.txt" )
	{
//		WORD	tmp_wObjectiveLensToWDIndex[CtrlDataSizeMax_WD];

		// 2005-2-25_ͳ��
		/////////////////////////////////
		for ( i=0; i<nLength; i++ )
		{	
			fscanf( stream, "%ld %lf %ld %lf",
				&nIndex, &dfFL2WDIndex, &nWDIndex2WD, &dfWDIndex2MAGmin );
			dfObjectiveLensToWDIndex[nIndex-1]	= dfFL2WDIndex;
			wWDIndexToWD[nIndex-1]				= (WORD)nWDIndex2WD;
			dfWDIndexToMAGmin[nIndex-1]			= dfWDIndex2MAGmin;
/*			nFL2WDIndex	= (int)( dfFL2WDIndex * 64 + 0.5 );
			if (nFL2WDIndex < 0)			nFL2WDIndex = 0;
			else if (nFL2WDIndex > 65535)	nFL2WDIndex = 65535;

			tmp_wObjectiveLensToWDIndex[nIndex-1]	= (WORD)nFL2WDIndex;
			wWDIndexToWD[nIndex-1]				= (WORD)nWDIndex2WD;
			dfWDIndexToMAGmin[nIndex-1]			= dfWDIndex2MAGmin;
*/		}
		/////////////////////////////////
/*
		// 2005-3-22_ͳ���������
		/////////////////////////////////
		DWORD	lGrid;
		for ( i=0; i<nLength-1; i++ )
		{
			lGrid = tmp_wObjectiveLensToWDIndex[i] + tmp_wObjectiveLensToWDIndex[i+1];
			wObjectiveLensToWDIndex[ i ]	= (WORD)(lGrid / 2.0 + 0.5);
		}
		wObjectiveLensToWDIndex[ nLength ]	= (WORD)65535;
*/		/////////////////////////////////
	}
	else
	{
		for ( i=0; i<nLength; i++ )
		{	
			fscanf( stream, "%ld %lf %ld %lf",
				&nIndex, &dfFL2WDIndex, &nWDIndex2WD, &dfWDIndex2MAGmin );
			dfObjectiveLensToWDIndex[nIndex-1]	= dfFL2WDIndex;
			wWDIndexToWD[nIndex-1]				= (WORD)nWDIndex2WD;
			dfWDIndexToMAGmin[nIndex-1]			= dfWDIndex2MAGmin;
/*			nFL2WDIndex	= (int)( dfFL2WDIndex * 64 + 0.5 );
			if (nFL2WDIndex < 0)			nFL2WDIndex = 0;
			else if (nFL2WDIndex > 65535)	nFL2WDIndex = 65535;

			wObjectiveLensToWDIndex[nIndex-1]	= (WORD)nFL2WDIndex;
			wWDIndexToWD[nIndex-1]			= (WORD)nWDIndex2WD;
			dfWDIndexToMAGmin[nIndex-1]		= dfWDIndex2MAGmin;
*/		}
//		wObjectiveLensToWDIndex[ nLength ]	= (WORD)65535;
	}

	fclose( stream );
	return TRUE;
}

void CDlgCaliCoadjust::Coadjust_Old6200( char* path )
{
	///////////////////////////////////////////////////////////////////////
	// ԭ����6200����ʹ�����¹�ʽ
	// M = 4 * K * sqrt( N/HV ) - CNT
	// WD = C0 * C1 / M - 120
	// Min = 360 / (WD +12)
	// K = Min * 32.1074 + 268.3826
	///////////////////////////////////////////////////////////////////////
	CString str;
	for(int i=0; i<11; i++ )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->GetWindowText( str );
//		str.Format( "%.2f", (CControlLayer::Instance().m_Cali.dCaliCoC[i] -32768) /32.0 );
		CControlLayer::Instance().m_Cali.dCaliCoX[i] = atof( str ) *32.0 +32768;

		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->GetWindowText( str );
		CControlLayer::Instance().m_Cali.dCaliCoWD[i] = atof( str );

		CControlLayer::Instance().m_Cali.bCaliCoSel[i] = ((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->GetCheck();

		// 6200: K = ( 360 /(WD + 12) ) * 32.1074 + 268.3826
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
			CControlLayer::Instance().m_Cali.dCaliCoY[i] = 360.0 / (CControlLayer::Instance().m_Cali.dCaliCoWD[i] +12) *32.1074 +268.3826;
	}

	// ����С���˷����ֱ�߷���
	double* pdX	= new double[CControlLayer::Instance().m_Cali.nCaliCoCounts];
	double* pdY	= new double[CControlLayer::Instance().m_Cali.nCaliCoCounts];
	int nIndex = 0;
	for(int i=0; i<10; i++ )
	{
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
		{
			pdX[nIndex]	= CControlLayer::Instance().m_Cali.dCaliCoX[i];
			pdY[nIndex]	= CControlLayer::Instance().m_Cali.dCaliCoY[i];
			nIndex++;
		}
	}
	double dKa, dKb;
	CAlgorithmManager::Instance().LeastSquares_LinearEquation2( CControlLayer::Instance().m_Cali.nCaliCoCounts, pdX, pdY, dKa, dKb );
	delete [] pdX;
	delete [] pdY;

	// ������Ի���
	str.Format( "ֱ�߷���Ϊ��K = %.6f   x OLctrl     %+.6f", dKa, dKb );
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Equation )->SetWindowText( str );
	// ϵ�����浽�����ļ�
	str.Format( "%.6f,%.6f", dKa, dKb );
	WritePrivateProfileString("Coadjust", "Equation", str, path);

	// ֪ͨSemCore��OL��Ч
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 9, dKa );
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 10, dKb );
}

void CDlgCaliCoadjust::Coadjust_62006900( char* path )
{
	///////////////////////////////////////////////////////////////////////
	// 6900����6200
	// ʹ��ԭ3200/3900���ﾵͳ�������
	// �ﾵ�빤������Ķ��ζ���ʽ��ϵ���Լ���С�Ŵ�����WD���ݹ�ϵ
	// ʵ��У׼ʱֻ��ʹ��2������������6200/6900��3200/3900�ﾵͳ�������ݵ����Թ�ϵ
	///////////////////////////////////////////////////////////////////////
	CString csFilename;
	if( m_nType < 2 )
		csFilename = _T("Coadjust_WD_3200.txt");
	else
		csFilename = _T("Coadjust_WD_3900.txt");

	WORD	nCtrlDataSize = 0;
	//���ڼ�����������
	//WorkDistance <--> ObjectiveLens electric-current : ObjectiveLens EC Ctl
	double	dfObjectiveLensToWDIndex[CtrlDataSizeMax_WD];
	//������ʾ��������
	//WorkDistance <--> ObjectiveLens electric-current : WorkDistance ( Unit : mm )
	WORD	nWDIndexToWD[CtrlDataSizeMax_WD];
	//���ڼ������ض����������£���С�Ŵ���
	double	dfWDIndexToMAGmin[CtrlDataSizeMax_WD];

	LoadSemWDAdjustParam( csFilename,
		CtrlDataSizeMax_WD, nCtrlDataSize,
		dfObjectiveLensToWDIndex, nWDIndexToWD, dfWDIndexToMAGmin );
	///////////////////////////////////////////////////////////////////////

	CString str;
	int i,j;
	for( i=0; i<11; i++ )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->GetWindowText( str );
		CControlLayer::Instance().m_Cali.dCaliCoX[i] = atof( str );

		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->GetWindowText( str );
		CControlLayer::Instance().m_Cali.dCaliCoWD[i] = atof( str );

		CControlLayer::Instance().m_Cali.bCaliCoSel[i] = ((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->GetCheck();
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && (CControlLayer::Instance().m_Cali.dCaliCoX[i] > 0) && (CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 0) )
		{
			for( j=0; j<nCtrlDataSize; j++ )
			{
				if( nWDIndexToWD[j]/10.0 == CControlLayer::Instance().m_Cali.dCaliCoWD[i] )
				{
					CControlLayer::Instance().m_Cali.dCaliCoY[i] = dfObjectiveLensToWDIndex[j];
					break;
				}
			}
		}
	}

	// ����С���˷����ֱ�߷���
	double* pdX	= new double[CControlLayer::Instance().m_Cali.nCaliCoCounts];
	double* pdY	= new double[CControlLayer::Instance().m_Cali.nCaliCoCounts];
	int nIndex = 0;
	for(int i=0; i<11; i++ )
	{
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
		{
			pdX[nIndex]	= CControlLayer::Instance().m_Cali.dCaliCoX[i];
			pdY[nIndex]	= CControlLayer::Instance().m_Cali.dCaliCoY[i];
			nIndex++;
		}
	}
	double dKa, dKb;
	CAlgorithmManager::Instance().LeastSquares_LinearEquation2( CControlLayer::Instance().m_Cali.nCaliCoCounts, pdX, pdY, dKa, dKb );
	delete [] pdX;
	delete [] pdY;


	// ֪ͨSemCore��OL��Ч
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 9, dKa );
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 10, dKb );

	// ������Ի���
	str.Format( "ֱ�߷���Ϊ��OLdisp_src = %.6f   x OLdisp_new     %+.6f", dKa, dKb );
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Equation )->SetWindowText( str );
	// ϵ�����浽�����ļ�
	str.Format( "%.6f,%.6f", dKa, dKb );
	WritePrivateProfileString("Coadjust", "Equation", str, path);
}

void CDlgCaliCoadjust::Coadjust_New6X00( char* path )
{
	///////////////////////////////////////////////////////////////////////
	// ʹ�����¹�ʽ��
	// �ﾵOL disp�빤������WD�Ĺ�ϵΪ��OL disp = 1 / sqrt(a*WD*WD +b*WD +C)
	// ��������WD����С�Ŵ���Min�Ĺ�ϵΪ��Min = a / (WD +b)
	// ʵ��У׼ʱʹ�����α����Ʒ���õ���������
	// ÿ�����ݰ������������롢�ﾵ��ʾֵOL disp������С�Ŵ���Mag min
	// Mag min = ��߳�����ʾֵD(uint:mm) * �Ŵ�����ʾֵMag / 3
	///////////////////////////////////////////////////////////////////////

	CString str;
	int i,j,k,l;
	j = 0;
	k = 0;
	l = 0;
	if( m_nType < 12 )
	{
		for( i=0; i<11; i++ )	// 6200
		{
			if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
			{
				if( CControlLayer::Instance().m_Cali.dCaliCoWD )
				{
					if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 12.5 && CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 11.5 )	// 6200���12mm
						j++;
					else if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 11.9 )
						k++;
					else
						l++;
				}
			}
		}
		str = "Param count at WD=12mm > 1";
	}
	else
	{
		for( i=0; i<11; i++ )	// 6900
		{
			if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
			{
				if( CControlLayer::Instance().m_Cali.dCaliCoWD )
				{
					if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 15.5 && CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 14.5 )	// 6900���15mm
						j++;
					else if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 14.9 )
						k++;
					else
						l++;
				}
			}
		}
		str = "Param count at WD=15mm > 1";
	}
	if( j != 1 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += str;
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	if( (k<1 && l<1) || (k+l <3) )	// ������Ŀ����
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "Param counts < 4";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	int nCountsWeight1, nCountsWeight2, nCountsWeight3;	// Ȩ��
	// �����������ó���297�����У�
	// ���Ȩ��240��С�ڱ�ߵ��ܹ�Ȩ��50�����ڱ�ߵ��ܹ�Ȩ��7
	nCountsWeight1	= 240;
	if( k < 1 )
	{
		nCountsWeight2 = 0;
		nCountsWeight3 = 56 / l;
	}
	else if( l < 1 )
	{
		nCountsWeight2 = 56 / k;
		nCountsWeight3 = 0;
	}
	else
	{
		nCountsWeight2	= 50 / k;
		nCountsWeight3	= 7 / l;
	}

	double dFactor[299],dX[299],dY[299],s[299][299],t[299][1];
	int nCounts,nPower = 2;
	///////////////////////////////////////////////////////////////////////
	// ��һ��
	// �ﾵOL disp�빤������WD�Ĺ�ϵΪ��OL disp = 1 / sqrt(a*WD*WD +b*WD +C)
	// ����1 / OLdisp /OLdisp = a*WD*WD +b*WD +C
	// ��(1 / OLdisp /OLdisp)��Ϊ����Y������Y = aX^2 +bX +C
	// ���ԣ�ʹ����С������Ϸ���϶���ʽ
	// ��ʼ��X��Y����
	j=0;
	for( i=0; i<11; i++ )
	{
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && (CControlLayer::Instance().m_Cali.dCaliCoX[i] > 0) && (CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 0) )
		{
			if( m_nType < 12 )
			{	// 6200
				if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 12.5 && CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 11.5 )	// 6200���12mm
					nCounts	= nCountsWeight1;
				else if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 11.9 )
					nCounts	= nCountsWeight2;
				else
					nCounts	= nCountsWeight3;
			}
			else
			{	// 6900
				if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 15.5 && CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 14.5 )	// 6900���15mm
					nCounts	= nCountsWeight1;
				else if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 14.9 )
					nCounts	= nCountsWeight2;
				else
					nCounts	= nCountsWeight3;
			}
			for( k=0; k<nCounts; k++ )
			{
				dX[j]	= CControlLayer::Instance().m_Cali.dCaliCoWD[i];
				dY[j]	= 1.0 /CControlLayer::Instance().m_Cali.dCaliCoX[i] /CControlLayer::Instance().m_Cali.dCaliCoX[i];
				j++;
			}
		}
	}
	nCounts = j;

	// ���ǰ�ı任׼��
	for(i=0;i<=nPower;i++)
	{
		for(j=0;j<=nPower;j++)
		{
			s[i][j]=0.0;
			for(k=0;k<=nCounts-1;k++)
				s[i][j]+= CAlgorithmManager::Instance().LeastSquares_Polynomial_cf(i+j,dX[k]);
		}
		t[i][0]=0.0;
		for(j=0;j<=nCounts-1;j++)
			t[i][0]+=dY[j]* CAlgorithmManager::Instance().LeastSquares_Polynomial_cf(i,dX[j]); 
	}
	for(i=0;i<=nPower;i++)
		s[i][nPower+1]=t[i][0];

	// ���
	CAlgorithmManager::Instance().LeastSquares_Polynomial_ColPivot(s,nPower+1,t,dFactor);
	// ������Ի���
	str.Format( "OL disp = 1/sqrt( %+e * WD * WD  %+e * WD  %+e )", dFactor[2], dFactor[1], dFactor[0] );
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Equation )->SetWindowText( str );
	theApp.DisplayMsg( str );
	// ϵ�����浽�����ļ�
	CString strFactors = "";
	for( i=nPower; i>0; i-- )
	{
		str.Format( "%e,", dFactor[i] );
		strFactors += str;
	}
	str.Format( "%e", dFactor[i] );
	strFactors += str;
	WritePrivateProfileString("Coadjust", "Factors", strFactors, path);
// 	CString sFactors[3];
// 	if( CCommonFunctionManager::Instance().ReadInOneLine( path, "Coadjust", "Factors", STRSTYLE, 3, &(sFactors[0]) ) == 3 )
// 		double dF = atof( sFactors[0] );

	CString strFactors2 = "";
	for( i=nPower; i>0; i-- )
	{
		str.Format( "%.20f,", dFactor[i] );
		strFactors2 += str;
	}
	str.Format( "%.20f", dFactor[i] );
	strFactors2 += str;
	WritePrivateProfileString("Coadjust", "Factors2", strFactors2, path);
// 	double* pdFactors = new double[nPower+1];
// 	CCommonFunctionManager::Instance().ReadInOneLine( path, "Coadjust", "Factors2", DOUBLESTYLE, nPower+1, pdFactors );
// 	delete [] pdFactors;

	// ��SemCore��Ч
	CSemCoreAssemble::Instance().m_WD.SetParams( 11, dFactor[2] );
	CSemCoreAssemble::Instance().m_WD.SetParams( 12, dFactor[1] );
	CSemCoreAssemble::Instance().m_WD.SetParams( 13, dFactor[0] );
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// �ڶ���
	// ��������WD����С�Ŵ���Min�Ĺ�ϵΪ��Min = a / (WD +b)
	// ����WD = a * (1/Min) -b
	// ��(1/Min)��Ϊ����X��(-b)��Ϊ����B������Y = a*X +B
	// ���ԣ�ʹ����С������Ϸ����ֱ��
	j=0;
	for( i=0; i<11; i++ )
	{
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && (CControlLayer::Instance().m_Cali.dCaliCoY[i] > 0) && (CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 0) )
		{
			if( m_nType < 12 )
			{	// 6200
				if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 12.5 && CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 11.5 )	// 6200���12mm
					nCounts	= nCountsWeight1;
				else if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 11.9 )
					nCounts	= nCountsWeight2;
				else
					nCounts	= nCountsWeight3;
			}
			else
			{	// 6900
				if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 15.5 && CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 14.5 )	// 6900���15mm
					nCounts	= nCountsWeight1;
				else if( CControlLayer::Instance().m_Cali.dCaliCoWD[i] < 14.9 )
					nCounts	= nCountsWeight2;
				else
					nCounts	= nCountsWeight3;
			}
			for( k=0; k<nCounts; k++ )
			{
				dX[j]	= 1.0 / CControlLayer::Instance().m_Cali.dCaliCoY[i];
				dY[j]	= CControlLayer::Instance().m_Cali.dCaliCoWD[i];
				j++;
			}
		}
	}
	double dKa, dKb;
	CAlgorithmManager::Instance().LeastSquares_LinearEquation2( j, dX, dY, dKa, dKb );
	dKb *= -1.0;
	// ������Ի���
	GetDlgItem(IDC_EDIT_AT_Cali_Coadjust_Equation)->GetWindowText(str);
	str += "\r\n";
	strFactors.Format( "Min = %.6f / (WD %+.6f)", dKa, dKb );
	str += strFactors;
	GetDlgItem(IDC_EDIT_AT_Cali_Coadjust_Equation)->SetWindowText(str);
	// ϵ�����浽�����ļ�
	strFactors.Format( "%.6f,%.6f", dKa, dKb );
	WritePrivateProfileString("Coadjust", "FactorsWDmin", strFactors, path);

	// ��SemCore��Ч
	CSemCoreAssemble::Instance().m_Magnifier.SetParams( 11, dKa );
	CSemCoreAssemble::Instance().m_Magnifier.SetParams( 12, dKb );
}

void CDlgCaliCoadjust::Coadjust_HV( char* path )
{
	///////////////////////////////////////////////////////////////////////
	// ��ѹ���ﾵIndexֵ�Ĺ�ϵ���� ��ѹ��KV��/30�������� ��HV������ OLindex = b *(HV ^ a)
	// ����bϵ���ɺ��ԣ�����ͨ����С���˷��õ�ϵ��a�����������߾����µõ���aϵ��д��ini����
	// ��OLindex = b * (HV ^ a) ==> log10 (OLindex) = log10 (b) + a* log10 (HV)
	// �� log10 (OLindex) ��Ϊ����Y��log10 (b) ��Ϊ����B��log10 (HV) ��Ϊ����X��
	// ����Y = a*X +B
	// ���ԣ�ʹ����С������Ϸ����ֱ��
	///////////////////////////////////////////////////////////////////////

	CString str;
	int i,j,k,l;
	j = 0;
	k = 0;
	l = 0;

	double dFactor[299],dX[299],dY[299],s[299][299],t[299][1];
	int nCounts = 25;	// 25*11=265;
	///////////////////////////////////////////////////////////////////////
	// �ڶ���
	// ��������WD����С�Ŵ���Min�Ĺ�ϵΪ��Min = a / (WD +b)
	// ����WD = a * (1/Min) -b
	// ��(1/Min)��Ϊ����X��(-b)��Ϊ����B������Y = a*X +B
	// ���ԣ�ʹ����С������Ϸ����ֱ��
	j=0;
	for( i=0; i<11; i++ )
	{
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && (CControlLayer::Instance().m_Cali.dCaliCoY[i] > 0) && (CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 0) )
		{
			for( k=0; k<nCounts; k++ )
			{
				dX[j]	= log10(CControlLayer::Instance().m_Cali.dCaliCoX[i] /30.0);
				dY[j]	= log10(CControlLayer::Instance().m_Cali.dCaliCoY[i] );
				j++;
			}
		}
	}
	double dKa, dKb;
	CAlgorithmManager::Instance().LeastSquares_LinearEquation2( j, dX, dY, dKa, dKb );
	dKb *= -1.0;
	// ������Ի���
	GetDlgItem(IDC_EDIT_AT_Cali_Coadjust_Equation)->GetWindowText(str);
	str.Format( "OLindex = %.6f * [ ((KV)/30.0) ^ %.4f]", dKb, dKa );
	GetDlgItem(IDC_EDIT_AT_Cali_Coadjust_Equation)->SetWindowText(str);

	// ϵ�����浽�����ļ�������ÿ��ֻ�ܵõ�һ��WD�µĸ�ѹͳ��ϵ��������Ҫע��ϵ���Ĵ���λ��
	// �ȵõ���ǰ�ѱ��浽�����ļ��е�ϵ��
	double* pdValue = new double[6];
	for( i=0; i<6; i++ )
		pdValue[i] = -1000.0;
	int nIndex, nCount = CCommonFunctionManager::Instance().ReadInOneLine( path, "Coadjust", "FactorsHV", DOUBLESTYLE, 6, pdValue );
	double dWD = CControlLayer::Instance().m_Cali.dCaliCoWD[0];
	if( dWD < 5.5 )
		nIndex = 0;
	else if( dWD < 10.5 )
		nIndex = 1;
	else if( dWD < 15.5 )
		nIndex = 2;
	else if( dWD < 20.5 )
		nIndex = 3;
	else if( dWD < 25.5 )
		nIndex = 4;
	else
		nIndex = 5;
	pdValue[nIndex] = dKa;
	CString strTemp;
	str = "";
	for( i=0; i<6; i++ )
	{
		if( pdValue[i] < -999.0 )
			strTemp = ",";
		else
			strTemp.Format( "%.6f,", pdValue[i] );
		str += strTemp;
	}
	strTemp = str.Left( str.GetLength() -1 );	// ȥ�����һ�����ŷָ���
	WritePrivateProfileString("Coadjust", "FactorsHV", strTemp, path);
	delete [] pdValue;

	// ��SemCore��Ч
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 21, 1.0 );
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( SET_PARAMS_Rate, CHVPowerDataManager::Instance().m_dHV );
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjustApply() 
{
	// ��ȡ��������
	CString str;
	int i, j = 0;
	for( i=0; i<11; i++ )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->GetWindowText( str );
		CControlLayer::Instance().m_Cali.dCaliCoX[i]	= atof( str );
		
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->GetWindowText( str );
		CControlLayer::Instance().m_Cali.dCaliCoWD[i]	= atof( str );
		
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->GetWindowText( str );
		CControlLayer::Instance().m_Cali.dCaliCoY[i]	= atof( str );
		
		CControlLayer::Instance().m_Cali.bCaliCoSel[i] = ((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->GetCheck();
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
			j++;
	}

	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Counts )->GetWindowText( str );
	CControlLayer::Instance().m_Cali.nCaliCoCounts = atoi( str );
	if( CControlLayer::Instance().m_Cali.nCaliCoCounts != j )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "Counts Exceeded !";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}

	// ���ݸ����Ƿ����Ҫ��
	if( CControlLayer::Instance().m_Cali.nCaliCoCounts < 2 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "Counts Exceeded !";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}

	if( m_nCoadjustMode < 1 )
		CalculateOLWDMag();
	else
		CalculateHV();
}

void CDlgCaliCoadjust::CalculateOLWDMag()
{
	CString str;
	int i, j=0;
	// ���ÿ��ֵ�Ĺ��������Ƿ���ͬ��������ͬ����ʾ�û�ֻѡһ��
	// ��֤ÿ��ֵ�Ĺ������붼��ͬ
	for( i=0; i<11; i++ )
	{
		for( j=0; j<11; j++ )
		{
			if( j == i )
				continue;
			if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && CControlLayer::Instance().m_Cali.bCaliCoSel[j]
				&& (fabs( CControlLayer::Instance().m_Cali.dCaliCoWD[i] -CControlLayer::Instance().m_Cali.dCaliCoWD[j]) < 0.5) )
			{
				CDlgPrompt dlg;
				dlg.m_byteType = 0x01;
				dlg.m_strMsg = "\r\n";
				dlg.m_strMsg += "\r\n";
				dlg.m_strMsg += "The WD value cannot be the same.";
				dlg.m_strMsg += "Please re-select the data.";
				dlg.m_strMsg += "\r\n";
				dlg.m_strMsg += "\r\n";
				dlg.DoModal();
				return;
			}
		}
	}

	theApp.DisplayMsg("Press calc & apply OL-WD-Mag on Coadjust dialog");
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	switch( m_nType )
	{
	case	0:
		Coadjust_Old6200( path );
		break;
	case	1:
		Coadjust_62006900( path );
		break;
	case	11:
	case	12:
		Coadjust_New6X00( path );
		break;
	}
	OnBUTTONATCaliCoadjustRecord();
}

void CDlgCaliCoadjust::CalculateHV()
{
	CString str;
	int i, j=0;
	// ���ÿ��ֵ�ĸ�ѹ�Ƿ���ͬ��������ͬ����ʾ�û�ֻѡһ��
	// ��֤ÿ��ֵ�ĸ�ѹ����ͬ
	for( i=0; i<11; i++ )
	{
		for( j=0; j<11; j++ )
		{
			if( j == i )
				continue;
			if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && CControlLayer::Instance().m_Cali.bCaliCoSel[j]
				&& (fabs( CControlLayer::Instance().m_Cali.dCaliCoX[i] -CControlLayer::Instance().m_Cali.dCaliCoX[j]) < 0.5) )
			{
				CDlgPrompt dlg;
				dlg.m_byteType = 0x01;
				dlg.m_strMsg = "\r\n";
				dlg.m_strMsg += "\r\n";
				dlg.m_strMsg += "The HV value cannot be the same.";
				dlg.m_strMsg += "Please re-select the data.";
				dlg.m_strMsg += "\r\n";
				dlg.m_strMsg += "\r\n";
				dlg.DoModal();
				return;
			}
			if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && CControlLayer::Instance().m_Cali.bCaliCoSel[j]
				&& (fabs( CControlLayer::Instance().m_Cali.dCaliCoWD[i] -CControlLayer::Instance().m_Cali.dCaliCoWD[j]) > 0.01) )
			{
				CDlgPrompt dlg;
				dlg.m_byteType = 0x01;
				dlg.m_strMsg = "\r\n";
				dlg.m_strMsg += "\r\n";
				dlg.m_strMsg += "The WD value should be the same.";
				dlg.m_strMsg += "Please re-select the data.";
				dlg.m_strMsg += "\r\n";
				dlg.m_strMsg += "\r\n";
				dlg.DoModal();
				return;
			}
		}
	}
	for( i=0; i<11; i++ )
	{
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i]
			&& (CControlLayer::Instance().m_Cali.dCaliCoY[i] > 32766.0 || CControlLayer::Instance().m_Cali.dCaliCoY[i] < 640.0) )
		{
			CDlgPrompt dlg;
			dlg.m_byteType = 0x01;
			dlg.m_strMsg = "\r\n";
			dlg.m_strMsg += "\r\n";
			dlg.m_strMsg += "The Objective Lens value is invalid.";
			dlg.m_strMsg += "Please re-select the data.";
			dlg.m_strMsg += "\r\n";
			dlg.m_strMsg += "\r\n";
			dlg.DoModal();
			return;
		}
	}

	theApp.DisplayMsg("Press calc & apply HV on Coadjust dialog");
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\KYKYcfg.ini");
	Coadjust_HV( path );
	OnBUTTONATCaliCoadjustRecord();
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjustReset() 
{
	// ��������ֵΪ0���������ѡ�б�־
	CControlLayer::Instance().m_Cali.nCaliCoCounts = 0;
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Counts )->SetWindowText( "0" );

	for( int i=0; i<11; i++ )
	{
		CControlLayer::Instance().m_Cali.dCaliCoX[i]	= 0.0;
		CControlLayer::Instance().m_Cali.dCaliCoY[i]	= 0.0;
		CControlLayer::Instance().m_Cali.dCaliCoWD[i]		= 0.0;
		CControlLayer::Instance().m_Cali.bCaliCoSel[i]		= FALSE;

		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->SetWindowText( "0.0" );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->SetWindowText( "0.0" );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->SetWindowText( "0.0" );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +i )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +i )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +i )->EnableWindow( FALSE );
		((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->SetCheck( 0 );
	}

	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Equation )->SetWindowText( "" );

	// ���������ļ��еĲ���
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	CString str = _T("");
	if( m_nCoadjustMode < 1 )
	{
		WritePrivateProfileString("Coadjust", "Equation", str, path);
		WritePrivateProfileString("Coadjust", "Factors", str, path);
		WritePrivateProfileString("Coadjust", "Factors2", str, path);
		WritePrivateProfileString("Coadjust", "FactorsWDmin", str, path);
		// ���õĳ�ʼ��������Ч
		// ��SemCore��Ч
		CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 9, 1.0 );
		CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 10, 0.0 );
		CSemCoreAssemble::Instance().m_WD.SetParams( 11, 0.0 );
		CSemCoreAssemble::Instance().m_WD.SetParams( 12, 0.0 );
		CSemCoreAssemble::Instance().m_WD.SetParams( 13, 0.0 );
		CSemCoreAssemble::Instance().m_Magnifier.SetParams( 11, 1.0 );
		CSemCoreAssemble::Instance().m_Magnifier.SetParams( 12, 0.0 );

		theApp.DisplayMsg("Press clear all OL-WD-Mag coadjust params");
	}
	else
	{
		WritePrivateProfileString("Coadjust", "FactorsHV", str, path);
		theApp.DisplayMsg("Press clear all HV coadjust params");
		// ���õĳ�ʼ��������Ч
		// ��SemCore��Ч
		CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 21, 0.0 );
	}
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjustRecord() 
{
	// TODO: Add your control notification handler code here
	// ��¼ͳ�����ݵ��ĵ�
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strCoadjustFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d%H%M");
	if( m_nCoadjustMode < 1 )
	{
		strCoadjustFileName.Format( "%s\\Config\\Coadjust-%s.csv", appfilepath, strTime );
		CFile file;
		file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite );

		if( file.m_hFile != CFile::hFileNull )
		{
			CString str;
			str.Format( "Check,OL_disp,WD,Mag_Min\r\n" );
			file.Write( str, str.GetLength() );
			for( int i=0; i<11; i++ )
			{
				str.Format( "%d,%16.6f,%13.6f,%13.6f\r\n", CControlLayer::Instance().m_Cali.bCaliCoSel[i], CControlLayer::Instance().m_Cali.dCaliCoX[i],
							CControlLayer::Instance().m_Cali.dCaliCoWD[i], CControlLayer::Instance().m_Cali.dCaliCoY[i] );
				file.Write( str, str.GetLength() );
			}
			GetDlgItemText( IDC_EDIT_AT_Cali_Coadjust_Equation, str );
			file.Write( str, str.GetLength() );
		}
		file.Close();
		theApp.DisplayMsg("Press record OL-WD-Mag data on Coadjust dialog");
	}
	else
	{
		strCoadjustFileName.Format( "%s\\Config\\CoadjustHV-%s.csv", appfilepath, strTime );
		CFile file;
		file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite );

		if( file.m_hFile != CFile::hFileNull )
		{
			CString str;
			str.Format( "Check,WD,HV,lCtlIndex\r\n" );
			file.Write( str, str.GetLength() );
			for( int i=0; i<11; i++ )
			{
				str.Format( "%d,%.1f,%.1f,%.1f\r\n", CControlLayer::Instance().m_Cali.bCaliCoSel[i], CControlLayer::Instance().m_Cali.dCaliCoWD[i],
							CControlLayer::Instance().m_Cali.dCaliCoX[i], CControlLayer::Instance().m_Cali.dCaliCoY[i] );
				file.Write( str, str.GetLength() );
			}
			GetDlgItemText( IDC_EDIT_AT_Cali_Coadjust_Equation, str );
			file.Write( str, str.GetLength() );
		}
		file.Close();
		theApp.DisplayMsg("Press record HV data on Coadjust dialog");
	}
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjustLoad() 
{
	// TODO: Add your control notification handler code here
	// ���ĵ���ȡͳ������
	CFileDialog dlg(TRUE, "csv", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "csv�ļ� (*.csv)|*.csv||", NULL);
	if( dlg.DoModal() != IDOK )
		return;

	CString csFullPath = dlg.GetPathName();
	FILE *stream = fopen( (LPCSTR)csFullPath, "r" );
	if( stream == NULL )
		return;

	TCHAR	cBuff[256];
	/* Set pointer to beginning of file: */
	fseek( stream, 0L, SEEK_SET );
	/* Read data back from file: */
	fscanf( stream, "%s", cBuff );
	int nCheck, i, j=0;
	if( m_nCoadjustMode < 1 )
	{
		double dOL, dWD, dMin;
		for( i=0; i<11; i++ )
		{
			fscanf( stream, "%ld,%lf,%lf,%lf", &nCheck, &dOL, &dWD, &dMin );
			CControlLayer::Instance().m_Cali.bCaliCoSel[i] = nCheck;
			CControlLayer::Instance().m_Cali.dCaliCoX[i]	= dOL;
			CControlLayer::Instance().m_Cali.dCaliCoWD[i]	= dWD;
			CControlLayer::Instance().m_Cali.dCaliCoY[i]	= dMin;
			if( nCheck > 0 )
				j++;
		}
		fclose( stream );
		CControlLayer::Instance().m_Cali.nCaliCoCounts = j;
		UpdateCaliData();
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Equation )->SetWindowText( "" );
		theApp.DisplayMsg("Press read OL-WD-Mag data from file on Coadjust dialog");
	}
	else
	{
		double dWD, dHV, lIndex;
		for( i=0; i<11; i++ )
		{
			fscanf( stream, "%ld,%lf,%lf,%lf", &nCheck, &dWD, &dHV, &lIndex );
			CControlLayer::Instance().m_Cali.bCaliCoSel[i]	= nCheck;
			CControlLayer::Instance().m_Cali.dCaliCoWD[i]	= dWD;
			CControlLayer::Instance().m_Cali.dCaliCoX[i]	= dHV;
			CControlLayer::Instance().m_Cali.dCaliCoY[i]	= lIndex;
			if( nCheck > 0 )
				j++;
		}
		fclose( stream );
		CControlLayer::Instance().m_Cali.nCaliCoCounts = j;
		UpdateCaliData();
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Equation )->SetWindowText( "" );
		theApp.DisplayMsg("Press read HV data from file on Coadjust dialog");
	}
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjustAdd() 
{
	// TODO: Add your control notification handler code here
	if( m_nCoadjustMode < 1 )
	{
		theApp.DisplayMsg("Press add OL ctrl");
		if( CControlLayer::Instance().m_Cali.nCaliCoCounts > 9 )
		{
			CDlgPrompt dlg;
			dlg.m_byteType = 0x01;
			dlg.m_strMsg = "\r\n";
			dlg.m_strMsg += "\r\n";
			dlg.m_strMsg += "Counts Exceeded !";
			dlg.m_strMsg += "\r\n";
			dlg.m_strMsg += "\r\n";
			dlg.DoModal();
			return;
		}
		CDlgInput dlg;
		dlg.m_nTitleIndex = IDS_SD_PROMPT_Input_Coadjust;
		if( dlg.DoModal() != IDOK )
			return;

		double dWD = atof( dlg.m_strInput );
		AddRecord_OLWDMag( dWD );
	}
	else
	{
		theApp.DisplayMsg("Press add HV ctrl");
		if( CControlLayer::Instance().m_Cali.nCaliCoCounts > 10 )
		{
			CDlgPrompt dlg;
			dlg.m_byteType = 0x01;
			dlg.m_strMsg = "\r\n";
			dlg.m_strMsg += "\r\n";
			dlg.m_strMsg += "Counts Exceeded !";
			dlg.m_strMsg += "\r\n";
			dlg.m_strMsg += "\r\n";
			dlg.DoModal();
			return;
		}

		CString str;
		int nSel = ((CComboBox*)(GetDlgItem(IDC_COMBO_AT_Cali_Coadjust_WD)))->GetCurSel();
		double dWD = (nSel +1) *5.0;
		AddRecord_HV( dWD );
	}
}

void CDlgCaliCoadjust::AddRecord_OLWDMag( double dWD )
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	/////////////////////////////////////////////////////////////
	// 15.05.14 ����˵�������ͳ����ʽʱ��3200�����Ч��ƫ��ϴ�
	// �����п���6200��ʹ��֮ǰ��ͳ����ʽ
	// �ʽ�m_nType��������ԭ��Ϊ0����6200,1����6900
	// �ָ�Ϊ��	0����6200��ʹ��ԭͳ����ʽ
	//			1����6200��ʹ����ͳ����ʽ(��ԭ3200ͳ�������¹�ʽ��ϳ��̶�ϵ��)
	//			11����6200��ʹ���±����Ʒ��̬����¹�ʽ��ϵ��
	//			12����6900(ʹ���±����Ʒ��̬����¹�ʽ��ϵ��)
	// �жϵ羵�ͺ�ʱ��ԭ��ע�����Ŀ����ʱʹ��ini�е�[Debug]/Start������6200������ͳ����ʽ����
	// ��ini��Ŀԭ�������ﾵ��ʾֵ��6X00-->3X00��ӳ�䣬���Ѳ���
	/////////////////////////////////////////////////////////////
	char szBuf[20];
	int nType = 0;
	int nPos = theApp.m_strLogo.Find( "6" );
	CString strType = theApp.m_strLogo.Right( theApp.m_strLogo.GetLength() - nPos );
	if( strType == "6200" )
	{
		int nLeastSquares_power = 1;
		if(GetPrivateProfileString( "Coadjust", "power", "1", szBuf, 255, path) != 0)
			nLeastSquares_power = atoi( szBuf );
		if( nLeastSquares_power < 2 )	// ֱ�߷���
			nType	= 0;
		else
		{
			if(GetPrivateProfileString("Debug", "Start", "", szBuf, 15, path) != 0)
			{
				int nStart = atoi(szBuf);
				if( nStart < 1 )
					nType = 1;	// ʹ�ö��ζ���ʽ���ͳ����ʽ��6200 15.07.22
				else
					nType = 11;	// ʹ�ö��ζ���ʽ��̬���ͳ����ʽ��6200 15.07.22
			}
			else
				nType	= 1;
		}
	}
	else
		nType = 12;	// ʹ�ö��ζ���ʽ��̬���ͳ����ʽ��6900 15.07.22

	if( nType < 1 )
	{
		// ԭ����6200
		VARIANT varPos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
		CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = varPos.lVal +32768;
	}
	else
	{
		// 6900����6200
		double dLength = 3.04;
		if( GetPrivateProfileString("Coadjust", "Length", "", szBuf, 15, path) != 0 )
			dLength = atof( szBuf );
		// OL disp
		VARIANT varPos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
		CString strText = CSemCoreAssemble::Instance().m_ObjectiveLens.FormatText( varPos );
		CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = atof( strText );
		// Mag min = ��߳�����ʾֵD(uint:mm) * �Ŵ�����ʾֵMag / 3
		CString strUScale = CSemVirtualMCICard::Instance().GetMobileUScaleText();
		CString strUnit = strUScale.Right(2);
		strText = strUScale.Left( strUScale.GetLength() -2 );
		double dUScale = atof( strText );
		if( strUnit == "cm" )
			dUScale *= 10;
		else if( strUnit == "mm" )
			dUScale = dUScale;
		else if( strUnit == "nm" )
			dUScale /= 1000000.0;
		else//if( strUnit == "��m" )
			dUScale /= 1000.0;
		varPos = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
		CControlLayer::Instance().m_Cali.dCaliCoY[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = dUScale * varPos.dblVal /dLength;
	}
	CControlLayer::Instance().m_Cali.dCaliCoWD[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = dWD;
	CControlLayer::Instance().m_Cali.bCaliCoSel[ CControlLayer::Instance().m_Cali.nCaliCoCounts ] = TRUE;
	CControlLayer::Instance().m_Cali.nCaliCoCounts++;
	UpdateCaliData();

	// 15.08.03 ��¼���ļ�
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strCoadjustFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d");
	strCoadjustFileName.Format( "%s\\Config\\CoadjustMag-%s.csv", appfilepath, strTime );
	CFile file;
	file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite );

	if( file.m_hFile != CFile::hFileNull )
	{
		strTime = timeCurrent.Format(IDF_MessageTimeFormat);
		CString csMessage;
		double dX = CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ];
		if( nType < 1 )
			dX = (CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ] -32768) / 32.0;
		csMessage.Format( "[%s], WD = %.1f, CoX = %.6f, CoY = %.6f\r\n", strTime,
							CControlLayer::Instance().m_Cali.dCaliCoWD[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ],
							dX,
							CControlLayer::Instance().m_Cali.dCaliCoY[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ]);
		file.SeekToEnd();
		file.Write( csMessage, csMessage.GetLength() );

		csMessage.Format( "WD = %.1f, CoX = %.6f, CoY = %.6f", 
						CControlLayer::Instance().m_Cali.dCaliCoWD[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ],
						dX,
						CControlLayer::Instance().m_Cali.dCaliCoY[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ]);
		theApp.DisplayMsg( csMessage );
		file.Close();
	}
}

void CDlgCaliCoadjust::AddRecord_HV( double dWD )
{
	CControlLayer::Instance().m_Cali.dCaliCoWD[ CControlLayer::Instance().m_Cali.nCaliCoCounts ]	= dWD;
	CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts ]		= CHVPowerDataManager::Instance().m_dHV;
	CControlLayer::Instance().m_Cali.dCaliCoY[ CControlLayer::Instance().m_Cali.nCaliCoCounts ]		= 1.0 *CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
	CControlLayer::Instance().m_Cali.bCaliCoSel[ CControlLayer::Instance().m_Cali.nCaliCoCounts ]	= TRUE;
	CControlLayer::Instance().m_Cali.nCaliCoCounts++;
	UpdateCaliData();

	// ��¼���ļ�
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strCoadjustFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d");
	strCoadjustFileName.Format( "%s\\Config\\CoadjustHV-%s.csv", appfilepath, strTime );
	CFile file;
	file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite );

	if( file.m_hFile != CFile::hFileNull )
	{
		strTime = timeCurrent.Format(IDF_MessageTimeFormat);
		CString csMessage;
		csMessage.Format( "[%s], WD = %.1f, CoX = %.1f, CoY = %.1f\r\n", strTime,
			CControlLayer::Instance().m_Cali.dCaliCoWD[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ],
			CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ],
			CControlLayer::Instance().m_Cali.dCaliCoY[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ]);
		file.SeekToEnd();
		file.Write( csMessage, csMessage.GetLength() );

		csMessage.Format( "WD = %.1f, CoX = %.1f, CoY = %.1f", 
			CControlLayer::Instance().m_Cali.dCaliCoWD[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ],
			CControlLayer::Instance().m_Cali.dCaliCoX[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ],
			CControlLayer::Instance().m_Cali.dCaliCoY[ CControlLayer::Instance().m_Cali.nCaliCoCounts-1 ]);
		theApp.DisplayMsg( csMessage );
		file.Close();
	}
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjustSetLength() 
{
	// TODO: Add your control notification handler code here
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	CString str;
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Length )->GetWindowText( str );
	WritePrivateProfileString("Coadjust", "Length", str, path);
	theApp.DisplayMsg("Press set length on Coadjust dialog");
}

void CDlgCaliCoadjust::OnCheckChanged(UINT nID)
{
	int nIndex = nID - IDC_CHECK_AT_Cali_Coadjust_1;
	int nCheck = ((CButton*)(GetDlgItem(nID)))->GetCheck();
	if( nCheck )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +nIndex )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +nIndex )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +nIndex )->EnableWindow( TRUE );
	}
	else
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_OL1 +nIndex )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_WD1 +nIndex )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Min1 +nIndex )->EnableWindow( FALSE );
	}

	CControlLayer::Instance().m_Cali.bCaliCoSel[nIndex] = (BOOL)nCheck;
	int nCounts = 0;
	for( int i=0; i<11; i++ )
	{
		if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] )
			nCounts++;
	}
	CControlLayer::Instance().m_Cali.nCaliCoCounts = nCounts;
	CString str;
	str.Format( "%d", nCounts );
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Counts )->SetWindowText( str );
}

void CDlgCaliCoadjust::OnShowGraph1() 
{
	// TODO: Add your control notification handler code here
	CDlgDiagLogDiffGraph dlg;
	dlg.m_nSemType = m_nType;
	dlg.m_nGraphType = 1;
	dlg.UpdateGraphData();
	dlg.DoModal();
}

void CDlgCaliCoadjust::OnShowGraph2() 
{
	// TODO: Add your control notification handler code here
	CDlgDiagLogDiffGraph dlg;
	dlg.m_nSemType = m_nType;
	dlg.m_nGraphType = 2;
	dlg.UpdateGraphData();
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// �Ŵ���У׼����

void CDlgCaliCoadjust::OnCHECKDebugModeCoadjust() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem( IDC_CHECK_DebugMode_Coadjust )))->GetCheck();
	if( nCheck )
	{	
		// ����ʹ��Write��ť
		GetDlgItem( IDC_BUTTON_WriteRateX )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_BUTTON_WriteRateY )->ShowWindow( SW_SHOW );
	}
	else
	{	
		GetDlgItem( IDC_BUTTON_WriteRateX )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_BUTTON_WriteRateY )->ShowWindow( SW_HIDE );
	}

	// 15.11 У׼crossoverʱʹ��Ļ������ʾ�ο��㣬�ұ�׼�����Բο���Ϊ���ĵ�������
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
		pView->m_bDebugCoadjust	= (BOOL)nCheck;

	CControlLayer::Instance().CHECKDebugModeCoadjust(nCheck);
}

void CDlgCaliCoadjust::OnApproxMagChanged( UINT nID )
{
	int nIndex = nID -IDC_RADIO_Approx_Mag_10;
	switch( nIndex )
	{
	case	0:
		CControlLayer::Instance().SCN_AmpMain();
		m_nApproxMag = 0;
		break;
	case	1:
		CControlLayer::Instance().SCN_AmpLow1();
		m_nApproxMag = 1;
		break;
	case	2:
		CControlLayer::Instance().SCN_AmpLow2();
		m_nApproxMag = 2;
		break;
	case	3:
		CControlLayer::Instance().SCN_AmpLow3();
		m_nApproxMag = 3;
		break;
	case	4:
		CControlLayer::Instance().SCN_AmpLow4();
		m_nApproxMag = 4;
		break;
	}
}


void CDlgCaliCoadjust::OnClearAllChannels() 
{
	theApp.DisplayMsg("Press clear all channels of SCN board");
	CControlLayer::Instance().ClearAllChannels();

	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_10 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_100_1 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_100_2 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_1000 )))->SetCheck(0);
	((CButton*)(GetDlgItem( IDC_RADIO_Approx_Mag_10000 )))->SetCheck(0);
	m_nApproxMag = -1;
}

void CDlgCaliCoadjust::OnWriteRateX() 
{
	theApp.DisplayMsg("Press write FineMagX");
	CString str = "";
	GetDlgItem( IDC_EDIT_Det45 )->GetWindowText( str );
	if( str.IsEmpty() )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "NO data to write!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	if( m_nApproxMag < 0 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "NO selected stage!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}

	double dMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	// 2017.10.24
	// ���е�������Ϊ�ɱ�����洢�������ļ���
	double dRateMag[5];
	CControlLayer::Instance().GetMagnifierParam(dRateMag);

/*	if( (dMag < 249.5 && m_nApproxMag != 0)
		|| (dMag > 249.5 && dMag < 2495 && m_nApproxMag != 1)
		|| (dMag > 2495 && dMag < 24950 && m_nApproxMag != 2)
		|| (dMag > 24950 && dMag < 59950 && m_nApproxMag != 3)
		|| (dMag > 59950 && m_nApproxMag != 4) )
*/	if( (dMag < dRateMag[1] -0.5 && m_nApproxMag != 0)
		|| (dMag > dRateMag[1] -0.5	&& dMag < dRateMag[2] -5 && m_nApproxMag != 1)
		|| (dMag > dRateMag[2] -5	&& dMag < dRateMag[3] -50 && m_nApproxMag != 2)
		|| (dMag > dRateMag[3] -50	&& dMag < dRateMag[4] -50 && m_nApproxMag != 3)
		|| (dMag > dRateMag[4] -50	&& m_nApproxMag != 4) )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "stage NOT MATCH mag!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	
	int nPos;
	char *bufNum = str.GetBuffer(4);
	sscanf(bufNum, "%x", &nPos);
	double dPos = 1.0*nPos;
	// 2017.10.24
	// ���е�������Ϊ�ɱ�����洢�������ļ���
/*	if( dMag < 249.5 )
		dPos = dMag / 15.0 * dPos;
	else if( dMag > 249.5 && dMag < 2495 )
		dPos = dMag / 250 * dPos;
	else if( dMag > 2495 && dMag < 24975 )
		dPos = dMag / 2500 * dPos;
	else if( dMag > 24975 && dMag < 59950 )
		dPos = dMag / 25000 *dPos;
	else
		dPos = dMag / 60000 *dPos;
*/	if( dMag < dRateMag[1] -0.5 )
		dPos = dMag / dRateMag[0] * dPos;
	else if( dMag > dRateMag[1] -0.5 && dMag < dRateMag[2] -5 )
		dPos = dMag / dRateMag[1] * dPos;
	else if( dMag > dRateMag[2] -5 && dMag < dRateMag[3] -25 )
		dPos = dMag / dRateMag[2] * dPos;
	else if( dMag > dRateMag[3] -25 && dMag < dRateMag[4] -50 )
		dPos = dMag / dRateMag[3] *dPos;
	else
		dPos = dMag / dRateMag[4] *dPos;
	CCFGFileManager::Instance().WriteINI( INI_Ratio, INIIndex_MagK, &dPos, m_nApproxMag, 1);

	// 15.08.03 ��¼���ļ�
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strCoadjustFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d");
	strCoadjustFileName.Format( "%s\\Config\\CoadjustMag-%s.csv", appfilepath, strTime );
	CFile file;
	file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite );

	if( file.m_hFile != CFile::hFileNull )
	{
		strTime = timeCurrent.Format(IDF_MessageTimeFormat);
		CString csMessage;
		csMessage.Format( "[%s], ApproxMag = %d, Mag = %.2f, MagX = 0x%s\r\n", strTime, m_nApproxMag, dMag, str );
		file.SeekToEnd();
		file.Write( csMessage, csMessage.GetLength() );
	}
	file.Close();
}

void CDlgCaliCoadjust::OnWriteRateY() 
{
	theApp.DisplayMsg("Press write FineMagY");
	CString str = "";
	GetDlgItem( IDC_EDIT_Det45Y )->GetWindowText( str );
	if( str.IsEmpty() )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "NO data to write!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	if( m_nApproxMag < 0 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "NOT select stage!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	
	double dMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	// 2017.10.24
	// ���е�������Ϊ�ɱ�����洢�������ļ���
	double dRateMag[5];
	CControlLayer::Instance().GetMagnifierParam(dRateMag);

/*	if( (dMag < 249.5 && m_nApproxMag != 0)
		|| (dMag > 249.5 && dMag < 2495 && m_nApproxMag != 1)
		|| (dMag > 2495 && dMag < 24950 && m_nApproxMag != 2)
		|| (dMag > 24950 && dMag < 59950 && m_nApproxMag != 3)
		|| (dMag > 59950 && m_nApproxMag != 4) )
*/	if( (dMag < dRateMag[1] -0.5 && m_nApproxMag != 0)
		|| (dMag > dRateMag[1] -0.5	&& dMag < dRateMag[2] -5 && m_nApproxMag != 1)
		|| (dMag > dRateMag[2] -5	&& dMag < dRateMag[3] -50 && m_nApproxMag != 2)
		|| (dMag > dRateMag[3] -50	&& dMag < dRateMag[4] -50 && m_nApproxMag != 3)
		|| (dMag > dRateMag[4] -50	&& m_nApproxMag != 4) )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "stage NOT MATCH mag!";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
		return;
	}
	
	int nPos;
	char *bufNum = str.GetBuffer(4);
	sscanf(bufNum, "%x", &nPos);
	double dPos = 1.0*nPos;
	// 2017.10.24
	// ���е�������Ϊ�ɱ�����洢�������ļ���
/*	if( dMag < 249.5 )
		dPos = dMag / 15.0 * dPos;
	else if( dMag > 249.5 && dMag < 2495 )
		dPos = dMag / 250 * dPos;
	else if( dMag > 2495 && dMag < 24975 )
		dPos = dMag / 2500 * dPos;
	else if( dMag > 24975 && dMag < 59950 )
		dPos = dMag / 25000 *dPos;
	else
		dPos = dMag / 60000 *dPos;
*/	if( dMag < dRateMag[1] -0.5 )
		dPos = dMag / dRateMag[0] * dPos;
	else if( dMag > dRateMag[1] -0.5 && dMag < dRateMag[2] -5 )
		dPos = dMag / dRateMag[1] * dPos;
	else if( dMag > dRateMag[2] -5 && dMag < dRateMag[3] -25 )
		dPos = dMag / dRateMag[2] * dPos;
	else if( dMag > dRateMag[3] -25 && dMag < dRateMag[4] -50 )
		dPos = dMag / dRateMag[3] *dPos;
	else
		dPos = dMag / dRateMag[4] *dPos;
	CCFGFileManager::Instance().WriteINI( INI_Ratio, INIIndex_MagK, &dPos, m_nApproxMag, 2 );

	// 15.08.03 ��¼���ļ�
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strCoadjustFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d");
	strCoadjustFileName.Format( "%s\\Config\\CoadjustMag-%s.csv", appfilepath, strTime );
	CFile file;
	file.Open( strCoadjustFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite );

	if( file.m_hFile != CFile::hFileNull )
	{
		strTime = timeCurrent.Format(IDF_MessageTimeFormat);
		CString csMessage;
		csMessage.Format( "[%s], ApproxMag = %d, Mag = %.2f, MagY = 0x%s\r\n", strTime, m_nApproxMag, dMag, str );
		file.SeekToEnd();
		file.Write( csMessage, csMessage.GetLength() );
	}
	file.Close();
}

BOOL CDlgCaliCoadjust::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_Det45:
		case	IDC_EDIT_Det45Y:
		case	IDC_EDIT_XxoverDAC:
		case	IDC_EDIT_YxoverDAC:
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
		case	IDC_EDIT_Det45:
			m_scrollDet45.SetScrollPos( nPos );
			MCI_Det45SB_Change( nPos );
			break;
		case	IDC_EDIT_Det45Y:
			m_scrollDet45Y.SetScrollPos( nPos );
			MCI_Det45SBY_Change( nPos );
			break;
		case	IDC_EDIT_XxoverDAC:
			m_scrollXxoverDAC.SetScrollPos( nPos -2048 );
			SCN_XxoverDACSB_Change( nPos -2048 );
			break;
		case	IDC_EDIT_YxoverDAC:
			m_scrollYxoverDAC.SetScrollPos( nPos -2048 );
			SCN_YxoverDACSB_Change( nPos -2048 );
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgCaliCoadjust::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int nOldPos = pScrollBar->GetScrollPos();
	int nNewPos = pScrollBar->GetScrollPos();
	SCROLLINFO  scrollInfo;
	pScrollBar->GetScrollInfo( &scrollInfo, SIF_ALL );
	switch (nSBCode) 
	{
	case SB_LINELEFT:
		nNewPos = nOldPos-1;								//������Ϊ1
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
		nNewPos = nOldPos-10;//scrollInfo.nPage;			//ҳ����Ϊ10
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

	int id = pScrollBar->GetDlgCtrlID();					//��ȡ������ID
	switch( id )
	{
	case IDC_SCROLLBAR_Det45:
		MCI_Det45SB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Det45Y:
		MCI_Det45SBY_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_XxoverDACSB:
		SCN_XxoverDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_YxoverDACSB:
		SCN_YxoverDACSB_Change( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgCaliCoadjust::MCI_Det45SB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	CString str = "";
	int nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );
	CControlLayer::Instance().MCI_Det45SB_Change(nPos);
}

void CDlgCaliCoadjust::MCI_Det45SBY_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	CString str = "";
	int nValue = nPos;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_Det45Y )->SetWindowText( str );

	CControlLayer::Instance().MCI_Det45SBY_Change(nPos);
}

void CDlgCaliCoadjust::MCI_SyncDet45( int nPosX, int nPosY )
{
	CString str;
	str.Format( "%X", nPosX );
	GetDlgItem( IDC_EDIT_Det45 )->SetWindowText( str );
	m_scrollDet45.SetScrollPos( nPosX );
	str.Format( "%X", nPosY );
	GetDlgItem( IDC_EDIT_Det45Y )->SetWindowText( str );
	m_scrollDet45Y.SetScrollPos( nPosY );
}

void CDlgCaliCoadjust::SCN_XxoverDACSB_Change( int nPos )
{
	int nMinpos, nMaxpos;
	m_scrollXxoverDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	int nValue = nPos + 2048;
	CString str = "";
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_XxoverDAC )->SetWindowText( str );

	CControlLayer::Instance().SCN_XxoverDACSB_Change(nPos);
}

void CDlgCaliCoadjust::SCN_YxoverDACSB_Change( int nPos )
{
	CString str = "";
	int nMinpos, nMaxpos;
	m_scrollYxoverDAC.GetScrollRange(&nMinpos, &nMaxpos); 
	if( nPos < nMinpos )
		nPos = nMinpos;
	if( nPos > nMaxpos )
		nPos = nMaxpos;
	int nValue = nPos + 2048;
	str.Format( "%X", nValue );
	GetDlgItem( IDC_EDIT_YxoverDAC )->SetWindowText( str );

	CControlLayer::Instance().SCN_YxoverDACSB_Change(nPos);
}

void CDlgCaliCoadjust::SCN_SyncXOverPos( int nPosX, int nPosY )
{
	m_scrollXxoverDAC.SetScrollPos( nPosX -2048 );
	m_scrollYxoverDAC.SetScrollPos( nPosY -2048 );

	CString str;
	str.Format( "%X", nPosX );
	GetDlgItem( IDC_EDIT_XxoverDAC )->SetWindowText( str );
	
	str.Format( "%X", nPosY );
	GetDlgItem( IDC_EDIT_YxoverDAC )->SetWindowText( str );

	CControlLayer::Instance().SCN_SyncXOverPos(nPosX, nPosY);
}

void CDlgCaliCoadjust::OnWriteRateX2() 
{
	// TODO: Add your control notification handler code here
	// 15.06.19 ʹ��ԭ�����õ�xover������������ͷŴ�������ƫ������
	int nPos = m_scrollXxoverDAC.GetScrollPos();
	CString str;
	str.Format( "%d", nPos );

	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	WritePrivateProfileString( "Calibrate_Offset", "LowAmpX", str, path );
}

void CDlgCaliCoadjust::OnWriteRateY2() 
{
	// TODO: Add your control notification handler code here
	// 15.06.19 ʹ��ԭ�����õ�xover������������ͷŴ�������ƫ������
	int nPos = m_scrollYxoverDAC.GetScrollPos();
	CString str;
	str.Format( "%d", nPos );

	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	WritePrivateProfileString( "Calibrate_Offset", "LowAmpY", str, path );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 2021.11: ��С�Ŵ�������ʱ����

void CDlgCaliCoadjust::InitMagMinLogFile()
{
	// ר�ż�¼�Ŵ����ڵͱ����Ĳ���
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d%H%M");
	CString strMagMinLogFileName;
	strMagMinLogFileName.Format( "%sLog\\MagMinLogFile%s.log", appfilepath, strTime );
	m_fMagMinLogFile.Open( strMagMinLogFileName, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite );
	if( m_fMagMinLogFile.m_hFile != CFile::hFileNull )
	{
		char szBuf[100];
		sprintf(szBuf, "Mag Min Log\r\n");
		m_fMagMinLogFile.Write( szBuf, strlen(szBuf) );
	}
}

void CDlgCaliCoadjust::MessageMagMin(LPCTSTR lpszMessage)
{
	if( m_fMagMinLogFile.m_hFile != CFile::hFileNull )
	{
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString strTime = timeCurrent.Format("%Y%m%d, %H:%M:%S");
		char szBuf[255];
		sprintf( szBuf, "[%s], %s\r\n", strTime, lpszMessage );
		m_fMagMinLogFile.Write( szBuf, strlen(szBuf) );
	}
}

void CDlgCaliCoadjust::OnDestroy()
{
	if( m_fMagMinLogFile.m_hFile != CFile::hFileNull )
		m_fMagMinLogFile.Close();
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

void CDlgCaliCoadjust::UpdateMagStage( int nStage )
{
	// ��ʱ������С�Ŵ�����λ����ʱ������ɨ����Ȧ
	if( nStage > 0 )
	{
		// ���������С�Ŵ�����λ����ֹͣ��ʱ
		if( m_nMag_MinStageTimer != 0 )
			KillMagMinTimer( TRUE );
		return;
	}
	// ���������С�Ŵ�����λ����ʼ��ʱ
	// 2022.11 �����Ż�����׼������30KV 15mm��ʱ����С�Ŵ���Ϊ20������ʱ100�����ڿ�ʼ��ʱ
	//					������ѹ�����������£�����׼�����ı�������
	// ����������С�Ŵ�����λ����һ��ʱ�䣬����ʾ�û�
	double dMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	VARIANT varMin, varMax;
	CSemCoreAssemble::Instance().m_Magnifier.GetRange( varMin, varMax );
	double dMagThresholds = sqrt( CHVPowerDataManager::Instance().m_dHV /30.0) * ( varMin.dblVal * 100 / 20 );
	if( m_nMag_MinStageTimer != 0 )
	{
		if( dMag < dMagThresholds )
		{
			// �Ѿ���ʼ��ʱ���ұ�����С����ֵ�����ж��Ƿ񳬹�ʱ��
			CompareMagTimeout();
		}
		else
		{
			// ������С�����������ѳ���ֵ����ֹͣ��ʱ
			KillMagMinTimer( TRUE );
		}
		return;
	}
	// û�п�ʼ��ʱ
	// ���ѼӸ�ѹ���ߵ���״̬�ٸ�ѹ���ұ���С����ֵ����������ʱ��
	if( dMag < dMagThresholds )
	{
		BOOL bNeedStartTimer = FALSE;
		if( theApp.m_blOverhaulState )	// �ٸ�ѹ״̬
			bNeedStartTimer = TRUE;
		else if( theApp.m_blUnlock_HV )	// SPR_ENΪTRUE������Ӹ�ѹ��
		{
			// ��ѹ��ʵ�����ֵ
			switch( CHVPowerDataManager::Instance().m_dwHVtype )
			{
			case	0:
				if( CHVPowerDataManager::Instance().m_CPS3603.dVoutA > 100 )
					bNeedStartTimer = TRUE;
				break;
			case	1:
				if( CHVPowerDataManager::Instance().m_Spellmans.pM3.dVE > 100 )
					bNeedStartTimer = TRUE;
				break;
			case	2:
				if( theApp.m_monWSM30KV.monFB.fACLv > 100 )
					bNeedStartTimer = TRUE;
				break;
			case	3:
				if( theApp.m_monWSM30KV2.monFB.fACLv > 100 )
					bNeedStartTimer = TRUE;
				break;
			}
		}
		if( bNeedStartTimer )
		{
			m_dwMagTickCountStart = GetTickCount();
			m_nMag_MinStageTimer = SetTimer( nMag_MinStageTimerID, nMag_MinStageTimerElapse, NULL );
			m_nMagMinStageTimeoutCounts = 1;
			char szBuf[255];
			sprintf( szBuf, "Into the lowest MAG range (%.1f X < %.1f), start timer.", dMag, dMagThresholds );
			theApp.DisplayMsg(szBuf);
			MessageMagMin(szBuf);
		}
	}
}


void CDlgCaliCoadjust::KillMagMinTimer( BOOL bHVon )
{
	KillTimer( nMag_MinStageTimerID );
	m_nMag_MinStageTimer = 0;
	m_nMagMinStageTimeoutCounts = 0;
	double dMag = CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
	char szBuf[255];
	if( bHVon )
		sprintf( szBuf, "Into safe MAG (%.1f X), kill timer.", dMag );
	else
		sprintf( szBuf, "MAG stays in the lowest range (%.1f X), but HV is NOT on, kill timer.", dMag );
	theApp.DisplayMsg(szBuf);
	MessageMagMin(szBuf);
}

void CDlgCaliCoadjust::CompareMagTimeout()
{
	// ����ѹ���ˣ���ֹͣ��ʱ��
	if( !(theApp.m_blOverhaulState)	)	// �Ǽٸ�ѹ
	{
		BOOL bNeedKillTimer = FALSE;
		// δ��V1
		if( !(theApp.m_blUnlock_HV)	)	// SPR_ENΪFALSE������Ϊδ��V1��
			bNeedKillTimer = TRUE;
		else if ( theApp.m_blUnlock_HV )// SPR_ENΪTRUE������Ӹ�ѹ��
		{
			// �����俪��V1����ѹ��ʵ�������С��0.1KV��
			switch( CHVPowerDataManager::Instance().m_dwHVtype )
			{
			case	0:
				if( CHVPowerDataManager::Instance().m_CPS3603.dVoutA < 100 )
					bNeedKillTimer = TRUE;
				break;
			case	1:
				if( CHVPowerDataManager::Instance().m_Spellmans.pM3.dVE < 100 )
					bNeedKillTimer = TRUE;
				break;
			case	2:
				if( theApp.m_monWSM30KV.monFB.fACLv < 100 )
					bNeedKillTimer = TRUE;
				break;
			case	3:
				if( theApp.m_monWSM30KV2.monFB.fACLv < 100 )
					bNeedKillTimer = TRUE;
				break;
			}
		}
		if( bNeedKillTimer )
		{
			KillMagMinTimer( FALSE );
			return;
		}
	}
	// ����n����ʱ��λ��1����ʱ��λ10���ӣ�������ʾ�û�
	if( GetTickCount() -m_dwMagTickCountStart < m_nMagMinStageTimeoutCounts *1000 *600 )
		return;
	// ��ʾ�û�
	char szBuf[255];
	sprintf( szBuf, "%d time(s) warning: The MAG stays in the lowest range for %d minutes!", m_nMagMinStageTimeoutCounts, m_nMagMinStageTimeoutCounts*10 );
	if( m_nMagMinStageTimeoutCounts > 2 )
		sprintf( szBuf, "%d time(s) warning: The MAG stays in the lowest range for %d minutes!  The software will increase the MAG now.",
		m_nMagMinStageTimeoutCounts, m_nMagMinStageTimeoutCounts*10 );
	theApp.DisplayMsg(szBuf);
	__int64 timestamp = GetCurrentTimeStampMS();
	CAlarmManager::Instance().AddAlarm(AlarmType::Alarm_LowMag,(long long)timestamp,string(szBuf));
	MessageMagMin(szBuf);
	// �ﵽ3����ʱ��λ����ǿ������
	if( m_nMagMinStageTimeoutCounts > 2 )
	{
		// ���Ŵ���������ȫֵ
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
		SerialNumber.lVal = -1;
		ClassID.lVal = SemControlPanelClass_Magnifier;
		Flag.lVal = 1;
		Xpos.dblVal = 500;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		Sleep( 100 );
	}
	m_nMagMinStageTimeoutCounts++;
	CDlgPrompt dlg(theApp.GetMainWnd());
	dlg.m_byteType = 0x01;
	dlg.m_strMsg = "\r\n";
	dlg.m_strMsg += "\r\n";
	dlg.m_strMsg += szBuf;
	dlg.m_strMsg += "\r\n";
	dlg.m_strMsg += "\r\n";
	dlg.DoModal();
}

void CDlgCaliCoadjust::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == nMag_MinStageTimerID )
		CompareMagTimeout();
	CDialog::OnTimer(nIDEvent);
}


void CDlgCaliCoadjust::OnRadioAtCaliCoadjustModeOL()
{
	// TODO: Add your message handler code here and/or call default
	GetDlgItem(IDC_STATIC_AT_Cali_Coadjust_Second)->SetWindowText( "OL disp" );
	GetDlgItem(IDC_STATIC_AT_Cali_Coadjust_Third)->SetWindowText( "Mag Min" );
	m_nCoadjustMode = 0;
	GetDlgItem(IDC_COMBO_AT_Cali_Coadjust_WD)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_AT_Cali_Graph1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_AT_Cali_Graph2)->ShowWindow(SW_SHOW);
}

void CDlgCaliCoadjust::OnRadioAtCaliCoadjustModeHV()
{
	// TODO: Add your message handler code here and/or call default
	GetDlgItem(IDC_STATIC_AT_Cali_Coadjust_Second)->SetWindowText( "HV" );
	GetDlgItem(IDC_STATIC_AT_Cali_Coadjust_Third)->SetWindowText( "lCtlIndex" );
	m_nCoadjustMode = 1;
	GetDlgItem(IDC_COMBO_AT_Cali_Coadjust_WD)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_AT_Cali_Graph1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_AT_Cali_Graph2)->ShowWindow(SW_HIDE);
}
