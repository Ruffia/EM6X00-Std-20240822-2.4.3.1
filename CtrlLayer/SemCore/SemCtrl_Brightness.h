// SemCtrl_Brightness.h: interface for the CSemCtrl_Brightness class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_BRIGHTNESS_H__60466999_4118_4766_A599_2CD0F8F20CB3__INCLUDED_)
#define AFX_SEMCTRL_BRIGHTNESS_H__60466999_4118_4766_A599_2CD0F8F20CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_Brightness : public CSemBasisCtrl  
{
public:
	CSemCtrl_Brightness();
	virtual ~CSemCtrl_Brightness();

	int	m_nDetPair;
	int	m_nDet2nd;
	virtual BOOL Flush();
	virtual void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	virtual long GetPosByValue(const double dValue);
};

#endif // !defined(AFX_SEMCTRL_BRIGHTNESS_H__60466999_4118_4766_A599_2CD0F8F20CB3__INCLUDED_)
