// SemCtrl_FilamentCurrent.h: interface for the CSemCtrl_FilamentCurrent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_FILAMENTCURRENT_H__1FBF9B8D_96BC_4792_8497_5F17EB6B1A93__INCLUDED_)
#define AFX_SEMCTRL_FILAMENTCURRENT_H__1FBF9B8D_96BC_4792_8497_5F17EB6B1A93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_FilamentCurrent : public CSemBasisCtrl  
{
public:
	CSemCtrl_FilamentCurrent();
	virtual ~CSemCtrl_FilamentCurrent();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_FILAMENTCURRENT_H__1FBF9B8D_96BC_4792_8497_5F17EB6B1A93__INCLUDED_)
