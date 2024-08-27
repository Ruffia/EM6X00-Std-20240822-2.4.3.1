#include "stdafx.h"
#include "CFGFileManager.h"
#include "GlobalParamSetting.h"
#include "SemBasisCtrl.h"
#include <math.h>
#include "ControlLayer.h"
#include "MCICardCommonSetting.h"
#include "AppOption.h"
#include "CommonFunctionManager.h"
#include "SemCoreAssemble.h"
#include "VACDataManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CCFGFileManager& CCFGFileManager::Instance()
{
	static CCFGFileManager Inst;
	return Inst;
}

CCFGFileManager::CCFGFileManager()
{
}

void CCFGFileManager::CFGfile_InitChangingParams( char* path, DL_Sum_Param& DL )
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
	// SCN & LNS
	nTotal = 11;
	sprintf(staitembuff, "SCN-Check");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongCheck);
	sprintf(staitembuff, "SCN-Min");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMin);
	sprintf(staitembuff, "SCN-Max");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMax);
	sprintf(staitembuff, "SCN-Step");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongStep);
	sprintf(staitembuff, "SCN-Intv");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongIntv);
	for( i=0; i<nTotal; i++,j++ )
	{
		DL.diag[ j ].nCheck	= plongCheck[i];
		DL.diag[ j ].nMin		= plongMin[i];
		DL.diag[ j ].nMax		= plongMax[i];
		DL.diag[ j ].nStep	= plongStep[i];
		DL.diag[ j ].nIntv	= plongIntv[i];
	}


	// VAC
	nTotal = 12;
	sprintf(staitembuff, "VAC-Check");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongCheck);
	sprintf(staitembuff, "VAC-Min");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMin);
	sprintf(staitembuff, "VAC-Max");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMax);
	sprintf(staitembuff, "VAC-Step");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongStep);
	sprintf(staitembuff, "VAC-Intv");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongIntv);
	for( i=0; i<nTotal; i++,j++ )
	{
		DL.diag[ j ].nCheck	= plongCheck[i];
		DL.diag[ j ].nMin		= plongMin[i];
		DL.diag[ j ].nMax		= plongMax[i];
		DL.diag[ j ].nStep	= plongStep[i];
		DL.diag[ j ].nIntv	= plongIntv[i];
	}


	// SPR
	nTotal = 4;
	sprintf(staitembuff, "SPR-Check");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongCheck);
	sprintf(staitembuff, "SPR-Min");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMin);
	sprintf(staitembuff, "SPR-Max");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMax);
	sprintf(staitembuff, "SPR-Step");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongStep);
	sprintf(staitembuff, "SPR-Intv");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongIntv);
	for( i=0; i<nTotal; i++,j++ )
	{
		DL.diag[ j ].nCheck	= plongCheck[i];
		DL.diag[ j ].nMin		= plongMin[i];
		DL.diag[ j ].nMax		= plongMax[i];
		DL.diag[ j ].nStep	= plongStep[i];
		DL.diag[ j ].nIntv	= plongIntv[i];
	}


	// MCI
	nTotal = 10;
	sprintf(staitembuff, "MCI-Check");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongCheck);
	sprintf(staitembuff, "MCI-Min");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMin);
	sprintf(staitembuff, "MCI-Max");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongMax);
	sprintf(staitembuff, "MCI-Step");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongStep);
	sprintf(staitembuff, "MCI-Intv");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, plongIntv);
	for( i=0; i<nTotal; i++,j++ )
	{
		DL.diag[ j ].nCheck	= plongCheck[i];
		DL.diag[ j ].nMin		= plongMin[i];
		DL.diag[ j ].nMax		= plongMax[i];
		DL.diag[ j ].nStep	= plongStep[i];
		DL.diag[ j ].nIntv	= plongIntv[i];
	}

	char szBuf[20];
	sprintf(staitembuff, "TimeElapse");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		str.Format( "%s", szBuf );
		DL.other.nElapseChanging = atoi( str );
	}
	
	delete [] plongCheck;
	delete [] plongMin;
	delete [] plongMax;
	delete [] plongStep;
	delete [] plongIntv;
}

