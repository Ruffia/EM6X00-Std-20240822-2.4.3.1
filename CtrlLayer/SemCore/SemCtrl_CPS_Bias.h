// SemCtrl_CPS_Bias.h: interface for the CSemCtrl_CPS_Bias class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CPS_BIAS_H__65A5A420_C843_4133_AAD3_69A93442CAC0__INCLUDED_)
#define AFX_SEMCTRL_CPS_BIAS_H__65A5A420_C843_4133_AAD3_69A93442CAC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_CPS_Bias : public CSemBasisCtrl
{
	double	m_dVScale_ABS;
	double	m_dVScale_Set;
	double	m_dVOffset_Set;
	double	m_dVscale_Set_r;

	double	m_dVScale;
	double	m_dVOffset;
	double	m_dIScale;
	double	m_dIOffset;

	long	m_lUserMode;
	long	m_lEnable;

	long	m_lOldCtlIndex;

public:
	CSemCtrl_CPS_Bias();
	virtual ~CSemCtrl_CPS_Bias();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	virtual long  GetPosByValue(const double dValue);
};

#endif // !defined(AFX_SEMCTRL_CPS_BIAS_H__65A5A420_C843_4133_AAD3_69A93442CAC0__INCLUDED_)
