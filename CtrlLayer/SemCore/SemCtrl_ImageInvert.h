// SemCtrl_ImageInvert.h: interface for the CSemCtrl_ImageInvert class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_IMAGEINVERT_H__4C5E3A61_1D31_42CF_BAF5_26D05DF0D585__INCLUDED_)
#define AFX_SEMCTRL_IMAGEINVERT_H__4C5E3A61_1D31_42CF_BAF5_26D05DF0D585__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_ImageInvert : public CSemBasisCtrl  
{
public:
	CSemCtrl_ImageInvert();
	virtual ~CSemCtrl_ImageInvert();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_IMAGEINVERT_H__4C5E3A61_1D31_42CF_BAF5_26D05DF0D585__INCLUDED_)