void CCFGFileManager::CFGfile_InitLoggingParams( char* path, DL_Sum_Param& DL,double* &testData )
{
	char		stabuff[20];
	char		staitembuff[20];
	char		szBuf[20];
	long*		pLog	= new long[16];
	CString*	pStr	= new CString[16];
	int			nTotal	= 16;
	int			i, j=0;

	DL.log2.nCounts = 0;
	ZeroMemory( &(DL.log2.logParams), sizeof(logParam)*6*16 );
	sprintf(stabuff, "LoggingParam");
	sprintf(staitembuff, "GaugeCompr");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		DL.other.bGaugeCompr = atoi( str );
		if( DL.other.bGaugeCompr )
		{
			DL.log2.logParams[DL.log2.nCounts].nType= 2;
			DL.log2.logParams[DL.log2.nCounts].nNum	= 0;
			DL.log2.logParams[DL.log2.nCounts].nMID	= 0;
			DL.log2.logParams[DL.log2.nCounts].nCID	= 0;
			DL.log2.logParams[DL.log2.nCounts].nChn	= 0;
			DL.log2.logParams[DL.log2.nCounts].bType= 0;
			DL.log2.logParams[DL.log2.nCounts].bDiff= 0;
			DL.log2.nCounts++;
		}
	}


	// SCN
	nTotal = 16;
	sprintf(staitembuff, "SCN");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			DL.log2.logParams[DL.log2.nCounts].nType= 1;
			DL.log2.logParams[DL.log2.nCounts].nNum	= 0;
			DL.log2.logParams[DL.log2.nCounts].nMID	= USB_MID_SCN;
			DL.log2.logParams[DL.log2.nCounts].nCID	= USB_CID_SCN_ADC_Read;
			DL.log2.logParams[DL.log2.nCounts].nChn	= i;
			DL.log2.logParams[DL.log2.nCounts].bType= 1;
			DL.log2.logParams[DL.log2.nCounts].bDiff= DL.adc.chn[j*16 +i].bDiff;
			DL.log2.nCounts++;
		}
	}


	// LNS
	j++;
	nTotal = 8;
	sprintf(staitembuff, "LNS");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			DL.log2.logParams[DL.log2.nCounts].nType= 1;
			DL.log2.logParams[DL.log2.nCounts].nNum	= 1;
			DL.log2.logParams[DL.log2.nCounts].nMID	= USB_MID_LNS;
			DL.log2.logParams[DL.log2.nCounts].nCID	= USB_CID_LNS_ADC_Read;
			DL.log2.logParams[DL.log2.nCounts].nChn	= i;
			DL.log2.logParams[DL.log2.nCounts].bType= 0;
			DL.log2.logParams[DL.log2.nCounts].bDiff= DL.adc.chn[j*16 +i].bDiff;
			DL.log2.nCounts++;
		}
	}
	
	sprintf(staitembuff, "LNS-Param");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, 9, pLog);
	for( i=0; i<9; i++ )
		DL.log.bLogParam[i] = pLog[i];
	for( i=0; i<4; i++ )
		DL.log.nCheckComp[i] = 0;


	// VAC0
	j++;
	nTotal = 16;
	sprintf(staitembuff, "VAC0");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			DL.log2.logParams[DL.log2.nCounts].nType= 1;
			DL.log2.logParams[DL.log2.nCounts].nNum	= 2;
			DL.log2.logParams[DL.log2.nCounts].nMID	= USB_MID_VAC;
			DL.log2.logParams[DL.log2.nCounts].nCID	= USB_CID_VAC_ADC0_Read;
			DL.log2.logParams[DL.log2.nCounts].nChn	= i;
			DL.log2.logParams[DL.log2.nCounts].bType= 1;
			DL.log2.logParams[DL.log2.nCounts].bDiff= DL.adc.chn[j*16 +i].bDiff;
			DL.log2.nCounts++;
		}
	}

	// VAC1
	j++;
	sprintf(staitembuff, "VAC1");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			DL.log2.logParams[DL.log2.nCounts].nType= 1;
			DL.log2.logParams[DL.log2.nCounts].nNum	= 3;
			DL.log2.logParams[DL.log2.nCounts].nMID	= USB_MID_VAC;
			DL.log2.logParams[DL.log2.nCounts].nCID	= USB_CID_VAC_ADC1_Read;
			DL.log2.logParams[DL.log2.nCounts].nChn	= i;
			DL.log2.logParams[DL.log2.nCounts].bType= 1;
			DL.log2.logParams[DL.log2.nCounts].bDiff= DL.adc.chn[j*16 +i].bDiff;
			DL.log2.nCounts++;
		}
	}


	// SPR0
	j++;
	sprintf(staitembuff, "SPR0");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			DL.log2.logParams[DL.log2.nCounts].nType= 1;
			DL.log2.logParams[DL.log2.nCounts].nNum	= 4;
			DL.log2.logParams[DL.log2.nCounts].nMID	= USB_MID_SPR;
			DL.log2.logParams[DL.log2.nCounts].nCID	= USB_CID_SPR_ADC0_Read;
			DL.log2.logParams[DL.log2.nCounts].nChn	= i;
			DL.log2.logParams[DL.log2.nCounts].bType= 1;
			DL.log2.logParams[DL.log2.nCounts].bDiff= DL.adc.chn[j*16 +i].bDiff;
			DL.log2.nCounts++;
		}
	}


	// SPR1
	j++;
	sprintf(staitembuff, "SPR1");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, LONGSTYLE, nTotal, pLog);
	for( i=0; i<nTotal; i++ )
	{
		DL.log.bLog[j*16 +i] = pLog[i];
		if( pLog[i] )
		{
			DL.log2.logParams[DL.log2.nCounts].nType= 1;
			DL.log2.logParams[DL.log2.nCounts].nNum	= 5;
			DL.log2.logParams[DL.log2.nCounts].nMID	= USB_MID_SPR;
			DL.log2.logParams[DL.log2.nCounts].nCID	= USB_CID_SPR_ADC1_Read;
			DL.log2.logParams[DL.log2.nCounts].nChn	= i;
			DL.log2.logParams[DL.log2.nCounts].bType= 1;
			DL.log2.logParams[DL.log2.nCounts].bDiff= DL.adc.chn[j*16 +i].bDiff;
			DL.log2.nCounts++;
		}
	}

	sprintf(staitembuff, "AverageCounts");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		DL.adc.nAveCounts = atoi( str );
	}
	sprintf(staitembuff, "TimeElapse");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		DL.other.nElapseLogging = atoi( str );
	}
	sprintf(staitembuff, "Graph");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		DL.other.bGraph = atoi( str );
	}
	sprintf(staitembuff, "Debug");
	if(GetPrivateProfileString(stabuff, staitembuff, "", szBuf, 255, path) != 0)
	{
		CString str;
		str.Format( "%s", szBuf );
		DL.other.bDebug = atoi( str );
	}


	// 读公式
	j = 0;
	sprintf(stabuff, "CalculateParam");
	// SCN
	nTotal = 16;
	sprintf(staitembuff, "SCN");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		DL.log.strCalc[j*16 +i] = pStr[i];

	// LNS
	j++;
	nTotal = 8;
	sprintf(staitembuff, "LNS");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		DL.log.strCalc[j*16 +i] = pStr[i];

	// VAC0
	j++;
	nTotal = 16;
	sprintf(staitembuff, "VAC0");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		DL.log.strCalc[j*16 +i] = pStr[i];

	// VAC1
	j++;
	sprintf(staitembuff, "VAC1");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		DL.log.strCalc[j*16 +i] = pStr[i];

	// SPR0
	j++;
	sprintf(staitembuff, "SPR0");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		DL.log.strCalc[j*16 +i] = pStr[i];

	// SPR1
	j++;
	sprintf(staitembuff, "SPR1");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, STRSTYLE, nTotal, pStr);
	for( i=0; i<nTotal; i++ )
		DL.log.strCalc[j*16 +i] = pStr[i];

	delete [] pLog;
	delete [] pStr;


	// 13.03.31 模拟TC回读值，实验绘制TC回读曲线
	testData = NULL;
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
	/* Set pointer to beginning of file: */
	fseek( stream, 0L, SEEK_SET );
	/* Read data back from file: */
	fscanf( stream, "%s", cBuff );
	fscanf( stream, "%ld",&nLength );

	testData = new double[4*nLength];
	float fp1,fp2,fp3,fp4;
	for ( k=0; k<nLength; k++ )
	{	
		fscanf( stream, "%f%f%f%f", &fp1, &fp2, &fp3, &fp4 );
		testData[0*nLength +k] = fp1;
		testData[1*nLength +k] = fp2;
		testData[2*nLength +k] = fp3;
		testData[3*nLength +k] = fp4;
	}

	fclose( stream );
}

