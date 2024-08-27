// insample0.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "insample0.h"
#include "insample0Dlg.h"

#include "Calc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE ghTimeDelay = CreateEvent( NULL, FALSE, TRUE, "TimeDelay" );

DocType doctypes[SEM_IMAGE_FORMATS] =
{
	{ -1, TRUE, FALSE, _T("Supported files"), _T("*.sem;*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;*.ppm;*.ras;*.mng;*.jng;*.ska;*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") },
#if CXIMAGE_SUPPORT_BMP
	{ CXIMAGE_FORMAT_BMP, TRUE, TRUE, _T("BMP files"), _T("*.bmp") },
#endif
#if CXIMAGE_SUPPORT_GIF
	{ CXIMAGE_FORMAT_GIF, TRUE, TRUE, _T("GIF files"), _T("*.gif") },
#endif
#if CXIMAGE_SUPPORT_JPG
	{ CXIMAGE_FORMAT_JPG, TRUE, TRUE, _T("JPG files"), _T("*.jpg;*.jpeg") },
#endif
#if CXIMAGE_SUPPORT_PNG
	{ CXIMAGE_FORMAT_PNG, TRUE, TRUE, _T("PNG files"), _T("*.png") },
#endif
#if CXIMAGE_SUPPORT_MNG
	{ CXIMAGE_FORMAT_MNG, TRUE, TRUE, _T("MNG files"), _T("*.mng;*.jng") },
#endif
#if CXIMAGE_SUPPORT_ICO
	{ CXIMAGE_FORMAT_ICO, TRUE, TRUE, _T("ICO CUR files"), _T("*.ico;*.cur") },
#endif
#if CXIMAGE_SUPPORT_TIF
	{ CXIMAGE_FORMAT_TIF, TRUE, TRUE, _T("TIF files"), _T("*.tif;*.tiff") },
#endif
#if CXIMAGE_SUPPORT_TGA
	{ CXIMAGE_FORMAT_TGA, TRUE, TRUE, _T("TGA files"), _T("*.tga") },
#endif
#if CXIMAGE_SUPPORT_PCX
	{ CXIMAGE_FORMAT_PCX, TRUE, TRUE, _T("PCX files"), _T("*.pcx") },
#endif
#if CXIMAGE_SUPPORT_WBMP
	{ CXIMAGE_FORMAT_WBMP, TRUE, FALSE, _T("WBMP files"), _T("*.wbmp") },
#endif
#if CXIMAGE_SUPPORT_WMF
	{ CXIMAGE_FORMAT_WMF, TRUE, FALSE, _T("WMF EMF files"), _T("*.wmf;*.emf") },
#endif
#if CXIMAGE_SUPPORT_JBG
	{ CXIMAGE_FORMAT_JBG, TRUE, TRUE, _T("JBG files"), _T("*.jbg") },
#endif
#if CXIMAGE_SUPPORT_JP2
	{ CXIMAGE_FORMAT_JP2, TRUE, TRUE, _T("JP2 files"), _T("*.j2k;*.jp2") },
#endif
#if CXIMAGE_SUPPORT_JPC
	{ CXIMAGE_FORMAT_JPC, TRUE, TRUE, _T("JPC files"), _T("*.j2c;*.jpc") },
#endif
#if CXIMAGE_SUPPORT_PGX
	{ CXIMAGE_FORMAT_PGX, TRUE, TRUE, _T("PGX files"), _T("*.pgx") },
#endif
#if CXIMAGE_SUPPORT_RAS
	{ CXIMAGE_FORMAT_RAS, TRUE, TRUE, _T("RAS files"), _T("*.ras") },
#endif
#if CXIMAGE_SUPPORT_PNM
	{ CXIMAGE_FORMAT_PNM, TRUE, TRUE, _T("PNM files"), _T("*.pnm;*.pgm;*.ppm") },
#endif
#if CXIMAGE_SUPPORT_SKA
	{ CXIMAGE_FORMAT_SKA, TRUE, TRUE, _T("SKA files"), _T("*.ska") },
#endif
#if CXIMAGE_SUPPORT_RAW
	{ CXIMAGE_FORMAT_RAW, TRUE, FALSE, _T("RAW files"), _T("*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") },
#endif
#if SEMIMAGE_SUPPORT_SEM
	{ SEMIMAGE_FORMAT_SEM, TRUE, TRUE, "SEM Image files (.sem)", "*.sem" }
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CInsample0App

BEGIN_MESSAGE_MAP(CInsample0App, CWinApp)
	//{{AFX_MSG_MAP(CInsample0App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsample0App construction

CInsample0App::CInsample0App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bIsDI16AReady	= FALSE;
	m_bIsStop	= TRUE;

	int i;
	// Diag/Logging Graph
	for( i=0; i<GraphDataBuffGroups; i++ )
		m_pGraphDataBuff[i] = NULL;

	// AutoTest
	m_AT.pAutoTestPower	= NULL;
	m_AT.pAutoTestPowerExp= NULL;
	m_AT.pAutoTestPowerErr= NULL;
	m_AT.pPower_OutofRange= NULL;
	m_AT.pAutoTestVAC		= NULL;
	for( i=0; i<4; i++ )
	{
		m_AT.pAutoTestLNS[i] = NULL;
		m_AT.pAutoTestSCN[i] = NULL;
		m_AT.pAutoTestMCI[i] = NULL;
		m_AT.pAutoTestSPR[i] = NULL;
	}

	// Calibration
	for( i=0; i<4; i++ )
	{
		m_Cali.m_pCaliLNS[i] = NULL;
		m_Cali.m_pCaliSCN[i] = NULL;
		m_Cali.m_pCaliMCI[i] = NULL;
		m_Cali.m_pCaliSPR[i] = NULL;
	}
	for( i=0; i<10; i++ )
	{
		m_Cali.m_dCaliCoC[i]	= 0.0;
		m_Cali.m_dCaliCoK[i]	= 0.0;
		m_Cali.m_bCaliCoSel[i]	= FALSE;
	}
	m_Cali.m_nCaliCoCounts	= 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CInsample0App object

CInsample0App theApp;

/////////////////////////////////////////////////////////////////////////////
// CInsample0App initialization

BOOL CInsample0App::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CInsample0Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CInsample0App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	ResetEvent( ghTimeDelay );
	if( ghTimeDelay != NULL )
		CloseHandle( ghTimeDelay );

	int i;
	for( i=0; i<GraphDataBuffGroups; i++ )
	{
		if ( m_pGraphDataBuff[i] != NULL )
		{
			delete [] m_pGraphDataBuff[i];
			m_pGraphDataBuff[i]	= NULL;
		}
	}

	if( m_AT.pAutoTestPower != NULL )
		delete [] m_AT.pAutoTestPower;
	if( m_AT.pAutoTestPowerExp != NULL )
		delete [] m_AT.pAutoTestPowerExp;
	if( m_AT.pAutoTestPowerErr != NULL )
		delete [] m_AT.pAutoTestPowerErr;
	if( m_AT.pPower_OutofRange != NULL )
		delete [] m_AT.pPower_OutofRange;
	if( m_AT.pAutoTestVAC != NULL )
		delete [] m_AT.pAutoTestVAC;
	for( i=0; i<4; i++ )
	{
		if( m_AT.pAutoTestLNS[i] != NULL )
			delete [] m_AT.pAutoTestLNS[i];
		if( m_AT.pAutoTestSCN[i] != NULL )
			delete [] m_AT.pAutoTestSCN[i];
		if( m_AT.pAutoTestMCI[i] != NULL )
			delete [] m_AT.pAutoTestMCI[i];
		if( m_AT.pAutoTestSPR[i] != NULL )
			delete [] m_AT.pAutoTestSPR[i];
	}
	for( i=0; i<4; i++ )
	{
		if( m_Cali.m_pCaliLNS[i] != NULL )
			delete [] m_Cali.m_pCaliLNS[i];
		if( m_Cali.m_pCaliSCN[i] != NULL )
			delete [] m_Cali.m_pCaliSCN[i];
		if( m_Cali.m_pCaliMCI[i] != NULL )
			delete [] m_Cali.m_pCaliMCI[i];
		if( m_Cali.m_pCaliSPR[i] != NULL )
			delete [] m_Cali.m_pCaliSPR[i];
	}
	m_232Device.Serial_Close();
/*	m_bRecvReady = FALSE;
	if( m_pRecv != NULL )
	{
		m_pRecv->KillThread();
		m_pRecv = NULL;
	}
	m_bSendReady = FALSE;
	if( m_pSend != NULL )
	{
		m_pSend->KillThread();
		m_pSend = NULL;
	}

	if( m_DL.adc.chn != NULL )
	{
		delete [] m_DL.adc.chn;
		m_DL.adc.chn = NULL;
	}
	if( m_pSPbackADCs != NULL )
	{
		delete [] m_pSPbackADCs;
		m_pSPbackADCs = NULL;
	}
	if( m_pSPbackOthers != NULL )
	{
		delete [] m_pSPbackOthers;
		m_pSPbackOthers = NULL;
	}
	if( m_pSPlist != NULL )
	{
		delete [] m_pSPlist;
		m_pSPlist = NULL;
	}
*/
	return CWinApp::ExitInstance();
}






