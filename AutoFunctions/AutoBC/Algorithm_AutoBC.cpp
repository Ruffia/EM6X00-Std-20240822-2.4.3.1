// Algorithm_AutoBC.cpp : implementation file
//
// Last Modify : 2011.01.28

#include "stdafx.h"
#include "Algorithm_AutoBC.h"

/////////////////////////////////////////////////////////////////////////////
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理
/////////////////////////////////////////////////////////////////////////////

long  lAutoBC_Loop	= 0;

long  lOld_Peak_Y	= 0;
long  lOld_Peak_X	= 0;
long  lOld_Count_PeakWidth	= 0;
long  lOld_Brightness		= 0;
long  lOld_Contrast			= 0;

long  lSaved_Brightness		= 0;
long  lSaved_Contrast		= 0;

long  lBrightnessStep		= 64;

long  lContrastStep			= 10;
long  lContrastMin			= 82;	//145;
long  lContrastMax			= 205;	//185;

/////////////////////////////////////////////////////////////////////////////
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理
BOOL	bJudgeContrast		= FALSE;
long	lBCStep				= 50;
long	lSaved_PeakWidth3	= 0;
double	dbSaved_SumDPeakY	= 0.0;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// AutoBC

void WINAPI AutoBC_Initialize(
	OUT	long&	lBrightness, 
	OUT	long&	lContrast)
{
	lOld_Peak_Y				= 0;
	lOld_Peak_X				= 0;
	
	lOld_Count_PeakWidth	= 0;
	lOld_Brightness			= 0;
	lOld_Contrast			= 0;
	
	lAutoBC_Loop		= 0;
	lBrightnessStep		= 64;
	lContrastStep		= 10;
	lSaved_Brightness	= -2;											// start bright value is -2
	lSaved_Contrast		= lContrastMin + lContrastStep * lAutoBC_Loop;	// start contrast value is lContrastMin
	
	lBrightness			= lSaved_Brightness;
	lContrast			= lSaved_Contrast;
}

UINT WINAPI AutoBC_Analyze_GrayHistogram(
	IN	DWORD	dwTotalPiexl, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	lBrightness, 
	OUT	long&	lContrast)
{
	UINT	nRet = AutoBC_Analyze_Continue;
	int		i;
	
	DWORD	dwPeak_Y = 0;
	DWORD	dwPeak_X = 0;
	long	lCount_PeakWidth = 0;
	DWORD	dwPeakMin = (int)(dwTotalPiexl * dblPeakMinParam + 0.5);
	
	// to get the peak of the histogram and Count_Width
	for (i = 0; i < 256; i++)
	{
		if (pGrayHistogramBuff[i] > dwPeak_Y)
		{
			dwPeak_Y = pGrayHistogramBuff[i];
			dwPeak_X = i;
		}
		
		if (pGrayHistogramBuff[i] > dwPeakMin)	// 250
		{
			lCount_PeakWidth ++;
		}
	}
	
	// to get the new brightness
	if ((60 <= dwPeak_X) &&(dwPeak_X <= 120))
	{
		if (lCount_PeakWidth >= lOld_Count_PeakWidth)
		{
			lOld_Peak_Y	= dwPeak_Y;
			lOld_Peak_X	= dwPeak_X;
			lOld_Count_PeakWidth	= lCount_PeakWidth;
			
			lOld_Brightness			= lSaved_Brightness;
			lOld_Contrast			= lSaved_Contrast;
		}
		lBrightnessStep = 0;
	}
	else if (dwPeak_X < 60)
	{
		if (lBrightnessStep <= 1)
		{
			lBrightnessStep = 0;
		}
		else
		{
			lBrightnessStep /= 2;
			lSaved_Brightness += lBrightnessStep;
		}
	}
	else if (120 < dwPeak_X) 
	{
		if (lBrightnessStep <= 1)
		{
			lBrightnessStep = 0;
		}
		else
		{
			lBrightnessStep /= 2;
			lSaved_Brightness -= lBrightnessStep;
		}
	}
	
	// to get the new contrast
	if (lBrightnessStep <= 0)
	{
		lAutoBC_Loop ++;
		
		lBrightnessStep		= 64;
		lSaved_Brightness	= -2;											// start bright value is -2
		lSaved_Contrast		= lContrastMin + lContrastStep * lAutoBC_Loop;	// start contrast value is lContrastMin
	}
	
	if ((lContrastMin + lContrastStep * lAutoBC_Loop) > lContrastMax)
	{
		if (lOld_Count_PeakWidth > 0)
		{
			lSaved_Brightness	= lOld_Brightness;
			lSaved_Contrast		= lOld_Contrast;
			
			nRet		= AutoBC_Analyze_Completed;
		}
		else
		{
			nRet		= AutoBC_Analyze_Failure;
		}
	}
	
	lBrightness			= lSaved_Brightness;
	lContrast			= lSaved_Contrast;
	
	return nRet;
}

