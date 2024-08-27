// Algorithm_AutoFilament.cpp : implementation file
//
// Last Modify : 2014.05.

#include "stdafx.h"
#include "Algorithm_AutoFilament.h"

int		nLoop;
int		nFound1, nFound2;	// �Ƿ��ҵ���һ���ڶ����͵�
int		n2Rise;				// �Ƿ񾭹���һ���͵��Ҿ������ͱ�խ�Ľ׶Σ��������쵽�ڶ����͵�֮ǰ�ķ��ͱ��׶�
int		n1Rise, n1Down;
int		n3Rise;
int		nReport;
int		nFitPeakX, nFitPeakY, nFitWidthCount;
int		n1Fila, n2Fila;
int		nPeakWidthCountBak;
double	dRadioBak;

long	lStep			= 10;
long	lFilaMin		= 190;	// 1.90 A (������������ļ��ж�ȡ���ڳ�ʼ��ʱ���ݸ�ģ��)
long	lFilaMax		= 270;	// 2.70 A (������������ļ��ж�ȡ���ڳ�ʼ��ʱ���ݸ�ģ��)

const int	c_nALTableLength	= 100;
ALParam		g_ALTable[ c_nALTableLength ];

/////////////////////////////////////////////////////////////////////////////
// long	lSaved_PeakWidth3	= 0;
// double	dbSaved_SumDPeakY	= 0.0;
/////////////////////////////////////////////////////////////////////////////

