#include "stdafx.h"
#include "ScaleManager.h"

static const double dScaleScanTable[nScaleCount] = {0.1, 0.2, 0.25, 0.50, 0.75, 1.0, 1.5, 2, 2.5, 3, 3.5, 4};

CScaleManager& CScaleManager::Instance()
{
	static CScaleManager Inst;
	return Inst;
}


CScaleManager::CScaleManager()
{
	for (int i = 0; i < nScaleCount; i++)
	{
		g_dScaleScanTable[i] = dScaleScanTable[i];
	}
	g_nScaleScanIndexMax		= sizeof(g_dScaleScanTable) / sizeof(double) - 1;
	g_nScaleScanIndexMin		= 0;
	g_nScaleScanIndexFit		= 5;
	g_nZoomNormal				= 5;

	g_nScaleScanIndex			= g_nZoomNormal;
}


double CScaleManager::GetCurScale()
{
	double dScale = g_dScaleScanTable[g_nScaleScanIndex];
	return dScale;
}


void CScaleManager::UpdateScale(double dScale, BOOL bZoomFit) 
{
	double	dScaleScanTable[]		= { 0.1, 0.2, 0.25, 0.50, 0.75, 1.0, 1.5, 2, 2.5, 3, 3.5, 4 };
	int		nScaleScanIndexMax		= sizeof(g_dScaleScanTable) / sizeof(double) - 1;
	int		nScaleScanIndexMin		= 0;
	g_nScaleScanIndexFit			= 0;
	if( dScale < 1.0 )
	{
		int i = 0;
		for(i=nScaleScanIndexMin; i<=nScaleScanIndexMax; i++ )
		{
			if( (dScale < dScaleScanTable[i]) && (g_nScaleScanIndexFit == 0) )
			{
				g_dScaleScanTable[i+1] = dScaleScanTable[i];
				g_dScaleScanTable[i] = dScale;
				g_nScaleScanIndexFit = i;
			}
			else if( (dScaleScanTable[i] <= 1.0) && (g_nScaleScanIndexFit == 0) )
				g_dScaleScanTable[i] = dScaleScanTable[i];
			else if( (dScaleScanTable[i] <= 1.0) && (g_nScaleScanIndexFit != 0) )
				g_dScaleScanTable[i+1] = dScaleScanTable[i];
			else
				break;
		}
		g_nScaleScanIndexMax = i;
	}
	else
	{
		int i = 0;
		for(i=nScaleScanIndexMin; i<=nScaleScanIndexMax; i++ )
		{
			if( dScale < dScaleScanTable[i] )
			{
				g_dScaleScanTable[i] = dScale;
				g_nScaleScanIndexMax = i;
				g_nScaleScanIndexFit = i;
				break;
			}
		}
		if( i == nScaleScanIndexMax +1 )
		{
			g_nScaleScanIndexMax = i-1;
			g_nScaleScanIndexFit = i-1;
		}
	}

	if( bZoomFit )
		g_nScaleScanIndex = g_nScaleScanIndexFit;
	//	else
	//	g_nScaleScanIndex = g_nScaleScanIndexMax;
}


void CScaleManager::OnViewZoomIn(BOOL& bZoomFit)
{
	if (++g_nScaleScanIndex >= g_nScaleScanIndexMax )
		g_nScaleScanIndex = g_nScaleScanIndexMax;

	if( g_nScaleScanIndex == g_nScaleScanIndexFit )
		bZoomFit = TRUE;
	else
		bZoomFit = FALSE;
}


void CScaleManager::OnViewZoomOut(BOOL& bZoomFit) 
{
	if (--g_nScaleScanIndex < g_nScaleScanIndexMin )
		g_nScaleScanIndex = g_nScaleScanIndexMin;

	if( g_nScaleScanIndex == g_nScaleScanIndexFit )
		bZoomFit = TRUE;
	else
		bZoomFit = FALSE;
}


BOOL  CScaleManager::ISMinIndex()
{
	return g_nScaleScanIndex == g_nScaleScanIndexMin;
}


BOOL  CScaleManager::ISMaxIndex()
{
	return g_nScaleScanIndex == g_nScaleScanIndexMax;
}


void  CScaleManager::OnViewActualSize(BOOL& bZoomFit)
{
	for( int i=g_nScaleScanIndexMin; i<=g_nScaleScanIndexMax; i++ )
	{
		if( g_dScaleScanTable[i] == 1.0 )
		{
			g_nScaleScanIndex	= i;
			g_nZoomNormal		= i;
			break;
		}
	}

	if( g_nScaleScanIndex == g_nScaleScanIndexFit )
		bZoomFit = TRUE;
	else
		bZoomFit = FALSE;
}


void  CScaleManager::OnViewZoomFit()
{
	g_nScaleScanIndex = g_nScaleScanIndexFit;
}


void CScaleManager::ScreenZoom(const double dScale)
{
	for (int i=g_nScaleScanIndexMin; i<=g_nScaleScanIndexMax; i++)
	{
		if ( dScale <= g_dScaleScanTable[i] )
		{
			g_nScaleScanIndex = i;		
			break;
		}
	}
}