///////////////////////////////////////////////////////////////////////////////
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理
/*
//zyx add 10.10.10
UINT WINAPI AutoBC_Analyze_Image(
	IN	DWORD	dwTotalPiexl, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	lPeakX, 
	OUT	long&	lPeakX2, 
	OUT	double&	dbSumDPeakY,
	OUT long&	lPeakWidth1,
	OUT long&	lPeakWidth2,
	OUT long&	lPeakWidth3)
{
	UINT	nRet = AutoBC_Analyze_Continue;
	int		i;
	
	DWORD	dwPeak_Y = 0;
//	DWORD	dwSum = 0;
	DWORD	dwPeak_Y2 = 0;
	DWORD	dwPeak_X = 0;
	DWORD	dwPeak_X2 = -1;
	long	lCount_PeakWidth1 = 0;
	long	lCount_PeakWidth2 = 0;
	long	lCount_PeakWidth3 = 0;

	DWORD	dwPeakMin = (int)(dwTotalPiexl * dblPeakMinParam + 0.5);
	
	// to get the peak of the histogram and Count_Width
	for (i = 0; i < 256; i++)
	{
		if (pGrayHistogramBuff[i] > dwPeak_Y)
		{
			dwPeak_Y = pGrayHistogramBuff[i];
			dwPeak_X = i;
		}
		if((pGrayHistogramBuff[i] > dwPeak_Y2) && (i > 0) && (i < 255))
		{
			dwPeak_Y2 = pGrayHistogramBuff[i];
			dwPeak_X2 = i;
		}
		
		if (pGrayHistogramBuff[i] >= 1)
		{
			lCount_PeakWidth1 ++;
		}
		if (pGrayHistogramBuff[i] > dwPeakMin)
		{
			lCount_PeakWidth2 ++;
		}
		if ((pGrayHistogramBuff[i] > dwPeakMin) && (i >= 60) && (i <= 120))
		{
			lCount_PeakWidth3 ++;
		}
//		dwSum= dwSum +pGrayHistogramBuff[i];

	}
	lPeakX		= dwPeak_X;
	lPeakX2		= dwPeak_X2;
	dbSumDPeakY	= (double)dwTotalPiexl /dwPeak_Y;
	lPeakWidth1	= lCount_PeakWidth1;
	lPeakWidth2	= lCount_PeakWidth2;
	lPeakWidth3	= lCount_PeakWidth3;

	return nRet;
}
// zyx add 10.10.10
*/
void WINAPI AutoBC_Initialize2(
	OUT	long&	lBrightness, 
	OUT	long&	lContrast)
{
	lOld_Peak_Y				= 0;
	lOld_Peak_X				= 0;
	
	lOld_Count_PeakWidth	= 0;
	lOld_Brightness			= 0;
	lOld_Contrast			= 0;
	
	lAutoBC_Loop			= 0;
	lBrightnessStep			= 50;
	lContrastStep			= 50;
	lSaved_Brightness		= -20;				// start bright value is -20
/*	if( (lBrightness < -30) || (lBrightness > 30) )
		lSaved_Brightness	= -30;				// start bright value is -20
	else
		lSaved_Brightness	= lBrightness;		// start bright value is current brightness
*/	lSaved_Contrast			= 10;				// start contrast value is 10

	bJudgeContrast			= FALSE;
	lBCStep					= 50;
	lSaved_PeakWidth3		= 0;
	dbSaved_SumDPeakY		= 0.0;
	
	lBrightness				= lSaved_Brightness;
	lContrast				= lSaved_Contrast;
}

