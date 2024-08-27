// SemCtrl_AuxiliaryStigmation.h: interface for the CSemCtrl_AuxiliaryStigmation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_AUXILIARYSTIGMATION_H__FC22EF83_3C0F_4471_8B68_B754D0BA2AA1__INCLUDED_)
#define AFX_SEMCTRL_AUXILIARYSTIGMATION_H__FC22EF83_3C0F_4471_8B68_B754D0BA2AA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_AuxiliaryStigmation : public CSemBasisCtrl  
{
public:
	CSemCtrl_AuxiliaryStigmation();
	virtual ~CSemCtrl_AuxiliaryStigmation();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_AUXILIARYSTIGMATION_H__FC22EF83_3C0F_4471_8B68_B754D0BA2AA1__INCLUDED_)