void CCFGFileManager::CFGfile_InitSelectedADC( char* path,DL_Sum_Param& DL )
{
	//m_nADCnum = m_nADCchn = 0;
	//m_dADCcurrentMeas = m_dADCvoltsPerAmp = m_dADCampsPerVolt = m_dADCvref = m_dADCCOMoffset = 0;
	//m_bADCdiff = m_bADCtype = FALSE;

 //   m_nADCmid	= 0x0;		// Module Selection ID
 //   m_nADCcid	= 0x0;		// Circuit Selection ID

	char stabuff[20];
	char staitembuff[20];
	double*		pdouble	= new double[16];
	CString*	pstr	= new CString[16];
	int			nTotal	= 16;
	int			i;

	// SCN ADC (only one ADC on SCN board):
	sprintf(stabuff, "ADCchannels");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SCN", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[0*16+i].strLabel = pstr[i];
	// Set channels 0/1, 2/3, 4/5, 6/7 to diff (value of x,x,1 = 2), all others to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SCN-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[0*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SCN-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[0*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "SCN-%d", i);
		if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			DL.adc.chn[0*16+i].dMin	= pdouble[0];
			DL.adc.chn[0*16+i].dMax	= pdouble[1];
		}
	}

	// LNS ADC (one LTC1859 8 channel ADC):
	sprintf(stabuff, "ADCchannels");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "LNS", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[1*16+i].strLabel = pstr[i];
	// Set all 8 channels' input modes as Single-ended:
	nTotal = 8;
	sprintf(stabuff, "ADCchanModes");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "LNS-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[1*16+i].bDiff = (pdouble[i] == 2);
	// ADC channels 0-3 are assigned to current measurements. Set scale factors according to circuit:
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "LNS-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[1*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "LNS-%d", i);
		if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			DL.adc.chn[1*16+i].dMin	= pdouble[0];
			DL.adc.chn[1*16+i].dMax	= pdouble[1];
		}
	}

	// VAC ADC#0:
	nTotal = 16;
	sprintf(stabuff, "ADCchannels");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "VAC0", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[2*16+i].strLabel = pstr[i];
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "VAC0-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[2*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "VAC0-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[2*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "VAC0-%d", i);
		if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			DL.adc.chn[2*16+i].dMin	= pdouble[0];
			DL.adc.chn[2*16+i].dMax	= pdouble[1];
		}
	}

	// VAC ADC#1:
	sprintf(stabuff, "ADCchannels");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "VAC1", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[3*16+i].strLabel = pstr[i];
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "VAC1-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[3*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "VAC1-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[3*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "VAC1-%d", i);
		if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			DL.adc.chn[3*16+i].dMin	= pdouble[0];
			DL.adc.chn[3*16+i].dMax	= pdouble[1];
		}
	}

	// SPR ADC#0:
	sprintf(stabuff, "ADCchannels");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SPR0", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[4*16+i].strLabel = pstr[i];
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SPR0-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[4*16+i].bDiff = (pdouble[i] == 2);
	// Set current measurement scale factor if applicable (non-zero value = scale factor in Volts per Amp)
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SPR0-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[4*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "SPR0-%d", i);
		if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			DL.adc.chn[4*16+i].dMin	= pdouble[0];
			DL.adc.chn[4*16+i].dMax	= pdouble[1];
		}
	}

	// SPR ADC#1:
	sprintf(stabuff, "ADCchannels");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SPR1", STRSTYLE, nTotal, pstr) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[5*16+i].strLabel = pstr[i];
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SPR1-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[5*16+i].bDiff = (pdouble[i] == 2);
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, "SPR1-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			DL.adc.chn[5*16+i].dFactor = pdouble[i];
	// Read all channels' range
	sprintf(stabuff, "ADCchanRange");
	for( i=0; i<nTotal; i++ )
	{
		sprintf(staitembuff, "SPR1-%d", i);
		if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, 2, pdouble) == 2 )
		{
			DL.adc.chn[5*16+i].dMin	= pdouble[0];
			DL.adc.chn[5*16+i].dMax	= pdouble[1];
		}
	}
	
	delete [] pdouble;
	delete [] pstr;
}



