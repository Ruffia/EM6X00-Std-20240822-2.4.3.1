// AutoTest.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AutoTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CAutoTestApp

BEGIN_MESSAGE_MAP(CAutoTestApp, CWinApp)
	//{{AFX_MSG_MAP(CAutoTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoTestApp construction

CAutoTestApp::CAutoTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_pnReadComprData = new unsigned short[ ReadBack_DataSize +256 ];
	ZeroMemory( m_pnReadComprData, sizeof(m_pnReadComprData) );
	m_pnReadADCData = new unsigned short[ ReadBack_DataSize +256 ];
	ZeroMemory( m_pnReadADCData, sizeof(m_pnReadADCData) );
//	m_pnReadCommonData = new unsigned short[ ReadBack_DataSize +256 ];
//	ZeroMemory( m_pnReadCommonData, sizeof(m_pnReadCommonData) );
	
	///////////////////////////////////////////////////////////
	m_hTimeDelay = CreateEvent( NULL, FALSE, TRUE, "TimeDelay" );
	
	m_bDiagLogging		= FALSE;

	int i;
	// Diag/Logging Graph
//	for( i=0; i<GraphDataBuffGroups; i++ )
//		m_pGraphDataBuff[i] = NULL;
//	for( i=0; i<GraphDataBuffGroups; i++ )
//		m_pDiffGraphDataBuff[i] = NULL;
	
	// AutoTest
	m_AT.pAutoTestPower		= NULL;
	m_AT.pAutoTestPowerExp	= NULL;
	m_AT.pAutoTestPowerErr	= NULL;
	m_AT.pPower_OutofRange	= NULL;
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
		m_Cali.pCaliLNS[i] = NULL;
		m_Cali.pCaliSCN[i] = NULL;
		m_Cali.pCaliMCI[i] = NULL;
		m_Cali.pCaliSPR[i] = NULL;
	}
	for( i=0; i<10; i++ )
	{
		m_Cali.dCaliCoX[i]	= 0.0;
		m_Cali.dCaliCoY[i]	= 0.0;
		m_Cali.dCaliCoWD[i]	= 0.0;
		m_Cali.bCaliCoSel[i]	= FALSE;
	}
	m_Cali.nCaliCoCounts	= 0;

	// Board MCI
	for( i=0; i<6; i++ )
	{
		m_BrdMCI.nOffset[i]	= 0;
		m_BrdMCI.nGain[i]	= 5;
	}
	for( i=0; i<3; i++ )
		m_BrdMCI.nFilter[i]	= 0;
	m_BrdMCI.nDetPair	= 0;
	m_BrdMCI.bDet2nd	= FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAutoTestApp object

CAutoTestApp theApp;


BOOL CAutoTestApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CFGfile_InitParams();	
	return CWinApp::InitInstance();
}

int CAutoTestApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete [] m_pnReadComprData;
	delete [] m_pnReadADCData;
//	delete [] m_pnReadCommonData;

	int i;
/*	for( i=0; i<GraphDataBuffGroups; i++ )
	{
		if ( m_pGraphDataBuff[i] != NULL )
		{
			delete [] m_pGraphDataBuff[i];
			m_pGraphDataBuff[i]	= NULL;
		}
	}
	for( i=0; i<GraphDataBuffGroups; i++ )
	{
		if ( m_pDiffGraphDataBuff[i] != NULL )
		{
			delete [] m_pDiffGraphDataBuff[i];
			m_pDiffGraphDataBuff[i]	= NULL;
		}
	}*/
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
		if( m_Cali.pCaliLNS[i] != NULL )
			delete [] m_Cali.pCaliLNS[i];
		if( m_Cali.pCaliSCN[i] != NULL )
			delete [] m_Cali.pCaliSCN[i];
		if( m_Cali.pCaliMCI[i] != NULL )
			delete [] m_Cali.pCaliMCI[i];
		if( m_Cali.pCaliSPR[i] != NULL )
			delete [] m_Cali.pCaliSPR[i];
	}
	
	return CWinApp::ExitInstance();
}

DWORD CAutoTestApp::SemCoreCtrlPanelMessageProc(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCtrlPanelManager::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	return 0;
}

void CAutoTestApp::Delay( int msecond )
{
	WaitForSingleObject( m_hTimeDelay, msecond );
}




///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//Read data in one line in *.ini.
//In one line the data like ***,*****,*****, they are seperated by comma.

int CAutoTestApp::ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
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

int CAutoTestApp::SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
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
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
void CAutoTestApp::CFGfile_InitParams()
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	CFGfile_InitChangingParams( path );
	CFGfile_InitSelectedADC( path );
	CFGfile_InitLoggingParams( path );

	CFGfile_InitAutoTestParams( path );

