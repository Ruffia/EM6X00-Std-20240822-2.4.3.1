// SemCtrl_HV.h: interface for the CSemCtrl_HV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_HV_H__0CD11B27_8489_4754_AF86_39EBCE4A7F30__INCLUDED_)
#define AFX_SEMCTRL_HV_H__0CD11B27_8489_4754_AF86_39EBCE4A7F30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"
#include "SemCtrl_HVAccelerate.h"

#include "SemCtrl_CondenserLens.h"
#include "SemCtrl_ObjectiveLens.h"

class AFX_EXT_CLASS CSemCtrl_HV : public CSemBasisCtrl  
{
// 2005-2-2_Í³µ÷
//	const static WORD	m_wHVCtlData[CtrlDataSize_HV];
//	const static WORD	m_wHVAdjustData[CtrlDataSize_HV];

	CSemCtrl_HVAccelerate*	m_pHVAccelerate;
	BOOL					m_bReady;

	CSemCtrl_CondenserLens*	m_pCL;
	CSemCtrl_ObjectiveLens*	m_pOL;

public:
	CSemCtrl_HV();
	virtual ~CSemCtrl_HV();

	BOOL Bind( CSemCtrl_HVAccelerate*	pHVAccelerate );
	BOOL Bind( CSemCtrl_CondenserLens* pCL );
	BOOL Bind( CSemCtrl_ObjectiveLens* pOL );
	virtual VARIANT SetPos( VARIANT varPos );

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_HV_H__0CD11B27_8489_4754_AF86_39EBCE4A7F30__INCLUDED_)
