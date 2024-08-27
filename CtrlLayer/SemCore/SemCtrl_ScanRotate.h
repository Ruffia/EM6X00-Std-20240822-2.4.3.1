// SemCtrl_ScanRotate.h: interface for the CSemCtrl_ScanRotate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_SCANROTATE_H__3386497A_3F3A_41F3_BB36_6B8F35175612__INCLUDED_)
#define AFX_SEMCTRL_SCANROTATE_H__3386497A_3F3A_41F3_BB36_6B8F35175612__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"
#include "SemCtrl_ObjectiveLens.h"

class AFX_EXT_CLASS CSemCtrl_ScanRotate : public CSemBasisCtrl  
{
	CSemCtrl_ObjectiveLens*	m_pOL;

public:
	CSemCtrl_ScanRotate();
	virtual ~CSemCtrl_ScanRotate();

	BOOL	Bind( CSemCtrl_ObjectiveLens*	pOL );

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	virtual void	SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual void	GetParams( int nType, double& dParam );
	BOOL	m_bON;
	double	m_dAngleInit, m_dAngleComp;
	
	friend class CSemCtrl_Magnifier;
};

#endif // !defined(AFX_SEMCTRL_SCANROTATE_H__3386497A_3F3A_41F3_BB36_6B8F35175612__INCLUDED_)