void Report( int nRet )
{
	if( nReport < 1 )
		return;

	// ʵ��׶����ÿһ�μ�¼��ֵ
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;
	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");

	str.Format( "%szAL%s.txt", exeFullPath, strTemp );
	HANDLE hFileAuto = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	if( hFileAuto != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		if( nRet == AutoFila_Analyze_Completed )
			str.Format( "\t%s\r\n", "AutoFilament Completed!" );
		else
			str.Format( "\t%s\r\n", "AutoFilament Failure!" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( hFileAuto, Buf, strlen(Buf), &cbRet, NULL );
		str.Format( "%s\t%s\t%s\t%s\t%s", "Count", "PeakX", "PeakY", "WidthCount", "Filament" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( hFileAuto, Buf, strlen(Buf), &cbRet, NULL );
		for( long i=0; i<=nLoop; i++ )
		{
			str.Format( "%d\t%ld\t%ld\t%ld\t%ld", i, g_ALTable[i].nPeakX, g_ALTable[i].nPeakY, g_ALTable[i].nPeakWidthCount, g_ALTable[i].nFila );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( hFileAuto, Buf, strlen(Buf), &cbRet, NULL );
		}
		if( n1Fila > lFilaMin )
		{
			str.Format( "The 1st point = %d", n1Fila );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( hFileAuto, Buf, strlen(Buf), &cbRet, NULL );
			str.Format( "The 2nd point = %d", n2Fila );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( hFileAuto, Buf, strlen(Buf), &cbRet, NULL );
		}
		else
		{
			str.Format( "Not found point" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( hFileAuto, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
	CloseHandle(hFileAuto);
}

/////////////////////////////////////////////////////////////////////////////
// AutoFilament
void WINAPI AutoFila_Initialize(
	IN	int		nRangeMin,
	IN	int		nRangeMax,
	IN	int		nCorseStep,
	IN	int		nDebug,
	OUT	long&	nFila )
{
	nLoop		= -1;
	lFilaMin	= nRangeMin;
	lFilaMax	= nRangeMax;
	lStep		= nCorseStep;
	nReport		= nDebug;

	nFound1		= 0;
	nFound2		= 0;
	n1Rise		= 0;
	n1Down		= 0;
	n2Rise		= 0;
	n3Rise		= 0;
	nFitPeakX = nFitPeakY = nFitWidthCount = 0;
	n1Fila		= lFilaMin;
	n2Fila		= lFilaMin;
	nPeakWidthCountBak	= 0;
	dRadioBak	= 0.0;

	for( int i=0; i<c_nALTableLength; i++ )
	{
		g_ALTable[i].nPeakX	= 0;
		g_ALTable[i].nPeakY	= 0;
		g_ALTable[i].nPeakWidthCount	= 0;
		g_ALTable[i].nFila		= lFilaMin;
	}
	nFila		= lFilaMin;
}

UINT WINAPI AutoFila_Analyze_GrayHistogram(
	IN	BOOL	bPause,
	IN	DWORD	dwTotalPixel, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	nFila)
{
	UINT	nRet = AutoFila_Analyze_Continue;
	if( bPause )
		return nRet;

	int		i;
	DWORD	dwPeak_Y = 0;
	DWORD	dwPeak_X = 0;
	long	lCount_PeakWidth = 0;
	DWORD	dwPeakMin = (int)(dwTotalPixel * dblAutoFila_PeakMinParam + 0.5);

	nLoop++;
	// �õ���߷�ĻҶ�ֵ���Լ��з�ĻҶȷ�Χ
	for (i = 0; i < 256; i++)
	{
		if (pGrayHistogramBuff[i] > dwPeak_Y)
		{
			dwPeak_Y = pGrayHistogramBuff[i];
			dwPeak_X = i;
		}

		if (pGrayHistogramBuff[i] > dwPeakMin)
		{
			lCount_PeakWidth ++;
		}
	}
	g_ALTable[nLoop].nPeakX	= dwPeak_X;
	g_ALTable[nLoop].nPeakY	= dwPeak_Y;
	g_ALTable[nLoop].nPeakWidthCount	= lCount_PeakWidth;
	g_ALTable[nLoop].nFila		= nFila;

	if( nLoop < 3 )		// ǰ����ֵ����������ֱ�ӷ��ؼ���
	{
		nFila += lStep;	// ����Ҫ���õĵ�˿������ֵ
		return nRet;
	}

	double dRadio = 1.0 * g_ALTable[nLoop].nPeakY / g_ALTable[0].nPeakY;
	if( n1Rise < 1 )
	{
		// ��߷�ļ���ֵ�½�����һ�����10%����Ż����ٶ�
		if( (dRadio < 0.1)
			|| (g_ALTable[nLoop].nPeakX > 85)
			|| ( (dRadio < 0.15) && (g_ALTable[nLoop].nPeakX > 75) ) )
		{
			n1Rise	= 1;
			lStep	= 2;
		}
		nFila += lStep;
	}
	else
	{
		if( n2Rise < 1 )
		{
			// ��߷�ļ���ֵ�½�����һ�����6.8%����ϸ��
			if( (dRadio < 0.068)
				|| (g_ALTable[nLoop].nPeakX > 85)
				|| ( (dRadio < 0.1) && (g_ALTable[nLoop].nPeakX > 75) ) )
			{
				n2Rise	= 1;
				lStep	= 1;
			}
			nFila += lStep;
		}
		else
		{
			if( nFound1 < 1 )
			{
				if( nFitPeakX < 1 )
				{
					// ��߷�Ҷ�ֵ>75
					if(g_ALTable[nLoop].nPeakX > 75)
						nFitPeakX = 1;
					// �����������β�ֵ��������
					else if( (g_ALTable[nLoop].nPeakX - g_ALTable[nLoop-1].nPeakX > 3)
						&& (g_ALTable[nLoop-1].nPeakX - g_ALTable[nLoop-2].nPeakX > 3) )
						nFitPeakX = 1;
				}

				if( nFitPeakX > 0 )
				{
					if(nFitWidthCount < 1)
					{
						// ������ֵ>75
						if(g_ALTable[nLoop].nPeakWidthCount > 75)
							nFitWidthCount = 1;
						// �����������β�ֵ�½�
						else if( (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-1].nPeakWidthCount <= 1)
							&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount <= 1)
							&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount <= 1) )
							nFitWidthCount = 1;
					}

					if( nFitPeakY < 1 )
					{
						// ��߷�ļ���ֵ��ֵ��+-35֮�䣬�����¼�������һ�μ���С200����
						if( (abs(g_ALTable[nLoop].nPeakY - g_ALTable[nLoop-1].nPeakY) < 35 )
							|| (g_ALTable[nLoop].nPeakY - g_ALTable[nLoop-1].nPeakY < -200) )
							nFitPeakY = 1;
					}

					if( (nFitWidthCount > 0) && (nFitPeakY > 0) )
					{
						// �ѵ���һ��ڶ����͵�
						nFound1		= 1;
						n1Fila		= nFila;
						n2Fila		= nFila;
						nPeakWidthCountBak	= g_ALTable[nLoop].nPeakWidthCount;
						dRadioBak	= dRadio;
						lStep		= 2;
						nFila		+= lStep;
//						nFila		-= 2;	//��һ��
//						nRet		= AutoFila_Analyze_Completed;
					}
					else
						nFila	+= lStep;
				}
				else
					nFila	+= lStep;
			}
			else
			{
				if( n3Rise < 1 )
				{
					
					if( (dRadio > 0.1) && (dRadio > dRadioBak *5)	// �������ֵ������������߷����ֵ��10%��˵���еڶ����͵�
						|| ( dRadio > dRadioBak *10 )				// ����ֵ��10�����ϵķ�����˵���еڶ����͵�
						|| (g_ALTable[nLoop].nPeakX > 95) )			// ��߷�Ҷ�ֵ>95
					{
						n3Rise	= 1;
						lStep	= 1;
						nFila	+= lStep;
					}
					// �����������β�ֵ��������
					else if( (g_ALTable[nLoop].nPeakX - g_ALTable[nLoop-1].nPeakX > 5)
						&& (g_ALTable[nLoop-1].nPeakX - g_ALTable[nLoop-2].nPeakX > 5) )
					{
						n3Rise	= 1;
						lStep	= 1;
						nFila	+= lStep;
					}
					// ��߷�Ҷ�ֵ>150
					else if( g_ALTable[nLoop].nPeakX > 150 )
					{
						nRet	= AutoFila_Analyze_Completed;
						n2Fila	= nFila -2;
					}
					else if( nFila +1 > lFilaMax )
					{
						// �����˿��������ֵ�ѳ������ֵ��˵��ֻ�е�һ���͵�
						nRet	= AutoFila_Analyze_Completed;
						nFila	= n1Fila;
						nFila	-= 2;	//��һ��
					}
					else
						nFila	+= lStep;
					dRadioBak	= dRadio;
				}
				else
				{
					if( (g_ALTable[nLoop].nPeakX > 150 )				// ��߷�Ҷ�ֵ>150
						|| ( g_ALTable[nLoop].nPeakWidthCount > 100 ) )	// ������ֵ>100
					{
						nRet	= AutoFila_Analyze_Completed;
						nFila	-= 2;	//��һ��
						n2Fila	= nFila;
					}
					// �����������β�ֵ�½�
					else if( (g_ALTable[nLoop].nPeakWidthCount > nPeakWidthCountBak)
						&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-1].nPeakWidthCount <= 1)
						&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount <= 1)
						&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount <= 1) )
					{
						nRet	= AutoFila_Analyze_Completed;
						nFila	-= 2;	//��һ��
						n2Fila	= nFila;
					}
					else
						nFila	+= lStep;
				}
			}
		}
	}

	if( (nRet == AutoFila_Analyze_Continue) && (nFila > lFilaMax) )
	{
		nFila	= lFilaMax -3;
		n2Fila	= nFila;
		nRet	= AutoFila_Analyze_Failure;
	}
	if( (nRet == AutoFila_Analyze_Completed) || (nRet == AutoFila_Analyze_Failure) )
		Report( nRet );
	return nRet;
}

