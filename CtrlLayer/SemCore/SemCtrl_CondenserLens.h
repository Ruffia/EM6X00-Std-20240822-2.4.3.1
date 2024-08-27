// SemCtrl_CondenserLens.h: interface for the CSemCtrl_CondenserLens class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CONDENSERLENS_H__CD95C251_3376_4EC1_AD3F_ECC32011DA40__INCLUDED_)
#define AFX_SEMCTRL_CONDENSERLENS_H__CD95C251_3376_4EC1_AD3F_ECC32011DA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_CondenserLens : public CSemBasisCtrl  
{
	double			m_dRateC;	// 统调比例系数
	BOOL			m_bReverse;

public:
	CSemCtrl_CondenserLens();
	virtual ~CSemCtrl_CondenserLens();

	virtual BOOL Flush();
	virtual void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	virtual long GetPosByValue(const double dValue);

	friend class CSemCtrl_CPS_HV;
};

#endif // !defined(AFX_SEMCTRL_CONDENSERLENS_H__CD95C251_3376_4EC1_AD3F_ECC32011DA40__INCLUDED_)