void  CCFGFileManager::CFGfile_InitSEMCtrlParams( int nPosX, int nPosY) 
{
	// 下送X参数值
	BYTE byteLo = (BYTE)( nPosX & 0x00FF );
	BYTE byteHi = (BYTE)( (nPosX >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.
	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
		USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
	// 下送Y参数值
	byteLo = (BYTE)( nPosY & 0x00FF );
	byteHi = (BYTE)( (nPosY >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.
	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
		USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
}


void CCFGFileManager::CFGfile_InitLNSParams( char* path ,int&  nDFforFFBase)
{
	// DF复用FF
	char szBuf[20];
	nDFforFFBase = 11;
	if(GetPrivateProfileString( "Options", "FFsteps", "11", szBuf, 255, path) != 0)
		nDFforFFBase = atoi( szBuf );
}

void CCFGFileManager::CFGfile_InitMCIParams( char* path )
{
	char szBuf[20], szBuf1[20];
	// 读ini中的通道
    int nChn = GetPrivateProfileInt( "Options", "Channel", 0, path );
	if( nChn < 2 )
		CControlLayer::Instance().m_BrdMCI.nDetPair = 0;
	else if( nChn < 4 )
		CControlLayer::Instance().m_BrdMCI.nDetPair = 1;
	else
		CControlLayer::Instance().m_BrdMCI.nDetPair = 2;
	if( nChn % 2 == 0 )
		CControlLayer::Instance().m_BrdMCI.bDet2nd = FALSE;
	else
		CControlLayer::Instance().m_BrdMCI.bDet2nd = TRUE;

	CString* pstrName = new CString[6];
	int i;
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "ChnNames", STRSTYLE, 6, pstrName) == 6 )
	{
		for( i=0; i<6; i++ )
			CControlLayer::Instance().m_BrdMCI.strChnName[i]	= pstrName[i];
	}
	else
	{
		CControlLayer::Instance().m_BrdMCI.strChnName[0]	= "SE";
		CControlLayer::Instance().m_BrdMCI.strChnName[1]	= "BSE";
		CControlLayer::Instance().m_BrdMCI.strChnName[2]	= "SE";
		CControlLayer::Instance().m_BrdMCI.strChnName[3]	= "SE(U)";
		CControlLayer::Instance().m_BrdMCI.strChnName[4]	= "SE";
		CControlLayer::Instance().m_BrdMCI.strChnName[5]	= "BSE";
	}
	delete [] pstrName;
	for( i=0; i<6; i++ )
		CSemVirtualMCICard::Instance().UpdateChnNames( i, CControlLayer::Instance().m_BrdMCI.strChnName[i] );

	int* pnIndex = new int[2];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "ChnIndex", INTSTYLE, 2, pnIndex) == 2 )
	{
		CControlLayer::Instance().m_BrdMCI.nChnIndex[0]	= pnIndex[0];
		CControlLayer::Instance().m_BrdMCI.nChnIndex[1]	= pnIndex[1];
	}
	else
	{
		CControlLayer::Instance().m_BrdMCI.nChnIndex[0]	= 0;
		CControlLayer::Instance().m_BrdMCI.nChnIndex[1]	= 1;
	}
	delete [] pnIndex;
	// 读ini中的滤波器
    int nFilter = GetPrivateProfileInt( "Options", "Filter", 4, path );
	CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair] = nFilter;
	// 读ini中的增益
	if(GetPrivateProfileString("Options", "Gain", "", szBuf, 15, path) != 0)
	{
		strcpy(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf1, strrchr(szBuf, ',')+1);	// szBuf1中是当前探测器组的第二通道的增益值
			*strchr(szBuf, ',') = '\0';				// szBuf中是当前探测器组的第一通道的增益值
			CControlLayer::Instance().m_BrdMCI.nGain[2 * CControlLayer::Instance().m_BrdMCI.nDetPair]		= atoi( szBuf );
			CControlLayer::Instance().m_BrdMCI.nGain[2 * CControlLayer::Instance().m_BrdMCI.nDetPair +1]	= atoi( szBuf1 );
		}
	}
}

