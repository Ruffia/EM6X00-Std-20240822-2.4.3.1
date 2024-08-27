// SemCtrl_Signal2Contrast.h: interface for the CSemCtrl_Signal2Contrast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_SIGNAL2CONTRAST_H__F98C48A9_23FB_4F28_87A7_8390F5D106AF__INCLUDED_)
#define AFX_SEMCTRL_SIGNAL2CONTRAST_H__F98C48A9_23FB_4F28_87A7_8390F5D106AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_Signal2Contrast : public CSemBasisCtrl  
{
public:
	CSemCtrl_Signal2Contrast();
	virtual ~CSemCtrl_Signal2Contrast();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_SIGNAL2CONTRAST_H__F98C48A9_23FB_4F28_87A7_8390F5D106AF__INCLUDED_)