//****************************************************************************************
BOOL CInsample0App::Serial_Write( void *pData, DWORD dwBufferSize, DWORD &dwBytesWritten )
{
	return m_232Device.Serial_Write( pData, dwBufferSize, dwBytesWritten );
}

int CInsample0App::Serial_Read( void *Buffer, DWORD &dwBytesReturned )
{
	return m_232Device.Serial_Read( Buffer, dwBytesReturned );
}
//****************************************************************************************





void CInsample0App::InitParams()
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'.');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, ".ini");

	InitChangingParams( path );
	InitSelectedADC( path );
	InitLoggingParams( path );

	InitAutoTestParams( path );

//	m_DL.other.nElapseChanging	= 2000;
//	m_DL.other.nElapseLogging	= 2000;
//	m_DL.other.bGaugeCompr		= FALSE;
}

void CInsample0App::InitChangingParams( char* path )
{
	char		stabuff[20];
	char		staitembuff[20];
	long*		plongCheck	= new long[12];
	long*		plongMin	= new long[12];
	long*		plongMax	= new long[12];
	long*		plongStep	= new long[12];
	long*		plongIntv	= new long[12];
	int			nTotal	= 11;
	int			i, j=0;
	CString		str;

	sprintf(stabuff, "ChangingParam");
	/////////////////////////////////////////////////////////////////
	// SCN & LNS
	nTotal = 11;
	sprintf(staitembuff, "SCN-Check");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongCheck);
	sprintf(staitembuff, "SCN-Min");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMin);
	sprintf(staitembuff, "SCN-Max");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMax);
	sprintf(staitembuff, "SCN-Step");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongStep);
	sprintf(staitembuff, "SCN-Intv");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongIntv);
	for( i=0; i<nTotal; i++,j++ )
	{
		m_DL.diag[ j ].nCheck	= plongCheck[i];
		m_DL.diag[ j ].nMin		= plongMin[i];
		m_DL.diag[ j ].nMax		= plongMax[i];
		m_DL.diag[ j ].nStep	= plongStep[i];
		m_DL.diag[ j ].nIntv	= plongIntv[i];
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC
	nTotal = 12;
	sprintf(staitembuff, "VAC-Check");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongCheck);
	sprintf(staitembuff, "VAC-Min");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMin);
	sprintf(staitembuff, "VAC-Max");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMax);
	sprintf(staitembuff, "VAC-Step");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongStep);
	sprintf(staitembuff, "VAC-Intv");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongIntv);
	for( i=0; i<nTotal; i++,j++ )
	{
		m_DL.diag[ j ].nCheck	= plongCheck[i];
		m_DL.diag[ j ].nMin		= plongMin[i];
		m_DL.diag[ j ].nMax		= plongMax[i];
		m_DL.diag[ j ].nStep	= plongStep[i];
		m_DL.diag[ j ].nIntv	= plongIntv[i];
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR
	nTotal = 4;
	sprintf(staitembuff, "SPR-Check");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongCheck);
	sprintf(staitembuff, "SPR-Min");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMin);
	sprintf(staitembuff, "SPR-Max");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMax);
	sprintf(staitembuff, "SPR-Step");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongStep);
	sprintf(staitembuff, "SPR-Intv");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongIntv);
	for( i=0; i<nTotal; i++,j++ )
	{
		m_DL.diag[ j ].nCheck	= plongCheck[i];
		m_DL.diag[ j ].nMin		= plongMin[i];
		m_DL.diag[ j ].nMax		= plongMax[i];
		m_DL.diag[ j ].nStep	= plongStep[i];
		m_DL.diag[ j ].nIntv	= plongIntv[i];
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// MCI
	nTotal = 8;
	sprintf(staitembuff, "MCI-Check");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongCheck);
	sprintf(staitembuff, "MCI-Min");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMin);
	sprintf(staitembuff, "MCI-Max");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMax);
	sprintf(staitembuff, "MCI-Step");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongStep);
	sprintf(staitembuff, "MCI-Intv");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongIntv);
	for( i=0; i<nTotal; i++,j++ )
	{
		m_DL.diag[ j ].nCheck	= plongCheck[i];
		m_DL.diag[ j ].nMin		= plongMin[i];
		m_DL.diag[ j ].nMax		= plongMax[i];
		m_DL.diag[ j ].nStep	= plongStep[i];
		m_DL.diag[ j ].nIntv	= plongIntv[i];
	}
	/////////////////////////////////////////////////////////////////

	char szBuf[20];
	sprintf(staitembuff, "TimeElapse");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		str.Format( "%s", szBuf );
		m_DL.other.nElapseChanging = atoi( str );
	}
	
	delete [] plongCheck;
	delete [] plongMin;
	delete [] plongMax;
	delete [] plongStep;
	delete [] plongIntv;
}