UINT AutoFila_Analyze_GrayHistogram2(
	IN	DWORD	dwTotalPixel, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	nFila)
{
	UINT	nRet = AutoFila_Analyze_Continue;
	int		i;

	DWORD	dwPeak_Y = 0;
	DWORD	dwPeak_X = 0;
	long	lCount_PeakWidth = 0;
	DWORD	dwPeakMin = (int)(dwTotalPixel * dblAutoFila_PeakMinParam + 0.5);

	nLoop++;
	// �õ���߷�ĻҶ�ֵ���Լ��з�ĻҶȷ�Χ
	for (i = 0; i < 256; i++)
	{
		if (pGrayHistogramBuff[i] > dwPeak_Y)
		{
			dwPeak_Y = pGrayHistogramBuff[i];
			dwPeak_X = i;
		}

		if (pGrayHistogramBuff[i] > dwPeakMin)
		{
			lCount_PeakWidth ++;
		}
	}
	g_ALTable[nLoop].nPeakX	= dwPeak_X;
	g_ALTable[nLoop].nPeakY	= dwPeak_Y;
	g_ALTable[nLoop].nPeakWidthCount	= lCount_PeakWidth;
	g_ALTable[nLoop].nFila	= (int)nFila;

	if( nLoop < 3 )	// ǰ����ֵ����������ֱ�ӷ��ؼ���
	{
		nFila += lStep;	// ����Ҫ���õĵ�˿������ֵ
		return nRet;
	}

	{
		if( n1Down < 1 )
		{
			// �������β�ֵС��0�����ѹ���һ���͵㣬����Ҫ�ڶ�������
			if( (g_ALTable[nLoop].nPeakWidthCount -g_ALTable[nLoop-1].nPeakWidthCount < 0)
				&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount < 0)
				&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount < 0) )
			{
				// ���˵�һ���͵㣬���½��Ľ�Ϊƽ̹
				n1Down	= 1;
//				lStep	= 2;
			}
			nFila += lStep;
		}
		else
		{
			if( n2Rise < 1 )
			{
				// �����������ߣ�����Ϊ�ǵڶ����͵�ǰ�Ĵ������
				long l1 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-1].nPeakWidthCount;
				long l2 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount;
				long l3 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount;
				if( (l1 > l2) && (l2 > l3) && (l1 > 2) && (l2 > 2) && (l3 > 2))
				{
					// �ڶ����͵�ǰ�Ĵ��������ʼ
					n2Rise	= 1;
//					lStep	= 1;
				}
				nFila	+= lStep;
			}
			else
			{
				// �������β�ֵ�½�������Ϊ�ѵ��ڶ����͵�
				long l1 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-1].nPeakWidthCount;
				long l2 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount;
				long l3 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount;
				if( (l1 < l2) && (l2 < l3) )
				{
					// �ѵ��ڶ����͵�
					nFound2	= 1;
					nRet	= AutoFila_Analyze_Completed;
//					nFila	-= 2;
				}
				else
					nFila	+= lStep;
			}
		}
	}

	if( (nRet == AutoFila_Analyze_Continue) && (nFila > lFilaMax) )
	{
		nFila = lFilaMax -3;
		nRet = AutoFila_Analyze_Failure;
	}

	if( (nRet == AutoFila_Analyze_Completed) || (nRet == AutoFila_Analyze_Failure) )
		Report( nRet );
	return nRet;
}

