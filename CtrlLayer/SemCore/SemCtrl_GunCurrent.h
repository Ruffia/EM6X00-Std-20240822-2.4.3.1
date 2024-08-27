// SemCtrl_GunCurrent.h: interface for the CSemCtrl_GunCurrent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_GUNCURRENT_H__FED4E284_A903_4427_A18F_1EDDA9EFD3B9__INCLUDED_)
#define AFX_SEMCTRL_GUNCURRENT_H__FED4E284_A903_4427_A18F_1EDDA9EFD3B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"
#include "SemCtrl_GunCurrentAccelerate.h"

class AFX_EXT_CLASS CSemCtrl_GunCurrent : public CSemBasisCtrl  
{
	CSemCtrl_GunCurrentAccelerate*	m_pGunCurrentAccelerate;
	BOOL					m_bReady;

public:
	CSemCtrl_GunCurrent();
	virtual ~CSemCtrl_GunCurrent();

	BOOL Bind( CSemCtrl_GunCurrentAccelerate*	pGunCurrentAccelerate );
	virtual VARIANT SetPos( VARIANT varPos );

	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_GUNCURRENT_H__FED4E284_A903_4427_A18F_1EDDA9EFD3B9__INCLUDED_)
