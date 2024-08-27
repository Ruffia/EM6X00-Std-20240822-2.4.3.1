#if !defined(AFX_SEMCTRL_SP_BIAS_H__B3BF5F83_E1EB_4D72_99E9_C6D7AC7C8E2E__INCLUDED_)
#define AFX_SEMCTRL_SP_BIAS_H__B3BF5F83_E1EB_4D72_99E9_C6D7AC7C8E2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemCtrl_SP_Bias.h : header file
//

#include "SemBasisCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_SP_Bias window

class AFX_EXT_CLASS CSemCtrl_SP_Bias : public CSemBasisCtrl
{
public:
	CSemCtrl_SP_Bias();
	virtual ~CSemCtrl_SP_Bias();
	
	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	long   GetPosByValue(const double dValue);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCTRL_SP_BIAS_H__B3BF5F83_E1EB_4D72_99E9_C6D7AC7C8E2E__INCLUDED_)
