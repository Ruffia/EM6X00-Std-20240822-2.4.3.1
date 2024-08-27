// SemCtrl_ScanShiftY.h: interface for the CSemCtrl_ScanShiftY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_SCANSHIFTY_H__016E26EE_01F7_458E_90ED_C8D93BBD77AB__INCLUDED_)
#define AFX_SEMCTRL_SCANSHIFTY_H__016E26EE_01F7_458E_90ED_C8D93BBD77AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_ScanShiftY : public CSemBasisCtrl  
{
public:
	CSemCtrl_ScanShiftY();
	virtual ~CSemCtrl_ScanShiftY();

	virtual BOOL Flush();
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

	friend class CSemCtrl_Magnifier;
};

#endif // !defined(AFX_SEMCTRL_SCANSHIFTY_H__016E26EE_01F7_458E_90ED_C8D93BBD77AB__INCLUDED_)
