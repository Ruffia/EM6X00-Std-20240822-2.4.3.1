// SemCtrl_InquirerZ80B.h: interface for the CSemCtrl_InquirerZ80B class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_INQUIRERZ80B_H__0AD18B3E_4638_4BD2_8CB0_F7B0689D8723__INCLUDED_)
#define AFX_SEMCTRL_INQUIRERZ80B_H__0AD18B3E_4638_4BD2_8CB0_F7B0689D8723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_InquirerZ80B : public CSemBasisCtrl  
{
public:
	CSemCtrl_InquirerZ80B();
	virtual ~CSemCtrl_InquirerZ80B();

	virtual BOOL Flush();
};

#endif // !defined(AFX_SEMCTRL_INQUIRERZ80B_H__0AD18B3E_4638_4BD2_8CB0_F7B0689D8723__INCLUDED_)
