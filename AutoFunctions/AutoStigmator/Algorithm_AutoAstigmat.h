#if !defined(Algorithm_AUTO_Astigmat__INCLUDED_)
#define Algorithm_AUTO_Astigmat__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Algorithm_AutoAstigmat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Algorithm_AutoAstigmat

const UINT AutoAstigmat_Analyze_Continue		= 0;
const UINT AutoAstigmat_Analyze_Completed		= 1;
const UINT AutoAstigmat_Analyze_Failure			= 2;

AFX_EXT_API void WINAPI AutoAstigmat_Initialize(
	long& 		lFinalLens,
	double&		dblMagnifier,
	long&		lStigmatorX,
	long&		lStigmatorY );

AFX_EXT_API UINT WINAPI AutoAstigmat_Analyze_Image( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness,
	long& 		lFinalLens,
	double&		dblMagnifier,
	long&		lStigmatorX,
	long&		lStigmatorY);

#endif // Algorithm_AUTO_Astigmat__INCLUDED_
