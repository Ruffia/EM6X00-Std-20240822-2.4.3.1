// SemCtrl_StigmatorY.h: interface for the CSemCtrl_StigmatorY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_STIGMATORY_H__4AB83710_34B7_4CEE_A1F0_720A757ADB02__INCLUDED_)
#define AFX_SEMCTRL_STIGMATORY_H__4AB83710_34B7_4CEE_A1F0_720A757ADB02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_StigmatorY : public CSemBasisCtrl  
{
public:
	CSemCtrl_StigmatorY();
	virtual ~CSemCtrl_StigmatorY();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
};

#endif // !defined(AFX_SEMCTRL_STIGMATORY_H__4AB83710_34B7_4CEE_A1F0_720A757ADB02__INCLUDED_)