void CCFGFileManager::CFGfile_InitAutoTestParams( char* path, AutoTest_Param& AT)
{
	// AutoTest - Power
	AT.pAutoTestPower = new double[35+8];
	AT.pAutoTestPowerExp = new double[35+8];
	AT.pAutoTestPowerErr = new double[35+8];
	AT.pPower_OutofRange = new BOOL[35+8];
	for(int i=0; i<35+8; i++ )
	{
		AT.pAutoTestPower[i] = 0.0;
		AT.pAutoTestPowerExp[i] = 0.0;
		AT.pAutoTestPowerErr[i] = 0.0;
		AT.pPower_OutofRange[i] = FALSE;
	}

	char stabuff[20], staitembuff[20];
	double*		pCheck	= new double[33+8];
	int nTotal = 33+8;
	sprintf(stabuff, "AutoTest");
	sprintf(staitembuff, "PowerExp");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, nTotal, pCheck) == nTotal )
	{
		for(int i=0; i<nTotal; i++ )
			AT.pAutoTestPowerExp[i] = pCheck[i];
	}
	sprintf(staitembuff, "PowerErr");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, DOUBLESTYLE, nTotal, pCheck) == nTotal )
	{
		for(int i=0; i<nTotal; i++ )
			AT.pAutoTestPowerErr[i] = pCheck[i];
	}
	delete [] pCheck;
}


