#pragma once
#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_AdjustParameter : public CSemBasisCtrl  
{
public:
	CSemCtrl_AdjustParameter();
	virtual ~CSemCtrl_AdjustParameter();

	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};