void CInsample0App::InitLoggingParams( char* path )
{
	char		stabuff[20];
	char		staitembuff[20];
	char		szBuf[20];
	long*		pLog	= new long[16];
	CString*	pStr	= new CString[16];
	int			nTotal	= 16;
	int			i, j=0;

	m_DL.log2.nCounts = 0;
	ZeroMemory( &(m_DL.log2.logParams), sizeof(logParam)*6*16 );
	sprintf(stabuff, "LoggingParam");
	/////////////////////////////////////////////////////////////////
	sprintf(staitembuff, "GaugeCompr");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		m_DL.other.bGaugeCompr = atoi( str );
		if( m_DL.other.bGaugeCompr )
		{
			m_DL.log2.logParams[m_DL.log2.nCounts].nType	= 2;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum		= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID		= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID		= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn		= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType	= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff	= 0;
			m_DL.log2.nCounts++;
		}
	}

	/////////////////////////////////////////////////////////////////
	// SCN
	nTotal = 16;
	sprintf(staitembuff, "SCN");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		m_DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			m_DL.log2.logParams[m_DL.log2.nCounts].nType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum		= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID		= USB_MID_SCN;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID		= USB_CID_SCN_ADC_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn		= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff	= m_DL.adc.chn[j*16 +i].bDiff;
			m_DL.log2.nCounts++;
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// LNS
	j++;
	nTotal = 8;
	sprintf(staitembuff, "LNS");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		m_DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			m_DL.log2.logParams[m_DL.log2.nCounts].nType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum		= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID		= USB_MID_LNS;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID		= USB_CID_LNS_ADC_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn		= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType	= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff	= m_DL.adc.chn[j*16 +i].bDiff;
			m_DL.log2.nCounts++;
		}
	}

	sprintf(staitembuff, "LNS-Param");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, 9, pLog);
	for( i=0; i<9; i++ )
		m_DL.log.bLogParam[i] = pLog[i];
	for( i=0; i<4; i++ )
		m_DL.log.nCheckComp[i] = 0;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC0
	j++;
	nTotal = 16;
	sprintf(staitembuff, "VAC0");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		m_DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			m_DL.log2.logParams[m_DL.log2.nCounts].nType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum		= 2;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID		= USB_MID_VAC;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID		= USB_CID_VAC_ADC0_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn		= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff	= m_DL.adc.chn[j*16 +i].bDiff;
			m_DL.log2.nCounts++;
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC1
	j++;
	sprintf(staitembuff, "VAC1");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		m_DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			m_DL.log2.logParams[m_DL.log2.nCounts].nType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum		= 3;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID		= USB_MID_VAC;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID		= USB_CID_VAC_ADC1_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn		= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff	= m_DL.adc.chn[j*16 +i].bDiff;
			m_DL.log2.nCounts++;
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR0
	j++;
	sprintf(staitembuff, "SPR0");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		m_DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			m_DL.log2.logParams[m_DL.log2.nCounts].nType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum		= 4;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID		= USB_MID_SPR;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID		= USB_CID_SPR_ADC0_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn		= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff	= m_DL.adc.chn[j*16 +i].bDiff;
			m_DL.log2.nCounts++;
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR1
	j++;
	sprintf(staitembuff, "SPR1");
	ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		m_DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			m_DL.log2.logParams[m_DL.log2.nCounts].nType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum		= 5;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID		= USB_MID_SPR;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID		= USB_CID_SPR_ADC1_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn		= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff	= m_DL.adc.chn[j*16 +i].bDiff;
			m_DL.log2.nCounts++;
		}
	}
	/////////////////////////////////////////////////////////////////

	sprintf(staitembuff, "AverageCounts");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		m_DL.adc.nAveCounts = atoi( str );
	}
	sprintf(staitembuff, "TimeElapse");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		m_DL.other.nElapseLogging = atoi( str );
	}
	sprintf(staitembuff, "Graph");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		m_DL.other.bGraph = atoi( str );
	}
	sprintf(staitembuff, "Debug");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		m_DL.other.bDebug = atoi( str );
	}

	/////////////////////////////////////////////////////////////////
	// 读公式
	j = 0;
	sprintf(stabuff, "CalculateParam");
	// SCN
	nTotal = 16;
	sprintf(staitembuff, "SCN");
	ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		m_DL.log.strCalc[j*16 +i] = pStr[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// LNS
	j++;
	nTotal = 8;
	sprintf(staitembuff, "LNS");
	ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		m_DL.log.strCalc[j*16 +i] = pStr[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC0
	j++;
	nTotal = 16;
	sprintf(staitembuff, "VAC0");
	ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		m_DL.log.strCalc[j*16 +i] = pStr[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC1
	j++;
	sprintf(staitembuff, "VAC1");
	ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		m_DL.log.strCalc[j*16 +i] = pStr[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR0
	j++;
	sprintf(staitembuff, "SPR0");
	ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		m_DL.log.strCalc[j*16 +i] = pStr[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR1
	j++;
	sprintf(staitembuff, "SPR1");
	ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		m_DL.log.strCalc[j*16 +i] = pStr[i];
	/////////////////////////////////////////////////////////////////

	delete [] pLog;
	delete [] pStr;
}

void CInsample0App::InitSelectedADC( char* path )
{
	m_nADCnum = m_nADCchn = 0;
// 	m_dADCcurrentMeas = m_dADCvoltsPerAmp = m_dADCampsPerVolt = m_dADCvref = m_dADCCOMoffset = 0;
// 	m_bADCdiff = m_bADCtype = FALSE;

// 	m_nADCmid	= 0x0;		// Module Selection ID
// 	m_nADCcid	= 0x0;		// Circuit Selection ID

	/////////////////////////////////////////////////////////////////
	char staitemname1[32], staitemname2[32], staitemname3[32];
	int	nTotal, nNum, nMID, nCID;

	nTotal	= 16;
	nNum	= 0;
	nMID	= 1;
	nCID	= USB_CID_SCN_ADC_Read;
	sprintf(staitemname1, "SCN");
	sprintf(staitemname2, "SCN-1");
	sprintf(staitemname3, "SCN-2");
	InitADCcfg( path, staitemname1, staitemname2, staitemname3, nTotal, nNum, nMID, nCID );

	nTotal	= 8;
	nNum++;
	nMID++;
	nCID	= USB_CID_LNS_ADC_Read;
	sprintf(staitemname1, "LNS");
	sprintf(staitemname2, "LNS-1");
	sprintf(staitemname3, "LNS-2");
	InitADCcfg( path, staitemname1, staitemname2, staitemname3, nTotal, nNum, nMID, nCID );

	nTotal	= 16;
	nNum++;
	nMID++;
	nCID	= USB_CID_VAC_ADC0_Read;
	sprintf(staitemname1, "VAC0");
	sprintf(staitemname2, "VAC0-1");
	sprintf(staitemname3, "VAC0-2");
	InitADCcfg( path, staitemname1, staitemname2, staitemname3, nTotal, nNum, nMID, nCID );

	nNum++;
	nCID	= USB_CID_VAC_ADC1_Read;
	sprintf(staitemname1, "VAC1");
	sprintf(staitemname2, "VAC1-1");
	sprintf(staitemname3, "VAC1-2");
	InitADCcfg( path, staitemname1, staitemname2, staitemname3, nTotal, nNum, nMID, nCID );

	nNum++;
	nMID++;
	nCID	= USB_CID_SPR_ADC0_Read;
	sprintf(staitemname1, "SPR0");
	sprintf(staitemname2, "SPR0-1");
	sprintf(staitemname3, "SPR0-2");
	InitADCcfg( path, staitemname1, staitemname2, staitemname3, nTotal, nNum, nMID, nCID );

	nNum++;
	nCID	= USB_CID_SPR_ADC1_Read;
	sprintf(staitemname1, "SPR1");
	sprintf(staitemname2, "SPR1-1");
	sprintf(staitemname3, "SPR1-2");
	InitADCcfg( path, staitemname1, staitemname2, staitemname3, nTotal, nNum, nMID, nCID );
}

void CInsample0App::InitADCcfg( char* path, char* Entry1, char* Entry2, char* Entry3, int nTotal, int nNum, int nMID, int nCID )
{
	char stabuff[32], staitembuff[32];
	double*		pdouble	= new double[16];
	CString*	pstr	= new CString[16];
	int i;
	/////////////////////////////////////////////////////////////////
	sprintf(stabuff, "ADCchannels");
	if( ReadInOneLine(path, stabuff, Entry1, STRSTYLE, 16, pstr) == 16 )
		for( i=0; i<16; i++ )
			m_DL.adc.chn[nNum*16+i].strLabel = pstr[i];
	// Set channels 0/1, 2/3, 4/5, 6/7 to diff (value of x,x,1 = 2), all others to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, Entry2, DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[nNum*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, Entry3, DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[nNum*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "%s-%d", Entry1, i);
		if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			m_DL.adc.chn[nNum*16+i].dMin = pdouble[0];
			m_DL.adc.chn[nNum*16+i].dMax = pdouble[1];
		}
	}

	int nIndex = 0;
	for( i=0; i<nTotal; i++ )
	{
		nIndex = nNum*16 +i;
		m_DL.adc.chn[nIndex].nNum	= nNum;
		m_DL.adc.chn[nIndex].nMID	= nMID;
		m_DL.adc.chn[nIndex].nCID	= nCID;
		m_DL.adc.chn[nIndex].nChn	= i;
		m_DL.adc.chn[nIndex].dCampsPerVolt = 1 / m_DL.adc.chn[nIndex].dFactor;

		if( nMID == 2 )
			m_DL.adc.chn[nIndex].bType	= FALSE;
		else
			m_DL.adc.chn[nIndex].bType	= TRUE;

		switch( nNum )
		{
		case	0:
			m_DL.adc.chn[nIndex].dVref		= 4;
			m_DL.adc.chn[nIndex].dCOMoffset	= 0;
			break;
		case	1:
			m_DL.adc.chn[nIndex].dVref		= 10;
			m_DL.adc.chn[nIndex].dCOMoffset	= 0;
			break;
		case	2:
			m_DL.adc.chn[nIndex].dVref		= 4;
			m_DL.adc.chn[nIndex].dCOMoffset	= 1.607;
			break;
		case	3:
			m_DL.adc.chn[nIndex].dVref		= 4;
			m_DL.adc.chn[nIndex].dCOMoffset	= 1.607;
			break;
		case	4:
			m_DL.adc.chn[nIndex].dVref		= 4.5;
			m_DL.adc.chn[nIndex].dCOMoffset	= 0;
			break;
		case	5:
			m_DL.adc.chn[nIndex].dVref		= 4.5;
			m_DL.adc.chn[nIndex].dCOMoffset	= 0;
			break;
		}
	}

	delete [] pdouble;
	delete [] pstr;
}
/*
	/////////////////////////////////////////////////////////////////
	// SCN ADC (only one ADC on SCN board):
	sprintf(stabuff, "ADCchannels");
	if( ReadInOneLine(path, stabuff, "SCN", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[nNum*16+i].strLabel = pstr[i];
	// Set channels 0/1, 2/3, 4/5, 6/7 to diff (value of x,x,1 = 2), all others to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "SCN-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[nNum*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "SCN-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[nNum*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "SCN-%d", i);
		if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			m_DL.adc.chn[nNum*16+i].dMin = pdouble[0];
			m_DL.adc.chn[nNum*16+i].dMax = pdouble[1];
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// LNS ADC (one LTC1859 8 channel ADC):
	sprintf(stabuff, "ADCchannels");
	if( ReadInOneLine(path, stabuff, "LNS", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[1*16+i].strLabel = pstr[i];
	// Set all 8 channels' input modes as Single-ended:
	nTotal = 8;
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "LNS-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[1*16+i].bDiff = (pdouble[i] == 2);
	// ADC channels 0-3 are assigned to current measurements. Set scale factors according to circuit:
	if( ReadInOneLine(path, stabuff, "LNS-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[1*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "LNS-%d", i);
		if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			m_DL.adc.chn[1*16+i].dMin = pdouble[0];
			m_DL.adc.chn[1*16+i].dMax = pdouble[1];
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC ADC#0:
	nTotal = 16;
	sprintf(stabuff, "ADCchannels");
	if( ReadInOneLine(path, stabuff, "VAC0", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[2*16+i].strLabel = pstr[i];
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "VAC0-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[2*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "VAC0-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[2*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "VAC0-%d", i);
		if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			m_DL.adc.chn[2*16+i].dMin = pdouble[0];
			m_DL.adc.chn[2*16+i].dMax = pdouble[1];
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC ADC#1:
	sprintf(stabuff, "ADCchannels");
	if( ReadInOneLine(path, stabuff, "VAC1", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[3*16+i].strLabel = pstr[i];
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "VAC1-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[3*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "VAC1-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[3*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "VAC1-%d", i);
		if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			m_DL.adc.chn[3*16+i].dMin = pdouble[0];
			m_DL.adc.chn[3*16+i].dMax = pdouble[1];
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR ADC#0:
	sprintf(stabuff, "ADCchannels");
	if( ReadInOneLine(path, stabuff, "SPR0", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[4*16+i].strLabel = pstr[i];
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "SPR0-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[4*16+i].bDiff = (pdouble[i] == 2);
	// Set current measurement scale factor if applicable (non-zero value = scale factor in Volts per Amp)
	if( ReadInOneLine(path, stabuff, "SPR0-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[4*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "SPR0-%d", i);
		if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			m_DL.adc.chn[4*16+i].dMin = pdouble[0];
			m_DL.adc.chn[4*16+i].dMax = pdouble[1];
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR ADC#1:
	sprintf(stabuff, "ADCchannels");
	if( ReadInOneLine(path, stabuff, "SPR1", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[5*16+i].strLabel = pstr[i];
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "SPR1-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[5*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "SPR1-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[5*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "SPR1-%d", i);
		if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			m_DL.adc.chn[5*16+i].dMin = pdouble[0];
			m_DL.adc.chn[5*16+i].dMax = pdouble[1];
		}
	}
	/////////////////////////////////////////////////////////////////
	
	delete [] pdouble;
	delete [] pstr;
}
*/

void CInsample0App::InitAutoTestParams( char* path )
{
	// AutoTest - Power
	m_AT.pAutoTestPower		= new double[35];
	m_AT.pAutoTestPowerExp	= new double[35];
	m_AT.pAutoTestPowerErr	= new double[35];
	m_AT.pPower_OutofRange	= new BOOL[35];
	int i;
	for( i=0; i<35; i++ )
	{
		m_AT.pAutoTestPower[i] = 0.0;
		m_AT.pAutoTestPowerExp[i] = 0.0;
		m_AT.pAutoTestPowerErr[i] = 0.0;
		m_AT.pPower_OutofRange[i] = FALSE;
	}

	char stabuff[20], staitembuff[20];
	double*		pCheck	= new double[33];
	int nTotal = 33;
	sprintf(stabuff, "AutoTest");
	sprintf(staitembuff, "PowerExp");
	if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, nTotal, pCheck) == nTotal )
	{
		for( i=0; i<nTotal; i++ )
			m_AT.pAutoTestPowerExp[i] = pCheck[i];
	}
	sprintf(staitembuff, "PowerErr");
	if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, nTotal, pCheck) == nTotal )
	{
		for( i=0; i<nTotal; i++ )
			m_AT.pAutoTestPowerErr[i] = pCheck[i];
	}
	delete [] pCheck;
}

