// SemCtrl_StigmatorX.h: interface for the CSemCtrl_StigmatorX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_STIGMATORX_H__5ECCE555_C5E2_4CF4_BCDC_8292B6C2D5FD__INCLUDED_)
#define AFX_SEMCTRL_STIGMATORX_H__5ECCE555_C5E2_4CF4_BCDC_8292B6C2D5FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_StigmatorX : public CSemBasisCtrl  
{
public:
	CSemCtrl_StigmatorX();
	virtual ~CSemCtrl_StigmatorX();

	virtual BOOL Flush();
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};

#endif // !defined(AFX_SEMCTRL_STIGMATORX_H__5ECCE555_C5E2_4CF4_BCDC_8292B6C2D5FD__INCLUDED_)
