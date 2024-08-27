#if !defined(AFX_SEMCTRL_SP_HEATER_H__F2840CA7_579B_4730_A02B_D315705295CC__INCLUDED_)
#define AFX_SEMCTRL_SP_HEATER_H__F2840CA7_579B_4730_A02B_D315705295CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemCtrl_SP_Heater.h : header file
//

#include "SemBasisCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_SP_Heater window

class AFX_EXT_CLASS CSemCtrl_SP_Heater : public CSemBasisCtrl
{
public:
	CSemCtrl_SP_Heater();
	virtual ~CSemCtrl_SP_Heater();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCTRL_SP_HEATER_H__F2840CA7_579B_4730_A02B_D315705295CC__INCLUDED_)
