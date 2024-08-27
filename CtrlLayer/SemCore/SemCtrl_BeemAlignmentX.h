// SemCtrl_BeemAlignmentX.h: interface for the CSemCtrl_BeemAlignmentX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_BEEMALIGNMENTX_H__424C73F2_48F8_4ED0_9530_532D5B57375B__INCLUDED_)
#define AFX_SEMCTRL_BEEMALIGNMENTX_H__424C73F2_48F8_4ED0_9530_532D5B57375B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_BeemAlignmentX : public CSemBasisCtrl  
{
public:
	CSemCtrl_BeemAlignmentX();
	virtual ~CSemCtrl_BeemAlignmentX();

	virtual BOOL Flush();
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_BEEMALIGNMENTX_H__424C73F2_48F8_4ED0_9530_532D5B57375B__INCLUDED_)
