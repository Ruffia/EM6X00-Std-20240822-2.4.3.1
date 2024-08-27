// SemCtrl_ScanShiftX.h: interface for the CSemCtrl_ScanShiftX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_SCANSHIFTX_H__6384360B_0C0C_416A_9FDF_760A35C17107__INCLUDED_)
#define AFX_SEMCTRL_SCANSHIFTX_H__6384360B_0C0C_416A_9FDF_760A35C17107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_ScanShiftX : public CSemBasisCtrl  
{
public:
	CSemCtrl_ScanShiftX();
	virtual ~CSemCtrl_ScanShiftX();

	virtual BOOL Flush();
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

	friend class CSemCtrl_Magnifier;
};

#endif // !defined(AFX_SEMCTRL_SCANSHIFTX_H__6384360B_0C0C_416A_9FDF_760A35C17107__INCLUDED_)