UINT AutoFila_Analyze_GrayHistogram1(
	IN	DWORD	dwTotalPixel, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	nFila)
{
	UINT	nRet = AutoFila_Analyze_Continue;
	int		i;
	
	DWORD	dwPeak_Y = 0;
	DWORD	dwPeak_X = 0;
	long	lCount_PeakWidth = 0;
	DWORD	dwPeakMin = (int)(dwTotalPixel * dblAutoFila_PeakMinParam + 0.5);
	
	// �õ���߷�ĻҶ�ֵ���Լ��з�ĻҶȷ�Χ
	for (i = 0; i < 256; i++)
	{
		if (pGrayHistogramBuff[i] > dwPeak_Y)
		{
			dwPeak_Y = pGrayHistogramBuff[i];
			dwPeak_X = i;
		}
		
		if (pGrayHistogramBuff[i] > dwPeakMin)
		{
			lCount_PeakWidth ++;
		}
	}
	g_ALTable[nLoop].nPeakX	= dwPeak_X;
	g_ALTable[nLoop].nPeakY	= dwPeak_Y;
	g_ALTable[nLoop].nPeakWidthCount	= lCount_PeakWidth;
	g_ALTable[nLoop++].nFila	= nFila;

//	nFila += lStep;

	if( nLoop < 3 )	// ��һ�ڶ���ֵ����������ֱ�ӷ��ؼ���
	{
		nFila += lStep;	// ����Ҫ���õĵ�˿������ֵ
		return nRet;
	}

	if( n1Rise < 1 )
	{
		if(	g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount > 12 )
		{
			// ��һ���͵�ǰ�Ĵ��������ʼ
			n1Rise	= 1;
		}
		nFila += lStep;
	}
	else
	{
		if( n1Down < 1 )
		{
			if( ( (g_ALTable[nLoop-1].nPeakWidthCount <= g_ALTable[nLoop-2].nPeakWidthCount)
				&& (g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount > -3) )
			|| ( (g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-2].nPeakWidthCount)
				&& (g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount < 2) ) )
			{
				// ���˵�һ���͵㣬���½��Ľ�Ϊƽ̹
				n1Down	= 1;
				lStep	= 2;
			}
			nFila += lStep;
		}
		else
		{
			if( n2Rise < 1 )
			{
				if( (g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount > 2 ) )
				{
					// �ڶ����͵�ǰ�Ĵ��������ʼ
					n2Rise	= 1;
					lStep	= 1;
				}
				nFila	+= lStep;
			}
			else
			{
				if( (g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount < 1)
					&& (g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount < 1)
					&& (g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-4].nPeakWidthCount < 1) )
				{
					// �ѵ��ڶ����͵�
					nFound2	= 1;
					nRet	= AutoFila_Analyze_Completed;
					nFila	-= 2;
				}
				else
					nFila	+= lStep;
			}
		}
	}
