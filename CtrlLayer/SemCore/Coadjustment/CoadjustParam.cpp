// CoadjustParam.cpp: implementation of the Coadjust Param.
// 2005-2-2_ͳ��
// Last Modify : 2007.03.19
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "CoadjustParam.h"

//���ڿ��Ʋ���ʾ���ٵ�ѹ
WORD	m_wHVCtlData[CtrlDataSizeMax_HV] = {0};
//����ϼ��ٵ�ѹ�ı仯�������ӹ�ѧϵͳ����ѹͳ����
WORD	m_wHVAdjustData[CtrlDataSizeMax_HV] = {0};

//���ڼ�����������
//WorkDistance <--> ObjectiveLens electric-current : ObjectiveLens EC Ctl
WORD	m_nObjectiveLensToWDIndex[CtrlDataSizeMax_WD];

//������ʾ��������
//WorkDistance <--> ObjectiveLens electric-current : WorkDistance ( Unit : mm )
WORD	m_nWDIndexToWD[CtrlDataSizeMax_WD];
//���ڼ������ض����������£���С�Ŵ���
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

	//======2007.03.19_3200��3900ͳ������(��ʱ)============
	// �޸ı�ʾ��2007.03.19_3200��3900ͳ������(��ʱ)
	// �޸����ڣ�2007.03.19
	// �޸���  ��J.Y.Ma
	// �޸�ԭ�򣺺ϲ������ͳ��������ʱʹ�ø���ԭ����

	// �����޸�ǰ���븱��
	/*----------------------------------------------------- 
	WORD	tmp_wObjectiveLensToWDIndex[CtrlDataSizeMax_WD];

	// 2005-2-25_ͳ��
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

	// 2005-3-22_ͳ���������
	/////////////////////////////////
	DWORD	lGrid;
	for ( i=0; i<nLength-1; i++ )
	{	
		lGrid = tmp_wObjectiveLensToWDIndex[i] + tmp_wObjectiveLensToWDIndex[i+1];
		wObjectiveLensToWDIndex[ i ]	= (WORD)(lGrid / 2.0 + 0.5);
	}
	wObjectiveLensToWDIndex[ nLength ]	= (WORD)65535;
	-----------------------------------------------------*/
	
	// �����޸ĺ����
	//-----------------------------------------------------
	if( filename == "Coadjust_WD.txt" )
	{
		WORD	tmp_wObjectiveLensToWDIndex[CtrlDataSizeMax_WD];

		// 2005-2-25_ͳ��
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

		// 2005-3-22_ͳ���������
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
	//======2007.03.19_3200��3900ͳ������(��ʱ)============

	fclose( stream );
	return TRUE;
}

