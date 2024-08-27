// Algorithm_AutoBC.cpp : implementation file
//
// Last Modify : 2011.01.28

#include "stdafx.h"
#include "Algorithm_AutoBC.h"

/////////////////////////////////////////////////////////////////////////////
// 11.01.28 �����Զ����ȶԱȶȹ��ܣ�ZYX�޸ģ�MJY����
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
// 11.01.28 �����Զ����ȶԱȶȹ��ܣ�ZYX�޸ģ�MJY����
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
// 11.01.28 �����Զ����ȶԱȶȹ��ܣ�ZYX�޸ģ�MJY����
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
			// ������ֵ������Ӧ�ĻҶ�ֵ
			dwPeak_Y = pGrayHistogramBuff[i];
			dwPeak_X = i;
		}
		if((pGrayHistogramBuff[i] > dwPeak_Y2) && (i > 0) && (i < 255))
		{
			// ������ڵ�0��������255�����������ֵ������Ӧ�ĻҶ�ֵ
			dwPeak_Y2 = pGrayHistogramBuff[i];
			dwPeak_X2 = i;
		}
		if (pGrayHistogramBuff[i] >= 1)
		{	
			// ֻҪ����1�������ĻҶ�ֵ����
			lCount_PeakWidth1++;
		}
		if (pGrayHistogramBuff[i] > dwPeakMin)
		{	
			// ���ڷ����С����ֵ�ĻҶ�ֵ����
			lCount_PeakWidth2++;
		}
		if ((pGrayHistogramBuff[i] > dwPeakMin) && (i >= 60) && (i <= 120))
		{	
			// ���ڷ����С����ֵ���һҶ�ֵ��60-120֮��ģ��Ҷ�ֵ����
			lCount_PeakWidth3++;
		}
	}
	dbSumDPeakY = (double)dwTotalPiexl /dwPeak_Y;

	lBrightness	= lSaved_Brightness;
	lContrast	= lSaved_Contrast;

	// �ж���������
	if( !bJudgeContrast )
	{
		if( (lCount_PeakWidth1 <= 10) && (dbSumDPeakY >= 1) && (dbSumDPeakY < 3.1) )
		{
			// �����ж�Ϊֱ��ͼ������C�͵�ֱ��ͼ
			// ��ΪC��ʼֵΪ10�����Լ���Ӧ�ú���
			// �����Cֵ�ܵ͵�������ܽ�����ж��˵�����ͻ����ԣ������ȵ�������
			dbSaved_SumDPeakY	= dbSumDPeakY;
			lSaved_PeakWidth3	= lCount_PeakWidth3;

			if( (dwPeak_X >= 22 ) && (dwPeak_X <= 89) )//10
			{
				// �Ҷ�ֵ�ֲ��Ϻã�ֱ�ӽ����ж϶Աȶȿռ�
				bJudgeContrast = TRUE;
			}
			else if( (dwPeak_X > 89) && (dwPeak_X < 255) )
			{
				// �Ҷ�ֵƫ�ߣ�ͼ�����������B/C
				lBrightness	= lSaved_Brightness -(dwPeak_X -52) /2.93;//-2
				lContrast	= 0;
			}
			else if( (dwPeak_X >= 1) && (dwPeak_X < 22) )
			{
				// �Ҷ�ֵ�ϵͣ�ͼ��ϰ�������B/C
				lBrightness	= lSaved_Brightness +(52 -dwPeak_X) /2.93;//-12;
				lContrast	= 0;
			}
			else if( dwPeak_X == 0 )
			{
				// ����߷�ĻҶ�ֵ��0��̫��������B/C
				lBrightness	= lSaved_Brightness +10;
				lContrast	= 0;
			}
			else
			{
				// ����߷�ĻҶ�ֵ��255��̫��������B/C
				lBrightness = lSaved_Brightness -10;
				lContrast	= 0;
			}
		}
		else
		{
			bJudgeContrast = TRUE;	//������
		}
	}

	// �ж϶Աȶ�����
	if( bJudgeContrast )
	{
		if( (dbSumDPeakY >= 1 ) && (dbSumDPeakY < 3.1) )
		{
			// ���ͽϺ�
			if( dwPeak_X < 120 )
			{
				// ��߷�Ҷ�ֵС��120
				dbSaved_SumDPeakY	= dbSumDPeakY;
				lSaved_PeakWidth3	= lCount_PeakWidth3;

				if( (lSaved_Contrast +lBCStep) >= lContrastMax )
				{
					// C���ڷ�Χ����ʱ������B��������C��ʼֵ
					lBrightness	= lSaved_Brightness +3;		//C ->0
					lContrast	= 0;
					lBCStep		= 20;
				}
				else
				{
					// ����������Cֵ
					lContrast	= lSaved_Contrast +lBCStep;
				}
			}
			else
			{
				// ��߷�Ҷ�ֵ��120���ϣ�����
				if( lBCStep <= 5 )//10 )
				{
					// ��������Ѿ�Ϊ10���£�˵��������Ч��ʧ���˳�
					nRet = AutoBC_Analyze_Failure;
				}
				else
				{
					// �ص���һ��Cֵ����С���࣬�����µ�Cֵ
					lContrast	= lSaved_Contrast -lBCStep;
					lBCStep		= 5;	//10;
					lContrast	= lSaved_Contrast +lBCStep;	//�ص���һ��ֵ��+10
				}
			}
		}
		else if( dwPeak_X < 60 )//( lPWidth1 > 10 ) && ( lPWidth1 != 0 )
		{
			// ��߷�Ҷ�ֵС��60���ϰ�
			// ��С����
			lBCStep = 10;	//20
			if( (lSaved_Contrast +lBCStep) >= lContrastMax )
			{
				// C���ڷ�Χ����ʱ������B
				lBrightness = lSaved_Brightness +3;
				lContrast	= lContrastMin;
			}
			else
			{
				// ����������Cֵ
				lContrast	= lSaved_Contrast +lBCStep;
			}

			dbSaved_SumDPeakY	= dbSumDPeakY;
			lSaved_PeakWidth3	= lCount_PeakWidth3;
		}
		else if( lCount_PeakWidth3 >= lSaved_PeakWidth3 )
		{
			// ���֮ǰ����
			// ��С����
			lBCStep = 3;
			if( lSaved_Contrast +lBCStep >= lContrastMax )
			{
				// C���ڷ�Χ����ʱ������B
				lBrightness = lSaved_Brightness +3;
				lContrast	= lContrastMin;
			}
			else
			{
				// ����������Cֵ
				lContrast = lSaved_Contrast +lBCStep;
			}

			dbSaved_SumDPeakY	= dbSumDPeakY;
			lSaved_PeakWidth3	= lCount_PeakWidth3;
		}
		else
		{
			if( dbSaved_SumDPeakY < 3.1 )
			{
				// ˵����ǰ���������죬Ӧ��С����
			    if( lBCStep <= 5 )//10 )
				{
					// ��������Ѿ�Ϊ10���£�˵��������Ч��ʧ���˳�
					nRet = AutoBC_Analyze_Failure;
				}
				else
				{
					// �ص���һ��Cֵ����С���࣬�����µ�Cֵ
					lContrast	= lSaved_Contrast -lBCStep;
					lBCStep		= 5;	//10;
					lContrast	= lSaved_Contrast +lBCStep;	//�ص���һ��ֵ��+10
				}
			}
			else
			{
				// �ϴε�BCֵ�������BCֵ���ɹ��˳�
				nRet = AutoBC_Analyze_Completed;
			}
		}
	}

	if( lAutoBC_Loop > 99 )		// ��������100�Σ�ʧ���˳�
		nRet = AutoBC_Analyze_Failure;

	lSaved_Brightness	= lBrightness;
	lSaved_Contrast		= lContrast;
	lAutoBC_Loop++;

	return nRet;
}
// 11.01.28 �����Զ����ȶԱȶȹ��ܣ�ZYX�޸ģ�MJY����
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
