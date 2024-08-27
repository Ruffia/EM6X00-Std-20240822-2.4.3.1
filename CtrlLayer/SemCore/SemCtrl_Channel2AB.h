// SemCtrl_Channel2AB.h: interface for the CSemCtrl_Channel2AB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CHANNEL2AB_H__82DEA277_C875_40F8_86A6_4D91C2A79949__INCLUDED_)
#define AFX_SEMCTRL_CHANNEL2AB_H__82DEA277_C875_40F8_86A6_4D91C2A79949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_Channel2AB : public CSemBasisCtrl
{
public:
	CSemCtrl_Channel2AB();
	virtual ~CSemCtrl_Channel2AB();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_CHANNEL2AB_H__82DEA277_C875_40F8_86A6_4D91C2A79949__INCLUDED_)
