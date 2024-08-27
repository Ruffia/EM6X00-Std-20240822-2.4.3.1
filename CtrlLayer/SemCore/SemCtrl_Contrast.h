// SemCtrl_Contrast.h: interface for the CSemCtrl_Contrast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CONTRAST_H__48854EED_D0AA_4EFA_912C_F6D75FDD6947__INCLUDED_)
#define AFX_SEMCTRL_CONTRAST_H__48854EED_D0AA_4EFA_912C_F6D75FDD6947__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_Contrast : public CSemBasisCtrl  
{
public:
	CSemCtrl_Contrast();
	virtual ~CSemCtrl_Contrast();

	virtual BOOL Flush();
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	virtual long GetPosByValue(const double dValue);
};

#endif // !defined(AFX_SEMCTRL_CONTRAST_H__48854EED_D0AA_4EFA_912C_F6D75FDD6947__INCLUDED_)
