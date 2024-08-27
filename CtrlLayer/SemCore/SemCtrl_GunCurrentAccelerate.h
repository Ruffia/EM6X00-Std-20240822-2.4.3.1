// SemCtrl_GunCurrentAccelerate.h: interface for the CSemCtrl_GunCurrentAccelerate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_GUNCURRENTACCELERATE_H__E9E3C127_2B48_4A7C_A287_FA668C3722D0__INCLUDED_)
#define AFX_SEMCTRL_GUNCURRENTACCELERATE_H__E9E3C127_2B48_4A7C_A287_FA668C3722D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_GunCurrentAccelerate : public CSemBasisCtrl  
{
public:
	CSemCtrl_GunCurrentAccelerate();
	virtual ~CSemCtrl_GunCurrentAccelerate();

	virtual const CString& FormatText();
};

#endif // !defined(AFX_SEMCTRL_GUNCURRENTACCELERATE_H__E9E3C127_2B48_4A7C_A287_FA668C3722D0__INCLUDED_)
