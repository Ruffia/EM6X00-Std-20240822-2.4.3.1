// Last Modify : 2014.05.

#if !defined(Algorithm_AUTO_FILAMENT__INCLUDED_)
#define Algorithm_AUTO_FILAMENT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Algorithm_AutoFilament.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Algorithm_AutoFilament
typedef struct tagFilamentParam
{
	int	nPeakX;
	int	nPeakY;
	int	nPeakWidthCount;
	int	nFila;
}ALParam;

const UINT AutoFila_Analyze_Continue	= 0;
const UINT AutoFila_Analyze_Completed	= 1;
const UINT AutoFila_Analyze_Failure		= 2;

const double dblAutoFila_PeakMinParam	= 0.004;

AFX_EXT_API void WINAPI AutoFila_Initialize(
	IN	int		nRangeMin,
	IN	int		nRangeMax,
	IN	int		nCorseStep,
	IN	int		nDebug,
	OUT	long&	lFila );

AFX_EXT_API UINT WINAPI AutoFila_Analyze_GrayHistogram( 
	IN	BOOL	bPause,
	IN	DWORD	dwTotalPixel, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	lFila );

#endif // Algorithm_AUTO_FILAMENT__INCLUDED_