void CCFGFileManager::CFGfile_InitFastParams( char* path, FastChanges& Fast,const DWORD dwHVtype )
{
	char szBuf[20];
	int i, nCount = 0;
	double dValue = 0.0;

	// ini中都是显示值（加显示单位），方便改；
	// 读出后都换算成要下送的整型数值存储，需要用的时候直接下送即可

	// HV
	CString* pstrHV = new CString[8];
	Fast.hv.nTotal = CCommonFunctionManager::Instance().ReadInOneLine(path, "Fast", "HV", STRSTYLE, 8, pstrHV);
	for( i=0; i< Fast.hv.nTotal; i++ )
	{
		sprintf(szBuf, "%s", pstrHV[i]);
		if( strchr(szBuf, ' ') != NULL )	// 找到空格分隔符
		{
			*strchr(szBuf, ' ') = '\0';
			dValue = atof( szBuf );
			if( dValue < 10.0 )
				Fast.hv.nFast[i] = (int)(dValue *10);
			else
				Fast.hv.nFast[i] = (int)(dValue -10 +100);
		}
	}
	delete [] pstrHV;

	// Beam
	CString* pstrBeam = new CString[8];
	Fast.beam.nTotal = CCommonFunctionManager::Instance().ReadInOneLine(path, "Fast", "Beam", STRSTYLE, 8, pstrBeam);
	for( i=0; i< Fast.beam.nTotal; i++ )
	{
		sprintf(szBuf, "%s", pstrBeam[i]);
		if( strchr(szBuf, ' ') != NULL )	// 找到空格分隔符
		{
			*strchr(szBuf, ' ') = '\0';
			Fast.beam.nFast[i] = atoi( szBuf );
		}
	}
	delete [] pstrBeam;

	// Heater
	CString* pstrFila = new CString[8];
	Fast.fila.nTotal = CCommonFunctionManager::Instance().ReadInOneLine(path, "Fast", "Fila", STRSTYLE, 8, pstrFila);
	for( i=0; i< Fast.fila.nTotal; i++ )
	{
		sprintf(szBuf, "%s", pstrFila[i]);
		if( strchr(szBuf, ' ') != NULL )	// 找到空格分隔符
		{
			*strchr(szBuf, ' ') = '\0';
			Fast.fila.nFast[i] = (int)(atof( szBuf ) *100);
		}
	}
	delete [] pstrFila;

	// Contrast
	double* pdCst = new double[8];
	Fast.cst.nTotal = CCommonFunctionManager::Instance().ReadInOneLine(path, "Fast", "Cst", DOUBLESTYLE, 8, pdCst);
	if( dwHVtype < 1 )
	{
		for( i=0; i< Fast.cst.nTotal; i++ )
			Fast.cst.nFast[i] = (int)(pdCst[i] * 255 /100);
	}
	else
	{
		for( i=0; i< Fast.cst.nTotal; i++ )
			Fast.cst.nFast[i] = (int)(pdCst[i] *4);	// *400 /100.0;
	}
	delete [] pdCst;

	// CL
	double dDisplay = (double)1.0/32.0;
	double* pdCL = new double[8];
	Fast.cl.nTotal = CCommonFunctionManager::Instance().ReadInOneLine(path, "Fast", "Cond", DOUBLESTYLE, 8, pdCL);
	for( i=0; i< Fast.cl.nTotal; i++ )
		Fast.cl.nFast[i] = (int)(pdCL[i] / dDisplay);
	delete [] pdCL;

	// Mag
	double* pdMag = new double[8];
	Fast.mag.nTotal = CCommonFunctionManager::Instance().ReadInOneLine(path, "Fast", "Mag", DOUBLESTYLE, 8, pdMag);
	for( i=0; i< Fast.mag.nTotal; i++ )
		Fast.mag.nFast[i] = (int)(pdMag[i]);
	delete [] pdMag;
}