int CInsample0App::ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
{
	char szBuf1[255];
	char szBuf2[255];
	int* pint = (int*) pData;
	double* pdouble = (double*)pData;
	long* plong = (long*)pData;
	CString* pstr = (CString*)pData;
	int i = 0;
	if(GetPrivateProfileString(Section, Entry, "", szBuf1, 255, path) != 0)
	{
		for(i=0; i<nTotal; i++)
		{
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, Separator) != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, Separator)+1);
				*strchr(szBuf2, Separator) = '\0';
				switch( uDataStyle )
				{
				case	INTSTYLE:
					pint[i] = atoi(szBuf2);
					break;
				case	DOUBLESTYLE:
					pdouble[i] = atof(szBuf2);
					break;
				case	LONGSTYLE:
					plong[i] = atol(szBuf2);
					break;
				case	STRSTYLE:
					pstr[i] = szBuf2;
					break;
				default:
					return 0;
				}
			}
			else
			{
				if(i == nTotal-1)
				{
					// This is the last data.
					switch( uDataStyle )
					{
					case	INTSTYLE:
						pint[i] = atoi(szBuf2);
						break;
					case	DOUBLESTYLE:
						pdouble[i] = atof(szBuf2);
						break;
					case	LONGSTYLE:
						plong[i] = atol(szBuf2);
						break;
					case	STRSTYLE:
						pstr[i] = szBuf2;
						break;
					default:
						return 0;
					}
					nTotal = i+1;
				}
				else
					nTotal = i;
				break;
			}
		}
	}
	return i+1;
}

int CInsample0App::SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
{
	char szBuf1[255];
	char szBuf2[255];
	*szBuf1 = '\0';
	int* pint = (int*) pData;
	double* pdouble = (double*)pData;
	long* plong = (long*)pData;
	int i = 0;
	for(i=0; i<nTotal; i++)
	{
		if(i == nTotal-1)
		{
			// This is the last data.
			switch( uDataStyle )
			{
			case	INTSTYLE:
				sprintf(szBuf2, "%d", pint[i]);
				break;
			case	DOUBLESTYLE:
				sprintf(szBuf2, "%f", pdouble[i]);
				break;
			case	LONGSTYLE:
				sprintf(szBuf2, "%ld", plong[i]);
				break;
			default:
				return 0;
			}
		}
		else
		{
			switch( uDataStyle )
			{
			case	INTSTYLE:
				sprintf(szBuf2, "%d%c", pint[i], Separator);
				break;
			case	DOUBLESTYLE:
				sprintf(szBuf2, "%f%c", pdouble[i], Separator);
				break;
			case	LONGSTYLE:
				sprintf(szBuf2, "%ld%c", plong[i], Separator);
				break;
			default:
				return 0;
			}
		}
		strcat(szBuf1, szBuf2);
	}
	WritePrivateProfileString(Section, Entry, szBuf1, path);
	return nTotal;
}
/////////////////////////////////////////////////////////////////////////////


void CInsample0App::UpdateListString( CString listStr )
{
	((CInsample0Dlg*)m_pMainWnd)->UpdateListString( listStr );
}

// 向MCI板写数据
// bOpcode:		操作码
// blOperand:	是否有操作数
// wData:		16 bits操作数
void CInsample0App::WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	if( !m_bIsFT232Ready )
		return;

// 	if( m_pSend != NULL )
// 		m_pSend->PostMCIOpcr( 0, bOpcode, blOperand, wData );
	CString str = m_232Device.Serial_WriteToBoard( bOpcode, blOperand, wData );
	UpdateListString( str );
}

void CInsample0App::CommToBoards( BYTE bOp1, BYTE bOp2 )
{
	if( !m_bIsFT232Ready )
		return;

// 	if( m_pSend != NULL )
// 		m_pSend->PostSubOpcr( 0, bOp1, bOp2 );
	CString str = m_232Device.Serial_CommToBoards( bOp1, bOp2 );
	UpdateListString( str );
}

void CInsample0App::CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
	if( !m_bIsFT232Ready )
		return;
	
// 	if( m_pSend != NULL )
// 		m_pSend->PostSubOpcr2( 0, bOp1, bOp2, bOp3, bOp4 );
	CString strRet = m_232Device.Serial_CommToBoards2( bOp1, bOp2, bOp3, bOp4 );
	CString str = strRet;
	int nPos = 0;
	do 
	{
		nPos = str.Find( '.' );
		if( nPos < 0 )
			UpdateListString( str );
		else
		{
			UpdateListString( str.Left( nPos ) );
			str = str.Right( str.GetLength() -nPos -2 );
		}
	} while ( nPos > 0 );
}

void CInsample0App::CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 )
{
	if( !m_bIsFT232Ready )
		return;

// 	if( m_pSend != NULL )
// 		m_pSend->PostSubOpcr3( 0, bOp1, bOp2, bOp3, bOp4, bOp5, bOp6 );
	CString str = m_232Device.Serial_CommToBoards3( bOp1, bOp2, bOp3, bOp4, bOp5, bOp6 );
	UpdateListString( str );
}

