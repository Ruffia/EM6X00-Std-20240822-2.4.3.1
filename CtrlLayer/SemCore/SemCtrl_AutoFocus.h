// SemCtrl_AutoFocus.h: interface for the CSemCtrl_AutoFocus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_AUTOFOCUS_H__DC008D23_77A5_4F28_9F09_0EB164A70FC7__INCLUDED_)
#define AFX_SEMCTRL_AUTOFOCUS_H__DC008D23_77A5_4F28_9F09_0EB164A70FC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class CSemCtrl_ObjectiveLens;

class AFX_EXT_CLASS CSemCtrl_AutoFocus : public CSemBasisCtrl  
{
	CSemCtrl_ObjectiveLens*		m_pFinalLens;

public:
	CSemCtrl_AutoFocus();
	virtual ~CSemCtrl_AutoFocus();

	BOOL Bind( CSemCtrl_ObjectiveLens*	pFinalLens );

	virtual BOOL Flush();
};

#endif // !defined(AFX_SEMCTRL_AUTOFOCUS_H__DC008D23_77A5_4F28_9F09_0EB164A70FC7__INCLUDED_)
