// CoadjustParam.cpp: implementation of the Coadjust Param.
// 2005-2-2_统调
// Last Modify : 2007.03.19
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "CoadjustParam.h"

//用于控制并显示加速电压
WORD	m_wHVCtlData[CtrlDataSizeMax_HV] = {0};
//用配合加速电压的变化调整电子光学系统（高压统调）
WORD	m_wHVAdjustData[CtrlDataSizeMax_HV] = {0};

//用于检索工作距离
//WorkDistance <--> ObjectiveLens electric-current : ObjectiveLens EC Ctl
WORD	m_nObjectiveLensToWDIndex[CtrlDataSizeMax_WD];

//用于显示工作距离
//WorkDistance <--> ObjectiveLens electric-current : WorkDistance ( Unit : mm )
WORD	m_nWDIndexToWD[CtrlDataSizeMax_WD];
//用于检索（特定工作距离下）最小放大倍数
double	m_dfWDIndexToMAGmin[CtrlDataSizeMax_WD];

////////////////////////////////////////////////////////////////////

bool GetModuleFilePath(LPTSTR exeFullPath)
{
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

	return TRUE;
}

DWORD LoadSemHVAdjustParam(const CString& filename, 
	const WORD nLengthMax,
	WORD wHVData[], WORD wHVAdjustData[] )
{
	int nLength = 0;
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFilePath( exeFullPath );
	CString csFullPath = exeFullPath + filename;

	FILE *stream = fopen( (LPCSTR)csFullPath, "r" );
	if( stream == NULL )
		return -1;
		//return FALSE;

	long	i = 0, nIndex = 0, nHV=0, nAdjust=0;
	char	cBuff[256];
	/* Set pointer to beginning of file: */
	fseek( stream, 0L, SEEK_SET );
	/* Read data back from file: */
	fscanf( stream, "%s", cBuff );
	fscanf( stream, "%s", cBuff );
	fscanf( stream, "%s", cBuff );

	/////////////////////////////////
	fscanf( stream, "%s", cBuff );
	fscanf( stream, "%d",&nLength );
	fscanf( stream, "%s", cBuff );
	if ( nLength > nLengthMax )
		nLength = nLengthMax;
	/////////////////////////////////
	
	/////////////////////////////////
	for ( i=0; i<nLength; i++ )
	{	
		fscanf( stream, "%ld %ld %ld",&nIndex, &nHV, &nAdjust );
		wHVData[nIndex-1]			= (WORD)nHV;
		wHVAdjustData[nIndex-1]		= (WORD)nAdjust;
	}
	/////////////////////////////////
	
	fclose( stream );
	return nLength;
	//return TRUE;
}

