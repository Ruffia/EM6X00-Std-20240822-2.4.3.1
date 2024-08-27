// SemCtrl_RotationScan.h: interface for the CSemCtrl_RotationScan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_ROTATIONSCAN_H__08081AB6_3C05_4FE5_982D_EEFC7E61D256__INCLUDED_)
#define AFX_SEMCTRL_ROTATIONSCAN_H__08081AB6_3C05_4FE5_982D_EEFC7E61D256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_RotationScan : public CSemBasisCtrl  
{
public:
	CSemCtrl_RotationScan();
	virtual ~CSemCtrl_RotationScan();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};

#endif // !defined(AFX_SEMCTRL_ROTATIONSCAN_H__08081AB6_3C05_4FE5_982D_EEFC7E61D256__INCLUDED_)
