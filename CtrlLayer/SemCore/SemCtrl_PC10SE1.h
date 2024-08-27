// SemCtrl_PC10SE1.h: interface for the CSemCtrl_PC10SE1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_PC10SE1_H__C59BEF16_8FFC_496C_A268_F686A4B83645__INCLUDED_)
#define AFX_SEMCTRL_PC10SE1_H__C59BEF16_8FFC_496C_A268_F686A4B83645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_PC10SE1 : public CSemBasisCtrl  
{
	BYTE FormatControlFlag();
public:
	CSemCtrl_PC10SE1();
	virtual ~CSemCtrl_PC10SE1();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};

#endif // !defined(AFX_SEMCTRL_PC10SE1_H__C59BEF16_8FFC_496C_A268_F686A4B83645__INCLUDED_)
