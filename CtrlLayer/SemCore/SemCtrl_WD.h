// SemCtrl_WD.h: interface for the CSemCtrl_WD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_WD_H__610836BD_AE8E_4B01_9A72_4695E2A6E158__INCLUDED_)
#define AFX_SEMCTRL_WD_H__610836BD_AE8E_4B01_9A72_4695E2A6E158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"
#include "SemCtrl_ObjectiveLens.h"

class CSemCtrl_Magnifier;

class AFX_EXT_CLASS CSemCtrl_WD : public CSemBasisCtrl  
{
	// 2005-2-2_统调
	// const static WORD	m_nObjectiveLensToWDIndex[ CtrlDataSize_WorkDistance ];
	// const static WORD	m_nWDIndexToWD[ CtrlDataSize_WorkDistance ];

	VARIANT	SetPos( VARIANT varPos ){ return CSemBasisCtrl::SetPos(varPos); }
	
	BOOL	LookupPos( WORD wObjectiveLensPos );
	BOOL	Co_UpdatePosByM( double dM );
	BOOL	Co_UpdatePosByOL( double dOL );
	double	GetOLValue( double dWD );

	CSemCtrl_Magnifier*		m_pMag;
	BOOL					m_bReady;

	double					m_dCoFactorA;
	double					m_dCoFactorB;
	double					m_dCoFactorC;

	CSemCtrl_ObjectiveLens*	m_pOL;
	double					m_dCurrentHV;	// 当前高压值
	int						m_nCoHVstage;	// 高压统调档位

public:
	CSemCtrl_WD();
	virtual ~CSemCtrl_WD();
	
	BOOL	Bind( CSemCtrl_Magnifier*	pMag );
	BOOL	Bind( CSemCtrl_ObjectiveLens*	pOL );

	virtual	BOOL	CoherenceCheck();

	virtual VARIANT	GetPos();
	virtual	void	SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual void	GetParams( int nType, double& dParam );
	virtual const	CString& FormatText();
	virtual const	CString& FormatText( VARIANT varPos );

	friend class CSemCtrl_ObjectiveLens;
};

#endif // !defined(AFX_SEMCTRL_WD_H__610836BD_AE8E_4B01_9A72_4695E2A6E158__INCLUDED_)
