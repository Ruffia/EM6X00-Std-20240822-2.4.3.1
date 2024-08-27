// SemCtrl_Channel2E.h: interface for the CSemCtrl_Channel2E class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CHANNEL2E_H__D2D4AA9E_B854_4AE9_8954_EC69654B30F7__INCLUDED_)
#define AFX_SEMCTRL_CHANNEL2E_H__D2D4AA9E_B854_4AE9_8954_EC69654B30F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_Channel2E : public CSemBasisCtrl
{
public:
	CSemCtrl_Channel2E();
	virtual ~CSemCtrl_Channel2E();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_CHANNEL2E_H__D2D4AA9E_B854_4AE9_8954_EC69654B30F7__INCLUDED_)