//	m_DL.other.nElapseChanging	= 2000;
//	m_DL.other.nElapseLogging	= 2000;
//	m_DL.other.bGaugeCompr		= FALSE;
	int i;
	for( i=0; i<4; i++ )
	{
		theApp.m_AT.pAutoTestMCI[i] = new double[AutoTest_16bitDAC_Steps*3];
		theApp.m_AT.nAutoTestMCI_Total[i] = 0;
	}
	for( i=0; i<4; i++ )
	{
		theApp.m_AT.pAutoTestSCN[i] = new double[AutoTest_12bitDAC_Steps*3];
		theApp.m_AT.nAutoTestSCN_Total[i] = 0;
	}
	for( i=0; i<4; i++ )
	{
		theApp.m_AT.pAutoTestLNS[i] = new double[AutoTest_16bitDAC_Steps*3];
		theApp.m_AT.nAutoTestLNS_Total[i] = 0;
	}
	theApp.m_AT.pAutoTestVAC = new double[22];
	for( i=0; i<22; i++ )
	{
		theApp.m_AT.pAutoTestVAC[i] = 0.0;
		theApp.m_AT.bVAC_OutofRange[i] = FALSE;
	}
	for( i=0; i<4; i++ )
	{
		theApp.m_AT.pAutoTestSPR[i] = new double[AutoTest_16bitDAC_Steps*3];
		theApp.m_AT.nAutoTestSPR_Total[i] = 0;
	}

	for( i=0; i<4; i++ )
	{
		theApp.m_Cali.pCaliMCI[i] = new double[Cali_16bitDAC_Steps+1];
		for( int j=0; j<Cali_16bitDAC_Steps+1; j++ )
			theApp.m_Cali.pCaliMCI[i][j] = 0;
		theApp.m_Cali.nCaliMCI_Total[i] = 0;
	}
	for( i=0; i<4; i++ )
	{
		theApp.m_Cali.pCaliSCN[i] = new double[Cali_16bitDAC_Steps+1];
		for( int j=0; j<Cali_16bitDAC_Steps+1; j++ )
			theApp.m_Cali.pCaliSCN[i][j] = 0;
		theApp.m_Cali.nCaliSCN_Total[i] = 0;
	}
	for( i=0; i<4; i++ )
	{
		theApp.m_Cali.pCaliLNS[i] = new double[Cali_16bitDAC_Steps+1];
		for( int j=0; j<Cali_16bitDAC_Steps+1; j++ )
			theApp.m_Cali.pCaliLNS[i][j] = 0;
		theApp.m_Cali.nCaliLNS_Total[i] = 0;
	}
	for( i=0; i<4; i++ )
	{
		theApp.m_Cali.pCaliSPR[i] = new double[Cali_16bitDAC_Steps+1];
		for( int j=0; j<Cali_16bitDAC_Steps+1; j++ )
			theApp.m_Cali.pCaliSPR[i][j] = 0;
		theApp.m_Cali.nCaliSPR_Total[i] = 0;
	}
}

void CAutoTestApp::CFGfile_InitChangingParams( char* path )
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
	nTotal = 10;
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

void CAutoTestApp::CFGfile_InitLoggingParams( char* path )
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
			m_DL.log2.logParams[m_DL.log2.nCounts].nType= 2;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum	= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID	= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID	= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn	= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff= 0;
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
			m_DL.log2.logParams[m_DL.log2.nCounts].nType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum	= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID	= USB_MID_SCN;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID	= USB_CID_SCN_ADC_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn	= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff= m_DL.adc.chn[j*16 +i].bDiff;
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
			m_DL.log2.logParams[m_DL.log2.nCounts].nType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum	= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID	= USB_MID_LNS;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID	= USB_CID_LNS_ADC_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn	= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType= 0;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff= m_DL.adc.chn[j*16 +i].bDiff;
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
			m_DL.log2.logParams[m_DL.log2.nCounts].nType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum	= 2;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID	= USB_MID_VAC;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID	= USB_CID_VAC_ADC0_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn	= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff= m_DL.adc.chn[j*16 +i].bDiff;
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
			m_DL.log2.logParams[m_DL.log2.nCounts].nType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum	= 3;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID	= USB_MID_VAC;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID	= USB_CID_VAC_ADC1_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn	= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff= m_DL.adc.chn[j*16 +i].bDiff;
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
			m_DL.log2.logParams[m_DL.log2.nCounts].nType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum	= 4;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID	= USB_MID_SPR;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID	= USB_CID_SPR_ADC0_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn	= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff= m_DL.adc.chn[j*16 +i].bDiff;
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
			m_DL.log2.logParams[m_DL.log2.nCounts].nType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].nNum	= 5;
			m_DL.log2.logParams[m_DL.log2.nCounts].nMID	= USB_MID_SPR;
			m_DL.log2.logParams[m_DL.log2.nCounts].nCID	= USB_CID_SPR_ADC1_Read;
			m_DL.log2.logParams[m_DL.log2.nCounts].nChn	= i;
			m_DL.log2.logParams[m_DL.log2.nCounts].bType= 1;
			m_DL.log2.logParams[m_DL.log2.nCounts].bDiff= m_DL.adc.chn[j*16 +i].bDiff;
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
/*
	// 2.430799(OL回读值)
//	GetPhysicsFromAdcs( 0*16 +8, 1.51 );
	
	/////////////////////////////////////////////////////////////////
	// 13.03.31 模拟TC回读值，实验绘制TC回读曲线
	/////////////////////////////////////////////////////////////////
	m_testData = NULL;
	// 获取工作路径
	char testpath[256];
	::GetModuleFileName(0, testpath, 255);
	char *exd = strrchr(testpath, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(testpath, "\\TCAmp0Test.txt");

	FILE *stream;
	stream = fopen( testpath, "r" );
	if( stream == NULL )
		return;

	int	k, nLength;
	char	cBuff[256];
	// Set pointer to beginning of file:
	fseek( stream, 0L, SEEK_SET );
	// Read data back from file:
	fscanf( stream, "%s", cBuff );
	fscanf( stream, "%ld",&nLength );

	m_testData = new double[4*nLength];
	float fp1,fp2,fp3,fp4;
	for ( k=0; k<nLength; k++ )
	{	
		fscanf( stream, "%f%f%f%f", &fp1, &fp2, &fp3, &fp4 );
		m_testData[0*nLength +k] = fp1;
		m_testData[1*nLength +k] = fp2;
		m_testData[2*nLength +k] = fp3;
		m_testData[3*nLength +k] = fp4;
	}

	fclose( stream );*/
	/////////////////////////////////////////////////////////////////
}

