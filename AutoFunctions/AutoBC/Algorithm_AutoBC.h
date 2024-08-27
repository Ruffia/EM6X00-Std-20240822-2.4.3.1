// Last Modify : 2011.01.28

/////////////////////////////////////////////////////////////////////////////
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理
/////////////////////////////////////////////////////////////////////////////

#if !defined(Algorithm_AUTO_BC__INCLUDED_)
#define Algorithm_AUTO_BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Algorithm_AutoBC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Algorithm_AutoBC

const UINT AutoBC_Analyze_Continue		= 0;
const UINT AutoBC_Analyze_Completed		= 1;
const UINT AutoBC_Analyze_Failure		= 2;

const double dblPeakMinParam			= 0.004;

AFX_EXT_API void WINAPI AutoBC_Initialize(
	OUT	long&	lBrightness, 
	OUT	long&	lContrast );

AFX_EXT_API UINT WINAPI AutoBC_Analyze_GrayHistogram( 
	IN	DWORD	dwTotalPiexl, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	lBrightness, 
	OUT	long&	lContrast );

///////////////////////////////////////////////////////////////////////////////
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理
/*
// 2010.08.06 add by zyx
AFX_EXT_API UINT WINAPI AutoBC_Analyze_Image(
	IN	DWORD	dwTotalPiexl, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	lPeakX, 
	OUT	long&	lPeakX2, 
	OUT	double&	dbSumDPeakY,
	OUT long&	lPeakWidth1,
	OUT long&	lPeakWidth2,
	OUT long&	lPeakWidth3);
// 2010.08.06 add by zyx
*/
AFX_EXT_API void WINAPI AutoBC_Initialize2(
	OUT	long&	lBrightness, 
	OUT	long&	lContrast );

AFX_EXT_API UINT WINAPI AutoBC_Analyze_Image2(
	IN	DWORD	dwTotalPiexl, 
	IN	DWORD	*pGrayHistogramBuff, 
	OUT	long&	lBrightness,
	OUT	long&	lContrast);
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理
///////////////////////////////////////////////////////////////////////////////

AFX_EXT_API void WINAPI AutoBC_Initialize3(
	OUT	long&	lBrightness, 
	OUT	long&	lContrast);

#endif // Algorithm_AUTO_BC__INCLUDED_
