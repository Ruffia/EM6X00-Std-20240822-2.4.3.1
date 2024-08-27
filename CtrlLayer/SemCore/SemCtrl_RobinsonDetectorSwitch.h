// SemCtrl_RobinsonDetectorSwitch.h: interface for the CSemCtrl_RobinsonDetectorSwitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_ROBINSONDETECTORSWITCH_H__91AE59BA_32E2_4732_96ED_B5F8948FB34B__INCLUDED_)
#define AFX_SEMCTRL_ROBINSONDETECTORSWITCH_H__91AE59BA_32E2_4732_96ED_B5F8948FB34B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_RobinsonDetectorSwitch : public CSemBasisCtrl  
{
public:
	CSemCtrl_RobinsonDetectorSwitch();
	virtual ~CSemCtrl_RobinsonDetectorSwitch();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_ROBINSONDETECTORSWITCH_H__91AE59BA_32E2_4732_96ED_B5F8948FB34B__INCLUDED_)