/*	if( nFound1 < 1 )
	{
		// ��������DLL����ǰ����˿�����Ѽӵ�1.80A���Ҷ�ֱ��ͼ���з���
		if( ( (g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-2].nPeakWidthCount)
			&& (g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-3].nPeakWidthCount) )
			|| ( (abs(g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount) < 3)
				&& (abs(g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-3].nPeakWidthCount) < 3) ) )
		{
			// �����߷������ҷ��ͱ��˵��δ�����͵㣬�ɼ����ӵ�˿����
			nFila += lStep;
		}
		else
		{
			// �Ѽӵ���һ���͵㣬�����ӵ�˿����
			g_satPoint1 = g_ALTable[nLoop-1];
			nFound1	= 1;
			nFila += lStep;
		}
	}
	else
	{
		if( n2Rise < 1 )
		{
			// �ڵ�һ���͵�͵ڶ����͵�֮�䣬������һ����խ���ֱ��Ĺ���
			if( (abs(g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount) < 3)
				&& (abs(g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount) < 3) )
			{
				// ������һ���͵��Ҿ������ͱ�խ�Ľ׶Σ��������쵽�ڶ����͵�֮ǰ�ķ��ͱ��׶�
				n2Rise	= 1;
				lStep	= 1;
			}
			nFila += lStep;
		}
		else
		{
			if( nFound2 < 1 )
			{
				if( ( (g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-2].nPeakWidthCount)
					&& (g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-3].nPeakWidthCount) )
					|| ( (abs(g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount) < 3)
					&& (abs(g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-3].nPeakWidthCount) < 3) ) )
				{
					// �����߷������ҷ��ͱ��˵��δ�����͵㣬�ɼ����ӵ�˿����
					nFila += lStep;
				}
				else
				{
					// �Ѽӵ��ڶ����͵㣬�����ӵ�˿����
					g_satPoint2 = g_ALTable[nLoop-1];
					nFound2	= 1;
					nRet	= AutoFila_Analyze_Completed;
				}
			}
		}
	}
*/
	if( (nRet == AutoFila_Analyze_Continue) && (nFila > lFilaMax) )
	{
		nFila = lFilaMax -3;
		nRet = AutoFila_Analyze_Failure;
	}

	if( (nRet == AutoFila_Analyze_Completed) || (nRet == AutoFila_Analyze_Failure) )
		Report( nRet );
	return nRet;
}
