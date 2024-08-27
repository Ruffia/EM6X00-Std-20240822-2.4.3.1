// SemCtrl_Signal2Brightness.h: interface for the CSemCtrl_Signal2Brightness class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_SIGNAL2BRIGHTNESS_H__9672DEC5_A749_4F3D_BEF1_E349D9F8C02E__INCLUDED_)
#define AFX_SEMCTRL_SIGNAL2BRIGHTNESS_H__9672DEC5_A749_4F3D_BEF1_E349D9F8C02E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_Signal2Brightness : public CSemBasisCtrl  
{
public:
	CSemCtrl_Signal2Brightness();
	virtual ~CSemCtrl_Signal2Brightness();

	int	m_nDetPair;
	virtual BOOL Flush();
	virtual void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_SIGNAL2BRIGHTNESS_H__9672DEC5_A749_4F3D_BEF1_E349D9F8C02E__INCLUDED_)
