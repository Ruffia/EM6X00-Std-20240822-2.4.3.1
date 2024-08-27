#if !defined(AFX_SEMCTRL_SP_PMT_H__914D3DA6_34EB_478F_86E7_637D91D4DE91__INCLUDED_)
#define AFX_SEMCTRL_SP_PMT_H__914D3DA6_34EB_478F_86E7_637D91D4DE91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemCtrl_SP_PMT.h : header file
//

#include "SemBasisCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_SP_PMT window

class AFX_EXT_CLASS CSemCtrl_SP_PMT : public CSemBasisCtrl
{
public:
	CSemCtrl_SP_PMT();
	virtual ~CSemCtrl_SP_PMT();

	virtual BOOL Flush();
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	virtual long    GetPosByValue(const double dValue);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCTRL_SP_PMT_H__914D3DA6_34EB_478F_86E7_637D91D4DE91__INCLUDED_)
