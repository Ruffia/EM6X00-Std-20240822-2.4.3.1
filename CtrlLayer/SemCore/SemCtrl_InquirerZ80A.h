// SemCtrl_InquirerZ80A.h: interface for the CSemCtrl_InquirerZ80A class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_INQUIRERZ80A_H__90FCDAF5_354D_4046_B1A0_942F329664DB__INCLUDED_)
#define AFX_SEMCTRL_INQUIRERZ80A_H__90FCDAF5_354D_4046_B1A0_942F329664DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_InquirerZ80A : public CSemBasisCtrl  
{
public:
	CSemCtrl_InquirerZ80A();
	virtual ~CSemCtrl_InquirerZ80A();

	virtual BOOL Flush();
};

#endif // !defined(AFX_SEMCTRL_INQUIRERZ80A_H__90FCDAF5_354D_4046_B1A0_942F329664DB__INCLUDED_)