UINT WINAPI AutoBC_Analyze_Image2(
	IN	DWORD	dwTotalPiexl, 
	IN	DWORD	*pGrayHistogramBuff,
	OUT	long&	lBrightness,
	OUT	long&	lContrast)
{
	UINT	nRet = AutoBC_Analyze_Continue;
	int		i;
	
	DWORD	dwPeak_X			= 0;
	DWORD	dwPeak_X2			= -1;
	DWORD	dwPeak_Y			= 0;
	DWORD	dwPeak_Y2			= 0;
	long	lCount_PeakWidth1	= 0;
	long	lCount_PeakWidth2	= 0;
	long	lCount_PeakWidth3	= 0;
	double	dbSumDPeakY			= 0.0;

	DWORD	dwPeakMin = (int)(dwTotalPiexl * dblPeakMinParam + 0.5);
	
	// to get the peak of the histogram and Count_Width
	for (i = 0; i < 256; i++)
	{
		if (pGrayHistogramBuff[i] > dwPeak_Y)
		{
			// 最大计数值，及对应的灰度值
			dwPeak_Y = pGrayHistogramBuff[i];
			dwPeak_X = i;
		}
		if((pGrayHistogramBuff[i] > dwPeak_Y2) && (i > 0) && (i < 255))
		{
			// 除掉最黑点0和最亮点255以外的最大计数值，及对应的灰度值
			dwPeak_Y2 = pGrayHistogramBuff[i];
			dwPeak_X2 = i;
		}
		if (pGrayHistogramBuff[i] >= 1)
		{	
			// 只要大于1个计数的灰度值个数
			lCount_PeakWidth1++;
		}
		if (pGrayHistogramBuff[i] > dwPeakMin)
		{	
			// 大于峰高最小计数值的灰度值个数
			lCount_PeakWidth2++;
		}
		if ((pGrayHistogramBuff[i] > dwPeakMin) && (i >= 60) && (i <= 120))
		{	
			// 大于峰高最小计数值，且灰度值在60-120之间的，灰度值个数
			lCount_PeakWidth3++;
		}
	}
	dbSumDPeakY = (double)dwTotalPiexl /dwPeak_Y;

	lBrightness	= lSaved_Brightness;
	lContrast	= lSaved_Contrast;

	// 判断亮度区间
	if( !bJudgeContrast )
	{
		if( (lCount_PeakWidth1 <= 10) && (dbSumDPeakY >= 1) && (dbSumDPeakY < 3.1) )
		{
			// 可以判断为直方图数据在C低点直方图
			// 因为C初始值为10，所以计数应该很少
			// 如果在C值很低的情况下能进入该判断项，说明峰型还可以，可以先调节亮度
			dbSaved_SumDPeakY	= dbSumDPeakY;
			lSaved_PeakWidth3	= lCount_PeakWidth3;

			if( (dwPeak_X >= 22 ) && (dwPeak_X <= 89) )//10
			{
				// 灰度值分布较好，直接进入判断对比度空间
				bJudgeContrast = TRUE;
			}
			else if( (dwPeak_X > 89) && (dwPeak_X < 255) )
			{
				// 灰度值偏高，图像较亮，调节B/C
				lBrightness	= lSaved_Brightness -(dwPeak_X -52) /2.93;//-2
				lContrast	= 0;
			}
			else if( (dwPeak_X >= 1) && (dwPeak_X < 22) )
			{
				// 灰度值较低，图像较暗，调节B/C
				lBrightness	= lSaved_Brightness +(52 -dwPeak_X) /2.93;//-12;
				lContrast	= 0;
			}
			else if( dwPeak_X == 0 )
			{
				// 有最高峰的灰度值是0，太暗，调节B/C
				lBrightness	= lSaved_Brightness +10;
				lContrast	= 0;
			}
			else
			{
				// 有最高峰的灰度值是255，太亮，调节B/C
				lBrightness = lSaved_Brightness -10;
				lContrast	= 0;
			}
		}
		else
		{
			bJudgeContrast = TRUE;	//调试用
		}
	}

	// 判断对比度区间
	if( bJudgeContrast )
	{
		if( (dbSumDPeakY >= 1 ) && (dbSumDPeakY < 3.1) )
		{
			// 峰型较好
			if( dwPeak_X < 120 )
			{
				// 最高峰灰度值小于120
				dbSaved_SumDPeakY	= dbSumDPeakY;
				lSaved_PeakWidth3	= lCount_PeakWidth3;

				if( (lSaved_Contrast +lBCStep) >= lContrastMax )
				{
					// C调节范围超出时，调节B，并重设C初始值
					lBrightness	= lSaved_Brightness +3;		//C ->0
					lContrast	= 0;
					lBCStep		= 20;
				}
				else
				{
					// 按步距增加C值
					lContrast	= lSaved_Contrast +lBCStep;
				}
			}
			else
			{
				// 最高峰灰度值在120以上，较亮
				if( lBCStep <= 5 )//10 )
				{
					// 如果步距已经为10以下，说明调节无效，失败退出
					nRet = AutoBC_Analyze_Failure;
				}
				else
				{
					// 回到上一个C值，减小步距，重设新的C值
					lContrast	= lSaved_Contrast -lBCStep;
					lBCStep		= 5;	//10;
					lContrast	= lSaved_Contrast +lBCStep;	//回到上一个值后+10
				}
			}
		}
		else if( dwPeak_X < 60 )//( lPWidth1 > 10 ) && ( lPWidth1 != 0 )
		{
			// 最高峰灰度值小于60，较暗
			// 减小步距
			lBCStep = 10;	//20
			if( (lSaved_Contrast +lBCStep) >= lContrastMax )
			{
				// C调节范围超出时，调节B
				lBrightness = lSaved_Brightness +3;
				lContrast	= lContrastMin;
			}
			else
			{
				// 按步距增加C值
				lContrast	= lSaved_Contrast +lBCStep;
			}

			dbSaved_SumDPeakY	= dbSumDPeakY;
			lSaved_PeakWidth3	= lCount_PeakWidth3;
		}
		else if( lCount_PeakWidth3 >= lSaved_PeakWidth3 )
		{
			// 峰比之前更高
			// 减小步距
			lBCStep = 3;
			if( lSaved_Contrast +lBCStep >= lContrastMax )
			{
				// C调节范围超出时，调节B
				lBrightness = lSaved_Brightness +3;
				lContrast	= lContrastMin;
			}
			else
			{
				// 按步距增加C值
				lContrast = lSaved_Contrast +lBCStep;
			}

			dbSaved_SumDPeakY	= dbSumDPeakY;
			lSaved_PeakWidth3	= lCount_PeakWidth3;
		}
		else
		{
			if( dbSaved_SumDPeakY < 3.1 )
			{
				// 说明当前步距大，跳变快，应减小步距
			    if( lBCStep <= 5 )//10 )
				{
					// 如果步距已经为10以下，说明调节无效，失败退出
					nRet = AutoBC_Analyze_Failure;
				}
				else
				{
					// 回到上一个C值，减小步距，重设新的C值
					lContrast	= lSaved_Contrast -lBCStep;
					lBCStep		= 5;	//10;
					lContrast	= lSaved_Contrast +lBCStep;	//回到上一个值后+10
				}
			}
			else
			{
				// 上次的BC值就是最佳BC值，成功退出
				nRet = AutoBC_Analyze_Completed;
			}
		}
	}

	if( lAutoBC_Loop > 99 )		// 次数超过100次，失败退出
		nRet = AutoBC_Analyze_Failure;

	lSaved_Brightness	= lBrightness;
	lSaved_Contrast		= lContrast;
	lAutoBC_Loop++;

	return nRet;
}
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理
///////////////////////////////////////////////////////////////////////////////

void WINAPI AutoBC_Initialize3(
	OUT	long&	lBrightness, 
	OUT	long&	lContrast)
{
	lOld_Peak_Y				= 0;
	lOld_Peak_X				= 0;

	lOld_Count_PeakWidth	= 0;
	lOld_Brightness			= 0;
	lOld_Contrast			= 0;

	lAutoBC_Loop			= 0;
	lBrightnessStep			= 20;	//50;
	lContrastStep			= 20;	//50;
	lSaved_Brightness		= lBrightness;		// start bright value is -20
/*	if( (lBrightness < -30) || (lBrightness > 30) )
		lSaved_Brightness	= -30;				// start bright value is -20
	else
		lSaved_Brightness	= lBrightness;		// start bright value is current brightness
*/	lSaved_Contrast			= lContrast;		// start contrast value is 10

	bJudgeContrast			= FALSE;
	lBCStep					= 20;	//50;
	lSaved_PeakWidth3		= 0;
	dbSaved_SumDPeakY		= 0.0;

	lBrightness				= lSaved_Brightness;
	lContrast				= lSaved_Contrast;
}
