// SemCtrl_BeemAlignmentY.h: interface for the CSemCtrl_BeemAlignmentY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_BEEMALIGNMENTY_H__FB72541B_187E_43A8_BAF7_7F43B029A7AA__INCLUDED_)
#define AFX_SEMCTRL_BEEMALIGNMENTY_H__FB72541B_187E_43A8_BAF7_7F43B029A7AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_BeemAlignmentY : public CSemBasisCtrl  
{
public:
	CSemCtrl_BeemAlignmentY();
	virtual ~CSemCtrl_BeemAlignmentY();

	virtual BOOL Flush();
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_BEEMALIGNMENTY_H__FB72541B_187E_43A8_BAF7_7F43B029A7AA__INCLUDED_)
