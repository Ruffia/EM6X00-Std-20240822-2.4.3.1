// SemCtrl_SACP.h: interface for the CSemCtrl_SACP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_SACP_H__EDB93797_5F26_4663_ADA6_94000F1C1CEA__INCLUDED_)
#define AFX_SEMCTRL_SACP_H__EDB93797_5F26_4663_ADA6_94000F1C1CEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_SACP : public CSemBasisCtrl  
{
public:
	CSemCtrl_SACP();
	virtual ~CSemCtrl_SACP();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_SACP_H__EDB93797_5F26_4663_ADA6_94000F1C1CEA__INCLUDED_)