bool LoadSemWDAdjustParam(const CString& filename, 
	const WORD nLengthMax, WORD &nLength,
	WORD wObjectiveLensToWDIndex[], WORD wWDIndexToWD[], double	dfWDIndexToMAGmin[] )
{
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFilePath( exeFullPath );
	CString csFullPath = exeFullPath + filename;

	FILE *stream;
	stream = fopen( (LPCSTR)csFullPath, "r" );
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
	
	/*
	/////////////////////////////////
	for ( i=0; i<nLength; i++ )
	{	
		fscanf( stream, "%ld %ld %ld %lf",
			&nIndex, &nFL2WDIndex, &nWDIndex2WD, &dfWDIndex2MAGmin );
		wObjectiveLensToWDIndex[nIndex-1]	= (WORD)nFL2WDIndex;
		wWDIndexToWD[nIndex-1]			= (WORD)nWDIndex2WD;
		dfWDIndexToMAGmin[nIndex-1]		= dfWDIndex2MAGmin;
	}
	/////////////////////////////////
	*/

	//======2007.03.19_3200与3900统调兼容(暂时)============
	// 修改标示：2007.03.19_3200与3900统调兼容(暂时)
	// 修改日期：2007.03.19
	// 修改人  ：J.Y.Ma
	// 修改原因：合并程序后，统调部分暂时使用各自原代码

	// 本次修改前代码副本
	/*----------------------------------------------------- 
	WORD	tmp_wObjectiveLensToWDIndex[CtrlDataSizeMax_WD];

	// 2005-2-25_统调
	/////////////////////////////////
	for ( i=0; i<nLength; i++ )
	{	
		fscanf( stream, "%ld %lf %ld %lf",
			&nIndex, &dfFL2WDIndex, &nWDIndex2WD, &dfWDIndex2MAGmin );
		nFL2WDIndex	= (int)( dfFL2WDIndex * 64 + 0.5 );
			if (nFL2WDIndex < 0)			nFL2WDIndex = 0;
			else if (nFL2WDIndex > 65535)	nFL2WDIndex = 65535;

		tmp_wObjectiveLensToWDIndex[nIndex-1]	= (WORD)nFL2WDIndex;
		wWDIndexToWD[nIndex-1]				= (WORD)nWDIndex2WD;
		dfWDIndexToMAGmin[nIndex-1]			= dfWDIndex2MAGmin;
	}
	/////////////////////////////////

	// 2005-3-22_统调参数差分
	/////////////////////////////////
	DWORD	lGrid;
	for ( i=0; i<nLength-1; i++ )
	{	
		lGrid = tmp_wObjectiveLensToWDIndex[i] + tmp_wObjectiveLensToWDIndex[i+1];
		wObjectiveLensToWDIndex[ i ]	= (WORD)(lGrid / 2.0 + 0.5);
	}
	wObjectiveLensToWDIndex[ nLength ]	= (WORD)65535;
	-----------------------------------------------------*/
	
	// 本次修改后代码
	//-----------------------------------------------------
	if( filename == "Coadjust_WD.txt" )
	{
		WORD	tmp_wObjectiveLensToWDIndex[CtrlDataSizeMax_WD];

		// 2005-2-25_统调
		/////////////////////////////////
		for ( i=0; i<nLength; i++ )
		{	
			fscanf( stream, "%ld %lf %ld %lf",
				&nIndex, &dfFL2WDIndex, &nWDIndex2WD, &dfWDIndex2MAGmin );
			nFL2WDIndex	= (int)( dfFL2WDIndex * 64 + 0.5 );
				if (nFL2WDIndex < 0)			nFL2WDIndex = 0;
				else if (nFL2WDIndex > 65535)	nFL2WDIndex = 65535;

			tmp_wObjectiveLensToWDIndex[nIndex-1]	= (WORD)nFL2WDIndex;
			wWDIndexToWD[nIndex-1]				= (WORD)nWDIndex2WD;
			dfWDIndexToMAGmin[nIndex-1]			= dfWDIndex2MAGmin;
		}
		/////////////////////////////////

		// 2005-3-22_统调参数差分
		/////////////////////////////////
		DWORD	lGrid;
		for ( i=0; i<nLength-1; i++ )
		{	
			lGrid = tmp_wObjectiveLensToWDIndex[i] + tmp_wObjectiveLensToWDIndex[i+1];
			wObjectiveLensToWDIndex[ i ]	= (WORD)(lGrid / 2.0 + 0.5);
		}
		wObjectiveLensToWDIndex[ nLength ]	= (WORD)65535;
		/////////////////////////////////
	}
	else
	{
		for ( i=0; i<nLength; i++ )
		{	
			fscanf( stream, "%ld %lf %ld %lf",
				&nIndex, &dfFL2WDIndex, &nWDIndex2WD, &dfWDIndex2MAGmin );
			nFL2WDIndex	= (int)( dfFL2WDIndex * 64 + 0.5 );
				if (nFL2WDIndex < 0)			nFL2WDIndex = 0;
				else if (nFL2WDIndex > 65535)	nFL2WDIndex = 65535;

			wObjectiveLensToWDIndex[nIndex-1]	= (WORD)nFL2WDIndex;
			wWDIndexToWD[nIndex-1]			= (WORD)nWDIndex2WD;
			dfWDIndexToMAGmin[nIndex-1]		= dfWDIndex2MAGmin;
		}
		wObjectiveLensToWDIndex[ nLength ]	= (WORD)65535;
	}
	//-----------------------------------------------------
	//======2007.03.19_3200与3900统调兼容(暂时)============

	fclose( stream );
	return TRUE;
}

