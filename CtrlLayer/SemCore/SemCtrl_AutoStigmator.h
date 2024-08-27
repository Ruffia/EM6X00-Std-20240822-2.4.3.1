// SemCtrl_AutoStigmator.h: interface for the CSemCtrl_AutoStigmator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_AUTOSTIGMATOR_H__12114D16_89EC_42F4_912D_05412AA53F36__INCLUDED_)
#define AFX_SEMCTRL_AUTOSTIGMATOR_H__12114D16_89EC_42F4_912D_05412AA53F36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class CSemCtrl_StigmatorX;
class CSemCtrl_StigmatorY;

class AFX_EXT_CLASS CSemCtrl_AutoStigmator : public CSemBasisCtrl  
{
	CSemCtrl_StigmatorX		*m_pStigmatorX;
	CSemCtrl_StigmatorY		*m_pStigmatorY;
	BOOL					m_bReady;

public:
	CSemCtrl_AutoStigmator();
	virtual ~CSemCtrl_AutoStigmator();

	BOOL Bind( CSemCtrl_StigmatorX*	pStigmatorX );
	BOOL Bind( CSemCtrl_StigmatorY*	pStigmatorY );

	virtual BOOL Flush();
};

#endif // !defined(AFX_SEMCTRL_AUTOSTIGMATOR_H__12114D16_89EC_42F4_912D_05412AA53F36__INCLUDED_)
