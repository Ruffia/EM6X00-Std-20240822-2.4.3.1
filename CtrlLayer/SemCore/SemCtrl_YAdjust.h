// SemCtrl_YAdjust.h: interface for the CSemCtrl_YAdjust class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_YADJUST_H__4A185FCC_C67B_499B_87A6_61300B77C46B__INCLUDED_)
#define AFX_SEMCTRL_YADJUST_H__4A185FCC_C67B_499B_87A6_61300B77C46B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_YAdjust : public CSemBasisCtrl  
{
public:
	CSemCtrl_YAdjust();
	virtual ~CSemCtrl_YAdjust();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_YADJUST_H__4A185FCC_C67B_499B_87A6_61300B77C46B__INCLUDED_)
