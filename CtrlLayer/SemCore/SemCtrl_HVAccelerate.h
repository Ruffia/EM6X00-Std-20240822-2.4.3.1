// SemCtrl_HVAccelerate.h: interface for the CSemCtrl_HVAccelerate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_HVACCELERATE_H__6CD253DF_C13B_4953_B365_35F62628D3A5__INCLUDED_)
#define AFX_SEMCTRL_HVACCELERATE_H__6CD253DF_C13B_4953_B365_35F62628D3A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_HVAccelerate : public CSemBasisCtrl  
{
public:
	CSemCtrl_HVAccelerate();
	virtual ~CSemCtrl_HVAccelerate();

	virtual const CString& FormatText();
};

#endif // !defined(AFX_SEMCTRL_HVACCELERATE_H__6CD253DF_C13B_4953_B365_35F62628D3A5__INCLUDED_)
