#if !defined(AUTO_BC__INCLUDED_)
#define AUTO_BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoBC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AutoBC

const UINT AutoBC_Analyze_Continue		= 0;
const UINT AutoBC_Analyze_Completed		= 1;
const UINT AutoBC_Analyze_Failure		= 2;
const double dblPeakMinParam			= 0.004;

void WINAPI AutoBC_Initialize(
	OUT	long&	lBrightness, 
	OUT	long&	lContrast );

UINT WINAPI AutoBC_Analyze_GrayHistogram( 
	IN	DWORD	dwTotalPiexl, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	lBrightness, 
	OUT	long&	lContrast );


#endif // AUTO_BC__INCLUDED_
