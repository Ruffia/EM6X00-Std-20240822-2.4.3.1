// SemCtrl_CondenserLensReverse.h: interface for the CSemCtrl_CondenserLensReverse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CONDENSERLENSREVERSE_H__2EF8B3AA_116A_4EB5_AB5B_2722DDF47759__INCLUDED_)
#define AFX_SEMCTRL_CONDENSERLENSREVERSE_H__2EF8B3AA_116A_4EB5_AB5B_2722DDF47759__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_CondenserLensReverse : public CSemBasisCtrl  
{
public:
	CSemCtrl_CondenserLensReverse();
	virtual ~CSemCtrl_CondenserLensReverse();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_CONDENSERLENSREVERSE_H__2EF8B3AA_116A_4EB5_AB5B_2722DDF47759__INCLUDED_)
