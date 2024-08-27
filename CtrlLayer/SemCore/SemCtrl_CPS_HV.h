// SemCtrl_CPS_HV.h: interface for the CSemCtrl_CPS_HV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CPS_HV_H__68647A29_644A_4E0B_9D68_7BE483E5A139__INCLUDED_)
#define AFX_SEMCTRL_CPS_HV_H__68647A29_644A_4E0B_9D68_7BE483E5A139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"
#include "SemCtrl_HVAccelerate.h"
#include "SemCtrl_CondenserLens.h"
#include "SemCtrl_ObjectiveLens.h"

//#include "ScanControlStructure.h"

//class CSemCtrl_CondenserLens;
//class CSemCtrl_ObjectiveLens;

class AFX_EXT_CLASS CSemCtrl_CPS_HV : public CSemBasisCtrl
{
// 2005-2-2_Í³µ÷
//	const static WORD	m_wHVCtlData[CtrlDataSize_HV];
//	const static WORD	m_wHVAdjustData[CtrlDataSize_HV];

	CSemCtrl_HVAccelerate*	m_pHVAccelerate;
	BOOL					m_bReady;

	CSemCtrl_CondenserLens*	m_pCL;
	CSemCtrl_ObjectiveLens*	m_pOL;

	/////////////////////////////////////////////////////////////////
	double	m_dVScale_ABS;
	double	m_dVScale_Set;
	double	m_dVOffset_Set;

	double	m_dVScale;
	double	m_dVOffset;
	double	m_dIScale;
	double	m_dIOffset;
	/////////////////////////////////////////////////////////////////

	long	m_lUserMode;
	long	m_lEnable;

	long	m_lOldCtlIndex;

	CRect	m_rcDestFrame;

public:
	CSemCtrl_CPS_HV();
	virtual ~CSemCtrl_CPS_HV();

	BOOL	Bind( CSemCtrl_HVAccelerate*	pHVAccelerate );
	BOOL	Bind( CSemCtrl_CondenserLens*	pCL );
	BOOL	Bind( CSemCtrl_ObjectiveLens*	pOL );
	virtual	VARIANT SetPos( VARIANT varPos );

	virtual	BOOL Flush();
	virtual	const CString& FormatText();
	virtual	const CString& FormatText( VARIANT varPos );
};

#endif // !defined(AFX_SEMCTRL_CPS_HV_H__68647A29_644A_4E0B_9D68_7BE483E5A139__INCLUDED_)
