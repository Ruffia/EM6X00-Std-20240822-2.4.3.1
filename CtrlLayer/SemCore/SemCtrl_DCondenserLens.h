// SemCtrl_DCondenserLens.h: interface for the CSemCtrl_DCondenserLens class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_DCONDENSERLENS_H__8F34D53A_2211_41C0_A599_17423BB2FA99__INCLUDED_)
#define AFX_SEMCTRL_DCONDENSERLENS_H__8F34D53A_2211_41C0_A599_17423BB2FA99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_DCondenserLens : public CSemBasisCtrl  
{
public:
	CSemCtrl_DCondenserLens();
	virtual ~CSemCtrl_DCondenserLens();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_DCONDENSERLENS_H__8F34D53A_2211_41C0_A599_17423BB2FA99__INCLUDED_)
