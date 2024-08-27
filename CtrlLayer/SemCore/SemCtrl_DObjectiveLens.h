// SemCtrl_DObjectiveLens.h: interface for the CSemCtrl_DObjectiveLens class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_DOBJECTIVELENS_H__618FB764_F5AA_4489_8E01_4474AB04EA0C__INCLUDED_)
#define AFX_SEMCTRL_DOBJECTIVELENS_H__618FB764_F5AA_4489_8E01_4474AB04EA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_DObjectiveLens : public CSemBasisCtrl  
{
	long	m_lSwitch;
	CString	m_strSwitchText;

public:
	CSemCtrl_DObjectiveLens();
	virtual ~CSemCtrl_DObjectiveLens();

	VARIANT GetSwitch( )
		{ 	VARIANT varSwitch;
			varSwitch.lVal = m_lSwitch;
			return varSwitch;	};
	void SetSwitch( VARIANT varSwitch )
		{ m_lSwitch = varSwitch.lVal; };
	const CString& FormatSwitchText();
	const CString& FormatSwitchText( VARIANT varSwitch );

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_DOBJECTIVELENS_H__618FB764_F5AA_4489_8E01_4474AB04EA0C__INCLUDED_)