long CInsample0App::CommToBoardsReadADC( int nProcessID, int nADCnum, int nADCchn, BOOL bAnalyze )
{
	// nADCNum: 0:SCN; 1:LNS; 2:VAC #0; 3:VAC #1; 4:SPR #0; 5:SPR #1
	BYTE bCID, bModID, bTM = 0xC0;	// bCID(op1), bTM(op2)
	BOOL bADCtype = TRUE;
	switch( nADCnum )
	{
	case	0:
		bModID	= 1;
		bCID	= USB_CID_SCN_ADC_Read;
		break;
	case	1:
		bModID	= 2;
		bCID	= USB_CID_LNS_ADC_Read;
		bADCtype= FALSE;
		bTM		= 0xA0;
		break;
	case	2:
		bModID	= 3;
		bCID	= USB_CID_VAC_ADC0_Read;
		break;
	case	3:
		bModID	= 3;
		bCID	= USB_CID_VAC_ADC1_Read;
		break;
	case	4:
		bModID	= 4;
		bCID	= USB_CID_SPR_ADC0_Read;
		break;
	case	5:
		bModID	= 4;
		bCID	= USB_CID_SPR_ADC1_Read;
		break;
	}
	bTM	|= bModID;	// OP2

	BYTE bOSR, bChanNib, bEnSE;
	WORD wData;
	bOSR	= 0x90;			// 144, which takes ~73mS to complete conversion (not including transfer latencies)
	if( m_DL.adc.chn[nADCnum*16 +nADCchn].bDiff )
		bEnSE = 0xA0;		// 0xA0 for diff measurements
	else
		bEnSE = 0xB0;		// 0xB0 for S.E. measurement

	if( bADCtype )
	{
		// LTC2448 type
		if( nADCchn % 2 == 0 )	// 偶数
			bChanNib	= nADCchn /2;
		else
			bChanNib	= nADCchn /2 +8;
		if( m_DL.adc.chn[nADCnum*16 +nADCchn].bDiff )
			bChanNib &= 7;		// Force bit 3 of ChanNib to 0 if diff measurement

		wData = (bEnSE *256) | (bChanNib *256) | bOSR;
	}
	else
	{
		// LTC1859 type
		if( nADCchn %2 == 0 )	// 偶数
			wData	= nADCchn /2 *4096 +0x8000;	// 0x8000 - 0xB000
		else
			wData	= nADCchn /2 *4096 +0xC000;	// 0xC000 - 0xF000
	}

//	m_pSend->PostSubReadADC( nProcessID, bModID, bCID, nADCnum, nADCchn, bTM, wData, bAnalyze );
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DWORD CInsample0App::BoardReadback( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, WORD& wData1, WORD& wData2, CString &strMsg )
{
	CommToBoards2( bOp1, bOp2, bOp3, bOp4 );
	Sleep( 30 );

	unsigned short nData[16];
	ZeroMemory( &nData, sizeof(unsigned short) *16 );
	DWORD	dwBytesReturned = 0;
	if( theApp.Serial_Read( nData, dwBytesReturned ) == 0 )
	{
		((CInsample0Dlg*)m_pMainWnd)->m_listReadback.ResetContent();

		CString str;
		str.Format( "Data Received: %ld words. ", dwBytesReturned /2 );
		((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( str );
		if( dwBytesReturned /2 > 0 )
		{
			int i, nCount = dwBytesReturned /2;
			for( i=0; i<nCount; i++ )
			{
				str.Format( "%04X, ", nData[i] );
				strMsg += str;
				((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( str );
			}
			((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( " " );

			for( i=0; i<nCount; i++ )
			{
				if( nData[i] != 0xD22D )
					continue;

				wData1 = (WORD)(nData[i+1]);
				if( nCount == 3 )
					wData2 = (WORD)(nData[i+2]);
				else
					wData2 = 0x0;
			}
		}
		return (dwBytesReturned /2);
	}
	return 0;
}

void CInsample0App::BoardWrite0816( int nWsize, int nModID, BYTE bCktAddr, WORD wWdata )
{
	// Write 8 or 16 bits to board
	// This mutha accepts arguments to simplify writing to boards with reduced repetition of code
	BYTE bOP1, bOP2, bOP3, bOP4;
	// For all writes, OP1 = circuit address (CktAddr), OP2 upper nibble = transfer type, OP2 lower nibble=Module ID
	// For 8 bit writes, OP3 is always set to 0, OP4 contains data to write to board
	// For 16 bit writes, OP3 contains low byte, OP4 contains high byte
	bOP1	= bCktAddr;
	if( nWsize == 8 )
	{
		bOP2	= USB_TTTT_Write_08 << 4;		// Upper nibble of OP2 = Transfer Type (1 for 8 bit write) 0x10
		bOP3	= 0x0;
		bOP4	= (BYTE)wWdata;
	}
	else
	{
		bOP2	= USB_TTTT_Write_16 << 4;		// Upper nibble of OP2 = Transfer Type (2 for 16 bit write) 0x20
		bOP3	= (BYTE)(wWdata & 0xFF);
		bOP4	= (BYTE)((wWdata /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
	}
	bOP2		|= (BYTE)nModID;				// OR the target Module ID into lower nibble of OP2
	CommToBoards2(	bOP1,						// Operand 1 = ckt address
					bOP2,						// Operand 2 = Transfer type and Module ID
					bOP3,						// Operand 3: Always set to 0!
					bOP4 );						// Operand 4: ONLY valid data byte for 8 bit write
}

void CInsample0App::BoardWrite24( int nWsize, int nModID, BYTE bCktAddr, DWORD dwWdata )
{
	// Write 8, 16, or 24 bits to a board (SCN,LNS,VAC,SPR,etc.)
	// This mutha accepts arguments to simplify writing to boards with reduced repetition of code
	BYTE bOP1, bOP2, bOP3, bOP4, bOP5, bOP6;			// OP5 and OP6 only used for 24 bit writes. OP1-4 used for all writes.
	BOOL bCluster3;
	// For all writes, OP1 = circuit address (CktAddr), OP2 upper nibble = transfer type, OP2 lower nibble=Module ID
	// For 8 bit writes, OP3 is always set to 0, OP4 contains data to write to board
	// For 16 bit writes, OP3 contains low byte, OP4 contains high byte
	// For 24 bit writes, OP3=Byte #2 of 3, OP4=Byte#1 of 3, OP5=0, OP6=Byte #3 of 3
	switch( nWsize )	// Replicated so 1=8, 2=16, 3=24 in case byte count is passed instead of bit count
	{
	case	1:
		bOP2		= USB_TTTT_Write_08 << 4;			// Upper nibble of OP2 = Transfer Type (1 for 8 bit write) 0x10
		bOP3		= 0x0;
		bOP4		= (BYTE)dwWdata;
		bCluster3	= 0;
		break;
	case	2:
		bOP2		= USB_TTTT_Write_16 << 4;			// Upper nibble of OP2 = Transfer Type (2 for 16 bit write) 0x20
		bOP3		= (BYTE)(dwWdata & 0xFF);
		bOP4		= (BYTE)((dwWdata /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
		bCluster3	= 0;
		break;
	case	3:
		bOP2		= USB_TTTT_Write_24 << 1;			// 0x30
		bOP3		= (BYTE)((dwWdata /256) & 0xFF);	// (Mid)Byte #2 of 3; shift Wdata down 8 bits
		bOP4		= (BYTE)((dwWdata /65536) & 0xFF);	// (MS) Byte #1 of 3; shift Wdata down 16 bits
		bOP5		= 0x0;
		bOP6		= (BYTE)(dwWdata & 0xFF);			// (LS)Byte #3 of 3 'Just low 8 bits
		bCluster3	= 1;
		break;
	case	8:
		bOP2		= USB_TTTT_Write_08 << 4;			// Upper nibble of OP2 = Transfer Type (1 for 8 bit write) 0x10
		bOP3		= 0x0;
		bOP4		= (BYTE)dwWdata;
		bCluster3	= 0;
		break;
	case	16:
		bOP2		= USB_TTTT_Write_16 << 4;			// Upper nibble of OP2 = Transfer Type (2 for 16 bit write) 0x20
		bOP3		= (BYTE)(dwWdata & 0xFF);
		bOP4		= (BYTE)((dwWdata /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
		bCluster3	= 0;
		break;
	case	24:
		bOP2		= USB_TTTT_Write_24 << 4;			// 0x30
		bOP3		= (BYTE)(((dwWdata & 0xFF00)/256) & 0xFF);		// (Mid)Byte #2 of 3; shift Wdata down 8 bits
		bOP4		= (BYTE)(((dwWdata & 0xFF0000)/65536) & 0xFF);	// (MS) Byte #1 of 3; shift Wdata down 16 bits
		bOP5		= 0x0;
		bOP6		= (BYTE)(dwWdata & 0xFF);			// (LS)Byte #3 of 3 'Just low 8 bits
		bCluster3	= 1;
		break;
	default:
		return;
		break;
	}
	bOP1	= bCktAddr;			// Regardless of transaction type
	bOP2	|= (BYTE)nModID;	// OR the target Module ID into lower nibble of OP2

	// Done setting OP's, now send data to board:
	if( bCluster3 )
		CommToBoards3(	bOP1,		// Operand 1 = ckt address
						bOP2,		// Operand 2 = Transfer type and Module ID
						bOP3,		// Operand 3
						bOP4,		// Operand 4
						bOP5,		// Operand 5: Always set to 0 for 24 bit writes!
						bOP6 );		// Operand 6: LS byte for 24 bit write
	else
		CommToBoards2(	bOP1,		// Operand 1 = ckt address
						bOP2,		// Operand 2 = Transfer type and Module ID
						bOP3,		// Operand 3
						bOP4 );		// Operand 4
}

long CInsample0App::BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze )
{
	// READ up to 16 bits from a subboard, Wdata is normally 0.
	// Globals g_RBwdCnt and array: BoardReadData(n,n) is loaded here (and also in BoardRead2432 Sub)
	// Globals: g_TTID , g_MID, g_CID, g_ADCnum, g_ADCchan (these gloabal variables are only used for ADC reads)
	// g_ADCnum is set when ADCselectOB option is selected
	// g_ADCchan is set when ADCsChSelect option is selected

	BYTE bOP1, bOP2, bOP3, bOP4;
	// For all transfers, OP1 = circuit address (CktAddr), OP2 upper nibble = transfer type, OP2 lower nibble=Module ID
	// For 8 bit writes, OP3 is always set to 0, OP4 contains data to write to board
	// For 16 bit writes, OP3 contains low byte, OP4 contains high byte
	// 8 and 16 bit reads don't typically include writing, and setting OP3 and OP4 to 0 (for future possible usage)
	bOP1 = bCktAddr;
	if( nRsize == 8 )
	{
		bOP2	= 0x90;							// Upper nibble of OP2 = Transfer Type (9 for 8 bit Read)
		bOP3	= 0;
		bOP4	= (BYTE)wWdata;
	}
	else
	{
		bOP2	= 0xA0;							// Upper nibble of OP2 = Transfer Type (A for 16 bit Read)
		bOP3	= (BYTE)(wWdata & 0xFF);
		bOP4	= (BYTE)((wWdata /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
	}
	bOP2		|= (BYTE)nModID;				// OR the target Module ID into lower nibble of OP2

	CommToBoards2(	bOP1,						// OP1 = ckt address
					bOP2,						// OP2 = Transfer type and Module ID
					bOP3,						// OP3: Set to 0 for 8 bit transaction, Low byte for 16 bit transaction
					bOP4 );						// OP4: ONLY valid data byte for 8 bit write, upper byte for 16 bit transaction
	Sleep(30);

	WORD	wData[16];
	DWORD	dwBytesReturned = 0;
	long	lSerialRet = 0;
	if( Serial_Read( wData, dwBytesReturned ) == 0 )
	{
		((CInsample0Dlg*)m_pMainWnd)->m_listReadback.ResetContent();

		CString str, strList;
		str.Format( "BoardRead0816, Data Received: %ld words. ", dwBytesReturned/2 );
		((CInsample0Dlg*)m_pMainWnd)->m_listReadback.AddString( str );
		if( dwBytesReturned > 0 )
			lSerialRet = 0;
		else
			lSerialRet = 1;
		if( !bAnalyze )
			return lSerialRet;

		if( dwBytesReturned /2 > 0 )
		{
			int i, nCount = dwBytesReturned /2;
			for( i=0; i<nCount; i++ )
			{
				str.Format( "%04X, ", wData[i] );
				strMsgData += str;
				strList.Format( "@ %ld (hex %04X)", wData[i], wData[i] );
				((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( strList );
			}
			((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( " " );

			for( i=0; i<nCount; i++ )
			{
				if( wData[i] != 0xD22D )
					continue;

				m_nRBwdCnt = 1;
				m_nBoardReadDataBuf[ m_nRBwdCnt*2 ]		= wData[i];
				m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ]	= 4;

				m_nRBwdCnt = 2;
				m_nBoardReadDataBuf[ m_nRBwdCnt*2 ]		= wData[i+1];
				m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ]	= 1;

				m_nBoardReadDataBuf[0] = m_nRBwdCnt;	// # of valid readback words contained in the array
			}
		}
	}
	return lSerialRet;
}

long CInsample0App::BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze )
{
	// READ up to 32 bits from a subboard, Wdata controls ADC setup.
	// Globals g_RBwdCnt and array: BoardReadData(n,n) is loaded here (and also in BoardRead2432 Sub)
	// Globals: g_TTID , g_MID, g_CID, g_ADCnum, g_ADCchan (these gloabal variables are only used for ADC reads)
	// g_ADCnum is set when ADCselectOB option is selected
	// g_ADCchan is set when ADCsChSelect option is selected
	BYTE bOP1, bOP2, bOP3, bOP4;
	// For all transfers, OP1 = circuit address (CktAddr), OP2 upper nibble = transfer type, OP2 lower nibble=Module ID
	// For 32 bit reads from LTC2448 ADC's, OP3 contains low byte, OP4 contains high byte to SEND TO ADC during read
	bOP1 = bCktAddr;
	if( nRsize == 24 )
	{
		bOP2	= 0xB0;							// Upper nibble of OP2 = Transfer Type (A for 24 bit Read) '24 bit read NOT USED as of 2-21-12
		bOP3	= 0;
		bOP4	= (BYTE)dwWdata;
	}
	else
	{
		bOP2	= 0xC0;							// Upper nibble of OP2 = Transfer Type (C for 32 bit Read)
		bOP3	= (BYTE)(dwWdata & 0xFF);
		bOP4	= (BYTE)((dwWdata /256) & 0xFF);// Shift data down 8 bits before assigning to OP4
	}
	bOP2		|= (BYTE)nModID;				// OR the target Module ID into lower nibble of OP2

	CommToBoards2(	bOP1,						// OP1 = ckt address
					bOP2,						// OP2 = Transfer type and Module ID
					bOP3,						// OP3: Set to 0 for 8 bit transaction, Low byte for 16 bit transaction
					bOP4 );						// OP4: ONLY valid data byte for 8 bit write, upper byte for 16 bit transaction
	Sleep(30);

	WORD	wData[16];
	DWORD	dwBytesReturned = 0;
	long	lSerialRet = 0;
	if( Serial_Read( wData, dwBytesReturned ) == 0 )
	{
//		((CInsample0Dlg*)m_pMainWnd)->m_listReadback.ResetContent();

		CString str, strList;
		str.Format( "BoardRead2432, Data Received: %ld words. ", dwBytesReturned /2 );
		((CInsample0Dlg*)m_pMainWnd)->m_listReadback.AddString( str );
		if( dwBytesReturned > 0 )
			lSerialRet = 0;
		else
			lSerialRet = 1;
		if( !bAnalyze )
			return lSerialRet;
		
		if( dwBytesReturned /2 > 0 )
		{
			int i, nCount = dwBytesReturned /2;
			for( i=0; i<nCount; i++ )
			{
				str.Format( "%04X, ", wData[i] );
				strMsgData += str;
				strList.Format( "@ %ld (hex %04X)", wData[i], wData[i] );
				((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( strList );
			}
			((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( " " );

			for( i=0; i<nCount; i++ )
			{
				if( wData[i] != 0xD22D )
					continue;

				m_nRBwdCnt = 1;
				for( int j=i; j<nCount; j++ )
				{
					m_nBoardReadDataBuf[ m_nRBwdCnt*2 ]		= wData[j];
					switch( wData[j] )
					{
					case	0xFFFF:
						m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 9;
						break;
					case	0xD22D:
						m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 4;
						break;
					default:
						m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 1;
						break;
					}
					m_nRBwdCnt++;
				}
				m_nBoardReadDataBuf[0] = m_nRBwdCnt;	// # of valid readback words contained in the array
			}
		}
	}
	return lSerialRet;
}

long CInsample0App::ReadComprs( unsigned short* pnData, CString &strRes )
{
	// Start MCI Serial instruction (Step 1 of Serial process A)
	CommToBoards2(	USB_CID_VAC_ReadThermoCCGComp,	// OP1 = ckt addr. of comparator read register on VAC board
					USB_TM_VAC_Read_16,				// OP2 = 0xA3 (16 bit read from VAC board)
					0x0,							// OP3 = 0 (No data to be written)
					0x0 );							// OP4 = 0 (No data to be written)
	Sleep(30);

	unsigned short nData[16];
	DWORD	dwBytesReturned = 0;
	long	lSerialRet = 0;
	strRes = "";
	if( Serial_Read( nData, dwBytesReturned ) == 0 )
	{
		((CInsample0Dlg*)m_pMainWnd)->m_listReadback.ResetContent();

		CString str, strList;
		str.Format( "ReadComprs, Data Received: %ld words", dwBytesReturned/2 );
		((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( str );

		if( dwBytesReturned /2 > 0 )
		{
			int i, nCount = dwBytesReturned /2;
			for( i=0; i<nCount; i++ )
			{
				str.Format( "%04X, ", nData[i] );
				strRes += str;
				strList.Format( "@ %ld (hex %04X)", nData[i], nData[i] );
				((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( strList );
			}
			((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( " " );

			lSerialRet = 0;
			for( i=0; i<nCount; i++ )
			{
				if( nData[i] != 0xD22D )
					continue;
				*pnData = nData[i+1];
			}
		}
		else
			lSerialRet = 1;
	}
	return lSerialRet;
}

void CInsample0App::Delay( int msecond )
{
	WaitForSingleObject( ghTimeDelay, msecond );
}

CString CInsample0App::ChangedADCselectOB( int nNum )
{
	// SCN(1x16),LNS(1x8),VAC(2x16),SPR(2x16) (all LTC2448 type ADC's except for LNS board)
	m_nADCnum = nNum;

	CString str;
	switch( m_nADCnum )
	{
	case	0:									// SCN board LTC2448 (one per board)
// 		m_nADCmid		= USB_MID_SCN;			// SCN board
// 		m_nADCcid		= USB_CID_SCN_ADC_Read;	// Circuit ID 0xC8
// 		m_bADCtype		= TRUE;					// Arbitrary assignment for this software to properly format ADC write data for channel select, etc.
// 		m_dADCvref		= 4;
// 		m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	case	1:									// LNS board LTC1859 (one per board)
// 		m_nADCmid		= USB_MID_LNS;			// LNS board
// 		m_nADCcid		= USB_CID_LNS_ADC_Read;	// Circuit ID 0xC8
// 		m_bADCtype		= FALSE;				// Type = LTC1859
// 		m_dADCvref		= 10;					// 2.5;
// 		m_dADCCOMoffset	= 0;
		str = "% of Vref*2";
		break;
	case	2:									// VAC board LTC2448 #0
// 		m_nADCmid		= USB_MID_VAC;			// VAC board
// 		m_nADCcid		= USB_CID_VAC_ADC0_Read;// Circuit ID 0xC6
// 		m_bADCtype		= TRUE;					// Type = LTC2448
// 		m_dADCvref		= 4;
// 		m_dADCCOMoffset	= 1.607;
		str = "% of Vref/2";
		break;
	case	3:									// VAC board LTC2448 #1
// 		m_nADCmid		= USB_MID_VAC;			// VAC board
// 		m_nADCcid		= USB_CID_VAC_ADC1_Read;// Circuit ID 0xCE
// 		m_bADCtype		= 1;					// Type = LTC2448
// 		m_dADCvref		= 4;
// 		m_dADCCOMoffset	= 1.607;
		str = "% of Vref/2";
		break;
	case	4:									// SPR board LTC2448 #0
// 		m_nADCmid		= USB_MID_SPR;			// SPR board
// 		m_nADCcid		= USB_CID_SPR_ADC0_Read;// Circuit ID 0xC6
// 		m_bADCtype		= TRUE;					// Type = LTC2448
// 		m_dADCvref		= 4.5;
// 		m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	case	5:									// SPR board LTC2448 #1
// 		m_nADCmid		= USB_MID_SPR;			// SPR board
// 		m_nADCcid		= USB_CID_SPR_ADC1_Read;// Circuit ID 0xCE
// 		m_bADCtype		= TRUE;					// Type = LTC2448
// 		m_dADCvref		= 4.5;
// 		m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	}
	return str;
}

void CInsample0App::ChangedADCsChSelect( int nChan )
{
	// SCN(1x16),LNS(1x8),VAC(2x16),SPR(2x16) (all LTC2448 type ADC's except for LNS board)
	m_nADCchn = nChan;

// 	m_bADCdiff			= m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].bDiff;	// If FALSE then S.E. / If TRUE then diff mode
// 	m_dADCcurrentMeas	= m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].dFactor;// if non-zero value, then convert voltage to current measurement
// 
// 	if( m_dADCcurrentMeas > 0 )		// Don't calculate unless non-zero, otherwise generate divide-by-0 error:
// 	{
// 		m_dADCvoltsPerAmp = m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].dFactor;
// 		m_dADCampsPerVolt = 1 / m_dADCvoltsPerAmp;
// 	}
}

long CInsample0App::ReadSelectedADC( BOOL bAnalyze )
{
//	CommToBoardsReadADC( 0, m_nADCnum, m_nADCchn, bAnalyze );
//	return 0;

	int nRsize, nModID;
	BYTE bCktAddr, bOSR, bChanNib, bEnSE ;
	DWORD dwWdata;
	// Uses globals: g_TTID, g_MID, g_CID, g_ADCtype, g_ADCnum, g_ADCchan
	bOSR	= 0x90;			// 144, which takes ~73mS to complete conversion (not including transfer latencies)
	if( m_DL.adc.chn[m_nADCnum*16+m_nADCchn].bDiff )
		bEnSE = 0xA0;		// 0xA0 for diff measurements
	else
		bEnSE = 0xB0;		// 0xB0 for S.E. measurement

	if(  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].bType )		// LTC2448 type
	{
		nRsize	= 32;
		if( m_nADCchn % 2 == 0 )	// 偶数
			bChanNib	= m_nADCchn /2;
		else
			bChanNib	= m_nADCchn /2 +8;
		if(  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].bDiff )
			bChanNib &= 7;			// Force bit 3 of ChanNib to 0 if diff measurement

		dwWdata = (bEnSE *256) | (bChanNib *256) | bOSR;
	}	// if LTC2448 type
	else
	{
		nRsize	= 16;
		if( m_nADCchn % 2 == 0 )	// 偶数
			dwWdata	= m_nADCchn /2 *4096 +0x8000;	// 0x8000 - 0xB000
		else
			dwWdata	= m_nADCchn /2 *4096 +0xC000;	// 0xC000 - 0xF000
	}	// LTC1859 type
	
	nModID		=  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].nMID;
	bCktAddr	= (BYTE)(m_DL.adc.chn[m_nADCnum*16+m_nADCchn].nCID);

	long lRet = 0;
	CString strMsgCounts, strMsgData;
	if(  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].bType )
		lRet = BoardRead2432( nRsize, nModID, bCktAddr, dwWdata, strMsgCounts, strMsgData, bAnalyze );
	else
		lRet = BoardRead0816( nRsize, nModID, bCktAddr, (WORD)dwWdata, strMsgCounts, strMsgData, bAnalyze );

	return lRet;
}

CString CInsample0App::ReadSelectedADC_Logging( DWORD dwMilliseconds )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes;
	strRes = "";

	long lRet1 = 1;
	long lRet2 = 1;
	while( lRet1 >0 || lRet2 > 0 )
	{
		lRet1 = ReadSelectedADC();
		Sleep( dwMilliseconds );
		lRet2 = ReadSelectedADC();
		// 12.11.09 for comm test
		if( lRet1 > 0 || lRet2 > 0 )
		{
			if( lRet1 > 0 && lRet2 == 0 )
				strRes = "1F2S,";
			else if( lRet1 == 0 && lRet2 > 0 )
				strRes = "1S2F,";
			else
				strRes = "12F,";
		}
		// 12.11.09 for comm test
	}

	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].dMin;
	double dRangeMax = m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].dMax;
	double dEquivCurrent = 0.0;

	if(  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].bType )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)m_nBoardReadDataBuf[2*2]<<16) | (DWORD)m_nBoardReadDataBuf[3*2];
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
		strRes += str;

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
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			// g_ADCvref varies per board design
			str.Format( "%.5f\t", dADCreadVrefFrac *  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2 );
			strRes += str;
			str.Format( "%.5f\t", dADCreadVrefFrac *  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2 + m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCOMoffset );
			strRes += str;
			dEquivCurrent = (dADCreadVrefFrac *  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2 + m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCOMoffset) *  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCampsPerVolt;
		}
		else
		{
			str.Format( "-%.4f\t", dADCreadVrefPct );
			strRes += str;
			str.Format( "-%.5f\t", dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2 );
			strRes += str;
			str.Format( "%.5f\t", m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCOMoffset - dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2 );
			strRes += str;
			dEquivCurrent = (m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCOMoffset - dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2) * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(m_nBoardReadDataBuf[2*2]);
		dwADCreadRaw &= 0xFFFF;
		str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
		strRes += str;

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
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			str.Format( "%.5f\t", dADCreadVrefFrac * 5 );
			strRes += str;
			strRes += str;
			dEquivCurrent = dADCreadVrefFrac * 5 * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCampsPerVolt;
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			str.Format( "%.5f\t", dADCreadVrefFrac * 5 );
			strRes += str;
			strRes += str;
			dEquivCurrent = dADCreadVrefFrac * 5 * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCampsPerVolt;
		}
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * m_dADCvref / 2 * m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if(  m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dFactor > 0 )
	{
		str.Format( "%.6f\t", dEquivCurrent );
		strRes += str;
	}
	else
	{
		str.Format( "n/a\t" );
		strRes += str;
	}

	Sleep( 0 );//dwMilliseconds +40 );
	if( strRes.IsEmpty() )
		strRes.Format( "\t\t\t\t" );
	return strRes;
}

CString CInsample0App::ReadSelectedADC_Logging2( DWORD dwMilliseconds, double &dValue )
{
	CString str, strRes = "";
	if( m_DL.adc.chn[m_nADCnum*16+m_nADCchn].bType )	// 2448
	{
		ReadSelectedADC( FALSE );		// 残留值
		ReadSelectedADC( FALSE );		// 刚刚建立，转换还未完成
		Delay( dwMilliseconds );		// 等待建立及转换
		{
			ReadSelectedADC( TRUE );	// 真正的回读值，只读一次
			Delay( dwMilliseconds );	// 等待转换完成
			strRes = ADCvconv_Logging( dValue );
		}
	}
	else				// 1859
	{
		ReadSelectedADC( FALSE );		// 残留值
		ReadSelectedADC( FALSE );		// 刚刚建立，转换还未完成
		Delay( dwMilliseconds );		// 等待转换完成
		{
			ReadSelectedADC( TRUE );	// 真正的回读值，只读一次
			strRes = ADCvconv_Logging( dValue );
		}
	}
	return strRes;
}

CString CInsample0App::ReadSelectedADC_Logging_onetime( DWORD dwMilliseconds, double &dValue, BOOL bDelay )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes;
	strRes = "";

	long lRet1 = 1;
	long lTimes = 0;
	while( (lRet1 >0) && (lTimes < 5) )
	{
		lRet1 = ReadSelectedADC( TRUE );
		// 12.11.09 for comm test
		if( lRet1 > 0 )
		{
			strRes += "Fail,";
			lTimes++;
		}
		// 12.11.09 for comm test
	}
	strRes = ADCvconv_Logging( dValue );
	if( bDelay )
		theApp.Delay( dwMilliseconds );
	return strRes;
}

CString CInsample0App::ADCvconv_Logging( double &dValue, BOOL bLog )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes = "";
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].dMin;
	double dRangeMax = m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].dMax;
	double dEquivCurrent = 0.0;

	if( m_DL.adc.chn[m_nADCnum*16+m_nADCchn].bType )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)m_nBoardReadDataBuf[2*2]<<16) | (DWORD)m_nBoardReadDataBuf[3*2];
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		if( bLog )
		{
			str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
			strRes += str;
		}

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
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
				// g_ADCvref varies per board design
				str.Format( "%.6f\t", dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2 );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2 +m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCOMoffset;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCampsPerVolt;
		}
		else
		{
			if( bLog )
			{
				str.Format( "-%.4f\t", dADCreadVrefPct );
				strRes += str;
				str.Format( "-%.6f\t", dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2 );
				strRes += str;
			}
			dValue = m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCOMoffset - dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref /2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(m_nBoardReadDataBuf[2*2]);
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		if( bLog )
		{
			str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
			strRes += str;
		}

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
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dVref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		dEquivCurrent = dValue * m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * m_dADCvref / 2 * m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( bLog )
	{
		if( m_DL.adc.chn[m_nADCnum*16+m_nADCchn].dFactor > 0 )
		{
			str.Format( "%.6f\t", dEquivCurrent );
			strRes += str;
		}
		else
		{
			str.Format( "n/a\t" );
			strRes += str;
		}

		if( strRes.IsEmpty() )
			strRes.Format( "\t\t\t\t" );
	}
	return strRes;
}

void CInsample0App::OnlySendReadSelectedADCcmd( logParam lP)
{
	if( lP.nType == 2 )
	{
		// read gauge compr
		CommToBoards2(	USB_CID_VAC_ReadThermoCCGComp,	// OP1 = ckt addr. of comparator read register on VAC board
						USB_TM_VAC_Read_16,				// OP2 = 0xA3 (16 bit read from VAC board)
						0x0,							// OP3 = 0 (No data to be written)
						0x0 );							// OP4 = 0 (No data to be written)
		return;
	}
	if( lP.nType == 3 )
	{
		// read Ext. switch
		return;
	}

	BYTE bOSR, bChanNib, bEnSE ;
	DWORD dwWdata;
	// Uses globals: g_TTID, g_MID, g_CID, g_ADCtype, g_ADCnum, g_ADCchan
	bOSR	= 0x90;			// 144, which takes ~73mS to complete conversion (not including transfer latencies)
	if( lP.bDiff )
		bEnSE = 0xA0;		// 0xA0 for diff measurements
	else
		bEnSE = 0xB0;		// 0xB0 for S.E. measurement

	if( lP.bType )	// LTC2448 type
	{
		if( lP.nChn % 2 == 0 )	// 偶数
			bChanNib	= lP.nChn /2;
		else
			bChanNib	= lP.nChn /2 +8;
		if( lP.bDiff )
			bChanNib &= 7;		// Force bit 3 of ChanNib to 0 if diff measurement
		if( lP.bDiff )
			bChanNib &= 7;		// Force bit 3 of ChanNib to 0 if diff measurement

		dwWdata = (bEnSE *256) | (bChanNib *256) | bOSR;
	}	// if LTC2448 type
	else	// LTC1859 type, format write data for channel select, etc.
	{
		if( lP.nChn %2 == 0 )	// 偶数
			dwWdata	= lP.nChn /2 *4096 +0x8000;	// 0x8000 - 0xB000
		else
			dwWdata	= lP.nChn /2 *4096 +0xC000;	// 0xC000 - 0xF000
	}	// LTC1859 type

	BYTE bOP2, bOP3, bOP4;
	if( lP.bType )	// 2448, 32 bit read adc
		// Upper nibble of OP2 = Transfer Type (C for 32 bit Read)
		// OR the target Module ID into lower nibble of OP2
		bOP2	= 0xC0 | (BYTE)lP.nMID;
	else			// 1859, 16 bit read adc
		// Upper nibble of OP2 = Transfer Type (A for 16 bit Read)
		// OR the target Module ID into lower nibble of OP2
		bOP2	= 0xA0 | (BYTE)lP.nMID;
	bOP3	= (BYTE)(dwWdata & 0xFF);
	bOP4	= (BYTE)((dwWdata /256) & 0xFF);// Shift data down 8 bits before assigning to OP4
	CommToBoards2(	(BYTE)lP.nCID,		// Operand 1 = ckt address
					bOP2,				// Operand 2 = Transfer type and Module ID
					bOP3,
					bOP4 );
}

CString CInsample0App::ADCvconv_Logging( int nData1, int nData2, logParam lP, double &dValue, BOOL bLog )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes = "";
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;

	double dRangeMin = m_DL.adc.chn[lP.nNum*16 +lP.nChn].dMin;
	double dRangeMax = m_DL.adc.chn[lP.nNum*16 +lP.nChn].dMax;
	double dEquivCurrent = 0.0;

	double dADCvref, dADCCOMoffset;
	switch( lP.nNum )
	{
	case	0:
		dADCvref		= 4;
		dADCCOMoffset	= 0;
		break;
	case	1:
		dADCvref		= 10;
		dADCCOMoffset	= 0;
		break;
	case	2:
		dADCvref		= 4;
		dADCCOMoffset	= 1.607;
		break;
	case	3:
		dADCvref		= 4;
		dADCCOMoffset	= 1.607;
		break;
	case	4:
		dADCvref		= 4.5;
		dADCCOMoffset	= 0;
		break;
	case	5:
		dADCvref		= 4.5;
		dADCCOMoffset	= 0;
		break;
	}
	double dADCcurrentMeas	= m_DL.adc.chn[lP.nNum*16 +lP.nChn].dFactor;// if non-zero value, then convert voltage to current measurement
	double dADCvoltsPerAmp, dADCampsPerVolt;
	dADCvoltsPerAmp = dADCampsPerVolt = 0;
	if( dADCcurrentMeas > 0 )		// Don't calculate unless non-zero, otherwise generate divide-by-0 error:
	{
		dADCvoltsPerAmp = m_DL.adc.chn[lP.nNum*16 +lP.nChn].dFactor;
		dADCampsPerVolt = 1 / dADCvoltsPerAmp;
	}

	if( lP.bType )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)nData1<<16) | (DWORD)nData2;
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		if( bLog )
		{
			str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
			strRes += str;
		}

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
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
				// g_ADCvref varies per board design
				str.Format( "%.6f\t", dADCreadVrefFrac * dADCvref /2 );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref /2 +dADCCOMoffset;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * dADCampsPerVolt;
		}
		else
		{
			if( bLog )
			{
				str.Format( "-%.4f\t", dADCreadVrefPct );
				strRes += str;
				str.Format( "-%.6f\t", dADCreadVrefFrac * dADCvref /2 );
				strRes += str;
			}
			dValue = dADCCOMoffset - dADCreadVrefFrac * dADCvref /2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)nData1;
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		if( bLog )
		{
			str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
			strRes += str;
		}

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
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		dEquivCurrent = dValue * dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * dADCvref / 2 * dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( bLog )
	{
		if( dADCcurrentMeas > 0 )
		{
			str.Format( "%.6f\t", dEquivCurrent );
			strRes += str;
		}
		else
		{
			str.Format( "n/a\t" );
			strRes += str;
		}

		if( strRes.IsEmpty() )
			strRes.Format( "\t\t\t\t" );
	}
	return strRes;
}

// ADC回读后使用ADCvconv将模拟量转换为数字量
// 现在要由ADC回读值数字量得到实际物理量（电压、电流）
double CInsample0App::GetPhysicsFromAdcs( int nIndex, double dVadc )
{
	// 由曲线图索引得到ADC通道号索引
	//	int nIndex = theApp.m_DL.graph[nIndexGraph].nIndex;
	// 取出该通道的公式字符串
	CString strFunc = theApp.m_DL.log.strCalc[nIndex];
	if( strFunc == "#" )
		return dVadc;

	// 进行公式改写
	CString strVfront, strVback, strNew;	// V前字符串，V后字符串，加入数值的公式字符串
	int nPos = strFunc.Find( "V" );
	if( nPos > 0 )	// V不是公式中的第一个字符
	{
		strVfront	= strFunc.Left( nPos );
		strVback	= strFunc.Right( strFunc.GetLength() - nPos -1 );
		strNew.Format( "%s%.6f%s", strVfront, dVadc, strVback );
	}
	else
	{
		strVback	= strFunc.Right( strFunc.GetLength() -1 );	// 取除去V的所有字符串
		strNew.Format( "%.6f%s", dVadc, strVback );
	}

	// 调用公式分析计算
	char cFunc[100];
	sprintf( cFunc, "%s", strNew );
	return counter( cFunc );
}