void CAutoTestApp::CFGfile_InitSelectedADC( char* path )
{
	m_nADCnum = m_nADCchn = 0;
	m_dADCcurrentMeas = m_dADCvoltsPerAmp = m_dADCampsPerVolt = m_dADCvref = m_dADCCOMoffset = 0;
	m_bADCdiff = m_bADCtype = FALSE;

    m_nADCmid	= 0x0;		// Module Selection ID
    m_nADCcid	= 0x0;		// Circuit Selection ID

	/////////////////////////////////////////////////////////////////
	char stabuff[20];
	char staitembuff[20];
	double*		pdouble	= new double[16];
	CString*	pstr	= new CString[16];
	int			nTotal	= 16;
	int			i;

	/////////////////////////////////////////////////////////////////
	// SCN ADC (only one ADC on SCN board):
	sprintf(stabuff, "ADCchannels");
	if( ReadInOneLine(path, stabuff, "SCN", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[0*16+i].strLabel = pstr[i];
	// Set channels 0/1, 2/3, 4/5, 6/7 to diff (value of x,x,1 = 2), all others to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "SCN-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[0*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "SCN-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_DL.adc.chn[0*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "SCN-%d", i);
		if( ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			m_DL.adc.chn[0*16+i].dMin	= pdouble[0];
			m_DL.adc.chn[0*16+i].dMax	= pdouble[1];
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
			m_DL.adc.chn[1*16+i].dMin	= pdouble[0];
			m_DL.adc.chn[1*16+i].dMax	= pdouble[1];
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
			m_DL.adc.chn[2*16+i].dMin	= pdouble[0];
			m_DL.adc.chn[2*16+i].dMax	= pdouble[1];
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
			m_DL.adc.chn[3*16+i].dMin	= pdouble[0];
			m_DL.adc.chn[3*16+i].dMax	= pdouble[1];
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
			m_DL.adc.chn[4*16+i].dMin	= pdouble[0];
			m_DL.adc.chn[4*16+i].dMax	= pdouble[1];
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
			m_DL.adc.chn[5*16+i].dMin	= pdouble[0];
			m_DL.adc.chn[5*16+i].dMax	= pdouble[1];
		}
	}
	/////////////////////////////////////////////////////////////////
	
	delete [] pdouble;
	delete [] pstr;
}

void CAutoTestApp::CFGfile_InitAutoTestParams( char* path )
{
	// AutoTest - Power
	m_AT.pAutoTestPower = new double[35+8];
	m_AT.pAutoTestPowerExp = new double[35+8];
	m_AT.pAutoTestPowerErr = new double[35+8];
	m_AT.pPower_OutofRange = new BOOL[35+8];
	int i;
	for( i=0; i<35+8; i++ )
	{
		m_AT.pAutoTestPower[i] = 0.0;
		m_AT.pAutoTestPowerExp[i] = 0.0;
		m_AT.pAutoTestPowerErr[i] = 0.0;
		m_AT.pPower_OutofRange[i] = FALSE;
	}

	char stabuff[20], staitembuff[20];
	double*		pCheck	= new double[33+8];
	int nTotal = 33+8;
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
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