BOOL CCFGFileManager::WriteINI( int nType, int nIndex, double* pdValue, int nIndexStage, int nIndexXY)	// 向配置文件写参数
{
	TCHAR exeFullPath[MAX_PATH];
	DWORD i, dwLength;

	dwLength = GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	for ( i=dwLength-1; i > 0; i-- )
	{
		if ( exeFullPath[i] == '\\' )
		{
			exeFullPath[i+1] = '\0';
			break;
		}
	}

	if ( dwLength == 0 )
		return FALSE;

	strcat(exeFullPath, m_strINIfileName);
	char szBuf[255];
	CString str = "";
	if( nType == INI_Ratio )
	{
		if( nIndex == INIIndex_OL )
			str = "OLmap";
		else if( nIndex == INIIndex_MagK )
		{
			str = "MagK";
			double* pdouble = new double[10];
			BOOL bRet = FALSE;
			if( CCommonFunctionManager::Instance().ReadInOneLine(exeFullPath, "Ratioes", "MagK", DOUBLESTYLE, 10, pdouble) == 10 )
			{
				if( nIndexXY ==1 )	// X rate
				{
					pdouble[2*nIndexStage]		= *pdValue;
					CSemCoreAssemble::Instance().m_Magnifier.SetParams( 2*nIndexStage, *pdValue );
				}
				else
				{
					pdouble[2*nIndexStage+1]	= *pdValue;
					CSemCoreAssemble::Instance().m_Magnifier.SetParams( 2*nIndexStage+1, *pdValue );
				}
				if( CCommonFunctionManager::Instance().SaveInOneLine(exeFullPath, "Ratioes", "MagK", DOUBLESTYLE, 10, pdouble) == 10 )
					bRet = TRUE;
			}
			delete [] pdouble;
			return bRet;
		}
		if(GetPrivateProfileString("Ratioes", str, "", szBuf, 15, exeFullPath) != 0)
		{
			*pdValue = atof( szBuf );
			return TRUE;
		}
	}
	else if( nType == INI_CaliOffset )
	{
		if( nIndex == INIIndex_ShiftX )
			str = "ShiftX";
		else if( nIndex == INIIndex_ShiftY )
			str = "ShiftY";
		else if( nIndex == INIIndex_StigX )
			str = "StigX";
		else if( nIndex == INIIndex_StigY )
			str = "StigY";
		else if( nIndex == INIIndex_AlignX )
			str = "AlignX";
		else if( nIndex == INIIndex_AlignY )
			str = "AlignY";
		if(GetPrivateProfileString("Calibrate_Offset", str, "", szBuf, 15, exeFullPath) != 0)
		{
			*pdValue = atof( szBuf );
			return TRUE;
		}
	}
	else if( nType == INI_Debug )
	{
		if( nIndex == INIIndex_Start )
			str = "Start";
		if(GetPrivateProfileString("Debug", str, "", szBuf, 15, exeFullPath) != 0)
		{
			*pdValue = atof( szBuf );
			return TRUE;
		}
	}
	else if( nType == INI_Coadjust )
	{
		if( nIndex == INIIndex_OL )
		{
			if( CCommonFunctionManager::Instance().SaveInOneLine(exeFullPath, "Coadjust", "Equation", DOUBLESTYLE, 2, pdValue) == 2 )
				return TRUE;
		}
		else if( nIndex == INIIndex_WD )
		{
			if( CCommonFunctionManager::Instance().ReadInOneLine(exeFullPath, "Coadjust", "WD", 2, 2, pdValue) == 2 )
				return TRUE;
			else
				return FALSE;
		}
	}

	return FALSE;
}


void CCFGFileManager::CFGFile_InitSemCoreParams( char * path ,const CString& m_strLogo ) 
{
	char szBuf[40] = {0};
	// 统调参数
	// 15.05.14 韩勇说在拟合新统调公式时，3200的拟合效果偏差较大
	// 所以有可能6200还使用之前的统调公式
	// 故将m_nType做调整，原来为0代表6200,1代表6900
	// 现改为：	0代表6200且使用原统调公式
	//			1代表6200且使用新统调公式(用原3200统调表，用新公式拟合出固定系数)
	//			11代表6200且使用新标高样品动态拟合新公式的系数
	//			12代表6900(使用新标高样品动态拟合新公式的系数)
	// 判断电镜型号时除原有注册表项目，暂时使用ini中的[Debug]/Start来区分6200的新老统调公式类型
	// 该ini项目原来用于物镜显示值的6X00-->3X00的映射，早已不用
	/////////////////////////////////////////////////////////////
	int nType = 0;
	int nPos = m_strLogo.Find( "6" );
	CString strType = m_strLogo.Right( m_strLogo.GetLength() - nPos );
	if( strType == "6200" )
	{
		int nLeastSquares_power = 1;
		if(GetPrivateProfileString( "Coadjust", "power", "1", szBuf, 255, path) != 0)
			nLeastSquares_power = atoi( szBuf );
		if( nLeastSquares_power < 2 )	// 直线方程
			nType	= 0;
		else
		{
			if(GetPrivateProfileString("Debug", "Start", "", szBuf, 15, path) != 0)
			{
				int nStart = atoi(szBuf);
				if( nStart < 1 )
					nType = 1;	// 使用二次多项式拟合统调公式的6200 15.07.22
				else
					nType = 11;	// 使用二次多项式动态拟合统调公式的6200 15.07.22
			}
			else
				nType	= 1;
		}
		// 		m_Stage.xy.nRangeX	= 50;
		// 		m_Stage.xy.nRangeY	= 50;
	}
	else
	{
		nType = 12;	// 使用二次多项式动态拟合统调公式的6900 15.07.22
		// 		m_Stage.xy.nRangeX	= 80;
		// 		m_Stage.xy.nRangeY	= 60;
	}
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetParams( 101, nType );
	CSemCoreAssemble::Instance().m_Magnifier.SetParams( 101, nType );
	CSemCoreAssemble::Instance().m_WD.SetParams( 101, nType );

	// 读ini中的扫描旋转度数校准值和限制值
	int nRotate = GetPrivateProfileInt( "Calibrate_Offset", "RotateLimit", 10, path );
	CSemCoreAssemble::Instance().m_Magnifier.SetParams( 51, nRotate );
	nRotate	= GetPrivateProfileInt( "Calibrate_Offset", "Rotate", 0, path );
	CSemCoreAssemble::Instance().m_ScanRotate.SetParams( 1, nRotate );
}


