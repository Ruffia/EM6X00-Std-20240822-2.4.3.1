// SemCtrl_Channel2D.h: interface for the CSemCtrl_Channel2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CHANNEL2D_H__94CC3FE4_C49C_487F_9F11_92E476077FA3__INCLUDED_)
#define AFX_SEMCTRL_CHANNEL2D_H__94CC3FE4_C49C_487F_9F11_92E476077FA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_Channel2D : public CSemBasisCtrl
{
public:
	CSemCtrl_Channel2D();
	virtual ~CSemCtrl_Channel2D();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_CHANNEL2D_H__94CC3FE4_C49C_487F_9F11_92E476077FA3__INCLUDED_)
