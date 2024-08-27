// Algorithm_AutoFilament.cpp : implementation file
//
// Last Modify : 2014.05.

#include "stdafx.h"
#include "Algorithm_AutoFilament.h"

int		nLoop;
int		nFound1, nFound2;	// 是否找到第一、第二饱和点
int		n2Rise;				// 是否经过第一饱和点且经过峰型变窄的阶段，已来到快到第二饱和点之前的峰型变宽阶段
int		n1Rise, n1Down;
int		n3Rise;
int		nReport;
int		nFitPeakX, nFitPeakY, nFitWidthCount;
int		n1Fila, n2Fila;
int		nPeakWidthCountBak;
double	dRadioBak;

long	lStep			= 10;
long	lFilaMin		= 190;	// 1.90 A (主程序从配置文件中读取，在初始化时传递给模块)
long	lFilaMax		= 270;	// 2.70 A (主程序从配置文件中读取，在初始化时传递给模块)

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

	// 实验阶段输出每一次记录的值
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
	// 得到最高峰的灰度值，以及有峰的灰度范围
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

	if( nLoop < 3 )		// 前三个值，不做处理直接返回继续
	{
		nFila += lStep;	// 更新要设置的灯丝电流新值
		return nRet;
	}

	double dRadio = 1.0 * g_ALTable[nLoop].nPeakY / g_ALTable[0].nPeakY;
	if( n1Rise < 1 )
	{
		// 最高峰的计数值下降到第一个点的10%，则放缓加速度
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
			// 最高峰的计数值下降到第一个点的6.8%，则细找
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
					// 最高峰灰度值>75
					if(g_ALTable[nLoop].nPeakX > 75)
						nFitPeakX = 1;
					// 或者连续两次差值显著上升
					else if( (g_ALTable[nLoop].nPeakX - g_ALTable[nLoop-1].nPeakX > 3)
						&& (g_ALTable[nLoop-1].nPeakX - g_ALTable[nLoop-2].nPeakX > 3) )
						nFitPeakX = 1;
				}

				if( nFitPeakX > 0 )
				{
					if(nFitWidthCount < 1)
					{
						// 峰宽计数值>75
						if(g_ALTable[nLoop].nPeakWidthCount > 75)
							nFitWidthCount = 1;
						// 或者连续三次差值下降
						else if( (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-1].nPeakWidthCount <= 1)
							&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount <= 1)
							&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount <= 1) )
							nFitWidthCount = 1;
					}

					if( nFitPeakY < 1 )
					{
						// 最高峰的计数值差值在+-35之间，或最新计数比上一次计数小200以上
						if( (abs(g_ALTable[nLoop].nPeakY - g_ALTable[nLoop-1].nPeakY) < 35 )
							|| (g_ALTable[nLoop].nPeakY - g_ALTable[nLoop-1].nPeakY < -200) )
							nFitPeakY = 1;
					}

					if( (nFitWidthCount > 0) && (nFitPeakY > 0) )
					{
						// 已到第一或第二饱和点
						nFound1		= 1;
						n1Fila		= nFila;
						n2Fila		= nFila;
						nPeakWidthCountBak	= g_ALTable[nLoop].nPeakWidthCount;
						dRadioBak	= dRadio;
						lStep		= 2;
						nFila		+= lStep;
//						nFila		-= 2;	//退一点
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
					
					if( (dRadio > 0.1) && (dRadio > dRadioBak *5)	// 如果计数值重新上升到最高峰计数值的10%，说明有第二饱和点
						|| ( dRadio > dRadioBak *10 )				// 计数值有10倍以上的反弹，说明有第二饱和点
						|| (g_ALTable[nLoop].nPeakX > 95) )			// 最高峰灰度值>95
					{
						n3Rise	= 1;
						lStep	= 1;
						nFila	+= lStep;
					}
					// 或者连续两次差值显著上升
					else if( (g_ALTable[nLoop].nPeakX - g_ALTable[nLoop-1].nPeakX > 5)
						&& (g_ALTable[nLoop-1].nPeakX - g_ALTable[nLoop-2].nPeakX > 5) )
					{
						n3Rise	= 1;
						lStep	= 1;
						nFila	+= lStep;
					}
					// 最高峰灰度值>150
					else if( g_ALTable[nLoop].nPeakX > 150 )
					{
						nRet	= AutoFila_Analyze_Completed;
						n2Fila	= nFila -2;
					}
					else if( nFila +1 > lFilaMax )
					{
						// 如果灯丝电流设置值已超过最大值，说明只有第一饱和点
						nRet	= AutoFila_Analyze_Completed;
						nFila	= n1Fila;
						nFila	-= 2;	//退一点
					}
					else
						nFila	+= lStep;
					dRadioBak	= dRadio;
				}
				else
				{
					if( (g_ALTable[nLoop].nPeakX > 150 )				// 最高峰灰度值>150
						|| ( g_ALTable[nLoop].nPeakWidthCount > 100 ) )	// 峰宽计数值>100
					{
						nRet	= AutoFila_Analyze_Completed;
						nFila	-= 2;	//退一点
						n2Fila	= nFila;
					}
					// 或者连续三次差值下降
					else if( (g_ALTable[nLoop].nPeakWidthCount > nPeakWidthCountBak)
						&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-1].nPeakWidthCount <= 1)
						&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount <= 1)
						&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount <= 1) )
					{
						nRet	= AutoFila_Analyze_Completed;
						nFila	-= 2;	//退一点
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
	// 得到最高峰的灰度值，以及有峰的灰度范围
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

	if( nLoop < 3 )	// 前三个值，不做处理直接返回继续
	{
		nFila += lStep;	// 更新要设置的灯丝电流新值
		return nRet;
	}

	{
		if( n1Down < 1 )
		{
			// 连续三次差值小于0，则已过第一饱和点，且正要第二次上升
			if( (g_ALTable[nLoop].nPeakWidthCount -g_ALTable[nLoop-1].nPeakWidthCount < 0)
				&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount < 0)
				&& (g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount < 0) )
			{
				// 过了第一饱和点，且下降的较为平坦
				n1Down	= 1;
//				lStep	= 2;
			}
			nFila += lStep;
		}
		else
		{
			if( n2Rise < 1 )
			{
				// 连续三次升高，则认为是第二饱和点前的大幅上升
				long l1 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-1].nPeakWidthCount;
				long l2 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount;
				long l3 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount;
				if( (l1 > l2) && (l2 > l3) && (l1 > 2) && (l2 > 2) && (l3 > 2))
				{
					// 第二饱和点前的大幅上升开始
					n2Rise	= 1;
//					lStep	= 1;
				}
				nFila	+= lStep;
			}
			else
			{
				// 连续三次差值下降，则认为已到第二饱和点
				long l1 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-1].nPeakWidthCount;
				long l2 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount;
				long l3 = g_ALTable[nLoop].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount;
				if( (l1 < l2) && (l2 < l3) )
				{
					// 已到第二饱和点
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
	
	// 得到最高峰的灰度值，以及有峰的灰度范围
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

	if( nLoop < 3 )	// 第一第二个值，不做处理直接返回继续
	{
		nFila += lStep;	// 更新要设置的灯丝电流新值
		return nRet;
	}

	if( n1Rise < 1 )
	{
		if(	g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount > 12 )
		{
			// 第一饱和点前的大幅上升开始
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
				// 过了第一饱和点，且下降的较为平坦
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
					// 第二饱和点前的大幅上升开始
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
					// 已到第二饱和点
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
		// 在启动本DLL功能前，灯丝电流已加到1.80A，灰度直方图已有峰型
		if( ( (g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-2].nPeakWidthCount)
			&& (g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-3].nPeakWidthCount) )
			|| ( (abs(g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount) < 3)
				&& (abs(g_ALTable[nLoop-1].nPeakWidthCount >= g_ALTable[nLoop-3].nPeakWidthCount) < 3) ) )
		{
			// 如果最高峰右移且峰型变宽，说明未到饱和点，可继续加灯丝电流
			nFila += lStep;
		}
		else
		{
			// 已加到第一饱和点，继续加灯丝电流
			g_satPoint1 = g_ALTable[nLoop-1];
			nFound1	= 1;
			nFila += lStep;
		}
	}
	else
	{
		if( n2Rise < 1 )
		{
			// 在第一饱和点和第二饱和点之间，峰型有一个变窄后又变宽的过程
			if( (abs(g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-2].nPeakWidthCount) < 3)
				&& (abs(g_ALTable[nLoop-1].nPeakWidthCount - g_ALTable[nLoop-3].nPeakWidthCount) < 3) )
			{
				// 经过第一饱和点且经过峰型变窄的阶段，已来到快到第二饱和点之前的峰型变宽阶段
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
					// 如果最高峰右移且峰型变宽，说明未到饱和点，可继续加灯丝电流
					nFila += lStep;
				}
				else
				{
					// 已加到第二饱和点，结束加灯丝电流
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