void CCFGFileManager::CFGFile_InitNetParams( char *path, Net_Param& m_Net) 
{
	char szBuf[40] = {0};
	if(GetPrivateProfileString( "Third", "Address", "", szBuf, 32, path) != 0)
		m_Net.LocalAddr.sin_addr.S_un.S_addr = atol( szBuf );
	else
		m_Net.LocalAddr.sin_addr.S_un.S_addr = inet_addr( "192.168.1.1" );
	m_Net.LocalAddr.sin_port = GetPrivateProfileInt( "Third", "Port", 4000, path );
}


void CCFGFileManager::InitializeColor( char * path ) 
{
	long* pnColor = new long[13];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "Colors", LONGSTYLE, 13, pnColor) == 13 )
	{
		for(int i=0; i<13; i++ )
		{
			CAppOption::Instance().m_sysOpt.color[i]	= pnColor[i];
			CSemVirtualMCICard::Instance().SetColor( i, pnColor[i] );
		}
	}
	else
	{
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrTransparent]		= g_clrUSB_Transparent;
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrBack]				= RGB(0,0,127);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrEditText]			= RGB(155,255,255);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrLabelText]			= RGB(155,255,255);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrLabelLogoText]		= RGB(155,255,255);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrMobileUScaleText]	= RGB(255,255,155);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrMobileUScaleLine]	= RGB(255,0,0);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrOscillographGrid]	= RGB(0,255,0);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrOscillographGraph]	= RGB(255,255,155);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrFrameRate]			= RGB(255,0,0);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleLine]			= RGB(255,0,0);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleArc]			= RGB(255,255,0);
		CAppOption::Instance().m_sysOpt.color[SysOpt_clrAngleText]			= RGB(155,255,255);
		for(int i=0; i<13; i++ )
			CSemVirtualMCICard::Instance().SetColor( i, CAppOption::Instance().m_sysOpt.color[i] );
	}
	delete [] pnColor;
}

void CCFGFileManager::InitializeLineWidth(char * path) 
{
	char szBuf[20];
	CAppOption::Instance().m_sysOpt.nLineWidth = 3;
	if(GetPrivateProfileString( "Options", "LineWidth", "3", szBuf, 10, path) != 0 )
		CAppOption::Instance().m_sysOpt.nLineWidth = atoi( szBuf );
	CSemVirtualMCICard::Instance().SetParams( 10, CAppOption::Instance().m_sysOpt.nLineWidth );
}

void CCFGFileManager::InitializeFontName(char * path) 
{
	CString* pstrName = new CString[3];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "FontNames", STRSTYLE, 3, pstrName) == 3 )
	{
		for(int i=0; i<3; i++ )
		{
			CAppOption::Instance().m_sysOpt.fName[i]	= pstrName[i];
			CSemVirtualMCICard::Instance().SetFontName( i, pstrName[i] );
		}
	}
	delete [] pstrName;
}

void CCFGFileManager::InitializeFontSize(char * path) 
{
	int* pnSize = new int[3];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "FontSizes", INTSTYLE, 3, pnSize) == 3 )
	{
		for(int i=0; i<3; i++ )
		{
			CAppOption::Instance().m_sysOpt.fSize[i]	= pnSize[i];
			CSemVirtualMCICard::Instance().SetFontSize( i, pnSize[i] );
		}
	}
	delete [] pnSize;
}

void CCFGFileManager::InitializeShowParameter(char * path) 
{
	// 显示参数
	CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans]	= 1;
	CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showScrMag]		= 1;
	CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showDate]		= 0;
	CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showNote]		= 0;
	CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showCCD]			= 1;
	CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showFOV]			= 1;
	CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showPixeSize]	= 1;
	int* pnShow = new int[7];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "ShowParams", INTSTYLE, 7, pnShow) == 7 )
	{
		for(int i=0; i<7; i++ )
			CAppOption::Instance().m_sysOpt.nShowParams[i]	= pnShow[i];
	}
	delete [] pnShow;
	CSemVirtualMCICard::Instance().SetTransparent( CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans] );
}

void CCFGFileManager::InitializeFooter(char * path) 
{
	// 底标参数
	if( CAppOption::Instance().ReadFooterSettings( path ) < 0 )
		CAppOption::Instance().SetFooterSettingsDefault();
	for(int i=0; i<11; i++ )
	{
		CSemVirtualMCICard::Instance().UpdateFooter( i, CAppOption::Instance().m_footerOpt.opt[i].bVisible, CAppOption::Instance().m_footerOpt.opt[i].ptPos );
		CSemVirtualMCICard::Instance().UpdateFooterText( i, CAppOption::Instance().m_footerOpt.opt[i].strDisp );
	}
}