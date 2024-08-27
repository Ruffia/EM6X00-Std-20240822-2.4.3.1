// AutoBC.cpp : implementation file
//

#include "stdafx.h"
#include "AutoBC.h"

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
long  lContrastMin			= 145;
long  lContrastMax			= 185;

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
	
	lBrightness		= lSaved_Brightness;
	lContrast		= lSaved_Contrast;
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
