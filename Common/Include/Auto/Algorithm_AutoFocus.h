#if !defined(Algorithm_AUTO_FOCUS__INCLUDED_)
#define Algorithm_AUTO_FOCUS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Algorithm_AutoFocus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Algorithm_AutoFocus

const double OL_TO_LED	= 64.0;

const UINT AutoFocus_Analyze_Continue		= 0;
const UINT AutoFocus_Analyze_Completed		= 1;
const UINT AutoFocus_Analyze_Failure		= 2;

AFX_EXT_API void WINAPI AutoFocus_Initialize(
	long& 		lFieldLens,
	double&		dblMagnifier );

AFX_EXT_API BOOL WINAPI Analyze_Image( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness,
	double&		dfTotal );

BOOL WINAPI Analyze_Image1( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp);
AFX_EXT_API UINT WINAPI AutoFocus_Analyze_Image( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness,
	long& 		lFieldLens,
	double&		dblMagnifier );

///////////////////////////////////////////////////////
// 2009.04.15_自动聚焦算法优化
AFX_EXT_API void WINAPI AutoFocus_Initialize2(
	long& 		lFieldLens,
	double&		dblMagnifier );

AFX_EXT_API UINT WINAPI AutoFocus_Analyze_Image2( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness,
	long& 		lFieldLens,
	double&		dblMagnifier );

AFX_EXT_API UINT WINAPI AutoFocus_Analyze_Image3( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness,
	long& 		lFieldLens,
	double&		dblMagnifier );

// 2009.04.15_自动聚焦算法优化
///////////////////////////////////////////////////////

//2022.12.28 自动聚焦算法优化
AFX_EXT_API void WINAPI AutoFocus_Initialize1(
	long& 		lFieldLens,
	double&		dblMagnifier);

AFX_EXT_API UINT WINAPI AutoFocus_Analyze_Image1( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness,
	long& 		lFieldLens,
	double&		dblMagnifier,
	long&		lFinalLensold);
//2022.12.28 自动聚焦算法优化
///////////////////////////////////////////////////////
/*
double WINAPI CtrlToMag( WORD wCtrl );

WORD WINAPI MagToCtrl( double dfMag );
*/

#endif // Algorithm_AUTO_FOCUS__INCLUDED_
