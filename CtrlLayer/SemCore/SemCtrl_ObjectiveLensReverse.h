// SemCtrl_ObjectiveLensReverse.h: interface for the CSemCtrl_ObjectiveLensReverse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_OBJECTIVELENSREVERSE_H__5D2C03CF_E65C_43F2_B794_3ABEBE54BFDD__INCLUDED_)
#define AFX_SEMCTRL_OBJECTIVELENSREVERSE_H__5D2C03CF_E65C_43F2_B794_3ABEBE54BFDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_ObjectiveLensReverse : public CSemBasisCtrl    
{
public:
	CSemCtrl_ObjectiveLensReverse();
	virtual ~CSemCtrl_ObjectiveLensReverse();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_OBJECTIVELENSREVERSE_H__5D2C03CF_E65C_43F2_B794_3ABEBE54BFDD__INCLUDED_)
