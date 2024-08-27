#if !defined(AFX_SEMCTRL_SP_HV_H__7226FE2E_32CC_4BE6_9C8C_71BDA3CFF9E8__INCLUDED_)
#define AFX_SEMCTRL_SP_HV_H__7226FE2E_32CC_4BE6_9C8C_71BDA3CFF9E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemCtrl_SP_HV.h : header file
//

#include "SemBasisCtrl.h"
#include "SemCtrl_CondenserLens.h"
#include "SemCtrl_ObjectiveLens.h"
#include "SemCtrl_HVAccelerate.h"

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_SP_HV window

class AFX_EXT_CLASS CSemCtrl_SP_HV : public CSemBasisCtrl
{
	CSemCtrl_HVAccelerate*	m_pHVAccelerate;
	BOOL					m_bReady;

	CSemCtrl_CondenserLens*	m_pCL;
	CSemCtrl_ObjectiveLens*	m_pOL;

public:
	CSemCtrl_SP_HV();
	virtual ~CSemCtrl_SP_HV();

	BOOL	Bind( CSemCtrl_HVAccelerate*	pHVAccelerate );
	BOOL	Bind( CSemCtrl_CondenserLens*	pCL );
	BOOL	Bind( CSemCtrl_ObjectiveLens*	pOL );
	virtual	VARIANT SetPos( VARIANT varPos );

	virtual	BOOL Flush();
	virtual	const CString& FormatText();
	virtual	const CString& FormatText( VARIANT varPos );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCTRL_SP_HV_H__7226FE2E_32CC_4BE6_9C8C_71BDA3CFF9E8__INCLUDED_)
